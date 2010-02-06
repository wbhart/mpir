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

mp_limb_t mpn_mod_1_1_wrap(mp_srcptr xp,mp_size_t xn,mp_limb_t d)// in each round we hack off a limb from the body , ie k=1
{mp_limb_t crap,h,l,sh,sl,ret,i,c,ds,db[2],rem[2];
 mp_size_t j;
 
ASSERT_MPN(xp,xn);
if(xn==0)return 0;
if(xn==1)return xp[0]%d;
ASSERT(d-1<=GMP_LIMB_HIGHBIT);
count_leading_zeros(c,d);ds=d<<c;
invert_limb(i,ds);
udiv_qrnnd_preinv(crap,db[0],((mp_limb_t)1)<<c,0,ds,i);  // is B%ds
udiv_qrnnd_preinv(crap,db[1],db[0],0,ds,i);//  is B^2%ds  could calc indep of db[0]
db[0]>>=c;db[1]>>=c;// are now B^i %d

mpn_mod_1_1(rem,xp,xn,db);
/*
h=xp[xn-1];l=xp[xn-2];
for(j=xn-3;j>=0;j--)
   {umul_ppmm(sh,sl,l,db[0]);
    add_ssaaaa(sh,sl,sh,sl,0,xp[j]);
    umul_ppmm(h,l,h,db[1]);
    add_ssaaaa(h,l,h,l,sh,sl);}
umul_ppmm(sh,sl,h,db[0]);
add_ssaaaa(sh,sl,sh,sl,0,l);
*/
sh=rem[1];sl=rem[0];
ASSERT(sh<d);
udiv_qrnnd_preinv(crap,ret,(sh<<c)|((sl>>(GMP_LIMB_BITS-1-c))>>1),sl<<c,ds,i);
return ret>>c;}

mp_limb_t mpn_mod_1_2_wrap(mp_srcptr xp,mp_size_t xn,mp_limb_t d)// in each round we hack off two limbs from the body , ie k=2
{mp_limb_t crap,h,l,sh,sl,th,tl,i,ret,ds,c,db[3],rem[2];
 mp_size_t j;
 
ASSERT_MPN(xp,xn);
if(xn==0)return 0;
if(xn==1)return xp[0]%d;
umul_ppmm(h,l,d-1,3);ASSERT(h==0 || (h==1 && l==0));// ie (k+1)(d-1)<=B
count_leading_zeros(c,d);ds=d<<c;
invert_limb(i,ds);
udiv_qrnnd_preinv(crap,db[0],((mp_limb_t)1)<<c,0,ds,i);
udiv_qrnnd_preinv(crap,db[1],db[0],0,ds,i);db[0]>>=c;
udiv_qrnnd_preinv(crap,db[2],db[1],0,ds,i);db[1]>>=c;
db[2]>>=c;
mpn_mod_1_2(rem,xp,xn,db);
h=rem[1];l=rem[0];
/*
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
*/
ASSERT(h<d);
udiv_qrnnd_preinv(crap,ret,(h<<c)|((l>>(GMP_LIMB_BITS-1-c))>>1),l<<c,ds,i);
return ret>>c;}

mp_limb_t mpn_mod_1_3_wrap(mp_srcptr xp,mp_size_t xn,mp_limb_t d)// in each round we hack off 3 limbs from the body
{mp_limb_t crap,h,l,sh,sl,th,tl,i,ret,ds,c,db[4],rem[2];
 mp_size_t j,jj;
 
ASSERT_MPN(xp,xn);
if(xn==0)return 0;
if(xn==1)return xp[0]%d;
umul_ppmm(h,l,d-1,4);ASSERT(h==0 || (h==1 && l==0));// ie (k+1)(d-1)<=B
count_leading_zeros(c,d);ds=d<<c;
invert_limb(i,ds);
udiv_qrnnd_preinv(crap,db[0],((mp_limb_t)1)<<c,0,ds,i);
udiv_qrnnd_preinv(crap,db[1],db[0],0,ds,i);db[0]>>=c;
udiv_qrnnd_preinv(crap,db[2],db[1],0,ds,i);db[1]>>=c;
udiv_qrnnd_preinv(crap,db[3],db[2],0,ds,i);db[2]>>=c;
db[3]>>=c;
mpn_mod_1_3(rem,xp,xn,db);
h=rem[1];l=rem[0];
/*
tl=xp[xn-2];th=xp[xn-1];
for(j=xn-5;j>=0;j-=3)
   {umul_ppmm(sh,sl,xp[j+1],db[0]);
    add_ssaaaa(sh,sl,sh,sl,0,xp[j]);
    umul_ppmm(h,l,xp[j+2],db[1]);
    add_ssaaaa(sh,sl,sh,sl,h,l);
    umul_ppmm(h,l,tl,db[2]);
    add_ssaaaa(sh,sl,sh,sl,h,l);
    umul_ppmm(th,tl,th,db[3]);
    add_ssaaaa(th,tl,th,tl,sh,sl);}
if(j>-3)// we have at least three limbs to do still ie xp[0],...,tl,th
  {sh=0;sl=xp[0];jj=1;
   if(j==-1)
     {umul_ppmm(sh,sl,xp[1],db[0]);
      add_ssaaaa(sh,sl,sh,sl,0,xp[0]);
      jj=2;}
   umul_ppmm(h,l,tl,db[jj-1]);
   add_ssaaaa(sh,sl,sh,sl,h,l);
   umul_ppmm(th,tl,th,db[jj]);
   add_ssaaaa(th,tl,th,tl,sh,sl);}
umul_ppmm(h,l,th,db[0]);
add_ssaaaa(h,l,h,l,0,tl);
*/
ASSERT(h<d);
udiv_qrnnd_preinv(crap,ret,(h<<c)|((l>>(GMP_LIMB_BITS-1-c))>>1),l<<c,ds,i);
return ret>>c;}

// This is a generic version for k>=2
mp_limb_t mpn_mod_1_k(mp_srcptr xp,mp_size_t xn,mp_limb_t d,mp_size_t k)// in each round we hack off k limbs from the body
{mp_limb_t crap,h,l,sh,sl,th,tl,i,ret,ds,c,db[30];//need k+1 entries in array
 mp_size_t j,jj;
 
ASSERT_MPN(xp,xn);
if(xn==0)return 0;
if(xn==1)return xp[0]%d;
ASSERT(k>=2);
umul_ppmm(h,l,d-1,k+1);ASSERT(h==0 || (h==1 && l==0));// ie (k+1)(d-1)<=B
count_leading_zeros(c,d);ds=d<<c;
invert_limb(i,ds);
udiv_qrnnd_preinv(crap,db[0],((mp_limb_t)1)<<c,0,ds,i);
ASSERT_ALWAYS(k+1<=numberof(db));
for(j=1;j<=k;j++){udiv_qrnnd_preinv(crap,db[j],db[j-1],0,ds,i);db[j-1]>>=c;}// so db[j]=B^j %d
db[k]>>=c;tl=xp[xn-2];th=xp[xn-1];
for(j=xn-k-2;j>=0;j-=k)
   {umul_ppmm(sh,sl,xp[j+1],db[0]);
    add_ssaaaa(sh,sl,sh,sl,0,xp[j]);
    for(jj=2;jj<=k-1;jj++)
       {umul_ppmm(h,l,xp[j+jj],db[jj-1]);
        add_ssaaaa(sh,sl,sh,sl,h,l);}
    umul_ppmm(h,l,tl,db[k-1]);
    add_ssaaaa(sh,sl,sh,sl,h,l);
    umul_ppmm(th,tl,th,db[k]);
    add_ssaaaa(th,tl,th,tl,sh,sl);}
if(j+k>0)// we have at least three limbs to do still ie xp[0],...,tl,th
  {sh=0;sl=xp[0];
   for(jj=1;jj<j+k;jj++)
      {umul_ppmm(h,l,xp[jj],db[jj-1]);
       add_ssaaaa(sh,sl,sh,sl,h,l);}
   umul_ppmm(h,l,tl,db[jj-1]);
   add_ssaaaa(sh,sl,sh,sl,h,l);
   umul_ppmm(th,tl,th,db[jj]);
   add_ssaaaa(th,tl,th,tl,sh,sl);}
umul_ppmm(h,l,th,db[0]);
add_ssaaaa(h,l,h,l,0,tl);
ASSERT(h<d);
udiv_qrnnd_preinv(crap,ret,(h<<c)|(l>>(GMP_LIMB_BITS-c)),l<<c,ds,i);
return ret>>c;}


// d is mormalized
#define udiv_inverse(i,d)  do{mp_limb_t __X;udiv_qrnnd(i,__X,~(d),GMP_LIMB_MAX,d);}while(0)

// note double lenght inverse can easily be calculated from single length inverse
// d is normalize
#define udiv_double_inverse(ih,il,d) \
  do{mp_limb_t __X;udiv_qrnnd(ih,__X,~(d),GMP_LIMB_MAX,d);udiv_qrnnd(il,__X,__X,GMP_LIMB_MAX,d);}while(0)


// set to 1=store or 0=not store
#define STORE_QUOTIENT 0
// set to 0=udiv  1=gmp-preinv   2-barrett
#define UDIV_METHOD 1

#if UDIV_NEEDS_NORMALIZATION==1 || UDIV_METHOD==1
#define NORMALIZE 1
#else
#define NORMALIZE 0
#endif

#if UDIV_METHOD==0
#define UDIV(q,r,h,l,d,i)	udiv_qrnnd(q,r,h,l,d)
#endif

#if UDIV_METHOD==1
#define UDIV	udiv_qrnnd_preinv
#endif

#if UDIV_METHOD==2
#define UDIV	udiv_qrnnd_barrett
#endif

#if STORE_QUOTIENT
mp_limb_t	mpn_divrem_euclidean_qr_1(mp_ptr qp,mp_size_t qxn,mp_srcptr xp,mp_size_t n,mp_limb_t d)
#else
mp_limb_t	mpn_divrem_euclidean_r_1(mp_srcptr xp,mp_size_t n,mp_limb_t d)
#endif
{mp_size_t j;
 mp_limb_t r=0,s=0,h,l,q,i;

#if STORE_QUOTIENT
ASSET_ALWAYS(qxn==0);
#endif

ASSERT(n>0);ASSERT(d!=0);ASSERT_MPN(xp,n);
#if STORE_QUOTIENT
ASSERT(MPN_SAME_OR_SEPARATE_P(qp,xp,n));
#endif

if(d<=GMP_LIMB_HIGHBIT/2+1 && ABOVE_THRESHOLD(n,MOD_1_3_THRESHOLD))return mpn_mod_1_3_wrap(xp,n,d);
if(d<=MP_LIMB_T_MAX/3+1 && ABOVE_THRESHOLD(n,MOD_1_2_THRESHOLD))return mpn_mod_1_2_wrap(xp,n,d);
if(d<=GMP_LIMB_HIGHBIT+1 && ABOVE_THRESHOLD(n,MOD_1_1_THRESHOLD))return mpn_mod_1_1_wrap(xp,n,d);

// for n=1 or n=2 probably faster to do a special case
#if NORMALIZE==1
count_leading_zeros(s,d);d=d<<s;
invert_limb(i,d);
#endif
for(j=n-1;j>=0;j--)
   {l=xp[j];    // out dlimb is (h=r,l)
    h=(l>>((GMP_LIMB_BITS-1-s))>>1);l=l<<s;// shift dlimb left by s
    h=h+r;		// carry in
    UDIV(q,r,h,l,d,i);	// carry out , carry-out to carry-in is the critical latency bottleneck
    #if STORE_QUOTIENT
    qp[j]=q;
    #endif
   }
ASSERT((r<<(GMP_LIMB_BITS-1-s)<<1)==0);// ie bottom s bits of r are zero
r>>=s;
return r;}    // so (xp,n) = (qp,n)*d +r   and 0 <= r < d
/*
in hensel-div we use shiftout which means we can use mmx shifting and dont need to always use it
in euclid-div shiftout needs a final div for the remainder
*/
