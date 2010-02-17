/* Implementation of the multiplication algorithm for Toom-Cook 8.5-way.

   Contributed to the GNU project by Marco Bodrato.

   THE FUNCTION IN THIS FILE IS INTERNAL WITH A MUTABLE INTERFACE.  IT IS ONLY
   SAFE TO REACH IT THROUGH DOCUMENTED INTERFACES.  IN FACT, IT IS ALMOST
   GUARANTEED THAT IT WILL CHANGE OR DISAPPEAR IN A FUTURE GNU MP RELEASE.

Copyright 2009, 2010 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library.  If not, see http://www.gnu.org/licenses/.  */


#include "mpir.h"
#include "gmp-impl.h"


#if GMP_NUMB_BITS < 29
#error Not implemented.
#endif

#if GMP_NUMB_BITS < 43
#define BIT_CORRECTION 1
#define CORRECTION_BITS GMP_NUMB_BITS
#else
#define BIT_CORRECTION 0
#define CORRECTION_BITS 0
#endif

#define TOOM8H_MUL_N_REC(p, a, b, n)				\
  do {									\
    if (BELOW_THRESHOLD (n, MUL_TOOM8H_THRESHOLD))		\
      mpn_mul_n (p, a, b, n);				\
    else								\
      mpn_toom8h_mul (p, a, n, b, n);				\
  } while (0)

#define TOOM8H_MUL_REC(p, a, na, b, nb)		\
  do {	mpn_mul (p, a, na, b, nb);			\
  } while (0)

/* Toom-8.5 , compute the product {pp,an+bn} <- {ap,an} * {bp,bn}
   With: an >= bn >= 86, an*5 <  bn * 11.
   It _may_ work with bn<=?? and bn*?? < an*? < bn*??

   Evaluate in: infinity, +8,-8,+4,-4,+2,-2,+1,-1,+1/2,-1/2,+1/4,-1/4,+1/8,-1/8,0.
*/
/* Estimate on needed scratch:
   S(n) <= (n+7)\8*13+5+MAX(S((n+7)\8),1+2*(n+7)\8),
   since n>80; S(n) <= ceil(log(n/10)/log(8))*(13+5)+n*15\8 < n*15\8 + lg2(n)*6
 */

void
mpn_toom8h_mul   (mp_ptr pp,
		  mp_srcptr ap, mp_size_t an,
		  mp_srcptr bp, mp_size_t bn)
{
  mp_size_t n, s, t;
  int p, q, half;
  int sign;
  mp_ptr scratch;

  TMP_DECL;

  TMP_MARK;

  /***************************** decomposition *******************************/

  ASSERT (an >= bn);
  /* Can not handle too small operands */
  ASSERT (bn >= 86);
  /* Can not handle too much unbalancement */
  ASSERT (an*4 <= bn*13);
  ASSERT (GMP_NUMB_BITS > 12*3 || an*4 <= bn*12);
  ASSERT (GMP_NUMB_BITS > 11*3 || an*5 <= bn*11);
  ASSERT (GMP_NUMB_BITS > 10*3 || an*6 <= bn*10);
  ASSERT (GMP_NUMB_BITS >  9*3 || an*7 <= bn* 9);

  /* Limit num/den is a rational number between
     (16/15)^(log(6)/log(2*6-1)) and (16/15)^(log(8)/log(2*8-1))             */
#define LIMIT_numerator (21)
#define LIMIT_denominat (20)

  if (LIKELY (an == bn) || an * (LIMIT_denominat>>1) < LIMIT_numerator * (bn>>1) ) /* is 8*... < 8*... */
    {
      half = 0;
      n = 1 + ((an - 1)>>3);
      p = q = 7;
      s = an - p * n;
      t = bn - q * n;
    }
  else
    {
      if (an * 13 < 16 * bn) /* (an*7*LIMIT_numerator<LIMIT_denominat*9*bn) */
	{ p = 9; q = 8; }
      else if (GMP_NUMB_BITS <= 9*3 ||
	       an *(LIMIT_denominat>>1) < (LIMIT_numerator/7*9) * (bn>>1))
	{ p = 9; q = 7; }
      else if (an * 10 < 33 * (bn>>1)) /* (an*3*LIMIT_numerator<LIMIT_denominat*5*bn) */
	{ p =10; q = 7; }
      else if (GMP_NUMB_BITS <= 10*3 ||
	       an * (LIMIT_denominat/5) < (LIMIT_numerator/3) * bn)
	{ p =10; q = 6; }
      else if (an * 6 < 13 * bn) /*(an * 5 * LIMIT_numerator < LIMIT_denominat *11 * bn)*/
	{ p =11; q = 6; }
      else if (GMP_NUMB_BITS <= 11*3 ||
	       an * 4 < 9 * bn)
	{ p =11; q = 5; }
      else if (an *(LIMIT_numerator/3) < LIMIT_denominat * bn )  /* is 4*... <12*... */
	{ p =12; q = 5; }
      else if (GMP_NUMB_BITS <= 12*3 ||
	       an * 9 < 28 * bn )  /* is 4*... <12*... */
	{ p =12; q = 4; }
      else
	{ p =13; q = 4; }

      half = (p+q)&1;
      n = 1 + (q * an >= p * bn ? (an - 1) / (size_t) p : (bn - 1) / (size_t) q);
      p--; q--;

      s = an - p * n;
      t = bn - q * n;

      if(half) { /* Recover from badly chosen splitting */
	if (s<1) {p--; s+=n; half=0;}
	else if (t<1) {q--; t+=n; half=0;}
      }
    }
#undef LIMIT_numerator
#undef LIMIT_denominat

  ASSERT (0 < s && s <= n);
  ASSERT (0 < t && t <= n);
  ASSERT (half || s + t > 3);
  ASSERT (n > 2);

  scratch = TMP_ALLOC_LIMBS(n*15 + GMP_LIMB_BITS*6);
  
#define   r6    (pp + 3 * n)			/* 3n+1 */
#define   r4    (pp + 7 * n)			/* 3n+1 */
#define   r2    (pp +11 * n)			/* 3n+1 */
#define   r0    (pp +15 * n)			/* s+t <= 2*n */
#define   r7    (scratch)			/* 3n+1 */
#define   r5    (scratch + 3 * n + 1)		/* 3n+1 */
#define   r3    (scratch + 6 * n + 2)		/* 3n+1 */
#define   r1    (scratch + 9 * n + 3)		/* 3n+1 */
#define   v0    (pp +11 * n)			/* n+1 */
#define   v1    (pp +12 * n+1)			/* n+1 */
#define   v2    (pp +13 * n+2)			/* n+1 */
#define   v3    (scratch +12 * n + 4)		/* n+1 */
#define   wsi   (scratch +12 * n + 4)		/* 3n+1 */

  /********************** evaluation and recursive calls *********************/

  /* $\pm1/8$ */
  sign = mpn_toom_eval_pm2rexp (v2, v0, p, ap, n, s, 3, pp) ^
	 mpn_toom_eval_pm2rexp (v3, v1, q, bp, n, t, 3, pp);
  TOOM8H_MUL_N_REC(pp, v0, v1, n + 1); /* A(-1/8)*B(-1/8)*8^. */
  TOOM8H_MUL_N_REC(r7, v2, v3, n + 1); /* A(+1/8)*B(+1/8)*8^. */
  mpn_toom_couple_handling (r7, 2 * n + 1 + BIT_CORRECTION, pp, sign, n, 3*(1+half), 3*(half));

  /* $\pm1/4$ */
  sign = mpn_toom_eval_pm2rexp (v2, v0, p, ap, n, s, 2, pp) ^
	 mpn_toom_eval_pm2rexp (v3, v1, q, bp, n, t, 2, pp);
  TOOM8H_MUL_N_REC(pp, v0, v1, n + 1); /* A(-1/4)*B(-1/4)*4^. */
  TOOM8H_MUL_N_REC(r5, v2, v3, n + 1); /* A(+1/4)*B(+1/4)*4^. */
  mpn_toom_couple_handling (r5, 2 * n + 1, pp, sign, n, 2*(1+half), 2*(half));

  /* $\pm2$ */
  sign = mpn_toom_eval_pm2 (v2, v0, p, ap, n, s, pp) ^
	 mpn_toom_eval_pm2 (v3, v1, q, bp, n, t, pp);
  TOOM8H_MUL_N_REC(pp, v0, v1, n + 1); /* A(-2)*B(-2) */
  TOOM8H_MUL_N_REC(r3, v2, v3, n + 1); /* A(+2)*B(+2) */
  mpn_toom_couple_handling (r3, 2 * n + 1, pp, sign, n, 1, 2);

  /* $\pm8$ */
  sign = mpn_toom_eval_pm2exp (v2, v0, p, ap, n, s, 3, pp) ^
	 mpn_toom_eval_pm2exp (v3, v1, q, bp, n, t, 3, pp);
  TOOM8H_MUL_N_REC(pp, v0, v1, n + 1); /* A(-8)*B(-8) */
  TOOM8H_MUL_N_REC(r1, v2, v3, n + 1); /* A(+8)*B(+8) */
  mpn_toom_couple_handling (r1, 2 * n + 1 + BIT_CORRECTION, pp, sign, n, 3, 6);

  /* $\pm1/2$ */
  sign = mpn_toom_eval_pm2rexp (v2, v0, p, ap, n, s, 1, pp) ^
	 mpn_toom_eval_pm2rexp (v3, v1, q, bp, n, t, 1, pp);
  TOOM8H_MUL_N_REC(pp, v0, v1, n + 1); /* A(-1/2)*B(-1/2)*2^. */
  TOOM8H_MUL_N_REC(r6, v2, v3, n + 1); /* A(+1/2)*B(+1/2)*2^. */
  mpn_toom_couple_handling (r6, 2 * n + 1, pp, sign, n, 1+half, half);

  /* $\pm1$ */
  sign = mpn_toom_eval_pm1 (v2, v0, p, ap, n, s, pp);
  if (q == 3)
    sign ^= mpn_toom_eval_dgr3_pm1 (v3, v1, bp, n, t, pp);
  else
    sign ^= mpn_toom_eval_pm1 (v3, v1, q, bp, n, t,    pp);
  TOOM8H_MUL_N_REC(pp, v0, v1, n + 1); /* A(-1)*B(-1) */
  TOOM8H_MUL_N_REC(r4, v2, v3, n + 1); /* A(1)*B(1) */
  mpn_toom_couple_handling (r4, 2 * n + 1, pp, sign, n, 0, 0);

  /* $\pm4$ */
  sign = mpn_toom_eval_pm2exp (v2, v0, p, ap, n, s, 2, pp) ^
	 mpn_toom_eval_pm2exp (v3, v1, q, bp, n, t, 2, pp);
  TOOM8H_MUL_N_REC(pp, v0, v1, n + 1); /* A(-4)*B(-4) */
  TOOM8H_MUL_N_REC(r2, v2, v3, n + 1); /* A(+4)*B(+4) */
  mpn_toom_couple_handling (r2, 2 * n + 1, pp, sign, n, 2, 4);

#undef v0
#undef v1
#undef v2
#undef v3

  /* A(0)*B(0) */
  TOOM8H_MUL_N_REC(pp, ap, bp, n);

  /* Infinity */
  if( half != 0) {
    if(s>t) {
      TOOM8H_MUL_REC(r0, ap + p * n, s, bp + q * n, t);
    } else {
      TOOM8H_MUL_REC(r0, bp + q * n, t, ap + p * n, s);
    };
  };

  mpn_toom_interpolate_16pts (pp, r1, r3, r5, r7, n, s+t, half, wsi);

  TMP_FREE;

#undef r0
#undef r1
#undef r2
#undef r3
#undef r4
#undef r5
#undef r6
#undef wsi
}
