/* Schönhage's 1987 algorithm, reorganized into hgcd form 

Copyright 2004, 2005 Niels Möller
Copyright 2008 Jason Martin
Copyright 2009 William Hart

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

#include <stdio.h>  /* for NULL */

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"

/*
 * This code is from Moller's patches.
 *
 * To make this code work with "make tune" we need to conditionally
 * exclude the Moller code when this file gets included inside of
 * gcd_bin.c in ../tune.
 */
#ifndef INSIDE_TUNE_GCD_BIN

/* For input of size n, matrix elements are of size at most ceil(n/2)
   - 1, but we need one limb extra. */

void
mpn_ngcd_matrix_init (struct ngcd_matrix *M, mp_size_t n, mp_ptr p)
{
  mp_size_t s = (n+1)/2;
  M->alloc = s;
  M->n = 1;
  MPN_ZERO (p, 4 * s);
  M->p[0][0] = p;
  M->p[0][1] = p + s;
  M->p[1][0] = p + 2 * s;
  M->p[1][1] = p + 3 * s;
  M->tp = p + 4*s;

  M->p[0][0][0] = M->p[1][1][0] = 1;
}

#define NHGCD_BASE_ITCH MPN_NGCD_STEP_ITCH

/* Reduces a,b until |a-b| fits in n/2 + 1 limbs. Constructs matrix M
   with elements of size at most (n+1)/2 - 1. Returns new size of a,
   b, or zero if no reduction is possible. */
static mp_size_t
nhgcd_base (mp_ptr ap, mp_ptr bp, mp_size_t n,
	    struct ngcd_matrix *M, mp_ptr tp)
{
  mp_size_t s = n/2 + 1;
  mp_size_t nn;

  ASSERT (n > s);
  ASSERT (ap[n-1] > 0 || bp[n-1] > 0);

  nn = mpn_ngcd_step (n, ap, bp, s, M, tp);
  if (!nn)
    return 0;

  for (;;)
    {
      n = nn;
      ASSERT (n > s);
      nn = mpn_ngcd_step (n, ap, bp, s, M, tp);
      if (!nn )
	return n;      
    }
}

/* Size analysis for nhgcd:

   For the recursive calls, we have n1 <= ceil(n / 2). Then the
   storage need is determined by the storage for the recursive call
   computing M1, and ngcd_matrix_adjust and ngcd_matrix_mul calls that use M1
   (after this, the storage needed for M1 can be recycled).

   Let S(r) denote the required storage. For M1 we need 5 * ceil(n1/2)
   = 5 * ceil(n/4), and for the ngcd_matrix_adjust call, we need n + 2. For the 
   matrix multiplication we need 4*n1 + 3*ceil(n1/2) + 3, so 3n + 3 will do.
   In total, 5 * ceil(n/4) + 3n + 3 <= 17 ceil(n/4) + 3.

   For the recursive call, we need S(n1) = S(ceil(n/2)).

   S(n) <= 17*ceil(n/4) + 3 + S(ceil(n/2))
        <= 17*(ceil(n/4) + ... + ceil(n/2^(1+k))) + 3k + S(ceil(n/2^k))
        <= 17*(2 ceil(n/4) + k) + 3k + S(n/2^k)   
	<= 34 ceil(n/4) + 20k + S(n/2^k)
	
*/

mp_size_t
mpn_nhgcd_itch (mp_size_t n)
{
  unsigned k;
  mp_size_t nn;

  /* Inefficient way to almost compute
     log_2(n/NHGCD_BASE_THRESHOLD) */
  for (k = 0, nn = n;
       ABOVE_THRESHOLD (nn, NHGCD_THRESHOLD);
       nn = (nn + 1) / 2)
    k++;

  if (k == 0)
    return NHGCD_BASE_ITCH (n);

  return 35 * ((n+3) / 4) + 20 * k
    + NHGCD_BASE_ITCH (NHGCD_THRESHOLD);
}

/* Reduces a,b until |a-b| fits in n/2 + 1 limbs. Constructs matrix M
   with elements of size at most (n+1)/2 - 1. Returns new size of a,
   b, or zero if no reduction is possible. */

mp_size_t
mpn_nhgcd (mp_ptr ap, mp_ptr bp, mp_size_t n,
	   struct ngcd_matrix *M, mp_ptr tp)
{
  mp_size_t s = n/2 + 1;
  mp_size_t n2 = (3*n)/4 + 1;
  
  mp_size_t p, nn;
  unsigned count;
  int success = 0;
  
  ASSERT (n > s);
  ASSERT ((ap[n-1] | bp[n-1]) > 0);

  ASSERT ((n+1)/2 - 1 < M->alloc);

  if (BELOW_THRESHOLD (n, NHGCD_THRESHOLD))
    return nhgcd_base (ap, bp, n, M, tp);

  p = n/2;
  nn = mpn_nhgcd (ap + p, bp + p, n - p, M, tp);
  if (nn > 0)
    {
      /* Needs 2*(p + M->n) <= 2*(floor(n/2) + ceil(n/2) - 1)
	 = 2 (n - 1) */
      n = mpn_ngcd_matrix_adjust (M, p + nn, ap, bp, p, tp);
      success = 1;
    }
  count = 0;
  while (n > n2)
    {
      count++;
      /* Needs n + 1 storage */
      nn = mpn_ngcd_step (n, ap, bp, s, M, tp);
      if (!nn)
	return success ? n : 0;
      n = nn;
      success = 1;
    }

  if (n > s + 2)
    {
      struct ngcd_matrix M1;
      mp_size_t scratch;

      p = 2*s - n + 1;
      scratch = MPN_NGCD_MATRIX_INIT_ITCH (n-p);

      mpn_ngcd_matrix_init(&M1, n - p, tp);
      nn = mpn_nhgcd (ap + p, bp + p, n - p, &M1, tp + scratch);
      if (nn > 0)
	{
	  /* Needs 2 (p + M->n) <= 2 (2*s - n2 + 1 + n2 - s - 1)
	     = 2*s <= 2*(floor(n/2) + 1) <= n + 2. */
	  n = mpn_ngcd_matrix_adjust (&M1, p + nn, ap, bp, p, tp + scratch);
	  /* Needs M->n <= n2 - s - 1 < n/4 */
	  mpn_ngcd_matrix_mul (M, &M1, tp + scratch);
	  success = 1;
	}
    }

  /* FIXME: This really is the base case */
  for (count = 0;; count++)
    {
      /* Needs s+3 < n */
      nn = mpn_ngcd_step (n, ap, bp, s, M, tp);
      if (!nn)
	return success ? n : 0;

      n = nn;
      success = 1;
    } 
}

#define EVEN_P(x) (((x) & 1) == 0)

#define P_SIZE(n) (n/2)

mp_size_t
mpn_gcd (mp_ptr gp, mp_ptr ap, mp_size_t an, mp_ptr bp, mp_size_t n)
{
  mp_size_t init_scratch;
  mp_size_t scratch;
  mp_ptr tp;
  TMP_DECL;
  
  ASSERT (an >= n);

  if (BELOW_THRESHOLD (n, GCD_THRESHOLD))
  {    
       return mpn_lgcd (gp, ap, an, bp, n);
  }
  
  init_scratch = MPN_NGCD_MATRIX_INIT_ITCH (n-P_SIZE(n));
  scratch = mpn_nhgcd_itch ((n+1)/2);

  /* Space needed for mpn_ngcd_matrix_adjust */
  if (scratch < 2*n)
    scratch = 2*n;
    
  if (scratch < MPN_NGCD_LEHMER_ITCH(n)) /* Space needed by Lehmer GCD */
    scratch = MPN_NGCD_LEHMER_ITCH(n);

  TMP_MARK;
  
  if (an + 1 > init_scratch + scratch)
    tp = TMP_ALLOC_LIMBS (an + 1);
  else
    tp = TMP_ALLOC_LIMBS (init_scratch + scratch);
  
  if (an > n)
    {
      mp_ptr rp = tp;
      mp_ptr qp = rp + n;

      mpn_tdiv_qr (qp, rp, 0, ap, an, bp, n);
      MPN_COPY (ap, rp, n);
      an = n;
      MPN_NORMALIZE (ap, an);
      if (an == 0)
	{	  
	  MPN_COPY (gp, bp, n);
	  TMP_FREE;
	  return n;
	}
    }

  while (ABOVE_THRESHOLD (n, GCD_THRESHOLD))
    {
      struct ngcd_matrix M;
      mp_size_t p = P_SIZE(n);
      mp_size_t nn;
      
      mpn_ngcd_matrix_init (&M, n - p, tp);
      nn = mpn_nhgcd (ap + p, bp + p, n - p, &M, tp + init_scratch);
      if (nn > 0)
	/* Needs 2*(p + M->n) <= 2*(floor(n/2) + ceil(n/2) - 1)
	   = 2(n-1) */
	n = mpn_ngcd_matrix_adjust (&M, p + nn, ap, bp, p, tp + init_scratch);

      else	
	{
	  mp_size_t gn;
	  n = mpn_ngcd_subdiv_step (gp, &gn, ap, bp, n, tp);
	  if (n == 0)
	    {
	      TMP_FREE;
	      return gn;
	    }
	}
    }

  ASSERT (ap[n-1] > 0 || bp[n-1] > 0);
  
  if (ap[n-1] < bp[n-1])
    MP_PTR_SWAP (ap, bp);
    
  if (BELOW_THRESHOLD (n, GCD_THRESHOLD))  
  {
    n = mpn_ngcd_lehmer (gp, ap, bp, n, tp);

    TMP_FREE;
    return n;
  }

  an = n;
  MPN_NORMALIZE (bp, n);

  if (n == 0)
    {
      MPN_COPY (gp, ap, an);
      TMP_FREE;
      return an;
    }

  if (EVEN_P (bp[0]))
    {
      /* Then a must be odd (since the calling convention implies that
	 there's no common factor of 2) */
      ASSERT (!EVEN_P (ap[0]));

      while (bp[0] == 0)
	{
	  bp++;
	  n--;
	}

      if (EVEN_P(bp[0]))
	{
	  int count;
	  count_trailing_zeros (count, bp[0]);
	  ASSERT_NOCARRY (mpn_rshift (bp, bp, n, count));
	  n -= (bp[n-1] == 0);
	}
    }

  TMP_FREE;  
  return mpn_lgcd (gp, ap, an, bp, n);
}
#endif
