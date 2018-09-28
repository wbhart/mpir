/* Reference mpn functions, designed to be simple, portable and independent
   of the normal gmp code.  Speed isn't a consideration.

Copyright 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005 Free
Software Foundation, Inc.

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

/* The following license applies to refmpn_mulmid_basecase -- middle product *only*

Copyright (C) 2009, David Harvey

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Most routines have assertions representing what the mpn routines are
   supposed to accept.  Many of these reference routines do sensible things
   outside these ranges (eg. for size==0), but the assertions are present to
   pick up bad parameters passed here that are about to be passed the same
   to a real mpn routine being compared.  */

/* always do assertion checking */
#define WANT_ASSERT  1

#include <stdio.h>  /* for NULL */
#include <stdlib.h> /* for malloc */

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"

#include "tests.h"



/* Return non-zero if regions {xp,xsize} and {yp,ysize} overlap, with sizes
   in bytes. */
int
byte_overlap_p (const void *v_xp, mp_size_t xsize,
                const void *v_yp, mp_size_t ysize)
{
  const char *xp = v_xp;
  const char *yp = v_yp;

  ASSERT (xsize >= 0);
  ASSERT (ysize >= 0);

  /* no wraparounds */
  ASSERT (xp+xsize >= xp);
  ASSERT (yp+ysize >= yp);

  if (xp + xsize <= yp)
    return 0;

  if (yp + ysize <= xp)
    return 0;

  return 1;
}

/* Return non-zero if limb regions {xp,xsize} and {yp,ysize} overlap. */
int
refmpn_overlap_p (mp_srcptr xp, mp_size_t xsize, mp_srcptr yp, mp_size_t ysize)
{
  return byte_overlap_p (xp, xsize * BYTES_PER_MP_LIMB,
                         yp, ysize * BYTES_PER_MP_LIMB);
}

/* Check overlap for a routine defined to work low to high. */
int
refmpn_overlap_low_to_high_p (mp_srcptr dst, mp_srcptr src, mp_size_t size)
{
  return (dst <= src || ! refmpn_overlap_p (dst, size, src, size));
}

/* Check overlap for a routine defined to work high to low. */
int
refmpn_overlap_high_to_low_p (mp_srcptr dst, mp_srcptr src, mp_size_t size)
{
  return (dst >= src || ! refmpn_overlap_p (dst, size, src, size));
}

/* Check overlap for a standard routine requiring equal or separate. */
int
refmpn_overlap_fullonly_p (mp_srcptr dst, mp_srcptr src, mp_size_t size)
{
  return (dst == src || ! refmpn_overlap_p (dst, size, src, size));
}
int
refmpn_overlap_fullonly_two_p (mp_srcptr dst, mp_srcptr src1, mp_srcptr src2,
                               mp_size_t size)
{
  return (refmpn_overlap_fullonly_p (dst, src1, size)
          && refmpn_overlap_fullonly_p (dst, src2, size));
}


mp_ptr
refmpn_malloc_limbs (mp_size_t size)
{
  mp_ptr  p;
  ASSERT (size >= 0);
  if (size == 0)
    size = 1;
  p = (mp_ptr) malloc ((size_t) (size * BYTES_PER_MP_LIMB));
  ASSERT (p != NULL);
  return p;
}

/* Free limbs allocated by refmpn_malloc_limbs. NOTE: Can't free
 * memory allocated by refmpn_malloc_limbs_aligned. */
void
refmpn_free_limbs (mp_ptr p)
{
  free (p);
}

mp_ptr
refmpn_memdup_limbs (mp_srcptr ptr, mp_size_t size)
{
  mp_ptr  p;
  p = refmpn_malloc_limbs (size);
  refmpn_copyi (p, ptr, size);
  return p;
}

/* malloc n limbs on a multiple of m bytes boundary */
mp_ptr
refmpn_malloc_limbs_aligned (mp_size_t n, size_t m)
{
  return (mp_ptr) align_pointer (refmpn_malloc_limbs (n + m-1), m);
}


void
refmpn_fill (mp_ptr ptr, mp_size_t size, mp_limb_t value)
{
  mp_size_t  i;
  ASSERT (size >= 0);
  for (i = 0; i < size; i++)
    ptr[i] = value;
}

void
refmpn_zero (mp_ptr ptr, mp_size_t size)
{
  refmpn_fill (ptr, size, CNST_LIMB(0));
}

void
refmpn_store (mp_ptr ptr, mp_size_t size,mp_limb_t val)
{
  refmpn_fill (ptr, size, val);
}

void
refmpn_zero_extend (mp_ptr ptr, mp_size_t oldsize, mp_size_t newsize)
{
  ASSERT (newsize >= oldsize);
  refmpn_zero (ptr+oldsize, newsize-oldsize);
}

int
refmpn_zero_p (mp_srcptr ptr, mp_size_t size)
{
  mp_size_t  i;
  for (i = 0; i < size; i++)
    if (ptr[i] != 0)
      return 0;
  return 1;
}

mp_size_t
refmpn_normalize (mp_srcptr ptr, mp_size_t size)
{
  ASSERT (size >= 0);
  while (size > 0 && ptr[size-1] == 0)
    size--;
  return size;
}

/* the highest one bit in x */
mp_limb_t
refmpn_msbone (mp_limb_t x)
{
  mp_limb_t  n = (mp_limb_t) 1 << (BITS_PER_MP_LIMB-1);

  while (n != 0)
    {
      if (x & n)
        break;
      n >>= 1;
    }
  return n;
}

/* a mask of the highest one bit plus and all bits below */
mp_limb_t
refmpn_msbone_mask (mp_limb_t x)
{
  if (x == 0)
    return 0;

  return (refmpn_msbone (x) << 1) - 1;
}

/* How many digits in the given base will fit in a limb.
   Notice that the product b is allowed to be equal to the limit
   2^GMP_NUMB_BITS, this ensures the result for base==2 will be
   GMP_NUMB_BITS (and similarly other powers of 2).  */
int
refmpn_chars_per_limb (int base)
{
  mp_limb_t  limit[2], b[2];
  int        chars_per_limb;

  ASSERT (base >= 2);

  limit[0] = 0;  /* limit = 2^GMP_NUMB_BITS */
  limit[1] = 1;
  b[0] = 1;      /* b = 1 */
  b[1] = 0;

  chars_per_limb = 0;
  for (;;)
    {
      if (refmpn_mul_1 (b, b, (mp_size_t) 2, (mp_limb_t) base))
        break;
      if (refmpn_cmp (b, limit, (mp_size_t) 2) > 0)
        break;
      chars_per_limb++;
    }
  return chars_per_limb;
}

/* The biggest value base**n which fits in GMP_NUMB_BITS. */
mp_limb_t
refmpn_big_base (int base)
{
  int        chars_per_limb = refmpn_chars_per_limb (base);
  int        i;
  mp_limb_t  bb;

  ASSERT (base >= 2);
  bb = 1;
  for (i = 0; i < chars_per_limb; i++)
    bb *= base;
  return bb;
}


void
refmpn_setbit (mp_ptr ptr, unsigned long bit)
{
  ptr[bit/GMP_NUMB_BITS] |= CNST_LIMB(1) << (bit%GMP_NUMB_BITS);
}

void
refmpn_clrbit (mp_ptr ptr, unsigned long bit)
{
  ptr[bit/GMP_NUMB_BITS] &= ~ (CNST_LIMB(1) << (bit%GMP_NUMB_BITS));
}

#define REFMPN_TSTBIT(ptr,bit) \
  (((ptr)[(bit)/GMP_NUMB_BITS] & (CNST_LIMB(1) << ((bit)%GMP_NUMB_BITS))) != 0)

int
refmpn_tstbit (mp_srcptr ptr, unsigned long bit)
{
  return REFMPN_TSTBIT (ptr, bit);
}

unsigned long
refmpn_scan0 (mp_srcptr ptr, unsigned long bit)
{
  while (REFMPN_TSTBIT (ptr, bit) != 0)
    bit++;
  return bit;
}

unsigned long
refmpn_scan1 (mp_srcptr ptr, unsigned long bit)
{
  while (REFMPN_TSTBIT (ptr, bit) == 0)
    bit++;
  return bit;
}

void
refmpn_copy (mp_ptr rp, mp_srcptr sp, mp_size_t size)
{
  ASSERT (refmpn_overlap_fullonly_p (rp, sp, size));
  refmpn_copyi (rp, sp, size);
}

void
refmpn_copyi (mp_ptr rp, mp_srcptr sp, mp_size_t size)
{
  mp_size_t i;

  ASSERT (refmpn_overlap_low_to_high_p (rp, sp, size));
  ASSERT (size >= 0);

  for (i = 0; i < size; i++)
    rp[i] = sp[i];
}

void
refmpn_copyd (mp_ptr rp, mp_srcptr sp, mp_size_t size)
{
  mp_size_t i;

  ASSERT (refmpn_overlap_high_to_low_p (rp, sp, size));
  ASSERT (size >= 0);

  for (i = size-1; i >= 0; i--)
    rp[i] = sp[i];
}

/* Copy {xp,xsize} to {wp,wsize}.  If x is shorter, then pad w with low
   zeros to wsize.  If x is longer, then copy just the high wsize limbs.  */
void
refmpn_copy_extend (mp_ptr wp, mp_size_t wsize, mp_srcptr xp, mp_size_t xsize)
{
  ASSERT (wsize >= 0);
  ASSERT (xsize >= 0);

  /* high part of x if x bigger than w */
  if (xsize > wsize)
    {
      xp += xsize - wsize;
      xsize = wsize;
    }

  refmpn_copy (wp + wsize-xsize, xp, xsize);
  refmpn_zero (wp, wsize-xsize);
}

void
refmpn_com_n (mp_ptr rp, mp_srcptr sp, mp_size_t size)
{
  mp_size_t i;

  ASSERT (refmpn_overlap_fullonly_p (rp, sp, size));
  ASSERT (size >= 1);
  ASSERT_MPN (sp, size);

  for (i = 0; i < size; i++)
    rp[i] = sp[i] ^ GMP_NUMB_MASK;
}

int
refmpn_cmp (mp_srcptr xp, mp_srcptr yp, mp_size_t size)
{
  mp_size_t  i;

  ASSERT (size >= 1);
  ASSERT_MPN (xp, size);
  ASSERT_MPN (yp, size);

  for (i = size-1; i >= 0; i--)
    {
      if (xp[i] > yp[i])  return 1;
      if (xp[i] < yp[i])  return -1;
    }
  return 0;
}

int
refmpn_cmp_allowzero (mp_srcptr xp, mp_srcptr yp, mp_size_t size)
{
  if (size == 0)
    return 0;
  else
    return refmpn_cmp (xp, yp, size);
}

int
refmpn_cmp_twosizes (mp_srcptr xp, mp_size_t xsize,
                     mp_srcptr yp, mp_size_t ysize)
{
  int  opp, cmp;

  ASSERT_MPN (xp, xsize);
  ASSERT_MPN (yp, ysize);

  opp = (xsize < ysize);
  if (opp)
    MPN_SRCPTR_SWAP (xp,xsize, yp,ysize);

  if (! refmpn_zero_p (xp+ysize, xsize-ysize))
    cmp = 1;
  else
    cmp = refmpn_cmp (xp, yp, ysize);

  return (opp ? -cmp : cmp);
}

int
refmpn_equal_anynail (mp_srcptr xp, mp_srcptr yp, mp_size_t size)
{
  mp_size_t  i;
  ASSERT (size >= 0);

  for (i = 0; i < size; i++)
      if (xp[i] != yp[i])
        return 0;
  return 1;
}


#define LOGOPS(operation)                                               \
  {                                                                     \
    mp_size_t  i;                                                       \
                                                                        \
    ASSERT (refmpn_overlap_fullonly_two_p (rp, s1p, s2p, size));        \
    ASSERT (size >= 1);                                                 \
    ASSERT_MPN (s1p, size);                                             \
    ASSERT_MPN (s2p, size);                                             \
                                                                        \
    for (i = 0; i < size; i++)                                          \
      rp[i] = operation;                                                \
  }

void
refmpn_and_n (mp_ptr rp, mp_srcptr s1p, mp_srcptr s2p, mp_size_t size)
{
  LOGOPS (s1p[i] & s2p[i]);
}
void
refmpn_andn_n (mp_ptr rp, mp_srcptr s1p, mp_srcptr s2p, mp_size_t size)
{
  LOGOPS (s1p[i] & ~s2p[i]);
}
void
refmpn_nand_n (mp_ptr rp, mp_srcptr s1p, mp_srcptr s2p, mp_size_t size)
{
  LOGOPS ((s1p[i] & s2p[i]) ^ GMP_NUMB_MASK);
}
void
refmpn_ior_n (mp_ptr rp, mp_srcptr s1p, mp_srcptr s2p, mp_size_t size)
{
  LOGOPS (s1p[i] | s2p[i]);
}
void
refmpn_iorn_n (mp_ptr rp, mp_srcptr s1p, mp_srcptr s2p, mp_size_t size)
{
  LOGOPS (s1p[i] | (s2p[i] ^ GMP_NUMB_MASK));
}
void
refmpn_nior_n (mp_ptr rp, mp_srcptr s1p, mp_srcptr s2p, mp_size_t size)
{
  LOGOPS ((s1p[i] | s2p[i]) ^ GMP_NUMB_MASK);
}
void
refmpn_xor_n (mp_ptr rp, mp_srcptr s1p, mp_srcptr s2p, mp_size_t size)
{
  LOGOPS (s1p[i] ^ s2p[i]);
}
void
refmpn_xnor_n (mp_ptr rp, mp_srcptr s1p, mp_srcptr s2p, mp_size_t size)
{
  LOGOPS ((s1p[i] ^ s2p[i]) ^ GMP_NUMB_MASK);
}


/* set *dh,*dl to mh:ml - sh:sl, in full limbs */
void
refmpn_sub_ddmmss (mp_limb_t *dh, mp_limb_t *dl,
                   mp_limb_t mh, mp_limb_t ml, mp_limb_t sh, mp_limb_t sl)
{
  *dl = ml - sl;
  *dh = mh - sh - (ml < sl);
}


/* set *w to x+y, return 0 or 1 carry */
mp_limb_t
ref_addc_limb (mp_limb_t *w, mp_limb_t x, mp_limb_t y)
{
  mp_limb_t  sum, cy;

  ASSERT_LIMB (x);
  ASSERT_LIMB (y);

  sum = x + y;
#if GMP_NAIL_BITS == 0
  *w = sum;
  cy = (sum < x);
#else
  *w = sum & GMP_NUMB_MASK;
  cy = (sum >> GMP_NUMB_BITS);
#endif
  return cy;
}

/* set *w to x-y, return 0 or 1 borrow */
mp_limb_t
ref_subc_limb (mp_limb_t *w, mp_limb_t x, mp_limb_t y)
{
  mp_limb_t  diff, cy;

  ASSERT_LIMB (x);
  ASSERT_LIMB (y);

  diff = x - y;
#if GMP_NAIL_BITS == 0
  *w = diff;
  cy = (diff > x);
#else
  *w = diff & GMP_NUMB_MASK;
  cy = (diff >> GMP_NUMB_BITS) & 1;
#endif
  return cy;
}

/* set *w to x+y+c (where c is 0 or 1), return 0 or 1 carry */
mp_limb_t
adc (mp_limb_t *w, mp_limb_t x, mp_limb_t y, mp_limb_t c)
{
  mp_limb_t  r;

  ASSERT_LIMB (x);
  ASSERT_LIMB (y);
  ASSERT (c == 0 || c == 1);

  r = ref_addc_limb (w, x, y);
  return r + ref_addc_limb (w, *w, c);
}

/* set *w to x-y-c (where c is 0 or 1), return 0 or 1 borrow */
mp_limb_t
sbb (mp_limb_t *w, mp_limb_t x, mp_limb_t y, mp_limb_t c)
{
  mp_limb_t  r;

  ASSERT_LIMB (x);
  ASSERT_LIMB (y);
  ASSERT (c == 0 || c == 1);

  r = ref_subc_limb (w, x, y);
  return r + ref_subc_limb (w, *w, c);
}


#define AORS_1(operation)                               \
  {                                                     \
    mp_limb_t  i;                                       \
                                                        \
    ASSERT (refmpn_overlap_fullonly_p (rp, sp, size));  \
    ASSERT (size >= 1);                                 \
    ASSERT_MPN (sp, size);                              \
    ASSERT_LIMB (n);                                    \
                                                        \
    for (i = 0; i < size; i++)                          \
      n = operation (&rp[i], sp[i], n);                 \
    return n;                                           \
  }

mp_limb_t
refmpn_add_1 (mp_ptr rp, mp_srcptr sp, mp_size_t size, mp_limb_t n)
{
  AORS_1 (ref_addc_limb);
}
mp_limb_t
refmpn_sub_1 (mp_ptr rp, mp_srcptr sp, mp_size_t size, mp_limb_t n)
{
  AORS_1 (ref_subc_limb);
}

#define AORS_NC(operation)                                              \
  {                                                                     \
    mp_size_t  i;                                                       \
                                                                        \
    ASSERT (refmpn_overlap_fullonly_two_p (rp, s1p, s2p, size));        \
    ASSERT (carry == 0 || carry == 1);                                  \
    ASSERT (size >= 1);                                                 \
    ASSERT_MPN (s1p, size);                                             \
    ASSERT_MPN (s2p, size);                                             \
                                                                        \
    for (i = 0; i < size; i++)                                          \
      carry = operation (&rp[i], s1p[i], s2p[i], carry);                \
    return carry;                                                       \
  }

mp_limb_t
refmpn_add_nc (mp_ptr rp, mp_srcptr s1p, mp_srcptr s2p, mp_size_t size,
               mp_limb_t carry)
{
  AORS_NC (adc);
}
mp_limb_t
refmpn_sub_nc (mp_ptr rp, mp_srcptr s1p, mp_srcptr s2p, mp_size_t size,
               mp_limb_t carry)
{
  AORS_NC (sbb);
}


mp_limb_t
refmpn_add_n (mp_ptr rp, mp_srcptr s1p, mp_srcptr s2p, mp_size_t size)
{
  return refmpn_add_nc (rp, s1p, s2p, size, CNST_LIMB(0));
}

mp_limb_t
refmpn_sub_n (mp_ptr rp, mp_srcptr s1p, mp_srcptr s2p, mp_size_t size)
{
  return refmpn_sub_nc (rp, s1p, s2p, size, CNST_LIMB(0));
}

mp_limb_t 
refmpn_addadd_n(mp_ptr rp, mp_srcptr xp, mp_srcptr yp, mp_srcptr zp, mp_size_t n)
{
  mp_limb_t r;
  mp_limb_t * tp = refmpn_malloc_limbs (n);
  r = refmpn_add_n (tp, yp, zp, n);
  r += refmpn_add_n (rp, tp, xp, n);
  free(tp);
  return r;
}

int 
refmpn_addsub_n(mp_ptr rp, mp_srcptr xp, mp_srcptr yp, mp_srcptr zp, mp_size_t n)
{
  mp_limb_t r;
  mp_limb_t * tp = refmpn_malloc_limbs (n);
  r = - refmpn_sub_n (tp, yp, zp, n);
  r += refmpn_add_n (rp, tp, xp, n);
  free(tp);
  return r;
}

mp_limb_t 
refmpn_subadd_n(mp_ptr rp, mp_srcptr xp, mp_srcptr yp, mp_srcptr zp, mp_size_t n)
{
  mp_limb_t r;
  mp_limb_t * tp = refmpn_malloc_limbs (n);
  r =  refmpn_sub_n (tp, xp, zp, n);
  r += refmpn_sub_n (rp, tp, yp, n);
  free(tp);
  return r;
}

mp_limb_t
refmpn_addlsh1_n (mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_size_t n)
{
  mp_limb_t cy;
  mp_ptr tp;

  ASSERT (refmpn_overlap_fullonly_two_p (rp, up, vp, n));
  ASSERT (n >= 1);
  ASSERT_MPN (up, n);
  ASSERT_MPN (vp, n);

  tp = refmpn_malloc_limbs (n);
  cy  = refmpn_lshift (tp, vp, n, 1);
  cy += refmpn_add_n (rp, up, tp, n);
  free (tp);
  return cy;
}

mp_limb_t
refmpn_sublsh1_n (mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_size_t n)
{
  mp_limb_t cy;
  mp_ptr tp;

  ASSERT (refmpn_overlap_fullonly_two_p (rp, up, vp, n));
  ASSERT (n >= 1);
  ASSERT_MPN (up, n);
  ASSERT_MPN (vp, n);

  tp = refmpn_malloc_limbs (n);
  cy  = refmpn_lshift (tp, vp, n, 1);
  cy += refmpn_sub_n (rp, up, tp, n);
  free (tp);
  return cy;
}

mp_limb_t
refmpn_addlsh_n (mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_size_t n,unsigned int c)
{return refmpn_addlsh_nc(rp,up,vp,n,c,0);}

mp_limb_t
refmpn_sublsh_n (mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_size_t n,unsigned int c)
{return refmpn_sublsh_nc(rp,up,vp,n,c,0);}

mp_limb_t
refmpn_addlsh_nc (mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_size_t n,unsigned int c,mp_limb_t cin)
{
  mp_limb_t cy;
  mp_ptr tp;

  ASSERT (refmpn_overlap_fullonly_two_p (rp, up, vp, n));
  ASSERT (n >= 1);
  ASSERT_MPN (up, n);
  ASSERT_MPN (vp, n);
  ASSERT(c>0);ASSERT(c<GMP_NUMB_BITS);

  tp = refmpn_malloc_limbs (n);
  cy  = refmpn_lshift (tp, vp, n, c);
  tp[0] |= cin >> (GMP_NUMB_BITS-c);
  cy += refmpn_add_n (rp, up, tp, n);
  free (tp);
  return cy;
}

mp_limb_t
refmpn_sublsh_nc (mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_size_t n,unsigned int c,mp_limb_t cin)
{
  mp_limb_t cy;
  mp_ptr tp;

  ASSERT (refmpn_overlap_fullonly_two_p (rp, up, vp, n));
  ASSERT (n >= 1);
  ASSERT_MPN (up, n);
  ASSERT_MPN (vp, n);
  ASSERT(c>0);ASSERT(c<GMP_NUMB_BITS);

  tp = refmpn_malloc_limbs (n);
  cy  = refmpn_lshift (tp, vp, n, c);
  tp[0] |= cin >> (GMP_NUMB_BITS-c);
  cy += refmpn_sub_n (rp, up, tp, n);
  free (tp);
  return cy;
}

mp_limb_t
refmpn_inclsh_n (mp_ptr rp, mp_srcptr up, mp_size_t n,unsigned int c)
{
  mp_limb_t cy;
  mp_ptr tp;

  ASSERT (refmpn_overlap_fullonly_p (rp, up, n));
  ASSERT (n >= 1);
  ASSERT_MPN (up, n);
  ASSERT_MPN (rp, n);
  ASSERT(c>0);ASSERT(c<GMP_NUMB_BITS);

  tp = refmpn_malloc_limbs (n);
  cy  = refmpn_lshift (tp, up, n, c);
  cy += refmpn_add_n (rp, rp, tp, n);
  free (tp);
  return cy;
}

mp_limb_t
refmpn_declsh_n (mp_ptr rp, mp_srcptr up, mp_size_t n,unsigned int c)
{
  mp_limb_t cy;
  mp_ptr tp;

  ASSERT (refmpn_overlap_fullonly_p (rp, up, n));
  ASSERT (n >= 1);
  ASSERT_MPN (up, n);
  ASSERT_MPN (rp, n);
  ASSERT(c>0);ASSERT(c<GMP_NUMB_BITS);

  tp = refmpn_malloc_limbs (n);
  cy  = refmpn_lshift (tp, up, n, c);
  cy += refmpn_sub_n (rp, rp, tp, n);
  free (tp);
  return cy;
}

mp_limb_t
refmpn_rsh1add_n (mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_size_t n)
{
  mp_limb_t cya, cys;

  ASSERT (refmpn_overlap_fullonly_two_p (rp, up, vp, n));
  ASSERT (n >= 1);
  ASSERT_MPN (up, n);
  ASSERT_MPN (vp, n);

  cya = refmpn_add_n (rp, up, vp, n);
  cys = refmpn_rshift (rp, rp, n, 1) >> (GMP_NUMB_BITS - 1);
  rp[n - 1] |= cya << (GMP_NUMB_BITS - 1);
  return cys;
}
mp_limb_t
refmpn_rsh1sub_n (mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_size_t n)
{
  mp_limb_t cya, cys;

  ASSERT (refmpn_overlap_fullonly_two_p (rp, up, vp, n));
  ASSERT (n >= 1);
  ASSERT_MPN (up, n);
  ASSERT_MPN (vp, n);

  cya = refmpn_sub_n (rp, up, vp, n);
  cys = refmpn_rshift (rp, rp, n, 1) >> (GMP_NUMB_BITS - 1);
  rp[n - 1] |= cya << (GMP_NUMB_BITS - 1);
  return cys;
}

mp_limb_t refmpn_add_err1_n (mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_ptr ep, mp_srcptr yp,mp_size_t n, mp_limb_t cy)
{mp_limb_t  h=0,l=0;
 mp_size_t i,j=n-1;
 
 ASSERT(n>=1);
 ASSERT(refmpn_overlap_fullonly_p(rp,up,n));
 ASSERT(refmpn_overlap_fullonly_p(rp,vp,n));
 ASSERT(!refmpn_overlap_p(rp,n,ep,2));
 ASSERT(!refmpn_overlap_p(rp,n,yp,n));
 ASSERT(!refmpn_overlap_p(up,n,ep,2));
 ASSERT(!refmpn_overlap_p(vp,n,ep,2));
 ASSERT(!refmpn_overlap_p(yp,n,ep,2));
for(i=0;i<n;i++,j--)
   {cy=adc(&rp[i],up[i],vp[i],cy);
    if(cy!=0)
      {h+=adc(&l,l,yp[j],0);
      }
   }
ep[0]=l;ep[1]=h;
return cy;}

mp_limb_t refmpn_sub_err1_n (mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_ptr ep, mp_srcptr yp,mp_size_t n, mp_limb_t cy)
{mp_limb_t  h=0,l=0;
 mp_size_t i,j=n-1;
 
 ASSERT(n>=1);
 ASSERT(refmpn_overlap_fullonly_p(rp,up,n));
 ASSERT(refmpn_overlap_fullonly_p(rp,vp,n));
 ASSERT(!refmpn_overlap_p(rp,n,ep,2));
 ASSERT(!refmpn_overlap_p(rp,n,yp,n));
 ASSERT(!refmpn_overlap_p(up,n,ep,2));
 ASSERT(!refmpn_overlap_p(vp,n,ep,2));
 ASSERT(!refmpn_overlap_p(yp,n,ep,2));
for(i=0;i<n;i++,j--)
   {cy=sbb(&rp[i],up[i],vp[i],cy);
    if(cy!=0)
      {h+=adc(&l,l,yp[j],0);
      }
   }
ep[0]=l;ep[1]=h;
return cy;}


mp_limb_t refmpn_add_err2_n (mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_ptr ep, mp_srcptr yp1,mp_srcptr yp2,mp_size_t n, mp_limb_t cy)
{mp_limb_t  h1=0,l1=0,h2=0,l2=0;
 mp_size_t i,j=n-1;
 
 ASSERT(n>=1);
 ASSERT(refmpn_overlap_fullonly_p(rp,up,n));
 ASSERT(refmpn_overlap_fullonly_p(rp,vp,n));
 ASSERT(!refmpn_overlap_p(rp,n,ep,4));
 ASSERT(!refmpn_overlap_p(rp,n,yp1,n));
 ASSERT(!refmpn_overlap_p(rp,n,yp2,n));
 ASSERT(!refmpn_overlap_p(up,n,ep,4));
 ASSERT(!refmpn_overlap_p(vp,n,ep,4));
 ASSERT(!refmpn_overlap_p(yp1,n,ep,4));
 ASSERT(!refmpn_overlap_p(yp2,n,ep,4));
for(i=0;i<n;i++,j--)
   {cy=adc(&rp[i],up[i],vp[i],cy);
    if(cy!=0)
      {h1+=adc(&l1,l1,yp1[j],0);
       h2+=adc(&l2,l2,yp2[j],0);
      }
   }
ep[0]=l1;ep[1]=h1;ep[2]=l2;ep[3]=h2;
return cy;}

mp_limb_t refmpn_sub_err2_n (mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_ptr ep, mp_srcptr yp1,mp_srcptr yp2,mp_size_t n, mp_limb_t cy)
{mp_limb_t  h1=0,l1=0,h2=0,l2=0;
 mp_size_t i,j=n-1;
 
 ASSERT(n>=1);
 ASSERT(refmpn_overlap_fullonly_p(rp,up,n));
 ASSERT(refmpn_overlap_fullonly_p(rp,vp,n));
 ASSERT(!refmpn_overlap_p(rp,n,ep,4));
 ASSERT(!refmpn_overlap_p(rp,n,yp1,n));
 ASSERT(!refmpn_overlap_p(rp,n,yp2,n));
 ASSERT(!refmpn_overlap_p(up,n,ep,4));
 ASSERT(!refmpn_overlap_p(vp,n,ep,4));
 ASSERT(!refmpn_overlap_p(yp1,n,ep,4));
 ASSERT(!refmpn_overlap_p(yp2,n,ep,4));
for(i=0;i<n;i++,j--)
   {cy=sbb(&rp[i],up[i],vp[i],cy);
    if(cy!=0)
      {h1+=adc(&l1,l1,yp1[j],0);
       h2+=adc(&l2,l2,yp2[j],0);
      }
   }
ep[0]=l1;ep[1]=h1;ep[2]=l2;ep[3]=h2;
return cy;}

/* Twos complement, return borrow. */
mp_limb_t
refmpn_neg_n (mp_ptr dst, mp_srcptr src, mp_size_t size)
{
  mp_ptr     zeros;
  mp_limb_t  ret;

  ASSERT (size >= 1);

  zeros = refmpn_malloc_limbs (size);
  refmpn_fill (zeros, size, CNST_LIMB(0));
  ret = refmpn_sub_n (dst, zeros, src, size);
  free (zeros);
  return ret;
}


#define AORS(aors_n, aors_1)                                    \
  {                                                             \
    mp_limb_t  c;                                               \
    ASSERT (s1size >= s2size);                                  \
    ASSERT (s2size >= 1);                                       \
    c = aors_n (rp, s1p, s2p, s2size);                          \
    if (s1size-s2size != 0)                                     \
      c = aors_1 (rp+s2size, s1p+s2size, s1size-s2size, c);     \
    return c;                                                   \
  }
mp_limb_t
refmpn_add (mp_ptr rp,
            mp_srcptr s1p, mp_size_t s1size,
            mp_srcptr s2p, mp_size_t s2size)
{
  AORS (refmpn_add_n, refmpn_add_1);
}
mp_limb_t
refmpn_sub (mp_ptr rp,
            mp_srcptr s1p, mp_size_t s1size,
            mp_srcptr s2p, mp_size_t s2size)
{
  AORS (refmpn_sub_n, refmpn_sub_1);
}


#define SHIFTHIGH(x) ((x) << BITS_PER_MP_LIMB/2)
#define SHIFTLOW(x)  ((x) >> BITS_PER_MP_LIMB/2)

#define LOWMASK   (((mp_limb_t) 1 << BITS_PER_MP_LIMB/2)-1)
#define HIGHMASK  SHIFTHIGH(LOWMASK)

#define LOWPART(x)   ((x) & LOWMASK)
#define HIGHPART(x)  SHIFTLOW((x) & HIGHMASK)

/* Set return:*lo to x*y, using full limbs not nails. */
mp_limb_t
refmpn_umul_ppmm (mp_limb_t *lo, mp_limb_t x, mp_limb_t y)
{
  mp_limb_t  hi, s;

  *lo = LOWPART(x) * LOWPART(y);
  hi = HIGHPART(x) * HIGHPART(y);

  s = LOWPART(x) * HIGHPART(y);
  hi += HIGHPART(s);
  s = SHIFTHIGH(LOWPART(s));
  *lo += s;
  hi += (*lo < s);

  s = HIGHPART(x) * LOWPART(y);
  hi += HIGHPART(s);
  s = SHIFTHIGH(LOWPART(s));
  *lo += s;
  hi += (*lo < s);

  return hi;
}

mp_limb_t
refmpn_umul_ppmm_r (mp_limb_t x, mp_limb_t y, mp_limb_t *lo)
{
  return refmpn_umul_ppmm (lo, x, y);
}

mp_limb_t
refmpn_mul_1c (mp_ptr rp, mp_srcptr sp, mp_size_t size, mp_limb_t multiplier,
               mp_limb_t carry)
{
  mp_size_t  i;
  mp_limb_t  hi, lo;

  ASSERT (refmpn_overlap_low_to_high_p (rp, sp, size));
  ASSERT (size >= 1);
  ASSERT_MPN (sp, size);
  ASSERT_LIMB (multiplier);
  ASSERT_LIMB (carry);

  multiplier <<= GMP_NAIL_BITS;
  for (i = 0; i < size; i++)
    {
      hi = refmpn_umul_ppmm (&lo, sp[i], multiplier);
      lo >>= GMP_NAIL_BITS;
      ASSERT_NOCARRY (ref_addc_limb (&hi, hi, ref_addc_limb (&lo, lo, carry)));
      rp[i] = lo;
      carry = hi;
    }
  return carry;
}

mp_limb_t
refmpn_mul_1 (mp_ptr rp, mp_srcptr sp, mp_size_t size, mp_limb_t multiplier)
{
  return refmpn_mul_1c (rp, sp, size, multiplier, CNST_LIMB(0));
}


mp_limb_t
refmpn_mul_2 (mp_ptr dst, mp_srcptr src, mp_size_t size, mp_srcptr mult)
{
  mp_ptr     src_copy;
  mp_limb_t  c;

  ASSERT (refmpn_overlap_fullonly_p (dst, src, size));
  ASSERT (! refmpn_overlap_p (dst, size+1, mult, (mp_size_t) 2));
  ASSERT (size >= 2);
  ASSERT_MPN (mult, 2);

  /* in case dst==src */
  src_copy = refmpn_malloc_limbs (size);
  refmpn_copyi (src_copy, src, size);
  src = src_copy;

  dst[size] = refmpn_mul_1 (dst, src, size, mult[0]);
  c = refmpn_addmul_1 (dst+1, src, size, mult[1]);
  free (src_copy);
  return c;
}


#define AORSMUL_1C(operation_n)                                 \
  {                                                             \
    mp_ptr     p;                                               \
    mp_limb_t  ret;                                             \
                                                                \
    ASSERT (refmpn_overlap_fullonly_p (rp, sp, size));          \
                                                                \
    p = refmpn_malloc_limbs (size);                             \
    ret = refmpn_mul_1c (p, sp, size, multiplier, carry);       \
    ret += operation_n (rp, rp, p, size);                       \
                                                                \
    free (p);                                                   \
    return ret;                                                 \
  }

mp_limb_t
refmpn_addmul_1c (mp_ptr rp, mp_srcptr sp, mp_size_t size,
                  mp_limb_t multiplier, mp_limb_t carry)
{
  AORSMUL_1C (refmpn_add_n);
}
mp_limb_t
refmpn_submul_1c (mp_ptr rp, mp_srcptr sp, mp_size_t size,
                  mp_limb_t multiplier, mp_limb_t carry)
{
  AORSMUL_1C (refmpn_sub_n);
}


mp_limb_t
refmpn_addmul_1 (mp_ptr rp, mp_srcptr sp, mp_size_t size, mp_limb_t multiplier)
{
  return refmpn_addmul_1c (rp, sp, size, multiplier, CNST_LIMB(0));
}
mp_limb_t
refmpn_submul_1 (mp_ptr rp, mp_srcptr sp, mp_size_t size, mp_limb_t multiplier)
{
  return refmpn_submul_1c (rp, sp, size, multiplier, CNST_LIMB(0));
}


mp_limb_t
refmpn_addmul_N (mp_ptr dst, mp_srcptr src, mp_size_t size,
                 mp_srcptr mult, mp_size_t msize)
{
  mp_ptr     src_copy;
  mp_limb_t  ret;
  mp_size_t  i;

  ASSERT (dst == src || ! refmpn_overlap_p (dst, size+msize-1, src, size));
  ASSERT (! refmpn_overlap_p (dst, size+msize-1, mult, msize));
  ASSERT (size >= msize);
  ASSERT_MPN (mult, msize);

  /* in case dst==src */
  src_copy = refmpn_malloc_limbs (size);
  refmpn_copyi (src_copy, src, size);
  src = src_copy;

  for (i = 0; i < msize-1; i++)
    dst[size+i] = refmpn_addmul_1 (dst+i, src, size, mult[i]);
  ret = refmpn_addmul_1 (dst+i, src, size, mult[i]);

  free (src_copy);
  return ret;
}

mp_limb_t
refmpn_addmul_2 (mp_ptr rp, mp_srcptr sp, mp_size_t size, mp_srcptr mult)
{
  return refmpn_addmul_N (rp, sp, size, mult, (mp_size_t) 2);
}
mp_limb_t
refmpn_addmul_3 (mp_ptr rp, mp_srcptr sp, mp_size_t size, mp_srcptr mult)
{
  return refmpn_addmul_N (rp, sp, size, mult, (mp_size_t) 3);
}
mp_limb_t
refmpn_addmul_4 (mp_ptr rp, mp_srcptr sp, mp_size_t size, mp_srcptr mult)
{
  return refmpn_addmul_N (rp, sp, size, mult, (mp_size_t) 4);
}
mp_limb_t
refmpn_addmul_5 (mp_ptr rp, mp_srcptr sp, mp_size_t size, mp_srcptr mult)
{
  return refmpn_addmul_N (rp, sp, size, mult, (mp_size_t) 5);
}
mp_limb_t
refmpn_addmul_6 (mp_ptr rp, mp_srcptr sp, mp_size_t size, mp_srcptr mult)
{
  return refmpn_addmul_N (rp, sp, size, mult, (mp_size_t) 6);
}
mp_limb_t
refmpn_addmul_7 (mp_ptr rp, mp_srcptr sp, mp_size_t size, mp_srcptr mult)
{
  return refmpn_addmul_N (rp, sp, size, mult, (mp_size_t) 7);
}
mp_limb_t
refmpn_addmul_8 (mp_ptr rp, mp_srcptr sp, mp_size_t size, mp_srcptr mult)
{
  return refmpn_addmul_N (rp, sp, size, mult, (mp_size_t) 8);
}

mp_limb_t
refmpn_sumdiff_nc (mp_ptr r1p, mp_ptr r2p,
                  mp_srcptr s1p, mp_srcptr s2p, mp_size_t size,
                  mp_limb_t carry)
{
  mp_ptr p;
  mp_limb_t acy, scy;

  /* Destinations can't overlap. */
  ASSERT (! refmpn_overlap_p (r1p, size, r2p, size));
  ASSERT (refmpn_overlap_fullonly_two_p (r1p, s1p, s2p, size));
  ASSERT (refmpn_overlap_fullonly_two_p (r2p, s1p, s2p, size));
  ASSERT (size >= 1);

  /* in case r1p==s1p or r1p==s2p */
  p = refmpn_malloc_limbs (size);

  acy = refmpn_add_nc (p, s1p, s2p, size, carry >> 1);
  scy = refmpn_sub_nc (r2p, s1p, s2p, size, carry & 1);
  refmpn_copyi (r1p, p, size);

  free (p);
  return 2 * acy + scy;
}

mp_limb_t
refmpn_nsumdiff_n (mp_ptr r1p, mp_ptr r2p,
                  mp_srcptr s1p, mp_srcptr s2p, mp_size_t size)
{
  mp_ptr p;
  mp_limb_t acy, ncy, scy;

  /* Destinations can't overlap. */
  ASSERT (! refmpn_overlap_p (r1p, size, r2p, size));
  ASSERT (refmpn_overlap_fullonly_two_p (r1p, s1p, s2p, size));
  ASSERT (refmpn_overlap_fullonly_two_p (r2p, s1p, s2p, size));
  ASSERT (size >= 1);

  /* in case r1p==s1p or r1p==s2p */
  p = refmpn_malloc_limbs (size);

  acy = refmpn_add_n (p, s1p, s2p, size);
  ncy = refmpn_neg_n (p, p, size);
  scy = refmpn_sub_n (r2p, s1p, s2p, size);
  refmpn_copyi (r1p, p, size);

  free (p);
  return 2 * (acy + ncy) + scy;
}

mp_limb_t
refmpn_sumdiff_n (mp_ptr r1p, mp_ptr r2p,
		 mp_srcptr s1p, mp_srcptr s2p, mp_size_t size)
{
  return refmpn_sumdiff_nc (r1p, r2p, s1p, s2p, size, CNST_LIMB(0));
}


/* Right shift hi,lo and return the low limb of the result.
   Note a shift by BITS_PER_MP_LIMB isn't assumed to work (doesn't on x86). */
mp_limb_t
rshift_make (mp_limb_t hi, mp_limb_t lo, unsigned shift)
{
  ASSERT (shift < GMP_NUMB_BITS);
  if (shift == 0)
    return lo;
  else
    return ((hi << (GMP_NUMB_BITS-shift)) | (lo >> shift)) & GMP_NUMB_MASK;
}

/* Left shift hi,lo and return the high limb of the result.
   Note a shift by BITS_PER_MP_LIMB isn't assumed to work (doesn't on x86). */
mp_limb_t
lshift_make (mp_limb_t hi, mp_limb_t lo, unsigned shift)
{
  ASSERT (shift < GMP_NUMB_BITS);
  if (shift == 0)
    return hi;
  else
    return ((hi << shift) | (lo >> (GMP_NUMB_BITS-shift))) & GMP_NUMB_MASK;
}


mp_limb_t
refmpn_rshift (mp_ptr rp, mp_srcptr sp, mp_size_t size, unsigned shift)
{
  mp_limb_t  ret;
  mp_size_t  i;

  ASSERT (refmpn_overlap_low_to_high_p (rp, sp, size));
  ASSERT (size >= 1);
  ASSERT (shift >= 1 && shift < GMP_NUMB_BITS);
  ASSERT_MPN (sp, size);

  ret = rshift_make (sp[0], CNST_LIMB(0), shift);

  for (i = 0; i < size-1; i++)
    rp[i] = rshift_make (sp[i+1], sp[i], shift);

  rp[i] = rshift_make (CNST_LIMB(0), sp[i], shift);
  return ret;
}

mp_limb_t
refmpn_lshift (mp_ptr rp, mp_srcptr sp, mp_size_t size, unsigned shift)
{
  mp_limb_t  ret;
  mp_size_t  i;

  ASSERT (refmpn_overlap_high_to_low_p (rp, sp, size));
  ASSERT (size >= 1);
  ASSERT (shift >= 1 && shift < GMP_NUMB_BITS);
  ASSERT_MPN (sp, size);

  ret = lshift_make (CNST_LIMB(0), sp[size-1], shift);

  for (i = size-2; i >= 0; i--)
    rp[i+1] = lshift_make (sp[i+1], sp[i], shift);

  rp[i+1] = lshift_make (sp[i+1], CNST_LIMB(0), shift);
  return ret;
}

mp_limb_t
refmpn_lshiftc (mp_ptr rp, mp_srcptr sp, mp_size_t size, unsigned shift)
{
  mp_limb_t  ret;

  ASSERT (refmpn_overlap_high_to_low_p (rp, sp, size));
  ASSERT (size >= 1);
  ASSERT (shift >= 1 && shift < GMP_NUMB_BITS);
  ASSERT_MPN (sp, size);

  ret=refmpn_lshift(rp,sp,size,shift);
  refmpn_com_n(rp,rp,size);
  return ret;
}

void 
refmpn_not(mp_ptr rp, mp_size_t n)
{
	refmpn_com_n (rp,rp, n);
}

mp_limb_t 
refmpn_lshift1(mp_ptr rp, mp_srcptr xp, mp_size_t n)
{
	return refmpn_lshift (rp, xp, n, 1);
}

mp_limb_t 
refmpn_rshift1(mp_ptr rp, mp_srcptr xp, mp_size_t n)
{
	return refmpn_rshift (rp, xp, n, 1);
}

mp_limb_t 
refmpn_double(mp_ptr rp, mp_size_t n)
{
	return refmpn_lshift (rp, rp, n, 1);
}

mp_limb_t 
refmpn_half(mp_ptr rp, mp_size_t n)
{
	return refmpn_rshift (rp, rp, n, 1);
}

mp_limb_t 
refmpn_lshift2(mp_ptr rp, mp_srcptr xp, mp_size_t n)
{
	return refmpn_lshift (rp, xp, n, 2);
}

mp_limb_t 
refmpn_rshift2(mp_ptr rp, mp_srcptr xp, mp_size_t n)
{
	return refmpn_rshift (rp, xp, n, 2);
}

/* accepting shift==0 and doing a plain copyi or copyd in that case */
mp_limb_t
refmpn_rshift_or_copy (mp_ptr rp, mp_srcptr sp, mp_size_t size, unsigned shift)
{
  if (shift == 0)
    {
      refmpn_copyi (rp, sp, size);
      return 0;
    }
  else
    {
      return refmpn_rshift (rp, sp, size, shift);
    }
}
mp_limb_t
refmpn_lshift_or_copy (mp_ptr rp, mp_srcptr sp, mp_size_t size, unsigned shift)
{
  if (shift == 0)
    {
      refmpn_copyd (rp, sp, size);
      return 0;
    }
  else
    {
      return refmpn_lshift (rp, sp, size, shift);
    }
}

/* accepting size==0 too */
mp_limb_t
refmpn_rshift_or_copy_any (mp_ptr rp, mp_srcptr sp, mp_size_t size,
                           unsigned shift)
{
  return (size == 0 ? 0 : refmpn_rshift_or_copy (rp, sp, size, shift));
}
mp_limb_t
refmpn_lshift_or_copy_any (mp_ptr rp, mp_srcptr sp, mp_size_t size,
                           unsigned shift)
{
  return (size == 0 ? 0 : refmpn_lshift_or_copy (rp, sp, size, shift));
}

/* Divide h,l by d, return quotient, store remainder to *rp.
   Operates on full limbs, not nails.
   Must have h < d.
   __udiv_qrnnd_c isn't simple, and it's a bit slow, but it works. */
mp_limb_t
refmpn_udiv_qrnnd (mp_limb_t *rp, mp_limb_t h, mp_limb_t l, mp_limb_t d)
{
  mp_limb_t  q, r;
  int  n;

  ASSERT (d != 0);
  ASSERT (h < d);

#if 0
  udiv_qrnnd (q, r, h, l, d);
  *rp = r;
  return q;
#endif

  n = refmpn_count_leading_zeros (d);
  d <<= n;

  if (n != 0)
    {
      h = (h << n) | (l >> (GMP_LIMB_BITS - n));
      l <<= n;
    }

  __udiv_qrnnd_c (q, r, h, l, d);
  r >>= n;
  *rp = r;
  return q;
}

mp_limb_t
refmpn_udiv_qrnnd_r (mp_limb_t h, mp_limb_t l, mp_limb_t d, mp_limb_t *rp)
{
  return refmpn_udiv_qrnnd (rp, h, l, d);
}

/* This little subroutine avoids some bad code generation from i386 gcc 3.0
   -fPIC -O2 -fomit-frame-pointer (%ebp being used uninitialized).  */
static mp_limb_t
refmpn_divmod_1c_workaround (mp_ptr rp, mp_srcptr sp, mp_size_t size,
                             mp_limb_t divisor, mp_limb_t carry)
{
  mp_size_t  i;
  mp_limb_t rem[1];
  for (i = size-1; i >= 0; i--)
    {
      rp[i] = refmpn_udiv_qrnnd (rem, carry,
                                 sp[i] << GMP_NAIL_BITS,
                                 divisor << GMP_NAIL_BITS);
      carry = *rem >> GMP_NAIL_BITS;
    }
  return carry;
}

mp_limb_t
refmpn_divmod_1c (mp_ptr rp, mp_srcptr sp, mp_size_t size,
                  mp_limb_t divisor, mp_limb_t carry)
{
  mp_ptr     sp_orig;
  mp_ptr     prod;
  mp_limb_t  carry_orig;

  ASSERT (refmpn_overlap_fullonly_p (rp, sp, size));
  ASSERT (size >= 0);
  ASSERT (carry < divisor);
  ASSERT_MPN (sp, size);
  ASSERT_LIMB (divisor);
  ASSERT_LIMB (carry);

  if (size == 0)
    return carry;

  sp_orig = refmpn_memdup_limbs (sp, size);
  prod = refmpn_malloc_limbs (size);
  carry_orig = carry;

  carry = refmpn_divmod_1c_workaround (rp, sp, size, divisor, carry);

  /* check by multiplying back */
#if 0
  printf ("size=%ld divisor=0x%lX carry=0x%lX remainder=0x%lX\n",
          size, divisor, carry_orig, carry);
  mpn_trace("s",sp_copy,size);
  mpn_trace("r",rp,size);
  printf ("mul_1c %lX\n", refmpn_mul_1c (prod, rp, size, divisor, carry));
  mpn_trace("p",prod,size);
#endif
  ASSERT (refmpn_mul_1c (prod, rp, size, divisor, carry) == carry_orig);
  ASSERT (refmpn_cmp (prod, sp_orig, size) == 0);
  free (sp_orig);
  free (prod);

  return carry;
}

mp_limb_t
refmpn_divmod_1 (mp_ptr rp, mp_srcptr sp, mp_size_t size, mp_limb_t divisor)
{
  return refmpn_divmod_1c (rp, sp, size, divisor, CNST_LIMB(0));
}

mp_limb_t
refmpn_divrem_euclidean_qr_1 (mp_ptr rp, mp_srcptr sp, mp_size_t size, mp_limb_t divisor)
{
  return refmpn_divmod_1c (rp, sp, size, divisor, CNST_LIMB(0));
}

mp_limb_t refmpn_divrem_hensel_qr_1(mp_ptr qp, mp_srcptr xp, mp_size_t n, mp_limb_t d)
{mp_size_t j;mp_limb_t c,h,q,dummy,h1,t,m,r;mp_ptr tp,tqp;

ASSERT(n>0);ASSERT_MPN(xp,n);ASSERT(refmpn_overlap_fullonly_p(qp,xp,n));
ASSERT(d%2==1);modlimb_invert(m,d);
c=0;h=0;t=0;tqp=refmpn_malloc_limbs(n);
for(j=0;j<=n-1;j++)
   {h1=xp[j];
    t=h+c;if(t>h1){h1=h1-t;c=1;}else{h1=h1-t;c=0;}
    q=h1*m;
    tqp[j]=q;
    h=refmpn_umul_ppmm(&dummy,q,d);
    ASSERT(dummy==h1);}
r=h+c;   //  (xp,n) = (tqp,n)*d -ret*B^n    and 0 <= ret < d
tp=refmpn_malloc_limbs(n);
ASSERT(r==refmpn_mul_1(tp,tqp,n,d));
ASSERT(r<d);
ASSERT(refmpn_cmp(tp,xp,n)==0);
refmpn_copy(qp,tqp,n);
free(tp);
free(tqp);
return r;}

mp_limb_t
refmpn_divrem_hensel_rsh_qr_1 (mp_ptr rp, mp_srcptr sp, mp_size_t size, mp_limb_t divisor,int shift)
{
ASSERT(divisor%2==1);ASSERT(shift>=0);// do we want shift ==0 ??
ASSERT(size>0);ASSERT_MPN(sp,size);
ASSERT(refmpn_overlap_fullonly_p(rp,sp,size));
refmpn_rshift(rp,sp,size,shift);
return refmpn_divrem_hensel_qr_1(rp,rp,size,divisor);
}

mp_limb_t
refmpn_rsh_divrem_hensel_qr_1 (mp_ptr rp, mp_srcptr sp, mp_size_t size, mp_limb_t divisor,int shift,mp_limb_t cin)
{mp_limb_t r;mp_ptr tp;
ASSERT(divisor%2==1);ASSERT(shift>=0);// do we want shift ==0 ??
ASSERT(size>0);ASSERT_MPN(sp,size);
ASSERT(refmpn_overlap_fullonly_p(rp,sp,size));
tp=refmpn_malloc_limbs(size);
cin=refmpn_sub_1(tp,sp,size,cin);
r=refmpn_divrem_hensel_qr_1(rp,tp,size,divisor);
refmpn_rshift(rp,rp,size,shift);
free(tp);
return r+cin;}

mp_limb_t
refmpn_divrem_hensel_r_1 (mp_srcptr sp, mp_size_t size, mp_limb_t divisor)
{mp_limb_t r;mp_ptr rp;
ASSERT(divisor%2==1);
ASSERT(size>0);ASSERT_MPN(sp,size);
rp=refmpn_malloc_limbs(size);
r=refmpn_divrem_hensel_qr_1(rp,sp,size,divisor);
free(rp);
return r;}

mp_limb_t
refmpn_mod_1c (mp_srcptr sp, mp_size_t size, mp_limb_t divisor,
               mp_limb_t carry)
{
  mp_ptr  p = refmpn_malloc_limbs (size);
  carry = refmpn_divmod_1c (p, sp, size, divisor, carry);
  free (p);
  return carry;
}

mp_limb_t
refmpn_mod_1 (mp_srcptr sp, mp_size_t size, mp_limb_t divisor)
{
  return refmpn_mod_1c (sp, size, divisor, CNST_LIMB(0));
}

mp_limb_t
refmpn_divrem_euclidean_r_1 (mp_srcptr sp, mp_size_t size, mp_limb_t divisor)
{
  return refmpn_mod_1c (sp, size, divisor, CNST_LIMB(0));
}

mp_limb_t
refmpn_preinv_mod_1 (mp_srcptr sp, mp_size_t size, mp_limb_t divisor,
                     mp_limb_t inverse)
{
  ASSERT (divisor & GMP_NUMB_HIGHBIT);
  ASSERT (inverse == refmpn_invert_limb (divisor));
  return refmpn_mod_1 (sp, size, divisor);
}

/* This implementation will be rather slow, but has the advantage of being
   in a different style than the libmpir versions.  */
mp_limb_t
refmpn_mod_34lsub1 (mp_srcptr p, mp_size_t n)
{
  ASSERT ((GMP_NUMB_BITS % 4) == 0);
  return refmpn_mod_1 (p, n, (CNST_LIMB(1) << (3 * GMP_NUMB_BITS / 4)) - 1);
}


mp_limb_t
refmpn_divrem_1c (mp_ptr rp, mp_size_t xsize,
                  mp_srcptr sp, mp_size_t size, mp_limb_t divisor,
                  mp_limb_t carry)
{
  mp_ptr  z;

  z = refmpn_malloc_limbs (xsize);
  refmpn_fill (z, xsize, CNST_LIMB(0));

  carry = refmpn_divmod_1c (rp+xsize, sp, size, divisor, carry);
  carry = refmpn_divmod_1c (rp, z, xsize, divisor, carry);

  free (z);
  return carry;
}

mp_limb_t
refmpn_divrem_1 (mp_ptr rp, mp_size_t xsize,
                 mp_srcptr sp, mp_size_t size, mp_limb_t divisor)
{
  return refmpn_divrem_1c (rp, xsize, sp, size, divisor, CNST_LIMB(0));
}

mp_limb_t 
refmpn_divexact_byff(mp_ptr rp, mp_srcptr xp, mp_size_t n)
{
  mp_limb_t r, cy;
  mp_limb_t * tp;
  mp_size_t i;
  
  tp = refmpn_malloc_limbs (n);

  /* x = q1*(B - 1) + r1 */
  r = refmpn_divrem_1 (rp, 0, xp, n, ~CNST_LIMB(0));
  if (!r) return r; // r1 == 0
  /* 0 < r1 < B - 1 
     rp[n - 1] <= 1
	  if n > 1 and rp[n - 1] == 1 then rp[n - 2] <= 1, etc  
	  if all limbs are 1 then r == 0, already dealt with */

  /* q = q1 + 1, -r = (B - 1) - r1, x = q*(B - 1) - r*/ 
  refmpn_add_1(rp, rp, n, CNST_LIMB(1));
  r = -(r + 1); 
  /* rp[n - 1] <= 1 
     if rp[n - 1] == 1 then rp[n - 1] <= 1, etc (***)*/
  
  /* x = q*(B - 1) + (B^n - 1)*r - (B^n - 1)*r - r
       = (B - 1)*(q + r + r*B + r*B^2 + ... + r*B^(n-1)) - B^n*r */
  for (i = 0; i < n; i++) tp[i] = r;
  refmpn_add_n(rp, rp, tp, n); /* cannot have carry out of top limb due to (***) */
  
  free(tp);

  return r;
}

mp_limb_t 
refmpn_divexact_byfobm1(mp_ptr qp, mp_srcptr xp, mp_size_t n, mp_limb_t f,mp_limb_t Bm1of)
{mp_size_t j;mp_limb_t c,acc,ax,dx,*tp,*tp1;

ASSERT(n>0);
ASSERT(qp==xp || !refmpn_overlap_p(xp,n,qp,n));
ASSERT(Bm1of*f+1==0);
acc=0*Bm1of;
tp1 = refmpn_malloc_limbs (n);refmpn_copy(tp1,xp,n);
for(j=0;j<=n-1;j++)
   {dx=refmpn_umul_ppmm(&ax,xp[j],Bm1of);
    c=ref_subc_limb(&acc,acc,ax);
    qp[j]=acc;
    acc-=dx+c;}
//return next quotient*-f
acc=acc*-f;
ASSERT(acc<f);
tp = refmpn_malloc_limbs (n);
c=refmpn_mul_1(tp,qp,n,f);
ASSERT(c==acc);
ASSERT(refmpn_cmp(tp1,tp,n)==0);
free(tp);free(tp1);
return acc;}   // so  (xp,n) = (qp,n)*f -ret*B^n    and 0 <= ret < f 

mp_limb_t
refmpn_preinv_divrem_1 (mp_ptr rp, mp_size_t xsize,
                        mp_srcptr sp, mp_size_t size,
                        mp_limb_t divisor, mp_limb_t inverse, unsigned shift)
{
  ASSERT (size >= 0);
  ASSERT (shift == refmpn_count_leading_zeros (divisor));
  ASSERT (inverse == refmpn_invert_limb (divisor << shift));

  return refmpn_divrem_1 (rp, xsize, sp, size, divisor);
}


/* Inverse is floor((b*(b-d)-1) / d), per division by invariant integers
   paper, figure 8.1 m', where b=2^BITS_PER_MP_LIMB.  Note that -d-1 < d
   since d has the high bit set. */

mp_limb_t
refmpn_invert_limb (mp_limb_t d)
{
  mp_limb_t r;
  ASSERT (d & GMP_LIMB_HIGHBIT);
  return refmpn_udiv_qrnnd (&r, -d-1, MP_LIMB_T_MAX, d);
}


/* The aim is to produce a dst quotient and return a remainder c, satisfying
   c*b^n + src-i == 3*dst, where i is the incoming carry.

   Some value c==0, c==1 or c==2 will satisfy, so just try each.

   If GMP_NUMB_BITS is even then 2^GMP_NUMB_BITS==1mod3 and a non-zero
   remainder from the first division attempt determines the correct
   remainder (3-c), but don't bother with that, since we can't guarantee
   anything about GMP_NUMB_BITS when using nails.

   If the initial src-i produces a borrow then refmpn_sub_1 leaves a twos
   complement negative, ie. b^n+a-i, and the calculation produces c1
   satisfying c1*b^n + b^n+src-i == 3*dst, from which clearly c=c1+1.  This
   means it's enough to just add any borrow back at the end.

   A borrow only occurs when a==0 or a==1, and, by the same reasoning as in
   mpn/generic/diveby3.c, the c1 that results in those cases will only be 0
   or 1 respectively, so with 1 added the final return value is still in the
   prescribed range 0 to 2. */

mp_limb_t
refmpn_divexact_by3c (mp_ptr rp, mp_srcptr sp, mp_size_t size, mp_limb_t carry)
{
  mp_ptr     spcopy;
  mp_limb_t  c, cs;

  ASSERT (refmpn_overlap_fullonly_p (rp, sp, size));
  ASSERT (size >= 1);
  ASSERT (carry <= 2);
  ASSERT_MPN (sp, size);

  spcopy = refmpn_malloc_limbs (size);
  cs = refmpn_sub_1 (spcopy, sp, size, carry);

  for (c = 0; c <= 2; c++)
    if (refmpn_divmod_1c (rp, spcopy, size, CNST_LIMB(3), c) == 0)
      goto done;
  ASSERT_FAIL (no value of c satisfies);

 done:
  c += cs;
  ASSERT (c <= 2);

  free (spcopy);
  return c;
}

mp_limb_t
refmpn_divexact_by3 (mp_ptr rp, mp_srcptr sp, mp_size_t size)
{
  return refmpn_divexact_by3c (rp, sp, size, CNST_LIMB(0));
}


/* The same as mpn/generic/mul_basecase.c, but using refmpn functions. */
void
refmpn_mul_basecase (mp_ptr prodp,
                     mp_srcptr up, mp_size_t usize,
                     mp_srcptr vp, mp_size_t vsize)
{
  mp_size_t i;

  ASSERT (! refmpn_overlap_p (prodp, usize+vsize, up, usize));
  ASSERT (! refmpn_overlap_p (prodp, usize+vsize, vp, vsize));
  ASSERT (usize >= vsize);
  ASSERT (vsize >= 1);
  ASSERT_MPN (up, usize);
  ASSERT_MPN (vp, vsize);

  prodp[usize] = refmpn_mul_1 (prodp, up, usize, vp[0]);
  for (i = 1; i < vsize; i++)
    prodp[usize+i] = refmpn_addmul_1 (prodp+i, up, usize, vp[i]);
}

void
refmpn_mul_n (mp_ptr prodp, mp_srcptr up, mp_srcptr vp, mp_size_t size)
{
  refmpn_mul_basecase (prodp, up, size, vp, size);
}

void
refmpn_sqr (mp_ptr dst, mp_srcptr src, mp_size_t size)
{
  refmpn_mul_basecase (dst, src, size, src, size);
}

/* Allowing usize<vsize, usize==0 or vsize==0. */
void
refmpn_mul_any (mp_ptr prodp,
                     mp_srcptr up, mp_size_t usize,
                     mp_srcptr vp, mp_size_t vsize)
{
  ASSERT (! refmpn_overlap_p (prodp, usize+vsize, up, usize));
  ASSERT (! refmpn_overlap_p (prodp, usize+vsize, vp, vsize));
  ASSERT (usize >= 0);
  ASSERT (vsize >= 0);
  ASSERT_MPN (up, usize);
  ASSERT_MPN (vp, vsize);

  if (usize == 0)
    {
      refmpn_fill (prodp, vsize, CNST_LIMB(0));
      return;
    }

  if (vsize == 0)
    {
      refmpn_fill (prodp, usize, CNST_LIMB(0));
      return;
    }

  if (usize >= vsize)
    refmpn_mul_basecase (prodp, up, usize, vp, vsize);
  else
    refmpn_mul_basecase (prodp, vp, vsize, up, usize);
}


/* Reference implementation of mpn_mulmid for testing.

   Algorithm: use mpn_mul and then subtract off O(n) cross-terms.
   Must have un >= vn >= 1. */
void
refmpn_mulmid_basecase (mp_ptr rp,
               mp_srcptr up, mp_size_t un,
               mp_srcptr vp, mp_size_t vn)
{
  mp_ptr temp;
  mp_limb_t diag[4];
  mp_limb_t hi;
  mp_size_t i, k;

  ASSERT (un >= vn);
  ASSERT (vn >= 1);

  /* special case */
  if (vn == 1)
    {
      rp[un] = refmpn_mul_1 (rp, up, un, vp[0]);
      rp[un+1] = 0;
      return;
    }

  /* compute plain product */
  temp = refmpn_malloc_limbs (un + vn);
  ASSERT (temp != NULL);
  refmpn_mul_any (temp, up, un, vp, vn);

  /* remove the cross-terms that interfere with the result we want, i.e. in
     the following diagram, we want only contributions from O's, but mpn_mul
     has given us all of O, A and X, and we will remove the A's.

     OOOOAAXX
     AOOOOAAX
     AAOOOOAA
     XAAOOOOA
     XXAAOOOO
  */

  /* bottom-left diagonal */
  diag[0] = diag[3] = 0;
  diag[2] = mpn_mul_1 (diag + 1, up, 1, vp[vn-2]);
  for (i = 0; i < vn - 2; i++)
    {
      hi = refmpn_addmul_1 (diag, up + vn - i - 3, 2, vp[i]);
      refmpn_add_1 (diag + 2, diag + 2, 2, hi);
    }
  k = (vn == 2);
  refmpn_sub (temp + vn - 3 + k, temp + vn - 3 + k, un - vn + 5 - k,
           diag + k, 4 - k);

  /* top-right diagonal */
  diag[1] = refmpn_mul_1 (diag, up + un - 1, 1, vp[1]);
  for (i = 2; i < vn; i++)
    refmpn_addmul_1 (diag, up + un - i, 2, vp[i]);
  refmpn_sub (temp + un, temp + un, 2, diag, 2);

  /* copy result to rp */
  refmpn_copy (rp, temp + vn - 1, un - vn + 3);
  refmpn_free_limbs (temp);
}

void
refmpn_mulmid (mp_ptr rp,
               mp_srcptr up, mp_size_t un,
               mp_srcptr vp, mp_size_t vn)
{
  refmpn_mulmid_basecase(rp, up, un, vp, vn);
}

void
refmpn_mulmid_n (mp_ptr rp,
               mp_srcptr up, mp_srcptr vp, mp_size_t n)
{
  refmpn_mulmid(rp, up, 2*n-1, vp, n);
}


mp_limb_t
refmpn_gcd_1 (mp_srcptr xp, mp_size_t xsize, mp_limb_t y)
{
  mp_limb_t  x;
  int  twos;

  ASSERT (y != 0);
  ASSERT (! refmpn_zero_p (xp, xsize));
  ASSERT_MPN (xp, xsize);
  ASSERT_LIMB (y);

  x = refmpn_mod_1 (xp, xsize, y);
  if (x == 0)
    return y;

  twos = 0;
  while ((x & 1) == 0 && (y & 1) == 0)
    {
      x >>= 1;
      y >>= 1;
      twos++;
    }

  for (;;)
    {
      while ((x & 1) == 0)  x >>= 1;
      while ((y & 1) == 0)  y >>= 1;

      if (x < y)
        MP_LIMB_T_SWAP (x, y);

      x -= y;
      if (x == 0)
        break;
    }

  return y << twos;
}


/* Based on the full limb x, not nails. */
unsigned
refmpn_count_leading_zeros (mp_limb_t x)
{
  unsigned  n = 0;

  ASSERT (x != 0);

  while ((x & GMP_LIMB_HIGHBIT) == 0)
    {
      x <<= 1;
      n++;
    }
  return n;
}

/* Full limbs allowed, not limited to nails. */
unsigned
refmpn_count_trailing_zeros (mp_limb_t x)
{
  unsigned  n = 0;

  ASSERT (x != 0);
  ASSERT_LIMB (x);

  while ((x & 1) == 0)
    {
      x >>= 1;
      n++;
    }
  return n;
}

/* Strip factors of two (low zero bits) from {p,size} by right shifting.
   The return value is the number of twos stripped.  */
mp_size_t
refmpn_strip_twos (mp_ptr p, mp_size_t size)
{
  mp_size_t  limbs;
  unsigned   shift;

  ASSERT (size >= 1);
  ASSERT (! refmpn_zero_p (p, size));
  ASSERT_MPN (p, size);

  for (limbs = 0; p[0] == 0; limbs++)
    {
      refmpn_copyi (p, p+1, size-1);
      p[size-1] = 0;
    }

  shift = refmpn_count_trailing_zeros (p[0]);
  if (shift)
    refmpn_rshift (p, p, size, shift);

  return limbs*GMP_NUMB_BITS + shift;
}

mp_limb_t
refmpn_gcd (mp_ptr gp, mp_ptr xp, mp_size_t xsize, mp_ptr yp, mp_size_t ysize)
{
  int       cmp;

  ASSERT (ysize >= 1);
  ASSERT (xsize >= ysize);
  ASSERT ((xp[0] & 1) != 0);
  ASSERT ((yp[0] & 1) != 0);
  /* ASSERT (xp[xsize-1] != 0); */  /* don't think x needs to be odd */
  ASSERT (yp[ysize-1] != 0);
  ASSERT (refmpn_overlap_fullonly_p (gp, xp, xsize));
  ASSERT (refmpn_overlap_fullonly_p (gp, yp, ysize));
  ASSERT (! refmpn_overlap_p (xp, xsize, yp, ysize));
  if (xsize == ysize)
    ASSERT (refmpn_msbone (xp[xsize-1]) >= refmpn_msbone (yp[ysize-1]));
  ASSERT_MPN (xp, xsize);
  ASSERT_MPN (yp, ysize);

  refmpn_strip_twos (xp, xsize);
  MPN_NORMALIZE (xp, xsize);
  MPN_NORMALIZE (yp, ysize);

  for (;;)
    {
      cmp = refmpn_cmp_twosizes (xp, xsize, yp, ysize);
      if (cmp == 0)
        break;
      if (cmp < 0)
        MPN_PTR_SWAP (xp,xsize, yp,ysize);

      ASSERT_NOCARRY (refmpn_sub (xp, xp, xsize, yp, ysize));

      refmpn_strip_twos (xp, xsize);
      MPN_NORMALIZE (xp, xsize);
    }

  refmpn_copyi (gp, xp, xsize);
  return xsize;
}

unsigned long
ref_popc_limb (mp_limb_t src)
{
  unsigned long  count;
  int  i;

  count = 0;
  for (i = 0; i < GMP_LIMB_BITS; i++)
    {
      count += (src & 1);
      src >>= 1;
    }
  return count;
}

unsigned long
refmpn_popcount (mp_srcptr sp, mp_size_t size)
{
  unsigned long  count = 0;
  mp_size_t  i;

  ASSERT (size >= 0);
  ASSERT_MPN (sp, size);

  for (i = 0; i < size; i++)
    count += ref_popc_limb (sp[i]);
  return count;
}

unsigned long
refmpn_hamdist (mp_srcptr s1p, mp_srcptr s2p, mp_size_t size)
{
  mp_ptr  d;
  unsigned long  count;

  ASSERT (size >= 0);
  ASSERT_MPN (s1p, size);
  ASSERT_MPN (s2p, size);

  if (size == 0)
    return 0;

  d = refmpn_malloc_limbs (size);
  refmpn_xor_n (d, s1p, s2p, size);
  count = refmpn_popcount (d, size);
  free (d);
  return count;
}


/* set r to a%d */
void
refmpn_mod2 (mp_limb_t r[2], const mp_limb_t a[2], const mp_limb_t d[2])
{
  mp_limb_t  D[2];
  int        n;

  ASSERT (! refmpn_overlap_p (r, (mp_size_t) 2, d, (mp_size_t) 2));
  ASSERT_MPN (a, 2);
  ASSERT_MPN (d, 2);

  D[1] = d[1], D[0] = d[0];
  r[1] = a[1], r[0] = a[0];
  n = 0;

  for (;;)
    {
      if (D[1] & GMP_NUMB_HIGHBIT)
        break;
      if (refmpn_cmp (r, D, (mp_size_t) 2) <= 0)
        break;
      refmpn_lshift (D, D, (mp_size_t) 2, 1);
      n++;
      ASSERT (n <= GMP_NUMB_BITS);
    }

  while (n >= 0)
    {
      if (refmpn_cmp (r, D, (mp_size_t) 2) >= 0)
        ASSERT_NOCARRY (refmpn_sub_n (r, r, D, (mp_size_t) 2));
      refmpn_rshift (D, D, (mp_size_t) 2, 1);
      n--;
    }

  ASSERT (refmpn_cmp (r, d, (mp_size_t) 2) < 0);
}


/* Similar to mpn/generic/sb_divrem_mn.c, but somewhat simplified, in
   particular the trial quotient is allowed to be 2 too big. */
mp_limb_t
refmpn_sb_divrem_mn (mp_ptr qp,
                     mp_ptr np, mp_size_t nsize,
                     mp_srcptr dp, mp_size_t dsize)
{
  mp_limb_t  retval = 0;
  mp_size_t  i;
  mp_limb_t  d1 = dp[dsize-1];
  mp_ptr     np_orig = refmpn_memdup_limbs (np, nsize);

  ASSERT (nsize >= dsize);
  /* ASSERT (dsize > 2); */
  ASSERT (dsize >= 2);
  ASSERT (dp[dsize-1] & GMP_NUMB_HIGHBIT);
  ASSERT (! refmpn_overlap_p (qp, nsize-dsize, np, nsize) || qp+dsize >= np);
  ASSERT_MPN (np, nsize);
  ASSERT_MPN (dp, dsize);

  i = nsize-dsize;
  if (refmpn_cmp (np+i, dp, dsize) >= 0)
    {
      ASSERT_NOCARRY (refmpn_sub_n (np+i, np+i, dp, dsize));
      retval = 1;
    }

  for (i--; i >= 0; i--)
    {
      mp_limb_t  n0 = np[i+dsize];
      mp_limb_t  n1 = np[i+dsize-1];
      mp_limb_t  q, dummy_r;

      ASSERT (n0 <= d1);
      if (n0 == d1)
        q = GMP_NUMB_MAX;
      else
        q = refmpn_udiv_qrnnd (&dummy_r, n0, n1 << GMP_NAIL_BITS,
                               d1 << GMP_NAIL_BITS);

      n0 -= refmpn_submul_1 (np+i, dp, dsize, q);
      ASSERT (n0 == 0 || n0 == MP_LIMB_T_MAX);
      if (n0)
        {
          q--;
          if (! refmpn_add_n (np+i, np+i, dp, dsize))
            {
              q--;
              ASSERT_CARRY (refmpn_add_n (np+i, np+i, dp, dsize) != 0);
            }
        }
      np[i+dsize] = 0;

      qp[i] = q;
    }

  /* remainder < divisor */
  ASSERT (refmpn_cmp (np, dp, dsize) < 0);

  /* multiply back to original */
  {
    mp_ptr  mp = refmpn_malloc_limbs (nsize);

    refmpn_mul_any (mp, qp, nsize-dsize, dp, dsize);
    if (retval)
      ASSERT_NOCARRY (refmpn_add_n (mp+nsize-dsize,mp+nsize-dsize, dp, dsize));
    ASSERT_NOCARRY (refmpn_add (mp, mp, nsize, np, dsize));
    ASSERT (refmpn_cmp (mp, np_orig, nsize) == 0);

    free (mp);
  }

  free (np_orig);
  return retval;
}

/* Similar to mpn/generic/sb_divrem_mn.c, but somewhat simplified, in
   particular the trial quotient is allowed to be 2 too big. */
void
refmpn_tdiv_qr (mp_ptr qp, mp_ptr rp, mp_size_t qxn,
                mp_ptr np, mp_size_t nsize,
                mp_srcptr dp, mp_size_t dsize)
{
  ASSERT (qxn == 0);
  ASSERT_MPN (np, nsize);
  ASSERT_MPN (dp, dsize);
  ASSERT (dsize > 0);
  ASSERT (dp[dsize-1] != 0);

  if (dsize == 1)
    {
      rp[0] = refmpn_divmod_1 (qp, np, nsize, dp[0]);
      return;
    }
  else
    {
      mp_ptr  n2p = refmpn_malloc_limbs (nsize+1);
      mp_ptr  d2p = refmpn_malloc_limbs (dsize);
      int     norm = refmpn_count_leading_zeros (dp[dsize-1]) - GMP_NAIL_BITS;

      n2p[nsize] = refmpn_lshift_or_copy (n2p, np, nsize, norm);
      ASSERT_NOCARRY (refmpn_lshift_or_copy (d2p, dp, dsize, norm));

      refmpn_sb_divrem_mn (qp, n2p, nsize+1, d2p, dsize);
      refmpn_rshift_or_copy (rp, n2p, dsize, norm);

      /* ASSERT (refmpn_zero_p (tp+dsize, nsize-dsize)); */
      free (n2p);
      free (d2p);
    }
}

void
refmpn_tdiv_q (mp_ptr qp, mp_ptr np, mp_size_t nsize,
                mp_srcptr dp, mp_size_t dsize)
{
  ASSERT_MPN (np, nsize);
  ASSERT_MPN (dp, dsize);
  ASSERT (dsize > 0);
  ASSERT (dp[dsize-1] != 0);

  if (dsize == 1)
    {
      refmpn_divmod_1 (qp, np, nsize, dp[0]);
      return;
    }
  else
    {
      mp_ptr  n2p = refmpn_malloc_limbs (nsize+1);
      mp_ptr  d2p = refmpn_malloc_limbs (dsize);
      int     norm = refmpn_count_leading_zeros (dp[dsize-1]);

      n2p[nsize] = refmpn_lshift_or_copy (n2p, np, nsize, norm);
      ASSERT_NOCARRY (refmpn_lshift_or_copy (d2p, dp, dsize, norm));

      refmpn_sb_divrem_mn (qp, n2p, nsize+1, d2p, dsize);
      
      /* ASSERT (refmpn_zero_p (tp+dsize, nsize-dsize)); */
      free (n2p);
      free (d2p);
    }
}


size_t
refmpn_get_str (unsigned char *dst, int base, mp_ptr src, mp_size_t size)
{
  unsigned char  *d;
  size_t  dsize;

  ASSERT (size >= 0);
  ASSERT (base >= 2);
  ASSERT (base < numberof (__mp_bases));
  ASSERT (size == 0 || src[size-1] != 0);
  ASSERT_MPN (src, size);

  MPN_SIZEINBASE (dsize, src, size, base);
  ASSERT (dsize >= 1);
  ASSERT (! byte_overlap_p (dst, (mp_size_t) dsize, src, size * BYTES_PER_MP_LIMB));

  if (size == 0)
    {
      dst[0] = 0;
      return 1;
    }

  /* don't clobber input for power of 2 bases */
  if (POW2_P (base))
    src = refmpn_memdup_limbs (src, size);

  d = dst + dsize;
  do
    {
      d--;
      ASSERT (d >= dst);
      *d = refmpn_divrem_1 (src, (mp_size_t) 0, src, size, (mp_limb_t) base);
      size -= (src[size-1] == 0);
    }
  while (size != 0);

  /* Move result back and decrement dsize if we didn't generate
     the maximum possible digits.  */
  if (d != dst)
    {
      size_t i;
      dsize -= d - dst;
      for (i = 0; i < dsize; i++)
	dst[i] = d[i];
    }

  if (POW2_P (base))
    free (src);

  return dsize;
}


mp_limb_t
ref_bswap_limb (mp_limb_t src)
{
  mp_limb_t  dst;
  int        i;

  dst = 0;
  for (i = 0; i < BYTES_PER_MP_LIMB; i++)
    {
      dst = (dst << 8) + (src & 0xFF);
      src >>= 8;
    }
  return dst;
}


/* These random functions are mostly for transitional purposes while adding
   nail support, since they're independent of the normal mpn routines.  They
   can probably be removed when those normal routines are reliable, though
   perhaps something independent would still be useful at times.  */

#if BITS_PER_MP_LIMB == 32
#define RAND_A  CNST_LIMB(0x29CF535)
#endif
#if BITS_PER_MP_LIMB == 64
#define RAND_A  CNST_LIMB(0xBAECD515DAF0B49D)
#endif

mp_limb_t  refmpn_random_seed;

mp_limb_t
refmpn_random_half (void)
{
  refmpn_random_seed = refmpn_random_seed * RAND_A + 1;
  return (refmpn_random_seed >> BITS_PER_MP_LIMB/2);
}

mp_limb_t
refmpn_random_limb (void)
{
  return ((refmpn_random_half () << (BITS_PER_MP_LIMB/2))
           | refmpn_random_half ()) & GMP_NUMB_MASK;
}

void
refmpn_random (mp_ptr ptr, mp_size_t size)
{
  mp_size_t  i;
  if (GMP_NAIL_BITS == 0)
    {
      mpn_random (ptr, size);
      return;
    }

  for (i = 0; i < size; i++)
    ptr[i] = refmpn_random_limb ();
}

void
refmpn_random2 (mp_ptr ptr, mp_size_t size)
{
  mp_size_t  i;
  mp_limb_t  bit, mask, limb;
  int        run;
  
  if (GMP_NAIL_BITS == 0)
    {
      mpn_random2 (ptr, size);
      return;
    }

#define RUN_MODULUS  32

  /* start with ones at a random pos in the high limb */
  bit = CNST_LIMB(1) << (refmpn_random_half () % GMP_NUMB_BITS);
  mask = 0;
  run = 0;

  for (i = size-1; i >= 0; i--)
    {
      limb = 0;
      do
        {
          if (run == 0)
            {
              run = (refmpn_random_half () % RUN_MODULUS) + 1;
              mask = ~mask;
            }

          limb |= (bit & mask);
          bit >>= 1;
          run--;
        }
      while (bit != 0);

      ptr[i] = limb;
      bit = GMP_NUMB_HIGHBIT;
    }
}

/* This is a simple bitwise algorithm working high to low across "s" and
   testing each time whether setting the bit would make s^2 exceed n.  */
mp_size_t
refmpn_sqrtrem (mp_ptr sp, mp_ptr rp, mp_srcptr np, mp_size_t nsize)
{
  mp_ptr     tp, dp;
  mp_size_t  ssize, talloc, tsize, dsize, ret, ilimbs;
  unsigned   ibit;
  long       i;
  mp_limb_t  c;

  ASSERT (nsize >= 0);

  /* If n==0, then s=0 and r=0.  */
  if (nsize == 0)
    return 0;

  ASSERT (np[nsize - 1] != 0);
  ASSERT (rp == NULL || MPN_SAME_OR_SEPARATE_P (np, rp, nsize));
  ASSERT (rp == NULL || ! MPN_OVERLAP_P (sp, (nsize + 1) / 2, rp, nsize));
  ASSERT (! MPN_OVERLAP_P (sp, (nsize + 1) / 2, np, nsize));

  /* root */
  ssize = (nsize+1)/2;
  refmpn_zero (sp, ssize);

  /* the remainder so far */
  dp = refmpn_memdup_limbs (np, nsize);
  dsize = nsize;

  /* temporary */
  talloc = 2*ssize + 1;
  tp = refmpn_malloc_limbs (talloc);

  for (i = GMP_NUMB_BITS * ssize - 1; i >= 0; i--)
    {
      /* t = 2*s*2^i + 2^(2*i), being the amount s^2 will increase by if 2^i
         is added to it */

      ilimbs = (i+1) / GMP_NUMB_BITS;
      ibit = (i+1) % GMP_NUMB_BITS;
      refmpn_zero (tp, ilimbs);
      c = refmpn_lshift_or_copy (tp+ilimbs, sp, ssize, ibit);
      tsize = ilimbs + ssize;
      tp[tsize] = c;
      tsize += (c != 0);

      ilimbs = (2*i) / GMP_NUMB_BITS;
      ibit = (2*i) % GMP_NUMB_BITS;
      if (ilimbs + 1 > tsize)
        {
          refmpn_zero_extend (tp, tsize, ilimbs + 1);
          tsize = ilimbs + 1;
        }
      c = refmpn_add_1 (tp+ilimbs, tp+ilimbs, tsize-ilimbs,
                        CNST_LIMB(1) << ibit);
      ASSERT (tsize < talloc);
      tp[tsize] = c;
      tsize += (c != 0);

      if (refmpn_cmp_twosizes (dp, dsize, tp, tsize) >= 0)
        {
          /* set this bit in s and subtract from the remainder */
          refmpn_setbit (sp, i);

          ASSERT_NOCARRY (refmpn_sub_n (dp, dp, tp, dsize));
          dsize = refmpn_normalize (dp, dsize);
        }
    }

  if (rp == NULL)
    {
      ret = ! refmpn_zero_p (dp, dsize);
    }
  else
    {
      ASSERT (dsize == 0 || dp[dsize-1] != 0);
      refmpn_copy (rp, dp, dsize);
      ret = dsize;
    }

  free (dp);
  free (tp);
  return ret;
}

void 
refmpn_redc_1 (mp_ptr cp, mp_ptr tp, mp_srcptr mp, mp_size_t n, mp_limb_t Nd)
{
  mp_limb_t cy, q;
  mp_size_t j;
      
  ASSERT_MPN (tp, 2*n);
        
  for (j = 0; j < n; j++)
  {
    q = (tp[0] * Nd) & GMP_NUMB_MASK;
    tp[0] = refmpn_addmul_1 (tp, mp, n, q);
    tp++;
  }
   
  cy = refmpn_add_n (cp, tp, tp - n, n);
  
  if (cy) refmpn_sub_n (cp, cp, mp, n);
}
