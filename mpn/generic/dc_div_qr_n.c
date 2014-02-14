/* mpn_dc_div_qr_n -- recursive divide-and-conquer division for arbitrary
   size operands.

   Contributed to the GNU project by Torbjorn Granlund.

   THE FUNCTIONS IN THIS FILE ARE INTERNAL WITH MUTABLE INTERFACES.  IT IS ONLY
   SAFE TO REACH THEM THROUGH DOCUMENTED INTERFACES.  IN FACT, IT IS ALMOST
   GUARANTEED THAT THEY WILL CHANGE OR DISAPPEAR IN A FUTURE GMP RELEASE.

Copyright 2006, 2007, 2009 Free Software Foundation, Inc.

Copyright 2010 William Hart (minor modifications)

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

mp_limb_t
mpn_dc_div_qr_n (mp_ptr qp, mp_ptr np, mp_srcptr dp, mp_size_t n,
		    mp_limb_t dinv, mp_ptr tp)
{
  mp_size_t lo, hi;
  mp_limb_t cy, qh, ql;

  lo = n >> 1; /* floor(n/2) */
  hi = n - lo;	/* ceil(n/2) */

  
  if (BELOW_THRESHOLD (hi, DC_DIV_QR_THRESHOLD))
    qh = mpn_sb_div_qr (qp + lo, np + 2 * lo, 2 * hi, dp + lo, hi, dinv);
  else
    qh = mpn_dc_div_qr_n (qp + lo, np + 2 * lo, dp + lo, hi, dinv, tp);

  mpn_mul (tp, qp + lo, hi, dp, lo);

  cy = mpn_sub_n (np + lo, np + lo, tp, n);
  if (qh != 0)
    cy += mpn_sub_n (np + n, np + n, dp, lo);

  while (cy != 0)
    {
      qh -= mpn_sub_1 (qp + lo, qp + lo, hi, 1);
      cy -= mpn_add_n (np + lo, np + lo, dp, n);
    }

  if (BELOW_THRESHOLD (lo, DC_DIV_QR_THRESHOLD))
    ql = mpn_sb_div_qr (qp, np + hi, 2 * lo, dp + hi, lo, dinv);
  else
    ql = mpn_dc_div_qr_n (qp, np + hi, dp + hi, lo, dinv, tp);

  mpn_mul (tp, dp, hi, qp, lo);

  cy = mpn_sub_n (np, np, tp, n);
  if (ql != 0)
    cy += mpn_sub_n (np + lo, np + lo, dp, hi);

  while (cy != 0)
    {
      mpn_sub_1 (qp, qp, lo, 1);
      cy -= mpn_add_n (np, np, dp, n);
    }

  return qh;
}

