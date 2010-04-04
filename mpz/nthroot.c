/* mpz_nthroot(root, u, nth) --  Set ROOT to floor(U^(1/nth)).

Copyright 1999, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

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
mpz_nthroot (mpz_ptr root, mpz_srcptr u, unsigned long int nth)
{
  mp_ptr rootp, up, remp;
  mp_size_t us, un, rootn;
  up = PTR(u);
  us = SIZ(u);
  /* even roots of negatives provoke an exception */
  if (us < 0 && (nth & 1) == 0)
    SQRT_OF_NEGATIVE;
  /* root extraction interpreted as c^(1/nth) means a zeroth root should
     provoke a divide by zero, do this even if c==0 */
  if (nth == 0)
    DIVIDE_BY_ZERO;
  if (us == 0)
    {
      if (root != 0)
	SIZ(root) = 0;
      return;
    }
  un = ABS (us);
  rootn = (un - 1) / nth + 1;
  rootp = MPZ_REALLOC (root, rootn);
  up = PTR(u);
  if (nth == 1)
    {
      MPN_COPY (rootp, up, un);
    }
  else
    {
      mpn_rootrem (rootp, 0, up, un, (mp_limb_t) nth);
    }
  SIZ(root) = us >= 0 ? rootn : -rootn;
  return;
}
