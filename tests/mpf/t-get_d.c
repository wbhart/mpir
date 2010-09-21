/* Test mpf_get_d and mpf_set_d.

   Copyright 1996, 1999, 2000, 2001 Free Software Foundation, Inc.

   This file is part of the GNU MP Library.

   The GNU MP Library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation; either version 2.1 of the License, or (at your
   option) any later version.

   The GNU MP Library is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
   License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA. */

#include <stdio.h>
#include <stdlib.h>
#include "mpir.h"
#include "tests.h"

#if ! defined (LOW_BOUND)
#define LOW_BOUND 1e-300
#define HIGH_BOUND 1e300
#endif

int
main (int argc, char **argv)
{
  double d, e, r;
  mpf_t u, v;
  int sign;
  
  tests_start ();
  mpf_init (u);
  mpf_init (v);

  
  for(sign=-1;sign<=1;sign+=2)
    {mpf_set_d (u, LOW_BOUND);
     if(sign==-1)mpf_neg(u,u);
     for (d = 2.0 * LOW_BOUND; d < HIGH_BOUND; d *= 1.01)
       {
         mpf_set_d (v, d*sign);
         if ( ! mpf_cmp (v, u) == sign )
        	abort ();
         e = mpf_get_d (v);
         r = e/d;
         if(r<0)r=-r;
         if (r < 0.99999999999999 || r > 1.00000000000001)
	   {
   	     fprintf (stderr, "should be one ulp from 1: %.16f\n", r);
    	     abort ();
     	   }
         mpf_set (u, v);
       }
    }
  mpf_clear (u);
  mpf_clear (v);

  tests_end ();
  exit (0);
}
