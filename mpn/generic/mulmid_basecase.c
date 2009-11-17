/* mpn_mulmid_basecase -- classical middle product algorithm

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

/* Let a = sum_0^{m-1} a_i B^i and b = sum_0^{n-1} b_j B^j

   then MP(a, m, b, n) = sum_{0<=i<m, 0<=j<n, n-1<=i+j<=m-1} a_ib_j B^{i+j-n+1}

   Note there are m-n+1 different values of i+j and each product a_ib_j will be two limbs. Thus when added together, the sum must take up n-m+3 limbs of space.

   This function computes MP(up,un,vp,vn), writing the result to {rp,un-vn+3}.
   Must have un >= vn >= 1.

   Neither input buffer may overlap with the output buffer. 

   It is required that vn << GMP_NUMBMAX. 
*/

void
mpn_mulmid_basecase (mp_ptr rp,
                     mp_srcptr up, mp_size_t un,
                     mp_srcptr vp, mp_size_t vn)
{
  mp_limb_t lo, hi;  /* last two limbs of output */
  mp_limb_t temp;

  ASSERT (un >= vn);
  ASSERT (vn >= 1);
  ASSERT (! MPN_OVERLAP_P (rp, un - vn + 3, up, un));
  ASSERT (! MPN_OVERLAP_P (rp, un - vn + 3, vp, vn));

  up += vn - 1;
  un -= vn - 1;

  /* multiply by first limb, store result */
  lo = mpn_mul_1 (rp, up, un, vp[0]);
  hi = 0;

  /* accumulate remaining rows */
  for (vn--; vn; vn--)
    {
      up--, vp++;
      temp = mpn_addmul_1 (rp, up, un, vp[0]);
      add_ssaaaa (hi, lo, hi, lo, 0, temp);
    }

  /* store final limbs */
#if GMP_NAIL_BITS != 0
  hi = (hi << GMP_NAIL_BITS) + (lo >> GMP_NUMB_BITS);
  lo &= GMP_NUMB_MASK;
#endif

  rp[un] = lo;
  rp[un + 1] = hi;
}
