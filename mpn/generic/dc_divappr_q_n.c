/* dc_divappr_q - middle-product-based divide and conquer approximate quotient

Copyright (C) 2009, David Harvey

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

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/*
  Computes an approximation to N/D, where N = {np,2n}, D = {dp,n}.
  D must be normalised (i.e. B/2 <= dp[n-1] < B).

  More precisely, returns Q such that N = Q*D + R, where -D < R < D.

  Q is n+1 limbs; low limbs written to {qp,n}, high limb returned. The high
  limb is either 0 or 1.

  {dip,2} is precomputed inverse of high limbs of dp (see mpn_sb_divappr_q).

  N is destroyed.

  None of the buffers may overlap.

  tp is scratch space. 
*/

#define DC_DIVAPPR_Q_N_THRESHOLD 36

mp_limb_t
mpn_dc_divappr_q_n (mp_ptr qp, mp_ptr np, mp_srcptr dp, mp_size_t n, 
		    mp_srcptr dip, mp_ptr tp)
{
  mp_limb_t qh, cy;
  mp_ptr q_hi;
  mp_size_t m;

  ASSERT (n >= 6);

  m = (n + 1) / 2;
  q_hi = qp + n - m;

  /* 
     FIXME: we could probably avoid this copy if we could guarantee that
     sb_div_appr_q/dc_divappr_q_n did not destroy the "bottom half" of N
  */
  MPN_COPY (tp, np, 2*n);

  /* estimate high m+1 limbs of quotient */
  if (m < DC_DIVAPPR_Q_N_THRESHOLD)
    qh = mpn_sb_divappr_q (q_hi, tp + 2*n - 2*m, 2*m,
			   dp + n - m, m, dip);
  else
    qh = mpn_dc_divappr_q_n (q_hi, tp + 2*n - 2*m,
			     dp + n - m, m, dip, tp + 2*n);

  /* decrease the estimate slightly (FIXME: actually I think 6 would be
     enough? but let's do 10 to be safe...) */
  qh -= mpn_sub_1 (q_hi, q_hi, m, (mp_limb_t) 10);
  /* don't let the estimate become negative */
  if (qh & GMP_NUMB_HIGHBIT)
    {
      MPN_ZERO (q_hi, m);
      qh = 0;
    }
  
  /* we know that {np+n-m, n+m} = q_hi * D + e0, where 0 <= e0 < C*B^n, where
     C is a small positive constant. Estimate q_hi * D using middle product. */
  mpn_mulmid (tp, dp, n, q_hi + 1, m - 2);
  /* do some parts of the middle product "manually": */
  tp[n - m + 2] += mpn_addmul_1 (tp, dp + m - 2, n - m + 2, q_hi[0]);
  mpn_addmul_1 (tp + 1, dp, n - m + 2, q_hi[m-1]);
  if (qh)
    mpn_add_n (tp + 2, tp + 2, dp, n - m + 1);

  /* subtract that estimate from N */
  mpn_sub_n (np + n - 2, np + n - 2, tp, n - m + 3);

  /* recursively divide to obtain low half of quotient */
  if (n - m + 2 < DC_DIVAPPR_Q_N_THRESHOLD)
    cy = mpn_sb_divappr_q (tp, np + m - 3, 2*n - 2*m + 4,
			   dp + m - 2, n - m + 2, dip);
  else
    cy = mpn_dc_divappr_q_n (tp, np + m - 3, dp + m - 2, n - m + 2,
			     dip, tp + n - m + 2);

  /* FIXME: this copy is annoying. The only reason it happens is that we
     elected to develop one extra quotient limb in the second recursive
     quotient. But I don't see how to avoid this and stay within the required
     error bounds. We inherit the error from the quotient, but there's also
     an error from the missed terms at the low end of the middle product. */
  MPN_COPY (qp, tp + 1, n - m);
  qh += mpn_add_1 (qp + n - m, qp + n - m, m, tp[n-m+1]);
  qh += mpn_add_1 (qp + n - m + 1, qp + n - m + 1, m - 1, cy);
  if (tp[0] >= GMP_NUMB_HIGHBIT)
    qh += mpn_add_1 (qp, qp, n, 1);   /* round quotient up */

  /* if qh == 2 (unlikely!), then Q must be 2000.... and we should return
     instead 1ffff.... */
  if (qh >= 2)
    {
      /* FIXME: hmmmm my test suite doesn't seem to generate this case,
       is it actually possible at all? */
      qh -= mpn_sub_1 (qp, qp, n, 1);
      ASSERT (qh == 1);
    }

  return qh;
}
