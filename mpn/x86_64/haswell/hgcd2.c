/* hgcd2.c

   THE FUNCTIONS IN THIS FILE ARE INTERNAL WITH MUTABLE INTERFACES.  IT IS ONLY
   SAFE TO REACH THEM THROUGH DOCUMENTED INTERFACES.  IN FACT, IT IS ALMOST
   GUARANTEED THAT THEY'LL CHANGE OR DISAPPEAR IN A FUTURE GNU MP RELEASE.

Copyright 1996, 1998, 2000, 2001, 2002, 2003, 2004, 2008, 2012 Free Software
Foundation, Inc.

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

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"

#if GMP_NAIL_BITS == 0

/* Copied from the old mpn/generic/gcdext.c, and modified slightly to return
   the remainder. */

static inline unsigned long
lzcnt(const unsigned long a)
{
  unsigned long t;
  __asm__("lzcnt %1, %0"
          : "=r" (t)
          : "r" (a)
          : "cc");
  return t;
}

static inline unsigned long
shld(const unsigned long hi, const unsigned long lo, const unsigned char i)
{
  unsigned long r;
  __asm__ (
    "shldq %3, %1, %0\n"
    : "=rm" (r)
    : "r" (lo), "0" (hi), "cJ" (i) /* i can be in %cl or a literal constant < 64 */
    : "cc");
 return r;
}

static inline unsigned long
shrd(const unsigned long hi, const unsigned long lo, const unsigned char i)
{
  unsigned long r;
  __asm__ (
    "shrdq %3, %2, %0\n"
    : "=rm" (r)
    : "0" (lo), "r" (hi), "cJ" (i) /* i can be in %cl or a literal constant < 64 */
    : "cc");
 return r;
}

/* if(s1>=s2){r=s1-s2;c++;}
   r and s1 can be the same variable, but no other alias is allowed */
#define TRY_SUB(r, s1, s2, c) \
do { \
    mp_limb_t t = s1; \
    __asm__( \
    "sub     %3, %1\n\t" /* t -= s2; */ \
    "cmovnc  %1, %0\n\t" /* if (s1 >= s2) r = s1;  */ \
    "sbb     $-1, %2\n\t" /* if (t >= s) c++; */ \
    : "+&r" (r), "+&r" (t), "+rm" (c) \
    : "rm" (s2) \
    : "cc"); \
} while(0)

#define TRY_SUB2(rhi, rlo, shi, slo, c) \
do { \
    const mp_limb_t thi = rhi, tlo = rlo; \
    __asm__( \
    "sub     %6, %1\n\t" /* r -= s; */ \
    "sbb     %5, %0\n\t" \
    "cmovc   %4, %1\n\t" /* if (t < s) r = t;  */ \
    "cmovc   %3, %0\n\t" \
    "sbb     $-1, %2\n\t" /* if (t >= s) c++; */ \
    : "+&r" (rhi), "+&r" (rlo), "+r" (c) \
    : "rm" (thi), "rm" (tlo), "rm" (shi), "rm" (slo) \
    : "cc"); \
} while(0)

static inline mp_limb_t
cmp2(const mp_limb_t ahi, const mp_limb_t alo, const mp_limb_t bhi, const mp_limb_t blo)
{
  mp_limb_t r = ahi;
  __asm__(
  "cmpq %3, %1\n\t"
  "sbbq %2, %0\n\t"
  "sbbq %0, %0\n\t"
  : "+&r" (r)
  : "r" (alo), "rm" (bhi), "rm" (blo)
  : "cc");
  return r;
}

/* Single-limb division optimized for small quotients. */
static inline mp_limb_t
div1 (mp_ptr rp,
      mp_limb_t n0,
      mp_limb_t d0)
{
  mp_limb_t q = 0;
  // const mp_limb_t correct_q = n0 / d0, correct_r = n0 % d0;

  int cnt = lzcnt(d0) - lzcnt(n0);
  d0 <<= cnt; /* (d0+1)/2 <= n0 <= 2*d0 - 1 */

  q = 0;
  TRY_SUB(n0, n0, d0, q);
  while (cnt)
    {
      q <<= 1;
      d0 >>= 1;
      TRY_SUB(n0, n0, d0, q);
      cnt--;
    }

  // ASSERT_ALWAYS(n0 == correct_r);
  // ASSERT_ALWAYS(q != correct_q);
  *rp = n0;
  return q;
}

/* Two-limb division optimized for small quotients.  */
static inline mp_limb_t
div2 (mp_ptr rp,
      mp_limb_t nh, mp_limb_t nl,
      mp_limb_t dh, mp_limb_t dl)
{
  mp_limb_t q = 0;

  int cnt = lzcnt(dh) - lzcnt(nh);
  dh = shld(dh, dl, cnt);
  dl <<= cnt;

  TRY_SUB2(nh, nl, dh, dl, q);

  while (cnt)
    {
      q <<= 1;
      dl = shrd(dh, dl, 1);
      dh = dh >> 1;
      TRY_SUB2(nh, nl, dh, dl, q);
      cnt--;
    }

  rp[0] = nl;
  rp[1] = nh;

  return q;
}

#else /* GMP_NAIL_BITS != 0 */
/* Check all functions for nail support. */
/* hgcd2 should be defined to take inputs including nail bits, and
   produce a matrix with elements also including nail bits. This is
   necessary, for the matrix elements to be useful with mpn_mul_1,
   mpn_addmul_1 and friends. */
#error Not implemented
#endif /* GMP_NAIL_BITS != 0 */

/* Reduces a,b until |a-b| (almost) fits in one limb + 1 bit. Constructs
   matrix M. Returns 1 if we make progress, i.e. can perform at least
   one subtraction. Otherwise returns zero. */

/* FIXME: Possible optimizations:

   The div2 function starts with checking the most significant bit of
   the numerator. We can maintained normalized operands here, call
   hgcd with normalized operands only, which should make the code
   simpler and possibly faster.

   Experiment with table lookups on the most significant bits.

   This function is also a candidate for assembler implementation.
*/
int
mpn_hgcd2 (mp_limb_t ah, mp_limb_t al, mp_limb_t bh, mp_limb_t bl,
	   struct hgcd_matrix1 *M)
{
  mp_limb_t u00, u01, u10, u11;

  if (ah < 2 || bh < 2)
    return 0;

  u00 = u11 = 1;
  if (cmp2(ah, al, bh, bl) == 0)
    {
      sub_ddmmss (ah, al, ah, al, bh, bl);
      if (ah < 2)
	return 0;

      u01 = 1;
      u10 = 0;
    }
  else
    {
      sub_ddmmss (bh, bl, bh, bl, ah, al);
      if (bh < 2)
	return 0;

      u10 = 1;
      u01 = 0;
    }

  if (ah < bh)
    goto subtract_a;

  for (;;)
    {
      ASSERT (ah >= bh);
      if (ah == bh)
	goto done;

      if (ah < (CNST_LIMB(1) << (GMP_LIMB_BITS / 2)))
	{
	  ah = shld(ah, al, GMP_LIMB_BITS / 2);
	  bh = shld(bh, bl, GMP_LIMB_BITS / 2);

	  break;
	}

      /* Subtract a -= q b, and multiply M from the right by (1 q ; 0
	 1), affecting the second column of M. */
      {
        mp_limb_t r[2];
        mp_limb_t q = div2 (r, ah, al, bh, bl);
        al = r[0]; ah = r[1];
        q -= (ah < 2) ? 1 : 0;
        u01 += q * u00;
        u11 += q * u10;
        if (ah < 2)
          {
            goto done;
          }
      }
    subtract_a:
      ASSERT (bh >= ah);
      if (ah == bh)
	goto done;

      if (bh < (CNST_LIMB(1) << (GMP_LIMB_BITS / 2)))
	{
	  ah = shld(ah, al, GMP_LIMB_BITS / 2);
	  bh = shld(bh, bl, GMP_LIMB_BITS / 2);

	  goto subtract_a1;
	}

      /* Subtract b -= q a, and multiply M from the right by (1 0 ; q
	 1), affecting the first column of M. */
      {
        mp_limb_t r[2];
        mp_limb_t q = div2 (r, bh, bl, ah, al);
        bl = r[0]; bh = r[1];
        q -= (bh < 2) ? 1 : 0;
        u00 += q * u01;
        u10 += q * u11;
        if (bh < 2)
          {
            goto done;
          }
      }
    }

  /* NOTE: Since we discard the least significant half limb, we don't
     get a truly maximal M (corresponding to |a - b| <
     2^{GMP_LIMB_BITS +1}). */
  /* Single precision loop */
  for (;;)
    {
      ASSERT (ah >= bh);

      {
        mp_limb_t q = div1 (&ah, ah, bh);
        if (ah < (CNST_LIMB(1) << (GMP_LIMB_BITS / 2 + 1)))
          {
            q--;
            u01 += q * u00;
            u11 += q * u10;
            break;
          }
        u01 += q * u00;
        u11 += q * u10;
      }
    subtract_a1:
      ASSERT (bh >= ah);

      {
        mp_limb_t q = div1 (&bh, bh, ah);
        if (bh < (CNST_LIMB(1) << (GMP_LIMB_BITS / 2 + 1)))
          {
            q--;
            u00 += q * u01;
            u10 += q * u11;
            break;
          }
        u00 += q * u01;
        u10 += q * u11;
      }
    }

 done:
  M->u[0][0] = u00; M->u[0][1] = u01;
  M->u[1][0] = u10; M->u[1][1] = u11;

  return 1;
}
