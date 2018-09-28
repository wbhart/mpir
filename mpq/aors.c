/* mpq_add, mpq_sub -- add or subtract rational numbers.

Copyright 1991, 1994, 1995, 1996, 1997, 2000, 2001, 2004, 2005 Free Software
Foundation, Inc.

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
#include "gmp-impl.h"


static void __gmpq_aors(REGPARM_3_1 (mpq_ptr w, mpq_srcptr x, mpq_srcptr y, void (*fun)(mpz_ptr, mpz_srcptr, mpz_srcptr))) REGPARM_ATTR (1);
#define mpq_aors(w,x,y,fun)  __gmpq_aors (REGPARM_3_1 (w, x, y, fun))

REGPARM_ATTR (1) static void
mpq_aors (mpq_ptr rop, mpq_srcptr op1, mpq_srcptr op2,
          void (*fun)(mpz_ptr, mpz_srcptr, mpz_srcptr))
{
  mpz_t gcd;
  mpz_t tmp1, tmp2;
  mp_size_t op1_num_size = ABS (op1->_mp_num._mp_size);
  mp_size_t op1_den_size =      op1->_mp_den._mp_size;
  mp_size_t op2_num_size = ABS (op2->_mp_num._mp_size);
  mp_size_t op2_den_size =      op2->_mp_den._mp_size;
  TMP_DECL;

  TMP_MARK;
  MPZ_TMP_INIT (gcd, MIN (op1_den_size, op2_den_size));
  MPZ_TMP_INIT (tmp1, op1_num_size + op2_den_size);
  MPZ_TMP_INIT (tmp2, op2_num_size + op1_den_size);

  /* ROP might be identical to either operand, so don't store the
     result there until we are finished with the input operands.  We
     dare to overwrite the numerator of ROP when we are finished
     with the numerators of OP1 and OP2.  */

  mpz_gcd (gcd, &(op1->_mp_den), &(op2->_mp_den));
  if (! MPZ_EQUAL_1_P (gcd))
    {
      mpz_t t;

      mpz_divexact_gcd (tmp1, &(op2->_mp_den), gcd);
      mpz_mul (tmp1, &(op1->_mp_num), tmp1);

      mpz_divexact_gcd (tmp2, &(op1->_mp_den), gcd);
      mpz_mul (tmp2, &(op2->_mp_num), tmp2);

      MPZ_TMP_INIT (t, MAX (ABS (tmp1->_mp_size), ABS (tmp2->_mp_size)) + 1);

      (*fun) (t, tmp1, tmp2);
      mpz_divexact_gcd (tmp2, &(op1->_mp_den), gcd);

      mpz_gcd (gcd, t, gcd);
      if (MPZ_EQUAL_1_P (gcd))
        {
          mpz_set (&(rop->_mp_num), t);
          mpz_mul (&(rop->_mp_den), &(op2->_mp_den), tmp2);
        }
      else
        {
          mpz_divexact_gcd (&(rop->_mp_num), t, gcd);
          mpz_divexact_gcd (tmp1, &(op2->_mp_den), gcd);
          mpz_mul (&(rop->_mp_den), tmp1, tmp2);
        }
    }
  else
    {
      /* The common divisor is 1.  This is the case (for random input) with
	 probability 6/(pi**2), which is about 60.8%.  */
      mpz_mul (tmp1, &(op1->_mp_num), &(op2->_mp_den));
      mpz_mul (tmp2, &(op2->_mp_num), &(op1->_mp_den));
      (*fun) (&(rop->_mp_num), tmp1, tmp2);
      mpz_mul (&(rop->_mp_den), &(op1->_mp_den), &(op2->_mp_den));
    }
  TMP_FREE;
}


void
mpq_add (mpq_ptr rop, mpq_srcptr op1, mpq_srcptr op2)
{
  mpq_aors (rop, op1, op2, mpz_add);
}

void
mpq_sub (mpq_ptr rop, mpq_srcptr op1, mpq_srcptr op2)
{
  mpq_aors (rop, op1, op2, mpz_sub);
}
