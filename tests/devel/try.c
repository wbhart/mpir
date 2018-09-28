/* Run some tests on various mpn routines.

   THIS IS A TEST PROGRAM USED ONLY FOR DEVELOPMENT.  IT'S ALMOST CERTAIN TO
   BE SUBJECT TO INCOMPATIBLE CHANGES IN FUTURE VERSIONS OF GMP.

Copyright 2000, 2001, 2002, 2003, 2004, 2005, 2006 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA. */


/* Usage: try [options] <function>...

   For example, "./try mpn_add_n" to run tests of that function.

   Combinations of alignments and overlaps are tested, with redzones above
   or below the destinations, and with the sources write-protected.

   The number of tests performed becomes ridiculously large with all the
   combinations, and for that reason this can't be a part of a "make check",
   it's meant only for development.  The code isn't very pretty either.

   During development it can help to disable the redzones, since seeing the
   rest of the destination written can show where the wrong part is, or if
   the dst pointers are off by 1 or whatever.  The magic DEADVAL initial
   fill (see below) will show locations never written.

   The -s option can be used to test only certain size operands, which is
   useful if some new code doesn't yet support say sizes less than the
   unrolling, or whatever.

   When a problem occurs it'll of course be necessary to run the program
   under gdb to find out quite where, how and why it's going wrong.  Disable
   the spinner with the -W option when doing this, or single stepping won't
   work.  Using the "-1" option to run with simple data can be useful.

   New functions to test can be added in try_array[].  If a new TYPE is
   required then add it to the existing constants, set up its parameters in
   param_init(), and add it to the call() function.  Extra parameter fields
   can be added if necessary, or further interpretations given to existing
   fields.


   Enhancements:

   umul_ppmm support is not very good, lots of source data is generated
   whereas only two limbs are needed.

   Make a little scheme for interpreting the "SIZE" selections uniformly.

   Make tr->size==SIZE_2 work, for the benefit of find_a which wants just 2
   source limbs.  Possibly increase the default repetitions in that case.

   Automatically detect gdb and disable the spinner (use -W for now).

   Make a way to re-run a failing case in the debugger.  Have an option to
   snapshot each test case before it's run so the data is available if a
   segv occurs.  (This should be more reliable than the current print_all()
   in the signal handler.)

   When alignment means a dst isn't hard against the redzone, check the
   space in between remains unchanged.

   When a source overlaps a destination, don't run both s[i].high 0 and 1,
   as s[i].high has no effect.  Maybe encode s[i].high into overlap->s[i].

   When partial overlaps aren't done, don't loop over source alignments
   during overlaps.

   Try to make the looping code a bit less horrible.  Right now it's pretty
   hard to see what iterations are actually done.

   Perhaps specific setups and loops for each style of function under test
   would be clearer than a parameterized general loop.  There's lots of
   stuff common to all functions, but the exceptions get messy.

   When there's no overlap, run with both src>dst and src<dst.  A subtle
   calling-conventions violation occured in a P6 copy which depended on the
   relative location of src and dst.

   multiplier_N is more or less a third source region for the addmul_N
   routines, and could be done with the redzoned region scheme.

*/

/* always do assertion checking */
#define WANT_ASSERT 1

#include "config.h"

#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined( _MSC_VER )
#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#endif

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#if HAVE_SYS_MMAN_H
#include <sys/mman.h>
#endif

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"
#include "tests.h"


#if !HAVE_DECL_OPTARG
extern char *optarg;
extern int optind, opterr;
#endif

#if ! HAVE_DECL_SYS_NERR
extern int sys_nerr;
#endif

#if ! HAVE_DECL_SYS_ERRLIST && !defined( _MSC_VER )
extern char *sys_errlist[];
#endif

#if ! HAVE_STRERROR
char *
strerror (int n)
{
  if (n < 0 || n >= sys_nerr)
    return "errno out of range";
  else
    return sys_errlist[n];
}
#endif

/* Rumour has it some systems lack a define of PROT_NONE. */
#ifndef PROT_NONE
#define PROT_NONE   0
#endif

/* Dummy defines for when mprotect doesn't exist. */
#ifndef PROT_READ
#define PROT_READ   0
#endif
#ifndef PROT_WRITE
#define PROT_WRITE  0
#endif

/* _SC_PAGESIZE is standard, but hpux 9 and possibly other systems have
   _SC_PAGE_SIZE instead. */
#if defined (_SC_PAGE_SIZE) && ! defined (_SC_PAGESIZE)
#define _SC_PAGESIZE  _SC_PAGE_SIZE
#endif


#ifdef EXTRA_PROTOS
EXTRA_PROTOS
#endif
#ifdef EXTRA_PROTOS2
EXTRA_PROTOS2
#endif


#define DEFAULT_REPETITIONS  10

int  option_repetitions = DEFAULT_REPETITIONS;
int  option_spinner = 1;
int  option_redzones = 1;
int  option_firstsize = 0;
int  option_lastsize = 500;
int  option_firstsize2 = 0;

#define ALIGNMENTS          4
#define OVERLAPS            4
#define CARRY_RANDOMS       5
#define MULTIPLIER_RANDOMS  5
#define DIVISOR_RANDOMS     5
#define FRACTION_COUNT      4

int  option_print = 0;

#define DATA_TRAND  0
#define DATA_ZEROS  1
#define DATA_SEQ    2
#define DATA_FFS    3
#define DATA_2FD    4
int  option_data = DATA_TRAND;


mp_size_t  pagesize;
#define PAGESIZE_LIMBS  (pagesize / BYTES_PER_MP_LIMB)

/* must be a multiple of the page size */
#define REDZONE_BYTES   (pagesize * 16)
#define REDZONE_LIMBS   (REDZONE_BYTES / BYTES_PER_MP_LIMB)


#define MAX3(x,y,z)   (MAX (x, MAX (y, z)))

#if BITS_PER_MP_LIMB == 32
#define DEADVAL  CNST_LIMB(0xDEADBEEF)
#else
#define DEADVAL  CNST_LIMB(0xDEADBEEFBADDCAFE)
#endif


struct region_t {
  mp_ptr     ptr;
  mp_size_t  size;
};


#define TRAP_NOWHERE 0
#define TRAP_REF     1
#define TRAP_FUN     2
#define TRAP_SETUPS  3
int trap_location = TRAP_NOWHERE;


#define NUM_SOURCES  3
#define NUM_DESTS    2

struct source_t {
  struct region_t  region;
  int        high;
  mp_size_t  align;
  mp_ptr     p;
};

struct source_t  s[NUM_SOURCES];

struct dest_t {
  int        high;
  mp_size_t  align;
  mp_size_t  size;
};

struct dest_t  d[NUM_DESTS];

struct source_each_t {
  mp_ptr     p;
};

struct dest_each_t {
  struct region_t  region;
  mp_ptr     p;
};

mp_size_t       size;
mp_size_t       size2;
unsigned long   shift;
mp_limb_t       carry;
mp_limb_t       divisor;
mp_limb_t	altdiv;
mp_limb_t       multiplier;
mp_limb_t       multiplier_N[8];

struct each_t {
  const char  *name;
  struct dest_each_t    d[NUM_DESTS];
  struct source_each_t  s[NUM_SOURCES];
  mp_limb_t  retval;
};

struct each_t  ref = { "Ref" };
struct each_t  fun = { "Fun" };

#define SRC_SIZE(n)  ((n) == 1 && tr->size2 ? size2 : size)

void validate_fail(void);


#if HAVE_TRY_NEW_C
#include "try-new.c"
#endif


typedef mp_limb_t (*tryfun_t)(ANYARGS);

struct try_t {
  char  retval;

  char  src[NUM_SOURCES];
  char  dst[NUM_DESTS];

#define SIZE_YES          1
#define SIZE_ALLOW_ZERO   2
#define SIZE_1            3  /* 1 limb  */
#define SIZE_2            4  /* 2 limbs */
#define SIZE_3            5  /* 3 limbs */
#define SIZE_4		  20 /* 4 limbs */
#define SIZE_FRACTION     6  /* size2 is fraction for divrem etc */
#define SIZE_SIZE2        7
#define SIZE_PLUS_1       8
#define SIZE_PLUS_2       9
#define SIZE_SUM         10
#define SIZE_DIFF        11
#define SIZE_DIFF_PLUS_1 12
#define SIZE_DIFF_PLUS_3 13
#define SIZE_RETVAL      14
#define SIZE_CEIL_HALF   15
#define SIZE_GET_STR     16
#define SIZE_PLUS_MSIZE_SUB_1 17  /* size+msize-1 */
#define SIZE_DOUBLE	18
#define SIZE_DOUBLE_MINUS_1	19
  char  size;
  char  size2;
  char  dst_size[NUM_DESTS];

  /* multiplier_N size in limbs */
  mp_size_t  msize;

  char  dst_bytes[NUM_DESTS];

  char  dst0_from_src1;

#define CARRY_BIT     1  /* single bit 0 or 1 */
#define CARRY_3       2  /* 0, 1, 2 */
#define CARRY_4       3  /* 0 to 3 */
#define CARRY_LIMB    4  /* any limb value */
#define CARRY_DIVISOR 5  /* carry<divisor */
  char  carry;

  /* a fudge to tell the output when to print negatives */
  char  carry_sign;

  char  multiplier;
  char  shift;

#define DIVISOR_LIMB  1
#define DIVISOR_NORM  2
#define DIVISOR_ODD   3
#define DIVISOR_DIVBM1	4
  char  divisor;

#define DATA_NON_ZERO         1
#define DATA_GCD              2
#define DATA_SRC1_ODD         3
#define DATA_SRC1_HIGHBIT     4
#define DATA_MULTIPLE_DIVISOR 5
#define DATA_UDIV_QRNND       6
#define DATA_SRC0_ODD	      7
  char  data;

/* Default is allow full overlap. */
#define OVERLAP_NONE         1
#define OVERLAP_LOW_TO_HIGH  2
#define OVERLAP_HIGH_TO_LOW  3
#define OVERLAP_NOT_SRCS     4
#define OVERLAP_NOT_SRC2     8
  char  overlap;

  tryfun_t    reference;
  const char  *reference_name;

  void        (*validate)(void);
  const char  *validate_name;
};

struct try_t  *tr;

void
validate_mod_34lsub1 (void)
{
#define CNST_34LSUB1   ((CNST_LIMB(1) << (3 * (GMP_NUMB_BITS / 4))) - 1)

  mp_srcptr  ptr = s[0].p;
  int        error = 0;
  mp_limb_t  got, got_mod, want, want_mod;

  ASSERT (size >= 1);

  got = fun.retval;
  got_mod = got % CNST_34LSUB1;

  want = refmpn_mod_34lsub1 (ptr, size);
  want_mod = want % CNST_34LSUB1;

  if (got_mod != want_mod)
    {
      gmp_printf ("got   0x%MX reduced from 0x%MX\n", got_mod, got);
      gmp_printf ("want  0x%MX reduced from 0x%MX\n", want_mod, want);
      error = 1;
    }

  if (error)
    validate_fail ();
}

void
validate_divexact_1 (void)
{
  mp_srcptr  src = s[0].p;
  mp_srcptr  dst = fun.d[0].p;
  int  error = 0;

  ASSERT (size >= 1);

  {
    mp_ptr     tp = refmpn_malloc_limbs (size);
    mp_limb_t  rem;

    rem = refmpn_divrem_1 (tp, 0, src, size, divisor);
    if (rem != 0)
      {
	gmp_printf ("Remainder a%%d == 0x%MX, mpn_divexact_1 undefined\n", rem);
	error = 1;
      }
    if (! refmpn_equal_anynail (tp, dst, size))
      {
	printf ("Quotient a/d wrong\n");
	mpn_trace ("fun ", dst, size);
	mpn_trace ("want", tp, size);
	error = 1;
      }
    free (tp);
  }

  if (error)
    validate_fail ();
}


void
validate_modexact_1c_odd (void)
{
  mp_srcptr  ptr = s[0].p;
  mp_limb_t  r = fun.retval;
  int  error = 0;

  ASSERT (size >= 1);
  ASSERT (divisor & 1);

  if ((r & GMP_NAIL_MASK) != 0)
    printf ("r has non-zero nail\n");

  if (carry < divisor)
    {
      if (! (r < divisor))
	{
	  printf ("Don't have r < divisor\n");
	  error = 1;
	}
    }
  else /* carry >= divisor */
    {
      if (! (r <= divisor))
	{
	  printf ("Don't have r <= divisor\n");
	  error = 1;
	}
    }

  {
    mp_limb_t  c = carry % divisor;
    mp_ptr     tp = refmpn_malloc_limbs (size+1);
    mp_size_t  k;

    for (k = size-1; k <= size; k++)
      {
	/* set {tp,size+1} to r*b^k + a - c */
	refmpn_copyi (tp, ptr, size);
	tp[size] = 0;
	ASSERT_NOCARRY (refmpn_add_1 (tp+k, tp+k, size+1-k, r));
	if (refmpn_sub_1 (tp, tp, size+1, c))
	  ASSERT_CARRY (mpn_add_1 (tp, tp, size+1, divisor));

	if (refmpn_mod_1 (tp, size+1, divisor) == 0)
	  goto good_remainder;
      }
    printf ("Remainder matches neither r*b^(size-1) nor r*b^size\n");
    error = 1;

  good_remainder:
    free (tp);
  }

  if (error)
    validate_fail ();
}

void
validate_modexact_1_odd (void)
{
  carry = 0;
  validate_modexact_1c_odd ();
}


void
validate_sqrtrem (void)
{
  mp_srcptr  orig_ptr = s[0].p;
  mp_size_t  orig_size = size;
  mp_size_t  root_size = (size+1)/2;
  mp_srcptr  root_ptr = fun.d[0].p;
  mp_size_t  rem_size = fun.retval;
  mp_srcptr  rem_ptr = fun.d[1].p;
  mp_size_t  prod_size = 2*root_size;
  mp_ptr     p;
  int  error = 0;

  if (rem_size < 0 || rem_size > size)
    {
      printf ("Bad remainder size retval %ld\n", (long) rem_size);
      validate_fail ();
    }

  p = refmpn_malloc_limbs (prod_size);

  p[root_size] = refmpn_lshift (p, root_ptr, root_size, 1);
  if (refmpn_cmp_twosizes (p,root_size+1, rem_ptr,rem_size) < 0)
    {
      printf ("Remainder bigger than 2*root\n");
      error = 1;
    }

  refmpn_sqr (p, root_ptr, root_size);
  if (rem_size != 0)
    refmpn_add (p, p, prod_size, rem_ptr, rem_size);
  if (refmpn_cmp_twosizes (p,prod_size, orig_ptr,orig_size) != 0)
    {
      printf ("root^2+rem != original\n");
      mpn_trace ("prod", p, prod_size);
      error = 1;
    }
  free (p);

  if (error)
    validate_fail ();
}


/* These types are indexes into the param[] array and are arbitrary so long
   as they're all distinct and within the size of param[].  Renumber
   whenever necessary or desired.  */

#define TYPE_ADD               1
#define TYPE_ADD_N             2
#define TYPE_ADD_NC            3
#define TYPE_SUB               4
#define TYPE_SUB_N             5
#define TYPE_SUB_NC            6

#define TYPE_MUL_1             7
#define TYPE_MUL_1C            8

#define TYPE_MUL_2             9

#define TYPE_ADDMUL_1         10
#define TYPE_ADDMUL_1C        11
#define TYPE_SUBMUL_1         12
#define TYPE_SUBMUL_1C        13

#define TYPE_ADDMUL_2         14
#define TYPE_ADDMUL_3         15
#define TYPE_ADDMUL_4         16
#define TYPE_ADDMUL_5         17
#define TYPE_ADDMUL_6         18
#define TYPE_ADDMUL_7         19
#define TYPE_ADDMUL_8         20

#define TYPE_SUMDIFF_N        21
#define TYPE_NSUMDIFF_N       141
#define TYPE_SUMDIFF_NC       22

#define TYPE_RSHIFT           23
#define TYPE_LSHIFT           24

#define TYPE_COPY             25
#define TYPE_COPYI            26
#define TYPE_COPYD            27
#define TYPE_COM_N            28

#define TYPE_ADDLSH1_N        30
#define TYPE_SUBLSH1_N        31
#define TYPE_RSH1ADD_N        32
#define TYPE_RSH1SUB_N        33

#define TYPE_MOD_1            35
#define TYPE_MOD_1C           36
#define TYPE_DIVMOD_1         37
#define TYPE_DIVMOD_1C        38
#define TYPE_DIVREM_1         39
#define TYPE_DIVREM_1C        40
#define TYPE_PREINV_DIVREM_1  41
#define TYPE_PREINV_MOD_1     42
#define TYPE_MOD_34LSUB1      43
#define TYPE_UDIV_QRNND       44
#define TYPE_UDIV_QRNND_R     45

#define TYPE_DIVEXACT_1       50
#define TYPE_DIVEXACT_BY3     51
#define TYPE_DIVEXACT_BY3C    52
#define TYPE_MODEXACT_1_ODD   53
#define TYPE_MODEXACT_1C_ODD  54

#define TYPE_GCD              60
#define TYPE_GCD_1            61
#define TYPE_GCD_FINDA        62
#define TYPE_MPZ_JACOBI       63
#define TYPE_MPZ_KRONECKER    64
#define TYPE_MPZ_KRONECKER_UI 65
#define TYPE_MPZ_KRONECKER_SI 66
#define TYPE_MPZ_UI_KRONECKER 67
#define TYPE_MPZ_SI_KRONECKER 68

#define TYPE_AND_N            70
#define TYPE_NAND_N           71
#define TYPE_ANDN_N           72
#define TYPE_IOR_N            73
#define TYPE_IORN_N           74
#define TYPE_NIOR_N           75
#define TYPE_XOR_N            76
#define TYPE_XNOR_N           77

#define TYPE_MUL_BASECASE     80
#define TYPE_MUL_N            81
#define TYPE_MULMID_BASECASE  82
#define TYPE_MULMID           83
#define TYPE_MULMID_N         84
#define TYPE_SQR              85
#define TYPE_UMUL_PPMM        86
#define TYPE_UMUL_PPMM_R      87

#define TYPE_SB_DIVREM_MN     90
#define TYPE_TDIV_QR          91
#define TYPE_TDIV_Q           92

#define TYPE_SQRTREM          100
#define TYPE_ZERO             101
#define TYPE_GET_STR          102
#define TYPE_POPCOUNT         103
#define TYPE_HAMDIST          104

#define TYPE_DIVEXACT_BYFF    105
#define TYPE_LSHIFT1	      106
#define TYPE_RSHIFT1	      107

#define TYPE_ADDADD_N	      108
#define TYPE_ADDSUB_N	      109
#define TYPE_SUBADD_N	      110

#define TYPE_REDC_BASECASE	111
#define TYPE_DIVREM_EUCLIDEAN_QR_1	112
#define TYPE_DIVREM_EUCLIDEAN_R_1	113
#define TYPE_DIVEXACT_BYFOBM1 114

#define TYPE_LSHIFT2	      115
#define TYPE_RSHIFT2	      116
#define TYPE_STORE		117
#define TYPE_LSHIFTC		118
//#define TYPE_DIVREM_EUCLIDEAN_QR_2	118
#define TYPE_ADDLSH_N		120
#define TYPE_SUBLSH_N		121

#define TYPE_INCLSH_N		122
#define TYPE_DECLSH_N		123
#define TYPE_ADDERR1_N		124
#define TYPE_SUBERR1_N		125
#define TYPE_ADDERR2_N		126
#define TYPE_SUBERR2_N		127
#define TYPE_ADDLSH_NC		128
#define TYPE_SUBLSH_NC		129

#define TYPE_DIVREM_HENSEL_QR_1		130
#define TYPE_DIVREM_HENSEL_QR_1_1	131
#define TYPE_DIVREM_HENSEL_QR_1_2	132
#define TYPE_DIVREM_HENSEL_R_1	133
#define TYPE_RSH_DIVREM_HENSEL_QR_1	134
#define TYPE_RSH_DIVREM_HENSEL_QR_1_1	135
#define TYPE_RSH_DIVREM_HENSEL_QR_1_2	136
#define TYPE_DIVREM_HENSEL_RSH_QR_1	137
#define TYPE_NOT			138

#define TYPE_DOUBLE	139
#define TYPE_HALF	140

#define TYPE_EXTRA            150

struct try_t  param[150];


void
param_init (void)
{
  struct try_t  *p;

#define COPY(index)  memcpy (p, &param[index], sizeof (*p))

#if HAVE_STRINGIZE
#define REFERENCE(fun)                  \
  p->reference = (tryfun_t) fun;        \
  p->reference_name = #fun
#define VALIDATE(fun)           \
  p->validate = fun;            \
  p->validate_name = #fun
#else
#define REFERENCE(fun)                  \
  p->reference = (tryfun_t) fun;        \
  p->reference_name = "fun"
#define VALIDATE(fun)           \
  p->validate = fun;            \
  p->validate_name = "fun"
#endif


  p = &param[TYPE_ADD_N];
  p->retval = 1;
  p->dst[0] = 1;
  p->src[0] = 1;
  p->src[1] = 1;
  REFERENCE (refmpn_add_n);

  p = &param[TYPE_ADD_NC];
  COPY (TYPE_ADD_N);
  p->carry = CARRY_BIT;
  REFERENCE (refmpn_add_nc);

  p = &param[TYPE_SUB_N];
  COPY (TYPE_ADD_N);
  REFERENCE (refmpn_sub_n);

  p = &param[TYPE_SUB_NC];
  COPY (TYPE_ADD_NC);
  REFERENCE (refmpn_sub_nc);

  p = &param[TYPE_ADD];
  COPY (TYPE_ADD_N);
  p->size = SIZE_ALLOW_ZERO;
  p->size2 = 1;
  REFERENCE (refmpn_add);

  p = &param[TYPE_SUB];
  COPY (TYPE_ADD);
  REFERENCE (refmpn_sub);


  p = &param[TYPE_MUL_1];
  p->retval = 1;
  p->dst[0] = 1;
  p->src[0] = 1;
  p->multiplier = 1;
  p->overlap = OVERLAP_LOW_TO_HIGH;
  REFERENCE (refmpn_mul_1);

  p = &param[TYPE_MUL_1C];
  COPY (TYPE_MUL_1);
  p->carry = CARRY_LIMB;
  REFERENCE (refmpn_mul_1c);


  p = &param[TYPE_MUL_2];
  p->retval = 1;
  p->dst[0] = 1;
  p->dst_size[0] = SIZE_PLUS_MSIZE_SUB_1;
  p->src[0] = 1;
  p->src[1] = 1;
  p->msize = 2;
  p->overlap = OVERLAP_NOT_SRC2;
  REFERENCE (refmpn_mul_2);


  p = &param[TYPE_ADDMUL_1];
  p->retval = 1;
  p->dst[0] = 1;
  p->src[0] = 1;
  p->multiplier = 1;
  p->dst0_from_src1 = 1;
  REFERENCE (refmpn_addmul_1);

  p = &param[TYPE_ADDMUL_1C];
  COPY (TYPE_ADDMUL_1);
  p->carry = CARRY_LIMB;
  REFERENCE (refmpn_addmul_1c);

  p = &param[TYPE_SUBMUL_1];
  COPY (TYPE_ADDMUL_1);
  REFERENCE (refmpn_submul_1);

  p = &param[TYPE_SUBMUL_1C];
  COPY (TYPE_ADDMUL_1C);
  REFERENCE (refmpn_submul_1c);


  p = &param[TYPE_ADDMUL_2];
  p->retval = 1;
  p->dst[0] = 1;
  p->dst_size[0] = SIZE_PLUS_MSIZE_SUB_1;
  p->src[0] = 1;
  p->src[1] = 1;
  p->msize = 2;
  p->dst0_from_src1 = 1;
  p->overlap = OVERLAP_NOT_SRC2;
  REFERENCE (refmpn_addmul_2);

  p = &param[TYPE_ADDMUL_3];
  COPY (TYPE_ADDMUL_2);
  p->msize = 3;
  REFERENCE (refmpn_addmul_3);

  p = &param[TYPE_ADDMUL_4];
  COPY (TYPE_ADDMUL_2);
  p->msize = 4;
  REFERENCE (refmpn_addmul_4);

  p = &param[TYPE_ADDMUL_5];
  COPY (TYPE_ADDMUL_2);
  p->msize = 5;
  REFERENCE (refmpn_addmul_5);

  p = &param[TYPE_ADDMUL_6];
  COPY (TYPE_ADDMUL_2);
  p->msize = 6;
  REFERENCE (refmpn_addmul_6);

  p = &param[TYPE_ADDMUL_7];
  COPY (TYPE_ADDMUL_2);
  p->msize = 7;
  REFERENCE (refmpn_addmul_7);

  p = &param[TYPE_ADDMUL_8];
  COPY (TYPE_ADDMUL_2);
  p->msize = 8;
  REFERENCE (refmpn_addmul_8);


  p = &param[TYPE_AND_N];
  p->dst[0] = 1;
  p->src[0] = 1;
  p->src[1] = 1;
  REFERENCE (refmpn_and_n);

  p = &param[TYPE_ANDN_N];
  COPY (TYPE_AND_N);
  REFERENCE (refmpn_andn_n);

  p = &param[TYPE_NAND_N];
  COPY (TYPE_AND_N);
  REFERENCE (refmpn_nand_n);

  p = &param[TYPE_IOR_N];
  COPY (TYPE_AND_N);
  REFERENCE (refmpn_ior_n);

  p = &param[TYPE_IORN_N];
  COPY (TYPE_AND_N);
  REFERENCE (refmpn_iorn_n);

  p = &param[TYPE_NIOR_N];
  COPY (TYPE_AND_N);
  REFERENCE (refmpn_nior_n);

  p = &param[TYPE_XOR_N];
  COPY (TYPE_AND_N);
  REFERENCE (refmpn_xor_n);

  p = &param[TYPE_XNOR_N];
  COPY (TYPE_AND_N);
  REFERENCE (refmpn_xnor_n);

  p = &param[TYPE_SUMDIFF_N];
  p->retval = 1;
  p->dst[0] = 1;
  p->dst[1] = 1;
  p->src[0] = 1;
  p->src[1] = 1;
  REFERENCE (refmpn_sumdiff_n);

  p = &param[TYPE_NSUMDIFF_N];
  COPY (TYPE_SUMDIFF_N);
  REFERENCE (refmpn_nsumdiff_n);

  p = &param[TYPE_ADDERR1_N];
  p->retval=1;
  p->dst[0]=1;
  p->dst[1]=1;
  p->dst_size[1]=SIZE_2;
  p->src[0]=1;
  p->src[1]=1;
  p->src[2]=1;
  p->carry=CARRY_BIT;
  p->overlap=OVERLAP_NONE;
  REFERENCE (refmpn_add_err1_n);
  
  p = &param[TYPE_SUBERR1_N];
  p->retval=1;
  p->dst[0]=1;
  p->dst[1]=1;
  p->dst_size[1]=SIZE_2;
  p->src[0]=1;
  p->src[1]=1;
  p->src[2]=1;
  p->carry=CARRY_BIT;
  p->overlap=OVERLAP_NONE;
  REFERENCE (refmpn_sub_err1_n);
  
  p = &param[TYPE_ADDERR2_N];
  p->retval=1;
  p->dst[0]=1;
  p->dst[1]=1;
  p->dst_size[1]=SIZE_4;
  p->src[0]=1;
  p->src[1]=1;
  p->src[2]=1;
  p->src[3]=1;//FIXME
  p->carry=CARRY_BIT;
  p->overlap=OVERLAP_NONE;
  REFERENCE (refmpn_add_err2_n);
  
  p = &param[TYPE_SUBERR2_N];
  p->retval=1;
  p->dst[0]=1;
  p->dst[1]=1;
  p->dst_size[1]=SIZE_4;
  p->src[0]=1;
  p->src[1]=1;
  p->src[2]=1;
  p->src[3]=1;//FIXME
  p->carry=CARRY_BIT;
  p->overlap=OVERLAP_NONE;
  REFERENCE (refmpn_sub_err2_n);
  
  p = &param[TYPE_SUMDIFF_NC];
  COPY (TYPE_SUMDIFF_N);
  p->carry = CARRY_4;
  REFERENCE (refmpn_sumdiff_nc);

  p = &param[TYPE_ADDADD_N];
  p->retval = 1;
  p->dst[0] = 1;
  p->src[0] = 1;
  p->src[1] = 1;
  p->src[2] = 1;
  REFERENCE (refmpn_addadd_n);

  p = &param[TYPE_ADDSUB_N];
  p->retval = 1;
  p->dst[0] = 1;
  p->src[0] = 1;
  p->src[1] = 1;
  p->src[2] = 1;
  REFERENCE (refmpn_addsub_n);

  p = &param[TYPE_SUBADD_N];
  p->retval = 1;
  p->dst[0] = 1;
  p->src[0] = 1;
  p->src[1] = 1;
  p->src[2] = 1;
  REFERENCE (refmpn_subadd_n);

  p = &param[TYPE_COPY];
  p->dst[0] = 1;
  p->src[0] = 1;
  p->overlap = OVERLAP_NONE;
  p->size = SIZE_ALLOW_ZERO;
  REFERENCE (refmpn_copy);

  p = &param[TYPE_COPYI];
  p->dst[0] = 1;
  p->src[0] = 1;
  p->overlap = OVERLAP_LOW_TO_HIGH;
  p->size = SIZE_ALLOW_ZERO;
  REFERENCE (refmpn_copyi);

  p = &param[TYPE_COPYD];
  p->dst[0] = 1;
  p->src[0] = 1;
  p->overlap = OVERLAP_HIGH_TO_LOW;
  p->size = SIZE_ALLOW_ZERO;
  REFERENCE (refmpn_copyd);

  p = &param[TYPE_COM_N];
  p->dst[0] = 1;
  p->src[0] = 1;
  REFERENCE (refmpn_com_n);

  p = &param[TYPE_ADDLSH_N];
  p->dst[0]=1;
  p->src[0]=1;
  p->src[1]=1;
  p->shift=1;
  REFERENCE (refmpn_addlsh_n);

  p = &param[TYPE_SUBLSH_N];
  p->dst[0]=1;
  p->src[0]=1;
  p->src[1]=1;
  p->shift=1;
  REFERENCE (refmpn_sublsh_n);

  p = &param[TYPE_ADDLSH_NC];
  p->dst[0]=1;
  p->src[0]=1;
  p->src[1]=1;
  p->shift=1;
  p->carry=CARRY_LIMB;
  REFERENCE (refmpn_addlsh_nc);

  p = &param[TYPE_SUBLSH_NC];
  p->dst[0]=1;
  p->src[0]=1;
  p->src[1]=1;
  p->shift=1;
  p->carry=CARRY_LIMB;
  REFERENCE (refmpn_sublsh_nc);
  
  p = &param[TYPE_INCLSH_N];
  p->dst[0]=1;
  p->src[0]=1;
  p->shift=1;
  REFERENCE (refmpn_inclsh_n);

  p = &param[TYPE_DECLSH_N];
  p->dst[0]=1;
  p->src[0]=1;
  p->shift=1;
  REFERENCE (refmpn_declsh_n);  
  
  p = &param[TYPE_ADDLSH1_N];
  COPY (TYPE_ADD_N);
  REFERENCE (refmpn_addlsh1_n);

  p = &param[TYPE_SUBLSH1_N];
  COPY (TYPE_ADD_N);
  REFERENCE (refmpn_sublsh1_n);

  p = &param[TYPE_RSH1ADD_N];
  COPY (TYPE_ADD_N);
  REFERENCE (refmpn_rsh1add_n);

  p = &param[TYPE_RSH1SUB_N];
  COPY (TYPE_ADD_N);
  REFERENCE (refmpn_rsh1sub_n);


  p = &param[TYPE_MOD_1];
  p->retval = 1;
  p->src[0] = 1;
  p->size = SIZE_ALLOW_ZERO;
  p->divisor = DIVISOR_LIMB;
  REFERENCE (refmpn_mod_1);

  p = &param[TYPE_MOD_1C];
  COPY (TYPE_MOD_1);
  p->carry = CARRY_DIVISOR;
  REFERENCE (refmpn_mod_1c);

  p = &param[TYPE_DIVMOD_1];
  COPY (TYPE_MOD_1);
  p->dst[0] = 1;
  REFERENCE (refmpn_divmod_1);

  p = &param[TYPE_DIVREM_EUCLIDEAN_QR_1];
  p->retval = 1;
  p->src[0] = 1;
  p->divisor = DIVISOR_LIMB;
  p->dst[0] = 1;
  REFERENCE (refmpn_divrem_1);

  p = &param[TYPE_DIVREM_EUCLIDEAN_R_1];
  p->retval = 1;
  p->src[0] = 1;
  p->divisor = DIVISOR_LIMB;
  REFERENCE (refmpn_divrem_euclidean_r_1);

  p = &param[TYPE_DIVREM_HENSEL_QR_1];
  p->retval = 1;
  p->src[0] = 1;
  p->divisor = DIVISOR_ODD;
  p->dst[0] = 1;
  REFERENCE (refmpn_divrem_hensel_qr_1);

  p = &param[TYPE_DIVREM_HENSEL_QR_1_1];
  p->retval = 1;
  p->src[0] = 1;
  p->divisor = DIVISOR_ODD;
  p->dst[0] = 1;
  REFERENCE (refmpn_divrem_hensel_qr_1);

  p = &param[TYPE_DIVREM_HENSEL_QR_1_2];
  p->retval = 1;
  p->src[0] = 1;
  p->divisor = DIVISOR_ODD;
  p->dst[0] = 1;
  REFERENCE (refmpn_divrem_hensel_qr_1);

  p = &param[TYPE_DIVREM_HENSEL_R_1];
  p->retval = 1;
  p->src[0] = 1;
  p->divisor = DIVISOR_ODD;
  REFERENCE (refmpn_divrem_hensel_r_1);

  p = &param[TYPE_DIVREM_HENSEL_RSH_QR_1];
  p->retval = 1;
  p->src[0] = 1;
  p->divisor = DIVISOR_ODD;
  p->dst[0] = 1;
  p->shift=1;
  REFERENCE (refmpn_divrem_hensel_rsh_qr_1);

  p = &param[TYPE_RSH_DIVREM_HENSEL_QR_1];
  p->retval = 1;
  p->src[0] = 1;
  p->divisor = DIVISOR_ODD;
  p->dst[0] = 1;
  p->shift=1;
  p->carry=CARRY_LIMB;
  REFERENCE (refmpn_rsh_divrem_hensel_qr_1);

  p = &param[TYPE_RSH_DIVREM_HENSEL_QR_1_1];
  p->retval = 1;
  p->src[0] = 1;
  p->divisor = DIVISOR_ODD;
  p->dst[0] = 1;
  p->shift=1;
  p->carry=CARRY_LIMB;
  REFERENCE (refmpn_rsh_divrem_hensel_qr_1);

  p = &param[TYPE_RSH_DIVREM_HENSEL_QR_1_2];
  p->retval = 1;
  p->src[0] = 1;
  p->divisor = DIVISOR_ODD;
  p->dst[0] = 1;
  p->shift=1;
  p->carry=CARRY_LIMB;
  REFERENCE (refmpn_rsh_divrem_hensel_qr_1);

  p = &param[TYPE_DIVEXACT_BYFOBM1];
  p->retval = 1;
  p->src[0] = 1;
  p->divisor = DIVISOR_DIVBM1;
  p->dst[0] = 1;
  REFERENCE (refmpn_divexact_byfobm1);

  p = &param[TYPE_DIVMOD_1C];
  COPY (TYPE_DIVMOD_1);
  p->carry = CARRY_DIVISOR;
  REFERENCE (refmpn_divmod_1c);

  p = &param[TYPE_DIVREM_1];
  COPY (TYPE_DIVMOD_1);
  p->size2 = SIZE_FRACTION;
  p->dst_size[0] = SIZE_SUM;
  REFERENCE (refmpn_divrem_1);

  p = &param[TYPE_DIVREM_1C];
  COPY (TYPE_DIVREM_1);
  p->carry = CARRY_DIVISOR;
  REFERENCE (refmpn_divrem_1c);

  p = &param[TYPE_PREINV_DIVREM_1];
  COPY (TYPE_DIVREM_1);
  p->size = SIZE_YES; /* ie. no size==0 */
  REFERENCE (refmpn_preinv_divrem_1);

  p = &param[TYPE_PREINV_MOD_1];
  p->retval = 1;
  p->src[0] = 1;
  p->divisor = DIVISOR_NORM;
  REFERENCE (refmpn_preinv_mod_1);

  p = &param[TYPE_MOD_34LSUB1];
  p->retval = 1;
  p->src[0] = 1;
  VALIDATE (validate_mod_34lsub1);

  p = &param[TYPE_UDIV_QRNND];
  p->retval = 1;
  p->src[0] = 1;
  p->dst[0] = 1;
  p->dst_size[0] = SIZE_1;
  p->divisor = UDIV_NEEDS_NORMALIZATION ? DIVISOR_NORM : DIVISOR_LIMB;
  p->data = DATA_UDIV_QRNND;
  p->overlap = OVERLAP_NONE;
  REFERENCE (refmpn_udiv_qrnnd);

  p = &param[TYPE_UDIV_QRNND_R];
  COPY (TYPE_UDIV_QRNND);
  REFERENCE (refmpn_udiv_qrnnd_r);


  p = &param[TYPE_DIVEXACT_1];
  p->dst[0] = 1;
  p->src[0] = 1;
  p->divisor = DIVISOR_LIMB;
  p->data = DATA_MULTIPLE_DIVISOR;
  VALIDATE (validate_divexact_1);
  REFERENCE (refmpn_divmod_1);


  p = &param[TYPE_DIVEXACT_BY3];
  p->retval = 1;
  p->dst[0] = 1;
  p->src[0] = 1;
  REFERENCE (refmpn_divexact_by3);

  p = &param[TYPE_DIVEXACT_BYFF];
  p->retval = 1;
  p->dst[0] = 1;
  p->src[0] = 1;
  REFERENCE (refmpn_divexact_byff);

  p = &param[TYPE_LSHIFT1];
  p->retval = 1;
  p->dst[0] = 1;
  p->src[0] = 1;
  REFERENCE (refmpn_lshift1);

  p = &param[TYPE_RSHIFT1];
  p->retval = 1;
  p->dst[0] = 1;
  p->src[0] = 1;
  REFERENCE (refmpn_rshift1);

  p = &param[TYPE_LSHIFT2];
  p->retval = 1;
  p->dst[0] = 1;
  p->src[0] = 1;
  REFERENCE (refmpn_lshift2);

  p = &param[TYPE_RSHIFT2];
  p->retval = 1;
  p->dst[0] = 1;
  p->src[0] = 1;
  REFERENCE (refmpn_rshift2);

  p = &param[TYPE_DIVEXACT_BY3C];
  COPY (TYPE_DIVEXACT_BY3);
  p->carry = CARRY_3;
  REFERENCE (refmpn_divexact_by3c);


  p = &param[TYPE_MODEXACT_1_ODD];
  p->retval = 1;
  p->src[0] = 1;
  p->divisor = DIVISOR_ODD;
  VALIDATE (validate_modexact_1_odd);

  p = &param[TYPE_MODEXACT_1C_ODD];
  COPY (TYPE_MODEXACT_1_ODD);
  p->carry = CARRY_LIMB;
  VALIDATE (validate_modexact_1c_odd);


  p = &param[TYPE_GCD_1];
  p->retval = 1;
  p->src[0] = 1;
  p->data = DATA_NON_ZERO;
  p->divisor = DIVISOR_LIMB;
  REFERENCE (refmpn_gcd_1);

  p = &param[TYPE_GCD];
  p->retval = 1;
  p->dst[0] = 1;
  p->src[0] = 1;
  p->src[1] = 1;
  p->size2 = 1;
  p->dst_size[0] = SIZE_RETVAL;
  p->overlap = OVERLAP_NOT_SRCS;
  p->data = DATA_GCD;
  REFERENCE (refmpn_gcd);

  p = &param[TYPE_MPZ_JACOBI];
  p->retval = 1;
  p->src[0] = 1;
  p->size = SIZE_ALLOW_ZERO;
  p->src[1] = 1;
  p->data = DATA_SRC1_ODD;
  p->size2 = 1;
  p->carry = CARRY_4;
  p->carry_sign = 1;
  REFERENCE (refmpz_jacobi);

  p = &param[TYPE_MPZ_KRONECKER];
  COPY (TYPE_MPZ_JACOBI);
  p->data = 0;			/* clear inherited DATA_SRC1_ODD */
  REFERENCE (refmpz_kronecker);


  p = &param[TYPE_MPZ_KRONECKER_UI];
  p->retval = 1;
  p->src[0] = 1;
  p->size = SIZE_ALLOW_ZERO;
  p->multiplier = 1;
  p->carry = CARRY_BIT;
  REFERENCE (refmpz_kronecker_ui);

  p = &param[TYPE_MPZ_KRONECKER_SI];
  COPY (TYPE_MPZ_KRONECKER_UI);
  REFERENCE (refmpz_kronecker_si);

  p = &param[TYPE_MPZ_UI_KRONECKER];
  COPY (TYPE_MPZ_KRONECKER_UI);
  REFERENCE (refmpz_ui_kronecker);

  p = &param[TYPE_MPZ_SI_KRONECKER];
  COPY (TYPE_MPZ_KRONECKER_UI);
  REFERENCE (refmpz_si_kronecker);

  p = &param[TYPE_REDC_BASECASE];
  p->dst[0] = 1;
  p->src[0] = 1;
  p->src[1] = 1;
  p->data = DATA_SRC0_ODD ;
  p->size2 = SIZE_DOUBLE;
  p->overlap = OVERLAP_NONE;
  REFERENCE (refmpn_redc_1);

  p = &param[TYPE_SQR];
  p->dst[0] = 1;
  p->src[0] = 1;
  p->dst_size[0] = SIZE_SUM;
  p->overlap = OVERLAP_NONE;
  REFERENCE (refmpn_sqr);

  p = &param[TYPE_MUL_N];
  COPY (TYPE_SQR);
  p->src[1] = 1;
  REFERENCE (refmpn_mul_n);

  p = &param[TYPE_MULMID_BASECASE];
  COPY (TYPE_MUL_BASECASE);
  p->dst_size[0] = SIZE_DIFF_PLUS_3;
  p->size2 = 1;
  p->overlap = OVERLAP_NONE;
  REFERENCE (refmpn_mulmid_basecase);

  p = &param[TYPE_MULMID];
  COPY (TYPE_MULMID_BASECASE);
  REFERENCE (refmpn_mulmid);

  p = &param[TYPE_MULMID_N];
  p->dst[0] = 1;
  p->src[0] = 1;
  p->src[1] = 1;
  p->dst_size[0] = SIZE_PLUS_2;
  p->size2 = SIZE_DOUBLE_MINUS_1;
  p->overlap = OVERLAP_NONE;
  REFERENCE (refmpn_mulmid_n);

  p = &param[TYPE_MUL_BASECASE];
  COPY (TYPE_MUL_N);
  p->dst_size[0] = SIZE_SUM;
  p->size2 = 1;
  REFERENCE (refmpn_mul_basecase);
  
  p = &param[TYPE_UMUL_PPMM];
  p->retval = 1;
  p->src[0] = 1;
  p->dst[0] = 1;
  p->dst_size[0] = SIZE_1;
  p->overlap = OVERLAP_NONE;
  REFERENCE (refmpn_umul_ppmm);

  p = &param[TYPE_UMUL_PPMM_R];
  COPY (TYPE_UMUL_PPMM);
  REFERENCE (refmpn_umul_ppmm_r);


  p = &param[TYPE_RSHIFT];
  p->retval = 1;
  p->dst[0] = 1;
  p->src[0] = 1;
  p->shift = 1;
  p->overlap = OVERLAP_LOW_TO_HIGH;
  REFERENCE (refmpn_rshift);

  p = &param[TYPE_LSHIFT];
  COPY (TYPE_RSHIFT);
  p->overlap = OVERLAP_HIGH_TO_LOW;
  REFERENCE (refmpn_lshift);

  p = &param[TYPE_LSHIFTC];
  COPY (TYPE_LSHIFT);
  REFERENCE (refmpn_lshiftc);

  p = &param[TYPE_POPCOUNT];
  p->retval = 1;
  p->src[0] = 1;
  REFERENCE (refmpn_popcount);

  p = &param[TYPE_NOT];
  //p->src[0] = 1;
  p->dst[0] = 1;
  REFERENCE (refmpn_not);

  p = &param[TYPE_DOUBLE];
  p->retval = 1;
  p->dst[0] = 1;
  REFERENCE (refmpn_double);

  p = &param[TYPE_HALF];
  p->retval = 1;
  p->dst[0] = 1;
  REFERENCE (refmpn_half);

  p = &param[TYPE_HAMDIST];
  COPY (TYPE_POPCOUNT);
  p->src[1] = 1;
  REFERENCE (refmpn_hamdist);

  p = &param[TYPE_TDIV_QR];
  p->dst[0] = 1;
  p->dst[1] = 1;
  p->src[0] = 1;
  p->src[1] = 1;
  p->size2 = 1;
  p->dst_size[0] = SIZE_DIFF_PLUS_1;
  p->dst_size[1] = SIZE_SIZE2;
  p->overlap = OVERLAP_NONE;
  REFERENCE (refmpn_tdiv_qr);

  p = &param[TYPE_TDIV_Q];
  p->dst[0] = 1;
  p->src[0] = 1;
  p->src[1] = 1;
  p->size2 = 1;
  p->dst_size[0] = SIZE_DIFF_PLUS_1;
  p->overlap = OVERLAP_NONE;
  REFERENCE (refmpn_tdiv_q);

  p = &param[TYPE_SQRTREM];
  p->retval = 1;
  p->dst[0] = 1;
  p->dst[1] = 1;
  p->src[0] = 1;
  p->dst_size[0] = SIZE_CEIL_HALF;
  p->dst_size[1] = SIZE_RETVAL;
  p->overlap = OVERLAP_NONE;
  VALIDATE (validate_sqrtrem);
  REFERENCE (refmpn_sqrtrem);

  p = &param[TYPE_ZERO];
  p->dst[0] = 1;
  p->size = SIZE_ALLOW_ZERO;
  REFERENCE (refmpn_zero);

  p = &param[TYPE_STORE];
  p->dst[0] = 1;
  p->size = SIZE_ALLOW_ZERO;
  REFERENCE (refmpn_store);

  p = &param[TYPE_GET_STR];
  p->retval = 1;
  p->src[0] = 1;
  p->size = SIZE_ALLOW_ZERO;
  p->dst[0] = 1;
  p->dst[1] = 1;
  p->dst_size[0] = SIZE_GET_STR;
  p->dst_bytes[0] = 1;
  p->overlap = OVERLAP_NONE;
  REFERENCE (refmpn_get_str);

#ifdef EXTRA_PARAM_INIT
  EXTRA_PARAM_INIT
#endif
}


/* The following are macros if there's no native versions, so wrap them in
   functions that can be in try_array[]. */

void
MPN_COPY_fun (mp_ptr rp, mp_srcptr sp, mp_size_t size)
{ MPN_COPY (rp, sp, size); }

void
MPN_COPY_INCR_fun (mp_ptr rp, mp_srcptr sp, mp_size_t size)
{ MPN_COPY_INCR (rp, sp, size); }

void
MPN_COPY_DECR_fun (mp_ptr rp, mp_srcptr sp, mp_size_t size)
{ MPN_COPY_DECR (rp, sp, size); }

void
__GMPN_COPY_fun (mp_ptr rp, mp_srcptr sp, mp_size_t size)
{ __GMPN_COPY (rp, sp, size); }

#ifdef __GMPN_COPY_INCR
void
__GMPN_COPY_INCR_fun (mp_ptr rp, mp_srcptr sp, mp_size_t size)
{ __GMPN_COPY_INCR (rp, sp, size); }
#endif

void
mpn_com_n_fun (mp_ptr rp, mp_srcptr sp, mp_size_t size)
{ mpn_com_n (rp, sp, size); }

void
mpn_and_n_fun (mp_ptr rp, mp_srcptr s1, mp_srcptr s2, mp_size_t size)
{ mpn_and_n (rp, s1, s2, size); }

void
mpn_andn_n_fun (mp_ptr rp, mp_srcptr s1, mp_srcptr s2, mp_size_t size)
{ mpn_andn_n (rp, s1, s2, size); }

void
mpn_nand_n_fun (mp_ptr rp, mp_srcptr s1, mp_srcptr s2, mp_size_t size)
{ mpn_nand_n (rp, s1, s2, size); }

void
mpn_ior_n_fun (mp_ptr rp, mp_srcptr s1, mp_srcptr s2, mp_size_t size)
{ mpn_ior_n (rp, s1, s2, size); }

void
mpn_iorn_n_fun (mp_ptr rp, mp_srcptr s1, mp_srcptr s2, mp_size_t size)
{ mpn_iorn_n (rp, s1, s2, size); }

void
mpn_nior_n_fun (mp_ptr rp, mp_srcptr s1, mp_srcptr s2, mp_size_t size)
{ mpn_nior_n (rp, s1, s2, size); }

void
mpn_xor_n_fun (mp_ptr rp, mp_srcptr s1, mp_srcptr s2, mp_size_t size)
{ mpn_xor_n (rp, s1, s2, size); }

void
mpn_xnor_n_fun (mp_ptr rp, mp_srcptr s1, mp_srcptr s2, mp_size_t size)
{ mpn_xnor_n (rp, s1, s2, size); }

mp_limb_t
udiv_qrnnd_fun (mp_limb_t *remptr, mp_limb_t n1, mp_limb_t n0, mp_limb_t d)
{
  mp_limb_t  q;
  udiv_qrnnd (q, *remptr, n1, n0, d);
  return q;
}

mp_limb_t
mpn_divexact_by3_fun (mp_ptr rp, mp_srcptr sp, mp_size_t size)
{
  return mpn_divexact_by3 (rp, sp, size);
}

void
mpn_not_fun (mp_ptr rp, mp_size_t size)
{
  mpn_not (rp, size);
}

mp_limb_t
mpn_double_fun (mp_ptr rp, mp_size_t size)
{
  return mpn_double (rp, size);
}

mp_limb_t
mpn_half_fun (mp_ptr rp, mp_size_t size)
{
  return mpn_half (rp, size);
}

mp_limb_t
mpn_lshift1_fun (mp_ptr rp, mp_srcptr sp, mp_size_t size)
{
  return mpn_lshift1 (rp, sp, size);
}

mp_limb_t
mpn_rshift1_fun (mp_ptr rp, mp_srcptr sp, mp_size_t size)
{
  return mpn_rshift1 (rp, sp, size);
}

mp_limb_t
mpn_lshift2_fun (mp_ptr rp, mp_srcptr sp, mp_size_t size)
{
  return mpn_lshift2 (rp, sp, size);
}

mp_limb_t
mpn_rshift2_fun (mp_ptr rp, mp_srcptr sp, mp_size_t size)
{
  return mpn_rshift2 (rp, sp, size);
}

#if HAVE_NATIVE_mpn_addlsh1_n
mp_limb_t
mpn_addlsh1_n_fun (mp_ptr rp, mp_srcptr sp, mp_srcptr sp1,mp_size_t size)
{
  return mpn_addlsh1_n (rp, sp,sp1, size);
}
#endif

#if HAVE_NATIVE_mpn_sublsh1_n
mp_limb_t
mpn_sublsh1_n_fun (mp_ptr rp, mp_srcptr sp, mp_srcptr sp1,mp_size_t size)
{
  return mpn_sublsh1_n (rp, sp,sp1, size);
}
#endif

#if HAVE_NATIVE_mpn_inclsh_n
mp_limb_t
mpn_inclsh_n_fun (mp_ptr rp, mp_srcptr sp,mp_size_t size,unsigned int c)
{
  return mpn_inclsh_n (rp, sp, size,c);
}
#endif

#if HAVE_NATIVE_mpn_declsh_n
mp_limb_t
mpn_declsh_n_fun (mp_ptr rp, mp_srcptr sp,mp_size_t size,unsigned int c)
{
  return mpn_declsh_n (rp, sp, size,c);
}
#endif

mp_limb_t
mpn_modexact_1_odd_fun (mp_srcptr ptr, mp_size_t size, mp_limb_t divisor)
{
  return mpn_modexact_1_odd (ptr, size, divisor);
}

void
mpn_kara_mul_n_fun (mp_ptr dst, mp_srcptr src1, mp_srcptr src2, mp_size_t size)
{
  mp_ptr  tspace;
  TMP_DECL;
  TMP_MARK;
  tspace = TMP_ALLOC_LIMBS (MPN_KARA_MUL_N_TSIZE (size));
  mpn_kara_mul_n (dst, src1, src2, size, tspace);
}
void
mpn_kara_sqr_n_fun (mp_ptr dst, mp_srcptr src, mp_size_t size)
{
  mp_ptr tspace;
  TMP_DECL;
  TMP_MARK;
  tspace = TMP_ALLOC_LIMBS (MPN_KARA_SQR_N_TSIZE (size));
  mpn_kara_sqr_n (dst, src, size, tspace);
  TMP_FREE;
}
void
mpn_toom3_mul_n_fun (mp_ptr dst, mp_srcptr src1, mp_srcptr src2, mp_size_t size)
{
  mp_ptr  tspace;
  TMP_DECL;
  TMP_MARK;
  tspace = TMP_ALLOC_LIMBS (MPN_TOOM3_MUL_N_TSIZE (size));
  mpn_toom3_mul_n (dst, src1, src2, size, tspace);
  TMP_FREE;
}
void
mpn_toom3_sqr_n_fun (mp_ptr dst, mp_srcptr src1, mp_size_t size)
{
  mp_ptr  tspace;
  TMP_DECL;
  TMP_MARK;
  tspace = TMP_ALLOC_LIMBS (MPN_TOOM3_SQR_N_TSIZE (size));
  mpn_toom3_sqr_n (dst, src1, size, tspace);
  TMP_FREE;
}
void
mpn_toom4_mul_n_fun (mp_ptr dst, mp_srcptr src1, mp_srcptr src2, mp_size_t size)
{
  mpn_toom4_mul_n (dst, src1, src2, size);
}
void
mpn_toom4_sqr_n_fun (mp_ptr dst, mp_srcptr src1, mp_size_t size)
{
  mpn_toom4_sqr_n (dst, src1, size);
}
void
mpn_toom8h_mul_fun (mp_ptr dst, mp_srcptr src1, mp_size_t size1, mp_srcptr src2, mp_size_t size2)
{
  mpn_toom8h_mul (dst, src1, size1, src2, size2);
}
void
mpn_toom8_sqr_n_fun (mp_ptr dst, mp_srcptr src1, mp_size_t size)
{
  mpn_toom8_sqr_n (dst, src1, size);
}

mp_limb_t
umul_ppmm_fun (mp_limb_t *lowptr, mp_limb_t m1, mp_limb_t m2)
{
  mp_limb_t  high;
  umul_ppmm (high, *lowptr, m1, m2);
  return high;
}

void
MPN_ZERO_fun (mp_ptr ptr, mp_size_t size)
{ MPN_ZERO (ptr, size); }

void
mpn_store_fun (mp_ptr ptr, mp_size_t size,mp_limb_t val)
{ mpn_store (ptr, size,val); }

struct choice_t {
  const char  *name;
  tryfun_t    function;
  int         type;
  mp_size_t   minsize;
};

#if HAVE_STRINGIZE
#define TRY(fun)        #fun, (tryfun_t) fun
#define TRY_FUNFUN(fun) #fun, (tryfun_t) fun##_fun
#else
#define TRY(fun)        "fun", (tryfun_t) fun
#define TRY_FUNFUN(fun) "fun", (tryfun_t) fun/**/_fun
#endif

const struct choice_t choice_array[] = {
  { TRY(mpn_add),       TYPE_ADD    },
  { TRY(mpn_sub),       TYPE_SUB    },

  { TRY(mpn_add_n),     TYPE_ADD_N  },
  { TRY(mpn_sub_n),     TYPE_SUB_N  },

#if HAVE_NATIVE_mpn_add_nc
  { TRY(mpn_add_nc),    TYPE_ADD_NC },
#endif
#if HAVE_NATIVE_mpn_sub_nc
  { TRY(mpn_sub_nc),    TYPE_SUB_NC },
#endif

  { TRY(mpn_sumdiff_n),  TYPE_SUMDIFF_N  },
#if HAVE_NATIVE_mpn_nsumdiff_n
  { TRY(mpn_nsumdiff_n),  TYPE_NSUMDIFF_N  },
#endif
#if HAVE_NATIVE_mpn_sumdiff_nc
  { TRY(mpn_sumdiff_nc), TYPE_SUMDIFF_NC },
#endif

  { TRY(mpn_addadd_n),  TYPE_ADDADD_N  },
  { TRY(mpn_addsub_n),  TYPE_ADDSUB_N  },
  { TRY(mpn_subadd_n),  TYPE_SUBADD_N  },

  { TRY(mpn_addmul_1),  TYPE_ADDMUL_1  },
  { TRY(mpn_submul_1),  TYPE_SUBMUL_1  },
#if HAVE_NATIVE_mpn_addmul_1c
  { TRY(mpn_addmul_1c), TYPE_ADDMUL_1C },
#endif
#if HAVE_NATIVE_mpn_submul_1c
  { TRY(mpn_submul_1c), TYPE_SUBMUL_1C },
#endif

#if HAVE_NATIVE_mpn_addmul_2
  { TRY(mpn_addmul_2), TYPE_ADDMUL_2, 2 },
#endif
#if HAVE_NATIVE_mpn_addmul_3
  { TRY(mpn_addmul_3), TYPE_ADDMUL_3, 3 },
#endif
#if HAVE_NATIVE_mpn_addmul_4
  { TRY(mpn_addmul_4), TYPE_ADDMUL_4, 4 },
#endif
#if HAVE_NATIVE_mpn_addmul_5
  { TRY(mpn_addmul_5), TYPE_ADDMUL_5, 5 },
#endif
#if HAVE_NATIVE_mpn_addmul_6
  { TRY(mpn_addmul_6), TYPE_ADDMUL_6, 6 },
#endif
#if HAVE_NATIVE_mpn_addmul_7
  { TRY(mpn_addmul_7), TYPE_ADDMUL_7, 7 },
#endif
#if HAVE_NATIVE_mpn_addmul_8
  { TRY(mpn_addmul_8), TYPE_ADDMUL_8, 8 },
#endif

  { TRY_FUNFUN(mpn_com_n),  TYPE_COM_N },

  { TRY_FUNFUN(MPN_COPY),      TYPE_COPY },
  { TRY_FUNFUN(MPN_COPY_INCR), TYPE_COPYI },
  { TRY_FUNFUN(MPN_COPY_DECR), TYPE_COPYD },

  { TRY_FUNFUN(__GMPN_COPY),      TYPE_COPY },
#ifdef __GMPN_COPY_INCR
  { TRY_FUNFUN(__GMPN_COPY_INCR), TYPE_COPYI },
#endif

#if HAVE_NATIVE_mpn_copyi
  { TRY(mpn_copyi), TYPE_COPYI },
#endif
#if HAVE_NATIVE_mpn_copyd
  { TRY(mpn_copyd), TYPE_COPYD },
#endif

#if HAVE_NATIVE_mpn_addlsh1_n
  { TRY_FUNFUN(mpn_addlsh1_n), TYPE_ADDLSH1_N },
#endif
#if HAVE_NATIVE_mpn_sublsh1_n
  { TRY_FUNFUN(mpn_sublsh1_n), TYPE_SUBLSH1_N },
#endif
#if HAVE_NATIVE_mpn_addlsh_n
  { TRY(mpn_addlsh_n), TYPE_ADDLSH_N },
#endif
#if HAVE_NATIVE_mpn_sublsh_n
  { TRY(mpn_sublsh_n), TYPE_SUBLSH_N },
#endif
#if HAVE_NATIVE_mpn_addlsh_nc
  { TRY(mpn_addlsh_nc), TYPE_ADDLSH_NC },
#endif
#if HAVE_NATIVE_mpn_sublsh_nc
  { TRY(mpn_sublsh_nc), TYPE_SUBLSH_NC },
#endif
#if HAVE_NATIVE_mpn_inclsh_n
  { TRY_FUNFUN(mpn_inclsh_n), TYPE_INCLSH_N },
#endif
#if HAVE_NATIVE_mpn_declsh_n
  { TRY_FUNFUN(mpn_declsh_n), TYPE_DECLSH_N },
#endif

#if HAVE_NATIVE_mpn_rsh1add_n
  { TRY(mpn_rsh1add_n), TYPE_RSH1ADD_N },
#endif
#if HAVE_NATIVE_mpn_rsh1sub_n
  { TRY(mpn_rsh1sub_n), TYPE_RSH1SUB_N },
#endif

  { TRY_FUNFUN(mpn_and_n),  TYPE_AND_N  },
  { TRY_FUNFUN(mpn_andn_n), TYPE_ANDN_N },
  { TRY_FUNFUN(mpn_nand_n), TYPE_NAND_N },
  { TRY_FUNFUN(mpn_ior_n),  TYPE_IOR_N  },
  { TRY_FUNFUN(mpn_iorn_n), TYPE_IORN_N },
  { TRY_FUNFUN(mpn_nior_n), TYPE_NIOR_N },
  { TRY_FUNFUN(mpn_xor_n),  TYPE_XOR_N  },
  { TRY_FUNFUN(mpn_xnor_n), TYPE_XNOR_N },

  { TRY(mpn_divrem_1),     TYPE_DIVREM_1 },
  { TRY(mpn_divrem_euclidean_qr_1),     TYPE_DIVREM_EUCLIDEAN_QR_1 },
  { TRY(mpn_divrem_euclidean_r_1),     TYPE_DIVREM_EUCLIDEAN_R_1 },
  { TRY(mpn_divrem_hensel_qr_1),     TYPE_DIVREM_HENSEL_QR_1 },
  { TRY(mpn_divrem_hensel_qr_1_1),     TYPE_DIVREM_HENSEL_QR_1_1 },
  { TRY(mpn_divrem_hensel_qr_1_2),     TYPE_DIVREM_HENSEL_QR_1_2 ,2},
  { TRY(mpn_divrem_hensel_r_1),     TYPE_DIVREM_HENSEL_R_1 },
  { TRY(mpn_rsh_divrem_hensel_qr_1),     TYPE_RSH_DIVREM_HENSEL_QR_1 },
  { TRY(mpn_rsh_divrem_hensel_qr_1_1),     TYPE_RSH_DIVREM_HENSEL_QR_1_1 },
  { TRY(mpn_rsh_divrem_hensel_qr_1_2),     TYPE_RSH_DIVREM_HENSEL_QR_1_2 ,3},
  
  { TRY(mpn_divrem_hensel_rsh_qr_1),     TYPE_DIVREM_HENSEL_RSH_QR_1 },

  { TRY(mpn_add_err1_n),	TYPE_ADDERR1_N},  
  { TRY(mpn_sub_err1_n),	TYPE_SUBERR1_N},
  { TRY(mpn_add_err2_n),	TYPE_ADDERR2_N},  
  { TRY(mpn_sub_err2_n),	TYPE_SUBERR2_N},
#if USE_PREINV_DIVREM_1
  { TRY(mpn_preinv_divrem_1), TYPE_PREINV_DIVREM_1 },
#endif
  { TRY(mpn_mod_1),        TYPE_MOD_1 },
#if USE_PREINV_MOD_1
  { TRY(mpn_preinv_mod_1), TYPE_PREINV_MOD_1 },
#endif
#if HAVE_NATIVE_mpn_divrem_1c
  { TRY(mpn_divrem_1c),    TYPE_DIVREM_1C },
#endif
#if HAVE_NATIVE_mpn_mod_1c
  { TRY(mpn_mod_1c),       TYPE_MOD_1C },
#endif
#if GMP_NUMB_BITS % 4 == 0
  { TRY(mpn_mod_34lsub1),  TYPE_MOD_34LSUB1 },
#endif

  { TRY_FUNFUN(udiv_qrnnd), TYPE_UDIV_QRNND, 2 },
#if HAVE_NATIVE_mpn_udiv_qrnnd
  { TRY(mpn_udiv_qrnnd),    TYPE_UDIV_QRNND, 2 },
#endif
#if HAVE_NATIVE_mpn_udiv_qrnnd_r
  { TRY(mpn_udiv_qrnnd_r),  TYPE_UDIV_QRNND_R, 2 },
#endif

  { TRY(mpn_divexact_1),          TYPE_DIVEXACT_1 },
  { TRY_FUNFUN(mpn_divexact_by3), TYPE_DIVEXACT_BY3 },
  { TRY(mpn_divexact_byff),       TYPE_DIVEXACT_BYFF },
  { TRY(mpn_divexact_byfobm1),    TYPE_DIVEXACT_BYFOBM1 },
  
  { TRY_FUNFUN(mpn_lshift1),	  TYPE_LSHIFT1 },
  { TRY_FUNFUN(mpn_rshift1),	  TYPE_RSHIFT1 },
  { TRY_FUNFUN(mpn_lshift2),	  TYPE_LSHIFT2 },
  { TRY_FUNFUN(mpn_rshift2),	  TYPE_RSHIFT2 },
  { TRY(mpn_divexact_by3c),       TYPE_DIVEXACT_BY3C },

  { TRY_FUNFUN(mpn_modexact_1_odd), TYPE_MODEXACT_1_ODD },
  { TRY(mpn_modexact_1c_odd),       TYPE_MODEXACT_1C_ODD },

  { TRY(mpn_tdiv_qr),      TYPE_TDIV_QR },
  { TRY(mpn_tdiv_q),      TYPE_TDIV_Q },

  { TRY(mpn_mul_1),      TYPE_MUL_1 },
#if HAVE_NATIVE_mpn_mul_1c
  { TRY(mpn_mul_1c),     TYPE_MUL_1C },
#endif
#if HAVE_NATIVE_mpn_mul_2
  { TRY(mpn_mul_2),      TYPE_MUL_2, 2 },
#endif

  { TRY(mpn_rshift),     TYPE_RSHIFT },
  { TRY(mpn_lshift),     TYPE_LSHIFT },
#if HAVE_NATIVE_mpn_lshiftc
  { TRY(mpn_lshiftc),     TYPE_LSHIFTC },
#endif

  { TRY(mpn_mul_basecase), TYPE_MUL_BASECASE },
  { TRY(mpn_redc_1), TYPE_REDC_BASECASE },
#if SQR_KARATSUBA_THRESHOLD > 0
  { TRY(mpn_sqr_basecase), TYPE_SQR },
#endif

  { TRY(mpn_mul),    TYPE_MUL_BASECASE },
  { TRY(mpn_mul_n),  TYPE_MUL_N },
  { TRY(mpn_sqr),  TYPE_SQR },
  { TRY(mpn_mulmid_basecase), TYPE_MULMID_BASECASE },
  { TRY(mpn_mulmid), TYPE_MULMID },
  { TRY(mpn_mulmid_n), TYPE_MULMID_N },
  
  { TRY_FUNFUN(umul_ppmm), TYPE_UMUL_PPMM, 2 },
#if HAVE_NATIVE_mpn_umul_ppmm
  { TRY(mpn_umul_ppmm),    TYPE_UMUL_PPMM, 2 },
#endif
#if HAVE_NATIVE_mpn_umul_ppmm_r
  { TRY(mpn_umul_ppmm_r),  TYPE_UMUL_PPMM_R, 2 },
#endif

  { TRY_FUNFUN(mpn_kara_mul_n),  TYPE_MUL_N, MPN_KARA_MUL_N_MINSIZE },
  { TRY_FUNFUN(mpn_kara_sqr_n),  TYPE_SQR,   MPN_KARA_SQR_N_MINSIZE },
  { TRY_FUNFUN(mpn_toom3_mul_n), TYPE_MUL_N, MPN_TOOM3_MUL_N_MINSIZE },
  { TRY_FUNFUN(mpn_toom4_mul_n), TYPE_MUL_N, MPN_TOOM4_MUL_N_MINSIZE },
  { TRY_FUNFUN(mpn_toom8h_mul), TYPE_MUL_BASECASE, MPN_TOOM8H_MUL_MINSIZE },
  { TRY_FUNFUN(mpn_toom3_sqr_n), TYPE_SQR,   MPN_TOOM3_SQR_N_MINSIZE },
  { TRY_FUNFUN(mpn_toom4_sqr_n), TYPE_SQR,   MPN_TOOM4_SQR_N_MINSIZE },
  { TRY_FUNFUN(mpn_toom8_sqr_n), TYPE_SQR,   MPN_TOOM8_SQR_N_MINSIZE },

  { TRY(mpn_gcd_1),        TYPE_GCD_1            },
  { TRY(mpn_gcd),          TYPE_GCD              },
  { TRY(mpz_jacobi),       TYPE_MPZ_JACOBI       },
  { TRY(mpz_kronecker_ui), TYPE_MPZ_KRONECKER_UI },
  { TRY(mpz_kronecker_si), TYPE_MPZ_KRONECKER_SI },
  { TRY(mpz_ui_kronecker), TYPE_MPZ_UI_KRONECKER },
  { TRY(mpz_si_kronecker), TYPE_MPZ_SI_KRONECKER },

  { TRY_FUNFUN(mpn_not),   TYPE_NOT },
  { TRY_FUNFUN(mpn_double),   TYPE_DOUBLE },
  { TRY_FUNFUN(mpn_half),   TYPE_HALF },
  
  { TRY(mpn_popcount),   TYPE_POPCOUNT },
  { TRY(mpn_hamdist),    TYPE_HAMDIST },

  { TRY(mpn_sqrtrem),    TYPE_SQRTREM },

  { TRY_FUNFUN(MPN_ZERO), TYPE_ZERO },
  { TRY_FUNFUN(mpn_store), TYPE_STORE },

  { TRY(mpn_get_str),    TYPE_GET_STR },

#ifdef EXTRA_ROUTINES
  EXTRA_ROUTINES
#endif
};

const struct choice_t *choice = NULL;


void
mprotect_maybe (void *addr, size_t len, int prot)
{
  if (!option_redzones)
    return;

#if HAVE_MPROTECT
  if (mprotect (addr, len, prot) != 0)
    {
      fprintf (stderr, "Cannot mprotect %p 0x%X 0x%X: %s\n",
	       addr, len, prot, strerror (errno));
      exit (1);
    }
#else
  {
    static int  warned = 0;
    if (!warned)
      {
	fprintf (stderr,
		 "mprotect not available, bounds testing not performed\n");
	warned = 1;
      }
  }
#endif
}

/* round "a" up to a multiple of "m" */
size_t
round_up_multiple (size_t a, size_t m)
{
  unsigned long  r;

  r = a % m;
  if (r == 0)
    return a;
  else
    return a + (m - r);
}


/* On some systems it seems that only an mmap'ed region can be mprotect'ed,
   for instance HP-UX 10.

   mmap will almost certainly return a pointer already aligned to a page
   boundary, but it's easy enough to share the alignment handling with the
   malloc case. */

void
malloc_region (struct region_t *r, mp_size_t n)
{
  mp_ptr  p;
  size_t  nbytes;

  ASSERT ((pagesize % BYTES_PER_MP_LIMB) == 0);

  n = round_up_multiple (n, PAGESIZE_LIMBS);
  r->size = n;

  nbytes = n*BYTES_PER_MP_LIMB + 2*REDZONE_BYTES + pagesize;

#if defined (MAP_ANONYMOUS) && ! defined (MAP_ANON)
#define MAP_ANON  MAP_ANONYMOUS
#endif

#if HAVE_MMAP && defined (MAP_ANON)
  /* note must pass fd=-1 for MAP_ANON on BSD */
  p = (mp_ptr)mmap (NULL, nbytes, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, -1, 0);
  if (p == (void *) -1)
    {
      fprintf (stderr, "Cannot mmap %#x anon bytes: %s\n",
	       nbytes, strerror (errno));
      exit (1);
    }
#else
  p = (mp_ptr) malloc (nbytes);
  ASSERT_ALWAYS (p != NULL);
#endif

  p = align_pointer (p, pagesize);

  mprotect_maybe (p, REDZONE_BYTES, PROT_NONE);
  p += REDZONE_LIMBS;
  r->ptr = p;

  mprotect_maybe (p + n, REDZONE_BYTES, PROT_NONE);
}

void
mprotect_region (const struct region_t *r, int prot)
{
  mprotect_maybe (r->ptr, r->size, prot);
}


/* First four entries must be 0,1,2,3 for the benefit of CARRY_BIT, CARRY_3,
   and CARRY_4 */
mp_limb_t  carry_array[] = {
  0, 1, 2, 3,
  4,
  CNST_LIMB(1) << 8,
  CNST_LIMB(1) << 16,
  GMP_NUMB_MAX
};
int        carry_index;

#define CARRY_COUNT                                             \
  ((tr->carry == CARRY_BIT) ? 2                                 \
   : tr->carry == CARRY_3   ? 3                                 \
   : tr->carry == CARRY_4   ? 4                                 \
   : (tr->carry == CARRY_LIMB || tr->carry == CARRY_DIVISOR)    \
     ? numberof(carry_array) + CARRY_RANDOMS                    \
   : 1)

#define MPN_RANDOM_ALT(index,dst,size) \
  (((index) & 1) ? refmpn_random (dst, size) : refmpn_random2 (dst, size))

/* The dummy value after MPN_RANDOM_ALT ensures both sides of the ":" have
   the same type */
#define CARRY_ITERATION                                                 \
  for (carry_index = 0;                                                 \
       (carry_index < numberof (carry_array)                            \
	? (carry = carry_array[carry_index])                            \
	: (MPN_RANDOM_ALT (carry_index, &carry, 1), (mp_limb_t) 0)),    \
	 (tr->carry == CARRY_DIVISOR ? carry %= divisor : 0),           \
	 carry_index < CARRY_COUNT;                                     \
       carry_index++)


mp_limb_t  multiplier_array[] = {
  0, 1, 2, 3,
  CNST_LIMB(1) << 8,
  CNST_LIMB(1) << 16,
  GMP_NUMB_MAX - 2,
  GMP_NUMB_MAX - 1,
  GMP_NUMB_MAX
};
int        multiplier_index;

mp_limb_t  divisor_array[] = {
  1, 2, 3,
  CNST_LIMB(1) << 8,
  CNST_LIMB(1) << 16,
  CNST_LIMB(1) << (GMP_NUMB_BITS/2 - 1),
  GMP_NUMB_MAX >> (GMP_NUMB_BITS/2),
  GMP_NUMB_HIGHBIT,
  GMP_NUMB_HIGHBIT + 1,
  GMP_NUMB_MAX - 2,
  GMP_NUMB_MAX - 1,
  GMP_NUMB_MAX
};

int        divisor_index;

mp_limb_t  altdiv_array[]={1,3,5,15,17,51,85,255,65535,
GMP_NUMB_MAX/1,GMP_NUMB_MAX/3,GMP_NUMB_MAX/5,GMP_NUMB_MAX/15,
GMP_NUMB_MAX/17,GMP_NUMB_MAX/51,GMP_NUMB_MAX/85,GMP_NUMB_MAX/255,GMP_NUMB_MAX/65535};

int 	altdiv_index;

/* The dummy value after MPN_RANDOM_ALT ensures both sides of the ":" have
   the same type */
#define ARRAY_ITERATION(var, index, limit, array, randoms, cond)        \
  for (index = 0;                                                       \
       (index < numberof (array)                                        \
	? (var = array[index])                                          \
	: (MPN_RANDOM_ALT (index, &var, 1), (mp_limb_t) 0)),            \
       index < limit;                                                   \
       index++)

#define MULTIPLIER_COUNT                                \
  (tr->multiplier                                       \
    ? numberof (multiplier_array) + MULTIPLIER_RANDOMS  \
    : 1)

#define MULTIPLIER_ITERATION                                            \
  ARRAY_ITERATION(multiplier, multiplier_index, MULTIPLIER_COUNT,       \
		  multiplier_array, MULTIPLIER_RANDOMS, TRY_MULTIPLIER)

#define DIVISOR_COUNT                           \
  (tr->divisor == 0 ? 1 :			\
   tr->divisor == DIVISOR_DIVBM1 		\
   ? 1			 			\
   : numberof (divisor_array) + DIVISOR_RANDOMS )

#define ALTDIV_COUNT                           \
  (tr->divisor == 0 ? 1 :			\
   tr->divisor == DIVISOR_DIVBM1 		\
   ? numberof (divisor_array) : 1 )

#define DIVISOR_ITERATION                                               \
  ARRAY_ITERATION(divisor, divisor_index, DIVISOR_COUNT,divisor_array , \
		  DIVISOR_RANDOMS, TRY_DIVISOR)


#define ALTDIV_ITERATION                                        \
  ARRAY_ITERATION(altdiv, altdiv_index, ALTDIV_COUNT, altdiv_array, \
		  0 , TRY_DIVISOR)


/* overlap_array[].s[i] is where s[i] should be, 0 or 1 means overlapping
   d[0] or d[1] respectively, -1 means a separate (write-protected)
   location. */

struct overlap_t {
  int  s[NUM_SOURCES];
} overlap_array[] = {
  { { -1, -1, -1 } },
  { {  0, -1, -1 } },
  { { -1,  0, -1 } },
  { {  0,  0, -1 } },
  { {  1, -1, -1 } },
  { { -1,  1, -1 } },
  { {  1,  1, -1 } },
  { {  0,  1, -1 } },
  { {  1,  0, -1 } },
  { { -1, -1, 0 } },
  { {  0, -1, 0 } },
  { { -1,  0, 0 } },
  { {  0,  0, 0 } },
  { {  1, -1, 0 } },
  { { -1,  1, 0 } },
  { {  1,  1, 0 } },
  { {  0,  1, 0 } },
  { {  1,  0, 0 } },
  { { -1, -1, 1 } },
  { {  0, -1, 1 } },
  { { -1,  0, 1 } },
  { {  0,  0, 1 } },
  { {  1, -1, 1 } },
  { { -1,  1, 1 } },
  { {  1,  1, 1 } },
  { {  0,  1, 1 } },
  { {  1,  0, 1 } },
};

struct overlap_t  *overlap, *overlap_limit;

/* 
   This is a count of the number of overlaps from the above table to try. 
   Each source operand can be overlapped with each destination operand (which 
	are fixed and cannot be overlapped) or put in a non-overlapping block all
	to itself. Some functions require that source operands don't overlap. They
	can't go beyond the first three entries of the table, as after that, this 
	starts to happen.

	Three source operands are available, but only those which are used by the
	function are actually filled with data and made part of the test. The rest
	are ignored.
*/

#define OVERLAP_COUNT                   \
  (tr->overlap & OVERLAP_NONE       ? 1 \
   : tr->overlap & OVERLAP_NOT_SRCS ? 3 \
   : tr->overlap & OVERLAP_NOT_SRC2 ? 2 \
   : tr->dst[1]                     ? 9 \
   : tr->src[2]                     ? 27 \
   : tr->src[1]                     ? 4 \
   : tr->dst[0]                     ? 2 \
   : 1)

#define OVERLAP_ITERATION                               \
  for (overlap = &overlap_array[0],                     \
    overlap_limit = &overlap_array[OVERLAP_COUNT];      \
    overlap < overlap_limit;                            \
    overlap++)


int  base = 10;

#define T_RAND_COUNT  2
int  t_rand;

void
t_random (mp_ptr ptr, mp_size_t n)
{
  if (n == 0)
    return;

  switch (option_data) {
  case DATA_TRAND:
    switch (t_rand) {
    case 0: refmpn_random (ptr, n); break;
    case 1: refmpn_random2 (ptr, n); break;
    default: abort();
    }
    break;
  case DATA_SEQ:
    {
      static mp_limb_t  counter = 0;
      mp_size_t  i;
      for (i = 0; i < n; i++)
	ptr[i] = ++counter;
    }
    break;
  case DATA_ZEROS:
    refmpn_zero (ptr, n);
    break;
  case DATA_FFS:
    refmpn_fill (ptr, n, GMP_NUMB_MAX);
    break;
  case DATA_2FD:
    /* Special value 0x2FFF...FFFD, which divided by 3 gives 0xFFF...FFF,
       inducing the q1_ff special case in the mul-by-inverse part of some
       versions of divrem_1 and mod_1. */
    refmpn_fill (ptr, n, (mp_limb_t) -1);
    ptr[n-1] = 2;
    ptr[0] -= 2;
    break;

  default:
    abort();
  }
}
#define T_RAND_ITERATION \
  for (t_rand = 0; t_rand < T_RAND_COUNT; t_rand++)


void
print_each (const struct each_t *e)
{
  int  i;

  printf ("%s %s\n", e->name, e == &ref ? tr->reference_name : choice->name);
  if (tr->retval)
    mpn_trace ("   retval", &e->retval, 1);

  for (i = 0; i < NUM_DESTS; i++)
    {
      if (tr->dst[i])
	{
	  if (tr->dst_bytes[i])
	    byte_tracen ("   d[%d]", i, e->d[i].p, d[i].size);
	  else
	    mpn_tracen ("   d[%d]", i, e->d[i].p, d[i].size);
	  printf ("        located %p\n", e->d[i].p);
	}
    }

  for (i = 0; i < NUM_SOURCES; i++)
    if (tr->src[i])
      printf ("   s[%d] located %p\n", i, e->s[i].p);
}


void
print_all (void)
{
  int  i;

  printf ("\n");
  printf ("size  %ld\n", (long) size);
  if (tr->size2)
    printf ("size2 %ld\n", (long) size2);

  for (i = 0; i < NUM_DESTS; i++)
    if (d[i].size != size)
      printf ("d[%d].size %ld\n", i, (long) d[i].size);

  if (tr->multiplier)
    mpn_trace ("   multiplier", &multiplier, 1);
  if (tr->divisor)
    mpn_trace ("   divisor", &divisor, 1);
  if (tr->shift)
    printf ("   shift %lu\n", shift);
  if (tr->carry)
    mpn_trace ("   carry", &carry, 1);
  if (tr->msize)
    mpn_trace ("   multiplier_N", multiplier_N, tr->msize);

  for (i = 0; i < NUM_DESTS; i++)
    if (tr->dst[i])
      printf ("   d[%d] %s, align %ld, size %ld\n",
	      i, d[i].high ? "high" : "low",
	      (long) d[i].align, (long) d[i].size);

  for (i = 0; i < NUM_SOURCES; i++)
    {
      if (tr->src[i])
	{
	  printf ("   s[%d] %s, align %ld, ",
		  i, s[i].high ? "high" : "low", (long) s[i].align);
	  switch (overlap->s[i]) {
	  case -1:
	    printf ("no overlap\n");
	    break;
	  default:
	    printf ("==d[%d]%s\n",
		    overlap->s[i],
		    tr->overlap == OVERLAP_LOW_TO_HIGH ? "+a"
		    : tr->overlap == OVERLAP_HIGH_TO_LOW ? "-a"
		    : "");
	    break;
	  }
	  printf ("   s[%d]=", i);
	  if (tr->carry_sign && (carry & (1 << i)))
	    printf ("-");
	  mpn_trace (NULL, s[i].p, SRC_SIZE(i));
	}
    }

  if (tr->dst0_from_src1)
    mpn_trace ("   d[0]", s[1].region.ptr, size);

  if (tr->reference)
    print_each (&ref);
  print_each (&fun);
}

void
compare (void)
{
  int  error = 0;
  int  i;

  if (tr->retval && ref.retval != fun.retval)
    {
      gmp_printf ("Different return values (%Mu, %Mu)\n",
		  ref.retval, fun.retval);
      error = 1;
    }

  for (i = 0; i < NUM_DESTS; i++)
    {
      switch (tr->dst_size[i]) {
      case SIZE_RETVAL:
      case SIZE_GET_STR:
	d[i].size = ref.retval;
	break;
      }
    }

  for (i = 0; i < NUM_DESTS; i++)
    {
      if (! tr->dst[i])
	continue;

      if (tr->dst_bytes[i])
	{
	  if (memcmp (ref.d[i].p, fun.d[i].p, d[i].size) != 0)
	    {
	      printf ("Different d[%d] data results, low diff at %ld, high diff at %ld\n",
		      i,
		      (long) byte_diff_lowest (ref.d[i].p, fun.d[i].p, d[i].size),
		      (long) byte_diff_highest (ref.d[i].p, fun.d[i].p, d[i].size));
	      error = 1;
	    }
	}
      else
	{
	  if (d[i].size != 0
	      && ! refmpn_equal_anynail (ref.d[i].p, fun.d[i].p, d[i].size))
	    {
	      printf ("Different d[%d] data results, low diff at %ld, high diff at %ld\n",
		      i,
		      (long) mpn_diff_lowest (ref.d[i].p, fun.d[i].p, d[i].size),
		      (long) mpn_diff_highest (ref.d[i].p, fun.d[i].p, d[i].size));
	      error = 1;
	    }
	}
    }

  if (error)
    {
      print_all();
      abort();
    }
}


/* The functions are cast if the return value should be a long rather than
   the default mp_limb_t.  This is necessary under _LONG_LONG_LIMB.  This
   might not be enough if some actual calling conventions checking is
   implemented on a long long limb system.  */

void
call (struct each_t *e, tryfun_t function)
{
  switch (choice->type) {
  case TYPE_ADD:
  case TYPE_SUB:
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, e->s[0].p, size, e->s[1].p, size2);
    break;

  case TYPE_ADDERR1_N:
  case TYPE_SUBERR1_N:
     e->retval =CALLING_CONVENTIONS(function)
         (e->d[0].p, e->s[0].p, e->s[1].p, e->d[1].p, e->s[2].p , size,carry);
     break;
  case TYPE_ADDERR2_N:
  case TYPE_SUBERR2_N:
     e->retval =CALLING_CONVENTIONS(function)
         (e->d[0].p, e->s[0].p, e->s[1].p, e->d[1].p, e->s[2].p ,e->s[3].p, size,carry);
     break;
  
  case TYPE_ADD_N:
  case TYPE_SUB_N:
  case TYPE_ADDLSH1_N:
  case TYPE_SUBLSH1_N:
  case TYPE_RSH1ADD_N:
  case TYPE_RSH1SUB_N:
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, e->s[0].p, e->s[1].p, size);
    break;
  case TYPE_ADDLSH_N:
  case TYPE_SUBLSH_N:
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, e->s[0].p, e->s[1].p, size,shift);
    break;
  case TYPE_ADDLSH_NC:
  case TYPE_SUBLSH_NC:
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, e->s[0].p, e->s[1].p, size,shift,carry);
    break;
  case TYPE_INCLSH_N:
  case TYPE_DECLSH_N:
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, e->s[0].p, size,shift);
    break;
  case TYPE_ADD_NC:
  case TYPE_SUB_NC:
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, e->s[0].p, e->s[1].p, size, carry);
    break;

  case TYPE_MUL_1:
  case TYPE_ADDMUL_1:
  case TYPE_SUBMUL_1:
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, e->s[0].p, size, multiplier);
    break;
  case TYPE_MUL_1C:
  case TYPE_ADDMUL_1C:
  case TYPE_SUBMUL_1C:
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, e->s[0].p, size, multiplier, carry);
    break;

  case TYPE_MUL_2:
    if (size == 1)
      abort ();
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, e->s[0].p, size, e->s[1].p);
    break;

  case TYPE_ADDMUL_2:
  case TYPE_ADDMUL_3:
  case TYPE_ADDMUL_4:
  case TYPE_ADDMUL_5:
  case TYPE_ADDMUL_6:
  case TYPE_ADDMUL_7:
  case TYPE_ADDMUL_8:
    if (size == 1)
      abort ();
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, e->s[0].p, size, multiplier_N);
    break;

  case TYPE_AND_N:
  case TYPE_ANDN_N:
  case TYPE_NAND_N:
  case TYPE_IOR_N:
  case TYPE_IORN_N:
  case TYPE_NIOR_N:
  case TYPE_XOR_N:
  case TYPE_XNOR_N:
    CALLING_CONVENTIONS (function) (e->d[0].p, e->s[0].p, e->s[1].p, size);
    break;

  case TYPE_SUMDIFF_N:
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, e->d[1].p, e->s[0].p, e->s[1].p, size);
    break;
  case TYPE_NSUMDIFF_N:
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, e->d[1].p, e->s[0].p, e->s[1].p, size);
    break;
  case TYPE_SUMDIFF_NC:
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, e->d[1].p, e->s[0].p, e->s[1].p, size, carry);
    break;

  case TYPE_ADDSUB_N:
    e->retval = (int)CALLING_CONVENTIONS (function)
      (e->d[0].p, e->s[0].p, e->s[1].p, e->s[2].p,size);
    break;
  case TYPE_ADDADD_N:
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, e->s[0].p, e->s[1].p, e->s[2].p,size);
    break;
  case TYPE_SUBADD_N:
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, e->s[0].p, e->s[1].p, e->s[2].p,size);
    break;
    

  case TYPE_COPY:
  case TYPE_COPYI:
  case TYPE_COPYD:
  case TYPE_COM_N:
    CALLING_CONVENTIONS (function) (e->d[0].p, e->s[0].p, size);
    break;


  case TYPE_DIVEXACT_BY3:
    e->retval = CALLING_CONVENTIONS (function) (e->d[0].p, e->s[0].p, size);
    break;
  case TYPE_DIVEXACT_BYFF:
    e->retval = CALLING_CONVENTIONS (function) (e->d[0].p, e->s[0].p, size);
    break;

  case TYPE_LSHIFT1:
  case TYPE_RSHIFT1:
  case TYPE_LSHIFT2:
  case TYPE_RSHIFT2:
    e->retval = CALLING_CONVENTIONS (function) (e->d[0].p, e->s[0].p, size);
    break;    

  case TYPE_DIVEXACT_BY3C:
    e->retval = CALLING_CONVENTIONS (function) (e->d[0].p, e->s[0].p, size,
						carry);
    break;


  case TYPE_DIVREM_HENSEL_QR_1:
  case TYPE_DIVREM_HENSEL_QR_1_1:
  case TYPE_DIVREM_HENSEL_QR_1_2:
  case TYPE_DIVMOD_1:
  case TYPE_DIVEXACT_1:
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, e->s[0].p, size, divisor);
    break;

  case TYPE_RSH_DIVREM_HENSEL_QR_1:
  case TYPE_RSH_DIVREM_HENSEL_QR_1_1:
  case TYPE_RSH_DIVREM_HENSEL_QR_1_2:
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, e->s[0].p, size, divisor,shift,carry);
    break;

  case TYPE_DIVREM_HENSEL_RSH_QR_1:
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, e->s[0].p, size, divisor,shift);
    break;
  case TYPE_DIVEXACT_BYFOBM1:
    e->retval = CALLING_CONVENTIONS (function) (e->d[0].p, e->s[0].p, size, altdiv,GMP_NUMB_MAX/altdiv);
    break;
  case TYPE_DIVMOD_1C:
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, e->s[0].p, size, divisor, carry);
    break;
  case TYPE_DIVREM_EUCLIDEAN_QR_1:
  case TYPE_DIVREM_1:
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, size2, e->s[0].p, size, divisor);
    break;
  case TYPE_DIVREM_1C:
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, size2, e->s[0].p, size, divisor, carry);
    break;
  case TYPE_PREINV_DIVREM_1:
    {
      mp_limb_t  dinv;
      unsigned   shift;
      shift = refmpn_count_leading_zeros (divisor);
      dinv = refmpn_invert_limb (divisor << shift);
      e->retval = CALLING_CONVENTIONS (function)
	(e->d[0].p, size2, e->s[0].p, size, divisor, dinv, shift);
    }
    break;
  case TYPE_DIVREM_HENSEL_R_1:
  case TYPE_DIVREM_EUCLIDEAN_R_1:
  case TYPE_MOD_1:
  case TYPE_MODEXACT_1_ODD:
    e->retval = CALLING_CONVENTIONS (function)
      (e->s[0].p, size, divisor);
    break;
  case TYPE_MOD_1C:
  case TYPE_MODEXACT_1C_ODD:
    e->retval = CALLING_CONVENTIONS (function)
      (e->s[0].p, size, divisor, carry);
    break;
  case TYPE_PREINV_MOD_1:
    e->retval = CALLING_CONVENTIONS (function)
      (e->s[0].p, size, divisor, refmpn_invert_limb (divisor));
    break;
  case TYPE_MOD_34LSUB1:
    e->retval = CALLING_CONVENTIONS (function) (e->s[0].p, size);
    break;

  case TYPE_UDIV_QRNND:
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, e->s[0].p[1], e->s[0].p[0], divisor);
    break;
  case TYPE_UDIV_QRNND_R:
    e->retval = CALLING_CONVENTIONS (function)
      (e->s[0].p[1], e->s[0].p[0], divisor, e->d[0].p);
    break;

  case TYPE_SB_DIVREM_MN:
    refmpn_copyi (e->d[1].p, e->s[0].p, size);        /* dividend */
    refmpn_fill (e->d[0].p, size-size2, 0x98765432);  /* quotient */
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, e->d[1].p, size, e->s[1].p, size2);
    refmpn_zero (e->d[1].p+size2, size-size2);    /* excess over remainder */
    break;
  case TYPE_TDIV_QR:
    CALLING_CONVENTIONS (function) (e->d[0].p, e->d[1].p, 0,
				    e->s[0].p, size, e->s[1].p, size2);
    break;
case TYPE_TDIV_Q:
    CALLING_CONVENTIONS (function) (e->d[0].p, e->s[0].p, 
                                 size, e->s[1].p, size2);
    break;

  case TYPE_GCD_1:
    /* Must have a non-zero src, but this probably isn't the best way to do
       it. */
    if (refmpn_zero_p (e->s[0].p, size))
      e->retval = 0;
    else
      e->retval = CALLING_CONVENTIONS (function) (e->s[0].p, size, divisor);
    break;

  case TYPE_GCD:
    /* Sources are destroyed, so they're saved and replaced, but a general
       approach to this might be better.  Note that it's still e->s[0].p and
       e->s[1].p that are passed, to get the desired alignments. */
    {
      mp_ptr  s0 = refmpn_malloc_limbs (size);
      mp_ptr  s1 = refmpn_malloc_limbs (size2);
      refmpn_copyi (s0, e->s[0].p, size);
      refmpn_copyi (s1, e->s[1].p, size2);

      mprotect_region (&s[0].region, PROT_READ|PROT_WRITE);
      mprotect_region (&s[1].region, PROT_READ|PROT_WRITE);
      e->retval = CALLING_CONVENTIONS (function) (e->d[0].p,
						  e->s[0].p, size,
						  e->s[1].p, size2);
      refmpn_copyi (e->s[0].p, s0, size);
      refmpn_copyi (e->s[1].p, s1, size2);
      free (s0);
      free (s1);
    }
    break;

  case TYPE_GCD_FINDA:
    {
      /* FIXME: do this with a flag */
      mp_limb_t  c[2];
      c[0] = e->s[0].p[0];
      c[0] += (c[0] == 0);
      c[1] = e->s[0].p[0];
      c[1] += (c[1] == 0);
      e->retval = CALLING_CONVENTIONS (function) (c);
    }
    break;

  case TYPE_MPZ_JACOBI:
  case TYPE_MPZ_KRONECKER:
    {
      mpz_t  a, b;
      PTR(a) = e->s[0].p; SIZ(a) = ((carry&1)==0 ? size : -size);
      PTR(b) = e->s[1].p; SIZ(b) = ((carry&2)==0 ? size2 : -size2);
      e->retval = CALLING_CONVENTIONS (function) (a, b);
    }
    break;
  case TYPE_MPZ_KRONECKER_UI:
    {
      mpz_t  a;
      PTR(a) = e->s[0].p; SIZ(a) = (carry==0 ? size : -size);
      e->retval = CALLING_CONVENTIONS(function) (a, (unsigned long)multiplier);
    }
    break;
  case TYPE_MPZ_KRONECKER_SI:
    {
      mpz_t  a;
      PTR(a) = e->s[0].p; SIZ(a) = (carry==0 ? size : -size);
      e->retval = CALLING_CONVENTIONS (function) (a, (long) multiplier);
    }
    break;
  case TYPE_MPZ_UI_KRONECKER:
    {
      mpz_t  b;
      PTR(b) = e->s[0].p; SIZ(b) = (carry==0 ? size : -size);
      e->retval = CALLING_CONVENTIONS(function) ((unsigned long)multiplier, b);
    }
    break;
  case TYPE_MPZ_SI_KRONECKER:
    {
      mpz_t  b;
      PTR(b) = e->s[0].p; SIZ(b) = (carry==0 ? size : -size);
      e->retval = CALLING_CONVENTIONS (function) ((long) multiplier, b);
    }
    break;

  case TYPE_MUL_BASECASE:
    CALLING_CONVENTIONS (function)
      (e->d[0].p, e->s[0].p, size, e->s[1].p, size2);
    break;
  case TYPE_MULMID_BASECASE:
    CALLING_CONVENTIONS (function)
      (e->d[0].p, e->s[0].p, size, e->s[1].p, size2);
    break;
  case TYPE_MULMID:
    CALLING_CONVENTIONS (function)
      (e->d[0].p, e->s[0].p, size, e->s[1].p, size2);
    break;
  case TYPE_MULMID_N:
    CALLING_CONVENTIONS (function)
      (e->d[0].p, e->s[1].p, e->s[0].p, size);
    break;
  case TYPE_REDC_BASECASE:
    /* Sources are destroyed, so they're saved and replaced, but a general
       approach to this might be better.  Note that it's still e->s[0].p and
       e->s[1].p that are passed, to get the desired alignments. */
    {
      mp_limb_t Np;
      mp_ptr  s0 = refmpn_malloc_limbs (size);
      mp_ptr  s1 = refmpn_malloc_limbs (size2);
      modlimb_invert(Np,e->s[0].p[0]);
      Np=-Np;
      refmpn_copyi (s0, e->s[0].p, size);
      refmpn_copyi (s1, e->s[1].p, size2);

      mprotect_region (&s[0].region, PROT_READ|PROT_WRITE);
      mprotect_region (&s[1].region, PROT_READ|PROT_WRITE);
      e->retval = CALLING_CONVENTIONS (function) (e->d[0].p,e->s[1].p,
						  e->s[0].p, size,Np);
      refmpn_copyi (e->s[0].p, s0, size);
      refmpn_copyi (e->s[1].p, s1, size2);
      free (s0);
      free (s1);
    }
  
    break;
  case TYPE_MUL_N:
    CALLING_CONVENTIONS (function) (e->d[0].p, e->s[0].p, e->s[1].p, size);
    break;
  case TYPE_SQR:
    CALLING_CONVENTIONS (function) (e->d[0].p, e->s[0].p, size);
    break;

  case TYPE_UMUL_PPMM:
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, e->s[0].p[0], e->s[0].p[1]);
    break;
  case TYPE_UMUL_PPMM_R:
    e->retval = CALLING_CONVENTIONS (function)
      (e->s[0].p[0], e->s[0].p[1], e->d[0].p);
    break;

  case TYPE_LSHIFTC:
  case TYPE_LSHIFT:
  case TYPE_RSHIFT:
    e->retval = CALLING_CONVENTIONS (function)
      (e->d[0].p, e->s[0].p, size, shift);
    break;

  case TYPE_NOT:
    	 CALLING_CONVENTIONS (function) (e->d[0].p, size);
    break;
  case TYPE_HALF:
  case TYPE_DOUBLE:
    	 e->retval=CALLING_CONVENTIONS (function) (e->d[0].p, size);
    break;
  case TYPE_POPCOUNT:
    e->retval = (* (unsigned long (*)(ANYARGS))
		 CALLING_CONVENTIONS (function)) (e->s[0].p, size);
    break;
  case TYPE_HAMDIST:
    e->retval = (* (unsigned long (*)(ANYARGS))
		 CALLING_CONVENTIONS (function)) (e->s[0].p, e->s[1].p, size);
    break;

  case TYPE_SQRTREM:
    e->retval = (* (long (*)(ANYARGS)) CALLING_CONVENTIONS (function))
      (e->d[0].p, e->d[1].p, e->s[0].p, size);
    break;

  case TYPE_ZERO:
    CALLING_CONVENTIONS (function) (e->d[0].p, size);
    break;
  case TYPE_STORE:
    CALLING_CONVENTIONS (function) (e->d[0].p, size,4354);
    break;

  case TYPE_GET_STR:
    {
      size_t  sizeinbase, fill;
      char    *dst;
      MPN_SIZEINBASE (sizeinbase, e->s[0].p, size, base);
      ASSERT_ALWAYS (sizeinbase <= d[0].size);
      fill = d[0].size - sizeinbase;
      if (d[0].high)
	{
	  memset (e->d[0].p, 0xBA, fill);
	  dst = (char *) e->d[0].p + fill;
	}
      else
	{
	  dst = (char *) e->d[0].p;
	  memset (dst + sizeinbase, 0xBA, fill);
	}
      if (POW2_P (base))
	{
	  e->retval = CALLING_CONVENTIONS (function) (dst, base,
						      e->s[0].p, size);
	}
      else
	{
	  refmpn_copy (e->d[1].p, e->s[0].p, size);
	  e->retval = CALLING_CONVENTIONS (function) (dst, base,
						      e->d[1].p, size);
	}
      refmpn_zero (e->d[1].p, size);  /* cloberred or unused */
    }
    break;

#ifdef EXTRA_CALL
    EXTRA_CALL
#endif

  default:
    printf ("Unknown routine type %d\n", choice->type);
    abort ();
    break;
  }
}


void
pointer_setup (struct each_t *e)
{
  int  i, j;

  for (i = 0; i < NUM_DESTS; i++)
    {
      switch (tr->dst_size[i]) {
      case 0:
      case SIZE_RETVAL: /* will be adjusted later */
	d[i].size = size;
	break;

      case SIZE_1:
	d[i].size = 1;
	break;
      case SIZE_2:
	d[i].size = 2;
	break;
      case SIZE_3:
	d[i].size = 3;
	break;
      case SIZE_4:
	d[i].size = 4;
	break;

      case SIZE_PLUS_1:
	d[i].size = size+1;
	break;
      case SIZE_PLUS_2:
	d[i].size = size+2;
	break;
      case SIZE_PLUS_MSIZE_SUB_1:
	d[i].size = size + tr->msize - 1;
	break;

      case SIZE_SUM:
	if (tr->size2)
	  d[i].size = size + size2;
	else
	  d[i].size = 2*size;
	break;

      case SIZE_SIZE2:
	d[i].size = size2;
	break;

      case SIZE_DIFF:
	d[i].size = size - size2;
	break;

      case SIZE_DIFF_PLUS_1:
	d[i].size = size - size2 + 1;
	break;

      case SIZE_DIFF_PLUS_3:
	d[i].size = size - size2 + 3;
	break;

      case SIZE_CEIL_HALF:
	d[i].size = (size+1)/2;
	break;

      case SIZE_GET_STR:
	{
	  mp_limb_t ff = GMP_NUMB_MAX;
	  MPN_SIZEINBASE (d[i].size, &ff - (size-1), size, base);
	}
	break;

      default:
	printf ("Unrecognised dst_size type %d\n", tr->dst_size[i]);
	abort ();
      }
    }

  /* establish e->d[].p destinations */
  for (i = 0; i < NUM_DESTS; i++)
    {
      mp_size_t  offset = 0;

      /* possible room for overlapping sources */
      for (j = 0; j < numberof (overlap->s); j++)
	if (overlap->s[j] == i)
	  offset = MAX (offset, s[j].align);

      if (d[i].high)
	{
	  if (tr->dst_bytes[i])
	    {
	      e->d[i].p = (mp_ptr)
		((char *) (e->d[i].region.ptr + e->d[i].region.size)
		 - d[i].size - d[i].align);
	    }
	  else
	    {
	      e->d[i].p = e->d[i].region.ptr + e->d[i].region.size
		- d[i].size - d[i].align;
	      if (tr->overlap == OVERLAP_LOW_TO_HIGH)
		e->d[i].p -= offset;
	    }
	}
      else
	{
	  if (tr->dst_bytes[i])
	    {
	      e->d[i].p = (mp_ptr) ((char *) e->d[i].region.ptr + d[i].align);
	    }
	  else
	    {
	      e->d[i].p = e->d[i].region.ptr + d[i].align;
	      if (tr->overlap == OVERLAP_HIGH_TO_LOW)
		e->d[i].p += offset;
	    }
	}
    }

  /* establish e->s[].p sources */
  for (i = 0; i < NUM_SOURCES; i++)
    {
      int  o = overlap->s[i];
      switch (o) {
      case -1:
	/* no overlap */
	e->s[i].p = s[i].p;
	break;
      case 0:
      case 1:
	/* overlap with d[o] */
	if (tr->overlap == OVERLAP_HIGH_TO_LOW)
	  e->s[i].p = e->d[o].p - s[i].align;
	else if (tr->overlap == OVERLAP_LOW_TO_HIGH)
	  e->s[i].p = e->d[o].p + s[i].align;
	else if (tr->size2 == SIZE_FRACTION)
	  e->s[i].p = e->d[o].p + size2;
	else
	  e->s[i].p = e->d[o].p;
	break;
      default:
	abort();
	break;
      }
    }
}


void
validate_fail (void)
{
  if (tr->reference)
    {
      trap_location = TRAP_REF;
      call (&ref, tr->reference);
      trap_location = TRAP_NOWHERE;
    }

  print_all();
  abort();
}


void
try_one (void)
{
  int  i;

  if (option_spinner)
    spinner();
  spinner_count++;

  trap_location = TRAP_SETUPS;

  if (tr->divisor == DIVISOR_NORM)
    divisor |= GMP_NUMB_HIGHBIT;
  if (tr->divisor == DIVISOR_ODD)
    divisor |= 1;

  for (i = 0; i < NUM_SOURCES; i++)
    {
      if (s[i].high)
	s[i].p = s[i].region.ptr + s[i].region.size - SRC_SIZE(i) - s[i].align;
      else
	s[i].p = s[i].region.ptr + s[i].align;
    }

  pointer_setup (&ref);
  pointer_setup (&fun);

  ref.retval = 0x04152637;
  fun.retval = 0x8C9DAEBF;

  t_random (multiplier_N, tr->msize);

  for (i = 0; i < NUM_SOURCES; i++)
    {
      if (! tr->src[i])
	continue;

      mprotect_region (&s[i].region, PROT_READ|PROT_WRITE);
      t_random (s[i].p, SRC_SIZE(i));

      switch (tr->data) {
      case DATA_NON_ZERO:
	if (refmpn_zero_p (s[i].p, SRC_SIZE(i)))
	  s[i].p[0] = 1;
	break;

      case DATA_MULTIPLE_DIVISOR:
	/* same number of low zero bits as divisor */
	s[i].p[0] &= ~ LOW_ZEROS_MASK (divisor);
	refmpn_sub_1 (s[i].p, s[i].p, size,
		      refmpn_mod_1 (s[i].p, size, divisor));
	break;

      case DATA_GCD:
	/* s[1] no more bits than s[0] */
	if (i == 1 && size2 == size)
	  s[1].p[size-1] &= refmpn_msbone_mask (s[0].p[size-1]);

	/* high limb non-zero */
	s[i].p[SRC_SIZE(i)-1] += (s[i].p[SRC_SIZE(i)-1] == 0);

	/* odd */
	s[i].p[0] |= 1;
	break;

      case DATA_SRC1_ODD:
	if (i == 1)
	  s[i].p[0] |= 1;
	break;
	
      case DATA_SRC0_ODD:
	if (i == 0)
	  s[i].p[0] |= 1;
	break;

      case DATA_SRC1_HIGHBIT:
	if (i == 1)
	  {
	    if (tr->size2)
	      s[i].p[size2-1] |= GMP_NUMB_HIGHBIT;
	    else
	      s[i].p[size-1] |= GMP_NUMB_HIGHBIT;
	  }
	break;

      case DATA_UDIV_QRNND:
	s[i].p[1] %= divisor;
	break;
      }

      mprotect_region (&s[i].region, PROT_READ);
    }

  for (i = 0; i < NUM_DESTS; i++)
    {
      if (! tr->dst[i])
	continue;

      if (tr->dst0_from_src1 && i==0)
	{
	  mp_size_t  copy = MIN (d[0].size, SRC_SIZE(1));
	  mp_size_t  fill = MAX (0, d[0].size - copy);
	  MPN_COPY (fun.d[0].p, s[1].region.ptr, copy);
	  MPN_COPY (ref.d[0].p, s[1].region.ptr, copy);
	  refmpn_fill (fun.d[0].p + copy, fill, DEADVAL);
	  refmpn_fill (ref.d[0].p + copy, fill, DEADVAL);
	}
      else if (tr->dst_bytes[i])
	{
	  memset (ref.d[i].p, 0xBA, d[i].size);
	  memset (fun.d[i].p, 0xBA, d[i].size);
	}
      else
	{
	  refmpn_fill (ref.d[i].p, d[i].size, DEADVAL);
	  refmpn_fill (fun.d[i].p, d[i].size, DEADVAL);
	}
    }

  for (i = 0; i < NUM_SOURCES; i++)
    {
      if (! tr->src[i])
	continue;

      if (ref.s[i].p != s[i].p)
	{
	  refmpn_copyi (ref.s[i].p, s[i].p, SRC_SIZE(i));
	  refmpn_copyi (fun.s[i].p, s[i].p, SRC_SIZE(i));
	}
    }

  if (option_print)
    print_all();

  if (tr->validate != NULL)
    {
      trap_location = TRAP_FUN;
      call (&fun, choice->function);
      trap_location = TRAP_NOWHERE;

      if (! CALLING_CONVENTIONS_CHECK ())
	{
	  print_all();
	  abort();
	}

      (*tr->validate) ();
    }
  else
    {
      trap_location = TRAP_REF;
      call (&ref, tr->reference);
      trap_location = TRAP_FUN;
      call (&fun, choice->function);
      trap_location = TRAP_NOWHERE;

      if (! CALLING_CONVENTIONS_CHECK ())
	{
	  print_all();
	  abort();
	}

      compare ();
    }
}


#define SIZE_ITERATION                                          \
  for (size = MAX3 (option_firstsize,                           \
		    choice->minsize,                            \
		    (tr->size == SIZE_ALLOW_ZERO) ? 0 : 1);     \
       size <= option_lastsize;                                 \
       size++)

#define SIZE2_FIRST                                     \
  (tr->size2 == SIZE_2 ? 2                              \
   : tr->size2 == SIZE_FRACTION ? option_firstsize2     \
   : tr->size2 == SIZE_DOUBLE ? size*2			\
   : tr->size2 == SIZE_DOUBLE_MINUS_1 ? size*2-1		\
   : tr->size2 ?                                        \
   MAX (choice->minsize, (option_firstsize2 != 0        \
			  ? option_firstsize2 : 1))     \
   : 0)

#define SIZE2_LAST                                      \
  (tr->size2 == SIZE_2 ? 2                              \
   : tr->size2 == SIZE_FRACTION ? FRACTION_COUNT-1      \
   : tr->size2 == SIZE_DOUBLE ? size*2		        	\
   : tr->size2 == SIZE_DOUBLE_MINUS_1 ? size*2-1		\
   : tr->size2 ? size                                   \
   : 0)

#define SIZE2_ITERATION \
  for (size2 = SIZE2_FIRST; size2 <= SIZE2_LAST; size2++)

#define ALIGN_COUNT(cond)  ((cond) ? ALIGNMENTS : 1)
#define ALIGN_ITERATION(w,n,cond) \
  for (w[n].align = 0; w[n].align < ALIGN_COUNT(cond); w[n].align++)

#define HIGH_LIMIT(cond)  ((cond) != 0)
#define HIGH_COUNT(cond)  (HIGH_LIMIT (cond) + 1)
#define HIGH_ITERATION(w,n,cond) \
  for (w[n].high = 0; w[n].high <= HIGH_LIMIT(cond); w[n].high++)

#define SHIFT_LIMIT                                     \
  ((unsigned long) (tr->shift ? GMP_NUMB_BITS -1 : 1))

#define SHIFT_ITERATION                                 \
  for (shift = 1; shift <= SHIFT_LIMIT; shift++)


void
try_many (void)
{
  int   i;

  {
    unsigned long  total = 1;

    total *= option_repetitions;
    total *= option_lastsize;
    if (tr->size2 == SIZE_FRACTION) total *= FRACTION_COUNT;
    else if (tr->size2 == SIZE_DOUBLE) total *= 1;
    else if (tr->size2)             total *= (option_lastsize+1)/2;

    total *= SHIFT_LIMIT;
    total *= MULTIPLIER_COUNT;
    total *= DIVISOR_COUNT;
    total *= ALTDIV_COUNT;
    total *= CARRY_COUNT;
    total *= T_RAND_COUNT;

    total *= HIGH_COUNT (tr->dst[0]);
    total *= HIGH_COUNT (tr->dst[1]);
    total *= HIGH_COUNT (tr->src[0]);
    total *= HIGH_COUNT (tr->src[1]);
    total *= HIGH_COUNT (tr->src[2]);
    
    total *= ALIGN_COUNT (tr->dst[0]);
    total *= ALIGN_COUNT (tr->dst[1]);
    total *= ALIGN_COUNT (tr->src[0]);
    total *= ALIGN_COUNT (tr->src[1]);
    total *= ALIGN_COUNT (tr->src[2]);

#if NUM_SOURCES > 3 || NUM_DESTS > 2
#error Need to adjust high_count and align_count above
#endif

    total *= OVERLAP_COUNT;

    printf ("%s %lu\n", choice->name, total);
  }

  spinner_count = 0;

  for (i = 0; i < option_repetitions; i++)
    SIZE_ITERATION
      SIZE2_ITERATION

      SHIFT_ITERATION
      MULTIPLIER_ITERATION
      ALTDIV_ITERATION
      DIVISOR_ITERATION
      CARRY_ITERATION /* must be after divisor */
      T_RAND_ITERATION

      HIGH_ITERATION(d,0, tr->dst[0])
      HIGH_ITERATION(d,1, tr->dst[1])
      HIGH_ITERATION(s,0, tr->src[0])
      HIGH_ITERATION(s,1, tr->src[1])
      HIGH_ITERATION(s,2, tr->src[2])

      ALIGN_ITERATION(d,0, tr->dst[0])
      ALIGN_ITERATION(d,1, tr->dst[1])
      ALIGN_ITERATION(s,0, tr->src[0])
      ALIGN_ITERATION(s,1, tr->src[1])
      ALIGN_ITERATION(s,2, tr->src[2])

#if NUM_SOURCES > 3 || NUM_DESTS > 2
#error Need to adjust high_iteration and align_iteration above
#endif

      OVERLAP_ITERATION
      try_one();

  printf("\n");
}


/* Usually print_all() doesn't show much, but it might give a hint as to
   where the function was up to when it died. */
void
trap (int sig)
{
  const char *name = "noname";

  switch (sig) {
  case SIGILL:  name = "SIGILL";  break;
#ifdef SIGBUS
  case SIGBUS:  name = "SIGBUS";  break;
#endif
  case SIGSEGV: name = "SIGSEGV"; break;
  case SIGFPE:  name = "SIGFPE";  break;
  }

  printf ("\n\nSIGNAL TRAP: %s\n", name);

  switch (trap_location) {
  case TRAP_REF:
    printf ("  in reference function: %s\n", tr->reference_name);
    break;
  case TRAP_FUN:
    printf ("  in test function: %s\n", choice->name);
    print_all ();
    break;
  case TRAP_SETUPS:
    printf ("  in parameter setups\n");
    print_all ();
    break;
  default:
    printf ("  somewhere unknown\n");
    break;
  }
  exit (1);
}


void
try_init (void)
{
#if HAVE_GETPAGESIZE
  /* Prefer getpagesize() over sysconf(), since on SunOS 4 sysconf() doesn't
     know _SC_PAGESIZE. */
  pagesize = getpagesize ();
#elif HAVE_SYSCONF
  if ((pagesize = sysconf (_SC_PAGESIZE)) == -1)
    {
      /* According to the linux man page, sysconf doesn't set errno */
      fprintf (stderr, "Cannot get sysconf _SC_PAGESIZE\n");
      exit (1);
    }
#elif defined( _MSC_VER )
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    pagesize = si.dwPageSize;
#else
#error Error, error, cannot get page size
#endif

  printf ("pagesize is 0x%lX bytes\n", pagesize);

  signal (SIGILL,  trap);
#ifdef SIGBUS
  signal (SIGBUS,  trap);
#endif
  signal (SIGSEGV, trap);
  signal (SIGFPE,  trap);

  {
    int  i;

    for (i = 0; i < NUM_SOURCES; i++)
      {
	malloc_region (&s[i].region, 2*option_lastsize+ALIGNMENTS-1);
	printf ("s[%d] %p to %p (0x%lX bytes)\n",
		i, s[i].region.ptr,
		s[i].region.ptr + s[i].region.size,
		(long) s[i].region.size * BYTES_PER_MP_LIMB);
      }

#define INIT_EACH(e,es)                                                 \
    for (i = 0; i < NUM_DESTS; i++)                                     \
      {                                                                 \
	malloc_region (&e.d[i].region, 2*option_lastsize+ALIGNMENTS-1); \
	printf ("%s d[%d] %p to %p (0x%lX bytes)\n",                    \
		es, i, e.d[i].region.ptr,                               \
		e.d[i].region.ptr + e.d[i].region.size,                 \
		(long) e.d[i].region.size * BYTES_PER_MP_LIMB);         \
      }

    INIT_EACH(ref, "ref");
    INIT_EACH(fun, "fun");
  }
}

int
strmatch_wild (const char *pattern, const char *str)
{
  size_t  plen, slen;

  /* wildcard at start */
  if (pattern[0] == '*')
    {
      pattern++;
      plen = strlen (pattern);
      slen = strlen (str);
      return (plen == 0
	      || (slen >= plen && memcmp (pattern, str+slen-plen, plen) == 0));
    }

  /* wildcard at end */
  plen = strlen (pattern);
  if (plen >= 1 && pattern[plen-1] == '*')
    return (memcmp (pattern, str, plen-1) == 0);

  /* no wildcards */
  return (strcmp (pattern, str) == 0);
}

void
try_name (const char *name)
{
  int  found = 0;
  int  i;

  for (i = 0; i < numberof (choice_array); i++)
    {
      if (strmatch_wild (name, choice_array[i].name))
	{
	  choice = &choice_array[i];
	  tr = &param[choice->type];
	  try_many ();
	  found = 1;
	}
    }

  if (!found)
    {
      printf ("%s unknown\n", name);
      /* exit (1); */
    }
}


void
usage (const char *prog)
{
  int  col = 0;
  int  i;

  printf ("Usage: %s [options] function...\n", prog);
  printf ("    -1        use limb data 1,2,3,etc\n");
  printf ("    -9        use limb data all 0xFF..FFs\n");
  printf ("    -a zeros  use limb data all zeros\n");
  printf ("    -a ffs    use limb data all 0xFF..FFs (same as -9)\n");
  printf ("    -a 2fd    use data 0x2FFF...FFFD\n");
  printf ("    -p        print each case tried (try this if seg faulting)\n");
  printf ("    -R        seed random numbers from time()\n");
  printf ("    -r reps   set repetitions (default %d)\n", DEFAULT_REPETITIONS);
  printf ("    -s size   starting size to test\n");
  printf ("    -S size2  starting size2 to test\n");
  printf ("    -s s1-s2  range of sizes to test\n");
  printf ("    -W        don't show the spinner (use this in gdb)\n");
  printf ("    -z        disable mprotect() redzones\n");
  printf ("Default data is refmpn_random() and refmpn_random2().\n");
  printf ("\n");
  printf ("Functions that can be tested:\n");

  for (i = 0; i < numberof (choice_array); i++)
    {
      if (col + 1 + strlen (choice_array[i].name) > 79)
	{
	  printf ("\n");
	  col = 0;
	}
      printf (" %s", choice_array[i].name);
      col += 1 + strlen (choice_array[i].name);
    }
  printf ("\n");

  exit(1);
}


int
main (int argc, char *argv[])
{
  int  i;

  /* unbuffered output */
  setbuf (stdout, NULL);
  setbuf (stderr, NULL);

  /* default trace in hex, and in upper-case so can paste into bc */
  mp_trace_base = -16;

  param_init ();

  {
    unsigned long  seed = 123;
    int   opt;

    while ((opt = getopt(argc, argv, "19a:b:E:pRr:S:s:Wz")) != EOF)
      {
	switch (opt) {
	case '1':
	  /* use limb data values 1, 2, 3, ... etc */
	  option_data = DATA_SEQ;
	  break;
	case '9':
	  /* use limb data values 0xFFF...FFF always */
	  option_data = DATA_FFS;
	  break;
	case 'a':
	  if (strcmp (optarg, "zeros") == 0)     option_data = DATA_ZEROS;
	  else if (strcmp (optarg, "seq") == 0)  option_data = DATA_SEQ;
	  else if (strcmp (optarg, "ffs") == 0)  option_data = DATA_FFS;
	  else if (strcmp (optarg, "2fd") == 0)  option_data = DATA_2FD;
	  else
	    {
	      fprintf (stderr, "unrecognised data option: %s\n", optarg);
	      exit (1);
	    }
	  break;
	case 'b':
	  mp_trace_base = atoi (optarg);
	  break;
	case 'E':
	  /* re-seed */
	  sscanf (optarg, "%lu", &seed);
	  printf ("Re-seeding with %lu\n", seed);
	  break;
	case 'p':
	  option_print = 1;
	  break;
	case 'R':
	  /* randomize */
	  seed = time (NULL);
	  printf ("Seeding with %lu, re-run using \"-E %lu\"\n", seed, seed);
	  break;
	case 'r':
	  option_repetitions = atoi (optarg);
	  break;
	case 's':
	  {
	    char  *p;
	    option_firstsize = atoi (optarg);
	    if ((p = strchr (optarg, '-')) != NULL)
	      option_lastsize = atoi (p+1);
	  }
	  break;
	case 'S':
	  /* -S <size> sets the starting size for the second of a two size
	     routine (like mpn_mul_basecase) */
	  option_firstsize2 = atoi (optarg);
	  break;
	case 'W':
	  /* use this when running in the debugger */
	  option_spinner = 0;
	  break;
	case 'z':
	  /* disable redzones */
	  option_redzones = 0;
	  break;
	case '?':
	  usage (argv[0]);
	  break;
	}
      }

    gmp_randinit_default (__gmp_rands);
    __gmp_rands_initialized = 1;
    gmp_randseed_ui (__gmp_rands, seed);
  }

  try_init();

  if (argc <= optind)
    usage (argv[0]);

  for (i = optind; i < argc; i++)
    try_name (argv[i]);

  return 0;
}
