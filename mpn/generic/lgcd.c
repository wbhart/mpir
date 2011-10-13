/* lgcd.c

   Lehmer gcd, based on mpn_nhgcd2.


Copyright 2004, 2005 Niels Möller
Copyright 2009, 2010 William Hart

This file is part of the MPIR Library.

The MPIR Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation; either version 2.1 of the License, or (at
your option) any later version.

The MPIR Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPIR Library; see the file COPYING.LIB.  If not, write
to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.
*/

#include <stdio.h>
#include <stdlib.h>

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"

/* Extract one limb, shifting count bits left
    ________  ________
   |___xh___||___xl___|
	  |____r____|
   >count <

   The count includes any nail bits, so it should work fine if
   count is computed using count_leading_zeros.
*/

#define MPN_EXTRACT_LIMB(count, xh, xl) \
  (((xh) << (count)) | ((xl) >> (GMP_LIMB_BITS - (count))))

/* Use binary algorithm to compute V <-- GCD (V, U) for usize, vsize == 2.
   Both U and V must be odd.  */
static inline mp_size_t
gcd_2 (mp_ptr vp, mp_srcptr up)
{
  mp_limb_t u0, u1, v0, v1;
  mp_size_t vsize;

  u0 = up[0];
  u1 = up[1];
  v0 = vp[0];
  v1 = vp[1];

  while (u1 != v1 && u0 != v0)
    {
      unsigned long int r;
      if (u1 > v1)
	{
	  u1 -= v1 + (u0 < v0);
	  u0 = (u0 - v0) & GMP_NUMB_MASK;
	  count_trailing_zeros (r, u0);
	  u0 = ((u1 << (GMP_NUMB_BITS - r)) & GMP_NUMB_MASK) | (u0 >> r);
	  u1 >>= r;
	}
      else  /* u1 < v1.  */
	{
	  v1 -= u1 + (v0 < u0);
	  v0 = (v0 - u0) & GMP_NUMB_MASK;
	  count_trailing_zeros (r, v0);
	  v0 = ((v1 << (GMP_NUMB_BITS - r)) & GMP_NUMB_MASK) | (v0 >> r);
	  v1 >>= r;
	}
    }

  vp[0] = v0, vp[1] = v1, vsize = 1 + (v1 != 0);

  /* If U == V == GCD, done.  Otherwise, compute GCD (V, |U - V|).  */
  if (u1 == v1 && u0 == v0)
    return vsize;

  v0 = (u0 == v0) ? (u1 > v1) ? u1-v1 : v1-u1 : (u0 > v0) ? u0-v0 : v0-u0;
  vp[0] = mpn_gcd_1 (vp, vsize, v0);

  return 1;
}

#if 1
static void
mul_2 (mp_ptr rp, mp_srcptr ap, mp_size_t n, mp_srcptr bp)
{
  mp_limb_t bh, bl;
  
  /* Chaining variables */
  mp_limb_t cy, hi;

  /* Temporaries */
  mp_limb_t sh, sl;
  mp_limb_t th, tl;
  mp_size_t i;

  ASSERT (n > 1);

  bl = bp[0];
  umul_ppmm (hi, rp[0], bl, ap[0]);
  bh = bp[1];
  
  for (i = 1, cy = 0; i < n; i++)
    {
      umul_ppmm (sh, sl, bh, ap[i-1]);
      umul_ppmm (th, tl, bl, ap[i]);

      /* We can always add in cy and hi without overflow */
      add_ssaaaa (sh, sl, sh, sl, cy, hi);

      add_ssaaaa (hi, rp[i], th, tl, sh, sl);
      cy = (hi < th);
    }

  umul_ppmm (sh, sl, bh, ap[n-1]);
  add_ssaaaa (rp[n+1], rp[n], sh, sl, cy, hi);  
}
#else
static void
mul_2 (mp_ptr rp, mp_srcptr ap, mp_size_t n, mp_srcptr bp)
{
  rp[n] = mpn_mul_1 (rp, ap, n, bp[0]);
  rp[n+1] = mpn_addmul_1 (rp + 1, ap, n, bp[1]);
}  
#endif

/* Computes x . y = x_1 y_1 + x_2 y_2. No check for overflow. */
#define dotmul_ppxxyy(ph, pl, x1, x2, y1, y2)				\
do {									\
  mp_limb_t dotmul_sh, dotmul_sl, dotmul_th, dotmul_tl;			\
  umul_ppmm (dotmul_sh, dotmul_sl, (x1), (y1));				\
  umul_ppmm (dotmul_th, dotmul_tl, (x2), (y2));				\
  add_ssaaaa ((ph), (pl), dotmul_sh, dotmul_sl, dotmul_th, dotmul_tl);	\
} while (0)

struct ngcd_matrix2
{
  mp_limb_t u[2][2][2];
};

/* Performs two hgcd2 steps on the five-limb numbers ap, bp.
   Three possible results:

   0 The first nhgcd2 call failed.
   
   1 The first nhgcd2 call succeeded, the second failed.
     ap, bp are updated, and the resulting matrix is returned in M1.

   2 Both hgcd2 calls succeeded. ap, bp are updated. The resulting
     matrix is returned in M.

   Returns the reduced size of ap, bp, >= 3.
*/

static mp_size_t
nhgcd5 (mp_ptr ap, mp_ptr bp, int *res,
	struct ngcd_matrix1 *M1, struct ngcd_matrix2 *M)
{
  struct ngcd_matrix1 M2;
  mp_limb_t t[5];
  mp_size_t n;
  mp_limb_t ah, al, bh, bl;
  mp_limb_t mask;

  mask = ap[4] | bp[4];
  ASSERT (mask > 0);

  if (mask & GMP_NUMB_HIGHBIT)
    {
      ah = ap[4]; al = ap[3];
      bh = bp[4]; bl = bp[3];
    }
  else
    {
      int shift;

      count_leading_zeros (shift, mask);
      ah = MPN_EXTRACT_LIMB (shift, ap[4], ap[3]);
      al = MPN_EXTRACT_LIMB (shift, ap[3], ap[2]);
      bh = MPN_EXTRACT_LIMB (shift, bp[4], bp[3]);
      bl = MPN_EXTRACT_LIMB (shift, bp[3], bp[2]);
    }

  /* Try an mpn_nhgcd2 step */
  if (!mpn_nhgcd2 (ah, al, bh, bl, M1))
    {
      *res = 0;
      return 0;
    }
  n = mpn_ngcd_matrix1_vector (M1, 5, ap, bp, t);
  ASSERT (n >= 4);
  
  mask = ap[n-1] | bp[n-1];
  ASSERT (mask > 0);
  
  if (mask & GMP_NUMB_HIGHBIT)
    {
      ah = ap[n-1]; al = ap[n-2];
      bh = bp[n-1]; bl = bp[n-2];
    }
  else
    {
      int shift;

      count_leading_zeros (shift, mask);
      ah = MPN_EXTRACT_LIMB (shift, ap[n-1], ap[n-2]);
      al = MPN_EXTRACT_LIMB (shift, ap[n-2], ap[n-3]);
      bh = MPN_EXTRACT_LIMB (shift, bp[n-1], bp[n-2]);
      bl = MPN_EXTRACT_LIMB (shift, bp[n-2], bp[n-3]);
    }
  if (!mpn_nhgcd2(ah, al, bh, bl, &M2))
    {
      *res = 1;
      return n;
    }
  
  n = mpn_ngcd_matrix1_vector (&M2, n, ap, bp, t);
  ASSERT (n >= 3);

  /* Multiply M = M1 M2 */
  dotmul_ppxxyy (M->u[0][0][1], M->u[0][0][0],
		 M1->u[0][0], M1->u[0][1], M2.u[0][0], M2.u[1][0]);
  dotmul_ppxxyy (M->u[0][1][1], M->u[0][1][0],
		 M1->u[0][0], M1->u[0][1], M2.u[0][1], M2.u[1][1]);
  dotmul_ppxxyy (M->u[1][0][1], M->u[1][0][0],
		 M1->u[1][0], M1->u[1][1], M2.u[0][0], M2.u[1][0]);
  dotmul_ppxxyy (M->u[1][1][1], M->u[1][1][0],
		 M1->u[1][0], M1->u[1][1], M2.u[0][1], M2.u[1][1]);

  *res = 2;
  return n;
}

/* Multiplies the least significant p limbs of a,b by M^-1, and adds
   to the most significant n limbs. Needs temporary space p. */
static mp_size_t
ngcd_matrix1_adjust (struct ngcd_matrix1 *M,
		     mp_size_t n, mp_ptr ap, mp_ptr bp,
		     mp_size_t p, mp_ptr tp)
{
  /* M^-1 (a;b) = (r11, -r01; -r10, r00) (a ; b)
     = (r11 a - r01 b; - r10 a + r00 b */

  mp_limb_t ah, bh;
  mp_limb_t cy;
  
  /* Copy old value */
  MPN_COPY(tp, ap, p);

  /* Update a */
  ah = mpn_mul_1 (ap, ap, p, M->u[1][1]);
  cy = mpn_submul_1 (ap, bp, p, M->u[0][1]);
  if (cy > ah)
    {
      MPN_DECR_U (ap + p, n, cy - ah);
      ah = 0;
    }
  else
    {
      ah -= cy;
      if (ah > 0)
	ah = mpn_add_1 (ap + p, ap + p, n, ah);
    }
  
  /* Update b */
  bh = mpn_mul_1 (bp, bp, p, M->u[0][0]);
  cy = mpn_submul_1 (bp, tp, p, M->u[1][0]);
  if(cy > bh)
    {
      MPN_DECR_U (bp + p, n, cy - bh);
      bh = 0;
    }
  else
    {
      bh -= cy;
      if (bh > 0)
	bh = mpn_add_1 (bp + p, bp + p, n, bh);
    }

  n += p;

  if (ah > 0 || bh > 0)
    {
      ap[n] = ah;
      bp[n] = bh;
      n++;
    }
  else
    {
      /* The subtraction can reduce the size by at most one limb. */
      if (ap[n-1] == 0 && bp[n-1] == 0)
	n--;
    }
  ASSERT (ap[n-1] > 0 || bp[n-1] > 0);
  return n;  
}

/* Multiplies the least significant p limbs of a,b by M^-1, and adds
   to the most significant n limbs. Needs temporary 2*(p + 2). */
static mp_size_t
ngcd_matrix2_adjust (struct ngcd_matrix2 *M,
		     mp_size_t n, mp_ptr ap, mp_ptr bp,
		     mp_size_t p, mp_ptr tp)
{
  /* M^-1 (a;b) = (r11, -r01; -r10, r00) (a ; b)
     = (r11 a - r01 b; - r10 a + r00 b */

  mp_ptr t0 = tp;
  mp_ptr t1 = tp + p + 2;
  mp_limb_t ah, bh;
  mp_limb_t cy;

  ASSERT (n > 2);
  ASSERT (p >= 2);
  
  /* First compute the two values depending on a, before overwriting a */
  mul_2 (t0, ap, p, M->u[1][1]);
  mul_2 (t1, ap, p, M->u[1][0]);

  /* Update a */
  MPN_COPY (ap, t0, p);
  ah = mpn_add (ap + p, ap + p, n, t0 + p, 2);

  mpn_mul (t0, bp, p, M->u[0][1], 2);
  cy = mpn_sub (ap, ap, n + p, t0, p + 2);

  ASSERT (cy <= ah);
  ah -= cy;

  /* Update b */
  mpn_mul (t0, bp, p, M->u[0][0], 2);
  MPN_COPY (bp, t0, p);
  bh = mpn_add (bp + p, bp + p, n, t0 + p, 2);

  cy = mpn_sub (bp, bp, n + p, t1, p + 2);
  ASSERT (cy <= bh);
  bh -= cy;

  n+= p;

  if (ah > 0 || bh > 0)
    {
      ap[n] = ah;
      bp[n] = bh;
      n++;
    }
  else
    {
      /* The subtraction can reduce the size by at most one limb. */
      if (ap[n-1] == 0 && bp[n-1] == 0)
	n--;
    }
  ASSERT (ap[n-1] > 0 || bp[n-1] > 0);
  return n;
}

/* Needs temporary storage for the division */

/* If the gcd is found, stores it in gp and *gn, and returns zero.
   Otherwise, compute the reduced a and b, and return the new size. */
mp_size_t
mpn_ngcd_subdiv_step (mp_ptr gp, mp_size_t *gn,
		      mp_ptr ap, mp_ptr bp, mp_size_t n, mp_ptr tp)
{
  /* Called when nhgcd or mpn_nhgcd2 has failed. Then either one of a or b
     is very small, or the difference is very small. Perform one
     subtraction followed by one division. */

  mp_size_t an, bn;

  ASSERT (n > 0);
  ASSERT (ap[n-1] > 0 || bp[n-1] > 0);

  /* First, make sure that an >= bn, and subtract an -= bn */
  for (an = n; an > 0; an--)
    if (ap[an-1] != bp[an-1])
      break;

  if (an == 0)
    {
      /* Done */
      MPN_COPY (gp, ap, n);
      *gn = n;
      return 0;
    }

  if (ap[an-1] < bp[an-1])
    MP_PTR_SWAP (ap, bp);

  bn = n;
  MPN_NORMALIZE (bp, bn);
  if (bn == 0)
    {
      MPN_COPY (gp, ap, n);
      *gn = n;
      return 0;
    }

  ASSERT_NOCARRY (mpn_sub_n (ap, ap, bp, an));
  MPN_NORMALIZE (ap, an);

  ASSERT (an > 0);
	  
  if (an < bn)
    MPN_PTR_SWAP (ap, an, bp, bn);
  else if (an == bn)
    {
      int c;
      MPN_CMP (c, ap, bp, an);
      if (c < 0)
	MP_PTR_SWAP (ap, bp);
    }

  ASSERT (an >= bn);

  mpn_tdiv_qr (tp, ap, 0, ap, an, bp, bn);

  /* Normalizing seems to be the simplest way to test if the remainder
     is zero. */
  an = bn;
  MPN_NORMALIZE (ap, an);
  if (an == 0)
    {
      MPN_COPY (gp, bp, bn);
      *gn = bn;
      return 0;
    }

  return bn;
}

#define EVEN_P(x) (((x) & 1) == 0)

/* Needs n limbs of storage for ngcd_matrix1_vector, or n+1 for
   division, or 2*n for ngcd_matrix2_adjust. */

mp_size_t
mpn_ngcd_lehmer (mp_ptr gp, mp_ptr ap, mp_ptr bp, mp_size_t n, mp_ptr tp)
{
  mp_size_t gn;
  
  while (ABOVE_THRESHOLD (n, NGCD_LEHMER_THRESHOLD))
    {
      struct ngcd_matrix1 M1;
      struct ngcd_matrix2 M2;
      mp_size_t p;
      int res;
      mp_size_t nn;

      p = n - 5;
      nn = nhgcd5 (ap + p, bp + p, &res, &M1, &M2);
      switch (res)
	{
	default: abort();
	case 0:
	  n = mpn_ngcd_subdiv_step (gp, &gn, ap, bp, n, tp);
	  if (n == 0)
	    return gn;
	  break;

	case 1:
	  n = ngcd_matrix1_adjust (&M1, nn, ap, bp, p, tp);
	  break;

	case 2:
	  n = ngcd_matrix2_adjust (&M2, nn, ap, bp, p, tp);
	  break;
	}
    }
  while (n > 2)
    {
      struct ngcd_matrix1 M;
      mp_limb_t ah, al, bh, bl;
      mp_limb_t mask;

      mask = ap[n-1] | bp[n-1];
      ASSERT (mask > 0);

      if (mask & GMP_NUMB_HIGHBIT)
	{
	  ah = ap[n-1]; al = ap[n-2];
	  bh = bp[n-1]; bl = bp[n-2];
	}
      else
	{
	  int shift;

	  count_leading_zeros (shift, mask);
	  ah = MPN_EXTRACT_LIMB (shift, ap[n-1], ap[n-2]);
	  al = MPN_EXTRACT_LIMB (shift, ap[n-2], ap[n-3]);
	  bh = MPN_EXTRACT_LIMB (shift, bp[n-1], bp[n-2]);
	  bl = MPN_EXTRACT_LIMB (shift, bp[n-2], bp[n-3]);
	}

      /* Try an mpn_nhgcd2 step */
      if (mpn_nhgcd2 (ah, al, bh, bl, &M))
	n = mpn_ngcd_matrix1_vector (&M, n, ap, bp, tp);

      else
	{
	  n = mpn_ngcd_subdiv_step (gp, &gn, ap, bp, n, tp);
	  if (n == 0)
	    return gn;
	}
    }

  ASSERT (n <= 2);

  if (n == 1)

    {
      *gp = mpn_gcd_1 (ap, 1, bp[0]);
      return 1;
    }
  
  /* Due to the calling convention for mpn_gcd, at most one can be
     even. */

  if (EVEN_P (ap[0]))
    MP_PTR_SWAP (ap, bp);

  ASSERT (!EVEN_P (ap[0]));

  if (bp[0] == 0)
    {
      *gp = mpn_gcd_1 (ap, 2, bp[1]);
      return 1;
    }
  else if (EVEN_P (bp[0]))
    {
      int r;
      count_trailing_zeros (r, bp[0]);
      bp[0] = ((bp[1] << (GMP_NUMB_BITS - r)) & GMP_NUMB_MASK) | (bp[0] >> r);
      bp[1] >>= r;
    }

  n = gcd_2 (ap, bp);
  MPN_COPY (gp, ap, n);
  return n;
}

mp_size_t
mpn_lgcd (mp_ptr gp, mp_ptr ap, mp_size_t an, mp_ptr bp, mp_size_t bn)
{
  mp_size_t gn;
  mp_ptr tp;
  mp_size_t scratch;
  TMP_DECL;

  scratch = MPN_NGCD_LEHMER_ITCH (bn);
  if (an >= scratch)
    scratch = an + 1;

  TMP_MARK;
  
  tp = TMP_ALLOC_LIMBS (scratch);

  if (an > bn)
    {
      mpn_tdiv_qr (tp + bn, tp, 0, ap, an, bp, bn);
      an = bn;
      MPN_NORMALIZE (tp, an);
      if (an == 0)
	{
	  MPN_COPY (gp, bp, bn);
	  TMP_FREE;
	  return bn;
	}
      else
	MPN_COPY (ap, tp, bn);
    }

  gn = mpn_ngcd_lehmer (gp, ap, bp, bn, tp);
  TMP_FREE;
  return gn;
}

/*
   Set (u0, u1) = (u0, u1) M 
	Requires temporary space un
*/
void ngcdext_cofactor1_adjust(mp_ptr u0, mp_ptr u1, mp_size_t * un, struct ngcd_matrix1 *M, mp_ptr tp)
{
	/* Let M = (r00, r01)
	           (r10, r11)
	   We want u0 = u0 * r00 + u1 * r10
		        u1 = u0 * r01 + u1 * r11
	   We make a copy of u0 at tp and update u0 first
	*/

    mp_limb_t cy, cy2;
   	
	MPN_COPY(tp, u0, *un);

	cy = mpn_mul_1(u0, u0, *un, M->u[0][0]); 
	cy += mpn_addmul_1(u0, u1, *un, M->u[1][0]);
	
	cy2 = mpn_mul_1(u1, u1, *un, M->u[1][1]); 
	cy2 += mpn_addmul_1(u1, tp, *un, M->u[0][1]); 
	
	if ((cy) || (cy2)) /* normalise u0, u1 */
	{
		u0[*un] = cy;
		u1[*un] = cy2;
		(*un) ++;
	} 
}

mp_limb_t mpn_gcdext_1(mp_limb_signed_t * a, mp_limb_signed_t *b, mp_limb_t x, mp_limb_t y)
{
   mp_limb_signed_t u1 = CNST_LIMB(1); 
   mp_limb_signed_t u2 = CNST_LIMB(0); 
   mp_limb_signed_t v1 = CNST_LIMB(0); 
   mp_limb_signed_t v2 = CNST_LIMB(1); 
   mp_limb_signed_t t1, t2; 
   mp_limb_t u3, v3;
   mp_limb_t quot, rem;
   
   u3 = x, v3 = y;
   
   if (v3 > u3)
   {
         rem = u3;
         t1 = u2; u2 = u1; u1 = t1; u3 = v3;
         t2 = v2; v2 = v1; v1 = t2; v3 = rem;
   }
   
   if ((mp_limb_signed_t) (x & y) < (mp_limb_signed_t) CNST_LIMB(0)) // x and y both have top bit set  
   {
     quot=u3-v3;
     t2 = v2; 
     t1 = u2; u2 = u1 - u2; u1 = t1; u3 = v3;
     v2 = v1 - v2; v1 = t2; v3 = quot;
   }

   while ((mp_limb_signed_t) (v3<<1) < (mp_limb_signed_t) CNST_LIMB(0)) // second value has second msb set 
   {
     quot=u3-v3;
     if (quot < v3)
     {
        t2 = v2; 
        t1 = u2; u2 = u1 - u2; u1 = t1; u3 = v3;
        v2 = v1 - v2; v1 = t2; v3 = quot;
     } else if (quot < (v3<<1))
     {  
        t1 = u2; u2 = u1 - (u2<<1); u1 = t1; u3 = v3;
        t2 = v2; v2 = v1 - (v2<<1); v1 = t2; v3 = quot-u3;
     } else
     {
        t1 = u2; u2 = u1 - 3*u2; u1 = t1; u3 = v3;
        t2 = v2; v2 = v1 - 3*v2; v1 = t2; v3 = quot-(u3<<1);
     }
   }
   
   while (v3) {
      quot=u3-v3;
      if (u3 < (v3<<2)) // overflow not possible due to top 2 bits of v3 not being set 
      {
         if (quot < v3)
         {
            t2 = v2; 
            t1 = u2; u2 = u1 - u2; u1 = t1; u3 = v3;
            v2 = v1 - v2; v1 = t2; v3 = quot;
         } else if (quot < (v3<<1))
         {  
            t1 = u2; u2 = u1 - (u2<<1); u1 = t1; u3 = v3;
            t2 = v2; v2 = v1 - (v2<<1); v1 = t2; v3 = quot-u3;
         } else
         {
            t1 = u2; u2 = u1 - 3*u2; u1 = t1; u3 = v3;
            t2 = v2; v2 = v1 - 3*v2; v1 = t2; v3 = quot-(u3<<1);
         }
      } else
      {
         quot=u3/v3;
         rem = u3 - v3*quot;
         t1 = u2; u2 = u1 - quot*u2; u1 = t1; u3 = v3;
         t2 = v2; v2 = v1 - quot*v2; v1 = t2; v3 = rem;
      }
   }
   
   // Quite remarkably, this always has |u1| < x/2 at this point, thus comparison with 0 is valid 
   /*if (u1 <= (mp_limb_signed_t) 0) 
   {    
	 u1 += y;
     v1 -= x;
   } */

   *a = u1;
   *b = -v1;
   
   return u3;
 
}

/* 
   Requires temporary space MPN_GCD_LEHMER_N_ITCH(n) + 2*n+2
*/
mp_size_t
mpn_ngcdext_lehmer (mp_ptr gp, mp_ptr s0p, mp_size_t *s0size, mp_ptr ap, mp_ptr bp, mp_size_t n, mp_ptr tp)
{
  mp_size_t gn, un;
  mp_ptr u0, u1;
  mp_limb_t cy, cy2;
  mp_limb_signed_t s, t;
  int c, negate = 0;
  
  MPN_ZERO(tp, 2*n+2);
  u0 = tp;
  u1 = tp + n + 1;
  tp += 2*n + 2;
  
  un = 1;
  u0[0] = CNST_LIMB(0); /* bp = 0*ap + ?*bp, thus u0 = -0 */
  u1[0] = CNST_LIMB(1); /* ap = 1*ap + ?*bp, thus u1 = 1 */

  while (n >= 2)
    {
      struct ngcd_matrix1 M;
      mp_limb_t ah, al, bh, bl;
      mp_limb_t mask;

      mask = ap[n-1] | bp[n-1];
      ASSERT (mask > 0);

      if (mask & GMP_NUMB_HIGHBIT)
	{
	  ah = ap[n-1]; al = ap[n-2];
	  bh = bp[n-1]; bl = bp[n-2];
	}
      else
	{
	  int shift;

	  count_leading_zeros (shift, mask);
	  ah = MPN_EXTRACT_LIMB (shift, ap[n-1], ap[n-2]);
	  bh = MPN_EXTRACT_LIMB (shift, bp[n-1], bp[n-2]);
	  if (n == 2) /* special case for n = 2 */
      {
        al = ap[0] << shift;
	    bl = bp[0] << shift;
      } else
      {
        al = MPN_EXTRACT_LIMB (shift, ap[n-2], ap[n-3]);
	    bl = MPN_EXTRACT_LIMB (shift, bp[n-2], bp[n-3]);
	  }
    }

      /* Try an mpn_nhgcd2 step */
      if (mpn_nhgcd2 (ah, al, bh, bl, &M))
	  {
        n = mpn_ngcd_matrix1_vector (&M, n, ap, bp, tp);
        ngcdext_cofactor1_adjust(u0, u1, &un, &M, tp);
      }

      else
	{
	  n = mpn_ngcdext_subdiv_step (gp, &gn, s0p, u0, u1, &un, ap, bp, n, tp);
	  if (n == 0)
	    {
	      (*s0size) = un;
			ASSERT(((*s0size) == 0) || (s0p[ABS(*s0size) - 1] != 0));
		   
			return gn;
	    }
	}
    }

  ASSERT (n < 2);
  	
  if (!ap[0])
  {
      /* 
        If ap == 0 then gp = bp
		with cofactor -u0
	  */
	  gp[0] = bp[0];
	  MPN_NORMALIZE(u0, un);
	  MPN_COPY(s0p, u0, un);
      (*s0size) = -un;
      
	  return 1;
  } else if (!bp[0])
  {
      /* 
        If bp == 0 then gp = ap
		with cofactor u1
		case is rare
	  */
	  gp[0] = ap[0];
	  MPN_NORMALIZE(u1, un);
	  MPN_COPY(s0p, u1, un);
      (*s0size) = un;
      
	  return 1;
  } 
  
  if (ap[0] == bp[0])
  {
     gp[0] = ap[0];
	 
	 /* we have gp = ap OR bp
	    so cofactor == u1 or -u0
		we want the smallest.
	 */
     
	 MPN_CMP(c, u1, u0, un);
	 if (c <= 0) // u1 is smallest
	 {
	    MPN_NORMALIZE(u1, un);
	    MPN_COPY(s0p, u1, un);
		(*s0size) = un;
 	 } else // -u0 is smaller
	 {
	    MPN_NORMALIZE(u0, un);
	    MPN_COPY(s0p, u0, un);
		(*s0size) = -un;
	 }
	 
	 return 1;		   
  }

  gp[0] = mpn_gcdext_1 (&s, &t, ap[0], bp[0]);
      
  if (s <= (mp_limb_signed_t) 0)
  {
	 s = -s;
	 t = -t;
	 negate = 1;
  }

  /* 
     We want to compute s*u1 + t*u0.
  */

  cy = mpn_mul_1(s0p, u0, un, t);
  cy2 = mpn_addmul_1(s0p, u1, un, s);
  
  if (cy | cy2) /* u0 is bigger than un limbs */
  {
     cy +=cy2;
     s0p[un] = cy;
     un++;
     if (cy < cy2) /* overflow on addition */
     {
        s0p[un] = CNST_LIMB(1);
        un++;
     }
  }
  
  MPN_NORMALIZE(s0p, un);
  (*s0size) = un;
  if (negate) 
     (*s0size) = -(*s0size);
  
  return 1;
}

