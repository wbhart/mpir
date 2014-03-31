/* mpz_nextprime(x,y) - compute the next probable prime > y and store that in x

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


/* This function is Obsolete  17/8/2009 */

/* 
   But people use it anyway! 
   
   FIXME: This function should prove the primality of x using 
   ECPP or APR-CL.
*/
void mpz_nextprime(mpz_ptr x, mpz_srcptr y)
{
  gmp_randstate_t rnd;
  
  gmp_randinit_default(rnd);
  mpz_next_prime_candidate(x, y, rnd);
  
  if (mpz_cmp_ui(x, 1000000L) >= 0) /* nextprime_candidate sieves primes up to 1000 */
  {
     while (!mpz_miller_rabin (x, 23, rnd)) /* we've done 2 rounds already, do another 23 */
     {
        mpz_add_ui(x, x, 2);
        mpz_next_prime_candidate(x, x, rnd);
     }
  }

  gmp_randclear(rnd);
}
