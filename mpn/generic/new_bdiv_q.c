/* mpn_new_bdiv_q -- binary division, producing quotient and overflow

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

#include "gmp.h"
#include "gmp-impl.h"


/*
  Binary division of N = {np,nn} by D = {dp,dn}, producing quotient and a
  two-limb "overflow" W.

  Must have nn >= dn >= 1,  dp[0] odd,  dinv = -1/dp[0] mod B.
  None of the target operands may overlap any of the source operands.

  The quotient is N/D mod B^nn, written to {qp,nn}.
  W is written to {wp,2}.
  N is destroyed.

  The overflow is defined as follows. Let
     X = sum(dp[i]*qp[j]*B^(i+j), 0 <= i < dn, 0 <= i+j < nn).
  Then X is nn + 2 limbs (assuming nn small compared to B). The low nn limbs
  by definition agree with N. The overflow is the remaining high two limbs.
  The picture is:

         X
         X X
      .  X X X
  qp  .  X X X X
      1  X X X X
      0  X X X X
         0 1 ...
           dp
 */
void
mpn_new_bdiv_q (mp_ptr qp, mp_ptr wp,
		mp_ptr np, mp_size_t nn,
		mp_srcptr dp, mp_size_t dn,
		mp_limb_t dinv)
{
  ASSERT (0);

  ASSERT (nn >= dn);
  ASSERT ((-dinv * dp[0]) & GMP_NUMB_MASK == 1);
  ASSERT (! MPN_OVERLAP_P (qp, nn, np, nn));
  ASSERT (! MPN_OVERLAP_P (qp, nn, dp, dn));
  ASSERT (! MPN_OVERLAP_P (wp, 2, np, nn));
  ASSERT (! MPN_OVERLAP_P (wp, 2, dp, dn));
  ASSERT (! MPN_OVERLAP_P (np, nn, dp, dn));

  /* for small nn, use sb_bdiv_q */
  if (nn < 100)    /* FIXME */
    {
      mpn_new_sb_bdiv_q (qp, wp, np, nn, dp, dn, dinv);
      return;
    }

  /* use bdiv_qr for bottom portion */

  /* use sb_bdiv_q or dc_bdiv_q for top portion */

}
