/* Include file for internal GNU MP types and definitions.

   THE CONTENTS OF THIS FILE ARE FOR INTERNAL USE AND ARE ALMOST CERTAIN TO
   BE SUBJECT TO INCOMPATIBLE CHANGES IN FUTURE GNU MP RELEASES.

Copyright 1991, 1993-1997, 1999, 2000-2015 Free Software Foundation, Inc.

Copyright 2009, 2013 William Hart


This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library.  If not, see http://www.gnu.org/licenses/.  */



/* __GMP_DECLSPEC must be given on any global data that will be accessed
   from outside libmpir, meaning from the test or development programs, or
   from libmpirxx.  Failing to do this will result in an incorrect address
   being used for the accesses.  On functions __GMP_DECLSPEC makes calls
   from outside libmpir more efficient, but they'll still work fine without
   it.  */


#ifndef __GMP_IMPL_H__
#define __GMP_IMPL_H__

/* limits.h is not used in general, since it's an ANSI-ism, and since on
   solaris gcc 2.95 under -mcpu=ultrasparc in ABI=32 ends up getting wrong
   values (the ABI=64 values).

   On Cray vector systems, however, we need the system limits.h since sizes
   of signed and unsigned types can differ there, depending on compiler
   options (eg. -hnofastmd), making our SHRT_MAX etc expressions fail.  For
   reference, int can be 46 or 64 bits, whereas uint is always 64 bits; and
   short can be 24, 32, 46 or 64 bits, and different for ushort.  */

#if defined _WIN64
#include <limits.h>
#endif

/* For fat.h and other fat binary stuff.
   No need for __GMP_ATTRIBUTE_PURE or __GMP_NOTHROW, since functions
   declared this way are only used to set function pointers in __gmp_cpuvec,
   they're not called directly.  */
#define DECL_add_err1_n(name) \
  mp_limb_t name __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_ptr, mp_srcptr, mp_size_t, mp_limb_t))
#define DECL_add_err2_n(name) \
  mp_limb_t name __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_ptr, mp_srcptr, mp_srcptr, mp_size_t, mp_limb_t))
#define DECL_add_n(name) \
  mp_limb_t name __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_size_t))
#define DECL_addmul_1(name) \
  mp_limb_t name __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_limb_t))
#define DECL_copyd(name) \
  void name __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t))
#define DECL_copyi(name) \
  void name __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t))
#define DECL_divexact_1(name) \
  void name __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_limb_t))
#define DECL_divexact_by3c(name) \
  mp_limb_t name __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_limb_t))
#define DECL_divexact_byfobm1(name) \
  mp_limb_t name __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_limb_t,mp_limb_t))
#define DECL_divrem_1(name) \
  mp_limb_t name __GMP_PROTO ((mp_ptr, mp_size_t, mp_srcptr, mp_size_t, mp_limb_t))
#define DECL_divrem_2(name) \
  mp_limb_t name __GMP_PROTO ((mp_ptr, mp_size_t, mp_ptr, mp_size_t, mp_srcptr))
#define DECL_divrem_euclidean_qr_1(name) \
  mp_limb_t name __GMP_PROTO ((mp_ptr, mp_size_t, mp_srcptr, mp_size_t, mp_limb_t))
#define DECL_divrem_euclidean_qr_2(name) \
  mp_limb_t name __GMP_PROTO ((mp_ptr, mp_ptr, mp_size_t, mp_srcptr))
#define DECL_gcd_1(name) \
  mp_limb_t name __GMP_PROTO ((mp_srcptr, mp_size_t, mp_limb_t))
#define DECL_lshift(name) \
  mp_limb_t name __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t, unsigned))
#define DECL_mod_1(name) \
  mp_limb_t name __GMP_PROTO ((mp_srcptr, mp_size_t, mp_limb_t))
#define DECL_mod_34lsub1(name) \
  mp_limb_t name __GMP_PROTO ((mp_srcptr, mp_size_t))
#define DECL_modexact_1c_odd(name) \
  mp_limb_t name __GMP_PROTO ((mp_srcptr, mp_size_t, mp_limb_t, mp_limb_t))
#define DECL_mul_1(name) \
  DECL_addmul_1 (name)
#define DECL_mul_basecase(name) \
  void name __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_srcptr, mp_size_t))
#define DECL_mulmid_basecase(name) \
  void name __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_srcptr, mp_size_t))
#define DECL_preinv_divrem_1(name) \
  mp_limb_t name __GMP_PROTO ((mp_ptr, mp_size_t, mp_srcptr, mp_size_t, mp_limb_t, mp_limb_t, int))
#define DECL_preinv_mod_1(name) \
  mp_limb_t name __GMP_PROTO ((mp_srcptr, mp_size_t, mp_limb_t, mp_limb_t))
#define DECL_redc_1(name) \
  void name __GMP_PROTO ((mp_ptr, mp_ptr, mp_srcptr, mp_size_t, mp_limb_t))
#define DECL_rshift(name) \
  DECL_lshift (name)
#define DECL_sqr_basecase(name) \
  void name __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t))
#define DECL_sub_err1_n(name) \
  mp_limb_t name __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_ptr, mp_srcptr, mp_size_t, mp_limb_t))
#define DECL_sub_err2_n(name) \
  mp_limb_t name __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_ptr, mp_srcptr, mp_srcptr, mp_size_t, mp_limb_t))
#define DECL_sub_n(name) \
  DECL_add_n (name)
#define DECL_submul_1(name) \
  DECL_addmul_1 (name)
#define DECL_sumdiff_n(name) \
  mp_limb_t name __GMP_PROTO ((mp_ptr, mp_ptr, mp_srcptr,mp_srcptr,mp_size_t))
#define DECL_nsumdiff_n(name) \
  DECL_sumdiff_n(name)

#if ! __GMP_WITHIN_CONFIGURE
#include "config.h"
#include "gmp-mparam.h"

/* These constants are generated by gen-fib.c header limbbits nailbits */
#if GMP_NUMB_BITS == 32
#define FIB_TABLE_LIMIT         47
#define FIB_TABLE_LUCNUM_LIMIT  46
#endif /* 32 bits */
#if GMP_NUMB_BITS == 64
#define FIB_TABLE_LIMIT         93
#define FIB_TABLE_LUCNUM_LIMIT  92
#endif /* 64 bits */

/* This constants are generated by gen-bases.c header limbbits nailbits */
#if GMP_NUMB_BITS == 32
#define MP_BASES_CHARS_PER_LIMB_10      9
#define MP_BASES_BIG_BASE_10            CNST_LIMB(0x3b9aca00)
#define MP_BASES_BIG_BASE_INVERTED_10   CNST_LIMB(0x12e0be82)
#define MP_BASES_NORMALIZATION_STEPS_10 2
#endif /* 32 bits */
#if GMP_NUMB_BITS == 64
#define MP_BASES_CHARS_PER_LIMB_10      19
#define MP_BASES_BIG_BASE_10            CNST_LIMB(0x8ac7230489e80000)
#define MP_BASES_BIG_BASE_INVERTED_10   CNST_LIMB(0xd83c94fb6d2ac34a)
#define MP_BASES_NORMALIZATION_STEPS_10 0
#endif /* 64 bits */

#if defined _LONG_LONG_LIMB
#if __GMP_HAVE_TOKEN_PASTE
#define CNST_LIMB(C) ((mp_limb_t) C##LL)
#else
#define CNST_LIMB(C) ((mp_limb_t) C/**/LL)
#endif
#else /* not _LONG_LONG_LIMB */
#if __GMP_HAVE_TOKEN_PASTE
#define CNST_LIMB(C) ((mp_limb_t) C##L)
#else
#define CNST_LIMB(C) ((mp_limb_t) C/**/L)
#endif
#endif /* _LONG_LONG_LIMB */

/* This constants and defines are generated by gen-psqr limbbits nailbits */
#if GMP_LIMB_BITS == 32 && GMP_NAIL_BITS == 0
/* Non-zero bit indicates a quadratic residue mod 0x100.
   This test identifies 82.81% as non-squares (212/256). */
static const mp_limb_t
sq_res_0x100[8] = {
  CNST_LIMB(0x2030213),
  CNST_LIMB(0x2020212),
  CNST_LIMB(0x2020213),
  CNST_LIMB(0x2020212),
  CNST_LIMB(0x2030212),
  CNST_LIMB(0x2020212),
  CNST_LIMB(0x2020212),
  CNST_LIMB(0x2020212),
};

/* 2^24-1 = 3^2 * 5 * 7 * 13 * 17 ... */
#define PERFSQR_MOD_BITS  25

/* This test identifies 95.66% as non-squares. */
#define PERFSQR_MOD_TEST(up, usize) \
  do {                              \
    mp_limb_t  r;                   \
    PERFSQR_MOD_34 (r, up, usize);  \
                                    \
    /* 73.33% */                    \
    PERFSQR_MOD_2 (r, CNST_LIMB(45), CNST_LIMB(0xfa4fa5), \
                   CNST_LIMB(0x920), CNST_LIMB(0x1a442481)); \
                                    \
    /* 47.06% */                    \
    PERFSQR_MOD_1 (r, CNST_LIMB(17), CNST_LIMB(0xf0f0f1), \
                   CNST_LIMB(0x1a317)); \
                                    \
    /* 46.15% */                    \
    PERFSQR_MOD_1 (r, CNST_LIMB(13), CNST_LIMB(0xec4ec5), \
                   CNST_LIMB(0x9e5)); \
                                    \
    /* 42.86% */                    \
    PERFSQR_MOD_1 (r, CNST_LIMB( 7), CNST_LIMB(0xdb6db7), \
                   CNST_LIMB(0x69)); \
  } while (0)

/* Grand total sq_res_0x100 and PERFSQR_MOD_TEST, 99.25% non-squares. */

/* helper for tests/mpz/t-perfsqr.c */
#define PERFSQR_DIVISORS  { 256, 45, 17, 13, 7, }

#elif GMP_LIMB_BITS == 64 && GMP_NAIL_BITS == 0

/* Non-zero bit indicates a quadratic residue mod 0x100.
   This test identifies 82.81% as non-squares (212/256). */
static const mp_limb_t
sq_res_0x100[4] = {
  CNST_LIMB(0x202021202030213),
  CNST_LIMB(0x202021202020213),
  CNST_LIMB(0x202021202030212),
  CNST_LIMB(0x202021202020212),
};

/* 2^48-1 = 3^2 * 5 * 7 * 13 * 17 * 97 ... */
#define PERFSQR_MOD_BITS  49

/* This test identifies 97.81% as non-squares. */
#define PERFSQR_MOD_TEST(up, usize) \
  do {                              \
    mp_limb_t  r;                   \
    PERFSQR_MOD_34 (r, up, usize);  \
                                    \
    /* 69.23% */                    \
    PERFSQR_MOD_2 (r, CNST_LIMB(91), CNST_LIMB(0xfd2fd2fd2fd3), \
                   CNST_LIMB(0x2191240), CNST_LIMB(0x8850a206953820e1)); \
                                    \
    /* 68.24% */                    \
    PERFSQR_MOD_2 (r, CNST_LIMB(85), CNST_LIMB(0xfcfcfcfcfcfd), \
                   CNST_LIMB(0x82158), CNST_LIMB(0x10b48c4b4206a105)); \
                                    \
    /* 55.56% */                    \
    PERFSQR_MOD_1 (r, CNST_LIMB( 9), CNST_LIMB(0xe38e38e38e39), \
                   CNST_LIMB(0x93)); \
                                    \
    /* 49.48% */                    \
    PERFSQR_MOD_2 (r, CNST_LIMB(97), CNST_LIMB(0xfd5c5f02a3a1), \
                   CNST_LIMB(0x1eb628b47), CNST_LIMB(0x6067981b8b451b5f)); \
  } while (0)

/* Grand total sq_res_0x100 and PERFSQR_MOD_TEST, 99.62% non-squares. */

/* helper for tests/mpz/t-perfsqr.c */
#define PERFSQR_DIVISORS  { 256, 91, 85, 9, 97, }

#else
#error no data available for this limb size in perfsqr.h
#endif

#if WANT_FAT_BINARY
#include "fat.h"
#endif
#endif

#if HAVE_INTTYPES_H      /* for uint_least32_t */
# include <inttypes.h>
#else
# ifdef HAVE_STDINT_H
#  include <stdint.h>
# endif
#endif

#ifdef __cplusplus
#include <cstring>  /* for strlen */
#include <string>   /* for std::string */
#endif


#ifndef WANT_TMP_DEBUG  /* for TMP_ALLOC_LIMBS_2 and others */
#define WANT_TMP_DEBUG 0
#endif


/* Might search and replace _PROTO to __GMP_PROTO internally one day, to
   avoid two names for one thing, but no hurry for that.  */
#define _PROTO(x)  __GMP_PROTO(x)

/* The following tries to get a good version of alloca.  The tests are
   adapted from autoconf AC_FUNC_ALLOCA, with a couple of additions.
   Whether this succeeds is tested by GMP_FUNC_ALLOCA and HAVE_ALLOCA will
   be setup appropriately.

   ifndef alloca - a cpp define might already exist.
       glibc <stdlib.h> includes <alloca.h> which uses GCC __builtin_alloca.
       HP cc +Olibcalls adds a #define of alloca to __builtin_alloca.

   GCC __builtin_alloca - preferred whenever available.

   _AIX pragma - IBM compilers need a #pragma in "each module that needs to
       use alloca".  Pragma indented to protect pre-ANSI cpp's.  _IBMR2 was
       used in past versions of GMP, retained still in case it matters.

       The autoconf manual says this pragma needs to be at the start of a C
       file, apart from comments and preprocessor directives.  Is that true?
       xlc on aix 4.xxx doesn't seem to mind it being after prototypes etc
       from mpir.h.
*/

#ifndef alloca
# ifdef __GNUC__
#  define alloca __builtin_alloca
# else
#  ifdef __DECC
#   define alloca(x) __ALLOCA(x)
#  else
#   ifdef _MSC_VER
#    include <malloc.h>
#    define alloca _alloca
#   else
#    if HAVE_ALLOCA_H
#     include <alloca.h>
#    else
#     if defined (_AIX) || defined (_IBMR2)
 #pragma alloca
#     else
       char *alloca ();
#     endif
#    endif
#   endif
#  endif
# endif
#endif


/* if not provided by gmp-mparam.h */
#ifndef BYTES_PER_MP_LIMB
#define BYTES_PER_MP_LIMB  SIZEOF_MP_LIMB_T
#endif
#ifndef BITS_PER_MP_LIMB
#define BITS_PER_MP_LIMB  (8 * SIZEOF_MP_LIMB_T)
#endif

#define BITS_PER_ULONG   (8 * SIZEOF_UNSIGNED_LONG)
#ifdef HAVE_STDINT_H
#define BITS_PER_UINTMAX (8 * SIZEOF_UINTMAX_T)
#endif

/* gmp_uint_least32_t is an unsigned integer type with at least 32 bits. */
#if HAVE_UINT_LEAST32_T
typedef uint_least32_t      gmp_uint_least32_t;
#else
#if SIZEOF_UNSIGNED_SHORT >= 4
typedef unsigned short      gmp_uint_least32_t;
#else
#if SIZEOF_UNSIGNED >= 4
typedef unsigned            gmp_uint_least32_t;
#else
typedef unsigned long       gmp_uint_least32_t;
#endif
#endif
#endif

/* pre-inverse types for truncating division and modulo */
typedef struct {mp_limb_t inv32;} gmp_pi1_t;
typedef struct {mp_limb_t inv21, inv32, inv53;} gmp_pi2_t;



/* const and signed must match __gmp_const and __gmp_signed, so follow the
   decision made for those in mpir.h.    */
#if ! __GMP_HAVE_CONST
#define const   /* empty */
#define signed  /* empty */
#endif

/* "const" basically means a function does nothing but examine its arguments
   and give a return value, it doesn't read or write any memory (neither
   global nor pointed to by arguments), and has no other side-effects.  This
   is more restrictive than "pure".  See info node "(gcc)Function
   Attributes".  __GMP_NO_ATTRIBUTE_CONST_PURE lets tune/common.c etc turn
   this off when trying to write timing loops.  */
#if HAVE_ATTRIBUTE_CONST && ! defined (__GMP_NO_ATTRIBUTE_CONST_PURE) && !( defined (__cplusplus) && defined (__sun))
#define ATTRIBUTE_CONST  __attribute__ ((const))
#else
#define ATTRIBUTE_CONST
#endif

#if HAVE_ATTRIBUTE_NORETURN && !( defined (__cplusplus) && defined (__sun))
#define ATTRIBUTE_NORETURN  __attribute__ ((noreturn))
#else
#define ATTRIBUTE_NORETURN
#endif

/* "malloc" means a function behaves like malloc in that the pointer it
   returns doesn't alias anything.  */
#if HAVE_ATTRIBUTE_MALLOC && !( defined (__cplusplus) && defined (__sun))
#define ATTRIBUTE_MALLOC  __attribute__ ((malloc))
#else
#define ATTRIBUTE_MALLOC
#endif


#if ! HAVE_STRCHR
#define strchr(s,c)  index(s,c)
#endif

#if ! HAVE_MEMSET
#define memset(p, c, n)                 \
  do {                                  \
    ASSERT ((n) >= 0);                  \
    char *__memset__p = (p);            \
    int  __i;                           \
    for (__i = 0; __i < (n); __i++)     \
      __memset__p[__i] = (c);           \
  } while (0)
#endif

/* va_copy is standard in C99, and gcc provides __va_copy when in strict C89
   mode.  Falling back to a memcpy will give maximum portability, since it
   works no matter whether va_list is a pointer, struct or array.  */
#if ! defined (va_copy) && defined (__va_copy)
#define va_copy(dst,src)  __va_copy(dst,src)
#endif
#if ! defined (va_copy)
#define va_copy(dst,src) \
  do { memcpy (&(dst), &(src), sizeof (va_list)); } while (0)
#endif

#if defined (__cplusplus)
extern "C" {
#endif


/* Usage: TMP_DECL;
          TMP_MARK;
          ptr = TMP_ALLOC (bytes);
          TMP_FREE;

   Small allocations should use TMP_SALLOC, big allocations should use
   TMP_BALLOC.  Allocations that might be small or big should use TMP_ALLOC.

   Functions that use just TMP_SALLOC should use TMP_SDECL, TMP_SMARK, and
   TMP_SFREE.

   TMP_DECL just declares a variable, but might be empty and so must be last
   in a list of variables.  TMP_MARK must be done before any TMP_ALLOC.
   TMP_ALLOC(0) is not allowed.  TMP_FREE doesn't need to be done if a
   TMP_MARK was made, but then no TMP_ALLOCs.  */

/* The alignment in bytes, used for TMP_ALLOCed blocks, when alloca or
   __gmp_allocate_func doesn't already determine it.  Currently TMP_ALLOC
   isn't used for "double"s, so that's not in the union.  */
union tmp_align_t {
  mp_limb_t  l;
  char       *p;
};
#define __TMP_ALIGN  sizeof (union tmp_align_t)

/* Return "a" rounded upwards to a multiple of "m", if it isn't already.
   "a" must be an unsigned type.
   This is designed for use with a compile-time constant "m".
   The POW2 case is expected to be usual, and gcc 3.0 and up recognises
   "(-(8*n))%8" or the like is always zero, which means the rounding up in
   the WANT_TMP_NOTREENTRANT version of TMP_ALLOC below will be a noop.  */
#define ROUND_UP_MULTIPLE(a,m)          \
  (POW2_P(m) ? (a) + (-(a))%(m)         \
   : (a)+(m)-1 - (((a)+(m)-1) % (m)))

#if defined (WANT_TMP_ALLOCA) || defined (WANT_TMP_REENTRANT)
struct tmp_reentrant_t {
  struct tmp_reentrant_t  *next;
  size_t		  size;	  /* bytes, including header */
};
__GMP_DECLSPEC void *__gmp_tmp_reentrant_alloc _PROTO ((struct tmp_reentrant_t **, size_t)) ATTRIBUTE_MALLOC;
__GMP_DECLSPEC void  __gmp_tmp_reentrant_free _PROTO ((struct tmp_reentrant_t *));
#endif

#if WANT_TMP_ALLOCA
#define TMP_SDECL
#define TMP_DECL		struct tmp_reentrant_t *__tmp_marker
#define TMP_SMARK
#define TMP_MARK		__tmp_marker = 0
#define TMP_SALLOC(n)		alloca(n)
#define TMP_BALLOC(n)		__gmp_tmp_reentrant_alloc (&__tmp_marker, n)
#define TMP_ALLOC(n)							\
  (LIKELY ((n) < 65536) ? TMP_SALLOC(n) : TMP_BALLOC(n))
#define TMP_SFREE
#define TMP_FREE							   \
  do {									   \
    if (UNLIKELY (__tmp_marker != 0)) __gmp_tmp_reentrant_free (__tmp_marker); \
  } while (0)
#endif

#if WANT_TMP_REENTRANT
#define TMP_SDECL		TMP_DECL
#define TMP_DECL		struct tmp_reentrant_t *__tmp_marker
#define TMP_SMARK		TMP_MARK
#define TMP_MARK		__tmp_marker = 0
#define TMP_SALLOC(n)		TMP_ALLOC(n)
#define TMP_BALLOC(n)		TMP_ALLOC(n)
#define TMP_ALLOC(n)		__gmp_tmp_reentrant_alloc (&__tmp_marker, n)
#define TMP_SFREE		TMP_FREE
#define TMP_FREE		__gmp_tmp_reentrant_free (__tmp_marker)
#endif

#if WANT_TMP_NOTREENTRANT
struct tmp_marker
{
  struct tmp_stack *which_chunk;
  void *alloc_point;
};
__GMP_DECLSPEC void *__gmp_tmp_alloc _PROTO ((unsigned long)) ATTRIBUTE_MALLOC;
__GMP_DECLSPEC void __gmp_tmp_mark _PROTO ((struct tmp_marker *));
__GMP_DECLSPEC void __gmp_tmp_free _PROTO ((struct tmp_marker *));
#define TMP_SDECL		TMP_DECL
#define TMP_DECL		struct tmp_marker __tmp_marker
#define TMP_SMARK		TMP_MARK
#define TMP_MARK		__gmp_tmp_mark (&__tmp_marker)
#define TMP_SALLOC(n)		TMP_ALLOC(n)
#define TMP_BALLOC(n)		TMP_ALLOC(n)
#define TMP_ALLOC(n)							\
  __gmp_tmp_alloc (ROUND_UP_MULTIPLE ((unsigned long) (n), __TMP_ALIGN))
#define TMP_SFREE		TMP_FREE
#define TMP_FREE		__gmp_tmp_free (&__tmp_marker)
#endif

#if WANT_TMP_DEBUG
/* See tal-debug.c for some comments. */
struct tmp_debug_t {
  struct tmp_debug_entry_t  *list;
  const char                *file;
  int                       line;
};
struct tmp_debug_entry_t {
  struct tmp_debug_entry_t  *next;
  char                      *block;
  size_t                    size;
};
__GMP_DECLSPEC void  __gmp_tmp_debug_mark  _PROTO ((const char *, int, struct tmp_debug_t **,
                                     struct tmp_debug_t *,
                                     const char *, const char *));
__GMP_DECLSPEC void *__gmp_tmp_debug_alloc _PROTO ((const char *, int, int,
                                     struct tmp_debug_t **, const char *,
                                     size_t)) ATTRIBUTE_MALLOC;
__GMP_DECLSPEC void  __gmp_tmp_debug_free  _PROTO ((const char *, int, int,
                                     struct tmp_debug_t **,
                                     const char *, const char *));
#define TMP_SDECL TMP_DECL_NAME(__tmp_xmarker, "__tmp_marker")
#define TMP_DECL TMP_DECL_NAME(__tmp_xmarker, "__tmp_marker")
#define TMP_SMARK TMP_MARK_NAME(__tmp_xmarker, "__tmp_marker")
#define TMP_MARK TMP_MARK_NAME(__tmp_xmarker, "__tmp_marker")
#define TMP_SFREE TMP_FREE_NAME(__tmp_xmarker, "__tmp_marker")
#define TMP_FREE TMP_FREE_NAME(__tmp_xmarker, "__tmp_marker")
/* The marker variable is designed to provoke an uninitialized varialble
   warning from the compiler if TMP_FREE is used without a TMP_MARK.
   __tmp_marker_inscope does the same for TMP_ALLOC.  Runtime tests pick
   these things up too.  */
#define TMP_DECL_NAME(marker, marker_name)                      \
  int marker;                                                   \
  int __tmp_marker_inscope;                                     \
  const char *__tmp_marker_name = marker_name;                  \
  struct tmp_debug_t  __tmp_marker_struct;                      \
  /* don't demand NULL, just cast a zero */                     \
  struct tmp_debug_t  *__tmp_marker = (struct tmp_debug_t *) 0
#define TMP_MARK_NAME(marker, marker_name)                      \
  do {                                                          \
    marker = 1;                                                 \
    __tmp_marker_inscope = 1;                                   \
    __gmp_tmp_debug_mark  (ASSERT_FILE, ASSERT_LINE,            \
                           &__tmp_marker, &__tmp_marker_struct, \
                           __tmp_marker_name, marker_name);     \
  } while (0)
#define TMP_SALLOC(n)		TMP_ALLOC(n)
#define TMP_BALLOC(n)		TMP_ALLOC(n)
#define TMP_ALLOC(size)                                                 \
  __gmp_tmp_debug_alloc (ASSERT_FILE, ASSERT_LINE,                      \
                         __tmp_marker_inscope,                          \
                         &__tmp_marker, __tmp_marker_name, size)
#define TMP_FREE_NAME(marker, marker_name)                      \
  do {                                                          \
    __gmp_tmp_debug_free  (ASSERT_FILE, ASSERT_LINE,            \
                           marker, &__tmp_marker,               \
                           __tmp_marker_name, marker_name);     \
  } while (0)
#endif /* WANT_TMP_DEBUG */


/* Allocating various types. */
#define TMP_ALLOC_TYPE(n,type)  ((type *) TMP_ALLOC ((n) * sizeof (type)))
#define TMP_SALLOC_TYPE(n,type) ((type *) TMP_SALLOC ((n) * sizeof (type)))
#define TMP_BALLOC_TYPE(n,type) ((type *) TMP_BALLOC ((n) * sizeof (type)))
#define TMP_ALLOC_LIMBS(n)      TMP_ALLOC_TYPE(n,mp_limb_t)
#define TMP_SALLOC_LIMBS(n)     TMP_SALLOC_TYPE(n,mp_limb_t)
#define TMP_BALLOC_LIMBS(n)     TMP_BALLOC_TYPE(n,mp_limb_t)
#define TMP_ALLOC_MP_PTRS(n)    TMP_ALLOC_TYPE(n,mp_ptr)
#define TMP_SALLOC_MP_PTRS(n)   TMP_SALLOC_TYPE(n,mp_ptr)
#define TMP_BALLOC_MP_PTRS(n)   TMP_BALLOC_TYPE(n,mp_ptr)

/* It's more efficient to allocate one block than two.  This is certainly
   true of the malloc methods, but it can even be true of alloca if that
   involves copying a chunk of stack (various RISCs), or a call to a stack
   bounds check (mingw).  In any case, when debugging keep separate blocks
   so a redzoning malloc debugger can protect each individually.  */
#define TMP_ALLOC_LIMBS_2(xp,xsize, yp,ysize)           \
  do {                                                  \
    if (WANT_TMP_DEBUG)                                 \
      {                                                 \
        (xp) = TMP_ALLOC_LIMBS (xsize);                 \
        (yp) = TMP_ALLOC_LIMBS (ysize);                 \
      }                                                 \
    else                                                \
      {                                                 \
        (xp) = TMP_ALLOC_LIMBS ((xsize) + (ysize));     \
        (yp) = (xp) + (xsize);                          \
      }                                                 \
  } while (0)


/* From mpir.h, nicer names for internal use. */
#define MPN_CMP(result, xp, yp, size)  __GMPN_CMP(result, xp, yp, size)
#define LIKELY(cond)                   __GMP_LIKELY(cond)
#define UNLIKELY(cond)                 __GMP_UNLIKELY(cond)

#define ABS(x) ((x) >= 0 ? (x) : -(x))
#undef MIN
#define MIN(l,o) ((l) < (o) ? (l) : (o))
#undef MAX
#define MAX(h,i) ((h) > (i) ? (h) : (i))
#define numberof(x)  (sizeof (x) / sizeof ((x)[0]))

/* Field access macros.  */
#define SIZ(x) ((x)->_mp_size)
#define ABSIZ(x) ABS (SIZ (x))
#define PTR(x) ((x)->_mp_d)
#define LIMBS(x) ((x)->_mp_d)
#define EXP(x) ((x)->_mp_exp)
#define PREC(x) ((x)->_mp_prec)
#define ALLOC(x) ((x)->_mp_alloc)
#define NUM(x) mpq_numref(x)
#define DEN(x) mpq_denref(x)

/* n-1 inverts any low zeros and the lowest one bit.  If n&(n-1) leaves zero
   then that lowest one bit must have been the only bit set.  n==0 will
   return true though, so avoid that.  */
#define POW2_P(n)  (((n) & ((n) - 1)) == 0)

/* This is intended for constant THRESHOLDs only, where the compiler
   can completely fold the result.  */
#define LOG2C(n) \
 (((n) >=    0x1) + ((n) >=    0x2) + ((n) >=    0x4) + ((n) >=    0x8) + \
  ((n) >=   0x10) + ((n) >=   0x20) + ((n) >=   0x40) + ((n) >=   0x80) + \
  ((n) >=  0x100) + ((n) >=  0x200) + ((n) >=  0x400) + ((n) >=  0x800) + \
  ((n) >= 0x1000) + ((n) >= 0x2000) + ((n) >= 0x4000) + ((n) >= 0x8000))

/* The "short" defines are a bit different because shorts are promoted to
   ints by ~ or >> etc.

   #ifndef's are used since on some systems (HP?) header files other than
   limits.h setup these defines.  We could forcibly #undef in that case, but
   there seems no need to worry about that.  */

#ifndef ULONG_MAX
#define ULONG_MAX   __GMP_ULONG_MAX
#endif
#ifndef UINT_MAX
#define UINT_MAX    __GMP_UINT_MAX
#endif
#ifndef USHRT_MAX
#define USHRT_MAX   __GMP_USHRT_MAX
#endif
#define MP_LIMB_T_MAX      (~ (mp_limb_t) 0)

/* Must cast ULONG_MAX etc to unsigned long etc, since they might not be
   unsigned on a K&R compiler.  In particular the HP-UX 10 bundled K&R cc
   treats the plain decimal values in <limits.h> as signed.  */
#define ULONG_HIGHBIT      (ULONG_MAX ^ ((unsigned long) ULONG_MAX >> 1))
#define UINT_HIGHBIT       (UINT_MAX ^ ((unsigned) UINT_MAX >> 1))
#define USHRT_HIGHBIT      ((unsigned short) (USHRT_MAX ^ ((unsigned short) USHRT_MAX >> 1)))
#define GMP_LIMB_HIGHBIT  (MP_LIMB_T_MAX ^ (MP_LIMB_T_MAX >> 1))
#ifdef HAVE_STDINT_H
#define UINTMAX_HIGHBIT   (UINTMAX_MAX ^ (UINTMAX_MAX >> 1))
#endif

#ifndef LONG_MIN
#define LONG_MIN           ((long) ULONG_HIGHBIT)
#endif
#ifndef LONG_MAX
#define LONG_MAX           (-(LONG_MIN+1))
#endif

#ifndef INT_MIN
#define INT_MIN            ((int) UINT_HIGHBIT)
#endif
#ifndef INT_MAX
#define INT_MAX            (-(INT_MIN+1))
#endif

#ifndef SHRT_MIN
#define SHRT_MIN           ((short) USHRT_HIGHBIT)
#endif
#ifndef SHRT_MAX
#define SHRT_MAX           ((short) (-(SHRT_MIN+1)))
#endif

#if defined( _WIN64 )
#define MP_SIZE_T_MAX      _I64_MAX
#define MP_SIZE_T_MIN      _I64_MIN
#elif __GMP_MP_SIZE_T_INT
#define MP_SIZE_T_MAX      INT_MAX
#define MP_SIZE_T_MIN      INT_MIN
#else
#define MP_SIZE_T_MAX      LONG_MAX
#define MP_SIZE_T_MIN      LONG_MIN
#endif

/* mp_exp_t is the same as mp_size_t */
#if defined( _WIN64 )
#define MP_EXP_T_MAX   LONG_MAX
#define MP_EXP_T_MIN   LONG_MIN
#else
#define MP_EXP_T_MAX   MP_SIZE_T_MAX
#define MP_EXP_T_MIN   MP_SIZE_T_MIN
#endif

#define LONG_HIGHBIT       LONG_MIN
#define INT_HIGHBIT        INT_MIN
#define SHRT_HIGHBIT       SHRT_MIN


#define GMP_NUMB_HIGHBIT  (CNST_LIMB(1) << (GMP_NUMB_BITS-1))

#if GMP_NAIL_BITS == 0
#define GMP_NAIL_LOWBIT   CNST_LIMB(0)
#else
#define GMP_NAIL_LOWBIT   (CNST_LIMB(1) << GMP_NUMB_BITS)
#endif

/* Swap macros. */

#define MP_LIMB_T_SWAP(x, y)                    \
  do {                                          \
    mp_limb_t __mp_limb_t_swap__tmp = (x);      \
    (x) = (y);                                  \
    (y) = __mp_limb_t_swap__tmp;                \
  } while (0)
#define MP_SIZE_T_SWAP(x, y)                    \
  do {                                          \
    mp_size_t __mp_size_t_swap__tmp = (x);      \
    (x) = (y);                                  \
    (y) = __mp_size_t_swap__tmp;                \
  } while (0)

#define MP_PTR_SWAP(x, y)               \
  do {                                  \
    mp_ptr __mp_ptr_swap__tmp = (x);    \
    (x) = (y);                          \
    (y) = __mp_ptr_swap__tmp;           \
  } while (0)
#define MP_SRCPTR_SWAP(x, y)                    \
  do {                                          \
    mp_srcptr __mp_srcptr_swap__tmp = (x);      \
    (x) = (y);                                  \
    (y) = __mp_srcptr_swap__tmp;                \
  } while (0)

#define MPN_PTR_SWAP(xp,xs, yp,ys)      \
  do {                                  \
    MP_PTR_SWAP (xp, yp);               \
    MP_SIZE_T_SWAP (xs, ys);            \
  } while(0)
#define MPN_SRCPTR_SWAP(xp,xs, yp,ys)   \
  do {                                  \
    MP_SRCPTR_SWAP (xp, yp);            \
    MP_SIZE_T_SWAP (xs, ys);            \
  } while(0)

#define MPZ_PTR_SWAP(x, y)              \
  do {                                  \
    mpz_ptr __mpz_ptr_swap__tmp = (x);  \
    (x) = (y);                          \
    (y) = __mpz_ptr_swap__tmp;          \
  } while (0)
#define MPZ_SRCPTR_SWAP(x, y)                   \
  do {                                          \
    mpz_srcptr __mpz_srcptr_swap__tmp = (x);    \
    (x) = (y);                                  \
    (y) = __mpz_srcptr_swap__tmp;               \
  } while (0)


/* Enhancement: __gmp_allocate_func could have "__attribute__ ((malloc))",
   but current gcc (3.0) doesn't seem to support that.  */
__GMP_DECLSPEC extern void * (*__gmp_allocate_func) __GMP_PROTO ((size_t));
__GMP_DECLSPEC extern void * (*__gmp_reallocate_func) __GMP_PROTO ((void *, size_t, size_t));
__GMP_DECLSPEC extern void   (*__gmp_free_func) __GMP_PROTO ((void *, size_t));

__GMP_DECLSPEC void *__gmp_default_allocate _PROTO ((size_t));
__GMP_DECLSPEC void *__gmp_default_reallocate _PROTO ((void *, size_t, size_t));
__GMP_DECLSPEC void __gmp_default_free _PROTO ((void *, size_t));

#define __GMP_ALLOCATE_FUNC_TYPE(n,type) \
  ((type *) (*__gmp_allocate_func) ((n) * sizeof (type)))
#define __GMP_ALLOCATE_FUNC_LIMBS(n)   __GMP_ALLOCATE_FUNC_TYPE (n, mp_limb_t)

#define __GMP_REALLOCATE_FUNC_TYPE(p, old_size, new_size, type) \
  ((type *) (*__gmp_reallocate_func)                            \
   (p, (old_size) * sizeof (type), (new_size) * sizeof (type)))
#define __GMP_REALLOCATE_FUNC_LIMBS(p, old_size, new_size) \
  __GMP_REALLOCATE_FUNC_TYPE(p, old_size, new_size, mp_limb_t)

#define __GMP_FREE_FUNC_TYPE(p,n,type) (*__gmp_free_func) (p, (n) * sizeof (type))
#define __GMP_FREE_FUNC_LIMBS(p,n)     __GMP_FREE_FUNC_TYPE (p, n, mp_limb_t)

#define __GMP_REALLOCATE_FUNC_MAYBE(ptr, oldsize, newsize)      \
  do {                                                          \
    if ((oldsize) != (newsize))                                 \
      (ptr) = (*__gmp_reallocate_func) (ptr, oldsize, newsize); \
  } while (0)

#define __GMP_REALLOCATE_FUNC_MAYBE_TYPE(ptr, oldsize, newsize, type)   \
  do {                                                                  \
    if ((oldsize) != (newsize))                                         \
      (ptr) = (type *) (*__gmp_reallocate_func)                         \
        (ptr, (oldsize) * sizeof (type), (newsize) * sizeof (type));    \
  } while (0)


/* Dummy for non-gcc, code involving it will go dead. */
#if ! defined (__GNUC__) || __GNUC__ < 2
#define __builtin_constant_p(x)   0
#endif


/* In gcc 2.96 and up on i386, tail calls are optimized to jumps if the
   stack usage is compatible.  __attribute__ ((regparm (N))) helps by
   putting leading parameters in registers, avoiding extra stack.

   regparm cannot be used with calls going through the PLT, because the
   binding code there may clobber the registers (%eax, %edx, %ecx) used for
   the regparm parameters.  Calls to local (ie. static) functions could
   still use this, if we cared to differentiate locals and globals.

   On athlon-unknown-freebsd4.9 with gcc 3.3.3, regparm cannot be used with
   -p or -pg profiling, since that version of gcc doesn't realize the
   .mcount calls will clobber the parameter registers.  Other systems are
   ok, like debian with glibc 2.3.2 (mcount doesn't clobber), but we don't
   bother to try to detect this.  regparm is only an optimization so we just
   disable it when profiling (profiling being a slowdown anyway).  */

#define USE_LEADING_REGPARM 0


/* Macros for altering parameter order according to regparm usage. */
#if USE_LEADING_REGPARM
#define REGPARM_2_1(a,b,x)    x,a,b
#define REGPARM_3_1(a,b,c,x)  x,a,b,c
#define REGPARM_ATTR(n) __attribute__ ((regparm (n)))
#else
#define REGPARM_2_1(a,b,x)    a,b,x
#define REGPARM_3_1(a,b,c,x)  a,b,c,x
#define REGPARM_ATTR(n)
#endif

__GMP_DECLSPEC int mpir_is_likely_prime_BPSW(mp_limb_t n);

__GMP_DECLSPEC mp_limb_t mpir_sqrt(mp_limb_t r);

__GMP_DECLSPEC void __gmpz_aorsmul_1 _PROTO ((REGPARM_3_1 (mpz_ptr w, mpz_srcptr u, mp_limb_t v, mp_size_t sub))) REGPARM_ATTR(1);
#define mpz_aorsmul_1(w,u,v,sub)  __gmpz_aorsmul_1 (REGPARM_3_1 (w, u, v, sub))

#define mpz_n_pow_ui __gmpz_n_pow_ui
__GMP_DECLSPEC void    mpz_n_pow_ui _PROTO ((mpz_ptr, mp_srcptr, mp_size_t, mpir_ui));


#define mpn_add_nc __MPN(add_nc)
__GMP_DECLSPEC mp_limb_t mpn_add_nc __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_size_t, mp_limb_t));

#define mpn_addmul_1c __MPN(addmul_1c)
__GMP_DECLSPEC mp_limb_t mpn_addmul_1c __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_limb_t, mp_limb_t));

#define mpn_addmul_2 __MPN(addmul_2)
__GMP_DECLSPEC mp_limb_t mpn_addmul_2 _PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_srcptr));

#define mpn_addmul_3 __MPN(addmul_3)
__GMP_DECLSPEC mp_limb_t mpn_addmul_3 _PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_srcptr));

#define mpn_addmul_4 __MPN(addmul_4)
__GMP_DECLSPEC mp_limb_t mpn_addmul_4 _PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_srcptr));

#define mpn_addmul_5 __MPN(addmul_5)
__GMP_DECLSPEC mp_limb_t mpn_addmul_5 _PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_srcptr));

#define mpn_addmul_6 __MPN(addmul_6)
__GMP_DECLSPEC mp_limb_t mpn_addmul_6 _PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_srcptr));

#define mpn_addmul_7 __MPN(addmul_7)
__GMP_DECLSPEC mp_limb_t mpn_addmul_7 _PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_srcptr));

#define mpn_addmul_8 __MPN(addmul_8)
__GMP_DECLSPEC mp_limb_t mpn_addmul_8 _PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_srcptr));

#define mpn_addlsh_n __MPN(addlsh_n)
__GMP_DECLSPEC mp_limb_t mpn_addlsh_n __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_size_t,unsigned int));

#define mpn_sublsh_n __MPN(sublsh_n)
__GMP_DECLSPEC mp_limb_t mpn_sublsh_n __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_size_t,unsigned int));

#define mpn_addlsh_nc __MPN(addlsh_nc)
__GMP_DECLSPEC mp_limb_t mpn_addlsh_nc __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_size_t,unsigned int, mp_limb_t));

#define mpn_sublsh_nc __MPN(sublsh_nc)
__GMP_DECLSPEC mp_limb_t mpn_sublsh_nc __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_size_t,unsigned int, mp_limb_t));

/* mpn_rsh1add_n(c,a,b,n), when it exists, sets {c,n} to ({a,n} + {b,n}) >> 1,
   and returns the bit rshifted out (0 or 1).  */
#define mpn_rsh1add_n __MPN(rsh1add_n)
__GMP_DECLSPEC mp_limb_t mpn_rsh1add_n __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_size_t));

/* mpn_rsh1sub_n(c,a,b,n), when it exists, sets {c,n} to ({a,n} - {b,n}) >> 1,
   and returns the bit rshifted out (0 or 1).  If there's a borrow from the
   subtract, it's stored as a 1 in the high bit of c[n-1], like a twos
   complement negative.  */
#define mpn_rsh1sub_n __MPN(rsh1sub_n)
__GMP_DECLSPEC mp_limb_t mpn_rsh1sub_n __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_size_t));

#if HAVE_NATIVE_mpn_lshiftc
#define mpn_lshiftc __MPN(lshiftc)
__GMP_DECLSPEC mp_limb_t mpn_lshiftc __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t,unsigned int));
#endif

#define mpn_addadd_n __MPN(addadd_n)
__GMP_DECLSPEC mp_limb_t mpn_addadd_n __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_srcptr, mp_size_t));

#define mpn_addsub_n __MPN(addsub_n)
__GMP_DECLSPEC int mpn_addsub_n __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_srcptr, mp_size_t));

#define mpn_subadd_n __MPN(subadd_n)
__GMP_DECLSPEC mp_limb_t mpn_subadd_n __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_srcptr, mp_size_t));

#if HAVE_NATIVE_mpn_karaadd
#define mpn_karaadd __MPN(karaadd)
__GMP_DECLSPEC void mpn_karaadd __GMP_PROTO ((mp_ptr, mp_ptr, mp_size_t));
#endif

#if HAVE_NATIVE_mpn_karasub
#define mpn_karasub __MPN(karasub)
__GMP_DECLSPEC void mpn_karasub __GMP_PROTO ((mp_ptr, mp_ptr, mp_size_t));
#endif

#ifndef mpn_sumdiff_n  /* if not done with cpuvec in a fat binary */
#define mpn_sumdiff_n __MPN(sumdiff_n)
__GMP_DECLSPEC mp_limb_t mpn_sumdiff_n __GMP_PROTO ((mp_ptr, mp_ptr, mp_srcptr, mp_srcptr, mp_size_t));
#endif

#ifndef mpn_nsumdiff_n
#define mpn_nsumdiff_n __MPN(nsumdiff_n)
__GMP_DECLSPEC mp_limb_t mpn_nsumdiff_n __GMP_PROTO ((mp_ptr, mp_ptr, mp_srcptr, mp_srcptr, mp_size_t));
#endif

#define mpn_sumdiff_nc __MPN(sumdiff_nc)
__GMP_DECLSPEC mp_limb_t mpn_sumdiff_nc __GMP_PROTO ((mp_ptr, mp_ptr, mp_srcptr, mp_srcptr, mp_size_t, mp_limb_t));

#define mpn_divexact_byff __MPN(divexact_byff)
__GMP_DECLSPEC mp_limb_t mpn_divexact_byff __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t));

#ifndef mpn_divexact_byfobm1      /* if not done with cpuvec in a fat binary */
#define mpn_divexact_byfobm1 __MPN(divexact_byfobm1)
__GMP_DECLSPEC mp_limb_t mpn_divexact_byfobm1 __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t,mp_limb_t,mp_limb_t));
#endif

#ifndef mpn_add_err1_n      /* if not done with cpuvec in a fat binary */
#define mpn_add_err1_n  __MPN(add_err1_n)
__GMP_DECLSPEC mp_limb_t mpn_add_err1_n (mp_ptr, mp_srcptr, mp_srcptr, mp_ptr, mp_srcptr, mp_size_t, mp_limb_t);
#endif

#ifndef mpn_sub_err1_n      /* if not done with cpuvec in a fat binary */
#define mpn_sub_err1_n  __MPN(sub_err1_n)
__GMP_DECLSPEC mp_limb_t mpn_sub_err1_n (mp_ptr, mp_srcptr, mp_srcptr, mp_ptr, mp_srcptr, mp_size_t, mp_limb_t);
#endif

#ifndef mpn_add_err2_n      /* if not done with cpuvec in a fat binary */
#define mpn_add_err2_n  __MPN(add_err2_n)
__GMP_DECLSPEC mp_limb_t mpn_add_err2_n (mp_ptr, mp_srcptr, mp_srcptr, mp_ptr, mp_srcptr, mp_srcptr, mp_size_t, mp_limb_t);
#endif

#ifndef mpn_sub_err2_n      /* if not done with cpuvec in a fat binary */
#define mpn_sub_err2_n  __MPN(sub_err2_n)
__GMP_DECLSPEC mp_limb_t mpn_sub_err2_n (mp_ptr, mp_srcptr, mp_srcptr, mp_ptr, mp_srcptr, mp_srcptr, mp_size_t, mp_limb_t);
#endif

#define mpn_divrem_1c __MPN(divrem_1c)
__GMP_DECLSPEC mp_limb_t mpn_divrem_1c __GMP_PROTO ((mp_ptr, mp_size_t, mp_srcptr, mp_size_t, mp_limb_t, mp_limb_t));

#define mpn_dump __MPN(dump)
__GMP_DECLSPEC void mpn_dump __GMP_PROTO ((mp_srcptr, mp_size_t));

#define mpn_fib2_ui __MPN(fib2_ui)
__GMP_DECLSPEC mp_size_t mpn_fib2_ui _PROTO ((mp_ptr, mp_ptr, mpir_ui));

/* Remap names of internal mpn functions.  */
#define __clz_tab               __MPN(clz_tab)
#define mpn_udiv_w_sdiv		__MPN(udiv_w_sdiv)

#define mpn_jacobi_base __MPN(jacobi_base)
__GMP_DECLSPEC int mpn_jacobi_base _PROTO ((mp_limb_t a, mp_limb_t b, int result_bit1)) ATTRIBUTE_CONST;

#define mpn_jacobi_2 __MPN(jacobi_2)
__GMP_DECLSPEC int mpn_jacobi_2 _PROTO ((mp_srcptr, mp_srcptr, unsigned));

#define mpn_jacobi_n __MPN(jacobi_n)
__GMP_DECLSPEC int mpn_jacobi_n _PROTO ((mp_ptr, mp_ptr, mp_size_t, unsigned));

#define mpn_mod_1c __MPN(mod_1c)
__GMP_DECLSPEC mp_limb_t mpn_mod_1c __GMP_PROTO ((mp_srcptr, mp_size_t, mp_limb_t, mp_limb_t)) __GMP_ATTRIBUTE_PURE;

#define mpn_mul_1c __MPN(mul_1c)
__GMP_DECLSPEC mp_limb_t mpn_mul_1c __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_limb_t, mp_limb_t));

#define mpn_mul_2 __MPN(mul_2)
mp_limb_t mpn_mul_2 _PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_srcptr));

#ifndef mpn_mul_basecase  /* if not done with cpuvec in a fat binary */
#define mpn_mul_basecase __MPN(mul_basecase)
__GMP_DECLSPEC void mpn_mul_basecase __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_srcptr, mp_size_t));
#endif

#define mpn_mullow_n __MPN(mullow_n)
__GMP_DECLSPEC void mpn_mullow_n __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_size_t));

#define mpn_mullow_n_basecase __MPN(mullow_n_basecase)
__GMP_DECLSPEC void mpn_mullow_n_basecase __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_size_t));

#define mpn_mulhigh_n __MPN(mulhigh_n)
__GMP_DECLSPEC void mpn_mulhigh_n __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_size_t));

#define mpn_mullow_basecase __MPN(mullow_basecase)
__GMP_DECLSPEC void mpn_mullow_basecase __GMP_PROTO ((mp_ptr, mp_srcptr,mp_size_t, mp_srcptr, mp_size_t,mp_size_t));

#ifndef mpn_mulmid_basecase      /* if not done with cpuvec in a fat binary */
#define mpn_mulmid_basecase __MPN(mulmid_basecase)
__GMP_DECLSPEC void mpn_mulmid_basecase __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_srcptr, mp_size_t));
#endif

#define mpn_mod_1_1 __MPN(mod_1_1)
__GMP_DECLSPEC void mpn_mod_1_1 __GMP_PROTO ((mp_ptr,mp_srcptr, mp_size_t, mp_srcptr));

#define mpn_mod_1_2 __MPN(mod_1_2)
__GMP_DECLSPEC void mpn_mod_1_2 __GMP_PROTO ((mp_ptr,mp_srcptr, mp_size_t, mp_srcptr));

#define mpn_mod_1_3 __MPN(mod_1_3)
__GMP_DECLSPEC void mpn_mod_1_3 __GMP_PROTO ((mp_ptr,mp_srcptr, mp_size_t, mp_srcptr));

#define mpn_mod_1_k __MPN(mod_1_k)
__GMP_DECLSPEC mp_limb_t mpn_mod_1_k __GMP_PROTO ((mp_srcptr, mp_size_t, mp_limb_t,mp_size_t));

#define mpn_mulmid __MPN(mulmid)
__GMP_DECLSPEC void mpn_mulmid __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_srcptr, mp_size_t));

#define mpn_mulmid_n __MPN(mulmid_n)
__GMP_DECLSPEC void mpn_mulmid_n __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_size_t));

#ifndef mpn_sqr_basecase  /* if not done with cpuvec in a fat binary */
#define mpn_sqr_basecase __MPN(sqr_basecase)
__GMP_DECLSPEC void mpn_sqr_basecase __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t));
#endif

#define mpz_trial_division __gmpz_trial_division
__GMP_DECLSPEC unsigned long mpz_trial_division __GMP_PROTO ((mpz_srcptr,unsigned long, unsigned long));

#define mpn_sub_nc __MPN(sub_nc)
__GMP_DECLSPEC mp_limb_t mpn_sub_nc __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_size_t, mp_limb_t));

#define mpn_submul_1c __MPN(submul_1c)
__GMP_DECLSPEC mp_limb_t mpn_submul_1c __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_limb_t, mp_limb_t));

#define mpn_invert_2exp __MPN(invert_2exp)
__GMP_DECLSPEC void mpn_invert_2exp __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_ptr));

#define mpn_is_invert __MPN(is_invert)
__GMP_DECLSPEC int mpn_is_invert __GMP_PROTO ((mp_srcptr, mp_srcptr, mp_size_t));

#define mpn_invert_trunc __MPN(invert_trunc)
__GMP_DECLSPEC void mpn_invert_trunc __GMP_PROTO ((mp_ptr, mp_size_t, mp_srcptr, mp_size_t, mp_srcptr));

#define   mpn_binvert __MPN(binvert)
__GMP_DECLSPEC void      mpn_binvert __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_ptr));
#define   mpn_binvert_itch __MPN(binvert_itch)
__GMP_DECLSPEC mp_size_t mpn_binvert_itch __GMP_PROTO ((mp_size_t)) ATTRIBUTE_CONST;

#define   mpn_powm __MPN(powm)
__GMP_DECLSPEC void      mpn_powm __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_srcptr, mp_size_t, mp_srcptr, mp_size_t, mp_ptr));
#define   mpn_powlo __MPN(powlo)
__GMP_DECLSPEC void      mpn_powlo __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_size_t, mp_size_t, mp_ptr));

#ifndef mpn_divrem_euclidean_qr_1    /* if not done with cpuvec in a fat binary */
#define mpn_divrem_euclidean_qr_1 __MPN(divrem_euclidean_qr_1)
__GMP_DECLSPEC mp_limb_t mpn_divrem_euclidean_qr_1 __GMP_PROTO ((mp_ptr, mp_size_t, mp_srcptr, mp_size_t,mp_limb_t));
#endif

#ifndef mpn_divrem_euclidean_qr_2    /* if not done with cpuvec in a fat binary */
#define mpn_divrem_euclidean_qr_2 __MPN(divrem_euclidean_qr_2)
__GMP_DECLSPEC mp_limb_t mpn_divrem_euclidean_qr_2 __GMP_PROTO ((mp_ptr, mp_ptr, mp_size_t,mp_srcptr));
#endif

#define mpn_divrem_euclidean_r_1 __MPN(divrem_euclidean_r_1)
__GMP_DECLSPEC mp_limb_t mpn_divrem_euclidean_r_1 __GMP_PROTO ((mp_srcptr, mp_size_t,mp_limb_t));

#define mpn_divrem_hensel_qr_1 __MPN(divrem_hensel_qr_1)
__GMP_DECLSPEC mp_limb_t mpn_divrem_hensel_qr_1 __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t,mp_limb_t));

#define mpn_divrem_hensel_qr_1_1 __MPN(divrem_hensel_qr_1_1)
__GMP_DECLSPEC mp_limb_t mpn_divrem_hensel_qr_1_1 __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t,mp_limb_t));

#define mpn_divrem_hensel_qr_1_2 __MPN(divrem_hensel_qr_1_2)
__GMP_DECLSPEC mp_limb_t mpn_divrem_hensel_qr_1_2 __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t,mp_limb_t));

#define mpn_divrem_hensel_rsh_qr_1_preinv __MPN(divrem_hensel_rsh_qr_1_preinv)
__GMP_DECLSPEC mp_limb_t mpn_divrem_hensel_rsh_qr_1_preinv __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t,mp_limb_t,mp_limb_t,int));

#define mpn_divrem_hensel_rsh_qr_1 __MPN(divrem_hensel_rsh_qr_1)
__GMP_DECLSPEC mp_limb_t mpn_divrem_hensel_rsh_qr_1 __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t,mp_limb_t,int));

#define mpn_rsh_divrem_hensel_qr_1 __MPN(rsh_divrem_hensel_qr_1)
__GMP_DECLSPEC mp_limb_t mpn_rsh_divrem_hensel_qr_1 __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t,mp_limb_t,int,mp_limb_t));

#define mpn_rsh_divrem_hensel_qr_1_1 __MPN(rsh_divrem_hensel_qr_1_1)
__GMP_DECLSPEC mp_limb_t mpn_rsh_divrem_hensel_qr_1_1 __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t,mp_limb_t,int,mp_limb_t));

#define mpn_rsh_divrem_hensel_qr_1_2 __MPN(rsh_divrem_hensel_qr_1_2)
__GMP_DECLSPEC mp_limb_t mpn_rsh_divrem_hensel_qr_1_2 __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t,mp_limb_t,int,mp_limb_t));

#define mpn_divrem_hensel_r_1 __MPN(divrem_hensel_r_1)
__GMP_DECLSPEC mp_limb_t mpn_divrem_hensel_r_1 __GMP_PROTO ((mp_srcptr, mp_size_t,mp_limb_t));

#define mpir_random_fermat(nn, state, limbs) \
   do { mp_limb_t t; \
      mpn_rrandom(nn, state, limbs); \
      mpn_rrandom(&nn[limbs], state, 1); \
      nn[limbs] %= 1024; \
      mpn_rrandom(&t, state, 1); \
      if (t % 2) \
         nn[limbs] = -nn[limbs]; \
   } while (0)

#define mpn_addmod_2expp1_1(r, limbs, c)                    \
do {                                                        \
   mp_limb_t __sum = (r)[0] + (mp_limb_signed_t)(c);        \
   /* check if adding c causes carry propagation */         \
   if ((mp_limb_signed_t)(__sum ^ (r)[0]) >= 0)             \
      (r)[0] = __sum;                                       \
   else                                                     \
   {                                                        \
      if ((mp_limb_signed_t) (c) >= 0) mpn_add_1((r),       \
          (r), (limbs) + 1, (mp_limb_signed_t) (c));        \
      else mpn_sub_1((r), (r), (limbs) + 1,                 \
                         -(mp_limb_signed_t) (c));          \
   }                                                        \
} while (0)

#define mpn_mul_2expmod_2expp1 __MPN(mul_2expmod_2expp1)
__GMP_DECLSPEC void mpn_mul_2expmod_2expp1 __GMP_PROTO ((mp_ptr t, mp_ptr i1, mp_size_t limbs, mp_bitcnt_t d));

#define mpir_revbin __mpir_revbin
__GMP_DECLSPEC mp_limb_t mpir_revbin __GMP_PROTO ((mp_limb_t in, mp_limb_t bits));

#define  mpir_fft_adjust_limbs  __mpir_fft_adjust_limbs
__GMP_DECLSPEC mpir_si mpir_fft_adjust_limbs __GMP_PROTO ((mp_size_t limbs));

#define mpir_fft_combine_bits __mpir_fft_combine_bits
__GMP_DECLSPEC void mpir_fft_combine_bits __GMP_PROTO ((mp_ptr res, const mp_ptr * poly, 
                long length, mp_bitcnt_t bits, mp_size_t output_limbs, mp_size_t total_limbs));

#define mpir_fft_split_bits __mpir_fft_split_bits
__GMP_DECLSPEC mp_size_t mpir_fft_split_bits __GMP_PROTO ((mp_ptr * poly, mp_srcptr limbs, 
                 mp_size_t total_limbs, mp_bitcnt_t bits, mp_size_t output_limbs));

#define mpir_fft_adjust __mpir_fft_adjust
__GMP_DECLSPEC void mpir_fft_adjust __GMP_PROTO ((mp_ptr r, mp_ptr i1, 
                                     mp_size_t i, mp_size_t limbs, mp_bitcnt_t w));

#define mpir_fft_adjust_sqrt2 __mpir_fft_adjust_sqrt2
__GMP_DECLSPEC void mpir_fft_adjust_sqrt2 __GMP_PROTO ((mp_ptr r, mp_ptr i1, 
                   mp_size_t i, mp_size_t limbs, mp_bitcnt_t w, mp_ptr temp));

#define mpir_butterfly_lshB __mpir_butterfly_lshB
__GMP_DECLSPEC void mpir_butterfly_lshB __GMP_PROTO ((mp_ptr t, mp_ptr u, mp_ptr i1, 
                       mp_ptr i2, mp_size_t limbs, mp_size_t x, mp_size_t y));

#define mpir_butterfly_rshB __mpir_butterfly_rshB
__GMP_DECLSPEC void mpir_butterfly_rshB __GMP_PROTO ((mp_ptr t, mp_ptr u, mp_ptr i1, 
                       mp_ptr i2, mp_size_t limbs, mp_size_t x, mp_size_t y));

#define mpir_fermat_to_mpz __fermat_to_mpz
__GMP_DECLSPEC void mpir_fermat_to_mpz __GMP_PROTO ((mpz_t m, mp_ptr i, mp_size_t limbs));

#define mpir_fft_butterfly_twiddle __mpir_fft_butterfly_twiddle
__GMP_DECLSPEC void mpir_fft_butterfly_twiddle __GMP_PROTO ((mp_ptr u, mp_ptr v, 
   mp_ptr s, mp_ptr t, mp_size_t limbs, mp_bitcnt_t b1, mp_bitcnt_t b2));

#define mpir_ifft_butterfly_twiddle __mpir_ifft_butterfly_twiddle
__GMP_DECLSPEC void mpir_ifft_butterfly_twiddle __GMP_PROTO ((mp_ptr u, mp_ptr v, 
   mp_ptr s, mp_ptr t, mp_size_t limbs, mp_bitcnt_t b1, mp_bitcnt_t b2));

#define mpir_fft_butterfly_sqrt2 __mpir_fft_butterfly_sqrt2
__GMP_DECLSPEC void mpir_fft_butterfly_sqrt2 __GMP_PROTO ((mp_ptr s, mp_ptr t, 
                         mp_ptr i1, mp_ptr i2, mp_size_t i, 
                                mp_size_t limbs, mp_bitcnt_t w, mp_ptr temp));

#define mpir_ifft_butterfly_sqrt2 __mpir_ifft_butterfly_sqrt2
__GMP_DECLSPEC void mpir_ifft_butterfly_sqrt2 __GMP_PROTO ((mp_ptr s, mp_ptr t, mp_ptr i1, 
   mp_ptr i2, mp_size_t i, mp_size_t limbs, mp_bitcnt_t w, mp_ptr temp));

#define mpir_fft_butterfly __mpir_fft_butterfly
__GMP_DECLSPEC void mpir_fft_butterfly __GMP_PROTO ((mp_ptr s, mp_ptr t, mp_ptr i1, 
                     mp_ptr i2, mp_size_t i, mp_size_t limbs, mp_bitcnt_t w));

#define mpir_ifft_butterfly __mpir_ifft_butterfly
__GMP_DECLSPEC void mpir_ifft_butterfly __GMP_PROTO ((mp_ptr s, mp_ptr t, mp_ptr i1, 
                     mp_ptr i2, mp_size_t i, mp_size_t limbs, mp_bitcnt_t w));

#define mpir_fft_combine_limbs __combine_limbs
__GMP_DECLSPEC void mpir_fft_combine_limbs __GMP_PROTO ((mp_ptr res, const mp_ptr * poly, long length, 
            mp_size_t coeff_limbs, mp_size_t output_limbs, mp_size_t total_limbs));

#define mpir_fft_split_limbs __mpir_fft_split_limbs
__GMP_DECLSPEC mp_size_t mpir_fft_split_limbs __GMP_PROTO ((mp_ptr * poly, mp_srcptr limbs, 
            mp_size_t total_limbs, mp_size_t coeff_limbs, mp_size_t output_limbs));

#define mpir_fft_radix2 __mpir_fft_radix2
__GMP_DECLSPEC void mpir_fft_radix2 __GMP_PROTO ((mp_ptr * ii, 
                    mp_size_t n, mp_bitcnt_t w, mp_ptr * t1, mp_ptr * t2));

#define mpir_ifft_radix2 __mpir_ifft_radix2
__GMP_DECLSPEC void mpir_ifft_radix2 __GMP_PROTO ((mp_ptr * ii, mp_size_t n, 
                                 mp_bitcnt_t w, mp_ptr * t1, mp_ptr * t2));

#define mpir_fft_trunc __mpir_fft_trunc
__GMP_DECLSPEC void mpir_fft_trunc __GMP_PROTO ((mp_ptr * ii,  mp_size_t n, mp_bitcnt_t w, 
                               mp_ptr * t1, mp_ptr * t2, mp_size_t trunc));

#define mpir_ifft_trunc __mpir_ifft_trunc
__GMP_DECLSPEC void mpir_ifft_trunc __GMP_PROTO ((mp_ptr * ii, mp_size_t n, mp_bitcnt_t w, 
                               mp_ptr * t1, mp_ptr * t2, mp_size_t trunc));

#define mpir_fft_trunc_sqrt2 __mpir_fft_trunc_sqrt2
__GMP_DECLSPEC void mpir_fft_trunc_sqrt2 __GMP_PROTO ((mp_ptr * ii, mp_size_t n, mp_bitcnt_t w, 
            mp_ptr * t1, mp_ptr * t2, mp_ptr * temp, mp_size_t trunc));

#define mpir_ifft_trunc_sqrt2 __mpir_ifft_trunc_sqrt2
__GMP_DECLSPEC void mpir_ifft_trunc_sqrt2 __GMP_PROTO ((mp_ptr * ii, mp_size_t n, mp_bitcnt_t w, 
            mp_ptr * t1, mp_ptr * t2, mp_ptr * temp, mp_size_t trunc));

#define mpir_fft_mfa_trunc_sqrt2 __mpir_fft_mfa_trunc_sqrt2
__GMP_DECLSPEC void mpir_fft_mfa_trunc_sqrt2 __GMP_PROTO ((mp_ptr * ii, mp_size_t n, 
                       mp_bitcnt_t w, mp_ptr * t1, mp_ptr * t2, 
                                mp_ptr * temp, mp_size_t n1, mp_size_t trunc));

#define mpir_ifft_mfa_trunc_sqrt2 __mpir_ifft_mfa_trunc_sqrt2
__GMP_DECLSPEC void mpir_ifft_mfa_trunc_sqrt2 __GMP_PROTO ((mp_ptr * ii, mp_size_t n, 
                      mp_bitcnt_t w, mp_ptr * t1, mp_ptr * t2,
                                mp_ptr * temp, mp_size_t n1, mp_size_t trunc));

#define mpir_fft_negacyclic __mpir_fft_negacyclic
__GMP_DECLSPEC void mpir_fft_negacyclic __GMP_PROTO ((mp_ptr * ii, mp_size_t n, mp_bitcnt_t w, 
                             mp_ptr * t1, mp_ptr * t2, mp_ptr * temp));

#define mpir_ifft_negacyclic __mpir_ifft_negacyclic
__GMP_DECLSPEC void mpir_ifft_negacyclic __GMP_PROTO ((mp_ptr * ii, mp_size_t n, mp_bitcnt_t w, 
                             mp_ptr * t1, mp_ptr * t2, mp_ptr * temp));

#define mpir_fft_mfa_trunc_sqrt2_outer __mpir_fft_mfa_trunc_sqrt2_outer
__GMP_DECLSPEC void mpir_fft_mfa_trunc_sqrt2_outer __GMP_PROTO ((mp_ptr * ii, mp_size_t n, 
                      mp_bitcnt_t w, mp_ptr * t1, mp_ptr * t2, 
                                mp_ptr * temp, mp_size_t n1, mp_size_t trunc));

#define mpir_ifft_mfa_trunc_sqrt2_outer __mpir_ifft_mfa_trunc_sqrt2_outer
__GMP_DECLSPEC void mpir_ifft_mfa_trunc_sqrt2_outer __GMP_PROTO ((mp_ptr * ii, mp_size_t n, 
                        mp_bitcnt_t w, mp_ptr * t1, mp_ptr * t2, 
                                mp_ptr * temp, mp_size_t n1, mp_size_t trunc));

#define mpir_fft_mfa_trunc_sqrt2_inner __mpir_fft_mfa_trunc_sqrt2_inner
__GMP_DECLSPEC void mpir_fft_mfa_trunc_sqrt2_inner __GMP_PROTO ((mp_ptr * ii, mp_ptr * jj, 
            mp_size_t n, mp_bitcnt_t w, mp_ptr * t1, mp_ptr * t2, 
                mp_ptr * temp, mp_size_t n1, mp_size_t trunc, mp_ptr tt));

#define mpir_fft_mulmod_2expp1 __mpir_fft_mulmod_2expp1
__GMP_DECLSPEC void mpir_fft_mulmod_2expp1(mp_ptr r1, mp_srcptr i1, mp_srcptr i2, 
                 mp_size_t r_limbs, mp_bitcnt_t depth, mp_bitcnt_t w);

#define mpir_fft_trunc1 __mpir_fft_trunc1
__GMP_DECLSPEC void mpir_fft_trunc1 __GMP_PROTO ((mp_ptr * ii, mp_size_t n, mp_bitcnt_t w, 
                               mp_ptr * t1, mp_ptr * t2, mp_size_t trunc));

#define mpir_ifft_trunc1 __mpir_ifft_trunc1
__GMP_DECLSPEC void mpir_ifft_trunc1 __GMP_PROTO ((mp_ptr * ii, mp_size_t n, mp_bitcnt_t w, 
                               mp_ptr * t1, mp_ptr * t2, mp_size_t trunc));

#define mpir_fft_radix2_twiddle __mpir_fft_radix2_twiddle
__GMP_DECLSPEC void mpir_fft_radix2_twiddle __GMP_PROTO ((mp_ptr * ii, mp_size_t is,
      mp_size_t n, mp_bitcnt_t w, mp_ptr * t1, mp_ptr * t2,
                            mp_size_t ws, mp_size_t r, mp_size_t c, mp_size_t rs));

#define mpir_ifft_radix2_twiddle __mpir_ifft_radix2_twiddle
__GMP_DECLSPEC void mpir_ifft_radix2_twiddle __GMP_PROTO ((mp_ptr * ii, mp_size_t is,
        mp_size_t n, mp_bitcnt_t w, mp_ptr * t1, mp_ptr * t2,
                            mp_size_t ws, mp_size_t r, mp_size_t c, mp_size_t rs));

#define mpir_fft_trunc1_twiddle __mpir_fft_trunc1_twiddle
__GMP_DECLSPEC void mpir_fft_trunc1_twiddle __GMP_PROTO ((mp_ptr * ii, mp_size_t is,
        mp_size_t n, mp_bitcnt_t w, mp_ptr * t1, mp_ptr * t2,
           mp_size_t ws, mp_size_t r, mp_size_t c, mp_size_t rs, mp_size_t trunc));

#define mpir_ifft_trunc1_twiddle __mpir_ifft_trunc1_twiddle
__GMP_DECLSPEC void mpir_ifft_trunc1_twiddle __GMP_PROTO ((mp_ptr * ii, mp_size_t is,
        mp_size_t n, mp_bitcnt_t w, mp_ptr * t1, mp_ptr * t2,
           mp_size_t ws, mp_size_t r, mp_size_t c, mp_size_t rs, mp_size_t trunc));

#define mpir_fft_naive_convolution_1 __mpir_fft_naive_convolution_1
__GMP_DECLSPEC void mpir_fft_naive_convolution_1 __GMP_PROTO ((mp_ptr r, mp_srcptr ii, 
                                                     mp_srcptr jj, mp_size_t m));

#define mpn_mulmod_2expp1_basecase __MPN(mulmod_2expp1_basecase)
__GMP_DECLSPEC int mpn_mulmod_2expp1_basecase __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, int, mpir_ui, mp_ptr));

typedef __gmp_randstate_struct *gmp_randstate_ptr;
typedef const __gmp_randstate_struct *gmp_randstate_srcptr;

#define mpn_normmod_2expp1 __MPN(normmod_2expp1)
__GMP_DECLSPEC void mpn_normmod_2expp1 __GMP_PROTO ((mp_ptr t, mp_size_t limbs));

#define mpn_div_2expmod_2expp1 __MPN(div_2expmod_2expp1)
__GMP_DECLSPEC void mpn_div_2expmod_2expp1 __GMP_PROTO ((mp_ptr t, mp_srcptr i1, mp_size_t limbs, mp_bitcnt_t d));

#define mpn_mul_trunc_sqrt2 __MPN(mul_trunc_sqrt2)
__GMP_DECLSPEC void mpn_mul_trunc_sqrt2 __GMP_PROTO ((mp_ptr r1, mp_srcptr i1, mp_size_t n1, 
                  mp_srcptr i2, mp_size_t n2, mp_bitcnt_t depth, mp_bitcnt_t w));

#define mpn_mul_mfa_trunc_sqrt2 __MPN(mul_mfa_trunc_sqrt2)
__GMP_DECLSPEC void mpn_mul_mfa_trunc_sqrt2 __GMP_PROTO ((mp_ptr r1, mp_srcptr i1, mp_size_t n1, 
                  mp_srcptr i2, mp_size_t n2, mp_bitcnt_t depth, mp_bitcnt_t w));

/* Pseudo-random number generator function pointers structure.  */
typedef struct {
  void (*randseed_fn) __GMP_PROTO ((gmp_randstate_t rstate, mpz_srcptr seed));
  void (*randget_fn) __GMP_PROTO ((gmp_randstate_t rstate, mp_ptr dest, mpir_ui nbits));
  void (*randclear_fn) __GMP_PROTO ((gmp_randstate_t rstate));
  void (*randiset_fn) __GMP_PROTO ((gmp_randstate_ptr, gmp_randstate_srcptr));
} gmp_randfnptr_t;

/* Macro to obtain a void pointer to the function pointers structure.  */
#define RNG_FNPTR(rstate) ((rstate)->_mp_algdata._mp_lc)

/* Macro to obtain a pointer to the generator's state.
   When used as a lvalue the rvalue needs to be cast to mp_ptr.  */
#define RNG_STATE(rstate) ((rstate)->_mp_seed->_mp_d)

/* Write a given number of random bits to rp.  */
#define _gmp_rand(rp, state, bits)                              \
  do {                                                          \
    gmp_randstate_ptr  __rstate = (state);                      \
    (*((gmp_randfnptr_t *) RNG_FNPTR (__rstate))->randget_fn)   \
       (__rstate, rp, bits);                                    \
  } while (0)

__GMP_DECLSPEC void __gmp_randinit_mt_noseed __GMP_PROTO ((gmp_randstate_t));


/* __gmp_rands is the global state for the old-style random functions, and
   is also used in the test programs (hence the __GMP_DECLSPEC).

   There's no seeding here, so mpz_random etc will generate the same
   sequence every time.  This is not unlike the C library random functions
   if you don't seed them, so perhaps it's acceptable.  Digging up a seed
   from /dev/random or the like would work on many systems, but might
   encourage a false confidence, since it'd be pretty much impossible to do
   something that would work reliably everywhere.  In any case the new style
   functions are recommended to applications which care about randomness, so
   the old functions aren't too important.  */

__GMP_DECLSPEC extern char             __gmp_rands_initialized;
__GMP_DECLSPEC extern gmp_randstate_t  __gmp_rands;

#define RANDS                                       \
  ((__gmp_rands_initialized ? 0                     \
    : (__gmp_rands_initialized = 1,                 \
       __gmp_randinit_mt_noseed (__gmp_rands), 0)), \
   __gmp_rands)

/* this is used by the test programs, to free memory */
#define RANDS_CLEAR()                   \
  do {                                  \
    if (__gmp_rands_initialized)        \
      {                                 \
        __gmp_rands_initialized = 0;    \
        gmp_randclear (__gmp_rands);    \
      }                                 \
  } while (0)

#define mpn_toom42_mulmid_itch(n) (3*(n) + 64)

/* kara uses n+1 limbs of temporary space and then recurses with the balance,
   so need (n+1) + (ceil(n/2)+1) + (ceil(n/4)+1) + ...  This can be solved to
   2n + o(n).  Since n is very limited, o(n) in practice could be around 15.
   For now, assume n is arbitrarily large.  */
#define MPN_KARA_MUL_N_TSIZE(n)   (2*(n) + 2*GMP_LIMB_BITS)
#define MPN_KARA_SQR_N_TSIZE(n)   (2*(n) + 2*GMP_LIMB_BITS)

/* toom3 uses 2n + 2n/3 + o(n) limbs of temporary space if mpn_sublsh1_n is
   unavailable, but just 2n + o(n) if mpn_sublsh1_n is available.  It is hard
   to pin down the value of o(n), since it is a complex function of
   MUL_TOOM3_THRESHOLD and n.  Normally toom3 is used between kara and fft; in
   that case o(n) will be really limited.  If toom3 is used for arbitrarily
   large operands, o(n) will be larger.  These definitions handle operands of
   up to 8956264246117233 limbs.  A single multiplication using toom3 on the
   fastest hardware currently (2003) would need 100 million years, which
   suggests that these limits are acceptable.  */
#if WANT_FFT
#if HAVE_NATIVE_mpn_sublsh1_n
#define MPN_TOOM3_MUL_N_TSIZE(n)  (2*(n) + 63)
#define MPN_TOOM3_MUL_TSIZE(n)    (3*(n) + 63)
#define MPN_TOOM3_SQR_N_TSIZE(n)  (2*(n) + 63)
#else
#define MPN_TOOM3_MUL_N_TSIZE(n)  (2*(n) + 2*(n/3) + 63)
#define MPN_TOOM3_MUL_TSIZE(n)    (3*(n) + 3*(n/3) + 63)
#define MPN_TOOM3_SQR_N_TSIZE(n)  (2*(n) + 2*(n/3) + 63)
#endif
#else /* WANT_FFT */
#if HAVE_NATIVE_mpn_sublsh1_n
#define MPN_TOOM3_MUL_N_TSIZE(n)  (2*(n) + 255)
#define MPN_TOOM3_MUL_TSIZE(n)    (3*(n) + 255)
#define MPN_TOOM3_SQR_N_TSIZE(n)  (2*(n) + 255)
#else
#define MPN_TOOM3_MUL_N_TSIZE(n)  (2*(n) + 2*(n/3) + 255)
#define MPN_TOOM3_MUL_TSIZE(n)    (3*(n) + 3*(n/3) + 255)
#define MPN_TOOM3_SQR_N_TSIZE(n)  (2*(n) + 2*(n/3) + 255)
#endif
#define MPN_TOOM3_MAX_N 285405
#endif /* WANT_FFT */

/* need 2 so that n2>=1 */
#if defined(HAVE_NATIVE_mpn_karaadd) || defined(HAVE_NATIVE_mpn_karasub)
#define MPN_KARA_MUL_N_MINSIZE    8
#define MPN_KARA_SQR_N_MINSIZE    8
#else
#define MPN_KARA_MUL_N_MINSIZE    2
#define MPN_KARA_SQR_N_MINSIZE    2
#endif

/* Need l>=1, ls>=1, and 2*ls > l (the latter for the tD MPN_INCR_U) */
#define MPN_TOOM3_MUL_N_MINSIZE   17 
#define MPN_TOOM4_MUL_N_MINSIZE   32
#define MPN_TOOM8H_MUL_MINSIZE    86
#define MPN_TOOM3_SQR_N_MINSIZE   17
#define MPN_TOOM4_SQR_N_MINSIZE   32
#define MPN_TOOM8_SQR_N_MINSIZE   58
#define MPN_FFT_MUL_N_MINSIZE     64

#define mpn_sqr_diagonal __MPN(sqr_diagonal)
__GMP_DECLSPEC void mpn_sqr_diagonal _PROTO ((mp_ptr, mp_srcptr, mp_size_t));

#define mpn_kara_mul_n	__MPN(kara_mul_n)
__GMP_DECLSPEC void mpn_kara_mul_n _PROTO((mp_ptr, mp_srcptr, mp_srcptr, mp_size_t, mp_ptr));

#define mpn_kara_sqr_n  __MPN(kara_sqr_n)
__GMP_DECLSPEC void mpn_kara_sqr_n _PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_ptr));

#define mpn_toom3_mul_n  __MPN(toom3_mul_n)
__GMP_DECLSPEC void mpn_toom3_mul_n _PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_size_t,mp_ptr));

#define mpn_toom3_mul  __MPN(toom3_mul)
__GMP_DECLSPEC void mpn_toom3_mul _PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_srcptr, 
                                                           mp_size_t,mp_ptr));

#define mpn_toom3_interpolate __MPN(toom3_interpolate)
__GMP_DECLSPEC void mpn_toom3_interpolate _PROTO ((mp_ptr c, mp_ptr v1, mp_ptr v2, mp_ptr vm1,
                     mp_ptr vinf, mp_size_t k, mp_size_t rr2, int sa,
                                          mp_limb_t vinf0, mp_ptr ws));

#define mpn_toom32_mul __MPN(toom32_mul)
__GMP_DECLSPEC void mpn_toom32_mul _PROTO ((mp_ptr c, mp_srcptr a, mp_size_t an, mp_srcptr b, 
                                                      mp_size_t bn, mp_ptr t));

#define mpn_toom42_mul  __MPN(toom42_mul)
__GMP_DECLSPEC void mpn_toom42_mul _PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_srcptr, mp_size_t,mp_ptr));


#define mpn_toom4_mul_n  __MPN(toom4_mul_n)
__GMP_DECLSPEC void mpn_toom4_mul_n _PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_size_t));

#define mpn_toom4_mul  __MPN(toom4_mul)
__GMP_DECLSPEC void mpn_toom4_mul _PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_srcptr,
                                                           mp_size_t));

#define mpn_toom53_mul __MPN(toom53_mul)
__GMP_DECLSPEC void mpn_toom53_mul _PROTO ((mp_ptr rp, mp_srcptr up, mp_size_t un,
                                           mp_srcptr vp, mp_size_t vn));

#define mpn_toom4_interpolate __MPN(toom4_interpolate)
__GMP_DECLSPEC void mpn_toom4_interpolate _PROTO ((mp_ptr rp, mp_size_t * rpn, mp_size_t sn,
               mp_ptr tp, mp_size_t s4, mp_size_t n4, mp_size_t n6, 
               mp_limb_t r30));

#define mpn_toom_eval_dgr3_pm1  __MPN(toom_eval_dgr3_pm1)
__GMP_DECLSPEC int mpn_toom_eval_dgr3_pm1 _PROTO ((mp_ptr xp1, mp_ptr xm1,
            mp_srcptr xp, mp_size_t n, mp_size_t x3n, mp_ptr tp));

#define mpn_toom_eval_dgr3_pm2  __MPN(toom_eval_dgr3_pm2)
__GMP_DECLSPEC int mpn_toom_eval_dgr3_pm2 _PROTO ((mp_ptr xp2, mp_ptr xm2,
            mp_srcptr xp, mp_size_t n, mp_size_t x3n, mp_ptr tp));

#define mpn_toom_eval_pm1  __MPN(toom_eval_pm1)
__GMP_DECLSPEC int mpn_toom_eval_pm1 _PROTO ((mp_ptr xp1, mp_ptr xm1, unsigned k,
           mp_srcptr xp, mp_size_t n, mp_size_t hn, mp_ptr tp));

#define mpn_toom_eval_pm2  __MPN(toom_eval_pm2)
__GMP_DECLSPEC int mpn_toom_eval_pm2 _PROTO ((mp_ptr xp2, mp_ptr xm2, unsigned k,
           mp_srcptr xp, mp_size_t n, mp_size_t hn, mp_ptr tp));

#define mpn_toom_eval_pm2exp  __MPN(toom_eval_pm2exp)
__GMP_DECLSPEC int mpn_toom_eval_pm2exp _PROTO ((mp_ptr xp2, mp_ptr xm2, unsigned k,
              mp_srcptr xp, mp_size_t n, mp_size_t hn, unsigned shift,
              mp_ptr tp));

#define mpn_toom_eval_pm2rexp  __MPN(toom_eval_pm2rexp)
__GMP_DECLSPEC int mpn_toom_eval_pm2rexp _PROTO ((mp_ptr rp, mp_ptr rm,
              unsigned int q, mp_srcptr ap, mp_size_t n, mp_size_t t,
              unsigned int s, mp_ptr ws));

#define mpn_toom_interpolate_16pts  __MPN(toom_interpolate_16pts)
__GMP_DECLSPEC void mpn_toom_interpolate_16pts _PROTO ((mp_ptr pp, mp_ptr r1, mp_ptr r3, 
                   mp_ptr r5, mp_ptr r7, mp_size_t n, mp_size_t spt, 
                         int half, mp_ptr wsi));

#define mpn_toom_couple_handling  __MPN(toom_couple_handling)
__GMP_DECLSPEC void mpn_toom_couple_handling _PROTO ((mp_ptr pp, mp_size_t n, mp_ptr np,
              int nsign, mp_size_t off, int ps, int ns));

#define mpn_toom8h_mul  __MPN(toom8h_mul)
__GMP_DECLSPEC void mpn_toom8h_mul _PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_srcptr,
                                                             mp_size_t));

#define mpn_toom3_sqr_n  __MPN(toom3_sqr_n)
__GMP_DECLSPEC void mpn_toom3_sqr_n _PROTO((mp_ptr, mp_srcptr, mp_size_t, mp_ptr));

#define mpn_toom4_sqr_n  __MPN(toom4_sqr_n)
__GMP_DECLSPEC void mpn_toom4_sqr_n _PROTO((mp_ptr, mp_srcptr, mp_size_t));

#define mpn_toom8_sqr_n  __MPN(toom8_sqr_n)
__GMP_DECLSPEC void mpn_toom8_sqr_n _PROTO((mp_ptr, mp_srcptr, mp_size_t));

#define   mpn_toom42_mulmid __MPN(toom42_mulmid)
__GMP_DECLSPEC void      mpn_toom42_mulmid __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_size_t, mp_ptr));

#define mpn_mulmod_Bexpp1_fft __MPN(mulmod_Bexpp1_fft)
__GMP_DECLSPEC int mpn_mulmod_Bexpp1_fft _PROTO ((mp_ptr op, mp_size_t pl,
	     mp_srcptr n, mp_size_t nl,
	     mp_srcptr m, mp_size_t ml));

#define DC_DIVAPPR_Q_N_ITCH(n) ((n)*4 + 64)
#define DC_BDIV_Q_N_ITCH(n) ((n)/2 + 2)
#define DC_BDIV_QR_N_ITCH(n) (n)

/* #define mpn_tdiv_q  __MPN(tdiv_q) */
/* void mpn_tdiv_q _PROTO ((mp_ptr, mp_size_t, mp_srcptr, mp_size_t, mp_srcptr, mp_size_t)); */

#define mpz_divexact_gcd  __gmpz_divexact_gcd
__GMP_DECLSPEC void mpz_divexact_gcd _PROTO ((mpz_ptr q, mpz_srcptr a, mpz_srcptr d));

#define mpz_prodlimbs  __gmpz_prodlimbs
__GMP_DECLSPEC mp_size_t mpz_prodlimbs (mpz_ptr, mp_ptr, mp_size_t);

#define mpz_oddfac_1  __gmpz_oddfac_1
__GMP_DECLSPEC void mpz_oddfac_1 (mpz_ptr, mp_limb_t, unsigned);

#define mpz_inp_str_nowhite __gmpz_inp_str_nowhite
#ifdef _GMP_H_HAVE_FILE
__GMP_DECLSPEC size_t mpz_inp_str_nowhite _PROTO ((mpz_ptr x, FILE *stream, int base, int c, size_t nread));
#endif

#define mpn_divisible_p __MPN(divisible_p)
__GMP_DECLSPEC int mpn_divisible_p _PROTO ((mp_srcptr ap, mp_size_t asize,
                                 mp_srcptr dp, mp_size_t dsize)) __GMP_ATTRIBUTE_PURE;

#define mpn_rootrem __MPN(rootrem)
__GMP_DECLSPEC mp_size_t mpn_rootrem _PROTO ((mp_ptr, mp_ptr, mp_srcptr, mp_size_t, mp_limb_t));

#define mpn_rootrem_basecase __MPN(rootrem_basecase)
__GMP_DECLSPEC mp_size_t mpn_rootrem_basecase _PROTO ((mp_ptr, mp_ptr, mp_srcptr, mp_size_t, mp_limb_t));

#if ! defined (MPN_COPY_INCR) && HAVE_NATIVE_mpn_copyi
#define MPN_COPY_INCR(dst, src, size)                   \
  do {                                                  \
    ASSERT ((size) >= 0);                               \
    ASSERT (MPN_SAME_OR_INCR_P (dst, src, size));       \
    mpn_copyi (dst, src, size);                         \
  } while (0)
#endif

/* Copy N limbs from SRC to DST incrementing, N==0 allowed.  */
#if ! defined (MPN_COPY_INCR)
#define MPN_COPY_INCR(dst, src, n)                      \
  do {                                                  \
    ASSERT ((n) >= 0);                                  \
    ASSERT (MPN_SAME_OR_INCR_P (dst, src, n));          \
    if ((n) != 0)                                       \
      {                                                 \
    mp_size_t __n = (n) - 1;                        \
    mp_ptr __dst = (dst);                           \
    mp_srcptr __src = (src);                        \
    mp_limb_t __x;                                  \
    __x = *__src++;                                 \
    if (__n != 0)                                   \
      {                                             \
        do                                          \
          {                                         \
        *__dst++ = __x;                         \
        __x = *__src++;                         \
          }                                         \
        while (--__n);                              \
      }                                             \
    *__dst++ = __x;                                 \
      }                                                 \
  } while (0)
#endif

#if ! defined (MPN_COPY_DECR) && HAVE_NATIVE_mpn_copyd
#define MPN_COPY_DECR(dst, src, size)                   \
  do {                                                  \
    ASSERT ((size) >= 0);                               \
    ASSERT (MPN_SAME_OR_DECR_P (dst, src, size));       \
    mpn_copyd (dst, src, size);                         \
  } while (0)
#endif

/* Copy N limbs from SRC to DST decrementing, N==0 allowed.  */
#if ! defined (MPN_COPY_DECR)
#define MPN_COPY_DECR(dst, src, n)                      \
  do {                                                  \
    ASSERT ((n) >= 0);                                  \
    ASSERT (MPN_SAME_OR_DECR_P (dst, src, n));          \
    if ((n) != 0)                                       \
      {                                                 \
    mp_size_t __n = (n) - 1;                        \
    mp_ptr __dst = (dst) + __n;                     \
    mp_srcptr __src = (src) + __n;                  \
    mp_limb_t __x;                                  \
    __x = *__src--;                                 \
    if (__n != 0)                                   \
      {                                             \
        do                                          \
          {                                         \
        *__dst-- = __x;                         \
        __x = *__src--;                         \
          }                                         \
        while (--__n);                              \
      }                                             \
    *__dst-- = __x;                                 \
      }                                                 \
  } while (0)
#endif


#ifndef MPN_COPY
#define MPN_COPY(d,s,n)                         \
  do {                                          \
    ASSERT (MPN_SAME_OR_SEPARATE_P (d, s, n));  \
    MPN_COPY_INCR (d, s, n);                    \
  } while (0)
#endif


/* Set {dst,size} to the limbs of {src,size} in reverse order. */
#define MPN_REVERSE(dst, src, size)                     \
  do {                                                  \
    mp_ptr     __dst = (dst);                           \
    mp_size_t  __size = (size);                         \
    mp_srcptr  __src = (src) + __size - 1;              \
    mp_size_t  __i;                                     \
    ASSERT ((size) >= 0);                               \
    ASSERT (! MPN_OVERLAP_P (dst, size, src, size));    \
    for (__i = 0; __i < __size; __i++)                  \
      {                                                 \
        *__dst = *__src;                                \
        __dst++;                                        \
        __src--;                                        \
      }                                                 \
  } while (0)



/* On the x86s repe/scasl doesn't seem useful, since it takes many cycles to
   start up and would need to strip a lot of zeros before it'd be faster
   than a simple cmpl loop.  Here are some times in cycles for
   std/repe/scasl/cld and cld/repe/scasl (the latter would be for stripping
   low zeros).

                std   cld
           P5    18    16
           P6    46    38
           K6    36    13
           K7    21    20
*/
#ifndef MPN_NORMALIZE
#define MPN_NORMALIZE(DST, NLIMBS) \
  do {									\
    while ((NLIMBS) > 0)                                                \
      {									\
    if ((DST)[(NLIMBS) - 1] != 0)					\
      break;							\
    (NLIMBS)--;							\
      }									\
  } while (0)
#endif
#ifndef MPN_NORMALIZE_NOT_ZERO
#define MPN_NORMALIZE_NOT_ZERO(DST, NLIMBS)     \
  do {                                          \
    ASSERT ((NLIMBS) >= 1);                     \
    while (1)                                   \
      {                                         \
    if ((DST)[(NLIMBS) - 1] != 0)           \
      break;                                \
    (NLIMBS)--;                             \
      }                                         \
  } while (0)
#endif

/* Strip least significant zero limbs from {ptr,size} by incrementing ptr
   and decrementing size.  low should be ptr[0], and will be the new ptr[0]
   on returning.  The number in {ptr,size} must be non-zero, ie. size!=0 and
   somewhere a non-zero limb.  */
#define MPN_STRIP_LOW_ZEROS_NOT_ZERO(ptr, size, low)    \
  do {                                                  \
    ASSERT ((size) >= 1);                               \
    ASSERT ((low) == (ptr)[0]);                         \
                                                        \
    while ((low) == 0)                                  \
      {                                                 \
        (size)--;                                       \
        ASSERT ((size) >= 1);                           \
        (ptr)++;                                        \
        (low) = *(ptr);                                 \
      }                                                 \
  } while (0)

/* Initialize X of type mpz_t with space for NLIMBS limbs.  X should be a
   temporary variable; it will be automatically cleared out at function
   return.  We use __x here to make it possible to accept both mpz_ptr and
   mpz_t arguments.  */
#define MPZ_TMP_INIT(X, NLIMBS)                                         \
  do {                                                                  \
    mpz_ptr __x = (X);                                                  \
    ASSERT ((NLIMBS) >= 1);                                             \
    __x->_mp_alloc = (NLIMBS);                                          \
    __x->_mp_d = (mp_ptr) TMP_ALLOC ((NLIMBS) * BYTES_PER_MP_LIMB);     \
  } while (0)


/* Realloc for an mpz_t WHAT if it has less than NEEDED limbs.  */
#define MPZ_REALLOC(z,n) (UNLIKELY ((n) > ALLOC(z))     \
                          ? (mp_ptr) _mpz_realloc(z,n)  \
                          : PTR(z))

#define MPZ_NEWALLOC MPZ_REALLOC

#define MPZ_EQUAL_1_P(z)  (SIZ(z)==1 && PTR(z)[0] == 1)


/* MPN_FIB2_SIZE(n) is the size in limbs required by mpn_fib2_ui for fp and
   f1p.

   From Knuth vol 1 section 1.2.8, F[n] = phi^n/sqrt(5) rounded to the
   nearest integer, where phi=(1+sqrt(5))/2 is the golden ratio.  So the
   number of bits required is n*log_2((1+sqrt(5))/2) = n*0.6942419.

   The multiplier used is 23/32=0.71875 for efficient calculation on CPUs
   without good floating point.  There's +2 for rounding up, and a further
   +2 since at the last step x limbs are doubled into a 2x+1 limb region
   whereas the actual F[2k] value might be only 2x-1 limbs.

   Note that a division is done first, since on a 32-bit system it's at
   least conceivable to go right up to n==ULONG_MAX.  (F[2^32-1] would be
   about 380Mbytes, plus temporary workspace of about 1.2Gbytes here and
   whatever a multiply of two 190Mbyte numbers takes.)

   Enhancement: When GMP_NUMB_BITS is not a power of 2 the division could be
   worked into the multiplier.  */

#define MPN_FIB2_SIZE(n) \
  ((mp_size_t) ((n) / 32 * 23 / GMP_NUMB_BITS) + 4)


/* FIB_TABLE(n) returns the Fibonacci number F[n].  Must have n in the range
   -1 <= n <= FIB_TABLE_LIMIT

   FIB_TABLE_LUCNUM_LIMIT is the largest n for which L[n] =
   F[n] + 2*F[n-1] fits in a limb.  */

__GMP_DECLSPEC extern const mp_limb_t __gmp_fib_table[];
#define FIB_TABLE(n)  (__gmp_fib_table[(n)+1])

extern const mp_limb_t __gmp_oddfac_table[];
extern const mp_limb_t __gmp_odd2fac_table[];
extern const unsigned char __gmp_fac2cnt_table[];
extern const mp_limb_t __gmp_limbroots_table[];

/* n^log <= GMP_NUMB_MAX, a limb can store log factors less than n */
static inline unsigned
log_n_max (mp_limb_t n)
{
  unsigned log;
  for (log = 8; n > __gmp_limbroots_table[log - 1]; log--);
  return log;
}

#define SIEVESIZE 512		/* FIXME: Allow gmp_init_primesieve to choose */
typedef struct
{
  mpir_ui d;		   /* current index in s[] */
  mpir_ui s0;		   /* number corresponding to s[0] */
  mpir_ui sqrt_s0;	   /* misnomer for sqrt(s[SIEVESIZE-1]) */
  unsigned char s[SIEVESIZE + 1];  /* sieve table */
} gmp_primesieve_t;

#define gmp_init_primesieve __gmp_init_primesieve
__GMP_DECLSPEC void gmp_init_primesieve (gmp_primesieve_t *);

#define gmp_nextprime __gmp_nextprime
__GMP_DECLSPEC mpir_ui gmp_nextprime (gmp_primesieve_t *);

#define gmp_primesieve __gmp_primesieve
__GMP_DECLSPEC mp_limb_t gmp_primesieve (mp_ptr, mp_limb_t);

/* For a threshold between algorithms A and B, size>=thresh is where B
   should be used.  Special value MP_SIZE_T_MAX means only ever use A, or
   value 0 means only ever use B.  The tests for these special values will
   be compile-time constants, so the compiler should be able to eliminate
   the code for the unwanted algorithm.  */

#define ABOVE_THRESHOLD(size,thresh)    \
  ((thresh) == 0                        \
   || ((thresh) != MP_SIZE_T_MAX        \
       && (size) >= (thresh)))
#define BELOW_THRESHOLD(size,thresh)  (! ABOVE_THRESHOLD (size, thresh))

/* If MUL_KARATSUBA_THRESHOLD is not already defined, define it to a
   value which is good on most machines.  */
#ifndef MUL_KARATSUBA_THRESHOLD
#define MUL_KARATSUBA_THRESHOLD 32
#endif

#ifndef SQR_KARATSUBA_THRESHOLD
#define SQR_KARATSUBA_THRESHOLD 32
#endif

/* If MUL_TOOM3_THRESHOLD is not already defined, define it to a
   value which is good on most machines.  */
#ifndef MUL_TOOM3_THRESHOLD
#define MUL_TOOM3_THRESHOLD 128
#endif

#ifndef MUL_TOOM4_THRESHOLD
#define MUL_TOOM4_THRESHOLD 300 
#endif

#ifndef MULMID_TOOM42_THRESHOLD
#define MULMID_TOOM42_THRESHOLD 36 
#endif

#ifndef MUL_TOOM8H_THRESHOLD
#define MUL_TOOM8H_THRESHOLD 401
#endif

#ifndef SQR_TOOM3_THRESHOLD
#define SQR_TOOM3_THRESHOLD 128
#endif

#ifndef SQR_TOOM4_THRESHOLD
#define SQR_TOOM4_THRESHOLD 300
#endif

#ifndef SQR_TOOM8_THRESHOLD
#define SQR_TOOM8_THRESHOLD 401
#endif

#ifndef MULLOW_BASECASE_THRESHOLD
#define MULLOW_BASECASE_THRESHOLD	8
#endif

#ifndef MULLOW_DC_THRESHOLD
#define MULLOW_DC_THRESHOLD	32
#endif

#ifndef MULLOW_MUL_THRESHOLD
#define MULLOW_MUL_THRESHOLD	8192
#endif

#ifndef MULHIGH_BASECASE_THRESHOLD
#define MULHIGH_BASECASE_THRESHOLD	16
#endif

#ifndef MULHIGH_DC_THRESHOLD
#define MULHIGH_DC_THRESHOLD	32
#endif

#ifndef MULHIGH_MUL_THRESHOLD
#define MULHIGH_MUL_THRESHOLD	8192
#endif

#ifndef MULMOD_2EXPM1_THRESHOLD
#define MULMOD_2EXPM1_THRESHOLD	16
#endif

#ifndef FAC_UI_THRESHOLD
#define FAC_UI_THRESHOLD	8192
#endif

#ifndef ROOTREM_THRESHOLD
#define ROOTREM_THRESHOLD	8
#endif

#ifndef DIVREM_HENSEL_QR_1_THRESHOLD
#define DIVREM_HENSEL_QR_1_THRESHOLD 8
#endif

#ifndef RSH_DIVREM_HENSEL_QR_1_THRESHOLD
#define RSH_DIVREM_HENSEL_QR_1_THRESHOLD 8
#endif

#ifndef DIVREM_EUCLID_HENSEL_THRESHOLD
#define DIVREM_EUCLID_HENSEL_THRESHOLD 32
#endif

#ifndef MOD_1_1_THRESHOLD
#define MOD_1_1_THRESHOLD	16
#endif

#ifndef MOD_1_2_THRESHOLD
#define MOD_1_2_THRESHOLD	32
#endif

#ifndef MOD_1_3_THRESHOLD
#define MOD_1_3_THRESHOLD	64
#endif

/* MUL_KARATSUBA_THRESHOLD_LIMIT is the maximum for MUL_KARATSUBA_THRESHOLD.
   In a normal build MUL_KARATSUBA_THRESHOLD is a constant and we use that.
   In a fat binary or tune program build MUL_KARATSUBA_THRESHOLD is a
   variable and a separate hard limit will have been defined.  Similarly for
   TOOM3.  */
#ifndef MUL_KARATSUBA_THRESHOLD_LIMIT
#define MUL_KARATSUBA_THRESHOLD_LIMIT  MUL_KARATSUBA_THRESHOLD
#endif
#ifndef MUL_TOOM3_THRESHOLD_LIMIT
#define MUL_TOOM3_THRESHOLD_LIMIT  MUL_TOOM3_THRESHOLD
#endif
#ifndef MUL_TOOM4_THRESHOLD_LIMIT
#define MUL_TOOM4_THRESHOLD_LIMIT  MUL_TOOM4_THRESHOLD
#endif
#ifndef MUL_TOOM8H_THRESHOLD_LIMIT
#define MUL_TOOM8H_THRESHOLD_LIMIT  MUL_TOOM8H_THRESHOLD
#endif
#ifndef MULLOW_BASECASE_THRESHOLD_LIMIT
#define MULLOW_BASECASE_THRESHOLD_LIMIT  MULLOW_BASECASE_THRESHOLD
#endif

/* SQR_BASECASE_THRESHOLD is where mpn_sqr_basecase should take over from
   mpn_mul_basecase in mpn_sqr_n.  Default is to use mpn_sqr_basecase
   always.  (Note that we certainly always want it if there's a native
   assembler mpn_sqr_basecase.)

   If it turns out that mpn_kara_sqr_n becomes faster than mpn_mul_basecase
   before mpn_sqr_basecase does, then SQR_BASECASE_THRESHOLD is the
   karatsuba threshold and SQR_KARATSUBA_THRESHOLD is 0.  This oddity arises
   more or less because SQR_KARATSUBA_THRESHOLD represents the size up to
   which mpn_sqr_basecase should be used, and that may be never.  */

#ifndef SQR_BASECASE_THRESHOLD
#define SQR_BASECASE_THRESHOLD 0
#endif

#ifndef SQR_KARATSUBA_THRESHOLD
#define SQR_KARATSUBA_THRESHOLD (2*MUL_KARATSUBA_THRESHOLD)
#endif

#ifndef SQR_TOOM3_THRESHOLD
#define SQR_TOOM3_THRESHOLD 128
#endif

#ifndef SQR_TOOM4_THRESHOLD
#define SQR_TOOM4_THRESHOLD 300
#endif

#ifndef SQR_TOOM8_THRESHOLD
#define SQR_TOOM8_THRESHOLD 400
#endif

/* See comments above about MUL_TOOM3_THRESHOLD_LIMIT.  */
#ifndef SQR_TOOM3_THRESHOLD_LIMIT
#define SQR_TOOM3_THRESHOLD_LIMIT  SQR_TOOM3_THRESHOLD
#endif

#ifndef SQR_TOOM4_THRESHOLD_LIMIT
#define SQR_TOOM4_THRESHOLD_LIMIT  SQR_TOOM4_THRESHOLD
#endif

#ifndef SQR_TOOM8_THRESHOLD_LIMIT
#define SQR_TOOM8_THRESHOLD_LIMIT  SQR_TOOM8_THRESHOLD
#endif

/* points at which fft is used for mul/sqr and mulmod_Bexp resp. */
#ifndef MUL_FFT_FULL_THRESHOLD
#define MUL_FFT_FULL_THRESHOLD   (MUL_TOOM8H_THRESHOLD * 10)
#endif
#ifndef SQR_FFT_FULL_THRESHOLD
#define SQR_FFT_FULL_THRESHOLD   (SQR_TOOM8_THRESHOLD * 10)
#endif

#ifndef MUL_FFT_THRESHOLD
#define MUL_FFT_THRESHOLD   (MUL_FFT_FULL_THRESHOLD / 2)
#endif
#ifndef SQR_FFT_THRESHOLD
#define SQR_FFT_THRESHOLD   (SQR_FFT_FULL_THRESHOLD / 2)
#endif

#ifndef FFT_MULMOD_2EXPP1_CUTOFF
#define FFT_MULMOD_2EXPP1_CUTOFF 128
#endif

#if HAVE_NATIVE_mpn_addmul_2 || HAVE_NATIVE_mpn_redc_2

#ifndef REDC_1_TO_REDC_2_THRESHOLD
#define REDC_1_TO_REDC_2_THRESHOLD       15
#endif
#ifndef REDC_2_TO_REDC_N_THRESHOLD
#define REDC_2_TO_REDC_N_THRESHOLD      100
#endif

#else

#ifndef REDC_1_TO_REDC_N_THRESHOLD
#define REDC_1_TO_REDC_N_THRESHOLD      100
#endif

#endif /* HAVE_NATIVE_mpn_addmul_2 || HAVE_NATIVE_mpn_redc_2 */

#ifndef DC_DIV_QR_THRESHOLD
#define DC_DIV_QR_THRESHOLD    (3 * MUL_KARATSUBA_THRESHOLD)
#endif

#ifndef DC_DIVAPPR_Q_N_THRESHOLD
#define DC_DIVAPPR_Q_N_THRESHOLD    (3 * MUL_KARATSUBA_THRESHOLD)
#endif

#ifndef DC_BDIV_QR_THRESHOLD
#define DC_BDIV_QR_THRESHOLD    (3 * MUL_KARATSUBA_THRESHOLD)
#endif

#ifndef DC_BDIV_Q_THRESHOLD
#define DC_BDIV_Q_THRESHOLD    (3 * MUL_KARATSUBA_THRESHOLD)
#endif

#ifndef INV_DIV_QR_THRESHOLD
#define INV_DIV_QR_THRESHOLD    (MUL_FFT_THRESHOLD/3)
#endif

#ifndef INV_DIVAPPR_Q_N_THRESHOLD
#define INV_DIVAPPR_Q_N_THRESHOLD    (MUL_FFT_THRESHOLD/3)
#endif

#ifndef DC_DIV_Q_THRESHOLD
#define DC_DIV_Q_THRESHOLD    (3 * MUL_KARATSUBA_THRESHOLD)
#endif

#ifndef INV_DIV_Q_THRESHOLD
#define INV_DIV_Q_THRESHOLD    (MUL_FFT_THRESHOLD/3)
#endif

#ifndef BINV_NEWTON_THRESHOLD
#define BINV_NEWTON_THRESHOLD           300
#endif

#ifndef DC_DIVAPPR_Q_THRESHOLD
#define DC_DIVAPPR_Q_THRESHOLD    (3 * MUL_TOOM3_THRESHOLD)
#endif

#ifndef INV_DIVAPPR_Q_THRESHOLD
#define INV_DIVAPPR_Q_THRESHOLD    (MUL_FFT_THRESHOLD/2)
#endif

#ifndef GET_STR_DC_THRESHOLD
#define GET_STR_DC_THRESHOLD             18
#endif

#ifndef GET_STR_PRECOMPUTE_THRESHOLD
#define GET_STR_PRECOMPUTE_THRESHOLD     35
#endif

#ifndef SET_STR_DC_THRESHOLD
#define SET_STR_DC_THRESHOLD            750
#endif

#ifndef SET_STR_PRECOMPUTE_THRESHOLD
#define SET_STR_PRECOMPUTE_THRESHOLD   2000
#endif

#ifndef FAC_ODD_THRESHOLD
#define FAC_ODD_THRESHOLD    35
#endif

#ifndef FAC_DSC_THRESHOLD
#define FAC_DSC_THRESHOLD   400
#endif

/* Return non-zero if xp,xsize and yp,ysize overlap.
   If xp+xsize<=yp there's no overlap, or if yp+ysize<=xp there's no
   overlap.  If both these are false, there's an overlap. */
#define MPN_OVERLAP_P(xp, xsize, yp, ysize) \
  ((xp) + (xsize) > (yp) && (yp) + (ysize) > (xp))
#define MEM_OVERLAP_P(xp, xsize, yp, ysize)     \
  (   (char *) (xp) + (xsize) > (char *) (yp)   \
   && (char *) (yp) + (ysize) > (char *) (xp))

/* Return non-zero if xp,xsize and yp,ysize are either identical or not
   overlapping.  Return zero if they're partially overlapping. */
#define MPN_SAME_OR_SEPARATE_P(xp, yp, size)    \
  MPN_SAME_OR_SEPARATE2_P(xp, size, yp, size)
#define MPN_SAME_OR_SEPARATE2_P(xp, xsize, yp, ysize)           \
  ((xp) == (yp) || ! MPN_OVERLAP_P (xp, xsize, yp, ysize))

/* Return non-zero if dst,dsize and src,ssize are either identical or
   overlapping in a way suitable for an incrementing/decrementing algorithm.
   Return zero if they're partially overlapping in an unsuitable fashion. */
#define MPN_SAME_OR_INCR2_P(dst, dsize, src, ssize)             \
  ((dst) <= (src) || ! MPN_OVERLAP_P (dst, dsize, src, ssize))
#define MPN_SAME_OR_INCR_P(dst, src, size)      \
  MPN_SAME_OR_INCR2_P(dst, size, src, size)
#define MPN_SAME_OR_DECR2_P(dst, dsize, src, ssize)             \
  ((dst) >= (src) || ! MPN_OVERLAP_P (dst, dsize, src, ssize))
#define MPN_SAME_OR_DECR_P(dst, src, size)      \
  MPN_SAME_OR_DECR2_P(dst, size, src, size)


/* ASSERT() is a private assertion checking scheme, similar to <assert.h>.
   ASSERT() does the check only if WANT_ASSERT is selected, ASSERT_ALWAYS()
   does it always.  Generally assertions are meant for development, but
   might help when looking for a problem later too.

   Note that strings shouldn't be used within the ASSERT expression,
   eg. ASSERT(strcmp(s,"notgood")!=0), since the quotes upset the "expr"
   used in the !HAVE_STRINGIZE case (ie. K&R).  */

#ifdef __LINE__
#define ASSERT_LINE  __LINE__
#else
#define ASSERT_LINE  -1
#endif

#ifdef __FILE__
#define ASSERT_FILE  __FILE__
#else
#define ASSERT_FILE  ""
#endif

__GMP_DECLSPEC void __gmp_assert_header _PROTO ((const char *filename, int linenum));
__GMP_DECLSPEC void __gmp_assert_fail _PROTO ((const char *filename, int linenum, const char *expr)) ATTRIBUTE_NORETURN;

#if HAVE_STRINGIZE
#define ASSERT_FAIL(expr)  __gmp_assert_fail (ASSERT_FILE, ASSERT_LINE, #expr)
#else
#define ASSERT_FAIL(expr)  __gmp_assert_fail (ASSERT_FILE, ASSERT_LINE, "expr")
#endif

#define ASSERT_ALWAYS(expr)     \
  do {                          \
    if (!(expr))                \
      ASSERT_FAIL (expr);       \
  } while (0)

#if WANT_ASSERT
#define ASSERT(expr)   ASSERT_ALWAYS (expr)
#else
#define ASSERT(expr)   do {} while (0)
#endif


/* ASSERT_CARRY checks the expression is non-zero, and ASSERT_NOCARRY checks
   that it's zero.  In both cases if assertion checking is disabled the
   expression is still evaluated.  These macros are meant for use with
   routines like mpn_add_n() where the return value represents a carry or
   whatever that should or shouldn't occur in some context.  For example,
   ASSERT_NOCARRY (mpn_add_n (rp, s1p, s2p, size)); */
#if WANT_ASSERT
#define ASSERT_CARRY(expr)     ASSERT_ALWAYS ((expr) != 0)
#define ASSERT_NOCARRY(expr)   ASSERT_ALWAYS ((expr) == 0)
#else
#define ASSERT_CARRY(expr)     (expr)
#define ASSERT_NOCARRY(expr)   (expr)
#endif


/* ASSERT_CODE includes code when assertion checking is wanted.  This is the
   same as writing "#if WANT_ASSERT", but more compact.  */
#if WANT_ASSERT
#define ASSERT_CODE(expr)  expr
#else
#define ASSERT_CODE(expr)
#endif


/* Test that an mpq_t is in fully canonical form.  This can be used as
   protection on routines like mpq_equal which give wrong results on
   non-canonical inputs.  */
#if WANT_ASSERT
#define ASSERT_MPQ_CANONICAL(q)                         \
  do {                                                  \
    ASSERT (q->_mp_den._mp_size > 0);                   \
    if (q->_mp_num._mp_size == 0)                       \
      {                                                 \
        /* zero should be 0/1 */                        \
        ASSERT (mpz_cmp_ui (mpq_denref(q), 1L) == 0);   \
      }                                                 \
    else                                                \
      {                                                 \
        /* no common factors */                         \
        mpz_t  __g;                                     \
        mpz_init (__g);                                 \
        mpz_gcd (__g, mpq_numref(q), mpq_denref(q));    \
        ASSERT (mpz_cmp_ui (__g, 1) == 0);              \
        mpz_clear (__g);                                \
      }                                                 \
  } while (0)
#else
#define ASSERT_MPQ_CANONICAL(q)  do {} while (0)
#endif

/* Check that the nail parts are zero. */
#define ASSERT_ALWAYS_LIMB(limb)                \
  do {                                          \
    mp_limb_t  __nail = (limb) & GMP_NAIL_MASK; \
    ASSERT_ALWAYS (__nail == 0);                \
  } while (0)
#define ASSERT_ALWAYS_MPN(ptr, size)            \
  do {                                          \
    /* let whole loop go dead when no nails */  \
    if (GMP_NAIL_BITS != 0)                     \
      {                                         \
        mp_size_t  __i;                         \
        for (__i = 0; __i < (size); __i++)      \
          ASSERT_ALWAYS_LIMB ((ptr)[__i]);      \
      }                                         \
  } while (0)
#if WANT_ASSERT
#define ASSERT_LIMB(limb)       ASSERT_ALWAYS_LIMB (limb)
#define ASSERT_MPN(ptr, size)   ASSERT_ALWAYS_MPN (ptr, size)
#else
#define ASSERT_LIMB(limb)       do {} while (0)
#define ASSERT_MPN(ptr, size)   do {} while (0)
#endif


/* Assert that an mpn region {ptr,size} is zero, or non-zero.
   size==0 is allowed, and in that case {ptr,size} considered to be zero.  */
#if WANT_ASSERT
#define ASSERT_MPN_ZERO_P(ptr,size)     \
  do {                                  \
    mp_size_t  __i;                     \
    ASSERT ((size) >= 0);               \
    for (__i = 0; __i < (size); __i++)  \
      ASSERT ((ptr)[__i] == 0);         \
  } while (0)
#define ASSERT_MPN_NONZERO_P(ptr,size)  \
  do {                                  \
    mp_size_t  __i;                     \
    int        __nonzero = 0;           \
    ASSERT ((size) >= 0);               \
    for (__i = 0; __i < (size); __i++)  \
      if ((ptr)[__i] != 0)              \
        {                               \
          __nonzero = 1;                \
          break;                        \
        }                               \
    ASSERT (__nonzero);                 \
  } while (0)
#else
#define ASSERT_MPN_ZERO_P(ptr,size)     do {} while (0)
#define ASSERT_MPN_NONZERO_P(ptr,size)  do {} while (0)
#endif


#if HAVE_NATIVE_mpn_com_n
#define mpn_com_n __MPN(com_n)
__GMP_DECLSPEC void mpn_com_n _PROTO ((mp_ptr, mp_srcptr, mp_size_t));
#elif !defined(mpn_com_n)
#define mpn_com_n(d,s,n)                                \
  do {                                                  \
    mp_ptr     __d = (d);                               \
    mp_srcptr  __s = (s);                               \
    mp_size_t  __n = (n);                               \
    ASSERT (__n >= 1);                                  \
    ASSERT (MPN_SAME_OR_SEPARATE_P (__d, __s, __n));    \
    do                                                  \
      *__d++ = (~ *__s++) & GMP_NUMB_MASK;              \
    while (--__n);                                      \
  } while (0)
#endif

#define MPN_LOGOPS_N_INLINE(d, s1, s2, n, operation)    \
  do {                                                  \
    mp_ptr	 __d = (d);                             \
    mp_srcptr	 __s1 = (s1);                           \
    mp_srcptr	 __s2 = (s2);                           \
    mp_size_t	 __n = (n);                             \
    ASSERT (__n >= 1);                                  \
    ASSERT (MPN_SAME_OR_SEPARATE_P (__d, __s1, __n));   \
    ASSERT (MPN_SAME_OR_SEPARATE_P (__d, __s2, __n));   \
    do                                                  \
      operation;                                        \
    while (--__n);                                      \
  } while (0)

#if !HAVE_NATIVE_mpn_and_n
#undef mpn_and_n
#define mpn_and_n(d, s1, s2, n) \
  MPN_LOGOPS_N_INLINE (d, s1, s2, n, *__d++ = *__s1++ & *__s2++)
#endif

#if !HAVE_NATIVE_mpn_andn_n
#undef mpn_andn_n
#define mpn_andn_n(d, s1, s2, n) \
  MPN_LOGOPS_N_INLINE (d, s1, s2, n, *__d++ = *__s1++ & ~*__s2++)
#endif

#if !HAVE_NATIVE_mpn_nand_n
#undef mpn_nand_n
#define mpn_nand_n(d, s1, s2, n) \
  MPN_LOGOPS_N_INLINE (d, s1, s2, n, *__d++ = ~(*__s1++ & *__s2++) & GMP_NUMB_MASK)
#endif

#if !HAVE_NATIVE_mpn_ior_n
#undef mpn_ior_n
#define mpn_ior_n(d, s1, s2, n) \
  MPN_LOGOPS_N_INLINE (d, s1, s2, n, *__d++ = *__s1++ | *__s2++)
#endif

#if !HAVE_NATIVE_mpn_iorn_n
#undef mpn_iorn_n
#define mpn_iorn_n(d, s1, s2, n) \
  MPN_LOGOPS_N_INLINE (d, s1, s2, n, *__d++ = (*__s1++ | ~*__s2++) & GMP_NUMB_MASK)
#endif

#if !HAVE_NATIVE_mpn_nior_n
#undef mpn_nior_n
#define mpn_nior_n(d, s1, s2, n) \
  MPN_LOGOPS_N_INLINE (d, s1, s2, n, *__d++ = ~(*__s1++ | *__s2++) & GMP_NUMB_MASK)
#endif

#if !HAVE_NATIVE_mpn_xor_n
#undef mpn_xor_n
#define mpn_xor_n(d, s1, s2, n) \
  MPN_LOGOPS_N_INLINE (d, s1, s2, n, *__d++ = *__s1++ ^ *__s2++)
#endif

#if !HAVE_NATIVE_mpn_xnor_n
#undef mpn_xnor_n
#define mpn_xnor_n(d, s1, s2, n) \
  MPN_LOGOPS_N_INLINE (d, s1, s2, n, *__d++ = ~(*__s1++ ^ *__s2++) & GMP_NUMB_MASK)
#endif

#if HAVE_NATIVE_mpn_not
#define mpn_not __MPN(not)
__GMP_DECLSPEC void mpn_not _PROTO ((mp_ptr,mp_size_t));
#else
#define mpn_not(__xp,__n) mpn_com((__xp),(__xp),(__n))
#endif

#if HAVE_NATIVE_mpn_double
#define mpn_double __MPN(double)
__GMP_DECLSPEC mp_limb_t mpn_double _PROTO ((mp_ptr,mp_size_t));
#else
#define mpn_double(__xp,__n) mpn_lshift1((__xp),(__xp),(__n))
#endif

#if HAVE_NATIVE_mpn_half
#define mpn_half __MPN(half)
__GMP_DECLSPEC mp_limb_t mpn_half _PROTO ((mp_ptr,mp_size_t));
#else
#define mpn_half(__xp,__n) mpn_rshift1((__xp),(__xp),(__n))
#endif

#if HAVE_NATIVE_mpn_lshift1
#define mpn_lshift1 __MPN(lshift1)
__GMP_DECLSPEC mp_limb_t mpn_lshift1 _PROTO ((mp_ptr,mp_srcptr,mp_size_t));
#else
#define mpn_lshift1(__xp,__yp,__n) mpn_lshift((__xp),(__yp),(__n),1)
#endif

#if HAVE_NATIVE_mpn_rshift1
#define mpn_rshift1 __MPN(rshift1)
__GMP_DECLSPEC mp_limb_t mpn_rshift1 _PROTO ((mp_ptr,mp_srcptr,mp_size_t));
#else
#define mpn_rshift1(__xp,__yp,__n) mpn_rshift((__xp),(__yp),(__n),1)
#endif

#if HAVE_NATIVE_mpn_lshift2
#define mpn_lshift2 __MPN(lshift2)
__GMP_DECLSPEC mp_limb_t mpn_lshift2 _PROTO ((mp_ptr,mp_srcptr,mp_size_t));
#else
#define mpn_lshift2(__xp,__yp,__n) mpn_lshift((__xp),(__yp),(__n),2)
#endif

#if HAVE_NATIVE_mpn_rshift2
#define mpn_rshift2 __MPN(rshift2)
__GMP_DECLSPEC mp_limb_t mpn_rshift2 _PROTO ((mp_ptr,mp_srcptr,mp_size_t));
#else
#define mpn_rshift2(__xp,__yp,__n) mpn_rshift((__xp),(__yp),(__n),2)
#endif

#if HAVE_NATIVE_mpn_addlsh1_n
#define mpn_addlsh1_n __MPN(addlsh1_n)
__GMP_DECLSPEC mp_limb_t mpn_addlsh1_n __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_size_t));
#elif HAVE_NATIVE_mpn_addlsh_n  
#define mpn_addlsh1_n(__xp,__yp,__zp,__n) mpn_addlsh_n((__xp),(__yp),(__zp),(__n),1)
#define HAVE_NATIVE_mpn_addlsh1_n 1
#endif

#if HAVE_NATIVE_mpn_sublsh1_n
#define mpn_sublsh1_n __MPN(sublsh1_n)
__GMP_DECLSPEC mp_limb_t mpn_sublsh1_n __GMP_PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_size_t));
#elif HAVE_NATIVE_mpn_sublsh_n  
#define mpn_sublsh1_n(__xp,__yp,__zp,__n) mpn_sublsh_n((__xp),(__yp),(__zp),(__n),1)
#define HAVE_NATIVE_mpn_sublsh1_n 1
#endif

#if HAVE_NATIVE_mpn_inclsh_n
#define mpn_inclsh_n __MPN(inclsh_n)
__GMP_DECLSPEC mp_limb_t mpn_inclsh_n __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t, unsigned int));
#elif HAVE_NATIVE_mpn_addlsh_n  
#define mpn_inclsh_n(__xp,__yp,__n,__c) mpn_addlsh_n((__xp),(__xp),(__yp),(__n),(__c))
#define HAVE_NATIVE_mpn_inclsh_n 1
#endif

#if HAVE_NATIVE_mpn_declsh_n
#define mpn_declsh_n __MPN(declsh_n)
__GMP_DECLSPEC mp_limb_t mpn_declsh_n __GMP_PROTO ((mp_ptr, mp_srcptr, mp_size_t, unsigned int));
#elif HAVE_NATIVE_mpn_sublsh_n  
#define mpn_declsh_n(__xp,__yp,__n,__c) mpn_sublsh_n((__xp),(__xp),(__yp),(__n),(__c))
#define HAVE_NATIVE_mpn_declsh_n 1
#endif

#if HAVE_NATIVE_mpn_store
#define mpn_store __MPN(store)
__GMP_DECLSPEC mp_limb_t mpn_store _PROTO ((mp_ptr,mp_size_t,mp_limb_t));
#else
#define mpn_store(dst, n,val)			\
  do {						\
    ASSERT ((n) >= 0);				\
    if ((n) != 0)				\
      {						\
    mp_ptr __dst = (dst);			\
    mp_size_t __n = (n);			\
    do					\
      *__dst++ = val;			\
    while (--__n);				\
      }						\
  } while (0)
#endif

#define MPN_ZERO(dst,n)	mpn_store(dst,n,0)

/* ADDC_LIMB sets w=x+y and cout to 0 or 1 for a carry from that addition. */
#if GMP_NAIL_BITS == 0
#define ADDC_LIMB(cout, w, x, y)        \
  do {                                  \
    mp_limb_t  __x = (x);               \
    mp_limb_t  __y = (y);               \
    mp_limb_t  __w = __x + __y;         \
    (w) = __w;                          \
    (cout) = __w < __x;                 \
  } while (0)
#else
#define ADDC_LIMB(cout, w, x, y)        \
  do {                                  \
    mp_limb_t  __w;                     \
    ASSERT_LIMB (x);                    \
    ASSERT_LIMB (y);                    \
    __w = (x) + (y);                    \
    (w) = __w & GMP_NUMB_MASK;          \
    (cout) = __w >> GMP_NUMB_BITS;      \
  } while (0)
#endif

/* SUBC_LIMB sets w=x-y and cout to 0 or 1 for a borrow from that
   subtract.  */
#if GMP_NAIL_BITS == 0
#define SUBC_LIMB(cout, w, x, y)        \
  do {                                  \
    mp_limb_t  __x = (x);               \
    mp_limb_t  __y = (y);               \
    mp_limb_t  __w = __x - __y;         \
    (w) = __w;                          \
    (cout) = __w > __x;                 \
  } while (0)
#else
#define SUBC_LIMB(cout, w, x, y)        \
  do {                                  \
    mp_limb_t  __w = (x) - (y);         \
    (w) = __w & GMP_NUMB_MASK;          \
    (cout) = __w >> (GMP_LIMB_BITS-1);  \
  } while (0)
#endif


/* MPN_INCR_U does {ptr,size} += n, MPN_DECR_U does {ptr,size} -= n, both
   expecting no carry (or borrow) from that.

   The size parameter is only for the benefit of assertion checking.  In a
   normal build it's unused and the carry/borrow is just propagated as far
   as it needs to go.

   On random data, usually only one or two limbs of {ptr,size} get updated,
   so there's no need for any sophisticated looping, just something compact
   and sensible.

   FIXME: Switch all code from mpn_{incr,decr}_u to MPN_{INCR,DECR}_U,
   declaring their operand sizes, then remove the former.  This is purely
   for the benefit of assertion checking.  */

#if GMP_NAIL_BITS == 0
#ifndef mpn_incr_u
#define mpn_incr_u(p,incr)                              \
  do {                                                  \
    mp_limb_t __x;                                      \
    mp_ptr __p = (p);                                   \
    if (__builtin_constant_p (incr) && (incr) == 1)     \
      {                                                 \
        while (++(*(__p++)) == 0)                       \
          ;                                             \
      }                                                 \
    else                                                \
      {                                                 \
        __x = *__p + (incr);                            \
        *__p = __x;                                     \
        if (__x < (incr))                               \
          while (++(*(++__p)) == 0)                     \
            ;                                           \
      }                                                 \
  } while (0)
#endif
#ifndef mpn_decr_u
#define mpn_decr_u(p,incr)                              \
  do {                                                  \
    mp_limb_t __x;                                      \
    mp_ptr __p = (p);                                   \
    if (__builtin_constant_p (incr) && (incr) == 1)     \
      {                                                 \
        while ((*(__p++))-- == 0)                       \
          ;                                             \
      }                                                 \
    else                                                \
      {                                                 \
        __x = *__p;                                     \
        *__p = __x - (incr);                            \
        if (__x < (incr))                               \
          while ((*(++__p))-- == 0)                     \
            ;                                           \
      }                                                 \
  } while (0)
#endif
#endif

#if GMP_NAIL_BITS >= 1
#ifndef mpn_incr_u
#define mpn_incr_u(p,incr)                              \
  do {							\
    mp_limb_t __x;					\
    mp_ptr __p = (p);					\
    if (__builtin_constant_p (incr) && (incr) == 1)	\
      {							\
    do						\
      {						\
        __x = (*__p + 1) & GMP_NUMB_MASK;		\
        *__p++ = __x;				\
      }						\
    while (__x == 0);				\
      }							\
    else						\
      {							\
    __x = (*__p + (incr));				\
    *__p++ = __x & GMP_NUMB_MASK;			\
    if (__x >> GMP_NUMB_BITS != 0)			\
      {						\
        do						\
          {						\
        __x = (*__p + 1) & GMP_NUMB_MASK;	\
        *__p++ = __x;				\
          }						\
        while (__x == 0);				\
      }						\
      }							\
  } while (0)
#endif
#ifndef mpn_decr_u
#define mpn_decr_u(p,incr)				\
  do {							\
    mp_limb_t __x;					\
    mp_ptr __p = (p);					\
    if (__builtin_constant_p (incr) && (incr) == 1)	\
      {							\
    do						\
      {						\
        __x = *__p;					\
        *__p++ = (__x - 1) & GMP_NUMB_MASK;		\
      }						\
    while (__x == 0);				\
      }							\
    else						\
      {							\
    __x = *__p - (incr);				\
    *__p++ = __x & GMP_NUMB_MASK;			\
    if (__x >> GMP_NUMB_BITS != 0)			\
      {						\
        do						\
          {						\
        __x = *__p;				\
        *__p++ = (__x - 1) & GMP_NUMB_MASK;	\
          }						\
        while (__x == 0);				\
      }						\
      }							\
  } while (0)
#endif
#endif

#ifndef MPN_INCR_U
#if WANT_ASSERT
#define MPN_INCR_U(ptr, size, n)                        \
  do {                                                  \
    ASSERT ((size) >= 1);                               \
    ASSERT_NOCARRY (mpn_add_1 (ptr, ptr, size, n));     \
  } while (0)
#else
#define MPN_INCR_U(ptr, size, n)   mpn_incr_u (ptr, n)
#endif
#endif

#ifndef MPN_DECR_U
#if WANT_ASSERT
#define MPN_DECR_U(ptr, size, n)                        \
  do {                                                  \
    ASSERT ((size) >= 1);                               \
    ASSERT_NOCARRY (mpn_sub_1 (ptr, ptr, size, n));     \
  } while (0)
#else
#define MPN_DECR_U(ptr, size, n)   mpn_decr_u (ptr, n)
#endif
#endif


/* Structure for conversion between internal binary format and
   strings in base 2..36.  */
struct bases
{
  /* Number of digits in the conversion base that always fits in an mp_limb_t.
     For example, for base 10 on a machine where a mp_limb_t has 32 bits this
     is 9, since 10**9 is the largest number that fits into a mp_limb_t.  */
  int chars_per_limb;

  /* log(2)/log(conversion_base) */
  double chars_per_bit_exactly;

  /* base**chars_per_limb, i.e. the biggest number that fits a word, built by
     factors of base.  Exception: For 2, 4, 8, etc, big_base is log2(base),
     i.e. the number of bits used to represent each digit in the base.  */
  mp_limb_t big_base;

  /* A BITS_PER_MP_LIMB bit approximation to 1/big_base, represented as a
     fixed-point number.  Instead of dividing by big_base an application can
     choose to multiply by big_base_inverted.  */
  mp_limb_t big_base_inverted;
};

#define mp_bases __MPN(bases)
#define __mp_bases __MPN(bases)
__GMP_DECLSPEC extern const struct bases mp_bases[257];

/* the following are exposed for the benefit of MPIR.Net. */

__GMP_DECLSPEC extern const unsigned char __gmp_digit_value_tab[480];

/* This struct is used to pass local state between functions that comprise the raw i/o interfaces mpz_inp_raw and mpz_out_raw.
   Previously monolithic, they were split into several calls for the benefit of MPIR.Net.
   The separation did not change the contract nor the implementation, merely separating the routines into several steps,
   in order for MPIR.Net to consume the raw format processing code while substituting its own file I/O. */
typedef struct
{
    char* allocated;
    size_t allocatedSize;
    char* written;
    size_t writtenSize;
} __mpir_out_struct;
typedef __mpir_out_struct mpir_out_struct[1];
typedef __mpir_out_struct *mpir_out_ptr;
/* Part of mpz_inp_raw that decodes input size and allocates appropriate memory. Does not need _GMP_H_HAVE_FILE. Also used by MPIR.Net. */
__GMP_DECLSPEC void mpz_inp_raw_p __GMP_PROTO ((mpz_ptr x, unsigned char* csize_bytes, mpir_out_ptr out));
/* Part of mpz_inp_raw that reconstitutes limb data from raw format. Does not need _GMP_H_HAVE_FILE. Also used by MPIR.Net. */
__GMP_DECLSPEC void mpz_inp_raw_m __GMP_PROTO ((mpz_ptr x, mpir_out_ptr out));
/* Part of mpz_out_raw that performs raw output into memory in preparation for writing out to a file. Does not need _GMP_H_HAVE_FILE. Also used by MPIR.Net. */
__GMP_DECLSPEC void mpz_out_raw_m __GMP_PROTO((mpir_out_ptr, mpz_srcptr));

/* End of MPIR.Net consumables */

/* For power of 2 bases this is exact.  For other bases the result is either
   exact or one too big.

   To be exact always it'd be necessary to examine all the limbs of the
   operand, since numbers like 100..000 and 99...999 generally differ only
   in the lowest limb.  It'd be possible to examine just a couple of high
   limbs to increase the probability of being exact, but that doesn't seem
   worth bothering with.  */

#define MPN_SIZEINBASE(result, ptr, size, base)                         \
  do {                                                                  \
    int       __lb_base, __cnt;                                         \
    size_t __totbits;                                                   \
                                                                        \
    ASSERT ((size) >= 0);                                               \
    ASSERT ((base) >= 2);                                               \
    ASSERT ((base) < numberof (mp_bases));                              \
                                                                        \
    /* Special case for X == 0.  */                                     \
    if ((size) == 0)                                                    \
      (result) = 1;                                                     \
    else                                                                \
      {                                                                 \
        /* Calculate the total number of significant bits of X.  */     \
        count_leading_zeros (__cnt, (ptr)[(size)-1]);                   \
        __totbits = (size_t) (size) * GMP_NUMB_BITS - (__cnt - GMP_NAIL_BITS);\
                                                                        \
        if (POW2_P (base))                                              \
          {                                                             \
            __lb_base = mp_bases[base].big_base;                        \
            (result) = (__totbits + __lb_base - 1) / __lb_base;         \
          }                                                             \
        else                                                            \
          (result) = (size_t)                                           \
            (__totbits * mp_bases[base].chars_per_bit_exactly) + 1;     \
      }                                                                 \
  } while (0)

#define MPN_SIZEINBASE_2EXP(result, ptr, size, base2exp)			\
  do {										\
    int          __cnt;								\
    mp_bitcnt_t  __totbits;							\
    ASSERT ((size) > 0);							\
    ASSERT ((ptr)[(size)-1] != 0);						\
    count_leading_zeros (__cnt, (ptr)[(size)-1]);				\
    __totbits = (mp_bitcnt_t) (size) * GMP_NUMB_BITS - (__cnt - GMP_NAIL_BITS);	\
    (result) = (__totbits + (base2exp)-1) / (base2exp);				\
  } while (0)

/* eliminate mp_bases lookups for base==16 */
#define MPN_SIZEINBASE_16(result, ptr, size)                            \
  do {                                                                  \
    int       __cnt;                                                    \
    mp_size_t __totbits;                                                \
                                                                        \
    ASSERT ((size) >= 0);                                               \
                                                                        \
    /* Special case for X == 0.  */                                     \
    if ((size) == 0)                                                    \
      (result) = 1;                                                     \
    else                                                                \
      {                                                                 \
        /* Calculate the total number of significant bits of X.  */     \
        count_leading_zeros (__cnt, (ptr)[(size)-1]);                   \
        __totbits = (size_t) (size) * GMP_NUMB_BITS - (__cnt - GMP_NAIL_BITS);\
        (result) = (__totbits + 4 - 1) / 4;                             \
      }                                                                 \
  } while (0)

/* bit count to limb count, rounding up */
#define BITS_TO_LIMBS(n)  (((n) + (GMP_NUMB_BITS - 1)) / GMP_NUMB_BITS)

/* MPN_SET_UI sets an mpn (ptr, cnt) to given ui.  MPZ_FAKE_UI creates fake
   mpz_t from ui.  The zp argument must have room for LIMBS_PER_UI limbs
   in both cases (LIMBS_PER_UI is also defined here.) */
#if BITS_PER_UI <= GMP_NUMB_BITS /* need one limb per ulong */

#define LIMBS_PER_UI 1
#define MPN_SET_UI(zp, zn, u)   \
  (zp)[0] = (u);                \
  (zn) = ((zp)[0] != 0);
#define MPZ_FAKE_UI(z, zp, u)   \
  (zp)[0] = (u);                \
  PTR (z) = (zp);               \
  SIZ (z) = ((zp)[0] != 0);     \
  ASSERT_CODE (ALLOC (z) = 1);

#else /* need two limbs per ulong */

#define LIMBS_PER_UI 2
#define MPN_SET_UI(zp, zn, u)                          \
  (zp)[0] = (u) & GMP_NUMB_MASK;                       \
  (zp)[1] = (u) >> GMP_NUMB_BITS;                      \
  (zn) = ((zp)[1] != 0 ? 2 : (zp)[0] != 0 ? 1 : 0);
#define MPZ_FAKE_UI(z, zp, u)                          \
  (zp)[0] = (u) & GMP_NUMB_MASK;                       \
  (zp)[1] = (u) >> GMP_NUMB_BITS;                      \
  SIZ (z) = ((zp)[1] != 0 ? 2 : (zp)[0] != 0 ? 1 : 0); \
  PTR (z) = (zp);                                      \
  ASSERT_CODE (ALLOC (z) = 2);

#endif

/* LIMB_HIGHBIT_TO_MASK(n) examines the high bit of a limb value and turns 1
   or 0 there into a limb 0xFF..FF or 0 respectively.

   On most CPUs this is just an arithmetic right shift by GMP_LIMB_BITS-1,
   but C99 doesn't guarantee signed right shifts are arithmetic, so we have
   a little compile-time test and a fallback to a "? :" form.  The latter is
   necessary for instance on Cray vector systems.

   Recent versions of gcc (eg. 3.3) will in fact optimize a "? :" like this
   to an arithmetic right shift anyway, but it's good to get the desired
   shift on past versions too (in particular since an important use of
   LIMB_HIGHBIT_TO_MASK is in udiv_qrnnd_preinv).  */

#define LIMB_HIGHBIT_TO_MASK(n)                                 \
  (((mp_limb_signed_t) -1 >> 1) < 0                             \
   ? (mp_limb_signed_t) (n) >> (GMP_LIMB_BITS - 1)              \
   : (n) & GMP_LIMB_HIGHBIT ? MP_LIMB_T_MAX : CNST_LIMB(0))


/* Use a library function for invert_limb, if available. */
#define mpn_invert_limb  __MPN(invert_limb)
mp_limb_t mpn_invert_limb _PROTO ((mp_limb_t)) ATTRIBUTE_CONST;
#if ! defined (invert_limb) && HAVE_NATIVE_mpn_invert_limb
#define invert_limb(invxl,xl)           \
  do {                                  \
    (invxl) = mpn_invert_limb (xl);     \
  } while (0)
#endif

#ifndef invert_limb
#define invert_limb(invxl,xl)                   \
  do {                                          \
    mp_limb_t dummy;                            \
    ASSERT ((xl) != 0);                         \
    udiv_qrnnd (invxl, dummy, ~(xl), ~CNST_LIMB(0), xl);  \
  } while (0)
#endif

#define mpir_invert_pi1(dinv, d1, d0)					\
  do {									\
    mp_limb_t _v, _p, _t1, _t0, _mask;					\
    invert_limb (_v, d1);						\
    _p = (d1) * _v;							\
    _p += (d0);								\
    if (_p < (d0))							\
      {									\
	_v--;								\
	_mask = -(mp_limb_t) (_p >= (d1));				\
	_p -= (d1);							\
	_v += _mask;							\
	_p -= _mask & (d1);						\
      }									\
    umul_ppmm (_t1, _t0, d0, _v);					\
    _p += _t1;								\
    if (_p < _t1)							\
      {									\
	_v--;								\
	if (UNLIKELY (_p >= (d1)))					\
	  {								\
	    if (_p > (d1) || _t0 >= (d0))				\
	      _v--;							\
	  }								\
      }									\
    dinv = _v;							\
  } while (0)

/* For compatibility with GMP only */
#define invert_pi1(dinv, d1, d0)				\
   mpir_invert_pi1((dinv).inv32, d1, d0)

/* Compute quotient the quotient and remainder for n / d. Requires d
   >= B^2 / 2 and n < d B. di is the inverse

     floor ((B^3 - 1) / (d0 + d1 B)) - B.

   NOTE: Output variables are updated multiple times. Only some inputs
   and outputs may overlap.
*/
#define udiv_qr_3by2(q, r1, r0, n2, n1, n0, d1, d0, dinv)		\
  do {									\
    mp_limb_t _q0, _t1, _t0;					\
    umul_ppmm ((q), _q0, (n2), (dinv));					\
    add_ssaaaa ((q), _q0, (q), _q0, (n2), (n1));			\
                                    \
    /* Compute the two most significant limbs of n - q'd */		\
    (r1) = (n1) - (d1) * (q);						\
    sub_ddmmss ((r1), (r0), (r1), (n0), (d1), (d0));			\
    umul_ppmm (_t1, _t0, (d0), (q));					\
    sub_ddmmss ((r1), (r0), (r1), (r0), _t1, _t0);			\
    (q)++;								\
                                    \
    /* Conditionally adjust q and the remainders */			\
    if ((r1) >= _q0) {				\
       (q)--;							\
       add_ssaaaa ((r1), (r0), (r1), (r0), (d1), (d0));	} \
    if (UNLIKELY ((r1) >= (d1)))					\
      {									\
    if ((r1) > (d1) || (r0) >= (d0))				\
      {								\
        (q)++;							\
        sub_ddmmss ((r1), (r0), (r1), (r0), (d1), (d0));		\
      }								\
      }									\
  } while (0)

#ifndef udiv_qrnnd_preinv
#define udiv_qrnnd_preinv udiv_qrnnd_preinv2
#endif

/* Divide the two-limb number in (NH,,NL) by D, with DI being the largest
   limb not larger than (2**(2*BITS_PER_MP_LIMB))/D - (2**BITS_PER_MP_LIMB).
   If this would yield overflow, DI should be the largest possible number
   (i.e., only ones).  For correct operation, the most significant bit of D
   has to be set.  Put the quotient in Q and the remainder in R.  */
#define udiv_qrnnd_preinv1(q, r, nh, nl, d, di)				\
  do {									\
    mp_limb_t _q, _ql, _r;						\
    mp_limb_t _xh, _xl;							\
    ASSERT ((d) != 0);							\
    umul_ppmm (_q, _ql, (nh), (di));					\
    _q += (nh);	/* Compensate, di is 2**GMP_LIMB_BITS too small */	\
    umul_ppmm (_xh, _xl, _q, (d));					\
    sub_ddmmss (_xh, _r, (nh), (nl), _xh, _xl);				\
    if (_xh != 0)							\
      {									\
    sub_ddmmss (_xh, _r, _xh, _r, 0, (d));				\
    _q += 1;							\
    if (_xh != 0)							\
      {								\
        _r -= (d);							\
        _q += 1;							\
      }								\
      }									\
    if (_r >= (d))							\
      {									\
    _r -= (d);							\
    _q += 1;							\
      }									\
    (r) = _r;								\
    (q) = _q;								\
  } while (0)

/* Like udiv_qrnnd_preinv, but branch-free. */
#define udiv_qrnnd_preinv2(q, r, nh, nl, d, di)				\
  do {									\
    mp_limb_t _n2, _n10, _nmask, _nadj, _q1;				\
    mp_limb_t _xh, _xl;							\
    _n2 = (nh);								\
    _n10 = (nl);							\
    _nmask = LIMB_HIGHBIT_TO_MASK (_n10);				\
    _nadj = _n10 + (_nmask & (d));					\
    umul_ppmm (_xh, _xl, di, _n2 - _nmask);				\
    add_ssaaaa (_xh, _xl, _xh, _xl, _n2, _nadj);			\
    _q1 = ~_xh;								\
    umul_ppmm (_xh, _xl, _q1, d);					\
    add_ssaaaa (_xh, _xl, _xh, _xl, nh, nl);				\
    _xh -= (d);					/* xh = 0 or -1 */	\
    (r) = _xl + ((d) & _xh);						\
    (q) = _xh - _q1;							\
  } while (0)

/* Like udiv_qrnnd_preinv2, but for for any value D.  DNORM is D shifted left
   so that its most significant bit is set.  LGUP is ceil(log2(D)).  */
#define udiv_qrnnd_preinv2gen(q, r, nh, nl, d, di, dnorm, lgup) \
  do {									\
    mp_limb_t _n2, _n10, _nmask, _nadj, _q1;				\
    mp_limb_t _xh, _xl;							\
    _n2 = ((nh) << (BITS_PER_MP_LIMB - (lgup))) + ((nl) >> 1 >> (l - 1));\
    _n10 = (nl) << (BITS_PER_MP_LIMB - (lgup));				\
    _nmask = LIMB_HIGHBIT_TO_MASK (_n10);				\
    _nadj = _n10 + (_nmask & (dnorm));					\
    umul_ppmm (_xh, _xl, di, _n2 - _nmask);				\
    add_ssaaaa (_xh, _xl, _xh, _xl, _n2, _nadj);			\
    _q1 = ~_xh;								\
    umul_ppmm (_xh, _xl, _q1, d);					\
    add_ssaaaa (_xh, _xl, _xh, _xl, nh, nl);				\
    _xh -= (d);								\
    (r) = _xl + ((d) & _xh);						\
    (q) = _xh - _q1;							\
  } while (0)


#ifndef mpn_preinv_divrem_1  /* if not done with cpuvec in a fat binary */
#define mpn_preinv_divrem_1  __MPN(preinv_divrem_1)
__GMP_DECLSPEC mp_limb_t mpn_preinv_divrem_1 _PROTO ((mp_ptr, mp_size_t, mp_srcptr, mp_size_t, mp_limb_t, mp_limb_t, int));
#endif


/* USE_PREINV_DIVREM_1 is whether to use mpn_preinv_divrem_1, as opposed to
   the plain mpn_divrem_1.  Likewise USE_PREINV_MOD_1 chooses between
   mpn_preinv_mod_1 and plain mpn_mod_1.  The default for both is yes, since
   the few CISC chips where preinv is not good have defines saying so.  */
#ifndef USE_PREINV_DIVREM_1
#define USE_PREINV_DIVREM_1   1
#endif
#ifndef USE_PREINV_MOD_1
#define USE_PREINV_MOD_1   1
#endif

#if USE_PREINV_DIVREM_1
#define MPN_DIVREM_OR_PREINV_DIVREM_1(qp,xsize,ap,size,d,dinv,shift)    \
  mpn_preinv_divrem_1 (qp, xsize, ap, size, d, dinv, shift)
#else
#define MPN_DIVREM_OR_PREINV_DIVREM_1(qp,xsize,ap,size,d,dinv,shift)    \
  mpn_divrem_1 (qp, xsize, ap, size, d)
#endif

#if USE_PREINV_MOD_1
#define MPN_MOD_OR_PREINV_MOD_1(src,size,divisor,inverse)       \
  mpn_preinv_mod_1 (src, size, divisor, inverse)
#else
#define MPN_MOD_OR_PREINV_MOD_1(src,size,divisor,inverse)       \
  mpn_mod_1 (src, size, divisor)
#endif


#ifndef mpn_mod_34lsub1  /* if not done with cpuvec in a fat binary */
#define mpn_mod_34lsub1 __MPN(mod_34lsub1)
__GMP_DECLSPEC mp_limb_t mpn_mod_34lsub1 _PROTO ((mp_srcptr, mp_size_t)) __GMP_ATTRIBUTE_PURE;
#endif


/* DIVEXACT_1_THRESHOLD is at what size to use mpn_divexact_1, as opposed to
   plain mpn_divrem_1.  Likewise MODEXACT_1_ODD_THRESHOLD for
   mpn_modexact_1_odd against plain mpn_mod_1.  On most CPUs divexact and
   modexact are faster at all sizes, so the defaults are 0.  Those CPUs
   where this is not right have a tuned threshold.  */
#ifndef DIVEXACT_1_THRESHOLD
#define DIVEXACT_1_THRESHOLD  0
#endif
#ifndef MODEXACT_1_ODD_THRESHOLD
#define MODEXACT_1_ODD_THRESHOLD  0
#endif

#ifndef mpn_divexact_1  /* if not done with cpuvec in a fat binary */
#define mpn_divexact_1 __MPN(divexact_1)
__GMP_DECLSPEC void mpn_divexact_1 _PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_limb_t));
#endif

#define MPN_DIVREM_OR_DIVEXACT_1(dst, src, size, divisor)                     \
  do {                                                                        \
    if (BELOW_THRESHOLD (size, DIVEXACT_1_THRESHOLD))                         \
      ASSERT_NOCARRY (mpn_divrem_1 (dst, (mp_size_t) 0, src, size, divisor)); \
    else                                                                      \
      {                                                                       \
        ASSERT (mpn_mod_1 (src, size, divisor) == 0);                         \
        mpn_divexact_1 (dst, src, size, divisor);                             \
      }                                                                       \
  } while (0)

#ifndef mpn_modexact_1c_odd  /* if not done with cpuvec in a fat binary */
#define mpn_modexact_1c_odd  __MPN(modexact_1c_odd)
__GMP_DECLSPEC mp_limb_t mpn_modexact_1c_odd _PROTO ((mp_srcptr src, mp_size_t size,
                                       mp_limb_t divisor, mp_limb_t c)) __GMP_ATTRIBUTE_PURE;
#endif

#if HAVE_NATIVE_mpn_modexact_1_odd
#define mpn_modexact_1_odd   __MPN(modexact_1_odd)
__GMP_DECLSPEC mp_limb_t mpn_modexact_1_odd _PROTO ((mp_srcptr src, mp_size_t size,
                                      mp_limb_t divisor)) __GMP_ATTRIBUTE_PURE;
#else
#define mpn_modexact_1_odd(src,size,divisor) \
  mpn_modexact_1c_odd (src, size, divisor, CNST_LIMB(0))
#endif

#define MPN_MOD_OR_MODEXACT_1_ODD(src,size,divisor)			\
  (ABOVE_THRESHOLD (size, MODEXACT_1_ODD_THRESHOLD)			\
   ? mpn_modexact_1_odd (src, size, divisor)				\
   : mpn_mod_1 (src, size, divisor))


/* modlimb_invert() sets inv to the multiplicative inverse of n modulo
   2^GMP_NUMB_BITS, ie. satisfying inv*n == 1 mod 2^GMP_NUMB_BITS.
   n must be odd (otherwise such an inverse doesn't exist).

   This is not to be confused with invert_limb(), which is completely
   different.

   The table lookup gives an inverse with the low 8 bits valid, and each
   multiply step doubles the number of bits.  See Jebelean "An algorithm for
   exact division" end of section 4 (reference in gmp.texi).

   Possible enhancement: Could use UHWtype until the last step, if half-size
   multiplies are faster (might help under _LONG_LONG_LIMB).

   Alternative: As noted in Granlund and Montgomery "Division by Invariant
   Integers using Multiplication" (reference in gmp.texi), n itself gives a
   3-bit inverse immediately, and could be used instead of a table lookup.
   A 4-bit inverse can be obtained effectively from xoring bits 1 and 2 into
   bit 3, for instance with (((n + 2) & 4) << 1) ^ n.  */

#define modlimb_invert_table  __gmp_modlimb_invert_table
__GMP_DECLSPEC extern const unsigned char  modlimb_invert_table[128];

#define modlimb_invert(inv,n)						\
  do {									\
    mp_limb_t  __n = (n);						\
    mp_limb_t  __inv;							\
    ASSERT ((__n & 1) == 1);						\
                                    \
    __inv = modlimb_invert_table[(__n/2) & 0x7F]; /*  8 */		\
    if (GMP_NUMB_BITS > 8)   __inv = 2 * __inv - __inv * __inv * __n;	\
    if (GMP_NUMB_BITS > 16)  __inv = 2 * __inv - __inv * __inv * __n;	\
    if (GMP_NUMB_BITS > 32)  __inv = 2 * __inv - __inv * __inv * __n;	\
                                    \
    if (GMP_NUMB_BITS > 64)						\
      {									\
    int  __invbits = 64;						\
    do {								\
      __inv = 2 * __inv - __inv * __inv * __n;			\
      __invbits *= 2;						\
    } while (__invbits < GMP_NUMB_BITS);				\
      }									\
                                    \
    ASSERT ((__inv * __n & GMP_NUMB_MASK) == 1);			\
    (inv) = __inv & GMP_NUMB_MASK;					\
  } while (0)

/* Multiplicative inverse of 3, modulo 2^GMP_NUMB_BITS.
   Eg. 0xAAAAAAAB for 32 bits, 0xAAAAAAAAAAAAAAAB for 64 bits.
   GMP_NUMB_MAX/3*2+1 is right when GMP_NUMB_BITS is even, but when it's odd
   we need to start from GMP_NUMB_MAX>>1. */
#define MODLIMB_INVERSE_3 (((GMP_NUMB_MAX >> (GMP_NUMB_BITS % 2)) / 3) * 2 + 1)

/* ceil(GMP_NUMB_MAX/3) and ceil(2*GMP_NUMB_MAX/3).
   These expressions work because GMP_NUMB_MAX%3 != 0 for all GMP_NUMB_BITS. */
#define GMP_NUMB_CEIL_MAX_DIV3   (GMP_NUMB_MAX / 3 + 1)
#define GMP_NUMB_CEIL_2MAX_DIV3  ((GMP_NUMB_MAX>>1) / 3 + 1 + GMP_NUMB_HIGHBIT)


/* Set r to -a mod d.  a>=d is allowed.  Can give r>d.  All should be limbs.

   It's not clear whether this is the best way to do this calculation.
   Anything congruent to -a would be fine for the one limb congruence
   tests.  */

#define NEG_MOD(r, a, d)						\
  do {									\
    ASSERT ((d) != 0);							\
    ASSERT_LIMB (a);							\
    ASSERT_LIMB (d);							\
                                    \
    if ((a) <= (d))							\
      {									\
        /* small a is reasonably likely */				\
        (r) = (d) - (a);						\
      }									\
    else								\
      {									\
        unsigned   __twos;						\
        mp_limb_t  __dnorm;						\
        count_leading_zeros (__twos, d);				\
        __twos -= GMP_NAIL_BITS;					\
        __dnorm = (d) << __twos;					\
        (r) = ((a) <= __dnorm ? __dnorm : 2*__dnorm) - (a);		\
      }									\
                                    \
    ASSERT_LIMB (r);							\
  } while (0)

/* A bit mask of all the least significant zero bits of n, or -1 if n==0. */
#define LOW_ZEROS_MASK(n)  (((n) & -(n)) - 1)


/* ULONG_PARITY sets "p" to 1 if there's an odd number of 1 bits in "n", or
   to 0 if there's an even number.  "n" should be an unsigned long and "p"
   an int.  */

#if ! defined (ULONG_PARITY)
#define ULONG_PARITY(p, n)						\
  do {									\
    unsigned long  __n = (n);						\
    int  __p = 0;							\
    do									\
      {									\
        __p ^= 0x96696996L >> (__n & 0x1F);				\
        __n >>= 5;							\
      }									\
    while (__n != 0);							\
                                    \
    (p) = __p & 1;							\
  } while (0)
#endif

#if ! defined (BSWAP_LIMB)
#if BITS_PER_MP_LIMB == 8
#define BSWAP_LIMB(dst, src)            \
  do { (dst) = (src); } while (0)
#endif
#if BITS_PER_MP_LIMB == 16
#define BSWAP_LIMB(dst, src)                    \
  do {                                          \
    (dst) = ((src) << 8) + ((src) >> 8);        \
  } while (0)
#endif
#if BITS_PER_MP_LIMB == 32
#define BSWAP_LIMB(dst, src)    \
  do {                          \
    (dst) =                     \
      ((src) << 24)             \
      + (((src) & 0xFF00) << 8) \
      + (((src) >> 8) & 0xFF00) \
      + ((src) >> 24);          \
  } while (0)
#endif
#if BITS_PER_MP_LIMB == 64
#define BSWAP_LIMB(dst, src)            \
  do {                                  \
    (dst) =                             \
      ((src) << 56)                     \
      + (((src) & 0xFF00) << 40)        \
      + (((src) & 0xFF0000) << 24)      \
      + (((src) & 0xFF000000) << 8)     \
      + (((src) >> 8) & 0xFF000000)     \
      + (((src) >> 24) & 0xFF0000)      \
      + (((src) >> 40) & 0xFF00)        \
      + ((src) >> 56);                  \
  } while (0)
#endif
#endif

#if ! defined (BSWAP_LIMB)
#define BSWAP_LIMB(dst, src)                            \
  do {                                                  \
    mp_limb_t  __bswapl_src = (src);                    \
    mp_limb_t  __dst = 0;                               \
    int        __i;                                     \
    for (__i = 0; __i < BYTES_PER_MP_LIMB; __i++)       \
      {                                                 \
        __dst = (__dst << 8) | (__bswapl_src & 0xFF);   \
        __bswapl_src >>= 8;                             \
      }                                                 \
    (dst) = __dst;                                      \
  } while (0)
#endif

#if ! defined (BSWAP_LIMB_FETCH)
#define BSWAP_LIMB_FETCH(limb, src)  BSWAP_LIMB (limb, *(src))
#endif

#if ! defined (BSWAP_LIMB_STORE)
#define BSWAP_LIMB_STORE(dst, limb)  BSWAP_LIMB (*(dst), limb)
#endif


/* Byte swap limbs from {src,size} and store at {dst,size}. */
#define MPN_BSWAP(dst, src, size)                       \
  do {                                                  \
    mp_ptr     __dst = (dst);                           \
    mp_srcptr  __src = (src);                           \
    mp_size_t  __size = (size);                         \
    mp_size_t  __i;                                     \
    ASSERT ((size) >= 0);                               \
    ASSERT (MPN_SAME_OR_SEPARATE_P (dst, src, size));   \
    for (__i = 0; __i < __size; __i++)                  \
      {                                                 \
        BSWAP_LIMB_FETCH (*__dst, __src);               \
        __dst++;                                        \
        __src++;                                        \
      }                                                 \
  } while (0)

/* Byte swap limbs from {dst,size} and store in reverse order at {src,size}. */
#define MPN_BSWAP_REVERSE(dst, src, size)               \
  do {                                                  \
    mp_ptr     __dst = (dst);                           \
    mp_size_t  __size = (size);                         \
    mp_srcptr  __src = (src) + __size - 1;              \
    mp_size_t  __i;                                     \
    ASSERT ((size) >= 0);                               \
    ASSERT (! MPN_OVERLAP_P (dst, size, src, size));    \
    for (__i = 0; __i < __size; __i++)                  \
      {                                                 \
        BSWAP_LIMB_FETCH (*__dst, __src);               \
        __dst++;                                        \
        __src--;                                        \
      }                                                 \
  } while (0)

/* Cool population count of an mp_limb_t.
   You have to figure out how this works, We won't tell you!

   The constants could also be expressed as:
     0x55... = [2^N / 3]     = [(2^N-1)/3]
     0x33... = [2^N / 5]     = [(2^N-1)/5]
     0x0f... = [2^N / 17]    = [(2^N-1)/17]
     (N is GMP_LIMB_BITS, [] denotes truncation.) */

#if ! defined (popc_limb) && GMP_LIMB_BITS == 8
#define popc_limb(result, input)					\
  do {									\
    mp_limb_t  __x = (input);						\
    __x -= (__x >> 1) & MP_LIMB_T_MAX/3;				\
    __x = ((__x >> 2) & MP_LIMB_T_MAX/5) + (__x & MP_LIMB_T_MAX/5);	\
    __x = ((__x >> 4) + __x) & MP_LIMB_T_MAX/17;			\
    (result) = __x & 0xff;						\
  } while (0)
#endif

#if ! defined (popc_limb) && GMP_LIMB_BITS == 16
#define popc_limb(result, input)					\
  do {									\
    mp_limb_t  __x = (input);						\
    __x -= (__x >> 1) & MP_LIMB_T_MAX/3;				\
    __x = ((__x >> 2) & MP_LIMB_T_MAX/5) + (__x & MP_LIMB_T_MAX/5);	\
    __x = ((__x >> 4) + __x) & MP_LIMB_T_MAX/17;			\
    if (GMP_LIMB_BITS > 8)						\
      __x = ((__x >> 8) + __x);						\
    (result) = __x & 0xff;						\
  } while (0)
#endif

#if ! defined (popc_limb) && GMP_LIMB_BITS == 32
#define popc_limb(result, input)					\
  do {									\
    mp_limb_t  __x = (input);						\
    __x -= (__x >> 1) & MP_LIMB_T_MAX/3;				\
    __x = ((__x >> 2) & MP_LIMB_T_MAX/5) + (__x & MP_LIMB_T_MAX/5);	\
    __x = ((__x >> 4) + __x) & MP_LIMB_T_MAX/17;			\
    if (GMP_LIMB_BITS > 8)						\
      __x = ((__x >> 8) + __x);						\
    if (GMP_LIMB_BITS > 16)						\
      __x = ((__x >> 16) + __x);					\
    (result) = __x & 0xff;						\
  } while (0)
#endif

#if ! defined (popc_limb) && GMP_LIMB_BITS == 64
#define popc_limb(result, input)					\
  do {									\
    mp_limb_t  __x = (input);						\
    __x -= (__x >> 1) & MP_LIMB_T_MAX/3;				\
    __x = ((__x >> 2) & MP_LIMB_T_MAX/5) + (__x & MP_LIMB_T_MAX/5);	\
    __x = ((__x >> 4) + __x) & MP_LIMB_T_MAX/17;			\
    if (GMP_LIMB_BITS > 8)						\
      __x = ((__x >> 8) + __x);						\
    if (GMP_LIMB_BITS > 16)						\
      __x = ((__x >> 16) + __x);					\
    if (GMP_LIMB_BITS > 32)						\
      __x = ((__x >> 32) + __x);					\
    (result) = __x & 0xff;						\
  } while (0)
#endif


/* Define stuff for longlong.h.  */
#if HAVE_ATTRIBUTE_MODE && defined (__GNUC__)
typedef unsigned int UQItype	__attribute__ ((mode (QI)));
typedef		 int SItype	__attribute__ ((mode (SI)));
typedef unsigned int USItype	__attribute__ ((mode (SI)));
typedef		 int DItype	__attribute__ ((mode (DI)));
typedef unsigned int UDItype	__attribute__ ((mode (DI)));
#else
typedef unsigned char UQItype;
typedef		 long SItype;
typedef unsigned long USItype;
#if HAVE_LONG_LONG
typedef	long long int DItype;
typedef unsigned long long int UDItype;
#else /* Assume `long' gives us a wide enough type.  Needed for hppa2.0w.  */
typedef long int DItype;
typedef unsigned long int UDItype;
#endif
#endif

typedef mp_limb_t UWtype;
typedef unsigned int UHWtype;
#define W_TYPE_SIZE BITS_PER_MP_LIMB

/* Define ieee_double_extract

   Bit field packing is "implementation defined" according to C99, which
   leaves us at the compiler's mercy here.  For some systems packing is
   defined in the ABI (eg. x86).  In any case so far it seems universal that
   little endian systems pack from low to high, and big endian from high to
   low within the given type.

   Within the fields we rely on the integer endianness being the same as the
   float endianness, this is true everywhere we know of and it'd be a fairly
   strange system that did anything else.  */

#if HAVE_DOUBLE_IEEE_LITTLE_SWAPPED
#define _GMP_IEEE_FLOATS 1
union ieee_double_extract
{
  struct
    {
      gmp_uint_least32_t manh:20;
      gmp_uint_least32_t exp:11;
      gmp_uint_least32_t sig:1;
      gmp_uint_least32_t manl:32;
    } s;
  double d;
};
#endif

#if HAVE_DOUBLE_IEEE_LITTLE_ENDIAN
#define _GMP_IEEE_FLOATS 1
union ieee_double_extract
{
  struct
    {
      gmp_uint_least32_t manl:32;
      gmp_uint_least32_t manh:20;
      gmp_uint_least32_t exp:11;
      gmp_uint_least32_t sig:1;
    } s;
  double d;
};
#endif

#if HAVE_DOUBLE_IEEE_BIG_ENDIAN
#define _GMP_IEEE_FLOATS 1
union ieee_double_extract
{
  struct
    {
      gmp_uint_least32_t sig:1;
      gmp_uint_least32_t exp:11;
      gmp_uint_least32_t manh:20;
      gmp_uint_least32_t manl:32;
    } s;
  double d;
};
#endif


/* Use (4.0 * ...) instead of (2.0 * ...) to work around buggy compilers
   that don't convert ulong->double correctly (eg. SunOS 4 native cc).  */
#define MP_BASE_AS_DOUBLE (4.0 * ((mp_limb_t) 1 << (GMP_NUMB_BITS - 2)))
/* Maximum number of limbs it will take to store any `double'.
   We assume doubles have 53 mantissam bits.  */
#define LIMBS_PER_DOUBLE ((53 + GMP_NUMB_BITS - 1) / GMP_NUMB_BITS + 1)

__GMP_DECLSPEC int __gmp_extract_double _PROTO ((mp_ptr, double));

#define mpn_get_d __gmpn_get_d
__GMP_DECLSPEC double mpn_get_d __GMP_PROTO ((mp_srcptr, mp_size_t, mp_size_t, long)) __GMP_ATTRIBUTE_PURE;


/* DOUBLE_NAN_INF_ACTION executes code a_nan if x is a NaN, or executes
   a_inf if x is an infinity.  Both are considered unlikely values, for
   branch prediction.  */

#if _GMP_IEEE_FLOATS
#define DOUBLE_NAN_INF_ACTION(x, a_nan, a_inf)  \
  do {                                          \
    union ieee_double_extract  u;               \
    u.d = (x);                                  \
    if (UNLIKELY (u.s.exp == 0x7FF))            \
      {                                         \
        if (u.s.manl == 0 && u.s.manh == 0)     \
          { a_inf; }                            \
        else                                    \
          { a_nan; }                            \
      }                                         \
  } while (0)
#endif

#ifndef DOUBLE_NAN_INF_ACTION
/* Unknown format, try something generic.
   NaN should be "unordered", so x!=x.
   Inf should be bigger than DBL_MAX.  */
#define DOUBLE_NAN_INF_ACTION(x, a_nan, a_inf)                  \
  do {                                                          \
    {                                                           \
      if (UNLIKELY ((x) != (x)))                                \
        { a_nan; }                                              \
      else if (UNLIKELY ((x) > DBL_MAX || (x) < -DBL_MAX))      \
        { a_inf; }                                              \
    }                                                           \
  } while (0)
#endif

__GMP_DECLSPEC extern int __gmp_junk;
__GMP_DECLSPEC extern const int __gmp_0;
__GMP_DECLSPEC void __gmp_exception _PROTO ((int)) ATTRIBUTE_NORETURN;
__GMP_DECLSPEC void __gmp_divide_by_zero _PROTO ((void)) ATTRIBUTE_NORETURN;
__GMP_DECLSPEC void __gmp_sqrt_of_negative _PROTO ((void)) ATTRIBUTE_NORETURN;
__GMP_DECLSPEC void __gmp_invalid_operation _PROTO ((void)) ATTRIBUTE_NORETURN;
#define GMP_ERROR(code)   __gmp_exception (code)
#define DIVIDE_BY_ZERO    __gmp_divide_by_zero ()
#define SQRT_OF_NEGATIVE  __gmp_sqrt_of_negative ()


/* Stuff used by mpn/generic/perfsqr.c and mpz/prime_p.c */
#if GMP_NUMB_BITS == 2
#define PP 0x3					/* 3 */
#define PP_FIRST_OMITTED 5
#endif
#if GMP_NUMB_BITS == 4
#define PP 0xF					/* 3 x 5 */
#define PP_FIRST_OMITTED 7
#endif
#if GMP_NUMB_BITS == 8
#define PP 0x69					/* 3 x 5 x 7 */
#define PP_FIRST_OMITTED 11
#endif
#if GMP_NUMB_BITS == 16
#define PP 0x3AA7				/* 3 x 5 x 7 x 11 x 13 */
#define PP_FIRST_OMITTED 17
#endif
#if GMP_NUMB_BITS == 32
#define PP 0xC0CFD797L				/* 3 x 5 x 7 x 11 x ... x 29 */
#define PP_INVERTED 0x53E5645CL
#define PP_FIRST_OMITTED 31
#endif
#if GMP_NUMB_BITS == 64
#define PP CNST_LIMB(0xE221F97C30E94E1D)	/* 3 x 5 x 7 x 11 x ... x 53 */
#define PP_INVERTED CNST_LIMB(0x21CFE6CFC938B36B)
#define PP_FIRST_OMITTED 59
#endif
#ifndef PP_FIRST_OMITTED
#define PP_FIRST_OMITTED 3
#endif

/* BIT1 means a result value in bit 1 (second least significant bit), with a
   zero bit representing +1 and a one bit representing -1.  Bits other than
   bit 1 are garbage.  These are meant to be kept in "int"s, and casts are
   used to ensure the expressions are "int"s even if a and/or b might be
   other types.

   JACOBI_TWOS_U_BIT1 and JACOBI_RECIP_UU_BIT1 are used in mpn_jacobi_base
   and their speed is important.  Expressions are used rather than
   conditionals to accumulate sign changes, which effectively means XORs
   instead of conditional JUMPs. */

/* (a/0), with a signed; is 1 if a=+/-1, 0 otherwise */
#define JACOBI_S0(a)   (((a) == 1) | ((a) == -1))

/* (a/0), with a unsigned; is 1 if a=+/-1, 0 otherwise */
#define JACOBI_U0(a)   ((a) == 1)

/* (a/0), with a given by low and size;
   is 1 if a=+/-1, 0 otherwise */
#define JACOBI_LS0(alow,asize) \
  (((asize) == 1 || (asize) == -1) && (alow) == 1)

/* (a/0), with a an mpz_t;
   fetch of low limb always valid, even if size is zero */
#define JACOBI_Z0(a)   JACOBI_LS0 (PTR(a)[0], SIZ(a))

/* (0/b), with b unsigned; is 1 if b=1, 0 otherwise */
#define JACOBI_0U(b)   ((b) == 1)

/* (0/b), with b unsigned; is 1 if b=+/-1, 0 otherwise */
#define JACOBI_0S(b)   ((b) == 1 || (b) == -1)

/* (0/b), with b given by low and size; is 1 if b=+/-1, 0 otherwise */
#define JACOBI_0LS(blow,bsize) \
  (((bsize) == 1 || (bsize) == -1) && (blow) == 1)

/* Convert a bit1 to +1 or -1. */
#define JACOBI_BIT1_TO_PN(result_bit1) \
  (1 - ((int) (result_bit1) & 2))

/* (2/b), with b unsigned and odd;
   is (-1)^((b^2-1)/8) which is 1 if b==1,7mod8 or -1 if b==3,5mod8 and
   hence obtained from (b>>1)^b */
#define JACOBI_TWO_U_BIT1(b) \
  ((int) (((b) >> 1) ^ (b)))

/* (2/b)^twos, with b unsigned and odd */
#define JACOBI_TWOS_U_BIT1(twos, b) \
  ((int) ((twos) << 1) & JACOBI_TWO_U_BIT1 (b))

/* (2/b)^twos, with b unsigned and odd */
#define JACOBI_TWOS_U(twos, b) \
  (JACOBI_BIT1_TO_PN (JACOBI_TWOS_U_BIT1 (twos, b)))

/* (-1/b), with b odd (signed or unsigned);
   is (-1)^((b-1)/2) */
#define JACOBI_N1B_BIT1(b) \
  ((int) (b))

/* (a/b) effect due to sign of a: signed/unsigned, b odd;
   is (-1/b) if a<0, or +1 if a>=0 */
#define JACOBI_ASGN_SU_BIT1(a, b) \
  ((((a) < 0) << 1) & JACOBI_N1B_BIT1(b))

/* (a/b) effect due to sign of b: signed/signed;
   is -1 if a and b both negative, +1 otherwise */
#define JACOBI_BSGN_SS_BIT1(a, b) \
  ((((a)<0) & ((b)<0)) << 1)

/* (a/b) effect due to sign of b: signed/mpz;
   is -1 if a and b both negative, +1 otherwise */
#define JACOBI_BSGN_SZ_BIT1(a, b) \
  JACOBI_BSGN_SS_BIT1 (a, SIZ(b))

/* (a/b) effect due to sign of b: mpz/signed;
   is -1 if a and b both negative, +1 otherwise */
#define JACOBI_BSGN_ZS_BIT1(a, b) \
  JACOBI_BSGN_SZ_BIT1 (b, a)

/* (a/b) reciprocity to switch to (b/a), a,b both unsigned and odd;
   is (-1)^((a-1)*(b-1)/4), which means +1 if either a,b==1mod4, or -1 if
   both a,b==3mod4, achieved in bit 1 by a&b.  No ASSERT()s about a,b odd
   because this is used in a couple of places with only bit 1 of a or b
   valid. */
#define JACOBI_RECIP_UU_BIT1(a, b) \
  ((int) ((a) & (b)))

/* Strip low zero limbs from {b_ptr,b_size} by incrementing b_ptr and
   decrementing b_size.  b_low should be b_ptr[0] on entry, and will be
   updated for the new b_ptr.  result_bit1 is updated according to the
   factors of 2 stripped, as per (a/2).  */
#define JACOBI_STRIP_LOW_ZEROS(result_bit1, a, b_ptr, b_size, b_low)    \
  do {                                                                  \
    ASSERT ((b_size) >= 1);                                             \
    ASSERT ((b_low) == (b_ptr)[0]);                                     \
                                                                        \
    while (UNLIKELY ((b_low) == 0))                                     \
      {                                                                 \
        (b_size)--;                                                     \
        ASSERT ((b_size) >= 1);                                         \
        (b_ptr)++;                                                      \
        (b_low) = *(b_ptr);                                             \
                                                                        \
        ASSERT (((a) & 1) != 0);                                        \
        if ((GMP_NUMB_BITS % 2) == 1)                                   \
          (result_bit1) ^= JACOBI_TWO_U_BIT1(a);                        \
      }                                                                 \
  } while (0)

/* Set a_rem to {a_ptr,a_size} reduced modulo b, either using mod_1 or
   modexact_1_odd, but in either case leaving a_rem<b.  b must be odd and
   unsigned.  modexact_1_odd effectively calculates -a mod b, and
   result_bit1 is adjusted for the factor of -1.

   The way mpn_modexact_1_odd sometimes bases its remainder on a_size and
   sometimes on a_size-1 means if GMP_NUMB_BITS is odd we can't know what
   factor to introduce into result_bit1, so for that case use mpn_mod_1
   unconditionally.

   FIXME: mpn_modexact_1_odd is more efficient, so some way to get it used
   for odd GMP_NUMB_BITS would be good.  Perhaps it could mung its result,
   or not skip a divide step, or something. */

#define JACOBI_MOD_OR_MODEXACT_1_ODD(result_bit1, a_rem, a_ptr, a_size, b) \
  do {                                                                     \
    mp_srcptr  __a_ptr  = (a_ptr);                                         \
    mp_size_t  __a_size = (a_size);                                        \
    mp_limb_t  __b      = (b);                                             \
                                                                           \
    ASSERT (__a_size >= 1);                                                \
    ASSERT (__b & 1);                                                      \
                                                                           \
    if ((GMP_NUMB_BITS % 2) != 0                                           \
        || BELOW_THRESHOLD (__a_size, MODEXACT_1_ODD_THRESHOLD))           \
      {                                                                    \
        (a_rem) = mpn_mod_1 (__a_ptr, __a_size, __b);                      \
      }                                                                    \
    else                                                                   \
      {                                                                    \
        (result_bit1) ^= JACOBI_N1B_BIT1 (__b);                            \
        (a_rem) = mpn_modexact_1_odd (__a_ptr, __a_size, __b);             \
      }                                                                    \
  } while (0)

/* State for the Jacobi computation using Lehmer. */
#define jacobi_table __gmp_jacobi_table
__GMP_DECLSPEC extern const unsigned char jacobi_table[208];

/* Bit layout for the initial state. b must be odd.

      3  2  1 0
   +--+--+--+--+
   |a1|a0|b1| s|
   +--+--+--+--+

 */
static inline unsigned
mpn_jacobi_init (unsigned a, unsigned b, unsigned s)
{
  ASSERT (b & 1);
  ASSERT (s <= 1);
  return ((a & 3) << 2) + (b & 2) + s;
}

static inline int
mpn_jacobi_finish (unsigned bits)
{
  /* (a, b) = (1,0) or (0,1) */
  ASSERT ( (bits & 14) == 0);

  return 1-2*(bits & 1);
}

static inline unsigned
mpn_jacobi_update (unsigned bits, unsigned denominator, unsigned q)
{
  /* FIXME: Could halve table size by not including the e bit in the
   * index, and instead xor when updating. Then the lookup would be
   * like
   *
   *   bits ^= table[((bits & 30) << 2) + (denominator << 2) + q];
   */

  ASSERT (bits < 26);
  ASSERT (denominator < 2);
  ASSERT (q < 4);

  /* For almost all calls, denominator is constant and quite often q
     is constant too. So use addition rather than or, so the compiler
     can put the constant part can into the offset of an indexed
     addressing instruction.

     With constant denominator, the below table lookup is compiled to

       C Constant q = 1, constant denominator = 1
       movzbl table+5(%eax,8), %eax

     or

       C q in %edx, constant denominator = 1
       movzbl table+4(%edx,%eax,8), %eax

     One could maintain the state preshifted 3 bits, to save a shift
     here, but at least on x86, that's no real saving.
  */
  return bits = jacobi_table[(bits << 3) + (denominator << 2) + q];
}


/* Matrix multiplication */
#define   mpn_matrix22_mul __MPN(matrix22_mul)
__GMP_DECLSPEC void      mpn_matrix22_mul __GMP_PROTO ((mp_ptr, mp_ptr, mp_ptr, mp_ptr, mp_size_t, mp_srcptr, mp_srcptr, mp_srcptr, mp_srcptr, mp_size_t, mp_ptr));
#define   mpn_matrix22_mul_strassen __MPN(matrix22_mul_strassen)
__GMP_DECLSPEC void      mpn_matrix22_mul_strassen __GMP_PROTO ((mp_ptr, mp_ptr, mp_ptr, mp_ptr, mp_size_t, mp_srcptr, mp_srcptr, mp_srcptr, mp_srcptr, mp_size_t, mp_ptr));
#define   mpn_matrix22_mul_itch __MPN(matrix22_mul_itch)
__GMP_DECLSPEC mp_size_t mpn_matrix22_mul_itch __GMP_PROTO ((mp_size_t, mp_size_t));

#ifndef MATRIX22_STRASSEN_THRESHOLD
#define MATRIX22_STRASSEN_THRESHOLD 30
#endif

/* HGCD definitions */

/* Extract one numb, shifting count bits left
    ________  ________
   |___xh___||___xl___|
      |____r____|
   >count <

   The count includes any nail bits, so it should work fine if count
   is computed using count_leading_zeros. If GMP_NAIL_BITS > 0, all of
   xh, xl and r include nail bits. Must have 0 < count < GMP_LIMB_BITS.

   FIXME: Omit masking with GMP_NUMB_MASK, and let callers do that for
   those calls where the count high bits of xh may be non-zero.
*/

#define MPN_EXTRACT_NUMB(count, xh, xl)				\
  ((((xh) << ((count) - GMP_NAIL_BITS)) & GMP_NUMB_MASK) |	\
   ((xl) >> (GMP_LIMB_BITS - (count))))


/* The matrix non-negative M = (u, u'; v,v') keeps track of the
   reduction (a;b) = M (alpha; beta) where alpha, beta are smaller
   than a, b. The determinant must always be one, so that M has an
   inverse (v', -u'; -v, u). Elements always fit in GMP_NUMB_BITS - 1
   bits. */
struct hgcd_matrix1
{
  mp_limb_t u[2][2];
};

#define mpn_hgcd2 __MPN (hgcd2)
__GMP_DECLSPEC int mpn_hgcd2 (mp_limb_t, mp_limb_t, mp_limb_t, mp_limb_t,	struct hgcd_matrix1 *);

#define mpn_hgcd_mul_matrix1_vector __MPN (hgcd_mul_matrix1_vector)
__GMP_DECLSPEC mp_size_t mpn_hgcd_mul_matrix1_vector (const struct hgcd_matrix1 *, mp_ptr, mp_srcptr, mp_ptr, mp_size_t);

#define mpn_matrix22_mul1_inverse_vector __MPN (matrix22_mul1_inverse_vector)
__GMP_DECLSPEC mp_size_t mpn_matrix22_mul1_inverse_vector (const struct hgcd_matrix1 *, mp_ptr, mp_srcptr, mp_ptr, mp_size_t);

#define mpn_hgcd2_jacobi __MPN (hgcd2_jacobi)
__GMP_DECLSPEC int mpn_hgcd2_jacobi (mp_limb_t, mp_limb_t, mp_limb_t, mp_limb_t, struct hgcd_matrix1 *, unsigned *);

struct hgcd_matrix
{
  mp_size_t alloc;		/* for sanity checking only */
  mp_size_t n;
  mp_ptr p[2][2];
};

#define MPN_HGCD_MATRIX_INIT_ITCH(n) (4 * ((n+1)/2 + 1))

#define mpn_hgcd_matrix_init __MPN (hgcd_matrix_init)
__GMP_DECLSPEC void mpn_hgcd_matrix_init (struct hgcd_matrix *, mp_size_t, mp_ptr);

#define mpn_hgcd_matrix_update_q __MPN (hgcd_matrix_update_q)
__GMP_DECLSPEC void mpn_hgcd_matrix_update_q (struct hgcd_matrix *, mp_srcptr, mp_size_t, unsigned, mp_ptr);

#define mpn_hgcd_matrix_mul_1 __MPN (hgcd_matrix_mul_1)
__GMP_DECLSPEC void mpn_hgcd_matrix_mul_1 (struct hgcd_matrix *, const struct hgcd_matrix1 *, mp_ptr);

#define mpn_hgcd_matrix_mul __MPN (hgcd_matrix_mul)
__GMP_DECLSPEC void mpn_hgcd_matrix_mul (struct hgcd_matrix *, const struct hgcd_matrix *, mp_ptr);

#define mpn_hgcd_matrix_adjust __MPN (hgcd_matrix_adjust)
__GMP_DECLSPEC mp_size_t mpn_hgcd_matrix_adjust (const struct hgcd_matrix *, mp_size_t, mp_ptr, mp_ptr, mp_size_t, mp_ptr);

#define mpn_hgcd_step __MPN(hgcd_step)
__GMP_DECLSPEC mp_size_t mpn_hgcd_step (mp_size_t, mp_ptr, mp_ptr, mp_size_t, struct hgcd_matrix *, mp_ptr);

#define mpn_hgcd_reduce __MPN(hgcd_reduce)
__GMP_DECLSPEC mp_size_t mpn_hgcd_reduce (struct hgcd_matrix *, mp_ptr, mp_ptr, mp_size_t, mp_size_t, mp_ptr);

#define mpn_hgcd_reduce_itch __MPN(hgcd_reduce_itch)
__GMP_DECLSPEC mp_size_t mpn_hgcd_reduce_itch (mp_size_t, mp_size_t);

#define mpn_hgcd_itch __MPN (hgcd_itch)
__GMP_DECLSPEC mp_size_t mpn_hgcd_itch (mp_size_t);

#define mpn_hgcd __MPN (hgcd)
__GMP_DECLSPEC mp_size_t mpn_hgcd (mp_ptr, mp_ptr, mp_size_t, struct hgcd_matrix *, mp_ptr);

#define mpn_hgcd_appr_itch __MPN (hgcd_appr_itch)
__GMP_DECLSPEC mp_size_t mpn_hgcd_appr_itch (mp_size_t);

#define mpn_hgcd_appr __MPN (hgcd_appr)
__GMP_DECLSPEC int mpn_hgcd_appr (mp_ptr, mp_ptr, mp_size_t, struct hgcd_matrix *, mp_ptr);

#define mpn_hgcd_jacobi __MPN (hgcd_jacobi)
__GMP_DECLSPEC mp_size_t mpn_hgcd_jacobi (mp_ptr, mp_ptr, mp_size_t, struct hgcd_matrix *, unsigned *, mp_ptr);

typedef void gcd_subdiv_step_hook(void *, mp_srcptr, mp_size_t, mp_srcptr, mp_size_t, int);

/* Needs storage for the quotient */
#define MPN_GCD_SUBDIV_STEP_ITCH(n) (n)

#define mpn_gcd_subdiv_step __MPN(gcd_subdiv_step)
__GMP_DECLSPEC mp_size_t mpn_gcd_subdiv_step (mp_ptr, mp_ptr, mp_size_t, mp_size_t, gcd_subdiv_step_hook *, void *, mp_ptr);

struct gcdext_ctx
{
  /* Result parameters. */
  mp_ptr gp;
  mp_size_t gn;
  mp_ptr up;
  mp_size_t *usize;

  /* Cofactors updated in each step. */
  mp_size_t un;
  mp_ptr u0, u1, tp;
};

#define mpn_gcdext_hook __MPN (gcdext_hook)
gcd_subdiv_step_hook mpn_gcdext_hook;

#define MPN_GCDEXT_LEHMER_N_ITCH(n) (4*(n) + 3)

#define mpn_gcdext_lehmer_n __MPN(gcdext_lehmer_n)
__GMP_DECLSPEC mp_size_t mpn_gcdext_lehmer_n (mp_ptr, mp_ptr, mp_size_t *, mp_ptr, mp_ptr, mp_size_t, mp_ptr);

/* 4*(an + 1) + 4*(bn + 1) + an */
#define MPN_GCDEXT_LEHMER_ITCH(an, bn) (5*(an) + 4*(bn) + 8)

#ifndef HGCD_THRESHOLD
#define HGCD_THRESHOLD 400
#endif

#ifndef HGCD_APPR_THRESHOLD
#define HGCD_APPR_THRESHOLD 400
#endif

#ifndef HGCD_REDUCE_THRESHOLD
#define HGCD_REDUCE_THRESHOLD 1000
#endif

#ifndef GCD_DC_THRESHOLD
#define GCD_DC_THRESHOLD 1000
#endif

#ifndef GCDEXT_DC_THRESHOLD
#define GCDEXT_DC_THRESHOLD 600
#endif

#define mpn_mulmod_bnm1 __MPN(mulmod_bnm1)
__GMP_DECLSPEC void mpn_mulmod_bnm1 (mp_ptr, mp_size_t, mp_srcptr, mp_size_t, mp_srcptr, mp_size_t, mp_ptr);

#define mpn_mulmod_bnm1_next_size(x) \
   ((x) <= 2*FFT_MULMOD_2EXPP1_CUTOFF ? (x) : 2*mpir_fft_adjust_limbs(((x) + 1)/2))

static inline mp_size_t
mpn_mulmod_bnm1_itch (mp_size_t rn, mp_size_t an, mp_size_t bn) {
  return 5*rn + 220;
}

/* Definitions for mpn_set_str and mpn_get_str */
struct powers
{
  mp_ptr p;			/* actual power value */
  mp_size_t n;			/* # of limbs at p */
  mp_size_t shift;		/* weight of lowest limb, in limb base B */
  size_t digits_in_base;	/* number of corresponding digits */
  int base;
};
typedef struct powers powers_t;
#define mpn_dc_set_str_powtab_alloc(n) ((n) + GMP_LIMB_BITS)
#define mpn_dc_set_str_itch(n) ((n) + GMP_LIMB_BITS)
#define mpn_dc_get_str_powtab_alloc(n) ((n) + 2 * GMP_LIMB_BITS)
#define mpn_dc_get_str_itch(n) ((n) + GMP_LIMB_BITS)

#define   mpn_dc_set_str __MPN(dc_set_str)
__GMP_DECLSPEC mp_size_t mpn_dc_set_str __GMP_PROTO ((mp_ptr, const unsigned char *, size_t, const powers_t *, mp_ptr));
#define   mpn_bc_set_str __MPN(bc_set_str)
__GMP_DECLSPEC mp_size_t mpn_bc_set_str __GMP_PROTO ((mp_ptr, const unsigned char *, size_t, int));
#define   mpn_set_str_compute_powtab __MPN(set_str_compute_powtab)
__GMP_DECLSPEC void      mpn_set_str_compute_powtab __GMP_PROTO ((powers_t *, mp_ptr, mp_size_t, int));
#define mpn_pre_set_str __MPN(pre_set_str)
__GMP_DECLSPEC void mpn_pre_set_str __GMP_PROTO ((mp_ptr wp, unsigned char *str, size_t str_len, powers_t *powtab, mp_ptr tp));


void _tc4_add(mp_ptr rp, mp_size_t * rn, mp_srcptr r1, mp_size_t r1n, mp_srcptr r2, mp_size_t r2n);

void tc4_add(mp_ptr rp, mp_size_t * rn, mp_srcptr r1, mp_size_t r1n, mp_srcptr r2, mp_size_t r2n);

void _tc4_add_unsigned(mp_ptr rp, mp_size_t * rn, mp_srcptr r1, mp_size_t r1n, mp_srcptr r2, mp_size_t r2n);

void tc4_add_unsigned(mp_ptr rp, mp_size_t * rn, mp_srcptr r1, mp_size_t r1n, mp_srcptr r2, mp_size_t r2n);

void tc4_sub(mp_ptr rp, mp_size_t * rn, mp_srcptr r1, mp_size_t r1n, mp_srcptr r2, mp_size_t r2n);

void tc4_lshift(mp_ptr rp, mp_size_t * rn, mp_srcptr xp, mp_size_t xn, mp_size_t bits);

void tc4_rshift_inplace(mp_ptr rp, mp_size_t * rn, mp_size_t bits);

void tc4_addlsh1_unsigned(mp_ptr rp, mp_size_t * rn, mp_srcptr xp, mp_size_t xn);

void tc4_divexact_ui(mp_ptr rp, mp_size_t * rn, mp_ptr x, mp_size_t xn, mp_limb_t c);

void tc4_divexact_by3(mp_ptr rp, mp_size_t * rn, mp_ptr x, mp_size_t xn);

void tc4_divexact_by15(mp_ptr rp, mp_size_t * rn, mp_ptr x, mp_size_t xn);

void tc4_addmul_1(mp_ptr wp, mp_size_t * wn, mp_srcptr xp, mp_size_t xn, mp_limb_t y);

void tc4_submul_1(mp_ptr wp, mp_size_t * wn, mp_srcptr x, mp_size_t xn, mp_limb_t y);

void tc4_copy (mp_ptr yp, mp_size_t * yn, mp_size_t offset, mp_srcptr xp, mp_size_t xn);

void __divappr_helper(mp_ptr qp, mp_ptr np, mp_srcptr dp, mp_size_t qn);

/* __GMPF_BITS_TO_PREC applies a minimum 53 bits, rounds upwards to a whole
   limb and adds an extra limb.  __GMPF_PREC_TO_BITS drops that extra limb,
   hence giving back the user's size in bits rounded up.  Notice that
   converting prec->bits->prec gives an unchanged value.  */
#define __GMPF_BITS_TO_PREC(n)						\
  ((mp_size_t) ((__GMP_MAX (53, n) + 2 * GMP_NUMB_BITS - 1) / GMP_NUMB_BITS))
#define __GMPF_PREC_TO_BITS(n) \
  ((mp_bitcnt_t) (n) * GMP_NUMB_BITS - GMP_NUMB_BITS)

__GMP_DECLSPEC extern mp_size_t __gmp_default_fp_limb_precision;


/* Set n to the number of significant digits an mpf of the given _mp_prec
   field, in the given base.  This is a rounded up value, designed to ensure
   there's enough digits to reproduce all the guaranteed part of the value.

   There are prec many limbs, but the high might be only "1" so forget it
   and just count prec-1 limbs into chars.  +1 rounds that upwards, and a
   further +1 is because the limbs usually won't fall on digit boundaries.

   FIXME: If base is a power of 2 and the bits per digit divides
   BITS_PER_MP_LIMB then the +2 is unnecessary.  This happens always for
   base==2, and in base==16 with the current 32 or 64 bit limb sizes. */

#define MPF_SIGNIFICANT_DIGITS(n, base, prec)                           \
  do {                                                                  \
    ASSERT (base >= 2 && base < numberof (mp_bases));                   \
    (n) = 2 + (size_t) ((((size_t) (prec) - 1) * GMP_NUMB_BITS)         \
                        * mp_bases[(base)].chars_per_bit_exactly);      \
  } while (0)


/* Decimal point string, from the current C locale.  Needs <langinfo.h> for
   nl_langinfo and constants, preferrably with _GNU_SOURCE defined to get
   DECIMAL_POINT from glibc, and needs <locale.h> for localeconv, each under
   their respective #if HAVE_FOO_H.

   GLIBC recommends nl_langinfo because getting only one facet can be
   faster, apparently. */

/* DECIMAL_POINT seems to need _GNU_SOURCE defined to get it from glibc. */
#if HAVE_NL_LANGINFO && defined (DECIMAL_POINT)
#define GMP_DECIMAL_POINT  (nl_langinfo (DECIMAL_POINT))
#endif
/* RADIXCHAR is deprecated, still in unix98 or some such. */
#if HAVE_NL_LANGINFO && defined (RADIXCHAR) && ! defined (GMP_DECIMAL_POINT)
#define GMP_DECIMAL_POINT  (nl_langinfo (RADIXCHAR))
#endif
/* localeconv is slower since it returns all locale stuff */
#if HAVE_LOCALECONV && ! defined (GMP_DECIMAL_POINT)
#define GMP_DECIMAL_POINT  (localeconv()->decimal_point)
#endif
#if ! defined (GMP_DECIMAL_POINT)
#define GMP_DECIMAL_POINT  (".")
#endif


#define DOPRNT_CONV_FIXED        1
#define DOPRNT_CONV_SCIENTIFIC   2
#define DOPRNT_CONV_GENERAL      3

#define DOPRNT_JUSTIFY_NONE      0
#define DOPRNT_JUSTIFY_LEFT      1
#define DOPRNT_JUSTIFY_RIGHT     2
#define DOPRNT_JUSTIFY_INTERNAL  3

#define DOPRNT_SHOWBASE_YES      1
#define DOPRNT_SHOWBASE_NO       2
#define DOPRNT_SHOWBASE_NONZERO  3

struct doprnt_params_t {
  int         base;          /* negative for upper case */
  int         conv;          /* choices above */
  const char  *expfmt;       /* exponent format */
  int         exptimes4;     /* exponent multiply by 4 */
  char        fill;          /* character */
  int         justify;       /* choices above */
  int         prec;          /* prec field, or -1 for all digits */
  int         showbase;      /* choices above */
  int         showpoint;     /* if radix point always shown */
  int         showtrailing;  /* if trailing zeros wanted */
  char        sign;          /* '+', ' ', or '\0' */
  int         width;         /* width field */
};

#if _GMP_H_HAVE_VA_LIST

typedef int (*doprnt_format_t) _PROTO ((void *data, const char *fmt, va_list ap));
typedef int (*doprnt_memory_t) _PROTO ((void *data, const char *str, size_t len));
typedef int (*doprnt_reps_t) _PROTO ((void *data, int c, int reps));
typedef int (*doprnt_final_t) _PROTO ((void *data));

struct doprnt_funs_t {
  doprnt_format_t  format;
  doprnt_memory_t  memory;
  doprnt_reps_t    reps;
  doprnt_final_t   final;   /* NULL if not required */
};

extern const struct doprnt_funs_t  __gmp_fprintf_funs;
extern const struct doprnt_funs_t  __gmp_sprintf_funs;
extern const struct doprnt_funs_t  __gmp_snprintf_funs;
extern const struct doprnt_funs_t  __gmp_obstack_printf_funs;
extern const struct doprnt_funs_t  __gmp_ostream_funs;

/* "buf" is a __gmp_allocate_func block of "alloc" many bytes.  The first
   "size" of these have been written.  "alloc > size" is maintained, so
   there's room to store a '\0' at the end.  "result" is where the
   application wants the final block pointer.  */
struct gmp_asprintf_t {
  char    **result;
  char    *buf;
  size_t  size;
  size_t  alloc;
};

#define GMP_ASPRINTF_T_INIT(d, output)                          \
  do {                                                          \
    (d).result = (output);                                      \
    (d).alloc = 256;                                            \
    (d).buf = (char *) (*__gmp_allocate_func) ((d).alloc);      \
    (d).size = 0;                                               \
  } while (0)

/* If a realloc is necessary, use twice the size actually required, so as to
   avoid repeated small reallocs.  */
#define GMP_ASPRINTF_T_NEED(d, n)                                       \
  do {                                                                  \
    size_t  alloc, newsize, newalloc;                                   \
    ASSERT ((d)->alloc >= (d)->size + 1);                               \
                                                                        \
    alloc = (d)->alloc;                                                 \
    newsize = (d)->size + (n);                                          \
    if (alloc <= newsize)                                               \
      {                                                                 \
        newalloc = 2*newsize;                                           \
        (d)->alloc = newalloc;                                          \
        (d)->buf = __GMP_REALLOCATE_FUNC_TYPE ((d)->buf,                \
                                               alloc, newalloc, char);  \
      }                                                                 \
  } while (0)

__GMP_DECLSPEC int __gmp_asprintf_memory _PROTO ((struct gmp_asprintf_t *d, const char *str, size_t len));
__GMP_DECLSPEC int __gmp_asprintf_reps _PROTO ((struct gmp_asprintf_t *d, int c, int reps));
__GMP_DECLSPEC int __gmp_asprintf_final _PROTO ((struct gmp_asprintf_t *d));

/* buf is where to write the next output, and size is how much space is left
   there.  If the application passed size==0 then that's what we'll have
   here, and nothing at all should be written.  */
struct gmp_snprintf_t {
  char    *buf;
  size_t  size;
};

/* Add the bytes printed by the call to the total retval, or bail out on an
   error.  */
#define DOPRNT_ACCUMULATE(call) \
  do {                          \
    int  __ret;                 \
    __ret = call;               \
    if (__ret == -1)            \
      goto error;               \
    retval += __ret;            \
  } while (0)
#define DOPRNT_ACCUMULATE_FUN(fun, params)      \
  do {                                          \
    ASSERT ((fun) != NULL);                     \
    DOPRNT_ACCUMULATE ((*(fun)) params);        \
  } while (0)

#define DOPRNT_FORMAT(fmt, ap)                          \
  DOPRNT_ACCUMULATE_FUN (funs->format, (data, fmt, ap))
#define DOPRNT_MEMORY(ptr, len)                                 \
  DOPRNT_ACCUMULATE_FUN (funs->memory, (data, ptr, len))
#define DOPRNT_REPS(c, n)                               \
  DOPRNT_ACCUMULATE_FUN (funs->reps, (data, c, n))

#define DOPRNT_STRING(str)      DOPRNT_MEMORY (str, strlen (str))

#define DOPRNT_REPS_MAYBE(c, n) \
  do {                          \
    if ((n) != 0)               \
      DOPRNT_REPS (c, n);       \
  } while (0)
#define DOPRNT_MEMORY_MAYBE(ptr, len)   \
  do {                                  \
    if ((len) != 0)                     \
      DOPRNT_MEMORY (ptr, len);         \
  } while (0)

__GMP_DECLSPEC int __gmp_doprnt _PROTO ((const struct doprnt_funs_t *, void *, const char *, va_list));
__GMP_DECLSPEC int __gmp_doprnt_integer _PROTO ((const struct doprnt_funs_t *, void *, const struct doprnt_params_t *, const char *));

#define __gmp_doprnt_mpf __gmp_doprnt_mpf2
__GMP_DECLSPEC int __gmp_doprnt_mpf _PROTO ((const struct doprnt_funs_t *, void *, const struct doprnt_params_t *, const char *, mpf_srcptr));

__GMP_DECLSPEC int __gmp_replacement_vsnprintf _PROTO ((char *, size_t, const char *, va_list));
#endif /* _GMP_H_HAVE_VA_LIST */


typedef int (*gmp_doscan_scan_t)  _PROTO ((void *, const char *, ...));
typedef void *(*gmp_doscan_step_t) _PROTO ((void *, int));
typedef int (*gmp_doscan_get_t)   _PROTO ((void *));
typedef int (*gmp_doscan_unget_t) _PROTO ((int, void *));

struct gmp_doscan_funs_t {
  gmp_doscan_scan_t   scan;
  gmp_doscan_step_t   step;
  gmp_doscan_get_t    get;
  gmp_doscan_unget_t  unget;
};
extern const struct gmp_doscan_funs_t  __gmp_fscanf_funs;
extern const struct gmp_doscan_funs_t  __gmp_sscanf_funs;

#if _GMP_H_HAVE_VA_LIST
__GMP_DECLSPEC int __gmp_doscan _PROTO ((const struct gmp_doscan_funs_t *, void *,
                          const char *, va_list));
#endif


/* For testing and debugging.  */
#define MPZ_CHECK_FORMAT(z)					\
  do {                                                          \
    ASSERT_ALWAYS (SIZ(z) == 0 || PTR(z)[ABSIZ(z) - 1] != 0);	\
    ASSERT_ALWAYS (ALLOC(z) >= ABSIZ(z));			\
    ASSERT_ALWAYS_MPN (PTR(z), ABSIZ(z));                       \
  } while (0)

#define MPQ_CHECK_FORMAT(q)                             \
  do {                                                  \
    MPZ_CHECK_FORMAT (mpq_numref (q));                  \
    MPZ_CHECK_FORMAT (mpq_denref (q));                  \
    ASSERT_ALWAYS (SIZ(mpq_denref(q)) >= 1);            \
                                                        \
    if (SIZ(mpq_numref(q)) == 0)                        \
      {                                                 \
        /* should have zero as 0/1 */                   \
        ASSERT_ALWAYS (SIZ(mpq_denref(q)) == 1          \
                       && PTR(mpq_denref(q))[0] == 1);  \
      }                                                 \
    else                                                \
      {                                                 \
        /* should have no common factors */             \
        mpz_t  g;                                       \
        mpz_init (g);                                   \
        mpz_gcd (g, mpq_numref(q), mpq_denref(q));      \
        ASSERT_ALWAYS (mpz_cmp_ui (g, 1) == 0);         \
        mpz_clear (g);                                  \
      }                                                 \
  } while (0)

#define MPF_CHECK_FORMAT(f)                             \
  do {                                                  \
    ASSERT_ALWAYS (PREC(f) >= __GMPF_BITS_TO_PREC(53)); \
    ASSERT_ALWAYS (ABSIZ(f) <= PREC(f)+1);              \
    if (SIZ(f) == 0)                                    \
      ASSERT_ALWAYS (EXP(f) == 0);                      \
    if (SIZ(f) != 0)                                    \
      ASSERT_ALWAYS (PTR(f)[ABSIZ(f) - 1] != 0);        \
  } while (0)


#define MPZ_PROVOKE_REALLOC(z)					\
  do { ALLOC(z) = ABSIZ(z); } while (0)


/* Enhancement: The "mod" and "gcd_1" functions below could have
   __GMP_ATTRIBUTE_PURE, but currently (gcc 3.3) that's not supported on
   function pointers, only actual functions.  It probably doesn't make much
   difference to the gmp code, since hopefully we arrange calls so there's
   no great need for the compiler to move things around.  */

#if WANT_FAT_BINARY
/* NOTE: The function pointers in this struct are also in CPUVEC_FUNCS_LIST
   in mpn/x86/x86-defs.m4 and in mpn/x86_64/x86_64-defs.m4.  Be sure to 
    update them there when changing here.  */
struct cpuvec_t {
  DECL_add_err1_n      ((*add_err1_n));
  DECL_add_err2_n      ((*add_err2_n));
  DECL_add_n           ((*add_n));
  DECL_addmul_1        ((*addmul_1));
  DECL_copyd           ((*copyd));
  DECL_copyi           ((*copyi));
  DECL_divexact_1      ((*divexact_1));
  DECL_divexact_by3c   ((*divexact_by3c));
  DECL_divexact_byfobm1   ((*divexact_byfobm1));
  DECL_divrem_1        ((*divrem_1));
  DECL_divrem_2        ((*divrem_2));
  DECL_divrem_euclidean_qr_1        ((*divrem_euclidean_qr_1)); 
  DECL_divrem_euclidean_qr_2        ((*divrem_euclidean_qr_2)); 
  DECL_gcd_1           ((*gcd_1));
  DECL_lshift          ((*lshift));
  DECL_mod_1           ((*mod_1));
  DECL_mod_34lsub1     ((*mod_34lsub1));
  DECL_modexact_1c_odd ((*modexact_1c_odd));
  DECL_mul_1           ((*mul_1));
  DECL_mul_basecase    ((*mul_basecase));
  DECL_mulmid_basecase ((*mulmid_basecase));
  DECL_preinv_divrem_1 ((*preinv_divrem_1));
  DECL_preinv_mod_1    ((*preinv_mod_1));
  DECL_redc_1   ((*redc_1));
  DECL_rshift          ((*rshift));
  DECL_sqr_basecase    ((*sqr_basecase));
  DECL_sub_err1_n      ((*sub_err1_n));
  DECL_sub_err2_n      ((*sub_err2_n));
  DECL_sub_n           ((*sub_n));
  DECL_submul_1        ((*submul_1));
  DECL_sumdiff_n       ((*sumdiff_n));

  int                  initialized;
  mp_size_t            mul_karatsuba_threshold;
  mp_size_t            mul_toom3_threshold;
  mp_size_t            sqr_karatsuba_threshold;
  mp_size_t            sqr_toom3_threshold;
};
__GMP_DECLSPEC extern struct cpuvec_t __gmpn_cpuvec;
#endif /* x86 fat binary */

__GMP_DECLSPEC void __gmpn_cpuvec_init __GMP_PROTO ((void));

/* Get a threshold "field" from __gmpn_cpuvec, running __gmpn_cpuvec_init()
   if that hasn't yet been done (to establish the right values).  */
#define CPUVEC_THRESHOLD(field)                                               \
  ((LIKELY (__gmpn_cpuvec.initialized) ? 0 : (__gmpn_cpuvec_init (), 0)),     \
   __gmpn_cpuvec.field)



static inline int
mpn_zero_p (mp_srcptr ap, mp_size_t n)
{
  mp_size_t i;
  for (i = n - 1; i >= 0; i--)
    {
      if (ap[i] != 0)
    return 0;
    }
  return 1;
}

#if TUNE_PROGRAM_BUILD
/* Some extras wanted when recompiling some .c files for use by the tune
   program.  Not part of a normal build.

   It's necessary to keep these thresholds as #defines (just to an
   identically named variable), since various defaults are established based
   on #ifdef in the .c files.  For some this is not so (the defaults are
   instead establshed above), but all are done this way for consistency. */

#undef  MUL_KARATSUBA_THRESHOLD
#define MUL_KARATSUBA_THRESHOLD      mul_karatsuba_threshold
extern mp_size_t                     mul_karatsuba_threshold;

#undef  MUL_TOOM3_THRESHOLD
#define MUL_TOOM3_THRESHOLD          mul_toom3_threshold
extern mp_size_t                     mul_toom3_threshold;

#undef  MUL_TOOM4_THRESHOLD
#define MUL_TOOM4_THRESHOLD          mul_toom4_threshold
extern mp_size_t                     mul_toom4_threshold;

#undef  MUL_TOOM8H_THRESHOLD
#define MUL_TOOM8H_THRESHOLD         mul_toom8h_threshold
extern mp_size_t                     mul_toom8h_threshold;

#undef  MUL_FFT_THRESHOLD
#define MUL_FFT_THRESHOLD            mul_fft_threshold
extern mp_size_t                     mul_fft_threshold;

#undef  MUL_FFT_FULL_THRESHOLD
#define MUL_FFT_FULL_THRESHOLD       mul_fft_full_threshold
extern mp_size_t                     mul_fft_full_threshold;

/* A native mpn_sqr_basecase is not tuned and SQR_BASECASE_THRESHOLD should
   remain as zero (always use it). */
#if ! HAVE_NATIVE_mpn_sqr_basecase
#undef  SQR_BASECASE_THRESHOLD
#define SQR_BASECASE_THRESHOLD       sqr_basecase_threshold
extern mp_size_t                     sqr_basecase_threshold;
#endif

#if TUNE_PROGRAM_BUILD_SQR
#undef  SQR_KARATSUBA_THRESHOLD
#define SQR_KARATSUBA_THRESHOLD      SQR_KARATSUBA_MAX_GENERIC
#else
#undef  SQR_KARATSUBA_THRESHOLD
#define SQR_KARATSUBA_THRESHOLD      sqr_karatsuba_threshold
extern mp_size_t                     sqr_karatsuba_threshold;
#endif

#undef  SQR_TOOM3_THRESHOLD
#define SQR_TOOM3_THRESHOLD          sqr_toom3_threshold
extern mp_size_t                     sqr_toom3_threshold;

#undef  SQR_TOOM4_THRESHOLD
#define SQR_TOOM4_THRESHOLD          sqr_toom4_threshold
extern mp_size_t                     sqr_toom4_threshold;

#undef  SQR_TOOM8_THRESHOLD
#define SQR_TOOM8_THRESHOLD          sqr_toom8_threshold
extern mp_size_t                     sqr_toom8_threshold;

#undef SQR_FFT_THRESHOLD
#define SQR_FFT_THRESHOLD            sqr_fft_threshold
extern mp_size_t                     sqr_fft_threshold;

#undef SQR_FFT_FULL_THRESHOLD
#define SQR_FFT_FULL_THRESHOLD       sqr_fft_full_threshold
extern mp_size_t                     sqr_fft_full_threshold;

#undef  MULLOW_BASECASE_THRESHOLD
#define MULLOW_BASECASE_THRESHOLD    mullow_basecase_threshold
extern mp_size_t                     mullow_basecase_threshold;

#undef  MULLOW_DC_THRESHOLD
#define MULLOW_DC_THRESHOLD          mullow_dc_threshold
extern mp_size_t                     mullow_dc_threshold;

#undef  MULLOW_MUL_THRESHOLD
#define MULLOW_MUL_THRESHOLD         mullow_mul_threshold
extern mp_size_t                     mullow_mul_threshold;

#undef  MULMID_TOOM42_THRESHOLD
#define MULMID_TOOM42_THRESHOLD      mulmid_toom42_threshold
extern mp_size_t                     mulmid_toom42_threshold;

#undef  MULHIGH_BASECASE_THRESHOLD
#define MULHIGH_BASECASE_THRESHOLD   mulhigh_basecase_threshold
extern mp_size_t                     mulhigh_basecase_threshold;

#undef  MULHIGH_DC_THRESHOLD
#define MULHIGH_DC_THRESHOLD         mulhigh_dc_threshold
extern mp_size_t                     mulhigh_dc_threshold;

#undef  MULHIGH_MUL_THRESHOLD
#define MULHIGH_MUL_THRESHOLD        mulhigh_mul_threshold
extern mp_size_t                     mulhigh_mul_threshold;

#undef  MULMOD_2EXPM1_THRESHOLD
#define MULMOD_2EXPM1_THRESHOLD      mulmod_2expm1_threshold
extern mp_size_t                     mulmod_2expm1_threshold;

#if ! UDIV_PREINV_ALWAYS
#undef  DIV_SB_PREINV_THRESHOLD
#define DIV_SB_PREINV_THRESHOLD      div_sb_preinv_threshold
extern mp_size_t                     div_sb_preinv_threshold;
#endif

#undef  DC_DIV_QR_THRESHOLD
#define DC_DIV_QR_THRESHOLD          dc_div_qr_threshold
extern mp_size_t                     dc_div_qr_threshold;

#undef  DC_BDIV_QR_THRESHOLD
#define DC_BDIV_QR_THRESHOLD         dc_bdiv_qr_threshold
extern mp_size_t                     dc_bdiv_qr_threshold;

#undef  DC_BDIV_Q_THRESHOLD
#define DC_BDIV_Q_THRESHOLD          dc_bdiv_q_threshold
extern mp_size_t                     dc_bdiv_q_threshold;

#undef  INV_DIV_QR_THRESHOLD
#define INV_DIV_QR_THRESHOLD         inv_div_qr_threshold
extern mp_size_t                     inv_div_qr_threshold;

#undef  INV_DIVAPPR_Q_N_THRESHOLD
#define INV_DIVAPPR_Q_N_THRESHOLD    inv_divappr_q_n_threshold
extern mp_size_t                     inv_divappr_q_n_threshold;

#undef  DC_DIV_Q_THRESHOLD
#define DC_DIV_Q_THRESHOLD           dc_div_q_threshold
extern mp_size_t                     dc_div_q_threshold;

#undef  INV_DIV_Q_THRESHOLD
#define INV_DIV_Q_THRESHOLD          inv_div_q_threshold
extern mp_size_t                     inv_div_q_threshold;

#undef BINV_NEWTON_THRESHOLD
#define BINV_NEWTON_THRESHOLD        binv_newton_threshold
extern mp_size_t                     binv_newton_threshold;

#undef  DC_DIVAPPR_Q_THRESHOLD
#define DC_DIVAPPR_Q_THRESHOLD       dc_divappr_q_threshold
extern mp_size_t                     dc_divappr_q_threshold;

#undef  INV_DIVAPPR_Q_THRESHOLD
#define INV_DIVAPPR_Q_THRESHOLD      inv_divappr_q_threshold
extern mp_size_t                     inv_divappr_q_threshold;

#undef  ROOTREM_THRESHOLD
#define ROOTREM_THRESHOLD            rootrem_threshold
extern mp_size_t                     rootrem_threshold;

#undef DIVREM_HENSEL_QR_1_THRESHOLD
#define DIVREM_HENSEL_QR_1_THRESHOLD divrem_hensel_qr_1_threshold
extern mp_size_t		     divrem_hensel_qr_1_threshold;

#undef RSH_DIVREM_HENSEL_QR_1_THRESHOLD
#define RSH_DIVREM_HENSEL_QR_1_THRESHOLD rsh_divrem_hensel_qr_1_threshold
extern mp_size_t		     rsh_divrem_hensel_qr_1_threshold;

#undef DIVREM_EUCLID_HENSEL_THRESHOLD
#define DIVREM_EUCLID_HENSEL_THRESHOLD divrem_euclid_hensel_threshold
extern mp_size_t		       divrem_euclid_hensel_threshold;

#undef  MOD_1_1_THRESHOLD
#define MOD_1_1_THRESHOLD            mod_1_1_threshold
extern mp_size_t                     mod_1_1_threshold;

#undef  MOD_1_2_THRESHOLD
#define MOD_1_2_THRESHOLD            mod_1_2_threshold
extern mp_size_t                     mod_1_2_threshold;

#undef  MOD_1_3_THRESHOLD
#define MOD_1_3_THRESHOLD            mod_1_3_threshold
extern mp_size_t                     mod_1_3_threshold;

#undef	REDC_1_TO_REDC_2_THRESHOLD
#define REDC_1_TO_REDC_2_THRESHOLD	redc_1_to_redc_2_threshold
extern mp_size_t			redc_1_to_redc_2_threshold;

#undef	REDC_2_TO_REDC_N_THRESHOLD
#define REDC_2_TO_REDC_N_THRESHOLD	redc_2_to_redc_n_threshold
extern mp_size_t			redc_2_to_redc_n_threshold;

#undef	REDC_1_TO_REDC_N_THRESHOLD
#define REDC_1_TO_REDC_N_THRESHOLD	redc_1_to_redc_n_threshold
extern mp_size_t			redc_1_to_redc_n_threshold;

#undef	MATRIX22_STRASSEN_THRESHOLD
#define MATRIX22_STRASSEN_THRESHOLD	matrix22_strassen_threshold
extern mp_size_t			matrix22_strassen_threshold;

#undef	HGCD_THRESHOLD
#define HGCD_THRESHOLD			hgcd_threshold
extern mp_size_t			hgcd_threshold;

#undef	HGCD_APPR_THRESHOLD
#define HGCD_APPR_THRESHOLD		hgcd_appr_threshold
extern mp_size_t			hgcd_appr_threshold;

#undef	HGCD_REDUCE_THRESHOLD
#define HGCD_REDUCE_THRESHOLD		hgcd_reduce_threshold
extern mp_size_t			hgcd_reduce_threshold;

#undef	GCD_DC_THRESHOLD
#define GCD_DC_THRESHOLD		gcd_dc_threshold
extern mp_size_t			gcd_dc_threshold;

#undef  GCDEXT_DC_THRESHOLD
#define GCDEXT_DC_THRESHOLD		gcdext_dc_threshold
extern mp_size_t			gcdext_dc_threshold;

#undef DIVREM_1_NORM_THRESHOLD
#define DIVREM_1_NORM_THRESHOLD      divrem_1_norm_threshold
extern mp_size_t                     divrem_1_norm_threshold;

#undef DIVREM_1_UNNORM_THRESHOLD
#define DIVREM_1_UNNORM_THRESHOLD    divrem_1_unnorm_threshold
extern mp_size_t                     divrem_1_unnorm_threshold;

#undef MOD_1_NORM_THRESHOLD
#define MOD_1_NORM_THRESHOLD         mod_1_norm_threshold
extern mp_size_t                     mod_1_norm_threshold;

#undef MOD_1_UNNORM_THRESHOLD
#define MOD_1_UNNORM_THRESHOLD       mod_1_unnorm_threshold
extern mp_size_t                     mod_1_unnorm_threshold;

#if ! UDIV_PREINV_ALWAYS
#undef  DIVREM_2_THRESHOLD
#define DIVREM_2_THRESHOLD           divrem_2_threshold
extern mp_size_t                     divrem_2_threshold;
#endif

#undef  GET_STR_DC_THRESHOLD
#define GET_STR_DC_THRESHOLD         get_str_dc_threshold
extern mp_size_t                     get_str_dc_threshold;

#undef GET_STR_PRECOMPUTE_THRESHOLD
#define GET_STR_PRECOMPUTE_THRESHOLD get_str_precompute_threshold
extern mp_size_t                     get_str_precompute_threshold;

#undef	SET_STR_DC_THRESHOLD
#define SET_STR_DC_THRESHOLD		set_str_dc_threshold
extern mp_size_t			set_str_dc_threshold;

#undef  SET_STR_PRECOMPUTE_THRESHOLD
#define SET_STR_PRECOMPUTE_THRESHOLD	set_str_precompute_threshold
extern mp_size_t			set_str_precompute_threshold;

#undef  FAC_ODD_THRESHOLD
#define FAC_ODD_THRESHOLD		fac_odd_threshold
extern  mp_size_t			fac_odd_threshold;

#undef  FAC_DSC_THRESHOLD
#define FAC_DSC_THRESHOLD		fac_dsc_threshold
extern  mp_size_t			fac_dsc_threshold;

/* Sizes the tune program tests up to, used in a couple of recompilations. */
#undef MUL_KARATSUBA_THRESHOLD_LIMIT
#undef MUL_TOOM3_THRESHOLD_LIMIT
#undef MUL_TOOM4_THRESHOLD_LIMIT
#undef MUL_TOOM8H_THRESHOLD_LIMIT
#undef MULLOW_BASECASE_THRESHOLD_LIMIT
#undef SQR_TOOM3_THRESHOLD_LIMIT
#undef SQR_TOOM4_THRESHOLD_LIMIT
#undef SQR_TOOM8_THRESHOLD_LIMIT
#define SQR_KARATSUBA_MAX_GENERIC       200
#define MUL_KARATSUBA_THRESHOLD_LIMIT   700
#define MUL_TOOM3_THRESHOLD_LIMIT       700
#define MUL_TOOM4_THRESHOLD_LIMIT       1000
#define MUL_TOOM8H_THRESHOLD_LIMIT      2000
#define MULLOW_BASECASE_THRESHOLD_LIMIT 200
#define SQR_TOOM3_THRESHOLD_LIMIT       400
#define SQR_TOOM4_THRESHOLD_LIMIT       1000
#define SQR_TOOM8_THRESHOLD_LIMIT       2000
#define GET_STR_THRESHOLD_LIMIT         150
#define FAC_DSC_THRESHOLD_LIMIT         2048

#endif /* TUNE_PROGRAM_BUILD */

#if defined (__cplusplus)
}
#endif


#ifdef __cplusplus

/* A little helper for a null-terminated __gmp_allocate_func string.
   The destructor ensures it's freed even if an exception is thrown.
   The len field is needed by the destructor, and can be used by anyone else
   to avoid a second strlen pass over the data.

   Since our input is a C string, using strlen is correct.  Perhaps it'd be
   more C++-ish style to use std::char_traits<char>::length, but char_traits
   isn't available in gcc 2.95.4.  */

class gmp_allocated_string {
 public:
  char *str;
  size_t len;
  gmp_allocated_string(char *arg)
  {
    str = arg;
    len = std::strlen (str);
  }
  ~gmp_allocated_string()
  {
    (*__gmp_free_func) (str, len+1);
  }
};

std::istream &__gmpz_operator_in_nowhite (std::istream &, mpz_ptr, char);
int __gmp_istream_set_base (std::istream &, char &, bool &, bool &);
void __gmp_istream_set_digits (std::string &, std::istream &, char &, bool &, int);
void __gmp_doprnt_params_from_ios (struct doprnt_params_t *p, std::ios &o);
std::ostream& __gmp_doprnt_integer_ostream (std::ostream &o, struct doprnt_params_t *p, char *s);
extern const struct doprnt_funs_t  __gmp_asprintf_funs_noformat;

#endif /* __cplusplus */

#endif /* __GMP_IMPL_H__ */
