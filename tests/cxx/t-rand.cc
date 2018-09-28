/* Test gmp_randclass.

Copyright 2002, 2003 Free Software Foundation, Inc.

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

#include "mpir.h"
#include "mpirxx.h"
#include "gmp-impl.h"
#include "tests.h"

using namespace std;


/* all flavours of initialization */
void
check_randinit (void)
{
  {
    gmp_randclass r(gmp_randinit_default);
  }

  {
    mpz_class a(0);
    mpir_ui c = 0;
    mp_bitcnt_t m2exp = 8;
    gmp_randclass r(gmp_randinit_lc_2exp, a, c, m2exp);
  }

  {
    mpir_ui m2exp = 64;
    gmp_randclass r(gmp_randinit_lc_2exp_size, m2exp);
  }

  /* gmp_randinit_lc_2exp_size, with excessive size */
  {
    try {
      mpir_ui m2exp = ULONG_MAX;
      gmp_randclass r(gmp_randinit_lc_2exp_size, m2exp);
      ASSERT_ALWAYS (0);  /* should not be reached */
    } catch (length_error) {
    }
  }

  {
    gmp_randclass r(gmp_randinit_mt);
  }

  /* obsolete, but still available */
  {
    gmp_randalg_t alg = GMP_RAND_ALG_LC;
    mpir_ui m2exp = 64;
    gmp_randclass r(alg, m2exp);
  }
  {
    gmp_randalg_t alg = GMP_RAND_ALG_DEFAULT;
    mpir_ui m2exp = 64;
    gmp_randclass r(alg, m2exp);
  }
  {
    gmp_randalg_t alg = (gmp_randalg_t) 0;
    mpir_ui m2exp = 64;
    gmp_randclass r(alg, m2exp);
  }
}

void
check_mpz (void)
{
  {
    gmp_randclass r(gmp_randinit_default);
    mpz_class a(123);
    unsigned int b = 256;
    mpz_class c;
    r.seed(a);
    c = r.get_z_bits(b);
  }
  {
    gmp_randclass r(gmp_randinit_default);
    mpz_class a(256);
    mpir_ui b = 123;
    mpz_class c;
    r.seed(b);
    c = r.get_z_bits(a);
  }
  {
    gmp_randclass r(gmp_randinit_default);
    mpz_class a(123), b(256);
    mpz_class c;
    r.seed(a);
    c = r.get_z_range(b);
  }
}

void
check_mpf (void)
{
  {
    gmp_randclass r(gmp_randinit_default);
    mpz_class a(123);
    r.seed(a);
    mpf_class b;
    b = r.get_f();
  }
  {
    gmp_randclass r(gmp_randinit_default);
    int a = 123, b = 128;
    r.seed(a);
    mpf_class c;
    c = r.get_f(b);
  }
}

/*check that get_randstate_t really returns the randstate_t underlying a gmp_randclass*/
void
check_randstate_t(void)
{
  /*seed gmp_randclass r using its method, and seed gmp_randclass s using get_randstate_t
    if they both generate the same sequence, then gmp_randstate_t must be returning the
    underlying randstate_t of s */
  {
    gmp_randclass r(gmp_randinit_default);
    gmp_randclass s(gmp_randinit_default);
    __gmp_randstate_struct s_state = *s.get_randstate_t();

    r.seed(0xdeadbeef);
    gmp_randseed_ui(&s_state, 0xdeadbeef);

    bool res = true;
    for (int i = 0; i < 100; ++i)
    {
      mpz_class n1 = r.get_z_bits(32);
      mpz_class n2 = s.get_z_bits(32);
      ASSERT_ALWAYS (n1 == n2);
    }
  }
}


int
main (void)
{
  tests_start();

  check_randinit();
  check_mpz();
  check_mpf();
  check_randstate_t();

  tests_end();
  return 0;
}
