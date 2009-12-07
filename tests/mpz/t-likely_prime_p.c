/* Test is_likely_prime_1.

Copyright 2001, 2002 Free Software Foundation, Inc.
Copyright 2009 William Hart

This file is part of the MPIR Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the MPIR Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The MPIR Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPIR Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA. */

#include <stdio.h>
#include <stdlib.h>
#include "mpir.h"
#include "gmp-impl.h"
#include "tests.h"

void
check_rand (void)
{
  gmp_randstate_t  rands;
  int    i;
  mpz_t  x;
  unsigned long bits, p;

  mpz_init (x);
  gmp_randinit_default(rands);

  for (i = 0; i < 200000; i++)
    {
      do
      {
         mpz_set_ui(x, GMP_NUMB_BITS);
         mpz_urandomm(x, rands, x);
         bits = mpz_get_ui(x) + 1;
         mpz_rrandomb(x, rands, bits);
         p = mpz_get_ui(x);
      } while (is_likely_prime_BPSW(p));

      if (mpz_probab_prime_p(x, 100))
      {
          printf ("mpz_likely_prime_p\n");
          printf    ("%lu is declared composite\n", p);
          abort();
      }
    }
  mpz_clear (x);
  gmp_randclear(rands);
}

/* return 1 if prime, 0 if composite */
int
isprime (long n)
{
  long  i;

  n = ABS(n);

  if (n < 2)
    return 0;
  if (n == 2)
    return 1;
  if ((n & 1) == 0)
    return 0;

  for (i = 3; i < n; i++)
    if ((n % i) == 0)
      return 0;

  return 1;
}

void
check_one (mpz_srcptr n, int want, gmp_randstate_t rands)
{
  int  got;

  got = mpz_likely_prime_p (n, rands, 0);

  /* "definitely prime" is fine if we only wanted "probably prime" */
  if (got == 2 && want == 1)
    want = 2;

  if (got != want)
    {
      printf ("mpz_likely_prime_p\n");
      mpz_trace ("  n    ", n);
      printf    ("  got =%d", got);
      printf    ("  want=%d", want);
      abort ();
    }
}

void
check_pn (mpz_ptr n, int want, gmp_randstate_t rands)
{
  check_one (n, want, rands);
  mpz_neg (n, n);
  check_one (n, want, rands);
}

/* expect certainty for small n */
void
check_small (void)
{
  mpz_t  n;
  long   i;
  gmp_randstate_t  rands;
  
  mpz_init (n);
  gmp_randinit_default(rands);

  for (i = 0; i < 3000; i++)
    {
      mpz_set_si (n, i);
      check_pn (n, isprime (i), rands);
    }

  gmp_randclear(rands);
  mpz_clear (n);
}

int
main (void)
{
  tests_start ();
  
  check_rand ();
  check_small ();

  tests_end ();
  exit (0);
}
