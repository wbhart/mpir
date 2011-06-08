/* 
Test mpz_set_ux and mpz_init_set_ux (not much use but perhaps better than nothing)

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
#include "config.h"
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif
#include "mpir.h"
#include "gmp-impl.h"
#include "tests.h"

#define NLIMBS ((8 * SIZEOF_UINTMAX_T + GMP_NUMB_BITS  - 1) / GMP_NUMB_BITS)

//#ifdef HAVE_STDINT_H
#ifdef HAVE_UINTMAX_T
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
#endif

void
check_data (void)
{   unsigned int i;
    mpz_t   z;

#ifdef HAVE_UINTMAX_T
    mpz_init(z);

    for( i = 0 ; i < sizeof(val) / sizeof(uintmax_t) ; ++i )
    {
        uintmax_t k = 0, n = val[i];
        mpz_set_ux(z, val[i]);
#if NLIMBS == 1
        if(n && n != z->_mp_d[k++])
        {
            printf("mpz_set_ux() failed for data on item %d\n", i);
            abort();
        }
#else
        while(n)
        {
            if((n & GMP_NUMB_MASK) != z->_mp_d[k++])
            {
                printf("mpz_set_ux() failed for data on item %d\n", i);
                abort();
            }
            n >>= GMP_NUMB_BITS;
        }
#endif
        if(z->_mp_size != k)
        {
            printf("mpz_set_ux() failed for length on item %d (mpz size: %d, size: %d)\n", i, z->_mp_size, k);
            abort();
        }
        if(z->_mp_alloc < k)
        {
            printf("mpz_set_ux() failed for allocation on item %d (mpz alloc: %d, size: %d)\n", i, z->_mp_alloc, k);
            abort();
        }
    }
    mpz_clear(z);

    for( i = 0 ; i < sizeof(val) / sizeof(uintmax_t) ; ++i )
    {
        uintmax_t k = 0, n = val[i];
        mpz_init_set_ux(z, val[i]);
#if NLIMBS == 1
        if(n && n != z->_mp_d[k++])
        {
            printf("mpz_init_set_ux() failed for data on item %d\n", i);
            abort();
        }
#else
        while(n)
        {
            if((n & GMP_NUMB_MASK) != z->_mp_d[k++])
            {
                printf("mpz_init_set_ux() failed for data on item %d\n", i);
                abort();
            }
            n >>= GMP_NUMB_BITS;
        }
#endif
        if(z->_mp_size != k)
        {
            printf("mpz_init_set_ux() failed for length on item %d (mpz size: %d, size: %d)\n", i, z->_mp_size, k);
            abort();
        }
        if(z->_mp_alloc < k)
        {
            printf("mpz_init_set_ux() failed for allocation on item %d (mpz alloc: %d, size: %d)\n", i, z->_mp_alloc, k);
            abort();
        }
        mpz_clear(z);
    }
#endif
}

int
main (void)
{
  tests_start ();

  check_data ();

  tests_end ();
  exit (0);
}

//#endif
