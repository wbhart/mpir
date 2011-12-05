## Process this file with automake to generate Makefile.in

# Copyright 2001, 2002, 2003 Free Software Foundation, Inc.
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

check_PROGRAMS = t-addadd_n t-addsub_n t-aors_1 t-asmtype t-dc_bdiv_q t-dc_bdiv_q_n t-dc_bdiv_qr t-dc_bdiv_qr_n t-dc_div_q t-dc_div_qr t-dc_div_qr_n t-dc_divappr_q t-dc_divappr_q_n t-divebyff t-divebyfobm1 t-divrem_1 t-fat t-gcdext t-get_d t-hgcd t-instrument t-inv_div_q t-inv_div_qr t-inv_div_qr_n t-inv_divappr_q t-inv_divappr_q_n t-invert t-iord_u t-lorrshift1 t-matrix22 t-mp_bases t-mullow_basecase t-mullowhigh t-mulmid t-mulmod_2expm1 t-mulmod_2expp1 t-neg t-perfsqr t-redc_1 t-sb_bdiv_q t-sb_bdiv_qr t-sb_div_q t-sb_div_qr t-sb_divappr_q t-scan t-subadd_n t-tdiv_q t-tdiv_qr 
  
if ENABLE_STATIC
if ENABLE_SHARED
check_PROGRAMS += st_fat st_instrument
st_fat_SOURCES = t-fat.c
st_fat_LDFLAGS = -static
st_instrument_SOURCES = t-instrument.c
st_instrument_LDFLAGS = -static
endif
endif

TESTS = $(check_PROGRAMS)

$(top_builddir)/tests/libtests.la:
	cd $(top_builddir)/tests; $(MAKE) $(AM_MAKEFLAGS) libtests.la
