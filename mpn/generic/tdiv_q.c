/* mpn_tdiv_qr -- Divide the numerator (np,nn) by the denominator (dp,dn) and
   write the nn-dn+1 quotient limbs at qp and the dn remainder limbs at rp.  If
   qxn is non-zero, generate that many fraction limbs and append them after the
   other quotient limbs, and update the remainder accordningly.  The input
   operands are unaffected.

   Preconditions:
   1. The most significant limb of of the divisor must be non-zero.
   2. No argument overlap is permitted.  (??? relax this ???)
   3. nn >= dn, even if qxn is non-zero.  (??? relax this ???)

   The time complexity of this is O(qn*qn+M(dn,qn)), where M(m,n) is the time
   complexity of multiplication.

Copyright 1997, 2000, 2001, 2002, 2005 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA. */

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"


void
mpn_tdiv_q (mp_ptr qp, mp_srcptr np, mp_size_t nn, 
                                   mp_srcptr dp, mp_size_t dn)
{
  ASSERT (nn >= 0);
  ASSERT (dn >= 0);
  ASSERT (dn == 0 || dp[dn - 1] != 0);
  ASSERT (! MPN_OVERLAP_P (qp, nn - dn + 1, np, nn));
  ASSERT (! MPN_OVERLAP_P (qp, nn - dn + 1, dp, dn));

  switch (dn)
    {
    case 0:
      DIVIDE_BY_ZERO;

    case 1:
      {
	mpn_divmod_1 (qp, np, nn, dp[0]);
	return;
      }

    case 2:
      {
	mp_ptr n2p, d2p;
	mp_limb_t qhl, cy;
	TMP_DECL;
	TMP_MARK;
	if ((dp[1] & GMP_NUMB_HIGHBIT) == 0)
	  {
	    int cnt;
	    mp_limb_t dtmp[2];
	    count_leading_zeros (cnt, dp[1]);
	    d2p = dtmp;
	    d2p[1] = (dp[1] << cnt) | (dp[0] >> (GMP_NUMB_BITS - cnt));
	    d2p[0] = (dp[0] << cnt);
	    n2p = (mp_ptr) TMP_ALLOC_LIMBS (nn + 1);
	    cy = mpn_lshift (n2p, np, nn, cnt);
	    n2p[nn] = cy;
#if HAVE_NATIVE_mpn_divrem_euclidean_qr_2
	    qhl = mpn_divrem_euclidean_qr_2 (qp, n2p, nn + (cy != 0), d2p);
#else
	    qhl = mpn_divrem_2 (qp, 0L, n2p, nn + (cy != 0), d2p);
#endif
	    if (cy == 0)
	      qp[nn - 2] = qhl;	/* always store nn-2+1 quotient limbs */
	  }
	else
	  {
	    d2p = (mp_ptr) dp;
	    n2p = (mp_ptr) TMP_ALLOC_LIMBS (nn);
	    MPN_COPY (n2p, np, nn);
#if HAVE_NATIVE_mpn_divrem_euclidean_qr_2
	    qhl = mpn_divrem_euclidean_qr_2 (qp, n2p, nn, d2p);
#else
	    qhl = mpn_divrem_2 (qp, 0L, n2p, nn, d2p);
#endif
	    qp[nn - 2] = qhl;	/* always store nn-2+1 quotient limbs */
	  }
	TMP_FREE;
	return;
      }

    default:
      {
	int adjust;
	TMP_DECL;
	TMP_MARK;
	adjust = np[nn - 1] >= dp[dn - 1];	/* conservative tests for quotient size */
	if (nn + adjust >= 2 * dn)
	  {
	    mp_ptr n2p, d2p, q2p;
	    mp_limb_t cy;
	    int cnt;

	    qp[nn - dn] = 0;			  /* zero high quotient limb */
	    if ((dp[dn - 1] & GMP_NUMB_HIGHBIT) == 0) /* normalize divisor */
	      {
		count_leading_zeros (cnt, dp[dn - 1]);
		d2p = (mp_ptr) TMP_ALLOC_LIMBS (dn);
		mpn_lshift (d2p, dp, dn, cnt);
		n2p = (mp_ptr) TMP_ALLOC_LIMBS (nn + 1);
		cy = mpn_lshift (n2p, np, nn, cnt);
		n2p[nn] = cy;
		nn += adjust;
	      }
	    else
	      {
		cnt = 0;
		d2p = (mp_ptr) dp;
		n2p = (mp_ptr) TMP_ALLOC_LIMBS (nn + 1);
		MPN_COPY (n2p, np, nn);
		n2p[nn] = 0;
		nn += adjust;
	      }

	    if (dn < DIV_DC_THRESHOLD)
	      mpn_sb_divrem_mn (qp, n2p, nn, d2p, dn);
	    else
	      {
		/* Divide 2*dn / dn limbs as long as the limbs in np last.  */
		q2p = qp + nn - dn;
		n2p += nn - dn;
		do
		  {
		    q2p -= dn;  n2p -= dn;
		    mpn_dc_divrem_n (q2p, n2p, d2p, dn);
		    nn -= dn;
		  }
		while (nn >= 2 * dn);

		if (nn != dn)
		  {
		    mp_limb_t ql;
		    n2p -= nn - dn;

		    /* We have now dn < nn - dn < 2dn.  Make a recursive call,
		       since falling out to the code below isn't pretty.
		       Unfortunately, mpn_tdiv_qr returns nn-dn+1 quotient
		       limbs, which would overwrite one already generated
		       quotient limbs.  Preserve it with an ugly hack.  */
		    /* FIXME: This suggests that we should have an
		       mpn_tdiv_qr_internal that instead returns the most
		       significant quotient limb and move the meat of this
		       function there.  */
		    /* FIXME: Perhaps call mpn_sb_divrem_mn here for certain
		       operand ranges, to decrease overhead for small
		       operands?  */
		    ql = qp[nn - dn]; /* preserve quotient limb... */
		    mpn_tdiv_q (qp, n2p, nn, d2p, dn);
		    qp[nn - dn] = ql; /* ...restore it again */
		  }
	      }

       TMP_FREE;
	    return;
	  }

	/* When we come here, the numerator/partial remainder is less
	   than twice the size of the denominator.  */

	  {
	    /* Problem:

	       Divide a numerator N with nn limbs by a denominator D with dn
	       limbs forming a quotient of qn=nn-dn+1 limbs.  When qn is small
	       compared to dn, conventional division algorithms perform poorly.
	       We want an algorithm that has an expected running time that is
	       dependent only on qn.

	       Algorithm (very informally stated):

	       1) Divide the 2 x qn most significant limbs from the numerator
		  by the qn most significant limbs from the denominator.  Call
		  the result qest.  This is either the correct quotient, but
		  might be 1 or 2 too large.  Compute the remainder from the
		  division.  (This step is implemented by a mpn_divrem call.)

	       2) Is the most significant limb from the remainder < p, where p
		  is the product of the most significant limb from the quotient
		  and the next(d)?  (Next(d) denotes the next ignored limb from
		  the denominator.)  If it is, decrement qest, and adjust the
		  remainder accordingly.

	       3) Is the remainder >= qest?  If it is, qest is the desired
		  quotient.  The algorithm terminates.

	       4) Subtract qest x next(d) from the remainder.  If there is
		  borrow out, decrement qest, and adjust the remainder
		  accordingly.

	       5) Skip one word from the denominator (i.e., let next(d) denote
		  the next less significant limb.  */

	    mp_size_t qn;
	    mp_ptr n2p, n3p, d2p;
	    mp_ptr tp;
	    mp_limb_t cy;
	    mp_size_t in, rn;
	    mp_limb_t quotient_too_large;
	    unsigned int cnt;

	    qn = nn - dn;
	    qp[qn] = 0;				/* zero high quotient limb */
	    qn += adjust;			/* qn cannot become bigger */

	    if (qn == 0)
	      {
		TMP_FREE;
		return;
	      }

	    in = dn - qn;		/* (at least partially) ignored # of limbs in ops */

	    /* Normalize denominator by shifting it to the left such that its
	       most significant bit is set.  Then shift the numerator the same
	       amount, to mathematically preserve quotient.  */
	    if ((dp[dn - 1] & GMP_NUMB_HIGHBIT) == 0)
	      {
		count_leading_zeros (cnt, dp[dn - 1]);
		
		d2p = (mp_ptr) TMP_ALLOC_LIMBS (qn);
		mpn_lshift (d2p, dp + in, qn, cnt);
		d2p[0] |= dp[in - 1] >> (GMP_NUMB_BITS - cnt);

		n2p = (mp_ptr) TMP_ALLOC_LIMBS (2 * qn + 1);
		n3p = (mp_ptr) TMP_ALLOC_LIMBS (2 * qn + 1);
		cy = mpn_lshift (n2p, np + nn - 2 * qn, 2 * qn, cnt);
		if (adjust)
		  {
		    n2p[2 * qn] = cy;
		    n2p++;
		  }
		else
		  {
		    n2p[0] |= np[nn - 2 * qn - 1] >> (GMP_NUMB_BITS - cnt);
		  }
	      }
	    else
	      {
		cnt = 0;
		d2p = (mp_ptr) dp + in;

		n2p = (mp_ptr) TMP_ALLOC_LIMBS (2 * qn + 1);
		n3p = (mp_ptr) TMP_ALLOC_LIMBS (2 * qn + 1);
		MPN_COPY (n2p, np + nn - 2 * qn, 2 * qn);
		if (adjust)
		  {
		    n2p[2 * qn] = 0;
		    n2p++;
		  }
	      }

	    /* Get an approximate quotient using the extracted operands.  */
	    if (qn == 1)
	      {
		mp_limb_t q0, r0;
		mp_limb_t gcc272bug_n1, gcc272bug_n0, gcc272bug_d0;
		/* Due to a gcc 2.7.2.3 reload pass bug, we have to use some
		   temps here.  This doesn't hurt code quality on any machines
		   so we do it unconditionally.  */
		gcc272bug_n1 = n2p[1];
		gcc272bug_n0 = n2p[0];
		gcc272bug_d0 = d2p[0];
		udiv_qrnnd (q0, r0, gcc272bug_n1, gcc272bug_n0,
			    gcc272bug_d0);
		qp[0] = q0;
	      }
	    else if (qn == 2)
       {
         MPN_COPY(n3p, n2p, 2 * qn);
#if HAVE_NATIVE_mpn_divrem_euclidean_qr_2
	      mpn_divrem_euclidean_qr_2 (qp, n3p, 4L, d2p);
#else
	      mpn_divrem_2 (qp, 0L, n3p, 4L, d2p);	      
#endif
       }
       else if (qn < DIV_DC_THRESHOLD)
       {
	        mp_limb_t dip[2];
           MPN_COPY(n3p, n2p, 2 * qn);
           mpn_invert(dip, d2p + qn - 2, 2);
           //mpn_sb_divrem_mn(qp, n3p, 2*qn, d2p, qn);
           mpn_sb_divappr_q (qp, n3p, 2 * qn, d2p, qn, dip);
       }
	    else             
       {
           mp_limb_t dip[2];
           mp_ptr tp = TMP_ALLOC_LIMBS (10*qn);
           MPN_COPY(n3p, n2p, 2 * qn);
           mpn_invert(dip, d2p + qn - 2, 2);

           //mpn_dc_divrem_n(qp, n3p, d2p, qn);
           mpn_dc_divappr_q_n (qp, n3p, d2p, qn, dip, tp);
       }
       /* At this point the quotient may be correct or up to 3 too large.
          It could be one too large from the approximate quotient and
          up to a further two too large from the fact that we didn't have
          all the limbs of the numerator and denominator.
       */

	    rn = qn;

       /* Each limb of q*d has a contribution from low limbs of product
          limbs of q*d, a contribution from high limbs and a contribution
          from carries, which does not exceed the number of limbs in q.

          We compute the low limb contribution which qp[qn-1]*d2p[1] 
          contributes to.          
       */
       {
          mp_limb_t lp[3] = {0L, 0L, 0L};
          mp_limb_t p[2];
          mp_limb_t d1, d2, dl, cy;
          long i;
          mp_limb_t max_carries = qn + in - 2;
          if (max_carries > qn) max_carries = qn; 
          
          for (i = qn - 1; i >= 1L; i--)
          {
             lp[2] += qp[i] * d2p[qn - i];
          }

          /* Now we compute the high limb contribution. We discard carries as
             they don't interest us.
          */

          for (i = qn - 1; i >= 0L; i--)
          {
             umul_ppmm(p[1], p[0], qp[i], d2p[qn - i - 1]);
             add_ssaaaa(lp[2], lp[1], lp[2], lp[1], p[1], p[0]);
          }

          /* If the product is too large, the quotient must be too large */
          while ((mp_limb_signed_t) (lp[2] - n2p[qn]) > 0L)
          {
             mpn_decr_u (qp, (mp_limb_t) 1);
             sub_ddmmss(lp[2], lp[1], lp[2], lp[1], CNST_LIMB(0), d2p[qn - 1]);
          }
          
          /* If carries can't take this up to the value of the limb in the
             numerator, we must have the right quotient
          */

          if (max_carries < n2p[qn] - lp[2])
          {
             TMP_FREE;
             return;
          }

          if (qn == 1) goto skip;

          if (max_carries > in + qn - 3) max_carries--;

          /* That proved inconclusive, so we compute the next limb of the
             product and perform a similar comparison
          */

          if (in)
          {
             if (in < 2)
                dl = 0;
             else 
                dl = dp[in - 2];

             if (cnt) d1 = (dp[in - 1] << cnt) | (dl >> (GMP_NUMB_BITS - cnt));
             else d1 = dp[in - 1];
          } else d1 = 0;

          for (i = qn - 2; i >= 0L; i--)
          {
             umul_ppmm(p[1], p[0], qp[i], d2p[qn - i - 2]);
             lp[2] += mpn_add_n(lp, lp, p, 2);
          }

          umul_ppmm(p[1], p[0], qp[qn - 1], d1);
          lp[2] += mpn_add_n(lp, lp, p, 2);

          while ((mp_limb_signed_t) (lp[2] - n2p[qn]) > 0L)
          {
             mpn_decr_u (qp, (mp_limb_t) 1);
             sub_ddmmss(lp[2], lp[1], lp[2], lp[1], CNST_LIMB(0), d2p[qn - 1]);
          }

          if ((mp_limb_signed_t) (n2p[qn] - lp[2]) > 0L)
          {
             if (n2p[qn] > lp[2] + 1)
             {
                TMP_FREE;
                return;
             }

             if (n2p[qn - 1] + max_carries >= lp[1])
             {
                TMP_FREE;
                return;
             }
             goto skip;
          }
          
          /* Now the top limb is known to be correct, so check next limb 
             If the product is too large, the quotient must be too large 
          */
          
          if (lp[1] > n2p[qn - 1])
          {
             mpn_decr_u (qp, (mp_limb_t) 1);
             cy = mpn_sub_n(lp, lp, d2p + qn - 2, 2);

             if (cy)
             {
                TMP_FREE;
                return;
             }
          }

          /* If carries can't take this up to the value of the limb in the
             numerator, we must have the right quotient
          */

          if (max_carries < n2p[qn - 1] - lp[1])
          {
             TMP_FREE;
             return;
          }

skip:
          /* We have been unable to determine the quotient so multiply out
             and see whether it is correct or one too large.
             We should get here only rarely, i.e. optimise something else
          */
          {
          
          mp_size_t n2n = nn + 1;
          n2p = TMP_ALLOC_LIMBS(nn+1);
          n2p[nn] = 0L;
          n2p[nn - 1] = 0L;
          mpn_mul(n2p, dp, dn, qp, qn);
          MPN_NORMALIZE (n2p, n2n);
          if ((n2n > nn) || (mpn_cmp(n2p, np, nn) > 0))
          {
             mpn_decr_u (qp, (mp_limb_t) 1);
          }
          
          TMP_FREE;
          return;

          }
       }
     }
   }
  }
}

