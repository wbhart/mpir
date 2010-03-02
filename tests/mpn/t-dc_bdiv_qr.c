/* Test mpn_dc_bdiv_qr.

Copyright 2002 Free Software Foundation, Inc.
Copyright 2009, 2010 William Hart

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
#include "longlong.h"
#include "tests.h"

#if defined(_MSC_VER) || defined(__MINGW32__)
#define random rand
#endif

#define MAX_LIMBS 400
#define ITERS 10000
   
/* Check divide and conquer hensel division routine. */
void
check_dc_bdiv_qr (void)
{
   mp_limb_t np[2*MAX_LIMBS];
   mp_limb_t np2[2*MAX_LIMBS];
   mp_limb_t rp[2*MAX_LIMBS];
   mp_limb_t dp[2*MAX_LIMBS];
   mp_limb_t qp[2*MAX_LIMBS];
   mp_limb_t dip, cy1, cy2;

   mp_size_t nn, rn, dn, qn;

   gmp_randstate_t rands;

   int i, j, s;
   gmp_randinit_default(rands);
  
   for (i = 0; i < ITERS; i++)
   {
      dn = (random() % (MAX_LIMBS - 2)) + 3;
      nn = (random() % (MAX_LIMBS - 1)) + dn + 1;
      qn = nn - dn;

      mpn_rrandom (np, rands, nn);
      mpn_rrandom (dp, rands, dn);
      
      dp[0] |= 1;
     
      MPN_COPY(np2, np, nn);
      
      modlimb_invert(dip, dp[0]);
      
      cy1 = mpn_dc_bdiv_qr(qp, np, nn, dp, dn, dip);
      
      if (qn)
      {
         if (qn >= dn) mpn_mul(rp, qp, qn, dp, dn);
         else mpn_mul(rp, dp, dn, qp, qn);
      } else
         MPN_ZERO(rp, nn);
      
      cy2 = mpn_sub_n(rp, np2, rp, nn);
      
      if (mpn_cmp(rp + qn, np + qn, dn) != 0)
      { 
         printf("failed: quotient wrong!\n");
         printf ("nn = %lu, dn = %lu, qn = %lu\n\n", nn, dn, qn);
         gmp_printf (" np: %Nx\n\n", np2, nn);
         gmp_printf (" dp: %Nx\n\n", dp, dn);
         gmp_printf (" qp: %Nx\n\n", qp, qn);
         gmp_printf (" rp: %Nx\n\n", rp, qn);
         abort ();
      }

      if (cy1 != cy2)
      {
         printf("failed: carry wrong!\n");
         printf("cy1 = %lx, cy2 = %lx\n", cy1, cy2);
      }
   }

   gmp_randclear(rands);
}

int
main (void)
{
  tests_start ();

  check_dc_bdiv_qr ();
  
  tests_end ();
  exit (0);
}
