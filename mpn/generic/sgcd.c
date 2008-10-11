/* Schönhage's 1987 algorithm */

#include <stdio.h>  /* for NULL */

#include "gmp.h"
#include "gmp-impl.h"
#include "longlong.h"

#define SGCD_MATRIX_INIT_ITCH(n) (5 * ((n)+1))

static void
sgcd_matrix_init (struct ngcd_matrix *M, mp_size_t n, mp_ptr p)
{
  /* Need one extra limb */
  n++;
  M->alloc = n;
  M->n = 1;
  MPN_ZERO (p, 4 * n);
  M->p[0][0] = p;
  M->p[0][1] = p + n;
  M->p[1][0] = p + 2 * n;
  M->p[1][1] = p + 3 * n;
  M->tp = p + 4*n;

  M->p[0][0][0] = M->p[1][1][0] = 1;
}


#define SGCD_BASE_ITCH MPN_NGCD_STEP_ITCH

/* Reduces a,b until |a-b| fits in s limbs. Constructs matrix M with
   elements of size at most n-s. Returns new size on success, or 0 if
   a or b are too small. M = NULL is allowed is M is not needed. */
static mp_size_t
sgcd_base (mp_size_t n, mp_ptr ap, mp_ptr bp, mp_size_t s,
	   struct ngcd_matrix *M, mp_ptr tp)
{
  int success = 0;

  ASSERT (s >= 1);
  /* Should these be assertions? */
  ASSERT (ap[n-1] > 0 || bp[n-1] > 0);

  for (;;)
    {
      mp_size_t nn;
      ASSERT (n > s);
      nn = mpn_ngcd_step (n, ap, bp, s, M, tp);
      if (nn > 0)
	{
	  success = 1;
	  n = nn;
	}
      else break;
    }
  return success ? n : 0;
}

/* Size analysis for sgcd:

   For all recursive calls, we have n <= 4 r. Then the storage need is
   determined by the storage for the recursive call computing M1, and
   the multiplication M * M1 (after this, the storage needed for M1
   can be recycled).

   Let S(r) denote the storage needed for this. For the storage M1 we
   need 5*(r/2 + 1). For the recursive call, we need S(r/2). Then if
   $k$ is the recursive depth, we have

     S(r) <= 5 (r/2 + r/4 + n/(2^k)) + 5k < 5(r+k)

   If we allocate 5(r+k), then after allocating M1, the remaining storage is

     5*(r+k) - 5*(r/2 + 1) = 5 * ceil(r/2) + 5*(k-1) >= 5*r / 2

   This is sufficient for the call to sgcd_matrix_mul, which needs storage 2*r.

   Then the other operations need storage depending on n, not only r:

   * sgcd_base and sgcd_step: n + 1

   * sgcd_matrix_adjust: 2 * n

   So the storage need is MAX (5*(r+k), 2*n)
*/

static mp_size_t
sgcd_itch (mp_size_t n, mp_size_t s)
{
  mp_size_t n1, n2, r, rr;
  int k;

  ASSERT (s < n);

  /* Inefficient way to compute log2, but it doesn'shouldn't matter */
  for (k = 0, rr = r = n - s;
       ABOVE_THRESHOLD (rr, SGCD_BASE_THRESHOLD);
       rr /= 2)
    k++;

  if (k == 0)
    return SGCD_BASE_ITCH (n);

  n1 = 5*(r + k);
  n2 = 2*n;

  return MAX (n1, n2);
}

/* Reduces a,b until |a-b| fits in s limbs. Constructs matrix M with
   elements of size at most n-s. Returns new size on success, or 0 if
   the inputs a or b or |a-b| already fit in s limbs. FIXME: Is the
   failure return value really useful? */
static mp_size_t
sgcd (mp_size_t n, mp_ptr ap, mp_ptr bp, mp_size_t s,
      struct ngcd_matrix *M, int need_m, mp_ptr tp)
{
  struct ngcd_matrix M1;
  mp_ptr trp;
  mp_size_t r, p, s1, h, nn;

  int success;
  
  ASSERT (n > s);
  ASSERT ((ap[n-1] | bp[n-1]) > 0);

  /* Number of limbs to reduce */
  r = n - s;
  ASSERT (r < M->alloc);

  /* Needs storage n + 1 (<= SGCD_BASE_THRESHOLD + 1) */
  if (BELOW_THRESHOLD (r, SGCD_BASE_THRESHOLD))
    return sgcd_base (n, ap, bp, s, need_m ? M : NULL, tp);
  success = 0;

  /* Is the smallest number of size smaller than s+2? */
  if (n <= s + 2)
    goto final;

  if (ap[n-1] == 0)
    {
      mp_size_t k = n - s;
      MPN_NORMALIZE (ap + s, k);
      if (k == 0)
	return 0;

      if (k <= 2)
	goto final;
    }
  else if (bp[n-1] == 0)
    {
      mp_size_t k = n - s;
      MPN_NORMALIZE (ap + s, k);

      if (k == 0)
	return 0;
      if (k <= 2)
	goto final;
    }

  if ( (ap[n-1] == 0 && ap[n-2] == 0)
       || (bp[n-1] == 0 && bp[n-2] == 0) )
    goto final;

  if (s <= r)
    {
      p = 0;
      s1 = s;
    }
  else
    {
      s1 = r;
      p = s - r + 1;
      need_m = 1;
    }

  h = s1 + r/2;

  /* Bound for the first recursive call */
  ASSERT (s1 <= r);
  ASSERT (h <= r + r/2);
  ASSERT (n - p <= 2*r);

  /* Shouldn't need any more storage than ourselves */
  nn = sgcd (n - p, ap + p, bp + p, h, M, need_m, tp);
  if (nn)
    {
      success = 1;
      n = nn + p;
    }
  
  for (;;)
    {
      /* Needs (n-p + 1) <= n + 1 storage */
      nn = mpn_ngcd_step (n - p, ap + p, bp + p, s1, need_m ? M : NULL, tp);
      if (nn == 0)
	goto adjust;

      success = 1;
      n = nn + p;
      if (nn < h)
	break;
    }

  /* Size bounds for the second recursive call */
  ASSERT (n - p <= h);

  /* Needs 5 * (r/2 + 1) words of storage. */
  sgcd_matrix_init (&M1, h - s1, tp);
  /* The remaining storage, until we can recycle the storage needed for M1. */
  trp = tp + SGCD_MATRIX_INIT_ITCH (h - s1);

  /* Needs storage corresponding to r/2 */
  nn = sgcd (n - p, ap + p, bp + p, s1, &M1, need_m, trp);
  if (nn)
    {
      success = 1;
      n = nn + p;
      /* Needs 2*M->n <= 2*r words of storage */
      if (need_m)
	mpn_ngcd_matrix_mul (M, &M1, trp);
    }
  
 adjust:
  if (p > 0)
    {
      ASSERT (need_m);
      /* Needs 2*(p + M->n) <= 2*(p + r) = 2*(s+1). */
      n = mpn_ngcd_matrix_adjust (M, n, ap, bp, p, tp);
    }
 final:
  for (;;)
    {
      /* Needs n + 1 limbs of storage */
      nn = mpn_ngcd_step (n, ap, bp, s, need_m ? M : NULL, tp);
      if (nn > 0)
	{
	  success = 1;
	  n = nn;
	}
      else break;
    }
  return success ? n : 0;
}

#define EVEN_P(x) (((x) & 1) == 0)

mp_size_t
mpn_sgcd (mp_ptr gp, mp_ptr ap, mp_size_t an, mp_ptr bp, mp_size_t bn)
{
  mp_size_t s;
  mp_size_t sgcd_matrix_scratch;
  mp_size_t sgcd_scratch;
  mp_size_t n;
  mp_ptr tp;  
  struct ngcd_matrix M;
  int c;

  TMP_DECL;

  if (BELOW_THRESHOLD (bn, SGCD_THRESHOLD))
    return mpn_gcd (gp, ap, an, bp, bn);

  TMP_MARK;

  if (an > bn)
    {
      mp_ptr rp = TMP_ALLOC_LIMBS (an + 1);
      mp_ptr qp = rp + bn;

      mpn_tdiv_qr (qp, rp, 0, ap, an, bp, bn);
      MPN_COPY (ap, rp, bn);
      an = bn;
      MPN_NORMALIZE (ap, an);
      if (an == 0)
	{	  
	  MPN_COPY (gp, bp, bn);
	  TMP_FREE;
	  return bn;
	}
    }  
  
  s = SGCD_THRESHOLD / 2;
  sgcd_matrix_scratch = SGCD_MATRIX_INIT_ITCH (bn - s);
  sgcd_scratch = sgcd_itch (bn, s);

  /* FIXME: Allocates space for M. Even if the full M isn't needed,
     some space is needed later in the recursion. But we allocate more
     than needed here. */
  tp = TMP_ALLOC_LIMBS (sgcd_matrix_scratch + sgcd_scratch);
  sgcd_matrix_init (&M, bn - s, tp);

  n = sgcd (bn, ap, bp, s, &M, 0, tp + sgcd_matrix_scratch);

  TMP_FREE;
  
  if (n == 0)
    /* sgcd failed */
    n = bn;
  
  MPN_CMP (c, ap, bp, n);
  if (c < 0)
    MP_PTR_SWAP (ap, bp);
  else if (c == 0)
    {
      MPN_COPY (gp, ap, n);
      return n;
    }

  an = n;
  bn = n;
  MPN_NORMALIZE (bp, bn);
  ASSERT (bn > 0);
  
  if (EVEN_P (bp[0]))
    {
      /* Then a must be odd (since the calling convention implies that
	 there's no common factor of 2) */
      ASSERT (!EVEN_P (ap[0]));
      while (bp[0] == 0)
	{
	  bp++;
	  bn--;
	}

      if (EVEN_P(bp[0]))
	{
	  int count;
	  count_trailing_zeros (count, bp[0]);
	  ASSERT_NOCARRY (mpn_rshift (bp, bp, n, count));
	  bn -= (bp[bn-1] == 0);
	}
    }

  return mpn_gcd (gp, ap, an, bp, bn);
}
