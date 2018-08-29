/* Test mpz_add, mpz_sub, mpz_add_ui, mpz_sub_ui, and mpz_ui_sub.

Copyright 2002 Free Software Foundation, Inc.

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

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"
#include "tests.h"

void debug_mp(mpz_t, int);
void dump_abort(int, char *, mpz_t, mpz_t);

int
main (int argc, char **argv)
{
  mpz_t op1, op2, r1, r2;
  mp_size_t op1n, op2n;
  unsigned long int op2long;
  int i;
  int reps = 100000;
  gmp_randstate_t rands;
  mpz_t bs;
  unsigned long bsi, size_range;

  tests_start ();
  gmp_randinit_default(rands);

  mpz_init (bs);

  if (argc == 2)
     reps = atoi (argv[1]);

  mpz_init (op1);
  mpz_init (op2);
  mpz_init (r1);
  mpz_init (r2);

  for (i = 0; i < reps; i++)
    {
      mpz_urandomb (bs, rands, 32);
      size_range = mpz_get_ui (bs) % 10 + 2;

      mpz_urandomb (bs, rands, size_range);
      op1n = mpz_get_ui (bs);
      mpz_rrandomb (op1, rands, op1n);

      mpz_urandomb (bs, rands, size_range);
      op2n = mpz_get_ui (bs);
      mpz_rrandomb (op2, rands, op2n);

      mpz_urandomb (bs, rands, 2);
      bsi = mpz_get_ui (bs);
      if ((bsi & 1) != 0)
	mpz_neg (op1, op1);
      if ((bsi & 2) != 0)
	mpz_neg (op2, op2);

      /* printf ("%ld %ld\n", SIZ (multiplier), SIZ (multiplicand)); */

      mpz_add (r1, op1, op2);
      mpz_sub (r2, r1, op2);
      if (mpz_cmp (r2, op1) != 0)
	dump_abort (i, "mpz_add or mpz_sub incorrect", op1, op2);

      if (mpz_fits_ulong_p (op2))
	{
	  op2long = mpz_get_ui (op2);
	  mpz_add_ui (r1, op1, op2long);
	  mpz_sub_ui (r2, r1, op2long);
	  if (mpz_cmp (r2, op1) != 0)
	    dump_abort (i, "mpz_add_ui or mpz_sub_ui incorrect", op1, op2);

	  mpz_ui_sub (r1, op2long, op1);
	  mpz_sub_ui (r2, op1, op2long);
	  mpz_neg (r2, r2);
	  if (mpz_cmp (r1, r2) != 0)
	    dump_abort (i, "mpz_add_ui or mpz_ui_sub incorrect", op1, op2);
	}
    }

  mpz_clear (bs);
  mpz_clear (op1);
  mpz_clear (op2);
  mpz_clear (r1);
  mpz_clear (r2);
  gmp_randclear(rands);
  tests_end ();
  exit (0);
}

void
dump_abort (int i, char *s, mpz_t op1, mpz_t op2)
{
  fprintf (stderr, "ERROR: %s in test %d\n", s, i);
  fprintf (stderr, "op1 = "); debug_mp (op1, -16);
  fprintf (stderr, "op2 = "); debug_mp (op2, -16);
  abort();
}

void
debug_mp (mpz_t x, int base)
{
  mpz_out_str (stderr, base, x); fputc ('\n', stderr);
}
