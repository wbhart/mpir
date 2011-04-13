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

/* You have to define the following before including this file:

   UWtype -- An unsigned type, default type for operations (typically a "word")
   UHWtype -- An unsigned type, at least half the size of UWtype.
   UDWtype -- An unsigned type, at least twice as large a UWtype
   W_TYPE_SIZE -- size in bits of UWtype

   SItype, USItype -- Signed and unsigned 32 bit types.
   DItype, UDItype -- Signed and unsigned 64 bit types.

   On a 32 bit machine UWtype should typically be USItype;
   on a 64 bit machine, UWtype should typically be UDItype.
*/

#define __BITS4 (W_TYPE_SIZE / 4)
#define __ll_B ((UWtype) 1 << (W_TYPE_SIZE / 2))
#define __ll_lowpart(t) ((UWtype) (t) & (__ll_B - 1))
#define __ll_highpart(t) ((UWtype) (t) >> (W_TYPE_SIZE / 2))

/* This is used to make sure no undesirable sharing between different libraries
   that use this file takes place.  */
#ifndef __MPN
#define __MPN(x) __##x
#endif

#ifndef _PROTO
#if (__STDC__-0) || defined (__cplusplus) || defined( _MSC_VER )
#define _PROTO(x) x
#else
#define _PROTO(x) ()
#endif
#endif

/* Define auxiliary asm macros.

   1) umul_ppmm(high_prod, low_prod, multipler, multiplicand) multiplies two
   UWtype integers MULTIPLER and MULTIPLICAND, and generates a two UWtype
   word product in HIGH_PROD and LOW_PROD.

   3) udiv_qrnnd(quotient, remainder, high_numerator, low_numerator,
   denominator) divides a UDWtype, composed by the UWtype integers
   HIGH_NUMERATOR and LOW_NUMERATOR, by DENOMINATOR and places the quotient
   in QUOTIENT and the remainder in REMAINDER.  HIGH_NUMERATOR must be less
   than DENOMINATOR for correct operation.  If, in addition, the most
   significant bit of DENOMINATOR must be 1, then the pre-processor symbol
   UDIV_NEEDS_NORMALIZATION is defined to 1.

   4) sdiv_qrnnd(quotient, remainder, high_numerator, low_numerator,
   denominator).  Like udiv_qrnnd but the numbers are signed.  The quotient
   is rounded towards 0.

   5) count_leading_zeros(count, x) counts the number of zero-bits from the
   msb to the first non-zero bit in the UWtype X.  This is the number of
   steps X needs to be shifted left to set the msb.  Undefined for X == 0,
   unless the symbol COUNT_LEADING_ZEROS_0 is defined to some value.

   6) count_trailing_zeros(count, x) like count_leading_zeros, but counts
   from the least significant end.

   7) add_ssaaaa(high_sum, low_sum, high_addend_1, low_addend_1,
   high_addend_2, low_addend_2) adds two UWtype integers, composed by
   HIGH_ADDEND_1 and LOW_ADDEND_1, and HIGH_ADDEND_2 and LOW_ADDEND_2
   respectively.  The result is placed in HIGH_SUM and LOW_SUM.  Overflow
   (i.e. carry out) is not stored anywhere, and is lost.

   8) sub_ddmmss(high_difference, low_difference, high_minuend, low_minuend,
   high_subtrahend, low_subtrahend) subtracts two two-word UWtype integers,
   composed by HIGH_MINUEND_1 and LOW_MINUEND_1, and HIGH_SUBTRAHEND_2 and
   LOW_SUBTRAHEND_2 respectively.  The result is placed in HIGH_DIFFERENCE
   and LOW_DIFFERENCE.  Overflow (i.e. carry out) is not stored anywhere,
   and is lost.

   If any of these macros are left undefined for a particular CPU,
   C macros are used.


   Notes:

   For add_ssaaaa the two high and two low addends can both commute, but
   unfortunately gcc only supports one "%" commutative in each asm block.
   This has always been so but is only documented in recent versions
   (eg. pre-release 3.3).  Having two or more "%"s can cause an internal
   compiler error in certain rare circumstances.

   Apparently it was only the last "%" that was ever actually respected, so
   the code has been updated to leave just that.  Clearly there's a free
   choice whether high or low should get it, if there's a reason to favour
   one over the other.  Also obviously when the constraints on the two
   operands are identical there's no benefit to the reloader in any "%" at
   all.

   */

/* The CPUs come in alphabetical order below.

   Please add support for more CPUs here, or improve the current support
   for the CPUs below!  */


/* count_leading_zeros_gcc_clz is count_leading_zeros implemented with gcc
   3.4 __builtin_clzl or __builtin_clzll, according to our limb size.
   Similarly count_trailing_zeros_gcc_ctz using __builtin_ctzl or
   __builtin_ctzll.

   These builtins are only used when we check what code comes out, on some
   chips they're merely libgcc calls, where we will instead want an inline
   in that case (either asm or generic C).

   These builtins are better than an asm block of the same insn, since an
   asm block doesn't give gcc any information about scheduling or resource
   usage.  We keep an asm block for use on prior versions of gcc though.

   For reference, __builtin_ffs existed in gcc prior to __builtin_clz, but
   it's not used (for count_leading_zeros) because it generally gives extra
   code to ensure the result is 0 when the input is 0, which we don't need
   or want.  */

#ifdef _LONG_LONG_LIMB
#define count_leading_zeros_gcc_clz(count,x)    \
  do {                                          \
    ASSERT ((x) != 0);                          \
    (count) = __builtin_clzll (x);              \
  } while (0)
#else
#define count_leading_zeros_gcc_clz(count,x)    \
  do {                                          \
    ASSERT ((x) != 0);                          \
    (count) = __builtin_clzl (x);               \
  } while (0)
#endif

#ifdef _LONG_LONG_LIMB
#define count_trailing_zeros_gcc_ctz(count,x)   \
  do {                                          \
    ASSERT ((x) != 0);                          \
    (count) = __builtin_ctzll (x);              \
  } while (0)
#else
#define count_trailing_zeros_gcc_ctz(count,x)   \
  do {                                          \
    ASSERT ((x) != 0);                          \
    (count) = __builtin_ctzl (x);               \
  } while (0)
#endif

#ifdef _MSC_VER
/* temporary */
#include "mpn/x86w/longlong.h"
#include "mpn/x86_64w/longlong.h"
#endif

/* FIXME: The macros using external routines like __MPN(count_leading_zeros)
   don't need to be under !NO_ASM */
#if ! defined (NO_ASM)

#if defined (__alpha) && W_TYPE_SIZE == 64
#include "mpn/alpha/longlong.h"
#endif 

#if defined (__ia64) && W_TYPE_SIZE == 64
#include "mpn/ia64/longlong.h"
#endif


#if defined (__GNUC__) || defined(INTEL_COMPILER)

#if (defined (__i386__) || defined (__i486__)) && W_TYPE_SIZE == 32
#include "mpn/x86/longlong.h"
#endif

#if defined (__amd64__) && W_TYPE_SIZE == 64
#include "mpn/x86_64/longlong.h"
#endif

#endif

#if defined (__GNUC__)

/* We sometimes need to clobber "cc" with gcc2, but that would not be
   understood by gcc1.  Use cpp to avoid major code duplication.  */
#if __GNUC__ < 2
#define __CLOBBER_CC
#define __AND_CLOBBER_CC
#else /* __GNUC__ >= 2 */
#define __CLOBBER_CC : "cc"
#define __AND_CLOBBER_CC , "cc"
#endif /* __GNUC__ < 2 */

#if defined (__arm__) && W_TYPE_SIZE == 32
#include "mpn/arm/longlong.h"
#endif

#if defined (__hppa) && W_TYPE_SIZE == 32
#include "mpn/pa32/longlong.h"
#endif

/* These macros are for ABI=2.0w.  In ABI=2.0n they can't be used, since GCC
   (3.2) puts longlong into two adjacent 32-bit registers.  Presumably this
   is just a case of no direct support for 2.0n but treating it like 1.0. */
#if defined (__hppa) && W_TYPE_SIZE == 64 && ! defined (_LONG_LONG_LIMB)
#include "mpn/pa64/longlong.h"
#endif

#if (defined (__i370__) || defined (__s390__) || defined (__mvs__)) && W_TYPE_SIZE == 32
#include "mpn/s390/longlong.h"
#endif

#if (defined (__mc68000__) || defined (__mc68020__) || defined(mc68020) \
     || defined (__m68k__) || defined (__mc5200__) || defined (__mc5206e__) \
     || defined (__mc5307__)) && W_TYPE_SIZE == 32
#include "mpn/m68k/longlong.h"
#endif

#if defined (__mips) && W_TYPE_SIZE == 32
#include "mpn/mips32/longlong.h"
#endif

#if (defined (__mips) && __mips >= 3) && W_TYPE_SIZE == 64
#include "mpn/mips64/longlong.h"
#endif

/* In the past we had a block of various #defines tested
       _ARCH_PPC    - AIX
       _ARCH_PWR    - AIX
       __powerpc__  - gcc
       __POWERPC__  - BEOS
       __ppc__      - Darwin
       PPC          - old gcc, GNU/Linux, SysV
   The plain PPC test was not good for vxWorks, since PPC is defined on all
   CPUs there (eg. m68k too), as a constant one is expected to compare
   CPU_FAMILY against.

   At any rate, this was pretty unattractive and a bit fragile.  The use of
   HAVE_HOST_CPU_FAMILY is designed to cut through it all and be sure of
   getting the desired effect.

   ENHANCE-ME: We should test _IBMR2 here when we add assembly support for
   the system vendor compilers.  (Is that vendor compilers with inline asm,
   or what?)  */

#if (HAVE_HOST_CPU_FAMILY_power || HAVE_HOST_CPU_FAMILY_powerpc)        \
  && W_TYPE_SIZE == 32
#include "mpn/powerpc32/longlong.h"
#endif

/* We should test _IBMR2 here when we add assembly support for the system
   vendor compilers.  */
#if HAVE_HOST_CPU_FAMILY_powerpc && W_TYPE_SIZE == 64
#include "mpn/powerpc64/longlong.h"
#endif

#if defined (__sh2__) && W_TYPE_SIZE == 32
#include "mpn/sh/longlong.h"
#endif

#if defined (__sparc__) && W_TYPE_SIZE == 32
#include "mpn/sparc32/longlong.h"
#endif

#if defined (__sparc__) && W_TYPE_SIZE == 64
#include "mpn/sparc64/longlong.h"
#endif

#endif /* __GNUC__ */

#endif /* NO_ASM */

/* Use mpn_umul_ppmm or mpn_udiv_qrnnd functions, if they exist.  The "_r"
   forms have "reversed" arguments, meaning the pointer is last, which
   sometimes allows better parameter passing, in particular on 64-bit
   hppa. */

#define mpn_umul_ppmm  __MPN(umul_ppmm)
extern UWtype mpn_umul_ppmm _PROTO ((UWtype *, UWtype, UWtype));

#if ! defined (umul_ppmm) && HAVE_NATIVE_mpn_umul_ppmm  \
  && ! defined (LONGLONG_STANDALONE)
#define umul_ppmm(wh, wl, u, v)						      \
  do {									      \
    UWtype __umul_ppmm__p0;						      \
    (wh) = mpn_umul_ppmm (&__umul_ppmm__p0, (UWtype) (u), (UWtype) (v));      \
    (wl) = __umul_ppmm__p0;						      \
  } while (0)
#endif

#define mpn_umul_ppmm_r  __MPN(umul_ppmm_r)
extern UWtype mpn_umul_ppmm_r _PROTO ((UWtype, UWtype, UWtype *));

#if ! defined (umul_ppmm) && HAVE_NATIVE_mpn_umul_ppmm_r	\
  && ! defined (LONGLONG_STANDALONE)
#define umul_ppmm(wh, wl, u, v)						      \
  do {									      \
    UWtype __umul_ppmm__p0;						      \
    (wh) = mpn_umul_ppmm_r ((UWtype) (u), (UWtype) (v), &__umul_ppmm__p0);    \
    (wl) = __umul_ppmm__p0;						      \
  } while (0)
#endif

#define mpn_udiv_qrnnd  __MPN(udiv_qrnnd)
extern UWtype mpn_udiv_qrnnd _PROTO ((UWtype *, UWtype, UWtype, UWtype));

#if ! defined (udiv_qrnnd) && HAVE_NATIVE_mpn_udiv_qrnnd	\
  && ! defined (LONGLONG_STANDALONE)
#define udiv_qrnnd(q, r, n1, n0, d)					\
  do {									\
    UWtype __udiv_qrnnd__r;						\
    (q) = mpn_udiv_qrnnd (&__udiv_qrnnd__r,				\
			  (UWtype) (n1), (UWtype) (n0), (UWtype) d);	\
    (r) = __udiv_qrnnd__r;						\
  } while (0)
#endif

#define mpn_udiv_qrnnd_r  __MPN(udiv_qrnnd_r)
extern UWtype mpn_udiv_qrnnd_r _PROTO ((UWtype, UWtype, UWtype, UWtype *));

#if ! defined (udiv_qrnnd) && HAVE_NATIVE_mpn_udiv_qrnnd_r	\
  && ! defined (LONGLONG_STANDALONE)
#define udiv_qrnnd(q, r, n1, n0, d)					\
  do {									\
    UWtype __udiv_qrnnd__r;						\
    (q) = mpn_udiv_qrnnd_r ((UWtype) (n1), (UWtype) (n0), (UWtype) d,	\
			    &__udiv_qrnnd__r);				\
    (r) = __udiv_qrnnd__r;						\
  } while (0)
#endif


/* If this machine has no inline assembler, use C macros.  */

#if !defined (add_ssaaaa)
#define add_ssaaaa(sh, sl, ah, al, bh, bl) \
  do {									\
    UWtype __x;								\
    __x = (al) + (bl);							\
    (sh) = (ah) + (bh) + (__x < (al));					\
    (sl) = __x;								\
  } while (0)
#endif

#if !defined (sub_ddmmss)
#define sub_ddmmss(sh, sl, ah, al, bh, bl) \
  do {									\
    UWtype __x;								\
    __x = (al) - (bl);							\
    (sh) = (ah) - (bh) - ((al) < (bl));                                 \
    (sl) = __x;								\
  } while (0)
#endif

/* If we lack umul_ppmm but have smul_ppmm, define umul_ppmm in terms of
   smul_ppmm.  */
#if !defined (umul_ppmm) && defined (smul_ppmm)
#define umul_ppmm(w1, w0, u, v)						\
  do {									\
    UWtype __w1;							\
    UWtype __xm0 = (u), __xm1 = (v);					\
    smul_ppmm (__w1, w0, __xm0, __xm1);					\
    (w1) = __w1 + (-(__xm0 >> (W_TYPE_SIZE - 1)) & __xm1)		\
		+ (-(__xm1 >> (W_TYPE_SIZE - 1)) & __xm0);		\
  } while (0)
#endif

/* If we still don't have umul_ppmm, define it using plain C.

   For reference, when this code is used for squaring (ie. u and v identical
   expressions), gcc recognises __x1 and __x2 are the same and generates 3
   multiplies, not 4.  The subsequent additions could be optimized a bit,
   but the only place GMP currently uses such a square is mpn_sqr_basecase,
   and chips obliged to use this generic C umul will have plenty of worse
   performance problems than a couple of extra instructions on the diagonal
   of sqr_basecase.  */

#if !defined (umul_ppmm)
#define umul_ppmm(w1, w0, u, v)						\
  do {									\
    UWtype __x0, __x1, __x2, __x3;					\
    UHWtype __ul, __vl, __uh, __vh;					\
    UWtype __u = (u), __v = (v);					\
									\
    __ul = __ll_lowpart (__u);						\
    __uh = __ll_highpart (__u);						\
    __vl = __ll_lowpart (__v);						\
    __vh = __ll_highpart (__v);						\
									\
    __x0 = (UWtype) __ul * __vl;					\
    __x1 = (UWtype) __ul * __vh;					\
    __x2 = (UWtype) __uh * __vl;					\
    __x3 = (UWtype) __uh * __vh;					\
									\
    __x1 += __ll_highpart (__x0);/* this can't give carry */		\
    __x1 += __x2;		/* but this indeed can */		\
    if (__x1 < __x2)		/* did we get it? */			\
      __x3 += __ll_B;		/* yes, add it in the proper pos. */	\
									\
    (w1) = __x3 + __ll_highpart (__x1);					\
    (w0) = (__x1 << W_TYPE_SIZE/2) + __ll_lowpart (__x0);		\
  } while (0)
#endif

/* If we don't have smul_ppmm, define it using umul_ppmm (which surely will
   exist in one form or another.  */
#if !defined (smul_ppmm)
#define smul_ppmm(w1, w0, u, v)						\
  do {									\
    UWtype __w1;							\
    UWtype __xm0 = (u), __xm1 = (v);					\
    umul_ppmm (__w1, w0, __xm0, __xm1);					\
    (w1) = __w1 - (-(__xm0 >> (W_TYPE_SIZE - 1)) & __xm1)		\
		- (-(__xm1 >> (W_TYPE_SIZE - 1)) & __xm0);		\
  } while (0)
#endif

/* Define this unconditionally, so it can be used for debugging.  */
#define __udiv_qrnnd_c(q, r, n1, n0, d) \
  do {									\
    UWtype __d1, __d0, __q1, __q0, __r1, __r0, __m;			\
									\
    ASSERT ((d) != 0);							\
    ASSERT ((n1) < (d));						\
									\
    __d1 = __ll_highpart (d);						\
    __d0 = __ll_lowpart (d);						\
									\
    __q1 = (n1) / __d1;							\
    __r1 = (n1) - __q1 * __d1;						\
    __m = __q1 * __d0;							\
    __r1 = __r1 * __ll_B | __ll_highpart (n0);				\
    if (__r1 < __m)							\
      {									\
	__q1--, __r1 += (d);						\
	if (__r1 >= (d)) /* i.e. we didn't get carry when adding to __r1 */\
	  if (__r1 < __m)						\
	    __q1--, __r1 += (d);					\
      }									\
    __r1 -= __m;							\
									\
    __q0 = __r1 / __d1;							\
    __r0 = __r1  - __q0 * __d1;						\
    __m = __q0 * __d0;							\
    __r0 = __r0 * __ll_B | __ll_lowpart (n0);				\
    if (__r0 < __m)							\
      {									\
	__q0--, __r0 += (d);						\
	if (__r0 >= (d))						\
	  if (__r0 < __m)						\
	    __q0--, __r0 += (d);					\
      }									\
    __r0 -= __m;							\
									\
    (q) = __q1 * __ll_B | __q0;						\
    (r) = __r0;								\
  } while (0)

/* If the processor has no udiv_qrnnd but sdiv_qrnnd, go through
   __udiv_w_sdiv (defined in libgcc or elsewhere).  */
#if !defined (udiv_qrnnd) && defined (sdiv_qrnnd)
#define udiv_qrnnd(q, r, nh, nl, d) \
  do {									\
    UWtype __r;								\
    (q) = __MPN(udiv_w_sdiv) (&__r, nh, nl, d);				\
    (r) = __r;								\
  } while (0)
#endif

/* If udiv_qrnnd was not defined for this processor, use __udiv_qrnnd_c.  */
#if !defined (udiv_qrnnd)
#define UDIV_NEEDS_NORMALIZATION 1
#define udiv_qrnnd __udiv_qrnnd_c
#endif

#if !defined (count_leading_zeros)
#define count_leading_zeros(count, x) \
  do {									\
    UWtype __xr = (x);							\
    UWtype __a;								\
									\
    if (W_TYPE_SIZE == 32)						\
      {									\
	__a = __xr < ((UWtype) 1 << 2*__BITS4)				\
	  ? (__xr < ((UWtype) 1 << __BITS4) ? 1 : __BITS4 + 1)		\
	  : (__xr < ((UWtype) 1 << 3*__BITS4) ? 2*__BITS4 + 1		\
	  : 3*__BITS4 + 1);						\
      }									\
    else								\
      {									\
	for (__a = W_TYPE_SIZE - 8; __a > 0; __a -= 8)			\
	  if (((__xr >> __a) & 0xff) != 0)				\
	    break;							\
	++__a;								\
      }									\
									\
    (count) = W_TYPE_SIZE + 1 - __a - __clz_tab[__xr >> __a];		\
  } while (0)
/* This version gives a well-defined value for zero. */
#define COUNT_LEADING_ZEROS_0 (W_TYPE_SIZE - 1)
#define COUNT_LEADING_ZEROS_NEED_CLZ_TAB
#endif

/* clz_tab needed by mpn/x86/pentium/mod_1.asm in a fat binary */
#if HAVE_HOST_CPU_FAMILY_x86 && WANT_FAT_BINARY
#define COUNT_LEADING_ZEROS_NEED_CLZ_TAB
#endif

#ifdef COUNT_LEADING_ZEROS_NEED_CLZ_TAB
extern const unsigned char __GMP_DECLSPEC __clz_tab[128];
#endif

#if !defined (count_trailing_zeros)
/* Define count_trailing_zeros using count_leading_zeros.  The latter might be
   defined in asm, but if it is not, the C version above is good enough.  */
#define count_trailing_zeros(count, x) \
  do {									\
    UWtype __ctz_x = (x);						\
    UWtype __ctz_c;							\
    ASSERT (__ctz_x != 0);						\
    count_leading_zeros (__ctz_c, __ctz_x & -__ctz_x);			\
    (count) = W_TYPE_SIZE - 1 - __ctz_c;				\
  } while (0)
#endif

#ifndef UDIV_NEEDS_NORMALIZATION
#define UDIV_NEEDS_NORMALIZATION 0
#endif

/* Whether udiv_qrnnd is actually implemented with udiv_qrnnd_preinv, and
   that hence the latter should always be used.  */
#ifndef UDIV_PREINV_ALWAYS
#define UDIV_PREINV_ALWAYS 0
#endif
