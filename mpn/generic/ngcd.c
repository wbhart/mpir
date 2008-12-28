/* Schönhage's 1987 algorithm, reorganized into hgcd form */

#include <stdio.h>  /* for NULL */

#include "gmp.h"
#include "gmp-impl.h"
#include "longlong.h"






/* For input of size n, matrix elements are of size at most ceil(n/2)
   - 1, but we need one limb extra. */

void
mpn_ngcd_matrix_init (struct ngcd_matrix *M, mp_size_t n, mp_ptr p);

#define NHGCD_BASE_ITCH MPN_NGCD_STEP_ITCH

/* Reduces a,b until |a-b| fits in n/2 + 1 limbs. Constructs matrix M
   with elements of size at most (n+1)/2 - 1. Returns new size of a,
   b, or zero if no reduction is possible. */
static mp_size_t
nhgcd_base (mp_ptr ap, mp_ptr bp, mp_size_t n,
	    struct ngcd_matrix *M, mp_ptr tp);

/* Size analysis for nhgcd:

   For the recursive calls, we have n1 <= ceil(n / 2). Then the
   storage need is determined by the storage for the recursive call
   computing M1, and ngcd_matrix_adjust and ngcd_matrix_mul calls that use M1
   (after this, the storage needed for M1 can be recycled).

   Let S(r) denote the required storage. For M1 we need 5 * ceil(n1/2)
   = 5 * ceil(n/4), and for the ngcd_matrix_adjust call, we need n + 2. In
   total, 5 * ceil(n/4) + n + 2 <= 9 ceil(n/4) + 2.

   For the recursive call, we need S(n1) = S(ceil(n/2)).

   S(n) <= 9*ceil(n/4) + 2 + S(ceil(n/2))
        <= 9*(ceil(n/4) + ... + ceil(n/2^(1+k))) + 2k + S(ceil(n/2^k))
        <= 9*(2 ceil(n/4) + k) + 2k + S(n/2^k)   
	<= 18 ceil(n/4) + 11k + S(n/2^k)
	
*/

mp_size_t
mpn_nhgcd_itch (mp_size_t n);


/* Reduces a,b until |a-b| fits in n/2 + 1 limbs. Constructs matrix M
   with elements of size at most (n+1)/2 - 1. Returns new size of a,
   b, or zero if no reduction is possible. */

mp_size_t
mpn_nhgcd (mp_ptr ap, mp_ptr bp, mp_size_t n,
	   struct ngcd_matrix *M, mp_ptr tp);


#define EVEN_P(x) (((x) & 1) == 0)

mp_size_t
mpn_ngcd (mp_ptr gp, mp_ptr ap, mp_size_t an, mp_ptr bp, mp_size_t n)
{
  mp_size_t init_scratch;
  mp_size_t scratch;
  mp_ptr tp;
  TMP_DECL;
  
  ASSERT (an >= n);

  if (BELOW_THRESHOLD (n, NGCD_THRESHOLD))
    return mpn_basic_gcd (gp, ap, an, bp, n);

  init_scratch = MPN_NGCD_MATRIX_INIT_ITCH ((n+1)/2);
  scratch = mpn_nhgcd_itch ((n+1)/2);

  /* Space needed for mpn_ngcd_matrix_adjust */
  if (scratch < 2*n)
    scratch = 2*n;

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

  while (ABOVE_THRESHOLD (n, NGCD_THRESHOLD))
    {
      struct ngcd_matrix M;
      mp_size_t p = n/2;
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
#if 0
  /* FIXME: We may want to use lehmer on some systems. */
  n = mpn_ngcd_lehmer (gp, ap, bp, n, tp);

  TMP_FREE;
  return n;
#endif

  if (ap[n-1] < bp[n-1])
    MP_PTR_SWAP (ap, bp);

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
  return mpn_basic_gcd (gp, ap, an, bp, n);
}
