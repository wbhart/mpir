/* Exercise mpz_perfect_power_p

  Copyright 2008 Jason Moxham

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

#include <stdio.h>
#include <stdlib.h>
#include "mpir.h"
#include "gmp-impl.h"
#include "tests.h"

int
perfpow_ref (mpz_t x)
{
  mpz_t y, q, r;
  unsigned long i;
  int ret = 0;

  if (mpz_cmp_ui (x, 0) == 0 || mpz_cmp_ui (x, 1) == 0
      || mpz_cmp_si (x, -1) == 0)
    return 1;
  mpz_init_set (y, x);
  mpz_init (q);
  mpz_init (r);
  mpz_abs (y, y);
  for (i = 2;; i++)
    {
      mpz_root (q, y, i);
      mpz_pow_ui (r, q, i);
      if (mpz_cmp (r, y) == 0 && (i % 2 == 1 || SIZ (x) > 0))
	{
	  ret = 1;
	  break;
	}
      if (mpz_cmp_ui (q, 1) <= 0)
	break;
    }
  mpz_clear (y);
  mpz_clear (q);
  mpz_clear (r);
  return ret;
}

int
main (void)
{
  int i, r1, r2, j, a, b, c, d;
  mpz_t x, m;
  tests_start ();

  mpz_init (x);
  mpz_init (m);
  /* check small values 
    -8000 picked up an error
    -729 picked up an error
  */
  for (i = -10000; i < 10000; i++)
    {
      mpz_set_si (x, i);
      r1 = mpz_perfect_power_p (x);
      r2 = perfpow_ref (x);
      if (r1 != r2)
	{
	  printf ("mpz_perfect_power_p wrong on %d got %d want %d\n", i, r1,
		  r2);
	  abort ();
	}
    }
  /* check (-1)^i.2^a.3^b.5^c.x^d where x is big */
  /* 1.0.0.0.5 picked up one error
     1.10.6.0.0 picked up another error     
     1.0.0.15.5 picked up another error
     want a good selection of powers to try all possibilitys
   */
  for (i = 1; i <= 2; i++)
    {
      for (d = 0; d < 10; d++)
	{
	  for (a = 0; a < 11; a++)
	    {
	      for (b = 0; b < 11; b++)
		{
		  for (c = 0; c < 20; c++)
		    {
		      mpz_set_ui (x, 1);
		      mpz_set_si (m, -1);
		      for (j = 0; j < i; j++)
			mpz_mul (x, x, m);
		      mpz_set_ui (m, 2);
		      for (j = 0; j < a; j++)
			mpz_mul (x, x, m);
		      mpz_set_ui (m, 3);
		      for (j = 0; j < b; j++)
			mpz_mul (x, x, m);
		      mpz_set_ui (m, 5);
		      for (j = 0; j < c; j++)
			mpz_mul (x, x, m);
		      mpz_set_ui (m, 117039007);
		      for (j = 0; j < d; j++)
			mpz_mul (x, x, m);
		      r1 = mpz_perfect_power_p (x);
		      r2 = perfpow_ref (x);
		      if (r1 != r2)
			{
			  printf
			    ("mpz_perfect_power_p wrong on %d.%d.%d.%d.%d got %d want %d\n",
			     i, a, b, c, d, r1, r2);
			  abort ();
			}
		    }
		}
	    }
	}
    }

  mpz_clear (x);
  mpz_clear (m);
  tests_end ();
  exit (0);
}
