/* Test mpn_mulmod_2expp1

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

#define DISP(xp,xn)  do{int __t;printf("%s=",#xp);for(__t=(xn)-1;__t>=0;__t--)printf("%ld ",(xp)[__t]);printf("\n");}while(0)

int
main (void)
{unsigned long bp,xn,n,yn,b,zn,c,dn;gmp_randstate_t rands;int qpn,j,k,i,l,i1,k1,j1,i2,k2,j2,cc;
mp_limb_t xp[10000],dp[10000],qp[10000],yp[10000],rp[10000],zp[10000],tp[10000],tb;
  tests_start ();
  gmp_randinit_default(rands);

b=1;tb=1;tb<<=b;
for(   ;b<600;b++,tb*=2)
   {xn=BITS_TO_LIMBS(b);k=xn*GMP_NUMB_BITS-b;if(tb==0 || tb>GMP_NUMB_MASK)tb=1;
    MPN_ZERO(dp,xn);mpn_com_n(dp,dp,xn);dp[xn-1]&=GMP_NUMB_MASK>>k;// dp is 2^b-1
    dn=xn;dp[xn]=mpn_add_1(dp,dp,xn,2);if(dp[dn]!=0)dn++;// dp is 2^b+1 
    for(c=0;c<20;c++)
       {mpn_rrandom(xp,rands,xn);mpn_rrandom(yp,rands,xn);
        xp[xn-1]&=GMP_NUMB_MASK>>k;
        yp[xn-1]&=GMP_NUMB_MASK>>k;
        ASSERT_MPN(xp,xn);ASSERT_MPN(yp,xn);ASSERT_MPN(zp,zn);ASSERT_MPN(dp,dn);
        mpn_mul_n(zp,xp,yp,xn);zn=xn*2;MPN_NORMALIZE(zp,zn);
        if(zn>=dn){mpn_tdiv_qr(qp,rp,0,zp,zn,dp,dn);}else{MPN_COPY(rp,zp,dn);}
        cc=tp[xn]=mpn_mulmod_2expp1(tp,xp,yp,0,b,qp);
        if(cc!=0 && dn==xn){tp[xn-1]|=tb;}
        if(mpn_cmp(tp,rp,dn)!=0){printf("mpn_mulmod_2expp1 error %ld\n",b);abort();}        
       }
   }

b=1;tb=1;tb<<=b;
for(   ;b<600;b++,tb*=2)
   {xn=BITS_TO_LIMBS(b);k=xn*GMP_NUMB_BITS-b;if(tb==0 || tb>GMP_NUMB_MASK)tb=1;
    MPN_ZERO(dp,xn);mpn_com_n(dp,dp,xn);dp[xn-1]&=GMP_NUMB_MASK>>k;// dp is 2^b-1
    dn=xn;dp[xn]=mpn_add_1(dp,dp,xn,2);if(dp[dn]!=0)dn++;// dp is 2^b+1 
    for(c=0;c<20;c++)
       {mpn_rrandom(xp,rands,xn);MPN_ZERO(yp,xn);// set yp to 2^b
        xp[xn-1]&=GMP_NUMB_MASK>>k;
        yp[xn-1]&=GMP_NUMB_MASK>>k;
        yn=xn;if(tb==1)yn++;
        yp[yn-1]=tb;
        ASSERT_MPN(xp,xn);
        ASSERT_MPN(yp,yn);
        ASSERT_MPN(zp,zn);
        ASSERT_MPN(dp,dn);
        mpn_mul(zp,yp,yn,xp,xn);zn=xn*2;MPN_NORMALIZE(zp,zn);MPN_ZERO(yp,xn);// set yp to 2^b
        if(zn>=dn){mpn_tdiv_qr(qp,rp,0,zp,zn,dp,dn);}else{MPN_COPY(rp,zp,dn);}
        cc=tp[xn]=mpn_mulmod_2expp1(tp,xp,yp,1,b,qp);
        if(cc!=0 && dn==xn){tp[xn-1]|=tb;}
        if(mpn_cmp(tp,rp,dn)!=0){printf("mpn_mulmod_2expp1 error %ld\n",b);abort();}        
       }
   }

b=1;tb=1;tb<<=b;
for(   ;b<600;b++,tb*=2)
   {xn=BITS_TO_LIMBS(b);k=xn*GMP_NUMB_BITS-b;if(tb==0 || tb>GMP_NUMB_MASK)tb=1;
    MPN_ZERO(dp,xn);mpn_com_n(dp,dp,xn);dp[xn-1]&=GMP_NUMB_MASK>>k;// dp is 2^b-1
    dn=xn;dp[xn]=mpn_add_1(dp,dp,xn,2);if(dp[dn]!=0)dn++;// dp is 2^b+1 
    for(c=0;c<20;c++)
       {mpn_rrandom(xp,rands,xn);MPN_ZERO(yp,xn);// set yp to 2^b
        xp[xn-1]&=GMP_NUMB_MASK>>k;
        yp[xn-1]&=GMP_NUMB_MASK>>k;
        yn=xn;if(tb==1)yn++;
        yp[yn-1]=tb;
        ASSERT_MPN(xp,xn);ASSERT_MPN(yp,yn);ASSERT_MPN(zp,zn);ASSERT_MPN(dp,dn);
        mpn_mul(zp,yp,yn,xp,xn);zn=xn*2;MPN_NORMALIZE(zp,zn);MPN_ZERO(yp,xn);// set yp to 2^b
        if(zn>=dn){mpn_tdiv_qr(qp,rp,0,zp,zn,dp,dn);}else{MPN_COPY(rp,zp,dn);}
        cc=tp[xn]=mpn_mulmod_2expp1(tp,yp,xp,2,b,qp);
        if(cc!=0 && dn==xn){tp[xn-1]|=tb;}
        if(mpn_cmp(tp,rp,dn)!=0){printf("mpn_mulmod_2expp1 error %ld\n",b);abort();}        
       }
   }

rp[0]=1;MPN_ZERO(rp+1,1000);
b=1;tb=1;tb<<=b;
for(   ;b<600;b++,tb*=2)
   {xn=BITS_TO_LIMBS(b);k=xn*GMP_NUMB_BITS-b;if(tb==0 || tb>GMP_NUMB_MASK)tb=1;
    MPN_ZERO(dp,xn);mpn_com_n(dp,dp,xn);dp[xn-1]&=GMP_NUMB_MASK>>k;// dp is 2^b-1
    dn=xn;dp[xn]=mpn_add_1(dp,dp,xn,2);if(dp[dn]!=0)dn++;// dp is 2^b+1 
    for(c=0;c<1;c++)
       {MPN_ZERO(xp,xn);MPN_ZERO(yp,xn);// set xp,yp to 2^b
        xp[xn-1]&=GMP_NUMB_MASK>>k;
        yp[xn-1]&=GMP_NUMB_MASK>>k;
        cc=tp[xn]=mpn_mulmod_2expp1(tp,yp,xp,3,b,qp);
        if(cc!=0 && dn==xn){tp[xn-1]|=tb;}
        if(mpn_cmp(tp,rp,dn)!=0){printf("mpn_mulmod_2expp1 error %ld\n",b);abort();}        
       }
   }
  gmp_randclear(rands);
  tests_end ();
  exit (0);
}
