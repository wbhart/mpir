/* double mpz_get_d_2exp (signed long int *exp, mpz_t src).

Copyright 2001, 2003, 2004 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA. */

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"

#ifndef _MSC_VER
static
#endif
mpir_si
mpz_get_2exp_d (double *r, mpz_srcptr src)
{
  mp_size_t size, abs_size;
  mp_srcptr ptr;
  int cnt;
  mpir_si exp;

  size = SIZ(src);
  if (UNLIKELY (size == 0))
    {
	  *r = 0.0;
	  return 0;
    }

  ptr = PTR(src);
  abs_size = ABS(size);
  count_leading_zeros (cnt, ptr[abs_size - 1]);
  exp = abs_size * GMP_NUMB_BITS - (cnt - GMP_NAIL_BITS);
  *r = mpn_get_d (ptr, abs_size, size, -exp);
  return exp;
}

double
mpz_get_d_2exp(signed long *exp2, mpz_srcptr src)
{
	double r;
	*exp2 = mpz_get_2exp_d(&r, src);
	return r;
}
