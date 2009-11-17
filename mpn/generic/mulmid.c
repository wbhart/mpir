/* mpn_mulmid -- middle product

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


#define CHUNK (200 + MULMID_TOOM42_THRESHOLD)

/* Let a = sum_0^{m-1} a_i B^i and b = sum_0^{n-1} b_j B^j

   then MP(a, m, b, n) = sum_{0<=i<m, 0<=j<n, n-1<=i+j<=m-1} a_ib_j B^{i+j-n+1}

   Note there are m-n+1 different values of i+j and each product a_ib_j will be two limbs. Thus when added together, the sum must take up n-m+3 limbs of space.

   This function computes MP(ap,an,bp,bn), placing the result in {rp, an-bn+3}.
   
   It is required that bn << GMP_NUMBMAX.
*/

void
mpn_mulmid (mp_ptr rp,
            mp_srcptr ap, mp_size_t an,
            mp_srcptr bp, mp_size_t bn)
{
  mp_size_t rn, k;
  mp_ptr scratch, temp;

  ASSERT (an >= bn);
  ASSERT (bn >= 1);
  ASSERT (! MPN_OVERLAP_P (rp, an - bn + 3, ap, an));
  ASSERT (! MPN_OVERLAP_P (rp, an - bn + 3, bp, bn));

  if (bn < MULMID_TOOM42_THRESHOLD)
    {
      /* region not tall enough to make toom42 worthwhile for any portion */

      if (an < CHUNK)
	{
	  /* region not too wide either, just call basecase directly */
	  mpn_mulmid_basecase (rp, ap, an, bp, bn);
	  return;
	}

      /* Region quite wide. For better locality, use basecase on chunks:

	 AAABBBCC..
	 .AAABBBCC.
	 ..AAABBBCC
      */

      k = CHUNK - bn + 1;    /* number of diagonals per chunk */

      /* first chunk (marked A in the above diagram) */
      mpn_mulmid_basecase (rp, ap, CHUNK, bp, bn);

      /* remaining chunks (B, C, etc) */
      an -= k;

      while (an >= CHUNK)
	{
	  mp_limb_t t0, t1, cy;
	  ap += k, rp += k;
	  t0 = rp[0], t1 = rp[1];
	  mpn_mulmid_basecase (rp, ap, CHUNK, bp, bn);
	  ADDC_LIMB (cy, rp[0], rp[0], t0);    /* add back saved limbs */
	  MPN_INCR_U (rp + 1, k + 1, t1 + cy);
	  an -= k;
	}

      if (an >= bn)
	{
	  /* last remaining chunk */
	  mp_limb_t t0, t1, cy;
	  ap += k, rp += k;
	  t0 = rp[0], t1 = rp[1];
	  mpn_mulmid_basecase (rp, ap, an, bp, bn);
	  ADDC_LIMB (cy, rp[0], rp[0], t0);
	  MPN_INCR_U (rp + 1, an - bn + 2, t1 + cy);
	}

      return;
    }

  /* region is tall enough for toom42 */

  rn = an - bn + 1;

  if (rn < MULMID_TOOM42_THRESHOLD)
    {
      /* region not wide enough to make toom42 worthwhile for any portion */

      if (bn < CHUNK)
	{
	  /* region not too tall either, just call basecase directly */
	  mpn_mulmid_basecase (rp, ap, an, bp, bn);
	  return;
	}

      /* Region quite tall. For better locality, use basecase on chunks:

	 AAAAA....
	 .AAAAA...
	 ..BBBBB..
	 ...BBBBB.
	 ....CCCCC
      */
      {
          TMP_DECL;
          TMP_MARK;

          temp = TMP_ALLOC_LIMBS (rn + 2);

          /* first chunk (marked A in the above diagram) */
          bp += bn - CHUNK, an -= bn - CHUNK;
          mpn_mulmid_basecase (rp, ap, an, bp, CHUNK);

          /* remaining chunks (B, C, etc) */
          bn -= CHUNK;

          while (bn >= CHUNK)
	    {
	      ap += CHUNK, bp -= CHUNK;
	      mpn_mulmid_basecase (temp, ap, an, bp, CHUNK);
	      mpn_add_n (rp, rp, temp, rn + 2);
	      bn -= CHUNK;
	    }

          if (bn)
	    {
	      /* last remaining chunk */
	      ap += CHUNK, bp -= bn;
	      mpn_mulmid_basecase (temp, ap, rn + bn - 1, bp, bn);
	      mpn_add_n (rp, rp, temp, rn + 2);
	    }

          TMP_FREE;
      }
      return;
    }

  /* we're definitely going to use toom42 somewhere */

  if (bn > rn)
    {
      /* slice region into chunks, use toom42 on all chunks except possibly
	 the last:

         AA....
         .AA...
         ..BB..
         ...BB.
         ....CC
      */

      TMP_DECL;
      TMP_MARK;

      temp = TMP_ALLOC_LIMBS (rn + 2 + mpn_toom42_mulmid_itch (rn));
      scratch = temp + rn + 2;

      /* first chunk (marked A in the above diagram) */
      bp += bn - rn;
      mpn_toom42_mulmid (rp, ap, bp, rn, scratch);

      /* remaining chunks (B, C, etc) */
      bn -= rn;

      while (bn >= rn)
        {
          ap += rn, bp -= rn;
	  mpn_toom42_mulmid (temp, ap, bp, rn, scratch);
          mpn_add_n (rp, rp, temp, rn + 2);
          bn -= rn;
        }

      if (bn)
        {
          /* last remaining chunk */
          ap += rn, bp -= bn;
	  mpn_mulmid (temp, ap, rn + bn - 1, bp, bn);
          mpn_add_n (rp, rp, temp, rn + 2);
        }

      TMP_FREE;
    }
  else
    {
      /* slice region into chunks, use toom42 on all chunks except possibly
	 the last:

         AAABBBCC..
         .AAABBBCC.
         ..AAABBBCC
      */

      TMP_DECL;
      TMP_MARK;

      scratch = TMP_ALLOC_LIMBS (mpn_toom42_mulmid_itch (bn));

      /* first chunk (marked A in the above diagram) */
      mpn_toom42_mulmid (rp, ap, bp, bn, scratch);

      /* remaining chunks (B, C, etc) */
      rn -= bn;

      while (rn >= bn)
        {
	  mp_limb_t t0, t1, cy;
          ap += bn, rp += bn;
          t0 = rp[0], t1 = rp[1];
          mpn_toom42_mulmid (rp, ap, bp, bn, scratch);
	  ADDC_LIMB (cy, rp[0], rp[0], t0);     /* add back saved limbs */
	  MPN_INCR_U (rp + 1, bn + 1, t1 + cy);
	  rn -= bn;
        }

      TMP_FREE;

      if (rn)
        {
          /* last remaining chunk */
	  mp_limb_t t0, t1, cy;
          ap += bn, rp += bn;
          t0 = rp[0], t1 = rp[1];
          mpn_mulmid (rp, ap, rn + bn - 1, bp, bn);
	  ADDC_LIMB (cy, rp[0], rp[0], t0);
	  MPN_INCR_U (rp + 1, rn + 1, t1 + cy);
        }
    }
}
