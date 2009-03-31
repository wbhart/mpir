/*
     Copyright 2009 Jason Moxham

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


// basic divrem_hensel_1
// divisor is 1 limb and odd
mp_limb_t	mpn_divrem_hensel_1(mp_ptr qp,mp_ptr xp,mp_size_t n,mp_limb_t d,mp_limb_t m)
{int j;mp_limb_t c,h,q,dummy,h1,t;

ASSERT(n>0);ASSERT_MPN(xp,n);ASSERT(MPN_SAME_OR_SEPARATE_P(qp,xp,n));
ASSERT(m*d==1);//modlimb_invert(m,d);
c=0;h=0;t=0;
for(j=0;j<=n-1;j++)
   {h1=xp[j];
    t=h+c;if(t>h1){h1=h1-t;c=1;}else{h1=h1-t;c=0;}// set borrow to c ; sbb t,h1 ; set c to borrow
    q=h1*m;
    qp[j]=q;
    umul_ppmm(h,dummy,q,d);
    ASSERT(dummy==h1);}
return h+c;}   // so  (xp,n) = (qp,n)*d -ret*B^n    and 0 <= ret < d

// basic divexact
mp_limb_t	divexact_shiftin(mp_ptr qp,mp_ptr xp,mp_size_t n,mp_limb_t d)
{int j,s;mp_limb_t c,h,q,dummy,h1,t,m;

ASSERT(n>0);ASSERT(d!=0);ASSERT_MPN(xp,n);ASSERT(MPN_SAME_OR_SEPARATE_P(qp,xp,n));
count_trailing_zeros(s,d);d=d>>s;modlimb_invert(m,d);
//shift local xp right by s
c=0;h=0;t=0;
for(j=0;j<=n-2;j++)
   {h1=(xp[j]>>s)|(xp[j+1]<<(GMP_LIMB_BITS-1-s)<<1);
    t=h+c;if(t>h1){h1=h1-t;c=1;}else{h1=h1-t;c=0;}// set borrow to c ; sbb t,h1 ; set c to borrow
    q=h1*m;
    qp[j]=q;
    umul_ppmm(h,dummy,q,d);
    ASSERT(dummy==h1);}
h1=xp[n-1]>>s;
t=h+c;if(t>h1){h1=h1-t;c=1;}else{h1=h1-t;c=0;}
q=h1*m;qp[n-1]=q;
umul_ppmm(h,dummy,q,d);
t=h+c;
return t;}   
// so  (xp,n)  = low_s_bitsof(xp[0]) + (qp,n)*(d) -(ret<<s)*B^n    and 0 <= ret < d/2^s

// basic divexact
mp_limb_t	divexact_shiftout(mp_ptr qp,mp_ptr xp,mp_size_t n,mp_limb_t d)
{int j,s;mp_limb_t c,h,q,dummy,h1,t,m,qo,qb;

ASSERT(n>0);ASSERT(d!=0);ASSERT_MPN(xp,n);ASSERT(MPN_SAME_OR_SEPARATE_P(qp,xp,n));
count_trailing_zeros(s,d);d=d>>s;modlimb_invert(m,d);
h1=xp[0];q=h1*m;
qo=q>>s;qb=(q<<(GMP_LIMB_BITS-1-s)<<1);
qb=(qb>>(GMP_LIMB_BITS-1-s)>>1);// ie qb is low s bits of q
umul_ppmm(h,dummy,q,d);
c=0;
for(j=1;j<=n-1;j++)
   {h1=xp[j];
    t=h+c;if(t>h1){h1=h1-t;c=1;}else{h1=h1-t;c=0;}// set borrow to c ; sbb t,h1 ; set c to borrow
    q=h1*m;
    qo=qo|(q<<(GMP_LIMB_BITS-1-s)<<1);
    qp[j-1]=qo;
    qo=q>>s;
    umul_ppmm(h,dummy,q,d);
    ASSERT(dummy==h1);}
qp[n-1]=qo;
t=h+c;  
t=(t<<s)+qb;
return t;}   
// so   (xp,n) = low_s_bitsof(ret)*d/2^s + (qp,n)*(d) - (ret>>s)*B^n    and 0 <= (ret>>s) < d/2^s



int main(void)
{mp_limb_t  qp[2000],xp[2000],yp[2000],tp[2000],d,m,r1,r2;mp_size_t s,n,j;unsigned long t1;


for(j=0;j<100;j++){
for(n=1;n<1000;n+=1)
   {mpn_random(&d,1);if(d==0)continue;
    mpn_random(xp,n);if(j>50)xp[n-1]=mpn_mul_1(xp,xp,n-1,d);
    count_trailing_zeros(s,d);
/*    
    r1=divexact_basic(qp,xp,n,d);
     // so  (xp,n) = (qp,n)*d -ret*B^n    and 0 <= ret < d
    r2=mpn_mul_1(tp,qp,n,d);
    if(r1!=r2){printf("error 1\n");abort();}
    if(mpn_cmp(tp,xp,n)!=0){printf("error 2\n");abort();}
*/
/*
    r1=divexact_shiftin(qp,xp,n,d);
    // so  (xp,n)  = low_s_bitsof(xp[0]) + (qp,n)*(d) -(ret<<s)*B^n    and 0 <= ret < d/2^s
    r2=mpn_mul_1(tp,qp,n,d);
    m=xp[0];m<<=63-s;m<<=1;m>>=63-s;m>>=1;
    if(mpn_add_1(tp,tp,n,m)!=0){printf("error 3a\n");abort();}
    if((r1<<s)!=r2){printf("error 1a\n");abort();}
    if(mpn_cmp(tp,xp,n)!=0){printf("error 2a %d\n",n);abort();}
*/
/*
    r1=divexact_shiftout(qp,xp,n,d);
    // so   (xp,n) = low_s_bitsof(ret)*d/2^s + (qp,n)*(d) - (ret>>s)*B^n    and 0 <= (ret>>s) < d/2^s
    r2=mpn_mul_1(tp,qp,n,d);
    m=r1;m<<=63-s;m<<=1;m>>=63-s;m>>=1;
    r2+=mpn_add_1(tp,tp,n,m*(d>>s));    
    if((r1>>s)!=r2){printf("error 1b\n");abort();}
    if((r1>>s)>(d>>s)){printf("error 3b\n");abort();}
    if(mpn_cmp(tp,xp,n)!=0){printf("error 2b\n");abort();}
*/
    
    modlimb_invert(m,d>>s);
    r1=jaydivexact(qp,xp,n,d,m);
    // so   (xp,n) = low_s_bitsof(ret)*d/2^s + (qp,n)*(d) - (ret>>s)*B^n    and 0 <= (ret>>s) < d/2^s
    r2=mpn_mul_1(tp,qp,n,d);
    m=r1;m<<=63-s;m<<=1;m>>=63-s;m>>=1;
    r2+=mpn_add_1(tp,tp,n,m*(d>>s));    
    if((r1>>s)!=r2){printf("error 1c\n");abort();}
    if((r1>>s)>(d>>s)){printf("error 3c\n");abort();}
    if(mpn_cmp(tp,xp,n)!=0){printf("error 2c\n");abort();}  
  
   }}  

d=242354423522;
count_trailing_zeros(s,d);
modlimb_invert(m,d>>s);   
t1=rdtsc();jaydivexact(qp,xp,1000,d,m);t1=rdtsc()-t1;
t1=rdtsc();jaydivexact(qp,xp,1000,d,m);t1=rdtsc()-t1;
t1=rdtsc();jaydivexact(qp,xp,1000,d,m);t1=rdtsc()-t1;
t1=rdtsc();jaydivexact(qp,xp,1000,d,m);t1=rdtsc()-t1;
printf("time %d\n",t1);
   
return 0;}



/*


mp_limb_t	jaybidivexact(mp_ptr qp,mp_ptr xp,mp_size_t n,mp_limb_t d,mp_limb_t m)
{int j;mp_limb_t c,h,q,dummy,h1,t,k,hd,ld,qd;

ASSERT(n>0);ASSERT(d!=0);ASSERT_MPN(xp,n);ASSERT(MPN_SAME_OR_SEPARATE_P(qp,xp,n));
ASSERT(d%2==1);
// if d is even then either shift the input xp or the output qp
// output is better as not on depandant path , and for div test dont need to do it
ASSERT(n%2==0);k=n/2;
c=0;h=0;t=0;hd=0;
for(j=0;j<=k-1;j++)
   {h1=xp[j];ld=xp[n-1-j]
    if(t>h1){h1=h1-t;c=1;}else{h1=h1-t;c=0;}
    q=h1*m;
    udiv_qrnnd(qd,dummy,hd,ld,d);
    hd=ld-qd*d;
    qp[n-1-j]=qd;    
    qp[j]=q;
    umul_ppmm(h,dummy,q,d);
    t=h+c;
  }
return hd-t;}    //  so (xp,n) = (qp,n)*d +(hd-t)*B^k   and          d divides xp <=>  ret=0
*/
