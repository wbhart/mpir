/* mpn_redc_1

Copyright 1991, 1993, 1994, 1996, 1997, 2000, 2001, 2002, 2005 Free Software
Foundation, Inc.  Contributed by Paul Zimmermann.

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

/* Set cp[] <- tp[]/R^n mod mp[].  Clobber tp[].
   mp[] is n limbs; tp[] is 2n limbs.  */
void mpn_redc_1 (mp_ptr cp, mp_ptr tp, mp_srcptr mp, mp_size_t n, mp_limb_t Nprim)
{
  mp_limb_t cy;
  mp_limb_t q;
  mp_size_t j;

  ASSERT_MPN (tp, 2*n);

  for (j = 0; j < n; j++)
    {
      q = (tp[0] * Nprim) & GMP_NUMB_MASK;
      tp[0] = mpn_addmul_1 (tp, mp, n, q);
      tp++;
    }
  cy = mpn_add_n (cp, tp, tp - n, n);
  if (cy != 0)
    mpn_sub_n (cp, cp, mp, n);
}

