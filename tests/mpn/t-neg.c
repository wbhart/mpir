/* Test mpn_neg_n

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
  int j, k, i, l, i1, k1, j1, i2, k2, j2, d;
  mp_limb_t xp[10000], yp[10000], zp[10000], cp[10000];

  tests_start ();
  gmp_randinit_default(rands);

  MPN_ZERO (zp, 10000);
  for (i1 = 0; i1 < 2; i1++)
    {
      for (n = 1; n < 1000; n++)
	{
	  if (mpn_neg_n (yp, zp, n) != 0)
	    {
	      printf ("mpn_neg_n wrong\n");
	      abort ();
	    }
	  if (mpn_cmp (yp, zp, n) != 0)
	    {
	      printf ("mpn_neg_n wrong\n");
	      abort ();
	    }
	  for (j = 1; j < 10; j++)
	    {
	      if (i1 == 0)
		{
		  mpn_randomb (xp,rands, n);
		}
	      else
		{
		  mpn_rrandom (xp, rands,n);
		}
	      k1 = mpn_sub_n (cp, zp, xp, n);
	      k2 = mpn_neg_n (yp, xp, n);
	      if (k1 != k2)
		{
		  printf ("mpn_neg_n wrong\n");
		  abort ();
		}
	      if (mpn_cmp (yp, cp, n) != 0)
		{
		  printf ("mpn_neg_n wrong\n");
		  abort ();
		}
	    }
	}
    }
  for (i1 = 0; i1 < 2; i1++)
    {
      for (n = 1; n < 1000; n++)
	{
	  MPN_COPY (yp, zp, n);
	  if (mpn_neg_n (yp, yp, n) != 0)
	    {
	      printf ("mpn_neg_n wrong\n");
	      abort ();
	    }
	  if (mpn_cmp (yp, zp, n) != 0)
	    {
	      printf ("mpn_neg_n wrong\n");
	      abort ();
	    }
	  for (j = 1; j < 10; j++)
	    {
	      if (i1 == 0)
		{
		  mpn_randomb (xp, rands,n);
		}
	      else
		{
		  mpn_rrandom (xp, rands,n);
		}
	      k1 = mpn_sub_n (cp, zp, xp, n);
	      k2 = mpn_neg_n (xp, xp, n);
	      if (k1 != k2)
		{
		  printf ("mpn_neg_n wrong\n");
		  abort ();
		}
	      if (mpn_cmp (xp, cp, n) != 0)
		{
		  printf ("mpn_neg_n wrong\n");
		  abort ();
		}
	    }
	}
    }

  gmp_randclear(rands);
  tests_end ();
  exit (0);
}
