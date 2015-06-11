/* 
Test mpz_get_ux (not much use but perhaps better than nothing)

Copyright 2011, Brian Gladman

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

#if defined( _MSC_VER )
#  if _MSC_VER < 1600
#    define SKIP_TEST
#  else
#    include <stdint.h>
#  endif
#else
#  include "config.h"
#  ifdef HAVE_STDINT_H
#    include <stdint.h>
#  else
#    define SKIP_TEST
#  endif
#endif

#ifdef SKIP_TEST

int
main (void)
{
  printf ("(u)intmax_t not available - test skipped\n");
  exit (0);
}

#else

#include "mpir.h"
#include "gmp-impl.h"
#include "tests.h"

#define NLIMBS ((8 * SIZEOF_UINTMAX_T + GMP_NUMB_BITS  - 1) / GMP_NUMB_BITS)

static const uintmax_t val[] =
{
    0, 1, 0xff, 
#if SIZEOF_UINTMAX_T >= 2
    0x0100, 0x0101, 0xffff, 
#endif
#if SIZEOF_UINTMAX_T >= 4     
    0x10000, 0x10001, 0xffffffff, 
#endif
#if SIZEOF_UINTMAX_T >= 8
    0x100000000, 0x100000001, 0xffffffffffffffff,
#endif
    UINTMAX_MAX
};

void
check_data (void)
{   unsigned int i;
    mpz_t   z;

    mpz_init(z);

    for( i = 0 ; i < sizeof(val) / sizeof(uintmax_t) ; ++i )
    {
        uintmax_t k = 0, n;

        mpz_set_ux(z, val[i]);
        n = mpz_get_ux(z);
#if NLIMBS == 1
        if(n && n != z->_mp_d[k++])
        {
            printf("mpz_get_ux() failed for data on item %d\n", i);
            abort();
        }
#else
        while(n)
        {
            if((n & GMP_NUMB_MASK) != z->_mp_d[k++])
            {
                printf("mpz_get_ux() failed for data on item %d\n", i);
                abort();
            }
            n >>= GMP_NUMB_BITS;
        }
#endif
        if(z->_mp_size != k)
        {
            printf("mpz_get_ux() failed for length on item %u (mpz size: %d, size: %lu)\n", i, z->_mp_size, k);
            abort();
        }
        if(z->_mp_alloc < k)
        {
            printf("mpz_get_ux() failed for allocation on item %u (mpz alloc: %d, size: %lu)\n", i, z->_mp_alloc, k);
            abort();
        }
    }
    mpz_clear(z);
}

int
main (void)
{
  tests_start ();

  check_data ();

  tests_end ();
  exit (0);
}

#endif
