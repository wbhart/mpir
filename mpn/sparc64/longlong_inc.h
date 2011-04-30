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
  __asm__ (								\
       "addcc	%r4,%5,%1\n"						\
      "	addccc	%r6,%7,%%g0\n"						\
      "	addc	%r2,%3,%0"						\
	  : "=r" (sh), "=&r" (sl)					\
	  : "rJ" (ah), "rI" (bh), "%rJ" (al), "rI" (bl),		\
	    "%rJ" ((al) >> 32), "rI" ((bl) >> 32)			\
	   __CLOBBER_CC)
#define sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ (								\
       "subcc	%r4,%5,%1\n"						\
      "	subccc	%r6,%7,%%g0\n"						\
      "	subc	%r2,%3,%0"						\
	  : "=r" (sh), "=&r" (sl)					\
	  : "rJ" (ah), "rI" (bh), "rJ" (al), "rI" (bl),		\
	    "rJ" ((al) >> 32), "rI" ((bl) >> 32)			\
	   __CLOBBER_CC)

#endif

/* No processor claiming to be SPARC v9 compliant seems to
   implement the POPC instruction.  Disable pattern for now.  */
#if 0
#if ! defined(popc_limb) && defined __GNUC__ && defined __sparc_v9__ 
#define popc_limb(result, input)					\
  do {									\
    DItype __res;							\
    __asm__ ("popc %1,%0" : "=r" (result) : "rI" (input));		\
  } while (0)
#endif
#endif
