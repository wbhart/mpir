/* Test mpn_tdiv_q.

Copyright 2002 Free Software Foundation, Inc.
Copyright 2009 William Hart

This file is part of the MPIR Library.

The MPIR Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The MPIR Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPIR Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA. */

#include <stdio.h>
#include <stdlib.h>

#include "mpir.h"
#include "gmp-impl.h"
#include "tests.h"

#if defined(_MSC_VER) || defined(__MINGW32__)
#define random rand
#endif

#define MAX_LIMBS 200
#define ITERS 1000
   
/* Check division routine. */
void
check_tdiv_q (void)
{
   mp_limb_t np[2*MAX_LIMBS];
   mp_limb_t rp[2*MAX_LIMBS];
   mp_limb_t dp[MAX_LIMBS];
   mp_limb_t qp[2*MAX_LIMBS];
   mp_limb_t qp2[2*MAX_LIMBS];
   mp_limb_t dip[2];

   mp_size_t nn, rn, dn, qn, qn2;

   gmp_randstate_t rands;

   int i, j, s;
   gmp_randinit_default(rands);
  
   for (i = 0; i < ITERS; i++)
   {
      dn = (random() % (MAX_LIMBS - 1)) + 1;
      nn = (random() % MAX_LIMBS) + dn;
         
      mpn_rrandom (np, rands, nn);
      mpn_rrandom (dp, rands, dn);
      
      qn = nn - dn + 1;
      qn2 = nn - dn + 1;
      rn = dn;
   
      mpn_tdiv_q(qp, np, nn, dp, dn);
      mpn_tdiv_qr(qp2, rp, 0, np, nn, dp, dn);
      
      MPN_NORMALIZE(qp, qn);
      MPN_NORMALIZE(qp2, qn2);
      MPN_NORMALIZE(rp, rn);

      if ((qn != qn2) || (mpn_cmp(qp, qp2, qn) != 0))
      {
         printf ("failed:\n");
         printf ("nn = %lu, dn = %lu, qn = %lu, qn2 = %lu, rn = %lu\n\n", nn, dn, qn, qn2, rn);
         gmp_printf (" np: %Nx\n\n", np, nn);
         gmp_printf (" dp: %Nx\n\n", dp, dn);
         gmp_printf (" qp: %Nx\n\n", qp, qn);
         gmp_printf (" qp2: %Nx\n\n", qp2, qn2);
         gmp_printf (" rp: %Nx\n\n", rp, rn);
         abort ();
      }
   }

   gmp_randclear(rands);
}

int
main (void)
{
  tests_start ();

  check_tdiv_q ();
  
  tests_end ();
  exit (0);
}
