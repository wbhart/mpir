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

mp_limb_t mpn_divrem_hensel_rsh_qr_1(mp_ptr qp,mp_srcptr xp,mp_size_t n,mp_limb_t d,int s)
{mp_size_t j;mp_limb_t c,h,q,dummy,h1,t,m;

ASSERT(n>0);ASSERT(d%2==1);ASSERT_MPN(xp,n);ASSERT(MPN_SAME_OR_SEPARATE_P(qp,xp,n));
ASSERT(s>=0);modlimb_invert(m,d);//should we allow s=0 ??
c=0;h=0;t=0;
for(j=0;j<=n-2;j++)
   {h1=(xp[j]>>s)|(xp[j+1]<<(GMP_LIMB_BITS-1-s)<<1);
    t=h+c;if(t>h1){h1=h1-t;c=1;}else{h1=h1-t;c=0;}
    q=h1*m;
    qp[j]=q;
    umul_ppmm(h,dummy,q,d);
    ASSERT(dummy==h1);}
h1=xp[n-1]>>s;
t=h+c;if(t>h1){h1=h1-t;c=1;}else{h1=h1-t;c=0;}
q=h1*m;qp[n-1]=q;
umul_ppmm(h,dummy,q,d);
ASSERT(dummy==h1);
return h+c;}   
