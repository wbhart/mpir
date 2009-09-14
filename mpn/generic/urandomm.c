/*
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

void mpn_urandomm(mp_ptr rp, gmp_randstate_t rnd, mp_srcptr mp,mp_size_t n)
{mp_size_t c,b;

ASSERT(n>0);ASSERT_MPN(mp,n);ASSERT(mp[n-1]!=0);
count_leading_zeros(c,mp[n-1]);
c=GMP_LIMB_BITS-c;b=GMP_NUMB_BITS*(n-1)+c;
do{_gmp_rand(rp,rnd,b);}while(mpn_cmp(rp,mp,n)>=0);
return;}
