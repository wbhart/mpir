/* toom_eval_pm1

Copyright 2011 The Code Cavern

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

// k degree poly so have k+1 coeffs and first k are size n
// k>3 so we can do the first add unconditionally 
int	mpn_toom_eval_pm1(mp_ptr pp,mp_ptr mp,unsigned int k,mp_srcptr xp,mp_size_t n,mp_size_t m,mp_ptr tp)
{int isneg=0;unsigned int i;

ASSERT(k>3);ASSERT(n>=m);ASSERT(m>0);ASSERT_MPN(xp,n*k+m);
//ASSERT_SPACE(pp,n+1);ASSERT_SPACE(mp,n+1);ASSERT_SPACE(tp,n+1);
ASSERT(!MPN_OVERLAP_P(pp,n+1,mp,n+1));ASSERT(!MPN_OVERLAP_P(pp,n+1,xp,n*k+m));ASSERT(!MPN_OVERLAP_P(pp,n+1,tp,n+1));
ASSERT(!MPN_OVERLAP_P(mp,n+1,xp,n*k+m));ASSERT(!MPN_OVERLAP_P(xp,n*k+m,tp,n+1));
#if ! HAVE_NATIVE_mpn_sumdiff_n
ASSERT(!MPN_OVERLAP_P(mp,n+1,tp,n+1));
#endif
#if HAVE_NATIVE_mpn_addadd_n
if(k==4){pp[n]=mpn_add_n(pp,xp,xp+2*n,n);tp[n]=mpn_add_n(tp,xp+n,xp+3*n,n);}else
if(k==5){pp[n]=mpn_addadd_n(pp,xp,xp+2*n,xp+4*n,n);tp[n]=mpn_add_n(tp,xp+n,xp+3*n,n);}else
  {pp[n]=mpn_addadd_n(pp,xp,xp+2*n,xp+4*n,n);tp[n]=mpn_addadd_n(tp,xp+n,xp+3*n,xp+5*n,n);
   for(i=7;i<k-2;i+=4){pp[n]+=mpn_addadd_n(pp,pp,xp+(i-1)*n,xp+(i+1)*n,n);tp[n]+=mpn_addadd_n(tp,tp,xp+i*n,xp+(i+2)*n,n);}
   if(k%4==3){pp[n]+=mpn_add_n(pp,pp,xp+(k-1)*n,n);}
   if(k%4==0){pp[n]+=mpn_add_n(pp,pp,xp+(k-2)*n,n);tp[n]+=mpn_add_n(tp,tp,xp+(k-1)*n,n);}
   if(k%4==1){pp[n]+=mpn_addadd_n(pp,pp,xp+(k-3)*n,xp+(k-1)*n,n);tp[n]+=mpn_add_n(tp,tp,xp+(k-2)*n,n);}}
if(k%2==0){pp[n]+=mpn_add(pp,pp,n,xp+k*n,m);}else{tp[n]+=mpn_add(tp,tp,n,xp+k*n,m);}
#else
// pp is xp+0 xp+2n xp+4n xp+6n ... xp+jn where j<=k-1
// mp is xp+1 xp+3n xp+5n xp+7n ... xp+jn where j<=k-1
pp[n]=mpn_add_n(pp,xp,xp+2*n,n);tp[n]=mpn_add_n(tp,xp+n,xp+3*n,n);
for(i=5;i<k;i+=2){pp[n]+=mpn_add_n(pp,pp,xp+(i-1)*n,n);tp[n]+=mpn_add_n(tp,tp,xp+i*n,n);}
if(k%2==1){pp[n]+=mpn_add_n(pp,pp,xp+(k-1)*n,n);tp[n]+=mpn_add(tp,tp,n,xp+k*n,m);}else{pp[n]+=mpn_add(pp,pp,n,xp+k*n,m);}
#endif
if(mpn_cmp(tp,pp,n+1)>0)isneg=-1;
#if HAVE_NATIVE_mpn_sumdiff_n
if(isneg==0){mpn_sumdiff_n(pp,mp,pp,tp,n+1);}else{mpn_sumdiff_n(pp,mp,tp,pp,n+1);}
#else
if(isneg==0){mpn_sub_n(mp,pp,tp,n+1);}else{mpn_sub_n(mp,tp,pp,n+1);}
mpn_add_n(pp,pp,tp,n+1);  
#endif
return isneg;}
