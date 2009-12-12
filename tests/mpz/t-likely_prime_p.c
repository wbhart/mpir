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
check_sqrt (void)
{
  gmp_randstate_t  rands;
  int    i;
  mpz_t  x;
  unsigned long bits;
  mp_limb_t p, m, m2, s;

  mpz_init (x);
  gmp_randinit_default(rands);

  for (i = 0; i < 200000; i++)
    {
      do
      {
         mpz_set_ui(x, GMP_NUMB_BITS/2 - 1);
         mpz_urandomm(x, rands, x);
         bits = mpz_get_ui(x) + 2;
         mpz_rrandomb(x, rands, bits);
         p = mpz_getlimbn(x, 0);
         m = p*p;
         mpz_set_ui(x, MIN(p, 1000));
         mpz_urandomm(x, rands, x);
         m2 = m + mpz_get_ui(x) - MIN(p, 1000)/2;
      } while ((m2 == m) || ((mp_limb_signed_t) (m2 ^ m) < (mp_limb_signed_t) 0) || (m2 == 0) || (m2 == 1));

      s = n_sqrt(m2);
      if (((m2 < m) && (s != p - 1)) || ((m2 > m) && (s != p)))
      {
          printf ("mpz_likely_prime_p\n");
          printf ("n_sqrt is broken\n");
#if defined( _MSC_VER ) && defined( _WIN64 )
          printf ("%llu is returned as n_sqrt(%llu)\n", s, m2);
#else
          printf ("%lu is returned as n_sqrt(%lu)\n", s, m2);
#endif
          abort();
      }
    }

  for (i = 0; i < 200000; i++)
    {
      mpz_set_ui(x, GMP_NUMB_BITS/2 - 1);
      mpz_urandomm(x, rands, x);
      bits = mpz_get_ui(x) + 2;
      mpz_rrandomb(x, rands, bits);
      p = mpz_getlimbn(x, 0);
      m = p*p;

      s = n_sqrt(m);
      if (s != p)
      {
          printf ("mpz_likely_prime_p\n");
          printf ("n_sqrt is broken\n");
#if defined( _MSC_VER ) && defined( _WIN64 )
          printf ("%llu is returned as n_sqrt(%llu)\n", s, m);
#else
          printf ("%lu is returned as n_sqrt(%lu)\n", s, m);
#endif
          abort();
      }
    }

  mpz_clear (x);
  gmp_randclear(rands);
}

void
check_rand (void)
{
  gmp_randstate_t  rands;
  int    i;
  mpz_t  x;
  unsigned long bits;
  mp_limb_t p;

  mpz_init (x);
  gmp_randinit_default(rands);

  for (i = 0; i < 2000; i++)
    {
      do
      {
         mpz_set_ui(x, GMP_NUMB_BITS);
         mpz_urandomm(x, rands, x);
         bits = mpz_get_ui(x) + 1;
         mpz_rrandomb(x, rands, bits);
         p = mpz_getlimbn(x, 0);
      } while (is_likely_prime_BPSW(p));

      if (mpz_probab_prime_p(x, 100))
      {
          printf ("mpz_likely_prime_p\n");
#if defined( _MSC_VER ) && defined( _WIN64 )
          printf ("%llu is declared composite\n", p);
#else
          printf ("%lu is declared composite\n", p);
#endif
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

void
check_large (void)
{
  gmp_randstate_t  rands;
  int    i, count, r1, r2;
  mpz_t  x;
  unsigned long bits;
  mp_limb_t p;

  mpz_init (x);
  gmp_randinit_default(rands);

  count = 0;
  for (i = 0; i < 100000; i++)
    {
      mpz_set_ui(x, 300);
      mpz_urandomm(x, rands, x);
      bits = mpz_get_ui(x) + 1;
      mpz_rrandomb(x, rands, bits);

      r1 = mpz_probab_prime_p(x, 100);
      r2 = mpz_likely_prime_p(x, rands, 0);

      if (r1 == 1 && r2 == 0)
      {
          printf ("mpz_likely_prime_p\n");
          gmp_printf ("%Zd is declared composite\n", x);
          abort();
      } else if (r1 == 0 && r2 == 1)
         count++;
    }

  if (count > 1)
  {
     printf ("mpz_likely_prime_p\n");
     printf ("%d composite(s) declared prime\n", count);
     abort();
  }

  mpz_clear (x);
  gmp_randclear(rands);
}

int
main (void)
{
  tests_start ();
  
  check_sqrt ();
  check_rand ();
  check_small ();
  check_large ();

  tests_end ();
  exit (0);
}
