/*
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

mp_limb_t mpn_rsh_divrem_hensel_qr_1(mp_ptr qp, mp_srcptr xp, 
                               mp_size_t n, mp_limb_t d, int s, mp_limb_t cin)
{
   ASSERT(n > 0);
   ASSERT(s >= 0);
   ASSERT_MPN(xp, n);
   ASSERT(MPN_SAME_OR_SEPARATE_P(qp, xp, n));
   ASSERT(d%2 == 1);

   if (BELOW_THRESHOLD(n, RSH_DIVREM_HENSEL_QR_1_THRESHOLD))
      return mpn_rsh_divrem_hensel_qr_1_1(qp, xp, n, d, s, cin);

   return mpn_rsh_divrem_hensel_qr_1_2(qp, xp, n, d, s, cin);
}
