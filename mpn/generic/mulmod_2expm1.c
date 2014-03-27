/* mpn_mulmod_2expm1 

Copyright 2009 Jason Moxham

This file is part of the MPIR Library.

The MPIR Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation; either version 2.1 of the License, or (at
your option) any later version.

The MPIR Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPIR Library; see the file COPYING.LIB.  If not, write
to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.
*/

#include "mpir.h"
#include "gmp-impl.h"

/* 
   (xp, n) = (yp, n)*(zp, n) % 2^b - 1  

   needs (tp,2n) temp space, everything reduced mod 2^b 
   inputs, outputs not fully reduced

   N.B: 2n is not the same as 2b rounded up to nearest limb!

   NOTE: not reduced fully means the representation is redundant, although
   only 0 has two representations i.e. 0 and 2^b - 1
*/
inline static void
mpn_mulmod_2expm1_basecase (mp_ptr xp, mp_srcptr yp, mp_srcptr zp,
			    mpir_ui b, mp_ptr tp)
{
  mp_size_t n, k;
  mp_limb_t c;

  n = BITS_TO_LIMBS(b);
  k = GMP_NUMB_BITS * n - b;

  ASSERT(n > 0);
  ASSERT_MPN(yp, n);
  ASSERT_MPN(zp, n);
  ASSERT(!MPN_OVERLAP_P (tp, 2 * n, yp, n));
  ASSERT(!MPN_OVERLAP_P (tp, 2 * n, zp, n));
  ASSERT(MPN_SAME_OR_SEPARATE_P (xp, tp, n));
  ASSERT(MPN_SAME_OR_SEPARATE_P (xp, tp + n, n));
  ASSERT(k == 0 || yp[n - 1] >> (GMP_NUMB_BITS - k) == 0);
  ASSERT(k == 0 || zp[n - 1] >> (GMP_NUMB_BITS - k) == 0);
  
  /* 
     as n is small, could use mpn_mul_basecase and save a fn call, 
     but this code is also used for large n when b is odd 
  */
  mpn_mul_n (tp, yp, zp, n);	

  if (k == 0)
    {
      c = mpn_add_n (xp, tp, tp + n, n);
      MPN_INCR_U (xp, n, c);
  
      return;
    }

  c = tp[n - 1];
  tp[n - 1] &= GMP_NUMB_MASK >> k;

#if HAVE_NATIVE_mpn_addlsh_nc
  ASSERT_NOCARRY(mpn_addlsh_nc (xp, tp, tp + n, n, k, c));
#else
  {
    mp_limb_t c1;

    c1 = mpn_lshift (tp + n, tp + n, n, k);
    tp[n] |= c >> (GMP_NUMB_BITS - k);
    c = mpn_add_n (xp, tp, tp + n, n) + c1;
    
    ASSERT (c == 0);
  }
#endif

  c = xp[n - 1] >> (GMP_NUMB_BITS - k);
  xp[n - 1] &= GMP_NUMB_MASK >> k;
  MPN_INCR_U (xp, n, c);

  return;
}

/* 
   Improvements:
   use 2^3n - 1 , 2^4n + 2 + 1 factorizations or others
   shift by byte values, i.e. copy unaligned
   addsublshift or a rshift1_lshift dual_rshift i.e. for both the mods
   separate out the k = 0 case 
   unroll the recursion
   if yp = zp i.e. a square then could do better
   different thresholds depending on how many twos divide b

   tp requires 5(n + lg(b)) space
*/
void
mpn_mulmod_2expm1(mp_ptr xp, mp_ptr yp, mp_ptr zp, mpir_ui b,
		   mp_ptr tp)
{
  mp_size_t h, n, m, k;
  int bor, c, c1, c2;
  mp_ptr S, D, typm, tzpm, typp, tzpp, temp;
  mp_limb_t car, Dm, car1;

  /* 
     want y*z %(2^n - 1)         
     want y*z%(2^(2m) - 1) = y*z%((2^m - 1)(2^m + 1)) -- use CRT 
     
     note (2^m - 1)*2^(m - 1) == 1 mod 2^m+1        
     note (2^m + 1)*2^(m - 1) == 1 mod 2^m-1
     
     let A = y*z%(2^m - 1)  B = y*z%(2^m + 1)        
     then A(2^m + 1)2^(m - 1) + B(2^m - 1)2^(m-1) = ((A - B) + (A + B)2^m)2^(m-1)
  */
  
  ASSERT (b > 0);
  
  n = BITS_TO_LIMBS (b);
  
  if (b % 2 == 1 || BELOW_THRESHOLD (n, MULMOD_2EXPM1_THRESHOLD))
    {
      mpn_mulmod_2expm1_basecase (xp, yp, zp, b, tp);
      return;
    }
  
  h = b / 2;
  m = BITS_TO_LIMBS (h);
  k = GMP_NUMB_BITS * m - h;	// if k==0 then n=2*m
  
  ASSERT_MPN(yp, n);
  ASSERT_MPN(zp, n);
  ASSERT(GMP_NUMB_BITS * n - b == 0
	  || yp[n - 1] >> (GMP_NUMB_BITS - (GMP_NUMB_BITS * n - b)) == 0);
  ASSERT(GMP_NUMB_BITS * n - b == 0
	  || zp[n - 1] >> (GMP_NUMB_BITS - (GMP_NUMB_BITS * n - b)) == 0);
  ASSERT(!MPN_OVERLAP_P (yp, n, tp, 5 * n));
  ASSERT(!MPN_OVERLAP_P (zp, n, tp, 5 * n));
  ASSERT(!MPN_OVERLAP_P (xp, n, tp, 5 * n));
  ASSERT(!MPN_OVERLAP_P (xp, n, yp, n));
  ASSERT(!MPN_OVERLAP_P (xp, n, zp, n));

  /* S, D, typm, tzpm, typp, tzpp all require m limbs */
  S = xp;
  D = tp;
  typm = tp + m;
  typp = typm + m;
  tzpm = typp + m;
  tzpp = tzpm + m;
  temp = tzpp + m;

  if (k == 0)
    D = xp + m;

  if (k == 0)
    {
      c = mpn_sumdiff_n (typm, typp, yp, yp + m, m);
      MPN_INCR_U (typm, m, c >> 1);
      c1 = mpn_add_1 (typp, typp, m, c & 1);
    }
  else
    {
      mpn_rshift (typp, yp + m - 1, m, GMP_NUMB_BITS - k);
      if (n == 2 * m)
	{
	  typp[m - 1] |= yp[2 * m - 1] << k;
	  ASSERT (yp[2 * m - 1] >> (GMP_NUMB_BITS - k) == 0);
	}
      ASSERT (typp[m - 1] >> (GMP_NUMB_BITS - k) == 0);	/* have h bits */

      car = yp[m - 1];
      yp[m - 1] &= GMP_NUMB_MASK >> k;
      
      ASSERT (yp[m - 1] >> (GMP_NUMB_BITS - k) == 0);	/* have h bits */  
      
      c1 = mpn_sumdiff_n (typm, typp, yp, typp, m);
      c = typm[m - 1] >> (GMP_NUMB_BITS - k);
      yp[m - 1] = car;
      MPN_INCR_U (typm, m, c);
      c1 = mpn_add_1 (typp, typp, m, c1);
      typm[m - 1] &= GMP_NUMB_MASK >> k;
      typp[m - 1] &= GMP_NUMB_MASK >> k;
    }

  if (k == 0)
    {
      c = mpn_sumdiff_n (tzpm, tzpp, zp, zp + m, m);
      MPN_INCR_U (tzpm, m, c >> 1);
      c2 = mpn_add_1 (tzpp, tzpp, m, c & 1);
    }
  else
    {
      mpn_rshift (tzpp, zp + m - 1, m, GMP_NUMB_BITS - k);
      if (n == 2 * m)
	{
	  tzpp[m - 1] |= zp[2 * m - 1] << k;
	  ASSERT (zp[2 * m - 1] >> (GMP_NUMB_BITS - k) == 0);
	}
      ASSERT (tzpp[m - 1] >> (GMP_NUMB_BITS - k) == 0);	/* have h bits */
      
      car = zp[m - 1];
      zp[m - 1] &= GMP_NUMB_MASK >> k;
      
      ASSERT (zp[m - 1] >> (GMP_NUMB_BITS - k) == 0);	/* have h bits */
      
      c2 = mpn_sumdiff_n (tzpm, tzpp, zp, tzpp, m);
      c = tzpm[m - 1] >> (GMP_NUMB_BITS - k);
      zp[m - 1] = car;
      MPN_INCR_U (tzpm, m, c);
      c2 = mpn_add_1 (tzpp, tzpp, m, c2);
      tzpm[m - 1] &= GMP_NUMB_MASK >> k;
      tzpp[m - 1] &= GMP_NUMB_MASK >> k;
    }
  
  mpn_mulmod_2expm1(S, typm, tzpm, h, temp);	/* unroll this recursion, S = A rename */
  c = mpn_mulmod_2expp1_basecase (D, typp, tzpp, c1 * 2 + c2, h, temp);	/* D = B rename */
  
  if (LIKELY (c == 0))
    {
      c1 = mpn_sumdiff_n (S, D, S, D, m);
      bor = c1 & 1;
      c = c1 >> 1;
      D[m - 1] &= GMP_NUMB_MASK >> k;
      
      if (k != 0 && S[m - 1] >> (GMP_NUMB_BITS - k) != 0)
	c = 1;
      
      S[m - 1] &= GMP_NUMB_MASK >> k;
    }
  else
    {
      c = 1;
      bor = 1;
      MPN_COPY (D, S, m);
    }
  
  bor = mpn_sub_1 (S, S, m, bor);
  S[m - 1] &= GMP_NUMB_MASK >> k;
  
  if (bor == 0)
    {
      c = mpn_add_1 (D, D, m, c);
      
      if (k != 0 && D[m - 1] >> (GMP_NUMB_BITS - k) != 0)
	c = 1;
      
      D[m - 1] &= GMP_NUMB_MASK >> k;
      
      if (c != 0)
	S[0] |= 1;
    }
  
  if (k == 0)
    {
      car = mpn_half (xp, n);
      xp[n - 1] |= car;
    }				/* C sequence point rule */
  else
    {
      car = mpn_half (xp, m);
      car1 = xp[m - 1];
      
      if (GMP_NUMB_BITS - k - 1 != 0)
	{
	  Dm = mpn_lshift (xp + m - 1, D, m, GMP_NUMB_BITS - k - 1);
	}
      else
	{
	  Dm = 0;
	  MPN_COPY (xp + m - 1, D, m);
	}
      
      xp[m - 1] |= car1;
      
      if (2 * m == n)
	xp[n - 1] = Dm;
      
      xp[n - 1] |= car >> (GMP_NUMB_BITS * n - b);
    }

  return;
}

/*
   {rp, min(rn, an + bn)} = {ap, an} * {bp, bn} mod(B^rn - 1)
*/
void
mpn_mulmod_bnm1 (mp_ptr rp, mp_size_t rn, mp_srcptr ap, mp_size_t an, 
                                          mp_srcptr bp, mp_size_t bn, mp_ptr scratch)
{
  mp_ptr tp, rp2;
  TMP_DECL;

  ASSERT (0 < bn);
  ASSERT (bn <= an);
  ASSERT (an <= rn);

  TMP_MARK;

  if (an < rn)
  {
     tp = TMP_ALLOC_LIMBS(rn);
     MPN_COPY(tp, ap, an);
     MPN_ZERO(tp + an, rn - an);
     ap = tp;
  }

  if (bn < rn)
  {
     tp = TMP_ALLOC_LIMBS(rn);
     MPN_COPY(tp, bp, bn);
     MPN_ZERO(tp + bn, rn - bn);
     bp = tp;
  }

  if (an + bn < rn)
  {
     tp = TMP_ALLOC_LIMBS(rn);
     mpn_mulmod_2expm1(tp, (mp_ptr) ap, (mp_ptr) bp, rn*GMP_LIMB_BITS, scratch);
     MPN_COPY(rp, tp, an + bn);
  } else
     mpn_mulmod_2expm1(rp, (mp_ptr) ap, (mp_ptr) bp, rn*GMP_LIMB_BITS, scratch);
     
  TMP_FREE;
}
