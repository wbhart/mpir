## Process this file with automake to generate Makefile.in
 
# Copyright 2000, 2001, 2002, 2003, 2005 Free Software Foundation, Inc.
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

INCLUDES = -I$(top_srcdir) -I$(top_srcdir)/tests

EXTRA_DIST =  README  divide.c  gexpr.c  mpirbench.sln  mpirver.c  multiply.c  rsa.c  runbench  runbench.py  timing.h  version
noinst_HEADERS = timing.h

# Prefer -static on the speed and tune programs, since that can avoid
# overheads of shared library linkages on some systems.  Libtool tends to
# botch -static if configured with --disable-static, perhaps reasonably
# enough.  In any event under --disable-static the only choice is a dynamic
# link so there's no point in -static.
#
if ENABLE_STATIC
STATIC = -static
else
STATIC =
endif

DEPENDENCIES = $(top_builddir)/libmpir.la -lm
LIBADD = $(libspeed_la_DEPENDENCIES) $(LIBM)
AM_LDFLAGS = $(STATIC)

$(top_builddir)/tests/libtests.la:
	cd $(top_builddir)/tests; $(MAKE) $(AM_MAKEFLAGS) libtests.la

EXTRA_PROGRAMS = gexpr rsa multiply divide mpirver

LDADD = $(DEPENDENCIES)

bench:
	./runbench ..

allprogs: $(EXTRA_PROGRAMS)
	  $(MAKE) $(AM_MAKEFLAGS) $(STATIC)
	
CLEANFILES = $(EXTRA_PROGRAMS) \
	RES-divide-8192,128	RES-divide-8388608,4194304	RES-multiply-8192,8192		\
	RES-divide-8192,32	RES-multiply-128,128		RES-rsa-1024	RES-rsa-512	\
	RES-divide-8192,4096	RES-multiply-131072,131072	RES-rsa-2048			\
	RES-divide-131072,65536	RES-divide-8192,64		RES-multiply-2097152,2097152	\
	RES-divide-131072,8192	RES-divide-8192,8064		RES-multiply-512,512
DISTCLEANFILES = $(EXTRA_PROGRAMS) \
	RES-divide-8192,128	RES-divide-8388608,4194304	RES-multiply-8192,8192		\
	RES-divide-8192,32	RES-multiply-128,128		RES-rsa-1024	RES-rsa-512	\
	RES-divide-8192,4096	RES-multiply-131072,131072	RES-rsa-2048			\
	RES-divide-131072,65536	RES-divide-8192,64		RES-multiply-2097152,2097152	\
	RES-divide-131072,8192	RES-divide-8192,8064		RES-multiply-512,512
