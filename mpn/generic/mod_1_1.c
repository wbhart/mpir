/*   

dnl  Copyright 2009 Jason Moxham

dnl  This file is part of the MPIR Library.

dnl  The MPIR Library is free software; you can redistribute it and/or modify
dnl  it under the terms of the GNU Lesser General Public License as published
dnl  by the Free Software Foundation; either version 2.1 of the License, or (at
dnl  your option) any later version.

dnl  The MPIR Library is distributed in the hope that it will be useful, but
dnl  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
dnl  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
dnl  License for more details.

dnl  You should have received a copy of the GNU Lesser General Public License
dnl  along with the MPIR Library; see the file COPYING.LIB.  If not, write
dnl  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
dnl  Boston, MA 02110-1301, USA.

*/

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"

/* in each round we remove one limb from the body, i.e. k = 1 */
void mpn_mod_1_1(mp_ptr rem, mp_srcptr xp, mp_size_t xn, mp_srcptr db)
{
   mp_limb_t h, l, sh, sl;
   mp_size_t j;
 
   ASSERT(xn >= 3);
   ASSERT_MPN(xp, xn);
   ASSERT_LIMB(db[0]);
   ASSERT_LIMB(db[1]);

   h = xp[xn - 1];
   l = xp[xn - 2];

   for (j = xn - 3; j >= 0; j--)
   {
      umul_ppmm(sh, sl, l, db[0]);
      add_ssaaaa(sh, sl, sh, sl, 0, xp[j]);
      umul_ppmm(h, l, h, db[1]);
      add_ssaaaa(h, l, h, l, sh, sl);
   }

   umul_ppmm(sh, sl, h, db[0]);
   add_ssaaaa(sh, sl, sh, sl, 0, l);

   rem[0] = sl;
   rem[1] = sh;
}
