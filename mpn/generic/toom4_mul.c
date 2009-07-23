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
       mpn_rshift1(rxx, rxx, nxx); \
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
   {prodp, un + vn}.

   Note that prodp gets un + vn limbs stored, even if the actual 
   result only needs un + vn - 1.
*/
void
mpn_toom4_mul (mp_ptr rp, mp_srcptr up, mp_size_t un,
		          mp_srcptr vp, mp_size_t vn)
{
  mp_size_t ind;
  mp_limb_t cy, r30, r31;
  mp_ptr tp;
  mp_size_t a0n, a1n, a2n, a3n, b0n, b1n, b2n, b3n, sn, n1, n2, n3, n4, n5, n6, n7, n8, rpn, t4;

  sn = (un + 3) / 4;

  ASSERT (vn > 3*sn);
  
#define a0 (up)
#define a1 (up + sn)
#define a2 (up + 2*sn)
#define a3 (up + 3*sn)
#define b0 (vp)
#define b1 (vp + sn)
#define b2 (vp + 2*sn)
#define b3 (vp + 3*sn)

   TC4_NORM(a0, a0n, sn);
   TC4_NORM(a1, a1n, sn);
   TC4_NORM(a2, a2n, sn);
   TC4_NORM(a3, a3n, un - 3*sn); 
   TC4_NORM(b0, b0n, sn);
   TC4_NORM(b1, b1n, sn);
   TC4_NORM(b2, b2n, sn);
   TC4_NORM(b3, b3n, vn - 3*sn); 

   t4 = 2*sn+2; // allows mult of 2 integers of sn + 1 limbs

   tp = __GMP_ALLOCATE_FUNC_LIMBS(4*t4 + 5*(sn + 1));

#define u2 (tp + 4*t4)
#define u3 (tp + 4*t4 + (sn+1))
#define u4 (tp + 4*t4 + 2*(sn+1))
#define u5 (tp + 4*t4 + 3*(sn+1))
#define u6 (tp + 4*t4 + 4*(sn+1))

   tc4_add_unsigned(u6, &n6, a3, a3n, a1, a1n); 
   tc4_add_unsigned(u5, &n5, a2, a2n, a0, a0n); 
#if HAVE_NATIVE_mpn_sumdiff_n
   tc4_sumdiff_unsigned(u3, &n3, u4, &n4, u5, n5, u6, n6); 
#else
   tc4_add_unsigned(u3, &n3, u5, n5, u6, n6); 
   tc4_sub(u4, &n4, u5, n5, u6, n6);
#endif
   tc4_add_unsigned(u6, &n6, b3, b3n, b1, b1n);
   tc4_add_unsigned(u5, &n5, b2, b2n, b0, b0n);
#if HAVE_NATIVE_mpn_sumdiff_n
   tc4_sumdiff_unsigned(r2, &n8, u5, &n5, u5, n5, u6, n6); 
#else
   tc4_add_unsigned(r2, &n8, u5, n5, u6, n6); 
   tc4_sub(u5, &n5, u5, n5, u6, n6);
#endif

   MUL_TC4_UNSIGNED(r3, n3, u3, n3, r2, n8);
   MUL_TC4(r4, n4, u4, n4, u5, n5);
   
   tc4_lshift(r1, &n1, a0, a0n, 3);
#if HAVE_NATIVE_mpn_addlsh1_n
   tc4_addlsh1_unsigned(r1, &n1, a2, a2n);
#else
   tc4_addmul_1(r1, &n1, a2, a2n, 2);
#endif
   tc4_lshift(r2, &n8, a1, a1n, 2);
   tc4_add(r2, &n8, r2, n8, a3, a3n);
   tc4_add(u5, &n5, r1, n1, r2, n8);
   tc4_sub(u6, &n6, r1, n1, r2, n8);
   tc4_lshift(r1, &n1, b0, b0n, 3);
#if HAVE_NATIVE_mpn_addlsh1_n
   tc4_addlsh1_unsigned(r1, &n1, b2, b2n);
#else
   tc4_addmul_1(r1, &n1, b2, b2n, 2);
#endif
   tc4_lshift(r2, &n8, b1, b1n, 2);
   tc4_add(r2, &n8, r2, n8, b3, b3n);
   tc4_add(u2, &n2, r1, n1, r2, n8);
   tc4_sub(r2, &n8, r1, n1, r2, n8);
   
   r30 = r3[0];
   if (!n3) r30 = CNST_LIMB(0);
   r31 = r3[1];
   MUL_TC4_UNSIGNED(r5, n5, u5, n5, u2, n2);
   MUL_TC4(r6, n6, u6, n6, r2, n8);
   r3[1] = r31;

   tc4_lshift(u2, &n2, a3, a3n, 3);
   tc4_addmul_1(u2, &n2, a2, a2n, 4);
#if HAVE_NATIVE_mpn_addlsh1_n
   tc4_addlsh1_unsigned(u2, &n2, a1, a1n);
#else
   tc4_addmul_1(u2, &n2, a1, a1n, 2);
#endif
   tc4_add(u2, &n2, u2, n2, a0, a0n);
   tc4_lshift(r1, &n1, b3, b3n, 3);
   tc4_addmul_1(r1, &n1, b2, b2n, 4);
#if HAVE_NATIVE_mpn_addlsh1_n
   tc4_addlsh1_unsigned(r1, &n1, b1, b1n);
#else
   tc4_addmul_1(r1, &n1, b1, b1n, 2);
#endif
   tc4_add(r1, &n1, r1, n1, b0, b0n);
   
   MUL_TC4_UNSIGNED(r2, n2, u2, n2, r1, n1);
   MUL_TC4_UNSIGNED(r1, n1, a3, a3n, b3, b3n);
   MUL_TC4_UNSIGNED(r7, n7, a0, a0n, b0, b0n);

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

   __GMP_FREE_FUNC_LIMBS (tp, 4*t4 + 5*(sn+1));
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
