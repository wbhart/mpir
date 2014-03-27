/* Test mpz_mul_ui and mpz_mul_si.

Copyright 1999, 2000, 2001 Free Software Foundation, Inc.
Copyright 2011 Brian Gladman 

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

/* nextprime(10^pow10) = 10^pow10 + np_off  */

struct
{ 
    int pow10, np_off;
} tests1[] =
{  { 1, 1 }, { 2, 1 }, {3, 9}, {4, 7}, {5, 3}, {6, 3}, {10, 19}, {20, 39} };

/* count of primes 10^pow10 < prime[i] < 10^pow10 + 1000 and the sum of the
   squares of (prime[i] - 10^pow10) in this range */

struct
{ 
    int pow10, count, sumsq;
} tests2[] =
{  {4, 106, 34598882}, {5, 81, 26944273}, {6, 75, 24031603}, {10, 44, 16383852}, {20, 24, 8189464} };

int 
main (int argc, char **argv)
{
    char str[1000];
    gmp_randstate_t rnd; 
    mpz_t x, y, z;
    int i, j, k, s;

    tests_start ();
    gmp_randinit_default (rnd);
    mpz_init(x);
    mpz_init(y);
    mpz_init(z);
    for( i = 0 ; i < sizeof(tests1) / sizeof(tests1[0]) ; ++i )
    {
      mpz_ui_pow_ui(x, 10, tests1[i].pow10);
      mpz_next_prime_candidate(y, x, rnd);
      mpz_sub(y, y, x);
      j = mpz_get_ui(y);
      if(j != tests1[i].np_off)
      {
          printf("\nnext_likely_prime(10^%d) - 10^%d: expected %d but got %d", 
              tests1[i].pow10, tests1[i].pow10, tests1[i].np_off, j);
          abort();
      }
    }

    for( i = 0 ; i < sizeof(tests2) / sizeof(tests2[0]) ; ++i )
    {
      mpz_ui_pow_ui(x, 10, tests2[i].pow10);
      mpz_set(y, x);
      s = j = 0;
      for( ; ; )
      {
          mpz_next_prime_candidate(y, y, rnd);
          mpz_sub(z, y, x);
          k = mpz_get_si(z);
          if(k >= 1000)
              break;
          j++;
          s += k * k;
      }
      if(j != tests2[i].count || s != tests2[i].sumsq)
      {
          printf("\nnext_likely_prime failed test2[%d], expected %d and %d but got %d and %d", 
              i, tests2[i].count, tests2[i].sumsq, j, s);
          abort();
      }
    }
      
    gmp_randclear (rnd);
    mpz_clear(z);
    mpz_clear(y);
    mpz_clear(x);
    tests_end ();
    exit (0);
}
