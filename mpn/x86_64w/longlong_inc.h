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
  __asm__ ("addq %5,%q1\n\tadcq %3,%q0"					\
	   : "=r" (sh), "=&r" (sl)					\
	   : "0"  ((UDItype)(ah)), "rme" ((UDItype)(bh)),		\
	     "%1" ((UDItype)(al)), "rme" ((UDItype)(bl)))
#define sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("subq %5,%q1\n\tsbbq %3,%q0"					\
	   : "=r" (sh), "=&r" (sl)					\
	   : "0" ((UDItype)(ah)), "rme" ((UDItype)(bh)),		\
	     "1" ((UDItype)(al)), "rme" ((UDItype)(bl)))
#define umul_ppmm(w1, w0, u, v) \
  __asm__ ("mulq %3"							\
	   : "=a" (w0), "=d" (w1)					\
	   : "%0" ((UDItype)(u)), "rm" ((UDItype)(v)))
#define udiv_qrnnd(q, r, n1, n0, dx) /* d renamed to dx avoiding "=d" */\
  __asm__ ("divq %4"		     /* stringification in K&R C */	\
	   : "=a" (q), "=d" (r)						\
	   : "0" ((UDItype)(n0)), "1" ((UDItype)(n1)), "rm" ((UDItype)(dx)))
#define add_333(sh, sm, sl, ah, am, al, bh, bm, bl)  \
  __asm__ ("addq %8,%q2\n\tadcq %6,%q1\n\tadcq %4,%q0"     \
       : "=r" (sh), "=r" (sm), "=&r" (sl)                  \
       : "0"  ((UDItype)(ah)), "rme" ((UDItype)(bh)),  \
         "1"  ((UDItype)(am)), "rme" ((UDItype)(bm)),  \
         "2"  ((UDItype)(al)), "rme" ((UDItype)(bl)))  
#define sub_333(sh, sm, sl, ah, am, al, bh, bm, bl)  \
  __asm__ ("subq %8,%q2\n\tsbbq %6,%q1\n\tsbbq %4,%q0"     \
       : "=r" (sh), "=r" (sm), "=&r" (sl)                  \
       : "0"  ((UDItype)(ah)), "rme" ((UDItype)(bh)),  \
         "1"  ((UDItype)(am)), "rme" ((UDItype)(bm)),  \
         "2"  ((UDItype)(al)), "rme" ((UDItype)(bl)))  

/* bsrq destination must be a 64-bit register, hence UDItype for __cbtmp. */
#define count_leading_zeros(count, x)					\
  do {									\
    UDItype __cbtmp;							\
    ASSERT ((x) != 0);							\
    __asm__ ("bsrq %1,%0" : "=r" (__cbtmp) : "rm" ((UDItype)(x)));	\
    (count) = __cbtmp ^ 63;						\
  } while (0)
/* bsfq destination must be a 64-bit register, "%q0" forces this in case
   count is only an int. */
#define count_trailing_zeros(count, x)					\
  do {									\
    ASSERT ((x) != 0);							\
    __asm__ ("bsfq %1,%q0" : "=r" (count) : "rm" ((UDItype)(x)));	\
  } while (0)

#endif

#if !defined(BSWAP_LIMB) && defined (__GNUC__) 
#define BSWAP_LIMB(dst, src)						\
  do {									\
    __asm__ ("bswap %q0" : "=r" (dst) : "0" (src));			\
  } while (0)
#endif

#if defined( _MSC_VER )
#include <intrin.h>

#if !defined( COUNT_LEADING_ZEROS_NEED_CLZ_TAB )
#  define COUNT_LEADING_ZEROS_NEED_CLZ_TAB
#endif

#if defined( _WIN64 )

#if !defined(count_leading_zeros)
#   pragma intrinsic(_BitScanReverse64)
#   define count_leading_zeros(c,x)	    \
    do  { unsigned long _z;	            \
          ASSERT ((x) != 0);            \
          _BitScanReverse64(&_z, (x));  \
          c = 63 - _z;                  \
        } while (0)
#endif

#if !defined(count_trailing_zeros)
#   pragma intrinsic(_BitScanForward64)
#   define count_trailing_zeros(c,x)	\
    do  {  unsigned long _z;		 	\
           ASSERT ((x) != 0);           \
           _BitScanForward64(&_z, (x));	\
		   c = _z;						\
         } while (0)
#endif

#if !defined(umul_ppmm)
#    pragma intrinsic(_umul128)
#    define umul_ppmm(xh, xl, m0, m1)       \
     do  {                                  \
           xl = _umul128( (m0), (m1), &xh); \
         } while (0)
#endif

#if !defined( BSWAP_LIMB )
#  pragma intrinsic(_byteswap_uint64)
#    define BSWAP_LIMB(dst, src)  dst = _byteswap_uint64(src)
#endif

#endif    /* _WIN64 */

#if 0	/* trial re-definition of C versions */
#define add_333(sh, sm, sl, ah, am, al, bh, bm, bl)  \
   do { \
      UWtype _cy; \
      (sl) = (al) + (bl); \
	  _cy = (sl) < (bl) ? 1 : 0; \
	  (sm) = (am) + (bm) + _cy; \
	  _cy  = (sm) < (bm) ? 1 : (sm) > (bm) ? 0 : _cy; \
	  (sh) = (ah) + (bh) + _cy; \
   } while (0)

#define sub_333(sh, sm, sl, ah, am, al, bh, bm, bl)  \
   do { \
      UWtype _bw; \
      (sl) = (al) - (bl); \
	  _bw = (sl) > (al) ? 1 : 0; \
	  (sm) = (am) - (bm) - _bw; \
	  _bw = (sm) > (am) ? 1 : (sm) < (am) ? 0 : _bw; \
	  (sh) = (ah) - (bh) - _bw; \
   } while (0)
#endif

#endif      /* _MSC_VER */
