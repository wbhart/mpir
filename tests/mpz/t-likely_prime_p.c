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

  for (i = 0; i < 20000; i++)
    {
      do
      {
         mpz_set_ui(GMP_NUMB_BITS);
         mpz_urandomm(x, state, x);
         bits = mpz_get_ui(x) + 1;
         mpz_rrandomb(x, state, bits);
         p = mpz_get_ui(x);
      } while is_likely_prime_BPSW(p);

      if (mpz_probab_prime_p(x, 100))
      {
          printf    ("%lu is declared composite\n", p);
          abort();
      }
    }
  mpz_clear (x);
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
