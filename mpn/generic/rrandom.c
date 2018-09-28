/* mpn_rrandom -- Generate random numbers with relatively long strings
   of ones and zeroes.  Suitable for border testing.

Copyright 1992, 1993, 1994, 1996, 2000, 2001, 2002, 2004 Free Software
Foundation, Inc.

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

static void gmp_rrandomb(mp_ptr rp, gmp_randstate_t rstate, mpir_ui nbits);

/* Ask _gmp_rand for 32 bits per call unless that's more than a limb can hold.
   Thus, we get the same random number sequence in the common cases.
   FIXME: We should always generate the same random number sequence! */
#if GMP_NUMB_BITS < 32
#define BITS_PER_RANDCALL GMP_NUMB_BITS
#else
#define BITS_PER_RANDCALL 32
#endif

void
mpn_rrandom (mp_ptr rp, gmp_randstate_t rnd, mp_size_t n)
{
  int bit_pos;			/* bit number of least significant bit where
				            next bit field to be inserted */
  mp_limb_t ran, ranm; /* buffer for random bits */

  /* FIXME: Is n == 0 supposed to be allowed? */
  ASSERT (n >= 0);

  _gmp_rand (&ranm, rnd, BITS_PER_RANDCALL);
  ran = ranm;

  /* Start off at a random bit position in the most significant limb. */
  bit_pos = ran % GMP_NUMB_BITS;

  gmp_rrandomb (rp, rnd, n * GMP_NUMB_BITS - bit_pos);
}

static void
gmp_rrandomb (mp_ptr rp, gmp_randstate_t rstate, mpir_ui nbits)
{
  mpir_ui bi;
  mp_limb_t ranm; /* buffer for random bits */
  unsigned cap_chunksize, chunksize;
  mp_size_t i;

  /* Set entire result to 111..1  */
  i = (nbits + GMP_NUMB_BITS - 1) / GMP_NUMB_BITS - 1;
  rp[i] = GMP_NUMB_MAX >> (GMP_NUMB_BITS - (nbits % GMP_NUMB_BITS)) % GMP_NUMB_BITS;
  for (i = i - 1; i >= 0; i--)
    rp[i] = GMP_NUMB_MAX;

  _gmp_rand (&ranm, rstate, BITS_PER_RANDCALL);
  cap_chunksize = nbits / (ranm % 4 + 1);
  cap_chunksize += cap_chunksize == 0; /* make it at least 1 */

  bi = nbits;

  for (;;)
    {
      _gmp_rand (&ranm, rstate, BITS_PER_RANDCALL);
      chunksize = 1 + ranm % cap_chunksize;
      bi = (bi < chunksize) ? 0 : bi - chunksize;

      if (bi == 0)
	break;			/* low chunk is ...1 */

      rp[bi / GMP_NUMB_BITS] ^= CNST_LIMB (1) << bi % GMP_NUMB_BITS;

      _gmp_rand (&ranm, rstate, BITS_PER_RANDCALL);
      chunksize = 1 + ranm % cap_chunksize;
      bi = (bi < chunksize) ? 0 : bi - chunksize;

      mpn_incr_u (rp + bi / GMP_NUMB_BITS, CNST_LIMB (1) << bi % GMP_NUMB_BITS);

      if (bi == 0)
	break; /* low chunk is ...0 */
    }
}
