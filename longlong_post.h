/* longlong.h -- definitions for mixed size 32/64 bit arithmetic.

Copyright 1991, 1992, 1993, 1994, 1996, 1997, 1999, 2000, 2001, 2002, 2003,
2004, 2005 Free Software Foundation, Inc.

Copyright 2013 William Hart

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

#if !defined (add_333)
#define add_333(sh, sm, sl, ah, am, al, bh, bm, bl)  \
   do { \
      UWtype __cy1, __cy2; \
      __cy1 = ((al) + (bl) < (al)); \
      (sl) = (al) + (bl); \
      __cy2 = ((am) + (bm) < (am)); \
      (sm) = (am) + (bm); \
      __cy2 += ((sm) + __cy1 < (sm)); \
      (sm) = (sm) + __cy1; \
      (sh) = (ah) + (bh) + __cy2; \
   } while (0)
#endif

#if !defined(sub_333)
#define sub_333(sh, sm, sl, ah, am, al, bh, bm, bl)  \
   do { \
      UWtype __cy1, __cy2; \
      __cy1 = ((al) < (bl)); \
      (sl) = (al) - (bl); \
      __cy2 = ((am) < (bm)); \
      (sm) = (am) - (bm); \
      __cy2 += ((sm) < __cy1); \
      (sm) = (sm) - __cy1; \
      (sh) = (ah) - (bh) - __cy2; \
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

#ifdef COUNT_LEADING_ZEROS_NEED_CLZ_TAB
extern const unsigned char __GMP_DECLSPEC __clz_tab[129];
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
