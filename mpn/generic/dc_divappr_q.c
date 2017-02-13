/* mpn_dc_divappr_q -- divide-and-conquer division, returning approximate
   quotient.  The quotient returned is either correct, or one too large.

   Contributed to the GNU project by Torbjorn Granlund.

   THE FUNCTIONS IN THIS FILE ARE INTERNAL WITH MUTABLE INTERFACES.  IT IS ONLY
   SAFE TO REACH THEM THROUGH DOCUMENTED INTERFACES.  IN FACT, IT IS ALMOST
   GUARANTEED THAT THEY WILL CHANGE OR DISAPPEAR IN A FUTURE GMP RELEASE.

Copyright 2006, 2007, 2009, 2010 Free Software Foundation, Inc.

Copyright 2010, 2013 William Hart

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

#define SB_DIVAPPR_Q_CUTOFF 43 /* must be at least 3 */

mp_limb_t
mpn_dc_divappr_q (mp_ptr qp, mp_ptr np, mp_size_t nn,
		     mp_srcptr dp, mp_size_t dn, mp_limb_t dinv)
{
  mp_size_t q_orig, qn, sh, sl, i;
  mp_limb_t qh, cy, cy2;
  mp_ptr tp;
  TMP_DECL;

  ASSERT (dn >= 6);
  ASSERT (nn >= dn + 3);
  ASSERT (dp[dn-1] & GMP_NUMB_HIGHBIT);

  qn = nn - dn;
  if (qn + 1 < dn)
    {
      dp += dn - (qn + 1);
      dn = qn + 1;
    }
  q_orig = qn;

  qh = mpn_cmp(np + nn - dn, dp, dn) >= 0;
  if (qh != 0)
    mpn_sub_n(np + nn - dn, np + nn - dn, dp, dn);

  np += nn - dn - qn;
  nn = dn + qn;

  /* Reduce until dn - 1 >= qn */
  while (dn - 1 < qn)
  {
     sh = MIN(dn, qn - dn + 1);
     if (sh <= DC_DIV_QR_THRESHOLD) cy2 = mpn_sb_div_qr(qp + qn - sh, np + nn - dn - sh, dn + sh, dp, dn, dinv);
     else cy2 = mpn_dc_div_qr(qp + qn - sh, np + nn - dn - sh, dn + sh, dp, dn, dinv);
     qn -= sh; nn -= sh; 
  }

  cy = np[nn - 1];

  /* split into two parts */
  sh = qn/2; sl = qn - sh;

  /* Rare case where truncation ruins normalisation */
  if (cy > dp[dn - 1] || (cy == dp[dn - 1] 
     && mpn_cmp(np + nn - qn, dp + dn - qn, qn - 1) >= 0))
     {
        __divappr_helper(qp, np + nn - qn - 2, dp + dn - qn - 1, qn);
        return qh;
     }

  if (mpn_cmp(np + sl + dn - 1, dp + dn - sh - 1, sh + 1) >= 0)
     __divappr_helper(qp + sl, np + dn + sl - 2, dp + dn - sh - 1, sh);
  else
  {
     if (sh < SB_DIVAPPR_Q_CUTOFF)
        mpn_sb_divappr_q(qp + sl, np + sl, dn + sh, dp, dn, dinv);
     else
        mpn_dc_divappr_q(qp + sl, np + sl, dn + sh, dp, dn, dinv);
  }

  cy = np[nn - sh];

  TMP_MARK;
  tp = TMP_ALLOC_LIMBS(sl + 2);

  mpn_mulmid(tp, dp + dn - qn - 1, qn - 1, qp + sl, sh);
  cy -= mpn_sub_n(np + nn - qn - 2, np + nn - qn - 2, tp, sl + 2);

  TMP_FREE;

  if ((mp_limb_signed_t) cy < 0)
  {
      
     qh -= mpn_sub_1(qp + sl, qp + sl, q_orig - sl, 1); /* ensure quotient is not too big */
     
     /*
        correct remainder, noting that "digits" of quotient aren't base B
        but in base varying with truncation, thus correction needs fixup
     */
     cy += mpn_add_n(np + nn - qn - 2, np + nn - qn - 2, dp + dn - sl - 2, sl + 2); 

     for (i = 0; i < sh - 1 && qp[sl + i] == ~CNST_LIMB(0); i++)
        cy += mpn_add_1(np + nn - qn - 2, np + nn - qn - 2, sl + 2, dp[dn - sl - 3 - i]);
  }
   
  if (cy != 0) /* special case: unable to canonicalise */
     __divappr_helper(qp, np + nn - qn - 2, dp + dn - sl - 1, sl);
  else
  {
     if (mpn_cmp(np + dn - 1, dp + dn - sl - 1, sl + 1) >= 0)
        __divappr_helper(qp, np + dn - 2, dp + dn - sl - 1, sl);
     else
     {
        if (sl < SB_DIVAPPR_Q_CUTOFF)
           mpn_sb_divappr_q(qp, np, dn + sl, dp, dn, dinv);
        else
           mpn_dc_divappr_q(qp, np, dn + sl, dp, dn, dinv);
     }

  }

  return qh;
}
