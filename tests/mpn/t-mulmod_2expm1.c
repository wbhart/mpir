/* Test mpn_mulmod_2expm1

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

#include <stdio.h>
#include <stdlib.h>
#include "mpir.h"
#include "gmp-impl.h"
#include "tests.h"

#define DISP(xp,xn)  do{int __t;printf("%s=",#xp);for(__t=(xn)-1;__t>=0;__t--)printf("%lX ",(xp)[__t]);printf("\n");}while(0)

int
main (void)
{unsigned long bp,xn,n,b,zn,c;gmp_randstate_t rands;int qpn,j,k,i,l,i1,k1,j1,i2,k2,j2;
mp_limb_t xp[10000],dp[10000],qp[10000],yp[10000],rp[10000],zp[10000],tp[10000];
  tests_start ();
  gmp_randinit_default(rands);
  
for(b=1;b<600;b++)
   {xn=BITS_TO_LIMBS(b);k=xn*GMP_NUMB_BITS-b;
    MPN_ZERO(dp,xn);mpn_com_n(dp,dp,xn);dp[xn-1]&=GMP_NUMB_MASK>>k;// dp is 2^b-1
    for(c=0;c<5*b;c++)
       {mpn_rrandom(xp,rands,xn);mpn_rrandom(yp,rands,xn);
        xp[xn-1]&=GMP_NUMB_MASK>>k;
        yp[xn-1]&=GMP_NUMB_MASK>>k;
        mpn_mul_n(zp,xp,yp,xn);zn=xn*2;MPN_NORMALIZE(zp,zn);
        if(zn>=xn){mpn_tdiv_qr(qp,rp,0,zp,zn,dp,xn);}else{MPN_COPY(rp,zp,xn);}
        mpn_mulmod_2expm1(tp,xp,yp,b,qp);
        if(mpn_cmp(tp,dp,xn)==0)MPN_ZERO(tp,xn);//fully reduce 
        if(mpn_cmp(tp,rp,xn)!=0){printf("mpn_mulmod_2expm1 error %ld\n",b);abort();}        
       }
   }

for(b=MULMOD_2EXPM1_THRESHOLD*GMP_NUMB_BITS;b<2*MULMOD_2EXPM1_THRESHOLD*GMP_NUMB_BITS;b+=2)
   {xn=BITS_TO_LIMBS(b);k=xn*GMP_NUMB_BITS-b;
    MPN_ZERO(dp,xn);mpn_com_n(dp,dp,xn);dp[xn-1]&=GMP_NUMB_MASK>>k;// dp is 2^b-1
    for(c=0;c<5;c++)
       {mpn_rrandom(xp,rands,xn);mpn_rrandom(yp,rands,xn);
        xp[xn-1]&=GMP_NUMB_MASK>>k;
        yp[xn-1]&=GMP_NUMB_MASK>>k;
        mpn_mul_n(zp,xp,yp,xn);zn=xn*2;MPN_NORMALIZE(zp,zn);
        if(zn>=xn){mpn_tdiv_qr(qp,rp,0,zp,zn,dp,xn);}else{MPN_COPY(rp,zp,xn);}
        mpn_mulmod_2expm1(tp,xp,yp,b,qp);
        if(mpn_cmp(tp,dp,xn)==0)MPN_ZERO(tp,xn);//fully reduce 
        if(mpn_cmp(tp,rp,xn)!=0){printf("mpn_mulmod_2expm1 error %ld\n",b);abort();}        
       }
   }

  gmp_randclear(rands);
  tests_end ();
  exit (0);
}
