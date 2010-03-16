/* Test mpn_dc_bdiv_q_n.

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
check_dc_bdiv_q_n (void)
{
   mp_limb_t np[2*MAX_LIMBS];
   mp_limb_t np2[2*MAX_LIMBS];
   mp_limb_t rp[3*MAX_LIMBS];
   mp_limb_t dp[MAX_LIMBS];
   mp_limb_t qp[2*MAX_LIMBS];
   mp_limb_t wp[2];
   mp_limb_t tp[DC_BDIV_Q_N_ITCH(MAX_LIMBS)];
   mp_limb_t dip, cy, hi, lo;

   mp_size_t nn, rn, dn, qn;

   gmp_randstate_t rands;

   int i, j, s;
   gmp_randinit_default(rands);
  
   for (i = 0; i < ITERS; i++)
   {
      dn = (random() % (MAX_LIMBS - 5)) + 6;
      nn = dn;

      mpn_rrandom (np, rands, nn);
      mpn_rrandom (dp, rands, dn);
      
      dp[0] |= 1;
     
      MPN_COPY(np2, np, nn);
      
      modlimb_invert(dip, dp[0]);
 
      mpn_dc_bdiv_q_n(qp, wp, np, dp, dn, dip, tp);
      
      if (nn >= dn) mpn_mul(rp, qp, nn, dp, dn);
      else mpn_mul(rp, dp, dn, qp, nn);

      if (mpn_cmp(rp, np2, nn) != 0)
      { 
         printf("failed: quotient wrong!\n");
         printf ("nn = %lu, dn = %lu\n\n", nn, dn);
         gmp_printf (" np: %Nx\n\n", np2, nn);
         gmp_printf (" dp: %Nx\n\n", dp, dn);
         gmp_printf (" qp: %Nx\n\n", qp, nn);
         gmp_printf (" rp: %Nx\n\n", rp, nn);
         abort ();
      }

      MPN_ZERO(rp, 3*MAX_LIMBS);
      hi = lo = 0;
      for (j = 0; j < dn; j++)
      {
         cy = mpn_addmul_1(rp + j, qp, nn - j, dp[j]);
         add_ssaaaa(hi, lo, hi, lo, 0, cy);
      } 
      if ((hi != wp[1]) || (lo != wp[0]))
      {
         printf("failed: wp wrong!\n");
         printf("wp = %lx %lx, wp2 = %lx %lx\n", wp[1], wp[0], hi, lo);
         abort();
      } 
   }

   gmp_randclear(rands);
}

int
main (void)
{
  tests_start ();

  check_dc_bdiv_q_n ();
  
  tests_end ();
  exit (0);
}
