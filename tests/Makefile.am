## Process this file with automake to generate Makefile.in

# Copyright 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.
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


SUBDIRS = . devel mpn mpz mpq mpf rand misc cxx

include ../mpn/Makeasm.am

INCLUDES = -I$(top_srcdir)
LDADD = libtests.la $(top_builddir)/libmpir.la

check_LTLIBRARIES = libtests.la

EXTRA_libtests_la_SOURCES = x86_64call.as x86_64check.c x86call.asm x86check.c
libtests_la_SOURCES = tests.h  memory.c misc.c refmpf.c refmpn.c refmpq.c refmpz.c spinner.c trace.c
libtests_la_DEPENDENCIES = @CALLING_CONVENTIONS_OBJS@
libtests_la_LIBADD = $(libtests_la_DEPENDENCIES) $(top_builddir)/libmpir.la $(LIBM)

check_PROGRAMS = t-bswap t-constants t-count_zeros t-gmpmax t-hightomask t-modlinv t-parity t-popc t-sub 
TESTS = $(check_PROGRAMS)
