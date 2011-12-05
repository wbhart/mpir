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


INCLUDES = -I$(top_srcdir) -I$(top_srcdir)/tests
LDADD = $(top_builddir)/tests/libtests.la $(top_builddir)/libmpir.la

check_PROGRAMS = bit convert dive dive_ui io logic reuse t-addsub t-aorsmul t-bin t-cdiv_ui t-cmp t-cmp_d t-cmp_si t-cong t-cong_2exp t-div_2exp t-divis t-divis_2exp t-export t-fac_ui t-fdiv t-fdiv_ui t-fib_ui t-fits t-gcd t-gcd_ui t-get_d t-get_d_2exp t-get_si t-get_sx t-get_ux t-hamdist t-import t-inp_str t-io_raw t-jac t-lcm t-likely_prime_p t-lucnum_ui t-mul t-mul_i t-next_likely_prime t-oddeven t-perfpow t-perfsqr t-popcount t-pow t-powm t-powm_ui t-pprime_p t-root t-scan t-set_d t-set_f t-set_si t-set_str t-set_sx t-set_ux t-sizeinbase t-sqrtrem t-tdiv t-tdiv_ui t-trial_division 

if ENABLE_STATIC
if ENABLE_SHARED
check_PROGRAMS += st_hamdist st_popcount
st_hamdist_SOURCES = t-hamdist.c
st_hamdist_LDFLAGS = -static
st_popcount_SOURCES = t-popcount.c
st_popcount_LDFLAGS = -static
endif
endif

TESTS = $(check_PROGRAMS)

# Temporary files used by the tests.  Removed automatically if the tests
# pass, but ensure they're cleaned if they fail.
#
CLEANFILES = *.tmp

$(top_builddir)/tests/libtests.la:
	cd $(top_builddir)/tests; $(MAKE) $(AM_MAKEFLAGS) libtests.la
