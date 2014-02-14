/* inv_divappr_q_n - approximate quotient using a precomputed inverse 

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
   Computes an approximate quotient of { np, 2*dn } by { dp, dn } which is
   either correct or one too large. We require dp to be normalised and inv
   to be a precomputed inverse given by mpn_invert.
*/
mp_limb_t 
mpn_inv_divappr_q_n(mp_ptr qp, mp_ptr np, 
                              mp_srcptr dp, mp_size_t dn, mp_srcptr inv)
{
   mp_limb_t cy, lo, ret = 0, ret2 = 0;
   mp_ptr tp;
   TMP_DECL;

   TMP_MARK;

   ASSERT(dp[dn-1] & GMP_LIMB_HIGHBIT);
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
   ret += mpn_add_1(qp, qp, dn, cy + 1);

   /* 
      Let X = B^dn + inv, D = { dp, dn }, N = { np, 2*dn }, then
      DX < B^{2*dn} <= D(X+1), thus
      Let N' = { np + n - 1, n + 1 }
	   N'X/B^{dn+1} < B^{dn-1}N'/D <= N'X/B^{dn+1} + N'/B^{dn+1} < N'X/B^{dn+1} + 1
      N'X/B^{dn+1} < N/D <=  N'X/B^{dn+1} + 1 + 2/B
      There is either one integer in this range, or two. However, in the latter case
	  the left hand bound is either an integer or < 2/B below one.
   */
    
   if (UNLIKELY(ret == 1))
   {
      ret -= mpn_sub_1(qp, qp, dn, 1);
      ASSERT(ret == 0);
   }
  
   if (UNLIKELY((lo == ~CNST_LIMB(0)) || (lo == ~CNST_LIMB(1)))) 
   {
	   /* Special case, multiply out to get accurate quotient */
	   ret -= mpn_sub_1(qp, qp, dn, 1);
      if (UNLIKELY(ret == ~CNST_LIMB(0)))
         ret += mpn_add_1(qp, qp, dn, 1);
      
      /* ret is now guaranteed to be 0 */
      ASSERT(ret == 0);
       
      mpn_mul_n(tp, qp, dp, dn);
      mpn_sub_n(tp, np, tp, dn+1);
      while (tp[dn] || mpn_cmp(tp, dp, dn) >= 0)
	   {
		   ret += mpn_add_1(qp, qp, dn, 1);
		   tp[dn] -= mpn_sub_n(tp, tp, dp, dn);
	   }
       
      /* Not possible for ret == 2 as we have qp*dp <= np */
      ASSERT(ret + ret2 < 2);
   }

   TMP_FREE;

   return ret + ret2;
}

