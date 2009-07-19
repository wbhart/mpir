/* mpn_gcdext -- Extended Greatest Common Divisor.

Copyright 1996, 1998, 2000, 2001, 2002 Free Software Foundation, Inc.
Copyright 2004, 2005 Niels Möller
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
along with the GNU MP Library; see the file COPYING.LIB.  If not, write
to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA. */

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"

#ifndef GCDEXT_THRESHOLD
#define GCDEXT_THRESHOLD 17
#endif

#ifndef EXTEND
#define EXTEND 1
#endif

#if STAT
int arr[GMP_LIMB_BITS + 1];
#endif


/* mpn_basic_gcdext (GP, SP, SSIZE, UP, USIZE, VP, VSIZE)

   Compute the extended GCD of {UP,USIZE} and {VP,VSIZE} and store the
   greatest common divisor at GP (unless it is 0), and the first cofactor at
   SP.  Write the size of the cofactor through the pointer SSIZE.  Return the
   size of the value at GP.  Note that SP might be a negative number; this is
   denoted by storing the negative of the size through SSIZE.

   {UP,USIZE} and {VP,VSIZE} are both clobbered.

   The space allocation for all four areas needs to be USIZE+1.

   Preconditions: 1) U >= V.
		  2) V > 0.  */

/* We use Lehmer's algorithm.  The idea is to extract the most significant
   bits of the operands, and compute the continued fraction for them.  We then
   apply the gathered cofactors to the full operands.

   Idea 1: After we have performed a full division, don't shift operands back,
	   but instead account for the extra factors-of-2 thus introduced.
   Idea 2: Simple generalization to use divide-and-conquer would give us an
	   algorithm that runs faster than O(n^2).
   Idea 3: The input numbers need less space as the computation progresses,
	   while the s0 and s1 variables need more space.  To save memory, we
	   could make them share space, and have the latter variables grow
	   into the former.
   Idea 4: We should not do double-limb arithmetic from the start.  Instead,
	   do things in single-limb arithmetic until the quotients differ,
	   and then switch to double-limb arithmetic.  */


/* One-limb division optimized for small quotients.  */
static mp_limb_t
div1 (mp_limb_t n0, mp_limb_t d0)
{
  if ((mp_limb_signed_t) n0 < 0)
    {
      mp_limb_t q;
      int cnt;
      for (cnt = 1; (mp_limb_signed_t) d0 >= 0; cnt++)
	{
	  d0 = d0 << 1;
	}

      q = 0;
      while (cnt)
	{
	  q <<= 1;
	  if (n0 >= d0)
	    {
	      n0 = n0 - d0;
	      q |= 1;
	    }
	  d0 = d0 >> 1;
	  cnt--;
	}

      return q;
    }
  else
    {
      mp_limb_t q;
      int cnt;
      for (cnt = 0; n0 >= d0; cnt++)
	{
	  d0 = d0 << 1;
	}

      q = 0;
      while (cnt)
	{
	  d0 = d0 >> 1;
	  q <<= 1;
	  if (n0 >= d0)
	    {
	      n0 = n0 - d0;
	      q |= 1;
	    }
	  cnt--;
	}

      return q;
    }
}

/* Two-limb division optimized for small quotients.  */
static mp_limb_t
div2 (mp_limb_t n1, mp_limb_t n0, mp_limb_t d1, mp_limb_t d0)
{
  if ((mp_limb_signed_t) n1 < 0)
    {
      mp_limb_t q;
      int cnt;
      for (cnt = 1; (mp_limb_signed_t) d1 >= 0; cnt++)
	{
	  d1 = (d1 << 1) | (d0 >> (GMP_LIMB_BITS - 1));
	  d0 = d0 << 1;
	}

      q = 0;
      while (cnt)
	{
	  q <<= 1;
	  if (n1 > d1 || (n1 == d1 && n0 >= d0))
	    {
	      sub_ddmmss (n1, n0, n1, n0, d1, d0);
	      q |= 1;
	    }
	  d0 = (d1 << (GMP_LIMB_BITS - 1)) | (d0 >> 1);
	  d1 = d1 >> 1;
	  cnt--;
	}

      return q;
    }
  else
    {
      mp_limb_t q;
      int cnt;
      for (cnt = 0; n1 > d1 || (n1 == d1 && n0 >= d0); cnt++)
	{
	  d1 = (d1 << 1) | (d0 >> (GMP_LIMB_BITS - 1));
	  d0 = d0 << 1;
	}

      q = 0;
      while (cnt)
	{
	  d0 = (d1 << (GMP_LIMB_BITS - 1)) | (d0 >> 1);
	  d1 = d1 >> 1;
	  q <<= 1;
	  if (n1 > d1 || (n1 == d1 && n0 >= d0))
	    {
	      sub_ddmmss (n1, n0, n1, n0, d1, d0);
	      q |= 1;
	    }
	  cnt--;
	}

      return q;
    }
}

mp_size_t
mpn_basic_gcdext (mp_ptr gp, mp_ptr s0p, mp_size_t *s0size,
	    mp_ptr up, mp_size_t size, mp_ptr vp, mp_size_t vsize)
{
  mp_limb_t A, B, C, D;
  int cnt;
  mp_ptr tp, wp;
#if RECORD
  mp_limb_t max = 0;
#endif
#if EXTEND
  mp_ptr s1p;
  mp_ptr orig_s0p = s0p;
  mp_size_t ssize;
  int sign = 1;
#endif
  int use_double_flag;
  TMP_DECL;

  ASSERT (size >= vsize);
  ASSERT (vsize >= 1);
  ASSERT (up[size-1] != 0);
  ASSERT (vp[vsize-1] != 0);
  ASSERT (! MPN_OVERLAP_P (up, size+1, vp, vsize+1));
#if EXTEND
  ASSERT (! MPN_OVERLAP_P (s0p, size, up, size+1));
  ASSERT (! MPN_OVERLAP_P (s0p, size, vp, vsize+1));
#endif
  ASSERT (MPN_SAME_OR_SEPARATE_P (gp, up, size));
  ASSERT (MPN_SAME_OR_SEPARATE2_P (gp, size, vp, vsize));

  TMP_MARK;

  tp = (mp_ptr) TMP_ALLOC ((size + 1) * BYTES_PER_MP_LIMB);
  wp = (mp_ptr) TMP_ALLOC ((size + 1) * BYTES_PER_MP_LIMB);
#if EXTEND
  s1p = (mp_ptr) TMP_ALLOC ((size + 1) * BYTES_PER_MP_LIMB);

#if ! WANT_GCDEXT_ONE_STEP
  MPN_ZERO (s0p, size);
  MPN_ZERO (s1p, size);
#endif

  s0p[0] = 1;
  s1p[0] = 0;
  ssize = 1;
#endif

  if (size > vsize)
    {
      mpn_tdiv_qr (tp, up, (mp_size_t) 0, up, size, vp, vsize);

#if EXTEND
      /* This is really what it boils down to in this case... */
      s0p[0] = 0;
      s1p[0] = 1;
      sign = -sign;
#endif
      size = vsize;
      MP_PTR_SWAP (up, vp);
    }

  use_double_flag = ABOVE_THRESHOLD (size, GCDEXT_THRESHOLD);

  for (;;)
    {
      mp_limb_t asign;
      /* Figure out exact size of V.  */
      vsize = size;
      MPN_NORMALIZE (vp, vsize);
      if (vsize <= 1)
	break;

      if (use_double_flag)
	{
	  mp_limb_t uh, vh, ul, vl;
	  /* Let UH,UL be the most significant limbs of U, and let VH,VL be
	     the corresponding bits from V.  */
	  uh = up[size - 1];
	  vh = vp[size - 1];
	  ul = up[size - 2];
	  vl = vp[size - 2];
	  count_leading_zeros (cnt, uh);
#if GMP_NAIL_BITS == 0
	  if (cnt != 0)
	    {
	      uh = (uh << cnt) | (ul >> (GMP_LIMB_BITS - cnt));
	      vh = (vh << cnt) | (vl >> (GMP_LIMB_BITS - cnt));
	      vl <<= cnt;
	      ul <<= cnt;
	      if (size >= 3)
		{
		  ul |= (up[size - 3] >> (GMP_LIMB_BITS - cnt));
		  vl |= (vp[size - 3] >> (GMP_LIMB_BITS - cnt));
		}
	    }
#else
	  uh = uh << cnt;
	  vh = vh << cnt;
	  if (cnt < GMP_NUMB_BITS)
	    {			     /* GMP_NAIL_BITS <= cnt < GMP_NUMB_BITS */
	      uh |= ul >> (GMP_NUMB_BITS - cnt);
	      vh |= vl >> (GMP_NUMB_BITS - cnt);
	      ul <<= cnt + GMP_NAIL_BITS;
	      vl <<= cnt + GMP_NAIL_BITS;
	      if (size >= 3)
		{
		  if (cnt + GMP_NAIL_BITS > GMP_NUMB_BITS)
		    {
		      ul |= up[size - 3] << cnt + GMP_NAIL_BITS - GMP_NUMB_BITS;
		      vl |= vp[size - 3] << cnt + GMP_NAIL_BITS - GMP_NUMB_BITS;
		      if (size >= 4)
			{
			  ul |= up[size - 4] >> 2 * GMP_NUMB_BITS - GMP_NAIL_BITS - cnt;
			  vl |= vp[size - 4] >> 2 * GMP_NUMB_BITS - GMP_NAIL_BITS - cnt;
			}
		    }
		  else
		    {
		      ul |= up[size - 3] >> (GMP_LIMB_BITS - cnt - 2 * GMP_NAIL_BITS);
		      vl |= vp[size - 3] >> (GMP_LIMB_BITS - cnt - 2 * GMP_NAIL_BITS);
		    }
		}
	    }
	  else
	    {			  /* GMP_NUMB_BITS <= cnt <= GMP_LIMB_BITS-1 */
	      uh |= ul << cnt - GMP_NUMB_BITS;	/* 0 <= c <= GMP_NAIL_BITS-1 */
	      vh |= vl << cnt - GMP_NUMB_BITS;
	      if (size >= 3)
		{
		  if (cnt - GMP_NUMB_BITS != 0)
		    {				/* uh/vh need yet more bits! */
		      uh |= up[size - 3] >> 2 * GMP_NUMB_BITS - cnt;
		      vh |= vp[size - 3] >> 2 * GMP_NUMB_BITS - cnt;
		      ul = up[size - 3] << cnt + GMP_NAIL_BITS - GMP_NUMB_BITS;
		      vl = vp[size - 3] << cnt + GMP_NAIL_BITS - GMP_NUMB_BITS;
		      if (size >= 4)
			{
			  ul |= up[size - 4] >> 2 * GMP_NUMB_BITS - GMP_NAIL_BITS - cnt;
			  vl |= vp[size - 4] >> 2 * GMP_NUMB_BITS - GMP_NAIL_BITS - cnt;
			}
		    }
		  else
		    {
		      ul = up[size - 3] << GMP_LIMB_BITS - cnt;
		      vl = vp[size - 3] << GMP_LIMB_BITS - cnt;
		      if (size >= 4)
			{
			  ul |= up[size - 4] >> GMP_NUMB_BITS - (GMP_LIMB_BITS - cnt);
			  vl |= vp[size - 4] >> GMP_NUMB_BITS - (GMP_LIMB_BITS - cnt);
			}
		    }
		}
	      else
		{
		  ul = 0;
		  vl = 0;
		}
	    }
#endif

	  A = 1;
	  B = 0;
	  C = 0;
	  D = 1;

	  asign = 0;
	  for (;;)
	    {
	      mp_limb_t Tac, Tbd;
	      mp_limb_t q1, q2;
	      mp_limb_t nh, nl, dh, dl;
	      mp_limb_t t1, t0;
	      mp_limb_t Th, Tl;

	      sub_ddmmss (dh, dl, vh, vl, 0, C);
	      if (dh == 0)
		break;
	      add_ssaaaa (nh, nl, uh, ul, 0, A);
	      q1 = div2 (nh, nl, dh, dl);

	      add_ssaaaa (dh, dl, vh, vl, 0, D);
	      if (dh == 0)
		break;
	      sub_ddmmss (nh, nl, uh, ul, 0, B);
	      q2 = div2 (nh, nl, dh, dl);

	      if (q1 != q2)
		break;

	      Tac = A + q1 * C;
	      if (GMP_NAIL_BITS != 0 && Tac > GMP_NUMB_MAX)
		break;
	      Tbd = B + q1 * D;
	      if (GMP_NAIL_BITS != 0 && Tbd > GMP_NUMB_MAX)
		break;
	      A = C;
	      C = Tac;
	      B = D;
	      D = Tbd;
	      umul_ppmm (t1, t0, q1, vl);
	      t1 += q1 * vh;
	      sub_ddmmss (Th, Tl, uh, ul, t1, t0);
	      uh = vh, ul = vl;
	      vh = Th, vl = Tl;

	      asign = ~asign;

	      add_ssaaaa (dh, dl, vh, vl, 0, C);
/*	      if (dh == 0)	should never happen
		break;	       */
	      sub_ddmmss (nh, nl, uh, ul, 0, A);
	      q1 = div2 (nh, nl, dh, dl);

	      sub_ddmmss (dh, dl, vh, vl, 0, D);
	      if (dh == 0)
		break;
	      add_ssaaaa (nh, nl, uh, ul, 0, B);
	      q2 = div2 (nh, nl, dh, dl);

	      if (q1 != q2)
		break;

	      Tac = A + q1 * C;
	      if (GMP_NAIL_BITS != 0 && Tac > GMP_NUMB_MAX)
		break;
	      Tbd = B + q1 * D;
	      if (GMP_NAIL_BITS != 0 && Tbd > GMP_NUMB_MAX)
		break;
	      A = C;
	      C = Tac;
	      B = D;
	      D = Tbd;
	      umul_ppmm (t1, t0, q1, vl);
	      t1 += q1 * vh;
	      sub_ddmmss (Th, Tl, uh, ul, t1, t0);
	      uh = vh, ul = vl;
	      vh = Th, vl = Tl;

	      asign = ~asign;
	    }
#if EXTEND
	  if (asign)
	    sign = -sign;
#endif
	}
      else /* Same, but using single-limb calculations.  */
	{
	  mp_limb_t uh, vh;
	  /* Make UH be the most significant limb of U, and make VH be
	     corresponding bits from V.  */
	  uh = up[size - 1];
	  vh = vp[size - 1];
	  count_leading_zeros (cnt, uh);
#if GMP_NAIL_BITS == 0
	  if (cnt != 0)
	    {
	      uh = (uh << cnt) | (up[size - 2] >> (GMP_LIMB_BITS - cnt));
	      vh = (vh << cnt) | (vp[size - 2] >> (GMP_LIMB_BITS - cnt));
	    }
#else
	  uh <<= cnt;
	  vh <<= cnt;
	  if (cnt < GMP_NUMB_BITS)
	    {
	      uh |= up[size - 2] >> (GMP_NUMB_BITS - cnt);
	      vh |= vp[size - 2] >> (GMP_NUMB_BITS - cnt);
	    }
	  else
	    {
	      uh |= up[size - 2] << cnt - GMP_NUMB_BITS;
	      vh |= vp[size - 2] << cnt - GMP_NUMB_BITS;
	      if (size >= 3)
		{
		  uh |= up[size - 3] >> 2 * GMP_NUMB_BITS - cnt;
		  vh |= vp[size - 3] >> 2 * GMP_NUMB_BITS - cnt;
		}
	    }
#endif

	  A = 1;
	  B = 0;
	  C = 0;
	  D = 1;

	  asign = 0;
	  for (;;)
	    {
	      mp_limb_t q, T;
	      if (vh - C == 0 || vh + D == 0)
		break;

	      q = (uh + A) / (vh - C);
	      if (q != (uh - B) / (vh + D))
		break;

	      T = A + q * C;
	      A = C;
	      C = T;
	      T = B + q * D;
	      B = D;
	      D = T;
	      T = uh - q * vh;
	      uh = vh;
	      vh = T;

	      asign = ~asign;

	      if (vh - D == 0)
		break;

	      q = (uh - A) / (vh + C);
	      if (q != (uh + B) / (vh - D))
		break;

	      T = A + q * C;
	      A = C;
	      C = T;
	      T = B + q * D;
	      B = D;
	      D = T;
	      T = uh - q * vh;
	      uh = vh;
	      vh = T;

	      asign = ~asign;
	    }
#if EXTEND
	  if (asign)
	    sign = -sign;
#endif
	}

#if RECORD
      max = MAX (A, max);  max = MAX (B, max);
      max = MAX (C, max);  max = MAX (D, max);
#endif

      if (B == 0)
	{
	  /* This is quite rare.  I.e., optimize something else!  */

	  mpn_tdiv_qr (wp, up, (mp_size_t) 0, up, size, vp, vsize);

#if EXTEND
	  MPN_COPY (tp, s0p, ssize);
	  {
	    mp_size_t qsize;
	    mp_size_t i;

	    qsize = size - vsize + 1; /* size of stored quotient from division */
	    MPN_ZERO (s1p + ssize, qsize); /* zero s1 too */

	    for (i = 0; i < qsize; i++)
	      {
		mp_limb_t cy;
		cy = mpn_addmul_1 (tp + i, s1p, ssize, wp[i]);
		tp[ssize + i] = cy;
	      }

	    ssize += qsize;
	    ssize -= tp[ssize - 1] == 0;
	  }

	  sign = -sign;
	  MP_PTR_SWAP (s0p, s1p);
	  MP_PTR_SWAP (s1p, tp);
#endif
	  size = vsize;
	  MP_PTR_SWAP (up, vp);
	}
      else
	{
#if EXTEND
	  mp_size_t tsize, wsize;
#endif
	  /* T = U*A + V*B
	     W = U*C + V*D
	     U = T
	     V = W	   */

#if STAT
	  { mp_limb_t x; x = A | B | C | D; count_leading_zeros (cnt, x);
	  arr[GMP_LIMB_BITS - cnt]++; }
#endif
	  if (A == 0)
	    {
	      /* B == 1 and C == 1 (D is arbitrary) */
	      mp_limb_t cy;
	      MPN_COPY (tp, vp, size);
	      MPN_COPY (wp, up, size);
	      mpn_submul_1 (wp, vp, size, D);
	      MP_PTR_SWAP (tp, up);
	      MP_PTR_SWAP (wp, vp);
#if EXTEND
	      MPN_COPY (tp, s1p, ssize);
	      tsize = ssize;
	      tp[ssize] = 0;	/* must zero since wp might spill below */
	      MPN_COPY (wp, s0p, ssize);
	      cy = mpn_addmul_1 (wp, s1p, ssize, D);
	      wp[ssize] = cy;
	      wsize = ssize + (cy != 0);
	      MP_PTR_SWAP (tp, s0p);
	      MP_PTR_SWAP (wp, s1p);
	      ssize = MAX (wsize, tsize);
#endif
	    }
	  else
	    {
	      mp_limb_t cy, cy1, cy2;

	      if (asign)
		{
		  mpn_mul_1 (tp, vp, size, B);
		  mpn_submul_1 (tp, up, size, A);
		  mpn_mul_1 (wp, up, size, C);
		  mpn_submul_1 (wp, vp, size, D);
		}
	      else
		{
		  mpn_mul_1 (tp, up, size, A);
		  mpn_submul_1 (tp, vp, size, B);
		  mpn_mul_1 (wp, vp, size, D);
		  mpn_submul_1 (wp, up, size, C);
		}
	      MP_PTR_SWAP (tp, up);
	      MP_PTR_SWAP (wp, vp);
#if EXTEND
	      /* Compute new s0 */
	      cy1 = mpn_mul_1 (tp, s0p, ssize, A);
	      cy2 = mpn_addmul_1 (tp, s1p, ssize, B);
	      cy = cy1 + cy2;
	      tp[ssize] = cy & GMP_NUMB_MASK;
	      tsize = ssize + (cy != 0);
#if GMP_NAIL_BITS == 0
	      if (cy < cy1)
#else
	      if (cy > GMP_NUMB_MAX)
#endif
		{
		  tp[tsize] = 1;
		  wp[tsize] = 0;
		  tsize++;
		  /* This happens just for nails, since we get more work done
		     per numb there.  */
		}

	      /* Compute new s1 */
	      cy1 = mpn_mul_1 (wp, s1p, ssize, D);
	      cy2 = mpn_addmul_1 (wp, s0p, ssize, C);
	      cy = cy1 + cy2;
	      wp[ssize] = cy & GMP_NUMB_MASK;
	      wsize = ssize + (cy != 0);
#if GMP_NAIL_BITS == 0
	      if (cy < cy1)
#else
	      if (cy > GMP_NUMB_MAX)
#endif
		{
		  wp[wsize] = 1;
		  if (wsize >= tsize)
		    tp[wsize] = 0;
		  wsize++;
		}

	      MP_PTR_SWAP (tp, s0p);
	      MP_PTR_SWAP (wp, s1p);
	      ssize = MAX (wsize, tsize);
#endif
	    }
	  size -= up[size - 1] == 0;
#if GMP_NAIL_BITS != 0
	  size -= up[size - 1] == 0;
#endif
	}

#if WANT_GCDEXT_ONE_STEP
      TMP_FREE;
      return 0;
#endif
    }

#if RECORD
  printf ("max: %lx\n", max);
#endif

#if STAT
 {int i; for (i = 0; i <= GMP_LIMB_BITS; i++) printf ("%d:%d\n", i, arr[i]);}
#endif

  if (vsize == 0)
    {
      if (gp != up && gp != 0)
	MPN_COPY (gp, up, size);
#if EXTEND
      MPN_NORMALIZE (s0p, ssize);
      if (orig_s0p != s0p)
	MPN_COPY (orig_s0p, s0p, ssize);
      *s0size = sign >= 0 ? ssize : -ssize;
#endif
      TMP_FREE;
      return size;
    }
  else
    {
      mp_limb_t vl, ul, t;
#if EXTEND
      mp_size_t qsize, i;
#endif
      vl = vp[0];
#if EXTEND
      t = mpn_divmod_1 (wp, up, size, vl);

      MPN_COPY (tp, s0p, ssize);

      qsize = size - (wp[size - 1] == 0); /* size of quotient from division */
      if (ssize < qsize)
	{
	  MPN_ZERO (tp + ssize, qsize - ssize);
	  MPN_ZERO (s1p + ssize, qsize); /* zero s1 too */
	  for (i = 0; i < ssize; i++)
	    {
	      mp_limb_t cy;
	      cy = mpn_addmul_1 (tp + i, wp, qsize, s1p[i]);
	      tp[qsize + i] = cy;
	    }
	}
      else
	{
	  MPN_ZERO (s1p + ssize, qsize); /* zero s1 too */
	  for (i = 0; i < qsize; i++)
	    {
	      mp_limb_t cy;
	      cy = mpn_addmul_1 (tp + i, s1p, ssize, wp[i]);
	      tp[ssize + i] = cy;
	    }
	}
      ssize += qsize;
      ssize -= tp[ssize - 1] == 0;

      sign = -sign;
      MP_PTR_SWAP (s0p, s1p);
      MP_PTR_SWAP (s1p, tp);
#else
      t = mpn_mod_1 (up, size, vl);
#endif
      ul = vl;
      vl = t;
      while (vl != 0)
	{
	  mp_limb_t t;
#if EXTEND
	  mp_limb_t q;
	  q = ul / vl;
	  t = ul - q * vl;

	  MPN_COPY (tp, s0p, ssize);

	  MPN_ZERO (s1p + ssize, 1); /* zero s1 too */

	  {
	    mp_limb_t cy;
	    cy = mpn_addmul_1 (tp, s1p, ssize, q);
	    tp[ssize] = cy;
	  }

	  ssize += 1;
	  ssize -= tp[ssize - 1] == 0;

	  sign = -sign;
	  MP_PTR_SWAP (s0p, s1p);
	  MP_PTR_SWAP (s1p, tp);
#else
	  t = ul % vl;
#endif
	  ul = vl;
	  vl = t;
	}
      if (gp != 0)
	gp[0] = ul;
#if EXTEND
      MPN_NORMALIZE (s0p, ssize);
      if (orig_s0p != s0p)
	MPN_COPY (orig_s0p, s0p, ssize);
      *s0size = sign >= 0 ? ssize : -ssize;
#endif
      TMP_FREE;
      return 1;
    }
}

/*---------------------------------------------------------

    Asymptotically fast xgcd based on Niels Mohler's ngcd

----------------------------------------------------------*/

/* Needs temporary storage for the division and multiplication
   The division has quotient + remainder of size an - bn + 1 + bn
	= an + 1
	quotient is no bigger than an
	Thus we need space at most 2*n + un + 1

	If the gcd is found, stores it in gp and *gn, and the associated 
   cofactor in {sp, *un} and returns zero.
   Otherwise, compute the reduced a and b, update u0p and u1p,
	and return the new size. /*
 
 *
 * To make this code work with "make tune" we need to conditionally
 * exclude the Moller code when this file gets included inside of
 * gcdext*.c in ../tune.
 */
#ifndef INSIDE_TUNE_GCDEXT_BIN

mp_size_t
mpn_ngcdext_subdiv_step (mp_ptr gp, mp_size_t *gn, mp_ptr s0p, mp_ptr u0, mp_ptr u1, 
		             mp_size_t *un, mp_ptr ap, mp_ptr bp, mp_size_t n, mp_ptr tp)
{
  /* Called when nhgcd or mpn_nhgcd2 has failed. Then either one of a or b
     is very small, or the difference is very small. Perform one
     subtraction followed by one division. */

  mp_size_t an, bn, cy, qn, qn2, u0n, u1n;
  int negate = 0;

  ASSERT (n > 0);
  ASSERT (ap[n-1] > 0 || bp[n-1] > 0);

  /* First, make sure that an >= bn, and subtract an -= bn */
  for (an = n; an > 0; an--)
    if (ap[an-1] != bp[an-1])
      break;

  if (an == 0)
    {
      /* ap is the gcd */
      MPN_COPY (gp, ap, n);
		MPN_NORMALIZE(u1, (*un));
      MPN_COPY (s0p, u1, (*un));
      (*gn) = n;
      return 0;
    }

  if (ap[an-1] < bp[an-1]) /* swap so that ap >= bp */
  {
	 MP_PTR_SWAP (ap, bp);
    MP_PTR_SWAP (u0, u1);
	 negate = ~negate;
  }

  bn = n;
  MPN_NORMALIZE (bp, bn);
  if (bn == 0)
    {
      /* ap is the gcd */
		MPN_COPY (gp, ap, n);
      MPN_NORMALIZE(u1, (*un));
      MPN_COPY (s0p, u1, (*un));
		if (negate) (*un) = -(*un);
      (*gn) = n;
      return 0;
    }

  ASSERT_NOCARRY (mpn_sub_n (ap, ap, bp, an)); /* ap -= bp, u1 += u0 */
  MPN_NORMALIZE (ap, an);
  
  ASSERT (an > 0);
	
  cy = mpn_add_n(u1, u1, u0, *un);
  if (cy) u1[(*un)++] = cy;

  if (an < bn) /* make an >= bn */
  {
	  MPN_PTR_SWAP (ap, an, bp, bn);
	  MP_PTR_SWAP(u0, u1);
	  negate = ~negate;
  }
  else if (an == bn)
    {
      int c;
      MPN_CMP (c, ap, bp, an);
      if (c < 0)
		{
			MP_PTR_SWAP (ap, bp);
		   MP_PTR_SWAP(u0, u1);
	      negate = ~negate;
      } else if (c == 0) /* gcd is ap */
		{
		   MPN_COPY (gp, ap, an);
         MPN_NORMALIZE(u1, (*un));
         MPN_COPY (s0p, u1, (*un));
         if (negate) (*un) = -(*un);
         (*gn) = an;
         return 0;
		}
    }

  ASSERT (an >= bn);

  qn = an - bn + 1;
  mpn_tdiv_qr (tp, tp + qn, 0, ap, an, bp, bn); /* ap -= q * bp, u1 += q * u0 */

  /* Normalizing seems to be the simplest way to test if the remainder
     is zero. */
  an = bn;
  MPN_NORMALIZE (tp + qn, an);
  if (an == 0)
    {
      /* gcd = bp */
		MPN_COPY (gp, bp, bn);
      MPN_NORMALIZE(u0, (*un));
      MPN_COPY (s0p, u0, (*un));
      if (!negate) (*un) = -(*un);
      (*gn) = bn;
      return 0;
    }

  MPN_COPY (ap, tp + qn, bn); /* must copy all bn limbs here, else the top limbs won't be zero */

  qn2 = qn;
  u0n = (*un);
  MPN_NORMALIZE (tp, qn2);
  MPN_NORMALIZE (u0, u0n);

  if (u0n > 0)
  {

  if (qn2 > u0n) mpn_mul(tp + qn, tp, qn2, u0, u0n);
  else mpn_mul(tp + qn, u0, u0n, tp, qn2);

  u0n += qn2;
  MPN_NORMALIZE(tp + qn, u0n);

  if ((*un) >= u0n) 
  {
	  cy = mpn_add(u1, u1, (*un), tp + qn, u0n);
	  if (cy) u1[(*un)++] = cy;
  } else
  {
	  cy = mpn_add(u1, tp + qn, u0n, u1, (*un));
	  (*un) = u0n;
	  if (cy) u1[(*un)++] = cy;
  }
  }

  return bn;
}

/*
   Set (u0, u1) = (u0, u1) M 
	Requires temporary space un + un  + M->n = 2*un + M->n
*/
void ngcdext_cofactor_adjust(mp_ptr u0, mp_ptr u1, mp_size_t * un, struct ngcd_matrix *M, mp_ptr tp)
{
	/* Let M = (r00, r01)
	           (r10, r11)
	   We want u0 = u0 * r00 + u1 * r10
		        u1 = u0 * r01 + u1 * r11
	   We make a copy of u0 at tp and update u0 first
	*/

   mp_limb_t cy, cy2;
	mp_ptr t2p =(tp + (*un)); /* second temporary space */
	ASSERT(tp > M->p[1][1] + M->n);


	MPN_COPY(tp, u0, *un);

	if (M->n >= (*un))
	{
		mpn_mul(t2p, M->p[1][0], M->n, u1, *un); /* t2p = r10 * u1 */
		mpn_mul(u0, M->p[0][0], M->n, tp, *un);  /* u0 = r00 * u0 */
	} else
	{
		mpn_mul(t2p, u1, *un, M->p[1][0], M->n); 
		mpn_mul(u0, tp, *un, M->p[0][0], M->n); 
	}
	
	cy = mpn_add_n(u0, u0, t2p, M->n + (*un)); /* u0 += t2p */
	
	
	if (M->n >= (*un))
	{
		mpn_mul(t2p, M->p[1][1], M->n, u1, *un); /* t2p = r11 * u1 */
		mpn_mul(u1, M->p[0][1], M->n, tp, *un);  /* u1 = r01 * u0 */
	} else
	{
		mpn_mul(t2p, u1, *un, M->p[1][1], M->n); 
		mpn_mul(u1, tp, *un, M->p[0][1], M->n); 
	}
	
	cy2 = mpn_add_n(u1, u1, t2p, M->n + (*un)); /* u1 += t2p */

	if ((cy) || (cy2)) /* normalise u0, u1 */
	{
		u0[M->n + (*un)] = cy;
		u1[M->n + (*un)] = cy2;
		(*un) += (M->n + 1);
	} else 
	{
      (*un) += M->n;
		while ((u0[*un - 1] == 0) && (u1[*un - 1] == 0)) (*un)--; /* both cannot be zero, so this won't overrun */
	}
}

/*
   Computes |t| where t = (gp - s*ap)/bp 
	Requires temporary space sn + an + n
*/

void gcdext_get_t(mp_ptr t, mp_size_t * tn, mp_ptr gp, mp_size_t gn, mp_ptr ap, 
					mp_size_t an, mp_ptr bp, mp_size_t n, mp_ptr s, mp_size_t sn, mp_ptr tp)
{
	mp_size_t ss = ABS(sn);
	mp_limb_t cy;
	
	if (ss >= an)
		mpn_mul(tp, s, ss, ap, an);
	else
      mpn_mul(tp, ap, an, s, ss);

	(*tn) = ss + an;
	(*tn) -= (tp[(*tn) - 1] == 0);

	/* We must have s*ap >= gp and we really want to compute -t */

	if (sn > 0)
	{
		mpn_sub(tp, tp, *tn, gp, gn);
	   MPN_NORMALIZE(tp, (*tn));
	} else 
	{
		cy = mpn_add(tp, tp, *tn, gp, gn);
		if (cy) tp[(*tn)++] = cy;
	}

	if ((*tn) == 0) 
	{
		return;
	}

	mpn_tdiv_qr(t, tp + (*tn), 0, tp, (*tn), bp, n);

	ASSERT_MPN_ZERO_P(tp + (*tn), n);

   (*tn) -= (n - 1);
	(*tn) -= (t[(*tn) - 1] == 0);
}

mp_limb_t mpn_gcdinv_1(mp_limb_t * a, mp_limb_signed_t x, mp_limb_signed_t y)
{
   mp_limb_signed_t u1 = CNST_LIMB(1); 
   mp_limb_signed_t u2 = CNST_LIMB(0); 
   mp_limb_signed_t t1; 
   mp_limb_t u3, v3;
   mp_limb_t quot, rem;
   mp_limb_signed_t xsign = 0;
   
   u3 = x, v3 = y;

   while (v3) {
      quot=u3-v3;
      if (u3 < (v3<<2))
      {
         if (quot < v3)
         {
            t1 = u2; u2 = u1 - u2; u1 = t1; u3 = v3;
            v3 = quot;
         } else if (quot < (v3<<1))
         {  
            t1 = u2; u2 = u1 - (u2<<1); u1 = t1; u3 = v3;
            v3 = quot-u3;
         } else
         {
            t1 = u2; u2 = u1 - 3*u2; u1 = t1; u3 = v3;
            v3 = quot-(u3<<1);
         }
      } else
      {
         quot=u3/v3;
         rem = u3 - v3*quot;
         t1 = u2; u2 = u1 - quot*u2; u1 = t1; u3 = v3;
         v3 = rem;
      }
   }
   
   if (u1 < (mp_limb_signed_t) 0) u1 += y;
   *a = u1;
   
   return u3;
}


#define P_SIZE(n) (n/2)
#define NGCDEXT_THRESHOLD 400

mp_size_t
mpn_gcdext (mp_ptr gp, mp_ptr s0p, mp_size_t *s0size,
	    mp_ptr ap, mp_size_t an, mp_ptr bp, mp_size_t n)
{
  mp_size_t init_scratch, orig_n = n;
  mp_size_t scratch, un, u0n, u1n;
  mp_limb_t t;
  mp_ptr tp, u0, u1;
  int swapped = 0;
    struct ngcd_matrix M;
    mp_size_t p;
    mp_size_t nn;
  TMP_DECL;
  
  ASSERT (an >= n);
  
  if (an == 1)
  {
    if (!n)
    {
       gp[0] = ap[0];
       s0p[0] = 1;
       *s0size = 1;
       return 1;
    }
    gp[0] = mpn_gcdinv_1(s0p, ap[0], bp[0]);
    *s0size = 1;
    return 1;
  }

  if (BELOW_THRESHOLD (n, NGCDEXT_THRESHOLD))
    return mpn_basic_gcdext (gp, s0p, s0size, ap, an, bp, n);
  
  init_scratch = MPN_NGCD_MATRIX_INIT_ITCH ((n+1)/2);
  scratch = mpn_nhgcd_itch ((n+1)/2);

  /* Space needed for mpn_ngcd_matrix_adjust */
  if (scratch < 2*n)
    scratch = 2*n;
  if (scratch < an + n - 1) /* the first division can sometimes be selfish!! */
	 scratch = an + n - 1;

 /* Space needed for cofactor adjust */
  scratch = MAX(scratch, 2*(n+1) + P_SIZE(n) + 1);

  TMP_MARK;
  
  if (5*n + 5 > init_scratch + scratch) 
    tp = TMP_ALLOC_LIMBS (7*n+5); /* 3n+1 for get_t, 2n+2 for s, t, 2n+2 for u0, u1 */
  else
    tp = TMP_ALLOC_LIMBS (2*(n+1) + init_scratch + scratch);

  u0 = tp; /* Cofactor space */
  u1 = tp + n + 1;

  MPN_ZERO(tp, 2*(n+1));

  tp += 2*(n+1);
  
  if (an > n)
    {
      mp_ptr rp = tp;
      mp_ptr qp = rp + n;

      mpn_tdiv_qr (qp, rp, 0, ap, an, bp, n);
      
		MPN_COPY (ap, rp, n);
      an = n;
      MPN_NORMALIZE (ap, an);
      if (an == 0)
	{	  
	  MPN_COPY (gp, bp, n);
	  TMP_FREE;
	  (*s0size) = 0;
	  return n;
	}
    }

    /* First iteration, setup u0 and u1 */

    p = P_SIZE(n);
  
    mpn_ngcd_matrix_init (&M, n - p, tp);
	 ASSERT(tp + init_scratch > M.p[1][1] + M.n);
	 nn = mpn_nhgcd (ap + p, bp + p, n - p, &M, tp + init_scratch);
  if (nn > 0)
	 {
		 n = mpn_ngcd_matrix_adjust (&M, p + nn, ap, bp, p, tp + init_scratch);
		 
		 /* 
            (ap'', bp'')^T = M^-1(ap', bp')^T 
		    and (ap', bp') = (1*ap + ?*bp, 0*ap + ?*bp) 
		    We let u0 be minus the factor of ap appearing 
            in the expression for bp'' and u1 be the 
            factor of ap appearing in the expression for ap''
        */

       MPN_COPY(u0, M.p[1][0], M.n);
	    MPN_COPY(u1, M.p[1][1], M.n);

	    un = M.n;
	    while ((u0[un-1] == 0) && (u1[un-1] == 0)) un--; /* normalise u0, u1, both cannot be zero as det = 1*/
     }
  else	
	 {
	   mp_size_t gn;

		un = 1;
	   u0[0] = 0; /* bp = 0*ap + ?*bp, thus u0 = -0 */
	   u1[0] = 1; /* ap = 1*ap + ?*bp, thus u1 = 1 */
   
	   n = mpn_ngcdext_subdiv_step (gp, &gn, s0p, u0, u1, &un, ap, bp, n, tp);
	 if (n == 0)
	   {
	      (*s0size) = un;
			ASSERT(s0p[*s0size - 1] != 0);
		   TMP_FREE;
	      return gn;
	   }
	 } 

  while (ABOVE_THRESHOLD (n, NGCDEXT_THRESHOLD))
    {
      struct ngcd_matrix M;
      mp_size_t p = P_SIZE(n);
      mp_size_t nn;
      
      mpn_ngcd_matrix_init (&M, n - p, tp);
      nn = mpn_nhgcd (ap + p, bp + p, n - p, &M, tp + init_scratch);
		if (nn > 0)
	{
	   n = mpn_ngcd_matrix_adjust (&M, p + nn, ap, bp, p, tp + init_scratch);

		ngcdext_cofactor_adjust(u0, u1, &un, &M, tp + init_scratch);
		
		/* 
            (ap'', bp'')^T = M^-1(ap', bp')^T 
		    and (ap', bp') = (u1*ap + ?*bp, -u0*ap + ?*bp) 
		    So we need u0' = -(-c*u1 + a*-u0) = a*u0 + c*u1
            and we need u1' = (d*u1 -b*-u0) = b*u0 + d*u1 
        */

     
		ASSERT(un <= orig_n + 1);

	}  else	
	{
	  mp_size_t gn;
	  n = mpn_ngcdext_subdiv_step (gp, &gn, s0p, u0, u1, &un, ap, bp, n, tp);
	  ASSERT(un <= orig_n + 1);
	  if (n == 0)
	    {
	      (*s0size) = un;
			ASSERT(((*s0size) == 0) || (s0p[ABS(*s0size) - 1] != 0));
		   TMP_FREE;
			return gn;
	    }
	}
    }

  ASSERT (ap[n-1] > 0 || bp[n-1] > 0);
  ASSERT (u0[un-1] > 0 || u1[un-1] > 0);

  if (ap[n-1] < bp[n-1])
  {
	  MP_PTR_SWAP (ap, bp);
	  MP_PTR_SWAP (u0, u1);
	  swapped = 1;
  }
   
  an = n; /* {ap, an} and {bp, bn} are normalised, {ap, an} >= {bp, bn} */
  MPN_NORMALIZE (bp, n);

  if (n == 0)
    {
      /* If bp == 0 then gp = ap
		   with cofactor u1
			If we swapped then cofactor is -u1
		*/
		MPN_COPY (gp, ap, an);
		MPN_NORMALIZE(u1, un);
		MPN_COPY(s0p, u1, un);
      (*s0size) = un;
		if (swapped) (*s0size) = -(*s0size);
      TMP_FREE;
      return an;
    }

  /* 
     If at this point we have s*ap' + t*bp' = gp where gp is the gcd
	  and (ap', bp') = (u1*ap + ?*bp, -u0*ap + ?*bp)
	  then gp = s*u1*ap - t*u0*ap + ?*bp
	  and the cofactor we want is (s*u1-t*u0).

	  First there is the special case u0 = 0, u1 = 1 in which case we do not need 
	  to compute t...
  */
    
  ASSERT(u1 + un <= tp);
  u0n = un;
  MPN_NORMALIZE(u0, u0n);  /* {u0, u0n} is now normalised */

  if (u0n == 0) /* u1 = 1 */
  {
	  mp_size_t gn;
	 
	  gn = mpn_basic_gcdext (gp, s0p, s0size, ap, an, bp, n);
	  if (swapped) (*s0size) = -(*s0size);
	  TMP_FREE;
	  return gn;
  }
  else
  {
	  /* Compute final gcd. */
  
	  mp_size_t gn, sn, tn;
	  mp_ptr s, t;
	  mp_limb_t cy;
	  int negate = 0;

	  /* Save an, bn first as gcdext destroys inputs */
		  
	  s = tp;
	  tp += (an + 1);
   
     MPN_COPY(tp, ap, an);
	  MPN_COPY(tp + an + 1, bp, n);

	  gn = mpn_basic_gcdext (gp, s, &sn, tp, an, tp + an + 1, n); /* {s, sn} is normalised */
     
	  /* Special case, s == 0, t == 1, cofactor = -u0 */

	  if (sn == 0)
	  {
		  MPN_COPY(s0p, u0, u0n);
		  (*s0size) = -u0n;
		  if (swapped) (*s0size) = -(*s0size);
		  TMP_FREE;
		  return gn;
	  }

	  /* We'll need the other cofactor t = (gp - s*ap)/bp 
		*/

	  t = tp;
	  tp += (an + 1);
		 
	  gcdext_get_t(t, &tn, gp, gn, ap, an, bp, n, s, sn, tp);

	  ASSERT((tn == 0) || (t[tn - 1] > 0)); /* {t, tn} is normalised */

	  ASSERT(tn > 0);
	  ASSERT(tn <= an + 1);

	  /* We want to compute s*u1 - t*u0, so if s is negative
	     t will be positive, so we'd be dealing with negative
		  numbers. We fix that here.
	  */

	  if (sn < 0)
	  {
		  sn = -sn;
		  negate = 1;
	  }

	  /* Now we can deal with the special case u1 = 0 */

	  u1n = un; 
	  MPN_NORMALIZE(u1, u1n); /* {u1, u1n} is now normalised */
     
	  if (u1n == 0)
	  {
		  MPN_COPY(s0p, t, tn);
		  (*s0size) = -tn;
		  if (swapped ^ negate) (*s0size) = -(*s0size);
		  TMP_FREE;
		  return gn;
	  }

	  /* t may be zero, but we need to compute s*u1 anyway */
	  if (sn >= u1n)
		  mpn_mul(s0p, s, sn, u1, u1n);
	  else
		  mpn_mul(s0p, u1, u1n, s, sn);

	  (*s0size) = sn + u1n;
	  (*s0size) -= (s0p[sn + u1n - 1] == 0);

	  ASSERT(s0p[*s0size - 1] > 0); /* {s0p, *s0size} is normalised now */

	  if (tn == 0)
	  {
		  if (swapped ^ negate) (*s0size) = -(*s0size);
        TMP_FREE;
		  return gn;
	  }

	  /* Now compute the rest of the cofactor, t*u0
	     and subtract it
		  We're done with u1 and s which happen to be
		  consecutive, so use that space
	  */

	  ASSERT(u1 + tn + u0n <= t);

     if (tn > u0n)
		  mpn_mul(u1, t, tn, u0, u0n);
	  else
		  mpn_mul(u1, u0, u0n, t, tn);

	  u1n = tn + u0n;
	  u1n -= (u1[tn + u0n - 1] == 0);

	  ASSERT(u1[u1n - 1] > 0);

	  /* Recall t is now negated so s*u1 - t*u0 
	     involves an *addition* 
	  */

	  if ((*s0size) >= u1n)
	  {
		  cy = mpn_add(s0p, s0p, *s0size, u1, u1n);
		  if (cy) s0p[(*s0size)++] = cy;
	  }
	  else
	  {
		  cy = mpn_add(s0p, u1, u1n, s0p, *s0size);
        (*s0size) = u1n;
	     if (cy) s0p[(*s0size)++] = cy;
	  }

	  if (swapped ^ negate) (*s0size) = -(*s0size);
     TMP_FREE;  
     return gn;
  }
}
#endif
