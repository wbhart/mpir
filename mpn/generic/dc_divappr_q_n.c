/* dc_divappr_q - middle-product-based divide and conquer approximate quotient

Copyright (C) 2009, David Harvey
Copyright (C) 2009, William Hart

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/*
   N.B: this implementation of approximate quotient is derived from original
   ideas and code of David Harvey, however all bug reports should be directed 
   to the MPIR developers, who are responsible for its current incarnation.
*/

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"
//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//#include <math.h>

/*
  Computes an approximation to N/D, where N = {np,2n}, D = {dp,n}.
  D must be normalised (i.e. B/2 <= dp[n-1] < B).

  More precisely, returns Q such that N = Q*D + R, where -D < R < D.

  Q is n+1 limbs; low limbs written to {qp,n}, high limb returned. 
  The high limb is either 0 or 1.

  {dip,2} is precomputed inverse of high limbs of dp (see 
  mpn_sb_divappr_q).

  N is destroyed.

  None of the buffers may overlap.

  tp is scratch space. 

  We assume that n << B (due to use of the middle product, which
  has that description.
*/

mp_limb_t
mpn_dc_divappr_q_n (mp_ptr qp, mp_ptr np, mp_srcptr dp, mp_size_t n, 
		    mp_limb_t dip, mp_ptr tp)
{
  mp_limb_t qh, cy;
  mp_ptr q_hi;
  mp_size_t m;
  mp_limb_t ret = 0;

  ASSERT (n >= 6);

  /* if the top n limbs of np are >= dp, high limb of quotient is 1 */
  if (mpn_cmp(np + n, dp, n) >= 0)
  {
     ret = 1;
     mpn_sub_n(np + n, np + n, dp, n);
  }

  /* top n limbs of np are now < dp */

  m = (n + 1) / 2;
  q_hi = qp + n - m;

  /* 
     FIXME: we could probably avoid this copy if we could guarantee 
     that sb_div_appr_q/dc_divappr_q_n did not destroy the "bottom 
     half" of N */
  MPN_COPY (tp, np, 2*n);

  /* estimate high m+1 limbs of quotient, using a 2*m by m division
     the quotient may be computed 1 too large as it is approximate, 
     moreover, even computed precisely it may be two too large due
     to the truncation we've done to a 2*m by m division... */
  if (m < DC_DIVAPPR_Q_N_THRESHOLD)
    qh = mpn_sb_divappr_q (q_hi, tp + 2*n - 2*m, 2*m,
			   dp + n - m, m, dip);
  else
    qh = mpn_dc_divappr_q_n (q_hi, tp + 2*n - 2*m,
			     dp + n - m, m, dip, tp + 2*n);

  /* we therefore decrease the estimate by 3... */
  qh -= mpn_sub_1 (q_hi, q_hi, m, (mp_limb_t) 3);
  
  /* ensuring it doesn't become negative */
  if (qh & GMP_NUMB_HIGHBIT)
    {
      MPN_ZERO (q_hi, m);
      qh = 0;
    }
  
  /* note qh is now always zero as the quotient we have is definitely
     correct or up to two too small, and we already normalised np */
  ASSERT (qh == 0);
  
  /* we know that {np+n-m, n+m} = q_hi * D + e0, where 0 <= e0 < C*B^n, 
     where C is a small positive constant. Estimate q_hi * D using 
     middle product, developing one additional limb, i.e. develop
     n - m + 3 limbs. The bottom limb is meaningless and the next limb
     may be too small by up to some small multiple of n, but recall 
     n << B. */
  mpn_mulmid (tp, dp, n, q_hi + 1, m - 2);

  /* do some parts of the middle product "manually": */
  tp[n - m + 2] += mpn_addmul_1 (tp, dp + m - 2, n - m + 2, q_hi[0]);
  mpn_addmul_1 (tp + 1, dp, n - m + 2, q_hi[m-1]);
  
  /* subtract that estimate from N. We note the limb at np + n - 2 
     is then meaningless, and the next limb mght be too large by a 
     small amount, i.e. the bottom n limbs of np are now possibly
     too large by a quantity much less than dp */
  mpn_sub_n (np + n - 2, np + n - 2, tp, n - m + 3);

  /* recursively divide to obtain low half of quotient, developing
     one more limb than we would need if everything had been exact.
     As this extra limb is out by only a small amount, rounding the
     remaining limbs based on its value and discarding the extra limb
     results in a quotient which is at most 1 too large */
  if (n - m + 2 < DC_DIVAPPR_Q_N_THRESHOLD)
    cy = mpn_sb_divappr_q (tp, np + m - 3, 2*n - 2*m + 4,
			   dp + m - 2, n - m + 2, dip);
  else
    cy = mpn_dc_divappr_q_n (tp, np + m - 3, dp + m - 2, n - m + 2,
			     dip, tp + n - m + 2);

  /* FIXME: The only reason this copy happens is that we elected to 
     develop one extra quotient limb in the second recursive quotient. */
  MPN_COPY (qp, tp + 1, n - m);

  /* Construct final quotient from low and hi parts... */
  ret += mpn_add_1 (qp + n - m, qp + n - m, m, tp[n-m+1]);
  ret += mpn_add_1 (qp + n - m + 1, qp + n - m + 1, m - 1, cy);
  if (tp[0] >= GMP_NUMB_HIGHBIT)
    ret += mpn_add_1 (qp, qp, n, 1);   /* ...rounding quotient up */

  /* As the final quotient may be 1 too large, we may have ret == 2 
     (it is very unlikely, but can be relatively easily triggered
     at random when dp = 0x80000...0000), then Q must be 2000.... 
     and we should return instead 1ffff.... */
  if (ret == 2)
    {
      ret -= mpn_sub_1 (qp, qp, n, 1);
      ASSERT (ret == 1);
    }

  return ret;
}
