/* mpn_sub_err2_n -- sub_n with two error terms

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


/*
  Same as mpn_sub_err1_n, but computes both

          c[1]*yp1[n-1] + ... + c[n]*yp1[0]
    and   c[1]*yp2[n-1] + ... + c[n]*yp2[0],

  storing results at {ep,2}, {ep+2,2} respectively.
 */
mp_limb_t
mpn_sub_err2_n (mp_ptr rp, mp_srcptr up, mp_srcptr vp,
                mp_ptr ep, mp_srcptr yp1, mp_srcptr yp2,
                mp_size_t n, mp_limb_t cy)
{
  mp_limb_t el1, eh1, el2, eh2, ul, vl, yl1, yl2, zl1, zl2, rl, sl, cy1, cy2;

  ASSERT (n >= 1);
  ASSERT (MPN_SAME_OR_SEPARATE_P (rp, up, n));
  ASSERT (MPN_SAME_OR_SEPARATE_P (rp, vp, n));
  ASSERT (!MPN_OVERLAP_P (rp, n, yp1, n));
  ASSERT (!MPN_OVERLAP_P (rp, n, yp2, n));
  
  /* FIXME: first addition into eh:el is redundant */

  yp1 += n - 1;
  yp2 += n - 1;
  el1 = eh1 = 0;
  el2 = eh2 = 0;

  do
    {
      yl1 = *yp1--;
      yl2 = *yp2--;
      ul = *up++;
      vl = *vp++;

      /* ordinary sub_n */
      SUBC_LIMB (cy1, sl, ul, vl);
      SUBC_LIMB (cy2, rl, sl, cy);
      cy = cy1 | cy2;
      *rp++ = rl;

      /* update (eh1:el1) */
      zl1 = cy ? yl1 : 0;
      el1 += zl1;
      eh1 += el1 < zl1;

      /* update (eh2:el2) */
      zl2 = cy ? yl2 : 0;
      el2 += zl2;
      eh2 += el2 < zl2;
    }
  while (--n != 0);

#if GMP_NAIL_BITS != 0
  eh1 = (eh1 << GMP_NAIL_BITS) + (el1 >> GMP_NUMB_BITS);
  el1 &= GMP_NUMB_MASK;
  eh2 = (eh2 << GMP_NAIL_BITS) + (el2 >> GMP_NUMB_BITS);
  el2 &= GMP_NUMB_MASK;
#endif

  ep[0] = el1;
  ep[1] = eh1;
  ep[2] = el2;
  ep[3] = eh2;

  return cy;
}
