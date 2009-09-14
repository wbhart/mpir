/* Test mpn_mullow_basecase

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
#include "tests.h"

#define DISP(xp,xn)  do{int __t;printf("%s=",#xp);for(__t=(xn)-1;__t>=0;__t--)printf("%lX ",(xp)[__t]);printf("\n");}while(0)

int
main (void)
{
  unsigned long yn, xn, n, b, zn, c;
  mp_limb_t xp[1000], yp[1000], mp[1000], lp[1000];
  gmp_randstate_t rands;
  tests_start ();
  gmp_randinit_default(rands);

  for (xn = 1; xn <= 100; xn++)
    {
      for (yn = 1; yn <= xn; yn++)
	{
	  for (n = xn; n <= xn + yn; n++)
	    {
	      mpn_randomb (xp, rands ,xn);
	      mpn_randomb (yp, rands, yn);
	      mpn_mul (mp, xp, xn, yp, yn);
	      mpn_mullow_basecase (lp, xp, xn, yp, yn, n);
	      if (mpn_cmp (mp, lp, n) != 0)
		{
		  printf ("mpn_mullow_basecase error %ld\n", n);
		  abort ();
		}
	    }
	}
    }
  gmp_randclear(rands);
  tests_end ();
  exit (0);
}
