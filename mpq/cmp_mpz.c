/* mpq_cmp(u,v) -- Compare U, V.  Return positive, zero, or negative
   based on if U > V, U == V, or U < V.

Copyright 2015 Free Software Foundation, Inc.

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
#include "longlong.h"

int
mpq_cmp_mpz(mpq_srcptr op, mpz_srcptr v)
{
    mp_size_t num_size = op->_mp_num._mp_size;
    mp_size_t den_size = op->_mp_den._mp_size;
    mp_size_t v_size    = v->_mp_size;
    mp_size_t tmp_size;
    mp_size_t num_sign;
    mp_ptr tmp_ptr;
    int cc;
    TMP_DECL;

    ASSERT (den_size > 0);

    /* 0. Check signs */
    if (num_size == 0)
        return -v_size;
    if (v_size == 0)
        return num_size;
    if ((num_size ^ v_size) < 0)         /* different signs */
        return num_size;
    if (!mpz_cmp_ui(mpq_denref(op), 1))  /* denominator 1 */
        return mpz_cmp(mpq_numref(op), v);

    /* 1. Check to see if we can tell which operand is larger by just looking at the number of limbs */
    num_sign = num_size;
    num_size = ABS(num_size);
    v_size   = ABS(v_size);
    tmp_size = v_size + den_size;

    /* den*v is either size tmp_size or tmp_size-1 */
    if(num_size > tmp_size)
        return num_sign;
    if(tmp_size > num_size + 1)
        return -num_sign;

    /* 2 . Same, but compare the number of significant bits. */
    {
      int cnt1, cnt2;
      unsigned long int bits1, bits2;

      count_leading_zeros(cnt1, op->_mp_num._mp_d[num_size-1]);
      bits1 = num_size * GMP_NUMB_BITS - cnt1 + GMP_NAIL_BITS;

      count_leading_zeros(cnt1, v->_mp_d[v_size-1]);
      count_leading_zeros(cnt2, op->_mp_den._mp_d[den_size-1]);
      bits2 = tmp_size * GMP_NUMB_BITS - cnt1 - cnt2 + 2*GMP_NAIL_BITS;

      if (bits1 > bits2 + 1)
        return num_sign;
      if (bits2 > bits1 + 1)
        return -num_sign;
    }

    /* 3. Finally, cross multiply and compare. */
    TMP_MARK;
    tmp_ptr = (mp_ptr) TMP_ALLOC(num_size * BYTES_PER_MP_LIMB);

    if( v_size >= den_size)
      tmp_size -= 0 == mpn_mul(tmp_ptr,
          v->_mp_d, v_size,
          op->_mp_den._mp_d, den_size);
    else
      tmp_size -= 0 == mpn_mul(tmp_ptr,
          op->_mp_den._mp_d, den_size,
          v->_mp_d, v_size);
    cc = num_size - tmp_size != 0
      ? num_size - tmp_size : mpn_cmp(op->_mp_num._mp_d, tmp_ptr, tmp_size);
    TMP_FREE;
    return num_sign < 0 ? -cc: cc;
}
