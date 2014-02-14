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

mp_limb_t mpn_addadd_n(mp_ptr t, mp_srcptr x, mp_srcptr y, mp_srcptr z, mp_size_t n)
{
   mp_limb_t ret;
   mp_srcptr a = x, b = y, c = z;

   ASSERT(n > 0);
   ASSERT_MPN(x, n);
   ASSERT_MPN(y, n);
   ASSERT_MPN(z, n);
   ASSERT(MPN_SAME_OR_SEPARATE_P(t, x, n));
   ASSERT(MPN_SAME_OR_SEPARATE_P(t, y, n));
   ASSERT(MPN_SAME_OR_SEPARATE_P(t, z, n));

   if(t == x)
   {
      if (t == y)
      {
         if (t == z)
         {
#ifdef HAVE_NATIVE_mpn_addlsh1_n
            return mpn_addlsh1_n(t, x, y, n);
#else
            return mpn_mul_1(t, x, n, 3);
#endif  
         }
      }
      else
      {
         MP_SRCPTR_SWAP(b, c);
      }
   }
   else
   {
      MP_SRCPTR_SWAP(a, c);
      
      if (t == y)
         MP_SRCPTR_SWAP(a, b);
   }

   ret = mpn_add_n(t, a, b, n);
   
   return ret + mpn_add_n(t, t, c, n);
}
