/* mpn_divexact_by3c exact division by 3.

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

mp_limb_t	mpn_divexact_by3c(mp_ptr qp,mp_srcptr xp,mp_size_t n,mp_limb_t ci)
{mp_size_t j;mp_limb_t c,m,acc,ax,dx;

ASSERT(n>0);
ASSERT_MPN(xp,n);
ASSERT(MPN_SAME_OR_SEPARATE_P(qp,xp,n));
m=0;m=~m;m=m/3;// m=(B-1)/3
acc=ci*m;
for(j=0;j<=n-1;j++)
   {umul_ppmm(dx,ax,xp[j],m);
    SUBC_LIMB(c,acc,acc,ax);
    qp[j]=acc;
    acc-=dx+c;}
// return next quotient*-3    
return acc*-3;}   // so  (xp,n) = (qp,n)*3 -ret*B^n    and 0 <= ret < 3




#if 0
#define ADD ADDC_LIMB
#define SUB SUBC_LIMB

#define ADC(co,w,x,y,ci) do{mp_limb_t c1,c2,t;ADD(c1,t,x,y);ADD(c2,w,t,ci);co=c1|c2;}while(0)
#define SBB(co,w,x,y,ci) do{mp_limb_t c1,c2,t;SUB(c1,t,x,y);SUB(c2,w,t,ci);co=c1|c2;}while(0)

// basic divexact
mp_limb_t	divexact_basic(mp_ptr qp,mp_ptr xp,mp_size_t n,mp_limb_t d)
{int j;mp_limb_t c,h,q,dummy,h1,t,m;

ASSERT(n>0);ASSERT(d!=0);ASSERT_MPN(xp,n);ASSERT(MPN_SAME_OR_SEPARATE_P(qp,xp,n));
ASSERT(d%2==1);modlimb_invert(m,d);
c=0;h=0;t=0;
for(j=0;j<=n-1;j++)
   {h1=xp[j];
    t=h+c;if(t>h1){h1=h1-t;c=1;}else{h1=h1-t;c=0;}// set borrow to c ; sbb t,h1 ; set c to borrow
    q=h1*m;
    qp[j]=q;
    umul_ppmm(h,dummy,q,d);
    ASSERT(dummy==h1);}
// ie returns next quotient*-d
return h+c;}   // so  (xp,n) = (qp,n)*d -ret*B^n    and 0 <= ret < d

/*
  A divexact by 3 can be obtained via   X * ((B-1)/3)  / (B-1)
  The advantage of this is that the multiplications are taken out of the dependant chain.
  The exact division by B-1 can be done like mpn_divexact_byff and can be thought of as a 
  multi-limb subtraction but with an accumulator instead of a destination.
  Combining the multiplication and exact division together we get the function below which
  is the usual mpn_submul_1 but with an accumulator instead of a destination.
*/


mp_limb_t	divexact_submul(mp_ptr qp,mp_ptr xp,mp_size_t n)
{int j;mp_limb_t c,m,t1,t2,t3,acc,ax,dx,t;

ASSERT(n>0);ASSERT_MPN(xp,n);ASSERT(MPN_SAME_OR_SEPARATE_P(qp,xp,n));
m=0;m=~m;m=m/3;// m=(B-1)/3
c=0;t1=t2=t3=acc=0;
    umul_ppmm(dx,ax,xp[0],m);
    SUB(c,acc,0,t1);
    ADC(c,t2,0,ax,c);
    ADC(c,t3,0,dx,c);
    ASSERT(c==0);
    t1=t2;t2=t3;
for(j=1;j<=n-1;j++)
   {
    t3=0;
    umul_ppmm(dx,ax,xp[j],m);
    SUB(c,acc,acc,t1);
    qp[j-1]=acc;
    ADC(c,t2,t2,ax,c);
    ADC(c,t3,t3,dx,c);
    ASSERT(c==0);
    t1=t2;t2=t3;
   }
    SUB(c,acc,acc,t1);
    qp[n-1]=acc;
    ADC(c,t2,t2,0,c);
    t=(t2-acc)*3;
// return next quotient*-3    
return t;}   // so  (xp,n) = (qp,n)*3 -ret*B^n    and 0 <= ret < 3


/*
Looking at the above submul type function we can see that for each quotient limb we 
subtract the high part of one mul and the low part of the next mul from the accumulator.
*/

mp_limb_t	divexact3_direct(mp_ptr qp,mp_ptr xp,mp_size_t n)
{int j;mp_limb_t c,m,acc,ax,dx;

ASSERT(n>0);ASSERT_MPN(xp,n);ASSERT(MPN_SAME_OR_SEPARATE_P(qp,xp,n));
m=0;m=~m;m=m/3;// m=(B-1)/3
c=0;t1=t2=t3=acc=0;
for(j=0;j<=n-1;j++)
   {
    umul_ppmm(dx,ax,xp[j],m);
    SBB(c,acc,acc,ax,c);
    qp[j]=acc;
    SBB(c,acc,acc,dx,c);
   }
    SBB(c,acc,acc,0,c);
// return next quotient*-3    
return acc*-3;}   // so  (xp,n) = (qp,n)*3 -ret*B^n    and 0 <= ret < 3

/*
   Using supernatural powers we discover that the second carry is always zero 
   , ie like below , leading to the used algorithm

*/

mp_limb_t	divexact3_byluck(mp_ptr qp,mp_ptr xp,mp_size_t n)
{int j;mp_limb_t c,m,acc,ax,dx;

ASSERT(n>0);ASSERT_MPN(xp,n);ASSERT(MPN_SAME_OR_SEPARATE_P(qp,xp,n));
m=0;m=~m;m=m/3;// m=(B-1)/3
c=0;acc=0;
for(j=0;j<=n-1;j++)
   {   
    umul_ppmm(dx,ax,xp[j],m);	// line 1
    SUB(c,acc,acc,ax);		// line 2
    qp[j]=acc;			// line 3
    SBB(c,acc,acc,dx,c);	// line 4
    if(c!=0){printf("c not zero\n");abort();}
   }
// return next quotient*-3    
return acc*-3;}   // so  (xp,n) = (qp,n)*3 -ret*B^n    and 0 <= ret < 3

/*
  Not wanting to rely on the above comments , a proof below

*/
#endif

/*

Consider one iteration

let m=(B-1)/3

at line 1  acc=m*some_ret 

write  xp[j]=3*p+d    with  0 < d <= 3      ie division but with remainder+1  and p<(B-1)/3=m

dx:ax = xp[j]*m = (3p+d)*(B-1)/3 = p(B-1)+d*(B-1)/3 = pB+(d(B-1)/3-p) so dx=p and ax=d(B-1)/3-p    (*)

at line 2 we have two cases if (1) ax>acc  or (2) ax<=acc
and we want to show that the carry at line 4 is zero , ie dx+c<=acc 
so we get 
case (1)   show that dx+1<=acc-ax+B  ie  ax+dx <= B-1+acc
case (2)   show that dx+0<=acc-ax    ie  ax+dx <=     acc

from (*) ax+dx=d(B-1)/3 = d*m <= 3(B-1)/3=B-1  

so case (1) is OK

case (2) says ax<=acc=m*r for some r : 0 <= r < 3
             as ax+dx=m*d   so r<=d 
             and as dx=p<m so d<r+1   therefore d=r so ax+dx=mr=acc<=acc
             so case is OK
*/
