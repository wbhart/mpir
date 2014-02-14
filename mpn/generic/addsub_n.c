/*   Copyright 2012 The Code cavern

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

/* t = x + y - z */
int mpn_addsub_n(mp_ptr t, mp_srcptr x, mp_srcptr y, mp_srcptr z, mp_size_t n)
{
   mp_limb_t ret;

   ASSERT(n > 0);
   ASSERT_MPN(x, n);
   ASSERT_MPN(y, n);
   ASSERT_MPN(z, n);
   ASSERT(MPN_SAME_OR_SEPARATE_P(t, x, n));
   ASSERT(MPN_SAME_OR_SEPARATE_P(t, y, n));
   ASSERT(MPN_SAME_OR_SEPARATE_P(t, z, n));

   if (t == x && t == y && t == z)
      return 0;

   if (t == x && t == y)
   {
      ret = mpn_add_n(t, x, y, n);
      ret -= mpn_sub_n(t, t, z, n);
   
      return ret;
   }

   if (t == x && t == z)
   {
      ret = -mpn_sub_n(t, x, z, n);
      ret += mpn_add_n(t, t, y, n);
   
      return ret;
   }

   if (t == y && t == z)
   {
      ret = -mpn_sub_n(t, y, z, n);
      ret += mpn_add_n(t, t, x, n);
   
      return ret;
   }

   if (t == x)
   {
      ret = mpn_add_n(t, x, y, n);
      ret -= mpn_sub_n(t, t, z, n);
   
      return ret;
   }

   if (t == y)
   {
      ret = mpn_add_n(t, y, x, n);
      ret -= mpn_sub_n(t, t, z, n);
    
      return ret;
   }

   if (t == z)
   {
      ret = -mpn_sub_n(t, x, z, n);
      ret += mpn_add_n(t, t, y, n);
   
      return ret;
   }

   ret = mpn_add_n(t, x, y, n);
   ret -= mpn_sub_n(t, t, z, n);

   return ret;
}
