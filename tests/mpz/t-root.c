/* Test mpz_add, mpz_add_ui, mpz_cmp, mpz_cmp, mpz_mul, mpz_sqrtrem.

Copyright 1991, 1993, 1994, 1996, 2000, 2001 Free Software Foundation, Inc.

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

void debug_mp _PROTO ((mpz_t, int));


void
mpz_add_si (mpz_ptr x, mpz_srcptr y, signed long z)
{
  if (z >= 0)
    {
      mpz_add_ui (x, y, z);
      return;
    }
  mpz_sub_ui (x, y, -z);
  return;
}

int
main (int argc, char **argv)
{
  mpz_t x2;
  mpz_t root1, root2, root3, rem2;
  mpz_t temp, temp2;
  mp_size_t x2_size;
  int i, r1, j, k;
  int reps = 1000;
  unsigned long nth;
  gmp_randstate_t rands;
  mpz_t bs;
  unsigned long bsi, size_range;

  tests_start ();
  gmp_randinit_default(rands);

  mpz_init (bs);

  if (argc == 2)
    reps = atoi (argv[1]);

  mpz_init (x2);
  mpz_init (root1);
  mpz_init (root2);
  mpz_init (root3);
  mpz_init (rem2);
  mpz_init (temp);
  mpz_init (temp2);

  for (i = 0; i < reps; i++)
    {
      mpz_urandomb (bs, rands, 32);
      size_range = mpz_get_ui (bs) % 12 + 2;

      mpz_urandomb (bs, rands, size_range);
      x2_size = mpz_get_ui (bs) + 10;
      mpz_rrandomb (x2, rands, x2_size);

      mpz_urandomb (bs, rands, 15);
      nth = mpz_getlimbn (bs, 0) % mpz_sizeinbase (x2, 2) + 2;

      mpz_root (root1, x2, nth);
      mpz_nthroot (root3, x2, nth);
      if (mpz_cmp (root1, root3) != 0)
      {
         printf ("nthroot to root mismatch\n");
         abort ();
      }

      mpz_urandomb (bs, rands, 4);
      bsi = mpz_get_ui (bs);
      if ((bsi & 1) != 0)
	{
	  /* With 50% probability, set x2 near a perfect power.  */
	  mpz_pow_ui (x2, root1, nth);
	  if ((bsi & 2) != 0)
	    {
	      mpz_sub_ui (x2, x2, bsi >> 2);
	      mpz_abs (x2, x2);
	    }
	  else
	    mpz_add_ui (x2, x2, bsi >> 2);
	  mpz_root (root1, x2, nth);
      mpz_nthroot (root3, x2, nth);
      if (mpz_cmp (root1, root3) != 0)
      {
         printf ("nthroot to root mismatch\n");
         abort ();
      }
   }

      /* printf ("%ld %lu\n", SIZ (x2), nth); */

      mpz_rootrem (root2, rem2, x2, nth);
      mpz_pow_ui (temp, root1, nth);
      mpz_add (temp2, temp, rem2);

      /* Is power of result > argument?  */
      if (mpz_cmp (root1, root2) != 0 || mpz_cmp (x2, temp2) != 0
	  || mpz_cmp (temp, x2) > 0)
	{
	  fprintf (stderr, "ERROR after test %d\n", i);
	  debug_mp (x2, 10);
	  debug_mp (root1, 10);
	  debug_mp (root2, 10);
	  fprintf (stderr, "nth: %lu\n", nth);
	  abort ();
	}

      if (nth > 1 && mpz_cmp_ui (temp, 1L) > 0 && !mpz_perfect_power_p (temp))
	{
	  fprintf (stderr, "ERROR in mpz_perfect_power_p after test %d\n", i);
	  debug_mp (temp, 10);
	  debug_mp (root1, 10);
	  fprintf (stderr, "nth: %lu\n", nth);
	  abort ();
	}

      if (nth > 10000)
	continue;		/* skip too expensive test */

      mpz_add_ui (temp2, root1, 1L);
      mpz_pow_ui (temp2, temp2, nth);

      /* Is square of (result + 1) <= argument?  */
      if (mpz_cmp (temp2, x2) <= 0)
	{
	  fprintf (stderr, "ERROR after test %d\n", i);
	  debug_mp (x2, 10);
	  debug_mp (root1, 10);
	  fprintf (stderr, "nth: %lu\n", nth);
	  abort ();
	}
    }
  for (j = 0; j < reps; j++)
    {				// pick a number bs and a power nth
      mpz_urandomb (bs, rands, GMP_LIMB_BITS);
      nth = mpz_get_ui (bs) % 30 + 1;
      mpz_urandomb (bs, rands, GMP_LIMB_BITS);
      x2_size = mpz_get_ui (bs) % 500;
      mpz_urandomb (bs, rands, x2_size);
      mpz_pow_ui (temp, bs, nth);
      for (k = 0; k < 2; k++)
	{
	  if (k != 0)
	    mpz_neg (temp, temp);
	  for (i = -10; i < 10; i++)
	    {
	      mpz_add_si (temp2, temp, i);
	      if (nth % 2 == 0 && mpz_cmp_ui (temp2, 0) < 0)
		continue;
	      //printf("i is %d nth is %d\n",i,nth);
          mpz_nthroot (root1, temp2, nth);
          r1 = mpz_root (root2, temp2, nth);
	      mpz_rootrem (root3, rem2, temp2, nth);
          if (mpz_cmp (root1, root2) != 0)
          {
             printf ("root12 mismatch\n");
             abort ();
          }
          if (mpz_cmp (root2, root3) != 0)
		{
		  printf ("root23 mismatch\n");
		  gmp_fprintf (stderr, "root2 = %ZX\n", root2);
		  gmp_fprintf (stderr, "root3 = %ZX\n", root3);
		  abort ();
		}
	      mpz_pow_ui (root3, root2, nth);
	      mpz_sub (root3, temp2, root3);
	      //printf("bs is ");mpz_out_str(stdout,10,bs);printf("\n");
	      //printf("temp2 is ");mpz_out_str(stdout,10,temp2);printf("\n");
	      //printf("root is ");mpz_out_str(stdout,10,root1);printf("\n");
	      //printf("rem is ");mpz_out_str(stdout,10,rem2);printf("\n");
	      if (mpz_cmp (root3, rem2) != 0)
		{
		  printf ("rootrem mismatch\n");
		  gmp_fprintf (stderr, "val = %ZX ^ (1 / %ld)\n", temp2, nth);
		  gmp_fprintf (stderr, "root = %ZX\n", root2);
		  gmp_fprintf (stderr, "rem2 = %ZX\n", root3);
		  gmp_fprintf (stderr, "rem3 = %ZX\n", rem2);
		  abort ();
		}
	      if (r1 && mpz_cmp_ui (rem2, 0) != 0)
		{
		  printf ("rootexact1 mismatch %d\n", r1);
		  abort ();
		}
	      if (!r1 && mpz_cmp_ui (rem2, 0) == 0)
		{
		  printf ("rootexact2 mismatch\n");
		  abort ();
		}
	      mpz_add_ui (root2, root2, 1);
	      mpz_pow_ui (root2, root2, nth);
	      if (mpz_cmp (root2, temp2) <= 0)
		{
		  printf ("root wrong\n");
		  abort ();
		}
	      if (!r1 && i == 0)
		{
		  printf ("rootexact2 wrong\n");
		  abort ();
		}
	    }
	}
    }
  // check small values and stupid stuff
  for (k = 0; k < 2; k++)
    {
      for (nth = 1; nth < 3 * reps; nth++)
	{
	  if (k % 2 != 0 && nth % 2 == 0)
	    continue;
	  for (i = 0; i < 2 * reps; i++)
	    {
	      mpz_set_ui (temp, i);
	      if (k % 2 != 0)
		mpz_neg (temp, temp);
	      mpz_rootrem (root1, rem2, temp, nth);
	      mpz_pow_ui (temp2, root1, nth);
	      mpz_add (temp2, temp2, rem2);
	      if (mpz_cmp (temp2, temp) != 0)
		{
		  printf ("root small mismatch\n");
		  abort ();
		}
	    }
	}
    }

  /* test case reported by Alex Dyachenko */
    {
  mpz_t a, cube;
  
  mpz_init_set_str(a, "8984948281360922385394772450147012613851354303", 10);
  
  mpz_init(cube);
  mpz_mul(cube, a, a);
  mpz_mul(cube, cube, a);

  mpz_set(root1, cube);
  mpz_nthroot(root1, root1, 3);
  
  mpz_nthroot(root2, cube, 3);

  if (mpz_cmp(root1, root2) != 0)
    {
  printf("nthroot aliasing failed\n");
  abort();
    }

  mpz_clear(cube);
  mpz_clear(a);
    }

  mpz_clear (bs);
  mpz_clear (x2);
  mpz_clear (root1);
  mpz_clear (root2);
  mpz_clear (root3);
  mpz_clear (rem2);
  mpz_clear (temp);
  mpz_clear (temp2);
  gmp_randclear(rands);
  tests_end ();
  exit (0);
}

void
debug_mp (mpz_t x, int base)
{
  mpz_out_str (stderr, base, x);
  fputc ('\n', stderr);
}
