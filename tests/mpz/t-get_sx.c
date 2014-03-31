/* 

Test mpz_get_sx

Copyright 2000, 2001 Free Software Foundation, Inc.

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

#include <stdio.h>
#include <stdlib.h>

#if defined( _MSC_VER )
#  if _MSC_VER < 1600
#    define SKIP_TEST
#  else
#    include <stdint.h>
#  endif
#else
#  include "config.h"
#  ifdef HAVE_STDINT_H
#    include <stdint.h>
#  else
#    define SKIP_TEST
#  endif
#endif

#ifdef SKIP_TEST

int
main (void)
{
  printf ("(u)intmax_t not available - test skipped\n");
  exit (0);
}

#else

#include "mpir.h"
#include "gmp-impl.h"
#include "tests.h"

void
check_data (void)
{
  static const struct {
    const char  *n;
    intmax_t     want;
  } data[] = {
    { "0",      0L },
    { "1",      1L },
    { "-1",     -1L },
    { "2",      2L },
    { "-2",     -2L },
    { "12345",  12345L },
    { "-12345", -12345L },
  };

  int    i;
  mpz_t  n;
  intmax_t got;

  mpz_init (n);
  for (i = 0; i < numberof (data); i++)
    {
      mpz_set_str_or_abort (n, data[i].n, 0);

      got = mpz_get_sx (n);
      if (got != data[i].want)
        {
          printf ("mpz_get_sx wrong at data[%d]\n", i); 
          printf ("   n     \"%s\" (", data[i].n);
          mpz_out_str (stdout, 10, n); printf (", hex ");
          mpz_out_str (stdout, 16, n); printf (")\n");
          printf ("   got   %ld (0x%lX)\n", got, got);
          printf ("   want  %ld (0x%lX)\n", data[i].want, data[i].want);
          abort();                                    
        }
    }
  mpz_clear (n);
}


void
check_max (void)
{
  mpz_t  n;
  intmax_t want;
  intmax_t   got;

  mpz_init (n);

#define CHECK_MAX(name)                                 \
  if (got != want)                                      \
    {                                                   \
      printf ("mpz_get_sx wrong on %s\n", name);        \
      printf ("   n    ");                              \
      mpz_out_str (stdout, 10, n); printf (", hex ");   \
      mpz_out_str (stdout, 16, n); printf ("\n");       \
      printf ("   got  %ld, hex %lx\n", got, got);      \
      printf ("   want %ld, hex %lx\n", want, want);    \
    }

  want = INTMAX_MAX;
  mpz_set_sx (n, want);
  got = mpz_get_sx (n);
  CHECK_MAX ("INTMAX_MAX");

  want = INTMAX_MIN;
  mpz_set_sx (n, want);
  got = mpz_get_sx (n);
  CHECK_MAX ("INTMAX_MIN");

  /* The following checks that -0x100000000 gives -0x80000000.  This doesn't
     actually fit in a long and the result from mpz_get_si() is undefined,
     but -0x80000000 is what comes out currently, and it should be that
     value irrespective of the mp_limb_t size (long or long long).  */

  want = INTMAX_MIN;
  mpz_mul_2exp (n, n, 1);
  CHECK_MAX ("-0x100...00");

  mpz_clear (n);
}


int
main (void)
{
  tests_start ();

  check_data ();
  check_max ();

  tests_end ();
  exit (0);
}

#endif
