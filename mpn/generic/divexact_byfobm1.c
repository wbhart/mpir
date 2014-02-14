/* mpn_divexact_by exact division by (B-1)/f

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

/*
   (xp, n) = (qp, n)*f - ret*B^n and 0 <= ret < f

   Note the divexact_by3 code is just a special case of this
*/
mp_limb_t mpn_divexact_byfobm1(mp_ptr qp, mp_srcptr xp, mp_size_t n,
                                             mp_limb_t f, mp_limb_t Bm1of)
{
   mp_size_t j;
   mp_limb_t c, acc, ax, dx;

   ASSERT(n > 0);
   ASSERT_MPN(xp, n);
   ASSERT(MPN_SAME_OR_SEPARATE_P(qp, xp, n));
   ASSERT(Bm1of*f + 1 == 0);

   acc = 0*Bm1of; /* carry in is 0 */

   for (j = 0; j <= n - 1; j++)
   {
      umul_ppmm(dx, ax, xp[j], Bm1of);
    
      SUBC_LIMB(c, acc, acc, ax);
    
      qp[j] = acc;
      acc -= dx + c;
   }

   /* return next quotient*(-f) */ 

   return acc*(-f);
}   
