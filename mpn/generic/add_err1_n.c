/* mpn_add_err1_n -- add_n with single error term

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
  Computes:

  (1) {rp,n} := {up,n} + {vp,n} (just like mpn_add_n) with incoming carry cy,
  return value is carry out.

  (2) Let c[i+1] = carry from i-th limb addition (c[0] = cy).
  Computes c[1]*yp[n-1] + ... + c[n]*yp[0], stores two-limb result at ep.

  Assumes n >= 1.
 */
mp_limb_t
mpn_add_err1_n (mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_ptr ep, mp_srcptr yp,
                mp_size_t n, mp_limb_t cy)
{
  mp_limb_t el, eh, ul, vl, yl, zl, rl, sl, cy1, cy2;

  ASSERT (n >= 1);
  ASSERT (MPN_SAME_OR_SEPARATE_P (rp, up, n));
  ASSERT (MPN_SAME_OR_SEPARATE_P (rp, vp, n));
  ASSERT (!MPN_OVERLAP_P (rp, n, yp, n));

  /* FIXME: first addition into eh:el is redundant */

  yp += n - 1;
  el = eh = 0;

  do
    {
      yl = *yp--;
      ul = *up++;
      vl = *vp++;

      /* ordinary add_n */
      ADDC_LIMB (cy1, sl, ul, vl);
      ADDC_LIMB (cy2, rl, sl, cy);
      cy = cy1 | cy2;
      *rp++ = rl;

      /* update (eh:el) */
      zl = cy ? yl : 0;
      /* FIXME: consider alternative:
            zl = (-cy) & yl;
         Might be better on some machines?
         Ditto for sub_err1_n, add_err2_n etc. */
      el += zl;
      eh += el < zl;
    }
  while (--n != 0);

#if GMP_NAIL_BITS != 0
  eh = (eh << GMP_NAIL_BITS) + (el >> GMP_NUMB_BITS);
  el &= GMP_NUMB_MASK;
#endif

  ep[0] = el;
  ep[1] = eh;

  return cy;
}
