/*
Copyright 2009 Jason Moxham

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

#include "mpir.h"
#include "gmp-impl.h"

int
mpz_probable_prime_p (mpz_srcptr N, gmp_randstate_t STATE, int PROB,
		      unsigned long td)
{
  int d, t, i, r;
  mpz_t base, nm1, x, e, n;

  ALLOC (n) = ALLOC (N);
  SIZ (n) = ABSIZ (N);
  PTR (n) = PTR (N);		// fake up an absolute value that we dont have de-allocate
// algorithm dose not handle small values , get rid of them here
  if (mpz_cmp_ui (n, 2) == 0 || mpz_cmp_ui (n, 3) == 0)
    return 1;
  if (mpz_cmp_ui (n, 5) < 0 || mpz_even_p (n))
    return 0;
// we assume we know nothing about N ie it is a random integer
// so we try here anything which speeds up the average case
// we try some trial division
#define LIM 1024
  d=mpz_trial_division(n,3,LIM);
  if(d!=0)
    {if(mpz_cmp_ui(n, d) == 0)return 1;
     return 0;}
  if (mpz_cmp_ui (n, LIM * LIM) < 0)
    return 1;
  ASSERT (mpz_odd_p (n));
  ASSERT (mpz_cmp_ui (n, 5) >= 0);
// now do some random strong pseudoprime tests
  mpz_init (base);
  mpz_init_set (nm1, n);
  mpz_sub_ui (nm1, nm1, 1);
  mpz_init (e);
  mpz_init (x);
  t = mpz_scan1 (nm1, 0);	// so 2^t divides nm1
  ASSERT (t > 0);
  mpz_tdiv_q_2exp (e, nm1, t);	// so e=nm1/2^t
  r = 1;
  while (PROB > 0)
    {
      PROB -= 2;
      do
	{
	  mpz_urandomm (base, STATE, nm1);
	}
      while (mpz_cmp_ui (base, 1) <= 0);
      mpz_powm (x, base, e, n);	// x=base^e mod n
      if (mpz_cmp_ui (x, 1) == 0 || mpz_cmp (x, nm1) == 0)
	continue;
      for (i = t - 1; i > 0; i--)
	{
	  mpz_mul (x, x, x);
	  mpz_mod (x, x, n);
	  if (mpz_cmp (x, nm1) == 0)
	    break;
	  if (mpz_cmp_ui (x, 1) == 0)
	    {
	      r=0;
	      break;
	    }
	}
      if (i == 0 || r == 0)
        {
          r=0;
          break;
        }
    }
  mpz_clear (nm1);
  mpz_clear (x);
  mpz_clear (e);
  mpz_clear (base);
  return r;
}
