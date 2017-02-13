/* Compute {up,n}^(-1) mod B^n.

   Contributed to the GNU project by Torbjorn Granlund.

   THE FUNCTIONS IN THIS FILE ARE INTERNAL WITH MUTABLE INTERFACES.  IT IS ONLY
   SAFE TO REACH THEM THROUGH DOCUMENTED INTERFACES.  IN FACT, IT IS ALMOST
   GUARANTEED THAT THEY WILL CHANGE OR DISAPPEAR IN A FUTURE GMP RELEASE.

Copyright (C) 2004-2007, 2009, 2012 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of either:

  * the GNU Lesser General Public License as published by the Free
    Software Foundation; either version 3 of the License, or (at your
    option) any later version.

or

  * the GNU General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any
    later version.

or both in parallel, as here.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received copies of the GNU General Public License and the
GNU Lesser General Public License along with the GNU MP Library.  If not,
see https://www.gnu.org/licenses/.  */

#include "mpir.h"
#include "gmp-impl.h"


/*
  r[k+1] = r[k] - r[k] * (u*r[k] - 1)
  r[k+1] = r[k] + r[k] - r[k]*(u*r[k])
*/

#if TUNE_PROGRAM_BUILD
#define NPOWS \
 ((sizeof(mp_size_t) > 6 ? 48 : 8*sizeof(mp_size_t)))
#else
#define NPOWS \
 ((sizeof(mp_size_t) > 6 ? 48 : 8*sizeof(mp_size_t)) - LOG2C (BINV_NEWTON_THRESHOLD))
#endif

mp_size_t
mpn_binvert_itch (mp_size_t n)
{
  mp_size_t itch_local = mpn_mulmod_bnm1_next_size (n);
  mp_size_t itch_out = mpn_mulmod_bnm1_itch (itch_local, n, (n + 1) >> 1);
  return itch_local + itch_out;
}

void
mpn_binvert (mp_ptr rp, mp_srcptr up, mp_size_t n, mp_ptr scratch)
{
  mp_ptr xp;
  mp_size_t rn, newrn;
  mp_size_t sizes[NPOWS], *sizp;
  mp_limb_t di;

  /* Compute the computation precisions from highest to lowest, leaving the
     base case size in 'rn'.  */
  sizp = sizes;
  for (rn = n; ABOVE_THRESHOLD (rn, BINV_NEWTON_THRESHOLD); rn = (rn + 1) >> 1)
    *sizp++ = rn;

  xp = scratch;

  /* Compute a base value of rn limbs.  */
  MPN_ZERO (xp, rn);
  xp[0] = 1;
  /* JPF: GMP goes the other way around and has renamed to binvert_limb */
  modlimb_invert (di, up[0]);
  /* JPF: GMP vs MPIR diff; MPIR returns a two limbs overflow and use sub rather than add */
  if (BELOW_THRESHOLD (rn, DC_BDIV_Q_THRESHOLD))
    mpn_sb_bdiv_q (rp, xp+rn, xp, rn, up, rn, di);
  else
    mpn_dc_bdiv_q (rp, xp, rn, up, rn, di);

  /* Use Newton iterations to get the desired precision.  */
  if (rn == n)
    return;
  newrn = *--sizp;
  for (; newrn < n;)
    {
      mp_size_t m;

      /* X <- UR. */
      m = mpn_mulmod_bnm1_next_size (newrn);
      mpn_mulmod_bnm1 (xp, m, up, newrn, rp, rn, xp + m);
      mpn_sub_1 (xp + m, xp, rn - (m - newrn), 1);

      /* R = R(X/B^rn) */
      mpn_mullow_n (rp + rn, rp, xp + rn, newrn - rn);
      mpn_neg (rp + rn, rp + rn, newrn - rn);

      rn = newrn;
      newrn = *--sizp;
    }
  /* Last iteration would overflow in the mullow call */
    {
      mp_size_t m;

      /* X <- UR. */
      m = mpn_mulmod_bnm1_next_size (newrn);
      mpn_mulmod_bnm1 (xp, m, up, newrn, rp, rn, xp + m);
      mpn_sub_1 (xp + m, xp, rn - (m - newrn), 1);

      /* R = R(X/B^rn) */
      mpn_mullow_n (xp + newrn, rp, xp + rn, newrn - rn); /* JPF: would overflow */
      /* At most we need 2*(newrn - rn) limbs at xp + newrn, so need 3*newrn - 2*rn */
      /* As 2rn > newrn, and n == newrn, this gives at max 2*n for xp */
      /* which we already ensure */
      mpn_neg (rp + rn, xp + newrn, newrn - rn);
    }
}
