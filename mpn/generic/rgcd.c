/* Schönhage algorithm, from GMP cvs */
#include <stdio.h>  /* for NULL */

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"

#define EVEN_P(x) (((x) & 1) == 0)

/* Allows an even v */
static mp_size_t
gcd_binary (mp_ptr gp, mp_ptr up, mp_size_t un, mp_ptr vp, mp_size_t vn)
{
  if ((vp[0] & 1) == 0)
    {
      /* Calling convention says there can be no common factors of two. */ 
      ASSERT ((up[0] & 1) == 1);

      while (vp[0] == 0)
	{
	  ASSERT (vn > 0);
	  vp++;
	  vn--;
	}
	  
      if ((vp[0] & 1) == 0)
	{
	  int shift;
	  count_trailing_zeros (shift, vp[0]);
	  ASSERT_NOCARRY (mpn_rshift (vp, vp, vn, shift));
	  vn -= (vp[vn-1] == 0);
	}
    }
  return mpn_gcd (gp, up, un, vp, vn);
}

#define MPN_LEQ_P(ap, asize, bp, bsize)				\
((asize) < (bsize) || ((asize) == (bsize)			\
		       && mpn_cmp ((ap), (bp), (asize)) <= 0))

/* Sets (a, b, c, d)  <--  (c, d, a, b) */
#define NHGCD_SWAP4_2(row)			\
do {						\
  struct hgcd_row __nhgcd_swap4_2_tmp;          \
  __nhgcd_swap4_2_tmp = row[0];                 \
  row[0] = row[2];				\
  row[2] = __nhgcd_swap4_2_tmp;			\
  __nhgcd_swap4_2_tmp = row[1];			\
  row[1] = row[3];				\
  row[3] = __nhgcd_swap4_2_tmp;			\
} while (0)

/* Sets (a, b, c)  <--  (b, c, a) */
#define NHGCD_SWAP3_LEFT(row)				\
do {							\
  struct hgcd_row __nhgcd_swap4_left_tmp;               \
  __nhgcd_swap4_left_tmp = row[0];                      \
  row[0] = row[1];					\
  row[1] = row[2];					\
  row[2] = __nhgcd_swap4_left_tmp;			\
} while (0)

static mp_size_t
hgcd_tdiv (mp_ptr qp,
	   mp_ptr rp, mp_size_t *rsizep,
	   mp_srcptr ap, mp_size_t asize,
	   mp_srcptr bp, mp_size_t bsize)
{
  mp_size_t qsize;
  mp_size_t rsize;

  mpn_tdiv_qr (qp, rp, 0, ap, asize, bp, bsize);

  rsize = bsize;
  MPN_NORMALIZE (rp, rsize);
  *rsizep = rsize;

  qsize = asize - bsize + 1;
  qsize -= (qp[qsize - 1] == 0);

  if (qsize == 1 && qp[0] == 1)
    return 0;

  return qsize;
}

static mp_size_t
gcd_schoenhage_itch (mp_size_t asize)
{
  /* Size for hgcd calls */
  mp_size_t ralloc = asize + 1;
  mp_size_t hgcd_size = (asize + 1) / 2;
  return (4 * ralloc				/* Remainder storage */
	  + mpn_hgcd_init_itch (hgcd_size)	/* hgcd storage */
	  + mpn_hgcd_quotients_init_itch (hgcd_size)
	  + mpn_hgcd_itch (hgcd_size)		/* nhgcd call */
	  + 1+ 3 * asize / 4);			/* hgcd_fix */
}

static mp_size_t
gcd_schoenhage (mp_ptr gp, mp_srcptr ap, mp_size_t asize,
		mp_srcptr bp, mp_size_t bsize,
		mp_ptr tp, mp_size_t talloc)
{
  mp_size_t scratch;
  struct hgcd hgcd;
  struct hgcd_row r[4];

  mp_size_t ralloc = asize + 1;

  ASSERT (asize >= bsize);
  ASSERT (bsize > 0);

  ASSERT (MPN_LEQ_P (bp, bsize, ap, asize));

  ASSERT (4 * ralloc <= talloc);
  tp += ralloc; talloc -= ralloc;
  r[0].rp = tp; tp += ralloc; talloc -= ralloc;
  r[1].rp = tp; tp += ralloc; talloc -= ralloc;
  r[2].rp = tp; tp += ralloc; talloc -= ralloc;
  r[3].rp = tp; tp += ralloc; talloc -= ralloc;

  MPN_COPY (r[0].rp, ap, asize); r[0].rsize = asize;
  MPN_COPY (r[1].rp, bp, bsize); r[1].rsize = bsize;

  scratch = mpn_hgcd_init_itch ((asize + 1)/2);
  ASSERT (scratch <= talloc);
  mpn_hgcd_init (&hgcd, (asize + 1)/2, tp);
  tp += scratch; talloc -= scratch;

  {
    mp_size_t nlimbs = mpn_hgcd_quotients_init_itch ((asize + 1)/2);

    ASSERT (nlimbs <= talloc);

    mpn_hgcd_quotients_init (&hgcd, (asize + 1) / 2, tp);

    tp += nlimbs;
    talloc -= nlimbs;
  }

  while (ABOVE_THRESHOLD (r[0].rsize, GCD_SCHOENHAGE_THRESHOLD)
         && r[1].rsize > 0)
    {
      mp_size_t k = r[0].rsize / 2;
      int res;

      if (r[1].rsize <= k)
	goto euclid;

      res = mpn_hgcd (&hgcd,
		      r[0].rp + k, r[0].rsize - k,
		      r[1].rp + k, r[1].rsize - k,
		      tp, talloc);

      if (res == 0 || res == 1)
	{
	euclid:
	  ASSERT (r[0].rsize - r[1].rsize + 1 <= talloc);
	  hgcd_tdiv (tp, r[2].rp, &r[2].rsize,
		     r[0].rp, r[0].rsize,
		     r[1].rp, r[1].rsize);

	  NHGCD_SWAP3_LEFT (r);
	}
      else
	{
	  const struct hgcd_row *s = hgcd.row + (res - 2);
	  int sign = hgcd.sign;
	  if (res == 3)
	    sign = ~sign;

	  /* s[0] and s[1] are correct */
	  r[2].rsize
	    = mpn_hgcd_fix (k, r[2].rp, ralloc,
			    sign, hgcd.size, s,
			    r[0].rp, r[1].rp,
			    tp, talloc);

	  r[3].rsize
	    = mpn_hgcd_fix (k, r[3].rp, ralloc,
			    ~sign, hgcd.size, s+1,
			    r[0].rp, r[1].rp,
			    tp, talloc);

	  NHGCD_SWAP4_2 (r);
	}
    }

  if (r[1].rsize == 0)
    {
      MPN_COPY (gp, r[0].rp, r[0].rsize);
      return r[0].rsize;
    }
  else
    return gcd_binary (gp,
		       r[0].rp, r[0].rsize,
		       r[1].rp, r[1].rsize);
}

/* Should we perform an initial division? */
mp_size_t
mpn_rgcd (mp_ptr gp, mp_ptr up, mp_size_t usize, mp_ptr vp, mp_size_t vsize)
{
  if (BELOW_THRESHOLD (usize, GCD_SCHOENHAGE_THRESHOLD))
    return mpn_gcd (gp, up, usize, vp, vsize);

  /* The algorithms below require U >= V, while mpn_gcd is long documented as
     requiring only that the position of U's msb >= V's msb.  */
  if (usize == vsize && mpn_cmp (up, vp, usize) < 0)
    MP_PTR_SWAP (up, vp);

    {
      mp_size_t scratch;
      mp_ptr tp;
      mp_size_t gsize;

      scratch = gcd_schoenhage_itch (usize);
      tp = __GMP_ALLOCATE_FUNC_LIMBS (scratch);

      gsize = gcd_schoenhage (gp, up, usize, vp, vsize, tp, scratch);
      __GMP_FREE_FUNC_LIMBS (tp, scratch);
      return gsize;
    }
}
