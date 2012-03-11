/*   Copyright 2012 The Code cavern

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

mp_limb_t	mpn_sumdiff_n(mp_ptr s,mp_ptr d,mp_srcptr x,mp_srcptr y,mp_size_t n)
{mp_limb_t ret;mp_ptr t;

ASSERT(n>0);
ASSERT_MPN(x,n);ASSERT_MPN(y,n);//ASSERT_SPACE(s,n);ASSERT_SPACE(d,n);
ASSERT(MPN_SAME_OR_SEPARATE_P(s,x,n));
ASSERT(MPN_SAME_OR_SEPARATE_P(s,y,n));
ASSERT(MPN_SAME_OR_SEPARATE_P(d,x,n));
ASSERT(MPN_SAME_OR_SEPARATE_P(d,y,n));
ASSERT(!MPN_OVERLAP_P(s,n,d,n));

if( (s==x && d==y)||(s==y && d==x) )
  {t=__GMP_ALLOCATE_FUNC_LIMBS(n);
   ret=mpn_sub_n(t,x,y,n);
   ret+=2*mpn_add_n(s,x,y,n);
   MPN_COPY(d,t,n);
   __GMP_FREE_FUNC_LIMBS(t,n);
   return ret;}
if(s==x || s==y)
  {ret=mpn_sub_n(d,x,y,n);
   ret+=2*mpn_add_n(s,x,y,n);
   return ret;}
ret=2*mpn_add_n(s,x,y,n);
ret+=mpn_sub_n(d,x,y,n);
return ret;}
