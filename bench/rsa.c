/* rsa.c -- benchmark signing with the RSA algorithm.

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

#define RSA_EXP 0x10001

int cputime (void);
void rsa_sign (mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, mpz_t);

int
main (int argc, char *argv[])
{
  gmp_randstate_t rs;
  mpz_t p, q, pq, pm1, qm1, phi, e, d, p_i_q, dp, dq, msg[1024], smsg;
  unsigned long int n, i, niter, t0, ti;
  double t, f, ops_per_sec;
  int decimals;

  if (argc != 2)
    {
      fprintf (stderr, "usage: %s n\n", argv[0]);
      fprintf (stderr, "  where n is number of bits in numbers tested\n");
      return -1;
    }

  if (argc == 2)
    n = atoi (argv[1]);

  gmp_randinit_default (rs);
  mpz_init (p);
  mpz_init (q);
  mpz_init (pq);

  printf ("Generating p, q, d..."); fflush (stdout);

  mpz_urandomb (p, rs, n/2);
  mpz_setbit (p, n / 2 - 1);
  mpz_setbit (p, n / 2 - 2);
  mpz_nextprime (p, p);

  mpz_urandomb (q, rs, n/2);
  mpz_setbit (q, n / 2 - 1);
  mpz_setbit (q, n / 2 - 2);
  mpz_nextprime (q, q);

  mpz_mul (pq, p, q);

  mpz_init_set_ui (e, RSA_EXP);
  mpz_init (d);
  mpz_init (pm1);
  mpz_init (qm1);
  mpz_init (phi);

  mpz_sub_ui (pm1, p, 1);
  mpz_sub_ui (qm1, q, 1);
  mpz_mul (phi, pm1, qm1);
  if (mpz_invert (d, e, phi) == 0)
    abort ();

  printf ("done; pq is %d bits\n", (int) mpz_sizeinbase (pq, 2));

  printf ("Precomputing CRT constants\n");

  mpz_init (p_i_q);
  if (mpz_invert (p_i_q, p, q) == 0)
    abort ();

  mpz_init (dp);
  mpz_init (dq);
  mpz_mod (dp, d, pm1);
  mpz_mod (dq, d, qm1);

  printf ("Generating random messages\n");

  for (i = 0; i < 1024; i++)
    {
      mpz_init (msg[i]);
      mpz_urandomb (msg[i], rs, n);
    }
  mpz_init (smsg);

  printf ("Calibrating CPU speed...");  fflush (stdout);
  TIME (t, rsa_sign (smsg, msg[0], p, q, pq, p_i_q, dp, dq));
  printf ("done\n");

  niter = (unsigned long) (1e4 / t);
  printf ("Signing random messages %lu times...", niter);  fflush (stdout);
  t0 = cputime ();
  for (i = niter; i > 0; i--)
    {
      rsa_sign (smsg, msg[i % 1024], p, q, pq, p_i_q, dp, dq);
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

void
rsa_sign (mpz_t smsg,
	  mpz_t msg, mpz_t p, mpz_t q, mpz_t pq,
	  mpz_t p_i_q, mpz_t dp, mpz_t dq)
{
  mpz_t  t, o, pr, qr, qr_m_pr;

  mpz_init (pr);
  mpz_init (qr);
  mpz_init (qr_m_pr);
  mpz_init (t);
  mpz_init (o);

  mpz_powm (pr, msg, dp, p);
  mpz_powm (qr, msg, dq, q);

  mpz_sub (qr_m_pr, qr, pr);

  mpz_mul (t, qr_m_pr, p_i_q);
  mpz_mod (o, t, q);		/* slow mod */

  mpz_mul (t, o, p);
  mpz_add (smsg, pr, t);
  mpz_mod (smsg, smsg, pq);	/* fast mod */

  mpz_clear (o);
  mpz_clear (t);
  mpz_clear (qr_m_pr);
  mpz_clear (qr);
  mpz_clear (pr);
}

/* Return user CPU time measured in milliseconds.  */
#if !defined (__sun) \
    && (defined (USG) || defined (__SVR4) || defined (_UNICOS) \
	|| defined (__hpux))
#include <time.h>

int
cputime ()
{
  return (int) ((double) clock () * 1000 / CLOCKS_PER_SEC);
}
#else
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

int
cputime ()
{
  struct rusage rus;

  getrusage (0, &rus);
  return rus.ru_utime.tv_sec * 1000 + rus.ru_utime.tv_usec / 1000;
}
#endif
