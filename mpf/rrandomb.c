/* mpf_rrandomb -- Generate a positive random mpf_t of specified size, with
   long runs of consecutive ones and zeros in the binary representation.
   Intended for testing of other MP routines.

Copyright 1995, 1996, 2001, 2002, 2003 Free Software Foundation, Inc.

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

void
mpf_rrandomb (mpf_ptr x, gmp_randstate_t rnd,mp_size_t xs, mp_exp_t exp)
{
  mp_size_t xn;
  mp_size_t prec;
  mp_limb_t elimb;

  xn = ABS (xs);
  prec = PREC(x);

  if (xn == 0)
    {
      EXP(x) = 0;
      SIZ(x) = 0;
      return;
    }

  if (xn > prec + 1)
    xn = prec + 1;

  /* General random mantissa.  */
  mpn_rrandom (PTR(x), rnd, xn);

  /* Generate random exponent.  */
  _gmp_rand (&elimb, rnd, GMP_NUMB_BITS);
  exp = ABS (exp);
  exp = elimb % (2 * exp + 1) - exp;

  EXP(x) = exp;
  SIZ(x) = xs < 0 ? -xn : xn;
}
