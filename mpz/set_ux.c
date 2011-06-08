/* 
mpz_set_ux(z, v) -- Set z to the uintmax_t value v

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

#include "config.h"
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif
#include "mpir.h"
#include "gmp-impl.h"

#ifdef HAVE_STDINT_H

#define NLIMBS ((8 * SIZEOF_UINTMAX_T + GMP_NUMB_BITS  - 1) / GMP_NUMB_BITS)

void
mpz_set_ux (mpz_ptr z, uintmax_t v)
{   uintmax_t i, uv;

#if NLIMBS == 1
    z->_mp_d[0] = (mp_limb_t)v;
    z->_mp_size = (v ? NLIMBS : 0);
#else
    if(NLIMBS > z->_mp_alloc)
        MPZ_REALLOC(z, NLIMBS);
    for( i = 0, uv = v ; i < NLIMBS && uv ; ++i )
    {
        z->_mp_d[i] = uv & GMP_NUMB_MASK;
        uv >>= GMP_NUMB_BITS;
    }
    z->_mp_size = (v ? i : 0);
#endif
}

#endif
