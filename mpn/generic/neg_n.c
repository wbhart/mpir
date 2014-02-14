/* mpn_neg_n

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

/* 
   macro this or use asm if we can get the loop running at same speed as a 
   com_n, then we only make one pass over the data.
*/

mp_limb_t
mpn_neg_n(mp_ptr rp, mp_srcptr up, mp_size_t n)
{
   mpn_com_n(rp, up, n);

   return 1 - mpn_add_1(rp, rp, n, 1);
}
