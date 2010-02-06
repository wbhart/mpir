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

// d is mormalized
#define udiv_inverse(i,d)  do{mp_limb_t __X;udiv_qrnnd(i,__X,~(d),GMP_LIMB_MAX,d);}while(0)

// note double lenght inverse can easily be calculated from single length inverse
// d is normalize
#define udiv_double_inverse(ih,il,d) \
  do{mp_limb_t __X;udiv_qrnnd(ih,__X,~(d),GMP_LIMB_MAX,d);udiv_qrnnd(il,__X,__X,GMP_LIMB_MAX,d);}while(0)





// set to 1=store or 0=not store
#define STORE_QUOTIENT 1
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
mp_limb_t	mpn_divrem_euclidean_qr_1(mp_ptr qp,mp_size_t qxn, mp_srcptr xp,mp_size_t n,mp_limb_t d)
#else
mp_limb_t	mpn_divrem_euclidean_r_1(mp_srcptr xp,mp_size_t n,mp_limb_t d)
#endif
{mp_size_t j;
 mp_limb_t r=0,s=0,h,l,q,i;

#if STORE_QUOTIENT
ASSERT_ALWAYS(qxn==0);
#endif

ASSERT(n>0);ASSERT(d!=0);ASSERT_MPN(xp,n);
#if STORE_QUOTIENT
ASSERT(MPN_SAME_OR_SEPARATE_P(qp,xp,n));
#endif
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
