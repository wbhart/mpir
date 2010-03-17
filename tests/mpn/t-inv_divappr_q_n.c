/* Test mpn_inv_divappr_q_n.

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

#define MAX_LIMBS 2000
#define ITERS 500
   
/* Check divide and conquer division routine. */
void
check_inv_divappr_q_n (void)
{
   mp_limb_t np[2*MAX_LIMBS];
   mp_limb_t np2[2*MAX_LIMBS];
   mp_limb_t rp[2*MAX_LIMBS];
   mp_limb_t dp[MAX_LIMBS];
   mp_limb_t qp[MAX_LIMBS];
   mp_limb_t dip[MAX_LIMBS];

   mp_size_t nn, rn, dn, qn;

   gmp_randstate_t rands;

   int i, j, s;
   gmp_randinit_default(rands);
  
   for (i = 0; i < ITERS; i++)
   {
      dn = (random() % (MAX_LIMBS - 6)) + 6;
      nn = 2*dn;
         
      mpn_rrandom (np, rands, nn);
      mpn_rrandom (dp, rands, dn);
      dp[dn-1] |= GMP_LIMB_HIGHBIT;

      MPN_COPY(np2, np, nn);
      
      mpn_invert(dip, dp, dn);

      qn = nn - dn + 1;
         
      qp[qn - 1] = mpn_inv_divappr_q_n(qp, np, dp, dn, dip);
      
      MPN_NORMALIZE(qp, qn);

      if (qn)
      {
         if (qn >= dn) mpn_mul(rp, qp, qn, dp, dn);
         else mpn_mul(rp, dp, dn, qp, qn);

         rn = dn + qn;
         MPN_NORMALIZE(rp, rn);

         s = (rn < nn) ? -1 : (rn > nn) ? 1 : mpn_cmp(rp, np2, nn);
         if (s <= 0) 
         {
            mpn_sub(rp, np2, nn, rp, rn);
            rn = nn;
            MPN_NORMALIZE(rp, rn);
         } else 
         {
            mpn_sub(rp, rp, rn, np2, nn);
            MPN_NORMALIZE(rp, rn);
         }
      } else
      {
         rn = nn;
         MPN_COPY(rp, np, nn);
      }
      
      s = (rn < dn) ? -1 : (rn > dn) ? 1 : mpn_cmp(rp, dp, dn);
      if (s >= 0)
      {
         printf ("failed:\n");
         printf ("nn = %lu, dn = %lu, qn = %lu, rn = %lu\n\n", nn, dn, qn, rn);
         gmp_printf (" np: %Nx\n\n", np2, nn);
         gmp_printf (" dp: %Nx\n\n", dp, dn);
         gmp_printf (" qp: %Nx\n\n", qp, qn);
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

  check_inv_divappr_q_n ();
  
  tests_end ();
  exit (0);
}
