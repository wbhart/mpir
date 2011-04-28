/* longlong.h -- definitions for mixed size 32/64 bit arithmetic.

Copyright 1991, 1992, 1993, 1994, 1996, 1997, 1999, 2000, 2001, 2002, 2003,
2004, 2005 Free Software Foundation, Inc.

This file is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

This file is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this file; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA. */

#if defined (__GNUC__)

#define __CLOBBER_CC : "cc"
#define __AND_CLOBBER_CC , "cc"

#define add_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("adds\t%1, %4, %5\n\tadc\t%0, %2, %3"			\
	   : "=r" (sh), "=&r" (sl)					\
	   : "r" (ah), "rI" (bh), "%r" (al), "rI" (bl) __CLOBBER_CC)
#define sub_ddmmss(sh, sl, ah, al, bh, bl) \
  do {									\
    if (__builtin_constant_p (al))					\
      {									\
	if (__builtin_constant_p (ah))					\
	  __asm__ ("rsbs\t%1, %5, %4\n\trsc\t%0, %3, %2"		\
		   : "=r" (sh), "=&r" (sl)				\
		   : "rI" (ah), "r" (bh), "rI" (al), "r" (bl) __CLOBBER_CC); \
	else								\
	  __asm__ ("rsbs\t%1, %5, %4\n\tsbc\t%0, %2, %3"		\
		   : "=r" (sh), "=&r" (sl)				\
		   : "r" (ah), "rI" (bh), "rI" (al), "r" (bl) __CLOBBER_CC); \
      }									\
    else if (__builtin_constant_p (ah))					\
      {									\
	if (__builtin_constant_p (bl))					\
	  __asm__ ("subs\t%1, %4, %5\n\trsc\t%0, %3, %2"		\
		   : "=r" (sh), "=&r" (sl)				\
		   : "rI" (ah), "r" (bh), "r" (al), "rI" (bl) __CLOBBER_CC); \
	else								\
	  __asm__ ("rsbs\t%1, %5, %4\n\trsc\t%0, %3, %2"		\
		   : "=r" (sh), "=&r" (sl)				\
		   : "rI" (ah), "r" (bh), "rI" (al), "r" (bl) __CLOBBER_CC); \
      }									\
    else if (__builtin_constant_p (bl))					\
      {									\
	if (__builtin_constant_p (bh))					\
	  __asm__ ("subs\t%1, %4, %5\n\tsbc\t%0, %2, %3"		\
		   : "=r" (sh), "=&r" (sl)				\
		   : "r" (ah), "rI" (bh), "r" (al), "rI" (bl) __CLOBBER_CC); \
	else								\
	  __asm__ ("subs\t%1, %4, %5\n\trsc\t%0, %3, %2"		\
		   : "=r" (sh), "=&r" (sl)				\
		   : "rI" (ah), "r" (bh), "r" (al), "rI" (bl) __CLOBBER_CC); \
      }									\
    else /* only bh might be a constant */				\
      __asm__ ("subs\t%1, %4, %5\n\tsbc\t%0, %2, %3"			\
	       : "=r" (sh), "=&r" (sl)					\
	       : "r" (ah), "rI" (bh), "r" (al), "rI" (bl) __CLOBBER_CC);\
    } while (0)
#if 1 || defined (__arm_m__)	/* `M' series has widening multiply support */
#define umul_ppmm(xh, xl, a, b) \
  __asm__ ("umull %0,%1,%2,%3" : "=&r" (xl), "=&r" (xh) : "r" (a), "r" (b))
#define smul_ppmm(xh, xl, a, b) \
  __asm__ ("smull %0,%1,%2,%3" : "=&r" (xl), "=&r" (xh) : "r" (a), "r" (b))
#ifndef LONGLONG_STANDALONE
#define udiv_qrnnd(q, r, n1, n0, d) \
  do { UWtype __di;							\
    __di = __MPN(invert_limb) (d);					\
    udiv_qrnnd_preinv (q, r, n1, n0, d, __di);				\
  } while (0)
#define UDIV_PREINV_ALWAYS  1
#define UDIV_NEEDS_NORMALIZATION 1
#endif /* LONGLONG_STANDALONE */
#else
#define umul_ppmm(xh, xl, a, b) \
  __asm__ ("%@ Inlined umul_ppmm\n"					\
"	mov	%|r0, %2, lsr #16\n"					\
"	mov	%|r2, %3, lsr #16\n"					\
"	bic	%|r1, %2, %|r0, lsl #16\n"				\
"	bic	%|r2, %3, %|r2, lsl #16\n"				\
"	mul	%1, %|r1, %|r2\n"					\
"	mul	%|r2, %|r0, %|r2\n"					\
"	mul	%|r1, %0, %|r1\n"					\
"	mul	%0, %|r0, %0\n"						\
"	adds	%|r1, %|r2, %|r1\n"					\
"	addcs	%0, %0, #65536\n"					\
"	adds	%1, %1, %|r1, lsl #16\n"				\
"	adc	%0, %0, %|r1, lsr #16"					\
	   : "=&r" (xh), "=r" (xl)					\
	   : "r" (a), "r" (b)						\
	   : "r0", "r1", "r2")
#ifndef LONGLONG_STANDALONE
#define udiv_qrnnd(q, r, n1, n0, d) \
  do { UWtype __r;							\
    (q) = __MPN(udiv_qrnnd) (&__r, (n1), (n0), (d));			\
    (r) = __r;								\
  } while (0)
extern UWtype __MPN(udiv_qrnnd) _PROTO ((UWtype *, UWtype, UWtype, UWtype));
#endif /* LONGLONG_STANDALONE */
#endif

#endif
