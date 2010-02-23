/* Test mpn_inv_div_qr_n.

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
#include "longlong.h"
#include "tests.h"

#if defined(_MSC_VER) || defined(__MINGW32__)
#define random rand
#endif

#define MAX_LIMBS 400
#define ITERS 10000
   
/* Check precomputed inverse division routine. */
void
check_inv_div_qr_n (void)
{
   mp_limb_t np[2*MAX_LIMBS];
   mp_limb_t np2[2*MAX_LIMBS];
   mp_limb_t rp[2*MAX_LIMBS+1];
   mp_limb_t dp[MAX_LIMBS];
   mp_limb_t qp[2*MAX_LIMBS];
   mp_limb_t inv[MAX_LIMBS];
   mp_limb_t cy;

   mp_size_t rn, dn, qn;

   gmp_randstate_t rands;

   int i, j, s;
   gmp_randinit_default(rands);
  
   for (i = 0; i < ITERS; i++)
   {
      dn = (random() % (MAX_LIMBS - 5)) + 6;
      
      mpn_rrandom (np, rands, 2*dn);
      mpn_rrandom (dp, rands, dn);
      dp[dn-1] |= GMP_LIMB_HIGHBIT;

      MPN_COPY(np2, np, 2*dn);
      
      mpn_invert(inv, dp, dn);
      
      qn = dn + 1;
         
      qp[qn - 1] = mpn_inv_div_qr_n(qp, np, dp, dn, inv);

      MPN_NORMALIZE(qp, qn);

      if (qn)
      {
         if (qn >= dn) mpn_mul(rp, qp, qn, dp, dn);
         else mpn_mul(rp, dp, dn, qp, qn);
      
         rn = dn + qn;
         MPN_NORMALIZE(rp, rn);
         
         if (rn > 2*dn)
         {
            printf("failed: q*d has too many limbs\n");
            abort();
         }
         
         if (mpn_cmp(rp, np2, 2*dn) > 0)
         {
            printf("failed: remainder negative\n");
            abort();
         }
         
         mpn_sub(rp, np2, 2*dn, rp, rn);
         rn = 2*dn;
         MPN_NORMALIZE(rp, rn);
      } else
      {
         rn = 2*dn;
         MPN_COPY(rp, np, 2*dn);
      }
      
      s = (rn < dn) ? -1 : (rn > dn) ? 1 : mpn_cmp(rp, dp, dn);
      if (s >= 0)
      {
         printf ("failed:\n");
         printf ("dn = %lu, qn = %lu, rn = %lu\n\n", dn, qn, rn);
         gmp_printf (" np: %Nx\n\n", np2, 2*dn);
         gmp_printf (" dp: %Nx\n\n", dp, dn);
         gmp_printf (" qp: %Nx\n\n", qp, qn);
         gmp_printf (" rp: %Nx\n\n", rp, rn);
         abort ();
      }

      if (mpn_cmp(rp, np, rn) != 0)
      {
         printf("failed: remainder does not match\n");
         gmp_printf (" np: %Nx\n\n", np2, 2*dn);
         gmp_printf (" dp: %Nx\n\n", dp, dn);
         gmp_printf (" qp: %Nx\n\n", qp, qn);
         gmp_printf (" rp: %Nx\n\n", rp, rn);        
         gmp_printf (" rp2: %Nx\n\n", np, rn);        
      }
   }

   gmp_randclear(rands);
}

int
main (void)
{
  tests_start ();

  check_inv_div_qr_n ();
  
  tests_end ();
  exit (0);
}
