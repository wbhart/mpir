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

/*
  FIXME: Should use mpn_addmul_2 (and higher).
*/

/* (rp, xn + yn) = (xp, xn)*(yp, yn) mod B^n */
void
mpn_mullow_basecase (mp_ptr rp, mp_srcptr xp, mp_size_t xn, mp_srcptr yp,
		     mp_size_t yn, mp_size_t n)
{
  mp_limb_t c;
  mp_size_t i;

  /* 
     want some sort of threshold to call mpn_mul() etc 
     as for the mpn_mullow_n cases
  */
  ASSERT(0 < yn);
  ASSERT(yn <= xn);
  ASSERT(xn <= n);
  ASSERT(n <= xn + yn);
  ASSERT_MPN(xp, xn);
  ASSERT_MPN(yp, yn);
  ASSERT(!MPN_OVERLAP_P(rp, xn + yn, xp, xn));
  ASSERT(!MPN_OVERLAP_P(rp, xn + yn, yp, yn));
  
  rp[xn] = mpn_mul_1(rp, xp, xn, yp[0]);
  
  for (i = 1; i <= n - xn && i < yn; i++)
    rp[xn + i] = mpn_addmul_1(rp + i, xp, xn, yp[i]);
  
  for ( ; i > n - xn && i < yn; i++)
    mpn_addmul_1(rp + i, xp, n - i, yp[i]);

  return;
}
