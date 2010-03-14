/* mpn_dc_bdiv_q_n -- binary division, producing quotient and overflow,
                        divide-and-conquer algorithm

Copyright (C) 2009, David Harvey
Copyright (C) 2010, William Hart.

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

/*
  Computes {np, n} / {dp, n} mod B^n, using divide-and-conquer
  algorithm, switching to classical for n <= BDIV_Q_DC_THRESHOLD.

  Also computes a 2 limb "overflow". See sb_bdiv_q.c for a definition.

  scratch is workspace.
*/
void
mpn_dc_bdiv_q_n (mp_ptr qp, mp_ptr wp, mp_ptr np, mp_srcptr dp,
		   mp_size_t n, mp_limb_t dinv, mp_ptr scratch)
{
  mp_size_t s, t;
  mp_limb_t cy;

  ASSERT (n >= 6);
  ASSERT (! MPN_OVERLAP_P (qp, n, np, n));
  ASSERT (! MPN_OVERLAP_P (qp, n, dp, n));
  ASSERT (! MPN_OVERLAP_P (wp, 2, np, n));
  ASSERT (! MPN_OVERLAP_P (wp, 2, dp, n));
  ASSERT (! MPN_OVERLAP_P (np, n, dp, n));

  /*
    Example with s = 4, t = 3, n = 7:

         C
         C C
         C C C
  qp  .  A B B B
      .  A A B B B
      1  A A A B B B
      0  A A A A B B B
         0 1 ...
           dp
  */

  t = n / 2;    /*  t = floor(n/2)  */
  s = n - t;    /*  s = ceil(n/2)   */

  /*  recurse into low half of quotient (region A)  */
  if (s <= DC_BDIV_Q_THRESHOLD)
    mpn_sb_bdiv_q (qp, wp, np, s, dp, s, dinv);
  else
    mpn_dc_bdiv_q_n (qp, wp, np, dp, s, dinv, scratch);

  /*  remove region B and overflow from A from N
      (if n odd, do first row of B separately --- we could have used
      mpn_mulmid, but this saves some logic) */
  mpn_mulmid_n (scratch, dp + 1, qp + (n & 1), t);
  if (n & 1)
    {
      cy = mpn_addmul_1 (scratch, dp + s, t, qp[0]);
      MPN_INCR_U (scratch + t, 2, cy);
    }
  ADDC_LIMB (cy, scratch[0], scratch[0], wp[0]);      /* overflow from A */
  MPN_INCR_U (scratch + 1, t + 1, wp[1] + cy);
  cy = mpn_sub_n (np + s, np + s, scratch, t);
  MPN_INCR_U (scratch + t, 2, cy);

  /*  recurse into top half of quotient (region C)
      (this does not overwrite {scratch + t, 2}, because n >= 6 implies
      t >= 3 implies floor(t/2) + 2 <= t) */
  if (t <= DC_BDIV_Q_THRESHOLD)
    mpn_sb_bdiv_q (qp + s, wp, np + s, t, dp, t, dinv);
  else
    mpn_dc_bdiv_q_n (qp + s, wp, np + s, dp, t, dinv, scratch);

  /*  combine overflows from B and C  */
  ADDC_LIMB (cy, wp[0], wp[0], scratch[t]);
  wp[1] += scratch[t + 1] + cy;
}
