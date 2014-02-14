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
 *                      Toom 3-way multiplication                             *
 *                                                                            *
 *****************************************************************************/

/*
   We have

	{v0,2k} {v1,2k+1} {c+4k+1,r+r2-1} 
		v0 	v1       {-}vinf

	vinf0 is the first limb of vinf, which is overwritten by v1

	{vm1,2k+1} {v2, 2k+1}

	ws is temporary space

	sa is the sign of vm1

	rr2 is r+r2

	We want to compute

     t1   <- (3*v0+2*vm1+v2)/6-2*vinf
     t2   <- (v1+vm1)/2
  then the result is c0+c1*t+c2*t^2+c3*t^3+c4*t^4 where
     c0   <- v0
     c1   <- v1 - t1
     c2   <- t2 - v0 - vinf
     c3   <- t1 - t2
     c4   <- vinf
*/ 
void
mpn_toom3_interpolate (mp_ptr c, mp_ptr v1, mp_ptr v2, mp_ptr vm1,
		                 mp_ptr vinf, mp_size_t k, mp_size_t rr2, int sa,
		                                       mp_limb_t vinf0, mp_ptr ws)
{
  mp_limb_t cy, saved;
  mp_size_t twok = k + k;
  mp_size_t kk1 = twok + 1;
  mp_ptr c1, c2, c3, c4, c5;
  mp_limb_t cout; /* final carry, should be zero at the end */

  c1 = c + k;
  c2 = c1 + k;
  c3 = c2 + k;
  c4 = c3 + k;
  c5 = c4 + k;

#define v0 (c)

/* {c,2k} {c+2k,2k+1} {c+4k+1,r+r2-1} 
		v0 	 v1          {-}vinf

	{vm1,2k+1} {v2, 2k+1}
*/

  /* v2 <- v2 - vm1 */
  if (sa < 0)
  {
	  mpn_add_n(v2, v2, vm1, kk1);
  } else
  {
	  mpn_sub_n(v2, v2, vm1, kk1);
  }

  ASSERT_NOCARRY (mpn_divexact_by3 (v2, v2, kk1));    /* v2 <- v2 / 3 */

 /* vm1 <- t2 := (v1 - sa*vm1) / 2
 */
  if (sa < 0)
    {
#ifdef HAVE_NATIVE_mpn_rsh1add_n
      mpn_rsh1add_n (vm1, v1, vm1, kk1);
#else
      mpn_add_n (vm1, vm1, v1, kk1);
      mpn_half (vm1, kk1);
#endif
    }
  else
    {
#ifdef HAVE_NATIVE_mpn_rsh1sub_n
      mpn_rsh1sub_n (vm1, v1, vm1, kk1);
#else
      mpn_sub_n (vm1, v1, vm1, kk1);
      mpn_half (vm1, kk1);
#endif
    }

  /* v1 <- v1 - v0 - vinf */

  saved = c4[0];
  c4[0] = vinf0;
#if HAVE_NATIVE_mpn_subadd_n
  cy = mpn_subadd_n(v1, v1, v0, c4, rr2);
#else
  cy = mpn_sub_n(v1, v1, v0, rr2);
  cy += mpn_sub_n(v1, v1, c4, rr2);
#endif
  c4[0] = saved;
  if (rr2 < twok)
  {
	  v1[twok] -= mpn_sub_n(v1 + rr2, v1 + rr2, v0 + rr2, twok - rr2); 
	  MPN_DECR_U(v1 + rr2, kk1 - rr2, cy);
  }
  else v1[twok] -= cy;

  saved = c4[0];
  c4[0] = vinf0;
/* subtract 5*vinf from v2,
  */
  cy = mpn_submul_1 (v2, c4, rr2, CNST_LIMB(5));
  MPN_DECR_U (v2 + rr2, kk1 - rr2, cy);
  c4[0] = saved;

  /* v2 = (v2 - v1)/2 (exact)
  */
#ifdef HAVE_NATIVE_mpn_rsh1sub_n
  mpn_rsh1sub_n (v2, v2, v1, kk1);
#else
  mpn_sub_n (v2, v2, v1, kk1);
  mpn_half (v2, kk1);
#endif

  /* v1 = v1 - vm1
  */
  mpn_sub_n(v1, v1, vm1, kk1);

  /* vm1 = vm1 - v2 and add vm1 in {c+k, ...} */
#if HAVE_NATIVE_mpn_addsub_n
  cy = mpn_addsub_n(c1, c1, vm1, v2, kk1);
#else
  mpn_sub_n(vm1, vm1, v2, kk1);
  cy = mpn_add_n (c1, c1, vm1, kk1);
#endif
  ASSERT_NOCARRY (mpn_add_1(c3 + 1, c3 + 1, rr2 + k - 1, cy)); /* 4k+rr2-(3k+1) = rr2+k-1 */

  /* don't forget to add vinf0 in {c+4k, ...} */
  ASSERT_NOCARRY (mpn_add_1(c4, c4, rr2, vinf0));

  /* add v2 in {c+3k, ...} */
  if (rr2 <= k + 1)
     ASSERT_NOCARRY (mpn_add_n (c3, c3, v2, k+rr2));
  else
  {
	  cy = mpn_add_n (c3, c3, v2, kk1);
     if (cy) ASSERT_NOCARRY (mpn_add_1(c5 + 1, c5 + 1, rr2 - k - 1, cy)); /* 4k+rr2-(5k+1) = rr2-k-1 */
  }

#undef v0
}

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

/* 
   This code is based on mpn_toom3_mul_n. Here a and b may not have the same size, but an >= bn.

  The algorithm is the following:

  0. k = ceil(an/3), r = an - 2k, r2 = bn - 2k,  B = 2^(GMP_NUMB_BITS), t = B^k
  1. split a and b in three parts each a0, a1, a2 and b0, b1, b2
     with a0, a1, b0, b1 of k limbs, and a2, b2 of r, r2 limbs, respectively
  2. v0   <- a0*b0
     v1   <- (a0+a1+a2)*(b0+b1+b2)
     v2   <- (a0+2*a1+4*a2)*(b0+2*b1+4*b2)
     vm1  <- (a0-a1+a2)*(b0-b1+b2)
     vinf <- a2*b2

  As with mpn_toom3_mul_n we do our best to save space. 

  In mpn_toom3_interpolate we are going to do:

     t1   <- (3*v0+2*vm1+v2)/6-2*vinf
     t2   <- (v1+vm1)/2
  then the result is c0+c1*t+c2*t^2+c3*t^3+c4*t^4 where
     c0   <- v0
     c1   <- v1 - t1
     c2   <- t2 - v0 - vinf
     c3   <- t1 - t2
     c4   <- vinf

  In particular we know that v0 will be 2k limbs, and vinf will be r+r2 limbs.
  We may as well stick those into place immediately. That leaves 2k limbs between,
  which we can store v1 in (though we have to preserve the bottom limb of vinf
  which will get overwritten - we pass this to the interpolate function).

  We don't have as much space to play with as in mpn_toom3_mul_n as an may be 3k-2
  and bn may be as little as 2k + 1. Thus the output may have as little as 5k-1 limbs
  of space.

  We'll pass v2 and vm1 to the interpolate function in temporary space.

  Temporary space needed is t(na) = 4k+4 + t(k+1) where k = (na + 2)/3
  i.e t(na) = (4na + 20)/3 + t((na+5)/3) <= 5na/3 + t(5na/12) 
                                         <= (5na/3)/(1-5/12) = 60na/21 < 3na
  for na >= 20.
 */

void
mpn_toom3_mul (mp_ptr c, mp_srcptr a, mp_size_t an, mp_srcptr b, mp_size_t bn, mp_ptr t)
{
  mp_size_t k, k1, kk1, r, r2, twok, rr2;
  mp_limb_t cy, cc, saved, vinf0, c20, c21;
  mp_ptr trec;
  int sa, sb;
  mp_ptr c1, c2, c3, c4, t1, t2, t3, t4;

  ASSERT(GMP_NUMB_BITS >= 6);

  k = (an + 2) / 3; /* ceil(an/3) */
  ASSERT(bn > 2*k);
  ASSERT(an >= 20);
  
  twok = 2 * k;
  k1 = k + 1;
  kk1 = k + k1;
  r = an - twok;   /* last chunk */
  r2 = bn - twok;   /* last chunk */
  rr2 = r + r2;

  c1 = c + k;
  c2 = c1 + k;
  c3 = c2 + k;
  c4 = c3 + k;
  
  t1 = t + k;
  t2 = t1 + k;
  t3 = t2 + k;
  t4 = t3 + k;

  trec = t + 4 * k + 4; 

  /* put a0+a2 in {t, k+1}, and b0+b2 in {t1 + 1, k+1};
     put a0+a1+a2 in {t2 + 2, k+1} and b0+b1+b2 in {t3 + 3,k+1}
  */
  cy = mpn_add_n (t, a, a + twok, r);
  cc = mpn_add_n (t1 + 1, b, b + twok, r2);
  if (r < k)
    {
      __GMPN_ADD_1 (cy, t + r, a + r, k - r, cy);
    }
  if (r2 < k)
    {
	  __GMPN_ADD_1 (cc, t1 + 1 + r2, b + r2, k - r2, cc);
    }
  t3[2] = (t1[0] = cy) + mpn_add_n (t2 + 2, t, a + k, k);
  t4[3] = (t2[1] = cc) + mpn_add_n (t3 + 3, t1 + 1, b + k, k);

  /* compute v1 := (a0+a1+a2)*(b0+b1+b2) in {c2, 2k+1};
     since v1 < 9*B^(2k), v1 uses only 2k+1 words if GMP_NUMB_BITS >= 4 */
  TOOM3_MUL_REC (c2, t2 + 2, t3 + 3, k1, trec);

  /* {c,2k} {c+2k,2k+1} {c+4k+1,r+r2-1} 
					v1
  */

  /* put |a0-a1+a2| in {t2 + 2, k+1} and |b0-b1+b2| in {t3 + 3,k+1} */
  /* sa = sign(a0-a1+a2) */
  /* sb = sign(b0-b1+b2) */
  sa = (t[k] != 0) ? 1 : mpn_cmp (t, a + k, k);
  t3[2] = (sa >= 0) ? t[k] - mpn_sub_n (t2 + 2, t, a + k, k)
		   : mpn_sub_n (t2 + 2, a + k, t, k);
  /* b0+b2 is in {c+k+1, k+1} now */
  sb = (t2[1] != 0) ? 1 : mpn_cmp (t1 + 1, b + k, k);
  t4[3] = (sb >= 0) ? t2[1] - mpn_sub_n (t3 + 3, t1 + 1, b + k, k)
		    : mpn_sub_n (t3 + 3, b + k, t1 + 1, k);
  sa *= sb; /* sign of vm1 */

  /* compute vm1 := (a0-a1+a2)*(b0-b1+b2) in {t, 2k+1};
     since |vm1| < 4*B^(2k), vm1 uses only 2k+1 limbs */
  TOOM3_MUL_REC (t, t2 + 2, t3 + 3, k1, trec);

  /* {c,2k} {c+2k,2k+1} {c+4k+1,r+r2-1} 
					v1

	  {t, 2k+1} {t+2k+1, 2k + 1}
	     vm1
  */

  c20 = c2[0]; /* save c2[0] and c2[1] giving space 2k+2 at c */
  c21 = c2[1];

  /* 
     compute a0+2a1+4a2 in {c, k+1} and b0+2b1+4b2 in {c1 + 1, k+1}
  */
#if HAVE_NATIVE_mpn_addlsh1_n
  c1[0] = mpn_addlsh1_n (c, a + k, a + twok, r);
  c2[1] = mpn_addlsh1_n (c1 + 1, b + k, b + twok, r2);
  if (r < k)
    {
      __GMPN_ADD_1 (c1[0], c + r, a + k + r, k - r, c1[0]);
    }
  if (r2 < k)
    {__GMPN_ADD_1 (c2[1], c1 + 1 + r2, b + k + r2, k - r2, c2[1]);
    }
  c1[0] = 2 * c1[0] + mpn_addlsh1_n (c, a, c, k);
  c2[1] = 2 * c2[1] + mpn_addlsh1_n (c1 + 1, b, c1 + 1, k);
#else
  c[r] = mpn_lshift1 (c, a + twok, r);
  c1[r2 + 1] = mpn_lshift1 (c1 + 1, b + twok, r2);
  if (r  < k)
    {
      MPN_ZERO(c + r + 1, k - r);
    }
  if (r2 < k)
    {
      MPN_ZERO(c1 + r2 + 2, k - r2);
    }
  c1[0] += mpn_add_n (c, c, a + k, k);
  c2[1] += mpn_add_n (c1 + 1, c1 + 1, b + k, k);
  mpn_double (c, k1);
  mpn_double (c1 + 1, k1);
  c1[0] += mpn_add_n (c, c, a, k);
  c2[1] += mpn_add_n (c1 + 1, c1 + 1, b, k);
#endif

#define v2 (t+2*k+1)

  /* compute v2 := (a0+2a1+4a2)*(b0+2b1+4b2) in {t+2k+1, 2k+1}
     v2 < 49*B^k so v2 uses at most 2k+1 limbs if GMP_NUMB_BITS >= 6 */
  TOOM3_MUL_REC (v2, c, c1 + 1, k1, trec);

  /* {c,2k} {c+2k,2k+1} {c+4k+1,r+r2-1} 
					v1

	  {t, 2k+1} {t+2k+1, 2k + 1}
	     vm1        v2
  */

  c2[0] = c20; /* restore c2[0] and c2[1] */
  c2[1] = c21;

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

  if (r == r2) TOOM3_MUL_REC (c4, a + twok, b + twok, r, trec);
  else if (r > r2) mpn_mul(c4, a + twok, r, b + twok, r2);
  else mpn_mul(c4, b + twok, r2, a + twok, r);

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
mpn_toom42_mul (mp_ptr c, mp_srcptr a, mp_size_t an, 
                                      mp_srcptr b, mp_size_t bn, mp_ptr t)
{
  mp_size_t k, k1, kk1, r, r2, twok, threek, rr2, n1, n2;
  mp_limb_t cy, cc, saved, vinf0, c20, c21;
  mp_ptr trec;
  int sa, sb;
  mp_ptr c1, c2, c3, c4, t1, t2, t3, t4;

  ASSERT(GMP_NUMB_BITS >= 6);

  k = (an + 3) / 4; /* ceil(an/4) */
  ASSERT(bn > k);
  ASSERT(bn <= 2*k);
  ASSERT(an >= 20);
  
  twok = 2 * k;
  threek = 3 * k;
  k1 = k + 1;
  kk1 = k + k1;
  r = an - threek;   /* last chunk */
  r2 = bn - k;   /* last chunk */
  rr2 = r + r2;

  c1 = c + k;
  c2 = c1 + k;
  c3 = c2 + k;
  c4 = c3 + k;
  
  t1 = t + k;
  t2 = t1 + k;
  t3 = t2 + k;
  t4 = t3 + k;

  trec = t + 4 * k + 4; 

  /* put a0+a2 in {t, k+1}, and b0+b1 in {t1 + 1, k+1};
     put a1+a3 in {t3+3, k+1}, put a0+a1+a2+a3 in {t2 + 2, k+1} 
  */
  t[k] = mpn_add_n (t, a, a + twok, k);
  
  t4[3] = mpn_add_n (t3 + 3, a + k, a + threek, r);
  if (k > r)
     t4[3] = mpn_add_1(t3 + r + 3, a + k + r, k - r, t4[3]);

  mpn_add_n (t2 + 2, t, t3 + 3, k1);

  t2[1] = mpn_add_n (t1 + 1, b, b + k, r2);
  if (k > r2)
	  t2[1] = mpn_add_1(t1 + 1 + r2, b + r2, k - r2, t2[1]);

  /* compute v1 := (a0+a1+a2+a3)*(b0+b1) in {c2, 2k+1};
     since v1 < 6*B^(2k), v1 uses only 2k+1 words if GMP_NUMB_BITS >= 3 */
  TOOM3_MUL_REC (c2, t2 + 2, t1 + 1, k1, trec);

  ASSERT(c2[k+k] < 6);

  /* {c,2k} {c+2k,2k+1} {c+4k+1,r+r2-1} 
					v1
  */

  /* put |a0-a1+a2-a3| in {t2 + 2, k+1} and |b0-b1| in {t3 + 3,k+1} */
  /* sa = sign(a0-a1+a2-a3) */
  /* sb = sign(b0-b1) */
  sa = mpn_cmp (t, t3 + 3, k1);
  if (sa >= 0) mpn_sub_n (t2 + 2, t, t3 + 3, k1);
  else mpn_sub_n (t2 + 2, t3 + 3, t, k1);

  n1 = k;
  n2 = r2;
  MPN_NORMALIZE(b, n1);
  MPN_NORMALIZE(b+k, n2);
  if (n1 != n2) sb = (n1 > n2) ? 1 : -1;
  else sb = mpn_cmp (b, b + k, n2);

  if (sb >= 0) 
  {
	  t4[3] = mpn_sub_n (t3 + 3, b, b + k, r2);
	  if (k > r2) t4[3] = -mpn_sub_1(t3 + 3 + r2, b + r2, k - r2, t4[3]);
  } else
  {
	  mpn_sub_n (t3 + 3, b + k, b, r2);
	  MPN_ZERO(t3 + r2 + 3, k1 - r2);
  }
  sa *= sb; /* sign of vm1 */

  /* compute vm1 := (a0-a1+a2-a3)*(b0-b1) in {t, 2k+1};
     since |vm1| < 2*B^(2k), vm1 uses only 2k+1 limbs */
  TOOM3_MUL_REC (t, t2 + 2, t3 + 3, k1, trec);

  ASSERT(t[k+k] < 2);
  /* {c,2k} {c+2k,2k+1} {c+4k+1,r+r2-1} 
					v1

	  {t, 2k+1} {t+2k+1, 2k + 1}
	     vm1
  */

  c20 = c2[0]; /* save c2[0] and c2[1] giving space 2k+2 at c */
  c21 = c2[1];

  /* 
     compute a0+2a1+4a2+8a3 in {c, k+1} and b0+2b1 in {c1 + 1, k+1}
  */
#if HAVE_NATIVE_mpn_addlsh1_n
  c1[0] = mpn_addlsh1_n (c, a + twok, a + threek, r);
  if (r < k)
    {
      c1[0] = mpn_add_1 (c + r, a + twok + r, k - r, c1[0]);
    }
  c1[0] = 2 * c1[0] + mpn_addlsh1_n (c, a + k, c, k);
  c1[0] = 2 * c1[0] + mpn_addlsh1_n (c, a, c, k);
  
  c2[1] = mpn_addlsh1_n (c1 + 1, b, b + k, r2);
  if (r2 < k)
    {
		 c2[1] = mpn_add_1(c1 + 1 + r2, b + r2, k - r2, c2[1]);
    }
#else
  c[r] = mpn_lshift1 (c, a + threek, r);
  if (r < k)
    {
      MPN_ZERO(c + r + 1, k - r);
    }
  c1[0] += mpn_add_n (c, c, a + twok, k);
  mpn_double (c, k1);
  c1[0] += mpn_add_n (c, c, a + k, k);
  mpn_double (c, k1);
  c1[0] += mpn_add_n (c, c, a, k);
  
  c1[r2 + 1] = mpn_lshift1 (c1 + 1, b + k, r2);
  if (r2 < k)
    {
      MPN_ZERO(c1 + r2 + 2, k - r2);
    }
  c2[1] += mpn_add_n (c1 + 1, c1 + 1, b, k);
#endif

#define v2 (t+2*k+1)

  /* compute v2 := (a0+2a1+4a2+8a3)*(b0+2b1) in {t+2k+1, 2k+1}
     v2 < 45*B^k so v2 uses at most 2k+1 limbs if GMP_NUMB_BITS >= 6 */
  TOOM3_MUL_REC (v2, c, c1 + 1, k1, trec);

  ASSERT(v2[k+k] < 45);
  /* {c,2k} {c+2k,2k+1} {c+4k+1,r+r2-1} 
					v1

	  {t, 2k+1} {t+2k+1, 2k + 1}
	     vm1        v2
  */

  c2[0] = c20; /* restore c2[0] and c2[1] */
  c2[1] = c21;

  /* compute v0 := a0*b0 in {c, 2k} */
  TOOM3_MUL_REC (c, a, b, k, trec);

 /* {c,2k} {c+2k,2k+1} {c+4k+1,r+r2-1} 
		v0 		v1

	  {t, 2k+1} {t+2k+1, 2k + 1}
	     vm1        v2
  */

#define vinf (c+4*k)

  /* compute vinf := a3*b1 in {c4, r + r2},
  */
  saved = c4[0];

  if (r == r2) TOOM3_MUL_REC (c4, a + threek, b + k, r, trec);
  else if (r > r2) mpn_mul(c4, a + threek, r, b + k, r2);
  else mpn_mul(c4, b + k, r2, a + threek, r);

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

/*
   We have a 3x2 blocked multiplication and therefore the output is of length 
   4 blocks. Therefore we evaluate at the 4 points 0, inf, -1, 1, i.e. we need
	(a0*b0), (a2*b1), (a0-a1+a2)*(b0-b1), (a0+a1+a2)*(b0+b1).
	The multiplication will be (2k+r) x (k + r2) and therefore the output has
	space for 3k + rr2 limbs.
*/
void
mpn_toom32_mul (mp_ptr c, mp_srcptr a, mp_size_t an, 
                                           mp_srcptr b, mp_size_t bn, mp_ptr t)
{
  mp_size_t k, k1, kk1, r, r2, twok, threek, rr2, n1, n2;
  mp_limb_t cy, cc, saved;
  mp_ptr trec;
  int sa, sb;
  mp_ptr c1, c2, c3, c4, c5, t1, t2, t3, t4;

  ASSERT(GMP_NUMB_BITS >= 6);

  k = (an + 2) / 3; /* ceil(an/3) */
  ASSERT(bn > k);
  ASSERT(an >= 20);
  
  twok = 2 * k;
  threek = 3 * k;
  k1 = k + 1;
  kk1 = k + k1;
  r = an - twok;   /* last chunk */
  r2 = bn - k;   /* last chunk */
  rr2 = r + r2;

  c1 = c + k;
  c2 = c1 + k;
  c3 = c2 + k;
  c4 = c3 + k;
  c5 = c4 + k;
  
  t1 = t + k;
  t2 = t1 + k;
  t3 = t2 + k;
  t4 = t3 + k;

  trec = t + 3 * k + 3; 

  /* put a0+a2 in {t, k+1}, and b0+b1 in {t2 + 2, k+1};
     put a0+a1+a2 in {t1 + 1, k+1}
  */
  cy = mpn_add_n (t, a, a + twok, r);
  t3[2] = mpn_add_n (t2 + 2, b, b + k, r2);
  if (r < k)
    {
      cy = mpn_add_1 (t + r, a + r, k - r, cy);
    }
  if (r2 < k)
    {
	  t3[2] = mpn_add_1 (t2 + 2 + r2, b + r2, k - r2, t3[2]);
    }
  t2[1] = (t1[0] = cy) + mpn_add_n (t1 + 1, t, a + k, k);
  
  /* compute v1 := (a0+a1+a2)*(b0+b1) in {c1, 2k+1};
     since v1 < 6*B^(2k), v1 uses only 2k+1 words if GMP_NUMB_BITS >= 3 */
  TOOM3_MUL_REC (c1, t1 + 1, t2 + 2, k1, trec);

  saved = c1[0];

  /* {c,2k} {c+2k,2k+1} {c+4k+1,r+r2-1} 
					v1
  */

  /* put |a0-a1+a2| in {c0, k+1} and |b0-b1| in {t2 + 2,k+1} */
  /* sa = sign(a0-a1+a2) */
  /* sb = sign(b0-b1) */
  sa = (t[k] != 0) ? 1 : mpn_cmp (t, a + k, k);
  if (sa >= 0) c[k] = t[k] - mpn_sub_n (c, t, a + k, k);
  else c[k] = -mpn_sub_n (c, a + k, t, k);
  
  n1 = k;
  n2 = r2;
  MPN_NORMALIZE(b, n1);
  MPN_NORMALIZE(b+k, n2);
  if (n1 != n2) sb = (n1 > n2) ? 1 : -1;
  else sb = mpn_cmp (b, b + k, n2);

  if (sb >= 0) 
  {
	  t3[2] = mpn_sub_n (t2 + 2, b, b + k, r2);
	  if (k > r2) t3[2] = -mpn_sub_1(t2 + 2 + r2, b + r2, k - r2, t3[2]);
  } else
  {
	  mpn_sub_n (t2 + 2, b + k, b, r2);
	  MPN_ZERO(t2 + r2 + 2, k1 - r2);
  }
  
  sa *= sb; /* sign of vm1 */

  /* compute vm1 := (a0-a1+a2)*(b0-b1) in {t, 2k+1};
     since |vm1| < 2*B^(2k), vm1 uses only 2k+1 limbs */
  TOOM3_MUL_REC (t, t2 + 2, c, k1, trec);

  /* {c,2k} {c+2k,2k+1} {c+4k+1,r+r2-1} 
					v1

	  {t, 2k+1} {t+2k+1, 2k + 1}
	     vm1
  */

  c1[0] = saved; 


  /* {c,k} {c+k,2k+1} {c+3k+1,r+r2-1} 
					v1

	  {t, 2k+1} {t+2k+1, 2k + 2}
	     vm1        
  */

  /* Compute vm1 <-- (vm1 + v1)/2 (note vm1 + v1 is positive) */

  if (sa > 0)
  {
#if HAVE_NATIVE_mpn_rsh1add_n
     mpn_rsh1add_n(t, t, c1, kk1);
#else
	  mpn_add_n(t, t, c1, kk1);
	  mpn_half(t, kk1);
#endif
  } else
  {
#if HAVE_NATIVE_mpn_rsh1sub_n
	  mpn_rsh1sub_n(t, c1, t, kk1);
#else
	  mpn_sub_n(t, c1, t, kk1);
     mpn_half(t, kk1);
#endif
  }
  
  /* Compute v1 <-- v1 - vm1 */

  mpn_sub_n(c1, c1, t, kk1);

  /* Note we could technically overflow
     the end of the output if we add
	  everything in place without subtracting
	  the right things first. We get around 
	  this by throwing away any high limbs
	  and carries, which must of necessity
	  cancel.

	  First we add vm1 in its place...
  */

  n1 = kk1;
  MPN_NORMALIZE(t, n1);

  if (n1 >= k + rr2) /* if > here, high limb of vm1 and carry may be discarded */
  { 
	  cy = mpn_add_n(c2, c2, t, k1);
	  mpn_add_1(c3 + 1, t + k1, rr2 - 1, cy);
	  n2 = threek + rr2;
  } else 
  {
	  c2[k1] = mpn_add_n(c2, c2, t, k1);
     if (n1 > k1) c2[n1] = mpn_add_1(c3 + 1, t + k1, n1 - k1, c2[k1]); 
	  n2 = twok + MAX(n1, k1) + 1;
  } 

  /* Compute vinf := a2*b1 in {t, rr2} */

  if (r == r2) TOOM3_MUL_REC (t, a + twok, b + k, r, trec);
  else if (r > r2) mpn_mul(t, a + twok, r, b + k, r2);
  else mpn_mul(t, b + k, r2, a + twok, r);

  /* Add vinf into place */

  cy = mpn_add_n(c3, c3, t, n2 - threek);
  if (rr2 + threek > n2) 
	  mpn_add_1(c + n2, t + n2 - threek, rr2 + threek - n2, cy);

  /* v1 <-- v1 - vinf */

  cy = mpn_sub_n(c1, c1, t, rr2);
  if (cy) mpn_sub_1(c1 + rr2, c1 + rr2, twok, cy);
  
  /* compute v0 := a0*b0 in {t, 2k} */

  TOOM3_MUL_REC (t, a, b, k, trec);

  /* Add v0 into place */

  MPN_COPY(c, t, k);
  cy = mpn_add_n(c + k, c + k, t + k, k);
  if (cy) mpn_add_1(c + twok, c + twok, k + rr2, cy);

  /* vm1 <-- vm1 - v0 */

  if (twok >= k + rr2)
     mpn_sub_n(c2, c2, t, k + rr2);
  else
  {
	  cy = mpn_sub_n(c2, c2, t, twok);
     mpn_sub_1(c4, c4, rr2 + k - twok, cy);
  }
}

