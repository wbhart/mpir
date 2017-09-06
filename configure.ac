dnl  Process this file with autoconf to produce a configure script.

define(GMP_COPYRIGHT,[[

Copyright 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006
Free Software Foundation, Inc.

Copyright 2008 William Hart.

This file is part of the MPIR Library.

The MPIR Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation; either version 2.1 of the License, or (at
your option) any later version.

The MPIR Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPIR Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
]])

AC_COPYRIGHT(GMP_COPYRIGHT)
AH_TOP(/*GMP_COPYRIGHT*/)

AC_REVISION($Revision: 1.11 $)
AC_PREREQ(2.68)
AC_INIT(MPIR, MPIR_VERSION, http://groups.google.co.uk/group/mpir-devel/, mpir)
AC_CONFIG_SRCDIR(gmp-impl.h)
m4_pattern_forbid([^[ \t]*GMP_])
m4_pattern_allow(GMP_LDFLAGS)
m4_pattern_allow(GMP_LIMB_BITS)
m4_pattern_allow(GMP_MPARAM_H_SUGGEST)
m4_pattern_allow(GMP_NAIL_BITS)
m4_pattern_allow(GMP_NUMB_BITS)
m4_pattern_allow(WANT_GMP_CC)

# If --target is not used then $target_alias is empty, but if say
# "./configure athlon-pc-freebsd3.5" is used, then all three of
# $build_alias, $host_alias and $target_alias are set to
# "athlon-pc-freebsd3.5".
#
if test -n "$target_alias" && test "$target_alias" != "$host_alias"; then
  AC_MSG_ERROR([--target is not appropriate for GMP
Use --build=CPU-VENDOR-OS if you need to specify your CPU and/or system
explicitly.  Use --host if cross-compiling (see "Installing GMP" in the
manual for more on this).])
fi

GMP_INIT(config.m4)

AC_CANONICAL_HOST

dnl  Automake "no-dependencies" is used because include file dependencies
dnl  are not useful to us.  Pretty much everything depends just on mpir.h,
dnl  gmp-impl.h and longlong.h, and yet only rarely does everything need to
dnl  be rebuilt for changes to those files.
dnl
dnl  Note that there's a copy of these options in the top-level Makefile.am,
dnl  so update there too if changing anything.
dnl
AM_INIT_AUTOMAKE([1.11 gnu subdir-objects no-dependencies parallel-tests no-dist-gzip dist-bzip2 dist-zip dist-lzip])
AC_CONFIG_HEADERS(config.h:config.in)
AM_MAINTAINER_MODE

dnl  Yasm configuration
AC_ARG_WITH([yasm],
[AC_HELP_STRING([--with-yasm],[use a custom Yasm])],
[],
[]
)

AC_ARG_ENABLE(assert,
AC_HELP_STRING([--enable-assert],[enable ASSERT checking [[default=no]]]),
[case $enableval in
yes|no) ;;
*) AC_MSG_ERROR([bad value $enableval for --enable-assert, need yes or no]) ;;
esac],
[enable_assert=no])

if test "$enable_assert" = "yes"; then
  AC_DEFINE(WANT_ASSERT,1,
  [Define to 1 to enable ASSERT checking, per --enable-assert])
  want_assert_01=1
else
  want_assert_01=0
fi
GMP_DEFINE_RAW(["define(<WANT_ASSERT>,$want_assert_01)"])


AC_ARG_ENABLE(alloca,
AC_HELP_STRING([--enable-alloca],[how to get temp memory [[default=reentrant]]]),
[case $enableval in
alloca|malloc-reentrant|malloc-notreentrant) ;;
yes|no|reentrant|notreentrant) ;;
debug) ;;
*)
  AC_MSG_ERROR([bad value $enableval for --enable-alloca, need one of:
yes no reentrant notreentrant alloca malloc-reentrant malloc-notreentrant debug]) ;;
esac],
[enable_alloca=reentrant])


# IMPROVE ME: The default for C++ is disabled.  The tests currently
# performed below for a working C++ compiler are not particularly strong,
# and in general can't be expected to get the right setup on their own.  The
# most significant problem is getting the ABI the same.  Defaulting CXXFLAGS
# to CFLAGS takes only a small step towards this.  It's also probably worth
# worrying whether the C and C++ runtimes from say gcc and a vendor C++ can
# work together.  Some rather broken C++ installations were encountered
# during testing, and though such things clearly aren't GMP's problem, if
# --enable-cxx=detect were to be the default then some careful checks of
# which, if any, C++ compiler on the system is up to scratch would be
# wanted.
#
AC_ARG_ENABLE(cxx,
AC_HELP_STRING([--enable-cxx],[enable C++ support [[default=no]]]),
[case $enableval in
yes|no|detect) ;;
*) AC_MSG_ERROR([bad value $enableval for --enable-cxx, need yes/no/detect]) ;;
esac],
[enable_cxx=no])


AC_ARG_ENABLE(fft,
AC_HELP_STRING([--enable-fft],[enable FFTs for multiplication [[default=yes]]]),
[case $enableval in
yes|no) ;;
*) AC_MSG_ERROR([bad value $enableval for --enable-fft, need yes or no]) ;;
esac],
[enable_fft=yes])

if test "$enable_fft" = "yes"; then
  AC_DEFINE(WANT_FFT,1,
  [Define to 1 to enable FFTs for multiplication, per --enable-fft])
fi

AC_ARG_ENABLE(gmpcompat,
AC_HELP_STRING([--enable-gmpcompat],
               [create library and header files named gmp as well as mpir [[default=no]]]),
[case $enableval in
yes|no) ;;
*) AC_MSG_ERROR([bad value $enableval for --enable-gmpcompat, need yes or no]) ;;
esac],
[enable_gmpcompat=no])
AM_CONDITIONAL(WANT_GMPCOMPAT, test "$enable_gmpcompat" = "yes")

AC_ARG_ENABLE(nails,
AC_HELP_STRING([--enable-nails],[use nails on limbs [[default=no]]]),
[case $enableval in
[no|[0]]) ;;
*) AC_MSG_ERROR([bad value $enableval for --enable-nails, Sorry nails not available in this build]) ;;
esac],
[enable_nails=no])

case $enable_nails in
yes) GMP_NAIL_BITS=2 ;;
no)  GMP_NAIL_BITS=0 ;;
*)   GMP_NAIL_BITS=$enable_nails ;;
esac
AC_SUBST(GMP_NAIL_BITS)


AC_ARG_ENABLE(profiling,
AC_HELP_STRING([--enable-profiling],
               [build with profiler support [[default=no]]]),
[case $enableval in
no|prof|gprof|instrument) ;;
*) AC_MSG_ERROR([bad value $enableval for --enable-profiling, need no/prof/gprof/instrument]) ;;
esac],
[enable_profiling=no])

case $enable_profiling in
  prof)
    AC_DEFINE(WANT_PROFILING_PROF, 1,
              [Define to 1 if --enable-profiling=prof])
    ;;
  gprof)
    AC_DEFINE(WANT_PROFILING_GPROF, 1,
              [Define to 1 if --enable-profiling=gprof])
    ;;
  instrument)
    AC_DEFINE(WANT_PROFILING_INSTRUMENT, 1,
              [Define to 1 if --enable-profiling=instrument])
    ;;
esac

GMP_DEFINE_RAW(["define(<WANT_PROFILING>,<\`$enable_profiling'>)"])

# -fomit-frame-pointer is incompatible with -pg on some chips
if test "$enable_profiling" = gprof; then
  fomit_frame_pointer=
else
  fomit_frame_pointer="-fomit-frame-pointer"
fi

AC_ARG_ENABLE(fat,
AC_HELP_STRING([--enable-fat],
               [build a fat binary on systems that support it [[default=no]]]),
[case $enableval in
yes|no) ;;
*) AC_MSG_ERROR([bad value $enableval for --enable-fat, need yes or no]) ;;
esac],
[enable_fat=no])

# Table of compilers, options, and mpn paths.  This code has various related
# purposes
#
#   - better default CC/CFLAGS selections than autoconf otherwise gives
#   - default CC/CFLAGS selections for extra CPU types specific to GMP
#   - a few tests for known bad compilers
#   - choice of ABIs on suitable systems
#   - selection of corresponding mpn search path
#
# After GMP specific searches and tests, the standard autoconf AC_PROG_CC is
# called.  User selections of CC etc are respected.
#
# Care is taken not to use macros like AC_TRY_COMPILE during the GMP
# pre-testing, since they of course depend on AC_PROG_CC, and also some of
# them cache their results, which is not wanted.
#
# The ABI selection mechanism is unique to GMP.  All that reaches autoconf
# is a different selection of CC/CFLAGS according to the best ABI the system
# supports, and/or what the user selects.  Naturally the mpn assembler code
# selected is very dependent on the ABI.
#
# The closest the standard tools come to a notion of ABI is something like
# "sparc64" which encodes a CPU and an ABI together.  This doesn't seem to
# scale well for GMP, where exact CPU types like "ultrasparc2" are wanted,
# separate from the ABI used on them.
#
#
# The variables set here are
#
#   cclist              the compiler choices
#   xx_cflags           flags for compiler xx
#   xx_cflags_maybe     flags for compiler xx, if they work
#   xx_cppflags         cpp flags for compiler xx
#   xx_cflags_optlist   list of sets of optional flags
#   xx_cflags_yyy       set yyy of optional flags for compiler xx
#   xx_ldflags          -Wc,-foo flags for libtool linking with compiler xx
#   ar_flags            extra flags for $AR
#   nm_flags            extra flags for $NM
#   limb                limb size, can be "longlong"
#   path                mpn search path
#   extra_functions     extra mpn functions
#   fat_path            fat binary mpn search path [if fat binary desired]
#   fat_functions       fat functions
#   fat_thresholds      fat thresholds
#
# Suppose xx_cflags_optlist="arch", then flags from $xx_cflags_arch are
# tried, and the first flag that works will be used.  An optlist like "arch
# cpu optimize" can be used to get multiple independent sets of flags tried.
# The first that works from each will be used.  If no flag in a set works
# then nothing from that set is added.
#
# For multiple ABIs, the scheme extends as follows.
#
#   abilist               set of ABI choices
#   cclist_aa             compiler choices in ABI aa
#   xx_aa_cflags          flags for xx in ABI aa
#   xx_aa_cflags_maybe    flags for xx in ABI aa, if they work
#   xx_aa_cppflags        cpp flags for xx in ABI aa
#   xx_aa_cflags_optlist  list of sets of optional flags in ABI aa
#   xx_aa_cflags_yyy      set yyy of optional flags for compiler xx in ABI aa
#   xx_aa_ldflags         -Wc,-foo flags for libtool linking
#   ar_aa_flags           extra flags for $AR in ABI aa
#   nm_aa_flags           extra flags for $NM in ABI aa
#   limb_aa               limb size in ABI aa, can be "longlong"
#   path_aa               mpn search path in ABI aa
#   extra_functions_aa    extra mpn functions in ABI aa
#
# As a convenience, the unadorned xx_cflags (etc) are used for the last ABI
# in ablist, if an xx_aa_cflags for that ABI isn't given.  For example if
# abilist="64 32" then $cc_64_cflags will be used for the 64-bit ABI, but
# for the 32-bit either $cc_32_cflags or $cc_cflags is used, whichever is
# defined.  This makes it easy to add some 64-bit compilers and flags to an
# unadorned 32-bit set.
#
# limb=longlong (or limb_aa=longlong) applies to all compilers within that
# ABI.  It won't work to have some needing long long and some not, since a
# single instantiated mpir.h will be used by both.
#
# SPEED_CYCLECOUNTER, cyclecounter_size and CALLING_CONVENTIONS_OBJS are
# also set here, with an ABI suffix.
#
#
#
# A table-driven approach like this to mapping cpu type to good compiler
# options is a bit of a maintenance burden, but there's not much uniformity
# between options specifications on different compilers.  Some sort of
# separately updatable tool might be cute.
#
# The use of lots of variables like this, direct and indirect, tends to
# obscure when and how various things are done, but unfortunately it's
# pretty much the only way.  If shell subroutines were portable then actual
# code like "if this .. do that" could be written, but attempting the same
# with full copies of GMP_PROG_CC_WORKS etc expanded at every point would
# hugely bloat the output.


AC_ARG_VAR(ABI, [desired ABI (for processors supporting more than one ABI)])

# abilist needs to be non-empty, "standard" is just a generic name here
abilist="standard"

# FIXME: We'd like to prefer an ANSI compiler, perhaps by preferring
# c89 over cc here.  But note that on HP-UX c89 provides a castrated
# environment, and would want to be excluded somehow.  Maybe
# AC_PROG_CC_STDC already does enough to stick cc into ANSI mode and
# we don't need to worry.
#
cclist="gcc cc"

gcc_cflags="-O2"
gcc_64_cflags="-O2"
cc_cflags="-O"
cc_64_cflags="-O"

SPEED_CYCLECOUNTER_OBJ=
cyclecounter_size=2

case $host in

  alpha*-*-*)
    case $host_cpu in
      alphaev5* | alphapca5*) path="alpha/ev5 alpha" ;;
      alphaev67 | alphaev68 | alphaev7*)
        path="alpha/ev67 alpha/ev6 alpha/ev5 alpha" ;;
      alphaev6* | alphaev7*)  path="alpha/ev6 alpha/ev5 alpha" ;;
      *)                      path="alpha" ;;
    esac
    extra_functions="cntlz"
    gcc_cflags_optlist="asm cpu" # need asm ahead of cpu, see below

    # gcc 2.7.2.3 doesn't know any -mcpu= for alpha, apparently.
    # gcc 2.95 knows -mcpu= ev4, ev5, ev56, pca56, ev6.
    # gcc 3.0 adds nothing.
    # gcc 3.1 adds ev45, ev67 (but ev45 is the same as ev4).
    # gcc 3.2 adds nothing.
    #
    # gcc version "2.9-gnupro-99r1" under "-O2 -mcpu=ev6" strikes internal
    # compiler errors too easily and is rejected by GMP_PROG_CC_WORKS.  Each
    # -mcpu=ev6 below has a fallback to -mcpu=ev56 for this reason.
    #
    case $host_cpu in
      alpha)        gcc_cflags_cpu="-mcpu=ev4" ;;
      alphaev5)     gcc_cflags_cpu="-mcpu=ev5" ;;
      alphaev56)    gcc_cflags_cpu="-mcpu=ev56" ;;
      alphapca56 | alphapca57)
                    gcc_cflags_cpu="-mcpu=pca56" ;;
      alphaev6)     gcc_cflags_cpu="-mcpu=ev6 -mcpu=ev56" ;;
      alphaev67 | alphaev68 | alphaev7*)
                    gcc_cflags_cpu="-mcpu=ev67 -mcpu=ev6 -mcpu=ev56" ;;
    esac

    # On old versions of gcc, which don't know -mcpu=, we believe an
    # explicit -Wa,-mev5 etc will be necessary to put the assembler in
    # the right mode for our .asm files and longlong.h asm blocks.
    #
    # On newer versions of gcc, when -mcpu= is known, we must give a -Wa
    # which is at least as high as the code gcc will generate.  gcc
    # establishes what it needs with a ".arch" directive, our command line
    # option seems to override that.
    #
    # gas prior to 2.14 doesn't accept -mev67, but -mev6 seems enough for
    # ctlz and cttz (in 2.10.0 at least).
    #
    case $host_cpu in
      alpha)        gcc_cflags_asm="-Wa,-arch,ev4 -Wa,-mev4" ;;
      alphaev5)     gcc_cflags_asm="-Wa,-arch,ev5 -Wa,-mev5" ;;
      alphaev56)    gcc_cflags_asm="-Wa,-arch,ev56 -Wa,-mev56" ;;
      alphapca56 | alphapca57)
                    gcc_cflags_asm="-Wa,-arch,pca56 -Wa,-mpca56" ;;
      alphaev6)     gcc_cflags_asm="-Wa,-arch,ev6 -Wa,-mev6" ;;
      alphaev67 | alphaev68 | alphaev7*)
                    gcc_cflags_asm="-Wa,-arch,ev67 -Wa,-mev67 -Wa,-arch,ev6 -Wa,-mev6" ;;
    esac

    # It might be better to ask "cc" whether it's Cray C or DEC C,
    # instead of relying on the OS part of $host.  But it's hard to
    # imagine either of those compilers anywhere except their native
    # systems.
    #
    GMP_INCLUDE_MPN(alpha/alpha-defs.m4)
    GMP_INCLUDE_MPN(alpha/default.m4)
    SPEED_CYCLECOUNTER_OBJ=alpha.lo
    cyclecounter_size=1
    ;;

  arm*-*-*)
    path="arm"
    gcc_cflags="$gcc_cflags $fomit_frame_pointer"
    gcc_testlist="gcc-arm-umodsi"
    GMP_INCLUDE_MPN(arm/arm-defs.m4)
    ;;

  IA64_PATTERN)
    abilist="64"
    GMP_INCLUDE_MPN(ia64/ia64-defs.m4)
    SPEED_CYCLECOUNTER_OBJ=ia64.lo
    gcc_64_cflags="-O2"

    case $host_cpu in
      itanium)   path="ia64/itanium  ia64" ;;
      itanium2)  path="ia64/itanium2 ia64" ;;
      *)         path="ia64" ;;
    esac

    gcc_64_cflags_optlist="tune"
    gcc_32_cflags_optlist=$gcc_64_cflags_optlist

    # gcc pre-release 3.4 adds -mtune itanium and itanium2
    case $host_cpu in
      itanium)   gcc_cflags_tune="-mtune=itanium" ;;
      itanium2)  gcc_cflags_tune="-mtune=itanium2" ;;
    esac

    case $host in
      *-*-linux*)
	cclist="gcc icc"
	icc_cflags="-no-gcc -fpic"
	icc_cflags_optlist="opt"
	# FIXME: Check if -O3 is really right.  The manual warns it is for
	# large data sets, for which GMP uses assembly loops.
	icc_cflags_opt="-O3 -O2 -O1"
	;;

      *-*-hpux*)
        # HP cc sometimes gets internal errors if the optimization level is
        # too high.  GMP_PROG_CC_WORKS detects this, the "_opt" fallbacks
        # let us use whatever seems to work.
        #
        abilist="32 64"

        cclist_32="gcc cc"
        path_32="ia64"
        cc_32_cflags=""
        cc_32_cflags_optlist="opt"
        cc_32_cflags_opt="+O3 +O2 +O1"
        gcc_32_cflags="-milp32 -O2"
        limb_32=longlong
        SPEED_CYCLECOUNTER_OBJ_32=ia64.lo
        cyclecounter_size_32=2

        # Must have +DD64 in CPPFLAGS to get the right __LP64__ for headers,
        # but also need it in CFLAGS for linking programs, since automake
        # only uses CFLAGS when linking, not CPPFLAGS.
        # FIXME: Maybe should use cc_64_ldflags for this, but that would
        # need GMP_LDFLAGS used consistently by all the programs.
        #
        cc_64_cflags="+DD64"
        cc_64_cppflags="+DD64"
        cc_64_cflags_optlist="opt"
        cc_64_cflags_opt="+O3 +O2 +O1"
        gcc_64_cflags="-mlp64 -O2"
        ;;
    esac
    ;;

  # Irix  5 and earlier can only run 32-bit o32.
  #
  # IRIX 6 and up always has a 64-bit mips CPU can run n32 or 64.  n32 is
  # preferred over 64, but only because that's been the default in past
  # versions of GMP.  The two are equally efficient.
  #
  # Linux kernel 2.2.13 arch/mips/kernel/irixelf.c has a comment about not
  # supporting n32 or 64.
  #
  # For reference, libtool (eg. 1.5.6) recognises the n32 ABI and knows the
  # right options to use when linking (both cc and gcc), so no need for
  # anything special from us.
  #
  mips*-*-*)
    abilist="o32"
    gcc_cflags_optlist="abi"
    gcc_cflags_abi="-mabi=32"
    gcc_testlist="gcc-mips-o32"
    path="mips32"
    cc_cflags="-O2 -o32"   # no -g, it disables all optimizations
    # this suits both mips32 and mips64
    GMP_INCLUDE_MPN(mips32/mips-defs.m4)

    case $host in
      [mips64*-*-*])
        abilist="64 n32 o32"

        cclist_n32="gcc cc"
	gcc_n32_cflags="-O2 -mabi=n32"
        cc_n32_cflags="-O2 -n32"	# no -g, it disables all optimizations
        limb_n32=longlong
        path_n32="mips64"

        cclist_64="pathcc gcc cc"
        gcc_64_cflags="-O2 -mabi=64"
        gcc_64_ldflags="-Wc,-mabi=64"
        cc_64_cflags="-O2 -64"		# no -g, it disables all optimizations
        cc_64_ldflags="-Wc,-64"
        pathcc_64_cflags="-O0 -mabi=64"
        pathcc_64_ldflags="-Wc,-mabi=64"
        path_64="mips64"
        ;;
    esac
    ;;


  # Darwin (powerpc-apple-darwin1.3) has it's hacked gcc installed as cc.
  # Our usual "gcc in disguise" detection means gcc_cflags etc here gets
  # used.
  #
  # The darwin pre-compiling preprocessor is disabled with -no-cpp-precomp
  # since it doesn't like "__attribute__ ((mode (SI)))" etc in gmp-impl.h,
  # and so always ends up running the plain preprocessor anyway.  This could
  # be done in CPPFLAGS rather than CFLAGS, but there's not many places
  # preprocessing is done separately, and this is only a speedup, the normal
  # preprocessor gets run if there's any problems.
  #
  # We used to use -Wa,-mppc with gcc, but can't remember exactly why.
  # Presumably it was for old versions of gcc where -mpowerpc doesn't put
  # the assembler in the right mode.  In any case -Wa,-mppc is not good, for
  # instance -mcpu=604 makes recent gcc use -m604 to get access to the
  # "fsel" instruction, but a -Wa,-mppc overrides that, making code that
  # comes out with fsel fail.
  #
  # (Note also that the darwin assembler doesn't accept "-mppc", so any
  # -Wa,-mppc was used only if it worked.  The right flag on darwin would be
  # "-arch ppc" or some such, but that's already the default.)
  #
  [powerpc*-*-* | power[3-9]-*-*])
    abilist="32"
    cclist="gcc cc"
    cc_cflags="-O2"
    gcc_cflags="-O2 -mpowerpc"
    gcc_cflags_optlist="precomp subtype asm cpu"
    gcc_cflags_precomp="-no-cpp-precomp"
    gcc_cflags_subtype="-force_cpusubtype_ALL"	# for vmx on darwin
    gcc_cflags_asm=""
    gcc_cflags_cpu=""

    # grab this object, though it's not a true cycle counter routine
    SPEED_CYCLECOUNTER_OBJ=powerpc.lo
    cyclecounter_size=0

    case $host_cpu in
      powerpc740 | powerpc750)
        path="powerpc32/750 powerpc32" ;;
      powerpc7400)
        path="powerpc32/vmx powerpc32/750 powerpc32" ;;
      [powerpc74[45]?])
        path="powerpc32/vmx powerpc32 powerpc32" ;;
      *)
        path="powerpc32" ;;
    esac

    # gcc 2.7.2 knows -mcpu=403, 601, 603, 604.
    # gcc 2.95 adds 401, 505, 602, 603e, ec603e, 604e, 620, 740, 750,
    #   801, 821, 823, 860.
    # gcc 3.0 adds 630, rs64a.
    # gcc 3.1 adds 405, 7400, 7450.
    # gcc 3.2 adds nothing.
    # gcc 3.3 adds power3, power4, 8540.  power3 seems to be a synonym for 630.
    # gcc pre-release 3.4 adds 405fp, 440, 440fp, 970.
    #
    # FIXME: The way 603e falls back to 603 for gcc 2.7.2 should be
    # done for all the others too.  But what would be the correct
    # arrangements?
    #
    case $host_cpu in
      powerpc401)   gcc_cflags_cpu="-mcpu=401" ;;
      powerpc403)   gcc_cflags_cpu="-mcpu=403" ;;
      powerpc405)   gcc_cflags_cpu="-mcpu=405" ;;
      powerpc505)   gcc_cflags_cpu="-mcpu=505" ;;
      powerpc601)   gcc_cflags_cpu="-mcpu=601" ;;
      powerpc602)   gcc_cflags_cpu="-mcpu=602" ;;
      powerpc603)   gcc_cflags_cpu="-mcpu=603" ;;
      powerpc603e)  gcc_cflags_cpu="-mcpu=603e -mcpu=603" ;;
      powerpc604)   gcc_cflags_cpu="-mcpu=604" ;;
      powerpc604e)  gcc_cflags_cpu="-mcpu=604e -mcpu=604" ;;
      powerpc620)   gcc_cflags_cpu="-mcpu=620" ;;
      powerpc630)   gcc_cflags_cpu="-mcpu=630" ;;
      powerpc740)   gcc_cflags_cpu="-mcpu=740" ;;
      powerpc7400 | powerpc7410)
		    gcc_cflags_asm="-Wa,-maltivec"
  		    gcc_cflags_cpu="-mcpu=7400 -mcpu=750" ;;
      powerpc74[45]?)
            gcc_cflags_asm="-Wa,-maltivec"
  		    gcc_cflags_cpu="-mcpu=7450" ;;
      powerpc750)   gcc_cflags_cpu="-mcpu=750" ;;
      powerpc801)   gcc_cflags_cpu="-mcpu=801" ;;
      powerpc821)   gcc_cflags_cpu="-mcpu=821" ;;
      powerpc823)   gcc_cflags_cpu="-mcpu=823" ;;
      powerpc860)   gcc_cflags_cpu="-mcpu=860" ;;
      powerpc970)   gcc_cflags_cpu="-mcpu=970" ;;
    esac

    case $host in
      *-*-aix*)
        cclist="gcc xlc cc"
        xlc_cflags="-O2 -qmaxmem=20000"
        xlc_cflags_optlist="arch"

        # xlc (what version?) knows -qarch=ppc, ppcgr, 601, 602, 603, 604,
        # 403, rs64a
        # -qarch=ppc is needed, so ensure everything falls back to that.
        # FIXME: Perhaps newer versions know more flavours.
        #
    	case $host_cpu in
	  powerpc403)   xlc_cflags_arch="-qarch=403 -qarch=ppc" ;;
	  powerpc601)   xlc_cflags_arch="-qarch=601 -qarch=ppc" ;;
	  powerpc602)   xlc_cflags_arch="-qarch=602 -qarch=ppc" ;;
	  powerpc603)   xlc_cflags_arch="-qarch=603 -qarch=ppc" ;;
	  powerpc603e)  xlc_cflags_arch="-qarch=603 -qarch=ppc" ;;
	  powerpc604)   xlc_cflags_arch="-qarch=604 -qarch=ppc" ;;
	  powerpc604e)  xlc_cflags_arch="-qarch=604 -qarch=ppc" ;;
	  *)            xlc_cflags_arch="-qarch=ppc" ;;
        esac
        ;;
    esac

    case $host in
      POWERPC64_PATTERN)
        case $host_cpu in
	  powerpc970)		vmx_path="powerpc64/vmx" ;;
	  *)			vmx_path="" ;;
	esac
        case $host in
          *-*-aix*)
            # On AIX a true 64-bit ABI is available.
            # Need -Wc to pass object type flags through to the linker.
            abilist="aix64 $abilist"
            cclist_aix64="gcc xlc"
            gcc_aix64_cflags="-O2 -maix64 -mpowerpc64"
            gcc_aix64_cflags_optlist="cpu"
	    gcc_aix64_ldflags="-Wc,-maix64"
            xlc_aix64_cflags="-O2 -q64 -qtune=pwr3 -qmaxmem=20000"
	    xlc_aix64_ldflags="-Wc,-q64"
            # Must indicate object type to ar and nm
	    ar_aix64_flags="-X64"
	    nm_aix64_flags="-X64"
            path_aix64="powerpc64/mode64 $vmx_path powerpc64"
            # grab this object, though it's not a true cycle counter routine
            SPEED_CYCLECOUNTER_OBJ_aix64=powerpc64.lo
            cyclecounter_size_aix64=0
            ;;
          *-*-darwin*)
            # On Darwin we can use 64-bit instructions with a longlong limb,
            # but the chip still in 32-bit mode.
            # In theory this can be used on any OS which knows how to save
            # 64-bit registers in a context switch.
            #
            # Note that we must use -mpowerpc64 with gcc, since the
            # longlong.h macros expect limb operands in a single 64-bit
            # register, not two 32-bit registers as would be given for a
            # long long without -mpowerpc64.  In theory we could detect and
            # accomodate both styles, but the proper 64-bit registers will
            # be fastest and are what we really want to use.
            #
	    # One would think -mpowerpc64 would set the assembler in the right
	    # mode to handle 64-bit instructions.  But for that, also
	    # -force_cpusubtype_ALL is needed.
	    #
	    # Do not use -fast for Darwin, it actually adds options
	    # incompatible with a shared library.
	    #
	    abilist="mode64 mode32 $abilist"
	    gcc_cflags_opt="-O3 -O2 -O1"	# will this become used?
	    cclist_mode32="gcc"
	    gcc_mode32_cflags="-mpowerpc64"
	    gcc_mode32_cflags_optlist="subtype cpu opt"
	    gcc_mode32_cflags_subtype="-force_cpusubtype_ALL"
	    gcc_mode32_cflags_opt="-O3 -O2 -O1"
	    path_mode32="powerpc64/mode32 $vmx_path powerpc64"
	    limb_mode32=longlong
	    cclist_mode64="gcc"
	    gcc_mode64_cflags="-m64"
	    gcc_mode64_cflags_optlist="cpu opt"
	    gcc_mode64_cflags_opt="-O3 -O2 -O1"
	    path_mode64="powerpc64/mode64 $vmx_path powerpc64"
            SPEED_CYCLECOUNTER_OBJ_mode64=powerpc64.lo
            cyclecounter_size_mode64=0
	    any_mode64_testlist="sizeof-long-8"
	    ;;
	  *-*-linux* | *-*-*bsd*)
	    # On GNU/Linux, assume the processor is in 64-bit mode.  Some
	    # environments have a gcc that is always in 64-bit mode, while
	    # others require -m64, hence the use of cflags_maybe.  The
	    # sizeof-long-8 test checks the mode is right (for the no option
	    # case).
            #
            # -mpowerpc64 is not used, since it should be the default in
            # 64-bit mode.  (We need its effect for the various longlong.h
            # asm macros to be right of course.)
            #
            # gcc64 was an early port of gcc to 64-bit mode, but should be
            # obsolete before too long.  We prefer plain gcc when it knows
            # 64-bits.
	    #
	    abilist="mode64 mode32 $abilist"
	    cclist_mode32="gcc"
	    gcc_mode32_cflags="-mpowerpc64 -Wa,-mppc64"
	    gcc_mode32_cflags_optlist="cpu opt"
	    gcc_mode32_cflags_opt="-O3 -O2 -O1"
	    path_mode32="powerpc64/mode32 $vmx_path powerpc64"
	    limb_mode32=longlong
	    cclist_mode64="gcc gcc64"
	    gcc_mode64_cflags_maybe="-m64"
	    gcc_mode64_cflags_optlist="cpu opt"
	    gcc_mode64_cflags_opt="-O3 -O2 -O1"
	    path_mode64="powerpc64/mode64 $vmx_path powerpc64"
            SPEED_CYCLECOUNTER_OBJ_mode64=powerpc64.lo
            cyclecounter_size_mode64=0
	    any_mode64_testlist="sizeof-long-8"
	    ;;
        esac
        ;;
    esac
    ;;

  *sparc*-*-*)
    # sizeof(long)==4 or 8 is tested, to ensure we get the right ABI.  We've
    # had various bug reports where users have set CFLAGS for their desired
    # mode, but not set our ABI.  For some reason it's sparc where this
    # keeps coming up, presumably users there are accustomed to driving the
    # compiler mode that way.  The effect of our testlist setting is to
    # reject ABI=64 in favour of ABI=32 if the user has forced the flags to
    # 32-bit mode.
    #
    abilist="32"
    cclist="gcc acc cc"
    any_testlist="sizeof-long-4"
    GMP_INCLUDE_MPN(sparc32/sparc-defs.m4)

    case $host_cpu in
      sparcv8 | microsparc | turbosparc)
        path="sparc32" ;;
      supersparc)
        path="sparc32" ;;
      sparc64 | sparcv9* | ultrasparc*)
        path="sparc32/v9 sparc32" ;;
      *)
        path="sparc32" ;;
    esac

    # gcc 2.7.2 doesn't know about v9 and doesn't pass -xarch=v8plus to the
    # assembler.  Add it explicitly since the solaris assembler won't accept
    # our sparc32/v9 asm code without it.  gas accepts -xarch=v8plus too, so
    # it can be in the cflags unconditionally (though gas doesn't need it).
    #
    # gcc -m32 is needed to force 32-bit mode on a dual-ABI system, but past
    # gcc doesn't know that flag, hence cflags_maybe.  Note that -m32 cannot
    # be done through the optlist since the plain cflags would be run first
    # and we don't want to require the default mode (whatever it is) works.
    #
    # Note it's gcc_32_cflags_maybe and not gcc_cflags_maybe because the
    # latter would be used in the 64-bit ABI on systems like "*bsd" where
    # abilist="64" only.
    #
    case $host_cpu in
      sparc64 | sparcv9* | ultrasparc*)
        gcc_cflags="-O2 -Wa,-xarch=v8plus" ;;
      *)
        gcc_cflags="-O2" ;;
    esac
    gcc_32_cflags_maybe="-m32"
    gcc_cflags_optlist="cpu"

    # gcc 2.7.2 knows -mcypress, -msupersparc, -mv8, -msparclite.
    # gcc 2.95 knows -mcpu= v7, hypersparc, sparclite86x, f930, f934,
    #   sparclet, tsc701, v9, ultrasparc.  A warning is given that the
    #   plain -m forms will disappear.
    # gcc 3.0 adds nothing.
    # gcc 3.1 adds nothing.
    # gcc 3.2 adds nothing.
    # gcc 3.3 adds ultrasparc3.
    #
    case $host_cpu in
      supersparc)           gcc_cflags_cpu="-mcpu=supersparc -msupersparc" ;;
      sparcv8 | microsparc | turbosparc)
			    gcc_cflags_cpu="-mcpu=v8 -mv8" ;;
      sparc64 | sparcv9*)   gcc_cflags_cpu="-mcpu=v9 -mv8" ;;
      ultrasparc3)          gcc_cflags_cpu="-mcpu=ultrasparc3 -mcpu=ultrasparc -mv8" ;;
      ultrasparc*)          gcc_cflags_cpu="-mcpu=ultrasparc -mv8" ;;
      *)                    gcc_cflags_cpu="-mcpu=v7 -mcypress" ;;
    esac

    # SunPRO cc and acc, and SunOS bundled cc
    case $host in
      *-*-solaris* | *-*-sunos*)
    	# Note no -g, it disables all optimizations.
    	cc_cflags=
    	cc_cflags_optlist="opt arch cpu"

        # SunOS cc doesn't know -xO4, fallback to -O2.
    	cc_cflags_opt="-xO4 -O2"

        # SunOS cc doesn't know -xarch, apparently always generating v7
        # code, so make this optional
    	case $host_cpu in
    	  sparcv8 | microsparc | supersparc | turbosparc)
					      cc_cflags_arch="-xarch=v8" ;;
    	  sparc64 | sparcv9* | ultrasparc*)   cc_cflags_arch="-xarch=v8plus" ;;
    	  *)                                  cc_cflags_arch="-xarch=v7" ;;
    	esac

        # SunOS cc doesn't know -xchip and doesn't seem to have an equivalent.
    	# SunPRO cc 5 recognises -xchip=generic, old, super, super2, micro,
    	#   micro2, hyper, hyper2, powerup, ultra, ultra2, ultra2i.
    	# SunPRO cc 6 adds -xchip=ultra2e, ultra3cu.
        #
    	# FIXME: Which of ultra, ultra2 or ultra2i is the best fallback for
    	# ultrasparc3?
    	#
    	case $host_cpu in
    	  supersparc)   cc_cflags_cpu="-xchip=super" ;;
    	  microsparc)   cc_cflags_cpu="-xchip=micro" ;;
    	  turbosparc)   cc_cflags_cpu="-xchip=micro2" ;;
    	  ultrasparc)   cc_cflags_cpu="-xchip=ultra" ;;
    	  ultrasparc2)  cc_cflags_cpu="-xchip=ultra2" ;;
    	  ultrasparc2i) cc_cflags_cpu="-xchip=ultra2i" ;;
    	  ultrasparc3)  cc_cflags_cpu="-xchip=ultra3 -xchip=ultra" ;;
    	  *)            cc_cflags_cpu="-xchip=generic" ;;
    	esac
    esac

    case $host_cpu in
      sparc64 | sparcv9* | ultrasparc*)
        case $host in
          # Solaris 6 and earlier cannot run ABI=64 since it doesn't save
          # registers properly, so ABI=32 is left as the only choice.
          #
          [*-*-solaris2.[0-6] | *-*-solaris2.[0-6].*]) ;;

          # BSD sparc64 ports are 64-bit-only systems, so ABI=64 is the only
          # choice.  In fact they need no special compiler flags, gcc -m64
          # is the default, but it doesn't hurt to add it.  v9 CPUs always
          # use the sparc64 port, since the plain 32-bit sparc ports don't
          # run on a v9.
          #
          *-*-*bsd*) abilist="64" ;;

          # For all other systems, we try both 64 and 32.
          #
          # GNU/Linux sparc64 has only recently gained a 64-bit user mode.
          # In the past sparc64 meant a v9 cpu, but there were no 64-bit
          # operations in user mode.  We assume that if "gcc -m64" works
          # then the system is suitable.  Hopefully even if someone attempts
          # to put a new gcc and/or glibc on an old system it won't run.
          #
          *) abilist="64 32" ;;
        esac

        path_64="sparc64"
        cclist_64="gcc"
        any_64_testlist="sizeof-long-8"

        # gcc -mptr64 is probably implied by -m64, but we're not sure if
        # this was always so.  On Solaris in the past we always used both
        # "-m64 -mptr64".
        #
        # gcc -Wa,-xarch=v9 is thought to be necessary in some cases on
        # solaris, but it would seem likely that if gcc is going to generate
        # 64-bit code it will have to add that option itself where needed.
        # An extra copy of this option should be harmless though, but leave
        # it until we're sure.  (Might want -xarch=v9a or -xarch=v9b for the
        # higher cpu types instead.)
        #
        gcc_64_cflags="-O2 -m64 -mptr64"
        gcc_64_ldflags="-Wc,-m64"
        gcc_64_cflags_optlist="cpu"

        case $host in
          *-*-solaris*)
            # Sun cc.
            #
            # -fast enables different optimizations depending on compiler
            # version.  Unfortunately it does things according to the native
            # system, which may not be optimal when cross compiling (to a
            # different sparc).  -xchip from cc_cflags_cpu will override at
            # least that part of its selections.
            #
             # -fns=no and -fsimple=1 disable some transformations that
            # conflict with IEEE 754, which some compiler versions perform
            # under -fast.
            #
            # In any case -fast can result in incorrect optimisations and so
            # has been removed (see
            # http://swox.com/list-archives/gmp-bugs/2008-April/000987.html)
            #
           cclist_64="$cclist_64 cc"
            cc_64_cflags="-xarch=v9"
            cc_64_cflags_optlist="cpu"
            ;;
        esac

        # using the v9 %tick register
        SPEED_CYCLECOUNTER_OBJ_32=sparcv9.lo
        SPEED_CYCLECOUNTER_OBJ_64=sparcv9.lo
        cyclecounter_size_32=2
        cyclecounter_size_64=2
        ;;
    esac
    ;;

  # AMD and Intel x86 configurations, including AMD64
  #
  # Rumour has it gcc -O2 used to give worse register allocation than just
  # -O, but lets assume that's no longer true.
  #
  # -m32 forces 32-bit mode on a bi-arch 32/64 amd64 build of gcc.  -m64 is
  # the default in such a build (we think), so -m32 is essential for ABI=32.
  # This is, of course, done for any $host_cpu, not just x86_64, so we can
  # get such a gcc into the right mode to cross-compile to say i486-*-*.
  #
  # -m32 is not available in gcc 2.95 and earlier, hence cflags_maybe to use
  # it when it works.  We check sizeof(long)==4 to ensure we get the right
  # mode, in case -m32 has failed not because it's an old gcc, but because
  # it's a dual 32/64-bit gcc without a 32-bit libc, or whatever.
  #
  X86_PATTERN | X86_64_PATTERN)
    abilist="32"
    cclist="gcc icc cc"
    gcc_cflags="-O2 $fomit_frame_pointer"
	case $host in
	*-*-linux-gnu)
	    gcc_32_ldflags="-Wl,-z,noexecstack" ;;
	esac
    gcc_32_cflags_maybe="-m32"
    icc_cflags="-no-gcc"
    icc_cflags_optlist="opt"
    icc_cflags_opt="-O3 -O2 -O1"
    any_32_testlist="sizeof-long-4"
    CALLING_CONVENTIONS_OBJS='x86call.lo x86check$U.lo'

    # Availability of rdtsc is checked at run-time.
    SPEED_CYCLECOUNTER_OBJ=pentium.lo

    case $host in
      *-*-solaris* | *-*-sunos*)
    	# Note no -g, it disables all optimizations.
    	cc_cflags=
    	cc_cflags_optlist="opt arch cpu"

        # SunOS cc doesn't know -xO4, fallback to -O2.
    	cc_cflags_opt="-xO4 -O2" ;;
    esac

    # gcc 2.7.2 only knows i386 and i486, using -m386 or -m486.  These
    #     represent -mcpu= since -m486 doesn't generate 486 specific insns.
    # gcc 2.95 adds k6, pentium and pentiumpro, and takes -march= and -mcpu=.
    # gcc 3.0 adds athlon.
    # gcc 3.1 adds k6-2, k6-3, pentium-mmx, pentium2, pentium3, pentium4,
    #     athlon-tbird, athlon-4, athlon-xp, athlon-mp.
    # gcc 3.2 adds winchip2.
    # gcc 3.3 adds winchip-c6.
    # gcc 3.3.1 from mandrake adds k8 and knows -mtune.
    # gcc 3.4 adds c3, c3-2, k8, and deprecates -mcpu in favour of -mtune.
    #
    # In gcc 2.95.[0123], -march=pentiumpro provoked a stack slot bug in an
    # old version of mpz/powm.c.  Seems to be fine with the current code, so
    # no need for any restrictions on that option.
    #
    # -march=pentiumpro can fail if the assembler doesn't know "cmov"
    # (eg. solaris 2.8 native "as"), so always have -march=pentium after
    # that as a fallback.
    #
    # -march=pentium4 and -march=k8 enable SSE2 instructions, which may or
    # may not be supported by the assembler and/or the OS, and is bad in gcc
    # prior to 3.3.  The tests will reject these if no good, so fallbacks
    # like "-march=pentium4 -mno-sse2" are given to try also without SSE2.
    # Note the relevant -march types are listed in the optflags handling
    # below, be sure to update there if adding new types emitting SSE2.
    #
    # -mtune is used at the start of each cpu option list to give something
    # gcc 3.4 will use, thereby avoiding warnings from -mcpu.  -mcpu forms
    # are retained for use by prior gcc.  For example pentium has
    # "-mtune=pentium -mcpu=pentium ...", the -mtune is for 3.4 and the
    # -mcpu for prior.  If there's a brand new choice in 3.4 for a chip,
    # like k8 for x86_64, then it can be the -mtune at the start, no need to
    # duplicate anything.
    #
    gcc_cflags_optlist="cpu arch"
    case $host_cpu in
      i386*)
        gcc_cflags_cpu="-mtune=i386 -mcpu=i386 -m386"
        gcc_cflags_arch="-march=i386"
        ;;
      i486*)
        gcc_cflags_cpu="-mtune=i486 -mcpu=i486 -m486"
        gcc_cflags_arch="-march=i486"
        ;;
      i586 | pentium)
        gcc_cflags_cpu="-mtune=pentium -mcpu=pentium -m486"
        gcc_cflags_arch="-march=pentium"
        ;;
      pentiummmx)
        gcc_cflags_cpu="-mtune=pentium-mmx -mcpu=pentium-mmx -mcpu=pentium -m486"
        gcc_cflags_arch="-march=pentium-mmx -march=pentium"
        ;;
      i686 | pentiumpro)
        gcc_cflags_cpu="-mtune=pentiumpro -mcpu=pentiumpro -mcpu=i486 -m486"
        gcc_cflags_arch="-march=pentiumpro -march=pentium"
        ;;
      pentium2)
        gcc_cflags_cpu="-mtune=pentium2 -mcpu=pentium2 -mcpu=pentiumpro -mcpu=i486 -m486"
        gcc_cflags_arch="-march=pentium2 -march=pentiumpro -march=pentium"
        ;;
      pentium3)
        gcc_cflags_cpu="-mtune=pentium3 -mcpu=pentium3 -mcpu=pentiumpro -mcpu=i486 -m486"
        gcc_cflags_arch="-march=pentium3 -march=pentiumpro -march=pentium"
        ;;
      core)
        gcc_cflags_cpu="-mtune=pentium-m -mtune=pentium3m -mtune=pentium3 -mcpu=pentium3 -mcpu=pentiumpro -mcpu=i486 -m486"
        gcc_cflags_arch="-march=pentium-m -march=pentium3m -march=pentium3 -march=pentiumpro -march=pentium"
        ;;
      k6)
        gcc_cflags_cpu="-mtune=k6 -mcpu=k6 -mcpu=i486 -m486"
        gcc_cflags_arch="-march=k6"
        ;;
      k62)
        gcc_cflags_cpu="-mtune=k6-2 -mcpu=k6-2 -mcpu=k6 -mcpu=i486 -m486"
        gcc_cflags_arch="-march=k6-2 -march=k6"
        ;;
      k63)
        gcc_cflags_cpu="-mtune=k6-3 -mcpu=k6-3 -mcpu=k6 -mcpu=i486 -m486"
        gcc_cflags_arch="-march=k6-3 -march=k6"
        ;;
      k7 | athlon)
        # Athlon instruction costs are close to P6 (3 cycle load latency,
        # 4-6 cycle mul, 40 cycle div, pairable adc, etc) so if gcc doesn't
        # know athlon (eg. 2.95.2 doesn't) then fall back on pentiumpro.
        gcc_cflags_cpu="-mtune=athlon -mcpu=athlon -mcpu=pentiumpro -mcpu=i486 -m486"
        gcc_cflags_arch="-march=athlon -march=pentiumpro -march=pentium"
        ;;
      i786 | pentium4)
        # pentiumpro is the primary fallback when gcc doens't know pentium4.
        # This gets us cmov to eliminate branches.  Maybe "athlon" would be
        # a possibility on gcc 3.0.
        #
        gcc_cflags_cpu="-mtune=pentium4 -mcpu=pentium4 -mcpu=pentiumpro -mcpu=i486 -m486"
        gcc_cflags_arch="-march=pentium4 -march=pentium4~-mno-sse2 -march=pentiumpro -march=pentium"
        ;;
      prescott)
        # prescott is defined for our purposes as 32 bit pentium4 with SSE3
        gcc_cflags_cpu="-mtune=prescott -mtune=pentium4 -mcpu=pentium4 -mcpu=pentiumpro -mcpu=i486 -m486"
        gcc_cflags_arch="-march=prescott -march=pentium4~-mno-sse2 -march=pentiumpro -march=pentium"
        ;;
      netburst | netburstlahf)
        # prescott is defined for our purposes as 32 bit pentium4 with SSE3
        gcc_cflags_cpu="-mtune=nocona -mtune=pentium4 -mcpu=pentium4 -mcpu=pentiumpro -mcpu=i486 -m486"
        gcc_cflags_arch="-march=nocona -march=pentium4~-mno-sse2 -march=pentiumpro -march=pentium"
        ;;
      nano | viac32)
        # Not sure of the best fallbacks here for -mcpu.
        # c3-2 has sse and mmx, so pentium3 is good for -march.
        gcc_cflags_cpu="-mtune=c3-2 -mcpu=c3-2 -mcpu=i486 -m486"
        gcc_cflags_arch="-march=c3-2 -march=pentium3 -march=pentiumpro -march=pentium"
        ;;
      viac3*)
        # Not sure of the best fallbacks here.
        gcc_cflags_cpu="-mtune=c3 -mcpu=c3 -mcpu=i486 -m486"
        gcc_cflags_arch="-march=c3 -march=pentium-mmx -march=pentium"
        ;;
      x86_64 | k8)
        gcc_cflags_cpu="-mtune=k8 -mcpu=athlon -mcpu=pentiumpro -mcpu=i486 -m486"
        gcc_cflags_arch="-march=k8 -march=k8~-mno-sse2 -march=athlon -march=pentiumpro -march=pentium"
        ;;
      k10 | k102 | k103)
        gcc_cflags_cpu="-mtune=amdfam10 -mtune=k8 -mcpu=athlon -mcpu=pentiumpro -mcpu=i486 -m486"
        gcc_cflags_arch="-march=amdfam10 -march=k8 -march=k8~-mno-sse2 -march=athlon -march=pentiumpro -march=pentium"
        ;;
      bulldozer)
        gcc_cflags_cpu="-mtune=bdver1 -mtune=amdfam10 -mtune=k8 -mcpu=athlon -mcpu=pentiumpro -mcpu=i486 -m486"
        gcc_cflags_arch="-march=bdver1 -march=amdfam10 -march=k8 -march=k8~-mno-sse2 -march=athlon -march=pentiumpro -march=pentium"
        ;;
      piledriver)
        gcc_cflags_cpu="-mtune=bdver2 -mtune=amdfam10 -mtune=k8 -mcpu=athlon -mcpu=pentiumpro -mcpu=i486 -m486"
        gcc_cflags_arch="-march=bdver2 -march=amdfam10 -march=k8 -march=k8~-mno-sse2 -march=athlon -march=pentiumpro -march=pentium"
        ;;
      bobcat)
        gcc_cflags_cpu="-mtune=btver1 -mtune=k8 -mcpu=athlon -mcpu=pentiumpro -mcpu=i486 -m486"
        gcc_cflags_arch="-march=btver1 -march=k8 -march=k8~-mno-sse2 -march=athlon -march=pentiumpro -march=pentium"
        ;;
      atom)
        gcc_cflags_cpu="-mtune=atom -mtune=k8 -mcpu=athlon -mcpu=pentiumpro -mcpu=i486 -m486"
        gcc_cflags_arch="-march=atom -march=k8 -march=k8~-mno-sse2 -march=athlon -march=pentiumpro -march=pentium"
        ;;
      core2 | penryn)
        gcc_cflags_cpu="-mtune=core2 -mtune=nocona -mtune=pentium3 -mcpu=pentiumpro -mcpu=i486 -m486"
        gcc_cflags_arch="-march=core2 -march=nocona -march=pentium3 -march=pentiumpro -march=pentium"
        ;;
      nehalem | westmere)
        gcc_cflags_cpu="-mtune=corei7 -mtune=core2 -mtune=nocona -mtune=pentium3 -mcpu=pentiumpro -mcpu=i486 -m486"
        gcc_cflags_arch="-march=corei7 -march=core2 -march=nocona -march=pentium3 -march=pentiumpro -march=pentium"
        ;;
      sandybridge | ivybridge | haswell)
        gcc_cflags_cpu="-mtune=corei7-avx -mtune=corei7 -mtune=core2 -mtune=nocona -mtune=pentium3 -mcpu=pentiumpro -mcpu=i486 -m486"
        gcc_cflags_arch="-march=corei7-avx -march=corei7 -march=core2 -march=nocona -march=pentium3 -march=pentiumpro -march=pentium"
        ;;
      skylake)
        gcc_cflags_cpu="-mtune=corei7 -mtune=core2 -mtune=nocona -mtune=pentium3 -mcpu=pentiumpro -mcpu=i486 -m486"
        gcc_cflags_arch="-march=corei7 -march=core2 -march=nocona -march=pentium3 -march=pentiumpro -march=pentium"
        ;;
      skylakeavx)
        gcc_cflags_cpu="-mtune=skylake -mtune=corei7-avx -mtune=corei7 -mtune=core2 -mtune=nocona -mtune=pentium3 -mcpu=pentiumpro -mcpu=i486 -m486"
        gcc_cflags_arch="-march=skylake -march=corei7-avx -march=corei7 -march=core2 -march=nocona -march=pentium3 -march=pentiumpro -march=pentium"
        ;;
      broadwell)
        gcc_cflags_cpu="-mtune=broadwell -mtune=corei7-avx -mtune=corei7 -mtune=core2 -mtune=nocona -mtune=pentium3 -mcpu=pentiumpro -mcpu=i486 -m486"
        gcc_cflags_arch="-march=broadwell -march=corei7-avx -march=corei7 -march=core2 -march=nocona -march=pentium3 -march=pentiumpro -march=pentium"
        ;;
      *)
        gcc_cflags_cpu="-mtune=i486 -mcpu=i486 -m486"
        gcc_cflags_arch="-march=i486"
        ;;
    esac

    case $host_cpu in
      i386*)                   path="x86/i386 x86" ;;
      i486*)                   path="x86/i486 x86" ;;
      i586 | pentium)          path="x86/pentium x86" ;;
      pentiummmx)              path="x86/pentium/mmx x86/pentium x86" ;;
      i686 | pentiumpro)       path="x86/p6 x86" ;;
      pentium2)                path="x86/p6/mmx x86/p6 x86" ;;
      pentium3)                path="x86/p6/p3mmx x86/p6/mmx x86/p6 x86";;
      core)                    path="x86/p6/p3mmx x86/p6/mmx x86/p6 x86";;
      [k6[23]])                path="x86/k6/k62mmx x86/k6/mmx x86/k6 x86" ;;
      k6)                      path="x86/k6/mmx x86/k6 x86" ;;
      athlon | k7 | x86_64 | atom)   path="x86/k7/mmx x86/k7 x86" ;;
      k102 | k103 | bulldozer | piledriver)       path="x86/k7/mmx/k8/k10/k102  x86/k7/mmx/k8/k10 x86/k7/mmx/k8 x86/k7/mmx x86/k7 x86" ;;
      k10)			path="x86/k7/mmx/k8/k10 x86/k7/mmx/k8 x86/k7/mmx x86/k7 x86" ;;
      k8 | bobcat)		path="x86/k7/mmx/k8 x86/k7/mmx x86/k7 x86" ;;
      core2 | penryn)		path="x86/core2 x86" ;;
      i786 | pentium4)		path="x86/pentium4/sse2 x86/pentium4/mmx x86/pentium4 x86" ;;
      nehalem | westmere | sandybridge | ivybridge | haswell | skylake | skylakeavx | broadwell)	path="x86/nehalem x86" ;;
      prescott | netburst | netburstlahf)       path="x86/pentium4/sse2 x86/pentium4/mmx x86/pentium4 x86" ;;
      # VIA/Centaur processors, sold as CyrixIII and C3.
      nano | viac32)           path="x86/p6/p3mmx x86/p6/mmx x86/p6 x86";;
      viac3*)                  path="x86/pentium/mmx x86/pentium x86";;
      *)                       path="x86" ;;
    esac

    # 32bit apple darwin doesn't like our PIC format asm code
    case $host in
	i[34567]86-apple-darwin* | pentium*-apple-darwin* | prescott-apple-darwin* | core-apple-darwin* )	path="x86/applenopic" ;;
	# assume Core2 or later
	*-apple-darwin* )	path="x86/applenopic/core2 x86/applenopic" ;;
	*)			;;
    esac

    # If the user asked for a fat build, override the path set above
    if test $enable_fat = yes; then
      gcc_cflags_cpu=""
      gcc_cflags_arch=""
      extra_functions="$extra_functions fat fat_entry"
      path="x86 x86/fat"
      fat_path="x86 x86/fat x86/i486
		x86/k6 x86/k6/mmx x86/k6/k62mmx
		x86/k7 x86/k7/mmx x86/k7/mmx/k8 x86/k7/mmx/k8/k10 x86/k7/mmx/k8/k10/k102
		x86/pentium x86/pentium/mmx
		x86/p6 x86/p6/mmx x86/p6/p3mmx
		x86/pentium4 x86/pentium4/mmx x86/pentium4/sse2
		x86/core2 x86/nehalem"
      fat_functions="add_n addmul_1 add_err1_n add_err2_n copyd copyi
		     divexact_1 divexact_by3c divexact_byfobm1 divrem_1 divrem_2
			 divrem_euclidean_qr_1
			divrem_euclidean_qr_2 gcd_1 lshift
		     mod_1 mod_34lsub1 modexact_1c_odd mul_1 mul_basecase
		     mulmid_basecase preinv_divrem_1 preinv_mod_1 redc_1 rshift
		     sqr_basecase sub_n submul_1 sumdiff_n sub_err1_n sub_err2_n"
      fat_exclude=""
      fat_thresholds="MUL_KARATSUBA_THRESHOLD MUL_TOOM3_THRESHOLD
		      SQR_KARATSUBA_THRESHOLD SQR_TOOM3_THRESHOLD"
    fi

    case $host in
      X86_64_PATTERN)
        abilist="64 32"
        cclist_64="gcc cc"
        gcc_64_cflags="-m64"
	gcc_cflags_opt="-O2 -O1"
	case $host in
	*-*-linux-gnu)
	    gcc_64_ldflags="-Wl,-z,noexecstack" ;;
	esac
        gcc_64_cflags_optlist="opt arch cpu"
        SPEED_CYCLECOUNTER_OBJ_64=x86_64.lo
        CALLING_CONVENTIONS_OBJS_64='x86_64call.lo x86_64check$U.lo'
        cyclecounter_size_64=2
        case $host in
           *-*-solaris* | *-*-sunos*)
           # Note no -g, it disables all optimizations.
           cc_64_cflags="-m64"
           cc_64_cflags_optlist="opt arch cpu"

           # SunOS cc doesn't know -xO4, fallback to -O2.
           cc_64_cflags_opt="-xO4 -O2" ;;
        esac

        case $host in
          # NOTE x86_64w and x86_64 paths MUST be the same , for the fat structure
          x86_64-pc-msys|x86_64-w64-mingw*|x86_64-*-cygwin*)
            path_64="x86_64w" ;;
          netburst-pc-msys|netburst-w64-mingw*|netburst-*-cygwin*)
            path_64="x86_64w/netburst x86_64w" ;;
          netburstlahf-pc-msys|netburstlahf-w64-mingw*|netburstlahf-*-cygwin*)
            path_64="x86_64w/netburst x86_64w" ;;
          k8-pc-msys|k8-w64-mingw*|k8-*-cygwin*)
            path_64="x86_64w/k8/k8only x86_64w/k8 x86_64w" ;;
          k10-pc-msys|k10-w64-mingw*|k10-*-cygwin*)
            path_64="x86_64w/k8/k10 x86_64w/k8 x86_64w" ;;
          k102-pc-msys|k102-w64-mingw*|k102-*-cygwin*)
            path_64="x86_64w/k8/k10/k102 x86_64w/k8/k10 x86_64w/k8 x86_64w" ;;
          k103-pc-msys|k103-w64-mingw*|k103-*-cygwin*)
            path_64="x86_64w/k8/k10/k102 x86_64w/k8/k10 x86_64w/k8 x86_64w" ;;
          bulldozer-pc-msys|bulldozer-w64-mingw*|bulldozer-*-cygwin*)
            path_64="x86_64w/bulldozer x86_64w/k8/k10/k102 x86_64w/k8/k10 x86_64w/k8 x86_64w" ;;
          piledriver-pc-msys|piledriver-w64-mingw*|piledriver-*-cygwin*)
            path_64="x86_64w/bulldozer/piledriver x86_64w/bulldozer x86_64w/k8/k10/k102 x86_64w/k8/k10 x86_64w/k8 x86_64w" ;;
          bobcat-pc-msys|bobcat-w64-mingw*|bobcat-*-cygwin*)
            path_64="x86_64w/bobcat x86_64w" ;;
          core2-pc-msys|core2-w64-mingw*|core2-*-cygwin*)
            path_64="x86_64w/core2 x86_64w" ;;
          penryn-pc-msys|penryn-w64-mingw*|penryn-*-cygwin*)
            path_64="x86_64w/core2/penryn x86_64w/core2 x86_64w" ;;
          nehalem-pc-msys|nehalem-w64-mingw*|nehalem-*-cygwin*)
            path_64="x86_64w/nehalem x86_64w" ;;
          westmere-pc-msys|westmere-w64-mingw*|westmere-*-cygwin*)
            path_64="x86_64w/nehalem/westmere x86_64w/nehalem x86_64w" ;;
          sandybridge-pc-msys|sandybridge-w64-mingw*|sandybridge-*-cygwin*)
            path_64="x86_64w/sandybridge x86_64w" ;;
          ivybridge-pc-msys|ivybridge-w64-mingw*|ivybridge-*-cygwin*)
            path_64="x86_64w/sandybridge/ivybridge x86_64w/sandybridge x86_64w" ;;
          haswell-pc-msys|haswell-w64-mingw*|haswell-*-cygwin*)
            path_64="x86_64w/haswell x86_64w/sandybridge x86_64w" ;;
          skylake-pc-msys|skylake-w64-mingw*|skylake-*-cygwin*)
            path_64="x86_64w/skylake x86_64w/sandybridge x86_64w" ;;
          skylakeavx-pc-msys|skylakeavx-w64-mingw*|skylakeavx-*-cygwin*)
            path_64="x86_64w/skylakeavx x86_64w/skylake x86_64w/haswell x86_64w/sandybridge x86_64w" ;;
          broadwell-pc-msys|broadwell-w64-mingw*|broadwell-*-cygwin*)
            path_64="x86_64w/haswell/broadwell x86_64w/haswell/avx x86_64w/haswell x86_64w/sandybridge x86_64w" ;;
          atom-pc-msys|atom-w64-mingw*|atom-*-cygwin*)
            path_64="x86_64w/atom x86_64w" ;;
          nano-pc-msys|nano-w64-mingw*|nano-*-cygwin*)
            path_64="x86_64w/k8/k8only x86_64w/k8 x86_64w" ;;

          x86_64-*-*)
            path_64="x86_64" ;;
          netburst-*-*)
            path_64="x86_64/netburst x86_64" ;;
          netburstlahf-*-*)
            path_64="x86_64/netburst x86_64" ;;
          k8-*-*)
            path_64="x86_64/k8/k8only x86_64/k8 x86_64" ;;
          k10-*-*)
            path_64="x86_64/k8/k10 x86_64/k8 x86_64" ;;
          k102-*-*)
            path_64="x86_64/k8/k10/k102 x86_64/k8/k10 x86_64/k8 x86_64" ;;
          k103-*-*)
            path_64="x86_64/k8/k10/k102 x86_64/k8/k10 x86_64/k8 x86_64" ;;
          bulldozer-*-*)
            path_64="x86_64/bulldozer x86_64/k8/k10/k102 x86_64/k8/k10 x86_64/k8 x86_64" ;;
          piledriver-*-*)
            path_64="x86_64/bulldozer/piledriver x86_64/bulldozer x86_64/k8/k10/k102 x86_64/k8/k10 x86_64/k8 x86_64" ;;
          bobcat-*-*)
            path_64="x86_64/bobcat x86_64" ;;
          core2-*-*)
            path_64="x86_64/core2 x86_64" ;;
          penryn-*-*)
            path_64="x86_64/core2/penryn x86_64/core2 x86_64" ;;
          nehalem-*-*)
            path_64="x86_64/nehalem x86_64/core2 x86_64" ;;
          westmere-*-*)
            path_64="x86_64/nehalem/westmere x86_64/nehalem x86_64/core2 x86_64" ;;
          sandybridge-*-*)
            path_64="x86_64/sandybridge x86_64/nehalem x86_64/core2 x86_64" ;;
          ivybridge-*-*)
            path_64="x86_64/sandybridge/ivybridge x86_64/sandybridge x86_64/nehalem x86_64/core2 x86_64" ;;
          haswell-*-*)
            path_64="x86_64/haswell/avx x86_64/haswell x86_64/sandybridge x86_64" ;;
          broadwell-*-*)
            path_64="x86_64/haswell/broadwell x86_64/haswell/avx x86_64/haswell x86_64/sandybridge x86_64" ;;
          skylake-*-*)
            path_64="x86_64/skylake x86_64/sandybridge x86_64" ;;
          skylakeavx-*-*)
            path_64="x86_64/skylake/avx x86_64/haswell/avx x86_64/skylake x86_64/haswell x86_64/sandybridge x86_64" ;;
          atom-*-*)
            path_64="x86_64/atom x86_64" ;;
          nano-*-*)
            path_64="x86_64/k8/k8only x86_64/k8 x86_64" ;;
        esac

        # If the user asked for a fat build, override the path set above
        if test $enable_fat = yes; then
          gcc_64_cflags_cpu=""
          gcc_64_cflags_arch=""
          extra_functions_64="$extra_functions_64 fat fat_entry"
          case $host in
            *-pc-msys|*-w64-mingw*|*-*-cygwin*)
              path_64="x86_64w x86_64w/fat"
              fat_path_64="x86_64w x86_64w/fat x86_64w/netburst x86_64w/k8 x86_64w/k8/k8only x86_64w/k8/k10 x86_64w/k8/k10/k102  x86_64w/core2 x86_64w/core2/penryn x86_64w/nehalem x86_64w/nehalem/westmere x86_64w/atom x86_64w/bobcat x86_64w/sandybridge x86_64w/haswell x86_64w/haswell/avx x86_64w/haswell/broadwell x86_64w/skylake x86_64w/skylake/avx" ;;
            *-*-*)
              path_64="x86_64 x86_64/fat"
              fat_path_64="x86_64 x86_64/fat x86_64/netburst x86_64/k8 x86_64/k8/k8only x86_64/k8/k10 x86_64/k8/k10/k102  x86_64/core2 x86_64/core2/penryn x86_64/nehalem x86_64/nehalem/westmere x86_64/atom x86_64/bobcat x86_64/bulldozer x86_64/bulldozer/piledriver x86_64/sandybridge x86_64/sandybridge/ivybridge x86_64/haswell x86_64/haswell/avx x86_64/haswell/broadwell x86_64/skylake x86_64/skylake/avx" ;;
          esac
        fi
        ;;

      esac
      ;;


  # Special CPU "none" selects generic C.
  #
  none-*-*)
    abilist="long longlong"
    cclist_long=$cclist
    gcc_long_cflags=$gcc_cflags
    cc_long_cflags=$cc_cflags
    cclist_longlong=$cclist
    gcc_longlong_cflags=$gcc_cflags
    cc_longlong_cflags=$cc_cflags
    limb_longlong=longlong
    ;;

esac

# mingw can be built by the cygwin gcc if -mno-cygwin is added.  For
# convenience add this automatically if it works.  Actual mingw gcc accepts
# -mno-cygwin too, but of course is the default.  mingw only runs on the
# x86s, but allow any CPU here so as to catch "none" too.
#
case $host in
  *-pc-msys|*-*-mingw*)
    gcc_cflags_optlist="$gcc_cflags_optlist nocygwin"
    gcc_cflags_nocygwin="-mno-cygwin"
    ;;
esac

CFLAGS_or_unset=${CFLAGS-'(unset)'}
CPPFLAGS_or_unset=${CPPFLAGS-'(unset)'}

cat >&AC_FD_CC <<EOF
User:
ABI=$ABI
CC=$CC
CFLAGS=$CFLAGS_or_unset
CPPFLAGS=$CPPFLAGS_or_unset
MPN_PATH=$MPN_PATH
GMP:
abilist=$abilist
cclist=$cclist
EOF


test_CFLAGS=${CFLAGS+set}
test_CPPFLAGS=${CPPFLAGS+set}

for abi in $abilist; do
  abi_last="$abi"
done

# If the user specifies an ABI then it must be in $abilist, after that
# $abilist is restricted to just that choice.
#
if test -n "$ABI"; then
  found=no
  for abi in $abilist; do
    if test $abi = "$ABI"; then found=yes; break; fi
  done
  if test $found = no; then
    AC_MSG_ERROR([ABI=$ABI is not among the following valid choices: $abilist])
  fi
  abilist="$ABI"
fi

found_compiler=no

for abi in $abilist; do

  echo "checking ABI=$abi"

  # Suppose abilist="64 32", then for abi=64, will have abi1="_64" and
  # abi2="_64".  For abi=32, will have abi1="_32" and abi2="".  This is how
  # $gcc_cflags becomes a fallback for $gcc_32_cflags (the last in the
  # abilist), but there's no fallback for $gcc_64_cflags.
  #
  abi1=[`echo _$abi | sed 's/[.]//g'`]
  if test $abi = $abi_last; then abi2=; else abi2="$abi1"; fi

  # Compiler choices under this ABI
                              eval cclist_chosen=\"\$cclist$abi1\"
  test -n "$cclist_chosen" || eval cclist_chosen=\"\$cclist$abi2\"

  # If there's a user specified $CC then don't use a list for
  # $cclist_chosen, just a single value for $ccbase.
  #
  if test -n "$CC"; then

    # The first word of $CC, stripped of any directory.  For instance
    # CC="/usr/local/bin/gcc -pipe" will give "gcc".
    #
    for ccbase in $CC; do break; done
    ccbase=`echo $ccbase | sed 's:.*/::'`

    # If this $ccbase is in $cclist_chosen then it's a compiler we know and
    # we can do flags defaulting with it.  If not, then $cclist_chosen is
    # set to "unrecognised" so no default flags are used.
    #
    # "unrecognised" is used to avoid bad effects with eval if $ccbase has
    # non-symbol characters.  For instance ccbase=my+cc would end up with
    # something like cflags="$my+cc_cflags" which would give
    # cflags="+cc_cflags" rather than the intended empty string for an
    # unknown compiler.
    #
    found=unrecognised
    for i in $cclist_chosen; do
      if test "$ccbase" = $i; then
        found=$ccbase
        break
      fi
    done
    cclist_chosen=$found
  fi

  for ccbase in $cclist_chosen; do

    # When cross compiling, look for a compiler with the $host_alias as a
    # prefix, the same way that AC_CHECK_TOOL does.  But don't do this to a
    # user-selected $CC.
    #
    # $cross_compiling will be yes/no/maybe at this point.  Do the host
    # prefixing for "maybe" as well as "yes".
    #
    if test "$cross_compiling" != no && test -z "$CC"; then
      cross_compiling_prefix="${host_alias}-"
    fi

    for ccprefix in $cross_compiling_prefix ""; do

      cc="$CC"
      test -n "$cc" || cc="$ccprefix$ccbase"

      # If the compiler is gcc but installed under another name, then change
      # $ccbase so as to use the flags we know for gcc.  This helps for
      # instance when specifying CC=gcc272 on Debian GNU/Linux, or the
      # native cc which is really gcc on NeXT or MacOS-X.
      #
      # FIXME: There's a slight misfeature here.  If cc is actually gcc but
      # gcc is not a known compiler under this $abi then we'll end up
      # testing it with no flags and it'll work, but chances are it won't be
      # in the right mode for the ABI we desire.  Let's quietly hope this
      # doesn't happen.
      #
      if test $ccbase != gcc; then
        GMP_PROG_CC_IS_GNU($cc,ccbase=gcc)
      fi

      # Similarly if the compiler is IBM xlc but invoked as cc or whatever
      # then change $ccbase and make the default xlc flags available.
      if test $ccbase != xlc; then
        GMP_PROG_CC_IS_XLC($cc,ccbase=xlc)
      fi

      # acc was Sun's first unbundled compiler back in the SunOS days, or
      # something like that, but today its man page says it's not meant to
      # be used directly (instead via /usr/ucb/cc).  The options are pretty
      # much the same as the main SunPRO cc, so share those configs.
      #
      case $host in
        *sparc*-*-solaris* | *sparc*-*-sunos*)
          if test "$ccbase" = acc; then ccbase=cc; fi ;;
      esac

      for tmp_cflags_maybe in yes no; do
                             eval cflags=\"\$${ccbase}${abi1}_cflags\"
        test -n "$cflags" || eval cflags=\"\$${ccbase}${abi2}_cflags\"

	if test "$tmp_cflags_maybe" = yes; then
          # don't try cflags_maybe when the user set CFLAGS
          if test "$test_CFLAGS" = set; then continue; fi
                                     eval cflags_maybe=\"\$${ccbase}${abi1}_cflags_maybe\"
          test -n "$cflags_maybe" || eval cflags_maybe=\"\$${ccbase}${abi2}_cflags_maybe\"
          # don't try cflags_maybe if there's nothing set
          if test -z "$cflags_maybe"; then continue; fi
          cflags="$cflags_maybe $cflags"
        fi

        # Any user CFLAGS, even an empty string, takes precendence
        if test "$test_CFLAGS" = set; then cflags=$CFLAGS; fi

        # Any user CPPFLAGS, even an empty string, takes precendence
                               eval cppflags=\"\$${ccbase}${abi1}_cppflags\"
        test -n "$cppflags" || eval cppflags=\"\$${ccbase}${abi2}_cppflags\"
        if test "$test_CPPFLAGS" = set; then cppflags=$CPPFLAGS; fi

        # --enable-profiling adds -p/-pg even to user-specified CFLAGS.
        # This is convenient, but it's perhaps a bit naughty to modify user
        # CFLAGS.
        case "$enable_profiling" in
          prof)       cflags="$cflags -p" ;;
          gprof)      cflags="$cflags -pg" ;;
          instrument) cflags="$cflags -finstrument-functions" ;;
        esac

        GMP_PROG_CC_WORKS($cc $cflags $cppflags,,continue)

        # If we're supposed to be using a "long long" for a limb, check that
        # it works.
                                  eval limb_chosen=\"\$limb$abi1\"
        test -n "$limb_chosen" || eval limb_chosen=\"\$limb$abi2\"
        if test "$limb_chosen" = longlong; then
          GMP_PROG_CC_WORKS_LONGLONG($cc $cflags $cppflags,,continue)
        fi

        # The tests to perform on this $cc, if any
                               eval testlist=\"\$${ccbase}${abi1}_testlist\"
        test -n "$testlist" || eval testlist=\"\$${ccbase}${abi2}_testlist\"
        test -n "$testlist" || eval testlist=\"\$any${abi1}_testlist\"
        test -n "$testlist" || eval testlist=\"\$any${abi2}_testlist\"

        testlist_pass=yes
        for tst in $testlist; do
          case $tst in
          gcc-arm-umodsi) GMP_GCC_ARM_UMODSI($cc,,testlist_pass=no) ;;
          gcc-mips-o32)   GMP_GCC_MIPS_O32($cc,,testlist_pass=no) ;;
          sizeof*)       GMP_C_TEST_SIZEOF($cc $cflags,$tst,,testlist_pass=no) ;;
          esac
          if test $testlist_pass = no; then break; fi
        done

        if test $testlist_pass = yes; then
          found_compiler=yes
          break
        fi
      done

      if test $found_compiler = yes; then break; fi
    done

    if test $found_compiler = yes; then break; fi
  done

  if test $found_compiler = yes; then break; fi
done


# If we recognised the CPU, as indicated by $path being set, then insist
# that we have a working compiler, either from our $cclist choices or from
# $CC.  We can't let AC_PROG_CC look around for a compiler because it might
# find one that we've rejected (for not supporting the modes our asm code
# demands, etc).
#
# If we didn't recognise the CPU (and this includes host_cpu=none), then
# fall through and let AC_PROG_CC look around for a compiler too.  This is
# mostly in the interests of following a standard autoconf setup, after all
# we've already tested cc and gcc adequately (hopefully).  As of autoconf
# 2.50 the only thing AC_PROG_CC really adds is a check for "cl" (Microsoft
# C on MS-DOS systems).
#
if test $found_compiler = no && test -n "$path"; then
  AC_MSG_ERROR([could not find a working compiler, see config.log for details])
fi


if test $found_compiler = yes; then

  # If we're creating CFLAGS, then look for optional additions.  If the user
  # set CFLAGS then leave it alone.
  #
  if test "$test_CFLAGS" != set; then
                          eval optlist=\"\$${ccbase}${abi1}_cflags_optlist\"
    test -n "$optlist" || eval optlist=\"\$${ccbase}${abi2}_cflags_optlist\"

    for opt in $optlist; do
                             eval optflags=\"\$${ccbase}${abi1}_cflags_${opt}\"
      test -n "$optflags" || eval optflags=\"\$${ccbase}${abi2}_cflags_${opt}\"
      test -n "$optflags" || eval optflags=\"\$${ccbase}_cflags_${opt}\"

      for flag in $optflags; do

	# ~ respresents a space in an option spec
        flag=`echo "$flag" | tr '~' ' '`

        case $flag in
          -march=pentium4 | -march=k8)
            # For -march settings which enable SSE2 we exclude certain bad
            # gcc versions and we need an OS knowing how to save xmm regs.
            #
            # This is only for ABI=32, any 64-bit gcc is good and any OS
            # knowing x86_64 will know xmm.
            #
            # -march=k8 was only introduced in gcc 3.3, so we shouldn't need
            # the GMP_GCC_PENTIUM4_SSE2 check (for gcc 3.2 and prior).  But
            # it doesn't hurt to run it anyway, sharing code with the
            # pentium4 case.
            #
            if test "$abi" = 32; then
              GMP_GCC_PENTIUM4_SSE2($cc $cflags $cppflags,, continue)
              GMP_OS_X86_XMM($cc $cflags $cppflags,, continue)
            fi
            ;;
          -no-cpp-precomp)
            # special check, avoiding a warning
            GMP_GCC_NO_CPP_PRECOMP($ccbase,$cc,$cflags,
                                   [cflags="$cflags $flag"
                                   break],
                                   [continue])
            ;;
          -Wa,-m*)
            ;;
        esac

        GMP_PROG_CC_WORKS($cc $cflags $cppflags $flag,
          [cflags="$cflags $flag"
          break])
      done
    done
  fi

  ABI="$abi"
  CC="$cc"
  CFLAGS="$cflags"
  CPPFLAGS="$cppflags"
  if test $enable_gmpcompat = yes; then
    GMP_CC="#define __GMP_CC __MPIR_CC"
    GMP_CFLAGS="#define __GMP_CFLAGS __MPIR_CFLAGS"
  else
    GMP_CC="/* No __GMP_CC here as --enable-gmpcompat option not selected */"
    GMP_CFLAGS=""
  fi

   AC_SUBST(GMP_CC)
   AC_SUBST(GMP_CFLAGS)

  # Could easily have this in config.h too, if desired.
  ABI_nodots=`echo $ABI | sed 's/\./_/'`
  GMP_DEFINE_RAW("define_not_for_expansion(\`HAVE_ABI_$ABI_nodots')", POST)


  # GMP_LDFLAGS substitution, selected according to ABI.
  # These are needed on libmpir.la and libmp.la, but currently not on
  # convenience libraries like tune/libspeed.la or mpz/libmpz.la.
  #
                            eval GMP_LDFLAGS=\"\$${ccbase}${abi1}_ldflags\"
  test -n "$GMP_LDFLAGS" || eval GMP_LDFLAGS=\"\$${ccbase}${abi1}_ldflags\"
  AC_SUBST(GMP_LDFLAGS)
  AC_SUBST(LIBGMP_LDFLAGS)
  AC_SUBST(LIBGMPXX_LDFLAGS)

  # extra_functions, selected according to ABI
                    eval tmp=\"\$extra_functions$abi1\"
  test -n "$tmp" || eval tmp=\"\$extra_functions$abi2\"
  extra_functions="$tmp"


  # Cycle counter, selected according to ABI.
  #
                    eval tmp=\"\$SPEED_CYCLECOUNTER_OBJ$abi1\"
  test -n "$tmp" || eval tmp=\"\$SPEED_CYCLECOUNTER_OBJ$abi2\"
  SPEED_CYCLECOUNTER_OBJ="$tmp"
                    eval tmp=\"\$cyclecounter_size$abi1\"
  test -n "$tmp" || eval tmp=\"\$cyclecounter_size$abi2\"
  cyclecounter_size="$tmp"

  if test -n "$SPEED_CYCLECOUNTER_OBJ"; then
    AC_DEFINE_UNQUOTED(HAVE_SPEED_CYCLECOUNTER, $cyclecounter_size,
    [Tune directory speed_cyclecounter, undef=none, 1=32bits, 2=64bits)])
  fi
  AC_SUBST(SPEED_CYCLECOUNTER_OBJ)


  # Calling conventions checking, selected according to ABI.
  #
                    eval tmp=\"\$CALLING_CONVENTIONS_OBJS$abi1\"
  test -n "$tmp" || eval tmp=\"\$CALLING_CONVENTIONS_OBJS$abi2\"
  CALLING_CONVENTIONS_OBJS="$tmp"

  if test -n "$CALLING_CONVENTIONS_OBJS"; then
    AC_DEFINE(HAVE_CALLING_CONVENTIONS,1,
    [Define to 1 if tests/libtests has calling conventions checking for the CPU])
  fi
  AC_SUBST(CALLING_CONVENTIONS_OBJS)

fi


# If the user gave an MPN_PATH, use that verbatim, otherwise choose
# according to the ABI and add "generic".
#
if test -n "$MPN_PATH"; then
  path="$MPN_PATH"
else
                    eval tmp=\"\$path$abi1\"
  test -n "$tmp" || eval tmp=\"\$path$abi2\"
  path="$tmp generic"
fi


# Long long limb setup for mpir.h.
case $limb_chosen in
longlong) DEFN_LONG_LONG_LIMB="#define _LONG_LONG_LIMB 1"    ;;
*)        DEFN_LONG_LONG_LIMB="/* #undef _LONG_LONG_LIMB */" ;;
esac
AC_SUBST(DEFN_LONG_LONG_LIMB)

AC_GNU_SOURCE

# The C compiler and preprocessor, put into ANSI mode if possible.
AC_PROG_CC
AC_PROG_CC_STDC
AC_PROG_CPP
GMP_H_ANSI


# The C compiler on the build system, and associated tests.
GMP_PROG_CC_FOR_BUILD
GMP_PROG_CPP_FOR_BUILD
GMP_PROG_EXEEXT_FOR_BUILD
GMP_C_FOR_BUILD_ANSI
GMP_CHECK_LIBM_FOR_BUILD

# How to assemble, used with CFLAGS etc, see mpn/Makeasm.am.
# Using the compiler is a lot easier than figuring out how to invoke the
# assembler directly.
#
test -n "$CCAS" || CCAS="$CC -c"
AC_SUBST(CCAS)

# If the user provided its Yasm, check it seems functional
MPIR_AS=""
if test -z "$with_yasm"; then
  echo "Looking for a system-wide yasm..."
  MPIR_AS=`which yasm`
  if test $? -ne 0; then
    AC_MSG_ERROR([no system-wide yasm found])
  fi
else
  MPIR_AS="$with_yasm"
fi
echo "Checking yasm..."
if ! test -f "$MPIR_AS" || ! test -x "$MPIR_AS"; then
  AC_MSG_ERROR([$MPIR_AS does not seem functional])
fi
AC_SUBST(MPIR_AS)

# The C++ compiler, if desired.

test_CXXFLAGS=${CXXFLAGS+set}

AC_PROG_CXX

# Now actually check that the C++ compiler works if it is needed.
want_cxx=no
if test $enable_cxx != no; then

  echo "CXXFLAGS chosen by autoconf: $CXXFLAGS" >&AC_FD_CC
  cxxflags_ac_prog_cxx=$CXXFLAGS
  cxxflags_list=ac_prog_cxx

  # If the user didn't specify $CXXFLAGS, then try $CFLAGS, with -g removed
  # if AC_PROG_CXX thinks that doesn't work.  $CFLAGS stands a good chance
  # of working, eg. on a GNU system where CC=gcc and CXX=g++.
  #
  if test "$test_CXXFLAGS" != set; then
    cxxflags_cflags=$CFLAGS
    cxxflags_list="cflags $cxxflags_list"
    if test "$ac_prog_cxx_g" = no; then
      cxxflags_cflags=`echo "$cxxflags_cflags" | sed -e 's/ -g //' -e 's/^-g //' -e 's/ -g$//'`
    fi
  fi

  # See if the C++ compiler works.  If the user specified CXXFLAGS then all
  # we're doing is checking whether AC_PROG_CXX succeeded, since it doesn't
  # give a fatal error, just leaves CXX set to a default g++.  If on the
  # other hand the user didn't specify CXXFLAGS then we get to try here our
  # $cxxflags_list alternatives.
  #
  # Automake includes $CPPFLAGS in a C++ compile, so we do the same here.
  #
  for cxxflags_choice in $cxxflags_list; do
    eval CXXFLAGS=\"\$cxxflags_$cxxflags_choice\"
    GMP_PROG_CXX_WORKS($CXX $CPPFLAGS $CXXFLAGS,
      [want_cxx=yes
      break])
  done

  # If --enable-cxx=yes but a C++ compiler can't be found, then abort.
  if test $want_cxx = no && test $enable_cxx = yes; then
    AC_MSG_ERROR([C++ compiler not available, see config.log for details])
  fi
fi

AM_CONDITIONAL(WANT_CXX, test $want_cxx = yes)

if test -z "$MPN_PATH"; then
  path="$add_path $path"
fi

# For a nail build, also look in "nails" subdirectories.
#
if test $GMP_NAIL_BITS != 0 && test -z "$MPN_PATH"; then
  new_path=
  for i in $path; do
    case $i in
    generic) new_path="$new_path $i" ;;
    *)       new_path="$new_path $i/nails $i" ;;
    esac
  done
  path=$new_path
fi


# Put all directories into CPUVEC_list so as to get a full set of
# CPUVEC_SETUP_$tmp_suffix defines into config.h, even if some of them are
# empty because mmx and/or sse2 had to be dropped.
# For MINGW64 we have have to hack this so as x86_64w gets turned into x86_64
for i in $fat_path; do
  GMP_FAT_SUFFIX(tmp_suffix, $i)
  CPUVEC_list="$CPUVEC_list CPUVEC_SETUP_$tmp_suffix"
done


# If there's any sse2 or mmx in the path, check whether the assembler
# supports it, and remove if not.
#
# We only need this in ABI=32, for ABI=64 on x86_64 we can assume a new
# enough assembler.
#
case $host in
  X86_PATTERN | X86_64_PATTERN)
    if test "$ABI" = 32; then
      case "$path $fat_path" in
        *mmx*)   GMP_ASM_X86_MMX( , [GMP_STRIP_PATH(*mmx*)]) ;;
      esac
      case "$path $fat_path" in
        *sse2*)  GMP_ASM_X86_SSE2( , [GMP_STRIP_PATH(sse2)]) ;;
      esac
    fi
    ;;
esac


cat >&AC_FD_CC <<EOF
Decided:
ABI=$ABI
CC=$CC
CFLAGS=$CFLAGS
CPPFLAGS=$CPPFLAGS
GMP_LDFLAGS=$GMP_LDFLAGS
CXX=$CXX
CXXFLAGS=$CXXFLAGS
path=$path
EOF
echo "using ABI=\"$ABI\""
echo "      CC=\"$CC\""
echo "      CFLAGS=\"$CFLAGS\""
echo "      CPPFLAGS=\"$CPPFLAGS\""
if test $want_cxx = yes; then
  echo "      CXX=\"$CXX\""
  echo "      CXXFLAGS=\"$CXXFLAGS\""
fi
echo "      MPN_PATH=\"$path\""


GMP_PROG_AR
GMP_PROG_NM

case $host in
  # FIXME: On AIX 3 and 4, $libname.a is included in libtool
  # $library_names_spec, so libmpir.a becomes a symlink to libmpir.so, making
  # it impossible to build shared and static libraries simultaneously.
  # Disable shared libraries by default, but let the user override with
  # --enable-shared --disable-static.
  #
  # FIXME: This $libname.a problem looks like it might apply to *-*-amigaos*
  # and *-*-os2* too, but wait for someone to test this before worrying
  # about it.  If there is a problem then of course libtool is the right
  # place to fix it.
  #
  [*-*-aix[34]*])
    if test -z "$enable_shared"; then enable_shared=no; fi ;;
esac


# Enable various configuration flags and DLL on Windows
LT_INIT([win32-dll])

# Configs for Windows DLLs.

AC_SUBST(LIBGMP_DLL,0)
case $host in
  *-pc-msys|*-*-cygwin* | *-*-mingw*)
    # By default, build only static.
    if test -z "$enable_shared"; then
      enable_shared=no
    fi
    # Don't allow both static and DLL.
    if test "$enable_shared" != no && test "$enable_static" != no; then
      AC_MSG_ERROR([cannot build both static and DLL, since mpir.h is different for each.
Use "--disable-static --enable-shared" to build just a DLL.])
    fi

    # "-no-undefined" is required when building a DLL, see documentation on
    # AC_LIBTOOL_WIN32_DLL.
    #
    # "-Wl,--export-all-symbols" is a bit of a hack, it gets all libmpir and
    # libmpirxx functions and variables exported.  This is what libtool did
    # in the past, and it's convenient for us in the test programs.
    #
    # Maybe it'd be prudent to check for --export-all-symbols before using
    # it, but it seems to have been in ld since at least 2000, and there's
    # not really any alternative we want to take up at the moment.
    #
    # "-Wl,output-def" is used to get a .def file for use by MS lib to make
    # a .lib import library, described in the manual.  libmpir-3.dll.def
    # corresponds to the libmp-3.dll.def generated by libtool (as a result
    # of -export-symbols on that library).
    #
    # Incidentally, libtool does generate an import library libmpir.dll.a,
    # but it's "ar" format and cannot be used by the MS linker.  There
    # doesn't seem to be any GNU tool for generating or converting to .lib.
    #
    # FIXME: The .def files produced by -Wl,output-def include isascii,
    # iscsym, iscsymf and toascii, apparently because mingw ctype.h doesn't
    # inline isascii (used in gmp).  It gives an extern inline for
    # __isascii, but for some reason not the plain isascii.
    #
    # LDFLAGS="$LDFLAGS -Wl,--enable-auto-import"  this is too general
    if test "$enable_shared" = yes; then
      GMP_LDFLAGS="$GMP_LDFLAGS -no-undefined -Wl,--export-all-symbols"
      LIBGMP_LDFLAGS="$LIBGMP_LDFLAGS -Wl,--output-def,.libs/libmpir-3.dll.def"
      LIBGMPXX_LDFLAGS="$LIBGMP_LDFLAGS -Wl,--output-def,.libs/libmpirxx-3.dll.def"
      LIBGMP_DLL=1
    fi
    ;;
esac

# Generate an error here if attempting to build both shared and static when
# $libname.a is in $library_names_spec (as mentioned above), rather than
# wait for ar or ld to fail.
#
if test "$enable_shared" = yes && test "$enable_static" = yes; then
  case $library_names_spec in
    *libname.a*)
      AC_MSG_ERROR([cannot create both shared and static libraries on this system, --disable one of the two])
      ;;
  esac
fi

AM_CONDITIONAL(ENABLE_STATIC, test "$enable_static" = yes)
AM_CONDITIONAL(ENABLE_SHARED, test "$enable_shared" = yes)


# Many of these library and header checks are for the benefit of
# supplementary programs.  libmpir doesn't use anything too weird.

AC_HEADER_STDC
AC_HEADER_TIME

# Reasons for testing:
#   float.h - not in SunOS bundled cc
#   langinfo.h - X/Open standard only, not in djgpp for instance
#   locale.h - old systems won't have this
#   nl_types.h - X/Open standard only, not in djgpp for instance
#       (usually langinfo.h gives nl_item etc, but not on netbsd 1.4.1)
#   sys/mman.h - not in Cray Unicos
#   sys/param.h - not in mingw
#   sys/processor.h - solaris specific, though also present in macos
#   sys/pstat.h - HPUX specific
#   sys/resource.h - not in mingw
#   sys/sysctl.h - not in mingw
#   sys/sysinfo.h - OSF specific
#   sys/systemcfg.h - AIX specific
#   sys/time.h - autoconf suggests testing, don't know anywhere without it
#   sys/times.h - not in mingw
#
# inttypes.h, stdint.h, unistd.h and sys/types.h are already in the autoconf
# default tests
#
AC_CHECK_HEADERS(fenv.h fcntl.h float.h langinfo.h locale.h nl_types.h sys/mman.h sys/param.h sys/processor.h sys/pstat.h sys/sysinfo.h sys/systemcfg.h sys/time.h sys/times.h)

# On SunOS, sys/resource.h needs sys/time.h (for struct timeval)
AC_CHECK_HEADERS(sys/resource.h,,,
[#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif])

# On NetBSD and OpenBSD, sys/sysctl.h needs sys/param.h for various constants
AC_CHECK_HEADERS(sys/sysctl.h,,,
[#if HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif])

# Reasons for testing:
#   optarg - not declared in mingw
#   fgetc, fscanf, ungetc, vfprintf - not declared in SunOS 4
#   sys_errlist, sys_nerr - not declared in SunOS 4
#
# optarg should be in unistd.h and the rest in stdio.h, both of which are
# in the autoconf default includes.
#
# sys_errlist and sys_nerr are supposed to be in <errno.h> on SunOS according
# to the man page (but aren't), in glibc they're in stdio.h.
#
AC_CHECK_DECLS([fgetc, fscanf, optarg, ungetc, vfprintf])
AC_CHECK_DECLS([sys_errlist, sys_nerr], , ,
[#include <stdio.h>
#include <errno.h>])

# Reasons for testing:
#   intmax_t       - C99
#   long double    - not in the HP bundled K&R cc
#   long long      - only in reasonably recent compilers
#   quad_t         - BSD specific
#   uint_least32_t - C99
#
# the default includes are sufficient for all these types
#
AC_CHECK_TYPES([intmax_t, uintmax_t, long double, long long, quad_t, uint_least32_t])

AC_C_STRINGIZE

# FIXME: Really want #ifndef __cplusplus around the #define volatile
# replacement autoconf gives, since volatile is always available in C++.
# But we don't use it in C++ currently.
AC_C_VOLATILE

# AC_C_RESTRICT

GMP_C_STDARG
GMP_C_ATTRIBUTE_CONST
GMP_C_ATTRIBUTE_MALLOC
GMP_C_ATTRIBUTE_MODE
GMP_C_ATTRIBUTE_NORETURN

GMP_H_EXTERN_INLINE

# from libtool
LT_LIB_M
AC_SUBST(LIBM)

GMP_FUNC_ALLOCA
GMP_OPTION_ALLOCA

GMP_H_HAVE_FILE

AC_C_BIGENDIAN(
  [AC_DEFINE(HAVE_LIMB_BIG_ENDIAN, 1)
   GMP_DEFINE_RAW("define_not_for_expansion(\`HAVE_LIMB_BIG_ENDIAN')", POST)],
  [AC_DEFINE(HAVE_LIMB_LITTLE_ENDIAN, 1)
   GMP_DEFINE_RAW("define_not_for_expansion(\`HAVE_LIMB_LITTLE_ENDIAN')", POST)
  ], [:])
AH_VERBATIM([HAVE_LIMB],
[/* Define one of these to 1 for the endianness of `mp_limb_t'.
   If the endianness is not a simple big or little, or you don't know what
   it is, then leave both undefined. */
#undef HAVE_LIMB_BIG_ENDIAN
#undef HAVE_LIMB_LITTLE_ENDIAN])

GMP_C_DOUBLE_FORMAT


# Reasons for testing:
#   alarm - not in mingw
#   attr_get - IRIX specific
#   clock_gettime - not in glibc 2.2.4, only very recent systems
#   cputime - not in glibc
#   getsysinfo - OSF specific
#   getrusage - not in mingw
#   gettimeofday - not in mingw
#   mmap - not in mingw, djgpp
#   nl_langinfo - X/Open standard only, not in djgpp for instance
#   obstack_vprintf - glibc specific
#   processor_info - solaris specific
#   pstat_getprocessor - HPUX specific (10.x and up)
#   raise - an ANSI-ism, though probably almost universal by now
#   read_real_time - AIX specific
#   strerror - not in SunOS
#   strnlen - glibc extension (some other systems too)
#   syssgi - IRIX specific
#   times - not in mingw
#
# clock_gettime is in librt on *-*-osf5.1.  We could look for it
# there, but that's not worth bothering with unless it has a decent
# resolution (in a quick test clock_getres said only 1 millisecond).
#
# AC_FUNC_STRNLEN is not used because we don't want the AC_LIBOBJ
# replacement setups it gives.  It detects a faulty strnlen on AIX, but
# missing out on that test is ok since our only use of strnlen is in
# __gmp_replacement_vsnprintf which is not required on AIX since it has a
# vsnprintf.
#
AC_CHECK_FUNCS(alarm attr_get clock clock_gettime cputime getpagesize getrusage gettimeofday getsysinfo localeconv mmap nl_langinfo obstack_vprintf popen processor_info pstat_getprocessor raise read_real_time syssgi strerror strnlen strtol strtoul sysconf sysctl sysctlbyname times)

# mingw returns OK on mprotect , but it's broken
#
case $host in
  *-pc-msys|*-*-mingw*)
    ;;
  *)
    AC_CHECK_FUNCS(mprotect)
esac

GMP_FUNC_VSNPRINTF
GMP_FUNC_SSCANF_WRITABLE_INPUT

# Reasons for checking:
#   pst_processor psp_iticksperclktick - not in hpux 9
#
AC_CHECK_MEMBER(struct pst_processor.psp_iticksperclktick,
                [AC_DEFINE(HAVE_PSP_ITICKSPERCLKTICK, 1,
[Define to 1 if <sys/pstat.h> `struct pst_processor' exists
and contains `psp_iticksperclktick'.])],,
                [#include <sys/pstat.h>])

# C++ tests, when required
#
if test $enable_cxx = yes; then
  AC_LANG_PUSH(C++)

  # Reasons for testing:
  #   <sstream> - not in g++ 2.95.2
  #   std::locale - not in g++ 2.95.4
  #
  AC_CHECK_HEADERS([sstream])
  AC_CHECK_TYPES([std::locale],,,[#include <locale>])

  AC_LANG_POP(C++)
fi


# Pick the correct source files in $path and link them to mpn/.
# $gmp_mpn_functions lists all functions we need.
#
# The rule is to find a file with the function name and a .asm, .S,
# .s, or .c extension.  Certain multi-function files with special names
# can provide some functions too.  (mpn/Makefile.am passes
# -DOPERATION_<func> to get them to generate the right code.)

# Note: $gmp_mpn_functions must have mod_1 before preinv_mod_1 so the former
#       can optionally provide the latter as an extra entrypoint.  Likewise
#       divrem_1 and preinv_divrem_1.

gmp_mpn_functions_optional="umul udiv copyi copyd com_n neg_n
  and_n andn_n nand_n ior_n iorn_n nior_n xor_n xnor_n			\
  invert_limb sqr_diagonal lshift1 rshift1 lshift2 rshift2	\
  mul_2 mul_3 mul_4 store not karaadd karasub half double		\
  addmul_2 addmul_3 addmul_4 addmul_5 addmul_6 addmul_7 addmul_8	\
  addlsh1_n sublsh1_n rsh1add_n rsh1sub_n sumdiff_n lshiftc		\
  addlsh_n sublsh_n inclsh_n declsh_n nsumdiff_n"

gmp_mpn_functions="$extra_functions					   \
  add add_1 add_n sub sub_1 sub_n mul_1 addmul_1 submul_1 lshift rshift    \
  addadd_n addsub_n subadd_n		\
  comb_tables divexact_1 divexact_by3c divexact_byff divexact_byfobm1 divisible_p divrem divrem_1 divrem_2       \
  divrem_euclidean_qr_1 divrem_euclidean_qr_2 divrem_euclidean_r_1	\
  divrem_hensel_qr_1 divrem_hensel_qr_1_1 divrem_hensel_qr_1_2		\
  divrem_hensel_r_1 divrem_hensel_rsh_qr_1 rsh_divrem_hensel_qr_1	\
  rsh_divrem_hensel_qr_1_1 rsh_divrem_hensel_qr_1_2			\
  add_err1_n add_err2_n sub_err1_n sub_err2_n				   \
  fib2_ui mod_1 mod_34lsub1 modexact_1c_odd preinv_divrem_1 preinv_mod_1 dump		   \
  gcd_subdiv_step gcdext_lehmer gcdext_1 hgcd matrix22_mul hgcd2            \
  hgcd2_jacobi hgcd_appr hgcd_jacobi hgcd_matrix hgcd_reduce hgcd_step \
  matrix22_mul1_inverse_vector \
  mod_1_1 mod_1_2 mod_1_3 tdiv_q mp_bases fib_table			\
  mulmid_basecase mulmid mulmid_n toom42_mulmid	mulmod_bexpp1 mulmod_2expm1 \
  mulmod_2expp1_basecase mul_fft \
  mul mul_n mul_basecase sqr_basecase random random2 	   \
  pow_1 powlo powm binvert \
  urandomb urandomm randomb rrandom invert \
  rootrem sizeinbase sqrtrem get_str set_str scan0 scan1 popcount hamdist cmp perfect_square_p \
  bdivmod gcd gcd_1 gcdext tdiv_qr jacobi_base jacobi jacobi_2 get_d  \
  mullow_n mulhigh_n mullow_n_basecase mullow_basecase \
  redc_1 redc_2 redc_n \
  sb_divappr_q toom3_mul toom3_mul_n toom4_mul toom4_mul_n \
  dc_div_q dc_divappr_q sb_div_q sb_div_qr dc_div_qr dc_div_qr_n inv_divappr_q_n \
  inv_divappr_q inv_div_q inv_div_qr inv_div_qr_n rootrem_basecase \
  toom_eval_dgr3_pm1 toom_eval_dgr3_pm2 toom_eval_pm1 toom_eval_pm2 \
  toom_eval_pm2exp toom_eval_pm2rexp toom_interpolate_16pts \
  toom8_sqr_n toom8h_mul toom_couple_handling sb_bdiv_q sb_bdiv_qr \
  dc_bdiv_q_n dc_bdiv_q dc_bdiv_qr dc_bdiv_qr_n divexact zero $gmp_mpn_functions_optional"

# should be able to remove the mulfunc below
define(GMP_MULFUNC_CHOICES,
[# functions that can be provided by multi-function files
tmp_mulfunc=
case $tmp_fn in
  and_n|andn_n|nand_n | ior_n|iorn_n|nior_n | xor_n|xnor_n)
                     tmp_mulfunc="logops_n"  ;;
esac
])

# the list of all object files used by mpn/Makefile.in and the
# top-level Makefile.in, respectively
mpn_objects=
mpn_objs_in_libgmp=

# links from the sources, to be removed by "make distclean"
gmp_srclinks=


# mpn_relative_top_srcdir is $top_srcdir, but for use from within the mpn
# build directory.  If $srcdir is relative then we use a relative path too,
# so the two trees can be moved together.
case $srcdir in
  [[\\/]* | ?:[\\/]*])  # absolute, as per autoconf
    mpn_relative_top_srcdir=$srcdir ;;
  *)                    # relative
    mpn_relative_top_srcdir=../$srcdir ;;
esac


define(MPN_SUFFIXES,[as asm S s c])


dnl  Usage: GMP_FILE_TO_FUNCTION_BASE(func,file)
dnl
dnl  Set $func to the function base name for $file, eg. dive_1 gives
dnl  divexact_1.
dnl
define(GMP_FILE_TO_FUNCTION,
[case $$2 in
  *)         	$1=$$2 ;;
esac
])

# Fat binary setups.
#
# We proceed through each $fat_path directory, and look for $fat_function
# routines there.  Those found are incorporated in the build by generating a
# little mpn/<foo>.asm or mpn/<foo>.c file in the build directory, with
# suitable function renaming, and adding that to $mpn_objects (the same as a
# normal mpn file).
#
# fat.h is generated with macros to let internal calls to each $fat_function
# go directly through __gmpn_cpuvec, plus macros and declarations helping to
# setup that structure, on a per-directory basis ready for
# mpn/<cpu>/fat/fat.c.
#
# fat.h includes thesholds listed in $fat_thresholds, extracted from
# gmp-mparam.h in each directory.  An overall maximum for each threshold is
# established, for use in making fixed size arrays of temporary space.
# (Eg. MUL_TOOM3_THRESHOLD_LIMIT used by mpn/generic/mul.c.)
#
# It'd be possible to do some of this manually, but when there's more than a
# few functions and a few directories it becomes very tedious, and very
# prone to having some routine accidentally omitted.  On that basis it seems
# best to automate as much as possible, even if the code to do so is a bit
# ugly.
#


if test -n "$fat_path"; then
  if test "$ABI" = 64; then
    fat_path="$fat_path_64"
  fi

  # Usually the mpn build directory is created with mpn/Makefile
  # instantiation, but we want to write to it sooner.
  mkdir mpn 2>/dev/null

  echo "/* fat.h - setups for fat binaries." >fat.h
  echo "   Generated by configure - DO NOT EDIT.  */" >>fat.h

  AC_DEFINE(WANT_FAT_BINARY, 1, [Define to 1 when building a fat binary.])
  GMP_DEFINE(WANT_FAT_BINARY, yes)

  # Don't want normal copies of fat functions
  for tmp_fn in $fat_functions; do
    GMP_REMOVE_FROM_LIST(gmp_mpn_functions, $tmp_fn)
    GMP_REMOVE_FROM_LIST(gmp_mpn_functions_optional, $tmp_fn)
  done

  for tmp_fn in $fat_functions; do
    GMP_FILE_TO_FUNCTION(tmp_fbase,tmp_fn)
    echo "
#ifndef OPERATION_$tmp_fn
#undef  mpn_$tmp_fbase
#define mpn_$tmp_fbase  (*__gmpn_cpuvec.$tmp_fbase)
#endif
DECL_$tmp_fbase (__MPN(${tmp_fbase}_init));" >>fat.h
    # encourage various macros to use fat functions
    AC_DEFINE_UNQUOTED(HAVE_NATIVE_$tmp_fbase)
  done

  echo "" >>fat.h
  echo "/* variable thresholds */" >>fat.h
  for tmp_tn in $fat_thresholds; do
    echo "#undef  $tmp_tn" >>fat.h
    echo "#define $tmp_tn  CPUVEC_THRESHOLD (`echo $tmp_tn | tr '[A-Z]' '[a-z]'`)" >>fat.h
  done

  echo "
/* Copy all fields into __gmpn_cpuvec.
   memcpy is not used because it might operate byte-wise (depending on its
   implemenation), and we need the function pointer writes to be atomic.
   "volatile" discourages the compiler from trying to optimize this.  */
#define CPUVEC_INSTALL(vec) \\
  do { \\
    volatile struct cpuvec_t *p = &__gmpn_cpuvec; \\" >>fat.h
  for tmp_fn in $fat_functions; do
    GMP_FILE_TO_FUNCTION(tmp_fbase,tmp_fn)
    echo "    p->$tmp_fbase = vec.$tmp_fbase; \\" >>fat.h
  done
  for tmp_tn in $fat_thresholds; do
    tmp_field_name=`echo $tmp_tn | tr '[[A-Z]]' '[[a-z]]'`
    echo "    p->$tmp_field_name = vec.$tmp_field_name; \\" >>fat.h
  done
  echo "  } while (0)" >>fat.h

  echo "
/* A helper to check all fields are filled. */
#define ASSERT_CPUVEC(vec) \\
  do { \\" >>fat.h
  for tmp_fn in $fat_functions; do
    GMP_FILE_TO_FUNCTION(tmp_fbase,tmp_fn)
    echo "    ASSERT (vec.$tmp_fbase != NULL); \\" >>fat.h
  done
  for tmp_tn in $fat_thresholds; do
    tmp_field_name=`echo $tmp_tn | tr '[[A-Z]]' '[[a-z]]'`
    echo "    ASSERT (vec.$tmp_field_name != 0); \\" >>fat.h
  done
  echo "  } while (0)" >>fat.h

  echo "
/* Call ITERATE(field) for each fat threshold field. */
#define ITERATE_FAT_THRESHOLDS() \\
  do { \\" >>fat.h
  for tmp_tn in $fat_thresholds; do
    tmp_field_name=`echo $tmp_tn | tr '[[A-Z]]' '[[a-z]]'`
    echo "    ITERATE ($tmp_tn, $tmp_field_name); \\" >>fat.h
  done
  echo "  } while (0)" >>fat.h

  for tmp_dir in $fat_path; do
    CPUVEC_SETUP=
    THRESH_ASM_SETUP=
    echo "" >>fat.h
    GMP_FAT_SUFFIX(tmp_suffix, $tmp_dir)

    # In order to keep names unique on a DOS 8.3 filesystem, use a prefix
    # (rather than a suffix) for the generated file names, and abbreviate.
    case $tmp_suffix in
      pentium)       tmp_prefix=p   ;;
      pentium_mmx)   tmp_prefix=pm  ;;
      p6_mmx)        tmp_prefix=p2  ;;
      p6_p3mmx)      tmp_prefix=p3  ;;
      pentium4)      tmp_prefix=p4  ;;
      pentium4_mmx)  tmp_prefix=p4m ;;
      pentium4_sse2) tmp_prefix=p4s ;;
      k6_mmx)        tmp_prefix=k6m ;;
      k6_k62mmx)     tmp_prefix=k62 ;;
      k7_mmx)        tmp_prefix=k7m ;;
      *)             tmp_prefix=$tmp_suffix ;;
    esac

    # Extract desired thresholds from gmp-mparam.h file in this directory,
    # if prsent.
    tmp_mparam=$srcdir/mpn/$tmp_dir/gmp-mparam.h
    if test -f $tmp_mparam; then
      for tmp_tn in $fat_thresholds; do
        tmp_thresh=`sed -n "s/^#define $tmp_tn[ 	]*\\([0-9][0-9]*\\).*$/\\1/p" $tmp_mparam`
        if test -n "$tmp_thresh"; then
          THRESH_ASM_SETUP=["${THRESH_ASM_SETUP}define($tmp_tn,$tmp_thresh)
"]
          CPUVEC_SETUP="$CPUVEC_SETUP    decided_cpuvec.`echo $tmp_tn | tr '[[A-Z]]' '[[a-z]]'` = $tmp_thresh; \\
"
          eval tmp_limit=\$${tmp_tn}_LIMIT
          if test -z "$tmp_limit"; then
            tmp_limit=0
          fi
          if test $tmp_thresh -gt $tmp_limit; then
            eval ${tmp_tn}_LIMIT=$tmp_thresh
          fi
        fi
      done
    fi

    for tmp_fn in $fat_functions; do
      GMP_MULFUNC_CHOICES

      for tmp_base in $tmp_fn $tmp_mulfunc; do
        for tmp_ext in MPN_SUFFIXES; do
          tmp_file=$srcdir/mpn/$tmp_dir/$tmp_base.$tmp_ext
          if test -f $tmp_file; then

            mpn_objects="$mpn_objects ${tmp_prefix}_$tmp_fn.lo"
            mpn_objs_in_libgmp="$mpn_objs_in_libgmp mpn/${tmp_prefix}_$tmp_fn.lo"

            GMP_FILE_TO_FUNCTION(tmp_fbase,tmp_fn)

            # carry-in variant, eg. divrem_1c or modexact_1c_odd
            case $tmp_fbase in
              *_1*) tmp_fbasec=`echo $tmp_fbase | sed 's/_1/_1c/'` ;;
              *)    tmp_fbasec=${tmp_fbase}c ;;
            esac

            # Create a little file doing an include from srcdir.  The
            # OPERATION and renamings aren't all needed all the time, but
            # they don't hurt if unused.
            #
            # FIXME: Should generate these via config.status commands.
            # Would need them all in one AC_CONFIG_COMMANDS though, since
            # that macro doesn't accept a set of separate commands generated
            # by shell code.
            #
	    gmp_srclinks="$gmp_srclinks mpn/${tmp_prefix}_$tmp_fn.$tmp_ext"
            case $tmp_ext in
              as)
                echo [";  mpn_$tmp_fbase - from $tmp_dir directory for fat binary.
;  Generated by configure - DO NOT EDIT.

%define OPERATION_$tmp_fn
%define suffix $tmp_suffix

%include \"$mpn_relative_top_srcdir/mpn/$tmp_dir/$tmp_base.as\"
"] >mpn/${tmp_prefix}_$tmp_fn.as
                ;;
              asm)
                # hide the d-n-l from autoconf's error checking
                tmp_d_n_l=d""nl
                echo ["$tmp_d_n_l  mpn_$tmp_fbase - from $tmp_dir directory for fat binary.
$tmp_d_n_l  Generated by configure - DO NOT EDIT.
$tmp_d_n_l  This line is to help MINGW64

define(OPERATION_$tmp_fn)
define(__gmpn_$tmp_fbase, __gmpn_${tmp_fbase}_$tmp_suffix)
define(__gmpn_$tmp_fbasec,__gmpn_${tmp_fbasec}_${tmp_suffix})
define(__gmpn_preinv_${tmp_fbase},__gmpn_preinv_${tmp_fbase}_${tmp_suffix})

$tmp_d_n_l  For k6 and k7 gcd_1 calling their corresponding mpn_modexact_1_odd
ifdef(\`__gmpn_modexact_1_odd',,
\`define(__gmpn_modexact_1_odd,__gmpn_modexact_1_odd_${tmp_suffix})')

$THRESH_ASM_SETUP
include][($mpn_relative_top_srcdir/mpn/$tmp_dir/$tmp_base.asm)
"] >mpn/${tmp_prefix}_$tmp_fn.asm
                ;;
              c)

                echo ["/* mpn_$tmp_fbase - from $tmp_dir directory for fat binary.
   Generated by configure - DO NOT EDIT. */

#define OPERATION_$tmp_fn 1
#define __gmpn_$tmp_fbase           __gmpn_${tmp_fbase}_$tmp_suffix
#define __gmpn_$tmp_fbasec          __gmpn_${tmp_fbasec}_${tmp_suffix}
#define __gmpn_preinv_${tmp_fbase}  __gmpn_preinv_${tmp_fbase}_${tmp_suffix}

#include \"$mpn_relative_top_srcdir/mpn/$tmp_dir/$tmp_base.c\"
"] >mpn/${tmp_prefix}_$tmp_fn.c
                ;;
            esac

            # Prototype, and append to CPUVEC_SETUP for this directory.
            echo "DECL_$tmp_fbase (__gmpn_${tmp_fbase}_$tmp_suffix);" >>fat.h
            CPUVEC_SETUP="$CPUVEC_SETUP    decided_cpuvec.$tmp_fbase = __gmpn_${tmp_fbase}_${tmp_suffix}; \\
"
            # Ditto for any preinv variant (preinv_divrem_1, preinv_mod_1).
            if grep "^PROLOGUE(mpn_preinv_$tmp_fn)\|GLOBAL_FUNC mpn_preinv_$tmp_fn" $tmp_file >/dev/null; then
              echo "DECL_preinv_$tmp_fbase (__gmpn_preinv_${tmp_fbase}_$tmp_suffix);" >>fat.h
              CPUVEC_SETUP="$CPUVEC_SETUP    decided_cpuvec.preinv_$tmp_fbase = __gmpn_preinv_${tmp_fbase}_${tmp_suffix}; \\
"
            fi
          fi
        done
      done
    done

    # Emit CPUVEC_SETUP for this directory
    echo "" >>fat.h
    echo "#define CPUVEC_SETUP_$tmp_suffix \\" >>fat.h
    echo "  do { \\" >>fat.h
    echo "$CPUVEC_SETUP  } while (0)" >>fat.h
  done

  # Emit threshold limits
  echo "" >>fat.h
  for tmp_tn in $fat_thresholds; do
    eval tmp_limit=\$${tmp_tn}_LIMIT
    echo "#define ${tmp_tn}_LIMIT  $tmp_limit" >>fat.h
  done
fi


# Normal binary setups.
#

for tmp_ext in MPN_SUFFIXES; do
  eval found_$tmp_ext=no
done

for tmp_fn in $gmp_mpn_functions; do
  for tmp_ext in MPN_SUFFIXES; do
    test "$no_create" = yes || rm -f mpn/$tmp_fn.$tmp_ext
  done

  # mpn_preinv_divrem_1 might have been provided by divrem_1.asm, likewise
  # mpn_preinv_mod_1 by mod_1.asm.
  case $tmp_fn in
  preinv_divrem_1)
    if test "$HAVE_NATIVE_mpn_preinv_divrem_1" = yes; then continue; fi ;;
  preinv_mod_1)
    if test "$HAVE_NATIVE_mpn_preinv_mod_1" = yes; then continue; fi ;;
  esac

  GMP_MULFUNC_CHOICES

  found=no
  for tmp_dir in $path; do
    for tmp_base in $tmp_fn $tmp_mulfunc; do
      for tmp_ext in MPN_SUFFIXES; do
        tmp_file=$srcdir/mpn/$tmp_dir/$tmp_base.$tmp_ext
        if test -f $tmp_file; then

          # For a nails build, check if the file supports our nail bits.
          # Generic code always supports all nails.
          #
          # FIXME: When a multi-function file is selected to provide one of
          # the nails-neutral routines, like logops_n for and_n, the
          # PROLOGUE grepping will create HAVE_NATIVE_mpn_<foo> defines for
          # all functions in that file, even if they haven't all been
          # nailified.  Not sure what to do about this, it's only really a
          # problem for logops_n, and it's not too terrible to insist those
          # get nailified always.
          #
          if test $GMP_NAIL_BITS != 0 && test $tmp_dir != generic; then
            case $tmp_fn in
              and_n | ior_n | xor_n | andn_n | \
              copyi | copyd | \
              popcount | hamdist | \
              udiv | udiv_w_sdiv | umul | \
              cntlz | invert_limb)
                # these operations are either unaffected by nails or defined
                # to operate on full limbs
                ;;
              *)
                nails=[`sed -n 's/^[ 	]*NAILS_SUPPORT(\(.*\))/\1/p' $tmp_file `]
                for n in $nails; do
                  case $n in
                  *-*)
                    n_start=`echo "$n" | sed -n 's/\(.*\)-.*/\1/p'`
                    n_end=`echo "$n" | sed -n 's/.*-\(.*\)/\1/p'`
                    ;;
                  *)
                    n_start=$n
                    n_end=$n
                    ;;
                  esac
                  if test $GMP_NAIL_BITS -ge $n_start && test $GMP_NAIL_BITS -le $n_end; then
                    found=yes
                    break
                  fi
                done
                if test $found != yes; then
                  continue
                fi
                ;;
            esac
          fi

          found=yes
          eval found_$tmp_ext=yes

          if test $tmp_ext = c; then
            tmp_u='$U'
          else
            tmp_u=
          fi

	  mpn_objects="$mpn_objects $tmp_fn$tmp_u.lo"
          mpn_objs_in_libgmp="$mpn_objs_in_libgmp mpn/$tmp_fn$tmp_u.lo"
          AC_CONFIG_LINKS(mpn/$tmp_fn.$tmp_ext:mpn/$tmp_dir/$tmp_base.$tmp_ext)
          gmp_srclinks="$gmp_srclinks mpn/$tmp_fn.$tmp_ext"

          # Duplicate AC_DEFINEs are harmless, so it doesn't matter
          # that multi-function files get grepped here repeatedly.
          # The PROLOGUE pattern excludes the optional second parameter.
          gmp_ep=[`
            sed -n 's/^[ 	]*MULFUNC_PROLOGUE(\(.*\))/\1/p' $tmp_file ;
            sed -n 's/^[ 	]*PROLOGUE(\([^,]*\).*)/\1/p' $tmp_file ;
            sed -n 's/^;[ 	]*PROLOGUE(\([^,]*\).*)/\1/p' $tmp_file ;
            sed -n 's/[^G]*GLOBAL_FUNC[[:space:]]*\(.*\)/\1/p' $tmp_file ;
          `]
          for gmp_tmp in $gmp_ep; do
            AC_DEFINE_UNQUOTED(HAVE_NATIVE_$gmp_tmp)
            eval HAVE_NATIVE_$gmp_tmp=yes
          done

          case $tmp_fn in
          sqr_basecase) sqr_basecase_source=$tmp_file ;;
          esac

          break
        fi
      done
      if test $found = yes; then break ; fi
    done
    if test $found = yes; then break ; fi
  done

  if test $found = no; then
    for tmp_optional in $gmp_mpn_functions_optional; do
      if test $tmp_optional = $tmp_fn; then
        found=yes
      fi
    done
    if test $found = no; then
      AC_MSG_ERROR([no version of $tmp_fn found in path: $path])
    fi
  fi
done

# For mingw64 rename all *.asm files to *.as files so we then will use yasm
# cant move them yet as the mpn directory is not created until the end.
# Apart from defining old_gmp_srclinks (and we could do without it...),
# what follows is not strictly needed and just make gmp_srclinks more
# consistent.
case $host in
  *-pc-msys|*-w64-mingw*|*-*-cygwin*)
    if test "$ABI" = "64"; then
      old_gmp_srclinks=$gmp_srclinks
      gmp_srclinks=
      for jay in $old_gmp_srclinks; do
        jayext=`echo "$jay" | awk -F . '{print $NF}'`
        #jayext=`/bin/echo "${jay#*.}"`
        #jayext=`echo "$jay" | sed -n 's/.*\.//'`
        base=`echo "${jay%.*}"`
        name=`basename $base`
        if test "$jayext" = "asm" && test "$name" != "fat_entry"; then
          gmp_srclinks="$gmp_srclinks $base.as"
        else
          gmp_srclinks="$gmp_srclinks $jay"
        fi
      done
    fi
  ;;
esac

# All cycle counters are .asm files currently
if test -n "$SPEED_CYCLECOUNTER_OBJ"; then
  found_asm=yes
fi

dnl  The following list only needs to have templates for those defines which
dnl  are going to be tested by the code, there's no need to have every
dnl  possible mpn routine.

AH_VERBATIM([HAVE_NATIVE],
[/* Define to 1 each of the following for which a native (ie. CPU specific)
    implementation of the corresponding routine exists.  */
#undef HAVE_NATIVE_mpn_add_n
#undef HAVE_NATIVE_mpn_add_nc
#undef HAVE_NATIVE_mpn_addadd_n
#undef HAVE_NATIVE_mpn_addlsh1_n
#undef HAVE_NATIVE_mpn_addlsh_n
#undef HAVE_NATIVE_mpn_addlsh_nc
#undef HAVE_NATIVE_mpn_addmul_1c
#undef HAVE_NATIVE_mpn_addmul_2
#undef HAVE_NATIVE_mpn_addmul_3
#undef HAVE_NATIVE_mpn_addmul_4
#undef HAVE_NATIVE_mpn_addmul_5
#undef HAVE_NATIVE_mpn_addmul_6
#undef HAVE_NATIVE_mpn_addmul_7
#undef HAVE_NATIVE_mpn_addmul_8
#undef HAVE_NATIVE_mpn_addsub_n
#undef HAVE_NATIVE_mpn_sumdiff_n
#undef HAVE_NATIVE_mpn_nsumdiff_n
#undef HAVE_NATIVE_mpn_and_n
#undef HAVE_NATIVE_mpn_andn_n
#undef HAVE_NATIVE_mpn_com_n
#undef HAVE_NATIVE_mpn_not
#undef HAVE_NATIVE_mpn_copyd
#undef HAVE_NATIVE_mpn_copyi
#undef HAVE_NATIVE_mpn_declsh_n
#undef HAVE_NATIVE_mpn_divexact_1
#undef HAVE_NATIVE_mpn_divrem_euclidean_qr_1
#undef HAVE_NATIVE_mpn_divrem_hensel_qr_1
#undef HAVE_NATIVE_mpn_divrem_hensel_qr_1_1
#undef HAVE_NATIVE_mpn_divrem_hensel_qr_1_2
#undef HAVE_NATIVE_mpn_divrem_hensel_r_1
#undef HAVE_NATIVE_mpn_divrem_hensel_rsh_qr_1
#undef HAVE_NATIVE_mpn_rsh_divrem_hensel_qr_1
#undef HAVE_NATIVE_mpn_rsh_divrem_hensel_qr_1_1
#undef HAVE_NATIVE_mpn_rsh_divrem_hensel_qr_1_2
#undef HAVE_NATIVE_mpn_divrem_euclidean_qr_2
#undef HAVE_NATIVE_mpn_divrem_euclidean_r_1
#undef HAVE_NATIVE_mpn_divrem_1
#undef HAVE_NATIVE_mpn_divrem_1c
#undef HAVE_NATIVE_mpn_divrem_2
#undef HAVE_NATIVE_mpn_double
#undef HAVE_NATIVE_mpn_gcd_1
#undef HAVE_NATIVE_mpn_half
#undef HAVE_NATIVE_mpn_inclsh_n
#undef HAVE_NATIVE_mpn_invert_limb
#undef HAVE_NATIVE_mpn_ior_n
#undef HAVE_NATIVE_mpn_iorn_n
#undef HAVE_NATIVE_mpn_karaadd
#undef HAVE_NATIVE_mpn_karasub
#undef HAVE_NATIVE_mpn_lshift1
#undef HAVE_NATIVE_mpn_lshift2
#undef HAVE_NATIVE_mpn_lshiftc
#undef HAVE_NATIVE_mpn_mod_1
#undef HAVE_NATIVE_mpn_mod_1_1
#undef HAVE_NATIVE_mpn_mod_1_2
#undef HAVE_NATIVE_mpn_mod_1_3
#undef HAVE_NATIVE_mpn_mod_1c
#undef HAVE_NATIVE_mpn_modexact_1_odd
#undef HAVE_NATIVE_mpn_modexact_1c_odd
#undef HAVE_NATIVE_mpn_mul_1c
#undef HAVE_NATIVE_mpn_mul_2
#undef HAVE_NATIVE_mpn_mul_3
#undef HAVE_NATIVE_mpn_mul_4
#undef HAVE_NATIVE_mpn_mul_basecase
#undef HAVE_NATIVE_mpn_nand_n
#undef HAVE_NATIVE_mpn_neg_n
#undef HAVE_NATIVE_mpn_nior_n
#undef HAVE_NATIVE_mpn_preinv_divrem_1
#undef HAVE_NATIVE_mpn_preinv_mod_1
#undef HAVE_NATIVE_mpn_redc_1
#undef HAVE_NATIVE_mpn_redc_2
#undef HAVE_NATIVE_mpn_rsh1add_n
#undef HAVE_NATIVE_mpn_rsh1sub_n
#undef HAVE_NATIVE_mpn_rshift1
#undef HAVE_NATIVE_mpn_rshift2
#undef HAVE_NATIVE_mpn_sqr_basecase
#undef HAVE_NATIVE_mpn_sqr_diagonal
#undef HAVE_NATIVE_mpn_store
#undef HAVE_NATIVE_mpn_sub_n
#undef HAVE_NATIVE_mpn_sub_nc
#undef HAVE_NATIVE_mpn_subadd_n
#undef HAVE_NATIVE_mpn_sublsh1_n
#undef HAVE_NATIVE_mpn_sublsh_n
#undef HAVE_NATIVE_mpn_sublsh_nc
#undef HAVE_NATIVE_mpn_submul_1c
#undef HAVE_NATIVE_mpn_umul_ppmm
#undef HAVE_NATIVE_mpn_umul_ppmm_r
#undef HAVE_NATIVE_mpn_udiv_qrnnd
#undef HAVE_NATIVE_mpn_udiv_qrnnd_r
#undef HAVE_NATIVE_mpn_xor_n
#undef HAVE_NATIVE_mpn_xnor_n])


# Don't demand an m4 unless it's actually needed.
if test $found_asm = yes; then
  GMP_PROG_M4
  GMP_M4_M4WRAP_SPURIOUS
else
  M4=m4-not-needed
fi

# Only do the GMP_ASM checks if there's a .S or .asm wanting them.
if test $found_asm = no && test $found_S = no; then
  gmp_asm_syntax_testing=no
fi

if test "$gmp_asm_syntax_testing" != no; then
  GMP_ASM_TEXT
  GMP_ASM_DATA
  GMP_ASM_LABEL_SUFFIX
  GMP_ASM_GLOBL
  GMP_ASM_GLOBL_ATTR
  GMP_ASM_UNDERSCORE
  GMP_ASM_RODATA
  GMP_ASM_TYPE
  GMP_ASM_SIZE
  GMP_ASM_LSYM_PREFIX
  GMP_ASM_W32
  GMP_ASM_ALIGN_LOG
  case $host in
    IA64_PATTERN)
      GMP_ASM_IA64_ALIGN_OK
      ;;
    [powerpc*-*-* | power[3-9]-*-*])
      GMP_ASM_POWERPC_PIC_ALWAYS
      GMP_ASM_POWERPC_R_REGISTERS
      GMP_INCLUDE_MPN(powerpc32/powerpc-defs.m4)
      case $host in
        *-*-aix*)
	  case $ABI in
	    64 | aix64)  GMP_INCLUDE_MPN(powerpc64/aix.m4) ;;
            *)           GMP_INCLUDE_MPN(powerpc32/aix.m4) ;;
          esac
          ;;
        *-*-linux* | *-*-*bsd*)
	  case $ABI in
	    mode64)      GMP_INCLUDE_MPN(powerpc64/elf.m4) ;;
	    mode32 | 32) GMP_INCLUDE_MPN(powerpc32/elf.m4) ;;
          esac
          ;;
        *-*-darwin*)
	  case $ABI in
	    mode64)      GMP_INCLUDE_MPN(powerpc64/darwin.m4) ;;
	    mode32 | 32) GMP_INCLUDE_MPN(powerpc32/darwin.m4) ;;
          esac
          ;;
        *)
	  # Assume unrecognized operating system is the powerpc eABI
          GMP_INCLUDE_MPN(powerpc32/eabi.m4)
	  ;;
      esac
      ;;
    power*-*-aix*)
      GMP_INCLUDE_MPN(powerpc32/aix.m4)
      ;;
    sparcv9*-*-* | ultrasparc*-*-* | sparc64-*-*)
      case $ABI in
        64)
          GMP_ASM_SPARC_REGISTER
          ;;
      esac
      ;;
    X86_PATTERN | X86_64_PATTERN)
      GMP_ASM_ALIGN_FILL_0x90
      case $ABI in
        32)
          GMP_INCLUDE_MPN(x86/x86-defs.m4)
          GMP_ASM_COFF_TYPE
          GMP_ASM_X86_GOT_UNDERSCORE
          GMP_ASM_X86_SHLDL_CL
      	  case $enable_profiling in
      	    prof | gprof)  GMP_ASM_X86_MCOUNT ;;
      	  esac
          case $host in
            *-*-darwin*)
              OBJECT_FORMAT="-f macho32" ;;
            *)
              OBJECT_FORMAT="-f elf32" ;;
           esac
          ;;

        64)
          case $host in
            *-*-darwin*)
              # Defined in mpn/x86_64/x86_64-defs.m4, but there currently
              # hardcoded just for ELF, so redefine it here for Mach-O:
              GMP_DEFINE_RAW(["define(\`JUMPTABSECT',\`    .section    __DATA,__const')"],POST)
              OBJECT_FORMAT="-f macho64" ;;
            *-pc-msys|*-w64-mingw*|*-*-cygwin*)
              OBJECT_FORMAT="-f x64"  ;;
            *)
              OBJECT_FORMAT="-f elf64" ;;
          esac
          GMP_INCLUDE_MPN(x86_64/x86_64-defs.m4)
          ;;
      esac
      AC_SUBST(OBJECT_FORMAT)
      ;;
  esac
fi

# Create link for gmp-mparam.h.
gmp_mparam_source=
for gmp_mparam_dir in $path; do
  test "$no_create" = yes || rm -f gmp-mparam.h
  tmp_file=$srcdir/mpn/$gmp_mparam_dir/gmp-mparam.h
  if test -f $tmp_file; then
    AC_CONFIG_LINKS(gmp-mparam.h:mpn/$gmp_mparam_dir/gmp-mparam.h)
    gmp_srclinks="$gmp_srclinks gmp-mparam.h"
    gmp_mparam_source=$tmp_file
    break
  fi
done
if test -z "$gmp_mparam_source"; then
  AC_MSG_ERROR([no version of gmp-mparam.h found in path: $path])
fi

# Create longlong.h from the path
longlong_source=
for longlong_dir in $path; do
  test "$no_create" = yes || rm -f longlong.h
  tmp_file=$srcdir/mpn/$longlong_dir/longlong_inc.h
  if test -f $tmp_file; then
    AC_CONFIG_FILES(longlong.h:longlong_pre.h:mpn/$longlong_dir/longlong_inc.h:longlong_post.h)
    gmp_srclinks="$gmp_srclinks longlong.h"
    longlong_source=$tmp_file
    break
  fi
done
if test -z "$longlong_source"; then
  AC_MSG_ERROR([no version of longlong_inc.h found in path: $path])
fi

# For a helpful message from tune/tuneup.c
gmp_mparam_suggest=$gmp_mparam_source
if test "$gmp_mparam_dir" = generic; then
  for i in $path; do break; done
  if test "$i" != generic; then
    gmp_mparam_suggest="new file $srcdir/mpn/$i/gmp-mparam.h"
  fi
fi
AC_DEFINE_UNQUOTED(GMP_MPARAM_H_SUGGEST, "$gmp_mparam_source",
[The gmp-mparam.h file (a string) the tune program should suggest updating.])


# Copy any SQR_KARATSUBA_THRESHOLD from gmp-mparam.h to config.m4.
# Some versions of sqr_basecase.asm use this.
# Fat binaries do this on a per-file basis, so skip in that case.
#
if test -z "$fat_path"; then
  tmp_gmp_karatsuba_sqr_threshold=`sed -n 's/^#define SQR_KARATSUBA_THRESHOLD[ 	]*\([0-9][0-9]*\).*$/\1/p' $gmp_mparam_source`
  if test -n "$tmp_gmp_karatsuba_sqr_threshold"; then
    GMP_DEFINE_RAW(["define(<SQR_KARATSUBA_THRESHOLD>,<$tmp_gmp_karatsuba_sqr_threshold>)"])
  fi
fi


# Sizes of some types, needed at preprocessing time.
#
# FIXME: The assumption that BITS_PER_MP_LIMB is 8*sizeof(mp_limb_t) might
# be slightly rash, but it's true everwhere we know of and ought to be true
# of any sensible system.  In a generic C build, grepping LONG_BIT out of
# <limits.h> might be an alternative, for maximum portability.
#
AC_CHECK_SIZEOF(unsigned short)
AC_CHECK_SIZEOF(unsigned)
AC_CHECK_SIZEOF(unsigned long)
AC_CHECK_SIZEOF(uintmax_t)
AC_CHECK_SIZEOF(mp_limb_t, , GMP_INCLUDE_GMP_H)
if test "$ac_cv_sizeof_mp_limb_t" = 0; then
  AC_MSG_ERROR([Oops, mp_limb_t doesn't seem to work])
fi
AC_SUBST(BITS_PER_MP_LIMB, `expr 8 \* $ac_cv_sizeof_mp_limb_t`)
GMP_DEFINE_RAW(["define(<SIZEOF_UNSIGNED>,<$ac_cv_sizeof_unsigned>)"])

# Check compiler limb size matches gmp-mparam.h
#
# FIXME: Some of the cycle counter objects in the tune directory depend on
# the size of ulong, it'd be possible to check that here, though a mismatch
# probably wouldn't want to be fatal, none of the libmpir assembler code
# depends on ulong.
#
mparam_bits=[`sed -n 's/^#define BITS_PER_MP_LIMB[ 	][ 	]*\([0-9]*\).*$/\1/p' $gmp_mparam_source`]
if test -n "$mparam_bits" && test "$mparam_bits" -ne $BITS_PER_MP_LIMB; then
  if test "$test_CFLAGS" = set; then
    AC_MSG_ERROR([Oops, mp_limb_t is $BITS_PER_MP_LIMB bits, but the assembler code
in this configuration expects $mparam_bits bits.
You appear to have set \$CFLAGS, perhaps you also need to tell GMP the
intended ABI, see "ABI and ISA" in the manual.])
  else
    AC_MSG_ERROR([Oops, mp_limb_t is $BITS_PER_MP_LIMB bits, but the assembler code
in this configuration expects $mparam_bits bits.])
  fi
fi

GMP_DEFINE_RAW(["define(<GMP_LIMB_BITS>,$BITS_PER_MP_LIMB)"])
GMP_DEFINE_RAW(["define(<GMP_NAIL_BITS>,$GMP_NAIL_BITS)"])
GMP_DEFINE_RAW(["define(<GMP_NUMB_BITS>,eval(GMP_LIMB_BITS-GMP_NAIL_BITS))"])

AC_SUBST(mpn_objects)
AC_SUBST(mpn_objs_in_libgmp)



AC_SUBST(gmp_srclinks)


# A recompiled sqr_basecase for use in the tune program, if necessary.
TUNE_SQR_OBJ=
test -d tune || mkdir tune
case $sqr_basecase_source in
  *.asm | *.as)
    sqr_max=[`sed -n 's/^def...(SQR_KARATSUBA_THRESHOLD_MAX, *\([0-9]*\))/\1/p' $sqr_basecase_source`]
    if test -n "$sqr_max"; then
      TUNE_SQR_OBJ=sqr_asm.o
      AC_DEFINE_UNQUOTED(TUNE_SQR_KARATSUBA_MAX,$sqr_max,
      [Maximum size the tune program can test for SQR_KARATSUBA_THRESHOLD])
    fi
    cat >tune/sqr_basecase.c <<EOF
/* not sure that an empty file can compile, so put in a dummy */
int sqr_basecase_dummy;
EOF
    ;;
  *.c)
    TUNE_SQR_OBJ=
    AC_DEFINE(TUNE_SQR_KARATSUBA_MAX,SQR_KARATSUBA_MAX_GENERIC)
    cat >tune/sqr_basecase.c <<EOF
#define TUNE_PROGRAM_BUILD 1
#define TUNE_PROGRAM_BUILD_SQR 1
#include "mpn/sqr_basecase.c"
EOF
    ;;
esac
AC_SUBST(TUNE_SQR_OBJ)

# Create config.m4.
GMP_FINISH

case $host in
  *-pc-msys|*-w64-mingw*|*-*-cygwin*)
    if test "$ABI" = "64"; then
      if test $enable_fat = yes; then
        YASM_MAC_INC=yasm_macwin.inc.fat
      else
        YASM_MAC_INC=yasm_macwin.inc.nofat
      fi
    else
      if test $enable_fat = yes; then
        YASM_MAC_INC=yasm_mac.inc.fat
      else
        YASM_MAC_INC=yasm_mac.inc.nofat
      fi
    fi
  ;;
  *-*-*)
    if test $enable_fat = yes; then
      YASM_MAC_INC=yasm_mac.inc.fat
    else
      YASM_MAC_INC=yasm_mac.inc.nofat
    fi
  ;;
esac
AC_CONFIG_LINKS(yasm_mac.inc:$YASM_MAC_INC)

# Create Makefiles
# FIXME: Upcoming version of autoconf/automake don't like broken lines.
#        Right now automake isn't accepting the new AC_CONFIG_FILES scheme.

AC_CONFIG_FILES(Makefile mpf/Makefile mpn/Makefile fft/Makefile mpq/Makefile mpz/Makefile printf/Makefile scanf/Makefile cxx/Makefile tests/Makefile tests/devel/Makefile tests/mpf/Makefile tests/mpn/Makefile tests/fft/Makefile tests/mpq/Makefile tests/mpz/Makefile tests/rand/Makefile tests/misc/Makefile tests/cxx/Makefile doc/Makefile tune/Makefile)
AC_CONFIG_FILES(mpir.h:gmp-h.in)
if test $enable_gmpcompat = yes; then
  AC_CONFIG_FILES(gmp.h:gmp-h.in)
  if test $enable_cxx = yes ; then
    AC_CONFIG_FILES(gmpxx.h:mpirxx.h)
  fi
fi

AC_OUTPUT

# now we have created and populated the mpn directory we can rename the files
# and post process the "m4 macros" into "yasm macros"
case $host in
  *-pc-msys|*-w64-mingw*|*-*-cygwin*)
    if test "$ABI" = "64"; then
      echo "Renaming assembly files on Win64..."
      for jay in $old_gmp_srclinks; do
        jayext=`echo "$jay" | awk -F . '{print $NF}'`
        #jayext=`/bin/echo "${jay#*.}"`
        #jayext=`echo "$jay" | sed -n 's/.*\.//'`
        base=`echo "${jay%.*}"`
        name=`basename $base`
        if test "$jayext" = "asm" && test "$name" != "fat_entry" ; then
          got=`grep -ce "d""nl  This line is to help MINGW64" $jay`
          if test "$got" = "1" ; then
            cat $jay | sed s/"include(\(.*\))"/"%include \"\1\""/ | sed s/"d""efine(\(.*\),\(.*\))"/"%d""efine \1 \2"/ | sed s/"d""efine(\(.*\))"/"%d""efine \1"/ | sed s/"d""nl\(.*\)"/"; \1"/ | sed s/ifdef.*// | sed s/"\`.*"// > $base.as
            rm -f $jay
          else
            mv $jay $base.as
          fi
        fi
      done
# the file x86_64_divrem_euclidean_qr_1.as should detect the defined macro EXCLUDE_PREINV , but for some reason it doesnt, so we hack it
      if test $enable_fat = yes; then
        echo "%d""efine EXCLUDE_PREINV 1" > mpn/x86_64_divrem_euclidean_qr_1.tmp
        cat mpn/x86_64_divrem_euclidean_qr_1.as >> mpn/x86_64_divrem_euclidean_qr_1.tmp
        mv mpn/x86_64_divrem_euclidean_qr_1.tmp mpn/x86_64_divrem_euclidean_qr_1.as
      fi
    fi
  ;;
esac
