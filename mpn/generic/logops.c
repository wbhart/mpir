/* mpn_and_n, mpn_ior_n, etc -- mpn logical operations.

Copyright 2009 Free Software Foundation, Inc.

Copyright 2010 Brian Gladman

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

void
__MPN(and_n)(mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_size_t n)
{
  mpn_and_n (rp, up, vp, n);
}

void
__MPN(andn_n)(mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_size_t n)
{
  mpn_andn_n (rp, up, vp, n);
}

void
__MPN(nand_n)(mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_size_t n)
{
  mpn_nand_n (rp, up, vp, n);
}

void
__MPN(ior_n)(mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_size_t n)
{
  mpn_ior_n (rp, up, vp, n);
}

void
__MPN(iorn_n)(mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_size_t n)
{
  mpn_iorn_n (rp, up, vp, n);
}

void
__MPN(nior_n)(mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_size_t n)
{
  mpn_nior_n (rp, up, vp, n);
}

void
__MPN(xor_n)(mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_size_t n)
{
  mpn_xor_n (rp, up, vp, n);
}

void
__MPN(xnor_n)(mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_size_t n)
{
  mpn_xnor_n (rp, up, vp, n);
}

