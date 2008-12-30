/* Stehle's and Zimmermann's binary algorithm */
#include <stdio.h>  /* for NULL */
#include <string.h>

/* #define WANT_ASSERT 0 */

#include "gmp.h"
#include "gmp-impl.h"
#include "longlong.h"

/* Notation: W = 2^GMP_NUMB_BITS */

#define ODD(x) ((x) & 1)

/* Extract one limb, shifting count bits right
    ________  ________
   |___xh___||___xl___|
     |____r____|
		>count<
*/

#define EXTRACT_LIMB(count, xh, xl) \
  (((xl) >> (count) ) | (((xh) << (GMP_NUMB_BITS - count))  & GMP_NUMB_MASK))

#define RSHIFT2(h, l, count) do {					   \
  (l) = ((l)>>(count)) | (((h)<<(GMP_NUMB_BITS - count)) & GMP_NUMB_MASK); \
  (h) >>= (count);							   \
} while (0)

#define MP_LIMB_SIGNED_T_SWAP(x, y)				\
  do {								\
    mp_limb_signed_t __mp_limb_signed_t_swap__tmp = (x);	\
    (x) = (y);							\
    (y) = __mp_limb_signed_t_swap__tmp;				\
  } while (0)

#define INT_SWAP(x, y)				\
  do {						\
    int __int_swap__tmp = (x);			\
    (x) = (y);					\
    (y) = __int_swap__tmp;			\
  } while (0)

/* Computes 2^(n * GMP_NUMB_BITS) - x. Returns 1 in case x was zero,
   otherwise zero. This function is used when a subtraction yields a
   borrow, to negate the difference. */
static mp_limb_t
negate_diff (mp_ptr xp, mp_size_t n)
{
  mp_size_t i;

  ASSERT (n > 0);

  for (i = 0; i < n; i++)
    {
      if (xp[i] > 0)
	{
	  xp[i] = ~xp[i] + 1;
	  i++;
	  if (i < n)
	    mpn_com_n (xp + i, xp + i, n - i);

	  return 0;
	}
    }
  return 1;
}

/* Computes r = u x + v y. Returns most significant limb. rp and xp
   may be the same. */

static mp_limb_t
addmul2_1 (mp_ptr rp, mp_limb_t *cy, mp_srcptr xp, mp_srcptr yp, mp_size_t n,
	   mp_limb_t u, mp_limb_t v)
{
  mp_limb_t h1;
  mp_limb_t h2;

  ASSERT (rp != yp);

  h1 = mpn_mul_1 (rp, xp, n, u);
  h2 = mpn_addmul_1 (rp, yp, n, v);

  h1 += h2;
  *cy = (h1 < h2);
  
  return h1;
}

/* Computes r = u x - v y. rp and xp may be the same. Returns most
   significant limb. Sets *rsign to reflect the sign of the result. */
static mp_limb_t
submul2_1 (mp_ptr rp, int *rsign, mp_srcptr xp, mp_srcptr yp, mp_size_t n,
	   mp_limb_t u, mp_limb_t v)
{
  mp_limb_t hi;
  mp_limb_t cy;

  ASSERT (rp != yp);

  hi = mpn_mul_1 (rp, xp, n, u);

  cy = mpn_submul_1 (rp, yp, n, v);
  if (cy <= hi)
    {
      hi -= cy;
      *rsign = 0;
    }
  else
    {
      /* We need to negate the difference,

      - ( (hi - cy) 2^k + r) = (cy - hi) 2^k - r = (cy - hi - 1) 2^k + 2^k - r */

      *rsign = -1;
      hi = cy - hi - 1 + negate_diff (rp, n);
    }
  return hi;
}

/* Returns the number of zero limbs at the least significant end of x,
   and the number of zero bits of the first non-zero limb. If x == 0,
   returns n and sets *bits = 0. */
static mp_size_t
power_of_2 (unsigned *bits, mp_srcptr xp, mp_size_t n)
{
  mp_size_t zlimbs;

  for (zlimbs = 0; zlimbs < n && xp[zlimbs] == 0; zlimbs++)
    ;

  if (zlimbs == n || ODD (xp[zlimbs]))
    *bits = 0;
  else
    {
      int count;

      count_trailing_zeros (count, xp[zlimbs]);
      *bits = count;
    }

  return zlimbs;
}

struct bgcd_matrix1
{
  unsigned j;

  /* Sign/magnitute representation. sign = 0 for positive, -1 for
     negative */
  signed char sign[2][2];
  mp_limb_t R[2][2];
};

/* Multiplies a,b by 2^(-2j) R. Changes the signs of the rows of R if
   needed to make a, b >= 0. Needs n limbs of temporary storage.
   Returns normalized length. */
static mp_size_t
bgcd_matrix1_apply (struct bgcd_matrix1 *m,
		    mp_ptr ap, mp_ptr bp, mp_size_t n, mp_ptr tp)
{
  /* Top two limbs */
  mp_limb_t a0, a1, b0, b1;
  int s0;
  int s1;
  int ts;
  unsigned j;

  /* Make a copy of a */
  MPN_COPY (tp, ap, n);

  s0 = m->sign[0][0];
  s1 = m->sign[0][1];
  
  if ( (s0 ^ s1) < 0)
    {
      a0 = submul2_1 (ap, &ts, ap, bp, n, m->R[0][0], m->R[0][1]);
      a1 = 0;
      ts ^= s0;
      m->sign[0][0] ^= ts;
      m->sign[0][1] ^= ts;
    }
  else
    {
      a0 = addmul2_1 (ap, &a1, ap, bp, n, m->R[0][0], m->R[0][1]);
      m->sign[0][0] = m->sign[0][1] = 0;
    }

  s0 = m->sign[1][0];
  s1 = m->sign[1][1];

  if ( (s0 ^ s1) < 0)
    {
      b0 = submul2_1 (bp, &ts, bp, tp, n, m->R[1][1], m->R[1][0]);
      b1 = 0;
      ts ^= s1;
      m->sign[1][0] ^= ts;
      m->sign[1][1] ^= ts;
    }
  else
    {
      b0 = addmul2_1 (bp, &b1, bp, tp, n, m->R[1][1], m->R[1][0]);
      m->sign[1][0] = m->sign[1][1] = 0;
    }

  /* Shift 2j bits right */
  j = 2 * m->j;

  if (j == GMP_NUMB_BITS)
    {
      ASSERT (n >= 2);
      ASSERT (ap[0] == 0);
      ASSERT (bp[0] == 0);
      ASSERT (a1 == 0);
      ASSERT (b1 == 0);
      
      MPN_COPY_INCR (ap, ap+1, n-1);
      MPN_COPY_INCR (bp, bp+1, n-1);

      ap[n-1] = a0;
      bp[n-1] = b0;
    }
  else if (j < GMP_NUMB_BITS)
    {
      /* This is the only case where n == 1 is allowed */
      ASSERT (a1 == 0);
      ASSERT (b1 == 0);
      ASSERT_NOCARRY (mpn_rshift (ap, ap, n, j));
      ASSERT_NOCARRY (mpn_rshift (bp, bp, n, j));

      ap[n-1] |= (a0 << (GMP_NUMB_BITS - j)) & GMP_NUMB_MASK;
      bp[n-1] |= (b0 << (GMP_NUMB_BITS - j)) & GMP_NUMB_MASK;

      ASSERT ( (a0 >> j) == 0);
      ASSERT ( (b0 >> j) == 0);
    }
  else
    {
      ASSERT (n >= 2);

      j -= GMP_NUMB_BITS;
      ASSERT (ap[0] == 0);
      ASSERT (bp[0] == 0);

      ASSERT_NOCARRY (mpn_rshift (ap, ap + 1, n - 1, j));
      ASSERT_NOCARRY (mpn_rshift (bp, bp + 1, n - 1, j));

      ap[n-2] |= (a0 << (GMP_NUMB_BITS - j)) & GMP_NUMB_MASK;
      bp[n-2] |= (b0 << (GMP_NUMB_BITS - j)) & GMP_NUMB_MASK;

      ap[n-1] = (a0 >> j) | (a1 << (GMP_NUMB_BITS - j));
      bp[n-1] = (b0 >> j) | (b1 << (GMP_NUMB_BITS - j));
    }

  ASSERT (ODD (ap[0]));
  ASSERT (!ODD (bp[0]));

  return n - ( (ap[n-1] | bp[n-1]) == 0);
}

/* When hbgcd is computed and results in j bits of reduction, then the
   matrix elements will fit in floor (11 (j+1) / 8) bits.

   If the input to hbgcd is n limbs, and k bits of desired reduction,
   then 2k <= n GMP_NUMB_BITS and j < k. Then elements of the resulting
   matrix must fit in

     floor (11 (j+1) / 8) <= 11 k / 8 <= 11 n * GMP_NUMB_BITS / 16

   Rounding up to an integral number of limbs, we get the limit

     ceil (11 n / 16)

   Table for small values:

     n   maximum size of matrix entries

     1   1
     2   2
     3   3
     4   3
     5   4
     6   5
     7   5
     8   6

   For the computations, we need one extra limb.
*/

void
mpn_bgcd_matrix_init (struct bgcd_matrix *m, mp_ptr limbs, mp_size_t alloc)
{
  m->alloc = alloc;
  m->j = 0;
  m->n = 1;

  memset (m->sign, 0, sizeof (m->sign));

  m->R[0][0] = limbs;
  m->R[0][1] = limbs + alloc;
  m->R[1][0] = limbs + 2 * alloc;
  m->R[1][1] = limbs + 3 * alloc;

  MPN_ZERO (limbs, 4 * alloc);
  m->R[0][0][0] = m->R[1][1][0] = 1;
}

/* Needs zlimbs + 1 + m->n <= m->alloc limbs of temporary storage */
static void
bgcd_matrix_mul_q (struct bgcd_matrix *m, mp_size_t zlimbs, unsigned zbits,
		   mp_ptr qp, int qsign, mp_ptr tp)
{
  unsigned i;
  mp_size_t n, nn;
  mp_limb_t grow;

  ASSERT (tp != qp);
  ASSERT (qp[zlimbs] < ((mp_limb_t ) 1 << zbits));

  /* To multiply a column (u ; v) by (0, 2^j ; 2^j, q), set

     u = 2^j u + q v
     v = 2^j v

     and then swap u, v. We need temporary storage for t = q * v.

     Size should be increased by zlimbs or zlimbs + 1.
  */

  grow = 0;

  n = m->n;
  nn = n + zlimbs;

  ASSERT (nn + 1 <= m->alloc);

  for (i = 0; i < 2; i++)
    {
      mp_ptr up = m->R[0][i];
      mp_ptr vp = m->R[1][i];
      int us = m->sign[0][i];
      int vs = m->sign[1][i];

      /* Set u <<= j */
      if (zbits)
	{
	  up[nn] = mpn_lshift (up + zlimbs, up, n, zbits);
	}
      else
	{
	  MPN_COPY_DECR (up + zlimbs, up, n);
	  ASSERT (up[nn] == 0);
	}

      /* We could make some clever use of negate_diff, but it's easier to
	 clear the low limbs first. */
      MPN_ZERO (up, zlimbs);

      /* Set t = q * v, and u += t */

      if (n > zlimbs)
	mpn_mul (tp, vp, n, qp, zlimbs + 1);
      else
	mpn_mul (tp, qp, zlimbs + 1, vp, n);

      if ( (us ^ vs ^ qsign) < 0)
	{
	  mp_limb_t cy;

	  cy = mpn_sub_n (up, up, tp, nn + 1);
	  if (cy > 0)
	    {
	      ASSERT_NOCARRY (negate_diff (up, nn + 1));
	      us = ~us;
	    }
	}
      else
	{
	  ASSERT_NOCARRY (mpn_add_n (up, up, tp, nn + 1));
	}
      grow |= up[nn];

      /* Set v <<= j */
      if (zbits)
	{
	  vp[nn] = mpn_lshift (vp + zlimbs, vp, n, zbits);
	  grow |= vp[nn];
	}
      else
	{
	  MPN_COPY_DECR (vp + zlimbs, vp, n);
	  ASSERT (vp[nn] == 0);
	}

      MPN_ZERO (vp, zlimbs);

      MP_PTR_SWAP (m->R[0][i], m->R[1][i]);
      m->sign[0][i] = vs;
      m->sign[1][i] = us;
    }
  m->n = nn + (grow != 0);
  m->j += zlimbs * GMP_NUMB_BITS + zbits;
}

/* Needs m->n limbs of temporary storage. */
static void
bgcd_matrix_mul_1 (struct bgcd_matrix *m, const struct bgcd_matrix1 *m1,
		   mp_ptr tp)
{
  unsigned i;
  mp_limb_t grow;
  mp_limb_t r00 = m1->R[0][0];
  mp_limb_t r01 = m1->R[0][1];
  mp_limb_t r10 = m1->R[1][0];
  mp_limb_t r11 = m1->R[1][1];
  int s00 = m1->sign[0][0];
  int s01 = m1->sign[0][1];
  int s10 = m1->sign[1][0];
  int s11 = m1->sign[1][1];

  /* Carries, for the unlikely case that R grows by two limbs. */
  mp_limb_t c[2][2];
  
  mp_size_t n = m->n;

  ASSERT (n + 1 <= m->alloc);

  grow = 0;
  
  for (i = 0; i < 2; i++)
    {
      mp_ptr up = m->R[0][i];
      mp_ptr vp = m->R[1][i];
      int us = m->sign[0][i];
      int vs = m->sign[1][i];
      int ts;
      mp_limb_t uh, vh;

      /* Make a copy of u */
      MPN_COPY (tp, up, n);

      if ( (s00 ^ s01 ^ us ^ vs) < 0)
	{
	  uh = submul2_1 (up, &ts, up, vp, n, r00, r01);
	  c[0][i] = 0;
	  ts ^= s00 ^ us;
	}
      else
	{
	  uh = addmul2_1 (up, &c[0][i], up, vp, n, r00, r01);
	  ts = s00 ^ us;	  
	}
      if ( (s10 ^ s11 ^ us ^ vs) < 0)
	{
	  int s;
	  vh = submul2_1 (vp, &s, vp, tp, n, r11, r10);
	  c[1][i] = 0;
	  vs ^= s11 ^ s;
	}
      else
	{
	  vh = addmul2_1 (vp, &c[1][i], vp, tp, n, r11, r10);	  
	  vs ^= s11;
	}

      up[n] = uh;
      vp[n] = vh;
      grow |= (uh | vh);

      m->sign[0][i] = ts;
      m->sign[1][i] = vs;
    }
  if ( (c[0][0] | c[0][1] | c[1][0] | c[1][1]) != 0)
    {
      /* Growth by two limbs. */
      m->n += 2;
      ASSERT (m->n + 1 <= m->alloc);
      
      m->R[0][0][n+1] = c[0][0];
      m->R[0][1][n+1] = c[0][1];
      m->R[1][0][n+1] = c[1][0];
      m->R[1][1][n+1] = c[1][1];
    }
  else
    {
      m->n += (grow != 0);
      ASSERT (m->n + 1 <= m->alloc);
    }
  m->j += m1->j;
}

/* Computes r = u x + v y. Needs temporary space 2*(xn + un). The
   least significant xn + un limbs are stored at rp. Returns carry.

   rp == tp is allowed. */
static mp_limb_t
addmul2_n (mp_ptr rp, mp_srcptr xp, mp_srcptr yp, mp_size_t xn,
	   mp_srcptr up, mp_srcptr vp, mp_size_t un, mp_ptr tp)
{
  mp_size_t n = xn + un;
  mp_ptr t1p = tp;
  mp_ptr t2p = tp + n;

  if (xn >= un)
    {
      mpn_mul (t1p, xp, xn, up, un);
      mpn_mul (t2p, yp, xn, vp, un);
    }
  else
    {
      mpn_mul (t1p, up, un, xp, xn);
      mpn_mul (t2p, vp, un, yp, xn);
    }

  return mpn_add_n (rp, t1p, t2p, n);
}

/* Computes r = u x - v y. Needs temporary space 2*(xn + un). Result,
   xn + un limbs, is stored at rp. Returns -1 or zero depending on the
   sign of the difference.

   rp may be the same as either input or tp. */
static int
submul2_n (mp_ptr rp, mp_srcptr xp, mp_srcptr yp, mp_size_t xn,
	   mp_srcptr up, mp_srcptr vp, mp_size_t un, mp_ptr tp)
{
  mp_size_t n = xn + un;
  mp_ptr t1p = tp;
  mp_ptr t2p = tp + n;

  if (xn >= un)
    {
      mpn_mul (t1p, xp, xn, up, un);
      mpn_mul (t2p, yp, xn, vp, un);
    }
  else
    {
      mpn_mul (t1p, up, un, xp, xn);
      mpn_mul (t2p, vp, un, yp, xn);
    }

  while (t1p[n-1] == t2p[n-1])
    {
      rp[--n] = 0;
      if (n == 0)
	return 0;
    }

  if (t1p[n-1] > t2p[n-1])
    {
      ASSERT_NOCARRY (mpn_sub_n (rp, t1p, t2p, n));
      return 0;
    }
  else
    {
      ASSERT (t1p[n-1] < t2p[n-1]);
      ASSERT_NOCARRY (mpn_sub_n (rp, t2p, t1p, n));
      return -1;
    }
}

/* Sets m = s * m. Needs temporary space 2 * (m->n + s->n) + m->n = 3 * m->n + 2 * s->n */
static void
bgcd_matrix_mul (struct bgcd_matrix *m, const struct bgcd_matrix *s,
		 mp_ptr tp)
{
  unsigned i;
  mp_limb_t grow = 0;
  mp_ptr r00 = s->R[0][0];
  mp_ptr r01 = s->R[0][1];
  mp_ptr r10 = s->R[1][0];
  mp_ptr r11 = s->R[1][1];
  int s00 = s->sign[0][0];
  int s01 = s->sign[0][1];
  int s10 = s->sign[1][0];
  int s11 = s->sign[1][1];

  mp_size_t mn = m->n;
  mp_size_t sn = s->n;
  mp_size_t n = m->n + s->n;

  /* Space for a copy of u */
  mp_ptr cp = tp + 2 * n;

  ASSERT (n + 1 <= m->alloc);

  for (i = 0; i < 2; i++)
    {
      mp_ptr up = m->R[0][i];
      mp_ptr vp = m->R[1][i];
      int us = m->sign[0][i];
      int vs = m->sign[1][i];
      int ts;

      /* Make a copy of u */
      MPN_COPY (cp, up, mn);

      /* Set u = r00 * u + r01 * v */

      /* If r00 u and r01 v have different signs, we should subtract */
      ts = us ^ s00;
      if ( (s00 ^ us ^ s01 ^ vs) < 0)
	{
	  ts ^= submul2_n (up, cp, vp, mn, r00, r01, sn, tp);
	  up[n] = 0;
	}
      else
	{
	  up[n] = addmul2_n (up, cp, vp, mn, r00, r01, sn, tp);
	  grow |= up[n];
	}

      /* Set v = r10 (old) u + r11 v */

      /* If r10 u and r11 v have different signs, we should subtract */
      if ( (s10 ^ us ^ s11 ^ vs) < 0)
	{
	  vs = us ^ s10 ^ submul2_n (vp, cp, vp, mn, r10, r11, sn, tp);
	  vp[n] = 0;
	}
      else
	{
	  vp[n] = addmul2_n (vp, cp, vp, mn, r10, r11, sn, tp);
	  vs = us ^ s10;
	  grow |= vp[n];
	}

      m->sign[0][i] = ts;
      m->sign[1][i] = vs;
    }

  if (grow > 0)
    m->n = n + 1;
  else
    {
      /* We don't need full normalization, stripping one leading zero
	 limb should be sufficient to get size below the bound. */
      
      m->n = n - (m->R[0][0][n-1] == 0 && m->R[0][1][n-1] == 0
		  && m->R[1][0][n-1] == 0 && m->R[1][1][n-1] == 0);
    }
  ASSERT (m->n + 1 <= m->alloc);

  m->j += s->j;
}

/* Sets a = a + c * 2^(shift * GMP_NUMB_BITS),
	________________________
      _|____________________a___|
   + |__________________c_|
			-->     <--  shift
   
   Stores MAX(an, cn + shift) limbs of the result at ap, and returns
   carry.
*/

static mp_limb_t
bgcd_fix_add (mp_ptr ap, mp_size_t an, mp_srcptr cp, mp_size_t cn, mp_size_t shift)
{
  if (cn + shift < an)
    {
      /* Can this really happen? */
      ASSERT_ALWAYS (0 == "Can this happen?");
      return mpn_add (ap + shift, ap + shift, an - shift, cp, cn);
    }
  else if (shift < an)
    return mpn_add (ap + shift, cp, cn,
		    ap + shift, an - shift);
  else
    {
      MPN_COPY (ap + shift, cp, cn);
      if (shift > an)
	MPN_ZERO (ap + an, shift - an);

      return 0;
    }
}

/* Sets a = a - c * 2^(shift * GMP_NUMB_BITS,
	________________________
      _|____________________a___|
   - |__________________c_|
			-->     <--  shift

   Stores the cn + shift limbs of absolute value at ap, and returns 0
   or -1 depending on the sign.
*/
static mp_size_t
bgcd_fix_sub (mp_ptr ap, mp_size_t an, mp_srcptr cp, mp_size_t cn, mp_size_t shift)
{
  while (an > cn + shift)
    {
      if (ap[an-1] == 0)
	an--;
      else
	{
	  ASSERT_NOCARRY (mpn_sub (ap + shift,
				   ap + shift, an - shift, cp, cn));
	  return 0;
	}
    }

  while (cn > 0 && cn + shift > an)
    {
      if (cp[cn - 1] == 0)
	ap[shift + --cn] = 0;
      else
	{
	  /* Result must be negative */
	  if (shift == 0)
	    ASSERT_NOCARRY (mpn_sub (ap, cp, cn, ap, an));
	  else
	    {
	      /* Negate the lower part of a */
	      if (shift <= an)
		{
		  ASSERT_NOCARRY (mpn_sub (ap + shift, cp, cn, ap + shift, an - shift));
		  if (negate_diff (ap, shift) == 0)
		    MPN_DECR_U (ap + shift, cn, 1);
		}
	      else
		{
		  if (negate_diff (ap, an) == 0)
		    {
		      mp_size_t i;
		      for (i = an; i < shift; i++)
			ap[i] = GMP_NUMB_MASK;

		      MPN_DECR_U (ap + shift, cn, 1);
		    }
		  else
		    MPN_ZERO (ap + an, shift - an);
		}
	    }
	  return -1;
	}
    }
  while (cn > 0)
    {
      if (ap[shift + cn - 1] == cp[cn - 1])
	ap[shift + --cn] = 0;
      else if (ap[shift + cn - 1] > cp[cn - 1])
	{
	  /* Result must be positive */
	  ASSERT_NOCARRY (mpn_sub_n (ap + shift, ap + shift, cp, cn));
	  return 0;
	}
      else
	{
	  /* Result must be negative */
	  ASSERT (ap[shift + cn - 1] < cp[cn - 1]);
	  ASSERT_NOCARRY (mpn_sub_n(ap + shift, cp, ap + shift, cn));

	  if (shift > 0 && negate_diff (ap, shift) == 0)
	    MPN_DECR_U (ap + shift, cn, 1);
	  return -1;
	}
    }
  return 0;
}

/* Computes (C ; D) = 2^(-2j) R (A; B) =

     2^(k * GMP_NUMB_BITS - 2j) R (a ; b) + (c ; d)

   where a, b are stored at ap[k, ..., n-1], bp[k, ..., n-1],
   and c, d are stored at   ap[0, ..., l-1], bp[0, ..., l-1].

   Results C, D are also stored at ap, bp. If necessary to get
   non-negative values, flip the signs of the rows of the matrix R.

   Sizes:

   Input c,d are l limbs. The output C,D must always fit in n limbs
   (and we do *not* have any temporary space beyond the n:th limb).

   The temporaries R(a;b) must fit in n limbs. This is because
   #(R(a;b)) <= #R + (n-k) GMP_NUMB_BITS + 1. From the bounds #R <=
   11(j+1)/8 and k GMP_NUMB_BITS >= 2(j+1) we get

   #(R(a;b)) <= n GMP_NUMB_BITS + (11 - 16)(j+1)/8 + 1
              = n GMP_NUMB_BITS - (5j -3)/8 < n GMP_NUMB_BITS.

   For shifted numbers 2^(k * GMP_NUMB_BITS - 2j) R (a ; b) we get

     k * GMP_NUMB_BITS - 2j + #R + (n-k) GMP_NUMB_BITS + 1
     <= n * GMP_NUMB_BITS + (11 j + 11 - 16j)/8 + 1
     <= n * GMP_NUMB_BITS + (17 - 5j)/8

   Taking the fllor of this expresstion, we see that if j >= 2, n
   limbs should be sufficient. A more careful study of j = 1 shows
   that n limbs is sufficient for all j >= 1.
   
   Temporary storage needed: 4*n.
*/
static mp_size_t
bgcd_matrix_apply (struct bgcd_matrix *m,
		   mp_ptr ap, mp_ptr bp, mp_size_t n,
		   mp_size_t k, mp_size_t l, mp_ptr tp)
{
  mp_ptr r00 = m->R[0][0];
  mp_ptr r01 = m->R[0][1];
  mp_ptr r10 = m->R[1][0];
  mp_ptr r11 = m->R[1][1];
  int s00 = m->sign[0][0];
  int s01 = m->sign[0][1];
  int s10 = m->sign[1][0];
  int s11 = m->sign[1][1];

  mp_ptr cp = tp + 2*n;
  mp_ptr dp = tp + 3*n;
  mp_size_t nn;
  mp_limb_t ch;
  mp_limb_t dh;
  int cs;
  int ds;

  unsigned shift;
  unsigned shift_bits;
  mp_size_t shift_limbs;

  /* This function probably works for k == 1, but then it's better to
     use hbgcd2 and bgcd_matrix1_apply. */
  ASSERT (k >= 2);
  ASSERT (l <= k);
  ASSERT (2 * m->j < k * GMP_NUMB_BITS);

  nn = n - k + m->n;
  ASSERT (nn <= n);

  /* There are three chances for nn to grow: addmul2_n, left shift,
     and bgcd_fix_add. */

  cs = s00;

  /* Compute R * (a ; b), and store result at cp, dp */
  if ( (s00 ^ s01) < 0)
    {
      cs ^= submul2_n (cp, ap + k, bp + k, n - k,
		      r00, r01, m->n, tp);
      ch = 0;
    }
  else
    ch = addmul2_n (cp, ap + k, bp + k, n - k,
		    r00, r01, m->n, tp);

  ds = s10;
  if ( (s10 ^ s11) < 0)
    {
      ds ^= submul2_n (dp, ap + k, bp + k, n - k,
		      r10, r11, m->n, tp);
      dh = 0;
    }
  else
    dh = addmul2_n (dp, ap + k, bp + k, n - k,
			r10, r11, m->n, tp);

  if ( (ch | dh) > 0)
    {
      ASSERT (nn < n);
      cp[nn] = ch;
      dp[nn] = dh;
      nn++;
    }

  shift = k * GMP_NUMB_BITS - 2 * m->j;
  shift_limbs = shift / GMP_NUMB_BITS;
  shift_bits = shift % GMP_NUMB_BITS;

  if (shift_bits > 0)
    {
      ch = mpn_lshift (cp, cp, nn, shift_bits);
      dh = mpn_lshift (dp, dp, nn, shift_bits);

      if ( (ch | dh) > 0)
	{
	  ASSERT (nn < n);
	  cp[nn] = ch;
	  dp[nn] = dh;
	  nn++;
	}
    }
  
  /* Update a */
  ASSERT (nn + shift_limbs <= n);

  if (cs >= 0)
    ch = bgcd_fix_add (ap, l, cp, nn, shift_limbs);

  else
    {
      int s = bgcd_fix_sub (ap, l, cp, nn, shift_limbs);

      ch = 0;

      /* Flip signs of first matrix row, if needed. */
      m->sign[0][0] ^= s;
      m->sign[0][1] ^= s;
    }

  /* Update b */
  if (ds >= 0)
    dh = bgcd_fix_add (bp, l, dp, nn, shift_limbs);
  else
    {
      int s = bgcd_fix_sub (bp, l, dp, nn, shift_limbs);

      dh = 0;

      /* Flip signs of second matrix row, if needed. */
      m->sign[1][0] ^= s;
      m->sign[1][1] ^= s;
    }

  nn += shift_limbs;
  if (nn < l)
    {
      /* Can this really happen? */
      ASSERT_ALWAYS (0 == "Can this happen?");
      nn = l;
    }
  
  if ( (ch | dh) > 0)
    {
      ASSERT (nn < n);
      ap[nn] = ch;
      bp[nn] = dh;
      nn++;
    }

  return nn;
}

/* Input: b is odd, k < GMP_NUMB_BITS. (Common case is a odd, k > 0).

   Returns q, such that -2^k <= q < 2^k and v (a + q b) > k, i.e. there
   are at least k+1 zeros at the least significant end.
*/

static inline mp_limb_signed_t
bdiv_1 (mp_limb_t a, mp_limb_t b, unsigned k)
{
  mp_limb_t b_inv;
  mp_limb_signed_t q;
  mp_limb_t bit;
#if 0
#define BDIV_TABLE_SIZE 3
#define BDIV_TABLE_MASK ((1 << (BDIV_TABLE_SIZE + 1)) - 1)
  
  /* Table of -a b^{-1} mod 2^{k+1}, indexed by

         [b_k b_{k-1} ... b_1 a_k a_{k-1} a_{k-2} ... a_0]
  */

  static const char qtable[1L << (BDIV_TABLE_SIZE * 2 + 1)] =
    {
      /* binv =  1 */  0, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1, 
      /* binv = 11 */  0,  5, 10, 15,  4,  9, 14,  3,  8, 13,  2,  7, 12,  1,  6, 11, 
      /* binv = 13 */  0,  3,  6,  9, 12, 15,  2,  5,  8, 11, 14,  1,  4,  7, 10, 13, 
      /* binv =  7 */  0,  9,  2, 11,  4, 13,  6, 15,  8,  1, 10,  3, 12,  5, 14,  7, 
      /* binv =  9 */  0,  7, 14,  5, 12,  3, 10,  1,  8, 15,  6, 13,  4, 11,  2,  9, 
      /* binv =  3 */  0, 13, 10,  7,  4,  1, 14, 11,  8,  5,  2, 15, 12,  9,  6,  3, 
      /* binv =  5 */  0, 11,  6,  1, 12,  7,  2, 13,  8,  3, 14,  9,  4, 15, 10,  5, 
      /* binv = 15 */  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 
    };
  
  ASSERT (ODD (b));
  ASSERT (k < GMP_NUMB_BITS);

  if (k <= BDIV_TABLE_SIZE)
    {
      bit = CNST_LIMB(1) << k;
      q = qtable[ (((b-1) & BDIV_TABLE_MASK) << BDIV_TABLE_SIZE )
		  | (a & BDIV_TABLE_MASK) ] & (2*bit - 1);
    }
  else
#endif
    {
      mp_limb_t mask;

      /* We need (k+1)-bit inverse */
      b_inv = modlimb_invert_table[(b/2) & 0x7f];
      if (k >= 8)
	{
	  b_inv = 2 * b_inv - b_inv * b_inv * b;
	  if (k >= 16)
	    {
	      b_inv = 2 * b_inv - b_inv * b_inv * b;
	      if (GMP_NUMB_BITS > 32 && k >= 32)
		{
		  b_inv = 2 * b_inv - b_inv * b_inv * b;
		  if (GMP_NUMB_BITS > 64 && k >= 64)
		    {
		      int inv_bits = 64;
		      do
			{
			  b_inv = 2 * b_inv - b_inv * b_inv * b;
			  inv_bits *= 2;
			}
		      while (inv_bits <= k);
		    }
		}
	    }
	}

      bit = CNST_LIMB(1) << k;
      mask = 2*bit - 1;
      
      ASSERT ( (b_inv * b & mask) == 1);

      q = (- a * b_inv) & mask;
    }
  if (q >= bit)
    q -= 2*bit;

  return q;
}

/* Input is two two-limb numbers a, b (typically the least significant
   limbs of some larger numbers). The integer k is the desired number
   of bits of reduction.

   Returns an array R of single limbs, and an integer j < k. If c, d
   are formed as

     /c\    -2j   /a\
     \d/ = 2    R \b/

   then c is odd, d is even, and gcd(a,b) = gcd(c, d). R is
   constructed by repeated bdiv, where iteration stops when either

     v (d) >= k - j

   or another iteration would cause R to overflow.

   Returns 0 if no reduction is possible, otherwise j.
*/

/* two's complement version */
static unsigned
hbgcd2 (mp_limb_t ah, mp_limb_t al, mp_limb_t bh, mp_limb_t bl,
        unsigned k,
        struct bgcd_matrix1 *m)
{
  mp_limb_signed_t r00, r01, r10, r11;
  unsigned s;
  unsigned j;
  unsigned v;
  
  ASSERT (ODD (al));
  ASSERT (!ODD (bl));

  /* Multiply by (0, 2^bits; 2^bits, q) */
#define MUL_Q(R00, R01, R10, R11) do {							\
      R00 = (R00 << v) + q * R10;					\
      R01 = (R01 << v) + q * R11;					\
      R10 <<= v;							\
      R11 <<= v;							\
									\
      s += v;								\
      s += ((R00 ^ (R00 >> 1)) | (R01 ^ (R01 >> 1))) >> s;		\
      j += v;								\
  } while (0)

  /* Add q b to a. We treat q as an unsigned in the multiplication. If
     q < 0 and th * w + tl = (q + w) bl is the result of the
     unsigned multiplication, what we want

       (q + w - w) (bh w + bl)
         = q * bh * w + th w + tl - w bl =
	 = (q * bh + th - bl) w + tl             (mod w^2)
  */

#define ADDMUL2_Q(AH, AL, BH, BL) do {			\
      mp_limb_t th, tl;					\
      mp_limb_t mask = q >> (GMP_LIMB_BITS - 1);	\
      							\
      umul_ppmm (th, tl, BL, q);			\
							\
      AL += tl;						\
      AH += th + (AL < tl) + q * BH - (mask & BL);	\
  } while (0)
  
  /* NOTE: We can reduce at most GMP_NUMB_BITS - 1 bits. Reducing
     GMP_NUMB_BITS would be possible if b was preshifted, so we had 2
     * GMP_NUMB_BITS + 1 of b. But a reduction of GMP_NUMB_BITS,
     without R overflowing, is quite unlikely (probability on the
     order of 1%). */
  if (k > GMP_NUMB_BITS)
    k = GMP_NUMB_BITS;

  if (bl == 0)
    return 0;

  count_trailing_zeros (v, bl);
  if (v >= k)
    return 0;

  RSHIFT2 (bh, bl, v);
  if (v == GMP_NUMB_BITS - 1)
    {
      /* Special case that the two's complement code can't handle */
      mp_limb_signed_t q;
      mp_limb_signed_t sign;
      
      q = bdiv_1 (al, bl, v);
      ASSERT (ODD (q));

      m->R[0][0] = 0;
      m->R[0][1] = m->R[1][0] = CNST_LIMB(1) << (GMP_NUMB_BITS - 1);
      m->sign[0][0] = m->sign[0][1] = m->sign[1][0] = 0;
      
      sign = q >> (GMP_LIMB_BITS - 1);
      m->R[1][1] = (q + sign) ^ sign;
      m->sign[1][1] = sign;
      
      return (m->j = GMP_NUMB_BITS - 1);
    }
  
  r00 = r11 = 1;
  r01 = r10 = 0;
  j = 0;
  s = 1;

  /* We need 2*(k-j) bits of accuracy */
  /* Loop while we need more than one limb of accuracy. */
  for (;;)
    {
      mp_limb_signed_t q;
      unsigned oldv;
      
      /* Divide by b */
      q = bdiv_1 (al, bl, v);
      ASSERT (ODD (q));

      MUL_Q (r00, r01, r10, r11);
      ASSERT (s < GMP_NUMB_BITS);

      ADDMUL2_Q(ah, al, bh, bl);
      ASSERT ( (al & ((CNST_LIMB(1) << (v + 1)) - 1)) == 0);

      if (al == 0)
	goto split_swap;

      oldv = v;
      count_trailing_zeros (v, al);
      ASSERT (v > oldv);
      
      RSHIFT2 (ah, al, v);
      v -= oldv;

      if (j + v >= k || s + v >= GMP_NUMB_BITS)
	goto split_swap;
      else if (s + v == GMP_NUMB_BITS - 1)
	goto final_swap;
      else if (2*(k-j) <= GMP_NUMB_BITS)
	goto middle;

      /* Divide by a */
      q = bdiv_1 (bl, al, v);
      ASSERT (ODD (q));

      MUL_Q (r10, r11, r00, r01);
      ASSERT (s < GMP_NUMB_BITS);

      ADDMUL2_Q(bh, bl, ah, al);
      ASSERT ( (bl & ((CNST_LIMB(1) << (v + 1)) - 1)) == 0);

      if (bl == 0)
        goto split;

      oldv = v;
      count_trailing_zeros (v, bl);
      RSHIFT2 (bh, bl, v);
      v -= oldv;

      if (j + v >= k || s + v >= GMP_NUMB_BITS)
	goto split;
      else if (s + v == GMP_NUMB_BITS - 1)
	goto final;
      else if (2*(k-j) <= GMP_NUMB_BITS)
	break;
    }

  /* Now we can forget about ah and bh. On the other hand, we need to
     check for an overflowing R */
  for (;;)
    {
      mp_limb_signed_t q;
      unsigned oldv;
      
      /* Divide by b */
      q = bdiv_1 (al, bl, v);
      ASSERT (ODD (q));

      MUL_Q (r00, r01, r10, r11);
      ASSERT (s < GMP_NUMB_BITS);
      
      /* Update a, b */
      al += q*bl;

      ASSERT ( (al & ((CNST_LIMB(1) << (v + 1)) - 1)) == 0);

      if (al == 0)
	goto split_swap;

      oldv = v;
      count_trailing_zeros (v, al);
      al >>= v;
      v -= oldv;

      if (j + v >= k || s + v >= GMP_NUMB_BITS)
	goto split_swap;
      else if (s + v == GMP_NUMB_BITS - 1)
	goto final_swap;

    middle:
      /* Divide by a */
      q = bdiv_1 (bl, al, v);
      ASSERT (ODD (q));

      MUL_Q (r10, r11, r00, r01);
      ASSERT (s < GMP_NUMB_BITS);
      
      /* Update a, b */
      bl += q*al;

      ASSERT ( (bl & ((CNST_LIMB(1) << (v + 1)) - 1)) == 0);

      if (bl == 0)
        goto split;

      oldv = v;
      count_trailing_zeros (v, bl);
      bl >>= v;
      v -= oldv;

      if (j + v >= k || s + v >= GMP_NUMB_BITS)
	goto split;
      else if (s + v == GMP_NUMB_BITS - 1)
	break;      
    }
#undef MUL_Q
#undef ADDMUL2_Q

 final:
  if (0)
    {
    final_swap:
      MP_LIMB_SIGNED_T_SWAP (r00, r10); 
      MP_LIMB_SIGNED_T_SWAP (r01, r11);
      MP_LIMB_T_SWAP (al, bl);
    }
  if (j + v < k && s + v < GMP_NUMB_BITS)
    {
      /* One more quotient will fit in sign-magnitude representation,
	 but two's complement calculation may overflow. */
      
      mp_limb_signed_t q;
      mp_limb_signed_t t0, t1; 
      mp_limb_signed_t sign;
      
      q = bdiv_1 (al, bl, v);
      ASSERT (ODD (q));

      t0 = r00 << v;
      t1 = q * r10; /* Can't overflow, since q > -2^{-v} */

      r00 = t0 + t1;

      /* Correct sign given by the majority function */
      sign = ((t0 & t1) | (r00 & t0) | (r00 & t1)) >> (GMP_LIMB_BITS - 1);
      m->R[1][0] = (r00 + sign) ^ sign;
      m->sign[1][0] = sign;
      
      t0 = r01 << v;
      t1 = q * r11;
      r01 = t0 + t1;

      sign = ((t0 & t1) | (r01 & t0) | (r01 & t1) ) >> (GMP_LIMB_BITS - 1);
      m->R[1][1] = (r01 + sign) ^ sign;
      m->sign[1][1] = sign;

      sign = r10 >> (GMP_LIMB_BITS - 1);
      m->R[0][0] = ( (r10 << v) + sign) ^ sign;
      m->sign[0][0] = sign;

      sign = r11 >> (GMP_LIMB_BITS - 1);
      m->R[0][1] = ( (r11 << v) + sign) ^ sign;
      m->sign[0][1] = sign;

      j += v;
    }
  else
    {
      mp_limb_signed_t sign;
      if (0)
	{
	split_swap:
	  MP_LIMB_SIGNED_T_SWAP (r00, r10); 
	  MP_LIMB_SIGNED_T_SWAP (r01, r11); 
	}
    split:
      /* Split into signed/magnitude representation */

      sign = r00 >> (GMP_LIMB_BITS - 1);
      m->R[0][0] = (r00 + sign) ^sign;
      m->sign[0][0] = sign;

      sign = r01 >> (GMP_LIMB_BITS - 1);
      m->R[0][1] = (r01 + sign) ^sign;
      m->sign[0][1] = sign;

      sign = r10 >> (GMP_LIMB_BITS - 1);
      m->R[1][0] = (r10 + sign) ^sign;
      m->sign[1][0] = sign;

      sign = r11 >> (GMP_LIMB_BITS - 1);
      m->R[1][1] = (r11 + sign) ^sign;
      m->sign[1][1] = sign;
    }
  
  ASSERT (j > 0);
  m->j = j; 
  return j;  
}


/* Compute the inverse of -b,

    x * b = -1 (mod 2^(n * GMP_NUMB_BITS))

  using the iteration formula

    x_{k+1} = 2 x_k + x_k^2 b

  In fact, when x_k is IN limbs, the only thing we need is the middle
  part, limb IN to 2 IN, of the product x_k^2 b. The lower limbs are
  known already, and the higher limbs are not significant.

  Needs 3*xn limbs of temporary storage.
*/

/* NOTE: Handles input b of fewer limbs than output x.We should
   usually have 2 zlimbs <= n  */
static void
invert_limbs (mp_ptr xp, mp_size_t xn, mp_srcptr bp, mp_size_t bn, mp_ptr tp)
{
  mp_limb_t b0;
  mp_limb_t x0;

  ASSERT (bn >= 1);
  if (bn > xn)
    bn = xn;

  ASSERT (!MPN_OVERLAP_P (xp, xn, bp, xn));
  ASSERT (!MPN_OVERLAP_P (xp, xn, tp, 3*xn));
  ASSERT (!MPN_OVERLAP_P (bp, bn, tp, 3*xn));
  
  b0 = bp[0];
  modlimb_invert (x0, -b0);
  xp[0] = x0;

  if (xn >= 2)
    {
      /* Iterate once, x1 = 2 x0 + x0^2 b, and keep only the two
	 least significant limbs. */
      mp_limb_t sh, sl;
      mp_limb_t t, dummy;

      /* Compute sh, sl = x0^2 */
      umul_ppmm (sh, sl, x0, x0);

      /* Compute th, tl = x0^2 b (ignoring higher limbs) */
      umul_ppmm (t, dummy, sl, b0);  /* x0^2 b0 */

      /* We always have a carry from the low limb which we don't
	 compute. */
      xp[1] = t + sh * b0 + 1;
      if (bn > 1)
	xp[1] += sl * bp[1];
      
      if (xn > 2)
	{
	  int count;
	  int bitsize;
	  mp_size_t mask;
	  mp_size_t in;

	  /* We need at most xn + 1 limbs to compute the the square,
	     but at most xn limbs to store it. */
	  mp_ptr sp = tp;

	  /* And this product can be at most 2*xn */
	  mp_ptr pp = tp + xn;

	  count_leading_zeros (count, (mp_limb_t) xn-1);
	  bitsize = GMP_LIMB_BITS - count;
	  ASSERT (bitsize >= 2);

	  mask = 1L << (bitsize - 2);

	  for (in = 2; in < xn; mask /= 2)
	    {
	      mp_limb_t nn;
	      ASSERT (mask != 0);

	      /* We know the in least significant words of the
		 inverse, and iterate

		   x = 2*x + x^2 * b

		 to double the size. */

	      mpn_sqr_n (sp, xp, in);

	      /* Drop the high limb if we don't need it. */
	      nn = 2 * in - ((-xn & mask) != 0);

	      ASSERT (nn <= xn);

	      /* FIXME: We don't need the nn most significant
		 limbs. And we don't need the least significant in
		 limbs either, as we already know them. */
	      mpn_mul (pp, sp, nn, bp, MIN (nn, bn));

	      /* For the addition of 2x_k, why shouldn't we add in the
		 most significant limb of x_k? */
	      mpn_add_1 (xp + in, pp + in, nn - in, 1);
	      in = nn;
	    }
	}
    }
}

/* For a, b odd, a of size n, b of size n - zlimbs, j = zlimbs *
   GMP_NUMB_BITS + zbits, compute q = - a (b)^-1 mod 2^(j+1), with
   -2^j < q < 2^j, and set

     r <-- 2^(-j) (a + q b)

   Stores the zlimb least significant limbs of r at ap, and returns
   the most significant limb (which is always 0 or 1). Stores the
   quotient, zlimbs + 1, limbs, at qp (if zbits == 0 and zlimbs > 0,
   then the most significant limb of q is always zero).

   Needs 4 * zlimbs of temporary storage to compute the quotient, and
   n limbs to compute the remainder (but not at the same time).
*/

#define BDIVMOD_N_ITCH(n, zlimbs) (MAX (4*(zlimbs), (n)))

static mp_limb_t
bdivmod_n (mp_ptr qp, int *qsign,
	   int *rsign,
	   mp_ptr ap, mp_srcptr bp, mp_size_t n,
	   mp_size_t zlimbs, unsigned zbits,
	   mp_ptr tp)
{
  mp_limb_signed_t q;
  mp_limb_t hi;
  mp_size_t scratch = BDIVMOD_N_ITCH (n, zlimbs);
  
  ASSERT (ODD (ap[0]));
  ASSERT (ODD (bp[0]));

  ASSERT (!MPN_OVERLAP_P (qp, zlimbs + 1, ap, n));
  ASSERT (!MPN_OVERLAP_P (qp, zlimbs + 1, bp, n - zlimbs));

  ASSERT (!MPN_OVERLAP_P (ap, n, bp, n - zlimbs));

  ASSERT (!MPN_OVERLAP_P (tp, scratch, ap, n));
  ASSERT (!MPN_OVERLAP_P (tp, scratch, bp, n - zlimbs));

  if (zlimbs == 0)
    hi = 0;
  else
    {
      /* First compute q such that a + q*b has zlimbs zero limbs at
	 the least significant end. */

      invert_limbs (tp, zlimbs, bp, n - zlimbs, tp + zlimbs);

      /* Let q = t * a. We only want the least significant
	 limbs. */
      mpn_mul_n (tp + zlimbs, tp, ap, zlimbs);
      MPN_COPY (qp, tp + zlimbs, zlimbs);

      /* Compute q*b. We don't really need the zlimb least
	 significant limbs. */
      if (zlimbs <= n - zlimbs)
	mpn_mul (tp, bp, n - zlimbs, qp, zlimbs);
      else
	mpn_mul (tp, qp, zlimbs, bp, n - zlimbs);

      /* We must get a carry when adding the lower limbs (since a odd
	 => [ap, zlimbs] != 0). This is actually the only piece of
	 this code that depends on a being odd. */

      MPN_COPY_INCR (ap, ap + zlimbs, n - zlimbs);
      n -= zlimbs;

      hi = mpn_add_n (ap, ap, tp + zlimbs, n);
      hi += mpn_add_1 (ap, ap, n, 1);
    }

  /* Now we have zbits + 1 bits left to clear of ap[0] */
  q = bdiv_1 (ap[0], bp[0], zbits);

  if (q >= 0)
    {
      *qsign = 0;
      *rsign = 0;

      /* Can't overflow, since q < 2^(GMP_NUMB_BITS-1) and hi <= 1 */
      hi += mpn_addmul_1 (ap, bp, n, q);
    }
  else
    {
      mp_limb_t cy;

      *qsign = -1;
      q = -q;

      cy = mpn_submul_1 (ap, bp, n, q);
      if (cy <= hi)
	{
	  *rsign = 0;
	  hi -= cy;
	}
      else
	{
	  *rsign = -1;

	  ASSERT_NOCARRY (negate_diff (ap, n));
	  hi = cy - hi - 1;
	}

      if (zlimbs > 0)
	{
	  ASSERT_NOCARRY (negate_diff (qp, zlimbs));
	  q--;
	}
    }

  qp[zlimbs] = q;

  if (zbits > 0)
    {
      mpn_rshift (ap, ap, n, zbits);
      ap[n-1] |= (hi << (GMP_NUMB_BITS - zbits) & GMP_NUMB_MASK);
      hi >>= zbits;
    }

  return hi;
}

/* Takes as input two n-limb numbers (typically the least
   significant limbs of some larger numbers), a odd, b even, and an
   integer k.

   Returns an array R, an integer j, two numbers a', b' such that

     /a'\    -2j   /a\
     \b'/ = 2    R \b/

   where

     a' is odd
     v (2^j a') < k,
     v (2^j b') >= k
*/

/* Needs temporary space for calling bgcd_matrix1_apply,
   bgcd_matrix1_mul, bdivmod_n, bgcd_matrix_mul_q, and storing q. The
   most demanding is bdivmod, for which we need 5*n/2 limbs. */

#define HBGCD_N_BASE_ITCH(n) (5*(n)/2)

static mp_size_t
hbgcd_n_base (mp_ptr ap, mp_ptr bp, mp_size_t n, unsigned k,
	      struct bgcd_matrix *m, mp_ptr tp)
{  
  ASSERT (2 * k <= n * GMP_NUMB_BITS);
  ASSERT (MPN_BGCD_MATRIX_ITCH (n) <= m->alloc);

  /* Should be initialized already */
  ASSERT (m->j == 0);

  ASSERT (n >= 2);
  ASSERT (ODD (ap[0]));
  ASSERT (!ODD (bp[0]));

  for (;;)
    {
      mp_size_t zlimbs;
      unsigned zbits;

      ASSERT (ODD (ap[0]));
      ASSERT (!ODD (bp[0]));

      zlimbs = power_of_2 (&zbits, bp, n);

      if (zlimbs * GMP_NUMB_BITS + zbits + m->j >= k)
	break;
	
      if (zlimbs == 0)
	{
	  /* FIXME: Some duplication with next case. */
	  struct bgcd_matrix1 m1;
	  
	  /* Here, we may have n == 1, and then the values ap[1] and
	     bp[1] will not be used. */
	  hbgcd2 (ap[1], ap[0], bp[1], bp[0], k - m->j, &m1);
	  ASSERT (m1.j > 0);

	  /* Modifies the signs of the matrix elements. */
	  n = bgcd_matrix1_apply (&m1, ap, bp, n, tp);

	  bgcd_matrix_mul_1 (m, &m1, tp);
	}
      else
	{
	  mp_size_t i;
	  mp_limb_t cy;
	  int qsign;
	  int rsign;

	  ASSERT (zlimbs <= (n-1) /2);

	  if (zbits)
	    ASSERT_NOCARRY (mpn_rshift (bp, bp + zlimbs, n - zlimbs, zbits));
	  else
	    MPN_COPY_INCR (bp, bp + zlimbs, n - zlimbs);

	  /* Needs zlimbs + 1 to store the quotient, and MAX (n, 4
	     zlimbs) for the computations. Since zlimbs <= (n-1)/2,
	     the total need is at most

	       (n + 1) / 2 + MAX (n, 2*(n-1)) = n/2 + 1/2 + 2*n - 2 <= 5*n/2
	  */
	  cy = bdivmod_n (tp, &qsign, &rsign, ap, bp, n,
			  zlimbs, zbits, tp + zlimbs + 1);

	  /* Needs zlimbs + 1 + m->n <= 2*n limbs of temporary storage. */
	  bgcd_matrix_mul_q (m, zlimbs, zbits, tp, qsign, tp + zlimbs + 1);

	  if (rsign < 0)
	    {
	      m->sign[1][0] = ~m->sign[1][0];
	      m->sign[1][1] = ~m->sign[1][1];
	    }

	  n -= zlimbs;
	  if (cy > 0)
	    {
	      ap[n] = cy;
	      bp[n] = 0;
	      n++;
	    }

	  /* Swapping pointers would confuse our caller, so we have to swap
	     the contents. */
	  for (i = 0; i<n; i++)
	    MP_LIMB_T_SWAP (ap[i], bp[i]);
	}
    }

  return n;
}

/* For input of size n, we need 11*n/2 + 3 limbs of temporary storage.

   This is sufficient for the first recursive call and the division
   step. For the final recursive call, we need 3*n/2 + 7 for the
   matrix, and 4*(n-1) limbs for the work after the recursive call.

   The recursive call needs 11*n/4 + 3limbs of storage, so this works out
   because

     4*(n-1) - (11*n/4 + 3) = (16 - 11) n / 4 - 4 - 3 >= 3, for n >= 8.

   Actually, approximately 5*n/4 limbs are unused by the recursive
   call, so perhaps it is possible to rearrange the storage so that we
   get by with only 17*n/4???
*/

mp_size_t
mpn_hbgcd_n_itch (mp_size_t n)
{
  if (BELOW_THRESHOLD (n, HBGCD_THRESHOLD))
    return HBGCD_N_BASE_ITCH (n);
  else
    return 11*n/2 + 3;
}

mp_size_t
mpn_hbgcd_n (mp_ptr ap, mp_ptr bp, mp_size_t n, unsigned k,
	     struct bgcd_matrix *m, mp_ptr tp)
{
  /* Original size */
  mp_size_t l = n;

  if (BELOW_THRESHOLD (n, HBGCD_THRESHOLD))
    return hbgcd_n_base (ap, bp, n, k, m, tp);
  
  /* Should be initialized already */
  ASSERT (m->j == 0);

  ASSERT (n >= 2);
  ASSERT (ODD (ap[0]));
  ASSERT (!ODD (bp[0]));

  /* FIXME: Should we allow smaller n? */
  ASSERT (2 * k <= n * GMP_NUMB_BITS);

  ASSERT (MPN_BGCD_MATRIX_ITCH (n) <= m->alloc);

  {
    /* First recursive call */
    mp_size_t zlimbs;
    unsigned zbits;
    unsigned k1;
    mp_size_t n1;
    mp_size_t nn;

    zlimbs = power_of_2 (&zbits, bp, n);

    if (zlimbs * GMP_NUMB_BITS + zbits >= k)
      goto done;

    /* Split on a limb boundary */
    n1 = (n+1)/2;
    k1 = n1 * (GMP_NUMB_BITS / 2);

    ASSERT (k1 >= (k+1)/2);

    if (zlimbs * GMP_NUMB_BITS + zbits < k1)
      {
	ASSERT (n1 < n);

	nn = mpn_hbgcd_n (ap, bp, n1, k1, m, tp);

	ASSERT (m->j > 0);

	/* Needs space 4 * l */
	n = bgcd_matrix_apply (m, ap, bp, n, n1, nn, tp);
      }
    ASSERT (m->j < k1);
  }
  ASSERT (m->n <= l);
  {
    /* Division. This stage needs temporary storage of 5 * l / 2 limbs
       (same analysis as the division case in in hbgcd_n_base) */
    mp_size_t zlimbs;
    unsigned zbits;
    mp_size_t i;
    mp_limb_t cy;
    int qsign;
    int rsign;

    zlimbs = power_of_2 (&zbits, bp, n);

    if (zlimbs * GMP_NUMB_BITS + zbits + m->j >= k)
      goto done;

    ASSERT (zlimbs <= (n-1) /2);

    if (zbits)
      ASSERT_NOCARRY (mpn_rshift (bp, bp + zlimbs, n - zlimbs, zbits));
    else
      MPN_COPY_INCR (bp, bp + zlimbs, n - zlimbs);

    cy = bdivmod_n (tp, &qsign, &rsign, ap, bp, n,
		    zlimbs, zbits, tp + zlimbs + 1);

    bgcd_matrix_mul_q (m, zlimbs, zbits, tp, qsign, tp + zlimbs + 1);

    if (rsign < 0)
      {
	m->sign[1][0] = ~m->sign[1][0];
	m->sign[1][1] = ~m->sign[1][1];
      }

    n -= zlimbs;
    if (cy > 0)
      {
	ap[n] = cy;
	bp[n] = 0;
	n++;
      }

    /* Swapping pointers would confuse our caller, so we have to swap
       the contents. */
    for (i = 0; i<n; i++)
      MP_LIMB_T_SWAP (ap[i], bp[i]);
  }
  ASSERT (m->j >= (k+1)/2);

  ASSERT (n <= 2 + (27 * l + 5) / 32);
  ASSERT (n < l);

  {
    /* Second and final recursive call */
    mp_size_t zlimbs;
    unsigned zbits;
    unsigned k1;
    mp_size_t n1;
    mp_size_t nn;

    zlimbs = power_of_2 (&zbits, bp, n);

    if (zlimbs * GMP_NUMB_BITS + zbits + m->j >= k)
      goto done;

    k1 = k - m->j;

    ASSERT (k1 <= (k+1) / 2);

    if (k1 <= GMP_NUMB_BITS)
      {
	struct bgcd_matrix1 m1;

	hbgcd2 (ap[1], ap[0], bp[1], bp[0], k1, &m1);
	ASSERT (m1.j > 0);

	n = bgcd_matrix1_apply (&m1, ap, bp, n, tp);
	bgcd_matrix_mul_1 (m, &m1, tp);

	/* May need another one */
	k1 = k - m->j;

	if (hbgcd2 (ap[1], ap[0], bp[1], bp[0], k1, &m1) > 0)
	  {
	    n = bgcd_matrix1_apply (&m1, ap, bp, n, tp);
	    bgcd_matrix_mul_1 (m, &m1, tp);
	  }
      }
    else
      {
	struct bgcd_matrix m1;

	/* Need at least 2 * k1 bits */
	n1 = (2 * k1 + GMP_NUMB_BITS - 1) / GMP_NUMB_BITS;
	ASSERT (n1 < n);
	ASSERT (n1 <= l/2);

	/* Each matrix element is of size at most

	   MPN_BGCD_MATRIX_ITCH (n1) <= (11*l/2 + 31) / 16
	     = 11 * l/32 + 31 / 16 < 3*l/8 + 31/16 - l/32 <= 3*l/8 + 27/16

	   Hence, 3*l/2 + 7 limbs should be enough for the entire matrix.
	*/

	mpn_bgcd_matrix_init (&m1, tp, MPN_BGCD_MATRIX_ITCH (n1));
	ASSERT (4 * MPN_BGCD_MATRIX_ITCH (n1) <= 3*l/2 + 7);
	tp += 3*l/2 + 7;
	nn = mpn_hbgcd_n (ap, bp, n1, k1, &m1, tp);

	ASSERT (m->j > 0);

	/* Needs space 4*n <= 4*(l - 1) */
	n = bgcd_matrix_apply (&m1, ap, bp, n, n1, nn, tp);

	/* Needs space 3 * (MPN_BGCD_MATRIX_ITCH (l) - 1) + 2 * (MPN_BGCD_MATRIX_ITCH(n1) - 1)
	   <= 3 * (11 * l + 15) / 16 + 2 * (11 * l/2 + 15) / 16
	   = 44 * l / 16 + (45 + 30)/16
	   < 11 * l / 4 + 5 < 3*l + 5

	   ==> need <= 3*l + 4 <= * (l - 1);

	   where l denotes the original input size. */

	bgcd_matrix_mul (m, &m1, tp);
      }
  }

 done:
  ASSERT (m->j < k);

  return n;
}

/* For a hbgcd step, we need

     4*MPN_BGCD_MATRIX_ITCH(n/2) + MAX(hbgcd_n_itch(n/2), 4*n)
     <= 3*n/2 + 7 + MAX (11*n/4 + 3, 4*n)
     == (3/2 + 4) * n + 7 = 11*n/2 + 7

   For a division step, we need

     zlimbs + 1 + (MAX (4 * zlimbs, n)) <= n + 4*(n-1) < 5*n

   For kinds of steps, 11*n/2 + 7 is sufficient.
*/

#define BGCD_N_ITCH(n) (11*(n)/2 + 7)

static mp_size_t
bgcd_n (mp_ptr gp, mp_ptr ap, mp_ptr bp, mp_size_t n, mp_ptr tp)
{
  ASSERT (ODD (ap[0]));
  
  if (ODD (bp[0]))
    {
      int cmp = mpn_cmp (ap, bp, n);
      if (cmp == 0)
	{
	  MPN_NORMALIZE (ap, n);
	  MPN_COPY (gp, ap, n);
	  return n;
	}
      else if (cmp > 0)
	MP_PTR_SWAP (ap, bp);

      ASSERT_NOCARRY (mpn_sub_n (bp, bp, ap, n));
    }

  while (ABOVE_THRESHOLD (n, BGCD_THRESHOLD))
    {
      struct bgcd_matrix m;
      mp_size_t zlimbs;
      unsigned zbits;

      mp_size_t n1;
      mp_size_t l;

      mp_size_t alloc;

      zlimbs = power_of_2 (&zbits, bp, n);
      if (zlimbs == n)
	{
	  MPN_NORMALIZE (ap, n);
	  MPN_COPY (gp, ap, n);
	  return n;
	}

      n1 = n/2;
      alloc = MPN_BGCD_MATRIX_ITCH (n1);
      ASSERT (4*alloc <= 3*n/2 + 7);
      mpn_bgcd_matrix_init (&m, tp + 4 * n, alloc);

      l = mpn_hbgcd_n (ap, bp, n1, n1 * GMP_NUMB_BITS / 2, &m, tp);
      if (m.j > 0)
	n = bgcd_matrix_apply (&m, ap, bp, n, n1, l, tp);
      else
	{
	  mp_limb_t cy;
	  int qsign;
	  int rsign;

	  if (zbits)
	    ASSERT_NOCARRY (mpn_rshift (bp, bp + zlimbs, n - zlimbs, zbits));
	  else
	    MPN_COPY_INCR (bp, bp + zlimbs, n - zlimbs);

	  cy = bdivmod_n (tp, &qsign, &rsign, ap, bp, n,
			  zlimbs, zbits, tp + zlimbs + 1);

	  n -= zlimbs;
	  if (cy > 0)
	    {
	      ap[n] = cy;
	      bp[n] = 0;
	      n++;
	    }

	  MP_PTR_SWAP (ap, bp);
	}
    }

  /* Use old algorithm */
  {
    mp_size_t an;
    mp_size_t bn;

    an = bn = n;
    MPN_NORMALIZE (ap, an);
    MPN_NORMALIZE (bp, bn);
    if (bn == 0)
      {
	MPN_COPY (gp, ap, an);
	return an;
      }
    /* The *smaller* number must be odd */
    if (an > bn || (an == bn && ap[an-1] > bp[an - 1]))
      {
	int bits;
	while (bp[0] == 0)
	  {
	    bp++;
	    bn--;
	  }
	count_trailing_zeros (bits, bp[0]);
	if (bits > 0)
	  {
	    ASSERT_NOCARRY (mpn_rshift (bp, bp, bn, bits));
	    bn -= (bp[bn-1] == 0);
	  }
	MPN_PTR_SWAP (ap, an, bp, bn);
      }

    return mpn_gcd (gp, bp, bn, ap, an); 
  }
}

/* Needs BDIVMOD_N_ITCH(an, an - bn) + an - bn + 1 for the initial division,
   and BGCD_N_ITCH (bn) for the bgcd_n call. I.e. the max of

     an - bn + 1 + MAX(4 * (an - bn), an) <= 5*an
     11 * bn/2 + 7
*/
#define BGCD_ITCH(an, bn) MAX(5*(an), BGCD_N_ITCH(bn))

static mp_size_t
bgcd (mp_ptr gp, mp_ptr ap, mp_size_t an, mp_ptr bp, mp_size_t bn, mp_ptr tp)
{
  /* mpn_gcd uses different conventions than the rest of the code. */
  ASSERT (ODD (bp[0]));
  ASSERT (an >= bn);

  if (an > bn)
    {
      mp_limb_t zlimbs;
      unsigned zbits;

      zlimbs = power_of_2 (&zbits, ap, an);
      ASSERT (zlimbs < an);

      if (zbits > 0)
	ASSERT_NOCARRY (mpn_rshift (ap, ap + zlimbs, an - zlimbs, zbits));
      else if (zlimbs > 0)
	MPN_COPY_INCR (ap, ap + zlimbs, an - zlimbs);

      an = an - zlimbs;
      an -= (ap[an-1] == 0);

      if (an < bn)
	MPN_ZERO (ap + an, an - bn);
	
      else if (an > bn)
	{
	  mp_size_t qn = an - bn + 1;
	  mp_limb_t cy;
	  int qsign;
	  int rsign;

	  cy = bdivmod_n (tp, &qsign, &rsign, ap, bp, an,
			  an - bn, 0, tp + qn);
	  an = bn;
	  ASSERT (!ODD (ap[0]));
	  
	  if (cy > 0)
	    {
	      ASSERT (cy == 1);
	      mpn_rshift1 (ap, ap, an);
	      ap[an-1] |= GMP_NUMB_HIGHBIT;
	    }
	}
    }
  /* Now both numbers are of size bn, and b is still odd */
  return bgcd_n (gp, bp, ap, bn, tp);
}

mp_size_t
mpn_bgcd (mp_ptr gp, mp_ptr ap, mp_size_t an, mp_ptr bp, mp_size_t bn)
{
  mp_ptr tp;
  mp_size_t gn;
  
  TMP_DECL;

  TMP_MARK;
  tp = TMP_ALLOC_LIMBS (BGCD_ITCH (an, bn));

  gn = bgcd (gp, ap, an, bp, bn, tp);

  TMP_FREE;
  return gn;
}
