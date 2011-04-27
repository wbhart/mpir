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
