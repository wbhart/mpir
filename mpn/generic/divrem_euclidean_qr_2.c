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

mp_limb_t	mpn_divrem_euclidean_qr_2(mp_ptr qp,mp_ptr xp,mp_size_t xn,mp_srcptr dp)
{mp_size_t qn;mp_limb_t qf,t[2],t1[2],q,h,l,d1,d2,i;int c1,c3,c4;

ASSERT(xn>=2);ASSERT_MPN(dp,2);ASSERT_MPN(xp,xn);ASSERT(dp[1]!=0);
qn=xn-1;
//ASSERT(!MPN_OVERLAP_P(qp,qn,xp,xn));//     correct this overlap requirement
ASSERT((dp[1]>>(GMP_NUMB_BITS-1))!=0);
h=0;d1=dp[1];d2=dp[0];invert_limb(i,d1);l=xp[xn-1];
qn=xn-2;t[0]=xp[qn];
if(l<d1){h=t[1]=l;l=t[0]=xp[qn];qf=0;}
else
  {qf=1;t[1]=l-d1;t1[1]=0;t1[0]=d2;
   if(mpn_sub_n(t,t,t1,2)){qf--;mpn_add_n(t,t,dp,2);}
   h=t[1];l=t[0];}
for(qn=xn-3;qn>=0;qn--)
   {t[0]=xp[qn];
    if(h<d1)
      {udiv_qrnnd_preinv(q,t[1],h,l,d1,i);
       umul_ppmm(t1[1],t1[0],q,d2);
       if(mpn_sub_n(t,t,t1,2))
         {q--;if(mpn_add_n(t,t,dp,2)==0)
                {q--;ASSERT_CARRY(mpn_add_n(t,t,dp,2));}}
      }
    else
      {ASSERT(h==d1);q=-1;t[1]=l;
       c3=mpn_add_n(t,t,dp,2);
       c1=mpn_sub_1(t+1,t+1,1,d2);
       c4=c3-c1;
       if(l>=d1)
         {ASSERT(c3!=0);ASSERT(c4==0);}// our guess is B+1   so q=B-1 is correct
       else
         {ASSERT(c4<=0);// our guess is B so q=B-1 or B-2
          if(c4!=0){q--;mpn_add_n(t,t,dp,2);}
         }       
      }
    h=t[1];l=t[0];qp[qn]=q;
   }
xp[1]=t[1];xp[0]=t[0];
return qf;}
