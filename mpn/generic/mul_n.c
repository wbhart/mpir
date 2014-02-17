/* mpn_mul_n and helper function -- Multiply/square natural numbers.

   THE HELPER FUNCTIONS IN THIS FILE (meaning everything except mpn_mul_n)
   ARE INTERNAL FUNCTIONS WITH MUTABLE INTERFACES.  IT IS ONLY SAFE TO REACH
   THEM THROUGH DOCUMENTED INTERFACES.  IN FACT, IT IS ALMOST GUARANTEED
   THAT THEY'LL CHANGE OR DISAPPEAR IN A FUTURE GNU MP RELEASE.


Copyright 1991, 1993, 1994, 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003,
2005, Free Software Foundation, Inc.

Copyright 2009 Jason Moxham
Copyright 2009 William Hart
Copyright 2011 The Code Cavern

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

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"


#if ! HAVE_NATIVE_mpn_karasub && HAVE_NATIVE_mpn_addsub_n

static void	mpn_karasub(mp_ptr rp, mp_ptr tp, mp_size_t n)
{
   mp_size_t n2, n3;
   mp_limb_t c1 = 0, c2, c3, top[2];

   n2 = n>>1;
   n3 = n - n2;

   c2 = mpn_addsub_n(tp, rp, rp + 2*n2, tp, 2*n2);
   c3 = mpn_add_n(rp + n2, rp + n2, tp, 2*n2);

   top[1] = rp[2*n2 + 2*n3 - 1];
   top[0] = rp[2*n2 + 2*n3 - 2];

   mpn_incr_u(rp + 3*n2, c3);

   if (c2 == 1) mpn_incr_u(rp + 3*n2, 1);
   if (c2 == -1) mpn_decr_u(rp + 3*n2, 1);

   if (n2 == n3) 
      return;

   c1=mpn_sub_n(rp + 3*n2, rp + 3*n2, tp + 2*n2, 2);
   c2=mpn_add_n(rp + 3*n2, rp + 3*n2, top, 2);

   if(c2 == 1 && c1 == 0) mpn_incr_u(rp + 3*n2 + 2, 1);
   if(c2 == 0 && c1 == 1) mpn_decr_u(rp + 3*n2 + 2, 1);
}
#endif

#if ! HAVE_NATIVE_mpn_karaadd && HAVE_NATIVE_mpn_addadd_n

static void	mpn_karaadd(mp_ptr rp, mp_ptr tp, mp_size_t n)
{
   mp_size_t n2, n3;
   mp_limb_t c1 = 0, c2, c3;

   n2 = n>>1;
   n3 = n - n2;

   c2 = mpn_addadd_n(tp, rp, rp + 2*n2, tp, 2*n2);

   if (n3 != n2) c1 = mpn_add_n(tp + 2*n2, rp + 4*n2, tp + 2*n2, 2);

   c3 = mpn_add_n(rp + n2, rp + n2, tp, 2*n3);

   mpn_incr_u(rp + n2 + 2*n3, c3 + c1);
   mpn_incr_u(rp + n2 + 2*n2, c2);
}
#endif

#if ! HAVE_NATIVE_mpn_karasub && ! HAVE_NATIVE_mpn_addsub_n

static void	mpn_karasub(mp_ptr rp, mp_ptr tp, mp_size_t n)
{
   mp_size_t n2, n3;
   mp_limb_t c1, c2, c3, top[2];

   n2 = n>>1;
   n3 = n - n2;

   c1 = mpn_sub_n(tp, rp + 2*n2, tp, 2*n2);
   c2 = mpn_add_n(tp, tp, rp, 2*n2);
   c3 = mpn_add_n(rp + n2, rp + n2, tp, 2*n2);

   top[1] = rp[2*n2 + 2*n3 - 1];
   top[0] = rp[2*n2 + 2*n3 - 2];

   mpn_incr_u(rp + 3*n2, c3);
   mpn_incr_u(rp + 3*n2, c2);
   mpn_decr_u(rp + 3*n2, c1);

   if(n2 == n3)
      return;

   c1 = mpn_sub_n(rp + 3*n2, rp + 3*n2, tp + 2*n2, 2);
   c2 = mpn_add_n(rp + 3*n2, rp + 3*n2, top, 2);

   if(c2 == 1 && c1 == 0) mpn_incr_u(rp + 3*n2 + 2, 1);
   if(c2 == 0 && c1 == 1) mpn_decr_u(rp + 3*n2 + 2, 1);
}
#endif

#if ! HAVE_NATIVE_mpn_karaadd && ! HAVE_NATIVE_mpn_addadd_n

static void	mpn_karaadd(mp_ptr rp, mp_ptr tp, mp_size_t n)
{
   mp_size_t n2, n3;
   mp_limb_t c1, c2, c3;

   n2 = n>>1;
   n3 = n - n2;

   c1 = mpn_add_n(tp, rp + 2*n2, tp, 2*n3);
   c2 = mpn_add_n(tp, tp, rp, 2*n2);
   c3 = mpn_add_n(rp + n2, rp + n2, tp, 2*n3);
   
   mpn_incr_u(rp + n2 + 2*n3, c3 + c1);
   mpn_incr_u(rp + n2 + 2*n2, c2);
}
#endif

/* (rp, 2n) = (xp, n)*(yp, n) with temp space (tp, 2*n + C) */
void	mpn_kara_mul_n(mp_ptr rp, mp_srcptr xp, mp_srcptr yp, mp_size_t n, mp_ptr tp)
{
   mp_size_t n2, n3;
   mp_srcptr xl, yl, xh, yh;
   mp_ptr dx, dy;
   int suboradd;
   mp_limb_t c;

   n2 = n>>1;
   suboradd = -1;
   xl = xp;
   xh = xp + n2;
   yl = yp;
   yh = yp + n2;
   n3 = n - n2;
   dx = rp + 2*n2;
   dy = dx + n3;

   if ((n&1) == 0)
   {
      if (mpn_cmp(xh, xl, n2) >= 0)
         mpn_sub_n(dx, xh, xl, n2);
      else
      {
         mpn_sub_n(dx, xl, xh, n2);
         suboradd = -suboradd;
      }
      
      if (mpn_cmp(yh, yl, n2) >= 0)
         mpn_sub_n(dy, yh, yl, n2);
      else
      {
         mpn_sub_n(dy, yl, yh, n2);
         suboradd = -suboradd;
      }
   }
   else
   {
      if (xh[n2] !=0 || mpn_cmp(xh, xl, n2) >= 0)
      {
         c = mpn_sub_n(dx, xh, xl, n2);
         dx[n2] = xh[n2] - c;
      }
      else
      {
         mpn_sub_n(dx, xl, xh, n2);
         dx[n2] = 0;
         suboradd = -suboradd;
      }
      
      if (yh[n2] != 0 || mpn_cmp(yh, yl, n2) >= 0)
      {
         c = mpn_sub_n(dy, yh, yl, n2);
         dy[n2] = yh[n2] - c;
      }
      else
      {
         mpn_sub_n(dy, yl, yh, n2);
         dy[n2] = 0;
         suboradd = -suboradd;
      }
   }

   if (BELOW_THRESHOLD(n3, MUL_KARATSUBA_THRESHOLD))
   {
      mpn_mul_basecase(rp, xl, n2, yl, n2);
      mpn_mul_basecase(tp, dx, n3, dy, n3);
      mpn_mul_basecase(rp + 2*n2, xh, n3, yh, n3);
   }
   else
   {
      mpn_kara_mul_n(rp, xl, yl, n2, tp + 2*n3);
      mpn_kara_mul_n(tp, dx, dy, n3, tp + 2*n3);   
      mpn_kara_mul_n(rp + 2*n2, xh, yh, n3, tp + 2*n3);
   }

   if (suboradd == -1)
      mpn_karasub(rp, tp, n);
   else
      mpn_karaadd(rp, tp, n);
}

/* (rp, 2n) = (xp, n)^2 with temp space (tp, 2*n + C) */
void mpn_kara_sqr_n(mp_ptr rp, mp_srcptr xp, mp_size_t n, mp_ptr tp)
{
   mp_size_t n2, n3;
   mp_srcptr xl, xh;
   mp_ptr dx;
   mp_limb_t c;

   n2 = n>>1;
   xl = xp;
   xh = xp + n2;
   n3 = n - n2;
   dx = rp + 2*n2;

   if ((n&1) == 0)
   {
      if (mpn_cmp(xh, xl, n2) >=0)
         mpn_sub_n(dx, xh, xl, n2);
      else
         mpn_sub_n(dx, xl, xh, n2);
   }
   else
   {
      if (xh[n2] != 0 || mpn_cmp(xh, xl, n2) >= 0)
      {
         c = mpn_sub_n(dx, xh, xl, n2);
         dx[n2] = xh[n2] - c;
      }
      else
      {
         mpn_sub_n(dx, xl, xh, n2);
         dx[n2] = 0;
      }
   }

   if (BELOW_THRESHOLD(n3, SQR_BASECASE_THRESHOLD))
   {
      mpn_mul_basecase(rp, xl, n2, xl, n2);
      mpn_mul_basecase(tp, dx, n3, dx, n3);
      mpn_mul_basecase(rp + 2*n2, xh, n3, xh, n3);
   }
   else if (BELOW_THRESHOLD(n3, SQR_KARATSUBA_THRESHOLD))
   {
      mpn_sqr_basecase(rp, xl, n2);
      mpn_sqr_basecase(tp, dx, n3);
      mpn_sqr_basecase(rp + 2*n2, xh, n3);
   }
   else
   {
      mpn_kara_sqr_n(rp, xl, n2, tp + 2*n3);
      mpn_kara_sqr_n(tp, dx, n3, tp + 2*n3);   
      mpn_kara_sqr_n(rp + 2*n2, xh, n3, tp + 2*n3);
   }

   mpn_karasub(rp, tp, n);
}

void
mpn_mul_n (mp_ptr p, mp_srcptr a, mp_srcptr b, mp_size_t n)
{
  ASSERT (n >= 1);
  ASSERT (! MPN_OVERLAP_P (p, 2 * n, a, n));
  ASSERT (! MPN_OVERLAP_P (p, 2 * n, b, n));

  if (BELOW_THRESHOLD (n, MUL_KARATSUBA_THRESHOLD))
    {
      mpn_mul_basecase (p, a, n, b, n);
    }
  else if (BELOW_THRESHOLD (n, MUL_TOOM3_THRESHOLD))
    {
      /* Allocate workspace of fixed size on stack: fast! */
      mp_limb_t ws[MPN_KARA_MUL_N_TSIZE (MUL_TOOM3_THRESHOLD_LIMIT-1)];
      ASSERT (MUL_TOOM3_THRESHOLD <= MUL_TOOM3_THRESHOLD_LIMIT);
      mpn_kara_mul_n (p, a, b, n, ws);
    }
  else if (BELOW_THRESHOLD (n, MUL_TOOM4_THRESHOLD))
    {
      mp_ptr ws;
      TMP_SDECL;
      TMP_SMARK;
      ws = TMP_SALLOC_LIMBS (MPN_TOOM3_MUL_N_TSIZE (n));
      mpn_toom3_mul_n (p, a, b, n, ws);
      TMP_SFREE;
    }
  else if (BELOW_THRESHOLD (n, MUL_TOOM8H_THRESHOLD))
    {
       mpn_toom4_mul_n (p, a, b, n);
    }
#if WANT_FFT || TUNE_PROGRAM_BUILD
  else if (BELOW_THRESHOLD (n, MUL_FFT_FULL_THRESHOLD))
    {
       mpn_toom8h_mul (p, a, n, b, n);
    }
#endif
  else
#if WANT_FFT || TUNE_PROGRAM_BUILD
    {
       mpn_mul_fft_main(p, a, n, b, n); 
    }
#else
    {
      /* Toom8 for large operands. */
      mpn_toom8h_mul (p, a, n, b, n);
    }
#endif
}

void
mpn_sqr (mp_ptr p, mp_srcptr a, mp_size_t n)
{
  ASSERT (n >= 1);
  ASSERT (! MPN_OVERLAP_P (p, 2 * n, a, n));

#if 0
  /* FIXME: Can this be removed? */
  if (n == 0)
    return;
#endif

  if (BELOW_THRESHOLD (n, SQR_BASECASE_THRESHOLD))
    { 
      /* mul_basecase is faster than sqr_basecase on small sizes sometimes */
      mpn_mul_basecase (p, a, n, a, n);
    }
  else if (BELOW_THRESHOLD (n, SQR_KARATSUBA_THRESHOLD))
    {
      mpn_sqr_basecase (p, a, n);
    }
  else if (BELOW_THRESHOLD (n, SQR_TOOM3_THRESHOLD))
    {
      /* Allocate workspace of fixed size on stack: fast! */
      mp_limb_t ws[MPN_KARA_SQR_N_TSIZE (SQR_TOOM3_THRESHOLD_LIMIT-1)];
      ASSERT (SQR_TOOM3_THRESHOLD <= SQR_TOOM3_THRESHOLD_LIMIT);
      mpn_kara_sqr_n (p, a, n, ws);
    }
  else if (BELOW_THRESHOLD (n, SQR_TOOM4_THRESHOLD))
    {
      mp_ptr ws;
      TMP_SDECL;
      TMP_SMARK;
      ws = TMP_SALLOC_LIMBS (MPN_TOOM3_SQR_N_TSIZE (n));
      mpn_toom3_sqr_n (p, a, n, ws);
      TMP_SFREE;
    }
  else if (BELOW_THRESHOLD (n, SQR_TOOM8_THRESHOLD))
    {
       mpn_toom4_sqr_n (p, a, n);
    }
#if WANT_FFT || TUNE_PROGRAM_BUILD
  else if (BELOW_THRESHOLD (n, SQR_FFT_FULL_THRESHOLD))
#else
  else 
#endif
    {
       mpn_toom8_sqr_n (p, a, n);
    }
#if WANT_FFT || TUNE_PROGRAM_BUILD
  else
    {
       mpn_mul_fft_main(p, a, n, a, n); 
    }
#endif
}
