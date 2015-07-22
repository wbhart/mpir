/* Test mpn_invert.

Copyright 2002, 2003, 2004 Free Software Foundation, Inc.
Copyright 2009 Paul Zimmermann
Copyright 2009 William Hart

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

/* Note that we don't use <limits.h> for LONG_MIN, but instead our own
   definition in gmp-impl.h.  In gcc 2.95.4 (debian 3.0) under
   -mcpu=ultrasparc, limits.h sees __sparc_v9__ defined and assumes that
   means long is 64-bit long, but it's only 32-bits, causing fatal compile
   errors.  */

#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"
#include "tests.h"

#define ITERS 100

int
test_invert (mp_ptr xp, mp_srcptr ap, mp_size_t n)
{
  int res = 1;
  mp_size_t i;
  mp_ptr tp, up;
  mp_limb_t cy;
  TMP_DECL;

  TMP_MARK;
  tp = TMP_ALLOC_LIMBS (2 * n);
  up = TMP_ALLOC_LIMBS (2 * n);

  /* first check X*A < B^(2*n) */
  mpn_mul_n (tp, xp, ap, n);
  cy = mpn_add_n (tp + n, tp + n, ap, n); /* A * msb(X) */
  if (cy != 0)
    return 0;

  /* now check B^(2n) - X*A <= A */
  mpn_com_n (tp, tp, 2 * n);
  mpn_add_1 (tp, tp, 2 * n, 1); /* B^(2n) - X*A */
  MPN_ZERO (up, 2 * n);
  MPN_COPY (up, ap, n);
  res = mpn_cmp (tp, up, 2 * n) <= 0;
  TMP_FREE;
  return res;
}

void check_rand(void)
{
  mp_size_t i, n;
  mp_ptr qp, dp;

  gmp_randstate_t rands;
  gmp_randinit_default(rands);

  for (n = 1; n < 3000; n++)
  {
     mp_limb_t bits;
     count_leading_zeros(bits, n);  
     bits = GMP_LIMB_BITS - bits;
     if (n > 100) n+=2;
     if (n > 300) n+=4;
     if (n > 1000) n+=8;
     if (n > 2000) n+=16;
     qp = malloc (n * sizeof (mp_limb_t));
     dp = malloc (n * sizeof (mp_limb_t));
  
     mpn_rrandom(dp, rands, n);
     dp[n - 1] |= GMP_NUMB_HIGHBIT;
   
     for (i = 0; i < ITERS/bits; i++)
     {
        mpn_rrandom(dp, rands, n);
        dp[n - 1] |= GMP_NUMB_HIGHBIT;
        mpn_invert (qp, dp, n);
        if (test_invert (qp, dp, n) == 0)
        {
          fprintf (stderr, "t-invert failed at n = %lu, i=%lu\n", n, i);
          gmp_printf ("A:= %Nx\n", dp, n);
          gmp_printf ("X:=B^%lu*%Nx\n", n, qp, n);
          abort();
        }
     }
  }

  free (qp);
  free (dp);
  gmp_randclear(rands);
}

int
main (void)
{
  tests_start ();
  
  check_rand ();

  tests_end ();
  exit (0);
}
