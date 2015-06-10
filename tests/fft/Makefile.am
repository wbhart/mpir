## Process this file with automake to generate Makefile.in

# Copyright 1996, 1997, 1999, 2000, 2001, 2002, 2003 Free Software
# Foundation, Inc.
#
# Copyright 2008 Jason Moxham
#
# This file is part of the GNU MP Library.
#
# The GNU MP Library is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2.1 of the License, or (at your
# option) any later version.
#
# The GNU MP Library is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
# License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA 02110-1301, USA.


AM_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/tests
LDADD = $(top_builddir)/tests/libtests.la $(top_builddir)/libmpir.la

check_PROGRAMS = t-adjust t-adjust_sqrt2 t-butterfly t-butterfly_lshB t-butterfly_rshB t-butterfly_sqrt2 t-butterfly_twiddle t-div_2expmod_2expp1 t-fft_ifft_mfa_trunc_sqrt2 t-fft_ifft_negacyclic t-fft_ifft_radix2 t-fft_ifft_trunc t-fft_ifft_trunc_sqrt2 t-mul_2expmod_2expp1 t-mul_fft_main t-mul_mfa_trunc_sqrt2 t-mul_trunc_sqrt2 t-mulmod_2expp1 t-normmod_2expp1 t-split_combine_bits 

#if ENABLE_STATIC
#if ENABLE_SHARED
#check_PROGRAMS += st_hamdist st_popcount
#st_hamdist_SOURCES = t-hamdist.c
#st_hamdist_LDFLAGS = -static
#st_popcount_SOURCES = t-popcount.c
#st_popcount_LDFLAGS = -static
#endif
#endif

TESTS = $(check_PROGRAMS)

# Temporary files used by the tests.  Removed automatically if the tests
# pass, but ensure they're cleaned if they fail.
#
CLEANFILES = *.tmp

$(top_builddir)/tests/libtests.la:
	cd $(top_builddir)/tests; $(MAKE) $(AM_MAKEFLAGS) libtests.la
