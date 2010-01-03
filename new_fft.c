/* mul_fft -- split-radix fft routines for MPIR.

Copyright William Hart 2009

This file is part of the MPIR Library.

The MPIR Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The MPIR Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPIR Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA. */

#include <stdio.h>
#include <stdlib.h>
#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"

#define ulong unsigned long

void mpn_to_mpz(mpz_t m, mp_limb_t * i, ulong limbs);
void set_p(mpz_t p, ulong n, ulong w);
void rand_n(mp_limb_t * n, gmp_randstate_t state, ulong limbs);
void ref_mul_2expmod(mpz_t m, mpz_t i2, mpz_t p, ulong n, ulong w, ulong d);
void ref_norm(mpz_t m, mpz_t p);
void ref_sumdiff_rshBmod(mpz_t t, mpz_t u, mpz_t i1, 
                      mpz_t i2, mpz_t p, ulong n, ulong w, ulong x, ulong y);

/*
   FIXME: This can be done faster for the case where c is small.
   The function adds c to the integer r modulo 2^l + 1.
*/
void mpn_addmod_2expp1_1(mp_limb_t * r, ulong l, mp_limb_signed_t c)
{
   if (c >= 0) mpn_add_1(r, r, l + 1, c);
   else mpn_sub_1(r, r, l + 1, -c);
}

/*
   FIXME: this is the most inefficient implementation I could come up with. ;-)
*/
ulong revbin(ulong c, ulong depth)
{
   ulong i = 0;
   ulong ret = 0;
   for (i = 0; i < depth + 1; i++)
      if (c & (1UL<<i)) ret += (1UL<<(depth - i));
   return ret;
}

/* 
   FIXME: the following functions should use mp_limb_t's not ulongs
*/

/*
   Splits an mpn into segments of length coeff_limbs and stores in 
   zero padded coefficients of length output_limbs, for use in FFT 
   convolution code. Assumes that the input is total_limbs in length. 
   The total number of coefficients written is returned.
*/
ulong FFT_split(mp_limb_t ** poly, mp_limb_t * limbs, 
                ulong total_limbs, ulong coeff_limbs, ulong output_limbs)
{
   ulong length = (total_limbs - 1)/coeff_limbs + 1;
   ulong i, j, skip;
   
   for (skip = 0, i = 0; skip + coeff_limbs <= total_limbs; skip += coeff_limbs, i++)
   {
      //if (i + 1 < length)
		   //for (j = 0; j + 8 < output_limbs; j += 8) PREFETCH(poly[i+1], j);
      
      MPN_ZERO(poly[i], output_limbs + 1);
      // convert a coefficient
      MPN_COPY(poly[i], limbs + skip, coeff_limbs);
   }
   if (i < length) MPN_ZERO(poly[i], output_limbs + 1);
   if (total_limbs > skip) MPN_COPY(poly[i], limbs + skip, total_limbs - skip);
   
   return length;
}

/*
   Splits an mpn into segments of length _bits_ and stores in
   zero padded coefficients of length output_limbs, for use in FFT 
   convolution code. Assumes that the input is total_limbs in length. 
   Returns the total number of coefficient written. 
*/

ulong FFT_split_bits(mp_limb_t ** poly, mp_limb_t * limbs, 
               ulong total_limbs, ulong bits, ulong output_limbs)
{
   ulong length = (GMP_LIMB_BITS*total_limbs - 1)/bits + 1;
   ulong i, j;
   
   ulong top_bits = ((GMP_LIMB_BITS - 1) & bits);
   if (top_bits == 0)
   {
      return FFT_split(poly, limbs, total_limbs, bits/GMP_LIMB_BITS, output_limbs);
   }

   ulong coeff_limbs = (bits/GMP_LIMB_BITS) + 1;
   ulong mask = (1L<<top_bits) - 1L;
   ulong shift_bits = 0L;
   ulong * limb_ptr = limbs;                      
    
   for (i = 0; i < length - 1; i++)
   {
      //for (j = 0; j + 8 < output_limbs; j += 8) PREFETCH(poly->coeffs[i+1], j);
      
      MPN_ZERO(poly[i], output_limbs + 1);
      // convert a coefficient
      if (!shift_bits)
      {
         MPN_COPY(poly[i], limb_ptr, coeff_limbs);
         poly[i][coeff_limbs - 1] &= mask;
         limb_ptr += (coeff_limbs - 1);
         shift_bits += top_bits;
      } else
      {
         mpn_rshift(poly[i], limb_ptr, coeff_limbs, shift_bits);
         limb_ptr += (coeff_limbs - 1);
         shift_bits += top_bits;
         if (shift_bits >= GMP_LIMB_BITS)
         {
            limb_ptr++;
            poly[i][coeff_limbs - 1] += (limb_ptr[0] << (GMP_LIMB_BITS - (shift_bits - top_bits)));
            shift_bits -= GMP_LIMB_BITS; 
         }
         poly[i][coeff_limbs - 1] &= mask;
      }                      
   }
   
   MPN_ZERO(poly[i], output_limbs + 1);
   ulong limbs_left = total_limbs - (limb_ptr - limbs);
   if (!shift_bits)
   {
      MPN_COPY(poly[i], limb_ptr, limbs_left);
   } else
   {
      mpn_rshift(poly[i], limb_ptr, limbs_left, shift_bits);
   }                      
      
   return length;
}

/*
   Recombines coefficients after doing a convolution. Assumes each of the 
   coefficients of the poly of the given length is output_limbs long, that each 
   of the coefficients is being shifted by a multiple of coeff_limbs and added
   to an mpn which is total_limbs long. It is assumed that the mpn has been 
   zeroed in advance.
*/

void FFT_combine(mp_limb_t * res, mp_limb_t ** poly, ulong length, ulong coeff_limbs, 
                             ulong output_limbs, ulong total_limbs)
{
   ulong skip, i, j;
   
   for (skip = 0, i = 0; (i < length) && (skip + output_limbs <= total_limbs); i++, skip += coeff_limbs)
   { 
      //for (j = 0; j < output_limbs; j += 8) PREFETCH(poly->coeffs[i+1], j);
      mpn_add(res + skip, res + skip, output_limbs + 1, poly[i], output_limbs);      
   } 

   while ((skip < total_limbs) && (i < length))
   {
      mpn_add(res + skip, res + skip, total_limbs - skip, poly[i], MIN(total_limbs - skip, output_limbs));
      i++;
      skip += coeff_limbs;
   }  
}

/*
   Recombines coefficients of a poly after doing a convolution. Assumes 
   each of the coefficients of the poly of the given length is output_limbs 
   long, that each is being shifted by a multiple of _bits_ and added
   to an mpn which is total_limbs long. It is assumed that the mpn has been 
   zeroed in advance.
*/

void FFT_combine_bits(mp_limb_t * res, mp_limb_t ** poly, ulong length, ulong bits, 
                             ulong output_limbs, ulong total_limbs)
{
   ulong top_bits = ((GMP_LIMB_BITS - 1) & bits);
   if (top_bits == 0)
   {
      FFT_combine(res, poly, length, bits/GMP_LIMB_BITS, output_limbs, total_limbs);
      return;
   }
   TMP_DECL;
   TMP_MARK;

   ulong coeff_limbs = (bits/GMP_LIMB_BITS) + 1;
   ulong i, j;
   ulong * temp = (ulong *) TMP_BALLOC_LIMBS(output_limbs + 1);
   ulong shift_bits = 0;
   ulong * limb_ptr = res;
   ulong * end = res + total_limbs;
   
   for (i = 0; (i < length) && (limb_ptr + output_limbs < end); i++)
   { 
      //for (j = 0; j < output_limbs; j += 8) PREFETCH(poly->coeffs[i+1], j);
      if (shift_bits)
      {
         mpn_lshift(temp, poly[i], output_limbs + 1, shift_bits);
         mpn_add_n(limb_ptr, limb_ptr, temp, output_limbs + 1);
      } else
      {
         mpn_add(limb_ptr, limb_ptr, output_limbs + 1, poly[i], output_limbs);
      }
      shift_bits += top_bits;
      limb_ptr += (coeff_limbs - 1);
      if (shift_bits >= GMP_LIMB_BITS)
      {
         limb_ptr++;
         shift_bits -= GMP_LIMB_BITS;
      }      
   } 

   while ((limb_ptr < end) && (i < length))
   {
      if (shift_bits)
      {
         mpn_lshift(temp, poly[i], output_limbs + 1, shift_bits);
         mpn_add_n(limb_ptr, limb_ptr, temp, end - limb_ptr);
      } else
      {
         mpn_add_n(limb_ptr, limb_ptr, poly[i], end - limb_ptr);
      }
      shift_bits += top_bits;
      limb_ptr += (coeff_limbs - 1);
      if (shift_bits >= GMP_LIMB_BITS)
      {
         limb_ptr++;
         shift_bits -= GMP_LIMB_BITS;
      }  
      i++;    
   }
   
   TMP_FREE;     
}

/*
   Normalise t to be in the range [0, 2^nw]
*/
void mpn_normmod_2expp1(mp_limb_t * t, ulong l)
{
   mp_limb_signed_t hi = t[l];
   
   if (hi)
   {
      t[l] = CNST_LIMB(0);
      mpn_addmod_2expp1_1(t, l, -hi);
      hi = t[l];

      // hi will be in [-1,1]
      if (t[l])
      {
         t[l] = CNST_LIMB(0);
         mpn_addmod_2expp1_1(t, l, -hi);
         if (t[l] == ~CNST_LIMB(0)) // if we now have -1 (very unlikely)
         {
            t[l] = CNST_LIMB(0);
            mpn_addmod_2expp1_1(t, l, 1);
         }
      }
   }
}

/*
   We are given two integers modulo 2^wn+1, i1 and i2, which are not 
   necessarily normalised. We compute t = sqrt(-1)*(i1 - i2) and are given 
   n and w. Note 2^wn/2 = sqrt(-1) mod 2^wn+1. Requires wn to be divisible
   by 2*GMP_LIMB_BITS.
*/
void mpn_submod_i_2expp1(mp_limb_t * t, mp_limb_t * i1, mp_limb_t * i2, ulong l)
{
   mp_limb_t cy;
   mp_limb_t l2 = l/2;
    
   t[l] = -mpn_sub_n(t + l2, i1, i2, l2);
   cy = i2[l] - i1[l] - mpn_sub_n(t, i2 + l2, i1 + l2, l2);
   mpn_addmod_2expp1_1(t + l2, l2, cy);
}

/*
   We are given two integers modulo 2^wn+1, i1 and i2, which are 
   not necessarily normalised and are given n and w. We compute 
   t = (i1 + i2)*B^x, u = (i1 - i2)*B^y. Aliasing between inputs and 
   outputs is not permitted. We require x and y to be less than limbs.
*/
void mpn_lshB_sumdiffmod_2expp1(mp_limb_t * t, mp_limb_t * u, mp_limb_t * i1, 
                      mp_limb_t * i2, ulong limbs, ulong x, ulong y)
{
   mp_limb_t cy, cy1, cy2;

   if (x == 0)
   {
      if (y == 0)
      {
         mpn_sumdiff_n(t + x, u + y, i1, i2, limbs + 1);
      } else
      {
         cy = mpn_sumdiff_n(t, u + y, i1, i2, limbs - y);
         u[limbs] = -(cy&1);
         cy1 = cy>>1;
         cy = mpn_sumdiff_n(t + limbs - y, u, i2 + limbs - y, i1 + limbs - y, y);
         t[limbs] = cy>>1;
         mpn_add_1(t + limbs - y, t + limbs - y, y + 1, cy1);
         cy1 = -(cy&1) + (i2[limbs] - i1[limbs]);
         mpn_addmod_2expp1_1(u + y, limbs - y, cy1);
         cy1 = -(i1[limbs] + i2[limbs]);
         mpn_addmod_2expp1_1(t, limbs, cy1);
      }
   } else if (y == 0)
   {
      cy = mpn_sumdiff_n(t + x, u, i1, i2, limbs - x);
      t[limbs] = cy>>1;
      cy1 = cy&1;
      cy = mpn_sumdiff_n(t, u + limbs - x, i1 + limbs - x, i2 + limbs - x, x);
      cy2 = mpn_neg_n(t, t, x);
      u[limbs] = -(cy&1);
      mpn_sub_1(u + limbs - x, u + limbs - x, x + 1, cy1);
      cy1 = -(cy>>1) - cy2;
      cy1 -= (i1[limbs] + i2[limbs]);
      mpn_addmod_2expp1_1(t + x, limbs - x, cy1);
      cy1 = (i2[limbs] - i1[limbs]);
      mpn_addmod_2expp1_1(u, limbs, cy1);
   } else if (x > y)
   {
      cy = mpn_sumdiff_n(t + x, u + y, i1, i2, limbs - x);
      t[limbs] = cy>>1;
      cy1 = cy&1;
      cy = mpn_sumdiff_n(t, u + y + limbs - x, i1 + limbs - x, i2 + limbs - x, x - y);
      cy2 = mpn_neg_n(t, t, x - y);
      u[limbs] = -(cy&1);
      mpn_sub_1(u + y + limbs - x, u + y + limbs - x, x - y + 1, cy1);
      cy1 = (cy>>1) + cy2;
      cy = mpn_sumdiff_n(t + x - y, u, i2 + limbs - y, i1 + limbs - y, y);
      cy2 = mpn_neg_n(t + x - y, t + x - y, y);
      cy1 = -(cy>>1) - mpn_sub_1(t + x - y, t + x - y, y, cy1) - cy2;
      cy1 -= (i1[limbs] + i2[limbs]);
      mpn_addmod_2expp1_1(t + x, limbs - x, cy1);
      cy1 = -(cy&1) + (i2[limbs] - i1[limbs]);
      mpn_addmod_2expp1_1(u + y, limbs - y, cy1);
   } else if (x < y)
   {
      cy = mpn_sumdiff_n(t + x, u + y, i1, i2, limbs - y);
      u[limbs] = -(cy&1);
      cy1 = cy>>1;
      cy = mpn_sumdiff_n(t + x + limbs - y, u, i2 + limbs - y, i1 + limbs - y, y - x);
      t[limbs] = cy>>1;
      mpn_add_1(t + x + limbs - y, t + x + limbs - y, y - x + 1, cy1);
      cy1 = cy&1;
      cy = mpn_sumdiff_n(t, u + y - x, i2 + limbs - x, i1 + limbs - x, x);
      cy1 = -(cy&1) - mpn_sub_1(u + y - x, u + y - x, x, cy1);
      cy1 += (i2[limbs] - i1[limbs]);
      mpn_addmod_2expp1_1(u + y, limbs - y, cy1);
      cy2 = mpn_neg_n(t, t, x);
      cy1 = -(cy>>1) - (i1[limbs] + i2[limbs]) - cy2;
      mpn_addmod_2expp1_1(t + x, limbs - x, cy1);
   } else // x == y
   {
      cy = mpn_sumdiff_n(t + x, u + x, i1, i2, limbs - x);
      t[limbs] = cy>>1;
      u[limbs] = -(cy&1);
      cy = mpn_sumdiff_n(t, u, i2 + limbs - x, i1 + limbs - x, x);
      cy2 = mpn_neg_n(t, t, x);
      cy1 = -(cy>>1) - (i1[limbs] + i2[limbs]) - cy2;
      mpn_addmod_2expp1_1(t + x, limbs - x, cy1);
      cy1 = -(cy&1) + i2[limbs] - i1[limbs];
      mpn_addmod_2expp1_1(u + x, limbs - x, cy1);
  }
}

/*
   We are given two integers modulo 2^wn+1, i1 and i2, which are 
   not necessarily normalised and are given n and w. We compute 
   t = i1 + i2/B^x, u = i1 - i2/B^y. Aliasing between inputs and 
   outputs is not permitted. We require x, y be less than the 
   number of limbs of i1 and i2.
*/
void mpn_sumdiff_rshBmod_2expp1(mp_limb_t * t, mp_limb_t * u, mp_limb_t * i1, 
                      mp_limb_t * i2, ulong limbs, ulong x, ulong y)
{
   mp_limb_t cy, cy1, cy2, cy3;

   if (x == 0)
   {
      if (y == 0)
      {
         mpn_sumdiff_n(t, u, i1, i2, limbs + 1);     
      } else // y != 0
      {
         cy = mpn_sumdiff_n(t, u, i1, i2 + y, limbs - y);
         cy1 = (cy>>1);
         cy2 = -(cy&1);
         cy = mpn_sumdiff_n(u + limbs - y, t + limbs - y, i1 + limbs - y, i2, y);
         u[limbs] = (cy>>1) + i1[limbs];
         t[limbs] = i1[limbs] - (cy&1);
         mpn_addmod_2expp1_1(t + limbs - y, y, cy1 + i2[limbs]);
         mpn_addmod_2expp1_1(u + limbs - y, y, cy2 - i2[limbs]);
      }
   } else if (y == 0) // x != 0
   {
      cy = mpn_sumdiff_n(t, u, i1 + x, i2, limbs - x);
      cy1 = (cy>>1);
      cy2 = -(cy&1);
      cy3 = mpn_neg_n(i1, i1, x);
      cy = mpn_sumdiff_n(t + limbs - x, u + limbs - x, i1, i2 + limbs - x, x);
      u[limbs] = -cy3 - (cy&1) - i2[limbs];
      t[limbs] = -cy3 + i2[limbs] + (cy>>1);
      mpn_addmod_2expp1_1(t + limbs - x, x, cy1 + i1[limbs]);
      mpn_addmod_2expp1_1(u + limbs - x, x, cy2 + i1[limbs]);
   } else if (x == y)
   {
      cy = mpn_sumdiff_n(t, u, i1 + x, i2 + x, limbs - x);
      cy1 = (cy>>1);
      cy2 = -(cy&1);
      cy = mpn_sumdiff_n(t + limbs - x, u + limbs - x, i2, i1, x);
      cy3 = mpn_neg_n(t + limbs - x, t + limbs - x, x);
      u[limbs] = -(cy&1);
      t[limbs] = -(cy>>1) - cy3;
      mpn_addmod_2expp1_1(t + limbs - x, x, cy1 + i1[limbs] + i2[limbs]);
      mpn_addmod_2expp1_1(u + limbs - x, x, cy2 + i1[limbs] - i2[limbs]);
   } else if (x > y)
   {
      cy = mpn_sumdiff_n(t + limbs - y, u + limbs - y, i2, i1 + x - y, y);
      cy3 = mpn_neg_n(t + limbs - y, t + limbs - y, y);
      t[limbs] = -(cy>>1) - cy3;
      u[limbs] = -(cy&1);
      cy3 = mpn_neg_n(i1, i1, x - y);
      cy = mpn_sumdiff_n(t + limbs - x, u + limbs - x, i1, i2 + limbs - x + y, x - y);
      mpn_addmod_2expp1_1(t + limbs - y, y, (cy>>1) + i2[limbs] - cy3);
      mpn_addmod_2expp1_1(u + limbs - y, y, -(cy&1) - i2[limbs] - cy3);
      cy = mpn_sumdiff_n(t, u, i1 + x, i2 + y, limbs - x);
      mpn_addmod_2expp1_1(t + limbs - x, x, (cy>>1) + i1[limbs]);
      mpn_addmod_2expp1_1(u + limbs - x, x, -(cy&1) + i1[limbs]);
   } else //(x < y)
   {
      cy = mpn_sumdiff_n(t + limbs - x, u + limbs - x, i2 + y - x, i1, x);
      cy3 = mpn_neg_n(t + limbs - x, t + limbs - x, x);
      t[limbs] = -(cy>>1) - cy3;
      u[limbs] = -(cy&1);
      cy3 = mpn_neg_n(i2, i2, y - x);
      cy = mpn_sumdiff_n(t + limbs - y, u + limbs - y, i1 + limbs - y + x, i2, y - x);
      mpn_addmod_2expp1_1(t + limbs - x, x, (cy>>1) + i1[limbs] - cy3);
      mpn_addmod_2expp1_1(u + limbs - x, x, -(cy&1) + i1[limbs] + cy3);
      cy = mpn_sumdiff_n(t, u, i1 + x, i2 + y, limbs - y);
      mpn_addmod_2expp1_1(t + limbs - y, y, (cy>>1) + i2[limbs]);
      mpn_addmod_2expp1_1(u + limbs - y, y, -(cy&1) - i2[limbs]);      
   }
}

/* 
   Given an integer i1 modulo 2^wn+1, set t to 2^d*i1 modulo 2^wm+1.
   We must have GMP_LIMB_BITS > d >= 0.
*/
void mpn_mul_2expmod_2expp1(mp_limb_t * t, mp_limb_t * i1, ulong limbs, ulong d)
{
   mp_limb_signed_t hi, hi2;
   
   if (d == 0)
   {   
      if (t != i1)
         MPN_COPY(t, i1, limbs + 1);
   } else
   {
      hi = i1[limbs]; 
      mpn_lshift(t, i1, limbs + 1, d);
      hi2 = t[limbs];
      t[limbs] = CNST_LIMB(0);
      mpn_sub_1(t, t, limbs + 1, hi2);
      hi >>= (GMP_LIMB_BITS - d);
      mpn_addmod_2expp1_1(t + 1, limbs - 1, -hi);
   }
}

/* 
   Given an integer i1 modulo 2^wn+1, set t to i1/2^d modulo 2^wm+1.
   We must have GMP_LIMB_BITS > d >= 0.
*/
void mpn_div_2expmod_2expp1(mp_limb_t * t, mp_limb_t * i1, ulong limbs, ulong d)
{
   mp_limb_t lo;
   mp_limb_t * ptr;
   mp_limb_signed_t hi;
   
   if (d == 0)
   {   
      if (t != i1)
         MPN_COPY(t, i1, limbs + 1);
   } else
   {
      hi = i1[limbs];
      lo = mpn_rshift(t, i1, limbs + 1, d);
      t[limbs] = (hi>>d);
      ptr = t + limbs - 1;
      sub_ddmmss(ptr[1], ptr[0], ptr[1], ptr[0], CNST_LIMB(0), lo);
   }
}

/* 
   Set  {s, t} to { z1^i*(s+t), z2^i*(s-t) } where 
   z1 = exp(2*Pi*I/(2*n)), z2 = exp(2*Pi*I*3/(2*n)), z1=>w bits
*/
void FFT_split_radix_butterfly(mp_limb_t * s, mp_limb_t * t, ulong i, ulong n, ulong w, mp_limb_t * u)
{
   mp_limb_t size = (w*n)/GMP_LIMB_BITS + 1;
   mp_limb_t * v;
   ulong x, y, b1, b2;
   int negate = 0;
   int negate2 = 0;
   
   v = u + size;

   b1 = i;
   while (b1 >= n) 
   {
      negate2 = 1 - negate2;
      b1 -= n;
   }
   b1 = b1*w;
   x = b1/GMP_LIMB_BITS;
   b1 -= x*GMP_LIMB_BITS;
   b2 = 3*i;
   while (b2 >= n) 
   {
      negate = 1 - negate;
      b2 -= n;
   }
   b2 = b2*w;
   y = b2/GMP_LIMB_BITS;
   b2 -= y*GMP_LIMB_BITS;
 
   mpn_lshB_sumdiffmod_2expp1(u, v, s, t, size - 1, x, y);
   mpn_mul_2expmod_2expp1(s, u, size - 1, b1);
   mpn_mul_2expmod_2expp1(t, v, size - 1, b2);
   if (negate) mpn_neg_n(t, t, size);
   if (negate2) mpn_neg_n(s, s, size);
}

void FFT_split_radix_butterfly2(mp_limb_t * u, mp_limb_t * v, mp_limb_t * s, mp_limb_t * t, ulong i, ulong n, ulong w)
{
   mp_limb_t size = (w*n)/GMP_LIMB_BITS + 1;
   ulong x, y, b1, b2;
   int negate = 0;
   int negate2 = 0;
   
   b1 = i;
   while (b1 >= n) 
   {
      negate2 = 1 - negate2;
      b1 -= n;
   }
   b1 = b1*w;
   x = b1/GMP_LIMB_BITS;
   b1 -= x*GMP_LIMB_BITS;
   b2 = 3*i;
   while (b2 >= n) 
   {
      negate = 1 - negate;
      b2 -= n;
   }
   b2 = b2*w;
   y = b2/GMP_LIMB_BITS;
   b2 -= y*GMP_LIMB_BITS;
 
   mpn_lshB_sumdiffmod_2expp1(u, v, s, t, size - 1, x, y);
   mpn_mul_2expmod_2expp1(u, u, size - 1, b1);
   if (negate2) mpn_neg_n(u, u, size);
   mpn_mul_2expmod_2expp1(v, v, size - 1, b2);
   if (negate) mpn_neg_n(v, v, size);
}

/*
   Set u = 2^{b1}*(s + t), v = 2^{b2}*(s - t)
   Note NW is just n*w.
*/
void FFT_radix2_twiddle_butterfly(mp_limb_t * u, mp_limb_t * v, 
          mp_limb_t * s, mp_limb_t * t, ulong NW, ulong b1, ulong b2)
{
   mp_limb_t size = NW/GMP_LIMB_BITS + 1;
   ulong x, y;
   int negate = 0;
   int negate2 = 0;
   
   b1 %= (2*NW);
   if (b1 >= NW) 
   {
      negate2 = 1;
      b1 -= NW;
   }
   x = b1/GMP_LIMB_BITS;
   b1 -= x*GMP_LIMB_BITS;

   b2 %= (2*NW);
   if (b2 >= NW) 
   {
      negate = 1;
      b2 -= NW;
   }
   y = b2/GMP_LIMB_BITS;
   b2 -= y*GMP_LIMB_BITS;
 
   mpn_lshB_sumdiffmod_2expp1(u, v, s, t, size - 1, x, y);
   mpn_mul_2expmod_2expp1(u, u, size - 1, b1);
   if (negate2) mpn_neg_n(u, u, size);
   mpn_mul_2expmod_2expp1(v, v, size - 1, b2);
   if (negate) mpn_neg_n(v, v, size);
}
    
/*
   Set s = i1 + i2, t = z1*(i1 - i2) where z1 = exp(2*Pi*I/m) => w bits
*/
void FFT_radix2_butterfly(mp_limb_t * s, mp_limb_t * t, 
                  mp_limb_t * i1, mp_limb_t * i2, ulong i, ulong n, ulong w)
{
   mp_limb_t size = (w*n)/GMP_LIMB_BITS + 1;
   ulong x, y, b1;
   int negate = 0;

   x = 0;

   b1 = i;
   while (b1 >= n) 
   {
      negate = 1 - negate;
      b1 -= n;
   }
   b1 = b1*w;
   y = b1/GMP_LIMB_BITS;
   b1 -= y*GMP_LIMB_BITS;
 
   mpn_lshB_sumdiffmod_2expp1(s, t, i1, i2, size - 1, x, y);
   mpn_mul_2expmod_2expp1(t, t, size - 1, b1);
   if (negate) mpn_neg_n(t, t, size);
}

/*
   Set s = i1 + z1*i2, t = i1 - z1*i2 where z1 = exp(-2*Pi*I/m) => w bits
*/
void FFT_radix2_inverse_butterfly(mp_limb_t * s, mp_limb_t * t, 
                  mp_limb_t * i1, mp_limb_t * i2, ulong i, ulong n, ulong w)
{
   mp_limb_t limbs = (w*n)/GMP_LIMB_BITS;
   ulong y, b1;
   
   b1 = i*w;
   y = b1/GMP_LIMB_BITS;
   b1 -= y*GMP_LIMB_BITS;

   mpn_div_2expmod_2expp1(i2, i2, limbs, b1);
   mpn_sumdiff_rshBmod_2expp1(s, t, i1, i2, limbs, 0, y);
}

/*
   s = 2^-b1*i1 + 2^-b2*i2, t = 2^-b1*i1 - 2^-b1*i2.
   Note NW is just n*w
*/
void FFT_radix2_twiddle_inverse_butterfly(mp_limb_t * s, mp_limb_t * t, 
                  mp_limb_t * i1, mp_limb_t * i2, ulong NW, ulong b1, ulong b2)
{
   mp_limb_t limbs = NW/GMP_LIMB_BITS;
   ulong x, y;
   int negate = 0;
   int negate2 = 0;
   
   b1 %= (2*NW);
   if (b1 >= NW)
   {
      negate = 1;
      b1 -= NW;
   }
   x = b1/GMP_LIMB_BITS;
   b1 -= x*GMP_LIMB_BITS;

   b2 %= (2*NW);
   if (b2 >= NW)
   {
      negate2 = 1;
      b2 -= NW;
   }
   y = b2/GMP_LIMB_BITS;
   b2 -= y*GMP_LIMB_BITS;

   if (negate) mpn_neg_n(i1, i1, limbs + 1);
   mpn_div_2expmod_2expp1(i1, i1, limbs, b1);
   if (negate2) mpn_neg_n(i2, i2, limbs + 1);
   mpn_div_2expmod_2expp1(i2, i2, limbs, b2);
   mpn_sumdiff_rshBmod_2expp1(s, t, i1, i2, limbs, x, y);
}

void FFT_radix2(mp_limb_t ** rr, ulong rs, mp_limb_t ** ii, 
        ulong n, ulong w, mp_limb_t ** t1, mp_limb_t ** t2, mp_limb_t ** temp);

/* 
   The split radix DIF FFT works as follows:
   Given: inputs [i0, i1, ..., i{m-1}], for m a power of 2

   Output: Fsplit[i0, i1, ..., i{m-1}] = [r0, r1, ..., r{m-1}]
   (Inputs and outputs may be subject to a stride and not in consecutive locations.)

   Algorithm: * Recursively compute [r0, r2, r4, ...., r{m-2}] 
                           = Fsplit[i0+i{m/2}, i1+i{m/2+1}, ..., i{m/2-1}+i{m-1}]
              * Let [s0, s1, ..., s{m/4-1}] 
                    = [i0-i{m/2}, i1-i{m/2+1}, ..., i{m/4-1}-i{3m/4-1}]
              * Let [t0, t1, ..., t{m/4-1}] 
                    = sqrt(-1)[i{m/4}-i{3m/4}, i{m/4+1}-i{3m/4+1}, ..., i{m/2-1}-i{m-1}]
              * Let [u0, u1, ..., u{m/4-1}] 
                    = [z1^0*(s0+t0), z1^1(s1+t1), ..., z1^{m/4-1}*(s{m/4-1}+t{m/4-1})]
                where z1 = exp(2*Pi*I/m)
              * Let [v0, v1, ..., v{m/4-1}] 
                    = [z2^0*(s0-t0), z2^1(s1-t1), ..., z2^{m/4-1}*(s{m/4-1}-t{m/4-1})]
                where z2 = exp(2*Pi*I*3/m)
              * Recursively compute [r1, r5, ..., r{m-3}]
                           = Fsplit[u0, u1, ..., u{m/4-1}]
              * Recursively compute [r3, r7, ..., r{m-1}]
                           = Fsplit[v0, v1, ..., v{m/4-1}]

   The parameters are as follows:
              * 2*n is the length of the input and output arrays (m in the above)
              * w is such that 2^w is an 2n-th root of unity in the ring Z/pZ that 
                we are working in, i.e. p = 2^wn + 1 (here n is divisible by 
                GMP_LIMB_BITS)
              * ii is the array of inputs (each input is an array of limbs of length 
                wn/GMP_LIMB_BITS + 1 (the extra limb being a "carry limb")
              * rr is the array of outputs (each being an array of limbs of length
                wn/GMP_LIMB_BITS + 1)
              * rs is the stride with which entries of rr should be written (e.g. 
                rs = 4 means that the output is to be written in every 4th entry of 
                rr)
              * is is the stride with which entries of ii should be read
*/

void FFT_split_radix(mp_limb_t ** rr, ulong rs, mp_limb_t ** ii, 
                    ulong n, ulong w, mp_limb_t ** t1, mp_limb_t ** t2, mp_limb_t ** temp)
{
   mp_limb_t * ptr;
   ulong i;
   ulong size = (w*n)/GMP_LIMB_BITS + 1;
   
   if (n < 4) 
   {
      FFT_radix2(rr, rs, ii, n, w, t1, t2, temp);
      return;
   }

   // [s0, s1, ..., s{m/4-1}] = [i0-i{m/2}, i1-i{m/2+1}, ..., i{m/4-1}-i{3m/4-1}]
   for (i = 0; i < n/2; i++) 
   {
      mpn_sub_n(temp[i], ii[i], ii[i+n], size);
      mpn_add_n(ii[i+n], ii[i], ii[i+n], size);
   }
   
   // [t0, t1, ..., t{m/4-1}] = sqrt(-1)[i{m/4}-i{3m/4}, i{m/4+1}-i{3m/4+1}, ..., i{m/2-1}-i{m-1}]
   for (i = 0; i < n/2; i++)
   {
      mpn_submod_i_2expp1(temp[i+n/2], ii[i+n/2], ii[i+3*n/2], size - 1);
      mpn_add_n(ii[i+3*n/2], ii[i+n/2], ii[i+3*n/2], size);
   }

   // [u0, u1, ..., u{m/4-1}] = [z1^0*(s0+t0), z1^1(s1+t1), ..., z1^{m/4-1}*(s{m/4-1}+t{m/4-1})]
   // [v0, v1, ..., v{m/4-1}] = [z2^0*(s0-t0), z2^1(s1-t1), ..., z2^{m/4-1}*(s{m/4-1}-t{m/4-1})]
   // where z1 = exp(2*Pi*I/m), z2 = exp(2*Pi*I*3/m), z1 => w bits
   // note {u, v} = {ss, tt}
   for (i = 0; i < n/2; i++) 
   {
      FFT_split_radix_butterfly2(*t1, *t2, temp[i], temp[i+n/2], i, n, w);
      ptr = temp[i];
      temp[i] = *t1;
      *t1 = ptr;
      ptr = temp[i+n/2];
      temp[i+n/2] = *t2;
      *t2 = ptr;
   }

   // [r0, r2, r4, ...., r{m-2}] = Fsplit[i0+i{m/2}, i1+i{m/2+1}, ..., i{m/2-1}+i{m-1}]
   FFT_split_radix(rr, 2*rs, ii+n, n/2, 2*w, t1, t2, temp + n);

   // [r1, r5, ..., r{m-3}] = Fsplit[u0, u1, ..., u{m/4-1}]
   FFT_split_radix(rr + rs, 4*rs, temp, n/4, 4*w, t1, t2, temp + n);

   // [r3, r7, ..., r{m-1}] = Fsplit[v0, v1, ..., v{m/4-1}]
   FFT_split_radix(rr + 3*rs, 4*rs, temp+n/2, n/4, 4*w, t1, t2, temp + n);
}

/* 
   The radix 2 DIF FFT works as follows:
   Given: inputs [i0, i1, ..., i{m-1}], for m a power of 2

   Output: Fradix2[i0, i1, ..., i{m-1}] = [r0, r1, ..., r{m-1}]
   (Inputs and outputs may be subject to a stride and not in consecutive locations.)

   Algorithm: * Recursively compute [r0, r2, r4, ...., r{m-2}] 
                           = Fsplit[i0+i{m/2}, i1+i{m/2+1}, ..., i{m/2-1}+i{m-1}]
              * Let [t0, t1, ..., t{m/2-1}] 
                    = [i0-i{m/2}, i1-i{m/2+1}, ..., i{m/2-1}-i{m-1}]
              * Let [u0, u1, ..., u{m/2-1}] 
                    = [z1^0*t0, z1^1*t1, ..., z1^{m/2-1}*t{m/2-1}]
                where z1 = exp(2*Pi*I/m)
              * Recursively compute [r1, r3, ..., r{m-1}]
                           = Fsplit[u0, u1, ..., u{m/2-1}]
              
   The parameters are as follows:
              * 2*n is the length of the input and output arrays (m in the above)
              * w is such that 2^w is an 2n-th root of unity in the ring Z/pZ that 
                we are working in, i.e. p = 2^wn + 1 (here n is divisible by 
                GMP_LIMB_BITS)
              * ii is the array of inputs (each input is an array of limbs of length 
                wn/GMP_LIMB_BITS + 1 (the extra limb being a "carry limb")
              * rr is the array of outputs (each being an array of limbs of length
                wn/GMP_LIMB_BITS + 1)
              * rs is the stride with which entries of rr should be written (e.g. 
                rs = 4 means that the output is to be written in every 4th entry of 
                rr)
              * is is the stride with which entries of ii should be read
*/

void FFT_radix2(mp_limb_t ** rr, ulong rs, mp_limb_t ** ii, 
      ulong n, ulong w, mp_limb_t ** t1, mp_limb_t ** t2, mp_limb_t ** temp)
{
   mp_limb_t ** ss, ** tt;
   mp_limb_t * ptr;
   ulong i;
   ulong size = (w*n)/GMP_LIMB_BITS + 1;
   
   if (n == 1) 
   {
      FFT_radix2_butterfly(*t1, *t2, ii[0], ii[1], 0, n, w);
      ptr = rr[0];
      rr[0] = *t1;
      *t1 = ptr;
      ptr = rr[rs];
      rr[rs] = *t2;
      *t2 = ptr;
      return;
   }

   // [s0, s1, ..., s{m/2}] = [i0+i{m/2}, i1+i{m/2+1}, ..., i{m/2-1}+i{m-1}]
   // [t0, t1, ..., t{m/2-1}] 
   // = [z1^0*(i0-i{m/2}), z1^1*(i1-i{m/2+1}), ..., z1^{m/2-1}*(i{m/2-1}-i{m-1})]
   // where z1 = exp(2*Pi*I/m), z1 => w bits
   for (i = 0; i < n; i++) 
   {   
      FFT_radix2_butterfly(*t1, temp[i], ii[i], ii[n+i], i, n, w);
   
      ptr = ii[i];
      ii[i] = *t1;
      *t1 = ptr;
   }

   // [r0, r2, ..., r{m-2}] = Fradix2[s0, s1, ..., s{m/2-1}]
   FFT_radix2(rr, 2*rs, ii, n/2, 2*w, t1, t2, temp + n);
   
   // [r1, r3, ..., r{m-1}] = Fradix2[t0, t1, ..., t{m/2-1}]
   FFT_radix2(rr + rs, 2*rs, temp, n/2, 2*w, t1, t2, temp + n);
}

void FFT_radix2_new(mp_limb_t ** rr, ulong rs, mp_limb_t ** ii, 
      ulong n, ulong w, mp_limb_t ** t1, mp_limb_t ** t2, mp_limb_t ** temp)
{
   mp_limb_t ** ss, ** tt;
   mp_limb_t * ptr;
   ulong i, j, k, l;
   ulong size = (w*n)/GMP_LIMB_BITS + 1;
   
   if (n == 1) 
   {
      FFT_radix2_butterfly(*t1, *t2, ii[0], ii[1], 0, n, w);
      ptr = rr[0];
      rr[0] = *t1;
      *t1 = ptr;
      ptr = rr[rs];
      rr[rs] = *t2;
      *t2 = ptr;
      return;
   }

   // [s0, s1, ..., s{m/2}] = [i0+i{m/2}, i1+i{m/2+1}, ..., i{m/2-1}+i{m-1}]
   // [t0, t1, ..., t{m/2-1}] 
   // = [z1^0*(i0-i{m/2}), z1^1*(i1-i{m/2+1}), ..., z1^{m/2-1}*(i{m/2-1}-i{m-1})]
   // where z1 = exp(2*Pi*I/m), z1 => w bits
   for (i = 0; i < n; i++) 
   {   
      FFT_radix2_butterfly(*t1, *t2, ii[i], ii[n+i], i, n, w);
   
      ptr = ii[i];
      ii[i] = *t1;
      *t1 = ptr;
      ptr = ii[n+i];
      ii[n+i] = *t2;
      *t2 = ptr;
   }

   // [r0, r2, ..., r{m-2}] = Fradix2[s0, s1, ..., s{m/2-1}]
   FFT_radix2_new(rr, 1, ii, n/2, 2*w, t1, t2, temp);
   
   // [r1, r3, ..., r{m-1}] = Fradix2[t0, t1, ..., t{m/2-1}]
   FFT_radix2_new(rr + n, 1, ii+n, n/2, 2*w, t1, t2, temp);
}

int FFT_negacyclic_twiddle(mp_limb_t * r, mp_limb_t * i1, ulong i, ulong n, ulong w)
{
   mp_limb_t cy;
   ulong limbs = (n*w)/GMP_LIMB_BITS;
   int negate = 0;
   while (i >= 2*n)
   {
      negate = 1 - negate;
      i -= 2*n;
   }
   ulong b1 = (w*i)/2;
   ulong x = b1/GMP_LIMB_BITS;
   b1 -= x*GMP_LIMB_BITS;
   //if ((!x) && (negate)) 
   if (negate) mpn_neg_n(i1, i1, limbs + 1);
   mpn_mul_2expmod_2expp1(i1, i1, limbs, b1);
   if (x)
   {
      /*if (negate)
      {
         r[limbs] = -mpn_neg_n(r + x, i1, limbs - x);
         MPN_COPY(r, i1 + limbs - x, x);
         mpn_addmod_2expp1_1(r + x, limbs - x, i1[limbs]);
      } else
      {*/
         MPN_COPY(r + x, i1, limbs - x);
         r[limbs] = CNST_LIMB(0);
         cy = mpn_neg_n(r, i1 + limbs - x, x);
         mpn_addmod_2expp1_1(r + x, limbs - x, -cy - i1[limbs]);
      //}
      return 1;
   }
   return 0;
}

void FFT_twiddle(mp_limb_t * r, mp_limb_t * i1, ulong i, ulong n, ulong w)
{
   mp_limb_t cy;
   ulong limbs = (n*w)/GMP_LIMB_BITS;
   int negate = 0;
   while (i >= n)
   {
      negate = 1 - negate;
      i -= n;
   }
   ulong b1 = w*i;
   ulong x = b1/GMP_LIMB_BITS;
   b1 -= x*GMP_LIMB_BITS;
   if (x)
   {
      MPN_COPY(r + x, i1, limbs - x);
      r[limbs] = CNST_LIMB(0);
      cy = mpn_neg_n(r, i1 + limbs - x, x);
      mpn_addmod_2expp1_1(r + x, limbs - x, -cy - i1[limbs]);
      if (negate) mpn_neg_n(r, r, limbs + 1);
      mpn_mul_2expmod_2expp1(r, r, limbs, b1);
   } else
   {
      if (negate) 
      {
         mpn_neg_n(r, i1, limbs + 1);
         mpn_mul_2expmod_2expp1(r, r, limbs, b1);
      } else
         mpn_mul_2expmod_2expp1(r, i1, limbs, b1);
   }
}

void FFT_radix2_twiddle(mp_limb_t ** ii, ulong is,
      ulong n, ulong w, mp_limb_t ** t1, mp_limb_t ** t2, mp_limb_t ** temp,
      ulong ws, ulong r, ulong c, ulong rs);

/* 
   Truncate FFT to any length given by trunc, so long as trunc is 
   divisible by 2.
*/
void FFT_radix2_truncate1(mp_limb_t ** rr, ulong rs, mp_limb_t ** ii, 
      ulong n, ulong w, mp_limb_t ** t1, mp_limb_t ** t2, mp_limb_t ** temp, 
      ulong trunc)
{
   mp_limb_t ** ss, ** tt;
   mp_limb_t * ptr;
   ulong i, j, k, l;
   ulong size = (w*n)/GMP_LIMB_BITS + 1;
   
   if (trunc == 2*n)
   {
      FFT_radix2_new(rr, rs, ii, n, w, t1, t2, temp);
      return;
   }

   if (trunc <= n)
   {
      for (i = 0; i < n; i++)
         mpn_add_n(ii[i], ii[i], ii[i+n], size);
      
      FFT_radix2_truncate1(rr, rs, ii, n/2, 2*w, t1, t2, temp, trunc);
   } else
   {

      // [s0, s1, ..., s{m/2}] = [i0+i{m/2}, i1+i{m/2+1}, ..., i{m/2-1}+i{m-1}]
      // [t0, t1, ..., t{m/2-1}] 
      // = [z1^0*(i0-i{m/2}), z1^1*(i1-i{m/2+1}), ..., z1^{m/2-1}*(i{m/2-1}-i{m-1})]
      // where z1 = exp(2*Pi*I/m), z1 => w bits
      for (i = 0; i < n; i++) 
      {   
         FFT_radix2_butterfly(*t1, *t2, ii[i], ii[n+i], i, n, w);
   
         ptr = ii[i];
         ii[i] = *t1;
         *t1 = ptr;
         ptr = ii[n+i];
         ii[n+i] = *t2;
         *t2 = ptr;
      }

      // [r0, r2, ..., r{m-2}] = Fradix2[s0, s1, ..., s{m/2-1}]
      FFT_radix2_new(rr, 1, ii, n/2, 2*w, t1, t2, temp);
   
      // [r1, r3, ..., r{m-1}] = Fradix2[t0, t1, ..., t{m/2-1}]
      FFT_radix2_truncate1(rr + n, 1, ii+n, n/2, 2*w, t1, t2, temp, trunc - n);
   }
}

void FFT_radix2_truncate1_twiddle(mp_limb_t ** ii, ulong is,
      ulong n, ulong w, mp_limb_t ** t1, mp_limb_t ** t2, mp_limb_t ** temp,
      ulong ws, ulong r, ulong c, ulong rs, ulong trunc)
{
   mp_limb_t ** ss, ** tt;
   mp_limb_t * ptr;
   ulong i, j, k, l;
   ulong size = (w*n)/GMP_LIMB_BITS + 1;
   
   if (trunc == 2*n)
   {
      FFT_radix2_twiddle(ii, is, n, w, t1, t2, temp, ws, r, c, rs);
      return;
   }

   if (trunc <= n)
   {
      for (i = 0; i < n; i++)
         mpn_add_n(ii[i*is], ii[i*is], ii[(i+n)*is], size);
      
      FFT_radix2_truncate1_twiddle(ii, is, n/2, 2*w, t1, t2, temp, ws, r, c, 2*rs, trunc);
   } else
   {

      // [s0, s1, ..., s{m/2}] = [i0+i{m/2}, i1+i{m/2+1}, ..., i{m/2-1}+i{m-1}]
      // [t0, t1, ..., t{m/2-1}] 
      // = [z1^0*(i0-i{m/2}), z1^1*(i1-i{m/2+1}), ..., z1^{m/2-1}*(i{m/2-1}-i{m-1})]
      // where z1 = exp(2*Pi*I/m), z1 => w bits
      for (i = 0; i < n; i++) 
      {   
         FFT_radix2_butterfly(*t1, *t2, ii[i*is], ii[(n+i)*is], i, n, w);
   
         ptr = ii[i*is];
         ii[i*is] = *t1;
         *t1 = ptr;
         ptr = ii[(n+i)*is];
         ii[(n+i)*is] = *t2;
         *t2 = ptr;
      }

      // [r0, r2, ..., r{m-2}] = Fradix2[s0, s1, ..., s{m/2-1}]
      FFT_radix2_twiddle(ii, is, n/2, 2*w, t1, t2, temp, ws, r, c, 2*rs);
   
      // [r1, r3, ..., r{m-1}] = Fradix2[t0, t1, ..., t{m/2-1}]
      FFT_radix2_truncate1_twiddle(ii + n*is, is, n/2, 2*w, t1, t2, temp, ws, r + rs, c, 2*rs, trunc - n);
   }
}

/* 
   Truncate FFT to any length given by trunc, so long as trunc is 
   divisible by 2. Assumes zeros from trunc to n.
*/
void FFT_radix2_truncate(mp_limb_t ** rr, ulong rs, mp_limb_t ** ii, 
      ulong n, ulong w, mp_limb_t ** t1, mp_limb_t ** t2, mp_limb_t ** temp, 
      ulong trunc)
{
   mp_limb_t ** ss, ** tt;
   mp_limb_t * ptr;
   ulong i, j, k, l;
   ulong size = (w*n)/GMP_LIMB_BITS + 1;
   
   if (trunc == 2*n)
   {
      FFT_radix2_new(rr, rs, ii, n, w, t1, t2, temp);
      return;
   }

   if (trunc <= n)
   {
      // PASS
      FFT_radix2_truncate(rr, rs, ii, n/2, 2*w, t1, t2, temp, trunc);
   } else
   {
      // PASS
      // [s0, s1, ..., s{m/2}] = [i0+i{m/2}, i1+i{m/2+1}, ..., i{m/2-1}+i{m-1}]
      // [t0, t1, ..., t{m/2-1}] 
      // = [z1^0*(i0-i{m/2}), z1^1*(i1-i{m/2+1}), ..., z1^{m/2-1}*(i{m/2-1}-i{m-1})]
      // where z1 = exp(2*Pi*I/m), z1 => w bits
      for (i = 0; i < trunc - n; i++) 
      {   
         FFT_radix2_butterfly(*t1, *t2, ii[i], ii[n+i], i, n, w);
   
         ptr = ii[i];
         ii[i] = *t1;
         *t1 = ptr;
         ptr = ii[n+i];
         ii[n+i] = *t2;
         *t2 = ptr;
      }

      for (i = trunc; i < 2*n; i++)
      {
         FFT_twiddle(ii[i], ii[i-n], i - n, n, w); 
      }
   
      // [r0, r2, ..., r{m-2}] = Fradix2[s0, s1, ..., s{m/2-1}]
      FFT_radix2_new(rr, 1, ii, n/2, 2*w, t1, t2, temp);

      // [r1, r3, ..., r{m-1}] = Fradix2[t0, t1, ..., t{m/2-1}]
      FFT_radix2_truncate1(rr + n, 1, ii+n, n/2, 2*w, t1, t2, temp, trunc - n);
   }
}

void FFT_radix2_truncate_twiddle(mp_limb_t ** ii, ulong is,
      ulong n, ulong w, mp_limb_t ** t1, mp_limb_t ** t2, mp_limb_t ** temp,
      ulong ws, ulong r, ulong c, ulong rs, ulong trunc)
{
   mp_limb_t ** ss, ** tt;
   mp_limb_t * ptr;
   ulong i, j, k, l;
   ulong size = (w*n)/GMP_LIMB_BITS + 1;
   
   if (trunc == 2*n)
   {
      FFT_radix2_twiddle(ii, is, n, w, t1, t2, temp, ws, r, c, rs);
      return;
   }

   if (trunc <= n)
   {
      // PASS
      FFT_radix2_truncate_twiddle(ii, is, n/2, 2*w, t1, t2, temp, ws, r, c, 2*rs, trunc);
   } else
   {
      // PASS
      // [s0, s1, ..., s{m/2}] = [i0+i{m/2}, i1+i{m/2+1}, ..., i{m/2-1}+i{m-1}]
      // [t0, t1, ..., t{m/2-1}] 
      // = [z1^0*(i0-i{m/2}), z1^1*(i1-i{m/2+1}), ..., z1^{m/2-1}*(i{m/2-1}-i{m-1})]
      // where z1 = exp(2*Pi*I/m), z1 => w bits
      for (i = 0; i < trunc - n; i++) 
      {   
         FFT_radix2_butterfly(*t1, *t2, ii[i*is], ii[(n+i)*is], i, n, w);
   
         ptr = ii[i*is];
         ii[i*is] = *t1;
         *t1 = ptr;
         ptr = ii[(n+i)*is];
         ii[(n+i)*is] = *t2;
         *t2 = ptr;
      }

      for (i = trunc; i < 2*n; i++)
      {
         FFT_twiddle(ii[i*is], ii[(i-n)*is], i - n, n, w); 
      }
   
      // [r0, r2, ..., r{m-2}] = Fradix2[s0, s1, ..., s{m/2-1}]
      FFT_radix2_twiddle(ii, is, n/2, 2*w, t1, t2, temp, ws, r, c, 2*rs);

      // [r1, r3, ..., r{m-1}] = Fradix2[t0, t1, ..., t{m/2-1}]
      FFT_radix2_truncate1_twiddle(ii + n*is, is, n/2, 2*w, t1, t2, temp, ws, r + rs, c, 2*rs, trunc - n);
   }
}


void FFT_radix2_negacyclic(mp_limb_t ** rr, ulong rs, mp_limb_t ** ii, 
      ulong n, ulong w, mp_limb_t ** t1, mp_limb_t ** t2, mp_limb_t ** temp)
{
   mp_limb_t ** ss, ** tt;
   mp_limb_t * ptr;
   ulong i, j, k, l;
   ulong size = (w*n)/GMP_LIMB_BITS + 1;
   
   // we first apply twiddle factors corresponding to shifts of w*i/2 bits
   // this could partially be combined with the butterflies below

   // [s0, s1, ..., s{m/2}] = [i0+i{m/2}, i1+i{m/2+1}, ..., i{m/2-1}+i{m-1}]
   // [t0, t1, ..., t{m/2-1}] 
   // = [z1^0*(i0-i{m/2}), z1^1*(i1-i{m/2+1}), ..., z1^{m/2-1}*(i{m/2-1}-i{m-1})]
   // where z1 = exp(2*Pi*I/m), z1 => w bits
   for (i = 0; i < n; i++) 
   {   
      if (FFT_negacyclic_twiddle(*t1, ii[i], i, n, w))
      {
         ptr = ii[i];
         ii[i] = *t1;
         *t1 = ptr;
      }
      
      if (FFT_negacyclic_twiddle(*t1, ii[n + i], n + i, n, w))
      {
         ptr = ii[n + i];
         ii[n + i] = *t1;
         *t1 = ptr;
      }
      
      FFT_radix2_butterfly(*t1, *t2, ii[i], ii[n+i], i, n, w);
   
      ptr = ii[i];
      ii[i] = *t1;
      *t1 = ptr;
      ptr = ii[n+i];
      ii[n+i] = *t2;
      *t2 = ptr;
   }

   // [r0, r2, ..., r{m-2}] = Fradix2[s0, s1, ..., s{m/2-1}]
   FFT_radix2_new(rr, 1, ii, n/2, 2*w, t1, t2, temp);
   
   // [r1, r3, ..., r{m-1}] = Fradix2[t0, t1, ..., t{m/2-1}]
   FFT_radix2_new(rr + n, 1, ii+n, n/2, 2*w, t1, t2, temp);

   j = 0; k = 1; l = n+1;
   temp[0] = ii[1];
   ii[1] = ii[n];
   for (i = 2; i < n; i += 2)
   {
      temp[k++] = ii[i];
      ii[i] = temp[j++];
      temp[k++] = ii[i+1];
      ii[i+1] = ii[l++];
   }
   for ( ; i < 2*n; i+=2)
   {
      ii[i] = temp[j++];
      ii[i+1] = ii[l++];
   }
}

/*
   FFT of length 2*n on entries of ii with stride is.
   Apply additional twists by z^{c*i} where i starts at r and increases by rs
   for each coeff. Note z => ws bits.
*/
void FFT_radix2_twiddle(mp_limb_t ** ii, ulong is,
      ulong n, ulong w, mp_limb_t ** t1, mp_limb_t ** t2, mp_limb_t ** temp,
      ulong ws, ulong r, ulong c, ulong rs)
{
   mp_limb_t ** ss, ** tt;
   mp_limb_t * ptr;
   ulong i, j, k, l;
   ulong size = (w*n)/GMP_LIMB_BITS + 1;
   
   if (n == 1) 
   {
      ulong tw1, tw2;
      tw1 = r*c;
      tw2 = tw1 + rs*c;
      FFT_radix2_twiddle_butterfly(*t1, *t2, ii[0], ii[is], n*w, tw1*ws, tw2*ws);
      ptr = ii[0];
      ii[0] = *t1;
      *t1 = ptr;
      ptr = ii[is];
      ii[is] = *t2;
      *t2 = ptr;
      return;
   }

   // [s0, s1, ..., s{m/2}] = [i0+i{m/2}, i1+i{m/2+1}, ..., i{m/2-1}+i{m-1}]
   // [t0, t1, ..., t{m/2-1}] 
   // = [z1^0*(i0-i{m/2}), z1^1*(i1-i{m/2+1}), ..., z1^{m/2-1}*(i{m/2-1}-i{m-1})]
   // where z1 = exp(2*Pi*I/m), z1 => w bits
   for (i = 0; i < n; i++) 
   {   
      FFT_radix2_butterfly(*t1, *t2, ii[i*is], ii[(n+i)*is], i, n, w);
   
      ptr = ii[i*is];
      ii[i*is] = *t1;
      *t1 = ptr;
      ptr = ii[(n+i)*is];
      ii[(n+i)*is] = *t2;
      *t2 = ptr;
   }

   // [r0, r2, ..., r{m-2}] = Fradix2[s0, s1, ..., s{m/2-1}]
   FFT_radix2_twiddle(ii, is, n/2, 2*w, t1, t2, temp, ws, r, c, 2*rs);
   
   // [r1, r3, ..., r{m-1}] = Fradix2[t0, t1, ..., t{m/2-1}]
   FFT_radix2_twiddle(ii+n*is, is, n/2, 2*w, t1, t2, temp, ws, r + rs, c, 2*rs);
}

void IFFT_radix2_new(mp_limb_t ** rr, ulong rs, mp_limb_t ** ii, 
      ulong n, ulong w, mp_limb_t ** t1, mp_limb_t ** t2, mp_limb_t ** temp)
{
   mp_limb_t ** ss, ** tt;
   mp_limb_t * ptr;
   ulong i, j, k, l;
   ulong size = (w*n)/GMP_LIMB_BITS + 1;
   
   if (n == 1) 
   {
      FFT_radix2_inverse_butterfly(*t1, *t2, ii[0], ii[1], 0, n, w);
      ptr = rr[0];
      rr[0] = *t1;
      *t1 = ptr;
      ptr = rr[rs];
      rr[rs] = *t2;
      *t2 = ptr;
      return;
   }

   // [s0, s1, ..., s{m/2-1}] = Fradix2_inverse[i0, i2, ..., i{m-2}]
   IFFT_radix2_new(ii, 1, ii, n/2, 2*w, t1, t2, temp);
   
   // [t{m/2}, t{m/2+1}, ..., t{m-1}] = Fradix2_inverse[i1, i3, ..., i{m-1}]
   IFFT_radix2_new(ii+n, 1, ii+n, n/2, 2*w, t1, t2, temp);

   // [r0, r1, ..., r{m/2-1}] 
   // = [s0+z1^0*t0, s1+z1^1*t1, ..., s{m/2-1}+z1^{m/2-1}*t{m-1}]
   // [r{m/2}, t{m/2+1}, ..., r{m-1}] 
   // = [s0-z1^0*t{m/2}, s1-z1^1*t{m/2+1}, ..., s{m/2-1}-z1^{m/2-1}*t{m-1}]
   // where z1 = exp(-2*Pi*I/m), z1 => w bits
   for (i = 0; i < n; i++) 
   {   
      FFT_radix2_inverse_butterfly(*t1, *t2, ii[i], ii[n+i], i, n, w);
   
      ptr = rr[i];
      rr[i] = *t1;
      *t1 = ptr;
      ptr = rr[n+i];
      rr[n+i] = *t2;
      *t2 = ptr;
   }
}

void IFFT_radix2_twiddle(mp_limb_t ** ii, ulong is,
      ulong n, ulong w, mp_limb_t ** t1, mp_limb_t ** t2, mp_limb_t ** temp,
              ulong ws, ulong r, ulong c, ulong rs);

void IFFT_radix2_truncate1(mp_limb_t ** rr, ulong rs, mp_limb_t ** ii, 
      ulong n, ulong w, mp_limb_t ** t1, mp_limb_t ** t2, mp_limb_t ** temp,
      ulong trunc)
{
   mp_limb_t ** ss, ** tt;
   mp_limb_t * ptr;
   ulong i, j, k, l;
   ulong size = (w*n)/GMP_LIMB_BITS + 1;
   
   if (trunc == 2*n)
   {
      IFFT_radix2_new(rr, rs, ii, n, w, t1, t2, temp);
      return;
   }

   if (trunc <= n)
   {
      // PASS
      for (i = trunc; i < n; i++)
      {
         mpn_add_n(ii[i], ii[i], ii[i+n], size);
         mpn_div_2expmod_2expp1(ii[i], ii[i], size - 1, 1);
      }
      
      IFFT_radix2_truncate1(rr, rs, ii, n/2, 2*w, t1, t2, temp, trunc);

      for (i = 0; i < trunc; i++)
         mpn_addsub_n(ii[i], ii[i], ii[i], ii[n+i], size);

      return;
   }

   // [s0, s1, ..., s{m/2-1}] = Fradix2_inverse[i0, i2, ..., i{m-2}]
   IFFT_radix2_new(ii, 1, ii, n/2, 2*w, t1, t2, temp);

   for (i = trunc; i < 2*n; i++)
   {
      mpn_sub_n(ii[i], ii[i-n], ii[i], size);
      FFT_twiddle(*t1, ii[i], i - n, n, w);
      ptr = ii[i];
      ii[i] = *t1;
      *t1 = ptr;
   }
   
   for (i = trunc - n; i < n; i++)
   {
       FFT_twiddle(*t1, ii[i + n], 2*n - i, n, w);
       mpn_add_n(ii[i], ii[i], *t1, size);
   }

   // [t{m/2}, t{m/2+1}, ..., t{m-1}] = Fradix2_inverse[i1, i3, ..., i{m-1}]
   IFFT_radix2_truncate1(ii+n, 1, ii+n, n/2, 2*w, t1, t2, temp, trunc - n);

   // [r0, r1, ..., r{m/2-1}] 
   // = [s0+z1^0*t0, s1+z1^1*t1, ..., s{m/2-1}+z1^{m/2-1}*t{m-1}]
   // [r{m/2}, t{m/2+1}, ..., r{m-1}] 
   // = [s0-z1^0*t{m/2}, s1-z1^1*t{m/2+1}, ..., s{m/2-1}-z1^{m/2-1}*t{m-1}]
   // where z1 = exp(-2*Pi*I/m), z1 => w bits
   for (i = 0; i < trunc - n; i++) 
   {   
      FFT_radix2_inverse_butterfly(*t1, *t2, ii[i], ii[n+i], i, n, w);
   
      ptr = rr[i];
      rr[i] = *t1;
      *t1 = ptr;
      ptr = rr[n+i];
      rr[n+i] = *t2;
      *t2 = ptr;
   }
}

void IFFT_radix2_truncate1_twiddle(mp_limb_t ** ii, ulong is,
      ulong n, ulong w, mp_limb_t ** t1, mp_limb_t ** t2, mp_limb_t ** temp,
      ulong ws, ulong r, ulong c, ulong rs, ulong trunc)
{
   mp_limb_t ** ss, ** tt;
   mp_limb_t * ptr;
   ulong i, j, k, l;
   ulong size = (w*n)/GMP_LIMB_BITS + 1;
   
   if (trunc == 2*n)
   {
      IFFT_radix2_twiddle(ii, is, n, w, t1, t2, temp, ws, r, c, rs);
      return;
   }

   if (trunc <= n)
   {
      // PASS
      for (i = trunc; i < n; i++)
      {
         mpn_add_n(ii[i*is], ii[i*is], ii[(i+n)*is], size);
         mpn_div_2expmod_2expp1(ii[i*is], ii[i*is], size - 1, 1);
      }
      
      IFFT_radix2_truncate1_twiddle(ii, is, n/2, 2*w, t1, t2, temp, ws, r, c, 2*rs, trunc);

      for (i = 0; i < trunc; i++)
         mpn_addsub_n(ii[i*is], ii[i*is], ii[i*is], ii[(n+i)*is], size);

      return;
   }

   // [s0, s1, ..., s{m/2-1}] = Fradix2_inverse[i0, i2, ..., i{m-2}]
   IFFT_radix2_twiddle(ii, is, n/2, 2*w, t1, t2, temp, ws, r, c, 2*rs);

   for (i = trunc; i < 2*n; i++)
   {
      mpn_sub_n(ii[i*is], ii[(i-n)*is], ii[i*is], size);
      FFT_twiddle(*t1, ii[i*is], i - n, n, w);
      ptr = ii[i*is];
      ii[i*is] = *t1;
      *t1 = ptr;
   }
   
   for (i = trunc - n; i < n; i++)
   {
       FFT_twiddle(*t1, ii[(i+n)*is], 2*n - i, n, w);
       mpn_add_n(ii[i*is], ii[i*is], *t1, size);
   }

   // [t{m/2}, t{m/2+1}, ..., t{m-1}] = Fradix2_inverse[i1, i3, ..., i{m-1}]
   IFFT_radix2_truncate1_twiddle(ii + n*is, is, n/2, 2*w, t1, t2, temp, ws, r + rs, c, 2*rs, trunc - n);

   // [r0, r1, ..., r{m/2-1}] 
   // = [s0+z1^0*t0, s1+z1^1*t1, ..., s{m/2-1}+z1^{m/2-1}*t{m-1}]
   // [r{m/2}, t{m/2+1}, ..., r{m-1}] 
   // = [s0-z1^0*t{m/2}, s1-z1^1*t{m/2+1}, ..., s{m/2-1}-z1^{m/2-1}*t{m-1}]
   // where z1 = exp(-2*Pi*I/m), z1 => w bits
   for (i = 0; i < trunc - n; i++) 
   {   
      FFT_radix2_inverse_butterfly(*t1, *t2, ii[i*is], ii[(n+i)*is], i, n, w);
   
      ptr = ii[i*is];
      ii[i*is] = *t1;
      *t1 = ptr;
      ptr = ii[(n+i)*is];
      ii[(n+i)*is] = *t2;
      *t2 = ptr;
   }
}

/* 
   Truncate IFFT to given length. Requires trunc a multiple of 8.
   Assumes (conceptually) zeroes from trunc to n.
*/
void IFFT_radix2_truncate(mp_limb_t ** rr, ulong rs, mp_limb_t ** ii, 
      ulong n, ulong w, mp_limb_t ** t1, mp_limb_t ** t2, mp_limb_t ** temp,
      ulong trunc)
{
   mp_limb_t ** ss, ** tt;
   mp_limb_t * ptr;
   ulong i, j, k, l;
   ulong size = (w*n)/GMP_LIMB_BITS + 1;
   
   if (trunc == 2*n)
   {
      IFFT_radix2_new(rr, rs, ii, n, w, t1, t2, temp);
      return;
   }

   if (trunc <= n)
   {
      // PASS
      IFFT_radix2_truncate(rr, rs, ii, n/2, 2*w, t1, t2, temp, trunc);

      for (i = 0; i < trunc; i++)
         mpn_add_n(ii[i], ii[i], ii[i], size);

      return;
   }

   //PASS
   // [s0, s1, ..., s{m/2-1}] = Fradix2_inverse[i0, i2, ..., i{m-2}]
   IFFT_radix2_new(ii, 1, ii, n/2, 2*w, t1, t2, temp);

   for (i = trunc; i < 2*n; i++)
   {
      FFT_twiddle(ii[i], ii[i-n], i - n, n, w);
   }
   
   // [t{m/2}, t{m/2+1}, ..., t{m-1}] = Fradix2_inverse[i1, i3, ..., i{m-1}]
   IFFT_radix2_truncate1(ii+n, 1, ii+n, n/2, 2*w, t1, t2, temp, trunc - n);

   // [r0, r1, ..., r{m/2-1}] 
   // = [s0+z1^0*t0, s1+z1^1*t1, ..., s{m/2-1}+z1^{m/2-1}*t{m-1}]
   // [r{m/2}, t{m/2+1}, ..., r{m-1}] 
   // = [s0-z1^0*t{m/2}, s1-z1^1*t{m/2+1}, ..., s{m/2-1}-z1^{m/2-1}*t{m-1}]
   // where z1 = exp(-2*Pi*I/m), z1 => w bits
   for (i = 0; i < trunc - n; i++) 
   {   
      FFT_radix2_inverse_butterfly(*t1, *t2, ii[i], ii[n+i], i, n, w);
   
      ptr = rr[i];
      rr[i] = *t1;
      *t1 = ptr;
      ptr = rr[n+i];
      rr[n+i] = *t2;
      *t2 = ptr;
   }

   for (i = trunc - n; i < n; i++)
         mpn_add_n(ii[i], ii[i], ii[i], size);
}

void IFFT_radix2_truncate_twiddle(mp_limb_t ** ii, ulong is,
      ulong n, ulong w, mp_limb_t ** t1, mp_limb_t ** t2, mp_limb_t ** temp,
      ulong ws, ulong r, ulong c, ulong rs, ulong trunc)
{
   mp_limb_t ** ss, ** tt;
   mp_limb_t * ptr;
   ulong i, j, k, l;
   ulong size = (w*n)/GMP_LIMB_BITS + 1;
   
   if (trunc == 2*n)
   {
      IFFT_radix2_twiddle(ii, is, n, w, t1, t2, temp, ws, r, c, rs);
      return;
   }

   if (trunc <= n)
   {
      // PASS
      IFFT_radix2_truncate_twiddle(ii, is, n/2, 2*w, t1, t2, temp, ws, r, c, 2*rs, trunc);

      for (i = 0; i < trunc; i++)
         mpn_add_n(ii[i*is], ii[i*is], ii[i*is], size);

      return;
   }

   //PASS
   // [s0, s1, ..., s{m/2-1}] = Fradix2_inverse[i0, i2, ..., i{m-2}]
   IFFT_radix2_twiddle(ii, is, n/2, 2*w, t1, t2, temp, ws, r, c, 2*rs);

   for (i = trunc; i < 2*n; i++)
   {
      FFT_twiddle(ii[i*is], ii[(i-n)*is], i - n, n, w);
   }
   
   // [t{m/2}, t{m/2+1}, ..., t{m-1}] = Fradix2_inverse[i1, i3, ..., i{m-1}]
   IFFT_radix2_truncate1_twiddle(ii+n*is, is, n/2, 2*w, t1, t2, temp, ws, r + rs, c, 2*rs, trunc - n);

   // [r0, r1, ..., r{m/2-1}] 
   // = [s0+z1^0*t0, s1+z1^1*t1, ..., s{m/2-1}+z1^{m/2-1}*t{m-1}]
   // [r{m/2}, t{m/2+1}, ..., r{m-1}] 
   // = [s0-z1^0*t{m/2}, s1-z1^1*t{m/2+1}, ..., s{m/2-1}-z1^{m/2-1}*t{m-1}]
   // where z1 = exp(-2*Pi*I/m), z1 => w bits
   for (i = 0; i < trunc - n; i++) 
   {   
      FFT_radix2_inverse_butterfly(*t1, *t2, ii[i*is], ii[(n+i)*is], i, n, w);
   
      ptr = ii[i*is];
      ii[i*is] = *t1;
      *t1 = ptr;
      ptr = ii[(n+i)*is];
      ii[(n+i)*is] = *t2;
      *t2 = ptr;
   }

   for (i = trunc - n; i < n; i++)
         mpn_add_n(ii[i*is], ii[i*is], ii[i*is], size);
}

void IFFT_radix2_negacyclic(mp_limb_t ** rr, ulong rs, mp_limb_t ** ii, 
      ulong n, ulong w, mp_limb_t ** t1, mp_limb_t ** t2, mp_limb_t ** temp)
{
   mp_limb_t ** ss, ** tt;
   mp_limb_t * ptr;
   ulong i, j, k, l;
   ulong size = (w*n)/GMP_LIMB_BITS + 1;

   j = 0; k = 0;
   for (i = 0; i < n; i += 2)
   {
      ii[i] = ii[2*i];
      temp[k++] = ii[i+1];
      ii[i+1] = ii[2*(i+1)];
   }
   for ( ; i < 2*n; i+=2)
   {
      ii[i] = temp[j++];
      temp[k++] = ii[i+1];
      ii[i+1] = temp[j++];
   }

   // [s0, s1, ..., s{m/2-1}] = Fradix2_inverse[i0, i2, ..., i{m-2}]
   IFFT_radix2_new(ii, 1, ii, n/2, 2*w, t1, t2, temp);
   
   // [t{m/2}, t{m/2+1}, ..., t{m-1}] = Fradix2_inverse[i1, i3, ..., i{m-1}]
   IFFT_radix2_new(ii+n, 1, ii+n, n/2, 2*w, t1, t2, temp);

   // [r0, r1, ..., r{m/2-1}] 
   // = [s0+z1^0*t0, s1+z1^1*t1, ..., s{m/2-1}+z1^{m/2-1}*t{m-1}]
   // [r{m/2}, t{m/2+1}, ..., r{m-1}] 
   // = [s0-z1^0*t{m/2}, s1-z1^1*t{m/2+1}, ..., s{m/2-1}-z1^{m/2-1}*t{m-1}]
   // where z1 = exp(-2*Pi*I/m), z1 => w bits
   for (i = 0; i < n; i++) 
   {   
      FFT_radix2_inverse_butterfly(*t1, *t2, ii[i], ii[n+i], i, n, w);
   
      ptr = rr[i];
      rr[i] = *t1;
      *t1 = ptr;
      ptr = rr[n+i];
      rr[n+i] = *t2;
      *t2 = ptr;
   
      // we finally apply twiddle factors corresponding to shifts of w*i bits
      // this could partially be combined with the butterflies above

      if (FFT_negacyclic_twiddle(*t1, ii[i], 4*n - i, n, w))
      {
         ptr = ii[i];
         ii[i] = *t1;
         *t1 = ptr;
      }

      if (FFT_negacyclic_twiddle(*t1, ii[n + i], 3*n - i, n, w))
      {
         ptr = ii[n + i];
         ii[n + i] = *t1;
         *t1 = ptr;
      }
   }
}

void IFFT_radix2_twiddle(mp_limb_t ** ii, ulong is,
      ulong n, ulong w, mp_limb_t ** t1, mp_limb_t ** t2, mp_limb_t ** temp,
              ulong ws, ulong r, ulong c, ulong rs)
{
   mp_limb_t ** ss, ** tt;
   mp_limb_t * ptr;
   ulong i, j, k, l;
   ulong size = (w*n)/GMP_LIMB_BITS + 1;
   
   if (n == 1) 
   {
      ulong tw1, tw2;
      tw1 = r*c;
      tw2 = tw1 + rs*c;
      FFT_radix2_twiddle_inverse_butterfly(*t1, *t2, ii[0], ii[is], n*w, tw1*ws, tw2*ws);
      ptr = ii[0];
      ii[0] = *t1;
      *t1 = ptr;
      ptr = ii[is];
      ii[is] = *t2;
      *t2 = ptr;
      return;
   }

   // [s0, s1, ..., s{m/2-1}] = Fradix2_inverse[i0, i2, ..., i{m-2}]
   IFFT_radix2_twiddle(ii, is, n/2, 2*w, t1, t2, temp, ws, r, c, 2*rs);
   
   // [t{m/2}, t{m/2+1}, ..., t{m-1}] = Fradix2_inverse[i1, i3, ..., i{m-1}]
   IFFT_radix2_twiddle(ii+n*is, is, n/2, 2*w, t1, t2, temp, ws, r + rs, c, 2*rs);

   // [r0, r1, ..., r{m/2-1}] 
   // = [s0+z1^0*t0, s1+z1^1*t1, ..., s{m/2-1}+z1^{m/2-1}*t{m-1}]
   // [r{m/2}, t{m/2+1}, ..., r{m-1}] 
   // = [s0-z1^0*t{m/2}, s1-z1^1*t{m/2+1}, ..., s{m/2-1}-z1^{m/2-1}*t{m-1}]
   // where z1 = exp(-2*Pi*I/m), z1 => w bits
   for (i = 0; i < n; i++) 
   {   
      FFT_radix2_inverse_butterfly(*t1, *t2, ii[i*is], ii[(n+i)*is], i, n, w);
   
      ptr = ii[i*is];
      ii[i*is] = *t1;
      *t1 = ptr;
      ptr = ii[(n+i)*is];
      ii[(n+i)*is] = *t2;
      *t2 = ptr;
   }
}

/*
   The matrix Fourier algorithm for a 1D Fourier transform of length m = 2*n,
   works as follows:

   * Split the coefficients into R rows of C columns (here C = n1, R = m/n1 = n2)
   * Perform a length R FFT on each column, i.e. with an input stride of n1
   * Multiply each coefficient by z^{r*c} where z = exp(2*Pi*I/m), note z=>w bits
   * Perform a length C FFT on each row, i.e. with an input stride of 1
*/
void FFT_radix2_mfa(mp_limb_t ** ii, ulong n, ulong w, 
                    mp_limb_t ** t1, mp_limb_t ** t2, mp_limb_t ** temp, ulong n1)
{
   ulong i, j;
   ulong n2 = (2*n)/n1;
   ulong depth = 0;
   ulong depth2 = 0;
   mp_limb_t * ptr;

   while ((1UL<<depth) < n2/2) depth++;
   while ((1UL<<depth2) < n1/2) depth2++;
   
   // n2 rows, n1 cols

   for (i = 0; i < n1; i++)
   {   
      // FFT of length n2 on column i, applying z^{r*i} for rows going up in steps 
      // of 1 starting at row 0, where z => w bits
      
      FFT_radix2_twiddle(ii + i, n1, n2/2, w*n1, t1, t2, temp, w, 0, i, 1);
      for (j = 0; j < n2; j++)
      {
         ulong s = revbin(j, depth);
         if (j < s)
         {
            ptr = ii[i + j*n1];
            ii[i + j*n1] = ii[i + s*n1];
            ii[i + s*n1] = ptr;
         }
      }
   }

   for (i = 0; i < n2; i++)
   {
      FFT_radix2_new(ii + i*n1, 1, ii + i*n1, n1/2, w*n2, t1, t2, temp);
      
      for (j = 0; j < n1; j++)
      {
         ulong s = revbin(j, depth2);
         if (j < s)
         {
            ptr = ii[i*n1 + j];
            ii[i*n1 + j] = ii[i*n1 + s];
            ii[i*n1 + s] = ptr;
         }
      }
   }
}

void FFT_radix2_mfa_truncate(mp_limb_t ** ii, ulong n, ulong w, 
        mp_limb_t ** t1, mp_limb_t ** t2, mp_limb_t ** temp, ulong n1, ulong trunc)
{
   ulong i, j, s;
   ulong n2 = (2*n)/n1;
   ulong limbs = (n*w)/GMP_LIMB_BITS;
   ulong depth = 0;
   ulong depth2 = 0;
   mp_limb_t * ptr;

   while ((1UL<<depth) < n2/2) depth++;
   while ((1UL<<depth2) < n1/2) depth2++;
   
   trunc /= n1;

   // n2 rows, n1 cols

   for (i = 0; i < n1; i++)
   {   
      // FFT of length n2 on column i, applying z^{r*i} for rows going up in steps 
      // of 1 starting at row 0, where z => w bits
      
      FFT_radix2_truncate_twiddle(ii + i, n1, n2/2, w*n1, t1, t2, temp, w, 0, i, 1, trunc);
      for (j = 0; j < n2; j++)
      {
         s = revbin(j, depth);
         if (j < s)
         {
            ptr = ii[i + j*n1];
            ii[i + j*n1] = ii[i + s*n1];
            ii[i + s*n1] = ptr;
         }
      }
   }
   
   for (s = 0; s < trunc; s++)
   {
      i = revbin(s, depth);
      FFT_radix2_new(ii + i*n1, 1, ii + i*n1, n1/2, w*n2, t1, t2, temp);
      
      for (j = 0; j < n1; j++)
         mpn_normmod_2expp1(ii[i*n1 + j], limbs);
   }
}

void IFFT_radix2_mfa(mp_limb_t ** ii, ulong n, ulong w, 
                    mp_limb_t ** t1, mp_limb_t ** t2, mp_limb_t ** temp, ulong n1)
{
   ulong i, j;
   ulong n2 = (2*n)/n1;
   ulong depth = 0;
   ulong depth2 = 0;
   mp_limb_t * ptr;

   while ((1UL<<depth) < n2/2) depth++;
   while ((1UL<<depth2) < n1/2) depth2++;

   // n2 rows, n1 cols

   for (i = 0; i < n2; i++)
   {
      for (j = 0; j < n1; j++)
      {
         ulong s = revbin(j, depth2);
         if (j < s)
         {
            ptr = ii[i*n1 + j];
            ii[i*n1 + j] = ii[i*n1 + s];
            ii[i*n1 + s] = ptr;
         }
      }      
      
      IFFT_radix2_new(ii + i*n1, 1, ii + i*n1, n1/2, w*n2, t1, t2, temp);
   }

   for (i = 0; i < n1; i++)
   {   
      for (j = 0; j < n2; j++)
      {
         ulong s = revbin(j, depth);
         if (j < s)
         {
            ptr = ii[i + j*n1];
            ii[i + j*n1] = ii[i + s*n1];
            ii[i + s*n1] = ptr;
         }
      }
      
      // IFFT of length n2 on column i, applying z^{r*i} for rows going up in steps 
      // of 1 starting at row 0, where z => w bits
      IFFT_radix2_twiddle(ii + i, n1, n2/2, w*n1, t1, t2, temp, w, 0, i, 1);
   }

}

void convolution_mfa_truncate(mp_limb_t ** ii, mp_limb_t ** jj, ulong n, ulong w, 
        mp_limb_t ** t1, mp_limb_t ** t2, mp_limb_t ** temp, 
             ulong n1, ulong trunc, mp_limb_t * tt)
{
   ulong i, j, s;
   ulong n2 = (2*n)/n1;
   ulong limbs = (n*w)/GMP_LIMB_BITS;
   ulong depth = 0;
   ulong depth2 = 0;
   mp_limb_t * ptr, c;

   while ((1UL<<depth) < n2/2) depth++;
   while ((1UL<<depth2) < n1/2) depth2++;
   
   trunc /= n1;

   // n2 rows, n1 cols

   for (i = 0; i < n1; i++)
   {   
      // FFT of length n2 on column i, applying z^{r*i} for rows going up in steps 
      // of 1 starting at row 0, where z => w bits
      
      FFT_radix2_truncate_twiddle(ii + i, n1, n2/2, w*n1, t1, t2, temp, w, 0, i, 1, trunc);
      for (j = 0; j < n2; j++)
      {
         s = revbin(j, depth);
         if (j < s)
         {
            ptr = ii[i + j*n1];
            ii[i + j*n1] = ii[i + s*n1];
            ii[i + s*n1] = ptr;
         }
      }
   }
   
   for (s = 0; s < trunc; s++)
   {
      i = revbin(s, depth);
      FFT_radix2_new(ii + i*n1, 1, ii + i*n1, n1/2, w*n2, t1, t2, temp);
      
      for (j = 0; j < n1; j++)
      {
         mpn_normmod_2expp1(ii[i*n1 + j], limbs);
         c = ii[i*n1 + j][limbs] + 2*jj[i*n1 + j][limbs];
         ii[i*n1 + j][limbs] = mpn_mulmod_2expp1(ii[i*n1 + j], ii[i*n1 + j], jj[i*n1 + j], c, n*w, tt);
      }
      
      IFFT_radix2_new(ii + i*n1, 1, ii + i*n1, n1/2, w*n2, t1, t2, temp);
   }

   for (i = 0; i < n1; i++)
   {   
      for (j = 0; j < n2; j++)
      {
         s = revbin(j, depth);
         if (j < s)
         {
            ptr = ii[i + j*n1];
            ii[i + j*n1] = ii[i + s*n1];
            ii[i + s*n1] = ptr;
         }
      }
      
      // IFFT of length n2 on column i, applying z^{r*i} for rows going up in steps 
      // of 1 starting at row 0, where z => w bits
      IFFT_radix2_truncate_twiddle(ii + i, n1, n2/2, w*n1, t1, t2, temp, w, 0, i, 1, trunc);
      for (j = 0; j < trunc; j++)
      {
         mpn_normmod_2expp1(ii[i + n1*j], limbs);
         mpn_div_2expmod_2expp1(ii[i + n1*j], ii[i + n1*j], limbs, depth + depth2 + 2);
         mpn_normmod_2expp1(ii[i + n1*j], limbs);
      }
   }
}

void IFFT_radix2_mfa_truncate(mp_limb_t ** ii, ulong n, ulong w, 
     mp_limb_t ** t1, mp_limb_t ** t2, mp_limb_t ** temp, ulong n1, ulong trunc)
{
   ulong i, j, s;
   ulong n2 = (2*n)/n1;
   ulong limbs = (n*w)/GMP_LIMB_BITS;
   ulong depth = 0;
   ulong depth2 = 0;
   mp_limb_t * ptr;

   while ((1UL<<depth) < n2/2) depth++;
   while ((1UL<<depth2) < n1/2) depth2++;

   trunc /= n1;

   // n2 rows, n1 cols

   for (s = 0; s < trunc; s++)
   {
      i = revbin(s, depth);
      
      IFFT_radix2_new(ii + i*n1, 1, ii + i*n1, n1/2, w*n2, t1, t2, temp);
   }

   for (i = 0; i < n1; i++)
   {   
      for (j = 0; j < n2; j++)
      {
         s = revbin(j, depth);
         if (j < s)
         {
            ptr = ii[i + j*n1];
            ii[i + j*n1] = ii[i + s*n1];
            ii[i + s*n1] = ptr;
         }
      }
      
      // IFFT of length n2 on column i, applying z^{r*i} for rows going up in steps 
      // of 1 starting at row 0, where z => w bits
      IFFT_radix2_truncate_twiddle(ii + i, n1, n2/2, w*n1, t1, t2, temp, w, 0, i, 1, trunc);
      for (j = 0; j < trunc; j++)
         mpn_normmod_2expp1(ii[i + j], limbs);
   }

}

void FFT_mulmod_2expp1(mp_limb_t * r1, mp_limb_t * i1, mp_limb_t * i2, 
                 ulong r_limbs, ulong depth, ulong w)
{
   ulong n = (1UL<<depth);
   ulong bits1 = (r_limbs*GMP_LIMB_BITS)/(2*n);
   
   ulong limbs = (n*w)/GMP_LIMB_BITS;
   ulong size = limbs + 1;
   ulong i, j;

   mp_limb_t * ptr;
   mp_limb_t ** ii, ** jj, *tt, *t1, *t2, *u1, *u2, **s1, **s2;
   mp_limb_t c;
   
   TMP_DECL;

   TMP_MARK;

   ii = (mp_limb_t **) TMP_BALLOC_LIMBS(2*(n + n*size) + 2*n + 2*size);
   for (i = 0, ptr = (mp_limb_t *) ii + 2*n; i < 2*n; i++, ptr += size) 
   {
      ii[i] = ptr;
   }
   t1 = ptr;
   t2 = t1 + size;
   s1 = (mp_limb_t **) t2 + size;
   
   jj = (mp_limb_t **) TMP_BALLOC_LIMBS(2*(n + n*size) + 2*n + 2*size);
   for (i = 0, ptr = (mp_limb_t *) jj + 2*n; i < 2*n; i++, ptr += size) 
   {
      jj[i] = ptr;
   }
   u1 = ptr;
   u2 = u1 + size;
   s2 = (mp_limb_t **) u2 + size;
   
   tt = (mp_limb_t *) TMP_BALLOC_LIMBS(2*size);

   j = FFT_split_bits(ii, i1, r_limbs, bits1, limbs);
   for ( ; j < 2*n; j++)
      MPN_ZERO(ii[j], limbs + 1);
   
   FFT_radix2_negacyclic(ii, 1, ii, n, w, &t1, &t2, s1);
   for (j = 0; j < 2*n; j++)
      mpn_normmod_2expp1(ii[j], limbs);
   
   j = FFT_split_bits(jj, i2, r_limbs, bits1, limbs);
   for ( ; j < 2*n; j++)
      MPN_ZERO(jj[j], limbs + 1);
   
   FFT_radix2_negacyclic(jj, 1, jj, n, w, &u1, &u2, s2);
   
   for (j = 0; j < 2*n; j++)
   {
      mpn_normmod_2expp1(jj[j], limbs);
      c = ii[j][limbs] + 2*jj[j][limbs];
      ii[j][limbs] = mpn_mulmod_2expp1(ii[j], ii[j], jj[j], c, n*w, tt);
   }
   
   IFFT_radix2_negacyclic(ii, 1, ii, n, w, &t1, &t2, s1);
   
   for (j = 0; j < 2*n; j++)
   {
      mpn_div_2expmod_2expp1(ii[j], ii[j], limbs, depth + 1);
      mpn_normmod_2expp1(ii[j], limbs);
   }
   MPN_ZERO(r1, r_limbs + 1);
   FFT_combine_bits(r1, ii, 2*n - 1, bits1, limbs, r_limbs + 1);
   
   // as the negacyclic convolution has effectively done subtractions
   // some of the coefficients will be negative, so need to subtract p
   // FIXME: the following is not very cache friendly
   ulong b = 0;
   ulong ll = 0;
   mp_limb_t bit;
   ulong limb_add = bits1/GMP_LIMB_BITS;
   ulong bit_add = bits1 - limb_add*GMP_LIMB_BITS;

   for (j = 0; j < 2*n - 1; j++)
   {
      if (b >= GMP_LIMB_BITS)
      {
         b -= GMP_LIMB_BITS;
         ll++;
      }
      bit = (1UL<<b);

      if (ii[j][limbs] || (mp_limb_signed_t) ii[j][limbs - 1] < 0) // coefficient was -ve
      {
         mpn_sub_1(r1 + ll, r1 + ll, r_limbs + 1 - ll, bit);
         mpn_sub_1(r1 + ll + limbs, r1 + ll + limbs, r_limbs + 1 - limbs - ll, bit);
      }

      b += bit_add;
      ll += limb_add;
   }

   // final coefficient wraps around
   ulong l = (bits1 - 1)/GMP_LIMB_BITS + 1;
   ulong shift = l*GMP_LIMB_BITS - bits1;
   mp_limb_t cy = 0;
   if (ii[2*n - 1][limbs] || (mp_limb_signed_t) ii[2*n - 1][limbs - 1] < 0)
   {
      if (ii[2*n - 1][limbs]) abort();
      cy = mpn_sub_1(ii[2*n - 1], ii[2*n - 1], limbs + 1, 1);
      cy += mpn_sub_1(ii[2*n - 1] + limbs, ii[2*n - 1] + limbs, 1, 1);
   }
   mpn_lshift(ii[2*n - 1], ii[2*n - 1], limbs + 1, shift);
   
   
   r1[r_limbs] += mpn_add_n(r1 + r_limbs - l, r1 + r_limbs - l, ii[2*n - 1], l);
   c = mpn_sub_n(r1, r1, ii[2*n - 1] + l, limbs + 1 - l);
   mpn_addmod_2expp1_1(r1 + limbs + 1 - l, r_limbs - limbs - 1 + l, -c + cy);
   mpn_normmod_2expp1(r1, r_limbs);
   
   TMP_FREE;
}

/*
   FIXME: currently doesn't use negacyclic fft as it is too inefficient.
*/
mp_limb_t new_mpn_mulmod_2expp1(mp_limb_t * r, mp_limb_t * i1, mp_limb_t * i2, 
                           mp_limb_t c, mp_limb_t bits, mp_limb_t * tt)
{
   if (1) // currently the negacyclic fft is too inefficient to use.
      return mpn_mulmod_2expp1(r, i1, i2, c, bits, tt);

   ulong w = 2;
   ulong depth = 1;

   ulong n = (1UL<<depth);
   ulong bits1 = (n*w - depth - 2)/2;
   ulong bits2 = 2*n*bits1;

   while (bits2 <= bits && bits % (2*n) == 0) 
   {
      depth++;
      n = (1UL<<depth);
      bits1 = (n*w - depth - 2)/2;
      bits2 = 2*n*bits1;
   }

   depth--;
   n = (1UL<<depth);
   bits1 = (n*w - depth - 2)/2;
   bits2 = 2*n*bits1;

   while (bits2 < bits)
   {
      w += 2;
      bits1 = (n*w - depth - 2)/2;
      bits2 = 2*n*bits1;
   }
   
   FFT_mulmod_2expp1(r, i1, i2, bits/GMP_LIMB_BITS, depth, w);

   return 0;
}

/*
   The main integer multiplication routine. Multiplies i1 of n1 limbs by i2 of
   n2 limbs and puts the result in r1, which must have space for n1 + n2 limbs.
   
   Currently there is no sqrt2 trick, thus a convolution of depth d gives a 
   convolution length 2*n where n = 2^d with coefficients of size n*w, where 
   w = w2^2. 

   Note the *output* polynomial must fit in that convolution size. 
   
   The computation, bits1 = (n*w - depth)/2, gives the maximum size of input 
   coefficients for a given n and w if you want the output to be meaningful.

   The function automatically truncates the FFT, pointwise mults and IFFT, 
   thus the inputs can be different lengths. The function will just segfault
   if n and w2 are not sufficiently large. Except for the smallest multiplications
   (where w2 should be 2), the value of w2 should probably always just be 1.
*/
void new_mpn_mul(mp_limb_t * r1, mp_limb_t * i1, ulong n1, mp_limb_t * i2, ulong n2,
                 ulong depth, ulong w)
{
   ulong n = (1UL<<depth);
   ulong bits1 = (n*w - depth)/2; 
   ulong sqrt = (1UL<<(depth/2));

   ulong r_limbs = n1 + n2;
   ulong j1 = (n1*GMP_LIMB_BITS - 1)/bits1 + 1;
   ulong j2 = (n2*GMP_LIMB_BITS - 1)/bits1 + 1;
   ulong trunc = ((j1 + j2 - 2 + 2*sqrt)/(2*sqrt)) * 2*sqrt;
   
   ulong limbs = (n*w)/GMP_LIMB_BITS;
   ulong size = limbs + 1;
   ulong i, j, s, t, u;

   mp_limb_t * ptr;
   mp_limb_t ** ii, ** jj, *tt, *t1, *t2, *u1, *u2, **s1, **s2;
   mp_limb_t c;
   
   TMP_DECL;

   TMP_MARK;

   ii = (mp_limb_t **) TMP_BALLOC_LIMBS(4*(n + n*size) + 4*n + 6*size);
   for (i = 0, ptr = (mp_limb_t *) ii + 2*n; i < 2*n; i++, ptr += size) 
   {
      ii[i] = ptr;
   }
   t1 = ptr;
   t2 = t1 + size;
   s1 = (mp_limb_t **) t2 + size;
   
   jj = ii + (2*(n + n*size) + 2*n + 2*size);
   for (i = 0, ptr = (mp_limb_t *) jj + 2*n; i < 2*n; i++, ptr += size) 
   {
      jj[i] = ptr;
   }
   u1 = ptr;
   u2 = u1 + size;
   s2 = (mp_limb_t **) u2 + size;
   
   tt = (mp_limb_t *) jj + (2*(n + n*size) + 2*n + 2*size);
            
   j = FFT_split_bits(jj, i2, n2, bits1, limbs);
   for ( ; j < trunc; j++)
      MPN_ZERO(jj[j], limbs + 1);
   FFT_radix2_mfa_truncate(jj, n, w, &u1, &u2, s2, sqrt, trunc);
    
   j = FFT_split_bits(ii, i1, n1, bits1, limbs);
   for ( ; j < trunc; j++)
      MPN_ZERO(ii[j], limbs + 1);
   
   convolution_mfa_truncate(ii, jj, n, w, &t1, &t2, s1, sqrt, trunc, tt);
   
   MPN_ZERO(r1, r_limbs);
   FFT_combine_bits(r1, ii, j1 + j2 - 1, bits1, limbs, r_limbs);
      
   TMP_FREE;
}

/************************************************************************************

   Test code

************************************************************************************/

void mpn_to_mpz(mpz_t m, mp_limb_t * i, ulong limbs)
{
   mp_limb_signed_t hi;
   
   mpz_realloc(m, limbs + 1);
   MPN_COPY(m->_mp_d, i, limbs + 1);
   hi = i[limbs];
   if (hi < 0L)
   {
      mpn_neg_n(m->_mp_d, m->_mp_d, limbs + 1);
      m->_mp_size = limbs + 1;
      while ((m->_mp_size) && (!m->_mp_d[m->_mp_size - 1])) 
         m->_mp_size--;
      m->_mp_size = -m->_mp_size;
   } else
   {
      m->_mp_size = limbs + 1;
      while ((m->_mp_size) && (!m->_mp_d[m->_mp_size - 1])) 
         m->_mp_size--;
   }
}

void ref_norm(mpz_t m, mpz_t p)
{
   mpz_mod(m, m, p);
}

void ref_submod_i(mpz_t m, mpz_t i1, mpz_t i2, mpz_t p, ulong n, ulong w)
{
   mpz_sub(m, i1, i2);
   mpz_mul_2exp(m, m, (n*w)/2);
   mpz_mod(m, m, p);
}

void ref_mul_2expmod(mpz_t m, mpz_t i2, mpz_t p, ulong n, ulong w, ulong d)
{
   mpz_mul_2exp(m, i2, d);
   mpz_mod(m, m, p);
}

void ref_div_2expmod(mpz_t m, mpz_t i2, mpz_t p, ulong n, ulong w, ulong d)
{
   mpz_t temp;
   mpz_init(temp);
   mpz_set_ui(temp, 1);
   mpz_mul_2exp(temp, temp, d);
   mpz_invert(temp, temp, p);
   mpz_mul(m, i2, temp);
   mpz_mod(m, m, p);
   mpz_clear(temp);
}

void ref_lshB_sumdiffmod(mpz_t t, mpz_t u, mpz_t i1, 
                      mpz_t i2, mpz_t p, ulong n, ulong w, ulong x, ulong y)
{
   mpz_add(t, i1, i2);
   mpz_sub(u, i1, i2);
   mpz_mul_2exp(t, t, x*GMP_LIMB_BITS);
   mpz_mul_2exp(u, u, y*GMP_LIMB_BITS);
   mpz_mod(t, t, p);
   mpz_mod(u, u, p);
}

void ref_sumdiff_rshBmod(mpz_t t, mpz_t u, mpz_t i1, 
                      mpz_t i2, mpz_t p, ulong n, ulong w, ulong x, ulong y)
{
   mpz_t mult1, mult2;
   mpz_init(mult1);
   mpz_init(mult2);
   mpz_set_ui(mult1, 1);
   mpz_mul_2exp(mult1, mult1, x*GMP_LIMB_BITS);
   mpz_invert(mult1, mult1, p);
   mpz_set_ui(mult2, 1);
   mpz_mul_2exp(mult2, mult2, y*GMP_LIMB_BITS);
   mpz_invert(mult2, mult2, p);
   mpz_mul(mult1, mult1, i1);
   mpz_mul(mult2, mult2, i2);
   mpz_add(t, mult1, mult2);
   mpz_sub(u, mult1, mult2);
   mpz_mod(t, t, p);
   mpz_mod(u, u, p);
   mpz_clear(mult1);
   mpz_clear(mult2);
}

void ref_FFT_split_radix_butterfly(mpz_t s, mpz_t t, mpz_t p, ulong i, ulong n, ulong w)
{
   mpz_t temp;
   mpz_init(temp);
   mpz_add(temp, s, t);
   mpz_sub(t, s, t);
   mpz_set(s, temp);
   mpz_mul_2exp(s, s, i*w);
   mpz_mul_2exp(t, t, 3*i*w);
   mpz_mod(s, s, p);
   mpz_mod(t, t, p);
   mpz_clear(temp);
}

void set_p(mpz_t p, ulong n, ulong w)
{
   mpz_set_ui(p, 1);
   mpz_mul_2exp(p, p, n*w);
   mpz_add_ui(p, p, 1);
}

void rand_n(mp_limb_t * n, gmp_randstate_t state, ulong limbs)
{
   mpn_rrandom(n, state, limbs);
   n[limbs] = gmp_urandomm_ui(state, 10);
   if (gmp_urandomm_ui(state, 2)) n[limbs] = -n[limbs];
}

void test_norm()
{
   ulong i, j, k, l, n, w, limbs;
   mpz_t p, m, m2;
   mpz_init(p);
   mpz_init(m);
   mpz_init(m2);
   mp_limb_t * nn;
   gmp_randstate_t state;
   gmp_randinit_default(state);
   TMP_DECL;

   for (i = GMP_LIMB_BITS; i < 32*GMP_LIMB_BITS; i += GMP_LIMB_BITS)
   {
      for (j = 1; j < 32; j++)
      {
         for (k = 1; k <= GMP_NUMB_BITS; k <<= 1)
         {
            n = i/k;
            w = j*k;
            limbs = (n*w)/GMP_LIMB_BITS;
            TMP_MARK;
            nn = TMP_BALLOC_LIMBS(limbs + 1);
            mpn_rrandom(nn, state, limbs + 1);
            mpn_to_mpz(m, nn, limbs);
            set_p(p, n, w);
            
            mpn_normmod_2expp1(nn, limbs);
            mpn_to_mpz(m2, nn, limbs);
            ref_norm(m, p);

            if (mpz_cmp(m, m2) != 0)
            {
               printf("mpn_normmod_2expp1 error\n");
               gmp_printf("want %Zx\n\n", m);
               gmp_printf("got  %Zx\n", m2);
               abort();
            }
            TMP_FREE;
         }
      }
   }
   mpz_clear(p);
   mpz_clear(m);
   mpz_clear(m2);
   gmp_randclear(state);
}

void test_submod_i()
{
   ulong i, j, k, l, n, w, limbs;
   mpz_t p, m, m2, mn1, mn2;
   mpz_init(p);
   mpz_init(m);
   mpz_init(m2);
   mpz_init(mn1);
   mpz_init(mn2);
   mp_limb_t * nn1, * nn2, * r;
   gmp_randstate_t state;
   gmp_randinit_default(state);
   TMP_DECL;

   for (i = 2*GMP_LIMB_BITS; i < 64*GMP_LIMB_BITS; i += 2*GMP_LIMB_BITS)
   {
      for (j = 1; j < 32; j++)
      {
         for (k = 1; k <= 2*GMP_NUMB_BITS; k <<= 1)
         {
            n = i/k;
            w = j*k;
            limbs = (n*w)/GMP_LIMB_BITS;
            TMP_MARK;
            nn1 = TMP_BALLOC_LIMBS(limbs + 1);
            nn2 = TMP_BALLOC_LIMBS(limbs + 1);
            r = TMP_BALLOC_LIMBS(limbs + 1);
            rand_n(nn1, state, limbs);
            rand_n(nn2, state, limbs);
            
            mpn_to_mpz(mn1, nn1, limbs);
            mpn_to_mpz(mn2, nn2, limbs);
            set_p(p, n, w);
            
            mpn_submod_i_2expp1(r, nn1, nn2, limbs);
            mpn_to_mpz(m2, r, limbs);
            ref_norm(m2, p);
            ref_submod_i(m, mn1, mn2, p, n, w);

            if (mpz_cmp(m, m2) != 0)
            {
               printf("mpn_submmod_i_2expp1 error\n");
               gmp_printf("want %Zx\n\n", m);
               gmp_printf("got  %Zx\n", m2);
               abort();
            }
            TMP_FREE;
         }
      }
   }
   mpz_clear(p);
   mpz_clear(m);
   mpz_clear(m2);
   mpz_clear(mn1);
   mpz_clear(mn2);
   gmp_randclear(state);
}

void test_mul_2expmod()
{
   ulong i, j, k, l, n, w, limbs, d;
   mpz_t p, m, m2, mn1, mn2;
   mpz_init(p);
   mpz_init(m);
   mpz_init(m2);
   mpz_init(mn1);
   mp_limb_t * nn1, * r;
   gmp_randstate_t state;
   gmp_randinit_default(state);
   TMP_DECL;

   for (i = 2*GMP_LIMB_BITS; i < 64*GMP_LIMB_BITS; i += 2*GMP_LIMB_BITS)
   {
      for (j = 1; j < 32; j++)
      {
         for (k = 1; k <= 2*GMP_NUMB_BITS; k <<= 1)
         {
            for (d = 0; d < GMP_LIMB_BITS; d++)
            {
               n = i/k;
               w = j*k;
               limbs = (n*w)/GMP_LIMB_BITS;
               TMP_MARK;
               nn1 = TMP_BALLOC_LIMBS(limbs + 1);
               r = TMP_BALLOC_LIMBS(limbs + 1);
               rand_n(nn1, state, limbs);
               mpn_to_mpz(mn1, nn1, limbs);
               set_p(p, n, w);
               
               mpn_mul_2expmod_2expp1(r, nn1, limbs, d);
               mpn_to_mpz(m2, r, limbs);
               ref_norm(m2, p);
               ref_mul_2expmod(m, mn1, p, n, w, d);
               
               if (mpz_cmp(m, m2) != 0)
               {
                  printf("mpn_mul_2expmod_2expp1 error\n");
                  gmp_printf("want %Zx\n\n", m);
                  gmp_printf("got  %Zx\n", m2);
                  abort();
               }
               TMP_FREE;
            }
         }
      }
   }
   mpz_clear(p);
   mpz_clear(m);
   mpz_clear(m2);
   mpz_clear(mn1);
   gmp_randclear(state);
}

void test_FFT_negacyclic_twiddle()
{
   ulong i, j, k, l, n, w, limbs, d;
   mpz_t p, m, m2, mn1, mn2;
   mpz_init(p);
   mpz_init(m);
   mpz_init(m2);
   mpz_init(mn1);
   mp_limb_t * nn1, * r;
   gmp_randstate_t state;
   gmp_randinit_default(state);
   TMP_DECL;

   for (i = 2*GMP_LIMB_BITS; i < 20*GMP_LIMB_BITS; i += 2*GMP_LIMB_BITS)
   {
      for (j = 1; j < 10; j++)
      {
         for (k = 1; k <= 2*GMP_NUMB_BITS; k <<= 1)
         {
            n = i/k;
            w = 2*j*k;
            for (d = 0; d < 2*n; d++)
            {
               limbs = (n*w)/GMP_LIMB_BITS;
               TMP_MARK;
               nn1 = TMP_BALLOC_LIMBS(limbs + 1);
               r = TMP_BALLOC_LIMBS(limbs + 1);
               rand_n(nn1, state, limbs);
               mpn_to_mpz(mn1, nn1, limbs);
               set_p(p, n, w);
               
               if (!FFT_negacyclic_twiddle(r, nn1, d, n, w))
                  MPN_COPY(r, nn1, limbs + 1);
               mpn_to_mpz(m2, r, limbs);
               ref_norm(m2, p);
               ref_mul_2expmod(m, mn1, p, n, w, d*w/2);
               
               if (mpz_cmp(m, m2) != 0)
               {
                  printf("FFT_negacyclic_twiddle error\n");
                  gmp_printf("want %Zx\n\n", m);
                  gmp_printf("got  %Zx\n", m2);
                  abort();
               }
               TMP_FREE;
            }
         }
      }
   }

   for (i = 2*GMP_LIMB_BITS; i < 20*GMP_LIMB_BITS; i += 2*GMP_LIMB_BITS)
   {
      for (j = 1; j < 10; j++)
      {
         for (k = 1; k <= 2*GMP_NUMB_BITS; k <<= 1)
         {
            n = i/k;
            w = 2*j*k;
            for (d = 0; d < 2*n; d++)
            {
               limbs = (n*w)/GMP_LIMB_BITS;
               TMP_MARK;
               nn1 = TMP_BALLOC_LIMBS(limbs + 1);
               r = TMP_BALLOC_LIMBS(limbs + 1);
               rand_n(nn1, state, limbs);
               mpn_to_mpz(mn1, nn1, limbs);
               set_p(p, n, w);
               
               if (!FFT_negacyclic_twiddle(r, nn1, 4*n - d, n, w))
                  MPN_COPY(r, nn1, limbs + 1);
               mpn_to_mpz(m2, r, limbs);
               ref_norm(m2, p);
               ref_div_2expmod(m, mn1, p, n, w, d*w/2);
               
               if (mpz_cmp(m, m2) != 0)
               {
                  printf("FFT_negacyclic_twiddle error\n");
                  gmp_printf("want %Zx\n\n", m);
                  gmp_printf("got  %Zx\n", m2);
                  abort();
               }
               TMP_FREE;
            }
         }
      }
   }
   mpz_clear(p);
   mpz_clear(m);
   mpz_clear(m2);
   mpz_clear(mn1);
   gmp_randclear(state);
}

void test_div_2expmod()
{
   ulong i, j, k, l, n, w, limbs, d;
   mpz_t p, m, m2, mn1, mn2;
   mpz_init(p);
   mpz_init(m);
   mpz_init(m2);
   mpz_init(mn1);
   mp_limb_t * nn1, * r;
   gmp_randstate_t state;
   gmp_randinit_default(state);
   TMP_DECL;

   for (i = 2*GMP_LIMB_BITS; i < 64*GMP_LIMB_BITS; i += 2*GMP_LIMB_BITS)
   {
      for (j = 1; j < 32; j++)
      {
         for (k = 1; k <= 2*GMP_NUMB_BITS; k <<= 1)
         {
            for (d = 0; d < GMP_LIMB_BITS; d++)
            {
               n = i/k;
               w = j*k;
               limbs = (n*w)/GMP_LIMB_BITS;
               TMP_MARK;
               nn1 = TMP_BALLOC_LIMBS(limbs + 1);
               r = TMP_BALLOC_LIMBS(limbs + 1);
               rand_n(nn1, state, limbs);
            
               mpn_to_mpz(mn1, nn1, limbs);
               set_p(p, n, w);
            
               mpn_div_2expmod_2expp1(r, nn1, limbs, d);
               mpn_to_mpz(m2, r, limbs);
               ref_norm(m2, p);
               ref_norm(mn1, p);
               ref_mul_2expmod(m, m2, p, n, w, d);

               if (mpz_cmp(m, mn1) != 0)
               {
                  printf("mpn_div_2expmod_2expp1 error\n");
                  gmp_printf("want %Zx\n\n", mn1);
                  gmp_printf("got  %Zx\n", m);
                  abort();
               }
               TMP_FREE;
            }
         }
      }
   }
   mpz_clear(p);
   mpz_clear(m);
   mpz_clear(m2);
   mpz_clear(mn1);
   gmp_randclear(state);
}

void test_lshB_sumdiffmod()
{
   ulong c, i, j, k, l, x, y, n, w, limbs;
   mpz_t p, ma, mb, m2a, m2b, mn1, mn2;
   mpz_init(p);
   mpz_init(ma);
   mpz_init(mb);
   mpz_init(m2a);
   mpz_init(m2b);
   mpz_init(mn1);
   mpz_init(mn2);
   mp_limb_t * nn1, * nn2, * r1, * r2;
   gmp_randstate_t state;
   gmp_randinit_default(state);
   TMP_DECL;

   for (i = 2*GMP_LIMB_BITS; i < 20*GMP_LIMB_BITS; i += 2*GMP_LIMB_BITS)
   {
      for (j = 1; j < 10; j++)
      {
         for (k = 1; k <= 2*GMP_NUMB_BITS; k <<= 1)
         {
            n = i/k;
            w = j*k;
            limbs = (n*w)/GMP_LIMB_BITS;
            for (c = 0; c < limbs; c++)
            {
               x = gmp_urandomm_ui(state, limbs + 1);
               y = gmp_urandomm_ui(state, limbs + 1);
               TMP_MARK;
               nn1 = TMP_BALLOC_LIMBS(limbs + 1);
               nn2 = TMP_BALLOC_LIMBS(limbs + 1);
               r1 = TMP_BALLOC_LIMBS(limbs + 1);
               r2 = TMP_BALLOC_LIMBS(limbs + 1);
               rand_n(nn1, state, limbs);
               rand_n(nn2, state, limbs);
            
               mpn_to_mpz(mn1, nn1, limbs);
               mpn_to_mpz(mn2, nn2, limbs);
               set_p(p, n, w);
            
               mpn_lshB_sumdiffmod_2expp1(r1, r2, nn1, nn2, limbs, x, y);
               mpn_to_mpz(m2a, r1, limbs);
               mpn_to_mpz(m2b, r2, limbs);
               ref_norm(m2a, p);
               ref_norm(m2b, p);
               ref_lshB_sumdiffmod(ma, mb, mn1, mn2, p, n, w, x, y);

               if (mpz_cmp(ma, m2a) != 0)
               {
                  printf("mpn_lshB_sumdiffmod_2expp1 error a\n");
                  printf("x = %ld, y = %ld\n", x, y);
                  gmp_printf("want %Zx\n\n", ma);
                  gmp_printf("got  %Zx\n", m2a);
                  abort();
               }
               if (mpz_cmp(mb, m2b) != 0)
               {
                  printf("mpn_lshB_sumdiffmod_2expp1 error b\n");
                  printf("x = %ld, y = %ld\n", x, y);
                  gmp_printf("want %Zx\n\n", mb);
                  gmp_printf("got  %Zx\n", m2b);
                  abort();
               }
               TMP_FREE;
            }
         }
      }
   }
   mpz_clear(p);
   mpz_clear(ma);
   mpz_clear(mb);
   mpz_clear(m2a);
   mpz_clear(m2b);
   mpz_clear(mn1);
   mpz_clear(mn2);
   gmp_randclear(state);
}

void test_sumdiff_rshBmod()
{
   ulong c, i, j, k, l, x, y, n, w, limbs;
   mpz_t p, ma, mb, m2a, m2b, mn1, mn2;
   mpz_init(p);
   mpz_init(ma);
   mpz_init(mb);
   mpz_init(m2a);
   mpz_init(m2b);
   mpz_init(mn1);
   mpz_init(mn2);
   mp_limb_t * nn1, * nn2, * r1, * r2;
   gmp_randstate_t state;
   gmp_randinit_default(state);
   TMP_DECL;

   for (i = 2*GMP_LIMB_BITS; i < 20*GMP_LIMB_BITS; i += 2*GMP_LIMB_BITS)
   {
      for (j = 1; j < 10; j++)
      {
         for (k = 1; k <= 2*GMP_NUMB_BITS; k <<= 1)
         {
            n = i/k;
            w = j*k;
            limbs = (n*w)/GMP_LIMB_BITS;
            for (c = 0; c < limbs; c++)
            {
               x = gmp_urandomm_ui(state, limbs);
               y = gmp_urandomm_ui(state, limbs);
               TMP_MARK;
               nn1 = TMP_BALLOC_LIMBS(limbs + 1);
               nn2 = TMP_BALLOC_LIMBS(limbs + 1);
               r1 = TMP_BALLOC_LIMBS(limbs + 1);
               r2 = TMP_BALLOC_LIMBS(limbs + 1);
               rand_n(nn1, state, limbs);
               rand_n(nn2, state, limbs);
            
               mpn_to_mpz(mn1, nn1, limbs);
               mpn_to_mpz(mn2, nn2, limbs);
               set_p(p, n, w);
            
               mpn_sumdiff_rshBmod_2expp1(r1, r2, nn1, nn2, limbs, x, y);
               mpn_to_mpz(m2a, r1, limbs);
               mpn_to_mpz(m2b, r2, limbs);
               ref_norm(m2a, p);
               ref_norm(m2b, p);
               ref_sumdiff_rshBmod(ma, mb, mn1, mn2, p, n, w, x, y);

               if (mpz_cmp(ma, m2a) != 0)
               {
                  printf("mpn_sumdiff_rshBmod_2expp1 error a\n");
                  printf("x = %ld, y = %ld, limbs = %ld\n", x, y, limbs);
                  gmp_printf("want %Zx\n\n", ma);
                  gmp_printf("got  %Zx\n", m2a);
                  abort();
               }
               if (mpz_cmp(mb, m2b) != 0)
               {
                  printf("mpn_sumdiff_rshBmod_2expp1 error b\n");
                  printf("x = %ld, y = %ld, limbs = %ld\n", x, y, limbs);
                  gmp_printf("want %Zx\n\n", mb);
                  gmp_printf("got  %Zx\n", m2b);
                  abort();
               }
               TMP_FREE;
            }
         }
      }
   }
   mpz_clear(p);
   mpz_clear(ma);
   mpz_clear(mb);
   mpz_clear(m2a);
   mpz_clear(m2b);
   mpz_clear(mn1);
   mpz_clear(mn2);
   gmp_randclear(state);
}

void test_FFT_split_radix_butterfly()
{
   ulong i, j, k, x, n, w, limbs;
   mpz_t p, ma, mb, m2a, m2b, mn1, mn2;
   mpz_init(p);
   mpz_init(ma);
   mpz_init(mb);
   mpz_init(m2a);
   mpz_init(m2b);
   mpz_init(mn1);
   mpz_init(mn2);
   mp_limb_t * nn1, * nn2, * temp;
   gmp_randstate_t state;
   gmp_randinit_default(state);
   TMP_DECL;

   for (i = 2*GMP_LIMB_BITS; i < 20*GMP_LIMB_BITS; i += 2*GMP_LIMB_BITS)
   {
      for (j = 1; j < 10; j++)
      {
         for (k = 1; k <= 2*GMP_NUMB_BITS; k <<= 1)
         {
            n = i/k;
            w = j*k;
            limbs = (n*w)/GMP_LIMB_BITS;
            for (x = 0; x < n + 1; x++)
            {
               TMP_MARK;
               nn1 = TMP_BALLOC_LIMBS(limbs + 1);
               nn2 = TMP_BALLOC_LIMBS(limbs + 1);
               temp = TMP_BALLOC_LIMBS(2*(limbs + 1));
               rand_n(nn1, state, limbs);
               rand_n(nn2, state, limbs);
            
               mpn_to_mpz(mn1, nn1, limbs);
               mpn_to_mpz(mn2, nn2, limbs);
               set_p(p, n, w);
            
               FFT_split_radix_butterfly(nn1, nn2, x, n, w, temp);
               mpn_to_mpz(m2a, nn1, limbs);
               mpn_to_mpz(m2b, nn2, limbs);
               ref_norm(m2a, p);
               ref_norm(m2b, p);
               ref_FFT_split_radix_butterfly(mn1, mn2, p, x, n, w);

               if (mpz_cmp(m2a, mn1) != 0)
               {
                  printf("FFT_split_radix_butterfly error a\n");
                  printf("x = %ld\n", x);
                  gmp_printf("want %Zx\n\n", mn1);
                  gmp_printf("got  %Zx\n", m2a);
                  abort();
               }
               if (mpz_cmp(m2b, mn2) != 0)
               {
                  printf("FFT_split_radix_butterfly error b\n");
                  printf("x = %ld\n", x);
                  gmp_printf("want %Zx\n\n", mn2);
                  gmp_printf("got  %Zx\n", m2b);
                  abort();
               }
               TMP_FREE;
            }
         }
      }
   }
   mpz_clear(p);
   mpz_clear(ma);
   mpz_clear(mb);
   mpz_clear(m2a);
   mpz_clear(m2b);
   mpz_clear(mn1);
   mpz_clear(mn2);
   gmp_randclear(state);
}

void time_mul_with_negacyclic()
{
   ulong depth = 17UL;
   ulong w2 = 1UL;
   ulong iters = 1;

   ulong n = (1UL<<depth);
   ulong w = w2*w2;
   ulong bits1 = (n*w - depth)/2; 
   ulong bits = n*bits1;
   ulong int_limbs = (bits - 1UL)/GMP_LIMB_BITS + 1;
   
   ulong i, j;
   mp_limb_t *i1, *i2, *r1;
   gmp_randstate_t state;
   gmp_randinit_default(state);

   TMP_DECL;

   TMP_MARK;

   i1 = TMP_BALLOC_LIMBS(4*int_limbs);
   i2 = i1 + int_limbs;
   r1 = i2 + int_limbs;
   
   mpn_urandomb(i1, state, bits);
   mpn_urandomb(i2, state, bits);
  
   for (i = 0; i < iters; i++)
   {
      new_mpn_mul(r1, i1, int_limbs, i2, int_limbs, depth, w2);
   }
      
   TMP_FREE;
   gmp_randclear(state);
}

void test_mulmod()
{
   ulong depth = 7UL; //should be at least 5 (or 4 on 32 bit machine)
   ulong w = 2UL; // should be even
   ulong iters = 1000;

   ulong n = (1UL<<depth);
   
   ulong bits1 = (n*w - depth - 2)/2; // length is 2*n, hence depth + 1, but the
                                      // negacyclic convolution subtracts coefficients
                                      // and signs need to be differentiated
   ulong bits = 2*n*bits1;
   ulong int_limbs = bits/GMP_LIMB_BITS;
   
   ulong i, j;
   mp_limb_t *i1, *i2, *r1, *r2, *tt;
   gmp_randstate_t state;
   gmp_randinit_default(state);

   TMP_DECL;

   TMP_MARK;

   i1 = TMP_BALLOC_LIMBS(6*(int_limbs+1));
   i2 = i1 + int_limbs + 1;
   r1 = i2 + int_limbs + 1;
   r2 = r1 + int_limbs + 1;
   tt = r2 + int_limbs + 1;
   
   for (i = 0; i < iters; i++)
   {
      mpn_rrandom(i1, state, int_limbs);
      i1[int_limbs] = CNST_LIMB(0);
      mpn_rrandom(i2, state, int_limbs);
      i2[int_limbs] = CNST_LIMB(0);
      mpn_mulmod_2expp1(r2, i1, i2, 0, bits, tt);
      FFT_mulmod_2expp1(r1, i1, i2, int_limbs, depth, w);
      
      ulong wrong = 0;
      for (j = 0; j < int_limbs; j++)
      {
         if (r1[j] != r2[j]) 
         {
            if (wrong < 10) 
               printf("error in limb %ld, %lx != %lx\n", j, r1[j], r2[j]);
            wrong++;
         } 
      }
      if (wrong) printf("%ld limbs wrong\n", wrong);
   }
      
   TMP_FREE;
   gmp_randclear(state);
}

void test_fft_ifft()
{
   ulong depth = 10UL;
   ulong n = (1UL<<depth);
   ulong w = 1;
   ulong limbs = (n*w)/GMP_LIMB_BITS;
   ulong size = limbs + 1;
   ulong i, j, s;
   mp_limb_t * ptr;
   mp_limb_t ** ii, ** jj, ** kk, *tt, *t1, *t2, *u1, *u2, *v1, *v2, **s1, **s2, **s3;
   mp_limb_t c;
   gmp_randstate_t state;
   gmp_randinit_default(state);

   TMP_DECL;

   TMP_MARK;

   ii = (mp_limb_t **) TMP_BALLOC_LIMBS(2*(n + n*size) + 2*n + 2*size);
   for (i = 0, ptr = (mp_limb_t *) ii + 2*n; i < 2*n; i++, ptr += size) 
   {
      ii[i] = ptr;
      rand_n(ii[i], state, limbs);
   }
   t1 = ptr;
   t2 = t1 + size;
   s1 = (mp_limb_t **) t2 + size;

   for (j = 0; j < 2*n; j++)
      mpn_normmod_2expp1(ii[j], limbs);

   
   jj = (mp_limb_t **) TMP_BALLOC_LIMBS(2*(n + n*size) + 2*n + 2*size);
   for (i = 0, ptr = (mp_limb_t *) jj + 2*n; i < 2*n; i++, ptr += size) 
   {
      jj[i] = ptr;
      MPN_COPY(jj[i], ii[i], limbs + 1);
   }
   u1 = ptr;
   u2 = u1 + size;
   s2 = (mp_limb_t **) u2 + size;
   
   kk = (mp_limb_t **) TMP_BALLOC_LIMBS(2*(n + n*size) + 2*n + 2*size);
   for (i = 0, ptr = (mp_limb_t *) kk + 2*n; i < 2*n; i++, ptr += size) 
   {
      kk[i] = ptr;
   }
   v1 = ptr;
   v2 = v1 + size;
   s3 = (mp_limb_t **) v2 + size;
   
   tt = (mp_limb_t *) TMP_BALLOC_LIMBS(2*size);
   
   for (i = 0; i < 1; i++)
   {
      //FFT_split_radix(ii, 1, ii, n, w, &t1, &t2, s1);
      FFT_radix2_new(ii, 1, ii, n, w, &t1, &t2, s1);
      for (j = 0; j < 2*n; j++)
         mpn_normmod_2expp1(ii[j], limbs);
      for (j = 0; j < 2*n; j++)
      {
         s = revbin(j, depth);
         MPN_COPY(kk[j], ii[j], limbs + 1);
      }
      IFFT_radix2_new(kk, 1, kk, n, w, &v1, &v2, s3);
      for (j = 0; j < 2*n; j++)
      {
         mpn_mul_2expmod_2expp1(jj[j], jj[j], limbs, depth + 1);
         mpn_normmod_2expp1(jj[j], limbs);
         mpn_normmod_2expp1(kk[j], limbs);
      }

      for (j = 0; j < 2*n; j++)
      {
         if (mpn_cmp(kk[j], jj[j], limbs + 1) != 0)
         {
            printf("Error in entry %ld\n", j);
            abort();
         }
      }
   }
      
   TMP_FREE;
   gmp_randclear(state);
}

void test_fft_ifft_truncate()
{
   ulong depth = 10UL;
   ulong n = (1UL<<depth);            
   ulong w = 1;
   ulong iter = 1000;

   ulong limbs = (n*w)/GMP_LIMB_BITS;
   ulong size = limbs + 1;
   ulong i, j, s, count;
   mp_limb_t * ptr;
   mp_limb_t ** ii, ** jj, ** kk, *tt, *t1, *t2, *u1, *u2, *v1, *v2, **s1, **s2, **s3;
   mp_limb_t c;
   gmp_randstate_t state;
   gmp_randinit_default(state);

   TMP_DECL;
 
   TMP_MARK;

   ii = (mp_limb_t **) TMP_BALLOC_LIMBS(2*n + 2*n*size + 2*n + 2*size);
   for (i = 0, ptr = (mp_limb_t *) ii + 2*n; i < 2*n; i++, ptr += size) 
   {
      ii[i] = ptr;
      rand_n(ii[i], state, limbs);
   }
   t1 = (mp_limb_t *) ii + 2*n + 2*n*size;
   t2 = t1 + size;
   s1 = (mp_limb_t **) t2 + size;

   for (j = 0; j < 2*n; j++)
      mpn_normmod_2expp1(ii[j], limbs);

   jj = (mp_limb_t **) TMP_BALLOC_LIMBS(2*n + 2*n*size + 2*n + 2*size);
   for (i = 0, ptr = (mp_limb_t *) jj + 2*n; i < 2*n; i++, ptr += size) 
   {
      jj[i] = ptr;
      MPN_COPY(jj[i], ii[i], limbs + 1);
   }
   u1 = (mp_limb_t *) jj + 2*n + 2*n*size;
   u2 = u1 + size;
   s2 = (mp_limb_t **) u2 + size;
   
   kk = (mp_limb_t **) TMP_BALLOC_LIMBS(2*n + 2*n*size + 2*n + 2*size);
   for (i = 0, j = 0, ptr = (mp_limb_t *) kk + 2*n; i < 2*n; i++, ptr += size) 
   {
      kk[i] = ptr;
   }
   v1 = (mp_limb_t *) kk + 2*n + 2*n*size;
   v2 = v1 + size;
   s3 = (mp_limb_t **) v2 + size;
   
   tt = (mp_limb_t *) TMP_BALLOC_LIMBS(2*size);
   
   for (count = 0; count < iter; count++)
   {
      for (i = 0; i < 2*n; i++) 
      {
         rand_n(ii[i], state, limbs);
      }
   
      for (j = 0; j < 2*n; j++)
         mpn_normmod_2expp1(ii[j], limbs);

      for (i = 0; i < 2*n; i++) 
      {
         MPN_COPY(jj[i], ii[i], limbs + 1);
      }
      
      ulong trunc = gmp_urandomm_ui(state, 2*n) + 1;
      trunc = ((trunc + 1)/2)*2;
   
      FFT_radix2_truncate(ii, 1, ii, n, w, &t1, &t2, s1, trunc);
      for (j = 0; j < trunc; j++)
      {
         mpn_normmod_2expp1(ii[j], limbs);
         MPN_COPY(kk[j], ii[j], limbs + 1);
      }
      
      IFFT_radix2_truncate(kk, 1, kk, n, w, &v1, &v2, s3, trunc);
      for (j = 0; j < trunc; j++)
      {
         mpn_mul_2expmod_2expp1(jj[j], jj[j], limbs, depth + 1);
         mpn_normmod_2expp1(jj[j], limbs);
         mpn_normmod_2expp1(kk[j], limbs);
         if (mpn_cmp(kk[j], jj[j], limbs + 1) != 0)
         {
            gmp_printf("Error in entry %ld, %Nx != %Nx\n", j, kk[j], limbs + 1, jj[j], limbs + 1);
            abort();
         }
      }
   }

   TMP_FREE;

   gmp_randclear(state);
}

void test_fft_ifft_mfa()
{
   ulong depth = 12UL;
   ulong n = (1UL<<depth);
   ulong w = 1;
   ulong sqrt = (1UL<<(depth/2));
   ulong limbs = (n*w)/GMP_LIMB_BITS;
   ulong size = limbs + 1;
   ulong i, j, s;
   mp_limb_t * ptr;
   mp_limb_t ** ii, ** jj, ** kk, *tt, *t1, *t2, *u1, *u2, *v1, *v2, **s1, **s2, **s3;
   mp_limb_t c;
   gmp_randstate_t state;
   gmp_randinit_default(state);

   TMP_DECL;

   TMP_MARK;

   ii = (mp_limb_t **) TMP_BALLOC_LIMBS(2*(n + n*size) + 2*n + 2*size);
   for (i = 0, ptr = (mp_limb_t *) ii + 2*n; i < 2*n; i++, ptr += size) 
   {
      ii[i] = ptr;
      rand_n(ii[i], state, limbs);
   }
   t1 = ptr;
   t2 = t1 + size;
   s1 = (mp_limb_t **) t2 + size;

   for (j = 0; j < 2*n; j++)
      mpn_normmod_2expp1(ii[j], limbs);

   
   jj = (mp_limb_t **) TMP_BALLOC_LIMBS(2*(n + n*size) + 2*n + 2*size);
   for (i = 0, ptr = (mp_limb_t *) jj + 2*n; i < 2*n; i++, ptr += size) 
   {
      jj[i] = ptr;
      MPN_COPY(jj[i], ii[i], limbs + 1);
   }
   u1 = ptr;
   u2 = u1 + size;
   s2 = (mp_limb_t **) u2 + size;
   
   kk = (mp_limb_t **) TMP_BALLOC_LIMBS(2*(n + n*size) + 2*n + 2*size);
   for (i = 0, ptr = (mp_limb_t *) kk + 2*n; i < 2*n; i++, ptr += size) 
   {
      kk[i] = ptr;
   }
   v1 = ptr;
   v2 = v1 + size;
   s3 = (mp_limb_t **) v2 + size;
   
   tt = (mp_limb_t *) TMP_BALLOC_LIMBS(2*size);
   
   for (i = 0; i < 1; i++)
   {
      FFT_radix2_mfa(ii, n, w, &t1, &t2, s1, sqrt);
      for (j = 0; j < 2*n; j++)
         mpn_normmod_2expp1(ii[j], limbs);
      for (j = 0; j < 2*n; j++)
      {
         MPN_COPY(kk[j], ii[j], limbs + 1);
      }
      IFFT_radix2_mfa(kk, n, w, &v1, &v2, s3, sqrt);
      for (j = 0; j < 2*n; j++)
      {
         mpn_mul_2expmod_2expp1(jj[j], jj[j], limbs, depth + 1);
         mpn_normmod_2expp1(jj[j], limbs);
         mpn_normmod_2expp1(kk[j], limbs);
      }

      for (j = 0; j < 2*n; j++)
      {
         if (mpn_cmp(kk[j], jj[j], limbs + 1) != 0)
         {
            gmp_printf("Error in entry %ld, %Nx != %Nx\n", j, kk[j], limbs + 1, jj[j], limbs + 1);
            abort();
         }
      }
   }
      
   TMP_FREE;
   gmp_randclear(state);
}

void test_fft_ifft_mfa_truncate()
{
   ulong depth = 12UL;
   ulong n = (1UL<<depth);
   ulong w = 1;
   ulong iters = 100;
   ulong sqrt = (1UL<<(depth/2));
   ulong limbs = (n*w)/GMP_LIMB_BITS;
   ulong size = limbs + 1;
   ulong i, j, s, count, trunc;
   mp_limb_t * ptr;
   mp_limb_t ** ii, ** jj, ** kk, *tt, *t1, *t2, *u1, *u2, *v1, *v2, **s1, **s2, **s3;
   mp_limb_t c;
   gmp_randstate_t state;
   gmp_randinit_default(state);

   TMP_DECL;

   TMP_MARK;

   ii = (mp_limb_t **) TMP_BALLOC_LIMBS(2*(n + n*size) + 2*n + 2*size);
   for (i = 0, ptr = (mp_limb_t *) ii + 2*n; i < 2*n; i++, ptr += size) 
   {
      ii[i] = ptr;
      rand_n(ii[i], state, limbs);
   }
   t1 = ptr;
   t2 = t1 + size;
   s1 = (mp_limb_t **) t2 + size;

   for (j = 0; j < 2*n; j++)
      mpn_normmod_2expp1(ii[j], limbs);

   
   jj = (mp_limb_t **) TMP_BALLOC_LIMBS(2*(n + n*size) + 2*n + 2*size);
   for (i = 0, ptr = (mp_limb_t *) jj + 2*n; i < 2*n; i++, ptr += size) 
   {
      jj[i] = ptr;
      MPN_COPY(jj[i], ii[i], limbs + 1);
   }
   u1 = ptr;
   u2 = u1 + size;
   s2 = (mp_limb_t **) u2 + size;
   
   kk = (mp_limb_t **) TMP_BALLOC_LIMBS(2*(n + n*size) + 2*n + 2*size);
   for (i = 0, ptr = (mp_limb_t *) kk + 2*n; i < 2*n; i++, ptr += size) 
   {
      kk[i] = ptr;
   }
   v1 = ptr;
   v2 = v1 + size;
   s3 = (mp_limb_t **) v2 + size;
   
   tt = (mp_limb_t *) TMP_BALLOC_LIMBS(2*size);
   
   for (count = 0; count < iters; count++)
   {
      ulong trunc = (gmp_urandomm_ui(state, (n/sqrt)/2) + 1)*sqrt*2;
      for (i = 0; i < 2*n; i++) 
      {
         rand_n(ii[i], state, limbs);
      }
      
      for (j = 0; j < 2*n; j++)
         mpn_normmod_2expp1(ii[j], limbs);

      for (i = 0; i < 2*n; i++) 
      {
         MPN_COPY(jj[i], ii[i], limbs + 1);
      }

      FFT_radix2_mfa_truncate(ii, n, w, &t1, &t2, s1, sqrt, trunc);
      
      for (j = 0; j < 2*n; j++)
         mpn_normmod_2expp1(ii[j], limbs);
      for (j = 0; j < 2*n; j++)
      {
         MPN_COPY(kk[j], ii[j], limbs + 1);
      }
      
      IFFT_radix2_mfa_truncate(kk, n, w, &v1, &v2, s3, sqrt, trunc);
      
      for (j = 0; j < 2*n; j++)
      {
         mpn_mul_2expmod_2expp1(jj[j], jj[j], limbs, depth + 1);
         mpn_normmod_2expp1(jj[j], limbs);
         mpn_normmod_2expp1(kk[j], limbs);
      }

      for (j = 0; j < trunc; j++)
      {
         if (mpn_cmp(kk[j], jj[j], limbs + 1) != 0)
         {
            gmp_printf("Error in entry %ld, %Nx != %Nx\n", j, kk[j], limbs + 1, jj[j], limbs + 1);
            abort();
         }
      }
   }
      
   TMP_FREE;
   gmp_randclear(state);
}

void test_fft_truncate()
{
   ulong depth = 10UL;
   ulong n = (1UL<<depth);            
   ulong w = 1;
   ulong iter = 1000;

   ulong limbs = (n*w)/GMP_LIMB_BITS;
   ulong size = limbs + 1;
   ulong i, j, s, count;
   mp_limb_t * ptr;
   mp_limb_t ** ii, ** jj, ** kk, *tt, *t1, *t2, *u1, *u2, *v1, *v2, **s1, **s2, **s3;
   mp_limb_t c;
   gmp_randstate_t state;
   gmp_randinit_default(state);

   TMP_DECL;
 
   for (count = 0; count < iter; count++)
   {
      ulong trunc = gmp_urandomm_ui(state, 2*n) + 1;
      trunc = ((trunc + 7)/8)*8;
   
      TMP_MARK;

      ii = (mp_limb_t **) TMP_BALLOC_LIMBS(2*n + 2*n*size + 2*n + 2*size);
      for (i = 0, ptr = (mp_limb_t *) ii + 2*n; i < 2*n; i++, ptr += size) 
      {
         ii[i] = ptr;
         if (i < trunc) rand_n(ii[i], state, limbs);
         else MPN_ZERO(ii[i], limbs + 1);
      }
      t1 = (mp_limb_t *) ii + 2*n + 2*n*size;
      t2 = t1 + size;
      s1 = (mp_limb_t **) t2 + size;

      for (j = 0; j < 2*n; j++)
         mpn_normmod_2expp1(ii[j], limbs);

      jj = (mp_limb_t **) TMP_BALLOC_LIMBS(2*n + 2*n*size + 2*n + 2*size);
      for (i = 0, ptr = (mp_limb_t *) jj + 2*n; i < 2*n; i++, ptr += size) 
      {
         jj[i] = ptr;
         MPN_COPY(jj[i], ii[i], limbs + 1);
      }
      u1 = (mp_limb_t *) jj + 2*n + 2*n*size;
      u2 = u1 + size;
      s2 = (mp_limb_t **) u2 + size;
   
      tt = (mp_limb_t *) TMP_BALLOC_LIMBS(2*size);
   
      for (i = 0; i < 1; i++)
      {
         FFT_radix2_truncate(ii, 1, ii, n, w, &t1, &t2, s1, trunc);
         FFT_radix2_new(jj, 1, jj, n, w, &u1, &u2, s2);
         
         for (j = 0; j < trunc; j++)
         {
            mpn_normmod_2expp1(jj[j], limbs);
            mpn_normmod_2expp1(ii[j], limbs);
            if (mpn_cmp(ii[j], jj[j], limbs + 1) != 0)
            {
               gmp_printf("Error in entry %ld, %Nx != %Nx\n", j, ii[j], limbs + 1, jj[j], limbs + 1);
               abort();
            }
         }
      }
      
      TMP_FREE;
   }

   gmp_randclear(state);
}

void time_mfa()
{
   ulong depth = 12L;
   ulong iters = 1000;
   ulong w2 = 1;

   ulong n = (1UL<<depth)/w2;
   ulong w = w2*w2;
   
   ulong limbs = (n*w)/GMP_LIMB_BITS;
   ulong size = limbs + 1;
   ulong i, j, s;
   mp_limb_t * ptr;
   mp_limb_t ** ii, ** jj, *tt, *t1, *t2, *u1, *u2, **s1, **s2;
   mp_limb_t c;
   gmp_randstate_t state;
   gmp_randinit_default(state);

   TMP_DECL;

   TMP_MARK;

   ii = (mp_limb_t **) TMP_BALLOC_LIMBS(2*(n + n*size) + 2*n + 2*size);
   for (i = 0, ptr = (mp_limb_t *) ii + 2*n; i < 2*n; i++, ptr += size) 
   {
      ii[i] = ptr;
      if (i < n) rand_n(ii[i], state, limbs);
      else MPN_ZERO(ii[i], limbs + 1);
   }
   t1 = ptr;
   t2 = t1 + size;
   s1 = (mp_limb_t **) t2 + size;
   
   for (j = 0; j < 2*n; j++)
      mpn_normmod_2expp1(ii[j], limbs);
     
   tt = (mp_limb_t *) TMP_BALLOC_LIMBS(2*size);
   
   for (i = 0; i < iters; i++)
   {
      FFT_radix2_mfa(ii, n, w, &t1, &t2, s1, (1UL<<(depth/2))/w2);
   }
     
   TMP_FREE;
   gmp_randclear(state);
}

void time_ifft()
{
   ulong depth = 16L;
   ulong iters = 1;
   ulong w = 1;
   
   ulong n = (1UL<<depth);
   ulong limbs = (n*w)/GMP_LIMB_BITS;
   ulong size = limbs + 1;
   ulong i, j, s;
   mp_limb_t * ptr;
   mp_limb_t ** ii, ** jj, *tt, *t1, *t2, *u1, *u2, **s1, **s2;
   mp_limb_t c;
   gmp_randstate_t state;
   gmp_randinit_default(state);

   TMP_DECL;

   TMP_MARK;

   ii = (mp_limb_t **) TMP_BALLOC_LIMBS(2*(n + n*size) + 2*n + 2*size);
   for (i = 0, ptr = (mp_limb_t *) ii + 2*n; i < 2*n; i++, ptr += size) 
   {
      ii[i] = ptr;
      if (i < n) rand_n(ii[i], state, limbs);
      else MPN_ZERO(ii[i], limbs + 1);
   }
   t1 = ptr;
   t2 = t1 + size;
   s1 = (mp_limb_t **) t2 + size;
   
   for (j = 0; j < 2*n; j++)
      mpn_normmod_2expp1(ii[j], limbs);
     
   tt = (mp_limb_t *) TMP_BALLOC_LIMBS(2*size);
   
   for (i = 0; i < iters; i++)
   {
      IFFT_radix2_new(ii, 1, ii, n, w, &t1, &t2, s1);
   }
     
   TMP_FREE;
   gmp_randclear(state);
}

void time_negacyclic_fft()
{
   ulong depth = 10L;
   ulong iters = 10000;
   ulong w = 4;
   
   ulong n = 512;//(1UL<<depth);
   ulong limbs = (n*w)/GMP_LIMB_BITS;
   ulong size = limbs + 1;
   ulong i, j, s;
   mp_limb_t * ptr;
   mp_limb_t ** ii, ** jj, *tt, *t1, *t2, *u1, *u2, **s1, **s2;
   mp_limb_t c;
   gmp_randstate_t state;
   gmp_randinit_default(state);

   TMP_DECL;

   TMP_MARK;

   ii = (mp_limb_t **) TMP_BALLOC_LIMBS(2*(n + n*size) + 2*n + 2*size);
   for (i = 0, ptr = (mp_limb_t *) ii + 2*n; i < 2*n; i++, ptr += size) 
   {
      ii[i] = ptr;
      if (i < n) rand_n(ii[i], state, limbs);
      else MPN_ZERO(ii[i], limbs + 1);
   }
   t1 = ptr;
   t2 = t1 + size;
   s1 = (mp_limb_t **) t2 + size;

   for (j = 0; j < 2*n; j++)
      mpn_normmod_2expp1(ii[j], limbs);
     
   tt = (mp_limb_t *) TMP_BALLOC_LIMBS(2*size);
   
   for (i = 0; i < iters; i++)
   {
      FFT_radix2_negacyclic(ii, 1, ii, n, w, &t1, &t2, s1);
   }
     
   TMP_FREE;
   gmp_randclear(state);
}

void time_imfa()
{
   ulong depth = 16L;
   ulong iters = 1;
   ulong w2 = 1;

   ulong w = w2*w2;
   ulong n = (1UL<<depth)/w2;
   ulong limbs = (n*w)/GMP_LIMB_BITS;
   ulong size = limbs + 1;
   ulong i, j, s;
   mp_limb_t * ptr;
   mp_limb_t ** ii, ** jj, *tt, *t1, *t2, *u1, *u2, **s1, **s2;
   mp_limb_t c;
   gmp_randstate_t state;
   gmp_randinit_default(state);

   TMP_DECL;

   TMP_MARK;

   ii = (mp_limb_t **) TMP_BALLOC_LIMBS(2*(n + n*size) + 2*n + 2*size);
   for (i = 0, ptr = (mp_limb_t *) ii + 2*n; i < 2*n; i++, ptr += size) 
   {
      ii[i] = ptr;
      if (i < n) rand_n(ii[i], state, limbs);
      else MPN_ZERO(ii[i], limbs + 1);
   }
   t1 = ptr;
   t2 = t1 + size;
   s1 = (mp_limb_t **) t2 + size;

   for (j = 0; j < 2*n; j++)
      mpn_normmod_2expp1(ii[j], limbs);
     
   tt = (mp_limb_t *) TMP_BALLOC_LIMBS(2*size);
   
   for (i = 0; i < iters; i++)
   {
      IFFT_radix2_mfa(ii, n, w, &t1, &t2, s1, (1UL<<(depth/2))/w2);
   }
     
   TMP_FREE;
   gmp_randclear(state);
}

void test_mul()
{
   ulong depth = 12UL;
   ulong w = 2UL;
   ulong iters = 1;

   ulong n = (1UL<<depth);
   ulong bits1 = (n*w - depth)/2; 
   ulong bits = (8364032*9)/8;//n*bits1;
   ulong int_limbs = (bits - 1UL)/GMP_LIMB_BITS + 1;
   
   ulong i, j;
   mp_limb_t *i1, *i2, *r1, *r2;
   gmp_randstate_t state;
   gmp_randinit_default(state);

   TMP_DECL;

   TMP_MARK;

   i1 = TMP_BALLOC_LIMBS(6*int_limbs);
   i2 = i1 + int_limbs;
   r1 = i2 + int_limbs;
   r2 = r1 + 2*int_limbs;
   
   for (i = 0; i < iters; i++)
   {
      mpn_urandomb(i1, state, bits);
      mpn_urandomb(i2, state, bits);
  
      mpn_mul_n(r2, i1, i2, int_limbs);
      new_mpn_mul(r1, i1, int_limbs, i2, int_limbs, depth, w);
      
      for (j = 0; j < 2*int_limbs; j++)
      {
         if (r1[j] != r2[j]) 
         {
            printf("error in limb %ld, %lx != %lx\n", j, r1[j], r2[j]);
            abort();
         } 
      }
   }
      
   TMP_FREE;
   gmp_randclear(state);
}

void time_mul()
{
#define NEW_MUL 1
   ulong depth = 16UL;
   ulong w = 1UL;
   ulong iters = 1;

   ulong n = (1UL<<depth);
   ulong bits1 = (n*w - depth)/2; 
   ulong bits = n*bits1;
   printf("bits = %ld\n", bits);
   ulong int_limbs = (bits - 1UL)/GMP_LIMB_BITS + 1;
   
   ulong i, j;
   mp_limb_t *i1, *i2, *r1, *r2;
   gmp_randstate_t state;
   gmp_randinit_default(state);

   TMP_DECL;

   TMP_MARK;

   i1 = TMP_BALLOC_LIMBS(6*int_limbs);
   i2 = i1 + int_limbs;
   r1 = i2 + int_limbs;
   r2 = r1 + 2*int_limbs;
   
   mpn_urandomb(i1, state, bits);
   mpn_urandomb(i2, state, bits);
  
   for (i = 0; i < iters; i++)
   {
#if NEW_MUL
      new_mpn_mul(r1, i1, int_limbs, i2, int_limbs, depth, w);
#else
      mpn_mul_n(r1, i1, i2, int_limbs);
#endif
   }
      
   TMP_FREE;
   gmp_randclear(state);
}

int main(void)
{
   test_norm(); printf("mpn_normmod_2expp1...PASS\n");
   test_submod_i(); printf("mpn_submod_i_2expp1...PASS\n");
   test_mul_2expmod(); printf("mpn_mul_2expmod_2expp1...PASS\n");
   test_div_2expmod(); printf("mpn_div_2expmod_2expp1...PASS\n");
   test_lshB_sumdiffmod(); printf("mpn_lshB_sumdiffmod_2expp1...PASS\n");
   test_sumdiff_rshBmod(); printf("mpn_sumdiff_rshBmod_2expp1...PASS\n");
   test_FFT_split_radix_butterfly(); printf("FFT_split_radix_butterfly...PASS\n");
   
   test_fft_ifft_mfa_truncate(); printf("FFT_IFFT_MFA_TRUNCATE...PASS\n");
   test_FFT_negacyclic_twiddle(); printf("FFT_negacyclic_twiddle...PASS\n");
   test_fft_ifft(); printf("FFT_IFFT...PASS\n");
   test_fft_ifft_mfa(); printf("FFT_IFFT_MFA...PASS\n");
   test_fft_truncate(); printf("FFT_TRUNCATE...PASS\n");
   test_fft_ifft_truncate(); printf("FFT_IFFT_TRUNCATE...PASS\n");
   
   //time_ifft();
   //time_mfa();
   //time_imfa();
   //test_mulmod();
   //test_mul();
   //time_mul_with_negacyclic(); // negacyclic is currently *disabled*
   //time_negacyclic_fft();
   time_mul();
   
   return 0;
}