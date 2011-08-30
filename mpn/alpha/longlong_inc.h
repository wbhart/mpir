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

/* Most alpha-based machines, except Cray systems. */
#if defined (__GNUC__)
#define umul_ppmm(ph, pl, m0, m1) \
  do {									\
    UDItype __m0 = (m0), __m1 = (m1);					\
    __asm__ ("umulh %r1,%2,%0"						\
	     : "=r" (ph)						\
	     : "%rJ" (m0), "rI" (m1));					\
    (pl) = __m0 * __m1;							\
  } while (0)
#else /* ! __GNUC__ */
#include <machine/builtins.h>
#define umul_ppmm(ph, pl, m0, m1) \
  do {									\
    UDItype __m0 = (m0), __m1 = (m1);					\
    (ph) = __UMULH (m0, m1);						\
    (pl) = __m0 * __m1;							\
  } while (0)
#endif
#ifndef LONGLONG_STANDALONE
#define udiv_qrnnd(q, r, n1, n0, d) \
  do { UWtype __di;							\
    __di = __MPN(invert_limb) (d);					\
    udiv_qrnnd_preinv (q, r, n1, n0, d, __di);				\
  } while (0)
#define UDIV_PREINV_ALWAYS  1
#define UDIV_NEEDS_NORMALIZATION 1
#endif /* LONGLONG_STANDALONE */

/* clz_tab is required in all configurations, since mpn/alpha/cntlz.asm
   always goes into libmpir.so, even when not actually used.  */
#define COUNT_LEADING_ZEROS_NEED_CLZ_TAB

#if ! defined (count_leading_zeros)                             \
  && defined (__GNUC__) && ! defined (LONGLONG_STANDALONE)
/* ALPHA_CMPBGE_0 gives "cmpbge $31,src,dst", ie. test src bytes == 0.
   "$31" is written explicitly in the asm, since an "r" constraint won't
   select reg 31.  There seems no need to worry about "r31" syntax for cray,
   since gcc itself (pre-release 3.4) emits just $31 in various places.  */
#define ALPHA_CMPBGE_0(dst, src)                                        \
  do { asm ("cmpbge $31, %1, %0" : "=r" (dst) : "r" (src)); } while (0)
/* Zero bytes are turned into bits with cmpbge, a __clz_tab lookup counts
   them, locating the highest non-zero byte.  A second __clz_tab lookup
   counts the leading zero bits in that byte, giving the result.  */
#define count_leading_zeros(count, x)                                   \
  do {                                                                  \
    UWtype  __clz__b, __clz__c, __clz__x = (x);                         \
    ALPHA_CMPBGE_0 (__clz__b,  __clz__x);           /* zero bytes */    \
    __clz__b = __clz_tab [(__clz__b >> 1) ^ 0x7F];  /* 8 to 1 byte */   \
    __clz__b = __clz__b * 8 - 7;                    /* 57 to 1 shift */ \
    __clz__x >>= __clz__b;                                              \
    __clz__c = __clz_tab [__clz__x];                /* 8 to 1 bit */    \
    __clz__b = 65 - __clz__b;                                           \
    (count) = __clz__b - __clz__c;                                      \
  } while (0)
#define COUNT_LEADING_ZEROS_NEED_CLZ_TAB
#endif /* clz using cmpbge */

#if ! defined (count_leading_zeros) && ! defined (LONGLONG_STANDALONE)
#if HAVE_ATTRIBUTE_CONST
long __MPN(count_leading_zeros) _PROTO ((UDItype)) __attribute__ ((const));
#else
long __MPN(count_leading_zeros) _PROTO ((UDItype));
#endif
#define count_leading_zeros(count, x) \
  ((count) = __MPN(count_leading_zeros) (x))
#endif /* clz using mpn */

