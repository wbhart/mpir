/* Test mpq_cmp_ui.

Copyright 1996, 1997, 2001, 2002 Free Software Foundation, Inc.

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
#include "tests.h"

#define NUM(x) (&((x)->_mp_num))
#define DEN(x) (&((x)->_mp_den))

#define SGN(x) ((x) < 0 ? -1 : (x) > 0 ? 1 : 0)

void
mpz_intrandom2 (mpz_ptr x, gmp_randstate_t rands, mp_size_t size)
{
  mp_size_t abs_size;

  abs_size = ABS (size);
  if (abs_size != 0)
    {
      if (x->_mp_alloc < abs_size)
	_mpz_realloc (x, abs_size);

      mpn_rrandom (x->_mp_d, rands ,abs_size);
    }

  x->_mp_size = size;
}


int
ref_mpq_cmp_ui (mpq_t a, mpir_ui bn, mpir_ui bd)
{
  mpz_t ai, bi;
  int cc;

  mpz_init (ai);
  mpz_init (bi);

  mpz_mul_ui (ai, NUM (a), bd);
  mpz_mul_ui (bi, DEN (a), bn);
  cc = mpz_cmp (ai, bi);
  mpz_clear (ai);
  mpz_clear (bi);
  return cc;
}

#ifndef SIZE
#define SIZE 8	/* increasing this lowers the probabilty of finding an error */
#endif

int
main (int argc, char **argv)
{
  mpq_t a, b;
  mp_size_t size;
  int reps = 10000;
  int i;
  int cc, ccref;
  mpir_ui bn, bd;
  gmp_randstate_t rands;
  
  tests_start ();
  gmp_randinit_default(rands);
  
  if (argc == 2)
     reps = atoi (argv[1]);

  mpq_init (a);
  mpq_init (b);

  for (i = 0; i < reps; i++)
    {
      size = urandom (rands) % SIZE - SIZE/2;
      mpz_intrandom2 (NUM (a), rands, size);
      do
	{
	  size = urandom (rands) % SIZE - SIZE/2;
	  mpz_intrandom2 (DEN (a), rands, size);
	}
      while (mpz_cmp_ui (DEN (a), 0) == 0);

      mpz_intrandom2 (NUM (b), rands, (mp_size_t) 1);
      mpz_mod_ui (NUM (b), NUM (b), ~(mpir_ui) 0);
      mpz_add_ui (NUM (b), NUM (b), 1);

      mpz_intrandom2 (DEN (b), rands ,(mp_size_t) 1);
      mpz_mod_ui (DEN (b), DEN (b), ~(mpir_ui) 0);
      mpz_add_ui (DEN (b), DEN (b), 1);

      mpq_canonicalize (a);
      mpq_canonicalize (b);

      bn = mpz_get_ui (NUM (b));
      bd = mpz_get_ui (DEN (b));

      ccref = ref_mpq_cmp_ui (a, bn, bd);
      cc = mpq_cmp_ui (a, bn, bd);

      if (SGN (ccref) != SGN (cc))
	abort ();
    }

  mpq_clear (a);
  mpq_clear (b);
  gmp_randclear(rands);
  tests_end ();
  exit (0);
}
