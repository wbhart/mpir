/* Test mpn_divexact_fobm1

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
#include "longlong.h"
#include "tests.h"

#define DISP(xp,xn)  do{int __t;printf("%s=",#xp);for(__t=(xn)-1;__t>=0;__t--)printf("%lX ",(xp)[__t]);printf("\n");}while(0)

int
main (void)
{
  unsigned long n, c,j;
  gmp_randstate_t rands;
  mp_limb_t xp[10000], qp[10000], tp[10000], r1, r2, i, f;
  tests_start ();
  gmp_randinit_default(rands);

  /* 
     where (xp,n) = (qp,n)*f -ret*B^n    and 0 <= ret < f      
     B=2^GMP_NUMB_BITS  where f divides B-1. This assumes 
     we use a divexact algorithm, a bi-directional algorithm 
     would give different results and so would a diveby3 type 
     div without the correction
  */

  for (i = 1; i < 10000; i += 2)
    {
      if (GMP_NUMB_MAX % i != 0)
	continue;
      f = i;
      for (j = 0; j < 2; j++)
	{
	  f = GMP_NUMB_MAX / f;
	  for (n = 1; n < 100; n++)
	    {
	      for (c = 0; c < 10; c++)
		{
		  mpn_randomb (xp, rands,n);
		  r1 = mpn_divexact_byfobm1 (qp, xp, n, f, GMP_NUMB_MAX / f);
		  r2 = mpn_mul_1 (tp, qp, n, f);
		  if (r1 != r2)
		    {
		      printf ("mpn_divexact_byfobm1 ret error\n");
		      abort ();
		    }
		  if (mpn_cmp (xp, tp, n) != 0)
		    {
		      printf ("mpn_divexact_byfobm1 error\n");
		      abort ();
		    }
		}
	    }

	  for (n = 2; n < 100; n++)
	    {
	      for (c = 0; c < 10; c++)
		{
		  mpn_randomb (xp, rands,n);
		  xp[n] = mpn_mul_1 (xp, xp, n - 1, f);
		  r1 = mpn_divexact_byfobm1 (qp, xp, n, f, GMP_NUMB_MAX / f);
		  r2 = mpn_mul_1 (tp, qp, n, f);
		  if (r1 != r2)
		    {
		      printf ("mpn_divexact_byfobm1 ret error\n");
		      abort ();
		    }
		  if (mpn_cmp (xp, tp, n) != 0)
		    {
		      printf ("mpn_divexact_byfobm1 error\n");
		      abort ();
		    }
		}
	    }

	  for (n = 0; n < 100; n++)
	    {
	      umul_ppmm (xp[1], xp[0], f, n);
	      r1 = mpn_divexact_byfobm1 (qp, xp, 2, f, GMP_NUMB_MAX / f);
	      r2 = mpn_mul_1 (tp, qp, 2, f);
	      if (r1 != r2)
		{
		  printf ("mpn_divexact_byfobm1 ret error\n");
		  abort ();
		}
	      if (mpn_cmp (xp, tp, 2) != 0)
		{
		  printf ("mpn_divexact_byfobm1 error\n");
		  abort ();
		}
	    }

	}
    }
  gmp_randclear(rands);
  tests_end ();
  exit (0);
}
