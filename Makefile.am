# Process this file with automake to generate Makefile.in


# Copyright 1991, 1993, 1994, 1996, 1997, 1999, 2000, 2001, 2002, 2003, 2004,
# 2006 Free Software Foundation, Inc.
#
# Copyright 2008 William Hart, Michael Abshoff
#
# This file is part of the MPIR Library.
#
# The MPIR Library is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2.1 of the License, or (at your
# option) any later version.
#
# The MPIR Library is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
# License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with the MPIR Library; see the file COPYING.LIB.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA 02110-1301, USA.

# Libtool -version-info for libmpir.la and libmp.la.  See "Versioning" in the
# libtool manual.
#
# 1. No interfaces changed, only implementations (good): Increment REVISION.
#
# 2. Interfaces added, none removed (good): Increment CURRENT, increment
#    AGE, set REVISION to 0.
#
# 3. Interfaces removed (BAD, breaks upward compatibility): Increment
#    CURRENT, set AGE and REVISION to 0.
#
# Do this separately for libgmp, libgmpxx and libmp, and only for releases.
#
#	  GMP	   -version-info
#       release   libgmp  libgmpxx libmp
#        2.0.x      -        -       -
#        3.0      3:0:0      -     3:0:0
#        3.0.1    3:1:0      -     3:0:0
#        3.1      4:0:1      -     4:0:1
#        3.1.1    4:1:1      -     4:1:1
#        4.0      5:0:2    3:0:0   4:2:1
#        4.0.1    5:1:2    3:1:0   4:3:1
#        4.1      6:0:3    3:2:0   4:4:1
#        4.1.1    6:1:3    3:3:0   4:5:1
#        4.1.2    6:2:3    3:4:0   4:6:1
#        4.1.3    6:3:3    3:5:0   4:7:1
#        4.1.4    6:3:3    3:5:0   4:7:1	WRONG, same as 4.1.3!
#        4.2      6:0:3    3:2:0   4:4:1	REALLY WRONG, same as 4.1!
#        4.2.1    7:1:4    4:1:1   4:10:1
#
#	  MPIR	   -version-info
#       release   libgmp  libgmpxx libmp
#	0.9.0	7:1:4	4:1:1	4:10:1   WRONG, same as GMP 4.2.1
#	1.0.0	7:2:4	4:2:1	4:11:1
#	1.1.0	7:3:4	4:3:1	4:12:1
#	1.2.0	7:4:4	4:4:1	4:13:1
#	1.3.0	8:0:0	4:5:1	4:14:1 
#	1.3.1	8:1:0	4:6:1	0:0:0
#	2.0.0	9:2:0	4:7:1
#	2.1.0	10:0:1	4:8:1
#	2.1.1	10:1:2	4:9:2
#	2.1.2	10:2:2	4:10:2
#	2.1.3	10:3:2	4:11:2
#	2.1.4	10:4:2	4:12:2
#	2.2.0	10:5:2	4:13:2
#	2.2.1	10:6:3	4:14:3
#	2.3.0	10:7:3	4:15:3
#	2.3.1	10:8:3	4:16:3
#	2.4.0   11:0:4  4:17:3
#	2.5.0	11:1:4	4:18:3
#	2.5.1	11:2:4	4:19:3
#	2.5.2	11:3:4	4:20:3
#	2.6.0   11:0:5  4:21:3               Clearly wrong!!
#       2.7.0   16:0:6  8:0:4                Attempt to correct wrt all prev 
#       2.7.1   22:1:6  12:1:4               Due to trailing space for 2.7.1
#       2.7.2   22:2:6  12:2:4
#       3.0.0   23:3:0  12:3:4
#
# Starting at 3:0:0 is a slight abuse of the versioning system, but it
# ensures we're past soname libgmp.so.2, which was used on Debian GNU/Linux
# packages of gmp 2.  Pretend gmp 2 was 2:0:0, so the interface changes for
# gmp 3 mean 3:0:0 is right.
#
# We interpret "implementation changed" in item "1." above as meaning any
# release, ie. the REVISION is incremented every time (if nothing else).
# Even if we thought the code generated will be identical on all systems,
# it's still good to get the shared library filename (like
# libgmpxx.so.3.0.4) incrementing, to make it clear which GMP it's from.

LIBGMP_LT_CURRENT = 23
LIBGMP_LT_REVISION = 3
LIBGMP_LT_AGE = 0

LIBGMPXX_LT_CURRENT = 12
LIBGMPXX_LT_REVISION = 3
LIBGMPXX_LT_AGE = 4

SUBDIRS = tests

SUBDIRS += mpn fft mpz mpq mpf printf scanf cxx tune doc

EXTRA_DIST = configfsf.guess configfsf.sub .gdbinit INSTALL.autoconf

if WANT_CXX
GMPXX_HEADERS_OPTION = mpirxx.h
endif
EXTRA_DIST += mpirxx.h yasm_macwin.inc.fat yasm_macwin.inc.nofat yasm_mac.inc.fat yasm_mac.inc.nofat strip_fPIC.sh mpn/x86w mpn/x86_64w build.vc11 build.vc12 build.vc14 build.vc15 build.vc doc/devel doc/fdl.texi cpuid.c gpl-2.0.txt lgpl-2.1.txt

# mpir.h is architecture dependent, mainly since they encode the
# limb size used in libmpir.  For that reason they belong under $exec_prefix
# not $prefix, strictly speaking.
#
# $exec_prefix/include is not in the default include path for gcc built to
# the same $prefix and $exec_prefix, which might mean mpir.h is not found,
# but anyone knowledgable enough to be playing with exec_prefix will be able
# to address that.
#
includeexecdir = $(exec_prefix)/include
include_HEADERS = $(GMPXX_HEADERS_OPTION)
nodist_includeexec_HEADERS = mpir.h
lib_LTLIBRARIES = libmpir.la
if WANT_GMPCOMPAT
lib_LTLIBRARIES += libgmp.la
endif
lib_LTLIBRARIES += $(GMPXX_LTLIBRARIES_OPTION)

BUILT_SOURCES = mpir.h
if WANT_GMPCOMPAT
BUILT_SOURCES += gmp.h
nodist_includeexec_HEADERS += gmp.h
if WANT_CXX
BUILT_SOURCES += gmpxx.h
nodist_includeexec_HEADERS += gmpxx.h
endif
endif

DISTCLEANFILES = $(BUILT_SOURCES) config.m4 fat.h @gmp_srclinks@

# Tell mpir.h it's building gmp, not an application, used by windows DLL stuff.
AM_CPPFLAGS=-D__GMP_WITHIN_GMP

MPF_OBJECTS = mpf/abs$U.lo mpf/add$U.lo mpf/add_ui$U.lo mpf/ceilfloor$U.lo mpf/clear$U.lo mpf/clears$U.lo mpf/cmp$U.lo mpf/cmp_d$U.lo mpf/cmp_si$U.lo mpf/cmp_ui$U.lo mpf/div$U.lo mpf/div_2exp$U.lo mpf/div_ui$U.lo mpf/dump$U.lo mpf/eq$U.lo mpf/fits_si$U.lo mpf/fits_sint$U.lo mpf/fits_slong$U.lo mpf/fits_sshort$U.lo mpf/fits_ui$U.lo mpf/fits_uint$U.lo mpf/fits_ulong$U.lo mpf/fits_ushort$U.lo mpf/get_d$U.lo mpf/get_d_2exp$U.lo mpf/get_dfl_prec$U.lo mpf/get_prc$U.lo mpf/get_si$U.lo mpf/get_str$U.lo mpf/get_ui$U.lo mpf/init$U.lo mpf/init2$U.lo mpf/inits$U.lo mpf/inp_str$U.lo mpf/int_p$U.lo mpf/iset$U.lo mpf/iset_d$U.lo mpf/iset_si$U.lo mpf/iset_str$U.lo mpf/iset_ui$U.lo mpf/mul$U.lo mpf/mul_2exp$U.lo mpf/mul_ui$U.lo mpf/neg$U.lo mpf/out_str$U.lo mpf/pow_ui$U.lo mpf/random2$U.lo mpf/reldiff$U.lo mpf/rrandomb$U.lo mpf/set$U.lo mpf/set_d$U.lo mpf/set_dfl_prec$U.lo mpf/set_prc$U.lo mpf/set_prc_raw$U.lo mpf/set_q$U.lo mpf/set_si$U.lo mpf/set_str$U.lo mpf/set_ui$U.lo mpf/set_z$U.lo mpf/size$U.lo mpf/sqrt$U.lo mpf/sqrt_ui$U.lo mpf/sub$U.lo mpf/sub_ui$U.lo mpf/swap$U.lo mpf/trunc$U.lo mpf/ui_div$U.lo mpf/ui_sub$U.lo mpf/urandomb$U.lo 

FFT_OBJECTS = fft/adjust$U.lo fft/adjust_sqrt2$U.lo fft/butterfly_lshB$U.lo fft/butterfly_rshB$U.lo fft/combine_bits$U.lo fft/div_2expmod_2expp1$U.lo fft/fermat_to_mpz$U.lo fft/fft_mfa_trunc_sqrt2$U.lo fft/fft_mfa_trunc_sqrt2_inner$U.lo fft/fft_negacyclic$U.lo fft/fft_radix2$U.lo fft/fft_trunc$U.lo fft/fft_trunc_sqrt2$U.lo fft/ifft_mfa_trunc_sqrt2$U.lo fft/ifft_negacyclic$U.lo fft/ifft_radix2$U.lo fft/ifft_trunc$U.lo fft/ifft_trunc_sqrt2$U.lo fft/mul_2expmod_2expp1$U.lo fft/mul_fft_main$U.lo fft/mul_mfa_trunc_sqrt2$U.lo fft/mul_trunc_sqrt2$U.lo fft/mulmod_2expp1$U.lo fft/normmod_2expp1$U.lo fft/revbin$U.lo fft/split_bits$U.lo 

MPZ_OBJECTS = mpz/2fac_ui$U.lo mpz/abs$U.lo mpz/add$U.lo mpz/add_ui$U.lo mpz/and$U.lo mpz/aorsmul$U.lo mpz/aorsmul_i$U.lo mpz/array_init$U.lo mpz/bin_ui$U.lo mpz/bin_uiui$U.lo mpz/cdiv_q$U.lo mpz/cdiv_q_ui$U.lo mpz/cdiv_qr$U.lo mpz/cdiv_qr_ui$U.lo mpz/cdiv_r$U.lo mpz/cdiv_r_ui$U.lo mpz/cdiv_ui$U.lo mpz/cfdiv_q_2exp$U.lo mpz/cfdiv_r_2exp$U.lo mpz/clear$U.lo mpz/clears$U.lo mpz/clrbit$U.lo mpz/cmp$U.lo mpz/cmp_d$U.lo mpz/cmp_si$U.lo mpz/cmp_ui$U.lo mpz/cmpabs$U.lo mpz/cmpabs_d$U.lo mpz/cmpabs_ui$U.lo mpz/com$U.lo mpz/combit$U.lo mpz/cong$U.lo mpz/cong_2exp$U.lo mpz/cong_ui$U.lo mpz/dive_ui$U.lo mpz/divegcd$U.lo mpz/divexact$U.lo mpz/divis$U.lo mpz/divis_2exp$U.lo mpz/divis_ui$U.lo mpz/dump$U.lo mpz/export$U.lo mpz/fac_ui$U.lo mpz/fdiv_q$U.lo mpz/fdiv_q_ui$U.lo mpz/fdiv_qr$U.lo mpz/fdiv_qr_ui$U.lo mpz/fdiv_r$U.lo mpz/fdiv_r_ui$U.lo mpz/fdiv_ui$U.lo mpz/fib2_ui$U.lo mpz/fib_ui$U.lo mpz/fits_si$U.lo mpz/fits_sint$U.lo mpz/fits_slong$U.lo mpz/fits_sshort$U.lo mpz/fits_ui$U.lo mpz/fits_uint$U.lo mpz/fits_ulong$U.lo mpz/fits_ushort$U.lo mpz/gcd$U.lo mpz/gcd_ui$U.lo mpz/gcdext$U.lo mpz/get_d$U.lo mpz/get_d_2exp$U.lo mpz/get_si$U.lo mpz/get_str$U.lo mpz/get_sx$U.lo mpz/get_ui$U.lo mpz/get_ux$U.lo mpz/getlimbn$U.lo mpz/hamdist$U.lo mpz/import$U.lo mpz/init$U.lo mpz/init2$U.lo mpz/inits$U.lo mpz/inp_raw$U.lo mpz/inp_str$U.lo mpz/invert$U.lo mpz/ior$U.lo mpz/iset$U.lo mpz/iset_d$U.lo mpz/iset_si$U.lo mpz/iset_str$U.lo mpz/iset_sx$U.lo mpz/iset_ui$U.lo mpz/iset_ux$U.lo mpz/jacobi$U.lo mpz/kronsz$U.lo mpz/kronuz$U.lo mpz/kronzs$U.lo mpz/kronzu$U.lo mpz/lcm$U.lo mpz/lcm_ui$U.lo mpz/likely_prime_p$U.lo mpz/limbs_finish$U.lo mpz/limbs_modify$U.lo mpz/limbs_read$U.lo mpz/limbs_write$U.lo mpz/lucnum2_ui$U.lo mpz/lucnum_ui$U.lo mpz/mfac_uiui$U.lo mpz/miller_rabin$U.lo mpz/millerrabin$U.lo mpz/mod$U.lo mpz/mul$U.lo mpz/mul_2exp$U.lo mpz/mul_si$U.lo mpz/mul_ui$U.lo mpz/n_pow_ui$U.lo mpz/neg$U.lo mpz/next_prime_candidate$U.lo mpz/nextprime$U.lo mpz/nthroot$U.lo mpz/oddfac_1$U.lo mpz/out_raw$U.lo mpz/out_str$U.lo mpz/perfpow$U.lo mpz/perfsqr$U.lo mpz/popcount$U.lo mpz/pow_ui$U.lo mpz/powm$U.lo mpz/powm_ui$U.lo mpz/pprime_p$U.lo mpz/primorial_ui$U.lo mpz/probable_prime_p$U.lo mpz/prodlimbs$U.lo mpz/realloc$U.lo mpz/realloc2$U.lo mpz/remove$U.lo mpz/roinit_n$U.lo mpz/root$U.lo mpz/rootrem$U.lo mpz/rrandomb$U.lo mpz/scan0$U.lo mpz/scan1$U.lo mpz/set$U.lo mpz/set_d$U.lo mpz/set_f$U.lo mpz/set_q$U.lo mpz/set_si$U.lo mpz/set_str$U.lo mpz/set_sx$U.lo mpz/set_ui$U.lo mpz/set_ux$U.lo mpz/setbit$U.lo mpz/size$U.lo mpz/sizeinbase$U.lo mpz/sqrt$U.lo mpz/sqrtrem$U.lo mpz/sub$U.lo mpz/sub_ui$U.lo mpz/swap$U.lo mpz/tdiv_q$U.lo mpz/tdiv_q_2exp$U.lo mpz/tdiv_q_ui$U.lo mpz/tdiv_qr$U.lo mpz/tdiv_qr_ui$U.lo mpz/tdiv_r$U.lo mpz/tdiv_r_2exp$U.lo mpz/tdiv_r_ui$U.lo mpz/tdiv_ui$U.lo mpz/trial_division$U.lo mpz/tstbit$U.lo mpz/ui_pow_ui$U.lo mpz/ui_sub$U.lo mpz/urandomb$U.lo mpz/urandomm$U.lo mpz/xor$U.lo 

MPQ_OBJECTS = mpq/abs$U.lo mpq/aors$U.lo mpq/canonicalize$U.lo mpq/clear$U.lo mpq/clears$U.lo mpq/cmp$U.lo mpq/cmp_si$U.lo mpq/cmp_ui$U.lo mpq/div$U.lo mpq/equal$U.lo mpq/get_d$U.lo mpq/get_den$U.lo mpq/get_num$U.lo mpq/get_str$U.lo mpq/init$U.lo mpq/inits$U.lo mpq/inp_str$U.lo mpq/inv$U.lo mpq/md_2exp$U.lo mpq/mul$U.lo mpq/neg$U.lo mpq/out_str$U.lo mpq/set$U.lo mpq/set_d$U.lo mpq/set_den$U.lo mpq/set_f$U.lo mpq/set_num$U.lo mpq/set_si$U.lo mpq/set_str$U.lo mpq/set_ui$U.lo mpq/set_z$U.lo mpq/swap$U.lo 

MPN_OBJECTS = mpn/dummy1$U.lo

PRINTF_OBJECTS = printf/asprintf$U.lo printf/asprntffuns$U.lo printf/doprnt$U.lo printf/doprntf$U.lo printf/doprnti$U.lo printf/fprintf$U.lo printf/obprintf$U.lo printf/obprntffuns$U.lo printf/obvprintf$U.lo printf/printf$U.lo printf/printffuns$U.lo printf/repl-vsnprintf$U.lo printf/snprintf$U.lo printf/snprntffuns$U.lo printf/sprintf$U.lo printf/sprintffuns$U.lo printf/vasprintf$U.lo printf/vfprintf$U.lo printf/vprintf$U.lo printf/vsnprintf$U.lo printf/vsprintf$U.lo 

SCANF_OBJECTS = scanf/doscan$U.lo scanf/fscanf$U.lo scanf/fscanffuns$U.lo scanf/scanf$U.lo scanf/sscanf$U.lo scanf/sscanffuns$U.lo scanf/vfscanf$U.lo scanf/vscanf$U.lo scanf/vsscanf$U.lo 

# no $U for C++ files
CXX_OBJECTS =								\
  cxx/isfuns.lo cxx/ismpf.lo cxx/ismpq.lo cxx/ismpz.lo cxx/ismpznw.lo	\
  cxx/osdoprnti.lo cxx/osfuns.lo					\
  cxx/osmpf.lo cxx/osmpq.lo cxx/osmpz.lo

# In libtool 1.5 it doesn't work to build libmpir.la from the convenience
# libraries like mpz/libmpz.la.  Or rather it works, but it ends up putting
# PIC objects into libmpir.a if shared and static are both built.  (The PIC
# objects go into mpz/.libs/libmpz.a, and thence into .libs/libmpir.a.)
#
# For now the big lists of objects above are used.  Something like mpz/*.lo
# would probably work, but might risk missing something out or getting
# something extra.  The source files for each .lo are listed in the
# Makefile.am's in the subdirectories.
#
# Currently, for libmpir, unlike libmp below, we're not using
# -export-symbols, since the tune and speed programs, and perhaps some of
# the test programs, want to access undocumented symbols.

libmpir_la_SOURCES = gmp-impl.h longlong_pre.h longlong_post.h randmt.h			\
  assert.c compat.c errno.c extract-dbl.c invalid.c memory.c		\
  mp_bpl.c mp_clz_tab.c mp_dv_tab.c mp_minv_tab.c mp_get_fns.c mp_set_fns.c \
  randclr.c randdef.c randiset.c randlc2s.c randlc2x.c randmt.c	\
  randmts.c rands.c randsd.c randsdui.c randbui.c randmui.c primesieve.c \
  nextprime.c version.c
EXTRA_libmpir_la_SOURCES = tal-debug.c tal-notreent.c tal-reent.c
libmpir_la_DEPENDENCIES = @TAL_OBJECT@		\
  $(MPF_OBJECTS) $(FFT_OBJECTS) $(MPZ_OBJECTS) $(MPQ_OBJECTS)	\
  $(MPN_OBJECTS) @mpn_objs_in_libgmp@		\
  $(PRINTF_OBJECTS)  $(SCANF_OBJECTS)
libmpir_la_LIBADD = $(libmpir_la_DEPENDENCIES)
libmpir_la_LDFLAGS = $(GMP_LDFLAGS) $(LIBGMP_LDFLAGS) \
  -version-info $(LIBGMP_LT_CURRENT):$(LIBGMP_LT_REVISION):$(LIBGMP_LT_AGE)

libgmp_la_SOURCES = $(libmpir_la_SOURCES)
EXTRA_libgmp_la_SOURCES = $(EXTRA_libmpir_la_SOURCES)
libgmp_la_DEPENDENCIES = @TAL_OBJECT@		\
  $(MPF_OBJECTS) $(FFT_OBJECTS) $(MPZ_OBJECTS) $(MPQ_OBJECTS)	\
  $(MPN_OBJECTS) @mpn_objs_in_libgmp@		\
  $(PRINTF_OBJECTS)  $(SCANF_OBJECTS)
libgmp_la_LIBADD = $(libgmp_la_DEPENDENCIES)
libgmp_la_LDFLAGS = $(GMP_LDFLAGS) $(LIBGMP_LDFLAGS) \
  -version-info $(LIBGMP_LT_CURRENT):$(LIBGMP_LT_REVISION):$(LIBGMP_LT_AGE)


# We need at least one .cc file in $(libmpirxx_la_SOURCES) so automake will
# use $(CXXLINK) rather than the plain C $(LINK).  cxx/dummy.cc is that
# file.

if WANT_CXX
GMPXX_LTLIBRARIES_OPTION = libmpirxx.la
if WANT_GMPCOMPAT
GMPXX_LTLIBRARIES_OPTION += libgmpxx.la
endif
endif
libmpirxx_la_SOURCES = cxx/dummy.cc
libmpirxx_la_DEPENDENCIES = $(CXX_OBJECTS) libmpir.la
libmpirxx_la_LIBADD = $(libmpirxx_la_DEPENDENCIES)
libmpirxx_la_LDFLAGS = $(GMP_LDFLAGS) $(LIBGMPXX_LDFLAGS) \
  -version-info $(LIBGMPXX_LT_CURRENT):$(LIBGMPXX_LT_REVISION):$(LIBGMPXX_LT_AGE)

libgmpxx_la_SOURCES = $(libmpirxx_la_SOURCES)
libgmpxx_la_DEPENDENCIES = $(CXX_OBJECTS) libgmp.la
libgmpxx_la_LIBADD = $(libgmpxx_la_DEPENDENCIES)
libgmpxx_la_LDFLAGS = $(libmpirxx_la_LDFLAGS)

	@echo ''
	@echo '+-------------------------------------------------------------+'
	@echo '| CAUTION:                                                    |'
	@echo '|                                                             |'
	@echo '| If you have not already run "make check", then we strongly  |'
	@echo '| recommend you do so.                                        |'
	@echo '|                                                             |'
	@echo '| MPIR has been carefully tested by its authors, but compilers|'
	@echo '| are all too often released with serious bugs.  MPIR tends to|'
	@echo '| explore interesting corners in compilers and has hit bugs   |'
	@echo '| on quite a few occasions.                                   |'
	@echo '|                                                             |'
	@echo '+-------------------------------------------------------------+'
	@echo ''

# The "test -f" support for srcdir!=builddir is similar to the automake .c.o
# etc rules, but with each foo.c explicitly, since $< is not portable
# outside an inference rule.
#
# A quoted 'foo.c' is used with the "test -f"'s to avoid Sun make rewriting
# it as part of its VPATH support.  See the autoconf manual "Limitations of
# Make".
#
# Generated .h files which are used by gmp-impl.h are BUILT_SOURCES since
# they must exist before anything can be compiled.
#
# Other generated .h files are also BUILT_SOURCES so as to get all the
# build-system stuff over and done with at the start.  Also, dependencies on
# the .h files are not properly expressed for the various objects that use
# them.

mpn/dummy1.c:
	echo "void dummy1(void){return;}" > mpn/dummy1.c
BUILT_SOURCES += mpn/dummy1.c


# Avoid: CVS - cvs directories
#        *~  - emacs backups
#        .#* - cvs merge originals
#
# *~ and .#* only occur when a whole directory without it's own Makefile.am
# is distributed, like "doc" or the mpn cpu subdirectories.
#
dist-hook:
	-find $(distdir) \( -name CVS -type d \) -o -name "*~" -o -name ".#*" -o -name ".svn" | xargs rm -rf
