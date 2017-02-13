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

   Note there are m-n+1 different values of i+j and each product a_ib_j will be two limbs. Thus when added together, the sum must take up m-n+3 limbs of space.

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

  /* We first multiply by the first limb (or depending on optional function
     availability, limbs).  This result can be stored, not added, to rp.  We
     also avoid a loop for zeroing this way.  */

  hi = 0;
#if HAVE_NATIVE_mpn_mul_2
  if (vn >= 2)
    {
      lo = mpn_mul_2 (rp, up, un, vp);
      up -= 2, vp += 2, vn -= 2;
    }
  else
    {
      rp[un] = mpn_mul_1 (rp, up, un, vp[0]);
      return;
    }
#else
  lo = mpn_mul_1 (rp, up, un, vp[0]);
  up -= 1, vp += 1, vn -= 1;
#endif

  /* Now accumulate the product of up[] and the next higher limb (or depending
     on optional function availability, limbs) from vp[].  */

#define MAX_LEFT MP_SIZE_T_MAX	/* Used to simplify loops into if statements */


#if HAVE_NATIVE_mpn_addmul_6
  while (vn >= 6)
    {
      temp = mpn_addmul_6 (rp, up, un, vp);
      add_ssaaaa (hi, lo, hi, lo, 0, temp);
      if (MAX_LEFT == 6)
	goto overflow;
      up -= 6, vp += 6, vn -= 6;
      if (MAX_LEFT < 2 * 6)
	break;
    }
#undef MAX_LEFT
#define MAX_LEFT (6 - 1)
#endif

#if HAVE_NATIVE_mpn_addmul_5
  while (vn >= 5)
    {
      temp = mpn_addmul_5 (rp, up, un, vp);
      add_ssaaaa (hi, lo, hi, lo, 0, temp);
      if (MAX_LEFT == 5)
	goto overflow;
      up -= 5, vp += 5, vn -= 5;
      if (MAX_LEFT < 2 * 5)
	break;
    }
#undef MAX_LEFT
#define MAX_LEFT (5 - 1)
#endif

#if HAVE_NATIVE_mpn_addmul_4
  while (vn >= 4)
    {
      temp = mpn_addmul_4 (rp, up, un, vp);
      add_ssaaaa (hi, lo, hi, lo, 0, temp);
      if (MAX_LEFT == 4)
	goto overflow;
      up -= 4, vp += 4, vn -= 4;
      if (MAX_LEFT < 2 * 4)
	break;
    }
#undef MAX_LEFT
#define MAX_LEFT (4 - 1)
#endif

#if HAVE_NATIVE_mpn_addmul_3
  while (vn >= 3)
    {
      temp = mpn_addmul_3 (rp, up, un, vp);
      add_ssaaaa (hi, lo, hi, lo, 0, temp);
      if (MAX_LEFT == 3)
	goto overflow;
      up -= 3, vp += 3, vn -= 3;
      if (MAX_LEFT < 2 * 3)
	break;
    }
#undef MAX_LEFT
#define MAX_LEFT (3 - 1)
#endif

#if HAVE_NATIVE_mpn_addmul_2
  while (vn >= 2)
    {
      temp = mpn_addmul_2 (rp, up, un, vp);
      add_ssaaaa (hi, lo, hi, lo, 0, temp);
      if (MAX_LEFT == 2)
	goto overflow;
      up -= 2, vp += 2, vn -= 2;
      if (MAX_LEFT < 2 * 2)
	break;
    }
#undef MAX_LEFT
#define MAX_LEFT (2 - 1)
#endif

  while (vn >= 1)
    {
      temp = mpn_addmul_1 (rp, up, un, vp[0]);
      add_ssaaaa (hi, lo, hi, lo, 0, temp);
      if (MAX_LEFT == 1)
	goto overflow;
      up -= 1, vp += 1, vn -= 1;
    }

overflow:
  /* store final limbs */
#if GMP_NAIL_BITS != 0
  hi = (hi << GMP_NAIL_BITS) + (lo >> GMP_NUMB_BITS);
  lo &= GMP_NUMB_MASK;
#endif

  rp[un] = lo;
  rp[un + 1] = hi;
}
