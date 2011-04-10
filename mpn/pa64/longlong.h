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
  __asm__ ("add%I5 %5,%r4,%1\n\tadd,dc %r2,%r3,%0"			\
	   : "=r" (sh), "=&r" (sl)					\
	   : "rM" (ah), "rM" (bh), "%rM" (al), "rI" (bl))
#define sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("sub%I4 %4,%r5,%1\n\tsub,db %r2,%r3,%0"			\
	   : "=r" (sh), "=&r" (sl)					\
	   : "rM" (ah), "rM" (bh), "rI" (al), "rM" (bl))
