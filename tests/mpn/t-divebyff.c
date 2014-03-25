/* Test mpn_divexact_byff

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
  unsigned long n, c;
  gmp_randstate_t rands;
  mp_limb_t xp[10000], qp[10000], tp[10000], r1, r2;
  tests_start ();
  gmp_randinit_default(rands);

  /*
     where (xp,n) = (qp,n)*(B-1) -ret*B^n and 0 <= ret < B-1      
     B=2^GMP_NUMB_BITS. This assumes we use a divexact algorithm, 
     a bi-directional algorithm would give different results.
  */
  for (n = 1; n < 100; n++)
    {
      for (c = 0; c < 10; c++)
	{
	  mpn_randomb (xp, rands, n);
	  r1 = mpn_divexact_byff (qp, xp, n);
	  r2 = mpn_mul_1 (tp, qp, n, GMP_NUMB_MAX);
	  if (r1 != r2)
	    {
	      printf ("mpn_divexact_byff ret error\n");
	      abort ();
	    }
	  if (mpn_cmp (xp, tp, n) != 0)
	    {
	      printf ("mpn_divexact_byff error\n");
	      abort ();
	    }
	}
    }

  for (n = 2; n < 100; n++)
    {
      for (c = 0; c < 10; c++)
	{
	  mpn_randomb (xp, rands, n);
	  xp[n] = mpn_mul_1 (xp, xp, n - 1, GMP_NUMB_MAX);
	  r1 = mpn_divexact_byff (qp, xp, n);
	  r2 = mpn_mul_1 (tp, qp, n, GMP_NUMB_MAX);
	  if (r1 != r2)
	    {
	      printf ("mpn_divexact_byff ret error\n");
	      abort ();
	    }
	  if (mpn_cmp (xp, tp, n) != 0)
	    {
	      printf ("mpn_divexact_byff error\n");
	      abort ();
	    }
	}
    }

  for (n = 0; n < 100; n++)
    {
      umul_ppmm (xp[1], xp[0], GMP_NUMB_MAX, n);
      r1 = mpn_divexact_byff (qp, xp, 2);
      r2 = mpn_mul_1 (tp, qp, 2, GMP_NUMB_MAX);
      if (r1 != r2)
	{
	  printf ("mpn_divexact_byff ret error\n");
	  abort ();
	}
      if (mpn_cmp (xp, tp, 2) != 0)
	{
	  printf ("mpn_divexact_byff error\n");
	  abort ();
	}
    }
  gmp_randclear(rands);
  tests_end ();
  exit (0);
}
