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

void mpn_mod_1_2(mp_ptr rem,mp_srcptr xp,mp_size_t xn,mp_srcptr db)// in each round we hack off a limb from the body , ie k=1
{mp_limb_t h,l,sh,sl,th,tl;
 mp_size_t j;
 
ASSERT(xn>=4);
ASSERT_MPN(xp,xn);
ASSERT_LIMB(db[0]);ASSERT_LIMB(db[1]);ASSERT_LIMB(db[2]);

tl=xp[xn-2];th=xp[xn-1];
for(j=xn-4;j>=0;j-=2)
   {umul_ppmm(sh,sl,xp[j+1],db[0]);
    add_ssaaaa(sh,sl,sh,sl,0,xp[j]);
    umul_ppmm(h,l,tl,db[1]);
    add_ssaaaa(sh,sl,sh,sl,h,l);
    umul_ppmm(th,tl,th,db[2]);
    add_ssaaaa(th,tl,th,tl,sh,sl);}
if(j>-2)// we have at least three limbs to do still ie xp[0],...,tl,th
  {umul_ppmm(sh,sl,tl,db[0]);
   add_ssaaaa(sh,sl,sh,sl,0,xp[0]);
   umul_ppmm(th,tl,th,db[1]);
   add_ssaaaa(th,tl,th,tl,sh,sl);}
umul_ppmm(h,l,th,db[0]);
add_ssaaaa(h,l,h,l,0,tl);

rem[0]=l;rem[1]=h;
return;}
