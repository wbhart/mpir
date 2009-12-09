/* floating-point Newton, with inversion in 3M(n) */

/* mpn_invert

Copyright 2009 Paul Zimmermann

This file is part of the MPIR Library.

The MPIR Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
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

/* #define WRAP_AROUND */
/* #define WANT_ASSERT 1 */

#ifdef WRAP_AROUND
#define INVERT_VERSION 3
#define WRAP_AROUND_BOUND 1500
#else
#define INVERT_VERSION 2
#define WRAP_AROUND_BOUND ~0UL
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"

#define ZERO (mp_limb_t) 0
#define ONE  (mp_limb_t) 1

void
mpn_print (mp_ptr A, mp_size_t n)
{
  int j;

  for (j=0; j<n; j++)
    {
      printf ("+%lu*B^%u", A[j], j);
      if (j % 4 == 3 && j != n-1)
        printf ("\n");
    }
  printf (":\n");
}

/* Input: A = {ap, n} with most significant bit set.
   Output: X = B^n + {xp, n} where B = 2^GMP_NUMB_BITS.

   X is a lower approximation of B^(2n)/A with implicit msb.
   More precisely, one has:

              A*X < B^(2n) <= A*(X+1)

   or X = ceil(B^(2n)/A) - 1.
*/
void
mpn_invert (mp_ptr xp, mp_srcptr ap, mp_size_t n)
{
  if (n == 1)
    {
      /* invert_limb returns min(B-1, floor(B^2/ap[0])-B),
	 which is B-1 when ap[0]=B/2, and 1 when ap[0]=B-1.
	 For X=B+xp[0], we have A*X < B^2 <= A*(X+1) where
	 the equality holds only when A=B/2.

	 We thus have A*X < B^2 <= A*(X+1).
      */
      invert_limb (xp[0], ap[0]);
    }
  else if (n == 2)
    {
      mp_limb_t tp[4], up[2], sp[2], cy;

      tp[0] = ZERO;
      invert_limb (xp[1], ap[1]);
      tp[3] = mpn_mul_1 (tp + 1, ap, 2, xp[1]);
      cy = mpn_add_n (tp + 2, tp + 2, ap, 2);
      while (cy) /* Xh is too large */
	{
	  xp[1] --;
	  cy -= mpn_sub (tp + 1, tp + 1, 3, ap, 2);
	}
      /* tp[3] should be 111...111 */

      mpn_com_n (sp, tp + 1, 2);
      cy = mpn_add_1 (sp, sp, 2, ONE);
      /* cy should be 0 */

      up[1] = mpn_mul_1 (up, sp + 1, 1, xp[1]);
      cy = mpn_add_1 (up + 1, up + 1, 1, sp[1]);
      /* cy should be 0 */
      xp[0] = up[1];

      /* update tp */
      cy = mpn_addmul_1 (tp, ap, 2, xp[0]);
      cy = mpn_add_1 (tp + 2, tp + 2, 2, cy);
      do
	{
	  cy = mpn_add (tp, tp, 4, ap, 2);
	  if (cy == ZERO)
	    mpn_add_1 (xp, xp, 2, ONE);
	}
      while (cy == ZERO);

      /* now A*X < B^4 <= A*(X+1) */
    }
  else
    {
      mp_size_t l, h;
      mp_ptr tp, up;
      mp_limb_t cy, th;
      TMP_DECL;

      l = (n - 1) / 2;
      h = n - l;

      mpn_invert (xp + l, ap + l, h);

      TMP_MARK;
      tp = TMP_ALLOC_LIMBS (n + h);
      up = TMP_ALLOC_LIMBS (2 * h);

      if (n <= WRAP_AROUND_BOUND)
	{
          mpn_mul (tp, ap, n, xp + l, h);
          cy = mpn_add_n (tp + h, tp + h, ap, n);
        }
      else
	{
          mp_size_t m = n + 1;
          unsigned long k;
          int cc;
#ifdef CHECK
          mp_ptr tp2;
          mp_limb_t cy2;

          tp2 = TMP_ALLOC_LIMBS (n + h);
          mpn_mul (tp2, ap, n, xp + l, h);
          cy2 = mpn_add_n (tp2 + h, tp2 + h, ap, n);
#endif

          k = mpn_fft_best_k (m, 0);
          m = mpn_fft_next_size (m, k);
          /* we have m >= n + 1 by construction, thus m > h */
          ASSERT(m < n + h);
          cy = mpn_mul_fft (tp, m, ap, n, xp + l, h, k);
          /* cy, {tp, m} = A * {xp + l, h} mod (B^m+1) */
          cy += mpn_add_n (tp + h, tp + h, ap, m - h);
          cc = mpn_sub_n (tp, tp, ap + m - h, n + h - m);
          cc = mpn_sub_1 (tp + n + h - m, tp + n + h - m, 2 * m - n - h, cc);
          if (cc > cy) /* can only occur if cc=1 and cy=0 */
            cy = mpn_add_1 (tp, tp, m, ONE);
          else
            cy -= cc;
          /* cy, {tp, m} = A * Xh */

          /* add B^(n+h) + B^(n+h-m) */
          MPN_ZERO (tp + m, n + h - m);
          tp[m] = cy;
          /* note: since tp[n+h-1] is either 0, or cy<=1 if m=n+h-1,
             the mpn_incr_u() below cannot produce a carry */
          mpn_incr_u (tp + n + h - m, ONE);
          cy = 1;
          do /* check if T >= B^(n+h) + 2*B^n */
            {
              mp_size_t i;

              if (cy == ZERO)
                break; /* surely T < B^(n+h) */
              if (cy == ONE)
                {
                  for (i = n + h - 1; tp[i] == ZERO && i > n; i--);
                  if (i == n && tp[i] < (mp_limb_t) 2)
                    break;
                }
              /* subtract B^m+1 */
              cy -= mpn_sub_1 (tp, tp, n + h, ONE);
              cy -= mpn_sub_1 (tp + m, tp + m, n + h - m, ONE);
            }
          while (1);

#ifdef CHECK
          if ((cy != cy2) || mpn_cmp (tp, tp2, n + h) != 0)
            {
              fprintf (stderr, "wrong wrap around reconstruction\n");
              exit (1);
            }
#endif
        }

      while (cy)
	{
	  mpn_sub_1 (xp + l, xp + l, h, ONE);
	  cy -= mpn_sub (tp, tp, n + h, ap, n);
	}

      mpn_com_n (tp, tp, n);
      th = ~tp[n] + mpn_add_1 (tp, tp, n, ONE);
      mpn_mul_n (up, tp + l, xp + l, h);
      cy = mpn_add_n (up + h, up + h, tp + l, h);
      if (th != ZERO)
        cy += ONE + mpn_add_n (up + h, up + h, xp + l, h);
      MPN_COPY (xp, up + 2 * h - l, l);
      mpn_add_1 (xp + l, xp + l, h, cy);
      TMP_FREE;
    }
}

#ifdef MAIN

int
test_invert (mp_ptr xp, mp_srcptr ap, mp_size_t n)
{
  int res = 1;
  mp_size_t i;
  mp_ptr tp, up;
  mp_limb_t cy;
  TMP_DECL;

  TMP_MARK;
  tp = TMP_ALLOC_LIMBS (2 * n);
  up = TMP_ALLOC_LIMBS (2 * n);

  /* first check X*A < B^(2*n) */
  mpn_mul_n (tp, xp, ap, n);
  cy = mpn_add_n (tp + n, tp + n, ap, n); /* A * msb(X) */
  if (cy != 0)
    res = 0;

  /* now check B^(2n) - X*A <= A */
  mpn_com_n (tp, tp, 2 * n);
  mpn_add_1 (tp, tp, 2 * n, 1); /* B^(2n) - X*A */
  MPN_ZERO (up, 2 * n);
  MPN_COPY (up, ap, n);
  res = mpn_cmp (tp, up, 2 * n) <= 0;
  TMP_FREE;
  return res;
}

#include <sys/types.h>
#include <sys/resource.h>

int
cputime ()
{
  struct rusage rus;

  getrusage (0, &rus);
  return rus.ru_utime.tv_sec * 1000 + rus.ru_utime.tv_usec / 1000;
}

int
main (int argc, char *argv[])
{
  mp_size_t n = atoi (argv[1]), i, j, k;
  mp_ptr qp, rp, dp, tp, qp2, rp2;
  mp_limb_t cy;
  pid_t pid;
  int st;

  k = (argc <= 2) ? 1 : atoi(argv[2]);

  qp = malloc (n * sizeof (mp_limb_t));
  qp2 = malloc (n * sizeof (mp_limb_t));
  rp = malloc (n * sizeof (mp_limb_t));
  rp2 = malloc (2 * n * sizeof (mp_limb_t));
  dp = malloc (n * sizeof (mp_limb_t));
  tp = malloc (2 * n * sizeof (mp_limb_t));

  pid = getpid ();
  printf ("Seed=%lu\n", pid);
  srand48 (pid);
  for (i = 0; i < n; i++)
    dp[i] = lrand48 ();
  dp[n - 1] |= GMP_NUMB_HIGHBIT;

  mpn_random (rp, n);
  st = cputime ();
  for (i = 0; i < k; i++)
    mpn_mul_n (tp, dp, rp, n);
  printf ("mpn_mul_n took %dms\n", cputime () - st);

  st = cputime ();
  for (i = 0; i < k; i++)
    {
#ifdef CHECK
      //      printf ("Test %lu\n", i);
      for (j = 0; j < n; j++)
	dp[j] = lrand48 ();
      dp[n - 1] |= GMP_NUMB_HIGHBIT;
#endif      
      mpn_invert (qp, dp, n);
#ifdef CHECK
  if (test_invert (qp, dp, n) == 0)
    {
      fprintf (stderr, "test_invert failed at i=%lu\n", i);
      printf ("A:="); mpn_print (dp, n);
      printf ("X:=B^%lu", n); mpn_print (qp, n);
      exit (1);
    }
#endif
    }
  printf ("mpn_invert%d took %dms", INVERT_VERSION, cputime () - st);
#ifdef WRAP_AROUND
  printf (" (with wrap-around trick, WRAP_AROUND_BOUND=%lu)",
          WRAP_AROUND_BOUND);
#endif
  printf ("\n");

  // printf ("xp="); mpn_print (qp, n);

  MPN_ZERO (rp2, 2 * n);
  rp2[2 * n - 1] = GMP_LIMB_HIGHBIT;
  st = cputime ();
  for (i = 0; i < k; i++)
    {
      MPN_ZERO (rp2, 2 * n);
      rp2[2 * n - 1] = GMP_LIMB_HIGHBIT;
      mpn_divrem (qp2, 0, rp2, 2 * n, dp, n);
    }
  printf ("mpn_divrem took %dms\n", cputime () - st);

  free (qp);
  free (rp);
  free (dp);
  free (tp);

  return 0;
}
#endif
