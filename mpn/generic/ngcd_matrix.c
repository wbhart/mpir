#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"

/* Computes r = u x + v y. rn is the size of the result area, and must
   be at least one larger than the result. Needs temporary space of the same size.
   Returns size of result. Zero inputs are allowed.

   No overlap between input and output is allowed, since rp is used
   for temporary storage. */
static mp_limb_t
addmul2_n (mp_ptr rp,
	   mp_srcptr up, mp_size_t un, mp_srcptr xp, mp_size_t xn,
	   mp_srcptr vp, mp_size_t vn, mp_srcptr yp, mp_size_t yn,
	   mp_ptr tp)
{
  mp_size_t t0n, t1n;
  mp_size_t n;
  mp_limb_t cy;

  /* t0 = u * x is stored at rp, and t1 = v * y at tp. */
  t0n = xn + un;
  t1n = yn + vn;

  /* Handle zero cases */
  if (xn == 0 || un == 0)
    {
      if (yn == 0 || vn == 0)
	return 0;

      tp = rp;
      t0n = 0;
    }
  else if (yn == 0 || vn == 0)
    {
      ASSERT (xn > 0);
      ASSERT (un > 0);

      t1n = 0;
    }

  if (t0n > 0)
    {
      if (xn >= un)
	mpn_mul (rp, xp, xn, up, un);
      else
	mpn_mul (rp, up, un, xp, xn);

      t0n -= (rp[t0n-1] == 0);
    }

  if (t1n > 0)
    {
      if (yn >= vn)
	mpn_mul (tp, yp, yn, vp, vn);
      else
	mpn_mul (tp, vp, vn, yp, yn);

      t1n -= (tp[t1n-1] == 0);
    }
  else
    return t0n;

  if (t0n == 0)
    return t1n;

  if (t0n >= t1n)
    {
      cy = mpn_add (rp, rp, t0n, tp, t1n);
      n = t0n;
    }
  else
    {
      cy = mpn_add (rp, tp, t1n, rp, t0n);
      n = t1n;
    }
  rp[n] = cy;
  n += (cy != 0);

  return n;
}

/* Multiply M by M1 from the right. Needs 2*M->n temporary storage
   (and additionally uses M->tp). */
void
mpn_ngcd_matrix_mul (struct ngcd_matrix *M, const struct ngcd_matrix *M1,
		     mp_ptr tp)
{
  unsigned row;

  mp_ptr m00 = M1->p[0][0];
  mp_ptr m01 = M1->p[0][1];
  mp_ptr m10 = M1->p[1][0];
  mp_ptr m11 = M1->p[1][1];
  mp_size_t n00, n01, n10, n11;

  mp_size_t n;

  mp_ptr up = tp;
  mp_ptr vp = tp + M->n;

  /* About the new size of M:s elements. Since M1's diagonal elements
     are > 0, no element can decrease. The typical case is that the
     new elements are of size M->n + M1->n, one limb more or less. But
     it may be smaller, consider for example (1,x;0,1)(1,x;0,1) =
     (1,2x;0,1), where size is increased by a single bit no matter how
     large x is. So to avoid writing past the end of M, we need to
     normalise the numbers. */

  n00 = n01 = n10 = n11 = M1->n;
  MPN_NORMALIZE (m00, n00);
  MPN_NORMALIZE (m01, n01);
  MPN_NORMALIZE (m10, n10);
  MPN_NORMALIZE (m11, n11);

  n = 0;
  for (row = 0; row < 2; row++)
    {
      mp_size_t un, vn;
      mp_size_t nn;

      MPN_COPY (up, M->p[row][0], M->n);
      MPN_COPY (vp, M->p[row][1], M->n);

      un = vn = M->n;
      MPN_NORMALIZE (up, un);
      MPN_NORMALIZE (vp, vn);

      ASSERT (n00 + un <= M->alloc);
      ASSERT (n01 + un <= M->alloc);
      ASSERT (n10 + vn <= M->alloc);
      ASSERT (n11 + vn <= M->alloc);

      /* Compute (u', v') = (u,v) (r00, r01; r10, r11)
	 = (r00 u + r10 v, r01 u + r11 v) */

      nn = addmul2_n (M->p[row][0], up, un, m00, n00, vp, vn, m10, n10, M->tp);
      if (nn > n)
	n = nn;
      nn = addmul2_n (M->p[row][1], up, un, m01, n01, vp, vn, m11, n11, M->tp);
      if (nn > n)
	n = nn;
    }
  ASSERT (n < M->alloc);
  M->n = n;
}

/* Multiplies the least significant p limbs of (a;b) by M^-1.
   Temporary space needed: 2 * (p + M->n)*/
mp_size_t
mpn_ngcd_matrix_adjust (struct ngcd_matrix *M,
			mp_size_t n, mp_ptr ap, mp_ptr bp,
			mp_size_t p, mp_ptr tp)
{
  /* M^-1 (a;b) = (r11, -r01; -r10, r00) (a ; b)
     = (r11 a - r01 b; - r10 a + r00 b */

  mp_ptr t0 = tp;
  mp_ptr t1 = tp + p + M->n;
  mp_limb_t ah, bh;
  mp_limb_t cy;

  ASSERT (p + M->n  < n);

  /* First compute the two values depending on a, before overwriting a */

  if (M->n >= p)
    {
      mpn_mul (t0, M->p[1][1], M->n, ap, p);
      mpn_mul (t1, M->p[1][0], M->n, ap, p);
    }
  else
    {
      mpn_mul (t0, ap, p, M->p[1][1], M->n);
      mpn_mul (t1, ap, p, M->p[1][0], M->n);
    }

  /* Update a */
  MPN_COPY (ap, t0, p);
  ah = mpn_add (ap + p, ap + p, n - p, t0 + p, M->n);

  if (M->n >= p)
    mpn_mul (t0, M->p[0][1], M->n, bp, p);
  else
    mpn_mul (t0, bp, p, M->p[0][1], M->n);

  cy = mpn_sub (ap, ap, n, t0, p + M->n);
  ASSERT (cy <= ah);
  ah -= cy;

  /* Update b */
  if (M->n >= p)
    mpn_mul (t0, M->p[0][0], M->n, bp, p);
  else
    mpn_mul (t0, bp, p, M->p[0][0], M->n);

  MPN_COPY (bp, t0, p);
  bh = mpn_add (bp + p, bp + p, n - p, t0 + p, M->n);
  cy = mpn_sub (bp, bp, n, t1, p + M->n);
  ASSERT (cy <= bh);
  bh -= cy;

  if (ah > 0 || bh > 0)
    {
      ap[n] = ah;
      bp[n] = bh;
      n++;
    }
  else
    {
      /* The subtraction can reduce the size by at most one limb. */
      if (ap[n-1] == 0 && bp[n-1] == 0)
	n--;
    }
  ASSERT (ap[n-1] > 0 || bp[n-1] > 0);
  return n;  
}
