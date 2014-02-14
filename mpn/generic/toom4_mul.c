/* mpn_toom4_mul -- Internal routine to multiply two natural numbers
   using unbalanced toom4.

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

#define mpn_clearit(rxx, nxx) \
  do { \
     mp_size_t ind = 0; \
     for ( ; ind < nxx; ind++) \
        (rxx)[ind] = CNST_LIMB(0); \
  } while (0)

/* Multiply {up, un} by {vp, vn} and write the result to
   {prodp, un + vn} assuming vn > 3*ceil(un/4).

   Note that prodp gets un + vn limbs stored, even if the actual 
   result only needs un + vn - 1.
*/
void
mpn_toom4_mul (mp_ptr rp, mp_srcptr up, mp_size_t un,
		          mp_srcptr vp, mp_size_t vn)
{
  mp_size_t ind;
  mp_limb_t cy, cy2, r30, r31;
  mp_ptr tp;
  mp_size_t sn, n1, n2, n3, n4, n5, n6, n7, n8, rpn, t4, h1, h2;
  TMP_DECL;

  sn = (un + 3) / 4;

  h1 = un - 3*sn;
  h2 = vn - 3*sn;

  ASSERT (vn > 3*sn);
  
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

   u6[sn] = mpn_add(u6, b1, sn, b3, h2);
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
   cy = mpn_addlsh_n(r2, b3, b1, h2, 2);
#else
   r1[sn] = mpn_lshift(r1, b2, sn, 1);
   MPN_COPY(r2, b3, h2);
   r1[sn] += mpn_addmul_1(r1, b0, sn, 8);
   cy = mpn_addmul_1(r2, b1, h2, 4);
#endif
   if (sn > h2) 
   {
      cy2 = mpn_lshift(r2 + h2, b1 + h2, sn - h2, 2);
      cy = cy2 + mpn_add_1(r2 + h2, r2 + h2, sn - h2, cy);
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
   cy = mpn_addlsh1_n(r1, b2, b3, h2);
   if (sn > h2)
      cy = mpn_add_1(r1 + h2, b2 + h2, sn - h2, cy); 
   r1[sn] = cy;
   r1[sn] = 2*r1[sn] + mpn_addlsh1_n(r1, b1, r1, sn);     
   r1[sn] = 2*r1[sn] + mpn_addlsh1_n(r1, b0, r1, sn);     
#else
   MPN_COPY(r1, b0, sn);
   r1[sn] = mpn_addmul_1(r1, b1, sn, 2);
   r1[sn] += mpn_addmul_1(r1, b2, sn, 4);
   cy = mpn_addmul_1(r1, b3, h2, 8);
   if (sn > h2) cy = mpn_add_1(r1 + h2, r1 + h2, sn - h2, cy);
   r1[sn] += cy;
#endif
   
   MUL_TC4_UNSIGNED(r2, n2, u2, sn + 1, r1, sn + 1); /* 2 */
   
   MUL_TC4_UNSIGNED(r1, n1, a3, h1, b3, h2); /* oo */
   MUL_TC4_UNSIGNED(r7, n7, a0, sn, b0, sn); /* 0 */

   TC4_DENORM(r1, n1, t4 - 1);

/*	rp        rp1          rp2           rp3          rp4           rp5         rp6           rp7
<----------- r7-----------><------------r5-------------->            
                                                       <-------------r3------------->

              <-------------r6------------->                        < -----------r2------------>{           }
                                         <-------------r4-------------->         <--------------r1---->
*/

   mpn_toom4_interpolate(rp, &rpn, sn, tp, t4 - 1, n4, n6, r30);

   if (rpn != un + vn) 
   {
	  MPN_ZERO((rp + rpn), un + vn - rpn);
   }

   TMP_FREE;
}

/* Multiply {up, un} by {vp, vn} and write the result to
   {prodp, un + vn} assuming vn > 2*ceil(un/5).

   Note that prodp gets un + vn limbs stored, even if the actual 
   result only needs un + vn - 1.
*/
void
mpn_toom53_mul (mp_ptr rp, mp_srcptr up, mp_size_t un,
		          mp_srcptr vp, mp_size_t vn)
{
  mp_size_t ind;
  mp_limb_t cy, r30, r31;
  mp_ptr tp;
  mp_size_t a0n, a1n, a2n, a3n, a4n, b0n, b1n, b2n, sn; 
  mp_size_t n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, rpn, t4;

  sn = (un + 4) / 5;

  ASSERT (vn > 2*sn);
  
#define a0 (up)
#define a1 (up + sn)
#define a2 (up + 2*sn)
#define a3 (up + 3*sn)
#define a4 (up + 4*sn)
#define b0 (vp)
#define b1 (vp + sn)
#define b2 (vp + 2*sn)

   TC4_NORM(a0, a0n, sn);
   TC4_NORM(a1, a1n, sn);
   TC4_NORM(a2, a2n, sn);
   TC4_NORM(a3, a3n, sn);
   TC4_NORM(a4, a4n, un - 4*sn); 
   TC4_NORM(b0, b0n, sn);
   TC4_NORM(b1, b1n, sn);
   TC4_NORM(b2, b2n, vn - 2*sn); 

   t4 = 2*sn+2; // allows mult of 2 integers of sn + 1 limbs

   tp = __GMP_ALLOCATE_FUNC_LIMBS(4*t4 + 4*(sn + 1));

#define u2 (tp + 4*t4)
#define u3 (tp + 4*t4 + (sn+1))
#define u4 (tp + 4*t4 + 2*(sn+1))
#define u5 (tp + 4*t4 + 3*(sn+1))

   tc4_add_unsigned(u2, &n2, a3, a3n, a1, a1n); 
   tc4_add_unsigned(u5, &n5, a2, a2n, a0, a0n); 
   tc4_add_unsigned(u5, &n5, u5, n5, a4, a4n); 
   tc4_add_unsigned(u3, &n3, u5, n5, u2, n2); 
   tc4_sub(u4, &n4, u5, n5, u2, n2);

   tc4_add_unsigned(u5, &n5, b2, b2n, b0, b0n);
   tc4_add_unsigned(r2, &n8, u5, n5, b1, b1n); 
   tc4_sub(u5, &n5, u5, n5, b1, b1n);

   MUL_TC4_UNSIGNED(r3, n3, u3, n3, r2, n8); /* 1 */
   MUL_TC4(r4, n4, u4, n4, u5, n5); /* -1 */
   
   tc4_lshift(r1, &n1, a0, a0n, 4);
   tc4_lshift(u3, &n9, a2, a2n, 2);
   tc4_add_unsigned(r1, &n1, r1, n1, u3, n9);
   tc4_add_unsigned(r1, &n1, r1, n1, a4, a4n);
   tc4_lshift(r2, &n8, a1, a1n, 3);
   tc4_addlsh1_unsigned(r2, &n8, a3, a3n);
   tc4_add_unsigned(u5, &n5, r1, n1, r2, n8);
   tc4_sub(u3, &n9, r1, n1, r2, n8);

   tc4_lshift(r1, &n1, b0, b0n, 2);
   tc4_add_unsigned(r1, &n1, r1, n1, b2, b2n);
   tc4_lshift(u4, &n10, b1, b1n, 1);
   tc4_add_unsigned(u2, &n2, r1, n1, u4, n10);
   tc4_sub(r2, &n8, r1, n1, u4, n10);
   
   r30 = r3[0];
   if (!n3) r30 = CNST_LIMB(0);
   r31 = r3[1];
   MUL_TC4_UNSIGNED(r5, n5, u5, n5, u2, n2); /* 1/2 */
   MUL_TC4(r6, n6, u3, n9, r2, n8); /* -1/2 */
   r3[1] = r31;

   tc4_lshift(u2, &n2, a4, a4n, 4);
   tc4_addmul_1(u2, &n2, a3, a3n, 8);
   tc4_addmul_1(u2, &n2, a2, a2n, 4);
   tc4_addlsh1_unsigned(u2, &n2, a1, a1n);
   tc4_add(u2, &n2, u2, n2, a0, a0n);

   tc4_lshift(r1, &n1, b2, b2n, 2);
   tc4_addlsh1_unsigned(r1, &n1, b1, b1n);
   tc4_add(r1, &n1, r1, n1, b0, b0n);
   
   MUL_TC4_UNSIGNED(r2, n2, u2, n2, r1, n1); /* 2 */

   MUL_TC4_UNSIGNED(r1, n1, a4, a4n, b2, b2n); /* oo */
   MUL_TC4_UNSIGNED(r7, n7, a0, a0n, b0, b0n); /* 0 */

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

   if (rpn != un + vn) 
   {
	  MPN_ZERO((rp + rpn), un + vn - rpn);
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
