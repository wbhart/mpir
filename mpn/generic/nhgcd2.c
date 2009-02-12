/* nhgcd2

   hgcd2 with stop condition suitable for sgcd and ngcd
*/

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"

/* Copied from mpn/generic/gcdext.c, and modified slightly to return
   the remainder. */
/* Two-limb division optimized for small quotients.  */
static inline mp_limb_t
div2 (mp_ptr rp,
      mp_limb_t nh, mp_limb_t nl,
      mp_limb_t dh, mp_limb_t dl)
{
  mp_limb_t q = 0;

  if ((mp_limb_signed_t) nh < 0)
    {
      int cnt;
      for (cnt = 1; (mp_limb_signed_t) dh >= 0; cnt++)
	{
	  dh = (dh << 1) | (dl >> (GMP_LIMB_BITS - 1));
	  dl = dl << 1;
	}

      while (cnt)
	{
	  q <<= 1;
	  if (nh > dh || (nh == dh && nl >= dl))
	    {
	      sub_ddmmss (nh, nl, nh, nl, dh, dl);
	      q |= 1;
	    }
	  dl = (dh << (GMP_LIMB_BITS - 1)) | (dl >> 1);
	  dh = dh >> 1;
	  cnt--;
	}
    }
  else
    {
      int cnt;
      for (cnt = 0; nh > dh || (nh == dh && nl >= dl); cnt++)
	{
	  dh = (dh << 1) | (dl >> (GMP_LIMB_BITS - 1));
	  dl = dl << 1;
	}

      while (cnt)
	{
	  dl = (dh << (GMP_LIMB_BITS - 1)) | (dl >> 1);
	  dh = dh >> 1;
	  q <<= 1;
	  if (nh > dh || (nh == dh && nl >= dl))
	    {
	      sub_ddmmss (nh, nl, nh, nl, dh, dl);
	      q |= 1;
	    }
	  cnt--;
	}
    }

  rp[0] = nl;
  rp[1] = nh;

  return q;
}

/* Reduces a,b until |a-b| fits in one limb + 1 bit. Constructs
   matrix M. Returns 1 if we make progress, i.e. can perform at least
   one subtraction. Otherwise returns zero.. */
int
mpn_nhgcd2 (mp_limb_t ah, mp_limb_t al, mp_limb_t bh, mp_limb_t bl,
	    struct ngcd_matrix1 *M)
{
  mp_limb_t u00, u01, u10, u11;
  
  if (ah < 2 || bh < 2)
    return 0;

  if (ah > bh || (ah == bh && al > bl))
    {
      sub_ddmmss (ah, al, ah, al, bh, bl);
      if (ah < 2)
	return 0;

      u00 = u01 = u11 = 1;
      u10 = 0;
    }
  else
    {
      sub_ddmmss (bh, bl, bh, bl, ah, al);
      if (bh < 2)
	return 0;

      u00 = u10 = u11 = 1;
      u01 = 0;
    }

  if (ah < bh)
    goto subtract_a;

  for (;;)
    {
      ASSERT (ah >= bh);
      if (ah == bh)
	break;

      /* Subtract a -= q b, and multiply M from the right by (1 q ; 0
	 1), affecting the second column of M. */
      ASSERT (ah > bh);
      sub_ddmmss (ah, al, ah, al, bh, bl);

      if (ah < 2)
	break;

      if (ah <= bh)
	{
	  /* Use q = 1 */
	  u01 += u00;
	  u11 += u10;
	}
      else
	{
	  mp_limb_t r[2];
	  mp_limb_t q = div2 (r, ah, al, bh, bl);
	  al = r[0]; ah = r[1];
	  if (ah < 2)
	    {
	      /* A is too small, but q is correct. */
	      u01 += q * u00;
	      u11 += q * u10;
	      break;
	    }
	  q++;
	  u01 += q * u00;
	  u11 += q * u10;
	}
    subtract_a:
      ASSERT (bh >= ah);
      if (ah == bh)
	break;

      /* Subtract b -= q a, and multiply M from the right by (1 0 ; q
	 1), affecting the first column of M. */
      sub_ddmmss (bh, bl, bh, bl, ah, al);

      if (bh < 2)
	break;

      if (bh <= ah)
	{
	  /* Use q = 1 */
	  u00 += u01;
	  u10 += u11;
	}
      else
	{
	  mp_limb_t r[2];
	  mp_limb_t q = div2 (r, bh, bl, ah, al);
	  bl = r[0]; bh = r[1];
	  if (bh < 2)
	    {
	      /* B is too small, but q is correct. */
	      u00 += q * u01;
	      u10 += q * u11;
	      break;
	    }
	  q++;
	  u00 += q * u01;
	  u10 += q * u11;
	}
    }
  M->u[0][0] = u00; M->u[0][1] = u01;
  M->u[1][0] = u10; M->u[1][1] = u11;

  return 1;
}

/* Multiply (a;b) by M^{-1} = (r11, -r01; -r10, r00). Needs n limbs of
   temporary storage. */
mp_size_t
mpn_ngcd_matrix1_vector (struct ngcd_matrix1 *M, mp_size_t n, mp_ptr ap, mp_ptr bp, mp_ptr tp)
{
  mp_limb_t h0, h1;

  /* Compute (a;b) <-- (u11 a - u01 b, -u10 a + u00 b) as

     t  = a
     a *= u11
     a -= u01 * b
     b *= u00
     b -= u10 * t
  */

  MPN_COPY (tp, ap, n);

  h0 =    mpn_mul_1 (ap, ap, n, M->u[1][1]);
  h1 = mpn_submul_1 (ap, bp, n, M->u[0][1]);
  ASSERT (h0 == h1);

  h0 =    mpn_mul_1 (bp, bp, n, M->u[0][0]);
  h1 = mpn_submul_1 (bp, tp, n, M->u[1][0]);
  ASSERT (h0 == h1);

  n -= (ap[n-1] | bp[n-1]) == 0;
  return n;
}

