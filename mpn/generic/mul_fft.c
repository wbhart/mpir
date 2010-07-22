/* An implementation in GMP of Scho"nhage's fast multiplication algorithm
  modulo 2^N+1, by Paul Zimmermann, INRIA Lorraine, February 1998.

   Revised July 2002 and January 2003, Paul Zimmermann.
   Further revised by Pierrick Gaudry, Paul Zimmermann, and Torbjorn Granlund,
   March/April and November/December 2006, and also by Alexander Kruppa in
   December 2006.

   THE CONTENTS OF THIS FILE ARE FOR INTERNAL USE AND THE FUNCTIONS HAVE
   MUTABLE INTERFACES.  IT IS ONLY SAFE TO REACH THEM THROUGH DOCUMENTED
   INTERFACES.  IT IS ALMOST GUARANTEED THAT THEY'LL CHANGE OR DISAPPEAR IN
   A FUTURE GNU MP RELEASE.

Copyright 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006 Free Software
Foundation, Inc.

Copyright 2009 William Hart

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

/* References:

   For the recent ideas used in the FFT, see:
  
   @InProceedings{GaKrZi07,
      author    =     {Pierrick Gaudry and Alexander Kruppa and Paul Zimmermann},
      title     =     {A {GMP}-based implementation of {S}ch\"onhage-{S}trassen's
                      large integer multiplication algorithm},
      booktitle =     {Proceedings of the 2007 International Symposium on
                      Symbolic and Algebraic Computation, {ISSAC'2007}},
      year      =     2007,
      address   =     {Waterloo, Ontario, Canada},
      pages     =     "167--174",
      editor    =     "C. W. Brown",
      annote    =     {\url{http://hal.inria.fr/inria-00126462}}
   }
   
	Schnelle Multiplikation grosser Zahlen, by Arnold Scho"nhage and Volker
   Strassen, Computing 7, p. 281-292, 1971.

   Asymptotically fast algorithms for the numerical multiplication
   and division of polynomials with complex coefficients, by Arnold Scho"nhage,
   Computer Algebra, EUROCAM'82, LNCS 144, p. 3-15, 1982.

   Tapes versus Pointers, a study in implementing fast algorithms,
   by Arnold Scho"nhage, Bulletin of the EATCS, 30, p. 23-32, 1986.

   See also http://www.loria.fr/~zimmerma/bignum


   Future:

   It might be possible to avoid a small number of MPN_COPYs by using a
   rotating temporary or two.

   Multiplications of unequal sized operands can be done with this code, but
   it needs a tighter test for identifying squaring (same sizes as well as
   same pointers).  */

   /* Throughout this file, Mp is chosen so that 
      ord_{2^Nprime + 1}(sqrt(2)^Mp) == 2^k */

#include <stdio.h>
#include <stdlib.h> /* for abort() */
#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"		/* for count_trailing_zeros */

#define FFT_FIRST_K 4

/* Uncomment this define to disable to use of sqrt(2) as a root of unity for 
   the transform/weight signal. The function mpn_fft_mul_sqrt2exp_modF() 
   will still get called, but parameters for the transform will be chosen 
   so that it will always be called with an even exponent, thus the 
   multiplication will be by a power of 2. */
/* #define NO_SQRT_2 */

/* Change this to "#define TRACE(x) x" for some traces. */
#define TRACE(x)

/* #define COUNT_ZEROCOPY */

/* This define enables interleaved decomposition/forward transform in 
   Bailey's algorithm for better data locality */
#define MERGED_BAILEY_DECOMPOSE

#if defined( _MSC_VER ) && defined( _M_X64 )
#include <intrin.h>
#pragma intrinsic(__stosq)
#define MPN_FFT_ZERO(d, l) __stosq(d, 0, l)
#else
#define MPN_FFT_ZERO(dst, n) MPN_ZERO(dst,n)
#endif

#if defined( _MSC_VER ) && defined( _M_X64 )
#include <intrin.h>
#pragma intrinsic(__stosq)
#define MPN_FFT_STORE(d, l, v) __stosq(d, v, l)
#else
#define MPN_FFT_STORE(dst, n,d)  mpn_store(dst,n,d)
#endif

#if defined( _MSC_VER ) && defined( _M_X64 )
#include <intrin.h>
#pragma intrinsic(__movsq)
#define MPN_FFT_COPY(d, s, l) __movsq(d, s, l)
#else
#define MPN_FFT_COPY(dst, src, n) MPN_COPY(dst,src,n)
#endif


/* If LOG2_GMP_NUMB_BITS is defined, GMP_NUMB_BITS=2^LOG2_GMP_NUMB_BITS;
   this enables to speed up multiplication or division by GMP_NUMB_BITS. */
#if (GMP_NUMB_BITS == 32)
#define LOG2_GMP_NUMB_BITS 5
#elif (GMP_NUMB_BITS == 64)
#define LOG2_GMP_NUMB_BITS 6
#endif

static inline mp_size_t mpn_mul_fft_lcm (mp_size_t, mp_size_t);

/* quotient, remainder, product by GMP_NUMB_BITS */
#ifdef LOG2_GMP_NUMB_BITS
#define MOD_GMP_NUMB_BITS(x) ((x) & ((1 << LOG2_GMP_NUMB_BITS) - 1))
#define DIV_GMP_NUMB_BITS(x) ((x) >> LOG2_GMP_NUMB_BITS)
/* x <- y / (2 * GMP_NUMB_BITS), y <- y % (2 * GMP_NUMB_BITS) */
#define DIVMOD_2GMP_NUMB_BITS(x,y)     \
  x = (y) >> (LOG2_GMP_NUMB_BITS + 1); \
  y = (y) & ((1 << (LOG2_GMP_NUMB_BITS + 1)) - 1)
#define MUL_GMP_NUMB_BITS(x) ((x) << LOG2_GMP_NUMB_BITS)
#define MUL_2GMP_NUMB_BITS(x) ((x) << (LOG2_GMP_NUMB_BITS + 1))
#define MUL_4GMP_NUMB_BITS(x) ((x) << (LOG2_GMP_NUMB_BITS + 2))
#define LCM_GMP_NUMB_BITS(x) (((x) > LOG2_GMP_NUMB_BITS) ? (1<<(x)) : GMP_NUMB_BITS)
#else
#define MOD_GMP_NUMB_BITS(x) ((x) % GMP_NUMB_BITS)
#define DIV_GMP_NUMB_BITS(x) ((x) / GMP_NUMB_BITS)
#define DIVMOD_2GMP_NUMB_BITS(x,y) \
  x = (y) / (2 * GMP_NUMB_BITS);   \
  y = (y) - (x) * (2 * GMP_NUMB_BITS)
#define MUL_GMP_NUMB_BITS(x) ((x) * GMP_NUMB_BITS)
#define MUL_2GMP_NUMB_BITS(x) ((x) * (2 * GMP_NUMB_BITS))
#define MUL_4GMP_NUMB_BITS(x) ((x) * (4 * GMP_NUMB_BITS))
/* lcm(GMP_NUMB_BITS, 2^x) */
#define LCM_GMP_NUMB_BITS(x) mpn_mul_fft_lcm (GMP_NUMB_BITS, x)
#endif

#define ONE CNST_LIMB(1)

FFT_TABLE_ATTRS mp_size_t mpn_fft_table[2][MPN_FFT_TABLE_SIZE] =
{
  MUL_FFT_TABLE,
  SQR_FFT_TABLE
};

static int mpn_mul_fft_internal
_PROTO ((mp_ptr, mp_size_t, mp_srcptr, mp_size_t, mp_srcptr, mp_size_t, mp_size_t,
         mp_ptr *, mp_ptr *, mp_ptr, mp_ptr, mp_size_t, mp_size_t, 
         mp_size_t **, mp_ptr, mp_size_t, int));

int
mpn_mul_fft_aux (mp_ptr op, mp_size_t pl,
		 mp_srcptr n, mp_size_t nl,
		 mp_srcptr m, mp_size_t ml,
		 mp_size_t k, int b);


/* Find the best k to use for a mod 2^(m*GMP_NUMB_BITS)+1 FFT for m >= n.
   sqr==0 if for a multiply, sqr==1 for a square.
   Don't declare it static since it is needed by tuneup.
*/
#ifdef MUL_FFT_TABLE2
#define MPN_FFT_TABLE2_SIZE 256

struct nk {
  mp_size_t n;
  unsigned char k;
};

static struct nk mpn_fft_table2[4][MPN_FFT_TABLE2_SIZE] =
{
  MUL_FFT_TABLE2,
  SQR_FFT_TABLE2,
  MUL_FFTM_TABLE2,
  SQR_FFTM_TABLE2
};

/* 
   sqr_b = 0: plain multiplication mod 2^N+1
   sqr_b = 1: square mod 2^N+1
   sqr_b = 2: plain multiplication mod 2^N-1
   sqr_b = 3: square mod 2^N-1
 */
int
mpn_fft_best_k (mp_size_t n, int sqr_b)
{
  struct nk *tab;
  int last_k;

  last_k = 4;
  for (tab = mpn_fft_table2[sqr_b] + 1; ; tab++)
    {
      if (n < tab->n)
	break;
      last_k = tab->k;
    }
  return last_k;
}
#else
int
mpn_fft_best_k (mp_size_t n, int sqr)
{
  int i;

  for (i = 0; mpn_fft_table[sqr&1][i] != 0; i++)
    if (n < mpn_fft_table[sqr&1][i])
      return i + FFT_FIRST_K;
  /* treat 4*last as one further entry */
  if (i == 0 || n < 4 * mpn_fft_table[sqr&1][i - 1])
    return i + FFT_FIRST_K;
  else
    return i + FFT_FIRST_K + 1;
}
#endif

#ifdef MUL_FFT_FULL_TABLE2
#define MPN_FFT_FULL_TABLE2_SIZE 256

#ifndef SQR_FFT_FULL_TABLE2
#define SQR_FFT_FULL_TABLE2 MUL_FFT_FULL_TABLE2
#endif

static struct nk mpn_fft_full_table2[4][MPN_FFT_FULL_TABLE2_SIZE] =
  {
    MUL_FFT_FULL_TABLE2,
    SQR_FFT_FULL_TABLE2
  };

int
mpn_fft_best_a (mp_size_t pl, int sqr)
{
  struct nk *tab;
  int last_a;

  last_a = 1;
  for (tab = mpn_fft_full_table2[sqr] + 1; ; tab++)
    {
      if (pl < tab->n)
	break;
      last_a = tab->k;
    }
  return last_a;
}
#endif /* MUL_FFT_FULL_TABLE2 */

/* Returns smallest possible number of limbs >= pl for a fft of size 2^k,
   i.e. smallest multiple of 2^k >= pl.

   Don't declare static: needed by tuneup.
*/

mp_size_t
mpn_fft_next_size (mp_size_t pl, int k)
{
  pl = 1 + ((pl - 1) >> k); /* ceil (pl/2^k) */
  return pl << k;
}

/* Initialize l[i][j] with bitrev(j) */

static void
mpn_fft_initl (mp_size_t **l, mp_size_t k)
{
  mp_size_t i, j, K;
  mp_size_t *li;

  l[0][0] = 0;
  for (i = 1, K = 1; i <= k; i++, K *= 2)
    {
      li = l[i];
      for (j = 0; j < K; j++)
	{
	  li[j] = 2 * l[i - 1][j];
	  li[K + j] = 1 + li[j];
	}
    }
}

#ifndef HAVE_NATIVE_mpn_lshiftc
/* Shift {up, n} cnt bits to the left, store the complemented result
   in {rp, n}, and output the shifted bits (not complemented).
   Same as:
     cc = mpn_lshift (rp, up, n, cnt);
     mpn_com_n (rp, rp, n);
     return cc;

  Assumes n >= 1 and 1 <= cnt < GMP_NUMB_BITS.
  {rp, n} and {up, n} may overlap, provided rp >= up (like mpn_lshift).
*/
static mp_limb_t
mpn_lshiftc (mp_ptr rp, mp_srcptr up, mp_size_t n, unsigned int cnt)
{
  mp_limb_t high_limb, low_limb;
  unsigned int tnc;
  mp_size_t i;
  mp_limb_t retval;

  ASSERT(n >= 1);
  ASSERT(1 <= cnt && cnt < GMP_NUMB_BITS);

  up += n;
  rp += n;

  tnc = GMP_NUMB_BITS - cnt;
  low_limb = *--up;
  retval = low_limb >> tnc;
  high_limb = (low_limb << cnt);

  for (i = n - 1; i != 0; i--)
    {
      low_limb = *--up;
      *--rp = (~(high_limb | (low_limb >> tnc))) & GMP_NUMB_MASK;
      high_limb = low_limb << cnt;
    }
  *--rp = (~high_limb) & GMP_NUMB_MASK;

  return retval;
}
#endif


/* Given ap[0..n] with ap[n]<=1, reduce it modulo 2^(n*GMP_NUMB_BITS)+1,
   by subtracting that modulus if necessary.

   If ap[0..n] is exactly 2^(n*GMP_NUMB_BITS) then mpn_sub_1 produces a
   borrow and the limbs must be zeroed out again.  This will occur very
   infrequently.  */

static inline void
mpn_fft_normalize (mp_ptr ap, mp_size_t n)
{
  if (ap[n] != 0)
    {
      MPN_DECR_U (ap, n + 1, ONE);
      if (ap[n] == 0)
	{
	  /* This happens with very low probability; we have yet to trigger it,
	     and thereby make sure this code is correct.  */
	  MPN_FFT_ZERO (ap, n);
	  ap[n] = 1;
	}
      else
	ap[n] = 0;
    }
}

/* r <- a*2^d mod 2^(n*GMP_NUMB_BITS)+1 with a = {a, n+1}
   Assumes a is semi-normalized, i.e. a[n] <= 1.
   r and a must have n+1 limbs, and not overlap.
*/
static void
mpn_fft_mul_2exp_modF (mp_ptr r, mp_srcptr a, mp_size_t d, mp_size_t n)
{
  int sh, negate;
  mp_limb_t cc, rd;

  ASSERT(d < 2 * n * GMP_NUMB_BITS);

  sh = MOD_GMP_NUMB_BITS(d);
  d  = DIV_GMP_NUMB_BITS(d);
  negate = d >= n;

  if (negate)
    {
      d -= n;
      /* r[0..d-1]  <--  lshift(a[n-d]..a[n-1], sh)
	 r[d..n-1]  <-- -lshift(a[0]..a[n-d-1], sh) */
      if (sh != 0)
	{
	  /* no out shift below since a[n] <= 1 */
	  mpn_lshift (r, a + n - d, d + 1, sh);
	  rd = r[d];
	  cc = mpn_lshiftc (r + d, a, n - d, sh);
	}
      else
	{
#ifdef COUNT_ZEROCOPY
	  printf ("mpn_fft_mul_2exp_modF: MPN_FFT_COPY 1 with %d limbs\n", d);
#endif
	  /* Executed 256 times for 1000000 limb mpn_mul_n, each 
             d in [0, 255] appears exactly once */
	  MPN_COPY (r, a + n - d, d);
	  rd = a[n];
	  mpn_com_n (r + d, a, n - d);
	  cc = 0;
	}

      /* add cc to r[0], and add rd to r[d] */

      /* now add 1 in r[d], subtract 1 in r[n], i.e. add 1 in r[0] */

      r[n] = 0;
      /* cc < 2^sh <= 2^(GMP_NUMB_BITS-1) thus no overflow here */
      mpn_incr_u (r, cc + 1);

      rd ++;
      /* rd might overflow when sh=GMP_NUMB_BITS-1 */
      cc = (rd == 0) ? ONE : rd;
      r = r + d + (rd == 0);
      mpn_incr_u (r, cc);

      return;
    }

  /* if negate=0,
	r[0..d-1]  <-- -lshift(a[n-d]..a[n-1], sh)
	r[d..n-1]  <-- lshift(a[0]..a[n-d-1],  sh)
  */
  if (sh != 0)
    {
      /* no out bits below since a[n] <= 1 */
      mpn_lshiftc (r, a + n - d, d + 1, sh);
      rd = ~r[d];
      /* {r, d+1} = {a+n-d, d+1} << sh */
      cc = mpn_lshift (r + d, a, n - d, sh); /* {r+d, n-d} = {a, n-d}<<sh */
    }
  else
    {
      /* r[d] is not used below, but we save a test for d=0 */
      mpn_com_n (r, a + n - d, d + 1);
      rd = a[n];
#ifdef COUNT_ZEROCOPY
      printf ("mpn_fft_mul_2exp_modF: MPN_FFT_COPY 2 with %d limbs\n", n-d);
#endif
      /* Executed 888 times for 1000000 limb mpn_mul_n, 
	 each n-d in [4, 255] appears 20 times, except
         n-d == 32 which appears 3860 times */

      MPN_COPY (r + d, a, n - d);
      cc = 0;
    }

  /* now complement {r, d}, subtract cc from r[0], subtract rd from r[d] */

  /* if d=0 we just have r[0]=a[n] << sh */
  if (d != 0)
    {
      /* now add 1 in r[0], subtract 1 in r[d] */
      if (cc-- == 0) /* then add 1 to r[0] */
	cc = mpn_add_1 (r, r, n, ONE);
      cc = mpn_sub_1 (r, r, d, cc) + ONE;
      /* add 1 to cc instead of rd since rd might overflow */
    }

  /* now subtract cc and rd from r[d..n] */

  r[n] = (mp_limb_t) (-(mp_limb_signed_t) mpn_sub_1 (r + d, r + d, n - d, cc));
  r[n] -= mpn_sub_1 (r + d, r + d, n - d, rd);
  if (r[n] & GMP_LIMB_HIGHBIT)
    r[n] = mpn_add_1 (r, r, n, ONE);
}

/* r <- a*2^d mod 2^(n*GMP_NUMB_BITS)+1 with a = {a, n+1}, r = {r, n+1}.
   Assumes a is semi-normalized, i.e. a[n] <= 1.
   Assumes 0 < d < GMP_NUMB_BITS.
   Returns r semi-normalized.
   r and a must have n+1 limbs, and may overlap (if so, r=a). */
static void
mpn_fft_mul_2exp_modFa (mp_ptr r, mp_srcptr a, mp_size_t d, mp_size_t n)
{
  mp_limb_t c;

  ASSERT (0 < d && d < GMP_NUMB_BITS);
  c = (a[n] << d) | mpn_lshift (r, a, n, d);
  /* now subtract c from r[0] */
  r[n] = r[0] < c;
  MPN_DECR_U (r, n + 1, c - r[n]);
}

/* r <- a+b mod 2^(n*GMP_NUMB_BITS)+1.
   Assumes a and b are semi-normalized, and so is the result r.
*/
static inline void
mpn_fft_add_modF (mp_ptr r, mp_srcptr a, mp_srcptr b, mp_size_t n)
{
  mp_limb_t c;

  c = a[n] + b[n] + mpn_add_n (r, a, b, n); /* 0 <= c <= 3 */
  r[n] = r[0] < c;
  MPN_DECR_U (r, n + 1, c - r[n]);
}

/* r <- a-b mod 2^(n*GMP_NUMB_BITS)+1.
   Assumes a and b are semi-normalized.
*/
static inline void
mpn_fft_sub_modF (mp_ptr r, mp_srcptr a, mp_srcptr b, mp_size_t n)
{
  mp_limb_t c;

  c = a[n] - b[n] - mpn_sub_n (r, a, b, n); /* -2 <= c <= 1 */
  r[n] = c == 1;
  MPN_INCR_U (r, n + 1, r[n] - c);
}

#ifdef _MSC_VER
#pragma optimize( "", off )
#endif
/* r <- (a-b)*B^d mod B^n+1, where B=2^GMP_NUMB_BITS.
   Assumes a and b are semi-normalized.
   It is equivalent to:
   mpn_fft_sub_modF (tmp, a, b, n);
   mpn_fft_mul_2exp_modF (r, tmp, d * GMP_NUMB_BITS, n);
*/
static inline void
mpn_fft_lshsub_modF (mp_ptr r, mp_srcptr a, mp_srcptr b, mp_size_t d,
		     mp_size_t n)
{
  ASSERT (0 <= d && d < 2 * n);

  if (d >= n) /* (a-b)*B^d = (b-a)*B^(d-n) */
    {
      mp_srcptr t;

      t = a;
      a = b;
      b = t;
      d -= n;
    }

  if (d == 0)
    mpn_fft_sub_modF (r, a, b, n);
  else
    {
      mp_limb_t cc;

      /* let a = ah * B^(n-d) + al
	 and b = bh * B^(n-d) + bl,
	 where ah, bh have d limbs + 1 bit, and al, bl have n-d limbs.
	 Then (a-b)*B^d = (al-bl) * B^d + (bh-ah). */

      ASSERT (0 < d && d < n);
      cc = mpn_sub_n (r, b + n - d, a + n - d, d); /* bh-ah */
#ifdef HAVE_NATIVE_mpn_sub_nc
      cc = mpn_sub_nc (r + d, a, b, n - d, cc);    /* al-bl */
#else
      cc = mpn_sub_n (r + d, a, b, n - d)
         + mpn_sub_1 (r + d, r + d, n - d, cc);
#endif
      /* 0 <= cc <= 1 */
      if (a[n] > b[n])
	cc += mpn_sub_1 (r + d, r + d, n - d, a[n] - b[n]); /* 0 <= cc <= 2 */
      else
	cc -= mpn_add_1 (r + d, r + d, n - d, b[n] - a[n]); /* -1 <= cc <= 1 */
      /* -1 <= cc <= 2 */
      /* cc is the borrow at r[n], which must be added at r[0] */
      r[n] = cc >> (GMP_NUMB_BITS - 1);
      MPN_INCR_U (r, n + 1, cc + r[n]);
    }
}
#ifdef _MSC_VER
#pragma optimize( "", on )
#endif

/* r <- a*sqrt(2)^d mod 2^(n*GMP_NUMB_BITS)+1 with a = {a, n+1}
   Assumes a is semi-normalized, i.e. a[n] <= 1.
   Assumes 0 < d < 4*n*GMP_NUMB_BITS.
   r and a must have n+1 limbs, and not overlap.
   Calls mpn_fft_mul_2exp_modF() and mpn_fft_sub_modF().
*/
static void
mpn_fft_mul_sqrt2exp_modF (mp_ptr r, mp_srcptr a, mp_size_t d, mp_size_t n)
{
  mp_size_t e = d >> 1;
  mp_size_t N = MUL_GMP_NUMB_BITS(n); /* n * GMP_NUMB_BITS */
  mp_ptr tmp;
  mp_srcptr b;
  mp_limb_t ca, cc;
  mp_size_t l;
  TMP_DECL;

  ASSERT(0 < d && d < 4 * N);
  ASSERT(a != r);

#ifdef NO_SQRT_2
  ASSERT_ALWAYS(d % 2 == 0);
#endif

  /* If d is even, we have a regular multiplication by a power of 2 */
  if ((d & 1) == 0)
    { /* since d cannot be zero, e cannot be zero too */
      if (e < GMP_NUMB_BITS)
	mpn_fft_mul_2exp_modFa (r, a, e, n);
      else
	mpn_fft_mul_2exp_modF (r, a, e, n);
      return;
    }
  
  ASSERT(N % 4 == 0);
  /* Multiply by sqrt(2) * 2^e = (2^(3N/4) - 2^(N/4)) * 2^e =
     2^(3N/4 + e) - 2^(N/4 + e) */
  e += 3 * (N >> 2); /* 3N/4 <= e < 11N/4 */
  if (e >= 2 * N)
    e -= 2 * N; /* 0 <= e < 2N */
  TMP_MARK;
  tmp = TMP_ALLOC_LIMBS(n + 1);
  /* the following variant avoids the -H-L computation, which
     requires a call to mpn_com_n(). */
  if (e != 0)
    {
      mpn_fft_mul_2exp_modF (r, a, e, n);        /* a*2^(e+N/2) */
      b = r;
    }
  else
    b = a;
  l = n >> 1;
  if ((n & 1) != 0)
    {
      mpn_lshift (tmp, b, n + 1, GMP_NUMB_BITS >> 1);
#ifdef HAVE_NATIVE_mpn_sumdiff_n  /* avoid the copy with mpn_addsub_n */
      ca = b[n] + mpn_add_n (r + l, b + l, (n & 1) ? tmp : b, n - l);
      cc = tmp[n] + mpn_sub_n (r, b, tmp + n - l, l);
    }
  else
    {
      /* rh <- bl + bh, rl <- bl - bh */
      ca = mpn_sumdiff_n (r + l, r, b, b + l, l);
      cc = b[n] + (ca & 1);
      ca = b[n] + (ca >> 1);
    }
#else /* native mpn_sumdiff_n not available */
    }
  else
    MPN_COPY (tmp + n - l, b + n - l, l + 1);
  /* we still have to shift {tmp, n+1} by l limbs to the left:
     let tl = {tmp, n-l} and th = {tmp+n-l,l+1} */
  /* rh <- bh + tl, rl <- bl - th */
  ca = b[n] + mpn_add_n (r + l, b + l, (n & 1) ? tmp : b, n - l);
  cc = tmp[n] + mpn_sub_n (r, b, tmp + n - l, l);
#endif /* HAVE_NATIVE_mpn_sumdiff_n */
  cc = mpn_sub_1 (r + l, r + l, n - l, cc);
  /* We must subtract 0 <= ca <= 2 and add 0 <= cc <= 1 at r[0].
     If cc >= ca: r[n]=0 and add cc - ca.
     If cc < ca: r[n]=1 and subtract ca-cc-1. */
  r[n] = cc < ca;
  if (cc >= ca)
    MPN_INCR_U (r, n + 1, cc - ca);
  else /* cc < ca */
    MPN_DECR_U (r, n + 1, ca - ONE - cc);
  TMP_FREE;
}

/* normalize {n, nl} mod 2^(Kl*GMP_NUMB_BITS)+b and store in tmp.
   tmp must have space for Kl + 1 limbs */
static void 
mpn_mul_fft_reduce (mp_ptr tmp, mp_srcptr n, mp_size_t nl, mp_size_t Kl, int b)
{
    mp_size_t dif = nl - Kl;
    mp_limb_signed_t cy;

    if (dif > Kl)
    {
	int subp = 0;
	
	cy = ((b == 1) ? mpn_sub_n : mpn_add_n) (tmp, n, n + Kl, Kl);
	n += 2 * Kl;
	dif -= Kl;
	
	/* now dif > 0 */
	while (dif > Kl)
	{
	    if (b == -1)
		cy += mpn_add_n (tmp, tmp, n, Kl);
	    else if (subp)
		cy += mpn_sub_n (tmp, tmp, n, Kl);
	    else
		cy -= mpn_add_n (tmp, tmp, n, Kl);
	    subp ^= 1;
	    n += Kl;
	    dif -= Kl;
	}
	/* now dif <= Kl */
	if (b == -1)
	    cy += mpn_add (tmp, tmp, Kl, n, dif);
	else if (subp)
	    cy += mpn_sub (tmp, tmp, Kl, n, dif);
	else
	    cy -= mpn_add (tmp, tmp, Kl, n, dif);
	if (cy >= 0)
	    cy = mpn_add_1 (tmp, tmp, Kl, cy);
	else
	    cy = mpn_sub_1 (tmp, tmp, Kl, -cy);
    }
    else /* dif <= Kl, i.e. nl <= 2 * Kl */
    {
	cy = ((b == 1) ? mpn_sub : mpn_add) (tmp, n, Kl, n + Kl, dif);
	cy = mpn_add_1 (tmp, tmp, Kl, cy);
    }
    tmp[Kl] = cy;
}


/* Store in {A+(nprime + 1) * offset, nprime+1} the first l limbs 
   (with zero padding) from {n + l*offset, ...},
   and in {A + (nprime+1)*(offset + 1<<stride), nprime+1} the first l
   limbs from {n + l*(offset+1<<stride), ...}, and generally
   in {A + (nprime+1)*(offset + i<<stride), nprime+1} the first l
   limbs from {n + l*(offset + i<<stride), ...}, for 
   offset + i<<stride < K.

   A must have space for at least (nprime + 1)*K limbs.
   T must have space for at least (nprime + 1) limbs.
   Adds weight signal for negacyclic convolution to result if b=1 
     (nothing if b=-1).
   We need Mp so that ord(sqrt(2)^Mp) == K, with K = 2^k.
*/
static void
mpn_mul_fft_decompose (mp_ptr A, mp_ptr *Ap, mp_size_t K, mp_size_t offset, 
		       mp_size_t stride, mp_size_t nprime, mp_srcptr n, 
		       mp_size_t nl, mp_size_t l, mp_size_t Mp, mp_ptr T, int b)
{
  mp_size_t i, j;
  mp_ptr tmp;
  const mp_size_t Kl = K * l;
  TMP_DECL;
  TMP_MARK;

  ASSERT(b == 1 || b == -1);
  ASSERT(Mp * K == 4 * nprime * GMP_LIMB_BITS);
  ASSERT(b == -1 || Mp % 2 == 0);

  if (nl > Kl + 1)
    {
      /* FIXME: We really don't want to do this multiple times if
         stride > 0 ! */
      TRACE(printf ("mpn_mul_fft_decompose: This takes too long!\n");)
      tmp = TMP_ALLOC_LIMBS(Kl + 1);
      mpn_mul_fft_reduce (tmp, n, nl, Kl, b);
      n = tmp;
      nl = Kl + 1;
    }

  A += (nprime + 1) * offset;
  n += l * offset;
  nl -= (l * offset < nl) ? l * offset : nl;
  /* for b=1, since we use {T, nprime+1} as temporary array below,
     and only the first l limbs may be non-zero, except for the last
     part, we can set {T+l, nprime+1-l} to zero now. */
  MPN_FFT_ZERO (T + l, nprime + 1 - l);

  for (i = offset; i < K; i += 1 << stride)
    {
      Ap[i] = A;
      /* store the next l limbs of n into A[0..nprime] */

      /* nl == 0  =>  j == 0, nl unchanged */
      j = (l <= nl && i < K - 1) ? l : nl; /* store j next limbs */
      nl -= j;
      nl -= (nl > (l << stride) - l) ? (l << stride) - l : nl;
      if (b == 1 && i != 0 && j > 0)
	{
	  /* add weight signal for negacyclic convolution. We need a 
	     root of unity here whose order is twice the transform
	     length K. Since ord(sqrt(2)^Mp) = K, sqrt(2)^(Mp/2) 
	     will do, so long as Mp is even. */
#define FORCE_EXPENSIVE_DECOMPOSE 0
	    if ((FORCE_EXPENSIVE_DECOMPOSE) || (i & (Mp / 2) & 1))
	    {
#ifdef COUNT_ZEROCOPY
	      printf ("mpn_mul_fft_decompose: MPN_FFT_COPY 1 with %d limbs\n", 
		      j);
#endif
	      MPN_FFT_COPY (T, n, j);
	      ASSERT_ALWAYS (j <= l + 1);
	      if (j < l)
		MPN_FFT_ZERO (T + j, l - j);
	      mpn_fft_mul_sqrt2exp_modF (A, T, i * (Mp / 2), nprime);
	    } 
	  else 
	    {
	      /* i * Mp / 2 is even, so weight signal is 
		 sqrt(2)^(i * Mp / 2) = 2^(i * Mp / 4).
                 Shift directly into A. */
	      const mp_size_t c = (i * Mp) / 4;
	      const mp_size_t d = c % GMP_NUMB_BITS;
	      const mp_size_t e = c / GMP_NUMB_BITS;

#undef DECOMPOSE_CAREFUL_CHECK
#ifdef DECOMPOSE_CAREFUL_CHECK
                      /* Do it the expensive way and store result 
			 in T for comparison */
                      MPN_FFT_COPY (T, n, j);
                      ASSERT_ALWAYS (j <= l + 1);
                      if (j < l)
		        MPN_FFT_ZERO (T + j, l - j);
                      mpn_fft_mul_2exp_modF (A, T, c, nprime);
		      MPN_COPY (T, A, nprime + 1);
#endif

	      /* Copy data from n to A+e, shifted by d bits. */
	      if (e + j < nprime || (e + j == nprime && d <= 1)) 
		{
		  /* The shifted data fits without wrapping */
		  MPN_FFT_ZERO (A, e);
		  if (d == 0)
		    {
		      MPN_COPY(A + e, n, j);
		      MPN_FFT_ZERO (A + e + j, nprime + 1 - e - j);
		    }
		  else
		    {
		      A[e + j] = mpn_lshift (A + e, n, j, d);
		      /* Now zero A[e + j + 1 ... nprime] */
		      MPN_FFT_ZERO (A + e + j + 1, nprime - e - j);
		    }
		}
	      else
		{ 
		  const int of = j + e - nprime;

                  if (d == 0)
                    {
		      /* Here, e + j > nprime, i.e. there is wrapping
		         but d == 0, so no bit shifting */
		      mp_limb_t cc;

		      ASSERT(e + j > nprime); /* Hence of > 0 */

		      /* Store ~(N_hi) to A[0 ... of[ */
		      mpn_com_n (A, n + nprime - e, of);
		      cc = mpn_add_1 (A, A, of, ONE);

		      MPN_FFT_STORE (A + of, nprime - j, cc - ONE);

		      /* Store n_lo * w^e */
		      ASSERT(nprime - e > 0);
		      cc = mpn_sub_1 (A + e, n, nprime - e, ONE - cc);
		      A[nprime] = 0;
		      MPN_INCR_U (A, nprime + 1, cc);
                    } 
		  else 
		    {
		      /* Here, e + j >= nprime and d != 0 */
		      mp_limb_t cc;
		      
                      /* We want n*2^i with i < nprime*w, i > (nprime-j)*w, 
			 Store nprime-e words, shifted left by d, at A+e.
		      */

		      cc = mpn_lshift (A + e, n, nprime - e, d);
		      A[nprime] = 0;
                      
		      if (of > 0)
			{
			  /* Store a_hi to A[0 ... of] */
			  A[of] = mpn_lshift (A, n + nprime - e, of, d);
			  A[0] |= cc;
			  /* And do binary negate */
			  mpn_not (A, of + 1);
			  cc = mpn_add_1 (A, A, of + 1, ONE);
			}
		      else
		        {
			  A[0] = (mp_limb_t)(-(mp_limb_signed_t)cc);
			  cc = (cc == 0);
		        }

		      /* Store cc-1 to A[of+1 ... e[ */
		      MPN_FFT_STORE (A + of + 1, nprime - j - 1, 
				     cc - ONE);

		      cc = mpn_sub_1 (A + e, A + e, nprime - e, ONE - cc);
		      MPN_INCR_U (A, nprime + 1, cc);
                    }
		}
#ifdef DECOMPOSE_CAREFUL_CHECK
	      ASSERT(A[nprime] <= 1);
	      if (A[nprime] == 1)
	      {
		  /* Fully normalize for the sake of the 
		     following comparison */
		  mp_limb_t cc;
		  cc = mpn_sub_1 (A, A, nprime, 1);
		  A[nprime] = 0;
		  mpn_add_1 (A, A, nprime + 1, cc);
	      }
	      if (mpn_cmp (A, T, nprime + 1) != 0)
	      {
		  printf ("nprime = %d, i = %d, j = %d, d = %d, "
			  "e = %d\n", nprime, i, j, d, e);
		  for (i = 0; i < nprime + 1; i++)
		      printf ("%d: %lx %lx %c\n", i, A[i], T[i],
			      (A[i] != T[i]) ? '!' : ' ');
		  abort ();
	      }
	      MPN_ZERO (T, nprime + 1);
#endif
	    }
	}
      else /* b = -1 or i == 0 or j == 0. No weight to be added here. */
	{
#ifdef COUNT_ZEROCOPY
	  printf ("mpn_mul_fft_decompose: MPN_FFT_COPY 2 with %d limbs\n", 
		      j);
#endif
	  MPN_COPY (A, n, j);
	  MPN_FFT_ZERO (A + j, nprime + 1 - j);
	}

      ASSERT(A[nprime] <= 1);
      n += l << stride;
      A += (nprime + 1) << stride;
    }
  ASSERT_ALWAYS (nl == 0 || (nl == 1 && stride > 0 && offset == 0));
  TMP_FREE;
}


/*
   A0 <-  A0+A1
   A1 <- (A0-A1)*2^e0
   
   Butterfly using a rotating buffer instead of temp space.
   The variable rotbuf is a size-1 array of coefficients; this might be
   exchanged with one of the coefficients of A.
*/

static inline void
mpn_fft_butterfly_rotbuf (mp_ptr *A, mp_size_t i0, mp_size_t i1,
			  mp_size_t e0, mp_ptr *rotbuf, mp_size_t n)
{
  mp_size_t d, e = e0;

  ASSERT(e0 != 0);
  DIVMOD_2GMP_NUMB_BITS(d, e); /* 0 <= d < 2*n, 0 <= e0 < 2*GMP_NUMB_BITS */
  mpn_fft_lshsub_modF (rotbuf[0], A[i0], A[i1], d, n);
  mpn_fft_add_modF (A[i0], A[i0], A[i1], n);
  if (e != 0)
      mpn_fft_mul_sqrt2exp_modF (A[i1], rotbuf[0], e, n);
  else
    {
      mp_ptr tmp = rotbuf[0];
      rotbuf[0] = A[i1];
      A[i1] = tmp;
    }
}

static inline void
mpn_fft_butterfly_rotbuf0 (mp_ptr *A, mp_size_t i0, mp_size_t i1,
			   mp_ptr *rotbuf, mp_size_t n)
{
  mp_ptr tmp;
  mpn_fft_sub_modF (rotbuf[0], A[i0], A[i1], n);
  mpn_fft_add_modF (A[i0], A[i0], A[i1], n);
  tmp = rotbuf[0];
  rotbuf[0] = A[i1];
  A[i1] = tmp;
}

/*
  In this version, the shift e0 is in [0..N], so we have to do one more test on e0.
*/

static inline void
mpn_fft_butterfly_rotbufN (mp_ptr *A, mp_size_t i0, mp_size_t i1,
			   mp_size_t e0, mp_ptr *rotbuf, mp_size_t n)
{
  mp_size_t N = MUL_4GMP_NUMB_BITS(n); /* 4 * n * GMP_NUMB_BITS */
  mp_size_t d;

  if (e0 >= N)
    e0 -= N;
  DIVMOD_2GMP_NUMB_BITS (d,e0); /* 0 <= d < 2*n, 0 <= e0 < 2*GMP_NUMB_BITS */
  mpn_fft_lshsub_modF (rotbuf[0], A[i0], A[i1], d, n);
  mpn_fft_add_modF (A[i0], A[i0], A[i1], n);
  if (e0 != 0)
    mpn_fft_mul_sqrt2exp_modF (A[i1], rotbuf[0], e0, n);
  else
    {
      mp_ptr tmp = rotbuf[0];
      rotbuf[0] = A[i1];
      A[i1] = tmp;
    }
}

/* 
   Radix 4 transform.
   This uses a rotating buffer: the array Ap gets unsorted (but we
   usually don't care).
*/

static void
mpn_fft_fft_radix4Rec (mp_ptr *Ap, mp_size_t ind_start, mp_size_t k,
    mp_size_t omega, mp_size_t n, mp_ptr *rotbuf)
{
  mp_size_t i, stride, stride2, K;
  
  K = 1<<k;
  stride2 = 1<<(k-2);
  stride = 1<<(k-1);

  if (k == 1) {
    mpn_fft_butterfly_rotbuf0(Ap, ind_start, ind_start+1, rotbuf, n);
    return;
  }

  mpn_fft_butterfly_rotbuf0 (Ap, ind_start, ind_start+stride, rotbuf, n);
  mpn_fft_butterfly_rotbuf (Ap, ind_start+stride2, ind_start+stride+stride2,
			    omega*stride2, rotbuf, n);

  mpn_fft_butterfly_rotbuf0 (Ap, ind_start+stride, ind_start+stride+stride2,
			     rotbuf, n);
  mpn_fft_butterfly_rotbuf0 (Ap, ind_start, ind_start+stride2, rotbuf, n);

  for (i = 1; i < stride2; ++i) {
    mpn_fft_butterfly_rotbuf(Ap, ind_start+i, ind_start+i+stride, omega*i, rotbuf, n);
    mpn_fft_butterfly_rotbuf(Ap, ind_start+i+stride2, ind_start+i+stride+stride2,
	omega*(i+stride2), rotbuf, n);

    mpn_fft_butterfly_rotbuf(Ap, ind_start+i+stride, ind_start+i+stride+stride2,
	omega*i*2, rotbuf, n);
    mpn_fft_butterfly_rotbuf(Ap, ind_start+i, ind_start+i+stride2, omega*i*2, rotbuf, n);
  }

  if (k == 3) {
    mpn_fft_butterfly_rotbuf0(Ap, ind_start+stride+stride2, ind_start+stride+stride2+1, rotbuf, n);
    mpn_fft_butterfly_rotbuf0(Ap, ind_start+stride, ind_start+stride+1, rotbuf, n);
    mpn_fft_butterfly_rotbuf0(Ap, ind_start, ind_start+1, rotbuf, n);
    mpn_fft_butterfly_rotbuf0(Ap, ind_start+stride2, ind_start+stride2+1, rotbuf, n);
  }

  if (k > 3) {
    mp_size_t omega4 = omega<<2;
    mpn_fft_fft_radix4Rec(Ap, ind_start, k-2, omega4, n, rotbuf);
    mpn_fft_fft_radix4Rec(Ap, ind_start+stride2, k-2, omega4, n, rotbuf);
    mpn_fft_fft_radix4Rec(Ap, ind_start+stride, k-2, omega4, n, rotbuf);
    mpn_fft_fft_radix4Rec(Ap, ind_start+stride+stride2, k-2, omega4, n, rotbuf);
  }
}

static void
mpn_fft_fft_radix4 (mp_ptr *Ap, mp_size_t k, mp_size_t omega,
    mp_size_t n, mp_ptr *rotbuf)
{
  mpn_fft_fft_radix4Rec(Ap, 0, k, omega, n, rotbuf);
}

/*
  The "Neg" versions multiply by the *inverse* of the root. This is used for
  the backward transform. Propagating this bit of information saves the %N, since
  only at the end we do N-blah.
  FIXME: The Neg and non-Neg versions can probably be merged at almost no cost.
*/


static void
mpn_fft_fft_radix4RecNeg (mp_ptr *Ap, mp_size_t ind_start, mp_size_t k,
    mp_size_t omega, mp_size_t n, mp_ptr *rotbuf)
{
  mp_size_t i, stride, stride2, K;
  mp_size_t N = MUL_4GMP_NUMB_BITS(n); /* 4 * n * GMP_NUMB_BITS */

  K = 1 << k;
  stride2 = 1 << (k - 2);
  stride  = 1 << (k - 1);

  if (k == 1)
    {
      mpn_fft_butterfly_rotbufN(Ap, ind_start, ind_start+1, 0, rotbuf, n);
      return;
    }

  for (i = 0; i < stride2; ++i) {
    mpn_fft_butterfly_rotbufN(Ap, ind_start+i, ind_start+i+stride, N-omega*i, rotbuf, n);
    mpn_fft_butterfly_rotbufN(Ap, ind_start+i+stride2, ind_start+i+stride+stride2,
	N-omega*(i+stride2), rotbuf, n);

    mpn_fft_butterfly_rotbufN(Ap, ind_start+i+stride, ind_start+i+stride+stride2,
	N-omega*i*2, rotbuf, n);
    mpn_fft_butterfly_rotbufN(Ap, ind_start+i, ind_start+i+stride2, N-omega*i*2, rotbuf, n);
  }

  if (k == 3) {
    mpn_fft_butterfly_rotbufN(Ap, ind_start+stride+stride2, ind_start+stride+stride2+1, 0, rotbuf, n);
    mpn_fft_butterfly_rotbufN(Ap, ind_start+stride, ind_start+stride+1, 0, rotbuf, n);
    mpn_fft_butterfly_rotbufN(Ap, ind_start, ind_start+1, 0, rotbuf, n);
    mpn_fft_butterfly_rotbufN(Ap, ind_start+stride2, ind_start+stride2+1, 0, rotbuf, n);
  }

  if (k > 3) {
    mp_size_t omega4 = omega<<2;
    mpn_fft_fft_radix4RecNeg(Ap, ind_start, k-2, omega4, n, rotbuf);
    mpn_fft_fft_radix4RecNeg(Ap, ind_start+stride2, k-2, omega4, n, rotbuf);
    mpn_fft_fft_radix4RecNeg(Ap, ind_start+stride, k-2, omega4, n, rotbuf);
    mpn_fft_fft_radix4RecNeg(Ap, ind_start+stride+stride2, k-2, omega4, n, rotbuf);
  }
}

static void
mpn_fft_fft_radix4Neg (mp_ptr *Ap, mp_size_t k, mp_size_t omega,
    mp_size_t n, mp_ptr *rotbuf)
{
  mpn_fft_fft_radix4RecNeg(Ap, 0, k, omega, n, rotbuf);
}

static void 
mpn_fft_fft_radix4Inv(mp_ptr *Ap, mp_size_t k, mp_size_t omega,
    mp_size_t n, mp_ptr *rotbuf, mp_size_t **ll)
{
  mp_size_t i;

  /* Bit-reverse table Ap.
     FIXME: these bit-rev copies might be avaoided. But do they really cost? */
  for (i = 0; i < 1<<k; ++i) {
    mp_ptr tmp;
    mp_size_t j = ll[k][i];
    if (i < j) {
      tmp = Ap[i];
      Ap[i] = Ap[j];
      Ap[j] = tmp;
    }
  }
 
  mpn_fft_fft_radix4RecNeg(Ap, 0, k, omega, n, rotbuf);

  /* Bit-reverse table Ap (again!) */
  for (i = 0; i < 1<<k; ++i) {
    mp_ptr tmp;
    mp_size_t j = ll[k][i];
    if (i < j) {
      tmp = Ap[i];
      Ap[i] = Ap[j];
      Ap[j] = tmp;
    }
  }
}


/*
   Twisted variant of radix 4.
   The roots are different; this is used in the first step of Bailey's algorithm.
   This twisted version avoids the normalisation phase (linear cost) of the
   original algorithm.
*/

static void mpn_fft_fftR4_twistedRec(mp_ptr * Ap, mp_size_t ind, mp_size_t k,
    mp_size_t omega, mp_size_t om_curr, mp_size_t om_mult, mp_size_t n, mp_ptr *rotbuf)
{
  const mp_size_t stride = 1<<(k-1);
  const mp_size_t stride2 = 1<<(k-2);
  mp_size_t i;
  
  if (k == 0)
    return;

  ASSERT (omega != 0);

  if (om_mult == 0)
  {
      if (k == 1) {
	  mpn_fft_butterfly_rotbuf0 (Ap, ind, ind + 1, rotbuf, n);
	  return;
      }
      
      mpn_fft_butterfly_rotbuf0 (Ap, ind, ind+stride, rotbuf, n);
      mpn_fft_butterfly_rotbuf (Ap, ind+stride2, ind+stride+stride2, omega*om_curr*stride2, rotbuf, n);
	  
      mpn_fft_butterfly_rotbuf0 (Ap, ind+stride, ind+stride+stride2, rotbuf, n);
      mpn_fft_butterfly_rotbuf0 (Ap, ind, ind+stride2, rotbuf, n);

      for (i = 1; i < stride2; ++i) {
	  mp_size_t root = omega*om_curr*i;
	  mpn_fft_butterfly_rotbuf(Ap, ind+i, ind+stride+i, root, rotbuf, n);
	  root = omega*om_curr*(i+stride2);
	  mpn_fft_butterfly_rotbuf(Ap, ind+i+stride2, ind+stride+stride2+i, root, rotbuf, n);
	  root = 2*omega*om_curr*i;
	  mpn_fft_butterfly_rotbuf(Ap, ind+i+stride, ind+stride+stride2+i, root, rotbuf, n);
	  mpn_fft_butterfly_rotbuf(Ap, ind+i, ind+stride2+i, root, rotbuf, n);
      }
      
      if (k == 3) {
	  mpn_fft_butterfly_rotbuf0 (Ap, ind+stride+stride2, ind+stride+stride2+1, rotbuf, n);
	  mpn_fft_butterfly_rotbuf0 (Ap, ind+stride, ind+stride+1, rotbuf, n);
	  mpn_fft_butterfly_rotbuf0 (Ap, ind, ind+1, rotbuf, n);
	  mpn_fft_butterfly_rotbuf0 (Ap, ind+stride2, ind+stride2+1, rotbuf, n);
      } 
      
      if (k > 3) {
	  mpn_fft_fftR4_twistedRec(Ap, ind, k-2, omega, om_curr<<2, om_mult<<2, n, rotbuf);
	  mpn_fft_fftR4_twistedRec(Ap, ind+stride2, k-2, omega, om_curr<<2, om_mult<<2, n, rotbuf);
	  mpn_fft_fftR4_twistedRec(Ap, ind+stride, k-2, omega, om_curr<<2, om_mult<<2, n, rotbuf);
	  mpn_fft_fftR4_twistedRec(Ap, ind+stride+stride2, k-2, omega, om_curr<<2, om_mult<<2, n, rotbuf);
      }
      return;
  }

  if (k == 1)
    {
      mpn_fft_butterfly_rotbuf (Ap, ind, ind + 1, omega * om_mult, rotbuf, n);
      return;
    }

  for (i = 0; i < stride2; ++i) {
    mp_size_t root = omega*(om_curr*i + om_mult);
    mpn_fft_butterfly_rotbuf(Ap, ind+i, ind+stride+i, root, rotbuf, n);
    root = omega*(om_curr*(i+stride2) + om_mult);
    mpn_fft_butterfly_rotbuf(Ap, ind+i+stride2, ind+stride+stride2+i, root, rotbuf, n);

    mpn_fft_butterfly_rotbuf(Ap, ind+i+stride, ind+stride+stride2+i, omega*(om_curr*i + om_mult)*2, rotbuf, n);
    mpn_fft_butterfly_rotbuf(Ap, ind+i, ind+stride2+i, omega*(om_curr*i + om_mult)*2, rotbuf, n);
  }

  if (k == 3) {
    mp_size_t root = omega*om_mult*4;
    mpn_fft_butterfly_rotbuf(Ap, ind+stride+stride2, ind+stride+stride2+1, root, rotbuf, n);
    mpn_fft_butterfly_rotbuf(Ap, ind+stride, ind+stride+1, root, rotbuf, n);
    mpn_fft_butterfly_rotbuf(Ap, ind, ind+1, root, rotbuf, n);
    mpn_fft_butterfly_rotbuf(Ap, ind+stride2, ind+stride2+1, root, rotbuf, n);
  } 

  if (k > 3) {
    mpn_fft_fftR4_twistedRec(Ap, ind, k-2, omega, om_curr<<2, om_mult<<2, n, rotbuf);
    mpn_fft_fftR4_twistedRec(Ap, ind+stride2, k-2, omega, om_curr<<2, om_mult<<2, n, rotbuf);
    mpn_fft_fftR4_twistedRec(Ap, ind+stride, k-2, omega, om_curr<<2, om_mult<<2, n, rotbuf);
    mpn_fft_fftR4_twistedRec(Ap, ind+stride+stride2, k-2, omega, om_curr<<2, om_mult<<2, n, rotbuf);
  }
}


static void mpn_fft_fftR4_twisted(mp_ptr * Ap, mp_size_t rk, mp_size_t k1, mp_size_t k,
    mp_size_t omega, mp_size_t n, mp_ptr *rotbuf)
{ 
  mpn_fft_fftR4_twistedRec(Ap, 0, k1, omega, 1<<(k-k1), rk, n, rotbuf);
} 
  
/* 
   Neg version for reverse transform.
   (see comments above)
*/

static void mpn_fft_fftR4_twistedRecNeg(mp_ptr * Ap, mp_size_t ind, mp_size_t k,
    mp_size_t omega, mp_size_t om_curr, mp_size_t om_mult, mp_size_t n, mp_ptr *rotbuf)
{
  mp_size_t stride = 1<<(k-1);
  mp_size_t stride2 = 1<<(k-2);
  mp_size_t i;
  mp_size_t N = MUL_4GMP_NUMB_BITS(n); /* 4 * n * GMP_NUMB_BITS */

    
  if (k == 0)
    return;

  if (k == 1)
    {
      mpn_fft_butterfly_rotbufN (Ap, ind, ind + 1, N - omega * om_mult, rotbuf,
				 n);
      return;
    }

  for (i = 0; i < stride2; ++i) {
    mp_size_t root = omega*(om_curr*i + om_mult);
    mpn_fft_butterfly_rotbufN(Ap, ind+i, ind+stride+i, N-root, rotbuf, n);
    root = omega*(om_curr*(i+stride2) + om_mult);
    mpn_fft_butterfly_rotbufN(Ap, ind+i+stride2, ind+stride+stride2+i, N-root, rotbuf, n);

    mpn_fft_butterfly_rotbufN(Ap, ind+i+stride, ind+stride+stride2+i, N-omega*(om_curr*i + om_mult)*2, rotbuf, n);
    mpn_fft_butterfly_rotbufN(Ap, ind+i, ind+stride2+i, N-omega*(om_curr*i + om_mult)*2, rotbuf, n);
  }

  if (k == 3) {
    mp_size_t root = N-omega*om_mult*4;
    mpn_fft_butterfly_rotbufN(Ap, ind+stride+stride2, ind+stride+stride2+1, root, rotbuf, n);
    mpn_fft_butterfly_rotbufN(Ap, ind+stride, ind+stride+1, root, rotbuf, n);
    mpn_fft_butterfly_rotbufN(Ap, ind, ind+1, root, rotbuf, n);
    mpn_fft_butterfly_rotbufN(Ap, ind+stride2, ind+stride2+1, root, rotbuf, n);
  } 

  if (k > 3) {
    mpn_fft_fftR4_twistedRecNeg(Ap, ind, k-2, omega, om_curr<<2, om_mult<<2, n, rotbuf);
    mpn_fft_fftR4_twistedRecNeg(Ap, ind+stride2, k-2, omega, om_curr<<2, om_mult<<2, n, rotbuf);
    mpn_fft_fftR4_twistedRecNeg(Ap, ind+stride, k-2, omega, om_curr<<2, om_mult<<2, n, rotbuf);
    mpn_fft_fftR4_twistedRecNeg(Ap, ind+stride+stride2, k-2, omega, om_curr<<2, om_mult<<2, n, rotbuf);
  }
}


static void mpn_fft_fftR4_twistedNeg(mp_ptr * Ap, mp_size_t rk, mp_size_t k1, mp_size_t k,
    mp_size_t omega, mp_size_t n, mp_ptr *rotbuf)
{ 
  mpn_fft_fftR4_twistedRecNeg(Ap, 0, k1, omega, 1<<(k-k1), rk, n, rotbuf);
} 
  
/*
   Radix-2 version of the previous function. Obsolete, now, but more easy to 
   understand; so I let it here.
*/

static void mpn_fft_fft_twistedRec(mp_ptr * Ap, mp_size_t ind, mp_size_t k,
    mp_size_t omega, mp_size_t om_curr, mp_size_t om_mult, mp_size_t n, mp_ptr *rotbuf)
{
  const mp_size_t stride = 1<<(k-1);
  mp_size_t i;
    
  if (k == 0)
    return;

  for (i = 0; i < stride; ++i) {
    mp_size_t root = (omega*(om_curr*i + om_mult));
    mpn_fft_butterfly_rotbuf(Ap, ind+i, ind+stride+i, root, rotbuf, n);
  }
  mpn_fft_fft_twistedRec(Ap, ind, k-1, omega, om_curr<<1, om_mult<<1, n, rotbuf);
  mpn_fft_fft_twistedRec(Ap, ind+stride, k-1, omega, om_curr<<1, om_mult<<1, n, rotbuf);
}


static void mpn_fft_fft_twisted(mp_ptr * Ap, mp_size_t rk, mp_size_t k1, mp_size_t k,
    mp_size_t omega, mp_size_t n, mp_ptr * rotbuf)
{ 
  mpn_fft_fft_twistedRec(Ap, 0, k1, omega, 1<<(k-k1), rk, n, rotbuf);
} 
  
/* 
   Bailey's algorithm.
   This cuts a 2^k-length transform in 2 blocks. Let k = k1+k2:
      - First do 2^k2 transforms of length 2^k1
      - Then do 2^k1 transforms of length 2^k2
   The small transforms are done using radix 4. We tried radix 8, but this does not seem
   to pay... Also tried cutting into 3 or 4 parts, but this also failed. Maybe have to
   investigate more for very large inputs.
*/
  
  
static void 
mpn_fft_fft_bailey (mp_ptr * Ap, mp_size_t k, mp_size_t omega,
    mp_size_t n, mp_ptr * rotbuf)
{ 
  const mp_size_t k1 = k >> 1;
  const mp_size_t k2 = k - k1;
  mp_size_t i, j;
  const mp_size_t K1 = 1 << k1;
  const mp_size_t K2 = 1 << k2;
  mp_size_t omegai;
  mp_ptr *BufA; 
    
  TMP_DECL;
  TMP_MARK; 
  
  BufA = TMP_ALLOC_MP_PTRS (K1);
  
  for (i = 0; i < K2; ++i) {
    // copy the i-th column of Ap into BufA (pointers... no real copy)
    for (j = 0; j < K1; ++j)
      BufA[j] = Ap[i+K2*j];
    // do the level k1 transform
    mpn_fft_fftR4_twisted(BufA, i, k1, k, omega, n, rotbuf);
    // copy back (since with the rotating buffer, the pointers have been
    // moved around.
    for (j = 0; j < K1; ++j)
      Ap[i+K2*j] = BufA[j];
  }

  omegai = omega<<k1; 
  for (j = 0; j < K1; ++j) 
    mpn_fft_fft_radix4(Ap+j*K2, k2, omegai, n, rotbuf);

  TMP_FREE;
}

static void 
mpn_fft_fft_bailey_decompose (mp_ptr A, mp_ptr *Ap, mp_size_t k, 
			      mp_size_t omega, mp_size_t nprime, mp_srcptr n, 
			      mp_size_t nl, mp_size_t l, mp_ptr *rotbuf, int b)
{ 
  const mp_size_t k1 = k >> 1;
  const mp_size_t k2 = k - k1;
  mp_size_t i, j;
  const mp_size_t K1 = 1 << k1;
  const mp_size_t K2 = 1 << k2;
  mp_size_t omegai;
  mp_ptr *BufA; 
  mp_ptr T, tmp;
  const mp_size_t Kl = l << k;
    
  TMP_DECL;
  TMP_MARK; 
  
  BufA = TMP_ALLOC_MP_PTRS (K1);

  T = TMP_ALLOC_LIMBS(nprime + 1);

  if (nl > Kl)
    {
      tmp = TMP_ALLOC_LIMBS(Kl + 1);
      mpn_mul_fft_reduce (tmp, n, nl, Kl, b);
      n = tmp;
      nl = Kl + 1;
    }

  for (i = 0; i < K2; ++i) {
    /* Do the decomposition */
    /* omega is equal to Mp value */
    mpn_mul_fft_decompose (A, Ap, 1<<k, i, k2, nprime, n, nl, l, 
			   omega, T, b);

    // copy the i-th column of Ap into BufA (pointers... no real copy)
    for (j = 0; j < K1; ++j)
      BufA[j] = Ap[i+K2*j];
    // do the level k1 transform
    mpn_fft_fftR4_twisted(BufA, i, k1, k, omega, nprime, rotbuf);
    // copy back (since with the rotating buffer, the pointers have been
    // moved around.
    for (j = 0; j < K1; ++j)
      Ap[i+K2*j] = BufA[j];
  }

  omegai = omega<<k1;
  for (j = 0; j < K1; ++j)
    mpn_fft_fft_radix4(Ap+j*K2, k2, omegai, nprime, rotbuf);

  TMP_FREE;
}

/*
   Bailey's algorithm for inverse transform.
   We use functions that propagate the fact that the root should be inverted
   before multiplying (i.e. the shift count negated)
*/

static void 
mpn_fft_fft_baileyInv (mp_ptr * Ap, mp_size_t k, mp_size_t omega,
    mp_size_t n, mp_ptr * rotbuf, mp_size_t ** ll)
{ 
  const mp_size_t k1 = k >> 1;
  const mp_size_t k2 = k - k1;
  mp_size_t i, j;
  const mp_size_t K1 = 1 << k1;
  const mp_size_t K2 = 1 << k2;
  mp_size_t omegai;
  mp_ptr *BufA; 
    
  TMP_DECL;
  TMP_MARK; 


  // Bit-reverse table Ap.
  // FIXME: these bit-rev copies might be avoided. But do they really cost?
  for (i = 0; i < 1<<k; ++i) {
    mp_ptr tmp;
    mp_size_t j = ll[k][i];
    if (i < j) {
      tmp = Ap[i];
      Ap[i] = Ap[j];
      Ap[j] = tmp;
    }
  }
  
  BufA = TMP_ALLOC_MP_PTRS (K1);
  
  for (i = 0; i < K2; ++i) {
    // copy the i-th column of Ap into BufA (pointers... no real copy)
    for (j = 0; j < K1; ++j)
      BufA[j] = Ap[i+K2*j];
    // do the level k1 transform
    mpn_fft_fftR4_twistedNeg(BufA, i, k1, k, omega, n, rotbuf);
    // copy back (since with the rotating buffer, the pointers have been
    // moved around.
    for (j = 0; j < K1; ++j)
      Ap[i+K2*j] = BufA[j];
  }

  omegai = (omega<<k1) ; 
  for (j = 0; j < K1; ++j) 
    mpn_fft_fft_radix4Neg(Ap+j*K2, k2, omegai, n, rotbuf);

  // Bit-reverse table Ap (again...)
  for (i = 0; i < 1<<k; ++i) {
    mp_ptr tmp;
    mp_size_t j = ll[k][i];
    if (i < j) {
      tmp = Ap[i];
      Ap[i] = Ap[j];
      Ap[j] = tmp;
    }
  }
 
  TMP_FREE;
}

/* a[i] <- a[i]*b[i] mod 2^(n*GMP_NUMB_BITS)+1 for 0 <= i < K */
static void
mpn_fft_mul_modF_K (mp_ptr *ap, mp_ptr *bp, mp_size_t n, mp_size_t K)
{
  mp_size_t i;
  int sqr = (ap == bp);
  TMP_DECL;

  TMP_MARK;

  if (n >= (sqr ? SQR_FFT_MODF_THRESHOLD : MUL_FFT_MODF_THRESHOLD))
    {
      mp_size_t k, K2, nprime2, Nprime2, M2, maxLK, l;
      mp_size_t **_fft_l;
      mp_ptr *Ap, *Bp, A, B, T;

      k = mpn_fft_best_k (n, sqr);
      K2 = 1 << k;
      ASSERT_ALWAYS((n & (K2 - 1)) == 0);
      maxLK = LCM_GMP_NUMB_BITS (k);
      M2 = MUL_GMP_NUMB_BITS(n) >> k;
      l = n >> k;
      Nprime2 = ((2 * M2 + k + 2 + maxLK) / maxLK) * maxLK;
      /* Nprime2 = ceil((2*M2+k+3)/maxLK)*maxLK*/
      nprime2 = DIV_GMP_NUMB_BITS (Nprime2); /* Nprime2 / GMP_NUMB_BITS */

      /* we should ensure that nprime2 is a multiple of the next K */
      if (nprime2 >= (sqr ? SQR_FFT_MODF_THRESHOLD : MUL_FFT_MODF_THRESHOLD))
	{
	  mp_size_t K3;
	  for (;;)
	    {
	      K3 = 1L << mpn_fft_best_k (nprime2, sqr);
	      if ((nprime2 & (K3 - 1)) == 0)
		break;
	      nprime2 = (nprime2 + K3 - 1) & -K3;
	      Nprime2 = nprime2 * GMP_LIMB_BITS;
	      /* warning: since nprime2 changed, K3 may change too! */
	    }
	}
      ASSERT_ALWAYS(nprime2 < n); /* otherwise we'll loop */

      Ap = TMP_ALLOC_MP_PTRS (K2);
      Bp = TMP_ALLOC_MP_PTRS (K2);
      A = TMP_ALLOC_LIMBS (2 * K2 * (nprime2 + 1));
      T = TMP_ALLOC_LIMBS (2 * (nprime2 + 1));
      B = A + K2 * (nprime2 + 1);
      _fft_l = TMP_ALLOC_TYPE (k + 1, mp_size_t *);
      for (i = 0; i <= k; i++)
	_fft_l[i] = TMP_ALLOC_TYPE (1UL<<i, mp_size_t);
      mpn_fft_initl (_fft_l, k);

      TRACE (printf ("recurse: %dx%d limbs -> %d times %dx%d (%1.2f)\n", n,
		    n, K2, nprime2, nprime2, 2.0*(double)n/nprime2/K2));
      for (i = 0; i < K; i++, ap++, bp++)
	{
	  mpn_fft_normalize (*ap, n);
	  if (!sqr)
	    mpn_fft_normalize (*bp, n);
	  mpn_mul_fft_internal (*ap, n, *ap, n + 1, *bp, n + 1, k, Ap, Bp,
                                A, B, nprime2, l, _fft_l, T, 1, 1);
	}
    }
  else
    {
      mp_ptr a, b, tp, tpn;
      mp_limb_t cc;
      mp_size_t n2 = 2 * n;
      tp = TMP_ALLOC_LIMBS (n2);
      tpn = tp + n;
      TRACE (printf ("mpn_fft_mul_modF_K:  mpn_mul_n %d of %d limbs\n", K, n));
      /* FIXME: write a special loop for the square case, to put the test
	 out of the loop, and optimize the case a[n] != 0: maybe normalizing
	 a and b will be faster? */
      for (i = 0; i < K; i++)
	{
	  a = *ap++;
	  b = *bp++;
	  if (LIKELY(a[0] >= a[n]))
	    {
	      a[0] -= a[n];
	      a[n] = 0;
	    }
	  if (LIKELY(b[0] >= b[n]))
	    {
	      b[0] -= b[n];
	      b[n] = 0;
	    }
	  if (sqr)
	    mpn_sqr (tp, a, n);
	  else
	    mpn_mul_n (tp, b, a, n);
	  cc = a[n] && mpn_add_n (tpn, tpn, b, n);
	  cc += b[n] && mpn_add_n (tpn, tpn, a, n);
	  cc += b[n] && a[n];
	  /* 0 <= cc <= 3 */
	  cc += mpn_sub_n (a, tp, tpn, n);
	  /* 0 <= cc <= 4 */
	  a[n] = 0;
	  MPN_INCR_U (a, n + 1, cc);
	}
    }
  TMP_FREE;
}



/* 
 * Mix Point-wise multiplication and inverse FFT.
 * This is useful, since we save one pass on the whole data, thus
 * improving the locality.
 *
 * FIXME: A lot of duplicated code in this function. At some point it
 * will be necessary to clean-up things to keep it possible to maintain.
 *
 */
static void
mpn_fft_mul_modF_K_fftInv (mp_ptr *ap, mp_ptr *bp, mp_size_t n, mp_size_t Mp, mp_size_t old_k,
    mp_ptr *rotbuf, mp_size_t**ll)
{
  mp_size_t i, j;
  int sqr = (ap == bp);
  TMP_DECL;

  TMP_MARK;

  if (n >= (sqr ? SQR_FFT_MODF_THRESHOLD : MUL_FFT_MODF_THRESHOLD))
    {
      mp_size_t k, K2, nprime2, Nprime2, M2, maxLK, l;
      mp_size_t **_fft_l;
      mp_ptr *Ap, *Bp, A, B, T;

      k = mpn_fft_best_k (n, sqr);
      K2 = 1 << k;
      ASSERT_ALWAYS((n & (K2 - 1)) == 0);
      maxLK = LCM_GMP_NUMB_BITS(k);
      M2 = MUL_GMP_NUMB_BITS(n) >> k;
      l = n >> k;
      Nprime2 = ((2 * M2 + k + 2 + maxLK) / maxLK) * maxLK;
      /* Nprime2 = ceil((2*M2+k+3)/maxLK)*maxLK*/
      nprime2 = DIV_GMP_NUMB_BITS(Nprime2);

      /* we should ensure that nprime2 is a multiple of the next K */
      if (nprime2 >= (sqr ? SQR_FFT_MODF_THRESHOLD : MUL_FFT_MODF_THRESHOLD))
	{
	  mp_size_t K3;
	  for (;;)
	    {
	      K3 = 1L << mpn_fft_best_k (nprime2, sqr);
	      if ((nprime2 & (K3 - 1)) == 0)
		break;
	      nprime2 = (nprime2 + K3 - 1) & -K3;
	      Nprime2 = nprime2 * GMP_LIMB_BITS;
	      /* warning: since nprime2 changed, K3 may change too! */
	    }
	}
      ASSERT_ALWAYS(nprime2 < n); /* otherwise we'll loop */

      Ap = TMP_ALLOC_MP_PTRS (K2);
      Bp = TMP_ALLOC_MP_PTRS (K2);
      A = TMP_ALLOC_LIMBS (2 * K2 * (nprime2 + 1));
      T = TMP_ALLOC_LIMBS (2 * (nprime2 + 1));
      B = A + K2 * (nprime2 + 1);
      _fft_l = TMP_ALLOC_TYPE (k + 1, mp_size_t *);
      for (i = 0; i <= k; i++)
	_fft_l[i] = TMP_ALLOC_TYPE (1UL<<i, mp_size_t);
      mpn_fft_initl (_fft_l, k);

      TRACE (printf ("recurse: %dx%d limbs -> %d times %dx%d (%1.2f)\n", n,
		    n, K2, nprime2, nprime2, 2.0*(double)n/nprime2/K2));
      
      {
    mp_size_t k1, k2, N, K1, omega, omegai;
	mp_ptr tp, tpn, *BufA;
	mp_size_t n2 = n << 1;
	tp = TMP_ALLOC_LIMBS (n2);
	tpn = tp + n;

	k1 = old_k >> 1;
	k2 = old_k - k1;
	N = MUL_4GMP_NUMB_BITS(n); /* 4 * n * GMP_NUMB_BITS */
	K1 = 1 << k1;
	K2 = 1 << k2; /* we overwrite the previous variable, here,
			 but it is no longer used */
	omega = Mp;

	BufA = TMP_ALLOC_MP_PTRS (K1);

	for (i = 0; i < K2; ++i) {
	  // copy the i-th column of Ap into BufA (pointers... no real copy)
	  for (j = 0; j < K1; ++j) {
	    // Do the point-wise multiplication, the bitreverse and the
	    // column selection at once. Should help locality (not
	    // readibility).
	    mp_size_t ind = ll[old_k][i+K2*j];

	    mpn_fft_normalize (ap[ind], n);
	    if (!sqr)
	      mpn_fft_normalize (bp[ind], n);
	    mpn_mul_fft_internal (ap[ind], n, ap[ind], n + 1, bp[ind], n + 1, k, Ap, Bp,
		A, B, nprime2, l, _fft_l, T, 1, 1);

	    BufA[j] = ap[ind];
	  }
	  // do the level k1 transform
	  mpn_fft_fftR4_twistedNeg(BufA, i, k1, old_k, omega, n, rotbuf);
	  // copy back (since with the rotating buffer, the pointers have been
	  // moved around.
	  for (j = 0; j < K1; ++j)
	    ap[ll[old_k][i+K2*j]] = BufA[j];
	}

	for (i = 0; i < 1<<old_k; ++i) {
	  mp_ptr tmp; 
	  mp_size_t j = ll[old_k][i];
	  if (i < j) {
	    tmp = ap[i];
	    ap[i] = ap[j];
	    ap[j] = tmp;
	  }
	}

	omegai = (omega<<k1) ; 
	for (j = 0; j < K1; ++j)
	  mpn_fft_fft_radix4Neg(ap+j*K2, k2, omegai, n, rotbuf);

	// Bit-reverse table Ap (again...)
	for (i = 0; i < 1<<old_k; ++i) {
	  mp_ptr tmp;
	  mp_size_t j = ll[old_k][i];
	  if (i < j) { 
	    tmp = ap[i];
	    ap[i] = ap[j];
	    ap[j] = tmp;
	  }
	}
      }
    }
  else
    {
      mp_size_t k1, k2, N, K1, K2, omega, omegai;
      mp_ptr a, b, tp, tpn, *BufA;
      mp_limb_t cc;
      mp_size_t n2 = 2 * n;
      tp = TMP_ALLOC_LIMBS (n2);
      tpn = tp + n;
      
      k1 = old_k / 2;
      k2 = old_k-k1;
      N = MUL_4GMP_NUMB_BITS(n); /* 4 * n * GMP_NUMB_BITS */
      K1 = 1<<k1;
      K2 = 1<<k2;
      omega = Mp;

      BufA = TMP_ALLOC_MP_PTRS (K1);

      
      for (i = 0; i < K2; ++i) {
	// copy the i-th column of Ap into BufA (pointers... no real copy)
	for (j = 0; j < K1; ++j) {
	  // Do the point-wise multiplication, the bitreverse and the
	  // column selection at once. Should help locality (not
	  // readibility).
	  int ind = ll[old_k][i+K2*j];
	  
	  a = ap[ind]; b = bp[ind];
	  if (sqr)
	    mpn_sqr (tp, a, n);
	  else
	    mpn_mul_n (tp, b, a, n);
	  if (a[n] != 0)
	    cc = mpn_add_n (tpn, tpn, b, n);
	  else
	    cc = 0;
	  if (b[n] != 0)
	    cc += mpn_add_n (tpn, tpn, a, n) + a[n];
	  if (cc != 0)
	    {
	      /* FIXME: use MPN_INCR_U here, since carry is not expected.  */
	      cc = mpn_add_1 (tp, tp, n2, cc);
	      ASSERT (cc == 0);
	    }
	  a[n] = mpn_sub_n (a, tp, tpn, n) && mpn_add_1 (a, a, n, ONE);
	
	  BufA[j] = ap[ind];
	}
	// do the level k1 transform
	mpn_fft_fftR4_twistedNeg(BufA, i, k1, old_k, omega, n, rotbuf);
	// copy back (since with the rotating buffer, the pointers have been
	// moved around.
	for (j = 0; j < K1; ++j)
	  ap[ll[old_k][i+K2*j]] = BufA[j];
      }

      for (i = 0; i < 1<<old_k; ++i) {
	mp_ptr tmp; 
	mp_size_t j = ll[old_k][i];
	if (i < j) {
	  tmp = ap[i];
	  ap[i] = ap[j];
	  ap[j] = tmp;
	}
      }

      omegai = (omega<<k1) ; 
      for (j = 0; j < K1; ++j)
	mpn_fft_fft_radix4Neg(ap+j*K2, k2, omegai, n, rotbuf);

      // Bit-reverse table Ap (again...)
      for (i = 0; i < 1<<old_k; ++i) {
	mp_ptr tmp;
	mp_size_t j = ll[old_k][i];
	if (i < j) { 
	  tmp = ap[i];
	  ap[i] = ap[j];
	  ap[j] = tmp;
	}
      }
    }
  TMP_FREE;
}


/* R <- A/2^k mod 2^(n*GMP_NUMB_BITS)+1,
   where A = {a, n+1} is semi-normalized.
   Returns full-normalized R.
   Assumes 0 < k < GMP_NUMB_BITS.
   {r, n+1} and {a, n+1} may overlap, provided r <= a (like mpn_rshift).
*/
static void
mpn_fft_div_2exp_modF (mp_ptr r, mp_srcptr a, mp_size_t k, mp_size_t n)
{
  mp_limb_t cc;

  /* Let c be the value of the low k bits from a: the idea is to subtract
     c*(B^n+1) to {a, n+1} to make the low k bits vanish, and then shift
     right by k bits. */
  ASSERT_ALWAYS (0 < k && k < GMP_NUMB_BITS);
  cc = mpn_rshift (r, a, n + 1, k);
  /* now r[n] = 0, and r[n-1] < 2^(GMP_NUMB_BITS + 1 - k), and we have to
     subtract cc at r[n-1] */
  if (r[n - 1] < cc)
    {
      MPN_INCR_U (r, n, ONE); /* compensates borrow in r[n] below.
				 Since r[n - 1] < cc, the carry
				 propagation cannot attain r[n]. */
      r[n] = r[n - 1] >= cc;  /* rare case where R = B^n */
    }
  r[n - 1] -= cc;
}

/* A <- A/sqrt(2)^k mod 2^(n*GMP_NUMB_BITS)+1.
   Assumes 0 < k < 4*n*GMP_NUMB_BITS.
   FIXME: can we use the trick used in mpn_fft_div_sqrt2exp_modF above? */
static void
mpn_fft_div_sqrt2exp_modF (mp_ptr r, mp_srcptr a, mp_size_t k, mp_size_t n)
{
  ASSERT (r != a);
#ifdef NO_SQRT_2
  ASSERT_ALWAYS (k % 2 == 0);
#endif
  ASSERT (0 < k && k < MUL_4GMP_NUMB_BITS(n));
  mpn_fft_mul_sqrt2exp_modF (r, a, MUL_4GMP_NUMB_BITS(n) - k, n);
  /* 1/2^k = 2^(2nL-k) mod 2^(n*GMP_NUMB_BITS)+1 */
  /* normalize so that R < 2^(n*GMP_NUMB_BITS)+1 */
  mpn_fft_normalize (r, n);
}

/* {rp,n} <- {ap,an} mod 2^(n*GMP_NUMB_BITS)+b, where b=1 or b=-1.
   Returns carry out, i.e. 1 iff b=1 and {ap,an}=-1 mod 2^(n*GMP_NUMB_BITS)+1,
   then {rp,n}=0. No restriction on an, except an >= 1.
*/
static mp_limb_t
mpn_fft_norm_modF (mp_ptr rp, mp_size_t n, mp_srcptr ap, mp_size_t an, int b)
{
  if (an <= n)
    {
#ifdef COUNT_ZEROCOPY
      printf ("mpn_fft_norm_modF: MPN_FFT_COPY with %d limbs\n", an);
#endif
      MPN_COPY (rp, ap, an);
      if (an < n)
	MPN_FFT_ZERO (rp + an, n - an);
      return 0;
    }
  else /* an > n */
    {
      mp_size_t l;
      mp_limb_t cc;
      mp_size_t i;

      l = (an <= 2 * n) ? an - n : n;
      if (b == -1)
	cc = mpn_add (rp, ap, n, ap + n, l);
      else
	cc = (mp_limb_t)(-(mp_limb_signed_t)mpn_sub (rp, ap, n, ap + n, l));
      ap += n + l;
      an -= n + l;
      for (i = -1; an > 0; i = -b * i)
	{ /* it remains to deal with {ap, an} */
	  l = (an <= n) ? an : n;
	  if (i == -1)
	    cc += mpn_add (rp, rp, n, ap, l);
	  else
	    cc -= mpn_sub (rp, rp, n, ap, l);
	  ap += l;
	  an -= l;
	}
      if (b == 1)
	{
	  if (cc & GMP_LIMB_HIGHBIT) /* cc < 0 */
	    cc = mpn_add_1 (rp, rp, n, (mp_limb_t)(-(mp_limb_signed_t)cc));
	  cc = mpn_sub_1 (rp, rp, n, cc);
	}
      else /* b = -1: necessarily cc >= 0 */
	cc = mpn_add_1 (rp, rp, n, cc);
      return mpn_add_1 (rp, rp, n, cc);
    }
}


/* op <- n*m mod 2^N+b with fft of size 2^k where N=pl*GMP_NUMB_BITS
   n and m have respectively nl and ml limbs
   op must have space for pl+1 limbs if rec=1 (and pl limbs if rec=0).
   One must have pl = mpn_fft_next_size (pl, k).
   T must have space for 2 * (nprime + 1) limbs.

   If rec=0, then store only the pl low bits of the result, and return
   the out carry.

   Assumes b=1 (negacyclic convolution) or b=-1 (cyclic convolution).
*/
static int
mpn_mul_fft_internal (mp_ptr op, mp_size_t pl,
                      mp_srcptr n, mp_size_t nl,
                      mp_srcptr m, mp_size_t ml,
		      mp_size_t k,
		      mp_ptr *Ap, mp_ptr *Bp,
		      mp_ptr A, mp_ptr B,
		      mp_size_t nprime, mp_size_t l,
		      mp_size_t **_fft_l,
		      mp_ptr T, mp_size_t rec, int b)
{
  const mp_size_t K = 1<<k; /* K = the transform length */
  /* Choose Mp so that sqrt(2)^Mp has order 2^k.
     Remember: sqrt(2) has order 4*Nprime (mod 2^Nprime + 1) */
  const mp_size_t Mp = (4 * nprime * GMP_LIMB_BITS) >> k;
  mp_size_t i, pla, lo, sh, j;
  int ii;
  int sqr;
  mp_ptr p;
  mp_limb_t cc;
  mp_ptr rotbufA[1], rotbufB[1]; /* we need two rotating buffers, otherwise
				    some Ap[i] may point to the B[] array,
				    and will be erase since we use the B[]
				    array to store the final result {p,pla} */
  TMP_DECL;
  TMP_MARK;

  rotbufA[0] = TMP_ALLOC_LIMBS(nprime+1);
  rotbufB[0] = TMP_ALLOC_LIMBS(nprime+1);

  ASSERT(b == 1 || b == -1);

  sqr = n == m && nl == ml;

  TRACE (printf ("mpn_mul_fft_internal: pl=%d k=%d K=%d np=%d l=%d Mp=%d "
                 "rec=%d sqr=%d b=%d\n", pl,k,K,nprime,l,Mp,rec,sqr,b));

#define BAILEY_THRESHOLD 9

  /* direct fft's */
  /* This threshold for Bailey's algorithm has been determined
   experimentally on an Opteron. */
  if (k >= BAILEY_THRESHOLD) {
    TRACE(printf("Calling mpn_fft_fft_bailey(Ap, %d, %d, %d, T, ...)\n",
          k,Mp,nprime);)
    /* decomposition of inputs into arrays Ap[i] and Bp[i] */
#ifdef MERGED_BAILEY_DECOMPOSE
    mpn_fft_fft_bailey_decompose (A, Ap, k, Mp, nprime, n, nl, l, rotbufA, b);
    if (!sqr)
      mpn_fft_fft_bailey_decompose (B, Bp, k, Mp, nprime, m, ml, l, rotbufB, b);
#else
    mpn_mul_fft_decompose (A, Ap, K, 0, 0, nprime, n, nl, l, Mp, T, b);
    mpn_fft_fft_bailey (Ap, k, Mp, nprime, rotbufA);
    if (!sqr) {
      mpn_mul_fft_decompose (B, Bp, K, 0, 0, nprime, m, ml, l, Mp, T, b);
      mpn_fft_fft_bailey (Bp, k, Mp, nprime, rotbufB);
    }
#endif
  } else {
    TRACE(printf("Calling mpn_fft_fft_radix4(Ap, %d, %d, %d, T, ...)\n",
		 k,Mp,nprime);)
    /* decomposition of inputs into arrays Ap[i] and Bp[i] */
    mpn_mul_fft_decompose (A, Ap, K, 0, 0, nprime, n, nl, l, Mp, T, b);
    if (sqr == 0)
      mpn_mul_fft_decompose (B, Bp, K, 0, 0, nprime, m, ml, l, Mp, T, b);

    mpn_fft_fft_radix4 (Ap, k, Mp, nprime, rotbufA);
    if (!sqr)
      mpn_fft_fft_radix4 (Bp, k, Mp, nprime, rotbufB);
  }

  /*
   * We want to multipy the K transformed elements of A and B (or A and A
   * if we're squaring), with products reduced (mod 2^Nprime+1)
   *
   * Then we must do the backward transform.
   *
   * If we are below Bailey's threshold, we assume that the data fits in
   * the cache and do those 2 tasks separately. Otherwise we mix them: we
   * do the point-wise products for the elements of one column, then we
   * readily do the transform of the column since we have it in cache.
   * The code becomes messy (especially when you add the bitreverse
   * stuff), but this saves a bit.
   */
  if (k >= BAILEY_THRESHOLD) {
    mpn_fft_mul_modF_K_fftInv (Ap, (sqr) ? Ap : Bp, nprime, Mp, k, rotbufA, _fft_l);
  } else {
    mpn_fft_mul_modF_K (Ap, (sqr) ? Ap : Bp, nprime, K);
    TRACE(printf("mpn_mul_fft_internal: Calling mpn_fft_fft_radix4Inv(Ap, %d, "
	  "%d, %d, T, ...)\n", k, Mp, nprime);)
    mpn_fft_fft_radix4Inv (Ap, k, Mp, nprime, rotbufA, _fft_l);
  }

  Bp[0] = T + nprime + 1;

  /* addition of terms in result p */
  MPN_FFT_ZERO (T, nprime + 1);

  pla = l * (K - 1) + nprime + 1; /* number of required limbs for p */
  p = B; /* B has K*(n' + 1) limbs, which is >= pla, i.e. enough */
  ASSERT (K * (nprime + 1) >= pla);
  MPN_FFT_ZERO (p, pla);

  cc = 0; /* will accumulate the (signed) carry at p[pla] */
  for (i = K - 1, lo = l * i + nprime,sh = l * i; i >= 0; i--,lo -= l,sh -= l)
    {
      mp_ptr n = p + sh;

      j = (K - i) & (K - 1);

      /* Multiply by appropriate root and reorder. We want to divide by the 
         transform length, so divide by sqrt(2)^(2*k) == 2^k */
      if (j > 0 && b == 1)
	mpn_fft_div_sqrt2exp_modF (Bp[0], Ap[K - j], 
				   2 * k + (K - j) * (Mp / 2), nprime);
      else /* No unweighting to be done, only divide by transform length */
	mpn_fft_div_2exp_modF (Bp[0], Ap[(K - j) & (K - 1)], k, nprime);
      Bp[j] = Bp[0];

      if (mpn_add_n (n, n, Bp[j], nprime + 1))
	cc += mpn_add_1 (n + nprime + 1, n + nprime + 1,
			 pla - sh - nprime - 1, ONE);
      T[2 * l] = (b == 1) ? i + 1 : K; /* T = (i + 1)*2^(2*M) */
      if (mpn_cmp (Bp[j], T, nprime + 1) > 0)
	{ /* subtract 2^N'+1 from {n, nprime} */
	  cc -= mpn_sub_1 (n, n , pla - sh, ONE);
	  cc -= mpn_sub_1 (p + lo, p + lo, pla - lo, ONE);
	}
    }
  if (cc == (mp_limb_t)(-(mp_limb_signed_t)ONE))
    {
      if ((cc = mpn_add_1 (p + pla - pl, p + pla - pl, pl, ONE)))
	{
	  /* p[pla-pl]...p[pla-1] are all zero */
	  mpn_sub_1 (p + pla - pl - 1, p + pla - pl - 1, pl + 1, ONE);
	  mpn_sub_1 (p + pla - 1, p + pla - 1, 1, ONE);
	}
    }
  else if (cc == ONE)
    {
      if (pla >= 2 * pl)
	{
	  while ((cc = mpn_add_1 (p + pla - 2 * pl, p + pla - 2 * pl, 2 * pl, cc)))
	    ;
	}
      else
	{
	  cc = mpn_sub_1 (p + pla - pl, p + pla - pl, pl, cc);
	  ASSERT (cc == 0);
	}
    }
  else
    ASSERT (cc == 0);

  /* here p < 2^(2M) [K 2^(M(K-1)) + (K-1) 2^(M(K-2)) + ... ]
     < K 2^(2M) [2^(M(K-1)) + 2^(M(K-2)) + ... ]
     < K 2^(2M) 2^(M(K-1))*2 = 2^(M*K+M+k+1) */
  ii = (int) mpn_fft_norm_modF (op, pl, p, pla, b);
  if (rec) /* store the carry out */
    op[pl] = ii;

  TMP_FREE;

  return ii;
}

/* return the lcm of a and 2^k */
static inline mp_size_t
mpn_mul_fft_lcm (mp_size_t a, mp_size_t k)
{
  mp_size_t l = k;

  while ((a & 1) == 0 && k > 0)
    {
      a >>= 1;
      k --;
    }
  return a << l;
}

/* put in {op, pl} the product of {n, nl} * {m, ml} mod (B^pl+1)
   where B = 2^GMP_NUMB_BITS. */
int
mpn_mul_fft (mp_ptr op, mp_size_t pl,
	     mp_srcptr n, mp_size_t nl,
	     mp_srcptr m, mp_size_t ml,
	     int k)
{
  return mpn_mul_fft_aux (op, pl, n, nl, m, ml, k, 1);
}

/* put in {op, pl} the product of {n, nl} * {m, ml} mod (B^pl-1)
   where B = 2^GMP_NUMB_BITS. */
int
mpn_mul_fft_mersenne (mp_ptr op, mp_size_t pl,
                      mp_srcptr n, mp_size_t nl,
                      mp_srcptr m, mp_size_t ml,
                      mp_size_t k)
{
  return mpn_mul_fft_aux (op, pl, n, nl, m, ml, k, -1);
}

/* put in {op, pl} + carry out the product {n, nl} * {m, ml}
   modulo 2^(pl*GMP_NUMB_BITS) + b, where b = 1 or b = -1.
*/
int
mpn_mul_fft_aux (mp_ptr op, mp_size_t pl,
		 mp_srcptr n, mp_size_t nl,
		 mp_srcptr m, mp_size_t ml,
		 mp_size_t k, int b)
{
  mp_size_t maxLK, i, c;
  const mp_size_t K = 1 << k;
  mp_size_t N, Nprime, nprime, M, l;
  mp_ptr *Ap, *Bp, A, T, B;
  mp_size_t **_fft_l;
  int sqr = (n == m && nl == ml);
  TMP_DECL;

  TRACE (printf ("\nmpn_mul_fft_aux: mpn_mul_fft pl=%ld nl=%ld ml=%ld k=%d "
                 "b=%d\n", pl, nl, ml, k, b));
  ASSERT_ALWAYS (mpn_fft_next_size (pl, k) == pl);

  TMP_MARK;
  
  /* first reduce {n, nl} or {m, ml} if nl > pl or ml > pl */
  if (nl > pl)
    {
      mp_ptr nn = TMP_ALLOC_LIMBS(pl + (b == 1));
      if ((i = (mp_size_t)mpn_fft_norm_modF (nn, pl, n, nl, b)))
	nn[pl] = CNST_LIMB(1);
      n = nn;
      nl = pl + i;
    }
  if ((!sqr) && (ml > pl))
    {
      mp_ptr mm = TMP_ALLOC_LIMBS(pl + (b == 1));
      if ((i = (mp_size_t)mpn_fft_norm_modF (mm, pl, m, ml, b)))
	mm[pl] = CNST_LIMB(1);
      m = mm;
      ml = pl + i;
    }
  else if (sqr)
  {
	  m = n;
	  ml = nl;
  }
  /* now nl,ml <= pl if b=-1, nl,ml <= pl+1 if b=1 */

  N = MUL_GMP_NUMB_BITS(pl); /* The entire integer product will be mod 2^N+b */
  _fft_l = TMP_ALLOC_TYPE (k + 1, mp_size_t *);
  for (i = 0; i <= k; i++)
    _fft_l[i] = TMP_ALLOC_TYPE (1UL << i, mp_size_t);
  mpn_fft_initl (_fft_l, k);
  M = N >> k;	/* The number of bits we need to be able to store in each 
                   of the 2^k pieces */
  
  l = 1 + DIV_GMP_NUMB_BITS(M - 1); /* nb of limbs in each of the 2^k pieces */

  /* Choose maxLK so that an order 4*2^k root of unity exists for the 
     negacyclic transform (which needs a root of unity of order twice the
     transform length for the weight signal), or an order 2*2^k root of 
     unity for the cyclic transform (which uses no weight signal) */
#ifdef NO_SQRT_2
  c = (b == -1) ? 1 : 0;
#else
  c = (b == -1) ? 2 : 1;
#endif
  ASSERT(k >= c);
  maxLK = LCM_GMP_NUMB_BITS (k - c);
  /* maxLK = lcm (GMP_NUMB_BITS, 2^(k-1) for b=1, 2^(k-2) for b=-1) */
  /* When we do the transforms with elements (mod 2^Nprime+1), we need
     GMP_NUMB_BITS|Nprime so that shifts are fast, and 
     transformlength|2*c*Nprime so that transformlength|ord(2) for b==1 or
     transformlength|ord(sqrt(2)) for b==-1 */

  Nprime = 2 * M + k + 2; /* make Nprime large enough so that the coefficients 
                             in the product polynomial are not affected by
                             reduction (mod 2^Nprime+1). 
                             FIXME is the +2 necessary? */
  Nprime = (Nprime / maxLK + 1) * maxLK; /* Round up Nprime to multiple of 
                                            both GMP_NUMB_BITS and 2^(k-1) */

  nprime = DIV_GMP_NUMB_BITS(Nprime); /* number of limbs in poly coefficient */
  TRACE(printf ("mpn_mul_fft_aux: N=%d K=%d, M=%d, l=%d, maxLK=%d, Np=%d, "
		"np=%d\n", N, K, M, l, maxLK, Nprime, nprime);)
  /* we should ensure that recursively, nprime is a multiple of the next K */
  if (nprime >= (sqr ? SQR_FFT_MODF_THRESHOLD : MUL_FFT_MODF_THRESHOLD))
    {
      mp_size_t K2;
      for (;;)
	{
	  K2 = 1L << mpn_fft_best_k (nprime, sqr);
	  if ((nprime & (K2 - 1)) == 0)
	    break;
	  nprime = (nprime + K2 - 1) & -K2; /* round up nprime to multiple of K2 */
	  Nprime = nprime * GMP_LIMB_BITS;
	  /* warning: since nprime changed, K2 may change too! */
	}
      TRACE (printf ("mpn_mul_fft_aux: new maxLK=%d, Np=%d, np=%d\n", maxLK, Nprime, nprime));
    }
  ASSERT_ALWAYS (nprime < pl); /* otherwise we'll loop */

  T = TMP_ALLOC_LIMBS (2 * (nprime + 1));

  TRACE (printf ("mpn_mul_fft_aux: %dx%d limbs -> %d times %dx%d limbs (%1.2f)\n",
		pl, pl, K, nprime, nprime, 2.0 * (double) N / Nprime / K);
	 printf ("   temp space %ld\n", 2 * K * (nprime + 1));)

  A = __GMP_ALLOCATE_FUNC_LIMBS (2 * K * (nprime + 1));
  B = A + K * (nprime + 1);
  Ap = TMP_ALLOC_MP_PTRS (K);
  Bp = TMP_ALLOC_MP_PTRS (K);

  i = mpn_mul_fft_internal (op, pl, n, nl, m, ml, k, Ap, Bp, A, B, nprime,
                            l, _fft_l, T, 0, b);

  TMP_FREE;
  __GMP_FREE_FUNC_LIMBS (A, 2 * K * (nprime + 1));

  return i;
}

/* multiply {n, nl} by {m, ml}, and put the result in {op, nl+ml},
   using one modular product mod 2^N-1 and one mod 2^(aN)+1,
   with a >= 1. */
void
mpn_mul_fft_full_a (mp_ptr op,
		    mp_srcptr n, mp_size_t nl,
		    mp_srcptr m, mp_size_t ml, int a)
{
  mp_size_t pl = nl + ml; /* total number of limbs of the result */
  int sqr = n == m && nl == ml;
  mp_size_t l, h;
  mp_limb_t muh, cc;
  mp_size_t k1, k2, i;
  mp_ptr tp;
  
  while (1)
  {

  ASSERT_ALWAYS(a > 0);
  l = (pl + a + (a > 1)) / (a + 1); /* ceil(pl/(a+1)) */
  /* Warning: for a > 1, the product may be larger than (2^N-1) * (2^(aN)+1),
     thus we take one extra limb. */

  k1 = mpn_fft_best_k (l, 2 + sqr); /* for 2^N-1 */
  k2 = mpn_fft_best_k (a * l, sqr); /* for 2^(aN)+1 */

  /* we must have l multiple of 2^k1 and a*l multiple of 2^k2.
     FIXME: the optimal k1 and k2 values might change in the while loop. */
  while (1)
    {
      h = mpn_fft_next_size (l, k1);
      if (h != l)
	l = h;
      else
	{
	  h = mpn_fft_next_size (a * l, k2);
	  if (h != a * l)
	    l = (h + a - 1) / a; /* ceil(h/a) */
	  else
	    break;
	}
    }

  h = a * l;
  /* now mpn_fft_next_size (l, k1) = l
     and mpn_fft_next_size (h, k2) = h with h = a * l */

  /* we perform one FFT mod 2^(aN)+1 and one mod 2^N-1.
     Let P = n * m. Assume P = lambda * (2^(aN)+1) + mu,
     with 0 <= mu < 2^(aN)+1, and 0 <= lambda < 2^N-1.
     Then P = mu mod (2^(aN)+1)
      and P = 2*lambda+mu mod (2^N-1).
     Let A := P mod (2^(aN)+1) and B := P mod (2^N-1),
     with 0 <= A < 2^(aN)+1 and 0 <= B < 2^N-1.
     Then mu = A, and lambda = (B-A)/2 mod (2^N-1). */

     if (h < pl) break;
	  a--;
  } 

  muh = mpn_mul_fft (op, h, n, nl, m, ml, k2); /* mu = muh+{op,h} */

  tp = __GMP_ALLOCATE_FUNC_LIMBS (l);
  mpn_mul_fft_mersenne (tp, l, n, nl, m, ml, k1); /* B */

  /* now compute B-A mod 2^N-1, where B = {tp, l}, and A = cc + {op, h} */
  for (cc = muh, i = 0; i < a; i++)
    cc += mpn_sub_n (tp, tp, op + i * l, l);
  /* cc is a borrow at tp[0] */
  while (cc > 0) /* add cc*(2^N-1): if cc=1 after the first loop, then
		    tp[l-1] = 111...111, and cc=0 after the 2nd loop */
    cc = mpn_sub_1 (tp, tp, l, cc);
  /* Check whether {tp,l} = 111...111, in which case we should reduce it
     to 000...000. */
  for (i = 0; i < l && ~tp[i] == 0; i++);
  if (i == l)
    mpn_add_1 (tp, tp, l, 1); /* reduces {tp,l} to 000...000 */

  /* make cc + {tp, l} even, and divide by 2 */
  if (tp[0] & (mp_limb_t) 1)
    cc = 1 - mpn_sub_1 (tp, tp, l, 1); /* add 2^N-1 */
  /* now we have to compute lambda * (2^(aN)+1) + mu,
     where 2*lambda = {tp, l} and mu = muh + {op, h} */
  mpn_rshift (op + h, tp, pl - h, 1); /* divide by 2 to obtain lambda */
  if (pl < l + h) /* i.e. pl - h < l: it remains high limbs in {tp, l} */
    {
      /* since the product is P = lambda * (2^N+1) + mu, if cc=1, the product
         would exceed pl < h+l limbs */
      ASSERT_ALWAYS (cc == 0);
      cc = tp[pl - h] & 1;
    }
  op[pl - 1] |= cc << (GMP_NUMB_BITS - 1);
  __GMP_FREE_FUNC_LIMBS (tp, l);

  /* since n * m has at most pl limbs, the high part of lambda should be 0 */
  cc = mpn_add_n (op, op, op + h, pl - h); /* add lambda to mu */
  MPN_INCR_U (op + pl - h, h, cc);
  MPN_INCR_U (op + h, pl - h, muh);
}

/* multiply {n, nl} by {m, ml}, and put the result in {op, nl+ml} */
void
mpn_mul_fft_full (mp_ptr op,
		   mp_srcptr n, mp_size_t nl,
		   mp_srcptr m, mp_size_t ml)
{
#ifndef MUL_FFT_FULL_TABLE2
  mpn_mul_fft_full_a (op, n, nl, m, ml, 1);
#else
  int a = mpn_fft_best_a ((nl + ml) / 2, n == m && nl == ml);
  mpn_mul_fft_full_a (op, n, nl, m, ml, a);
#endif
  return;
}
