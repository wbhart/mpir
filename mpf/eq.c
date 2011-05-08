/* mpf_eq -- Compare two floats up to a specified bit #.

Copyright 1993, 1995, 1996, 2001, 2002 Free Software Foundation, Inc.
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

int
mpf_eq (mpf_srcptr u, mpf_srcptr v, mp_bitcnt_t n_bits)
{
  mp_srcptr up, vp;
  mp_size_t usize, vsize, size, i, cu, cv, n, k;
  mp_limb_t uval, vval;
  mp_exp_t uexp, vexp;

  uexp = u->_mp_exp;
  vexp = v->_mp_exp;

  usize = u->_mp_size;
  vsize = v->_mp_size;

  /* 1. Are the signs different?  */
  if ((usize ^ vsize) >= 0)
    {
      /* U and V are both non-negative or both negative.  */
      if (usize == 0)
	return vsize == 0;
      if (vsize == 0)
	return 0;

      /* Fall out.  */
    }
  else
    {
      /* Either U or V is negative, but not both.  */
      return 0;
    }

  /* U and V have the same sign and are both non-zero.  */

  /* 2. Are the exponents different?  */
  if (uexp > vexp)
    return 0;			/* ??? handle (uexp = vexp + 1)   */
  if (vexp > uexp)
    return 0;			/* ??? handle (vexp = uexp + 1)   */

  usize = ABS (usize);
  vsize = ABS (vsize);

  up = u->_mp_d;
  vp = v->_mp_d;

  count_leading_zeros (cu, up[usize - 1]);
  count_leading_zeros (cv, vp[vsize - 1]);
  if (cu != cv)
    return 0;
  n = BITS_TO_LIMBS (n_bits + cu);
//compair bottom limb
  k = n * GMP_NUMB_BITS - n_bits - cu;
  uval = vval = 0;
  if (usize - n >= 0)
    uval = up[usize - n];
  if (vsize - n >= 0)
    vval = vp[vsize - n];
  if ((uval >> k) != (vval >> k))
    return 0;
// compair rest of limbs
  for (i = usize - n + 1; i < usize ; i++)
    {
      uval = vval = 0;
      if (i >= 0)
	uval = up[i];
      if (i - usize + vsize >= 0)
	vval = vp[i - usize + vsize];
      if (uval != vval)
	return 0;
    }
  return 1;
}
