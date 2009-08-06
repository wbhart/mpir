/* mpn_mulmod_2expp1 

Copyright 2009 Jason Moxham

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

//          k           0 1 2    3   4   5    6    7    8     9    10     11     12      13      14       15
static mp_size_t tab[]={0,0,0,2632,304,448,1024,2304,6400,11264,45056,114688,327680,1310720,3145728,12582912 };// OUT OF DATE
// from K8 old mul_fft_table           560 1184 1856 3840
// these thresholds could do with tuning , and extending

// ret+(xp,n)=(yp,n)*(zp,n) % 2^b+1  
// needs (tp,2n) temp space , everything reduced mod 2^b 
// inputs,outputs are fully reduced
// NOTE: 2n is not the same as 2b rounded up to nearest limb
inline static int
mpn_mulmod_2expp1_internal (mp_ptr xp, mp_srcptr yp, mp_srcptr zp,
			    unsigned long b, mp_ptr tp)
{
  mp_size_t n, k;
  mp_limb_t c;

  n = BITS_TO_LIMBS (b);
  k = GMP_NUMB_BITS * n - b;
  ASSERT (b > 0);
  ASSERT (n > 0);
  ASSERT_MPN (yp, n);
  ASSERT_MPN (zp, n);
  ASSERT (!MPN_OVERLAP_P (tp, 2 * n, yp, n));
  ASSERT (!MPN_OVERLAP_P (tp, 2 * n, zp, n));
  ASSERT (!MPN_OVERLAP_P (xp, n, yp, n));
  ASSERT (!MPN_OVERLAP_P (xp, n, zp, n));
  ASSERT (MPN_SAME_OR_SEPARATE_P (xp, tp, n));
  ASSERT (MPN_SAME_OR_SEPARATE_P (xp, tp + n, n));
  ASSERT (k == 0 || yp[n - 1] >> (GMP_NUMB_BITS - k) == 0);
  ASSERT (k == 0 || zp[n - 1] >> (GMP_NUMB_BITS - k) == 0);
#if 0 && GMP_NAIL_BITS == 0
// mpn_mul_fft dont do nails
// fft has changed cant use this like this , but can use it HOW?
  if (k == 0 && n % 8 == 0)
    {
      count_trailing_zeros (c, n);
      if (c > 15)
	c = 15;
      for (c = c; c > 2; c--)
	if (n >= tab[c])
	  return mpn_mul_fft (xp, n, yp, n, zp, n, c);
    }
#endif
  mpn_mul_n (tp, yp, zp, n);
  if (k == 0)
    {
      c = mpn_sub_n (xp, tp, tp + n, n);
      return mpn_add_1 (xp, xp, n, c);
    }
  c = tp[n - 1];
  tp[n - 1] &= GMP_NUMB_MASK >> k;
#if HAVE_NATIVE_mpn_sublsh_nc
  c = mpn_sublsh_nc (xp, tp, tp + n, n, k, c);
#else
  {
    mp_limb_t c1;
    c1 = mpn_lshift (tp + n, tp + n, n, k);
    tp[n] |= c >> (GMP_NUMB_BITS - k);
    c = mpn_sub_n (xp, tp, tp + n, n) + c1;
  }
#endif
  c = mpn_add_1 (xp, xp, n, c);
  xp[n - 1] &= GMP_NUMB_MASK >> k;
  return c;
}

// c is the top bits of the inputs, must be fully reduced
int
mpn_mulmod_2expp1 (mp_ptr xp, mp_srcptr yp, mp_srcptr zp, int c,
		   unsigned long b, mp_ptr tp)
{
  int cy, cz;
  mp_size_t n, k;

  cy = c & 2;
  cz = c & 1;
  n = BITS_TO_LIMBS (b);
  k = GMP_NUMB_BITS * n - b;
  ASSERT (b > 0);
  ASSERT (n > 0);
  ASSERT_MPN (yp, n);
  ASSERT_MPN (zp, n);
  ASSERT (!MPN_OVERLAP_P (tp, 2 * n, yp, n));
  ASSERT (!MPN_OVERLAP_P (tp, 2 * n, zp, n));
  ASSERT (MPN_SAME_OR_SEPARATE_P (xp, tp, n));
  ASSERT (MPN_SAME_OR_SEPARATE_P (xp, tp + n, n));
  ASSERT (k == 0 || yp[n - 1] >> (GMP_NUMB_BITS - k) == 0);
  ASSERT (k == 0 || zp[n - 1] >> (GMP_NUMB_BITS - k) == 0);
#if WANT_ASSERT
  mp_size_t t;
  t = n;
  MPN_NORMALIZE (yp, t);
  ASSERT (cy == 0 || t == 0);
  t = n;
  MPN_NORMALIZE (zp, t);
  ASSERT (cz == 0 || t == 0);
#endif
  if (LIKELY (cy == 0))
    {
      if (LIKELY (cz == 0))
	{
	  c = mpn_mulmod_2expp1_internal (xp, yp, zp, b, tp);
	}
      else
	{
	  c = mpn_neg_n (xp, yp, n);
	  c = mpn_add_1 (xp, xp, n, c);
	  xp[n - 1] &= GMP_NUMB_MASK >> k;
	}
    }
  else
    {
      if (LIKELY (cz == 0))
	{
	  c = mpn_neg_n (xp, zp, n);
	  c = mpn_add_1 (xp, xp, n, c);
	  xp[n - 1] &= GMP_NUMB_MASK >> k;
	}
      else
	{
	  c = 0;
	  xp[0] = 1;
	  MPN_ZERO (xp + 1, n - 1);
	}
    }
  return c;
}
