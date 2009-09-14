/* mpz_urandomm (rop, state, n) -- Generate a uniform pseudorandom
   integer in the range 0 to N-1, using STATE as the random state
   previously initialized by a call to gmp_randinit().

Copyright 2000, 2002  Free Software Foundation, Inc.

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
#include "longlong.h" /* for count_leading_zeros */

void
mpz_urandomm (mpz_ptr rop, gmp_randstate_t rstate, mpz_srcptr n)
{
  mp_ptr rp, np, nlast;
  mp_size_t nbits, size;
  int count;
  int pow2;
  int cmp;
  int overlap=0;

  size = ABSIZ (n);
  if (size == 0)
    DIVIDE_BY_ZERO;

  nlast = &PTR (n)[size - 1];

  /* Detect whether n is a power of 2.  */
  pow2 = POW2_P (*nlast);
  if (pow2 != 0)
    for (np = PTR (n); np < nlast; np++)
      if (*np != 0)
	{
	  pow2 = 0;		/* Mark n as `not a power of two'.  */
	  break;
	}
  count_leading_zeros (count, *nlast);
  nbits = size * GMP_NUMB_BITS - (count - GMP_NAIL_BITS) - pow2;
  if (nbits == 0)		/* nbits == 0 means that n was == 1.  */
    {
      SIZ (rop) = 0;
      return;
    }

  np=PTR(n);
  rp=PTR(rop);
  if(np==rp)
    {overlap=1;
     np=__GMP_ALLOCATE_FUNC_LIMBS(size);
     MPN_COPY(np,PTR(n),size);
    }
  /* Here the allocated size can be one too much if n is a power of
     (2^GMP_NUMB_BITS) but it's convenient for using mpn_cmp below.  */
  rp = MPZ_REALLOC (rop, size);
  /* Clear last limb to prevent the case in which size is one too much.  */
  rp[size - 1] = 0;

  do
    {
      _gmp_rand (rp, rstate, nbits);
      MPN_CMP (cmp, rp, np, size);
    }
  while (cmp >= 0);

  if(overlap)__GMP_FREE_FUNC_LIMBS(np,size);
  MPN_NORMALIZE (rp, size);
  SIZ (rop) = size;
}
