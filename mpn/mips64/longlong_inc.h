/* longlong.h -- definitions for mixed size 32/64 bit arithmetic.

Copyright 1991, 1992, 1993, 1994, 1996, 1997, 1999, 2000, 2001, 2002, 2003,
2004, 2005 , 2007 , 2008 , 2009 Free Software Foundation, Inc.

This file is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
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
#if __mips >= 3
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 4)
#define umul_ppmm(w1, w0, u, v) \
  do {                                                                  \
    typedef unsigned int __ll_UTItype __attribute__((mode(TI)));        \
    __ll_UTItype __ll = (__ll_UTItype)(u) * (v);                        \
    w1 = __ll >> 64;                                                    \
    w0 = __ll;                                                          \
  } while (0)
#endif
#if !defined (umul_ppmm) && (__GNUC__ > 2 || __GNUC_MINOR__ >= 7)
#define umul_ppmm(w1, w0, u, v) \
  __asm__ ("dmultu %2,%3" : "=l" (w0), "=h" (w1) : "d" (u), "d" (v))
#endif
#if !defined (umul_ppmm)
#define umul_ppmm(w1, w0, u, v) \
  __asm__ ("dmultu %2,%3\n\tmflo %0\n\tmfhi %1"				\
	   : "=d" (w0), "=d" (w1) : "d" (u), "d" (v))
#endif
#endif
#endif
