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

/*
   Returns smallest d such that  d|N, start <= d < stop, d != 1. If no such d exists 
   return 0. The name trial_division implies a method, perhaps call it 
   mpz_smallest_divisor.
   N must have no divisors < start.
*/
unsigned long mpz_trial_division(mpz_srcptr N, unsigned long start, unsigned long stop)
{
   unsigned long i, dd;

   /* ASSERT N has no divisors < start excluding 1 */

   ASSERT(mpz_cmp_ui(N, 0) != 0);
   ASSERT(mpz_cmp_ui(N, 1) != 0);
   ASSERT(mpz_cmp_si(N, -1) != 0);

   if (start <= 2 && 2 < stop && mpz_even_p(N))
      return 2;
   
   if (start <= 3 && 3 < stop && mpz_divisible_ui_p(N, 3))
      return 3;
   
   if (start < 5) 
      start = 5; /* dont be silly */
   
   if (start%2 == 0) 
      start+=1;
   
   if (start%3 == 0)
      start+=2;

   ASSERT(start%2 != 0);
   ASSERT(start%3 != 0);

   dd = 2;
   if (start%6 == 1)
      dd = 4;

   for (i = start; i < stop; i += dd, dd = 6 - dd)
      if (mpz_divisible_ui_p(N, i))
         return i;
 
   return 0;
}
