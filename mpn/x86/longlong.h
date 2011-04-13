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

#define add_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("addl %5,%k1\n\tadcl %3,%k0"					\
	   : "=r" (sh), "=&r" (sl)					\
	   : "0"  ((USItype)(ah)), "g" ((USItype)(bh)),			\
	     "%1" ((USItype)(al)), "g" ((USItype)(bl)))
#define sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("subl %5,%k1\n\tsbbl %3,%k0"					\
	   : "=r" (sh), "=&r" (sl)					\
	   : "0" ((USItype)(ah)), "g" ((USItype)(bh)),			\
	     "1" ((USItype)(al)), "g" ((USItype)(bl)))
#define umul_ppmm(w1, w0, u, v) \
  __asm__ ("mull %3"							\
	   : "=a" (w0), "=d" (w1)					\
	   : "%0" ((USItype)(u)), "rm" ((USItype)(v)))
#define udiv_qrnnd(q, r, n1, n0, dx) /* d renamed to dx avoiding "=d" */\
  __asm__ ("divl %4"		     /* stringification in K&R C */	\
	   : "=a" (q), "=d" (r)						\
	   : "0" ((USItype)(n0)), "1" ((USItype)(n1)), "rm" ((USItype)(dx)))

#if HAVE_HOST_CPU_i586 || HAVE_HOST_CPU_pentium || HAVE_HOST_CPU_pentiummmx
/* Pentium bsrl takes between 10 and 72 cycles depending where the most
   significant 1 bit is, hence the use of the following alternatives.  bsfl
   is slow too, between 18 and 42 depending where the least significant 1
   bit is, so let the generic count_trailing_zeros below make use of the
   count_leading_zeros here too.  */

#if HAVE_HOST_CPU_pentiummmx && ! defined (LONGLONG_STANDALONE)
/* The following should be a fixed 14 or 15 cycles, but possibly plus an L1
   cache miss reading from __clz_tab.  For P55 it's favoured over the float
   below so as to avoid mixing MMX and x87, since the penalty for switching
   between the two is about 100 cycles.

   The asm block sets __shift to -3 if the high 24 bits are clear, -2 for
   16, -1 for 8, or 0 otherwise.  This could be written equivalently as
   follows, but as of gcc 2.95.2 it results in conditional jumps.

       __shift = -(__n < 0x1000000);
       __shift -= (__n < 0x10000);
       __shift -= (__n < 0x100);

   The middle two sbbl and cmpl's pair, and with luck something gcc
   generates might pair with the first cmpl and the last sbbl.  The "32+1"
   constant could be folded into __clz_tab[], but it doesn't seem worth
   making a different table just for that.  */

#define count_leading_zeros(c,n)					\
  do {									\
    USItype  __n = (n);							\
    USItype  __shift;							\
    __asm__ ("cmpl  $0x1000000, %1\n"					\
	     "sbbl  %0, %0\n"						\
	     "cmpl  $0x10000, %1\n"					\
	     "sbbl  $0, %0\n"						\
	     "cmpl  $0x100, %1\n"					\
	     "sbbl  $0, %0\n"						\
	     : "=&r" (__shift) : "r"  (__n));				\
    __shift = __shift*8 + 24 + 1;					\
    (c) = 32 + 1 - __shift - __clz_tab[__n >> __shift];			\
  } while (0)
#define COUNT_LEADING_ZEROS_NEED_CLZ_TAB
#define COUNT_LEADING_ZEROS_0   31   /* n==0 indistinguishable from n==1 */

#else /* ! pentiummmx || LONGLONG_STANDALONE */
/* The following should be a fixed 14 cycles or so.  Some scheduling
   opportunities should be available between the float load/store too.  This
   sort of code is used in gcc 3 for __builtin_ffs (with "n&-n") and is
   apparently suggested by the Intel optimizing manual (don't know exactly
   where).  gcc 2.95 or up will be best for this, so the "double" is
   correctly aligned on the stack.  */
#define count_leading_zeros(c,n)					\
  do {									\
    union {								\
      double    d;							\
      unsigned  a[2];							\
    } __u;								\
    ASSERT ((n) != 0);							\
    __u.d = (UWtype) (n);						\
    (c) = 0x3FF + 31 - (__u.a[1] >> 20);				\
  } while (0)
#define COUNT_LEADING_ZEROS_0   (0x3FF + 31)
#endif /* pentiummx */

#else /* ! pentium */

#if __GMP_GNUC_PREREQ (3,4)  /* using bsrl */
#define count_leading_zeros(count,x)  count_leading_zeros_gcc_clz(count,x)
#endif /* gcc clz */

/* On P6, gcc prior to 3.0 generates a partial register stall for
   __cbtmp^31, due to using "xorb $31" instead of "xorl $31", the former
   being 1 code byte smaller.  "31-__cbtmp" is a workaround, probably at the
   cost of one extra instruction.  Do this for "i386" too, since that means
   generic x86.  */
#if ! defined (count_leading_zeros) && __GNUC__ < 3                     \
  && (HAVE_HOST_CPU_i386						\
      || HAVE_HOST_CPU_i686						\
      || HAVE_HOST_CPU_pentiumpro					\
      || HAVE_HOST_CPU_pentium2						\
      || HAVE_HOST_CPU_pentium3)
#define count_leading_zeros(count, x)					\
  do {									\
    USItype __cbtmp;							\
    ASSERT ((x) != 0);							\
    __asm__ ("bsrl %1,%0" : "=r" (__cbtmp) : "rm" ((USItype)(x)));	\
    (count) = 31 - __cbtmp;						\
  } while (0)
#endif /* gcc<3 asm bsrl */

#ifndef count_leading_zeros
#define count_leading_zeros(count, x)					\
  do {									\
    USItype __cbtmp;							\
    ASSERT ((x) != 0);							\
    __asm__ ("bsrl %1,%0" : "=r" (__cbtmp) : "rm" ((USItype)(x)));	\
    (count) = __cbtmp ^ 31;						\
  } while (0)
#endif /* asm bsrl */

#if __GMP_GNUC_PREREQ (3,4)  /* using bsfl */
#define count_trailing_zeros(count,x)  count_trailing_zeros_gcc_ctz(count,x)
#endif /* gcc ctz */

#ifndef count_trailing_zeros
#define count_trailing_zeros(count, x)					\
  do {									\
    ASSERT ((x) != 0);							\
    __asm__ ("bsfl %1,%0" : "=r" (count) : "rm" ((USItype)(x)));	\
  } while (0)
#endif /* asm bsfl */

#endif /* ! pentium */

#ifndef UMUL_TIME
#define UMUL_TIME 10
#endif
