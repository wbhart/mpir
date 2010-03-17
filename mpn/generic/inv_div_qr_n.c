/* inv_div_qr_n - quotient and remainder using a precomputed inverse 

Copyright 2010 William Hart

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

#include <mpir.h>
#include "gmp-impl.h"
#include "longlong.h"

/* 
   Computes the quotient and remainder of { np, 2*dn } by { dp, dn }.
   We require dp to be normalised and inv to be a precomputed inverse 
   of { dp, dn } given by mpn_invert.
*/
mp_limb_t 
mpn_inv_div_qr_n(mp_ptr qp, mp_ptr np, 
                           mp_srcptr dp, mp_size_t dn, mp_srcptr inv)
{
   mp_limb_t cy, lo, ret = 0, ret2 = 0;
   mp_size_t m, i;
   mp_ptr tp, tp2;
   TMP_DECL;

   TMP_MARK;

   ASSERT(mpn_is_invert(inv, dp, dn));

   if (mpn_cmp(np + dn, dp, dn) >= 0)
   {
      ret2 = 1;
      mpn_sub_n(np + dn, np + dn, dp, dn);
   }

   tp = TMP_ALLOC_LIMBS(2*dn + 1);
   mpn_mul(tp, np + dn - 1, dn + 1, inv, dn);
   add_ssaaaa(cy, lo, 0, np[dn - 1], 0, tp[dn]);
   ret += mpn_add_n(qp, tp + dn + 1, np + dn, dn);
   ret += mpn_add_1(qp, qp, dn, cy);

   /* 
      Let X = B^dn + inv, D = { dp, dn }, N = { np, 2*dn }, then
      DX < B^{2*dn} <= D(X+1), thus
      Let N' = { np + n - 1, n + 1 }
	  N'X/B^{dn+1} < B^{dn-1}N'/D <= N'X/B^{dn+1} + N'/B^{dn+1} < N'X/B^{dn+1} + 1
      N'X/B^{dn+1} < N/D <= N'X/B^{dn+1} + 1 + 2/B
      There is either one integer in this range, or two. However, in the latter case
	  the left hand bound is either an integer or < 2/B below one.
   */
     
   if (UNLIKELY(ret == 1))
   {
      ret -= mpn_sub_1(qp, qp, dn, 1);
      ASSERT(ret == 0);
   }

   ret -= mpn_sub_1(qp, qp, dn, 1); 
   if (UNLIKELY(ret == ~CNST_LIMB(0))) 
      ret += mpn_add_1(qp, qp, dn, 1);
   /* ret is now guaranteed to be 0 or 1*/
   ASSERT(ret == 0);

   m = dn + 1;
   if ((dn <= MPN_FFT_MUL_N_MINSIZE) || (ret))
   {
      mpn_mul_n(tp, qp, dp, dn);
   } else
   {
      mp_limb_t cy, cy2;
      mp_size_t k;
      k = mpn_fft_best_k (m, 0);
      m = mpn_fft_next_size (m, k);

      cy = mpn_mul_fft (tp, m, qp, dn, dp, dn, k);
      /* cy, {tp, m} = qp * dp mod (B^m+1) */ 
      cy2 = mpn_add_n(tp, tp, np + m, 2*dn - m);
      mpn_add_1(tp + 2*dn - m, tp + 2*dn - m, 2*m - 2*dn, cy2);
          
      /* Make correction */    
      mpn_sub_1(tp, tp, m, tp[0] - dp[0]*qp[0]);
   }
   
   mpn_sub_n(np, np, tp, m);
   MPN_ZERO(np + m, 2*dn - m);
   while (np[dn] || mpn_cmp(np, dp, dn) >= 0)
   {
	   ret += mpn_add_1(qp, qp, dn, 1);
	   np[dn] -= mpn_sub_n(np, np, dp, dn);
   }
  
   /* Not possible for ret == 2 as we have qp*dp <= np */
   ASSERT(ret + ret2 < 2);

   TMP_FREE;

   return ret + ret2;
}

