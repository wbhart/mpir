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

#if defined(__ICL)

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
#define add_333(sh, sm, sl, ah, am, al, bh, bm, bl)  \
  __asm__ ("addl %8,%q2\n\tadcl %6,%q1\n\tadcl %4,%q0"     \
       : "=r" (sh), "=r" (sm), "=&r" (sl)                  \
       : "0"  ((USItype)(ah)), "rme" ((USItype)(bh)),  \
         "1"  ((USItype)(am)), "rme" ((USItype)(bm)),  \
         "2"  ((USItype)(al)), "rme" ((USItype)(bl)))  
#define sub_333(sh, sm, sl, ah, am, al, bh, bm, bl)  \
  __asm__ ("subl %8,%q2\n\tsbbl %6,%q1\n\tsbbl %4,%q0"     \
       : "=r" (sh), "=r" (sm), "=&r" (sl)                  \
       : "0"  ((USItype)(ah)), "rme" ((USItype)(bh)),  \
         "1"  ((USItype)(am)), "rme" ((USItype)(bm)),  \
         "2"  ((USItype)(al)), "rme" ((USItype)(bl)))  
#define umul_ppmm(w1, w0, u, v) \
  __asm__ ("mull %3"							\
	   : "=a" (w0), "=d" (w1)					\
	   : "%0" ((USItype)(u)), "rm" ((USItype)(v)))
#define udiv_qrnnd(q, r, n1, n0, dx) /* d renamed to dx avoiding "=d" */\
  __asm__ ("divl %4"		     /* stringification in K&R C */	\
	   : "=a" (q), "=d" (r)						\
	   : "0" ((USItype)(n0)), "1" ((USItype)(n1)), "rm" ((USItype)(dx)))


#if __GMP_GNUC_PREREQ (3,4)  /* using bsrl */
#define count_leading_zeros(count,x)  count_leading_zeros_gcc_clz(count,x)
#endif /* gcc clz */

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


/* ASM_L gives a local label for a gcc asm block, for use when temporary
   local labels like "1:" might not be available, which is the case for
   instance on the x86s (the SCO assembler doesn't support them).

   The label generated is made unique by including "%=" which is a unique
   number for each insn.  This ensures the same name can be used in multiple
   asm blocks, perhaps via a macro.  Since jumps between asm blocks are not
   allowed there's no need for a label to be usable outside a single
   block.  */

#define ASM_L(name)  LSYM_PREFIX "asm_%=_" #name

#if ! WANT_ASSERT
/* Better flags handling than the generic C gives on i386, saving a few
   bytes of code and maybe a cycle or two.  */

#define MPN_IORD_U(ptr, incr, aors)					\
  do {									\
    mp_ptr  __ptr_dummy;						\
    if (__builtin_constant_p (incr) && (incr) == 1)			\
      {									\
        __asm__ __volatile__						\
          ("\n" ASM_L(top) ":\n"					\
           "\t" aors " $1, (%0)\n"					\
           "\tleal 4(%0),%0\n"						\
           "\tjc " ASM_L(top)						\
           : "=r" (__ptr_dummy)						\
           : "0"  (ptr)							\
           : "memory");							\
      }									\
    else								\
      {									\
        __asm__ __volatile__						\
          (   aors  " %2,(%0)\n"					\
           "\tjnc " ASM_L(done) "\n"					\
           ASM_L(top) ":\n"						\
           "\t" aors " $1,4(%0)\n"					\
           "\tleal 4(%0),%0\n"						\
           "\tjc " ASM_L(top) "\n"					\
           ASM_L(done) ":\n"						\
           : "=r" (__ptr_dummy)						\
           : "0"  (ptr),						\
             "ri" (incr)						\
           : "memory");							\
      }									\
  } while (0)

#ifndef MPN_INCR_U
#define MPN_INCR_U(ptr, size, incr)  MPN_IORD_U (ptr, incr, "addl")
#endif
#ifndef MPN_DECR_U
#define MPN_DECR_U(ptr, size, incr)  MPN_IORD_U (ptr, incr, "subl")
#endif
#ifndef mpn_incr_u
#define mpn_incr_u(ptr, incr)  MPN_INCR_U (ptr, 0, incr)
#endif
#ifndef mpn_decr_u
#define mpn_decr_u(ptr, incr)  MPN_DECR_U (ptr, 0, incr)
#endif
#endif

#endif 

#if defined (__GNUC__)
#if __GMP_GNUC_PREREQ (3,1)
#define __GMP_qm "=Qm"
#define __GMP_q "=Q"
#else
#define __GMP_qm "=qm"
#define __GMP_q "=q"
#endif
#ifndef ULONG_PARITY
#define ULONG_PARITY(p, n)						\
  do {									\
    char	   __p;							\
    unsigned long  __n = (n);						\
    __n ^= (__n >> 16);							\
    __asm__ ("xorb %h1, %b1\n\t"					\
	     "setpo %0"							\
	 : __GMP_qm (__p), __GMP_q (__n)				\
	 : "1" (__n));							\
    (p) = __p;								\
  } while (0)
#endif
#endif

#if !defined(BSWAP_LIMB) && defined (__GNUC__)
#define BSWAP_LIMB(dst, src)						\
  do {									\
    __asm__ ("bswap %0" : "=r" (dst) : "0" (src));			\
  } while (0)
#endif

#if defined( _MSC_VER )
#  if !defined(COUNT_LEADING_ZEROS_NEED_CLZ_TAB)
#    define COUNT_LEADING_ZEROS_NEED_CLZ_TAB
#  endif
#endif
