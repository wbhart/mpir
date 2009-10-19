/* ngcd_step

Copyright 2004, 2005 Niels Möller

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
#include "longlong.h"

/* Extract one limb, shifting count bits left
    ________  ________
   |___xh___||___xl___|
	  |____r____|
   >count <

   The count includes any nail bits, so it should work fine if
   count is computed using count_leading_zeros.
*/

#define MPN_EXTRACT_LIMB(count, xh, xl) \
  (((xh) << (count)) | ((xl) >> (GMP_LIMB_BITS - (count))))

static void
ngcd_matrix_update_1 (struct ngcd_matrix *M, unsigned col)
{
  mp_limb_t c0, c1;
  ASSERT (col < 2);

  c0 = mpn_add_n (M->p[0][col], M->p[0][0], M->p[0][1], M->n);
  c1 = mpn_add_n (M->p[1][col], M->p[1][0], M->p[1][1], M->n);

  M->p[0][col][M->n] = c0;
  M->p[1][col][M->n] = c1;

  M->n += (c0 | c1) != 0;
  ASSERT (M->n < M->alloc);
}

static void
ngcd_matrix_update_q (struct ngcd_matrix *M, mp_srcptr qp, mp_size_t qn,
		      unsigned col)
{
  ASSERT (col < 2);

  if (qn == 1)
    {
      mp_limb_t q = qp[0];
      mp_limb_t c0, c1;

      c0 = mpn_addmul_1 (M->p[0][col], M->p[0][1-col], M->n, q);
      c1 = mpn_addmul_1 (M->p[1][col], M->p[1][1-col], M->n, q);

      M->p[0][col][M->n] = c0;
      M->p[1][col][M->n] = c1;

      M->n += (c0 | c1) != 0;
    }
  else
    {
      unsigned row;

      /* Carries for the unlikely case that we get both high words
	 from the multiplication and carries from the addition. */
      mp_limb_t c[2];
      mp_size_t n;

      /* The matrix will not necessarily grow in size by qn, so we
	 need normalization in order not to overflow M. */

      for (n = M->n; n + qn > M->n; n--)
	{
	  ASSERT (n > 0);
	  if (M->p[0][1-col][n-1] > 0 ||  M->p[1][1-col][n-1] > 0)
	    break;
	}
      
      ASSERT (qn + n <= M->alloc);

      for (row = 0; row < 2; row++)
	{
	  if (qn <= n)
	    mpn_mul (M->tp, M->p[row][1-col], n, qp, qn);
	  else
	    mpn_mul (M->tp, qp, qn, M->p[row][1-col], n);

	  ASSERT (n + qn >= M->n);
	  c[row] = mpn_add (M->p[row][col], M->tp, n + qn, M->p[row][col], M->n);
	}
      if (c[0] | c[1])
	{
	  M->n = n + qn + 1;
	  M->p[0][col][n-1] = c[0];
	  M->p[1][col][n-1] = c[1];
	}
      else
	{
	  n += qn;
	  n -= (M->p[0][col][n-1] | M->p[1][col][n-1]) == 0;
	  if (n > M->n)
	    M->n = n;
	}
    }

  ASSERT (M->n < M->alloc);
}

/* Multiply M by M1 from the right. Since the M1 elements fit in
   GMP_NUMB_BITS - 1 bits, M grows by at most one limb. Needs
   temporary space M->n */
static void
ngcd_matrix_mul_1 (struct ngcd_matrix *M, const struct ngcd_matrix1 *M1)
{
  unsigned row;
  mp_limb_t grow;
  for (row = 0, grow = 0; row < 2; row++)
    {
      mp_limb_t c0, c1;

      /* Compute (u, u') <-- (r00 u + r10 u', r01 u + r11 u') as

	  t   = u
	  u  *= r00
	  u  += r10 * u'
	  u' *= r11
	  u' += r01 * t
      */

      MPN_COPY (M->tp, M->p[row][0], M->n);
      c0 =     mpn_mul_1 (M->p[row][0], M->p[row][0], M->n, M1->u[0][0]);
      c0 += mpn_addmul_1 (M->p[row][0], M->p[row][1], M->n, M1->u[1][0]);
      M->p[row][0][M->n] = c0;

      c1 =     mpn_mul_1 (M->p[row][1], M->p[row][1], M->n, M1->u[1][1]);
      c1 += mpn_addmul_1 (M->p[row][1], M->tp,        M->n, M1->u[0][1]);
      M->p[row][1][M->n] = c1;

      grow |= (c0 | c1);
    }
  M->n += (grow != 0);
  ASSERT (M->n < M->alloc);
}

/* Perform a few steps, using some of mpn_nhgcd2, subtraction and division.
   Reduces the size by almost one limb or more, but never below the
   given size s. Return new size for a and b, or 0 if no more steps
   are possible. M = NULL is allowed, if M is not needed.

   Needs temporary space for division, n + 1 limbs, and for
   ngcd_matrix1_vector, n limbs. */
mp_size_t
mpn_ngcd_step (mp_size_t n, mp_ptr ap, mp_ptr bp, mp_size_t s,
	       struct ngcd_matrix *M, mp_ptr tp)
{
  struct ngcd_matrix1 M1;
  mp_limb_t mask;
  mp_limb_t ah, al, bh, bl;
  mp_size_t an, bn, qn;
  mp_ptr qp;
  mp_ptr rp;
  int col;

  ASSERT (n > s);

  mask = ap[n-1] | bp[n-1];
  ASSERT (mask > 0);

  if (n == s + 1)
    {
      if (mask < 4)
	goto subtract;

      ah = ap[n-1]; al = ap[n-2];
      bh = bp[n-1]; bl = bp[n-2];
    }
  else if (mask & GMP_NUMB_HIGHBIT)
    {
      ah = ap[n-1]; al = ap[n-2];
      bh = bp[n-1]; bl = bp[n-2];
    }
  else
    {
      int shift;

      count_leading_zeros (shift, mask);
      ah = MPN_EXTRACT_LIMB (shift, ap[n-1], ap[n-2]);
      al = MPN_EXTRACT_LIMB (shift, ap[n-2], ap[n-3]);
      bh = MPN_EXTRACT_LIMB (shift, bp[n-1], bp[n-2]);
      bl = MPN_EXTRACT_LIMB (shift, bp[n-2], bp[n-3]);
    }

  /* Try an mpn_nhgcd2 step */
  if (mpn_nhgcd2 (ah, al, bh, bl, &M1))
    {
      /* Multiply M <- M * M1 */
      if (M)
	ngcd_matrix_mul_1 (M, &M1);

      /* Multiply M1^{-1} (a;b) */
      return mpn_ngcd_matrix1_vector (&M1, n, ap, bp, tp);
    }

 subtract:
  /* There are two ways in which mpn_nhgcd2 can fail. Either one of ah and
     bh was too small, or ah, bh were (almost) equal. Perform one
     subtraction step (for possible cancellation of high limbs),
     followed by one division. */

  /* Since we must ensure that #(a-b) > s, we handle cancellation of
     high limbs explicitly up front. (FIXME: Or is it better to just
     subtract, normalize, and use an addition to undo if it turns out
     the the difference is too small?) */
  for (an = n; an > s; an--)
    if (ap[an-1] != bp[an-1])
      break;

  if (an == s)
    return 0;

  /* Maintain a > b. When needed, swap a and b, and let col keep track
     of how to update M. */
  if (ap[an-1] > bp[an-1])
    {
      /* a is largest. In the subtraction step, we need to update
	 column 1 of M */
      col = 1;
    }
  else
    {
      MP_PTR_SWAP (ap, bp);
      col = 0;
    }

  bn = n;
  MPN_NORMALIZE (bp, bn);  
  if (bn <= s)
    return 0;
  
  /* We have #a, #b > s. When is it possible that #(a-b) < s? For
     cancellation to happen, the numbers must be of the form

       a = x + 1, 0,            ..., 0,            al
       b = x    , GMP_NUMB_MAX, ..., GMP_NUMB_MAX, bl

     where al, bl denotes the least significant k limbs. If al < bl,
     then #(a-b) < k, and if also high(al) != 0, high(bl) != GMP_NUMB_MAX,
     then #(a-b) = k. If al >= bl, then #(a-b) = k + 1. */

  if (ap[an-1] == bp[an-1] + 1)
    {
      mp_size_t k;
      int c;
      for (k = an-1; k > s; k--)
	if (ap[k-1] != 0 || bp[k-1] != GMP_NUMB_MAX)
	  break;

      MPN_CMP (c, ap, bp, k);
      if (c < 0)
	{
	  mp_limb_t cy;
	  
	  /* The limbs from k and up are cancelled. */
	  if (k == s)
	    return 0;
	  cy = mpn_sub_n (ap, ap, bp, k);
	  ASSERT (cy == 1);
	  an = k;
	}
      else
	{
	  ASSERT_NOCARRY (mpn_sub_n (ap, ap, bp, k));
	  ap[k] = 1;
	  an = k + 1;
	}
    }
  else
    ASSERT_NOCARRY (mpn_sub_n (ap, ap, bp, an));
  
  ASSERT (an > s);
  ASSERT (ap[an-1] > 0);
  ASSERT (bn > s);
  ASSERT (bp[bn-1] > 0);
  
  if (M)
    ngcd_matrix_update_1 (M, col);

  if (an < bn)
    {
      MPN_PTR_SWAP (ap, an, bp, bn);
      col ^= 1;
    }
  else if (an == bn)
    {
      int c;
      MPN_CMP (c, ap, bp, an);
      if (c < 0)
	{
	  MP_PTR_SWAP (ap, bp);
	  col ^= 1;
	}
    }

  /* Divide a / b. Store first the quotient (qn limbs) and then the
     remainder (bn limbs) starting at tp. */
  qn = an + 1 - bn;
  qp = tp;
  rp = tp + qn;

  /* FIXME: We could use an approximate division, that may return a
     too small quotient, and only guarantess that the size of r is
     almost the size of b. */
  mpn_tdiv_qr (qp, rp, 0, ap, an, bp, bn);
  qn -= (qp[qn -1] == 0);

  /* Normalize remainder */
  an = bn;
  for ( ; an > s; an--)
    if (rp[an-1] > 0)
      break;

  if (an > s)
    /* Include leading zero limbs */
    MPN_COPY (ap, rp, bn);
  else
    {
      /* Quotient is too large */
      mp_limb_t cy;

      cy = mpn_add (ap, bp, bn, rp, an);

      if (cy > 0)
	{
	  ASSERT (bn < n);
	  ap[bn] = cy;
	  bp[bn] = 0;
	  bn++;
	}

      MPN_DECR_U (qp, qn, 1);
      qn -= (qp[qn-1] == 0);
    }

  if (qn > 0 && M)
    ngcd_matrix_update_q (M, qp, qn, col);

  return bn;
}
