/* mpn_mulmod_2expp1_basecase 

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
#include "longlong.h"

static mp_size_t mulmod_2expp1_table_n[FFT_N_NUM] = MULMOD_TAB;

/*
   ret + (xp, n) = (yp, n)*(zp, n) % 2^b + 1  
   needs (tp, 2n) temp space, everything reduced mod 2^b 
   inputs, outputs are fully reduced
  
   N.B: 2n is not the same as 2b rounded up to nearest limb!
*/
inline static int
mpn_mulmod_2expp1_internal (mp_ptr xp, mp_srcptr yp, mp_srcptr zp,
			                                         mpir_ui b, mp_ptr tp)
{
  mp_size_t n, k;
  mp_limb_t c;

  TMP_DECL;

  n = BITS_TO_LIMBS (b);
  k = GMP_NUMB_BITS * n - b;

  ASSERT(b > 0);
  ASSERT(n > 0);
  ASSERT_MPN(yp, n);
  ASSERT_MPN(zp, n);
  ASSERT(!MPN_OVERLAP_P (tp, 2 * n, yp, n));
  ASSERT(!MPN_OVERLAP_P (tp, 2 * n, zp, n));
  ASSERT(MPN_SAME_OR_SEPARATE_P (xp, tp, n));
  ASSERT(MPN_SAME_OR_SEPARATE_P (xp, tp + n, n));
  ASSERT(k == 0 || yp[n - 1] >> (GMP_NUMB_BITS - k) == 0);
  ASSERT(k == 0 || zp[n - 1] >> (GMP_NUMB_BITS - k) == 0);

#ifndef TUNE_PROGRAM_BUILD
  if (k == 0 && n > FFT_MULMOD_2EXPP1_CUTOFF && n == mpir_fft_adjust_limbs(n))
  {
      mp_bitcnt_t depth1, depth = 1;
      mp_size_t w1, off;
      mp_ptr tx, ty, tz;
      mp_limb_t ret;

      TMP_MARK;

      tx = TMP_BALLOC_LIMBS(3*n + 3);
      ty = tx + n + 1;
      tz = ty + n + 1;

      MPN_COPY(ty, yp, n);
      MPN_COPY(tz, zp, n);
      ty[n] = 0;
      tz[n] = 0;

      while ((((mp_limb_t)1)<<depth) < b) depth++;
   
      if (depth < 12) off = mulmod_2expp1_table_n[0];
      else off = mulmod_2expp1_table_n[MIN(depth, FFT_N_NUM + 11) - 12];
      depth1 = depth/2 - off;
   
      w1 = b/(((mp_limb_t)1)<<(2*depth1));

      mpir_fft_mulmod_2expp1(tx, ty, tz, n, depth1, w1);

      MPN_COPY(xp, tx, n);
      ret = tx[n];
      
      TMP_FREE;

	   return ret;
  }
#endif

  if (yp == zp)
     mpn_sqr(tp, yp, n);
  else
     mpn_mul_n (tp, yp, zp, n);

  if (k == 0)
    {
      c = mpn_sub_n (xp, tp, tp + n, n);

      return mpn_add_1 (xp, xp, n, c);
    }

  c = tp[n - 1];
  tp[n - 1] &= GMP_NUMB_MASK >> k;

#if HAVE_NATIVE_mpn_sublsh_nc
  c = mpn_sublsh_nc (xp, tp, tp + n, n, k, c);
#else
  {
    mp_limb_t c1;
    c1 = mpn_lshift (tp + n, tp + n, n, k);
    tp[n] |= c >> (GMP_NUMB_BITS - k);
    c = mpn_sub_n (xp, tp, tp + n, n) + c1;
  }
#endif

  c = mpn_add_1 (xp, xp, n, c);
  xp[n - 1] &= GMP_NUMB_MASK >> k;

  return c;
}

/* 
   c is the top bits of the inputs, (fully reduced)
   c & 2 is the top bit of y
   c & 1 is the top bit of z
*/
int
mpn_mulmod_2expp1_basecase (mp_ptr xp, mp_srcptr yp, mp_srcptr zp, 
                                           int c, mpir_ui b, mp_ptr tp)
{
  int cy, cz;
  mp_size_t n, k;

  cy = c & 2;
  cz = c & 1;

  n = BITS_TO_LIMBS (b);
  k = GMP_NUMB_BITS * n - b;

  ASSERT(b > 0);
  ASSERT(n > 0);
  ASSERT_MPN(yp, n);
  ASSERT_MPN(zp, n);
  ASSERT(!MPN_OVERLAP_P (tp, 2 * n, yp, n));
  ASSERT(!MPN_OVERLAP_P (tp, 2 * n, zp, n));
  ASSERT(MPN_SAME_OR_SEPARATE_P (xp, tp, n));
  ASSERT(MPN_SAME_OR_SEPARATE_P (xp, tp + n, n));
  ASSERT(k == 0 || yp[n - 1] >> (GMP_NUMB_BITS - k) == 0);
  ASSERT(k == 0 || zp[n - 1] >> (GMP_NUMB_BITS - k) == 0);

#if WANT_ASSERT
  {
     mp_size_t t = n;

     MPN_NORMALIZE(yp, t);
     ASSERT(cy == 0 || t == 0);
     
     t = n; 
     MPN_NORMALIZE(zp, t);
     ASSERT(cz == 0 || t == 0);
  }
#endif

  if (LIKELY (cy == 0))
    {
      if (LIKELY (cz == 0))
	{
	  c = mpn_mulmod_2expp1_internal (xp, yp, zp, b, tp);
	}
      else
	{
	  c = mpn_neg_n (xp, yp, n);
	  c = mpn_add_1 (xp, xp, n, c);
	  xp[n - 1] &= GMP_NUMB_MASK >> k;
	}
    }
  else
    {
      if (LIKELY (cz == 0))
	{
	  c = mpn_neg_n (xp, zp, n);
	  c = mpn_add_1 (xp, xp, n, c);
	  xp[n - 1] &= GMP_NUMB_MASK >> k;
	}
      else
	{
	  c = 0;
	  xp[0] = 1;
	  MPN_ZERO (xp + 1, n - 1);
	}
    }
  return c;
}
