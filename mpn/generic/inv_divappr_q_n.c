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

/* 
   Computes an approximate quotient of { np, 2*dn } by { dp, dn } which is
   either correct or one too large. We require dp to be normalised and inv
   to be a precomputed inverse given by mpn_invert.
*/
mp_limb_t mpn_inv_divappr_q_n(mp_ptr_t qp, mp_srcptr np, 
                              mp_srcptr dp, mp_size_t dn, mp_srcptr inv)
{
   mp_ptr_t temp;
   mp_limb_t ret = 0;

   TMP_DECL;
   
   TMP_MARK;
   
   temp = TMP_ALLOC_LIMBS(dn);

   if (mpn_cmp_n(np + dn, dp, dn) >= 0)
   {
      ret = 1;
      mpn_sub_n(np + dn, np + dn, dp, dn);
   }

   mpn_mulhigh_n(qp, np + dn, inv, dn);
   mpn_add_n(qp, qp, dp, dn);

   /* 
      Let X = B^dn + inv, D = { dp, dn }, N = { np, 2*dn }, then
      DX < B^{2*dn} <= D(X+1), thus
      NX < B^{2*dn}N/D <= NX + N < NX + B^{2*dn}, i.e.
      NX/B^{2*dn} < N/D < NX/B^{2*dn} + 1, i.e.
      NX//B^{2*dn} <= N//D <= NX//B^{2*dn} + 1.
   */
      
   ret += mpn_add_1(qp, qp, dn, 1);
   
   if (UNLIKELY(ret == 2))
   {
      ret = 1;
      mpn_sub_1(qp, qp, dn, 1);
   }

   TMP_FREE;

   return ret;
}