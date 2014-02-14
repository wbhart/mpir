/* mpn_mullow_n -- multiply two n-limb nunbers and return the low n limbs
   of their products.
            
 Copyright 2004, 2005 Free Software Foundation, Inc.
 Copyright 2009 Jason Moxham
           
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

 /*  Copyright 2008 Complete rewrite  Only the name is the same
    Note: sets 2n limbs 

  */

#include "mpir.h"
#include "gmp-impl.h"

void
mpn_mullow_n (mp_ptr rp, mp_srcptr xp, mp_srcptr yp, mp_size_t n)
{
  mp_size_t m;

  ASSERT (n > 0);
  ASSERT_MPN (xp, n);
  ASSERT_MPN (yp, n);
  ASSERT (!MPN_OVERLAP_P (rp, 2 * n, xp, n));
  ASSERT (!MPN_OVERLAP_P (rp, 2 * n, yp, n));

  if (BELOW_THRESHOLD (n, MULLOW_BASECASE_THRESHOLD))
    {
      mpn_mul_basecase (rp, xp, n, yp, n);
      return;
    }

  if (BELOW_THRESHOLD (n, MULLOW_DC_THRESHOLD))
    {
      mpn_mullow_n_basecase (rp, xp, yp, n);
      return;
    }
  
  if (ABOVE_THRESHOLD (n, MULLOW_MUL_THRESHOLD))
    {
      mpn_mul_n (rp, xp, yp, n);
      return;
    }

  /* choose optimal m st n/2 <= m <= n, choosing m == n is same as above */
  m = n * 87 / 128;

  if (2 * m < n)
    m = n - n / 2;
  
  if (m > n)
    m = n;
  
  ASSERT (n / 2 <= m);
  ASSERT (m <= n);
  
  mpn_mul_n(rp, xp, yp, m);
  mpn_mullow_n(rp + 2 * m, xp, yp + m, n - m);
  mpn_add_n(rp + m, rp + m, rp + 2 * m, n - m);
  mpn_mullow_n(rp + 2 * m, xp + m, yp, n - m);
  mpn_add_n(rp + m, rp + m, rp + 2 * m, n - m);
  
  return;
}
