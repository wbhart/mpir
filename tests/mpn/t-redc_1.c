
/* Test mpn_redc_1

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

#include <stdio.h>
#include <stdlib.h>
#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"
#include "tests.h"

/* Set cp[] <- tp[]/R^n mod mp[].  Clobber tp[].
   mp[] is n limbs; tp[] is 2n limbs.  */
void
ref_redc_1 (mp_ptr cp, mp_ptr tp, mp_srcptr mp, mp_size_t n, mp_limb_t Nprim)
{
    mp_limb_t cy;
    mp_limb_t q;
    mp_size_t j;

    ASSERT_MPN (tp, 2 * n);
    for (j = 0; j < n; j++)
    {
        q = (tp[0] * Nprim) & GMP_NUMB_MASK;
        tp[0] = mpn_addmul_1 (tp, mp, n, q);
        tp++;
    }
    cy = mpn_add_n (cp, tp, tp - n, n);
    if (cy != 0)
        mpn_sub_n (cp, cp, mp, n);
}

int
main (void)
{
    gmp_randstate_t rands;
    int j, n;
    mp_limb_t cp1[1000], cp2[1000], mp[1000], tp1[1000], tp2[1000], inv;

    tests_start ();
    gmp_randinit_default (rands);

    for (n = 1; n < 100; n++)
    {
        for (j = 1; j < 100; j++)
        {
            mpn_randomb (mp, rands, n);
            mp[0] |= 1;
            modlimb_invert (inv, mp[0]);
            inv = -inv;
            mpn_randomb (tp1, rands, 2 * n);
            MPN_COPY (tp2, tp1, 2 * n);
            ref_redc_1 (cp1, tp1, mp, n, inv);
            mpn_redc_1 (cp2, tp2, mp, n, inv);
            if (mpn_cmp (cp1, cp2, n) != 0)
            {
                printf ("mpn_redc_1 error %d\n", n);
                abort ();
            }
            if (n != 1 && mpn_cmp (tp1, tp2, 2 * n) != 0)
            {
                printf ("mpn_redc_1 possible error\n");
                abort ();
            }
            /* we dont require the above to be the same but it could be a useful test */
        }
    }
    for (n = 1; n < 100; n++)
    {
        for (j = 1; j < 100; j++)
        {
            mpn_rrandom (mp, rands, n);
            mp[0] |= 1;
            modlimb_invert (inv, mp[0]);
            inv = -inv;
            mpn_rrandom (tp1, rands, 2 * n);
            MPN_COPY (tp2, tp1, 2 * n);
            ref_redc_1 (cp1, tp1, mp, n, inv);
            mpn_redc_1 (cp2, tp2, mp, n, inv);
            if (mpn_cmp (cp1, cp2, n) != 0)
            {
                printf ("mpn_redc_1 error %d\n", n);
                abort ();
            }
            if (n != 1 && mpn_cmp (tp1, tp2, 2 * n) != 0)
            {
                printf ("mpn_redc_1 possible error\n");
                abort ();
            }
            /* we dont require the above to be the same but it could be a useful test */
        }
    }

    gmp_randclear (rands);
    tests_end ();
    exit (0);
}
