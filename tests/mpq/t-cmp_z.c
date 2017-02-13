/* Test mpq_cmp_z.

Copyright 1996, 2001, 2015 Free Software Foundation, Inc.

This file is part of the GNU MP Library test suite.

The GNU MP Library test suite is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 3 of the License,
or (at your option) any later version.

The GNU MP Library test suite is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License along with
the GNU MP Library test suite.  If not, see https://www.gnu.org/licenses/.  */

#include <stdio.h>
#include <stdlib.h>

#include "mpir.h"
#include "gmp-impl.h"
#include "tests.h"

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
ref_mpq_cmp_z (mpq_t a, mpz_t b)
{
  mpz_t bi;
  int cc;

  mpz_init (bi);

  mpz_mul (bi, b, DEN (a));
  cc = mpz_cmp (NUM (a), bi);
  mpz_clear (bi);
  return cc;
}

#ifndef SIZE
#define SIZE 8	/* increasing this lowers the probability of finding an error */
#endif

#ifndef MAXN
#define MAXN 5	/* increasing this impatcs on total timing */
#endif

void
sizes_test (int m)
{
  mpq_t a;
  mpz_t b;
  int i, j, k, s;
  int cc, ccref;

  mpq_init (a);
  mpz_init (b);

  for (i = 0; i <= MAXN ; ++i)
    {
      mpz_setbit (DEN (a), i*m); /* \sum_0^i 2^(i*m) */
      for (j = 0; j <= MAXN; ++j)
	{
	  mpz_set_ui (NUM (a), 0);
	  mpz_setbit (NUM (a), j*m); /* 2^(j*m) */
	  for (k = 0; k <= MAXN; ++k)
	    {
	      mpz_set_ui (b, 0);
	      mpz_setbit (b, k*m); /* 2^(k*m) */
	      if (i == 0) /* Denominator is 1, compare the two exponents */
		ccref = (j>k)-(j<k);
	      else
		ccref = j-i > k ? 1 : -1;
	      for (s = 1; s >= -1; s -= 2)
		{
		  cc = mpq_cmp_z (a, b);

		  if (ccref != SGN (cc))
		    {
		      fprintf (stderr, "i=%i, j=%i, k=%i, m=%i, s=%i\n; ccref= %i, cc= %i\n", i, j, k, m, s, ccref, cc);
		      abort ();
		    }

		  mpq_neg (a, a);
		  mpz_neg (b, b);
		  ccref = - ccref;
		}
	    }
	}
    }

  mpq_clear (a);
  mpz_clear (b);
}

int
main (int argc, char **argv)
{
  mpq_t a;
  mpz_t b;
  mp_size_t size;
  int reps = 10000;
  int i;
  int cc, ccref;
  gmp_randstate_t rands;

  tests_start ();
  gmp_randinit_default(rands);

  if (argc == 2)
     reps = atoi (argv[1]);

  mpq_init (a);
  mpz_init (b);

  for (i = 0; i < reps; i++)
    {
      if (i % 8192 == 0)
	sizes_test (urandom (rands) % (i + 1) + 1);	  
      size = urandom (rands) % SIZE - SIZE/2;
      mpz_intrandom2 (NUM (a), rands, size);
      do
	{
	  size = urandom (rands) % (SIZE/2);
	  mpz_intrandom2 (DEN (a), rands, size);
	}
      while (mpz_cmp_ui (DEN (a), 0) == 0);

      size = urandom (rands) % SIZE - SIZE/2;
      mpz_intrandom2 (b, rands, size);

      mpq_canonicalize (a);

      ccref = ref_mpq_cmp_z (a, b);
      cc = mpq_cmp_z (a, b);

      if (SGN (ccref) != SGN (cc))
	abort ();
    }

  mpq_clear (a);
  mpz_clear (b);
  gmp_randclear(rands);

  tests_end ();
  exit (0);
}
