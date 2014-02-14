/* mpn_toom4_mul_n -- Internal routine to multiply two natural numbers
   of length n.

   THIS IS AN INTERNAL FUNCTION WITH A MUTABLE INTERFACE.  IT IS ONLY
   SAFE TO REACH THIS FUNCTION THROUGH DOCUMENTED INTERFACES.
*/

/* Implementation of the Bodrato-Zanoni algorithm for Toom-Cook 4-way.

Copyright 2001, 2002, 2004, 2005, 2006 Free Software Foundation, Inc.
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

/*
   This implementation is based on that of Paul Zimmmermann, which is available
	for mpz_t's at http://www.loria.fr/~zimmerma/software/toom4.c
*/

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"

void
mpn_toom4_mul_n (mp_ptr rp, mp_srcptr up,
		          mp_srcptr vp, mp_size_t n);

void _tc4_add(mp_ptr rp, mp_size_t * rn, mp_srcptr r1, mp_size_t r1n, 
                                                 mp_srcptr r2, mp_size_t r2n)
{
   mp_limb_t cy;
   mp_size_t s1 = ABS(r1n);
   mp_size_t s2 = ABS(r2n);
   
   if (!s1)
   {
      *rn = 0;
   } else if (!s2)
   {
      if (rp != r1) MPN_COPY(rp, r1, s1);
		*rn = r1n;
   } else if ((r1n ^ r2n) >= 0)
   {
      *rn = r1n;
      cy = mpn_add(rp, r1, s1, r2, s2);
      if (cy) 
      {
         rp[s1] = cy;
         if ((*rn) < 0) (*rn)--;
         else (*rn)++;
      }
   } else
   {
      mp_size_t ct;
		if (s1 != s2) ct = 1;
		else MPN_CMP(ct, r1, r2, s1); 
		    
      if (!ct) *rn = 0;
      else if (ct > 0) 
      {
         mpn_sub(rp, r1, s1, r2, s2);
         *rn = s1;
         MPN_NORMALIZE(rp, (*rn));
			if (r1n < 0) *rn = -(*rn);
      }
      else
      {
         mpn_sub_n(rp, r2, r1, s1);
         *rn = s1;
         MPN_NORMALIZE(rp, (*rn));
			if (r1n > 0) *rn = -(*rn);
      }
   }
}

void tc4_add(mp_ptr rp, mp_size_t * rn, mp_srcptr r1, mp_size_t r1n, 
                                                 mp_srcptr r2, mp_size_t r2n)
{
   mp_size_t s1 = ABS(r1n);
   mp_size_t s2 = ABS(r2n);
   
   if (s1 < s2) _tc4_add(rp, rn, r2, r2n, r1, r1n);
	else _tc4_add(rp, rn, r1, r1n, r2, r2n);
} 

void _tc4_add_unsigned(mp_ptr rp, mp_size_t * rn, mp_srcptr r1, 
                                   mp_size_t r1n, mp_srcptr r2, mp_size_t r2n)
{
   mp_limb_t cy;
   mp_size_t s1 = r1n;
   mp_size_t s2 = r2n;
   
   if (!s2)
   {
      if (!s1) *rn = 0;
      else
      {
         if (rp != r1) MPN_COPY(rp, r1, s1);
		   *rn = r1n;
		}
   } else
   {
      *rn = r1n;
      cy = mpn_add(rp, r1, s1, r2, s2);
      if (cy) 
      {
         rp[s1] = cy;
         if ((*rn) < 0) (*rn)--;
         else (*rn)++;
      }
   } 
}

void tc4_add_unsigned(mp_ptr rp, mp_size_t * rn, mp_srcptr r1, 
                                    mp_size_t r1n, mp_srcptr r2, mp_size_t r2n)
{
   if (r1n < r2n) _tc4_add_unsigned(rp, rn, r2, r2n, r1, r1n);
	else _tc4_add_unsigned(rp, rn, r1, r1n, r2, r2n);
} 

void tc4_sub(mp_ptr rp, mp_size_t * rn, mp_srcptr r1, mp_size_t r1n, 
                                                   mp_srcptr r2, mp_size_t r2n)
{
   tc4_add(rp, rn, r1, r1n, r2, -r2n);
}
	
void tc4_lshift(mp_ptr rp, mp_size_t * rn, mp_srcptr xp, 
                                                  mp_size_t xn, mp_size_t bits)
{
   if (xn == 0) *rn = 0;
   else
	{
		mp_size_t xu = ABS(xn);
		mp_limb_t msl = mpn_lshift(rp, xp, xu, bits);
      if (msl) 
		{
			rp[xu] = msl;
			*rn = (xn >= 0 ? xn + 1 : xn - 1);
		} else
		   *rn = xn;
	}
}

void tc4_rshift_inplace(mp_ptr rp, mp_size_t * rn, mp_size_t bits)
{
   if (*rn)
	{
		if ((*rn) > 0) 
		{
			mpn_rshift(rp, rp, *rn, bits);
	      if (rp[(*rn) - 1] == CNST_LIMB(0)) (*rn)--;
		} else 
		{
			mpn_rshift(rp, rp, -(*rn), bits);
	      if (rp[-(*rn) - 1] == CNST_LIMB(0)) (*rn)++;
		}
	}
}

void tc4_addlsh1_unsigned(mp_ptr rp, mp_size_t * rn, mp_srcptr xp, mp_size_t xn)
{
	if (xn)
	{
		if (xn >= *rn)
		{
            mp_limb_t cy;
			if (xn > *rn) MPN_ZERO(rp + *rn, xn - *rn);
#if HAVE_NATIVE_mpn_addlsh1_n
            cy = mpn_addlsh1_n(rp, rp, xp, xn);
#else
            cy = mpn_add_n(rp, rp, xp, xn);
            cy += mpn_add_n(rp, rp, xp, xn);
#endif
			if (cy) 
			{
				rp[xn] = cy;
				*rn = xn + 1;
			} else *rn = xn;
		} else
	   {
		   mp_limb_t cy;
#if HAVE_NATIVE_mpn_addlsh1_n
            cy = mpn_addlsh1_n(rp, rp, xp, xn);
#else
            cy = mpn_add_n(rp, rp, xp, xn);
            cy += mpn_add_n(rp, rp, xp, xn);
#endif
	      if (cy) cy = mpn_add_1(rp + xn, rp + xn, *rn - xn, cy);
		   if (cy) 
		   {
			   rp[*rn] = cy;
			   (*rn)++;
		   }
		}
	}
}

void tc4_divexact_ui(mp_ptr rp, mp_size_t * rn, mp_ptr x, mp_size_t xn, mp_limb_t c)
{
  mp_size_t abs_size;
  if (xn == 0)
    {
      *rn = 0;
      return;
    }
  abs_size = ABS (xn);

  MPN_DIVREM_OR_DIVEXACT_1 (rp, x, abs_size, c);
  abs_size -= (rp[abs_size-1] == 0);
  *rn = (xn >= 0 ? abs_size : -abs_size);
}

void tc4_divexact_by3(mp_ptr rp, mp_size_t * rn, mp_ptr x, mp_size_t xn)
{
	if (xn)
	{
		mp_size_t xu = ABS(xn);
		mpn_divexact_by3(rp, x, xu);
		if (xn > 0)
		{
			if (rp[xu - 1] == CNST_LIMB(0)) *rn = xn - 1;
			else *rn = xn;
		} else
		{
			if (rp[xu - 1] == CNST_LIMB(0)) *rn = xn + 1;
			else *rn = xn;
		}	
	} else *rn = 0;
}

void tc4_divexact_by15(mp_ptr rp, mp_size_t * rn, mp_ptr x, mp_size_t xn)
{
	if (xn)
	{
		mp_size_t xu = ABS(xn);
		mpn_divexact_byfobm1(rp, x, xu, CNST_LIMB(15), CNST_LIMB((~0)/15)); /* works for 32 and 64 bits */
		if (xn > 0)
		{
			if (rp[xu - 1] == CNST_LIMB(0)) *rn = xn - 1;
			else *rn = xn;
		} else
		{
			if (rp[xu - 1] == CNST_LIMB(0)) *rn = xn + 1;
			else *rn = xn;
		}	
	} else *rn = 0;
}

#if HAVE_NATIVE_mpn_mul_1c
#define MPN_MUL_1C(cout, dst, src, size, n, cin)        \
  do {                                                  \
    (cout) = mpn_mul_1c (dst, src, size, n, cin);       \
  } while (0)
#else
#define MPN_MUL_1C(cout, dst, src, size, n, cin)        \
  do {                                                  \
    mp_limb_t __cy;                                     \
    __cy = mpn_mul_1 (dst, src, size, n);               \
    (cout) = __cy + mpn_add_1 (dst, dst, size, cin);    \
  } while (0)
#endif

void tc4_addmul_1(mp_ptr wp, mp_size_t * wn, mp_srcptr xp, mp_size_t xn, mp_limb_t y)
{
  mp_size_t  sign, wu, xu, ws, new_wn, min_size, dsize;
  mp_limb_t  cy;

  /* w unaffected if x==0 or y==0 */
  if (xn == 0 || y == 0)
    return;

  sign = xn;
  xu = ABS (xn);

  ws = *wn;
  if (*wn == 0)
  {
      /* nothing to add to, just set x*y, "sign" gives the sign */
      cy = mpn_mul_1 (wp, xp, xu, y);
      if (cy)
		{
			wp[xu] = cy;
         xu = xu + 1;
		} 
      *wn = (sign >= 0 ? xu : -xu);
      return;
  }
  
  sign ^= *wn;
  wu = ABS (*wn);

  new_wn = MAX (wu, xu);
  min_size = MIN (wu, xu);

  if (sign >= 0)
  {
      /* addmul of absolute values */

      cy = mpn_addmul_1 (wp, xp, min_size, y);
      
      dsize = xu - wu;
#if HAVE_NATIVE_mpn_mul_1c
      if (dsize > 0)
        cy = mpn_mul_1c (wp + min_size, xp + min_size, dsize, y, cy);
      else if (dsize < 0)
      {
          dsize = -dsize;
          cy = mpn_add_1 (wp + min_size, wp + min_size, dsize, cy);
      }
#else
      if (dsize != 0)
      {
          mp_limb_t cy2;
          if (dsize > 0)
            cy2 = mpn_mul_1 (wp + min_size, xp + min_size, dsize, y);
          else
          {
              dsize = -dsize;
              cy2 = 0;
          }
          cy = cy2 + mpn_add_1 (wp + min_size, wp + min_size, dsize, cy);
      }
#endif

      if (cy)
		{
			wp[dsize + min_size] = cy;
         new_wn ++;
		}
   } else
   {
      /* submul of absolute values */

      cy = mpn_submul_1 (wp, xp, min_size, y);
      if (wu >= xu)
      {
          /* if w bigger than x, then propagate borrow through it */
          if (wu != xu)
            cy = mpn_sub_1 (wp + xu, wp + xu, wu - xu, cy);

          if (cy != 0)
          {
              /* Borrow out of w, take twos complement negative to get
                 absolute value, flip sign of w.  */
              wp[new_wn] = ~-cy;  /* extra limb is 0-cy */
              mpn_not (wp, new_wn);
              new_wn++;
              MPN_INCR_U (wp, new_wn, CNST_LIMB(1));
              ws = -*wn;
          }
      } else /* wu < xu */
      {
          /* x bigger than w, so want x*y-w.  Submul has given w-x*y, so
             take twos complement and use an mpn_mul_1 for the rest.  */

          mp_limb_t  cy2;

          /* -(-cy*b^n + w-x*y) = (cy-1)*b^n + ~(w-x*y) + 1 */
          mpn_not (wp, wu);
          cy += mpn_add_1 (wp, wp, wu, CNST_LIMB(1));
          cy -= 1;

          /* If cy-1 == -1 then hold that -1 for latter.  mpn_submul_1 never
             returns cy==MP_LIMB_T_MAX so that value always indicates a -1. */
          cy2 = (cy == MP_LIMB_T_MAX);
          cy += cy2;
          MPN_MUL_1C (cy, wp + wu, xp + wu, xu - wu, y, cy);
          wp[new_wn] = cy;
          new_wn += (cy != 0);

          /* Apply any -1 from above.  The value at wp+wsize is non-zero
             because y!=0 and the high limb of x will be non-zero.  */
          if (cy2)
            MPN_DECR_U (wp+wu, new_wn - wu, CNST_LIMB(1));

          ws = -*wn;
        }

      /* submul can produce high zero limbs due to cancellation, both when w
         has more limbs or x has more  */
      MPN_NORMALIZE (wp, new_wn);
  }

  *wn = (ws >= 0 ? new_wn : -new_wn);

  ASSERT (new_wn == 0 || wp[new_wn - 1] != 0);
}

void tc4_submul_1(mp_ptr wp, mp_size_t * wn, mp_srcptr x, mp_size_t xn, mp_limb_t y)
{
	tc4_addmul_1(wp, wn, x, -xn, y);
}

void tc4_copy (mp_ptr yp, mp_size_t * yn, mp_size_t offset, mp_srcptr xp, mp_size_t xn)
{
  mp_size_t yu = ABS(*yn);
  mp_size_t xu = ABS(xn);
  mp_limb_t cy = 0;

  if (xn == 0)
    return;

  if (offset < yu) /* low part of x overlaps with y */
  {
      if (offset + xu <= yu) /* x entirely inside y */
      {
          cy = mpn_add_n (yp + offset, yp + offset, xp, xu);
          if (offset + xu < yu)
            cy = mpn_add_1 (yp + offset + xu, yp + offset + xu,
                            yu - (offset + xu), cy);
      } else
        cy = mpn_add_n (yp + offset, yp + offset, xp, yu - offset);
      /* now cy is the carry at yp + yu */
      if (xu + offset > yu) /* high part of x exceeds y */
      {
          MPN_COPY (yp + yu, xp + yu - offset, xu + offset - yu);
          cy = mpn_add_1 (yp + yu, yp + yu, xu + offset - yu, cy);
          yu = xu + offset;
      }
      /* now cy is the carry at yp + yn */
      if (cy)
        yp[yu++] = cy;
      MPN_NORMALIZE(yp, yu);
      *yn = yu;
  } else /* x does not overlap */
  {
      if (offset > yu)
        MPN_ZERO (yp + yu, offset - yu);
      MPN_COPY (yp + offset, xp, xu);
      *yn = offset + xu;
  }
}

#define MUL_TC4_UNSIGNED(r3xx, n3xx, r1xx, n1xx, r2xx, n2xx) \
   do \
   { \
      if ((n1xx != 0) && (n2xx != 0)) \
      { mp_size_t len; \
	      if (n1xx == n2xx) \
		   { \
			   if (n1xx > MUL_TOOM4_THRESHOLD) mpn_toom4_mul_n(r3xx, r1xx, r2xx, n1xx); \
            else mpn_mul_n(r3xx, r1xx, r2xx, n1xx); \
		   } else if (n1xx > n2xx) \
		      mpn_mul(r3xx, r1xx, n1xx, r2xx, n2xx); \
		   else \
		      mpn_mul(r3xx, r2xx, n2xx, r1xx, n1xx); \
	      len = n1xx + n2xx; \
		   MPN_NORMALIZE(r3xx, len); \
		   n3xx = len; \
      } else \
         n3xx = 0; \
   } while (0)

#define MUL_TC4(r3xx, n3xx, r1xx, n1xx, r2xx, n2xx) \
	do \
	{ \
	   mp_size_t sign = n1xx ^ n2xx; \
	   mp_size_t un1 = ABS(n1xx); \
	   mp_size_t un2 = ABS(n2xx); \
		MUL_TC4_UNSIGNED(r3xx, n3xx, r1xx, un1, r2xx, un2); \
		if (sign < 0) n3xx = -n3xx; \
	} while (0)

#define SQR_TC4_UNSIGNED(r3xx, n3xx, r1xx, n1xx) \
   do \
   { \
      if (n1xx != 0) \
      { mp_size_t len; \
	      if (n1xx > MUL_TOOM4_THRESHOLD) mpn_toom4_sqr_n(r3xx, r1xx, n1xx); \
         else mpn_sqr(r3xx, r1xx, n1xx); \
		   len = 2*n1xx; \
		   MPN_NORMALIZE(r3xx, len); \
		   n3xx = len; \
      } else \
         n3xx = 0; \
   } while (0)

#define SQR_TC4(r3xx, n3xx, r1xx, n1xx) \
	do \
	{ \
	   mp_size_t un1 = ABS(n1xx); \
	   SQR_TC4_UNSIGNED(r3xx, n3xx, r1xx, un1); \
	} while (0)

#define TC4_NORM(rxx, nxx, sxx) \
	do \
	{ \
	   nxx = sxx; \
	   MPN_NORMALIZE(rxx, nxx); \
	} while(0)

/* Zero out limbs to end of integer */
#define TC4_DENORM(rxx, nxx, sxx) \
	do { \
	MPN_ZERO(rxx + ABS(nxx), sxx - ABS(nxx)); \
	} while (0)

/* Two's complement divexact by power of 2 */
#define TC4_DIVEXACT_2EXP(rxx, nxx, sxx) \
	do { \
	   mp_limb_t sign = (LIMB_HIGHBIT_TO_MASK(rxx[nxx-1]) << (GMP_LIMB_BITS - sxx)); \
      mpn_rshift(rxx, rxx, nxx, sxx); \
	   rxx[nxx-1] |= sign; \
	} while (0)

#if HAVE_NATIVE_mpn_rshift1
#define TC4_RSHIFT1(rxx, nxx) \
	do { \
	   mp_limb_t sign = (LIMB_HIGHBIT_TO_MASK(rxx[nxx-1]) << (GMP_LIMB_BITS - 1)); \
       mpn_half(rxx, nxx); \
	   rxx[nxx-1] |= sign; \
	} while (0)
#else
#define TC4_RSHIFT1(rxx, nxx) \
	do { \
	   mp_limb_t sign = (LIMB_HIGHBIT_TO_MASK(rxx[nxx-1]) << (GMP_LIMB_BITS - 1)); \
       mpn_rshift(rxx, rxx, nxx, 1); \
	   rxx[nxx-1] |= sign; \
	} while (0)
#endif

#define r1 (tp)
#define r2 (tp + t4)
#define r4 (tp + 2*t4)
#define r6 (tp + 3*t4)

#define r3 (rp + 4*sn)
#define r5 (rp + 2*sn)
#define r7 (rp)

/* Multiply {up, n} by {vp, n} and write the result to
   {prodp, 2n}.

   Note that prodp gets 2n limbs stored, even if the actual result
   only needs 2n - 1.
*/

#define mpn_clearit(rxx, nxx) \
  do { \
     mp_size_t ind = 0; \
     for ( ; ind < nxx; ind++) \
        (rxx)[ind] = CNST_LIMB(0); \
  } while (0)

void
mpn_toom4_mul_n (mp_ptr rp, mp_srcptr up,
		          mp_srcptr vp, mp_size_t n)
{
  mp_size_t ind;
  mp_limb_t cy, cy2, r30, r31;
  mp_ptr tp;
  mp_size_t sn, n1, n2, n3, n4, n5, n6, n7, n8, rpn, t4, h1;
  TMP_DECL;

  sn = (n + 3) / 4;

  h1 = n - 3*sn;
  
#define a0 (up)
#define a1 (up + sn)
#define a2 (up + 2*sn)
#define a3 (up + 3*sn)
#define b0 (vp)
#define b1 (vp + sn)
#define b2 (vp + 2*sn)
#define b3 (vp + 3*sn)

   t4 = 2*sn+2; // allows mult of 2 integers of sn + 1 limbs

   TMP_MARK;

   tp = TMP_ALLOC_LIMBS(4*t4 + 5*(sn + 1));

#define u2 (tp + 4*t4)
#define u3 (tp + 4*t4 + (sn+1))
#define u4 (tp + 4*t4 + 2*(sn+1))
#define u5 (tp + 4*t4 + 3*(sn+1))
#define u6 (tp + 4*t4 + 4*(sn+1))

   u6[sn] = mpn_add(u6, a1, sn, a3, h1);
   u5[sn] = mpn_add_n(u5, a2, a0, sn);
   mpn_add_n(u3, u5, u6, sn + 1);
   n4 = sn + 1;
   if (mpn_cmp(u5, u6, sn + 1) >= 0)
      mpn_sub_n(u4, u5, u6, sn + 1);
   else
   {  
      mpn_sub_n(u4, u6, u5, sn + 1);
      n4 = -n4;
   }

   u6[sn] = mpn_add(u6, b1, sn, b3, h1);
   u5[sn] = mpn_add_n(u5, b2, b0, sn);
   mpn_add_n(r2, u5, u6, sn + 1);
   n5 = sn + 1;
   if (mpn_cmp(u5, u6, sn + 1) >= 0)
      mpn_sub_n(u5, u5, u6, sn + 1);
   else
   {  
      mpn_sub_n(u5, u6, u5, sn + 1);
      n5 = -n5;
   }
 
   MUL_TC4_UNSIGNED(r3, n3, u3, sn + 1, r2, sn + 1); /* 1 */
   MUL_TC4(r4, n4, u4, n4, u5, n5); /* -1 */
   
#if HAVE_NATIVE_mpn_addlsh_n
   r1[sn] = mpn_addlsh_n(r1, a2, a0, sn, 2);
   mpn_lshift(r1, r1, sn + 1, 1);
   cy = mpn_addlsh_n(r2, a3, a1, h1, 2);
#else
   r1[sn] = mpn_lshift(r1, a2, sn, 1);
   MPN_COPY(r2, a3, h1);
   r1[sn] += mpn_addmul_1(r1, a0, sn, 8);
   cy = mpn_addmul_1(r2, a1, h1, 4);
#endif
   if (sn > h1) 
   {
      cy2 = mpn_lshift(r2 + h1, a1 + h1, sn - h1, 2);
      cy = cy2 + mpn_add_1(r2 + h1, r2 + h1, sn - h1, cy);
   }
   r2[sn] = cy;
   mpn_add_n(u5, r1, r2, sn + 1);
   n6 = sn + 1;
   if (mpn_cmp(r1, r2, sn + 1) >= 0)
      mpn_sub_n(u6, r1, r2, sn + 1);
   else
   {  
      mpn_sub_n(u6, r2, r1, sn + 1);
      n6 = -n6;
   }
 
#if HAVE_NATIVE_mpn_addlsh_n
   r1[sn] = mpn_addlsh_n(r1, b2, b0, sn, 2);
   mpn_lshift(r1, r1, sn + 1, 1);
   cy = mpn_addlsh_n(r2, b3, b1, h1, 2);
#else
   r1[sn] = mpn_lshift(r1, b2, sn, 1);
   MPN_COPY(r2, b3, h1);
   r1[sn] += mpn_addmul_1(r1, b0, sn, 8);
   cy = mpn_addmul_1(r2, b1, h1, 4);
#endif
   if (sn > h1) 
   {
      cy2 = mpn_lshift(r2 + h1, b1 + h1, sn - h1, 2);
      cy = cy2 + mpn_add_1(r2 + h1, r2 + h1, sn - h1, cy);
   }
   r2[sn] = cy;
   mpn_add_n(u2, r1, r2, sn + 1);
   n8 = sn + 1;
   if (mpn_cmp(r1, r2, sn + 1) >= 0)
      mpn_sub_n(r2, r1, r2, sn + 1);
   else
   {  
      mpn_sub_n(r2, r2, r1, sn + 1);
      n8 = -n8;
   }
    
   r30 = r3[0];
   r31 = r3[1];
   MUL_TC4_UNSIGNED(r5, n5, u5, sn + 1, u2, sn + 1); /* 1/2 */
   MUL_TC4(r6, n6, u6, n6, r2, n8); /* -1/2 */
   r3[1] = r31;

#if HAVE_NATIVE_mpn_addlsh1_n
   cy = mpn_addlsh1_n(u2, a2, a3, h1);
   if (sn > h1)
      cy = mpn_add_1(u2 + h1, a2 + h1, sn - h1, cy); 
   u2[sn] = cy;
   u2[sn] = 2*u2[sn] + mpn_addlsh1_n(u2, a1, u2, sn);     
   u2[sn] = 2*u2[sn] + mpn_addlsh1_n(u2, a0, u2, sn);     
#else
   MPN_COPY(u2, a0, sn);
   u2[sn] = mpn_addmul_1(u2, a1, sn, 2);
   u2[sn] += mpn_addmul_1(u2, a2, sn, 4);
   cy = mpn_addmul_1(u2, a3, h1, 8);
   if (sn > h1) cy = mpn_add_1(u2 + h1, u2 + h1, sn - h1, cy);
   u2[sn] += cy;
#endif

#if HAVE_NATIVE_mpn_addlsh1_n
   cy = mpn_addlsh1_n(r1, b2, b3, h1);
   if (sn > h1)
      cy = mpn_add_1(r1 + h1, b2 + h1, sn - h1, cy); 
   r1[sn] = cy;
   r1[sn] = 2*r1[sn] + mpn_addlsh1_n(r1, b1, r1, sn);     
   r1[sn] = 2*r1[sn] + mpn_addlsh1_n(r1, b0, r1, sn);     
#else
   MPN_COPY(r1, b0, sn);
   r1[sn] = mpn_addmul_1(r1, b1, sn, 2);
   r1[sn] += mpn_addmul_1(r1, b2, sn, 4);
   cy = mpn_addmul_1(r1, b3, h1, 8);
   if (sn > h1) cy = mpn_add_1(r1 + h1, r1 + h1, sn - h1, cy);
   r1[sn] += cy;
#endif
   
   MUL_TC4_UNSIGNED(r2, n2, u2, sn + 1, r1, sn + 1); /* 2 */
   
   MUL_TC4_UNSIGNED(r1, n1, a3, h1, b3, h1); /* oo */
   MUL_TC4_UNSIGNED(r7, n7, a0, sn, b0, sn); /* 0 */

   TC4_DENORM(r1, n1, t4 - 1);

/*	rp        rp1          rp2           rp3          rp4           rp5         rp6           rp7
<----------- r7-----------><------------r5-------------->            
                                                       <-------------r3------------->

              <-------------r6------------->                        < -----------r2------------>{           }
                                         <-------------r4-------------->         <--------------r1---->
*/

   mpn_toom4_interpolate(rp, &rpn, sn, tp, t4 - 1, n4, n6, r30);

   if (rpn != 2*n) 
   {
	  MPN_ZERO((rp + rpn), 2*n - rpn);
   }

   TMP_FREE;
}

/* Square {up, n} and write the result to {prodp, 2n}.

   Note that prodp gets 2n limbs stored, even if the actual result
   only needs 2n - 1.
*/

void
mpn_toom4_sqr_n (mp_ptr rp, mp_srcptr up, mp_size_t n)
{
  mp_size_t len1, ind;
  mp_limb_t cy, r30, r31;
  mp_ptr tp;
  mp_size_t a0n, a1n, a2n, a3n, sn, n1, n2, n3, n4, n5, n6, n7, n8, n9, rpn, t4;

  len1 = n;
  ASSERT (n >= 1);

  MPN_NORMALIZE(up, len1);
  
  sn = (n - 1) / 4 + 1;

  /* a0 - a3 are defined in mpn_toom4_mul_n above */
  
   TC4_NORM(a0, a0n, sn);
	TC4_NORM(a1, a1n, sn);
	TC4_NORM(a2, a2n, sn);
	TC4_NORM(a3, a3n, n - 3*sn); 

   t4 = 2*sn+2; // allows mult of 2 integers of sn + 1 limbs

   tp = __GMP_ALLOCATE_FUNC_LIMBS(4*t4 + 4*(sn + 1));

   tc4_add_unsigned(u5, &n5, a3, a3n, a1, a1n); 
   tc4_add_unsigned(u4, &n4, a2, a2n, a0, a0n); 
	tc4_add_unsigned(u2, &n2, u4, n4, u5, n5); 
   tc4_sub(u3, &n3, u4, n4, u5, n5);

	SQR_TC4(r4, n4, u3, n3);
   SQR_TC4_UNSIGNED(r3, n3, u2, n2);
	
	tc4_lshift(r1, &n1, a0, a0n, 3);
	tc4_addlsh1_unsigned(r1, &n1, a2, a2n);
 	tc4_lshift(r2, &n8, a1, a1n, 2);
   tc4_add(r2, &n8, r2, n8, a3, a3n);
   tc4_add(u4, &n9, r1, n1, r2, n8);
   tc4_sub(u5, &n5, r1, n1, r2, n8);
   
	r30 = r3[0];
	if (!n3) r30 = CNST_LIMB(0);
   r31 = r3[1];
	SQR_TC4(r6, n6, u5, n5);
   SQR_TC4_UNSIGNED(r5, n5, u4, n9);
   r3[1] = r31;

   tc4_lshift(u2, &n8, a3, a3n, 3);
   tc4_addmul_1(u2, &n8, a2, a2n, 4);
	tc4_addlsh1_unsigned(u2, &n8, a1, a1n);
	tc4_add(u2, &n8, u2, n8, a0, a0n);
   
	SQR_TC4_UNSIGNED(r2, n2, u2, n8);
   SQR_TC4_UNSIGNED(r1, n1, a3, a3n);
   SQR_TC4_UNSIGNED(r7, n7, a0, a0n);

	TC4_DENORM(r1, n1,  t4 - 1);
   TC4_DENORM(r2, n2,  t4 - 1);
   if (n3)
     TC4_DENORM(r3, n3,  t4 - 1);
   else {
     /* MPN_ZERO defeats gcc 4.1.2 here, hence the explicit for loop */
     for (ind = 1 ; ind < t4 - 1; ind++)
        (r3)[ind] = CNST_LIMB(0);
   }
   TC4_DENORM(r4, n4,  t4 - 1);
   TC4_DENORM(r5, n5,  t4 - 1);
   TC4_DENORM(r6, n6,  t4 - 1);
   TC4_DENORM(r7, n7,  t4 - 2); // we treat r7 differently (it cannot exceed t4-2 in length)

/*	rp        rp1          rp2           rp3          rp4           rp5         rp6           rp7
<----------- r7-----------><------------r5-------------->            
                                                       <-------------r3------------->

              <-------------r6------------->                        < -----------r2------------>{           }
                                         <-------------r4-------------->         <--------------r1---->
*/

	mpn_toom4_interpolate(rp, &rpn, sn, tp, t4 - 1, n4, n6, r30);

	if (rpn != 2*n) 
	{
		MPN_ZERO((rp + rpn), 2*n - rpn);
	}

   __GMP_FREE_FUNC_LIMBS (tp, 4*t4 + 4*(sn+1));
}

/*
   Toom 4 interpolation. Interpolates the value at 2^(sn*B) of a 
	polynomial p(x) with 7 coefficients given the values 
	p(oo), p(2), p(1), p(-1), 2^6*p(1/2), 2^6*p(-1/2), p(0).
	The output is placed in rp and the final number of limbs of the
	output is given in rpn.
	The 4th and 6th values may be negative, and if so, n4 and n6 
	should be set to a negative value respectively.
   To save space we pass r3, r5, r7 in place in the output rp.
	The other r's are stored separately in space tp.
	The low limb of r3 is stored in r30, as it will be overwritten
	by the high limb of r5.

rp          rp1          rp2           rp3          rp4           rp5         rp6           rp7
<----------- r7-----------><------------r5-------------->            
                                                      <-------------r3------------->

   We assume that r1 is stored at tp, r2 at (tp + t4), r4 at (tp + 2*t4) 
	and r6 (tp + 3*t4). Each of these r's has t4 = s4 + 1 limbs allocated.
*/
void mpn_toom4_interpolate(mp_ptr rp, mp_size_t * rpn, mp_size_t sn,  
		       mp_ptr tp, mp_size_t s4, mp_size_t n4, mp_size_t n6, mp_limb_t r30)
{
	mp_size_t n1, n2, n3, n5, n7, t4;
	mp_limb_t saved, saved2, cy;

   t4 = s4 + 1; 
   
	mpn_add_n(r2, r2, r5, s4);

   if (n6 < 0) 
		mpn_add_n(r6, r5, r6, s4);
	else
      mpn_sub_n(r6, r5, r6, s4);
	/* r6 is now in twos complement format */

	saved = r3[0];
	r3[0] = r30;
	if (n4 < 0) 
		mpn_add_n(r4, r3, r4, s4);
	else
      mpn_sub_n(r4, r3, r4, s4);
	r3[0] = saved;
	/* r4 is now in twos complement format */
	
	mpn_sub_n(r5, r5, r1, s4);

#if HAVE_NATIVE_mpn_sublsh_n
	r5[s4-1] -= mpn_sublsh_n(r5, r5, r7, s4-1, 6);
#else
	r5[s4-1] -= mpn_submul_1(r5, r7, s4-1, 64);
#endif
   
   TC4_RSHIFT1(r4, s4); 
	
	saved = r3[0];
	r3[0] = r30;
	mpn_sub_n(r3, r3, r4, s4);
	r30 = r3[0];
	r3[0] = saved;

	mpn_double(r5, s4); 

	mpn_sub_n(r5, r5, r6, s4);

   saved = r3[0];
	r3[0] = r30;
	mpn_submul_1(r2, r3, s4, 65);
   r3[0] = saved;
	
	saved2 = r7[s4-1];
	r7[s4-1] = CNST_LIMB(0); // r7 is always positive so no sign extend needed
	saved = r3[0];
	r3[0] = r30;
#if HAVE_NATIVE_mpn_subadd_n
	mpn_subadd_n(r3, r3, r7, r1, s4);
#else
    mpn_sub_n(r3, r3, r7, s4);
    mpn_sub_n(r3, r3, r1, s4);
#endif
	r7[s4-1] = saved2;
   r30 = r3[0];
	
   mpn_addmul_1(r2, r3, s4, 45);

#if HAVE_NATIVE_mpn_sublsh_n
   cy = mpn_sublsh_n(r5, r5, r3, s4 - 1, 3);
#else
   cy = mpn_submul_1(r5, r3, s4 - 1, 8);
#endif
   r3[0] = saved;
	r3[0] -= (cy + 8*r3[s4-1]);
   
	mpn_rshift(r5, r5, s4, 3); 

	mpn_divexact_by3(r5, r5, s4); 
   
	mpn_sub_n(r6, r6, r2, s4);

#if HAVE_NATIVE_mpn_sublsh_n
	mpn_sublsh_n(r2, r2, r4, s4, 4);
#else
	mpn_submul_1(r2, r4, s4, 16);
#endif
   
   mpn_rshift(r2, r2, s4, 1); 

	mpn_divexact_by3(r2, r2, s4); 

   mpn_divexact_by3(r2, r2, s4); 
   
   saved = r3[0];
	r3[0] = r30;
   cy = mpn_sub_n(r3, r3, r5, s4 - 1);
   r30 = r3[0];
	r3[0] = saved;
	r3[s4-1] -= (cy + r5[s4-1]);
   
	mpn_sub_n(r4, r4, r2, s4);
	
	mpn_addmul_1(r6, r2, s4, 30);

   mpn_divexact_byfobm1(r6, r6, s4, CNST_LIMB(15), CNST_LIMB(~0/15));

	mpn_rshift(r6, r6, s4, 2);

	mpn_sub_n(r2, r2, r6, s4);

	TC4_NORM(r1, n1, s4);
   TC4_NORM(r2, n2, s4);
   
   (*rpn) = 6*sn+1;
	cy = mpn_add_1(r3, r3, *rpn - 4*sn, r30); /* don't forget to add r3[0] back in */
   if (cy) 
	{
		rp[*rpn] = cy;
	   (*rpn)++;
	}

	tc4_copy(rp, rpn, 5*sn, r2, n2);
   tc4_copy(rp, rpn, 6*sn, r1, n1);

	tc4_copy(rp, rpn, sn, r6, s4);
   tc4_copy(rp, rpn, 3*sn, r4, s4); 
}
