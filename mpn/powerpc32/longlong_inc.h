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

#define add_ssaaaa(sh, sl, ah, al, bh, bl) \
  do {									\
    if (__builtin_constant_p (bh) && (bh) == 0)				\
      __asm__ ("{a%I4|add%I4c} %1,%3,%4\n\t{aze|addze} %0,%2"		\
	     : "=r" (sh), "=&r" (sl) : "r" (ah), "%r" (al), "rI" (bl));\
    else if (__builtin_constant_p (bh) && (bh) == ~(USItype) 0)		\
      __asm__ ("{a%I4|add%I4c} %1,%3,%4\n\t{ame|addme} %0,%2"		\
	     : "=r" (sh), "=&r" (sl) : "r" (ah), "%r" (al), "rI" (bl));\
    else								\
      __asm__ ("{a%I5|add%I5c} %1,%4,%5\n\t{ae|adde} %0,%2,%3"		\
	     : "=r" (sh), "=&r" (sl)					\
	     : "r" (ah), "r" (bh), "%r" (al), "rI" (bl));		\
  } while (0)
#define sub_ddmmss(sh, sl, ah, al, bh, bl) \
  do {									\
    if (__builtin_constant_p (ah) && (ah) == 0)				\
      __asm__ ("{sf%I3|subf%I3c} %1,%4,%3\n\t{sfze|subfze} %0,%2"	\
	       : "=r" (sh), "=&r" (sl) : "r" (bh), "rI" (al), "r" (bl));\
    else if (__builtin_constant_p (ah) && (ah) == ~(USItype) 0)		\
      __asm__ ("{sf%I3|subf%I3c} %1,%4,%3\n\t{sfme|subfme} %0,%2"	\
	       : "=r" (sh), "=&r" (sl) : "r" (bh), "rI" (al), "r" (bl));\
    else if (__builtin_constant_p (bh) && (bh) == 0)			\
      __asm__ ("{sf%I3|subf%I3c} %1,%4,%3\n\t{ame|addme} %0,%2"		\
	       : "=r" (sh), "=&r" (sl) : "r" (ah), "rI" (al), "r" (bl));\
    else if (__builtin_constant_p (bh) && (bh) == ~(USItype) 0)		\
      __asm__ ("{sf%I3|subf%I3c} %1,%4,%3\n\t{aze|addze} %0,%2"		\
	       : "=r" (sh), "=&r" (sl) : "r" (ah), "rI" (al), "r" (bl));\
    else								\
      __asm__ ("{sf%I4|subf%I4c} %1,%5,%4\n\t{sfe|subfe} %0,%3,%2"	\
	       : "=r" (sh), "=&r" (sl)					\
	       : "r" (ah), "r" (bh), "rI" (al), "r" (bl));		\
  } while (0)
#define count_leading_zeros(count, x) \
  __asm__ ("{cntlz|cntlzw} %0,%1" : "=r" (count) : "r" (x))
#define COUNT_LEADING_ZEROS_0 32

#define umul_ppmm(ph, pl, m0, m1) \
  do {									\
    USItype __m0 = (m0), __m1 = (m1);					\
    __asm__ ("mulhwu %0,%1,%2" : "=r" (ph) : "%r" (m0), "r" (m1));	\
    (pl) = __m0 * __m1;							\
  } while (0)
#define smul_ppmm(ph, pl, m0, m1) \
  do {									\
    SItype __m0 = (m0), __m1 = (m1);					\
    __asm__ ("mulhw %0,%1,%2" : "=r" (ph) : "%r" (m0), "r" (m1));	\
    (pl) = __m0 * __m1;							\
  } while (0)

#endif

/* 3 cycles on 604 or 750 since shifts and rlwimi's can pair.  gcc (as of
   version 3.1 at least) doesn't seem to know how to generate rlwimi for
   anything other than bit-fields, so use "asm".  */
#if !defined(BSWAP_LIMB) && defined (__GNUC__)
#define BSWAP_LIMB(dst, src)						\
  do {									\
    mp_limb_t  __bswapl_src = (src);					\
    mp_limb_t  __tmp1 = __bswapl_src >> 24;		/* low byte */	\
    mp_limb_t  __tmp2 = __bswapl_src << 24;		/* high byte */	\
    __asm__ ("rlwimi %0, %2, 24, 16, 23"		/* 2nd low */	\
	 : "=r" (__tmp1) : "0" (__tmp1), "r" (__bswapl_src));		\
    __asm__ ("rlwimi %0, %2,  8,  8, 15"		/* 3nd high */	\
	 : "=r" (__tmp2) : "0" (__tmp2), "r" (__bswapl_src));		\
    (dst) = __tmp1 | __tmp2;				/* whole */	\
  } while (0)
#endif

/* Apparently lwbrx might be slow on some PowerPC chips, so restrict it to
   those we know are fast.  */
#if !defined(BSWAP_LIMB_FETCH) && defined (__GNUC__) && HAVE_LIMB_BIG_ENDIAN    
#define BSWAP_LIMB_FETCH(limb, src)					\
  do {									\
    mp_srcptr  __blf_src = (src);					\
    mp_limb_t  __limb;							\
    __asm__ ("lwbrx %0, 0, %1"						\
	     : "=r" (__limb)						\
	     : "r" (__blf_src),						\
	       "m" (*__blf_src));					\
    (limb) = __limb;							\
  } while (0)
#endif


/* On the same basis that lwbrx might be slow, restrict stwbrx to those we
   know are fast.  FIXME: Is this necessary?  */
#if !defined(BSWAP_LIMB_STORE) && defined (__GNUC__) && HAVE_LIMB_BIG_ENDIAN
#define BSWAP_LIMB_STORE(dst, limb)					\
  do {									\
    mp_ptr     __dst = (dst);						\
    mp_limb_t  __limb = (limb);						\
    __asm__ ("stwbrx %1, 0, %2"						\
	     : "=m" (*__dst)						\
	     : "r" (__limb),						\
	       "r" (__dst));						\
  } while (0)
#endif
