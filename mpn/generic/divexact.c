/* mpn_divexact(qp,np,nn,dp,dn,tp) -- Divide N = {np,nn} by D = {dp,dn} storing
   the result in Q = {qp,nn-dn+1} expecting no remainder.  Overlap allowed
   between Q and N; all other overlap disallowed.

   Contributed to the GNU project by Torbjorn Granlund.

   THE FUNCTIONS IN THIS FILE ARE INTERNAL WITH MUTABLE INTERFACES.  IT IS ONLY
   SAFE TO REACH THEM THROUGH DOCUMENTED INTERFACES.  IN FACT, IT IS ALMOST
   GUARANTEED THAT THEY WILL CHANGE OR DISAPPEAR IN A FUTURE GMP RELEASE.

Copyright 2006, 2007, 2009 Free Software Foundation, Inc.
Copyright 2010, William Hart (modofied for MPIR)

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library.  If not, see http://www.gnu.org/licenses/.  */


#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"

/* FIXME: the INV_DIV_Q_THRESHOLD should be replaced with a tuned cutoff specifically for this function */

void
mpn_divexact (mp_ptr qp,
	      mp_srcptr np, mp_size_t nn,
	      mp_srcptr dp, mp_size_t dn)
{
  unsigned shift, shift2;
  int q_even;
  mp_size_t qn;
  mp_ptr tp, n2p, inv;
  mp_limb_t dinv, wp[2], cy;
  int extend = 0;

  TMP_DECL;

  ASSERT (dn > 0);
  ASSERT (nn >= dn);
  ASSERT (dp[dn-1] > 0);

  while (dp[0] == 0)
    {
      ASSERT (np[0] == 0);
      dp++;
      np++;
      dn--;
      nn--;
    }

  if (dn == 1)
    {
      MPN_DIVREM_OR_DIVEXACT_1 (qp, np, nn, dp[0]);
      return;
    }

  TMP_MARK;

  qn = nn + 1 - dn;
  count_trailing_zeros (shift, dp[0]);

  if ((BELOW_THRESHOLD (qn, INV_DIV_QR_THRESHOLD) && BELOW_THRESHOLD(dn, INV_DIV_QR_THRESHOLD)) || (dn <= 6))
  {
    if (shift > 0)
      {
        mp_size_t ss = (dn > qn) ? qn + 1 : dn;

        tp = TMP_ALLOC_LIMBS (ss);
        mpn_rshift (tp, dp, ss, shift);
        dp = tp;

        /* Since we have excluded dn == 1, we have nn > qn, and we need
	   to shift one limb beyond qn. */
        n2p = TMP_ALLOC_LIMBS (qn + 1);
        mpn_rshift (n2p, np, qn + 1, shift);
      }
    else
      {
        n2p = TMP_ALLOC_LIMBS (qn);
        MPN_COPY (n2p, np, qn);
      }

    if (dn > qn)
      dn = qn;

    modlimb_invert(dinv, dp[0]);

    if (BELOW_THRESHOLD (dn, DC_BDIV_Q_THRESHOLD))
      mpn_sb_bdiv_q (qp, wp, n2p, qn, dp, dn, dinv);
    else 
      mpn_dc_bdiv_q (qp, n2p, qn, dp, dn, dinv);
  } else
  {
    /* determine if the quotient is even */
    count_trailing_zeros (shift2, np[0]);
    q_even = ((np[0] == 0) || (shift2 > shift)) ? 1 : 0; 

    if (dp[dn - 1] & GMP_LIMB_HIGHBIT)
    {
       n2p = TMP_ALLOC_LIMBS(nn);
       MPN_COPY(n2p, np, nn);
    
       dp;
    } else
    { 
       count_leading_zeros (shift, dp[dn - 1]);
       n2p = TMP_ALLOC_LIMBS(nn + 1);
       n2p[nn] = mpn_lshift(n2p, np, nn, shift);
       extend = (n2p[nn] != 0);
       nn += extend;

       tp = TMP_ALLOC_LIMBS(dn);
       mpn_lshift(tp, dp, dn, shift);   
       dp = tp;
    }

    qn = nn - dn;
 if(qn)
   {       
    inv = TMP_ALLOC_LIMBS(dn);
    mpn_invert(inv, dp, dn);
    cy = mpn_inv_divappr_q(qp, n2p, nn, dp, dn, inv);
    if (!extend) qp[qn] = cy;

    if ((qp[0] & 1) + q_even != 1) /* quotient is out by 1 */
       mpn_sub_1(qp, qp, qn + 1, 1);
   } else
     qp[0] = 1; // as dp is normalised, exact division means qp = 1
  }

  TMP_FREE;
}

