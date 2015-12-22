## Process this file with automake to generate Makefile.in

# Copyright 1996, 1998, 1999, 2000, 2001, 2002, 2005 Free Software Foundation,
# Inc.
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


AM_CPPFLAGS = -D__GMP_WITHIN_GMP -I$(top_srcdir) \
  -DOPERATION_`echo $* | sed 's/_$$//'`

OFILES = @mpn_objects@

noinst_LTLIBRARIES = libmpn.la
nodist_libmpn_la_SOURCES = dummy1.c
libmpn_la_LIBADD = $(OFILES)
libmpn_la_DEPENDENCIES = $(OFILES)

TARG_DIST = alpha arm generic ia64 mips32 mips64 powerpc32 powerpc64 sparc32 sparc64 x86 x86_64

EXTRA_DIST = asm-defs.m4 cpp-ccas m4-ccas $(TARG_DIST)

# These are BUILT_SOURCES at the top-level, so normally they're built before
# recursing into this directory.
#
dummy1.c:
	cd ..; $(MAKE) $(AM_MAKEFLAGS) mpn/dummy1.c

include Makeasm.am
