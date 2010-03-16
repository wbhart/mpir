/* mpn_sb_bdiv_q -- schoolbook Hensel division with precomputed inverse,
   returning quotient only.

   Contributed to the GNU project by Niels Möller.

   THE FUNCTIONS IN THIS FILE ARE INTERNAL FUNCTIONS WITH MUTABLE INTERFACES.
   IT IS ONLY SAFE TO REACH THEM THROUGH DOCUMENTED INTERFACES.  IN FACT, IT IS
   ALMOST GUARANTEED THAT THEY'LL CHANGE OR DISAPPEAR IN A FUTURE GMP RELEASE.

Copyright 2005, 2006, 2009 Free Software Foundation, Inc.
Copyright 2009 David Havey
Copyright 2010 William Hart

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


/* Computes Q = N / D mod B^nn, with an overflow W. Destroys N.

   D must be odd. dinv is D^-1 mod B.

   Note Q will be nn limbs.

   To compute Q we cancel one limb at a time, using

     qp[i] = D^{-1} * np[i] (mod B)
     N -= B^i * qp[i] * D

   The overflow is defined as follows: 
   Let X = sum(dp[i]*qp[j]*B^(i+j), 0 <= i < dn, 0 <= i+j < nn).
   Then X is nn + 2 limbs. The low nn limbs by definition agree with N. 
   The overflow is the remaining high two limbs.
*/

void
mpn_sb_bdiv_q (mp_ptr qp, mp_ptr wp,
		  mp_ptr np, mp_size_t nn,
		  mp_srcptr dp, mp_size_t dn,
		  mp_limb_t dinv)
{
  mp_size_t i;
  mp_limb_t cy, q, hi, w0, w1;

  ASSERT (dn > 0);
  ASSERT ((dp[0] & 1) != 0);
  ASSERT (((dinv * dp[0]) & GMP_NUMB_MASK) == 1);
  ASSERT (nn >= dn);

  w0 = 0;

  for (i = nn - dn; i > 0; i--)
    {
      q = dinv * np[0];
      qp[0] = q;
      qp++;
      cy = mpn_submul_1 (np, dp, dn, q);
      w0 += mpn_sub_1 (np + dn, np + dn, i, cy);
      ASSERT (np[0] == 0);
      np++;
    }

  w1 = 0;

  for (i = dn; i > 0; i--)
    {
      q = dinv * np[0];
      qp[0] = q;
      qp++;
      hi = mpn_submul_1 (np, dp, i, q);
      ADDC_LIMB(hi, w0, w0, hi);
      w1 += hi;
      ASSERT (np[0] == 0);
      np++;
    }

  wp[0] = w0;
  wp[1] = w1;
}
