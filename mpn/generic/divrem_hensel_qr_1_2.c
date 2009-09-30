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

// using a two limb inverse of a one limb divisor
mp_limb_t mpn_divrem_hensel_qr_1_2(mp_ptr qp, mp_srcptr xp, mp_size_t n, mp_limb_t d)
{mp_size_t j;mp_limb_t c,h,q,dummy,h1,t,ml,mh,xl,xh,ql,qh;

ASSERT(n>=2);ASSERT_MPN(xp,n);ASSERT(MPN_SAME_OR_SEPARATE_P(qp,xp,n));
ASSERT(d%2==1);
modlimb_invert(ml,d);
umul_ppmm(h,dummy,d,ml);ASSERT(dummy==1);
h=-h;mh=ml*h;// (mh,ml) is our two limb inverse
c=0;h=0;
for(j=0;j+1<=n-1;j+=2)
   {xl=xp[j];xh=xp[j+1];
    t=h+c;if(xh==0 && t>xl){c=1;}else{c=0;}
    sub_ddmmss(xh,xl,xh,xl,0,t);
    umul_ppmm(qh,ql,xl,ml);
    qh=qh+xh*ml+xl*mh;
    qp[j]=ql;qp[j+1]=qh;
    umul_ppmm(h,h1,qh,d);
    if(h1>xh)h++;}
if(j<=n-1)
  {h1=xp[j];
   t=h+c;if(t>h1){h1=h1-t;c=1;}else{h1=h1-t;c=0;}
   q=h1*ml;
   qp[j]=q;
   umul_ppmm(h,dummy,q,d);
   ASSERT(dummy==h1);}
return h+c;}   // so  (xp,n) = (qp,n)*d -ret*B^n    and 0 <= ret < d
