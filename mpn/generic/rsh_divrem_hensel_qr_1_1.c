/*
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

mp_limb_t mpn_rsh_divrem_hensel_qr_1_1(mp_ptr qp,mp_srcptr xp,mp_size_t n,mp_limb_t d,int s,mp_limb_t cin)
{mp_size_t j;mp_limb_t c,h,q,dummy,h1,t,m,qo,qb;

ASSERT(n>0);ASSERT(d%2==1);ASSERT_MPN(xp,n);ASSERT(MPN_SAME_OR_SEPARATE_P(qp,xp,n));
ASSERT(s>=0);modlimb_invert(m,d);//should we allow s=0 ??
h1=xp[0];c=0;h=cin;
t=h+c;if(t>h1){h1=h1-t;c=1;}else{h1=h1-t;c=0;}
q=h1*m;qo=q>>s;
umul_ppmm(h,dummy,q,d);
for(j=1;j<=n-1;j++)
   {h1=xp[j];
    t=h+c;if(t>h1){h1=h1-t;c=1;}else{h1=h1-t;c=0;}
    q=h1*m;
    qo=qo|(q<<(GMP_LIMB_BITS-1-s)<<1);
    qp[j-1]=qo;
    qo=q>>s;
    umul_ppmm(h,dummy,q,d);
    ASSERT(dummy==h1);}
qp[n-1]=qo;
return h+c;}   
