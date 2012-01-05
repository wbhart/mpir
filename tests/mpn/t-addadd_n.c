/* Test mpn_addadd_n

dnl  Copyright 2009 Jason Moxham

dnl  This file is part of the MPIR Library.

dnl  The MPIR Library is free software; you can redistribute it and/or modify
dnl  it under the terms of the GNU Lesser General Public License as published
dnl  by the Free Software Foundation; either version 2.1 of the License, or (at
dnl  your option) any later version.

dnl  The MPIR Library is distributed in the hope that it will be useful, but
dnl  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
dnl  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
dnl  License for more details.

dnl  You should have received a copy of the GNU Lesser General Public License
dnl  along with the MPIR Library; see the file COPYING.LIB.  If not, write
dnl  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
dnl  Boston, MA 02110-1301, USA.
*/

#include <stdio.h>
#include <stdlib.h>
#include "mpir.h"
#include "gmp-impl.h"
#include "tests.h"

int
main (void)
{
  unsigned long n;
  gmp_randstate_t rands;
  int j, k, i, i1;
  mp_limb_t sp[10000], tp[10000], xp[10000], yp[10000], zp[10000];

  tests_start ();
  gmp_randinit_default(rands);

  for (i1 = 0; i1 < 2; i1++)
    {
      for (n = 1; n < 100; n++)
	{
	  for (j = 1; j < 5; j++)
	    {
	      if (i1 == 0)
		{
		  mpn_randomb (xp, rands, n);
		  mpn_randomb (yp, rands, n);
		  mpn_randomb (zp, rands, n);
		}
	      else
		{
		  mpn_rrandom (xp, rands, n);
		  mpn_rrandom (yp, rands, n);
		  mpn_rrandom (zp, rands, n);
		}
	      k = mpn_addadd_n (sp, xp, yp, zp, n);
	      i = mpn_add_n (tp, xp, yp, n);
	      i += mpn_add_n (tp, tp, zp, n);
	      if (k != i)
		{
		  printf ("mpn_addadd_n ret wrong\n");
		  abort ();
		}
	      if (mpn_cmp (sp, tp, n) != 0)
		{
		  printf ("mpn_addadd_n sum wrong\n");
		  abort ();
		}
	    }
	}
    }
  gmp_randclear(rands);
  tests_end ();
  exit (0);
}
