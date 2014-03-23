/* mpz_fac_ui(RESULT, N) -- Set RESULT to N!.

Contributed to the GNU project by Marco Bodrato.

Copyright 1991, 1993, 1994, 1995, 2000, 2001, 2002, 2003, 2011, 2012
Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library.  If not, see http://www.gnu.org/licenses/.  */

#include "mpir.h"
#include "gmp-impl.h"

#if GMP_LIMB_BITS == 64

/* This table is 0!,1!,2!,3!,...,n! where n! has <= GMP_NUMB_BITS bits */
#define ONE_LIMB_FACTORIAL_TABLE CNST_LIMB(0x1),CNST_LIMB(0x1),CNST_LIMB(0x2),CNST_LIMB(0x6),CNST_LIMB(0x18),CNST_LIMB(0x78),CNST_LIMB(0x2d0),CNST_LIMB(0x13b0),CNST_LIMB(0x9d80),CNST_LIMB(0x58980),CNST_LIMB(0x375f00),CNST_LIMB(0x2611500),CNST_LIMB(0x1c8cfc00),CNST_LIMB(0x17328cc00),CNST_LIMB(0x144c3b2800),CNST_LIMB(0x13077775800),CNST_LIMB(0x130777758000),CNST_LIMB(0x1437eeecd8000),CNST_LIMB(0x16beecca730000),CNST_LIMB(0x1b02b9306890000),CNST_LIMB(0x21c3677c82b40000)

#define TABLE_LIMIT_2N_MINUS_POPC_2N 81

#else

/* This table is 0!,1!,2!,3!,...,n! where n! has <= GMP_NUMB_BITS bits */
#define ONE_LIMB_FACTORIAL_TABLE CNST_LIMB(0x1),CNST_LIMB(0x1),CNST_LIMB(0x2),CNST_LIMB(0x6),CNST_LIMB(0x18),CNST_LIMB(0x78),CNST_LIMB(0x2d0),CNST_LIMB(0x13b0),CNST_LIMB(0x9d80),CNST_LIMB(0x58980),CNST_LIMB(0x375f00),CNST_LIMB(0x2611500),CNST_LIMB(0x1c8cfc00)

#define TABLE_LIMIT_2N_MINUS_POPC_2N 49

#endif

#define FACTOR_LIST_STORE(P, PR, MAX_PR, VEC, I)		\
  do {								\
    if ((PR) > (MAX_PR)) {					\
      (VEC)[(I)++] = (PR);					\
      (PR) = (P);						\
    } else							\
      (PR) *= (P);						\
  } while (0)

#if TUNE_PROGRAM_BUILD
#define FACTORS_PER_LIMB (GMP_NUMB_BITS / (LOG2C(FAC_DSC_THRESHOLD_LIMIT-1)+1))
#else
#define FACTORS_PER_LIMB (GMP_NUMB_BITS / (LOG2C(FAC_ODD_THRESHOLD)+1))
#endif

/* Computes n!, the factorial of n.
   WARNING: it assumes that n fits in a limb!
 */
void
mpz_fac_ui (mpz_ptr x, mpir_ui n)
{
  static const mp_limb_t table[] = { ONE_LIMB_FACTORIAL_TABLE };

  ASSERT (n <= GMP_NUMB_MAX);

  if (n < numberof (table))
    {
      PTR (x)[0] = table[n];
      SIZ (x) = 1;
    }
  else if (BELOW_THRESHOLD (n, FAC_ODD_THRESHOLD))
    {
      mp_limb_t prod, max_prod;
      mp_size_t j;
      mp_ptr    factors;
      TMP_SDECL;

      TMP_SMARK;
      factors = TMP_SALLOC_LIMBS (2 + (n - numberof (table)) / FACTORS_PER_LIMB);

      factors[0] = table[numberof (table)-1];
      j = 1;
      prod = n;
#if TUNE_PROGRAM_BUILD
      max_prod = GMP_NUMB_MAX / FAC_DSC_THRESHOLD_LIMIT;
#else
      max_prod = GMP_NUMB_MAX / (FAC_ODD_THRESHOLD | 1);
#endif
      while (--n >= numberof (table))
	FACTOR_LIST_STORE (n, prod, max_prod, factors, j);

      factors[j++] = prod;
      mpz_prodlimbs (x, factors, j);

      TMP_SFREE;
    }
  else
    {
      mp_limb_t count;
      mpz_oddfac_1 (x, n, 0);
      if (n <= TABLE_LIMIT_2N_MINUS_POPC_2N)
	count = __gmp_fac2cnt_table[n / 2 - 1];
      else
	{
	  popc_limb (count, n);
	  count = n - count;
	}
      mpz_mul_2exp (x, x, count);
    }
}

#undef FACTORS_PER_LIMB
#undef FACTOR_LIST_STORE
