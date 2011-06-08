/* 
Test mpz_set_sx and mpz_init_set_sx

Copyright 2000, 2001, 2002 Free Software Foundation, Inc.

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

//#ifdef HAVE_STDINT_H

void
check_data (void)
{
#ifdef HAVE_INTMAX_T
  static const struct {
    intmax_t n;
    mp_size_t  want_size;
    mp_limb_t  want_data[2];
  } data[] = {

    {  0L,  0 },
    {  1L,  1, { 1 } },
    { -1L, -1, { 1 } },

#if GMP_NUMB_BITS >= BITS_PER_UINTMAX
    {  INTMAX_MAX,   1, { INTMAX_MAX, 0 } },
    { -INTMAX_MAX,  -1, { INTMAX_MAX, 0 } },
#else
    {  INTMAX_MAX,   2, { INTMAX_MAX & GMP_NUMB_MASK, INTMAX_MAX >> GMP_NUMB_BITS } },
    { -INTMAX_MAX,  -2, { INTMAX_MAX & GMP_NUMB_MASK, INTMAX_MAX >> GMP_NUMB_BITS } },
#endif

#if GMP_NUMB_BITS >= BITS_PER_UINTMAX
    { INTMAX_MIN, -1, { -INTMAX_MIN, 0 } },
#else
    { INTMAX_MIN,  -2, { -INTMAX_MIN & GMP_NUMB_MASK, -INTMAX_MIN >> GMP_NUMB_BITS } },
#endif
  };

  mpz_t  n;
  int    i;

  for (i = 0; i < numberof (data); i++)
    {
      mpz_init (n);
      mpz_set_sx (n, data[i].n);
      MPZ_CHECK_FORMAT (n);
      if (n->_mp_size != data[i].want_size
          || refmpn_cmp_allowzero (n->_mp_d, data[i].want_data,
                                   ABS (data[i].want_size)) != 0)
        {
          printf ("mpz_set_sx wrong on data[%d]\n", i);
          abort();
        }
      mpz_clear (n);

      mpz_init_set_sx (n, data[i].n);
      MPZ_CHECK_FORMAT (n);
      if (n->_mp_size != data[i].want_size
          || refmpn_cmp_allowzero (n->_mp_d, data[i].want_data,
                                   ABS (data[i].want_size)) != 0)
        {
          printf ("mpz_init_set_sx wrong on data[%d]\n", i);
          abort();
        }
      mpz_clear (n);
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
