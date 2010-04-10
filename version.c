/* gmp_version and mpir_version -- version numbers compiled into the library.

Copyright 1996, 1999, 2000, 2001 Free Software Foundation, Inc.

Copyright 2008 William Hart.

This file is part of the MPIR Library.

The MPIR Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The MPIR Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPIR Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA. */

#include "mpir.h"
#include "gmp-impl.h"

const char * const gmp_version = GMP_VERSION;
#if defined( _MSC_VER )
const char * const mpir_version = _MSC_MPIR_VERSION;
#else
const char * const mpir_version = VERSION;
#endif
