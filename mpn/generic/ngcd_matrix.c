/* Matrix multiplication used by XGCD

Copyright 2004, 2005 Niels Möller
Copyright 2009 William Hart

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

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"

/* Computes r = u x + v y. rn is the size of the result area, and must
   be at least one larger than the result. Needs temporary space of the same size.
   Returns size of result. Zero inputs are allowed.

   No overlap between input and output is allowed, since rp is used
   for temporary storage. */
static mp_limb_t
addmul2_n (mp_ptr rp,
	   mp_srcptr up, mp_size_t un, mp_srcptr xp, mp_size_t xn,
	   mp_srcptr vp, mp_size_t vn, mp_srcptr yp, mp_size_t yn,
	   mp_ptr tp)
{
  mp_size_t t0n, t1n;
  mp_size_t n;
  mp_limb_t cy;

  /* t0 = u * x is stored at rp, and t1 = v * y at tp. */
  t0n = xn + un;
  t1n = yn + vn;

  /* Handle zero cases */
  if (xn == 0 || un == 0)
    {
      if (yn == 0 || vn == 0)
	return 0;

      tp = rp;
      t0n = 0;
    }
  else if (yn == 0 || vn == 0)
    {
      ASSERT (xn > 0);
      ASSERT (un > 0);

      t1n = 0;
    }

  if (t0n > 0)
    {
      if (xn >= un)
	mpn_mul (rp, xp, xn, up, un);
      else
	mpn_mul (rp, up, un, xp, xn);

      t0n -= (rp[t0n-1] == 0);
    }

  if (t1n > 0)
    {
      if (yn >= vn)
	mpn_mul (tp, yp, yn, vp, vn);
      else
	mpn_mul (tp, vp, vn, yp, yn);

      t1n -= (tp[t1n-1] == 0);
    }
  else
    return t0n;

  if (t0n == 0)
    return t1n;

  if (t0n >= t1n)
    {
      cy = mpn_add (rp, rp, t0n, tp, t1n);
      n = t0n;
    }
  else
    {
      cy = mpn_add (rp, tp, t1n, rp, t0n);
      n = t1n;
    }
  rp[n] = cy;
  n += (cy != 0);

  return n;
}

void _strassen_add(mp_ptr rp, mp_size_t * rn, mp_srcptr r1, 
				  mp_size_t r1n, mp_srcptr r2, mp_size_t r2n)
{
   mp_limb_t cy;
   mp_size_t s1 = ABS(r1n);
   mp_size_t s2 = ABS(r2n);
   
   if (!s1)
   {
      *rn = 0;
   } else if (!s2)
   {
      if (rp != r1) MPN_COPY(rp, r1, s1);
		*rn = r1n;
   } else if ((r1n ^ r2n) >= 0)
   {
      *rn = r1n;
      cy = mpn_add(rp, r1, s1, r2, s2);
      if (cy) 
      {
         rp[s1] = cy;
         if ((*rn) < 0) (*rn)--;
         else (*rn)++;
      }
   } else
   {
      mp_size_t ct;
		if (s1 != s2) ct = 1;
		else MPN_CMP(ct, r1, r2, s1); 
		    
      if (!ct) *rn = 0;
      else if (ct > 0) 
      {
         mpn_sub(rp, r1, s1, r2, s2);
         *rn = s1;
         MPN_NORMALIZE(rp, (*rn));
			if (r1n < 0) *rn = -(*rn);
      }
      else
      {
         mpn_sub_n(rp, r2, r1, s1);
         *rn = s1;
         MPN_NORMALIZE(rp, (*rn));
			if (r1n > 0) *rn = -(*rn);
      }
   }
}

void strassen_add(mp_ptr rp, mp_size_t * rn, mp_srcptr r1, mp_size_t r1n, mp_srcptr r2, mp_size_t r2n)
{
   mp_size_t s1 = ABS(r1n);
   mp_size_t s2 = ABS(r2n);
   
   if (s1 < s2) _strassen_add(rp, rn, r2, r2n, r1, r1n);
	else _strassen_add(rp, rn, r1, r1n, r2, r2n);
} 

void _strassen_add_unsigned(mp_ptr rp, mp_size_t * rn, mp_srcptr r1, mp_size_t r1n, mp_srcptr r2, mp_size_t r2n)
{
   mp_limb_t cy;
   mp_size_t s1 = r1n;
   mp_size_t s2 = r2n;
   
   if (!s2)
   {
      if (!s1) *rn = 0;
      else
      {
         if (rp != r1) MPN_COPY(rp, r1, s1);
		   *rn = r1n;
		}
   } else
   {
      *rn = r1n;
      cy = mpn_add(rp, r1, s1, r2, s2);
      if (cy) 
      {
         rp[s1] = cy;
         if ((*rn) < 0) (*rn)--;
         else (*rn)++;
      }
   } 
}

void strassen_add_unsigned(mp_ptr rp, mp_size_t * rn, mp_srcptr r1, mp_size_t r1n, mp_srcptr r2, mp_size_t r2n)
{
   if (r1n < r2n) _strassen_add_unsigned(rp, rn, r2, r2n, r1, r1n);
	else _strassen_add_unsigned(rp, rn, r1, r1n, r2, r2n);
} 

void strassen_sub(mp_ptr rp, mp_size_t * rn, mp_ptr r1, mp_size_t r1n, mp_ptr r2, mp_size_t r2n)
{
   strassen_add(rp, rn, r1, r1n, r2, -r2n);
}

#define MUL_STRASSEN_UNSIGNED(r3xx, n3xx, r1xx, n1xx, r2xx, n2xx) \
   do \
   { \
      if ((n1xx != 0) && (n2xx != 0)) \
      {   mp_size_t len; \
	      if (n1xx == n2xx) \
		      mpn_mul_n(r3xx, r1xx, r2xx, n1xx); \
		  else if (n1xx > n2xx) \
		      mpn_mul(r3xx, r1xx, n1xx, r2xx, n2xx); \
		  else \
		      mpn_mul(r3xx, r2xx, n2xx, r1xx, n1xx); \
	      len = n1xx + n2xx; \
		  MPN_NORMALIZE(r3xx, len); \
		  n3xx = len; \
      } else \
         n3xx = 0; \
   } while (0)

#define MUL_STRASSEN(r3xx, n3xx, r1xx, n1xx, r2xx, n2xx) \
	do \
	{ \
	   mp_size_t sign = n1xx ^ n2xx; \
	   mp_size_t un1 = ABS(n1xx); \
	   mp_size_t un2 = ABS(n2xx); \
	   MUL_STRASSEN_UNSIGNED(r3xx, n3xx, r1xx, un1, r2xx, un2); \
	   if (sign < 0) n3xx = -n3xx; \
	} while (0)

#define STRASSEN_DENORM(rxx, nxx, sxx) \
	do { \
	MPN_ZERO(rxx + ABS(nxx), sxx - ABS(nxx)); \
	} while (0)

/* Multiply M by M1 from the right. Needs 2*Mn + 3*Rn temporary storage
   (and additionally uses M->tp, of size Rn), where Mn is the size of the
   entries in M and Rn is the possible size of the entries in the output
   (computed naively, i.e. M1n + Mn + 1). */
void
mpn_ngcd_matrix_mul_strassen (struct ngcd_matrix *M, const struct ngcd_matrix *M1,
		     mp_ptr tp)
{
  mp_ptr m00 = M1->p[0][0];
  mp_ptr m01 = M1->p[0][1];
  mp_ptr m10 = M1->p[1][0];
  mp_ptr m11 = M1->p[1][1];
  mp_ptr r00 = M->p[0][0];
  mp_ptr r01 = M->p[0][1];
  mp_ptr r10 = M->p[1][0];
  mp_ptr r11 = M->p[1][1];
  mp_size_t n00, n01, n10, n11;
  mp_size_t s00, s01, s10, s11;
  mp_size_t x0n, x1n, tan, tbn, tcn, tdn;

  mp_size_t Rn = M->n + M1->n + 1;
  mp_size_t sn;
  
  mp_ptr x0 = tp;
  mp_ptr x1 = tp + Rn;
  mp_ptr temp_a = tp + 2*Rn;
  mp_ptr temp_b = tp + 3*Rn;
  mp_ptr temp_d = tp + 3*Rn + M->n;
  mp_ptr temp_c = M->tp;

  s00 = s01 = s10 = s11 = M->n;
  n00 = n01 = n10 = n11 = M1->n;

  MPN_NORMALIZE (m00, n00);
  MPN_NORMALIZE (m01, n01);
  MPN_NORMALIZE (m10, n10);
  MPN_NORMALIZE (m11, n11);

  MPN_NORMALIZE (r00, s00);
  MPN_NORMALIZE (r01, s01);
  MPN_NORMALIZE (r10, s10);
  MPN_NORMALIZE (r11, s11);

  strassen_sub(x0, &x0n, r00, s00, r10, s10);
  strassen_sub(x1, &x1n, m11, n11, m01, n01);
  MUL_STRASSEN(temp_c, tcn, x0, x0n, x1, x1n); 

  strassen_add_unsigned(x0, &x0n, r10, s10, r11, s11); 
  strassen_sub(x1, &x1n, m01, n01, m00, n00);
	
  MPN_COPY (temp_d, r11, s11);
  tdn = s11; 
  MUL_STRASSEN(r11, s11, x0, x0n, x1, x1n); 

  strassen_sub(x0, &x0n, x0, x0n, r00, s00);
  strassen_sub(x1, &x1n, m11, n11, x1, x1n);
	
  MPN_COPY (temp_b, r01, s01);
  tbn = s01;
  MUL_STRASSEN(r01, s01, x0, x0n, x1, x1n);

  strassen_sub(x0, &x0n, temp_b, tbn, x0, x0n);
  MUL_STRASSEN(temp_a, tan, x0, x0n, m11, n11); 

  MUL_STRASSEN(x0, x0n, r00, s00, m00, n00); 

  strassen_add(r01, &s01, x0, x0n, r01, s01);
  strassen_add(r10, &s10, r01, s01, temp_c, tcn); 
  strassen_add(r01, &s01, r01, s01, r11, s11);
  strassen_add(r11, &s11, r10, s10, r11, s11);
  strassen_add(r01, &s01, r01, s01, temp_a, tan); 
  strassen_sub(x1, &x1n, x1, x1n, m10, n10);
	
  MUL_STRASSEN(temp_a, tan, temp_d, tdn, x1, x1n); 
  strassen_sub(r10, &s10, r10, s10, temp_a, tan); 
	
  MUL_STRASSEN(r00, s00, temp_b, tbn, m10, n10); 

  strassen_add(r00, &s00, r00, s00, x0, x0n);
  
  sn = MAX(s00, s01);
  sn = MAX(s10, sn);
  sn = MAX(s11, sn);
  M->n = sn;
  
  STRASSEN_DENORM(r00, s00, sn);
  STRASSEN_DENORM(r01, s01, sn);
  STRASSEN_DENORM(r10, s10, sn);
  STRASSEN_DENORM(r11, s11, sn);
}

#define MPN_NGCD_MATRIX_CUTOFF 25

/* Multiply M by M1 from the right. Needs 2*M->n temporary storage
   (and additionally uses M->tp). */
void
mpn_ngcd_matrix_mul (struct ngcd_matrix *M, const struct ngcd_matrix *M1,
		     mp_ptr tp)
{
  unsigned int row;

  mp_ptr m00, m01, m10, m11;
  mp_size_t n00, n01, n10, n11;
  
  mp_size_t n;

  mp_ptr up, vp;
  
  if ((M->n > MPN_NGCD_MATRIX_CUTOFF) && (M1->n > MPN_NGCD_MATRIX_CUTOFF))
  {
    mpn_ngcd_matrix_mul_strassen(M, M1, tp);
    return;
  }
  
  
  m00 = M1->p[0][0];
  m01 = M1->p[0][1];
  m10 = M1->p[1][0];
  m11 = M1->p[1][1];
  
  
  up = tp;
  vp = tp + M->n;

  /* About the new size of M:s elements. Since M1's diagonal elements
     are > 0, no element can decrease. The typical case is that the
     new elements are of size M->n + M1->n, one limb more or less. But
     it may be smaller, consider for example (1,x;0,1)(1,x;0,1) =
     (1,2x;0,1), where size is increased by a single bit no matter how
     large x is. So to avoid writing past the end of M, we need to
     normalise the numbers. */

  n00 = n01 = n10 = n11 = M1->n;
  MPN_NORMALIZE (m00, n00);
  MPN_NORMALIZE (m01, n01);
  MPN_NORMALIZE (m10, n10);
  MPN_NORMALIZE (m11, n11);

  n = 0;
  for (row = 0; row < 2; row++)
    {
      mp_size_t un, vn;
      mp_size_t nn;

      MPN_COPY (up, M->p[row][0], M->n);
      MPN_COPY (vp, M->p[row][1], M->n);

      un = vn = M->n;
      MPN_NORMALIZE (up, un);
      MPN_NORMALIZE (vp, vn);

      ASSERT (n00 + un <= M->alloc);
      ASSERT (n01 + un <= M->alloc);
      ASSERT (n10 + vn <= M->alloc);
      ASSERT (n11 + vn <= M->alloc);

      /* Compute (u', v') = (u,v) (r00, r01; r10, r11)
	 = (r00 u + r10 v, r01 u + r11 v) */

      nn = addmul2_n (M->p[row][0], up, un, m00, n00, vp, vn, m10, n10, M->tp);
      if (nn > n)
	n = nn;
      nn = addmul2_n (M->p[row][1], up, un, m01, n01, vp, vn, m11, n11, M->tp);
      if (nn > n)
	n = nn;
    }
  ASSERT (n < M->alloc);
  M->n = n;
}

/* Multiplies the least significant p limbs of (a;b) by M^-1.
   Temporary space needed: 2 * (p + M->n)*/
mp_size_t
mpn_ngcd_matrix_adjust (struct ngcd_matrix *M,
			mp_size_t n, mp_ptr ap, mp_ptr bp,
			mp_size_t p, mp_ptr tp)
{
  /* M^-1 (a;b) = (r11, -r01; -r10, r00) (a ; b)
     = (r11 a - r01 b; - r10 a + r00 b */

  mp_ptr t0 = tp;
  mp_ptr t1 = tp + p + M->n;
  mp_limb_t ah, bh;
  mp_limb_t cy;

  ASSERT (p + M->n  < n);

  /* First compute the two values depending on a, before overwriting a */

  if (M->n >= p)
    {
      mpn_mul (t0, M->p[1][1], M->n, ap, p);
      mpn_mul (t1, M->p[1][0], M->n, ap, p);
    }
  else
    {
      mpn_mul (t0, ap, p, M->p[1][1], M->n);
      mpn_mul (t1, ap, p, M->p[1][0], M->n);
    }

  /* Update a */
  MPN_COPY (ap, t0, p);
  ah = mpn_add (ap + p, ap + p, n - p, t0 + p, M->n);

  if (M->n >= p)
    mpn_mul (t0, M->p[0][1], M->n, bp, p);
  else
    mpn_mul (t0, bp, p, M->p[0][1], M->n);

  cy = mpn_sub (ap, ap, n, t0, p + M->n);
  ASSERT (cy <= ah);
  ah -= cy;

  /* Update b */
  if (M->n >= p)
    mpn_mul (t0, M->p[0][0], M->n, bp, p);
  else
    mpn_mul (t0, bp, p, M->p[0][0], M->n);

  MPN_COPY (bp, t0, p);
  bh = mpn_add (bp + p, bp + p, n - p, t0 + p, M->n);
  cy = mpn_sub (bp, bp, n, t1, p + M->n);
  ASSERT (cy <= bh);
  bh -= cy;

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
