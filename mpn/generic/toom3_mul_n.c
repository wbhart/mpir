/* mpn_toom3_mul and helper functions -- Multiply/square natural numbers.

   THE HELPER FUNCTIONS IN THIS FILE (meaning everything except mpn_mul_n)
   ARE INTERNAL FUNCTIONS WITH MUTABLE INTERFACES.  IT IS ONLY SAFE TO REACH
   THEM THROUGH DOCUMENTED INTERFACES.  IN FACT, IT IS ALMOST GUARANTEED
   THAT THEY'LL CHANGE OR DISAPPEAR IN A FUTURE GNU MP RELEASE.


Copyright 1991, 1993, 1994, 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003,
2005, Free Software Foundation, Inc.

Copyright 2009 Jason Moxham
Copyright 2009 William Hart

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA. */

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"

/******************************************************************************
 *                                                                            *
 *              Toom 3-way multiplication and squaring                        *
 *                                                                            *
 *****************************************************************************/
#define TOOM3_MUL_REC(p, a, b, n, ws) \
  do {								\
    if (MUL_TOOM3_THRESHOLD / 3 < MUL_KARATSUBA_THRESHOLD	\
	&& BELOW_THRESHOLD (n, MUL_KARATSUBA_THRESHOLD))	\
      mpn_mul_basecase (p, a, n, b, n);				\
    else if (BELOW_THRESHOLD (n, MUL_TOOM3_THRESHOLD))		\
      mpn_kara_mul_n (p, a, b, n, ws);				\
    else							\
      mpn_toom3_mul_n (p, a, b, n, ws);				\
  } while (0)

#define TOOM3_SQR_REC(p, a, n, ws)				\
  do {								\
    if (SQR_TOOM3_THRESHOLD / 3 < SQR_BASECASE_THRESHOLD	\
	&& BELOW_THRESHOLD (n, SQR_BASECASE_THRESHOLD))		\
      mpn_mul_basecase (p, a, n, a, n);				\
    else if (SQR_TOOM3_THRESHOLD / 3 < SQR_KARATSUBA_THRESHOLD	\
	&& BELOW_THRESHOLD (n, SQR_KARATSUBA_THRESHOLD))	\
      mpn_sqr_basecase (p, a, n);				\
    else if (BELOW_THRESHOLD (n, SQR_TOOM3_THRESHOLD))		\
      mpn_kara_sqr_n (p, a, n, ws);				\
    else							\
      mpn_toom3_sqr_n (p, a, n, ws);				\
  } while (0)

/* The necessary temporary space T(n) satisfies T(n)=0 for n < THRESHOLD,
   and T(n) <= max(2n+2, 6k+3, 4k+3+T(k+1)) otherwise, where k = ceil(n/3).

   Assuming T(n) >= 2n, 6k+3 <= 4k+3+T(k+1).
   Similarly, 2n+2 <= 6k+2 <= 4k+3+T(k+1).

   With T(n) = 2n+S(n), this simplifies to S(n) <= 9 + S(k+1).
   Since THRESHOLD >= 17, we have n/(k+1) >= 19/8
   thus S(n) <= S(n/(19/8)) + 9 thus S(n) <= 9*log(n)/log(19/8) <= 8*log2(n).

   We need in addition 2*r for mpn_sublsh1_n, so the total is at most
   8/3*n+8*log2(n).
*/
void
mpn_toom3_mul_n (mp_ptr c, mp_srcptr a, mp_srcptr b, mp_size_t n, mp_ptr t)
{
  mp_size_t k, k1, kk1, r, twok, rr2;
  mp_limb_t cy, cc, saved, vinf0;
  mp_ptr trec;
  int sa, sb;
  mp_ptr c1, c2, c3, c4, c5, t1, t2, t3, t4;

  ASSERT(GMP_NUMB_BITS >= 6);

  k = (n + 2) / 3; /* ceil(n/3) */
  ASSERT(GMP_NUMB_BITS >= 6);
  ASSERT(n >= 17); /* so that r <> 0 and 5k+3 <= 2n */

  twok = 2 * k;
  k1 = k + 1;
  kk1 = k + k1;
  r = n - twok;   /* last chunk */
  rr2 = 2*r;

  c1 = c + k;
  c2 = c1 + k;
  c3 = c2 + k;
  c4 = c3 + k;
  c5 = c4 + k;
  
  t1 = t + k;
  t2 = t1 + k;
  t3 = t2 + k;
  t4 = t3 + k;

  trec = t + 4 * k + 4; 

  /* put a0+a2 in {c, k+1}, and b0+b2 in {c4 + 2, k+1};
     put a0+a1+a2 in {t2 + 1, k+1} and b0+b1+b2 in {t3 + 2,k+1}
  */
  c1[0] = mpn_add_n (c, a, a + twok, r);
  c5[2] = mpn_add_n (c4 + 2, b, b + twok, r);
  if (r < k)
    {
      c1[0] = mpn_add_1 (c + r, a + r, k - r, c1[0]);
	   c5[2] = mpn_add_1 (c4 + 2 + r, b + r, k - r, c5[2]);
    }
  t3[1] = c1[0] + mpn_add_n (t2 + 1, c, a + k, k);
  t4[2] = c5[2] + mpn_add_n (t3 + 2, c4 + 2, b + k, k);

  ASSERT(c1[0] < 2);
  ASSERT(c5[2] < 2);
  ASSERT(t3[1] < 3);
  ASSERT(t4[2] < 3);

  /* compute v1 := (a0+a1+a2)*(b0+b1+b2) in {c2, 2k+1};
     since v1 < 9*B^(2k), v1 uses only 2k+1 words if GMP_NUMB_BITS >= 4 */
  TOOM3_MUL_REC (c2, t2 + 1, t3 + 2, k1, trec);

  ASSERT(c2[k+k] < 9);

  /* {c,2k} {c+2k,2k+1} {c+4k+1,r+r2-1} 
					v1
  */

  /* put |a0-a1+a2| in {c,k+1} and |b0-b1+b2| in {c4 + 2,k+1} */
  /* sa = sign(a0-a1+a2) */
  /* sb = sign(b0-b1+b2) */
  sa = (c[k] != 0) ? 1 : mpn_cmp (c, a + k, k);
  c[k] = (sa >= 0) ? c[k] - mpn_sub_n (c, c, a + k, k)
		   : mpn_sub_n (c, a + k, c, k);
  /* b0+b2 is in {c4+2, k+1} now */
  sb = (c5[2] != 0) ? 1 : mpn_cmp (c4 + 2, b + k, k);
  c5[2] = (sb >= 0) ? c5[2] - mpn_sub_n (c4 + 2, c4 + 2, b + k, k)
		    : mpn_sub_n (c4 + 2, b + k, c4 + 2, k);
  
  ASSERT(c[k] < 2);
  ASSERT(c5[2] < 2);

  sa *= sb; /* sign of vm1 */

  /* compute vm1 := (a0-a1+a2)*(b0-b1+b2) in {t, 2k+1};
     since |vm1| < 4*B^(2k), vm1 uses only 2k+1 limbs */
  TOOM3_MUL_REC (t, c, c4 + 2, k1, trec);

  ASSERT(t[k+k] < 4);

  /* {c,2k} {c+2k,2k+1} {c+4k+1,r+r2-1} 
					v1

	  {t, 2k+1} {t+2k+1, 2k + 1}
	     vm1
  */

  /* 
     compute a0+2a1+4a2 in {c, k+1} and b0+2b1+4b2 in {c4 + 2, k+1}
  */
#if HAVE_NATIVE_mpn_addlsh1_n
  c1[0] = mpn_addlsh1_n (c, a + k, a + twok, r);
  c5[2] = mpn_addlsh1_n (c4 + 2, b + k, b + twok, r);
  if (r < k)
    {
      c1[0] = mpn_add_1(c + r, a + k + r, k - r, c1[0]);
      c5[2] = mpn_add_1(c4 + 2 + r, b + k + r, k - r, c5[2]);
    }
  c1[0] = 2 * c1[0] + mpn_addlsh1_n (c, a, c, k);
  c5[2] = 2 * c5[2] + mpn_addlsh1_n (c4 + 2, b, c4 + 2, k);
#else
  c[r] = mpn_lshift1 (c, a + twok, r);
  c4[r + 2] = mpn_lshift1 (c4 + 2, b + twok, r);
  if (r < k)
    {
      MPN_ZERO(c + r + 1, k - r);
      MPN_ZERO(c4 + r + 3, k - r);
    }
  c1[0] += mpn_add_n (c, c, a + k, k);
  c5[2] += mpn_add_n (c4 + 2, c4 + 2, b + k, k);
  mpn_double (c, k1);
  mpn_double (c4 + 2, k1);
  c1[0] += mpn_add_n (c, c, a, k);
  c5[2] += mpn_add_n (c4 + 2, c4 + 2, b, k);
#endif

  ASSERT(c[k] < 7);
  ASSERT(c5[2] < 7);

#define v2 (t+2*k+1)

  /* compute v2 := (a0+2a1+4a2)*(b0+2b1+4b2) in {t+2k+1, 2k+1}
     v2 < 49*B^k so v2 uses at most 2k+1 limbs if GMP_NUMB_BITS >= 6 */
  TOOM3_MUL_REC (v2, c, c4 + 2, k1, trec);

  ASSERT(v2[k+k] < 49);

  /* {c,2k} {c+2k,2k+1} {c+4k+1,r+r2-1} 
					v1

	  {t, 2k+1} {t+2k+1, 2k + 1}
	     vm1        v2
  */

  /* compute v0 := a0*b0 in {c, 2k} */
  TOOM3_MUL_REC (c, a, b, k, trec);

 /* {c,2k} {c+2k,2k+1} {c+4k+1,r+r2-1} 
		v0 		v1

	  {t, 2k+1} {t+2k+1, 2k + 1}
	     vm1        v2
  */

#define vinf (c+4*k)

  /* compute vinf := a2*b2 in {c4, r + r2},
  */
  saved = c4[0];

  TOOM3_MUL_REC (c4, a + twok, b + twok, r, trec);
  
  vinf0 = c4[0];
  c4[0] = saved;
  
 /* {c,2k} {c+2k,2k+1} {c+4k+1,r+r2-1} 
		v0 		v1        {-}vinf

	  {t, 2k+1} {t+2k+1, 2k + 1}
	     vm1        v2

	  vinf0 = {-}
  */

  mpn_toom3_interpolate (c, c2, v2, t, vinf, k, rr2, sa, vinf0, t4+2);

#undef v2
#undef vinf
}

void
mpn_toom3_sqr_n (mp_ptr c, mp_srcptr a, mp_size_t n, mp_ptr t)
{
  mp_size_t k, k1, kk1, r, twok, rr2;
  mp_limb_t cy, cc, saved, vinf0;
  mp_ptr trec;
  int sa;
  mp_ptr c1, c2, c3, c4, c5, t1, t2, t3, t4;

  ASSERT(GMP_NUMB_BITS >= 6);

  k = (n + 2) / 3; /* ceil(n/3) */
  ASSERT(GMP_NUMB_BITS >= 6);
  ASSERT(n >= 17); /* so that r <> 0 and 5k+3 <= 2n */

  twok = 2 * k;
  k1 = k + 1;
  kk1 = k + k1;
  r = n - twok;   /* last chunk */
  rr2 = 2*r;

  c1 = c + k;
  c2 = c1 + k;
  c3 = c2 + k;
  c4 = c3 + k;
  c5 = c4 + k;
  
  t1 = t + k;
  t2 = t1 + k;
  t3 = t2 + k;
  t4 = t3 + k;

  trec = t + 4 * k + 3; 

  /* put a0+a2 in {c, k+1}
     put a0+a1+a2 in {t2 + 1, k+1}
  */
  cy = mpn_add_n (c, a, a + twok, r);
  if (r < k)
    {
      __GMPN_ADD_1 (cy, c + r, a + r, k - r, cy);
   }
  t3[1] = (c1[0] = cy) + mpn_add_n (t2 + 1, c, a + k, k);

  /* compute v1 := (a0+a1+a2)^2 in {c2, 2k+1};
     since v1 < 9*B^(2k), v1 uses only 2k+1 words if GMP_NUMB_BITS >= 4 */
  TOOM3_SQR_REC (c2, t2 + 1, k1, trec);

  /* {c,2k} {c+2k,2k+1} {c+4k+1,r+r2-1} 
					v1
  */

  /* put |a0-a1+a2| in {c,k+1} */
  sa = (c[k] != 0) ? 1 : mpn_cmp (c, a + k, k);
  c[k] = (sa >= 0) ? c[k] - mpn_sub_n (c, c, a + k, k)
		   : mpn_sub_n (c, a + k, c, k);
  
  /* compute vm1 := (a0-a1+a2)^2 in {t, 2k+1};
     since |vm1| < 4*B^(2k), vm1 uses only 2k+1 limbs */
  TOOM3_SQR_REC (t, c, k1, trec);

  /* {c,2k} {c+2k,2k+1} {c+4k+1,r+r2-1} 
					v1

	  {t, 2k+1} {t+2k+1, 2k + 1}
	     vm1
  */

  /* 
     compute a0+2a1+4a2 in {c, k+1}
  */
#if HAVE_NATIVE_mpn_addlsh1_n
  c1[0] = mpn_addlsh1_n (c, a + k, a + twok, r);
  if (r < k)
    {
      __GMPN_ADD_1 (c1[0], c + r, a + k + r, k - r, c1[0]);
    }
  c1[0] = 2 * c1[0] + mpn_addlsh1_n (c, a, c, k);
#else
  c[r] = mpn_lshift1 (c, a + twok, r);
  if (r < k)
    {
      MPN_ZERO(c + r + 1, k - r);
    }
  c1[0] += mpn_add_n (c, c, a + k, k);
  mpn_double (c, k1);
  c1[0] += mpn_add_n (c, c, a, k);
#endif

#define v2 (t+2*k+1)

  /* compute v2 := (a0+2a1+4a2)^2 in {t+2k+1, 2k+1}
     v2 < 49*B^k so v2 uses at most 2k+1 limbs if GMP_NUMB_BITS >= 6 */
  TOOM3_SQR_REC (v2, c, k1, trec);

  /* {c,2k} {c+2k,2k+1} {c+4k+1,r+r2-1} 
					v1

	  {t, 2k+1} {t+2k+1, 2k + 1}
	     vm1        v2
  */

  /* compute v0 := a0^2 in {c, 2k} */
  TOOM3_SQR_REC (c, a, k, trec);

 /* {c,2k} {c+2k,2k+1} {c+4k+1,r+r2-1} 
		v0 		v1

	  {t, 2k+1} {t+2k+1, 2k + 1}
	     vm1        v2
  */

#define vinf (c+4*k)

  /* compute vinf := a2*b2 in {c4, r + r2},
  */
  saved = c4[0];

  TOOM3_SQR_REC (c4, a + twok, r, trec);
  
  vinf0 = c4[0];
  c4[0] = saved;

 /* {c,2k} {c+2k,2k+1} {c+4k+1,r+r2-1} 
		v0 		v1        {-}vinf

	  {t, 2k+1} {t+2k+1, 2k + 1}
	     vm1        v2

	  vinf0 = {-}
  */

  mpn_toom3_interpolate (c, c2, v2, t, vinf, k, rr2, 1, vinf0, t4+2);

#undef v2
#undef vinf
}
