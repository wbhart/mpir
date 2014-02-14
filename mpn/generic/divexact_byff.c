/*  mpn_divexact_byff

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

/*
	Where (xp,n) = (qp, n)*(B - 1) - ret*B^n and 0 <= ret < B - 1, B = 2^GMP_NUMB_BITS
	
	This is the standard divexact algorithm with simplifications for the divisior B-1
*/
mp_limb_t
mpn_divexact_byff (mp_ptr qp, mp_srcptr xp, mp_size_t n)
{
  mp_size_t j;
  mp_limb_t t, a, b;

  ASSERT (n > 0);
  ASSERT_MPN (xp, n);
  ASSERT (MPN_SAME_OR_SEPARATE_P (qp, xp, n));

  a = 0;
  for (j = 0; j <= n - 1; j++)
    {
      t = xp[j];
      if (t > a)
	{
	  b = 1;
	}
      else
	{
	  b = 0;
	}
      a = a - t;
      qp[j] = a;
      a = a - b;		/* no borrow from this sub */
    }
  return a;
}
