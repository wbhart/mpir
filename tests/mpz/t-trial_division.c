/* Exercise mpz_trial_division

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

unsigned long trial_divref(mpz_t n,unsigned long start,unsigned long stop)
{unsigned long i;

if(start<=1)start=2;
for(i=start;i<stop;i++)if(mpz_divisible_ui_p(n,i))return i;
return 0;}

int
main (void)
{long i,d1,d2,start,stop;int correct;
  mpz_t n;
  tests_start ();

  mpz_init (n);
  for(i=2;i<300;i++)
     {mpz_set_ui(n,i);
      for(start=0;start<i+10;start++)
         {correct=1;
          if(trial_divref(n,0,start)!=0)correct=0;// n has divisor <start so answer is not fully correct
          for(stop=0;stop<i+10;stop++)
             {d1=mpz_trial_division(n,start,stop);              
              if(correct)
                {d2=trial_divref(n,start,stop);
                 if(d1!=d2){printf("trial div mismatch %lu %lu %lu %lu %ld\n",d1,d2,start,stop,i);abort();}}
              else
                {if(d1!=0 && !mpz_divisible_ui_p(n,d1)){printf("trial div divisor does not divide %lu %lu %lu %ld\n",d1,start,stop,i);abort();}
                }
             }
         }
     }
  for(i=2;i<300;i++)
     {mpz_set_si(n,-i);
      for(start=0;start<i+10;start++)
         {correct=1;
          if(trial_divref(n,0,start)!=0)correct=0;// n has divisor <start so answer is not fully correct
          for(stop=0;stop<i+10;stop++)
             {d1=mpz_trial_division(n,start,stop);              
              if(correct)
                {d2=trial_divref(n,start,stop);
                 if(d1!=d2){printf("trial div mismatch %lu %lu %lu %lu %ld\n",d1,d2,start,stop,i);abort();}}
              else
                {if(d1!=0 && !mpz_divisible_ui_p(n,d1)){printf("trial div divisor does not divide %lu %lu %lu %ld\n",d1,start,stop,i);abort();}
                }
             }
         }
     }
  
  
  
  
  mpz_clear (n);
  tests_end ();
  exit (0);
}
