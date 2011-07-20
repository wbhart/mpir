/* mpn_mulmod_2expm1 

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

// (xp,n)=(yp,n)*(zp,n) % 2^b-1  
// needs (tp,2n) temp space , everything reduced mod 2^b 
// inputs,outputs not fully reduced
// NOTE: 2n is not the same as 2b rounded up to nearest limb
// NOTE: not reduced fully means the representation is redundant , although only 0 has two representations ie 0 and 2^b-1 
inline static void
mpn_mulmod_2expm1_basecase (mp_ptr xp, mp_srcptr yp, mp_srcptr zp,
			    unsigned long b, mp_ptr tp)
{
  mp_size_t n, k;
  mp_limb_t c;

  n = BITS_TO_LIMBS (b);
  k = GMP_NUMB_BITS * n - b;
  ASSERT (n > 0);
  ASSERT_MPN (yp, n);
  ASSERT_MPN (zp, n);
  ASSERT (!MPN_OVERLAP_P (tp, 2 * n, yp, n));
  ASSERT (!MPN_OVERLAP_P (tp, 2 * n, zp, n));
  ASSERT (MPN_SAME_OR_SEPARATE_P (xp, tp, n));
  ASSERT (MPN_SAME_OR_SEPARATE_P (xp, tp + n, n));
  ASSERT (k == 0 || yp[n - 1] >> (GMP_NUMB_BITS - k) == 0);
  ASSERT (k == 0 || zp[n - 1] >> (GMP_NUMB_BITS - k) == 0);
  mpn_mul_n (tp, yp, zp, n);	// n is small is could use mpn_mul_basecase , and save a fn call , but this code is also used for large n when b is odd
  if (k == 0)
    {
      c = mpn_add_n (xp, tp, tp + n, n);
      MPN_INCR_U (xp, n, c);
      return;
    }
  c = tp[n - 1];
  tp[n - 1] &= GMP_NUMB_MASK >> k;
#if HAVE_NATIVE_mpn_addlsh_nc
  ASSERT_NOCARRY (mpn_addlsh_nc (xp, tp, tp + n, n, k, c));
#else
  {
    mp_limb_t c1;
    c1 = mpn_lshift (tp + n, tp + n, n, k);
    tp[n] |= c >> (GMP_NUMB_BITS - k);
    c = mpn_add_n (xp, tp, tp + n, n) + c1;
    ASSERT (c == 0);
  }
#endif
  c = xp[n - 1] >> (GMP_NUMB_BITS - k);
  xp[n - 1] &= GMP_NUMB_MASK >> k;
  MPN_INCR_U (xp, n, c);
  return;
}

/* Improvements 
 use 2^3n-1 , 2^4n+2+1 factorizations or others
 shift by byte values , ie copy unaligned
 addsublshift   or a   rshift1_lshift   dual_rshift ie for both the mods
 separate out the k=0 case 
 unroll the recursion
 if yp=zp ie a square then could do better
 different thresholds depending on how many twos divide b
*/
// tp requires 5(n+lg(b)) space 
void
mpn_mulmod_2expm1 (mp_ptr xp, mp_ptr yp, mp_ptr zp, unsigned long b,
		   mp_ptr tp)
{
  mp_size_t h, n, m, k;
  int bor, c, c1, c2;
  mp_ptr S, D, typm, tzpm, typp, tzpp, temp;
  mp_limb_t car, Dm, car1;

// want y*z %(2^n-1)         // want y*z%(2^(2m)-1) = y*z%((2^m-1)(2^m+1)) // use CRT 
// note (2^m-1)*2^(m-1) == 1 mod 2^m+1        // note (2^m+1)*2^(m-1) == 1 mod 2^m-1
// let A=y*z%(2^m-1)  B=y*z%(2^m+1)        // then  A(2^m+1)2^(m-1)+B(2^m-1)2^(m-1) = ((A-B)+(A+B)2^m)2^(m-1)
  ASSERT (b > 0);
  n = BITS_TO_LIMBS (b);
  if (b % 2 == 1 || BELOW_THRESHOLD (n, MULMOD_2EXPM1_THRESHOLD))
    {
      mpn_mulmod_2expm1_basecase (xp, yp, zp, b, tp);
      return;
    }
  h = b / 2;
  m = BITS_TO_LIMBS (h);
  k = GMP_NUMB_BITS * m - h;	// if k==0 then n=2*m
  ASSERT_MPN (yp, n);
  ASSERT_MPN (zp, n);
  ASSERT (GMP_NUMB_BITS * n - b == 0
	  || yp[n - 1] >> (GMP_NUMB_BITS - (GMP_NUMB_BITS * n - b)) == 0);
  ASSERT (GMP_NUMB_BITS * n - b == 0
	  || zp[n - 1] >> (GMP_NUMB_BITS - (GMP_NUMB_BITS * n - b)) == 0);
  ASSERT (!MPN_OVERLAP_P (yp, n, tp, 5 * n));
  ASSERT (!MPN_OVERLAP_P (zp, n, tp, 5 * n));
  ASSERT (!MPN_OVERLAP_P (xp, n, tp, 5 * n));
  ASSERT (!MPN_OVERLAP_P (xp, n, yp, n));
  ASSERT (!MPN_OVERLAP_P (xp, n, zp, n));
// S,D,typm,tzpm,typp,tzpp all require m limbs
  S = xp;
  D = tp;
  typm = tp + m;
  typp = typm + m;
  tzpm = typp + m;
  tzpp = tzpm + m;
  temp = tzpp + m;
  if (k == 0)
    D = xp + m;
//mpn_mod_2expm1(typm,yp,n,h);c1=mpn_mod_2expp1(typp,yp,n,h);
  if (k == 0)
    {
      c = mpn_sumdiff_n (typm, typp, yp, yp + m, m);
      MPN_INCR_U (typm, m, c >> 1);
      c1 = mpn_add_1 (typp, typp, m, c & 1);
    }
  else
    {
      mpn_rshift (typp, yp + m - 1, m, GMP_NUMB_BITS - k);
      if (n == 2 * m)
	{
	  typp[m - 1] |= yp[2 * m - 1] << k;
	  ASSERT (yp[2 * m - 1] >> (GMP_NUMB_BITS - k) == 0);
	}
      ASSERT (typp[m - 1] >> (GMP_NUMB_BITS - k) == 0);	// have h bits
      car = yp[m - 1];
      yp[m - 1] &= GMP_NUMB_MASK >> k;
      ASSERT (yp[m - 1] >> (GMP_NUMB_BITS - k) == 0);	// have h bits   
      c1 = mpn_sumdiff_n (typm, typp, yp, typp, m);
      c = typm[m - 1] >> (GMP_NUMB_BITS - k);
      yp[m - 1] = car;
      MPN_INCR_U (typm, m, c);
      c1 = mpn_add_1 (typp, typp, m, c1);
      typm[m - 1] &= GMP_NUMB_MASK >> k;
      typp[m - 1] &= GMP_NUMB_MASK >> k;
    }
//mpn_mod_2expm1(tzpm,zp,n,h);c2=mpn_mod_2expp1(tzpp,zp,n,h);
  if (k == 0)
    {
      c = mpn_sumdiff_n (tzpm, tzpp, zp, zp + m, m);
      MPN_INCR_U (tzpm, m, c >> 1);
      c2 = mpn_add_1 (tzpp, tzpp, m, c & 1);
    }
  else
    {
      mpn_rshift (tzpp, zp + m - 1, m, GMP_NUMB_BITS - k);
      if (n == 2 * m)
	{
	  tzpp[m - 1] |= zp[2 * m - 1] << k;
	  ASSERT (zp[2 * m - 1] >> (GMP_NUMB_BITS - k) == 0);
	}
      ASSERT (tzpp[m - 1] >> (GMP_NUMB_BITS - k) == 0);	// have h bits
      car = zp[m - 1];
      zp[m - 1] &= GMP_NUMB_MASK >> k;
      ASSERT (zp[m - 1] >> (GMP_NUMB_BITS - k) == 0);	// have h bits
      c2 = mpn_sumdiff_n (tzpm, tzpp, zp, tzpp, m);
      c = tzpm[m - 1] >> (GMP_NUMB_BITS - k);
      zp[m - 1] = car;
      MPN_INCR_U (tzpm, m, c);
      c2 = mpn_add_1 (tzpp, tzpp, m, c2);
      tzpm[m - 1] &= GMP_NUMB_MASK >> k;
      tzpp[m - 1] &= GMP_NUMB_MASK >> k;
    }
  mpn_mulmod_2expm1 (S, typm, tzpm, h, temp);	// unroll this recursion  S=A rename
  c = mpn_mulmod_2expp1 (D, typp, tzpp, c1 * 2 + c2, h, temp);	// D=B rename
  if (LIKELY (c == 0))
    {
      c1 = mpn_sumdiff_n (S, D, S, D, m);
      bor = c1 & 1;
      c = c1 >> 1;
      D[m - 1] &= GMP_NUMB_MASK >> k;
      if (k != 0 && S[m - 1] >> (GMP_NUMB_BITS - k) != 0)
	c = 1;
      S[m - 1] &= GMP_NUMB_MASK >> k;
    }
  else
    {
      c = 1;
      bor = 1;
      MPN_COPY (D, S, m);
    }
  bor = mpn_sub_1 (S, S, m, bor);
  S[m - 1] &= GMP_NUMB_MASK >> k;
  if (bor == 0)
    {
      c = mpn_add_1 (D, D, m, c);
      if (k != 0 && D[m - 1] >> (GMP_NUMB_BITS - k) != 0)
	c = 1;
      D[m - 1] &= GMP_NUMB_MASK >> k;
      if (c != 0)
	S[0] |= 1;
    }
  if (k == 0)
    {
      car = mpn_half (xp, n);
      xp[n - 1] |= car;
    }				// C sequence pt rule
  else
    {
      car = mpn_half (xp, m);
      car1 = xp[m - 1];
      if (GMP_NUMB_BITS - k - 1 != 0)
	{
	  Dm = mpn_lshift (xp + m - 1, D, m, GMP_NUMB_BITS - k - 1);
	}
      else
	{
	  Dm = 0;
	  MPN_COPY (xp + m - 1, D, m);
	}
      xp[m - 1] |= car1;
      if (2 * m == n)
	xp[n - 1] = Dm;
      xp[n - 1] |= car >> (GMP_NUMB_BITS * n - b);
    }
  return;
}


/*

DO WE STILL WANT TO KEEP THIS JUNK???????


// (xp,n) = (yp,yn)%(2^(GMP_NUMB_BITS*n)-1) not fully reduced
void	mpn_mod_2expm1_limb(mp_ptr xp,mp_srcptr yp,mp_size_t yn,mp_size_t n)
{mp_limb_t c=0;

ASSERT(yn>0);ASSERT(n>0);ASSERT_MPN(yp,yn);ASSERT(MPN_SAME_OR_SEPARATE2_P(xp,n,yp,yn));
if(yn<=n)
  {if(xp!=yp)MPN_COPY(xp,yp,yn);
   MPN_ZERO(xp+yn,n-yn);return;}
if(xp!=yp)MPN_COPY(xp,yp,n);
yp+=n;yn-=n;
while(yn>=n){c+=mpn_add_n(xp,xp,yp,n);yp+=n;yn-=n;}
if(yn>0)c+=mpn_add(xp,xp,n,yp,yn);
c=mpn_add_1(xp,xp,n,c);
ASSERT_NOCARRY(mpn_add_1(xp,xp,n,c));
return;}

// (xp,n) = (yp,yn)%(2^b-1) not fully reduced , reduced to nearest limb
void	mpn_mod_2expm1(mp_ptr xp,mp_srcptr yp,mp_size_t yn,unsigned long b)
{mp_limb_t c=0;mp_size_t n;

ASSERT(yn>0);//ASSERT(n>0);
ASSERT_MPN(yp,yn);//ASSERT(MPN_SAME_OR_SEPARATE2_P(xp,n,yp,yn));
n=BITS_TO_LIMBS(b);
if(b%GMP_NUMB_BITS==0){mpn_mod_2expm1_limb(xp,yp,yn,b/GMP_NUMB_BITS);return;}
if(yn<=n)
  {if(xp!=yp)MPN_COPY(xp,yp,yn);
   MPN_ZERO(xp+yn,n-yn);return;}
// so yn > n 
???????????????????

if(xp!=yp)MPN_COPY(xp,yp,n);
yp+=n;yn-=n;
while(yn>=n){c+=mpn_add_n(xp,xp,yp,n);yp+=n;yn-=n;}
if(yn>0)c+=mpn_add(xp,xp,n,yp,yn);
c=mpn_add_1(xp,xp,n,c);
ASSERT_NOCARRY(mpn_add_1(xp,xp,n,c));
return;}

// (xp,n)+ret*2^(GMP_NUMB_BITS*n) = (yp,yn)%(2^(GMP_NUMB_BITS*n)+1) not fully reduced
// ret =0 or 1
mp_limb_t	mpn_mod_2expp1_limb(mp_ptr xp,mp_srcptr yp,mp_size_t yn,mp_size_t n)
{mp_size_t c=0,s=-1;

ASSERT(yn>0);ASSERT(n>0);ASSERT_MPN(yp,yn);ASSERT(MPN_SAME_OR_SEPARATE2_P(xp,n,yp,yn));
if(yn<=n)
  {if(xp!=yp)MPN_COPY(xp,yp,yn);
   MPN_ZERO(xp+yn,n-yn);return 0;}
if(xp!=yp)MPN_COPY(xp,yp,n);
yp+=n;yn-=n;
while(yn>=n)
     {if(s==1){c+=mpn_add_n(xp,xp,yp,n);}else{c-=mpn_sub_n(xp,xp,yp,n);}
      yp+=n;yn-=n;s=-s;}
if(yn>0){if(s==1){c+=mpn_add(xp,xp,n,yp,yn);}else{c-=mpn_sub(xp,xp,n,yp,yn);}}
if(c>=0){c=mpn_sub_1(xp,xp,n,c);}else{c=-c;}
c=mpn_add_1(xp,xp,n,c);
return c;}

void	mpn_mod_2expm1(mp_ptr xp,mp_ptr tp,mp_size_t m,unsigned long b)
{mp_limb_t c;mp_size_t n,k;mp_ptr ttp;
n=BITS_TO_LIMBS(b);k=GMP_NUMB_BITS*n-b;
ttp=__GMP_ALLOCATE_FUNC_LIMBS(2*n);MPN_ZERO(ttp,2*n);MPN_COPY(ttp,tp,m);
if(k==0){c=mpn_add_n(xp,ttp,ttp+n,n);MPN_INCR_U(xp,n,c);return;}
c=ttp[n-1];ttp[n-1]&=GMP_NUMB_MASK>>k;
ASSERT_NOCARRY(mpn_addlsh(xp,ttp,ttp+n,n,k,c));
c=xp[n-1]>>(GMP_NUMB_BITS-k);xp[n-1]&=GMP_NUMB_MASK>>k;
MPN_INCR_U(xp,n,c);return;}


int      mpn_mod_2expp1(mp_ptr xp,mp_ptr tp,mp_size_t m,unsigned long b)
{mp_size_t n,k;mp_limb_t c;mp_ptr ttp;

n=BITS_TO_LIMBS(b);k=GMP_NUMB_BITS*n-b;
ttp=__GMP_ALLOCATE_FUNC_LIMBS(2*n);MPN_ZERO(ttp,2*n);MPN_COPY(ttp,tp,m);
if(k==0){c=mpn_sub_n(xp,ttp,ttp+n,n);return mpn_add_1(xp,xp,n,c);}
c=ttp[n-1];ttp[n-1]&=GMP_NUMB_MASK>>k;
c=mpn_sublsh(xp,ttp,ttp+n,n,k,c);
c=mpn_add_1(xp,xp,n,c);
xp[n-1]&=GMP_NUMB_MASK>>k;
return c;}


void	mpn_mulmod_2expm1_new(mp_ptr xp,mp_ptr yp,mp_ptr zp,unsigned long b,mp_ptr tp)
{mp_size_t h,n,m,k;int bor,c,c1,c2;mp_ptr S,D,typm,tzpm,typp,tzpp,temp;mp_limb_t car;

// want y*z %(2^n-1)         // want y*z%(2^(2m)-1) = y*z%((2^m-1)(2^m+1)) // use CRT 
// note (2^m-1)*2^(m-1) == 1 mod 2^m+1        // note (2^m+1)*2^(m-1) == 1 mod 2^m-1
// let A=y*z%(2^m-1)  B=y*z%(2^m+1)        // then  A(2^m+1)2^(m-1)+B(2^m-1)2^(m-1) = ((A-B)+(A+B)2^m)2^(m-1)
if(b%2==1 || b<2){mpn_mulmod_2expm1(xp,yp,zp,b,tp);return;}
h=b/2;n=BITS_TO_LIMBS(b);m=BITS_TO_LIMBS(h);
S=__GMP_ALLOCATE_FUNC_LIMBS(m+1);D=__GMP_ALLOCATE_FUNC_LIMBS(m+1);
typm=__GMP_ALLOCATE_FUNC_LIMBS(m);tzpm=__GMP_ALLOCATE_FUNC_LIMBS(m);
typp=__GMP_ALLOCATE_FUNC_LIMBS(m);tzpp=__GMP_ALLOCATE_FUNC_LIMBS(m);temp=__GMP_ALLOCATE_FUNC_LIMBS(2*m);
k=GMP_NUMB_BITS*m-h;// if k==0 then n=2*m
//mpn_mod_2expm1(typm,yp,n,h);c1=mpn_mod_2expp1(typp,yp,n,h);
if(k==0){c=mpn_sumdiff_n(typm,typp,yp,yp+m,m);MPN_INCR_U(typm,m,c>>1);c1=mpn_add_1(typp,typp,m,c&1);}
else
  {MPN_COPY(typm,yp,m);typm[m-1]&=GMP_NUMB_MASK>>k;ASSERT(typm[m-1]>>(GMP_NUMB_BITS-k)==0);// have h bits
   mpn_rshift(typp,yp+m-1,m,GMP_NUMB_BITS-k);
   if(n==2*m){typp[m-1]|=yp[2*m-1]<<k;ASSERT(yp[2*m-1]>>(GMP_NUMB_BITS-k)==0);}         
   ASSERT(typp[m-1]>>(GMP_NUMB_BITS-k)==0);// have h bits
   c1=mpn_sumdiff_n(typm,typp,typm,typp,m);   
   c=typm[m-1]>>(GMP_NUMB_BITS-k);
   MPN_INCR_U(typm,m,c);   
   c1=mpn_add_1(typp,typp,m,c1);
   typm[m-1]&=GMP_NUMB_MASK>>k;
   typp[m-1]&=GMP_NUMB_MASK>>k;}
//mpn_mod_2expm1(tzpm,zp,n,h);c2=mpn_mod_2expp1(tzpp,zp,n,h);
if(k==0){c=mpn_sumdiff_n(tzpm,tzpp,zp,zp+m,m);MPN_INCR_U(tzpm,m,c>>1);c2=mpn_add_1(tzpp,tzpp,m,c&1);}
else
  {MPN_COPY(tzpm,zp,m);tzpm[m-1]&=GMP_NUMB_MASK>>k;ASSERT(tzpm[m-1]>>(GMP_NUMB_BITS-k)==0);// have h bits
   mpn_rshift(tzpp,zp+m-1,m,GMP_NUMB_BITS-k);
   if(n==2*m){tzpp[m-1]|=zp[2*m-1]<<k;ASSERT(zp[2*m-1]>>(GMP_NUMB_BITS-k)==0);}
   ASSERT(tzpp[m-1]>>(GMP_NUMB_BITS-k)==0);// have h bits
   c2=mpn_sumdiff_n(tzpm,tzpp,tzpm,tzpp,m);   
   c=tzpm[m-1]>>(GMP_NUMB_BITS-k);
   MPN_INCR_U(tzpm,m,c);   
   c2=mpn_add_1(tzpp,tzpp,m,c2);
   tzpm[m-1]&=GMP_NUMB_MASK>>k;
   tzpp[m-1]&=GMP_NUMB_MASK>>k;}

mpn_mulmod_2expm1_new(S,typm,tzpm,h,temp);// unroll this recursion  S=A rename
c=mpn_mulmod_2expp1(D,typp,tzpp,c1*2+c2,h,temp);	// D=B rename
__GMP_FREE_FUNC_LIMBS(typm,m);__GMP_FREE_FUNC_LIMBS(tzpm,m);__GMP_FREE_FUNC_LIMBS(typp,m);__GMP_FREE_FUNC_LIMBS(tzpp,m);
__GMP_FREE_FUNC_LIMBS(temp,2*m);
if(LIKELY(c==0))
  {c1=mpn_sumdiff_n(S,D,S,D,m);bor=c1&1;c=c1>>1;
   D[m-1]&=GMP_NUMB_MASK>>k;
   if(k!=0 && S[m-1]>>(GMP_NUMB_BITS-k)!=0){c=1;}
   S[m-1]&=GMP_NUMB_MASK>>k;}
else{c=1;bor=1;MPN_COPY(D,S,m);//MPN_COPY(S,S,m);
    }    
bor=mpn_sub_1(S,S,m,bor);
S[m-1]&=GMP_NUMB_MASK>>k;
if(bor==0)
  {// only got a possible carry left
   c=mpn_add_1(D,D,m,c);
   if(k!=0 && D[m-1]>>(GMP_NUMB_BITS-k)!=0){c=1;}
   D[m-1]&=GMP_NUMB_MASK>>k;
   if(c!=0)
     {// before D=111111 now D=00000
      // so A=11111,B=0    or   B=A+1    , if was A=111111,B=0 then A+B<2^h so no carry , therefore B=A+1 and had a borrow
      // S=A+B-borrow=2A+1-borrow=2A , so can add carry to it with no furthur carry
      S[0]|=1;// could just or| it in low bit      
     }
  }
else
  {// before S=0000  now S=1111 // so A+B=0 or 2^h  , if was 0 then D=A-B=0 no borrow so A+B=2^h  and carry was 1
   ASSERT(c==1);  // this new borrow cancles with old carry so nothing to do 
  }
// so join together D,S
MPN_COPY(xp,S,m);
if(k==0){MPN_COPY(xp+m,D,m);ASSERT(n==2*m);}
else{D[m]=mpn_lshift(D,D,m,GMP_NUMB_BITS-k);
     xp[m-1]|=D[0];MPN_COPY(xp+m,D+1,m-1);
     ASSERT(2*m>=n);
     if(D[m]!=0){xp[2*m-1]=D[m];}else{if(2*m==n)xp[n-1]=0;}}
car=mpn_rshift(xp,xp,n,1);
xp[n-1]|=car>>(GMP_NUMB_BITS*n-b);// rotate
__GMP_FREE_FUNC_LIMBS(S,m+1);__GMP_FREE_FUNC_LIMBS(D,m+1);
return;}


*/
