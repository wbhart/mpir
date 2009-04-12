/* divide.c -- benchmark plain integer division.

Copyright 2003 Free Software Foundation, Inc.

This file is part of the MPIRbench.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA.  */


#include <stdlib.h>
#include <stdio.h>
#include "mpir.h"
#include "timing.h"

int
main (int argc, char *argv[])
{
  gmp_randstate_t rs;
  mpz_t x, y, z;
  unsigned long int m, n, i, niter, t0, ti;
  double t, f, ops_per_sec;
  int decimals;

  if (argc != 3)
    {
      fprintf (stderr, "usage: %s m n\n", argv[0]);
      fprintf (stderr, "  where m is number of dividend bits\n");
      fprintf (stderr, "    and n is number of divisor bits\n");
      return -1;
    }

  m = atoi (argv[1]);
  n = atoi (argv[2]);

  gmp_randinit_default (rs);

  mpz_init (x);
  mpz_init (y);
  mpz_init (z);
  mpz_urandomb (x, rs, m);
  mpz_urandomb (y, rs, n);

  printf ("Calibrating CPU speed...");  fflush (stdout);
  TIME (t, mpz_tdiv_q (z, x, y));
  printf ("done\n");

  niter = 1 + (unsigned long) (1e4 / t);
  printf ("Dividing an %lu-bit number by an %lu-bit number %lu times...",
	  m, n, niter);
  fflush (stdout);
  t0 = cputime ();
  for (i = niter; i > 0; i--)
    {
      mpz_tdiv_q (z, x, y);
    }
  ti = cputime () - t0;
  printf ("done!\n");

  ops_per_sec = 1000.0 * niter / ti;
  f = 100.0;
  for (decimals = 0;; decimals++)
    {
      if (ops_per_sec > f)
	break;
      f = f * 0.1;
    }

  printf ("RESULT: %.*f operations per second\n", decimals, ops_per_sec);
  return 0;
}
