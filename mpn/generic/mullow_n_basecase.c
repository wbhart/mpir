/* mpn_mullow_n_basecase -- Internal routine to multiply two natural
   numbers of length n and return the low part.

   THIS IS AN INTERNAL FUNCTION WITH A MUTABLE INTERFACE.  IT IS ONLY
   SAFE TO REACH THIS FUNCTION THROUGH DOCUMENTED INTERFACES.


Copyright (C) 2000, 2002, 2004 Free Software Foundation, Inc.

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

/*
  FIXME: Should this function also output the two limbs overflow?
*/

void
mpn_mullow_n_basecase (mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_size_t n)
{
  mp_size_t i;

  ASSERT (n >= 1);
  ASSERT (! MPN_OVERLAP_P (rp, 2*n, up, n));
  ASSERT (! MPN_OVERLAP_P (rp, 2*n, vp, n));

  /* We first multiply by the low order limb (or depending on optional function
     availability, limbs).  This result can be stored, not added, to rp.  We
     also avoid a loop for zeroing this way.  */

#if HAVE_NATIVE_mpn_mul_2
  if (n >= 2)
    {
      mpn_mul_2 (rp, up, n, vp);
      rp += 2, vp += 2, n -= 2;
    }
  else
    {
      mpn_mul_1 (rp, up, n, vp[0]);
      return;
    }
#else
  mpn_mul_1 (rp, up, n, vp[0]);
  rp += 1, vp += 1, n -= 1;
#endif

  /* Now accumulate the product of up[] and the next higher limb (or depending
     on optional function availability, limbs) from vp[].  */

#define MAX_LEFT MP_SIZE_T_MAX	/* Used to simplify loops into if statements */


#if HAVE_NATIVE_mpn_addmul_6
  while (n >= 6)
    {
      mpn_addmul_6 (rp, up, n, vp);
      if (MAX_LEFT == 6)
	return;
      rp += 6, vp += 6, n -= 6;
      if (MAX_LEFT < 2 * 6)
	break;
    }
#undef MAX_LEFT
#define MAX_LEFT (6 - 1)
#endif

#if HAVE_NATIVE_mpn_addmul_5
  while (n >= 5)
    {
      mpn_addmul_5 (rp, up, n, vp);
      if (MAX_LEFT == 5)
	return;
      rp += 5, vp += 5, n -= 5;
      if (MAX_LEFT < 2 * 5)
	break;
    }
#undef MAX_LEFT
#define MAX_LEFT (5 - 1)
#endif

#if HAVE_NATIVE_mpn_addmul_4
  while (n >= 4)
    {
      mpn_addmul_4 (rp, up, n, vp);
      if (MAX_LEFT == 4)
	return;
      rp += 4, vp += 4, n -= 4;
      if (MAX_LEFT < 2 * 4)
	break;
    }
#undef MAX_LEFT
#define MAX_LEFT (4 - 1)
#endif

#if HAVE_NATIVE_mpn_addmul_3
  while (n >= 3)
    {
      mpn_addmul_3 (rp, up, n, vp);
      if (MAX_LEFT == 3)
	return;
      rp += 3, vp += 3, n -= 3;
      if (MAX_LEFT < 2 * 3)
	break;
    }
#undef MAX_LEFT
#define MAX_LEFT (3 - 1)
#endif

#if HAVE_NATIVE_mpn_addmul_2
  while (n >= 2)
    {
      mpn_addmul_2 (rp, up, n, vp);
      if (MAX_LEFT == 2)
	return;
      rp += 2, vp += 2, n -= 2;
      if (MAX_LEFT < 2 * 2)
	break;
    }
#undef MAX_LEFT
#define MAX_LEFT (2 - 1)
#endif

  while (n >= 1)
    {
      mpn_addmul_1 (rp, up, n, vp[0]);
      if (MAX_LEFT == 1)
	return;
      rp += 1, vp += 1, n -= 1;
    }

}
