/* mpn_toom42_mulmid -- toom42 middle product

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
  Let a = sum_0^{m-1} a_i B^i and b = sum_0^{n-1} b_j B^j

  then MP(a, m, b, n) = sum_{0<=i<m, 0<=j<n, n-1<=i+j<=m-1} a_ib_j B^{i+j-n+1}

  Note there are m-n+1 different values of i+j and each product a_ib_j will be two limbs. Thus when added together, the sum must take up n-m+3 limbs of space.

  This function computes the middle product of {ap,2n-1} and {bp,n}, 
  output written to {rp,n+2}, i.e. it computes MP(ap,2n-1,bp,n).

  Neither ap nor bp may overlap rp.

  Must have n >= 4.

  See mpn_toom42_mulmid_itch() for the amount of scratch space required.

  FIXME: this code assumes that n is small compared to GMP_NUMB_MAX. The exact
  requirements should be clarified.
*/
void
mpn_toom42_mulmid (mp_ptr rp, mp_srcptr ap, mp_srcptr bp, mp_size_t n,
                   mp_ptr scratch)
{
  mp_limb_t cy, e[12], t0, t1;
  mp_size_t m;
  int neg;

  ASSERT (n >= 4);
  ASSERT (! MPN_OVERLAP_P (rp, n + 2, ap, 2*n - 1));
  ASSERT (! MPN_OVERLAP_P (rp, n + 2, bp, n));

  ap += n & 1;   /* handle odd row and diagonal later */
  m = n / 2;

  /* (e0h:e0l) etc are correction terms, in 2's complement */
#define e0l (e[0])
#define e0h (e[1])
#define e1l (e[2])
#define e1h (e[3])
#define e2l (e[4])
#define e2h (e[5])
#define e3l (e[6])
#define e3h (e[7])
#define e4l (e[8])
#define e4h (e[9])
#define e5l (e[10])
#define e5h (e[11])

#define s scratch
#define d rp
#define p0 rp
#define p1 (scratch + 2*m - 1)
#define p2 (rp + m)
#define rec_scratch (scratch + 3*m + 1)

  /*
            rp                            scratch
  |---------|-----------|    |---------|---------|----------|
  0         m         2m+2   0         m         2m        3m+1
  <----d---><---p2----->     <-------------s------------->
  <----p0---->                                  <----p1---->
  */

  /* transposed interpolation, and compute correction terms */
  cy = mpn_add_err1_n (s, ap, ap + m, &e0l, bp + m, m - 1, 0);
  cy = mpn_add_err2_n (s + m - 1, ap + m - 1, ap + 2*m - 1, &e1l,
		       bp + m, bp, m, cy);
  mpn_add_err1_n (s + 2*m - 1, ap + 2*m - 1, ap + 3*m - 1, &e3l, bp, m, cy);

  if (mpn_cmp (bp + m, bp, m) < 0)
    {
      ASSERT_NOCARRY (mpn_sub_err2_n (d, bp, bp + m, &e4l,
				      ap + m - 1, ap + 2*m - 1, m, 0));
      neg = 1;
    }
  else
    {
      ASSERT_NOCARRY (mpn_sub_err2_n (d, bp + m, bp, &e4l,
				      ap + m - 1, ap + 2*m - 1, m, 0));
      neg = 0;
    }

  /* recursive middle products. The picture is:

      b[2m-1]   A   A   A   B   B   B   -   -   -   -   -
      ...       -   A   A   A   B   B   B   -   -   -   -
      b[m]      -   -   A   A   A   B   B   B   -   -   -
      b[m-1]    -   -   -   C   C   C   D   D   D   -   -
      ...       -   -   -   -   C   C   C   D   D   D   -
      b[0]      -   -   -   -   -   C   C   C   D   D   D
               a[0]   ...  a[m]  ...  a[2m]    ...    a[4m-2]
  */

  if (m < MULMID_TOOM42_THRESHOLD)
    {
      /* C + D */
      mpn_mulmid_basecase (p2, s + m, 2*m - 1, bp, m);
      t0 = p2[0], t1 = p2[1];    /* save first two limbs of p2 */
      /* B - C  (or C - B if neg == 1) */
      mpn_mulmid_basecase (p1, ap + m, 2*m - 1, d, m);
      /* A + B */
      mpn_mulmid_basecase (p0, s, 2*m - 1, bp + m, m);
    }
  else
    {
      /* as above, but call toom42 instead */
      mpn_toom42_mulmid (p2, s + m, bp, m, rec_scratch);
      t0 = p2[0], t1 = p2[1];
      mpn_toom42_mulmid (p1, ap + m, d, m, rec_scratch);
      mpn_toom42_mulmid (p0, s, bp + m, m, rec_scratch);
    }

  /* apply correction terms */

  /* -e0 at p0[0] */
  SUBC_LIMB (cy, p0[0], p0[0], e0l);
  e0h += cy;
  SUBC_LIMB (cy, p0[1], p0[1], e0h);
  if (UNLIKELY (cy))
    e2h += mpn_sub_1 (p0 + 2, p0 + 2, m - 1, 1);

  /* absorb t0 into e1 */
  ADDC_LIMB (cy, e1l, e1l, t0);
  e1h += cy;

  /* e1 at p0[m] */
  ADDC_LIMB (cy, p0[m], p0[m], e1l);
  p0[m + 1] += e1h + cy;

  /* add back t1 */
  ADDC_LIMB (cy, p2[1], p2[1], t1);
  if (UNLIKELY (cy))
    mpn_add_1 (p2 + 2, p2 + 2, m, 1);

  /* -e2 at p2[0] */
  SUBC_LIMB (cy, p2[0], p2[0], e2l);
  e2h += cy;
  SUBC_LIMB (cy, p2[1], p2[1], e2h);
  if (UNLIKELY (cy))
    mpn_sub_1 (p2 + 2, p2 + 2, m, 1);

  /* e3 at p2[m] */
  ADDC_LIMB (cy, p2[m], p2[m], e3l);
  p2[m + 1] = (p2[m + 1] + e3h + cy) & GMP_NUMB_MASK;

  /* e4 at p1[0] */
  ADDC_LIMB (cy, p1[0], p1[0], e4l);
  e4h += cy;
  ADDC_LIMB (cy, p1[1], p1[1], e4h);
  if (UNLIKELY (cy))
    mpn_add_1 (p1 + 2, p1 + 2, m, 1);

  /* -e5 at p1[m] */
  SUBC_LIMB (cy, p1[m], p1[m], e5l);
  p1[m + 1] = (p1[m + 1] - e5h - cy) & GMP_NUMB_MASK;

  /* adjustment if p1 ends up negative */
  cy = (p1[m + 1] & GMP_NUMB_HIGHBIT) != 0;

  /* transposed evaluation */
  if (neg)
    {
      mpn_sub_1 (rp + m + 2, rp + m + 2, m, cy);
      mpn_add (rp, rp, 2*m + 2, p1, m + 2);             /* A + C */
      mpn_sub_n (rp + m, rp + m, p1, m + 2);            /* B + D */
    }
  else
    {
      mpn_add_1 (rp + m + 2, rp + m + 2, m, cy);
      mpn_sub (rp, rp, 2*m + 2, p1, m + 2);             /* A + C */
      mpn_add_n (rp + m, rp + m, p1, m + 2);            /* B + D */
    }

  /* odd row and diagonal */
  if (n & 1)
    {
      /*
        Cells marked E are already done. We need to do cells marked O.

        OOOOO----
        -EEEEO---
        --EEEEO--
        ---EEEEO-
        ----EEEEO
       */

      /* first row of O's */
      cy = mpn_addmul_1 (rp, ap - 1, n, bp[n - 1]);
      ADDC_LIMB (rp[n + 1], rp[n], rp[n], cy);

      /* O's on diagonal */
      /* FIXME: should probably define an interface "mpn_mulmid_diag_1"
         that can handle the sum below. Currently we're relying on
         mulmid_basecase being pretty fast for a diagonal sum like this,
	 which is true at least for the K8 asm verion, but surely false
	 for the generic version. */
      mpn_mulmid_basecase (e, ap + n - 1, n - 1, bp, n - 1);
      mpn_add_n (rp + n - 1, rp + n - 1, e, 3);
    }
}
