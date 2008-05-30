# aclocal.m4 generated automatically by aclocal 1.6.3 -*- Autoconf -*-

# Copyright 1996, 1997, 1998, 1999, 2000, 2001, 2002
# Free Software Foundation, Inc.
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

dnl  GMP specific autoconf macros


dnl  Copyright 2000, 2001, 2002, 2003, 2004, 2005, 2006 Free Software
dnl  Foundation, Inc.
dnl
dnl  This file is part of the GNU MP Library.
dnl
dnl  The GNU MP Library is free software; you can redistribute it and/or modify
dnl  it under the terms of the GNU Lesser General Public License as published
dnl  by the Free Software Foundation; either version 2.1 of the License, or (at
dnl  your option) any later version.
dnl
dnl  The GNU MP Library is distributed in the hope that it will be useful, but
dnl  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
dnl  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
dnl  License for more details.
dnl
dnl  You should have received a copy of the GNU Lesser General Public License
dnl  along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
dnl  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
dnl  MA 02110-1301, USA.


dnl  Some tests use, or must delete, the default compiler output.  The
dnl  possible filenames are based on what autoconf looks for, namely
dnl
dnl    a.out - normal unix style
dnl    b.out - i960 systems, including gcc there
dnl    a.exe - djgpp
dnl    a_out.exe - OpenVMS DEC C called via GNV wrapper (gnv.sourceforge.net)
dnl    conftest.exe - various DOS compilers


define(IA64_PATTERN,
[[ia64*-*-* | itanium-*-* | itanium2-*-*]])

dnl  Need to be careful not to match m6811, m6812, m68hc11 and m68hc12, all
dnl  of which config.sub accepts.  (Though none of which are likely to work
dnl  with GMP.)
dnl
define(M68K_PATTERN,
[[m68k-*-* | m68[0-9][0-9][0-9]-*-*]])

define(POWERPC64_PATTERN,
[[powerpc64-*-* | powerpc64le-*-* | powerpc620-*-* | powerpc630-*-* | powerpc970-*-* | power[3-9]-*-*]])

define(X86_PATTERN,
[[i?86*-*-* | k[5-8]*-*-* | pentium*-*-* | athlon-*-* | viac3*-*-*]])


dnl  GMP_FAT_SUFFIX(DSTVAR, DIRECTORY)
dnl  ---------------------------------
dnl  Emit code to set shell variable DSTVAR to the suffix for a fat binary
dnl  routine from DIRECTORY.  DIRECTORY can be a shell expression like $foo
dnl  etc.
dnl
dnl  The suffix is directory separators / or \ changed to underscores, and
dnl  if there's more than one directory part, then the first is dropped.
dnl
dnl  For instance,
dnl
dnl      x86         ->  x86
dnl      x86/k6      ->  k6
dnl      x86/k6/mmx  ->  k6_mmx

define(GMP_FAT_SUFFIX,
[[$1=`echo $2 | sed -e '/\//s:^[^/]*/::' -e 's:[\\/]:_:g'`]])


dnl  GMP_REMOVE_FROM_LIST(listvar,item)
dnl  ----------------------------------
dnl  Emit code to remove any occurance of ITEM from $LISTVAR.  ITEM can be a
dnl  shell expression like $foo if desired.

define(GMP_REMOVE_FROM_LIST,
[remove_from_list_tmp=
for remove_from_list_i in $[][$1]; do
  if test $remove_from_list_i = [$2]; then :;
  else
     remove_from_list_tmp="$remove_from_list_tmp $remove_from_list_i"
  fi
done
[$1]=$remove_from_list_tmp
])


dnl  GMP_STRIP_PATH(subdir)
dnl  ----------------------
dnl  Strip entries */subdir from $path and $fat_path.

define(GMP_STRIP_PATH,
[GMP_STRIP_PATH_VAR(path, [$1])
GMP_STRIP_PATH_VAR(fat_path, [$1])
])

define(GMP_STRIP_PATH_VAR,
[tmp_path=
for i in $[][$1]; do
  case $i in
    */[$2]) ;;
    *) tmp_path="$tmp_path $i" ;;
  esac
done
[$1]="$tmp_path"
])


dnl  GMP_INCLUDE_GMP_H
dnl  -----------------
dnl  Expand to the right way to #include gmp-h.in.  This must be used
dnl  instead of gmp.h, since that file isn't generated until the end of the
dnl  configure.
dnl
dnl  Dummy values for __GMP_BITS_PER_MP_LIMB and GMP_LIMB_BITS are enough
dnl  for all current configure-time uses of gmp.h.

define(GMP_INCLUDE_GMP_H,
[[#define __GMP_WITHIN_CONFIGURE 1   /* ignore template stuff */
#define GMP_NAIL_BITS $GMP_NAIL_BITS
#define __GMP_BITS_PER_MP_LIMB 123 /* dummy for GMP_NUMB_BITS etc */
#define GMP_LIMB_BITS 123
$DEFN_LONG_LONG_LIMB
#include "$srcdir/gmp-h.in"]
])


dnl  GMP_HEADER_GETVAL(NAME,FILE)
dnl  ----------------------------
dnl  Expand at autoconf time to the value of a "#define NAME" from the given
dnl  FILE.  The regexps here aren't very rugged, but are enough for gmp.
dnl  /dev/null as a parameter prevents a hang if $2 is accidentally omitted.

define(GMP_HEADER_GETVAL,
[patsubst(patsubst(
esyscmd([grep "^#define $1 " $2 /dev/null 2>/dev/null]),
[^.*$1[ 	]+],[]),
[[
 	]*$],[])])


dnl  GMP_VERSION
dnl  -----------
dnl  The gmp version number, extracted from the #defines in gmp-h.in at
dnl  autoconf time.  Two digits like 3.0 if patchlevel <= 0, or three digits
dnl  like 3.0.1 if patchlevel > 0.

define(GMP_VERSION,
[GMP_HEADER_GETVAL(__GNU_MP_VERSION,gmp-h.in)[]dnl
.GMP_HEADER_GETVAL(__GNU_MP_VERSION_MINOR,gmp-h.in)[]dnl
ifelse(m4_eval(GMP_HEADER_GETVAL(__GNU_MP_VERSION_PATCHLEVEL,gmp-h.in) > 0),1,
[.GMP_HEADER_GETVAL(__GNU_MP_VERSION_PATCHLEVEL,gmp-h.in)])])


dnl  GMP_SUBST_CHECK_FUNCS(func,...)
dnl  ------------------------------
dnl  Setup an AC_SUBST of HAVE_FUNC_01 for each argument.

AC_DEFUN([GMP_SUBST_CHECK_FUNCS],
[m4_if([$1],,,
[_GMP_SUBST_CHECK_FUNCS(ac_cv_func_[$1],HAVE_[]m4_translit([$1],[a-z],[A-Z])_01)
GMP_SUBST_CHECK_FUNCS(m4_shift($@))])])

dnl  Called: _GMP_SUBST_CHECK_FUNCS(cachevar,substvar)
AC_DEFUN([_GMP_SUBST_CHECK_FUNCS],
[case $[$1] in
yes) AC_SUBST([$2],1) ;;
no)  [$2]=0 ;;
esac
])


dnl  GMP_SUBST_CHECK_HEADERS(foo.h,...)
dnl  ----------------------------------
dnl  Setup an AC_SUBST of HAVE_FOO_H_01 for each argument.

AC_DEFUN([GMP_SUBST_CHECK_HEADERS],
[m4_if([$1],,,
[_GMP_SUBST_CHECK_HEADERS(ac_cv_header_[]m4_translit([$1],[./],[__]),
HAVE_[]m4_translit([$1],[a-z./],[A-Z__])_01)
GMP_SUBST_CHECK_HEADERS(m4_shift($@))])])

dnl  Called: _GMP_SUBST_CHECK_HEADERS(cachevar,substvar)
AC_DEFUN([_GMP_SUBST_CHECK_HEADERS],
[case $[$1] in
yes) AC_SUBST([$2],1) ;;
no)  [$2]=0 ;;
esac
])


dnl  GMP_COMPARE_GE(A1,B1, A2,B2, ...)
dnl  ---------------------------------
dnl  Compare two version numbers A1.A2.etc and B1.B2.etc.  Set
dnl  $gmp_compare_ge to yes or no accoring to the result.  The A parts
dnl  should be variables, the B parts fixed numbers.  As many parts as
dnl  desired can be included.  An empty string in an A part is taken to be
dnl  zero, the B parts should be non-empty and non-zero.
dnl
dnl  For example,
dnl
dnl      GMP_COMPARE($major,10, $minor,3, $subminor,1)
dnl
dnl  would test whether $major.$minor.$subminor is greater than or equal to
dnl  10.3.1.

AC_DEFUN([GMP_COMPARE_GE],
[gmp_compare_ge=no
GMP_COMPARE_GE_INTERNAL($@)
])

AC_DEFUN([GMP_COMPARE_GE_INTERNAL],
[ifelse(len([$3]),0,
[if test -n "$1" && test "$1" -ge $2; then
  gmp_compare_ge=yes
fi],
[if test -n "$1"; then
  if test "$1" -gt $2; then
    gmp_compare_ge=yes
  else
    if test "$1" -eq $2; then
      GMP_COMPARE_GE_INTERNAL(m4_shift(m4_shift($@)))
    fi
  fi
fi])
])
  

dnl  GMP_PROG_AR
dnl  -----------
dnl  GMP additions to $AR.
dnl
dnl  A cross-"ar" may be necessary when cross-compiling since the build
dnl  system "ar" might try to interpret the object files to build a symbol
dnl  table index, hence the use of AC_CHECK_TOOL.
dnl
dnl  A user-selected $AR is always left unchanged.  AC_CHECK_TOOL is still
dnl  run to get the "checking" message printed though.
dnl
dnl  If extra flags are added to AR, then ac_cv_prog_AR and
dnl  ac_cv_prog_ac_ct_AR are set too, since libtool (cvs 2003-03-31 at
dnl  least) will do an AC_CHECK_TOOL and that will AR from one of those two
dnl  cached variables.  (ac_cv_prog_AR is used if there's an ac_tool_prefix,
dnl  or ac_cv_prog_ac_ct_AR is used otherwise.)  FIXME: This is highly
dnl  dependent on autoconf internals, perhaps it'd work to put our extra
dnl  flags into AR_FLAGS instead.
dnl
dnl  $AR_FLAGS is set to "cq" rather than leaving it to libtool "cru".  The
dnl  latter fails when libtool goes into piecewise mode and is unlucky
dnl  enough to have two same-named objects in separate pieces, as happens
dnl  for instance to random.o (and others) on vax-dec-ultrix4.5.  Naturally
dnl  a user-selected $AR_FLAGS is left unchanged.
dnl
dnl  For reference, $ARFLAGS is used by automake (1.8) for its ".a" archive
dnl  file rules.  This doesn't get used by the piecewise linking, so we
dnl  leave it at the default "cru".
dnl
dnl  FIXME: Libtool 1.5.2 has its own arrangments for "cq", but that version
dnl  is broken in other ways.  When we can upgrade, remove the forcible
dnl  AR_FLAGS=cq.

AC_DEFUN([GMP_PROG_AR],
[dnl  Want to establish $AR before libtool initialization.
AC_BEFORE([$0],[AC_PROG_LIBTOOL])
gmp_user_AR=$AR
AC_CHECK_TOOL(AR, ar, ar)
if test -z "$gmp_user_AR"; then
                        eval arflags=\"\$ar${abi1}_flags\"
  test -n "$arflags" || eval arflags=\"\$ar${abi2}_flags\"
  if test -n "$arflags"; then
    AC_MSG_CHECKING([for extra ar flags])
    AR="$AR $arflags"
    ac_cv_prog_AR="$AR $arflags"
    ac_cv_prog_ac_ct_AR="$AR $arflags"
    AC_MSG_RESULT([$arflags])
  fi
fi
if test -z "$AR_FLAGS"; then
  AR_FLAGS=cq
fi
])


dnl  GMP_PROG_M4
dnl  -----------
dnl  Find a working m4, either in $PATH or likely locations, and setup $M4
dnl  and an AC_SUBST accordingly.  If $M4 is already set then it's a user
dnl  choice and is accepted with no checks.  GMP_PROG_M4 is like
dnl  AC_PATH_PROG or AC_CHECK_PROG, but tests each m4 found to see if it's
dnl  good enough.
dnl 
dnl  See mpn/asm-defs.m4 for details on the known bad m4s.

AC_DEFUN([GMP_PROG_M4],
[AC_ARG_VAR(M4,[m4 macro processor])
AC_CACHE_CHECK([for suitable m4],
                gmp_cv_prog_m4,
[if test -n "$M4"; then
  gmp_cv_prog_m4="$M4"
else
  cat >conftest.m4 <<\EOF
dnl  Must protect this against being expanded during autoconf m4!
dnl  Dont put "dnl"s in this as autoconf will flag an error for unexpanded
dnl  macros.
[define(dollarhash,``$][#'')ifelse(dollarhash(x),1,`define(t1,Y)',
``bad: $][# not supported (SunOS /usr/bin/m4)
'')ifelse(eval(89),89,`define(t2,Y)',
`bad: eval() doesnt support 8 or 9 in a constant (OpenBSD 2.6 m4)
')ifelse(t1`'t2,YY,`good
')]
EOF
dnl ' <- balance the quotes for emacs sh-mode
  echo "trying m4" >&AC_FD_CC
  gmp_tmp_val=`(m4 conftest.m4) 2>&AC_FD_CC`
  echo "$gmp_tmp_val" >&AC_FD_CC
  if test "$gmp_tmp_val" = good; then
    gmp_cv_prog_m4="m4"
  else
    IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS=":"
dnl $ac_dummy forces splitting on constant user-supplied paths.
dnl POSIX.2 word splitting is done only on the output of word expansions,
dnl not every word.  This closes a longstanding sh security hole.
    ac_dummy="$PATH:/usr/5bin"
    for ac_dir in $ac_dummy; do
      test -z "$ac_dir" && ac_dir=.
      echo "trying $ac_dir/m4" >&AC_FD_CC
      gmp_tmp_val=`($ac_dir/m4 conftest.m4) 2>&AC_FD_CC`
      echo "$gmp_tmp_val" >&AC_FD_CC
      if test "$gmp_tmp_val" = good; then
        gmp_cv_prog_m4="$ac_dir/m4"
        break
      fi
    done
    IFS="$ac_save_ifs"
    if test -z "$gmp_cv_prog_m4"; then
      AC_MSG_ERROR([No usable m4 in \$PATH or /usr/5bin (see config.log for reasons).])
    fi
  fi
  rm -f conftest.m4
fi])
M4="$gmp_cv_prog_m4"
AC_SUBST(M4)
])


dnl  GMP_M4_M4WRAP_SPURIOUS
dnl  ----------------------
dnl  Check for spurious output from m4wrap(), as described in mpn/asm-defs.m4.
dnl
dnl  The following systems have been seen with the problem.
dnl
dnl  - Unicos alpha, but its assembler doesn't seem to mind.
dnl  - MacOS X Darwin, its assembler fails.
dnl  - NetBSD 1.4.1 m68k, and gas 1.92.3 there gives a warning and ignores
dnl    the bad last line since it doesn't have a newline.
dnl  - NetBSD 1.4.2 alpha, but its assembler doesn't seem to mind.
dnl  - HP-UX ia64.
dnl
dnl  Enhancement: Maybe this could be in GMP_PROG_M4, and attempt to prefer
dnl  an m4 with a working m4wrap, if it can be found.

AC_DEFUN([GMP_M4_M4WRAP_SPURIOUS],
[AC_REQUIRE([GMP_PROG_M4])
AC_CACHE_CHECK([if m4wrap produces spurious output],
               gmp_cv_m4_m4wrap_spurious,
[# hide the d-n-l from autoconf's error checking
tmp_d_n_l=d""nl
cat >conftest.m4 <<EOF
[changequote({,})define(x,)m4wrap({x})$tmp_d_n_l]
EOF
echo test input is >&AC_FD_CC
cat conftest.m4 >&AC_FD_CC
tmp_chars=`$M4 conftest.m4 | wc -c`
echo produces $tmp_chars chars output >&AC_FD_CC
rm -f conftest.m4
if test $tmp_chars = 0; then
  gmp_cv_m4_m4wrap_spurious=no
else
  gmp_cv_m4_m4wrap_spurious=yes
fi
])
GMP_DEFINE_RAW(["define(<M4WRAP_SPURIOUS>,<$gmp_cv_m4_m4wrap_spurious>)"])
])


dnl  GMP_PROG_NM
dnl  -----------
dnl  GMP additions to libtool AC_PROG_NM.
dnl
dnl  Note that if AC_PROG_NM can't find a working nm it still leaves
dnl  $NM set to "nm", so $NM can't be assumed to actually work.
dnl
dnl  A user-selected $NM is always left unchanged.  AC_PROG_NM is still run
dnl  to get the "checking" message printed though.
dnl
dnl  Perhaps it'd be worthwhile checking that nm works, by running it on an
dnl  actual object file.  For instance on sparcv9 solaris old versions of
dnl  GNU nm don't recognise 64-bit objects.  Checking would give a better
dnl  error message than just a failure in later tests like GMP_ASM_W32 etc.
dnl
dnl  On the other hand it's not really normal autoconf practice to take too
dnl  much trouble over detecting a broken set of tools.  And libtool doesn't
dnl  do anything at all for say ranlib or strip.  So for now we're inclined
dnl  to just demand that the user provides a coherent environment.

AC_DEFUN([GMP_PROG_NM],
[dnl  Make sure we're the first to call AC_PROG_NM, so our extra flags are
dnl   used by everyone.
AC_BEFORE([$0],[AC_PROG_NM])
gmp_user_NM=$NM
AC_PROG_NM

# FIXME: When cross compiling (ie. $ac_tool_prefix not empty), libtool
# defaults to plain "nm" if a "${ac_tool_prefix}nm" is not found.  In this
# case run it again to try the native "nm", firstly so that likely locations
# are searched, secondly so that -B or -p are added if necessary for BSD
# format.  This is necessary for instance on OSF with "./configure
# --build=alphaev5-dec-osf --host=alphaev6-dec-osf".
#
if test -z "$gmp_user_NM" && test -n "$ac_tool_prefix" && test "$NM" = nm; then
  $as_unset lt_cv_path_NM
  gmp_save_ac_tool_prefix=$ac_tool_prefix
  ac_tool_prefix=
  NM=
  AC_PROG_NM
  ac_tool_prefix=$gmp_save_ac_tool_prefix
fi

if test -z "$gmp_user_NM"; then
                        eval nmflags=\"\$nm${abi1}_flags\"
  test -n "$nmflags" || eval nmflags=\"\$nm${abi2}_flags\"
  if test -n "$nmflags"; then
    AC_MSG_CHECKING([for extra nm flags])
    NM="$NM $nmflags"
    AC_MSG_RESULT([$nmflags])
  fi
fi
])


dnl  GMP_PROG_CC_WORKS(cc+cflags,[ACTION-IF-WORKS][,ACTION-IF-NOT-WORKS])
dnl  --------------------------------------------------------------------
dnl  Check if cc+cflags can compile and link.
dnl
dnl  This test is designed to be run repeatedly with different cc+cflags
dnl  selections, so the result is not cached.
dnl
dnl  For a native build, meaning $cross_compiling == no, we require that the
dnl  generated program will run.  This is the same as AC_PROG_CC does in
dnl  _AC_COMPILER_EXEEXT_WORKS, and checking here will ensure we don't pass
dnl  a CC/CFLAGS combination that it rejects.
dnl
dnl  sparc-*-solaris2.7 can compile ABI=64 but won't run it if the kernel
dnl  was booted in 32-bit mode.  The effect of requiring the compiler output
dnl  will run is that a plain native "./configure" falls back on ABI=32, but
dnl  ABI=64 is still available as a cross-compile.
dnl
dnl  The various specific problems we try to detect are done in separate
dnl  compiles.  Although this is probably a bit slower than one test
dnl  program, it makes it easy to indicate the problem in AC_MSG_RESULT,
dnl  hence giving the user a clue about why we rejected the compiler.

AC_DEFUN([GMP_PROG_CC_WORKS],
[AC_MSG_CHECKING([compiler $1])
gmp_prog_cc_works=yes

# first see a simple "main()" works, then go on to other checks
GMP_PROG_CC_WORKS_PART([$1], [])

GMP_PROG_CC_WORKS_PART([$1], [function pointer return],
[/* The following provokes an internal error from gcc 2.95.2 -mpowerpc64
   (without -maix64), hence detecting an unusable compiler */
void *g() { return (void *) 0; }
void *f() { return g(); }
])

GMP_PROG_CC_WORKS_PART([$1], [cmov instruction],
[/* The following provokes an invalid instruction syntax from i386 gcc
   -march=pentiumpro on Solaris 2.8.  The native sun assembler
   requires a non-standard syntax for cmov which gcc (as of 2.95.2 at
   least) doesn't know.  */
int n;
int cmov () { return (n >= 0 ? n : 0); }
])

GMP_PROG_CC_WORKS_PART([$1], [double -> ulong conversion],
[/* The following provokes a linker invocation problem with gcc 3.0.3
   on AIX 4.3 under "-maix64 -mpowerpc64 -mcpu=630".  The -mcpu=630
   option causes gcc to incorrectly select the 32-bit libgcc.a, not
   the 64-bit one, and consequently it misses out on the __fixunsdfdi
   helper (double -> uint64 conversion).  */
double d;
unsigned long gcc303 () { return (unsigned long) d; }
])

GMP_PROG_CC_WORKS_PART([$1], [double negation],
[/* The following provokes an error from hppa gcc 2.95 under -mpa-risc-2-0 if
   the assembler doesn't know hppa 2.0 instructions.  fneg is a 2.0
   instruction, and a negation like this comes out using it.  */
double fneg_data;
unsigned long fneg () { return -fneg_data; }
])

GMP_PROG_CC_WORKS_PART([$1], [double -> float conversion],
[/* The following makes gcc 3.3 -march=pentium4 generate an SSE2 xmm insn
   (cvtsd2ss) which will provoke an error if the assembler doesn't recognise
   those instructions.  Not sure how much of the gmp code will come out
   wanting sse2, but it's easiest to reject an option we know is bad.  */
double ftod_data;
float ftod () { return (float) ftod_data; }
])

GMP_PROG_CC_WORKS_PART([$1], [gnupro alpha ev6 char spilling],
[/* The following provokes an internal compiler error from gcc version
   "2.9-gnupro-99r1" under "-O2 -mcpu=ev6", apparently relating to char
   values being spilled into floating point registers.  The problem doesn't
   show up all the time, but has occurred enough in GMP for us to reject
   this compiler+flags.  */
struct try_t
{
 char dst[2];
 char size;
 long d0, d1, d2, d3, d4, d5, d6;
 char overlap;
};
struct try_t param[6];
int
param_init ()
{
 struct try_t *p;
 memcpy (p, &param[ 2 ], sizeof (*p));
 memcpy (p, &param[ 2 ], sizeof (*p));
 p->size = 2;
 memcpy (p, &param[ 1 ], sizeof (*p));
 p->dst[0] = 1;
 p->overlap = 2;
 memcpy (p, &param[ 3 ], sizeof (*p));
 p->dst[0] = 1;
 p->overlap = 8;
 memcpy (p, &param[ 4 ], sizeof (*p));
 memcpy (p, &param[ 4 ], sizeof (*p));
 p->overlap = 8;
 memcpy (p, &param[ 5 ], sizeof (*p));
 memcpy (p, &param[ 5 ], sizeof (*p));
 memcpy (p, &param[ 5 ], sizeof (*p));
 return 0;
}
])

# __builtin_alloca is not available everywhere, check it exists before
# seeing that it works
GMP_PROG_CC_WORKS_PART_TEST([$1],[__builtin_alloca availability],
[int k; int foo () { __builtin_alloca (k); }],
  [GMP_PROG_CC_WORKS_PART([$1], [alloca array],
[/* The following provokes an internal compiler error from Itanium HP-UX cc
    under +O2 or higher.  We use this sort of code in mpn/generic/mul_fft.c. */
int k;
int foo ()
{
  int i, **a;
  a = __builtin_alloca (k);
  for (i = 0; i <= k; i++)
    a[i] = __builtin_alloca (1 << i);
}
])])

GMP_PROG_CC_WORKS_PART([$1], [abs int -> double conversion],
[/* The following provokes an internal error from the assembler on
   power2-ibm-aix4.3.1.0.  gcc -mrios2 compiles to nabs+fcirz, and this
   results in "Internal error related to the source program domain".

   For reference it seems to be the combination of nabs+fcirz which is bad,
   not either alone.  This sort of thing occurs in mpz/get_str.c with the
   way double chars_per_bit_exactly is applied in MPN_SIZEINBASE.  Perhaps
   if that code changes to a scaled-integer style then we won't need this
   test.  */

double fp[1];
int x;
int f ()
{
  int a;
  a = (x >= 0 ? x : -x);
  return a * fp[0];
}
])

GMP_PROG_CC_WORKS_PART([$1], [long long reliability test 1],
[/* The following provokes a segfault in the compiler on powerpc-apple-darwin.
   Extracted from tests/mpn/t-iord_u.c.  Causes Apple's gcc 3.3 build 1640 and
   1666 to segfault with e.g., -O2 -mpowerpc64.  */

#ifdef __GNUC__
typedef unsigned long long t1;typedef t1*t2;
__inline__ t1 e(t2 rp,t2 up,int n,t1 v0)
{t1 c,x,r;int i;if(v0){c=1;for(i=1;i<n;i++){x=up[i];r=x+1;rp[i]=r;}}return c;}
f(){static const struct{t1 n;t1 src[9];t1 want[9];}d[]={{1,{0},{1}},};t1 got[9];int i;
for(i=0;i<1;i++){if(e(got,got,9,d[i].n)==0)h();g(i,d[i].src,d[i].n,got,d[i].want,9);if(d[i].n)h();}}
h(){}g(){}
#else
int dummy;
#endif
])

GMP_PROG_CC_WORKS_PART([$1], [long long reliability test 2],
[/* The following provokes an internal compiler error on powerpc-apple-darwin.
   Extracted from mpz/cfdiv_q_2exp.c.  Causes Apple's gcc 3.3 build 1640 and
   1666 to get an ICE with -O1 -mpowerpc64.  */

#ifdef __GNUC__
f(int u){int i;long long x;x=u?~0:0;if(x)for(i=0;i<9;i++);x&=g();if(x)g();}
g(){}
#else
int dummy;
#endif
])

GMP_PROG_CC_WORKS_PART_MAIN([$1], [mpn_lshift_com optimization],
[/* The following is mis-compiled by HP ia-64 cc version
        cc: HP aC++/ANSI C B3910B A.05.55 [Dec 04 2003]
   under "cc +O3", both in +DD32 and +DD64 modes.  The mpn_lshift_com gets
   inlined and its return value somehow botched to be 0 instead of 1.  This
   arises in the real mpn_lshift_com in mul_fft.c.  A lower optimization
   level, like +O2 seems ok.  This code needs to be run to show the problem,
   but that's fine, the offending cc is a native-only compiler so we don't
   have to worry about cross compiling.  */

unsigned long
lshift_com (rp, up, n, cnt)
  unsigned long *rp;
  unsigned long *up;
  long n;
  unsigned cnt;
{
  unsigned long retval, high_limb, low_limb;
  unsigned tnc;
  long i;
  tnc = 8 * sizeof (unsigned long) - cnt;
  low_limb = *up++;
  retval = low_limb >> tnc;
  high_limb = low_limb << cnt;
  for (i = n - 1; i != 0; i--)
    {
      low_limb = *up++;
      *rp++ = ~(high_limb | (low_limb >> tnc));
      high_limb = low_limb << cnt;
    }
  return retval;
}
int
main ()
{
  unsigned long cy, rp[2], up[2];
  up[0] = ~ 0L;
  up[1] = 0;
  cy = lshift_com (rp, up, 2L, 1);
  if (cy != 1L)
    return 1;
  return 0;
}
])

GMP_PROG_CC_WORKS_PART_MAIN([$1], [mpn_lshift_com optimization 2],
[/* The following is mis-compiled by Intel ia-64 icc version 1.8 under
    "icc -O3",  After several calls, the function writes parial garbage to
    the result vector.  Perhaps relates to the chk.a.nc insn.  This code needs
    to be run to show the problem, but that's fine, the offending cc is a
    native-only compiler so we don't have to worry about cross compiling.  */

#include <stdlib.h>
void
lshift_com (rp, up, n, cnt)
  unsigned long *rp;
  unsigned long *up;
  long n;
  unsigned cnt;
{
  unsigned long high_limb, low_limb;
  unsigned tnc;
  long i;
  up += n;
  rp += n;
  tnc = 8 * sizeof (unsigned long) - cnt;
  low_limb = *--up;
  high_limb = low_limb << cnt;
  for (i = n - 1; i != 0; i--)
    {
      low_limb = *--up;
      *--rp = ~(high_limb | (low_limb >> tnc));
      high_limb = low_limb << cnt;
    }
  *--rp = ~high_limb;
}
int
main ()
{
  unsigned long *r, *r2;
  unsigned long a[88 + 1];
  long i;
  for (i = 0; i < 88 + 1; i++)
    a[i] = ~0L;
  r = malloc (10000 * sizeof (unsigned long));
  r2 = r;
  for (i = 0; i < 528; i += 22)
    {
      lshift_com (r2, a,
		  i / (8 * sizeof (unsigned long)) + 1,
		  i % (8 * sizeof (unsigned long)));
      r2 += 88 + 1;
    }
  if (r[2048] != 0 || r[2049] != 0 || r[2050] != 0 || r[2051] != 0 ||
      r[2052] != 0 || r[2053] != 0 || r[2054] != 0)
    abort ();
  return 0;
}
])


# A certain _GLOBAL_OFFSET_TABLE_ problem in past versions of gas, tickled
# by recent versions of gcc.
#
if test "$gmp_prog_cc_works" = yes; then
  case $host in
    X86_PATTERN)
      # this problem only arises in PIC code, so don't need to test when
      # --disable-shared.  We don't necessarily have $enable_shared set to
      # yes at this point, it will still be unset for the default (which is
      # yes); hence the use of "!= no".
      if test "$enable_shared" != no; then
        GMP_PROG_CC_X86_GOT_EAX_EMITTED([$1],
          [GMP_ASM_X86_GOT_EAX_OK([$1],,
            [gmp_prog_cc_works="no, bad gas GOT with eax"])])
      fi
      ;;
  esac
fi

AC_MSG_RESULT($gmp_prog_cc_works)
case $gmp_prog_cc_works in
  yes)
    [$2]
    ;;
  *)
    [$3]
    ;;
esac
])

dnl  Called: GMP_PROG_CC_WORKS_PART(CC+CFLAGS,FAIL-MESSAGE [,CODE])
dnl  A dummy main() is appended to the CODE given.
dnl
AC_DEFUN([GMP_PROG_CC_WORKS_PART],
[GMP_PROG_CC_WORKS_PART_MAIN([$1],[$2],
[$3]
[int main () { return 0; }])
])

dnl  Called: GMP_PROG_CC_WORKS_PART_MAIN(CC+CFLAGS,FAIL-MESSAGE,CODE)
dnl  CODE must include a main().
dnl
AC_DEFUN([GMP_PROG_CC_WORKS_PART_MAIN],
[GMP_PROG_CC_WORKS_PART_TEST([$1],[$2],[$3],
  [],
  gmp_prog_cc_works="no[]m4_if([$2],,,[[, ]])[$2]",
  gmp_prog_cc_works="no[]m4_if([$2],,,[[, ]])[$2][[, program does not run]]")
])

dnl  Called: GMP_PROG_CC_WORKS_PART_TEST(CC+CFLAGS,TITLE,[CODE],
dnl            [ACTION-GOOD],[ACTION-BAD][ACTION-NORUN])
dnl
AC_DEFUN([GMP_PROG_CC_WORKS_PART_TEST],
[if test "$gmp_prog_cc_works" = yes; then
  # remove anything that might look like compiler output to our "||" expression
  rm -f conftest* a.out b.out a.exe a_out.exe
  cat >conftest.c <<EOF
[$3]
EOF
  echo "Test compile: [$2]" >&AC_FD_CC
  gmp_compile="$1 conftest.c >&AC_FD_CC"
  if AC_TRY_EVAL(gmp_compile); then
    cc_works_part=yes
    if test "$cross_compiling" = no; then
      if AC_TRY_COMMAND([./a.out || ./b.out || ./a.exe || ./a_out.exe || ./conftest]); then :;
      else
        cc_works_part=norun
      fi
    fi
  else
    cc_works_part=no
  fi
  if test "$cc_works_part" != yes; then
    echo "failed program was:" >&AC_FD_CC
    cat conftest.c >&AC_FD_CC
  fi
  rm -f conftest* a.out b.out a.exe a_out.exe
  case $cc_works_part in
    yes)
      $4
      ;;
    no)
      $5
      ;;
    norun)
      $6
      ;;
  esac
fi
])


dnl  GMP_PROG_CC_WORKS_LONGLONG(cc+cflags,[ACTION-YES][,ACTION-NO])
dnl  --------------------------------------------------------------
dnl  Check that cc+cflags accepts "long long".
dnl
dnl  This test is designed to be run repeatedly with different cc+cflags
dnl  selections, so the result is not cached.

AC_DEFUN([GMP_PROG_CC_WORKS_LONGLONG],
[AC_MSG_CHECKING([compiler $1 has long long])
cat >conftest.c <<EOF
long long  foo;
long long  bar () { return foo; }
int main () { return 0; }
EOF
gmp_prog_cc_works=no
gmp_compile="$1 -c conftest.c >&AC_FD_CC"
if AC_TRY_EVAL(gmp_compile); then
  gmp_prog_cc_works=yes
else
  echo "failed program was:" >&AC_FD_CC
  cat conftest.c >&AC_FD_CC
fi
rm -f conftest* a.out b.out a.exe a_out.exe
AC_MSG_RESULT($gmp_prog_cc_works)
if test $gmp_prog_cc_works = yes; then
  ifelse([$2],,:,[$2])
else
  ifelse([$3],,:,[$3])
fi
])


dnl  GMP_C_TEST_SIZEOF(cc/cflags,test,[ACTION-GOOD][,ACTION-BAD])
dnl  ------------------------------------------------------------
dnl  The given cc/cflags compiler is run to check the size of a type
dnl  specified by the "test" argument.  "test" can either be a string, or a
dnl  variable like $foo.  The value should be for instance "sizeof-long-4",
dnl  to test that sizeof(long)==4.
dnl
dnl  This test is designed to be run for different compiler and/or flags
dnl  combinations, so the result is not cached.
dnl
dnl  The idea for making an array that has a negative size if the desired
dnl  condition test is false comes from autoconf AC_CHECK_SIZEOF.  The cast
dnl  to "long" in the array dimension also follows autoconf, apparently it's
dnl  a workaround for a HP compiler bug.

AC_DEFUN([GMP_C_TEST_SIZEOF],
[echo "configure: testlist $2" >&AC_FD_CC
[gmp_sizeof_type=`echo "$2" | sed 's/sizeof-\([a-z]*\).*/\1/'`]
[gmp_sizeof_want=`echo "$2" | sed 's/sizeof-[a-z]*-\([0-9]*\).*/\1/'`]
AC_MSG_CHECKING([compiler $1 has sizeof($gmp_sizeof_type)==$gmp_sizeof_want])
cat >conftest.c <<EOF
[int
main ()
{
  static int test_array [1 - 2 * (long) (sizeof ($gmp_sizeof_type) != $gmp_sizeof_want)];
  test_array[0] = 0;
  return 0;
}]
EOF
gmp_c_testlist_sizeof=no
gmp_compile="$1 -c conftest.c >&AC_FD_CC"
if AC_TRY_EVAL(gmp_compile); then
  gmp_c_testlist_sizeof=yes
fi
rm -f conftest*
AC_MSG_RESULT($gmp_c_testlist_sizeof)
if test $gmp_c_testlist_sizeof = yes; then
  ifelse([$3],,:,[$3])
else
  ifelse([$4],,:,[$4])
fi
])


dnl  GMP_PROG_CC_IS_GNU(CC,[ACTIONS-IF-YES][,ACTIONS-IF-NO])
dnl  -------------------------------------------------------
dnl  Determine whether the given compiler is GNU C.
dnl
dnl  This test is the same as autoconf _AC_LANG_COMPILER_GNU, but doesn't
dnl  cache the result.  The same "ifndef" style test is used, to avoid
dnl  problems with syntax checking cpp's used on NeXT and Apple systems.

AC_DEFUN([GMP_PROG_CC_IS_GNU],
[cat >conftest.c <<EOF
#if ! defined (__GNUC__) || defined (__INTEL_COMPILER)
  choke me
#endif
EOF
gmp_compile="$1 -c conftest.c >&AC_FD_CC"
if AC_TRY_EVAL(gmp_compile); then
  rm -f conftest*
  AC_MSG_CHECKING([whether $1 is gcc])
  AC_MSG_RESULT(yes)
  ifelse([$2],,:,[$2])
else
  rm -f conftest*
  ifelse([$3],,:,[$3])
fi
])


dnl  GMP_PROG_CC_IS_XLC(CC,[ACTIONS-IF-YES][,ACTIONS-IF-NO])
dnl  -------------------------------------------------------
dnl  Determine whether the given compiler is IBM xlc (on AIX).
dnl
dnl  There doesn't seem to be a preprocessor symbol to test for this, or if
dnl  there is one then it's well hidden in xlc 3.1 on AIX 4.3, so just grep
dnl  the man page printed when xlc is invoked with no arguments.

AC_DEFUN([GMP_PROG_CC_IS_XLC],
[gmp_command="$1 2>&1 | grep xlc >/dev/null"
if AC_TRY_EVAL(gmp_command); then
  AC_MSG_CHECKING([whether $1 is xlc])
  AC_MSG_RESULT(yes)
  ifelse([$2],,:,[$2])
else
  ifelse([$3],,:,[$3])
fi
])


dnl  GMP_PROG_CC_X86_GOT_EAX_EMITTED(CC+CFLAGS, [ACTION-YES] [, ACTION-NO])
dnl  ----------------------------------------------------------------------
dnl  Determine whether CC+CFLAGS emits instructions using %eax with
dnl  _GLOBAL_OFFSET_TABLE_.  This test is for use on x86 systems.
dnl
dnl  Recent versions of gcc will use %eax for the GOT in leaf functions, for
dnl  instance gcc 3.3.3 with -O3.  This avoids having to save and restore
dnl  %ebx which otherwise usually holds the GOT, and is what gcc used in the
dnl  past.
dnl
dnl  %ecx and %edx are also candidates for this sort of optimization, and
dnl  are used under lesser optimization levels, like -O2 in 3.3.3.  FIXME:
dnl  It's not quite clear what the conditions for using %eax are, we might
dnl  need more test code to provoke it.
dnl
dnl  The motivation for this test is that past versions of gas have bugs
dnl  affecting this usage, see GMP_ASM_X86_GOT_EAX_OK.
dnl
dnl  This test is not specific to gcc, other compilers might emit %eax GOT
dnl  insns like this, though we've not investigated that.
dnl
dnl  This is for use by compiler probing in GMP_PROG_CC_WORKS, so we doesn't
dnl  cache the result.
dnl
dnl  -fPIC is hard coded here, because this test is for use before libtool
dnl  has established the pic options.  It's right for gcc, but perhaps not
dnl  other compilers.

AC_DEFUN([GMP_PROG_CC_X86_GOT_EAX_EMITTED],
[echo "Testing gcc GOT with eax emitted" >&AC_FD_CC
cat >conftest.c <<\EOF
[int foo;
int bar () { return foo; }
]EOF
tmp_got_emitted=no
gmp_compile="$1 -fPIC -S conftest.c >&AC_FD_CC 2>&1"
if AC_TRY_EVAL(gmp_compile); then
  if grep "addl.*_GLOBAL_OFFSET_TABLE_.*eax" conftest.s >/dev/null; then
    tmp_got_emitted=yes
  fi
fi
rm -f conftest.*
echo "Result: $tmp_got_emitted" >&AC_FD_CC
if test "$tmp_got_emitted" = yes; then
  ifelse([$2],,:,[$2])
else
  ifelse([$3],,:,[$3])
fi
])


dnl  GMP_HPC_HPPA_2_0(cc,[ACTION-IF-GOOD][,ACTION-IF-BAD])
dnl  ---------------------------------------------------------
dnl  Find out whether a HP compiler is good enough to generate hppa 2.0.
dnl
dnl  This test might be repeated for different compilers, so the result is
dnl  not cached.

AC_DEFUN([GMP_HPC_HPPA_2_0],
[AC_MSG_CHECKING([whether HP compiler $1 is good for 64-bits])
# Bad compiler output:
#   ccom: HP92453-01 G.10.32.05 HP C Compiler
# Good compiler output:
#   ccom: HP92453-01 A.10.32.30 HP C Compiler
# Let A.10.32.30 or higher be ok.
echo >conftest.c
gmp_tmp_vs=`$1 $2 -V -c -o conftest.$OBJEXT conftest.c 2>&1 | grep "^ccom:"`
echo "Version string: $gmp_tmp_vs" >&AC_FD_CC
rm conftest*
gmp_tmp_v1=`echo $gmp_tmp_vs | sed 's/.* .\.\([[0-9]]*\).*/\1/'`
gmp_tmp_v2=`echo $gmp_tmp_vs | sed 's/.* .\..*\.\(.*\)\..* HP C.*/\1/'`
gmp_tmp_v3=`echo $gmp_tmp_vs | sed 's/.* .\..*\..*\.\(.*\) HP C.*/\1/'`
echo "Version number: $gmp_tmp_v1.$gmp_tmp_v2.$gmp_tmp_v3" >&AC_FD_CC
if test -z "$gmp_tmp_v1"; then
  gmp_hpc_64bit=not-applicable
else
  GMP_COMPARE_GE($gmp_tmp_v1, 10, $gmp_tmp_v2, 32, $gmp_tmp_v3, 30)
  gmp_hpc_64bit=$gmp_compare_ge
fi
AC_MSG_RESULT($gmp_hpc_64bit)
if test $gmp_hpc_64bit = yes; then
  ifelse([$2],,:,[$2])
else
  ifelse([$3],,:,[$3])
fi
])


dnl  GMP_GCC_ARM_UMODSI(CC,[ACTIONS-IF-GOOD][,ACTIONS-IF-BAD])
dnl  ---------------------------------------------------------
dnl  gcc 2.95.3 and earlier on arm has a bug in the libgcc __umodsi routine
dnl  making "%" give wrong results for some operands, eg. "0x90000000 % 3".
dnl  We're hoping it'll be fixed in 2.95.4, and we know it'll be fixed in
dnl  gcc 3.
dnl
dnl  There's only a couple of places gmp cares about this, one is the
dnl  size==1 case in mpn/generic/mode1o.c, and this shows up in
dnl  tests/mpz/t-jac.c as a wrong result from mpz_kronecker_ui.

AC_DEFUN([GMP_GCC_ARM_UMODSI],
[AC_MSG_CHECKING([whether ARM gcc unsigned division works])
tmp_version=`$1 --version`
echo "$tmp_version" >&AC_FD_CC
case $tmp_version in
  [2.95 | 2.95.[123]])
    ifelse([$3],,:,[$3])
    gmp_gcc_arm_umodsi_result=["no, gcc 2.95.[0123]"] ;;
  *)
    ifelse([$2],,:,[$2])
    gmp_gcc_arm_umodsi_result=yes ;;
esac
AC_MSG_RESULT([$gmp_gcc_arm_umodsi_result])
])


dnl  GMP_GCC_MIPS_O32(gcc,[actions-yes][,[actions-no]])
dnl  -------------------------------------------------
dnl  Test whether gcc supports o32.
dnl
dnl  gcc 2.7.2.2 only does o32, and doesn't accept -mabi=32.
dnl
dnl  gcc 2.95 accepts -mabi=32 but it only works on irix5, on irix6 it gives
dnl  "cc1: The -mabi=32 support does not work yet".

AC_DEFUN([GMP_GCC_MIPS_O32],
[AC_MSG_CHECKING([whether gcc supports o32])
echo 'int x;' >conftest.c
echo "$1 -mabi=32 -c conftest.c" >&AC_FD_CC
if $1 -mabi=32 -c conftest.c >conftest.out 2>&1; then
  result=yes
else
  cat conftest.out >&AC_FD_CC
  if grep "cc1: Invalid option \`abi=32'" conftest.out >/dev/null; then
    result=yes
  else
    result=no
  fi
fi
rm -f conftest.*
AC_MSG_RESULT($result)
if test $result = yes; then
  ifelse([$2],,:,[$2])
else
  ifelse([$3],,:,[$3])
fi
])


dnl  GMP_GCC_NO_CPP_PRECOMP(CCBASE,CC,CFLAGS,[ACTIONS-YES][,ACTIONS-NO])
dnl  -------------------------------------------------------------------
dnl  Check whether -no-cpp-precomp should be used on this compiler, and
dnl  execute the corresponding ACTIONS-YES or ACTIONS-NO.
dnl
dnl  -no-cpp-precomp is only meant for Apple's hacked version of gcc found
dnl  on powerpc*-*-darwin*, but we can give it a try on any gcc.  Normal gcc
dnl  (as of 3.0 at least) only gives a warning, not an actual error, and we
dnl  watch for that and decide against the option in that case, to avoid
dnl  confusing the user.

AC_DEFUN([GMP_GCC_NO_CPP_PRECOMP],
[if test "$ccbase" = gcc; then
  AC_MSG_CHECKING([compiler $2 $3 -no-cpp-precomp])
  result=no
  cat >conftest.c <<EOF
int main () { return 0; }
EOF
  gmp_compile="$2 $3 -no-cpp-precomp conftest.c >conftest.out 2>&1"
  if AC_TRY_EVAL(gmp_compile); then
    if grep "unrecognized option.*-no-cpp-precomp" conftest.out >/dev/null; then : ;
    else
      result=yes
    fi
  fi
  cat conftest.out >&AC_FD_CC
  rm -f conftest* a.out b.out a.exe a_out.exe
  AC_MSG_RESULT($result)
  if test "$result" = yes; then
      ifelse([$4],,:,[$4])
  else
      ifelse([$5],,:,[$5])
  fi
fi
])


dnl  GMP_GCC_PENTIUM4_SSE2(CC+CFLAGS,[ACTION-IF-YES][,ACTION-IF-NO])
dnl  ---------------------------------------------------------------
dnl  Determine whether gcc CC+CFLAGS is a good enough version for
dnl  -march=pentium4 with sse2.
dnl
dnl  Gcc 3.2.1 was seen generating incorrect code for raw double -> int
dnl  conversions through a union.  We believe the problem is in all 3.1 and
dnl  3.2 versions, but that it's fixed in 3.3.

AC_DEFUN([GMP_GCC_PENTIUM4_SSE2],
[AC_MSG_CHECKING([whether gcc is good for sse2])
case `$1 -dumpversion` in
  [3.[012] | 3.[012].*]) result=no ;;
  *)                     result=yes ;;
esac
AC_MSG_RESULT($result)
if test "$result" = yes; then
  ifelse([$2],,:,[$2])
else
  ifelse([$3],,:,[$3])
fi
])


dnl  GMP_GCC_WA_MCPU(CC+CFLAGS, NEWFLAG [,ACTION-YES [,ACTION-NO]])
dnl  --------------------------------------------------------------
dnl  Check whether gcc (or gas rather) accepts a flag like "-Wa,-mev67".
dnl
dnl  Gas doesn't give an error for an unknown cpu, it only prints a warning
dnl  like "Warning: Unknown CPU identifier `ev78'".
dnl
dnl  This is intended for use on alpha, since only recent versions of gas
dnl  accept -mev67, but there's nothing here that's alpha specific.

AC_DEFUN([GMP_GCC_WA_MCPU],
[AC_MSG_CHECKING([assembler $1 $2])
result=no
cat >conftest.c <<EOF
int main () {}
EOF
gmp_compile="$1 $2 -c conftest.c >conftest.out 2>&1"
if AC_TRY_EVAL(gmp_compile); then
  if grep "Unknown CPU identifier" conftest.out >/dev/null; then : ;
  else
    result=yes
  fi    
fi
cat conftest.out >&AC_FD_CC
rm -f conftest*
AC_MSG_RESULT($result)
if test "$result" = yes; then
  ifelse([$3],,:,[$3])
else
  ifelse([$4],,:,[$4])
fi
])


dnl  GMP_GCC_WA_OLDAS(CC+CFLAGS [,ACTION-YES [,ACTION-NO]])
dnl  ------------------------------------------------------
dnl  Check whether gcc should be run with "-Wa,-oldas".
dnl
dnl  On systems alpha*-*-osf* (or maybe just osf5), apparently there's a
dnl  newish Compaq "as" which doesn't work with the gcc mips-tfile.
dnl  Compiling an empty file with "gcc -c foo.c" produces for instance
dnl
dnl      mips-tfile, /tmp/ccaqUNnF.s:7 Segmentation fault
dnl
dnl  The fix is to pass "-oldas" to that assembler, as noted by
dnl
dnl      http://gcc.gnu.org/install/specific.html#alpha*-dec-osf*
dnl
dnl  The test here tries to compile an empty file, and if that fails but
dnl  adding -Wa,-oldas makes it succeed, then that flag is considered
dnl  necessary.
dnl
dnl  We look for the failing case specifically, since it may not be a good
dnl  idea to use -Wa,-oldas in other circumstances.  For instance gas takes
dnl  "-oldas" to mean the "-o" option and will write a file called "ldas" as
dnl  its output.  Normally gcc puts its own "-o" after any -Wa options, so
dnl  -oldas ends up being harmless, but clearly that's only through good
dnl  luck.
dnl
dnl  This macro is designed for use while probing for a good compiler, and
dnl  so doesn't cache it's result.

AC_DEFUN([GMP_GCC_WA_OLDAS],
[AC_MSG_CHECKING([for $1 -Wa,-oldas])
result=no
cat >conftest.c <<EOF
EOF
echo "with empty conftest.c" >&AC_FD_CC
gmp_compile="$1 -c conftest.c >&AC_FD_CC 2>&1"
if AC_TRY_EVAL(gmp_compile); then : ;
else
  # empty fails
  gmp_compile="$1 -Wa,-oldas -c conftest.c >&AC_FD_CC 2>&1"
  if AC_TRY_EVAL(gmp_compile); then
    # but with -Wa,-oldas it works
    result=yes
  fi
fi
rm -f conftest*
AC_MSG_RESULT($result)
if test "$result" = yes; then
  ifelse([$2],,:,[$2])
else
  ifelse([$3],,:,[$3])
fi
])


dnl  GMP_OS_X86_XMM(CC+CFLAGS,[ACTION-IF-YES][,ACTION-IF-NO])
dnl  --------------------------------------------------------
dnl  Determine whether the operating system supports XMM registers.
dnl
dnl  If build==host then a test program is run, executing an SSE2
dnl  instruction using an XMM register.  This will give a SIGILL if the
dnl  system hasn't set the OSFXSR bit in CR4 to say it knows it must use
dnl  fxsave/fxrestor in a context switch (to save xmm registers).
dnl
dnl  If build!=host, we can fallback on:
dnl
dnl      - FreeBSD version 4 is the first supporting xmm.
dnl
dnl      - Linux kernel 2.4 might be the first stable series supporting xmm
dnl        (not sure).  But there's no version number in the GNU/Linux
dnl        config tuple to test anyway.
dnl
dnl  The default is to allow xmm.  This might seem rash, but it's likely
dnl  most systems know xmm by now, so this will normally be what's wanted.
dnl  And cross compiling is a bit hairy anyway, so hopefully anyone doing it
dnl  will be smart enough to know what to do.
dnl
dnl  In the test program, .text and .globl are hard coded because this macro
dnl  is wanted before GMP_ASM_TEXT and GMP_ASM_GLOBL are run.  A .byte
dnl  sequence is used (for xorps %xmm0, %xmm0) to make us independent of
dnl  tests for whether the assembler supports sse2/xmm.  Obviously we need
dnl  both assembler and OS support, but this means we don't force the order
dnl  in which we test.
dnl
dnl  FIXME: Maybe we should use $CCAS to assemble, if it's set.  (Would
dnl  still want $CC/$CFLAGS for the link.)  But this test is used before
dnl  AC_PROG_CC sets $OBJEXT, so we'd need to check for various object file
dnl  suffixes ourselves.

AC_DEFUN([GMP_OS_X86_XMM],
[AC_CACHE_CHECK([whether the operating system supports XMM registers],
		gmp_cv_os_x86_xmm,
[if test "$build" = "$host"; then
  # remove anything that might look like compiler output to our "||" expression
  rm -f conftest* a.out b.out a.exe a_out.exe
  cat >conftest.s <<EOF
	.text
main:
_main:
	.globl	main
	.globl	_main
	.byte	0x0f, 0x57, 0xc0
	xorl	%eax, %eax
	ret
EOF
  gmp_compile="$1 conftest.s -o conftest >&AC_FD_CC"
  if AC_TRY_EVAL(gmp_compile); then
    if AC_TRY_COMMAND([./a.out || ./b.out || ./a.exe || ./a_out.exe || ./conftest]); then
      gmp_cv_os_x86_xmm=yes
    else
      gmp_cv_os_x86_xmm=no
    fi
  else
    AC_MSG_WARN([Oops, cannot compile test program])
  fi
  rm -f conftest*
fi

if test -z "$gmp_cv_os_x86_xmm"; then
  case $host_os in
    [freebsd[123] | freebsd[123].*])
      gmp_cv_os_x86_xmm=no ;;
    freebsd*)
      gmp_cv_os_x86_xmm=yes ;;
    *)
      gmp_cv_os_x86_xmm=probably ;;
  esac
fi
])

if test "$gmp_cv_os_x86_xmm" = probably; then
  AC_MSG_WARN([Not certain of OS support for xmm when cross compiling.])
  AC_MSG_WARN([Will assume it's ok, expect a SIGILL if this is wrong.])
fi

case $gmp_cv_os_x86_xmm in
no)
  $3
  ;;
*)
  $2
  ;;
esac
])


dnl  GMP_CRAY_HOST_TYPES(C90/T90-IEEE, C90/T90-CFP, J90/SV1)
dnl  -------------------------------------------------------
dnl  Execute the actions in the arguments on the respective Cray vector
dnl  systems.  For other hosts, do nothing.
dnl
dnl  This macro should be used after the C compiler has been chosen, since
dnl  on c90 and t90 we ask the compiler whether we're in IEEE or CFP float
dnl  mode.
dnl
dnl  This code is in a macro so that any AC_REQUIRE pre-requisites of
dnl  AC_EGREP_CPP will be expanded at the top-level, ie. for all hosts not
dnl  merely c90 and t90.  In autoconf 2.57 for instance this means
dnl  AC_PROG_EGREP, which is needed by various other macros.

AC_DEFUN([GMP_CRAY_OPTIONS],
[case $host_cpu in
  c90 | t90)
    AC_EGREP_CPP(yes,
[#ifdef _CRAYIEEE
yes
#endif],
    [$1],
    [$2])
    ;;
  j90 | sv1)
    [$3]
    ;;
esac
])


dnl  GMP_HPPA_LEVEL_20(cc/cflags [, ACTION-GOOD [,ACTION-BAD]])
dnl  ----------------------------------------------------------
dnl  Check that the given cc/cflags accepts HPPA 2.0n assembler code.
dnl
dnl  Old versions of gas don't know 2.0 instructions.  It rejects ".level
dnl  2.0" for a start, so just test that.
dnl
dnl  This test is designed to be run for various different compiler and
dnl  flags combinations, and hence doesn't cache its result.

AC_DEFUN([GMP_HPPA_LEVEL_20],
[AC_MSG_CHECKING([$1 assembler knows hppa 2.0])
result=no
cat >conftest.s <<EOF
	.level 2.0
EOF
gmp_compile="$1 -c conftest.s >&AC_FD_CC 2>&1"
if AC_TRY_EVAL(gmp_compile); then
  result=yes
else
  echo "failed program was" >&AC_FD_CC
  cat conftest.s >&AC_FD_CC
fi
rm -f conftest*
AC_MSG_RESULT($result)
if test "$result" = yes; then
  ifelse([$2],,:,[$2])
else
  ifelse([$3],,:,[$3])
fi
])


dnl  GMP_PROG_CXX_WORKS(cxx/cxxflags [, ACTION-YES [,ACTION-NO]])
dnl  ------------------------------------------------------------
dnl  Check whether cxx/cxxflags can compile and link.
dnl
dnl  This test is designed to be run repeatedly with different cxx/cxxflags
dnl  selections, so the result is not cached.
dnl
dnl  For a native build, we insist on being able to run the program, so as
dnl  to detect any problems with the standard C++ library.  During
dnl  development various systems with broken or incomplete C++ installations
dnl  were seen.
dnl
dnl  The various features and problems we try to detect are done in separate
dnl  compiles.  Although this is probably a bit slower than one test
dnl  program, it makes it easy to indicate the problem in AC_MSG_RESULT,
dnl  hence giving the user a clue about why we rejected the compiler.

AC_DEFUN([GMP_PROG_CXX_WORKS],
[AC_MSG_CHECKING([C++ compiler $1])
gmp_prog_cxx_works=yes

# start with a plain "main()", then go on to further checks
GMP_PROG_CXX_WORKS_PART([$1], [])

GMP_PROG_CXX_WORKS_PART([$1], [namespace],
[namespace foo { }
using namespace foo;
])

# GMP requires the standard C++ iostream classes
GMP_PROG_CXX_WORKS_PART([$1], [std iostream],
[/* This test rejects g++ 2.7.2 which doesn't have <iostream>, only a
    pre-standard iostream.h. */
#include <iostream>

/* This test rejects OSF 5.1 Compaq C++ in its default pre-standard iostream
   mode, since that mode puts cout in the global namespace, not "std".  */
void someoutput (void) { std::cout << 123; }
])

AC_MSG_RESULT($gmp_prog_cxx_works)
case $gmp_prog_cxx_works in
  yes)
    [$2]
    ;;
  *)
    [$3]
    ;;
esac
])

dnl  Called: GMP_PROG_CXX_WORKS_PART(CXX+CXXFLAGS, FAIL-MESSAGE [,CODE])
dnl
AC_DEFUN([GMP_PROG_CXX_WORKS_PART],
[if test "$gmp_prog_cxx_works" = yes; then
  # remove anything that might look like compiler output to our "||" expression
  rm -f conftest* a.out b.out a.exe a_out.exe
  cat >conftest.cc <<EOF
[$3]
int main (void) { return 0; }
EOF
  echo "Test compile: [$2]" >&AC_FD_CC
  gmp_cxxcompile="$1 conftest.cc >&AC_FD_CC"
  if AC_TRY_EVAL(gmp_cxxcompile); then
    if test "$cross_compiling" = no; then
      if AC_TRY_COMMAND([./a.out || ./b.out || ./a.exe || ./a_out.exe || ./conftest]); then :;
      else
        gmp_prog_cxx_works="no[]m4_if([$2],,,[, ])[$2], program does not run"
      fi
    fi
  else
    gmp_prog_cxx_works="no[]m4_if([$2],,,[, ])[$2]"
  fi
  case $gmp_prog_cxx_works in
    no*)
      echo "failed program was:" >&AC_FD_CC
      cat conftest.cc >&AC_FD_CC
      ;;
  esac
  rm -f conftest* a.out b.out a.exe a_out.exe
fi
])


dnl  GMP_INIT([M4-DEF-FILE])
dnl  -----------------------
dnl  Initializations for GMP config.m4 generation.
dnl
dnl  FIXME: The generated config.m4 doesn't get recreated by config.status.
dnl  Maybe the relevant "echo"s should go through AC_CONFIG_COMMANDS.

AC_DEFUN([GMP_INIT],
[ifelse([$1], , gmp_configm4=config.m4, gmp_configm4="[$1]")
gmp_tmpconfigm4=cnfm4.tmp
gmp_tmpconfigm4i=cnfm4i.tmp
gmp_tmpconfigm4p=cnfm4p.tmp
rm -f $gmp_tmpconfigm4 $gmp_tmpconfigm4i $gmp_tmpconfigm4p

# CONFIG_TOP_SRCDIR is a path from the mpn builddir to the top srcdir.
# The pattern here tests for an absolute path the same way as
# _AC_OUTPUT_FILES in autoconf acgeneral.m4.
case $srcdir in
[[\\/]]* | ?:[[\\/]]* )  tmp="$srcdir"    ;;
*)                       tmp="../$srcdir" ;;
esac
echo ["define(<CONFIG_TOP_SRCDIR>,<\`$tmp'>)"] >>$gmp_tmpconfigm4

# All CPUs use asm-defs.m4 
echo ["include][(CONFIG_TOP_SRCDIR\`/mpn/asm-defs.m4')"] >>$gmp_tmpconfigm4i
])


dnl  GMP_FINISH
dnl  ----------
dnl  Create config.m4 from its accumulated parts.
dnl
dnl  __CONFIG_M4_INCLUDED__ is used so that a second or subsequent include
dnl  of config.m4 is harmless.
dnl
dnl  A separate ifdef on the angle bracket quoted part ensures the quoting
dnl  style there is respected.  The basic defines from gmp_tmpconfigm4 are
dnl  fully quoted but are still put under an ifdef in case any have been
dnl  redefined by one of the m4 include files.
dnl
dnl  Doing a big ifdef within asm-defs.m4 and/or other macro files wouldn't
dnl  work, since it'd interpret parentheses and quotes in dnl comments, and
dnl  having a whole file as a macro argument would overflow the string space
dnl  on BSD m4.

AC_DEFUN([GMP_FINISH],
[AC_REQUIRE([GMP_INIT])
echo "creating $gmp_configm4"
echo ["d""nl $gmp_configm4.  Generated automatically by configure."] > $gmp_configm4
if test -f $gmp_tmpconfigm4; then
  echo ["changequote(<,>)"] >> $gmp_configm4
  echo ["ifdef(<__CONFIG_M4_INCLUDED__>,,<"] >> $gmp_configm4
  cat $gmp_tmpconfigm4 >> $gmp_configm4
  echo [">)"] >> $gmp_configm4
  echo ["changequote(\`,')"] >> $gmp_configm4
  rm $gmp_tmpconfigm4
fi
echo ["ifdef(\`__CONFIG_M4_INCLUDED__',,\`"] >> $gmp_configm4
if test -f $gmp_tmpconfigm4i; then
  cat $gmp_tmpconfigm4i >> $gmp_configm4
  rm $gmp_tmpconfigm4i
fi
if test -f $gmp_tmpconfigm4p; then
  cat $gmp_tmpconfigm4p >> $gmp_configm4
  rm $gmp_tmpconfigm4p
fi
echo ["')"] >> $gmp_configm4
echo ["define(\`__CONFIG_M4_INCLUDED__')"] >> $gmp_configm4
])


dnl  GMP_INCLUDE_MPN(FILE)
dnl  ---------------------
dnl  Add an include_mpn(`FILE') to config.m4.  FILE should be a path
dnl  relative to the mpn source directory, for example
dnl
dnl      GMP_INCLUDE_MPN(`x86/x86-defs.m4')
dnl

AC_DEFUN([GMP_INCLUDE_MPN],
[AC_REQUIRE([GMP_INIT])
echo ["include_mpn(\`$1')"] >> $gmp_tmpconfigm4i
])


dnl  GMP_DEFINE(MACRO, DEFINITION [, LOCATION])
dnl  ------------------------------------------
dnl  Define M4 macro MACRO as DEFINITION in temporary file.
dnl
dnl  If LOCATION is `POST', the definition will appear after any include()
dnl  directives inserted by GMP_INCLUDE.  Mind the quoting!  No shell
dnl  variables will get expanded.  Don't forget to invoke GMP_FINISH to
dnl  create file config.m4.  config.m4 uses `<' and '>' as quote characters
dnl  for all defines.

AC_DEFUN([GMP_DEFINE], 
[AC_REQUIRE([GMP_INIT])
echo ['define(<$1>, <$2>)'] >>ifelse([$3], [POST],
                              $gmp_tmpconfigm4p, $gmp_tmpconfigm4)
])


dnl  GMP_DEFINE_RAW(STRING [, LOCATION])
dnl  ------------------------------------
dnl  Put STRING into config.m4 file.
dnl
dnl  If LOCATION is `POST', the definition will appear after any include()
dnl  directives inserted by GMP_INCLUDE.  Don't forget to invoke GMP_FINISH
dnl  to create file config.m4.

AC_DEFUN([GMP_DEFINE_RAW],
[AC_REQUIRE([GMP_INIT])
echo [$1] >> ifelse([$2], [POST], $gmp_tmpconfigm4p, $gmp_tmpconfigm4)
])


dnl  GMP_TRY_ASSEMBLE(asm-code,[action-success][,action-fail])
dnl  ----------------------------------------------------------
dnl  Attempt to assemble the given code.
dnl  Do "action-success" if this succeeds, "action-fail" if not.
dnl
dnl  conftest.o and conftest.out are available for inspection in
dnl  "action-success".  If either action does a "break" out of a loop then
dnl  an explicit "rm -f conftest*" will be necessary.
dnl
dnl  This is not unlike AC_TRY_COMPILE, but there's no default includes or
dnl  anything in "asm-code", everything wanted must be given explicitly.

AC_DEFUN([GMP_TRY_ASSEMBLE],
[cat >conftest.s <<EOF
[$1]
EOF
gmp_assemble="$CCAS $CFLAGS $CPPFLAGS conftest.s >conftest.out 2>&1"
if AC_TRY_EVAL(gmp_assemble); then
  cat conftest.out >&AC_FD_CC
  ifelse([$2],,:,[$2])
else
  cat conftest.out >&AC_FD_CC
  echo "configure: failed program was:" >&AC_FD_CC
  cat conftest.s >&AC_FD_CC
  ifelse([$3],,:,[$3])
fi
rm -f conftest*
])


dnl  GMP_ASM_LABEL_SUFFIX
dnl  --------------------
dnl  : - is usual.
dnl  empty - hppa on HP-UX doesn't use a :, just the label name
dnl
dnl  Note that it's necessary to test the empty case first, since HP "as"
dnl  will accept "somelabel:", and take it to mean a label with a name that
dnl  happens to end in a colon.

AC_DEFUN([GMP_ASM_LABEL_SUFFIX],
[AC_REQUIRE([GMP_ASM_TEXT])
AC_CACHE_CHECK([for assembler label suffix],
                gmp_cv_asm_label_suffix,
[gmp_cv_asm_label_suffix=unknown
for i in "" ":"; do
  echo "trying $i" >&AC_FD_CC
  GMP_TRY_ASSEMBLE(
[	$gmp_cv_asm_text
somelabel$i],
    [gmp_cv_asm_label_suffix=$i
     rm -f conftest*
     break],
    [cat conftest.out >&AC_FD_CC])
done
if test "$gmp_cv_asm_label_suffix" = "unknown"; then
  AC_MSG_ERROR([Cannot determine label suffix])
fi
])
echo ["define(<LABEL_SUFFIX>, <$gmp_cv_asm_label_suffix>)"] >> $gmp_tmpconfigm4
])


dnl  GMP_ASM_UNDERSCORE
dnl  ------------------
dnl  Determine whether global symbols need to be prefixed with an underscore.
dnl  The output from "nm" is grepped to see what a typical symbol looks like.
dnl
dnl  This test used to grep the .o file directly, but that failed with greps
dnl  that don't like binary files (eg. SunOS 4).
dnl
dnl  This test also used to construct an assembler file with and without an
dnl  underscore and try to link that to a C file, to see which worked.
dnl  Although that's what will happen in the real build we don't really want
dnl  to depend on creating asm files within configure for every possible CPU
dnl  (or at least we don't want to do that more than we have to).
dnl
dnl  The fallback on no underscore is based on the assumption that the world
dnl  is moving towards non-underscore systems.  There should actually be no
dnl  good reason for nm to fail though.

AC_DEFUN([GMP_ASM_UNDERSCORE],
[AC_REQUIRE([GMP_PROG_NM])
AC_CACHE_CHECK([if globals are prefixed by underscore], 
               gmp_cv_asm_underscore,
[gmp_cv_asm_underscore="unknown"
cat >conftest.c <<EOF
int gurkmacka;
EOF
gmp_compile="$CC $CFLAGS $CPPFLAGS -c conftest.c >&AC_FD_CC"
if AC_TRY_EVAL(gmp_compile); then
  $NM conftest.$OBJEXT >conftest.out
  if grep _gurkmacka conftest.out >/dev/null; then
    gmp_cv_asm_underscore=yes
  elif grep gurkmacka conftest.out >/dev/null; then
    gmp_cv_asm_underscore=no
  else
    echo "configure: $NM doesn't have gurkmacka:" >&AC_FD_CC
    cat conftest.out >&AC_FD_CC
  fi
else
  echo "configure: failed program was:" >&AC_FD_CC
  cat conftest.c >&AC_FD_CC
fi
rm -f conftest*
])
case $gmp_cv_asm_underscore in
  yes)
    GMP_DEFINE(GSYM_PREFIX, [_]) ;;
  no)
    GMP_DEFINE(GSYM_PREFIX, []) ;;
  *)
    AC_MSG_WARN([+----------------------------------------------------------])
    AC_MSG_WARN([| Cannot determine global symbol prefix.])
    AC_MSG_WARN([| $NM output doesn't contain a global data symbol.])
    AC_MSG_WARN([| Will proceed with no underscore.])
    AC_MSG_WARN([| If this is wrong then you'll get link errors referring])
    AC_MSG_WARN([| to ___gmpn_add_n (note three underscores).])
    AC_MSG_WARN([| In this case do a fresh build with an override,])
    AC_MSG_WARN([|     ./configure gmp_cv_asm_underscore=yes])
    AC_MSG_WARN([+----------------------------------------------------------])
    GMP_DEFINE(GSYM_PREFIX, [])
    ;;
esac
])


dnl  GMP_ASM_ALIGN_LOG
dnl  -----------------
dnl  Is parameter to `.align' logarithmic?

AC_DEFUN([GMP_ASM_ALIGN_LOG],
[AC_REQUIRE([GMP_ASM_GLOBL])
AC_REQUIRE([GMP_ASM_BYTE])
AC_REQUIRE([GMP_ASM_DATA])
AC_REQUIRE([GMP_ASM_LABEL_SUFFIX])
AC_REQUIRE([GMP_PROG_NM])
AC_CACHE_CHECK([if .align assembly directive is logarithmic],
               gmp_cv_asm_align_log,
[GMP_TRY_ASSEMBLE(
[      	$gmp_cv_asm_data
      	.align  4
	$gmp_cv_asm_globl	foo
	$gmp_cv_asm_byte	1
	.align	4
foo$gmp_cv_asm_label_suffix
	$gmp_cv_asm_byte	2],
  [gmp_tmp_val=[`$NM conftest.$OBJEXT | grep foo | \
     sed -e 's;[[][0-9][]]\(.*\);\1;' -e 's;[^1-9]*\([0-9]*\).*;\1;'`]
  if test "$gmp_tmp_val" = "10" || test "$gmp_tmp_val" = "16"; then
    gmp_cv_asm_align_log=yes
  else
    gmp_cv_asm_align_log=no
  fi],
  [AC_MSG_ERROR([cannot assemble alignment test])])])

GMP_DEFINE_RAW(["define(<ALIGN_LOGARITHMIC>,<$gmp_cv_asm_align_log>)"])
])


dnl  GMP_ASM_ALIGN_FILL_0x90
dnl  -----------------------
dnl  Determine whether a ",0x90" suffix works on a .align directive.
dnl  This is only meant for use on x86, 0x90 being a "nop".
dnl
dnl  Old gas, eg. 1.92.3
dnl       Needs ",0x90" or else the fill is 0x00, which can't be executed
dnl       across.
dnl
dnl  New gas, eg. 2.91
dnl       Generates multi-byte nop fills even when ",0x90" is given.
dnl
dnl  Solaris 2.6 as
dnl       ",0x90" is not allowed, causes a fatal error.
dnl
dnl  Solaris 2.8 as
dnl       ",0x90" does nothing, generates a warning that it's being ignored.
dnl
dnl  SCO OpenServer 5 as
dnl       Second parameter is max bytes to fill, not a fill pattern.
dnl       ",0x90" is an error due to being bigger than the first parameter.
dnl       Multi-byte nop fills are generated in text segments.
dnl
dnl  Note that both solaris "as"s only care about ",0x90" if they actually
dnl  have to use it to fill something, hence the .byte in the test.  It's
dnl  the second .align which provokes the error or warning.
dnl
dnl  The warning from solaris 2.8 is supressed to stop anyone worrying that
dnl  something might be wrong.

AC_DEFUN([GMP_ASM_ALIGN_FILL_0x90],
[AC_REQUIRE([GMP_ASM_TEXT])
AC_CACHE_CHECK([if the .align directive accepts an 0x90 fill in .text],
               gmp_cv_asm_align_fill_0x90,
[GMP_TRY_ASSEMBLE(
[      	$gmp_cv_asm_text
      	.align  4, 0x90
	.byte   0
      	.align  4, 0x90],
[if grep "Warning: Fill parameter ignored for executable section" conftest.out >/dev/null; then
  echo "Supressing this warning by omitting 0x90" 1>&AC_FD_CC
  gmp_cv_asm_align_fill_0x90=no
else
  gmp_cv_asm_align_fill_0x90=yes
fi],
[gmp_cv_asm_align_fill_0x90=no])])

GMP_DEFINE_RAW(["define(<ALIGN_FILL_0x90>,<$gmp_cv_asm_align_fill_0x90>)"])
])


dnl  GMP_ASM_BYTE
dnl  ------------
dnl  .byte - is usual.
dnl  data1 - required by ia64 (on hpux at least).
dnl
dnl  This macro is just to support other configure tests, not any actual asm
dnl  code.

AC_DEFUN([GMP_ASM_BYTE],
[AC_REQUIRE([GMP_ASM_TEXT])
AC_REQUIRE([GMP_ASM_LABEL_SUFFIX])
AC_CACHE_CHECK([for assembler byte directive],
                gmp_cv_asm_byte,
[for i in .byte data1; do
  echo "trying $i" >&AC_FD_CC
  GMP_TRY_ASSEMBLE(
[	$gmp_cv_asm_data
	$i	0
],
    [gmp_cv_asm_byte=$i
     rm -f conftest*
     break],
    [cat conftest.out >&AC_FD_CC])
done
if test -z "$gmp_cv_asm_byte"; then
  AC_MSG_ERROR([Cannot determine how to emit a data byte])
fi
])
])


dnl  GMP_ASM_TEXT
dnl  ------------
dnl  .text - is usual.
dnl  .code - is needed by the hppa on HP-UX (but ia64 HP-UX uses .text)
dnl  .csect .text[PR] - is for AIX.

AC_DEFUN([GMP_ASM_TEXT],
[AC_CACHE_CHECK([how to switch to text section],
                gmp_cv_asm_text,
[for i in ".text" ".code" [".csect .text[PR]"]; do
  echo "trying $i" >&AC_FD_CC
  GMP_TRY_ASSEMBLE([	$i],
    [gmp_cv_asm_text=$i
     rm -f conftest*
     break])
done
if test -z "$gmp_cv_asm_text"; then
  AC_MSG_ERROR([Cannot determine text section directive])
fi
])
echo ["define(<TEXT>, <$gmp_cv_asm_text>)"] >> $gmp_tmpconfigm4
])


dnl  GMP_ASM_DATA
dnl  ------------
dnl  Can we say `.data'?

AC_DEFUN([GMP_ASM_DATA],
[AC_CACHE_CHECK([how to switch to data section],
                gmp_cv_asm_data,
[case $host in
  *-*-aix*) gmp_cv_asm_data=[".csect .data[RW]"] ;;
  *)        gmp_cv_asm_data=".data" ;;
esac
])
echo ["define(<DATA>, <$gmp_cv_asm_data>)"] >> $gmp_tmpconfigm4
])


dnl  GMP_ASM_RODATA
dnl  --------------
dnl  Find out how to switch to the read-only data section.
dnl
dnl  The compiler output is grepped for the right directive.  It's not
dnl  considered wise to just probe for ".section .rodata" or whatever works,
dnl  since arbitrary section names might be accepted, but not necessarily do
dnl  the right thing when they get to the linker.
dnl
dnl  Only a few asm files use RODATA, so this code is perhaps a bit
dnl  excessive right now, but should find more uses in the future.
dnl
dnl  FIXME: gcc on aix generates something like ".csect _foo.ro_c[RO],3"
dnl  where foo is the object file.  Might need to check for that if we use
dnl  RODATA there.

AC_DEFUN([GMP_ASM_RODATA],
[AC_REQUIRE([GMP_ASM_TEXT])
AC_REQUIRE([GMP_ASM_DATA])
AC_REQUIRE([GMP_ASM_LABEL_SUFFIX])
AC_REQUIRE([GMP_ASM_UNDERSCORE])
AC_CACHE_CHECK([how to switch to read-only data section],
               gmp_cv_asm_rodata,
[
dnl Default to DATA on CPUs with split code/data caching, and TEXT
dnl elsewhere.  i386 means generic x86, so use DATA on it.
case $host in
X86_PATTERN | x86_64-*-*)
  gmp_cv_asm_rodata="$gmp_cv_asm_data" ;;
*)
  gmp_cv_asm_rodata="$gmp_cv_asm_text" ;;
esac

cat >conftest.c <<EOF
const int foo = 123;
EOF
echo "Test program:" >&AC_FD_CC
cat conftest.c >&AC_FD_CC
gmp_compile="$CC $CFLAGS $CPPFLAGS -S conftest.c >&AC_FD_CC"
if AC_TRY_EVAL(gmp_compile); then
  echo "Compiler output:" >&AC_FD_CC
  cat conftest.s >&AC_FD_CC
  if test $gmp_cv_asm_underscore = yes; then
    tmp_gsym_prefix=_
  else
    tmp_gsym_prefix=
  fi
  # must see our label
  if grep "^${tmp_gsym_prefix}foo$gmp_cv_asm_label_suffix" conftest.s >/dev/null 2>&AC_FD_CC; then
    # take the last directive before our label (hence skipping segments
    # getting debugging info etc)
    tmp_match=`sed -n ["/^${tmp_gsym_prefix}foo$gmp_cv_asm_label_suffix/q
                        /^[. 	]*data/p
                        /^[. 	]*rdata/p
                        /^[. 	]*text/p
                        /^[. 	]*section/p
                        /^[. 	]*csect/p
                        /^[. 	]*CSECT/p"] conftest.s | sed -n '$p'`
    echo "Match: $tmp_match" >&AC_FD_CC
    if test -n "$tmp_match"; then
      gmp_cv_asm_rodata=$tmp_match
    fi
  else
    echo "Couldn't find label: ^${tmp_gsym_prefix}foo$gmp_cv_asm_label_suffix" >&AC_FD_CC
  fi
fi
rm -f conftest*
])
echo ["define(<RODATA>, <$gmp_cv_asm_rodata>)"] >> $gmp_tmpconfigm4
])


dnl  GMP_ASM_GLOBL
dnl  -------------
dnl  The assembler directive to mark a label as a global symbol.
dnl
dnl  ia64 - .global is standard, according to the Intel documentation.
dnl
dnl  hppa - ".export foo,entry" is demanded by HP hppa "as".  ".global" is a
dnl      kind of import.
dnl
dnl  other - .globl is usual.
dnl
dnl  "gas" tends to accept .globl everywhere, in addition to .export or
dnl  .global or whatever the system assembler demands.  

AC_DEFUN([GMP_ASM_GLOBL],
[AC_REQUIRE([GMP_ASM_TEXT])
AC_CACHE_CHECK([for assembler global directive],
                gmp_cv_asm_globl,
[case $host in
  hppa*-*-*)     gmp_cv_asm_globl=.export ;;
  IA64_PATTERN)  gmp_cv_asm_globl=.global ;;
  *)             gmp_cv_asm_globl=.globl  ;;
esac
])
echo ["define(<GLOBL>, <$gmp_cv_asm_globl>)"] >> $gmp_tmpconfigm4
])


dnl  GMP_ASM_GLOBL_ATTR
dnl  ------------------
dnl  Do we need something after `GLOBL symbol'?

AC_DEFUN([GMP_ASM_GLOBL_ATTR],
[AC_REQUIRE([GMP_ASM_GLOBL])
AC_CACHE_CHECK([for assembler global directive attribute],
                gmp_cv_asm_globl_attr,
[case $gmp_cv_asm_globl in
  .export) gmp_cv_asm_globl_attr=",entry" ;;
  *)       gmp_cv_asm_globl_attr="" ;;
esac
])
echo ["define(<GLOBL_ATTR>, <$gmp_cv_asm_globl_attr>)"] >> $gmp_tmpconfigm4
])


dnl  GMP_ASM_TYPE
dnl  ------------
dnl  Can we say ".type", and how?
dnl
dnl  For i386 GNU/Linux ELF systems, and very likely other ELF systems,
dnl  .type and .size are important on functions in shared libraries.  If
dnl  .type is omitted and the mainline program references that function then
dnl  the code will be copied down to the mainline at load time like a piece
dnl  of data.  If .size is wrong or missing (it defaults to 4 bytes or some
dnl  such) then incorrect bytes will be copied and a segv is the most likely
dnl  result.  In any case such copying is not what's wanted, a .type
dnl  directive will ensure a PLT entry is used.
dnl
dnl  In GMP the assembler functions are normally only used from within the
dnl  library (since most programs are not interested in the low level
dnl  routines), and in those circumstances a missing .type isn't fatal,
dnl  letting the problem go unnoticed.  tests/mpn/t-asmtype.c aims to check
dnl  for it.

AC_DEFUN([GMP_ASM_TYPE],
[AC_CACHE_CHECK([for assembler .type directive],
                gmp_cv_asm_type,
[gmp_cv_asm_type=
for gmp_tmp_prefix in @ \# %; do
  GMP_TRY_ASSEMBLE([	.type	sym,${gmp_tmp_prefix}function],
    [if grep "\.type pseudo-op used outside of \.def/\.endef ignored" conftest.out >/dev/null; then : ;
    else
      gmp_cv_asm_type=".type	\$][1,${gmp_tmp_prefix}\$][2"
      break
    fi])
done
rm -f conftest*
])
echo ["define(<TYPE>, <$gmp_cv_asm_type>)"] >> $gmp_tmpconfigm4
])


dnl  GMP_ASM_SIZE
dnl  ------------
dnl  Can we say `.size'?

AC_DEFUN([GMP_ASM_SIZE],
[AC_CACHE_CHECK([for assembler .size directive],
                gmp_cv_asm_size,
[gmp_cv_asm_size=
GMP_TRY_ASSEMBLE([	.size	sym,1],
  [if grep "\.size pseudo-op used outside of \.def/\.endef ignored" conftest.out >/dev/null; then : ;
  else
    gmp_cv_asm_size=".size	\$][1,\$][2"
  fi])
])
echo ["define(<SIZE>, <$gmp_cv_asm_size>)"] >> $gmp_tmpconfigm4
])


dnl  GMP_ASM_COFF_TYPE
dnl  -----------------
dnl  Determine whether the assembler supports COFF type information.
dnl
dnl  Currently this is only needed for mingw (and cygwin perhaps) and so is
dnl  run only on the x86s, but it ought to work anywhere.
dnl
dnl  On MINGW, recent versions of the linker have an automatic import scheme
dnl  for data in a DLL which is referenced by a mainline but without
dnl  __declspec (__dllimport__) on the prototype.  It seems functions
dnl  without type information are treated as data, or something, and calls
dnl  to them from the mainline will crash.  gcc puts type information on the
dnl  C functions it generates, we need to do the same for assembler
dnl  functions.
dnl
dnl  This applies only to functions without __declspec(__dllimport__),
dnl  ie. without __GMP_DECLSPEC in the case of libgmp, so it also works just
dnl  to ensure all assembler functions used from outside libgmp have
dnl  __GMP_DECLSPEC on their prototypes.  But this isn't an ideal situation,
dnl  since we don't want perfectly valid calls going wrong just because
dnl  there wasn't a prototype in scope.
dnl
dnl  When an auto-import takes place, the following warning is given by the
dnl  linker.  This shouldn't be seen for any functions.
dnl
dnl      Info: resolving _foo by linking to __imp__foo (auto-import)
dnl
dnl
dnl  COFF type directives look like the following
dnl
dnl      .def    _foo
dnl      .scl    2
dnl      .type   32
dnl      .endef
dnl
dnl  _foo is the symbol with GSYM_PREFIX (_).  .scl is the storage class, 2
dnl  for external, 3 for static.  .type is the object type, 32 for a
dnl  function.
dnl
dnl  On an ELF system, this is (correctly) rejected due to .def, .endef and
dnl  .scl being invalid, and .type not having enough arguments.

AC_DEFUN([GMP_ASM_COFF_TYPE],
[AC_REQUIRE([GMP_ASM_TEXT])
AC_REQUIRE([GMP_ASM_GLOBL])
AC_REQUIRE([GMP_ASM_GLOBL_ATTR])
AC_REQUIRE([GMP_ASM_LABEL_SUFFIX])
AC_REQUIRE([GMP_ASM_UNDERSCORE])
AC_CACHE_CHECK([for assembler COFF type directives],
		gmp_cv_asm_x86_coff_type,
[GMP_TRY_ASSEMBLE(
[	$gmp_cv_asm_text
	$gmp_cv_asm_globl ${tmp_gsym_prefix}foo$gmp_cv_asm_globl_attr
	.def	${tmp_gsym_prefix}foo
	.scl	2
	.type	32
	.endef
${tmp_gsym_prefix}foo$gmp_cv_asm_label_suffix
],
  [gmp_cv_asm_x86_coff_type=yes],
  [gmp_cv_asm_x86_coff_type=no])
])
echo ["define(<HAVE_COFF_TYPE>, <$gmp_cv_asm_x86_coff_type>)"] >> $gmp_tmpconfigm4
])


dnl  GMP_ASM_LSYM_PREFIX
dnl  -------------------
dnl  What is the prefix for a local label?
dnl
dnl  The prefixes tested are,
dnl
dnl      L  - usual for underscore systems
dnl      .L - usual for non-underscore systems
dnl      $  - alpha (gas and OSF system assembler)
dnl      L$ - hppa (gas and HP-UX system assembler)
dnl
dnl  The default is "L" if the tests fail for any reason.  There's a good
dnl  chance this will be adequate, since on most systems labels are local
dnl  anyway unless given a ".globl", and an "L" will avoid clashes with
dnl  other identifers.
dnl
dnl  For gas, ".L" is normally purely local to the assembler, it doesn't get
dnl  put into the object file at all.  This style is preferred, to keep the
dnl  object files nice and clean.
dnl
dnl  BSD format nm produces a line like
dnl
dnl      00000000 t Lgurkmacka
dnl
dnl  The symbol code is normally "t" for text, but any lower case letter
dnl  indicates a local definition.
dnl
dnl  Code "n" is for a debugging symbol, OSF "nm -B" gives that as an upper
dnl  case "N" for a local.
dnl
dnl  HP-UX nm prints an error message (though seems to give a 0 exit) if
dnl  there's no symbols at all in an object file, hence the use of "dummy".

AC_DEFUN([GMP_ASM_LSYM_PREFIX],
[AC_REQUIRE([GMP_ASM_LABEL_SUFFIX])
AC_REQUIRE([GMP_ASM_TEXT])
AC_REQUIRE([GMP_PROG_NM])
AC_CACHE_CHECK([for assembler local label prefix], 
               gmp_cv_asm_lsym_prefix,
[gmp_tmp_pre_appears=yes
for gmp_tmp_pre in L .L $ L$; do
  echo "Trying $gmp_tmp_pre" >&AC_FD_CC
  GMP_TRY_ASSEMBLE(
[	$gmp_cv_asm_text
dummy${gmp_cv_asm_label_suffix}
${gmp_tmp_pre}gurkmacka${gmp_cv_asm_label_suffix}],
  [if $NM conftest.$OBJEXT >conftest.nm 2>&AC_FD_CC; then : ; else
    cat conftest.nm >&AC_FD_CC
    AC_MSG_WARN(["$NM" failure])
    break
  fi
  cat conftest.nm >&AC_FD_CC
  if grep gurkmacka conftest.nm >/dev/null; then : ; else
    # no mention of the symbol, this is good
    echo "$gmp_tmp_pre label doesn't appear in object file at all (good)" >&AC_FD_CC
    gmp_cv_asm_lsym_prefix="$gmp_tmp_pre"
    gmp_tmp_pre_appears=no
    break
  fi
  if grep [' [a-zN] .*gurkmacka'] conftest.nm >/dev/null; then
    # symbol mentioned as a local, use this if nothing better
    echo "$gmp_tmp_pre label is local but still in object file" >&AC_FD_CC
    if test -z "$gmp_cv_asm_lsym_prefix"; then
      gmp_cv_asm_lsym_prefix="$gmp_tmp_pre"
    fi
  else
    echo "$gmp_tmp_pre label is something unknown" >&AC_FD_CC
  fi
  ])
done
rm -f conftest*
if test -z "$gmp_cv_asm_lsym_prefix"; then
  gmp_cv_asm_lsym_prefix=L
  AC_MSG_WARN([cannot determine local label, using default $gmp_cv_asm_lsym_prefix])
fi
# for development purposes, note whether we got a purely temporary local label
echo "Local label appears in object files: $gmp_tmp_pre_appears" >&AC_FD_CC
])
echo ["define(<LSYM_PREFIX>, <${gmp_cv_asm_lsym_prefix}>)"] >> $gmp_tmpconfigm4
AC_DEFINE_UNQUOTED(LSYM_PREFIX, "$gmp_cv_asm_lsym_prefix",
                   [Assembler local label prefix])
])


dnl  GMP_ASM_W32
dnl  -----------
dnl  How to define a 32-bit word.
dnl
dnl  FIXME: This test is not right for ia64-*-hpux*.  The directive should
dnl  be "data4", but the W32 macro is not currently used by the mpn/ia64 asm
dnl  files.

AC_DEFUN([GMP_ASM_W32],
[AC_REQUIRE([GMP_ASM_DATA])
AC_REQUIRE([GMP_ASM_BYTE])
AC_REQUIRE([GMP_ASM_GLOBL])
AC_REQUIRE([GMP_ASM_LABEL_SUFFIX])
AC_REQUIRE([GMP_PROG_NM])
AC_CACHE_CHECK([how to define a 32-bit word],
	       gmp_cv_asm_w32,
[case $host in 
  *-*-hpux*)
    # FIXME: HPUX puts first symbol at 0x40000000, breaking our assumption
    # that it's at 0x0.  We'll have to declare another symbol before the
    # .long/.word and look at the distance between the two symbols.  The
    # only problem is that the sed expression(s) barfs (on Solaris, for
    # example) for the symbol with value 0.  For now, HPUX uses .word.
    gmp_cv_asm_w32=".word"
    ;;
  *-*-*)
    gmp_tmp_val=
    for gmp_tmp_op in .long .word data4; do
      GMP_TRY_ASSEMBLE(
[	$gmp_cv_asm_data
	$gmp_cv_asm_globl	foo
	$gmp_tmp_op	0
foo$gmp_cv_asm_label_suffix
	$gmp_cv_asm_byte	0],
        [gmp_tmp_val=[`$NM conftest.$OBJEXT | grep foo | \
          sed -e 's;[[][0-9][]]\(.*\);\1;' -e 's;[^1-9]*\([0-9]*\).*;\1;'`]
        if test "$gmp_tmp_val" = 4; then
          gmp_cv_asm_w32="$gmp_tmp_op"
          break
        fi])
    done
    rm -f conftest*
    ;;
esac
if test -z "$gmp_cv_asm_w32"; then
  AC_MSG_ERROR([cannot determine how to define a 32-bit word])
fi
])
echo ["define(<W32>, <$gmp_cv_asm_w32>)"] >> $gmp_tmpconfigm4
])


dnl  GMP_X86_ASM_GOT_UNDERSCORE
dnl  --------------------------
dnl  Determine whether i386 _GLOBAL_OFFSET_TABLE_ needs an additional
dnl  underscore prefix.
dnl
dnl    SVR4      - the standard is _GLOBAL_OFFSET_TABLE_
dnl    GNU/Linux - follows SVR4
dnl    OpenBSD   - an a.out underscore system, uses __GLOBAL_OFFSET_TABLE_
dnl    NetBSD    - also an a.out underscore system, but _GLOBAL_OFFSET_TABLE_
dnl
dnl  The test attempts to link a program using _GLOBAL_OFFSET_TABLE_ or
dnl  __GLOBAL_OFFSET_TABLE_ to see which works.
dnl
dnl  $lt_prog_compiler_pic is included in the compile because old versions
dnl  of gas wouldn't accept PIC idioms without the right option (-K).  This
dnl  is the same as what libtool and mpn/Makeasm.am will do.
dnl
dnl  $lt_prog_compiler_pic is also included in the link because OpenBSD ld
dnl  won't accept an R_386_GOTPC relocation without the right options.  This
dnl  is not what's done by the Makefiles when building executables, but
dnl  let's hope it's ok (it works fine with gcc).
dnl
dnl  The fallback is no additional underscore, on the basis that this will
dnl  suit SVR4/ELF style systems, which should be much more common than
dnl  a.out systems with shared libraries.
dnl
dnl  Note that it's not an error for the tests to fail, since for instance
dnl  cygwin, mingw and djgpp don't have a _GLOBAL_OFFSET_TABLE_ scheme at
dnl  all.
dnl
dnl  Perhaps $CCAS could be asked to do the linking as well as the
dnl  assembling, but in the Makefiles it's only used for assembling, so lets
dnl  keep it that way.
dnl
dnl  The test here is run even under --disable-shared, so that PIC objects
dnl  can be built and tested by the tune/many.pl development scheme.  The
dnl  tests will be reasonably quick and won't give a fatal error, so this
dnl  arrangement is ok.  AC_LIBTOOL_PROG_COMPILER_PIC does its
dnl  $lt_prog_compiler_pic setups even for --disable-shared too.

AC_DEFUN([GMP_ASM_X86_GOT_UNDERSCORE],
[AC_REQUIRE([GMP_ASM_TEXT])
AC_REQUIRE([GMP_ASM_GLOBL])
AC_REQUIRE([GMP_ASM_GLOBL_ATTR])
AC_REQUIRE([GMP_ASM_LABEL_SUFFIX])
AC_REQUIRE([GMP_ASM_UNDERSCORE])
AC_REQUIRE([AC_LIBTOOL_PROG_COMPILER_PIC])
AC_CACHE_CHECK([if _GLOBAL_OFFSET_TABLE_ is prefixed by underscore], 
               gmp_cv_asm_x86_got_underscore,
[gmp_cv_asm_x86_got_underscore="not applicable"
if test $gmp_cv_asm_underscore = yes; then
  tmp_gsym_prefix=_
else
  tmp_gsym_prefix=
fi
for tmp_underscore in "" "_"; do
  cat >conftest.s <<EOF
	$gmp_cv_asm_text
	$gmp_cv_asm_globl ${tmp_gsym_prefix}main$gmp_cv_asm_globl_attr
${tmp_gsym_prefix}main$gmp_cv_asm_label_suffix
	addl	$ ${tmp_underscore}_GLOBAL_OFFSET_TABLE_, %ebx
EOF
  gmp_compile="$CCAS $CFLAGS $CPPFLAGS $lt_prog_compiler_pic conftest.s >&AC_FD_CC && $CC $CFLAGS $CPPFLAGS $lt_prog_compiler_pic conftest.$OBJEXT >&AC_FD_CC"
  if AC_TRY_EVAL(gmp_compile); then
    if test "$tmp_underscore" = "_"; then
      gmp_cv_asm_x86_got_underscore=yes
    else
      gmp_cv_asm_x86_got_underscore=no
    fi
    break
  fi
done
rm -f conftest* a.out b.out a.exe a_out.exe
])
if test "$gmp_cv_asm_x86_got_underscore" = "yes"; then
  GMP_DEFINE(GOT_GSYM_PREFIX, [_])
else
  GMP_DEFINE(GOT_GSYM_PREFIX, [])
fi    
])


dnl  GMP_ASM_X86_GOT_EAX_OK(CC+CFLAGS, [ACTION-YES] [, ACTION-NO])
dnl  -------------------------------------------------------------
dnl  Determine whether _GLOBAL_OFFSET_TABLE_ used with %eax is ok.
dnl
dnl  An instruction
dnl
dnl          addl  $_GLOBAL_OFFSET_TABLE_, %eax
dnl
dnl  is incorrectly assembled by gas 2.12 (or thereabouts) and earlier.  It
dnl  puts an addend 2 into the R_386_GOTPC relocation, but it should be 1
dnl  for this %eax form being a 1 byte opcode (with other registers it's 2
dnl  opcode bytes).  See note about this in mpn/x86/README too.
dnl
dnl  We assemble this, surrounded by some unlikely byte sequences as
dnl  delimiters, and check for the bad output.
dnl
dnl  This is for use by compiler probing in GMP_PROG_CC_WORKS, so the result
dnl  is not cached.
dnl
dnl  This test is not specific to gas, but old gas is the only assembler we
dnl  know of with this problem.  The Solaris has been seen coming out ok.
dnl
dnl  ".text" is hard coded because this macro is wanted before GMP_ASM_TEXT.
dnl  This should be fine, ".text" is normal on x86 systems, and certainly
dnl  will be fine with the offending gas.
dnl
dnl  If an error occurs when assembling, we consider the assembler ok, since
dnl  the bad output does not occur.  This happens for instance on mingw,
dnl  where _GLOBAL_OFFSET_TABLE_ results in a bfd error, since there's no
dnl  GOT etc in PE object files.
dnl
dnl  This test is used before the object file extension has been determined,
dnl  so we force output to conftest.o.  Using -o with -c is not portable,
dnl  but we think all x86 compilers will accept -o with -c, certainly gcc
dnl  does.
dnl
dnl  -fPIC is hard coded here, because this test is for use before libtool
dnl  has established the pic options.  It's right for gcc, but perhaps not
dnl  other compilers.

AC_DEFUN([GMP_ASM_X86_GOT_EAX_OK],
[echo "Testing gas GOT with eax good" >&AC_FD_CC
cat >conftest.awk <<\EOF
[BEGIN {
  want[0]  = "001"
  want[1]  = "043"
  want[2]  = "105"
  want[3]  = "147"
  want[4]  = "211"
  want[5]  = "253"
  want[6]  = "315"
  want[7]  = "357"

  want[8]  = "005"
  want[9]  = "002"
  want[10] = "000"
  want[11] = "000"
  want[12] = "000"

  want[13] = "376"
  want[14] = "334"
  want[15] = "272"
  want[16] = "230"
  want[17] = "166"
  want[18] = "124"
  want[19] = "062"
  want[20] = "020"

  result = "yes"
}
{
  for (f = 2; f <= NF; f++)
    {
      for (i = 0; i < 20; i++)
        got[i] = got[i+1];
      got[20] = $f;

      found = 1
      for (i = 0; i < 21; i++)
        if (got[i] != want[i])
          {
            found = 0
            break
          }
      if (found)
        {
          result = "no"
          exit
        }
    }
}
END {
  print result
}
]EOF
cat >conftest.s <<\EOF
[	.text
	.byte	1, 35, 69, 103, 137, 171, 205, 239
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	.byte	254, 220, 186, 152, 118, 84, 50, 16
]EOF
tmp_got_good=yes
gmp_compile="$1 -fPIC -o conftest.o -c conftest.s >&AC_FD_CC 2>&1"
if AC_TRY_EVAL(gmp_compile); then
  tmp_got_good=`od -b conftest.o | $AWK -f conftest.awk`
fi
rm -f conftest.*
echo "Result: $tmp_got_good" >&AC_FD_CC
if test "$tmp_got_good" = no; then
  ifelse([$3],,:,[$3])
else
  ifelse([$2],,:,[$2])
fi
])


dnl  GMP_ASM_X86_MMX([ACTION-IF-YES][,ACTION-IF-NO])
dnl  -----------------------------------------------
dnl  Determine whether the assembler supports MMX instructions.
dnl
dnl  This macro is wanted before GMP_ASM_TEXT, so ".text" is hard coded
dnl  here.  ".text" is believed to be correct on all x86 systems.  Actually
dnl  ".text" probably isn't needed at all, at least for just checking
dnl  instruction syntax.
dnl
dnl  "movq %mm0, %mm1" should assemble to "0f 6f c8", but Solaris 2.6 and
dnl  2.7 wrongly assemble it to "0f 6f c1" (that being the reverse "movq
dnl  %mm1, %mm0").  It seems more trouble than it's worth to work around
dnl  this in the code, so just detect and reject.

AC_DEFUN([GMP_ASM_X86_MMX],
[AC_CACHE_CHECK([if the assembler knows about MMX instructions],
		gmp_cv_asm_x86_mmx,
[GMP_TRY_ASSEMBLE(
[	.text
	movq	%mm0, %mm1],
[gmp_cv_asm_x86_mmx=yes
case $host in
*-*-solaris*)
  if (dis conftest.$OBJEXT >conftest.out) 2>/dev/null; then
    if grep "0f 6f c1" conftest.out >/dev/null; then
      gmp_cv_asm_x86_mmx=movq-bug
    fi
  else
    AC_MSG_WARN(["dis" not available to check for "as" movq bug])
  fi
esac],
[gmp_cv_asm_x86_mmx=no])])

case $gmp_cv_asm_x86_mmx in
movq-bug)
  AC_MSG_WARN([+----------------------------------------------------------])
  AC_MSG_WARN([| WARNING WARNING WARNING])
  AC_MSG_WARN([| Host CPU has MMX code, but the assembler])
  AC_MSG_WARN([|     $CCAS $CFLAGS $CPPFLAGS])
  AC_MSG_WARN([| has the Solaris 2.6 and 2.7 bug where register to register])
  AC_MSG_WARN([| movq operands are reversed.])
  AC_MSG_WARN([| Non-MMX replacements will be used.])
  AC_MSG_WARN([| This will be an inferior build.])
  AC_MSG_WARN([+----------------------------------------------------------])
  ;;
no)
  AC_MSG_WARN([+----------------------------------------------------------])
  AC_MSG_WARN([| WARNING WARNING WARNING])
  AC_MSG_WARN([| Host CPU has MMX code, but it can't be assembled by])
  AC_MSG_WARN([|     $CCAS $CFLAGS $CPPFLAGS])
  AC_MSG_WARN([| Non-MMX replacements will be used.])
  AC_MSG_WARN([| This will be an inferior build.])
  AC_MSG_WARN([+----------------------------------------------------------])
  ;;
esac
if test "$gmp_cv_asm_x86_mmx" = yes; then
  ifelse([$1],,:,[$1])
else
  ifelse([$2],,:,[$2])
fi
])


dnl  GMP_ASM_X86_SHLDL_CL
dnl  --------------------

AC_DEFUN([GMP_ASM_X86_SHLDL_CL],
[AC_REQUIRE([GMP_ASM_TEXT])
AC_CACHE_CHECK([if the assembler takes cl with shldl],
		gmp_cv_asm_x86_shldl_cl,
[GMP_TRY_ASSEMBLE(
[	$gmp_cv_asm_text
	shldl	%cl, %eax, %ebx],
  gmp_cv_asm_x86_shldl_cl=yes,
  gmp_cv_asm_x86_shldl_cl=no)
])
if test "$gmp_cv_asm_x86_shldl_cl" = "yes"; then
  GMP_DEFINE(WANT_SHLDL_CL,1)
else
  GMP_DEFINE(WANT_SHLDL_CL,0)
fi
])


dnl  GMP_ASM_X86_SSE2([ACTION-IF-YES][,ACTION-IF-NO])
dnl  ------------------------------------------------
dnl  Determine whether the assembler supports SSE2 instructions.
dnl
dnl  This macro is wanted before GMP_ASM_TEXT, so ".text" is hard coded
dnl  here.  ".text" is believed to be correct on all x86 systems, certainly
dnl  it's all GMP_ASM_TEXT gives currently.  Actually ".text" probably isn't
dnl  needed at all, at least for just checking instruction syntax.

AC_DEFUN([GMP_ASM_X86_SSE2],
[AC_CACHE_CHECK([if the assembler knows about SSE2 instructions],
		gmp_cv_asm_x86_sse2,
[GMP_TRY_ASSEMBLE(
[	.text
	paddq	%mm0, %mm1],
  [gmp_cv_asm_x86_sse2=yes],
  [gmp_cv_asm_x86_sse2=no])
])
case $gmp_cv_asm_x86_sse2 in
yes)
  ifelse([$1],,:,[$1])
  ;;
*)
  AC_MSG_WARN([+----------------------------------------------------------])
  AC_MSG_WARN([| WARNING WARNING WARNING])
  AC_MSG_WARN([| Host CPU has SSE2 code, but it can't be assembled by])
  AC_MSG_WARN([|     $CCAS $CFLAGS $CPPFLAGS])
  AC_MSG_WARN([| Non-SSE2 replacements will be used.])
  AC_MSG_WARN([| This will be an inferior build.])
  AC_MSG_WARN([+----------------------------------------------------------])
  ifelse([$2],,:,[$2])
  ;;
esac
])


dnl  GMP_ASM_X86_MCOUNT
dnl  ------------------
dnl  Find out how to call mcount for profiling on an x86 system.
dnl
dnl  A dummy function is compiled and the ".s" output examined.  The pattern
dnl  matching might be a bit fragile, but should work at least with gcc on
dnl  sensible systems.  Certainly it's better than hard coding a table of
dnl  conventions.
dnl
dnl  For non-PIC, any ".data" is taken to mean a counter might be passed.
dnl  It's assumed a movl will set it up, and the right register is taken
dnl  from that movl.  Any movl involving %esp is ignored (a frame pointer
dnl  setup normally).
dnl
dnl  For PIC, any ".data" is similarly interpreted, but a GOTOFF identifies
dnl  the line setting up the right register.
dnl
dnl  In both cases a line with "mcount" identifies the call and that line is
dnl  used literally.
dnl
dnl  On some systems (eg. FreeBSD 3.5) gcc emits ".data" but doesn't use it,
dnl  so it's not an error to have .data but then not find a register.
dnl
dnl  Variations in mcount conventions on different x86 systems can be found
dnl  in gcc config/i386.  mcount can have a "_" prefix or be .mcount or
dnl  _mcount_ptr, and for PIC it can be called through a GOT entry, or via
dnl  the PLT.  If a pointer to a counter is required it's passed in %eax or
dnl  %edx.
dnl
dnl  Flags to specify PIC are taken from $lt_prog_compiler_pic set by
dnl  AC_PROG_LIBTOOL.
dnl
dnl  Enhancement: Cache the values determined here. But what's the right way
dnl  to get two variables (mcount_nonpic_reg and mcount_nonpic_call say) set
dnl  from one block of commands?

AC_DEFUN([GMP_ASM_X86_MCOUNT],
[AC_REQUIRE([AC_ENABLE_SHARED])
AC_REQUIRE([AC_PROG_LIBTOOL])
AC_MSG_CHECKING([how to call x86 mcount])
cat >conftest.c <<EOF
foo(){bar();}
EOF

if test "$enable_static" = yes; then
  gmp_asmout_compile="$CC $CFLAGS $CPPFLAGS -S conftest.c 1>&AC_FD_CC"
  if AC_TRY_EVAL(gmp_asmout_compile); then
    if grep '\.data' conftest.s >/dev/null; then
      mcount_nonpic_reg=`sed -n ['/esp/!s/.*movl.*,\(%[a-z]*\).*$/\1/p'] conftest.s`
    else
      mcount_nonpic_reg=
    fi
    mcount_nonpic_call=`grep 'call.*mcount' conftest.s`
    if test -z "$mcount_nonpic_call"; then
      AC_MSG_ERROR([Cannot find mcount call for non-PIC])
    fi
  else
    AC_MSG_ERROR([Cannot compile test program for non-PIC])
  fi
fi

if test "$enable_shared" = yes; then
  gmp_asmout_compile="$CC $CFLAGS $CPPFLAGS $lt_prog_compiler_pic -S conftest.c 1>&AC_FD_CC"
  if AC_TRY_EVAL(gmp_asmout_compile); then
    if grep '\.data' conftest.s >/dev/null; then
      case $lt_prog_compiler_pic in
        *-DDLL_EXPORT*)
          # Windows DLLs have non-PIC style mcount
          mcount_pic_reg=`sed -n ['/esp/!s/.*movl.*,\(%[a-z]*\).*$/\1/p'] conftest.s`
          ;;
        *)
          mcount_pic_reg=`sed -n ['s/.*GOTOFF.*,\(%[a-z]*\).*$/\1/p'] conftest.s`
          ;;
      esac
    else
      mcount_pic_reg=
    fi
    mcount_pic_call=`grep 'call.*mcount' conftest.s`
    if test -z "$mcount_pic_call"; then
      AC_MSG_ERROR([Cannot find mcount call for PIC])
    fi
  else
    AC_MSG_ERROR([Cannot compile test program for PIC])
  fi
fi

GMP_DEFINE_RAW(["define(<MCOUNT_NONPIC_REG>, <\`$mcount_nonpic_reg'>)"])
GMP_DEFINE_RAW(["define(<MCOUNT_NONPIC_CALL>,<\`$mcount_nonpic_call'>)"])
GMP_DEFINE_RAW(["define(<MCOUNT_PIC_REG>,    <\`$mcount_pic_reg'>)"])
GMP_DEFINE_RAW(["define(<MCOUNT_PIC_CALL>,   <\`$mcount_pic_call'>)"])

rm -f conftest.*
AC_MSG_RESULT([determined])
])


dnl  GMP_ASM_IA64_ALIGN_OK
dnl  ---------------------
dnl  Determine whether .align correctly pads with nop instructions in a text
dnl  segment.
dnl
dnl  gas 2.14 and earlier byte swaps its padding bundle on big endian
dnl  systems, which is incorrect (endianness only changes data).  What
dnl  should be "nop.m / nop.f / nop.i" comes out as "break" instructions.
dnl
dnl  The test here detects the bad case, and assumes anything else is ok
dnl  (there are many sensible nop bundles, so it'd be impractical to try to
dnl  match everything good).

AC_DEFUN([GMP_ASM_IA64_ALIGN_OK],
[AC_CACHE_CHECK([whether assembler .align padding is good],
		gmp_cv_asm_ia64_align_ok,
[cat >conftest.awk <<\EOF
[BEGIN {
  want[0]  = "011"
  want[1]  = "160"
  want[2]  = "074"
  want[3]  = "040"
  want[4]  = "000"
  want[5]  = "040"
  want[6]  = "020"
  want[7]  = "221"
  want[8]  = "114"
  want[9]  = "000"
  want[10] = "100"
  want[11] = "200"
  want[12] = "122"
  want[13] = "261"
  want[14] = "000"
  want[15] = "200"

  want[16] = "000"
  want[17] = "004"
  want[18] = "000"
  want[19] = "000"
  want[20] = "000"
  want[21] = "000"
  want[22] = "002"
  want[23] = "000"
  want[24] = "000"
  want[25] = "000"
  want[26] = "000"
  want[27] = "001"
  want[28] = "000"
  want[29] = "000"
  want[30] = "000"
  want[31] = "014"

  want[32] = "011"
  want[33] = "270"
  want[34] = "140"
  want[35] = "062"
  want[36] = "000"
  want[37] = "040"
  want[38] = "240"
  want[39] = "331"
  want[40] = "160"
  want[41] = "000"
  want[42] = "100"
  want[43] = "240"
  want[44] = "343"
  want[45] = "371"
  want[46] = "000"
  want[47] = "200"

  result = "yes"
}
{
  for (f = 2; f <= NF; f++)
    {
      for (i = 0; i < 47; i++)
        got[i] = got[i+1];
      got[47] = $f;

      found = 1
      for (i = 0; i < 48; i++)
        if (got[i] != want[i])
          {
            found = 0
            break
          }
      if (found)
        {
          result = "no"
          exit
        }
    }
}
END {
  print result
}
]EOF
GMP_TRY_ASSEMBLE(
[	.text
	.align	32
{ .mmi;	add	r14 = r15, r16
	add	r17 = r18, r19
	add	r20 = r21, r22 ;; }
	.align	32
{ .mmi;	add	r23 = r24, r25
	add	r26 = r27, r28
	add	r29 = r30, r31 ;; }
],
  [gmp_cv_asm_ia64_align_ok=`od -b conftest.$OBJEXT | $AWK -f conftest.awk`],
  [AC_MSG_WARN([oops, cannot compile test program])
   gmp_cv_asm_ia64_align_ok=yes])
])
GMP_DEFINE_RAW(["define(<IA64_ALIGN_OK>, <\`$gmp_cv_asm_ia64_align_ok'>)"])
])




dnl  GMP_ASM_M68K_INSTRUCTION
dnl  ------------------------
dnl  Not sure if ".l" and "%" are independent settings, but it doesn't hurt
dnl  to try all four possibilities.  Note that the % ones must be first, so
dnl  "d0" won't be interpreted as a label.
dnl
dnl  gas 1.92.3 on NetBSD 1.4 needs to be tested with a two operand
dnl  instruction.  It takes registers without "%", but a single operand
dnl  "clrl %d0" only gives a warning, not an error.

AC_DEFUN([GMP_ASM_M68K_INSTRUCTION],
[AC_REQUIRE([GMP_ASM_TEXT])
AC_CACHE_CHECK([assembler instruction and register style],
		gmp_cv_asm_m68k_instruction,
[for i in "addl %d0,%d1" "add.l %d0,%d1" "addl d0,d1" "add.l d0,d1"; do
  GMP_TRY_ASSEMBLE(
    [	$gmp_cv_asm_text
	$i],
    [gmp_cv_asm_m68k_instruction=$i
    rm -f conftest*
    break])
done
if test -z "$gmp_cv_asm_m68k_instruction"; then
  AC_MSG_ERROR([cannot determine assembler instruction and register style])
fi
])
case $gmp_cv_asm_m68k_instruction in
"addl d0,d1")    want_dot_size=no;  want_register_percent=no  ;;
"addl %d0,%d1")  want_dot_size=no;  want_register_percent=yes ;;
"add.l d0,d1")   want_dot_size=yes; want_register_percent=no  ;;
"add.l %d0,%d1") want_dot_size=yes; want_register_percent=yes ;;
*) AC_MSG_ERROR([oops, unrecognised instruction and register style]) ;;
esac
GMP_DEFINE_RAW(["define(<WANT_REGISTER_PERCENT>, <\`$want_register_percent'>)"])
GMP_DEFINE_RAW(["define(<WANT_DOT_SIZE>, <\`$want_dot_size'>)"])
])


dnl  GMP_ASM_M68K_ADDRESSING
dnl  -----------------------

AC_DEFUN([GMP_ASM_M68K_ADDRESSING],
[AC_REQUIRE([GMP_ASM_TEXT])
AC_REQUIRE([GMP_ASM_M68K_INSTRUCTION])
AC_CACHE_CHECK([assembler addressing style],
		gmp_cv_asm_m68k_addressing,
[case $gmp_cv_asm_m68k_instruction in
addl*)  movel=movel ;;
add.l*) movel=move.l ;;
*) AC_MSG_ERROR([oops, unrecognised gmp_cv_asm_m68k_instruction]) ;;
esac
case $gmp_cv_asm_m68k_instruction in
*"%d0,%d1") dreg=%d0; areg=%a0 ;;
*"d0,d1")   dreg=d0;  areg=a0  ;;
*) AC_MSG_ERROR([oops, unrecognised gmp_cv_asm_m68k_instruction]) ;;
esac
GMP_TRY_ASSEMBLE(
[	$gmp_cv_asm_text
	$movel	$dreg, $areg@-],
  [gmp_cv_asm_m68k_addressing=mit],
[GMP_TRY_ASSEMBLE(
[	$gmp_cv_asm_text
	$movel	$dreg, -($areg)],
  [gmp_cv_asm_m68k_addressing=motorola],
[AC_MSG_ERROR([cannot determine assembler addressing style])])])
])
GMP_DEFINE_RAW(["define(<WANT_ADDRESSING>, <\`$gmp_cv_asm_m68k_addressing'>)"])
])


dnl  GMP_ASM_M68K_BRANCHES
dnl  ---------------------
dnl  "bra" is the standard branch instruction.  "jra" or "jbra" are
dnl  preferred where available, since on gas for instance they give a
dnl  displacement only as big as it needs to be, whereas "bra" is always
dnl  16-bits.  This applies to the conditional branches "bcc" etc too.
dnl  However "dbcc" etc on gas are already only as big as they need to be.

AC_DEFUN([GMP_ASM_M68K_BRANCHES],
[AC_REQUIRE([GMP_ASM_TEXT])
AC_CACHE_CHECK([assembler shortest branches],
		gmp_cv_asm_m68k_branches,
[for i in jra jbra bra; do
  GMP_TRY_ASSEMBLE(
[	$gmp_cv_asm_text
foo$gmp_cv_asm_label_suffix
	$i	foo],
  [gmp_cv_asm_m68k_branches=$i
  rm -f conftest*
  break])
done
if test -z "$gmp_cv_asm_m68k_branches"; then
  AC_MSG_ERROR([cannot determine assembler branching style])
fi
])
GMP_DEFINE_RAW(["define(<WANT_BRANCHES>, <\`$gmp_cv_asm_m68k_branches'>)"])
])


dnl  GMP_ASM_POWERPC_PIC_ALWAYS
dnl  --------------------------
dnl  Determine whether PIC is the default compiler output.
dnl
dnl  SVR4 style "foo@ha" addressing is interpreted as non-PIC, and anything
dnl  else is assumed to require PIC always (Darwin or AIX).  SVR4 is the
dnl  only non-PIC addressing syntax the asm files have at the moment anyway.
dnl
dnl  Libtool does this by taking "*-*-aix* | *-*-darwin* | *-*-rhapsody*" to
dnl  mean PIC always, but it seems more reliable to grep the compiler
dnl  output.
dnl
dnl The next paragraph is untrue for Tiger.  Was it ever true?  For tiger,
dnl "cc -fast" makes non-PIC the default (and the binaries do run).
dnl  On Darwin "cc -static" is non-PIC with syntax "ha16(_foo)", but that's
dnl  apparently only for use in the kernel, which we're not attempting to
dnl  target at the moment, so don't look for that.

AC_DEFUN([GMP_ASM_POWERPC_PIC_ALWAYS],
[AC_REQUIRE([AC_PROG_CC])
AC_CACHE_CHECK([whether compiler output is PIC by default],
               gmp_cv_asm_powerpc_pic,
[gmp_cv_asm_powerpc_pic=yes
cat >conftest.c <<EOF
int foo;
int *bar() { return &foo; }
EOF
echo "Test program:" >&AC_FD_CC
cat conftest.c >&AC_FD_CC
gmp_compile="$CC $CFLAGS $CPPFLAGS -S conftest.c >&AC_FD_CC"
if AC_TRY_EVAL(gmp_compile); then
  echo "Compiler output:" >&AC_FD_CC
  cat conftest.s >&AC_FD_CC
  if grep 'foo@ha' conftest.s >/dev/null 2>&AC_FD_CC; then
    gmp_cv_asm_powerpc_pic=no
  fi
  if grep 'ha16(_foo)' conftest.s >/dev/null 2>&AC_FD_CC; then
    gmp_cv_asm_powerpc_pic=no
  fi
fi
rm -f conftest*
])
GMP_DEFINE_RAW(["define(<PIC_ALWAYS>,<$gmp_cv_asm_powerpc_pic>)"])
])


dnl  GMP_ASM_POWERPC_R_REGISTERS
dnl  ---------------------------
dnl  Determine whether the assembler takes powerpc registers with an "r" as
dnl  in "r6", or as plain "6".  The latter is standard, but NeXT, Rhapsody,
dnl  and MacOS-X require the "r" forms.
dnl
dnl  See also mpn/powerpc32/powerpc-defs.m4 which uses the result of this
dnl  test.

AC_DEFUN([GMP_ASM_POWERPC_R_REGISTERS],
[AC_REQUIRE([GMP_ASM_TEXT])
AC_CACHE_CHECK([if the assembler needs r on registers],
               gmp_cv_asm_powerpc_r_registers,
[GMP_TRY_ASSEMBLE(
[	$gmp_cv_asm_text
	mtctr	6],
[gmp_cv_asm_powerpc_r_registers=no],
[GMP_TRY_ASSEMBLE(
[	$gmp_cv_asm_text
	mtctr	r6],
[gmp_cv_asm_powerpc_r_registers=yes],
[AC_MSG_ERROR([neither "mtctr 6" nor "mtctr r6" works])])])])

GMP_DEFINE_RAW(["define(<WANT_R_REGISTERS>,<$gmp_cv_asm_powerpc_r_registers>)"])
])


dnl  GMP_ASM_SPARC_REGISTER
dnl  ----------------------
dnl  Determine whether the assembler accepts the ".register" directive.
dnl  Old versions of solaris "as" don't.
dnl
dnl  See also mpn/sparc32/sparc-defs.m4 which uses the result of this test.

AC_DEFUN([GMP_ASM_SPARC_REGISTER],
[AC_REQUIRE([GMP_ASM_TEXT])
AC_CACHE_CHECK([if the assembler accepts ".register"],
               gmp_cv_asm_sparc_register,
[GMP_TRY_ASSEMBLE(
[	$gmp_cv_asm_text
	.register	%g2,#scratch
],
[gmp_cv_asm_sparc_register=yes],
[gmp_cv_asm_sparc_register=no])])

GMP_DEFINE_RAW(["define(<HAVE_REGISTER>,<$gmp_cv_asm_sparc_register>)"])
])


dnl  GMP_C_ATTRIBUTE_CONST
dnl  ---------------------

AC_DEFUN([GMP_C_ATTRIBUTE_CONST],
[AC_CACHE_CHECK([whether gcc __attribute__ ((const)) works],
                gmp_cv_c_attribute_const,
[AC_TRY_COMPILE([int foo (int x) __attribute__ ((const));], ,
  gmp_cv_c_attribute_const=yes, gmp_cv_c_attribute_const=no)
])
if test $gmp_cv_c_attribute_const = yes; then
  AC_DEFINE(HAVE_ATTRIBUTE_CONST, 1,
  [Define to 1 if the compiler accepts gcc style __attribute__ ((const))])
fi
])


dnl  GMP_C_ATTRIBUTE_MALLOC
dnl  ----------------------
dnl  gcc 2.95.x accepts __attribute__ ((malloc)) but with a warning that
dnl  it's ignored.  Pretend it doesn't exist in this case, to avoid that
dnl  warning.

AC_DEFUN([GMP_C_ATTRIBUTE_MALLOC],
[AC_CACHE_CHECK([whether gcc __attribute__ ((malloc)) works],
                gmp_cv_c_attribute_malloc,
[cat >conftest.c <<EOF
void *foo (int x) __attribute__ ((malloc));
EOF
gmp_compile="$CC $CFLAGS $CPPFLAGS -c conftest.c >conftest.out 2>&1"
if AC_TRY_EVAL(gmp_compile); then
  if grep "attribute directive ignored" conftest.out >/dev/null; then
    gmp_cv_c_attribute_malloc=no
  else
    gmp_cv_c_attribute_malloc=yes
  fi
else
  gmp_cv_c_attribute_malloc=no
fi
cat conftest.out >&AC_FD_CC
rm -f conftest*
])
if test $gmp_cv_c_attribute_malloc = yes; then
  AC_DEFINE(HAVE_ATTRIBUTE_MALLOC, 1,
  [Define to 1 if the compiler accepts gcc style __attribute__ ((malloc))])
fi
])


dnl  GMP_C_ATTRIBUTE_MODE
dnl  --------------------
dnl  Introduced in gcc 2.2, but perhaps not in all Apple derived versions.

AC_DEFUN([GMP_C_ATTRIBUTE_MODE],
[AC_CACHE_CHECK([whether gcc __attribute__ ((mode (XX))) works],
                gmp_cv_c_attribute_mode,
[AC_TRY_COMPILE([typedef int SItype __attribute__ ((mode (SI)));], ,
  gmp_cv_c_attribute_mode=yes, gmp_cv_c_attribute_mode=no)
])
if test $gmp_cv_c_attribute_mode = yes; then
  AC_DEFINE(HAVE_ATTRIBUTE_MODE, 1,
  [Define to 1 if the compiler accepts gcc style __attribute__ ((mode (XX)))])
fi
])


dnl  GMP_C_ATTRIBUTE_NORETURN
dnl  ------------------------

AC_DEFUN([GMP_C_ATTRIBUTE_NORETURN],
[AC_CACHE_CHECK([whether gcc __attribute__ ((noreturn)) works],
                gmp_cv_c_attribute_noreturn,
[AC_TRY_COMPILE([void foo (int x) __attribute__ ((noreturn));], ,
  gmp_cv_c_attribute_noreturn=yes, gmp_cv_c_attribute_noreturn=no)
])
if test $gmp_cv_c_attribute_noreturn = yes; then
  AC_DEFINE(HAVE_ATTRIBUTE_NORETURN, 1,
  [Define to 1 if the compiler accepts gcc style __attribute__ ((noreturn))])
fi
])


dnl  GMP_C_DOUBLE_FORMAT
dnl  -------------------
dnl  Determine the floating point format.
dnl
dnl  The object file is grepped, in order to work when cross compiling.  A
dnl  start and end sequence is included to avoid false matches, and
dnl  allowance is made for the desired data crossing an "od -b" line
dnl  boundary.  The test number is a small integer so it should appear
dnl  exactly, no rounding or truncation etc.
dnl
dnl  "od -b", incidentally, is supported even by Unix V7, and the awk script
dnl  used doesn't have functions or anything, so even an "old" awk should
dnl  suffice.

AC_DEFUN([GMP_C_DOUBLE_FORMAT],
[AC_REQUIRE([AC_PROG_CC])
AC_REQUIRE([AC_PROG_AWK])
AC_CACHE_CHECK([format of `double' floating point],
                gmp_cv_c_double_format,
[gmp_cv_c_double_format=unknown
cat >conftest.c <<\EOF
[struct {
  char    before[8];
  double  x;
  char    after[8];
} foo = {
  { '\001', '\043', '\105', '\147', '\211', '\253', '\315', '\357' },
  -123456789.0,
  { '\376', '\334', '\272', '\230', '\166', '\124', '\062', '\020' },
};]
EOF
gmp_compile="$CC $CFLAGS $CPPFLAGS -c conftest.c >&AC_FD_CC 2>&1"
if AC_TRY_EVAL(gmp_compile); then
cat >conftest.awk <<\EOF
[
BEGIN {
  found = 0
}

{
  for (f = 2; f <= NF; f++)
    {
      for (i = 0; i < 23; i++)
        got[i] = got[i+1];
      got[23] = $f;

      # match the special begin and end sequences
      if (got[0] != "001") continue
      if (got[1] != "043") continue
      if (got[2] != "105") continue
      if (got[3] != "147") continue
      if (got[4] != "211") continue
      if (got[5] != "253") continue
      if (got[6] != "315") continue
      if (got[7] != "357") continue
      if (got[16] != "376") continue
      if (got[17] != "334") continue
      if (got[18] != "272") continue
      if (got[19] != "230") continue
      if (got[20] != "166") continue
      if (got[21] != "124") continue
      if (got[22] != "062") continue
      if (got[23] != "020") continue

      saw = " (" got[8] " " got[9] " " got[10] " " got[11] " " got[12] " " got[13] " " got[14] " " got[15] ")"

      if (got[8]  == "000" &&  \
          got[9]  == "000" &&  \
          got[10] == "000" &&  \
          got[11] == "124" &&  \
          got[12] == "064" &&  \
          got[13] == "157" &&  \
          got[14] == "235" &&  \
          got[15] == "301")
        {
          print "IEEE little endian"
          found = 1
          exit
        }

      # Little endian with the two 4-byte halves swapped, as used by ARM
      # when the chip is in little endian mode.
      #
      if (got[8]  == "064" &&  \
          got[9]  == "157" &&  \
          got[10] == "235" &&  \
          got[11] == "301" &&  \
          got[12] == "000" &&  \
          got[13] == "000" &&  \
          got[14] == "000" &&  \
          got[15] == "124")
        {
          print "IEEE little endian, swapped halves"
          found = 1
          exit
        }

      # gcc 2.95.4 on one GNU/Linux ARM system was seen generating 000 in
      # the last byte, whereas 124 is correct.  Not sure where the bug
      # actually lies, but a running program didn't seem to get a full
      # mantissa worth of working bits.
      #
      # We match this case explicitly so we can give a nice result message,
      # but we deliberately exclude it from the normal IEEE double setups
      # since it's too broken.
      #
      if (got[8]  == "064" &&  \
          got[9]  == "157" &&  \
          got[10] == "235" &&  \
          got[11] == "301" &&  \
          got[12] == "000" &&  \
          got[13] == "000" &&  \
          got[14] == "000" &&  \
          got[15] == "000")
        {
          print "bad ARM software floats"
          found = 1
          exit
        }

      if (got[8]  == "301" &&  \
          got[9]  == "235" &&  \
          got[10] == "157" &&  \
          got[11] == "064" &&  \
          got[12] == "124" &&  \
          got[13] == "000" &&  \
          got[14] == "000" &&  \
	  got[15] == "000")
        {
          print "IEEE big endian"
          found = 1
          exit
        }

      if (got[8]  == "353" &&  \
          got[9]  == "315" &&  \
          got[10] == "242" &&  \
          got[11] == "171" &&  \
          got[12] == "000" &&  \
          got[13] == "240" &&  \
          got[14] == "000" &&  \
          got[15] == "000")
        {
          print "VAX D"
          found = 1
          exit
        }

      if (got[8]  == "275" &&  \
          got[9]  == "301" &&  \
          got[10] == "064" &&  \
          got[11] == "157" &&  \
          got[12] == "000" &&  \
          got[13] == "124" &&  \
          got[14] == "000" &&  \
          got[15] == "000")
        {
          print "VAX G"
          found = 1
          exit
        }

      if (got[8]  == "300" &&  \
          got[9]  == "033" &&  \
          got[10] == "353" &&  \
          got[11] == "171" &&  \
          got[12] == "242" &&  \
          got[13] == "240" &&  \
          got[14] == "000" &&  \
          got[15] == "000")
        {
          print "Cray CFP"
          found = 1
          exit
        }
    }
}

END {
  if (! found)
    print "unknown", saw
}
]
EOF
  gmp_cv_c_double_format=`od -b conftest.$OBJEXT | $AWK -f conftest.awk`
  case $gmp_cv_c_double_format in
  unknown*)
    echo "cannot match anything, conftest.$OBJEXT contains" >&AC_FD_CC
    od -b conftest.$OBJEXT >&AC_FD_CC
    ;;
  esac
else
  AC_MSG_WARN([oops, cannot compile test program])
fi
rm -f conftest*
])

AH_VERBATIM([HAVE_DOUBLE],
[/* Define one of the following to 1 for the format of a `double'.
   If your format is not among these choices, or you don't know what it is,
   then leave all undefined.
   IEEE_LITTLE_SWAPPED means little endian, but with the two 4-byte halves
   swapped, as used by ARM CPUs in little endian mode.  */
#undef HAVE_DOUBLE_IEEE_BIG_ENDIAN
#undef HAVE_DOUBLE_IEEE_LITTLE_ENDIAN
#undef HAVE_DOUBLE_IEEE_LITTLE_SWAPPED
#undef HAVE_DOUBLE_VAX_D
#undef HAVE_DOUBLE_VAX_G
#undef HAVE_DOUBLE_CRAY_CFP])

case $gmp_cv_c_double_format in
  "IEEE big endian")
    AC_DEFINE(HAVE_DOUBLE_IEEE_BIG_ENDIAN, 1)
    GMP_DEFINE_RAW("define_not_for_expansion(\`HAVE_DOUBLE_IEEE_BIG_ENDIAN')", POST)
    ;;
  "IEEE little endian")
    AC_DEFINE(HAVE_DOUBLE_IEEE_LITTLE_ENDIAN, 1)
    GMP_DEFINE_RAW("define_not_for_expansion(\`HAVE_DOUBLE_IEEE_LITTLE_ENDIAN')", POST)
    ;;
  "IEEE little endian, swapped halves")
    AC_DEFINE(HAVE_DOUBLE_IEEE_LITTLE_SWAPPED, 1) ;;
  "VAX D")
    AC_DEFINE(HAVE_DOUBLE_VAX_D, 1) ;;
  "VAX G")
    AC_DEFINE(HAVE_DOUBLE_VAX_G, 1) ;;
  "Cray CFP")
    AC_DEFINE(HAVE_DOUBLE_CRAY_CFP, 1) ;;
  "bad ARM software floats")
    ;;
  unknown*)
    AC_MSG_WARN([Could not determine float format.])
    AC_MSG_WARN([Conversions to and from "double" may be slow.])
    ;;
  *)
    AC_MSG_WARN([oops, unrecognised float format: $gmp_cv_c_double_format])
    ;;
esac
])


dnl  GMP_C_STDARG
dnl  ------------
dnl  Test whether to use <stdarg.h> or <varargs.h>.
dnl
dnl  Notice the AC_DEFINE here is HAVE_STDARG to avoid clashing with
dnl  HAVE_STDARG_H which could arise from AC_CHECK_HEADERS.
dnl
dnl  This test might be slight overkill, after all there's really only going
dnl  to be ANSI or K&R and the two can be differentiated by AC_PROG_CC_STDC
dnl  or very likely by the setups for _PROTO in gmp.h.  On the other hand
dnl  this test is nice and direct, being what we're going to actually use.

AC_DEFUN([GMP_C_STDARG],
[AC_CACHE_CHECK([whether <stdarg.h> exists and works],
                gmp_cv_c_stdarg,
[AC_TRY_COMPILE(
[#include <stdarg.h>
int foo (int x, ...)
{
  va_list  ap;
  int      y;
  va_start (ap, x);
  y = va_arg (ap, int);
  va_end (ap);
  return y;
}],,
gmp_cv_c_stdarg=yes, gmp_cv_c_stdarg=no)
])
if test $gmp_cv_c_stdarg = yes; then
  AC_DEFINE(HAVE_STDARG, 1, [Define to 1 if <stdarg.h> exists and works])
fi
])


dnl  GMP_FUNC_ALLOCA
dnl  ---------------
dnl  Determine whether "alloca" is available.  This is AC_FUNC_ALLOCA from
dnl  autoconf, but changed so it doesn't use alloca.c if alloca() isn't
dnl  available, and also to use gmp-impl.h for the conditionals detecting
dnl  compiler builtin alloca's.

AC_DEFUN([GMP_FUNC_ALLOCA],
[AC_REQUIRE([GMP_HEADER_ALLOCA])
AC_CACHE_CHECK([for alloca (via gmp-impl.h)],
               gmp_cv_func_alloca,
[AC_TRY_LINK(
GMP_INCLUDE_GMP_H
[#include "$srcdir/gmp-impl.h"
],
  [char *p = (char *) alloca (1);],
  gmp_cv_func_alloca=yes,
  gmp_cv_func_alloca=no)])
if test $gmp_cv_func_alloca = yes; then
  AC_DEFINE(HAVE_ALLOCA, 1, [Define to 1 if alloca() works (via gmp-impl.h).])
fi
])

AC_DEFUN([GMP_HEADER_ALLOCA],
[# The Ultrix 4.2 mips builtin alloca declared by alloca.h only works
# for constant arguments.  Useless!
AC_CACHE_CHECK([for working alloca.h],
               gmp_cv_header_alloca,
[AC_TRY_LINK([#include <alloca.h>],
  [char *p = (char *) alloca (2 * sizeof (int));],
  gmp_cv_header_alloca=yes,
  gmp_cv_header_alloca=no)])
if test $gmp_cv_header_alloca = yes; then
  AC_DEFINE(HAVE_ALLOCA_H, 1,
  [Define to 1 if you have <alloca.h> and it should be used (not on Ultrix).])
fi
])


dnl  GMP_OPTION_ALLOCA
dnl  -----------------
dnl  Decide what to do about --enable-alloca from the user.
dnl  This is a macro so it can require GMP_FUNC_ALLOCA.

AC_DEFUN([GMP_OPTION_ALLOCA],
[AC_REQUIRE([GMP_FUNC_ALLOCA])
AC_CACHE_CHECK([how to allocate temporary memory],
               gmp_cv_option_alloca,
[case $enable_alloca in
  yes)
    gmp_cv_option_alloca=alloca
    ;;
  no)
    gmp_cv_option_alloca=malloc-reentrant
    ;;
  reentrant | notreentrant)
    case $gmp_cv_func_alloca in
    yes)  gmp_cv_option_alloca=alloca ;;
    *)    gmp_cv_option_alloca=malloc-$enable_alloca ;;
    esac
    ;;
  *)
    gmp_cv_option_alloca=$enable_alloca
    ;;
esac
])

AH_VERBATIM([WANT_TMP],
[/* Define one of these to 1 for the desired temporary memory allocation
   method, per --enable-alloca. */
#undef WANT_TMP_ALLOCA
#undef WANT_TMP_REENTRANT
#undef WANT_TMP_NOTREENTRANT
#undef WANT_TMP_DEBUG])

case $gmp_cv_option_alloca in
  alloca)
    if test $gmp_cv_func_alloca = no; then
      AC_MSG_ERROR([--enable-alloca=alloca specified, but alloca not available])
    fi
    AC_DEFINE(WANT_TMP_ALLOCA)
    TAL_OBJECT=tal-reent$U.lo
    ;;
  malloc-reentrant)
    AC_DEFINE(WANT_TMP_REENTRANT)
    TAL_OBJECT=tal-reent$U.lo
    ;;
  malloc-notreentrant)
    AC_DEFINE(WANT_TMP_NOTREENTRANT)
    TAL_OBJECT=tal-notreent$U.lo
    ;;
  debug)
    AC_DEFINE(WANT_TMP_DEBUG)
    TAL_OBJECT=tal-debug$U.lo
    ;;
  *)
    # checks at the start of configure.in should protect us
    AC_MSG_ERROR([unrecognised --enable-alloca=$gmp_cv_option_alloca])
    ;;
esac
AC_SUBST(TAL_OBJECT)
])


dnl  GMP_FUNC_SSCANF_WRITABLE_INPUT
dnl  ------------------------------
dnl  Determine whether sscanf requires a writable input string.
dnl
dnl  It might be nicer to run a program to determine this when doing a
dnl  native build, but the systems afflicted are few and far between these
dnl  days, so it seems good enough just to list them.

AC_DEFUN([GMP_FUNC_SSCANF_WRITABLE_INPUT],
[AC_CACHE_CHECK([whether sscanf needs writable input],
                 gmp_cv_func_sscanf_writable_input,
[case $host in
  *-*-hpux9 | *-*-hpux9.*)
     gmp_cv_func_sscanf_writable_input=yes ;;
  *) gmp_cv_func_sscanf_writable_input=no  ;;
esac
])
case $gmp_cv_func_sscanf_writable_input in
  yes) AC_DEFINE(SSCANF_WRITABLE_INPUT, 1,
                 [Define to 1 if sscanf requires writable inputs]) ;;
  no)  ;;
  *)   AC_MSG_ERROR([unrecognised \$gmp_cv_func_sscanf_writable_input]) ;;
esac
])


dnl  GMP_FUNC_VSNPRINTF
dnl  ------------------
dnl  Check whether vsnprintf exists, and works properly.
dnl
dnl  Systems without vsnprintf include mingw32, OSF 4.
dnl
dnl  Sparc Solaris 2.7 in 64-bit mode doesn't always truncate, making
dnl  vsnprintf like vsprintf, and hence completely useless.  On one system a
dnl  literal string is enough to provoke the problem, on another a "%n" was
dnl  needed.  There seems to be something weird going on with the optimizer
dnl  or something, since on the first system adding a second check with
dnl  "%n", or even just an initialized local variable, makes it work.  In
dnl  any case, without bothering to get to the bottom of this, the two
dnl  program runs in the code below end up successfully detecting the
dnl  problem.
dnl
dnl  glibc 2.0.x returns either -1 or bufsize-1 for an overflow (both seen,
dnl  not sure which 2.0.x does which), but still puts the correct null
dnl  terminated result into the buffer.

AC_DEFUN([GMP_FUNC_VSNPRINTF],
[AC_REQUIRE([GMP_C_STDARG])
AC_CHECK_FUNC(vsnprintf,
              [gmp_vsnprintf_exists=yes],
              [gmp_vsnprintf_exists=no])
if test "$gmp_vsnprintf_exists" = no; then
  gmp_cv_func_vsnprintf=no
else
  AC_CACHE_CHECK([whether vsnprintf works],
                 gmp_cv_func_vsnprintf,
  [gmp_cv_func_vsnprintf=yes
   for i in 'check ("hello world");' 'int n; check ("%nhello world", &n);'; do
     AC_TRY_RUN([
#include <string.h>  /* for strcmp */
#include <stdio.h>   /* for vsnprintf */

#if HAVE_STDARG
#include <stdarg.h>
#else
#include <varargs.h>
#endif

int
#if HAVE_STDARG
check (const char *fmt, ...)
#else
check (va_alist)
     va_dcl
#endif
{
  static char  buf[128];
  va_list  ap;
  int      ret;

#if HAVE_STDARG
  va_start (ap, fmt);
#else
  char *fmt;
  va_start (ap);
  fmt = va_arg (ap, char *);
#endif

  ret = vsnprintf (buf, 4, fmt, ap);

  if (strcmp (buf, "hel") != 0)
    exit (1);

  /* allowed return values */
  if (ret != -1 && ret != 3 && ret != 11)
    exit (2);

  return 0;
}

int
main ()
{
$i
  exit (0);
}
],
      [:],
      [gmp_cv_func_vsnprintf=no; break],
      [gmp_cv_func_vsnprintf=probably; break])
  done
  ])
  if test "$gmp_cv_func_vsnprintf" = probably; then
    AC_MSG_WARN([cannot check for properly working vsnprintf when cross compiling, will assume it's ok])
  fi
  if test "$gmp_cv_func_vsnprintf" != no; then
    AC_DEFINE(HAVE_VSNPRINTF,1,
    [Define to 1 if you have the `vsnprintf' function and it works properly.])
  fi
fi
])


dnl  GMP_H_ANSI
dnl  ----------
dnl  Check whether gmp.h recognises the compiler as ANSI capable.

AC_DEFUN([GMP_H_ANSI],
[AC_REQUIRE([AC_PROG_CC_STDC])
case $ac_cv_prog_cc_stdc in
  no)
    ;;
  *)
    AC_TRY_COMPILE(
GMP_INCLUDE_GMP_H
[#if ! __GMP_HAVE_PROTOTYPES
die die die
#endif
],,,
    [AC_MSG_WARN([gmp.h doesnt recognise compiler as ANSI, prototypes and "const" will be unavailable])])
    ;;
esac
])


dnl  GMP_H_EXTERN_INLINE
dnl  -------------------
dnl  If the compiler has an "inline" of some sort, check whether the
dnl  #ifdef's in gmp.h recognise it.

AC_DEFUN([GMP_H_EXTERN_INLINE],
[AC_REQUIRE([AC_C_INLINE])
case $ac_cv_c_inline in
no) ;;
*)
  AC_TRY_COMPILE(
[#define __GMP_WITHIN_CONFIGURE_INLINE 1
]GMP_INCLUDE_GMP_H[
#ifndef __GMP_EXTERN_INLINE
die die die
#endif
],,,
  [case $ac_cv_c_inline in
  yes) tmp_inline=inline ;;
  *)   tmp_inline=$ac_cv_c_inline ;;
  esac    
  AC_MSG_WARN([gmp.h doesnt recognise compiler "$tmp_inline", inlines will be unavailable])])
  ;;
esac
])


dnl  GMP_H_HAVE_FILE
dnl  ---------------
dnl  Check whether the #ifdef's in gmp.h recognise when stdio.h has been
dnl  included to get FILE.

AC_DEFUN([GMP_H_HAVE_FILE],
[AC_TRY_COMPILE(
[#include <stdio.h>]
GMP_INCLUDE_GMP_H
[#if ! _GMP_H_HAVE_FILE
die die die
#endif
],,,
  [AC_MSG_WARN([gmp.h doesnt recognise <stdio.h>, FILE prototypes will be unavailable])])
])


dnl  GMP_PROG_CC_FOR_BUILD
dnl  ---------------------
dnl  Establish CC_FOR_BUILD, a C compiler for the build system.
dnl
dnl  If CC_FOR_BUILD is set then it's expected to work, likewise the old
dnl  style HOST_CC, otherwise some likely candidates are tried, the same as
dnl  configfsf.guess.

AC_DEFUN([GMP_PROG_CC_FOR_BUILD],
[AC_REQUIRE([AC_PROG_CC])
if test -n "$CC_FOR_BUILD"; then
  GMP_PROG_CC_FOR_BUILD_WORKS($CC_FOR_BUILD,,
    [AC_MSG_ERROR([Specified CC_FOR_BUILD doesn't seem to work])])
elif test -n "$HOST_CC"; then
  GMP_PROG_CC_FOR_BUILD_WORKS($HOST_CC,
    [CC_FOR_BUILD=$HOST_CC],
    [AC_MSG_ERROR([Specified HOST_CC doesn't seem to work])])
else
  for i in "$CC" "$CC $CFLAGS $CPPFLAGS" cc gcc c89 c99; do
    GMP_PROG_CC_FOR_BUILD_WORKS($i,
      [CC_FOR_BUILD=$i
       break])
  done
  if test -z "$CC_FOR_BUILD"; then
    AC_MSG_ERROR([Cannot find a build system compiler])
  fi
fi
    
AC_ARG_VAR(CC_FOR_BUILD,[build system C compiler])
AC_SUBST(CC_FOR_BUILD)
])


dnl  GMP_PROG_CC_FOR_BUILD_WORKS(cc/cflags[,[action-if-good][,action-if-bad]])
dnl  -------------------------------------------------------------------------
dnl  See if the given cc/cflags works on the build system.
dnl
dnl  It seems easiest to just use the default compiler output, rather than
dnl  figuring out the .exe or whatever at this stage.

AC_DEFUN([GMP_PROG_CC_FOR_BUILD_WORKS],
[AC_MSG_CHECKING([build system compiler $1])
# remove anything that might look like compiler output to our "||" expression
rm -f conftest* a.out b.out a.exe a_out.exe
cat >conftest.c <<EOF
int
main ()
{
  exit(0);
}
EOF
gmp_compile="$1 conftest.c"
cc_for_build_works=no
if AC_TRY_EVAL(gmp_compile); then
  if (./a.out || ./b.out || ./a.exe || ./a_out.exe || ./conftest) >&AC_FD_CC 2>&1; then
    cc_for_build_works=yes
  fi
fi
rm -f conftest* a.out b.out a.exe a_out.exe
AC_MSG_RESULT($cc_for_build_works)
if test "$cc_for_build_works" = yes; then
  ifelse([$2],,:,[$2])
else
  ifelse([$3],,:,[$3])
fi
])


dnl  GMP_PROG_CPP_FOR_BUILD
dnl  ---------------------
dnl  Establish CPP_FOR_BUILD, the build system C preprocessor.
dnl  The choices tried here are the same as AC_PROG_CPP, but with
dnl  CC_FOR_BUILD.

AC_DEFUN([GMP_PROG_CPP_FOR_BUILD],
[AC_REQUIRE([GMP_PROG_CC_FOR_BUILD])
AC_MSG_CHECKING([for build system preprocessor])
if test -z "$CPP_FOR_BUILD"; then
  AC_CACHE_VAL(gmp_cv_prog_cpp_for_build,
  [cat >conftest.c <<EOF
#define FOO BAR
EOF
  for i in "$CC_FOR_BUILD -E" "$CC_FOR_BUILD -E -traditional-cpp" "/lib/cpp"; do
    gmp_compile="$i conftest.c"
    if AC_TRY_EVAL(gmp_compile) >&AC_FD_CC 2>&1; then
      gmp_cv_prog_cpp_for_build=$i
      break
    fi
  done
  rm -f conftest* a.out b.out a.exe a_out.exe
  if test -z "$gmp_cv_prog_cpp_for_build"; then
    AC_MSG_ERROR([Cannot find build system C preprocessor.])
  fi
  ])
  CPP_FOR_BUILD=$gmp_cv_prog_cpp_for_build
fi
AC_MSG_RESULT([$CPP_FOR_BUILD])

AC_ARG_VAR(CPP_FOR_BUILD,[build system C preprocessor])
AC_SUBST(CPP_FOR_BUILD)
])


dnl  GMP_PROG_EXEEXT_FOR_BUILD
dnl  -------------------------
dnl  Determine EXEEXT_FOR_BUILD, the build system executable suffix.
dnl
dnl  The idea is to find what "-o conftest$foo" will make it possible to run
dnl  the program with ./conftest.  On Unix-like systems this is of course
dnl  nothing, for DOS it's ".exe", or for a strange RISC OS foreign file
dnl  system cross compile it can be ",ff8" apparently.  Not sure if the
dnl  latter actually applies to a build-system executable, maybe it doesn't,
dnl  but it won't hurt to try.

AC_DEFUN([GMP_PROG_EXEEXT_FOR_BUILD],
[AC_REQUIRE([GMP_PROG_CC_FOR_BUILD])
AC_CACHE_CHECK([for build system executable suffix],
               gmp_cv_prog_exeext_for_build,
[cat >conftest.c <<EOF
int
main ()
{
  exit (0);
}
EOF
for i in .exe ,ff8 ""; do
  gmp_compile="$CC_FOR_BUILD conftest.c -o conftest$i"
  if AC_TRY_EVAL(gmp_compile); then
    if (./conftest) 2>&AC_FD_CC; then
      gmp_cv_prog_exeext_for_build=$i
      break
    fi
  fi
done
rm -f conftest*
if test "${gmp_cv_prog_exeext_for_build+set}" != set; then
  AC_MSG_ERROR([Cannot determine executable suffix])
fi
])
AC_SUBST(EXEEXT_FOR_BUILD,$gmp_cv_prog_exeext_for_build)
])


dnl  GMP_C_FOR_BUILD_ANSI
dnl  --------------------
dnl  Determine whether CC_FOR_BUILD is ANSI, and establish U_FOR_BUILD
dnl  accordingly.

AC_DEFUN([GMP_C_FOR_BUILD_ANSI],
[AC_REQUIRE([GMP_PROG_CC_FOR_BUILD])
AC_CACHE_CHECK([whether build system compiler is ANSI],
               gmp_cv_c_for_build_ansi,
[cat >conftest.c <<EOF
int
main (int argc, char *argv[])
{
  exit(0);
}
EOF
gmp_compile="$CC_FOR_BUILD conftest.c"
if AC_TRY_EVAL(gmp_compile); then
  gmp_cv_c_for_build_ansi=yes
else
  gmp_cv_c_for_build_ansi=no
fi
rm -f conftest* a.out b.out a.exe a_out.exe
])
if test "$gmp_cv_c_for_build_ansi" = yes; then
  U_FOR_BUILD=
else
  AC_SUBST(U_FOR_BUILD,_)
fi
])


dnl  GMP_CHECK_LIBM_FOR_BUILD
dnl  ------------------------
dnl  Establish LIBM_FOR_BUILD as -lm, if that seems to work.
dnl
dnl  Libtool AC_CHECK_LIBM also uses -lmw on *-ncr-sysv4.3*, if it works.
dnl  Don't know what that does, lets assume it's not needed just for log().

AC_DEFUN([GMP_CHECK_LIBM_FOR_BUILD],
[AC_REQUIRE([GMP_PROG_CC_FOR_BUILD])
AC_CACHE_CHECK([for build system compiler math library],
               gmp_cv_check_libm_for_build,
[cat >conftest.c <<EOF
int
main ()
{
  exit(0);
}
double d;
double
foo ()
{
  return log (d);
}
EOF
gmp_compile="$CC_FOR_BUILD conftest.c -lm"
if AC_TRY_EVAL(gmp_compile); then
  gmp_cv_check_libm_for_build=-lm
else
  gmp_cv_check_libm_for_build=no
fi
rm -f conftest* a.out b.out a.exe a_out.exe
])
case $gmp_cv_check_libm_for_build in
  yes) AC_SUBST(LIBM_FOR_BUILD,-lm) ;;
  no)  LIBM_FOR_BUILD= ;;
  *)   LIBM_FOR_BUILD=$gmp_cv_check_libm_for_build ;;
esac
])

# libtool.m4 - Configure libtool for the host system. -*-Autoconf-*-

# serial 47 AC_PROG_LIBTOOL


# AC_PROVIDE_IFELSE(MACRO-NAME, IF-PROVIDED, IF-NOT-PROVIDED)
# -----------------------------------------------------------
# If this macro is not defined by Autoconf, define it here.
m4_ifdef([AC_PROVIDE_IFELSE],
         [],
         [m4_define([AC_PROVIDE_IFELSE],
	         [m4_ifdef([AC_PROVIDE_$1],
		           [$2], [$3])])])


# AC_PROG_LIBTOOL
# ---------------
AC_DEFUN([AC_PROG_LIBTOOL],
[AC_REQUIRE([_AC_PROG_LIBTOOL])dnl
dnl If AC_PROG_CXX has already been expanded, run AC_LIBTOOL_CXX
dnl immediately, otherwise, hook it in at the end of AC_PROG_CXX.
  AC_PROVIDE_IFELSE([AC_PROG_CXX],
    [AC_LIBTOOL_CXX],
    [define([AC_PROG_CXX], defn([AC_PROG_CXX])[AC_LIBTOOL_CXX
  ])])
dnl And a similar setup for Fortran 77 support
  AC_PROVIDE_IFELSE([AC_PROG_F77],
    [AC_LIBTOOL_F77],
    [define([AC_PROG_F77], defn([AC_PROG_F77])[AC_LIBTOOL_F77
])])

dnl Quote A][M_PROG_GCJ so that aclocal doesn't bring it in needlessly.
dnl If either AC_PROG_GCJ or A][M_PROG_GCJ have already been expanded, run
dnl AC_LIBTOOL_GCJ immediately, otherwise, hook it in at the end of both.
  AC_PROVIDE_IFELSE([AC_PROG_GCJ],
    [AC_LIBTOOL_GCJ],
    [AC_PROVIDE_IFELSE([A][M_PROG_GCJ],
      [AC_LIBTOOL_GCJ],
      [AC_PROVIDE_IFELSE([LT_AC_PROG_GCJ],
	[AC_LIBTOOL_GCJ],
      [ifdef([AC_PROG_GCJ],
	     [define([AC_PROG_GCJ], defn([AC_PROG_GCJ])[AC_LIBTOOL_GCJ])])
       ifdef([A][M_PROG_GCJ],
	     [define([A][M_PROG_GCJ], defn([A][M_PROG_GCJ])[AC_LIBTOOL_GCJ])])
       ifdef([LT_AC_PROG_GCJ],
	     [define([LT_AC_PROG_GCJ],
		defn([LT_AC_PROG_GCJ])[AC_LIBTOOL_GCJ])])])])
])])# AC_PROG_LIBTOOL


# _AC_PROG_LIBTOOL
# ----------------
AC_DEFUN([_AC_PROG_LIBTOOL],
[AC_REQUIRE([AC_LIBTOOL_SETUP])dnl
AC_BEFORE([$0],[AC_LIBTOOL_CXX])dnl
AC_BEFORE([$0],[AC_LIBTOOL_F77])dnl
AC_BEFORE([$0],[AC_LIBTOOL_GCJ])dnl

# This can be used to rebuild libtool when needed
LIBTOOL_DEPS="$ac_aux_dir/ltmain.sh"

# Always use our own libtool.
LIBTOOL='$(SHELL) $(top_builddir)/libtool'
AC_SUBST(LIBTOOL)dnl

# Prevent multiple expansion
define([AC_PROG_LIBTOOL], [])
])# _AC_PROG_LIBTOOL


# AC_LIBTOOL_SETUP
# ----------------
AC_DEFUN([AC_LIBTOOL_SETUP],
[AC_PREREQ(2.50)dnl
AC_REQUIRE([AC_ENABLE_SHARED])dnl
AC_REQUIRE([AC_ENABLE_STATIC])dnl
AC_REQUIRE([AC_ENABLE_FAST_INSTALL])dnl
AC_REQUIRE([AC_CANONICAL_HOST])dnl
AC_REQUIRE([AC_CANONICAL_BUILD])dnl
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_LD])dnl
AC_REQUIRE([AC_PROG_LD_RELOAD_FLAG])dnl
AC_REQUIRE([AC_PROG_NM])dnl

AC_REQUIRE([AC_PROG_LN_S])dnl
AC_REQUIRE([AC_DEPLIBS_CHECK_METHOD])dnl
# Autoconf 2.13's AC_OBJEXT and AC_EXEEXT macros only works for C compilers!
AC_REQUIRE([AC_OBJEXT])dnl
AC_REQUIRE([AC_EXEEXT])dnl
dnl

AC_LIBTOOL_SYS_MAX_CMD_LEN
AC_LIBTOOL_SYS_GLOBAL_SYMBOL_PIPE
AC_LIBTOOL_OBJDIR

AC_REQUIRE([_LT_AC_SYS_COMPILER])dnl
_LT_AC_PROG_ECHO_BACKSLASH

case $host_os in
aix3*)
  # AIX sometimes has problems with the GCC collect2 program.  For some
  # reason, if we set the COLLECT_NAMES environment variable, the problems
  # vanish in a puff of smoke.
  if test "X${COLLECT_NAMES+set}" != Xset; then
    COLLECT_NAMES=
    export COLLECT_NAMES
  fi
  ;;
esac

# Sed substitution that helps us do robust quoting.  It backslashifies
# metacharacters that are still active within double-quoted strings.
Xsed='sed -e s/^X//'
[sed_quote_subst='s/\([\\"\\`$\\\\]\)/\\\1/g']

# Same as above, but do not quote variable references.
[double_quote_subst='s/\([\\"\\`\\\\]\)/\\\1/g']

# Sed substitution to delay expansion of an escaped shell variable in a
# double_quote_subst'ed string.
delay_variable_subst='s/\\\\\\\\\\\$/\\\\\\$/g'

# Sed substitution to undo escaping of the cmd sep variable
unescape_variable_subst='s/\\\(${_S_}\)/\1/g'

# Sed substitution to avoid accidental globbing in evaled expressions
no_glob_subst='s/\*/\\\*/g'

# Constants:
rm="rm -f"

# Global variables:
default_ofile=libtool
can_build_shared=yes

# All known linkers require a `.a' archive for static linking (except M$VC,
# which needs '.lib').
libext=a
ltmain="$ac_aux_dir/ltmain.sh"
ofile="$default_ofile"
with_gnu_ld="$lt_cv_prog_gnu_ld"

AC_CHECK_TOOL(AR, ar, false)
AC_CHECK_TOOL(RANLIB, ranlib, :)
AC_CHECK_TOOL(STRIP, strip, :)

old_CC="$CC"
old_CFLAGS="$CFLAGS"

# Set sane defaults for various variables
test -z "$AR" && AR=ar
test -z "$AR_FLAGS" && AR_FLAGS=cru
test -z "$AS" && AS=as
test -z "$CC" && CC=cc
test -z "$LTCC" && LTCC=$CC
test -z "$DLLTOOL" && DLLTOOL=dlltool
test -z "$LD" && LD=ld
test -z "$LN_S" && LN_S="ln -s"
test -z "$MAGIC_CMD" && MAGIC_CMD=file
test -z "$NM" && NM=nm
test -z "$SED" && SED=sed
test -z "$OBJDUMP" && OBJDUMP=objdump
test -z "$RANLIB" && RANLIB=:
test -z "$STRIP" && STRIP=:
test -z "$ac_objext" && ac_objext=o

# Determine commands to create old-style static archives.
old_archive_cmds='$AR $AR_FLAGS $oldlib$oldobjs$old_deplibs'
old_postinstall_cmds='chmod 644 $oldlib'
old_postuninstall_cmds=

if test -n "$RANLIB"; then
  case $host_os in
  openbsd*)
    old_postinstall_cmds="\$RANLIB -t \$oldlib\${_S_}$old_postinstall_cmds"
    ;;
  *)
    old_postinstall_cmds="\$RANLIB \$oldlib\${_S_}$old_postinstall_cmds"
    ;;
  esac
  old_archive_cmds="$old_archive_cmds\${_S_}\$RANLIB \$oldlib"
fi

# Only perform the check for file, if the check method requires it
case $deplibs_check_method in
file_magic*)
  if test "$file_magic_cmd" = '$MAGIC_CMD'; then
    AC_PATH_MAGIC
  fi
  ;;
esac

AC_PROVIDE_IFELSE([AC_LIBTOOL_DLOPEN], enable_dlopen=yes, enable_dlopen=no)
AC_PROVIDE_IFELSE([AC_LIBTOOL_WIN32_DLL],
enable_win32_dll=yes, enable_win32_dll=no)

AC_ARG_ENABLE([libtool-lock],
    [AC_HELP_STRING([--disable-libtool-lock],
	[avoid locking (might break parallel builds)])])
test "x$enable_libtool_lock" != xno && enable_libtool_lock=yes

AC_ARG_WITH([pic],
    [AC_HELP_STRING([--with-pic],
	[try to use only PIC/non-PIC objects @<:@default=use both@:>@])],
    [pic_mode="$withval"],
    [pic_mode=default])
test -z "$pic_mode" && pic_mode=default

# Use C for the default configuration in the libtool script
tagname=
AC_LIBTOOL_LANG_C_CONFIG
_LT_AC_TAGCONFIG
])# AC_LIBTOOL_SETUP


# _LT_AC_SYS_COMPILER
# -------------------
AC_DEFUN([_LT_AC_SYS_COMPILER],
[AC_REQUIRE([AC_PROG_CC])dnl

# If no C compiler was specified, use CC.
LTCC=${LTCC-"$CC"}

# Allow CC to be a program name with arguments.
compiler=$CC
])# _LT_AC_SYS_COMPILER


# _LT_AC_SYS_LIBPATH_AIX
# ----------------------
# Links a minimal program and checks the executable
# for the system default hardcoded library path. In most cases,
# this is /usr/lib:/lib, but when the MPI compilers are used
# the location of the communication and MPI libs are included too.
# If we don't find anything, use the default library path according
# to the aix ld manual.
AC_DEFUN([_LT_AC_SYS_LIBPATH_AIX],
[AC_LINK_IFELSE(AC_LANG_PROGRAM,[
aix_libpath=`dump -H conftest$ac_exeext 2>/dev/null | $SED -n -e '/Import File Strings/,/^$/ { /^0/ { s/^0  *\(.*\)$/\1/; p; }
}'`
# Check for a 64-bit object if we didn't find anything.
if test -z "$aix_libpath"; then aix_libpath=`dump -HX64 conftest$ac_exeext 2>/dev/null | $SED -n -e '/Import File Strings/,/^$/ { /^0/ { s/^0  *\(.*\)$/\1/; p; }
}'`; fi],[])
if test -z "$aix_libpath"; then aix_libpath="/usr/lib:/lib"; fi
])# _LT_AC_SYS_LIBPATH_AIX


# _LT_AC_SHELL_INIT(ARG)
# ----------------------
AC_DEFUN([_LT_AC_SHELL_INIT],
[ifdef([AC_DIVERSION_NOTICE],
	     [AC_DIVERT_PUSH(AC_DIVERSION_NOTICE)],
	 [AC_DIVERT_PUSH(NOTICE)])
$1
AC_DIVERT_POP
])# _LT_AC_SHELL_INIT


# _LT_AC_PROG_ECHO_BACKSLASH
# --------------------------
# Add some code to the start of the generated configure script which
# will find an echo command which doesn't interpret backslashes.
AC_DEFUN([_LT_AC_PROG_ECHO_BACKSLASH],
[_LT_AC_SHELL_INIT([
# Check that we are running under the correct shell.
SHELL=${CONFIG_SHELL-/bin/sh}

case X$ECHO in
X*--fallback-echo)
  # Remove one level of quotation (which was required for Make).
  ECHO=`echo "$ECHO" | sed 's,\\\\\[$]\\[$]0,'[$]0','`
  ;;
esac

echo=${ECHO-echo}
if test "X[$]1" = X--no-reexec; then
  # Discard the --no-reexec flag, and continue.
  shift
elif test "X[$]1" = X--fallback-echo; then
  # Avoid inline document here, it may be left over
  :
elif test "X`($echo '\t') 2>/dev/null`" = 'X\t' ; then
  # Yippee, $echo works!
  :
else
  # Restart under the correct shell.
  exec $SHELL "[$]0" --no-reexec ${1+"[$]@"}
fi

if test "X[$]1" = X--fallback-echo; then
  # used as fallback echo
  shift
  cat <<EOF
[$]*
EOF
  exit 0
fi

# The HP-UX ksh and POSIX shell print the target directory to stdout
# if CDPATH is set.
if test "X${CDPATH+set}" = Xset; then CDPATH=:; export CDPATH; fi

if test -z "$ECHO"; then
if test "X${echo_test_string+set}" != Xset; then
# find a string as large as possible, as long as the shell can cope with it
  for cmd in 'sed 50q "[$]0"' 'sed 20q "[$]0"' 'sed 10q "[$]0"' 'sed 2q "[$]0"' 'echo test'; do
    # expected sizes: less than 2Kb, 1Kb, 512 bytes, 16 bytes, ...
    if (echo_test_string="`eval $cmd`") 2>/dev/null &&
       echo_test_string="`eval $cmd`" &&
       (test "X$echo_test_string" = "X$echo_test_string") 2>/dev/null
    then
      break
    fi
  done
fi

if test "X`($echo '\t') 2>/dev/null`" = 'X\t' &&
   echo_testing_string=`($echo "$echo_test_string") 2>/dev/null` &&
   test "X$echo_testing_string" = "X$echo_test_string"; then
  :
else
  # The Solaris, AIX, and Digital Unix default echo programs unquote
  # backslashes.  This makes it impossible to quote backslashes using
  #   echo "$something" | sed 's/\\/\\\\/g'
  #
  # So, first we look for a working echo in the user's PATH.

  lt_save_ifs="$IFS"; IFS=$PATH_SEPARATOR
  for dir in $PATH /usr/ucb; do
    IFS="$lt_save_ifs"
    if (test -f $dir/echo || test -f $dir/echo$ac_exeext) &&
       test "X`($dir/echo '\t') 2>/dev/null`" = 'X\t' &&
       echo_testing_string=`($dir/echo "$echo_test_string") 2>/dev/null` &&
       test "X$echo_testing_string" = "X$echo_test_string"; then
      echo="$dir/echo"
      break
    fi
  done
  IFS="$lt_save_ifs"

  if test "X$echo" = Xecho; then
    # We didn't find a better echo, so look for alternatives.
    if test "X`(print -r '\t') 2>/dev/null`" = 'X\t' &&
       echo_testing_string=`(print -r "$echo_test_string") 2>/dev/null` &&
       test "X$echo_testing_string" = "X$echo_test_string"; then
      # This shell has a builtin print -r that does the trick.
      echo='print -r'
    elif (test -f /bin/ksh || test -f /bin/ksh$ac_exeext) &&
	 test "X$CONFIG_SHELL" != X/bin/ksh; then
      # If we have ksh, try running configure again with it.
      ORIGINAL_CONFIG_SHELL=${CONFIG_SHELL-/bin/sh}
      export ORIGINAL_CONFIG_SHELL
      CONFIG_SHELL=/bin/ksh
      export CONFIG_SHELL
      exec $CONFIG_SHELL "[$]0" --no-reexec ${1+"[$]@"}
    else
      # Try using printf.
      echo='printf %s\n'
      if test "X`($echo '\t') 2>/dev/null`" = 'X\t' &&
	 echo_testing_string=`($echo "$echo_test_string") 2>/dev/null` &&
	 test "X$echo_testing_string" = "X$echo_test_string"; then
	# Cool, printf works
	:
      elif echo_testing_string=`($ORIGINAL_CONFIG_SHELL "[$]0" --fallback-echo '\t') 2>/dev/null` &&
	   test "X$echo_testing_string" = 'X\t' &&
	   echo_testing_string=`($ORIGINAL_CONFIG_SHELL "[$]0" --fallback-echo "$echo_test_string") 2>/dev/null` &&
	   test "X$echo_testing_string" = "X$echo_test_string"; then
	CONFIG_SHELL=$ORIGINAL_CONFIG_SHELL
	export CONFIG_SHELL
	SHELL="$CONFIG_SHELL"
	export SHELL
	echo="$CONFIG_SHELL [$]0 --fallback-echo"
      elif echo_testing_string=`($CONFIG_SHELL "[$]0" --fallback-echo '\t') 2>/dev/null` &&
	   test "X$echo_testing_string" = 'X\t' &&
	   echo_testing_string=`($CONFIG_SHELL "[$]0" --fallback-echo "$echo_test_string") 2>/dev/null` &&
	   test "X$echo_testing_string" = "X$echo_test_string"; then
	echo="$CONFIG_SHELL [$]0 --fallback-echo"
      else
	# maybe with a smaller string...
	prev=:

	for cmd in 'echo test' 'sed 2q "[$]0"' 'sed 10q "[$]0"' 'sed 20q "[$]0"' 'sed 50q "[$]0"'; do
	  if (test "X$echo_test_string" = "X`eval $cmd`") 2>/dev/null
	  then
	    break
	  fi
	  prev="$cmd"
	done

	if test "$prev" != 'sed 50q "[$]0"'; then
	  echo_test_string=`eval $prev`
	  export echo_test_string
	  exec ${ORIGINAL_CONFIG_SHELL-${CONFIG_SHELL-/bin/sh}} "[$]0" ${1+"[$]@"}
	else
	  # Oops.  We lost completely, so just stick with echo.
	  echo=echo
	fi
      fi
    fi
  fi
fi
fi

# Copy echo and quote the copy suitably for passing to libtool from
# the Makefile, instead of quoting the original, which is used later.
ECHO=$echo
if test "X$ECHO" = "X$CONFIG_SHELL [$]0 --fallback-echo"; then
   ECHO="$CONFIG_SHELL \\\$\[$]0 --fallback-echo"
fi

AC_SUBST(ECHO)
])])# _LT_AC_PROG_ECHO_BACKSLASH


# _LT_AC_LOCK
# -----------
AC_DEFUN([_LT_AC_LOCK],
[AC_ARG_ENABLE([libtool-lock],
    [AC_HELP_STRING([--disable-libtool-lock],
	[avoid locking (might break parallel builds)])])
test "x$enable_libtool_lock" != xno && enable_libtool_lock=yes

# Some flags need to be propagated to the compiler or linker for good
# libtool support.
case $host in
ia64-*-hpux*)
  # Find out which ABI we are using.
  echo 'int i;' > conftest.$ac_ext
  if AC_TRY_EVAL(ac_compile); then
    case `/usr/bin/file conftest.$ac_objext` in
    *ELF-32*)
      HPUX_IA64_MODE="32"
      ;;
    *ELF-64*)
      HPUX_IA64_MODE="64"
      ;;
    esac
  fi
  rm -rf conftest*
  ;;
*-*-irix6*)
  # Find out which ABI we are using.
  echo '[#]line __oline__ "configure"' > conftest.$ac_ext
  if AC_TRY_EVAL(ac_compile); then
   if test "$lt_cv_prog_gnu_ld" = yes; then
    case `/usr/bin/file conftest.$ac_objext` in
    *32-bit*)
      LD="${LD-ld} -melf32bsmip"
      ;;
    *N32*)
      LD="${LD-ld} -melf32bmipn32"
      ;;
    *64-bit*)
      LD="${LD-ld} -melf64bmip"
      ;;
    esac
   else
    case `/usr/bin/file conftest.$ac_objext` in
    *32-bit*)
      LD="${LD-ld} -32"
      ;;
    *N32*)
      LD="${LD-ld} -n32"
      ;;
    *64-bit*)
      LD="${LD-ld} -64"
      ;;
    esac
   fi
  fi
  rm -rf conftest*
  ;;

x86_64-*linux*|ppc*-*linux*|powerpc*-*linux*|s390*-*linux*|sparc*-*linux*)
  # Find out which ABI we are using.
  echo 'int i;' > conftest.$ac_ext
  if AC_TRY_EVAL(ac_compile); then
    case "`/usr/bin/file conftest.o`" in
    *32-bit*)
      case $host in
        x86_64-*linux*)
          LD="${LD-ld} -m elf_i386"
          ;;
        ppc64-*linux*)
          LD="${LD-ld} -m elf32ppclinux"
          ;;
        s390x-*linux*)
          LD="${LD-ld} -m elf_s390"
          ;;
        sparc64-*linux*)
          LD="${LD-ld} -m elf32_sparc"
          ;;
      esac
      ;;
    *64-bit*)
      case $host in
        x86_64-*linux*)
          LD="${LD-ld} -m elf_x86_64"
          ;;
        ppc*-*linux*|powerpc*-*linux*)
          LD="${LD-ld} -m elf64ppc"
          ;;
        s390*-*linux*)
          LD="${LD-ld} -m elf64_s390"
          ;;
        sparc*-*linux*)
          LD="${LD-ld} -m elf64_sparc"
          ;;
      esac
      ;;
    esac
  fi
  rm -rf conftest*
  ;;

*-*-sco3.2v5*)
  # On SCO OpenServer 5, we need -belf to get full-featured binaries.
  SAVE_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS -belf"
  AC_CACHE_CHECK([whether the C compiler needs -belf], lt_cv_cc_needs_belf,
    [AC_LANG_PUSH(C)
     AC_TRY_LINK([],[],[lt_cv_cc_needs_belf=yes],[lt_cv_cc_needs_belf=no])
     AC_LANG_POP])
  if test x"$lt_cv_cc_needs_belf" != x"yes"; then
    # this is probably gcc 2.8.0, egcs 1.0 or newer; no need for -belf
    CFLAGS="$SAVE_CFLAGS"
  fi
  ;;
AC_PROVIDE_IFELSE([AC_LIBTOOL_WIN32_DLL],
[*-*-cygwin* | *-*-mingw* | *-*-pw32*)
  AC_CHECK_TOOL(DLLTOOL, dlltool, false)
  AC_CHECK_TOOL(AS, as, false)
  AC_CHECK_TOOL(OBJDUMP, objdump, false)
  ;;
  ])
esac

need_locks="$enable_libtool_lock"

])# _LT_AC_LOCK


# AC_LIBTOOL_COMPILER_OPTION(MESSAGE, VARIABLE-NAME, FLAGS,
#		[OUTPUT-FILE], [ACTION-SUCCESS], [ACTION-FAILURE])
# ----------------------------------------------------------------
# Check whether the given compiler option works
AC_DEFUN([AC_LIBTOOL_COMPILER_OPTION],
[AC_CACHE_CHECK([$1], [$2],
  [$2=no
  ifelse([$4], , [ac_outfile=conftest.$ac_objext], [ac_outfile=$4])
   printf "$lt_simple_compile_test_code" > conftest.$ac_ext
   lt_compiler_flag="$3"
   # Insert the option either (1) after the last *FLAGS variable, or
   # (2) before a word containing "conftest.", or (3) at the end.
   # Note that $ac_compile itself does not contain backslashes and begins
   # with a dollar sign (not a hyphen), so the echo should work correctly.
   # The option is referenced via a variable to avoid confusing sed.
   lt_compile=`echo "$ac_compile" | $SED \
   -e 's:.*FLAGS}? :&$lt_compiler_flag :; t' \
   -e 's: [[^ ]]*conftest\.: $lt_compiler_flag&:; t' \
   -e 's:$: $lt_compiler_flag:'`
   (eval echo "\"\$as_me:__oline__: $lt_compile\"" >&AS_MESSAGE_LOG_FD)
   (eval "$lt_compile" 2>conftest.err)
   ac_status=$?
   cat conftest.err >&AS_MESSAGE_LOG_FD
   echo "$as_me:__oline__: \$? = $ac_status" >&AS_MESSAGE_LOG_FD
   if (exit $ac_status) && test -s "$ac_outfile"; then
     # The compiler can only warn and ignore the option if not recognized
     # So say no if there are warnings
     if test ! -s conftest.err; then
       $2=yes
     fi
   fi
   $rm conftest*
])

if test x"[$]$2" = xyes; then
    ifelse([$5], , :, [$5])
else
    ifelse([$6], , :, [$6])
fi
])# AC_LIBTOOL_COMPILER_OPTION


# AC_LIBTOOL_LINKER_OPTION(MESSAGE, VARIABLE-NAME, FLAGS,
#                          [ACTION-SUCCESS], [ACTION-FAILURE])
# ------------------------------------------------------------
# Check whether the given compiler option works
AC_DEFUN([AC_LIBTOOL_LINKER_OPTION],
[AC_CACHE_CHECK([$1], [$2],
  [$2=no
   save_LDFLAGS="$LDFLAGS"
   LDFLAGS="$LDFLAGS $3"
   printf "$lt_simple_link_test_code" > conftest.$ac_ext
   if (eval $ac_link 2>conftest.err) && test -s conftest$ac_exeext; then
     # The compiler can only warn and ignore the option if not recognized
     # So say no if there are warnings
     if test -s conftest.err; then
       # Append any errors to the config.log.
       cat conftest.err 1>&AS_MESSAGE_LOG_FD
     else
       $2=yes
     fi
   fi
   $rm conftest*
   LDFLAGS="$save_LDFLAGS"
])

if test x"[$]$2" = xyes; then
    ifelse([$4], , :, [$4])
else
    ifelse([$5], , :, [$5])
fi
])# AC_LIBTOOL_LINKER_OPTION


# AC_LIBTOOL_SYS_MAX_CMD_LEN
# --------------------------
AC_DEFUN([AC_LIBTOOL_SYS_MAX_CMD_LEN],
[# find the maximum length of command line arguments
AC_MSG_CHECKING([the maximum length of command line arguments])
AC_CACHE_VAL([lt_cv_sys_max_cmd_len], [dnl
  i=0
  testring="ABCD"

  case $build_os in
  msdosdjgpp*)
    # On DJGPP, this test can blow up pretty badly due to problems in libc
    # (any single argument exceeding 2000 bytes causes a buffer overrun
    # during glob expansion).  Even if it were fixed, the result of this
    # check would be larger than it should be.
    lt_cv_sys_max_cmd_len=12288;    # 12K is about right
    ;;

  gnu*)
    # Under GNU Hurd, this test is not required because there is
    # no limit to the length of command line arguments.
    # Libtool will interpret -1 as no limit whatsoever
    lt_cv_sys_max_cmd_len=-1;
    ;;

  cygwin* | mingw*)
    # On Win9x/ME, this test blows up -- it succeeds, but takes
    # about 5 minutes as the teststring grows exponentially.
    # Worse, since 9x/ME are not pre-emptively multitasking,
    # you end up with a "frozen" computer, even though with patience
    # the test eventually succeeds (with a max line length of 256k).
    # Instead, let's just punt: use the minimum linelength reported by
    # all of the supported platforms: 8192 (on NT/2K/XP).
    lt_cv_sys_max_cmd_len=8192;
    ;;

 *)
    # If test is not a shell built-in, we'll probably end up computing a
    # maximum length that is only half of the actual maximum length, but
    # we can't tell.
    while (test "X"`$CONFIG_SHELL [$]0 --fallback-echo "X$testring" 2>/dev/null` \
	       = "XX$testring") >/dev/null 2>&1 &&
	    new_result=`expr "X$testring" : ".*" 2>&1` &&
	    lt_cv_sys_max_cmd_len=$new_result &&
	    test $i != 17 # 1/2 MB should be enough
    do
      i=`expr $i + 1`
      testring=$testring$testring
    done
    testring=
    # Add a significant safety factor because C++ compilers can tack on massive
    # amounts of additional arguments before passing them to the linker.
    # It appears as though 1/2 is a usable value.
    lt_cv_sys_max_cmd_len=`expr $lt_cv_sys_max_cmd_len \/ 2`
    ;;
  esac
])
if test -n $lt_cv_sys_max_cmd_len ; then
  AC_MSG_RESULT($lt_cv_sys_max_cmd_len)
else
  AC_MSG_RESULT(none)
fi
])# AC_LIBTOOL_SYS_MAX_CMD_LEN


# _LT_AC_CHECK_DLFCN
# --------------------
AC_DEFUN([_LT_AC_CHECK_DLFCN],
[AC_CHECK_HEADERS(dlfcn.h)dnl
])# _LT_AC_CHECK_DLFCN


# _LT_AC_TRY_DLOPEN_SELF (ACTION-IF-TRUE, ACTION-IF-TRUE-W-USCORE,
#                           ACTION-IF-FALSE, ACTION-IF-CROSS-COMPILING)
# ------------------------------------------------------------------
AC_DEFUN([_LT_AC_TRY_DLOPEN_SELF],
[AC_REQUIRE([_LT_AC_CHECK_DLFCN])dnl
if test "$cross_compiling" = yes; then :
  [$4]
else
  lt_dlunknown=0; lt_dlno_uscore=1; lt_dlneed_uscore=2
  lt_status=$lt_dlunknown
  cat > conftest.$ac_ext <<EOF
[#line __oline__ "configure"
#include "confdefs.h"

#if HAVE_DLFCN_H
#include <dlfcn.h>
#endif

#include <stdio.h>

#ifdef RTLD_GLOBAL
#  define LT_DLGLOBAL		RTLD_GLOBAL
#else
#  ifdef DL_GLOBAL
#    define LT_DLGLOBAL		DL_GLOBAL
#  else
#    define LT_DLGLOBAL		0
#  endif
#endif

/* We may have to define LT_DLLAZY_OR_NOW in the command line if we
   find out it does not work in some platform. */
#ifndef LT_DLLAZY_OR_NOW
#  ifdef RTLD_LAZY
#    define LT_DLLAZY_OR_NOW		RTLD_LAZY
#  else
#    ifdef DL_LAZY
#      define LT_DLLAZY_OR_NOW		DL_LAZY
#    else
#      ifdef RTLD_NOW
#        define LT_DLLAZY_OR_NOW	RTLD_NOW
#      else
#        ifdef DL_NOW
#          define LT_DLLAZY_OR_NOW	DL_NOW
#        else
#          define LT_DLLAZY_OR_NOW	0
#        endif
#      endif
#    endif
#  endif
#endif

#ifdef __cplusplus
extern "C" void exit (int);
#endif

void fnord() { int i=42;}
int main ()
{
  void *self = dlopen (0, LT_DLGLOBAL|LT_DLLAZY_OR_NOW);
  int status = $lt_dlunknown;

  if (self)
    {
      if (dlsym (self,"fnord"))       status = $lt_dlno_uscore;
      else if (dlsym( self,"_fnord")) status = $lt_dlneed_uscore;
      /* dlclose (self); */
    }

    exit (status);
}]
EOF
  if AC_TRY_EVAL(ac_link) && test -s conftest${ac_exeext} 2>/dev/null; then
    (./conftest; exit; ) 2>/dev/null
    lt_status=$?
    case x$lt_status in
      x$lt_dlno_uscore) $1 ;;
      x$lt_dlneed_uscore) $2 ;;
      x$lt_unknown|x*) $3 ;;
    esac
  else :
    # compilation failed
    $3
  fi
fi
rm -fr conftest*
])# _LT_AC_TRY_DLOPEN_SELF


# AC_LIBTOOL_DLOPEN_SELF
# -------------------
AC_DEFUN([AC_LIBTOOL_DLOPEN_SELF],
[AC_REQUIRE([_LT_AC_CHECK_DLFCN])dnl
if test "x$enable_dlopen" != xyes; then
  enable_dlopen=unknown
  enable_dlopen_self=unknown
  enable_dlopen_self_static=unknown
else
  lt_cv_dlopen=no
  lt_cv_dlopen_libs=

  case $host_os in
  beos*)
    lt_cv_dlopen="load_add_on"
    lt_cv_dlopen_libs=
    lt_cv_dlopen_self=yes
    ;;

  mingw* | pw32*)
    lt_cv_dlopen="LoadLibrary"
    lt_cv_dlopen_libs=
   ;;

  cygwin*)
    lt_cv_dlopen="dlopen"
    lt_cv_dlopen_libs=
   ;;

  darwin*)
  # if libdl is installed we need to link against it
    AC_CHECK_LIB([dl], [dlopen],
		[lt_cv_dlopen="dlopen" lt_cv_dlopen_libs="-ldl"],[
    lt_cv_dlopen="dyld"
    lt_cv_dlopen_libs=
    lt_cv_dlopen_self=yes
    ])
   ;;
    
  *)
    AC_CHECK_FUNC([shl_load],
	  [lt_cv_dlopen="shl_load"],
      [AC_CHECK_LIB([dld], [shl_load],
	    [lt_cv_dlopen="shl_load" lt_cv_dlopen_libs="-dld"],
	[AC_CHECK_FUNC([dlopen],
	      [lt_cv_dlopen="dlopen"],
	  [AC_CHECK_LIB([dl], [dlopen],
		[lt_cv_dlopen="dlopen" lt_cv_dlopen_libs="-ldl"],
	    [AC_CHECK_LIB([svld], [dlopen],
		  [lt_cv_dlopen="dlopen" lt_cv_dlopen_libs="-lsvld"],
	      [AC_CHECK_LIB([dld], [dld_link],
		    [lt_cv_dlopen="dld_link" lt_cv_dlopen_libs="-dld"])
	      ])
	    ])
	  ])
	])
      ])
    ;;
  esac

  if test "x$lt_cv_dlopen" != xno; then
    enable_dlopen=yes
  else
    enable_dlopen=no
  fi

  case $lt_cv_dlopen in
  dlopen)
    save_CPPFLAGS="$CPPFLAGS"
    test "x$ac_cv_header_dlfcn_h" = xyes && CPPFLAGS="$CPPFLAGS -DHAVE_DLFCN_H"

    save_LDFLAGS="$LDFLAGS"
    eval LDFLAGS=\"\$LDFLAGS $export_dynamic_flag_spec\"

    save_LIBS="$LIBS"
    LIBS="$lt_cv_dlopen_libs $LIBS"

    AC_CACHE_CHECK([whether a program can dlopen itself],
	  lt_cv_dlopen_self, [dnl
	  _LT_AC_TRY_DLOPEN_SELF(
	    lt_cv_dlopen_self=yes, lt_cv_dlopen_self=yes,
	    lt_cv_dlopen_self=no, lt_cv_dlopen_self=cross)
    ])

    if test "x$lt_cv_dlopen_self" = xyes; then
      LDFLAGS="$LDFLAGS $link_static_flag"
      AC_CACHE_CHECK([whether a statically linked program can dlopen itself],
    	  lt_cv_dlopen_self_static, [dnl
	  _LT_AC_TRY_DLOPEN_SELF(
	    lt_cv_dlopen_self_static=yes, lt_cv_dlopen_self_static=yes,
	    lt_cv_dlopen_self_static=no,  lt_cv_dlopen_self_static=cross)
      ])
    fi

    CPPFLAGS="$save_CPPFLAGS"
    LDFLAGS="$save_LDFLAGS"
    LIBS="$save_LIBS"
    ;;
  esac

  case $lt_cv_dlopen_self in
  yes|no) enable_dlopen_self=$lt_cv_dlopen_self ;;
  *) enable_dlopen_self=unknown ;;
  esac

  case $lt_cv_dlopen_self_static in
  yes|no) enable_dlopen_self_static=$lt_cv_dlopen_self_static ;;
  *) enable_dlopen_self_static=unknown ;;
  esac
fi
])# AC_LIBTOOL_DLOPEN_SELF


# AC_LIBTOOL_PROG_CC_C_O([TAGNAME])
# ---------------------------------
# Check to see if options -c and -o are simultaneously supported by compiler
AC_DEFUN([AC_LIBTOOL_PROG_CC_C_O],
[AC_REQUIRE([_LT_AC_SYS_COMPILER])dnl
AC_CACHE_CHECK([if $compiler supports -c -o file.$ac_objext],
  [_LT_AC_TAGVAR(lt_cv_prog_compiler_c_o, $1)],
  [_LT_AC_TAGVAR(lt_cv_prog_compiler_c_o, $1)=no
   $rm -r conftest 2>/dev/null
   mkdir conftest
   cd conftest
   mkdir out
   printf "$lt_simple_compile_test_code" > conftest.$ac_ext

   # According to Tom Tromey, Ian Lance Taylor reported there are C compilers
   # that will create temporary files in the current directory regardless of
   # the output directory.  Thus, making CWD read-only will cause this test
   # to fail, enabling locking or at least warning the user not to do parallel
   # builds.
   chmod -w .

   lt_compiler_flag="-o out/conftest2.$ac_objext"
   # Insert the option either (1) after the last *FLAGS variable, or
   # (2) before a word containing "conftest.", or (3) at the end.
   # Note that $ac_compile itself does not contain backslashes and begins
   # with a dollar sign (not a hyphen), so the echo should work correctly.
   lt_compile=`echo "$ac_compile" | $SED \
   -e 's:.*FLAGS}? :&$lt_compiler_flag :; t' \
   -e 's: [[^ ]]*conftest\.: $lt_compiler_flag&:; t' \
   -e 's:$: $lt_compiler_flag:'`
   (eval echo "\"\$as_me:__oline__: $lt_compile\"" >&AS_MESSAGE_LOG_FD)
   (eval "$lt_compile" 2>out/conftest.err)
   ac_status=$?
   cat out/conftest.err >&AS_MESSAGE_LOG_FD
   echo "$as_me:__oline__: \$? = $ac_status" >&AS_MESSAGE_LOG_FD
   if (exit $ac_status) && test -s out/conftest2.$ac_objext
   then
     # The compiler can only warn and ignore the option if not recognized
     # So say no if there are warnings
     if test ! -s out/conftest.err; then
       _LT_AC_TAGVAR(lt_cv_prog_compiler_c_o, $1)=yes
     fi
   fi
   chmod u+w .
   $rm conftest* out/*
   rmdir out
   cd ..
   rmdir conftest
   $rm conftest*
])
])# AC_LIBTOOL_PROG_CC_C_O


# AC_LIBTOOL_SYS_HARD_LINK_LOCKS([TAGNAME])
# -----------------------------------------
# Check to see if we can do hard links to lock some files if needed
AC_DEFUN([AC_LIBTOOL_SYS_HARD_LINK_LOCKS],
[AC_REQUIRE([_LT_AC_LOCK])dnl

hard_links="nottested"
if test "$_LT_AC_TAGVAR(lt_cv_prog_compiler_c_o, $1)" = no && test "$need_locks" != no; then
  # do not overwrite the value of need_locks provided by the user
  AC_MSG_CHECKING([if we can lock with hard links])
  hard_links=yes
  $rm conftest*
  ln conftest.a conftest.b 2>/dev/null && hard_links=no
  touch conftest.a
  ln conftest.a conftest.b 2>&5 || hard_links=no
  ln conftest.a conftest.b 2>/dev/null && hard_links=no
  AC_MSG_RESULT([$hard_links])
  if test "$hard_links" = no; then
    AC_MSG_WARN([`$CC' does not support `-c -o', so `make -j' may be unsafe])
    need_locks=warn
  fi
else
  need_locks=no
fi
])# AC_LIBTOOL_SYS_HARD_LINK_LOCKS


# AC_LIBTOOL_OBJDIR
# -----------------
AC_DEFUN([AC_LIBTOOL_OBJDIR],
[AC_CACHE_CHECK([for objdir], [lt_cv_objdir],
[rm -f .libs 2>/dev/null
mkdir .libs 2>/dev/null
if test -d .libs; then
  lt_cv_objdir=.libs
else
  # MS-DOS does not allow filenames that begin with a dot.
  lt_cv_objdir=_libs
fi
rmdir .libs 2>/dev/null])
objdir=$lt_cv_objdir
])# AC_LIBTOOL_OBJDIR


# AC_LIBTOOL_PROG_LD_HARDCODE_LIBPATH([TAGNAME])
# ----------------------------------------------
# Check hardcoding attributes.
AC_DEFUN([AC_LIBTOOL_PROG_LD_HARDCODE_LIBPATH],
[AC_MSG_CHECKING([how to hardcode library paths into programs])
_LT_AC_TAGVAR(hardcode_action, $1)=
if test -n "$_LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)" || \
   test -n "$_LT_AC_TAGVAR(runpath_var $1)" || \
   test "X$_LT_AC_TAGVAR(hardcode_automatic, $1)"="Xyes" ; then

  # We can hardcode non-existant directories.
  if test "$_LT_AC_TAGVAR(hardcode_direct, $1)" != no &&
     # If the only mechanism to avoid hardcoding is shlibpath_var, we
     # have to relink, otherwise we might link with an installed library
     # when we should be linking with a yet-to-be-installed one
     ## test "$_LT_AC_TAGVAR(hardcode_shlibpath_var, $1)" != no &&
     test "$_LT_AC_TAGVAR(hardcode_minus_L, $1)" != no; then
    # Linking always hardcodes the temporary library directory.
    _LT_AC_TAGVAR(hardcode_action, $1)=relink
  else
    # We can link without hardcoding, and we can hardcode nonexisting dirs.
    _LT_AC_TAGVAR(hardcode_action, $1)=immediate
  fi
else
  # We cannot hardcode anything, or else we can only hardcode existing
  # directories.
  _LT_AC_TAGVAR(hardcode_action, $1)=unsupported
fi
AC_MSG_RESULT([$_LT_AC_TAGVAR(hardcode_action, $1)])

if test "$_LT_AC_TAGVAR(hardcode_action, $1)" = relink; then
  # Fast installation is not supported
  enable_fast_install=no
elif test "$shlibpath_overrides_runpath" = yes ||
     test "$enable_shared" = no; then
  # Fast installation is not necessary
  enable_fast_install=needless
fi
])# AC_LIBTOOL_PROG_LD_HARDCODE_LIBPATH


# AC_LIBTOOL_SYS_LIB_STRIP
# ------------------------
AC_DEFUN([AC_LIBTOOL_SYS_LIB_STRIP],
[striplib=
old_striplib=
AC_MSG_CHECKING([whether stripping libraries is possible])
if test -n "$STRIP" && $STRIP -V 2>&1 | grep "GNU strip" >/dev/null; then
  test -z "$old_striplib" && old_striplib="$STRIP --strip-debug"
  test -z "$striplib" && striplib="$STRIP --strip-unneeded"
  AC_MSG_RESULT([yes])
else
# FIXME - insert some real tests, host_os isn't really good enough
  case $host_os in
   NOT-darwin*)
       if test -n "$STRIP" ; then
         striplib="$STRIP -x"
         AC_MSG_RESULT([yes])
       else
  AC_MSG_RESULT([no])
fi
       ;;
   *)
  AC_MSG_RESULT([no])
    ;;
  esac  
fi
])# AC_LIBTOOL_SYS_LIB_STRIP


# AC_LIBTOOL_SYS_DYNAMIC_LINKER
# -----------------------------
# PORTME Fill in your ld.so characteristics
AC_DEFUN([AC_LIBTOOL_SYS_DYNAMIC_LINKER],
[AC_MSG_CHECKING([dynamic linker characteristics])
library_names_spec=
libname_spec='lib$name'
soname_spec=
shrext=".so"
postinstall_cmds=
postuninstall_cmds=
finish_cmds=
finish_eval=
shlibpath_var=
shlibpath_overrides_runpath=unknown
version_type=none
dynamic_linker="$host_os ld.so"
sys_lib_dlsearch_path_spec="/lib /usr/lib"
if test "$GCC" = yes; then
  sys_lib_search_path_spec=`$CC -print-search-dirs | grep "^libraries:" | $SED -e "s/^libraries://" -e "s,=/,/,g"`
  if echo "$sys_lib_search_path_spec" | grep ';' >/dev/null ; then
    # if the path contains ";" then we assume it to be the separator
    # otherwise default to the standard path separator (i.e. ":") - it is
    # assumed that no part of a normal pathname contains ";" but that should
    # okay in the real world where ";" in dirpaths is itself problematic.
    sys_lib_search_path_spec=`echo "$sys_lib_search_path_spec" | $SED -e 's/;/ /g'`
  else
    sys_lib_search_path_spec=`echo "$sys_lib_search_path_spec" | $SED  -e "s/$PATH_SEPARATOR/ /g"`
  fi
else
  sys_lib_search_path_spec="/lib /usr/lib /usr/local/lib"
fi
need_lib_prefix=unknown
hardcode_into_libs=no

# when you set need_version to no, make sure it does not cause -set_version
# flags to be left without arguments
need_version=unknown

case $host_os in
aix3*)
  version_type=linux
  library_names_spec='${libname}${release}${shared_ext}$versuffix $libname.a'
  shlibpath_var=LIBPATH

  # AIX 3 has no versioning support, so we append a major version to the name.
  soname_spec='${libname}${release}${shared_ext}$major'
  ;;

aix4* | aix5*)
  version_type=linux
  need_lib_prefix=no
  need_version=no
  hardcode_into_libs=yes
  if test "$host_cpu" = ia64; then
    # AIX 5 supports IA64
    library_names_spec='${libname}${release}${shared_ext}$major ${libname}${release}${shared_ext}$versuffix $libname${shared_ext}'
    shlibpath_var=LD_LIBRARY_PATH
  else
    # With GCC up to 2.95.x, collect2 would create an import file
    # for dependence libraries.  The import file would start with
    # the line `#! .'.  This would cause the generated library to
    # depend on `.', always an invalid library.  This was fixed in
    # development snapshots of GCC prior to 3.0.
    case $host_os in
      aix4 | aix4.[[01]] | aix4.[[01]].*)
      if { echo '#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 97)'
	   echo ' yes '
	   echo '#endif'; } | ${CC} -E - | grep yes > /dev/null; then
	:
      else
	can_build_shared=no
      fi
      ;;
    esac
    # AIX (on Power*) has no versioning support, so currently we can not hardcode correct
    # soname into executable. Probably we can add versioning support to
    # collect2, so additional links can be useful in future.
    if test "$aix_use_runtimelinking" = yes; then
      # If using run time linking (on AIX 4.2 or later) use lib<name>.so
      # instead of lib<name>.a to let people know that these are not
      # typical AIX shared libraries.
      library_names_spec='${libname}${release}${shared_ext}$versuffix ${libname}${release}${shared_ext}$major $libname${shared_ext}'
    else
      # We preserve .a as extension for shared libraries through AIX4.2
      # and later when we are not doing run time linking.
      library_names_spec='${libname}${release}.a $libname.a'
      soname_spec='${libname}${release}${shared_ext}$major'
    fi
    shlibpath_var=LIBPATH
  fi
  ;;

amigaos*)
  library_names_spec='$libname.ixlibrary $libname.a'
  # Create ${libname}_ixlibrary.a entries in /sys/libs.
  finish_eval='for lib in `ls $libdir/*.ixlibrary 2>/dev/null`; do libname=`$echo "X$lib" | $Xsed -e '\''s%^.*/\([[^/]]*\)\.ixlibrary$%\1%'\''`; test $rm /sys/libs/${libname}_ixlibrary.a; $show "(cd /sys/libs && $LN_S $lib ${libname}_ixlibrary.a)"; (cd /sys/libs && $LN_S $lib ${libname}_ixlibrary.a) || exit 1; done'
  ;;

beos*)
  library_names_spec='${libname}${shared_ext}'
  dynamic_linker="$host_os ld.so"
  shlibpath_var=LIBRARY_PATH
  ;;

bsdi4*)
  version_type=linux
  need_version=no
  library_names_spec='${libname}${release}${shared_ext}$versuffix ${libname}${release}${shared_ext}$major $libname${shared_ext}'
  soname_spec='${libname}${release}${shared_ext}$major'
  finish_cmds='PATH="\$PATH:/sbin" ldconfig $libdir'
  shlibpath_var=LD_LIBRARY_PATH
  sys_lib_search_path_spec="/shlib /usr/lib /usr/X11/lib /usr/contrib/lib /lib /usr/local/lib"
  sys_lib_dlsearch_path_spec="/shlib /usr/lib /usr/local/lib"
  # the default ld.so.conf also contains /usr/contrib/lib and
  # /usr/X11R6/lib (/usr/X11 is a link to /usr/X11R6), but let us allow
  # libtool to hard-code these into programs
  ;;

cygwin* | mingw* | pw32*)
  version_type=windows
  shrext=".dll"
  need_version=no
  need_lib_prefix=no

  case $GCC,$host_os in
  yes,cygwin* | yes,mingw* | yes,pw32*)
    library_names_spec='$libname.dll.a'
    # DLL is installed to $(libdir)/../bin by postinstall_cmds
    postinstall_cmds='base_file=`basename \${file}`${_S_}
      dlpath=`$SHELL 2>&1 -c '\''. $dir/'\''\${base_file}'\''i;echo \$dlname'\''`${_S_}
      dldir=$destdir/`dirname \$dlpath`${_S_}
      test -d \$dldir || mkdir -p \$dldir${_S_}
      $install_prog $dir/$dlname \$dldir/$dlname'
    postuninstall_cmds='dldll=`$SHELL 2>&1 -c '\''. $file; echo \$dlname'\''`${_S_}
      dlpath=$dir/\$dldll${_S_}
       $rm \$dlpath'
    shlibpath_overrides_runpath=yes

    case $host_os in
    cygwin*)
      # Cygwin DLLs use 'cyg' prefix rather than 'lib'
      soname_spec='`echo ${libname} | sed -e 's/^lib/cyg/'``echo ${release} | $SED -e 's/[[.]]/-/g'`${versuffix}${shared_ext}'
      sys_lib_search_path_spec="/lib /lib/w32api /usr/lib /usr/local/lib"
      ;;
    mingw*)
      # MinGW DLLs use traditional 'lib' prefix
      soname_spec='${libname}`echo ${release} | $SED -e 's/[[.]]/-/g'`${versuffix}${shared_ext}'
      sys_lib_search_path_spec=`$CC -print-search-dirs | grep "^libraries:" | $SED -e "s/^libraries://" -e "s,=/,/,g"`
      if echo "$sys_lib_search_path_spec" | [grep ';[c-zC-Z]:/' >/dev/null]; then
        # It is most probably a Windows format PATH printed by
        # mingw gcc, but we are running on Cygwin. Gcc prints its search
        # path with ; separators, and with drive letters. We can handle the
        # drive letters (cygwin fileutils understands them), so leave them,
        # especially as we might pass files found there to a mingw objdump,
        # which wouldn't understand a cygwinified path. Ahh.
        sys_lib_search_path_spec=`echo "$sys_lib_search_path_spec" | $SED -e 's/;/ /g'`
      else
        sys_lib_search_path_spec=`echo "$sys_lib_search_path_spec" | $SED  -e "s/$PATH_SEPARATOR/ /g"`
      fi
      ;;
    pw32*)
      # pw32 DLLs use 'pw' prefix rather than 'lib'
      library_names_spec='`echo ${libname} | sed -e 's/^lib/pw/'``echo ${release} | $SED -e 's/[.]/-/g'`${versuffix}${shared_ext}'
      ;;
    esac
    ;;

  *)
    library_names_spec='${libname}`echo ${release} | $SED -e 's/[[.]]/-/g'`${versuffix}${shared_ext} $libname.lib'
    ;;
  esac
  dynamic_linker='Win32 ld.exe'
  # FIXME: first we should search . and the directory the executable is in
  shlibpath_var=PATH
  ;;

darwin* | rhapsody*)
  dynamic_linker="$host_os dyld"
  version_type=darwin
  need_lib_prefix=no
  need_version=no
  # FIXME: Relying on posixy $() will cause problems for
  #        cross-compilation, but unfortunately the echo tests do not
  #        yet detect zsh echo's removal of \ escapes.
  library_names_spec='${libname}${release}${major}$shared_ext ${libname}$shared_ext ${libname}${release}${versuffix}$shared_ext'
  soname_spec='${libname}${release}${major}$shared_ext'
  shlibpath_overrides_runpath=yes
  shlibpath_var=DYLD_LIBRARY_PATH
  shrext='$(test .$module = .yes && echo .so || echo .dylib)'
  # Apple's gcc prints 'gcc -print-search-dirs' doesn't operate the same.
  if $CC -v 2>&1 | grep 'Apple' >/dev/null ; then 
  sys_lib_search_path_spec=`$CC -print-search-dirs | tr "\n" "$PATH_SEPARATOR" | sed -e 's/libraries:/@libraries:/' | tr "@" "\n" | grep "^libraries:" | sed -e "s/^libraries://" -e "s,=/,/,g" -e "s,$PATH_SEPARATOR, ,g" -e "s,.*,& /lib /usr/lib /usr/local/lib,g"`
  fi
  sys_lib_dlsearch_path_spec='/usr/local/lib /lib /usr/lib'
  ;;

dgux*)
  version_type=linux
  need_lib_prefix=no
  need_version=no
  library_names_spec='${libname}${release}${shared_ext}$versuffix ${libname}${release}${shared_ext}$major $libname$shared_ext'
  soname_spec='${libname}${release}${shared_ext}$major'
  shlibpath_var=LD_LIBRARY_PATH
  ;;

freebsd1*)
  dynamic_linker=no
  ;;

freebsd*)
  objformat=`test -x /usr/bin/objformat && /usr/bin/objformat || echo aout`
  version_type=freebsd-$objformat
  case $version_type in
    freebsd-elf*)
      library_names_spec='${libname}${release}${shared_ext}$versuffix ${libname}${release}${shared_ext} $libname${shared_ext}'
      need_version=no
      need_lib_prefix=no
      ;;
    freebsd-*)
      library_names_spec='${libname}${release}${shared_ext}$versuffix $libname${shared_ext}$versuffix'
      need_version=yes
      ;;
  esac
  shlibpath_var=LD_LIBRARY_PATH
  case $host_os in
  freebsd2*)
    shlibpath_overrides_runpath=yes
    ;;
  freebsd3.[01]* | freebsdelf3.[01]*)
    shlibpath_overrides_runpath=yes
    hardcode_into_libs=yes
    ;;
  *) # from 3.2 on
    shlibpath_overrides_runpath=no
    hardcode_into_libs=yes
    ;;
  esac
  ;;

gnu*)
  version_type=linux
  need_lib_prefix=no
  need_version=no
  library_names_spec='${libname}${release}${shared_ext}$versuffix ${libname}${release}${shared_ext}${major} ${libname}${shared_ext}'
  soname_spec='${libname}${release}${shared_ext}$major'
  shlibpath_var=LD_LIBRARY_PATH
  hardcode_into_libs=yes
  ;;

hpux9* | hpux10* | hpux11*)
  # Give a soname corresponding to the major version so that dld.sl refuses to
  # link against other versions.
  version_type=sunos
  need_lib_prefix=no
  need_version=no
  case "$host_cpu" in
  ia64*)
    shrext='.so'
    hardcode_into_libs=yes
    dynamic_linker="$host_os dld.so"
    shlibpath_var=LD_LIBRARY_PATH
    shlibpath_overrides_runpath=yes # Unless +noenvvar is specified.
    library_names_spec='${libname}${release}${shared_ext}$versuffix ${libname}${release}${shared_ext}$major $libname${shared_ext}'
    soname_spec='${libname}${release}${shared_ext}$major'
    if test "X$HPUX_IA64_MODE" = X32; then
      sys_lib_search_path_spec="/usr/lib/hpux32 /usr/local/lib/hpux32 /usr/local/lib"
    else
      sys_lib_search_path_spec="/usr/lib/hpux64 /usr/local/lib/hpux64"
    fi
    sys_lib_dlsearch_path_spec=$sys_lib_search_path_spec
    ;;
   hppa*64*)
     shrext='.sl'
     hardcode_into_libs=yes
     dynamic_linker="$host_os dld.sl"
     shlibpath_var=LD_LIBRARY_PATH # How should we handle SHLIB_PATH
     shlibpath_overrides_runpath=yes # Unless +noenvvar is specified.
     library_names_spec='${libname}${release}${shared_ext}$versuffix ${libname}${release}${shared_ext}$major $libname${shared_ext}'
     soname_spec='${libname}${release}${shared_ext}$major'
     sys_lib_search_path_spec="/usr/lib/pa20_64 /usr/ccs/lib/pa20_64"
     sys_lib_dlsearch_path_spec=$sys_lib_search_path_spec
     ;;
   *)
    shrext='.sl'
    dynamic_linker="$host_os dld.sl"
    shlibpath_var=SHLIB_PATH
    shlibpath_overrides_runpath=no # +s is required to enable SHLIB_PATH
    library_names_spec='${libname}${release}${shared_ext}$versuffix ${libname}${release}${shared_ext}$major $libname${shared_ext}'
    soname_spec='${libname}${release}${shared_ext}$major'
    ;;
  esac
  # HP-UX runs *really* slowly unless shared libraries are mode 555.
  postinstall_cmds='chmod 555 $lib'
  ;;

irix5* | irix6* | nonstopux*)
  case $host_os in
    nonstopux*) version_type=nonstopux ;;
    *)
	if test "$lt_cv_prog_gnu_ld" = yes; then
		version_type=linux
	else
		version_type=irix
	fi ;;
  esac
  need_lib_prefix=no
  need_version=no
  soname_spec='${libname}${release}${shared_ext}$major'
  library_names_spec='${libname}${release}${shared_ext}$versuffix ${libname}${release}${shared_ext}$major ${libname}${release}${shared_ext} $libname${shared_ext}'
  case $host_os in
  irix5* | nonstopux*)
    libsuff= shlibsuff=
    ;;
  *)
    case $LD in # libtool.m4 will add one of these switches to LD
    *-32|*"-32 "|*-melf32bsmip|*"-melf32bsmip ")
      libsuff= shlibsuff= libmagic=32-bit;;
    *-n32|*"-n32 "|*-melf32bmipn32|*"-melf32bmipn32 ")
      libsuff=32 shlibsuff=N32 libmagic=N32;;
    *-64|*"-64 "|*-melf64bmip|*"-melf64bmip ")
      libsuff=64 shlibsuff=64 libmagic=64-bit;;
    *) libsuff= shlibsuff= libmagic=never-match;;
    esac
    ;;
  esac
  shlibpath_var=LD_LIBRARY${shlibsuff}_PATH
  shlibpath_overrides_runpath=no
  sys_lib_search_path_spec="/usr/lib${libsuff} /lib${libsuff} /usr/local/lib${libsuff}"
  sys_lib_dlsearch_path_spec="/usr/lib${libsuff} /lib${libsuff}"
  hardcode_into_libs=yes
  ;;

# No shared lib support for Linux oldld, aout, or coff.
linux*oldld* | linux*aout* | linux*coff*)
  dynamic_linker=no
  ;;

# This must be Linux ELF.
linux*)
  version_type=linux
  need_lib_prefix=no
  need_version=no
  library_names_spec='${libname}${release}${shared_ext}$versuffix ${libname}${release}${shared_ext}$major $libname${shared_ext}'
  soname_spec='${libname}${release}${shared_ext}$major'
  finish_cmds='PATH="\$PATH:/sbin" ldconfig -n $libdir'
  shlibpath_var=LD_LIBRARY_PATH
  shlibpath_overrides_runpath=no
  # This implies no fast_install, which is unacceptable.
  # Some rework will be needed to allow for fast_install
  # before this can be enabled.
  hardcode_into_libs=yes

  # We used to test for /lib/ld.so.1 and disable shared libraries on
  # powerpc, because MkLinux only supported shared libraries with the
  # GNU dynamic linker.  Since this was broken with cross compilers,
  # most powerpc-linux boxes support dynamic linking these days and
  # people can always --disable-shared, the test was removed, and we
  # assume the GNU/Linux dynamic linker is in use.
  dynamic_linker='GNU/Linux ld.so'
  ;;

netbsd*)
  version_type=sunos
  need_lib_prefix=no
  need_version=no
  if echo __ELF__ | $CC -E - | grep __ELF__ >/dev/null; then
    library_names_spec='${libname}${release}${shared_ext}$versuffix ${libname}${shared_ext}$versuffix'
    finish_cmds='PATH="\$PATH:/sbin" ldconfig -m $libdir'
    dynamic_linker='NetBSD (a.out) ld.so'
  else
    library_names_spec='${libname}${release}${shared_ext}$versuffix ${libname}${release}${shared_ext}$major ${libname}${release}${shared_ext} ${libname}${shared_ext}'
    soname_spec='${libname}${release}${shared_ext}$major'
    dynamic_linker='NetBSD ld.elf_so'
  fi
  shlibpath_var=LD_LIBRARY_PATH
  shlibpath_overrides_runpath=yes
  hardcode_into_libs=yes
  ;;

newsos6)
  version_type=linux
  library_names_spec='${libname}${release}${shared_ext}$versuffix ${libname}${release}${shared_ext}$major $libname${shared_ext}'
  shlibpath_var=LD_LIBRARY_PATH
  shlibpath_overrides_runpath=yes
  ;;

nto-qnx)
  version_type=linux
  need_lib_prefix=no
  need_version=no
  library_names_spec='${libname}${release}${shared_ext}$versuffix ${libname}${release}${shared_ext}$major $libname${shared_ext}'
  soname_spec='${libname}${release}${shared_ext}$major'
  shlibpath_var=LD_LIBRARY_PATH
  shlibpath_overrides_runpath=yes
  ;;

openbsd*)
  version_type=sunos
  need_lib_prefix=no
  need_version=no
  library_names_spec='${libname}${release}${shared_ext}$versuffix ${libname}${shared_ext}$versuffix'
  finish_cmds='PATH="\$PATH:/sbin" ldconfig -m $libdir'
  shlibpath_var=LD_LIBRARY_PATH
  if test -z "`echo __ELF__ | $CC -E - | grep __ELF__`" || test "$host_os-$host_cpu" = "openbsd2.8-powerpc"; then
    case $host_os in
      openbsd2.[[89]] | openbsd2.[[89]].*)
	shlibpath_overrides_runpath=no
	;;
      *)
	shlibpath_overrides_runpath=yes
	;;
      esac
  else
    shlibpath_overrides_runpath=yes
  fi
  ;;

os2*)
  libname_spec='$name'
  shrext=".dll"
  need_lib_prefix=no
  library_names_spec='$libname${shared_ext} $libname.a'
  dynamic_linker='OS/2 ld.exe'
  shlibpath_var=LIBPATH
  ;;

osf3* | osf4* | osf5*)
  version_type=osf
  need_lib_prefix=no
  need_version=no
  soname_spec='${libname}${release}${shared_ext}$major'
  library_names_spec='${libname}${release}${shared_ext}$versuffix ${libname}${release}${shared_ext}$major $libname${shared_ext}'
  shlibpath_var=LD_LIBRARY_PATH
  sys_lib_search_path_spec="/usr/shlib /usr/ccs/lib /usr/lib/cmplrs/cc /usr/lib /usr/local/lib /var/shlib"
  sys_lib_dlsearch_path_spec="$sys_lib_search_path_spec"
  ;;

sco3.2v5*)
  version_type=osf
  soname_spec='${libname}${release}${shared_ext}$major'
  library_names_spec='${libname}${release}${shared_ext}$versuffix ${libname}${release}${shared_ext}$major $libname${shared_ext}'
  shlibpath_var=LD_LIBRARY_PATH
  ;;

solaris*)
  version_type=linux
  need_lib_prefix=no
  need_version=no
  library_names_spec='${libname}${release}${shared_ext}$versuffix ${libname}${release}${shared_ext}$major $libname${shared_ext}'
  soname_spec='${libname}${release}${shared_ext}$major'
  shlibpath_var=LD_LIBRARY_PATH
  shlibpath_overrides_runpath=yes
  hardcode_into_libs=yes
  # ldd complains unless libraries are executable
  postinstall_cmds='chmod +x $lib'
  ;;

sunos4*)
  version_type=sunos
  library_names_spec='${libname}${release}${shared_ext}$versuffix ${libname}${shared_ext}$versuffix'
  finish_cmds='PATH="\$PATH:/usr/etc" ldconfig $libdir'
  shlibpath_var=LD_LIBRARY_PATH
  shlibpath_overrides_runpath=yes
  if test "$with_gnu_ld" = yes; then
    need_lib_prefix=no
  fi
  need_version=yes
  ;;

sysv4 | sysv4.2uw2* | sysv4.3* | sysv5*)
  version_type=linux
  library_names_spec='${libname}${release}${shared_ext}$versuffix ${libname}${release}${shared_ext}$major $libname${shared_ext}'
  soname_spec='${libname}${release}${shared_ext}$major'
  shlibpath_var=LD_LIBRARY_PATH
  case $host_vendor in
    sni)
      shlibpath_overrides_runpath=no
      need_lib_prefix=no
      export_dynamic_flag_spec='${wl}-Blargedynsym'
      runpath_var=LD_RUN_PATH
      ;;
    siemens)
      need_lib_prefix=no
      ;;
    motorola)
      need_lib_prefix=no
      need_version=no
      shlibpath_overrides_runpath=no
      sys_lib_search_path_spec='/lib /usr/lib /usr/ccs/lib'
      ;;
  esac
  ;;

sysv4*MP*)
  if test -d /usr/nec ;then
    version_type=linux
    library_names_spec='$libname${shared_ext}.$versuffix $libname${shared_ext}.$major $libname${shared_ext}'
    soname_spec='$libname${shared_ext}.$major'
    shlibpath_var=LD_LIBRARY_PATH
  fi
  ;;

uts4*)
  version_type=linux
  library_names_spec='${libname}${release}${shared_ext}$versuffix ${libname}${release}${shared_ext}$major $libname${shared_ext}'
  soname_spec='${libname}${release}${shared_ext}$major'
  shlibpath_var=LD_LIBRARY_PATH
  ;;

*)
  dynamic_linker=no
  ;;
esac
AC_MSG_RESULT([$dynamic_linker])
test "$dynamic_linker" = no && can_build_shared=no
])# AC_LIBTOOL_SYS_DYNAMIC_LINKER


# _LT_AC_TAGCONFIG
# ----------------
AC_DEFUN([_LT_AC_TAGCONFIG],
[AC_ARG_WITH([tags],
    [AC_HELP_STRING([--with-tags@<:@=TAGS@:>@],
        [include additional configurations @<:@automatic@:>@])],
    [tagnames="$withval"])

if test -f "$ltmain" && test -n "$tagnames"; then
  if test ! -f "${ofile}"; then
    AC_MSG_WARN([output file `$ofile' does not exist])
  fi

  if test -z "$LTCC"; then
    eval "`$SHELL ${ofile} --config | grep '^LTCC='`"
    if test -z "$LTCC"; then
      AC_MSG_WARN([output file `$ofile' does not look like a libtool script])
    else
      AC_MSG_WARN([using `LTCC=$LTCC', extracted from `$ofile'])
    fi
  fi

  # Extract list of available tagged configurations in $ofile.
  # Note that this assumes the entire list is on one line.
  available_tags=`grep "^available_tags=" "${ofile}" | $SED -e 's/available_tags=\(.*$\)/\1/' -e 's/\"//g'`

  lt_save_ifs="$IFS"; IFS="${IFS}$PATH_SEPARATOR,"
  for tagname in $tagnames; do
    IFS="$lt_save_ifs"
    # Check whether tagname contains only valid characters
    case `$echo "X$tagname" | $Xsed -e 's:[[-_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890,/]]::g'` in
    "") ;;
    *)  AC_MSG_ERROR([invalid tag name: $tagname])
	;;
    esac

    if grep "^# ### BEGIN LIBTOOL TAG CONFIG: $tagname$" < "${ofile}" > /dev/null
    then
      AC_MSG_ERROR([tag name \"$tagname\" already exists])
    fi

    # Update the list of available tags.
    if test -n "$tagname"; then
      echo appending configuration tag \"$tagname\" to $ofile

      case $tagname in
      CXX)
	if test -n "$CXX" && test "X$CXX" != "Xno"; then
	  AC_LIBTOOL_LANG_CXX_CONFIG
	else
	  tagname=""
	fi
	;;

      F77)
	if test -n "$F77" && test "X$F77" != "Xno"; then
	  AC_LIBTOOL_LANG_F77_CONFIG
	else
	  tagname=""
	fi
	;;

      GCJ)
	if test -n "$GCJ" && test "X$GCJ" != "Xno"; then
	  AC_LIBTOOL_LANG_GCJ_CONFIG
	else
	  tagname=""
	fi
	;;

      RC)
	AC_LIBTOOL_LANG_RC_CONFIG
	;;

      *)
	AC_MSG_ERROR([Unsupported tag name: $tagname])
	;;
      esac

      # Append the new tag name to the list of available tags.
      if test -n "$tagname" ; then
      available_tags="$available_tags $tagname"
    fi
    fi
  done
  IFS="$lt_save_ifs"

  # Now substitute the updated list of available tags.
  if eval "sed -e 's/^available_tags=.*\$/available_tags=\"$available_tags\"/' \"$ofile\" > \"${ofile}T\""; then
    mv "${ofile}T" "$ofile"
    chmod +x "$ofile"
  else
    rm -f "${ofile}T"
    AC_MSG_ERROR([unable to update list of available tagged configurations.])
  fi
fi
])# _LT_AC_TAGCONFIG


# AC_LIBTOOL_DLOPEN
# -----------------
# enable checks for dlopen support
AC_DEFUN([AC_LIBTOOL_DLOPEN],
 [AC_BEFORE([$0],[AC_LIBTOOL_SETUP])
])# AC_LIBTOOL_DLOPEN


# AC_LIBTOOL_WIN32_DLL
# --------------------
# declare package support for building win32 dll's
AC_DEFUN([AC_LIBTOOL_WIN32_DLL],
[AC_BEFORE([$0], [AC_LIBTOOL_SETUP])
])# AC_LIBTOOL_WIN32_DLL


# AC_ENABLE_SHARED([DEFAULT])
# ---------------------------
# implement the --enable-shared flag
# DEFAULT is either `yes' or `no'.  If omitted, it defaults to `yes'.
AC_DEFUN([AC_ENABLE_SHARED],
[define([AC_ENABLE_SHARED_DEFAULT], ifelse($1, no, no, yes))dnl
AC_ARG_ENABLE([shared],
    [AC_HELP_STRING([--enable-shared@<:@=PKGS@:>@],
	[build shared libraries @<:@default=]AC_ENABLE_SHARED_DEFAULT[@:>@])],
    [p=${PACKAGE-default}
    case $enableval in
    yes) enable_shared=yes ;;
    no) enable_shared=no ;;
    *)
      enable_shared=no
      # Look at the argument we got.  We use all the common list separators.
      lt_save_ifs="$IFS"; IFS="${IFS}$PATH_SEPARATOR,"
      for pkg in $enableval; do
	IFS="$lt_save_ifs"
	if test "X$pkg" = "X$p"; then
	  enable_shared=yes
	fi
      done
      IFS="$lt_save_ifs"
      ;;
    esac],
    [enable_shared=]AC_ENABLE_SHARED_DEFAULT)
])# AC_ENABLE_SHARED


# AC_DISABLE_SHARED
# -----------------
#- set the default shared flag to --disable-shared
AC_DEFUN([AC_DISABLE_SHARED],
[AC_BEFORE([$0],[AC_LIBTOOL_SETUP])dnl
AC_ENABLE_SHARED(no)
])# AC_DISABLE_SHARED


# AC_ENABLE_STATIC([DEFAULT])
# ---------------------------
# implement the --enable-static flag
# DEFAULT is either `yes' or `no'.  If omitted, it defaults to `yes'.
AC_DEFUN([AC_ENABLE_STATIC],
[define([AC_ENABLE_STATIC_DEFAULT], ifelse($1, no, no, yes))dnl
AC_ARG_ENABLE([static],
    [AC_HELP_STRING([--enable-static@<:@=PKGS@:>@],
	[build static libraries @<:@default=]AC_ENABLE_STATIC_DEFAULT[@:>@])],
    [p=${PACKAGE-default}
    case $enableval in
    yes) enable_static=yes ;;
    no) enable_static=no ;;
    *)
     enable_static=no
      # Look at the argument we got.  We use all the common list separators.
      lt_save_ifs="$IFS"; IFS="${IFS}$PATH_SEPARATOR,"
      for pkg in $enableval; do
	IFS="$lt_save_ifs"
	if test "X$pkg" = "X$p"; then
	  enable_static=yes
	fi
      done
      IFS="$lt_save_ifs"
      ;;
    esac],
    [enable_static=]AC_ENABLE_STATIC_DEFAULT)
])# AC_ENABLE_STATIC


# AC_DISABLE_STATIC
# -----------------
# set the default static flag to --disable-static
AC_DEFUN([AC_DISABLE_STATIC],
[AC_BEFORE([$0],[AC_LIBTOOL_SETUP])dnl
AC_ENABLE_STATIC(no)
])# AC_DISABLE_STATIC


# AC_ENABLE_FAST_INSTALL([DEFAULT])
# ---------------------------------
# implement the --enable-fast-install flag
# DEFAULT is either `yes' or `no'.  If omitted, it defaults to `yes'.
AC_DEFUN([AC_ENABLE_FAST_INSTALL],
[define([AC_ENABLE_FAST_INSTALL_DEFAULT], ifelse($1, no, no, yes))dnl
AC_ARG_ENABLE([fast-install],
    [AC_HELP_STRING([--enable-fast-install@<:@=PKGS@:>@],
    [optimize for fast installation @<:@default=]AC_ENABLE_FAST_INSTALL_DEFAULT[@:>@])],
    [p=${PACKAGE-default}
    case $enableval in
    yes) enable_fast_install=yes ;;
    no) enable_fast_install=no ;;
    *)
      enable_fast_install=no
      # Look at the argument we got.  We use all the common list separators.
      lt_save_ifs="$IFS"; IFS="${IFS}$PATH_SEPARATOR,"
      for pkg in $enableval; do
	IFS="$lt_save_ifs"
	if test "X$pkg" = "X$p"; then
	  enable_fast_install=yes
	fi
      done
      IFS="$lt_save_ifs"
      ;;
    esac],
    [enable_fast_install=]AC_ENABLE_FAST_INSTALL_DEFAULT)
])# AC_ENABLE_FAST_INSTALL


# AC_DISABLE_FAST_INSTALL
# -----------------------
# set the default to --disable-fast-install
AC_DEFUN([AC_DISABLE_FAST_INSTALL],
[AC_BEFORE([$0],[AC_LIBTOOL_SETUP])dnl
AC_ENABLE_FAST_INSTALL(no)
])# AC_DISABLE_FAST_INSTALL


# AC_LIBTOOL_PICMODE([MODE])
# --------------------------
# implement the --with-pic flag
# MODE is either `yes' or `no'.  If omitted, it defaults to `both'.
AC_DEFUN([AC_LIBTOOL_PICMODE],
[AC_BEFORE([$0],[AC_LIBTOOL_SETUP])dnl
pic_mode=ifelse($#,1,$1,default)
])# AC_LIBTOOL_PICMODE


# AC_PROG_EGREP
# -------------
# This is predefined starting with Autoconf 2.54, so this conditional
# definition can be removed once we require Autoconf 2.54 or later.
m4_ifndef([AC_PROG_EGREP], [AC_DEFUN([AC_PROG_EGREP],
[AC_CACHE_CHECK([for egrep], [ac_cv_prog_egrep],
   [if echo a | (grep -E '(a|b)') >/dev/null 2>&1
    then ac_cv_prog_egrep='grep -E'
    else ac_cv_prog_egrep='egrep'
    fi])
 EGREP=$ac_cv_prog_egrep
 AC_SUBST([EGREP])
])])


# AC_PATH_TOOL_PREFIX
# -------------------
# find a file program which can recognise shared library
AC_DEFUN([AC_PATH_TOOL_PREFIX],
[AC_REQUIRE([AC_PROG_EGREP])dnl
AC_MSG_CHECKING([for $1])
AC_CACHE_VAL(lt_cv_path_MAGIC_CMD,
[case $MAGIC_CMD in
[[\\/*] |  ?:[\\/]*])
  lt_cv_path_MAGIC_CMD="$MAGIC_CMD" # Let the user override the test with a path.
  ;;
*)
  lt_save_MAGIC_CMD="$MAGIC_CMD"
  lt_save_ifs="$IFS"; IFS=$PATH_SEPARATOR
dnl $ac_dummy forces splitting on constant user-supplied paths.
dnl POSIX.2 word splitting is done only on the output of word expansions,
dnl not every word.  This closes a longstanding sh security hole.
  ac_dummy="ifelse([$2], , $PATH, [$2])"
  for ac_dir in $ac_dummy; do
    IFS="$lt_save_ifs"
    test -z "$ac_dir" && ac_dir=.
    if test -f $ac_dir/$1; then
      lt_cv_path_MAGIC_CMD="$ac_dir/$1"
      if test -n "$file_magic_test_file"; then
	case $deplibs_check_method in
	"file_magic "*)
	  file_magic_regex="`expr \"$deplibs_check_method\" : \"file_magic \(.*\)\"`"
	  MAGIC_CMD="$lt_cv_path_MAGIC_CMD"
	  if eval $file_magic_cmd \$file_magic_test_file 2> /dev/null |
	    $EGREP "$file_magic_regex" > /dev/null; then
	    :
	  else
	    cat <<EOF 1>&2

*** Warning: the command libtool uses to detect shared libraries,
*** $file_magic_cmd, produces output that libtool cannot recognize.
*** The result is that libtool may fail to recognize shared libraries
*** as such.  This will affect the creation of libtool libraries that
*** depend on shared libraries, but programs linked with such libtool
*** libraries will work regardless of this problem.  Nevertheless, you
*** may want to report the problem to your system manager and/or to
*** bug-libtool@gnu.org

EOF
	  fi ;;
	esac
      fi
      break
    fi
  done
  IFS="$lt_save_ifs"
  MAGIC_CMD="$lt_save_MAGIC_CMD"
  ;;
esac])
MAGIC_CMD="$lt_cv_path_MAGIC_CMD"
if test -n "$MAGIC_CMD"; then
  AC_MSG_RESULT($MAGIC_CMD)
else
  AC_MSG_RESULT(no)
fi
])# AC_PATH_TOOL_PREFIX


# AC_PATH_MAGIC
# -------------
# find a file program which can recognise a shared library
AC_DEFUN([AC_PATH_MAGIC],
[AC_PATH_TOOL_PREFIX(${ac_tool_prefix}file, /usr/bin$PATH_SEPARATOR$PATH)
if test -z "$lt_cv_path_MAGIC_CMD"; then
  if test -n "$ac_tool_prefix"; then
    AC_PATH_TOOL_PREFIX(file, /usr/bin$PATH_SEPARATOR$PATH)
  else
    MAGIC_CMD=:
  fi
fi
])# AC_PATH_MAGIC


# AC_PROG_LD
# ----------
# find the path to the GNU or non-GNU linker
AC_DEFUN([AC_PROG_LD],
[AC_ARG_WITH([gnu-ld],
    [AC_HELP_STRING([--with-gnu-ld],
	[assume the C compiler uses GNU ld @<:@default=no@:>@])],
    [test "$withval" = no || with_gnu_ld=yes],
    [with_gnu_ld=no])
AC_REQUIRE([LT_AC_PROG_SED])dnl
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_CANONICAL_HOST])dnl
AC_REQUIRE([AC_CANONICAL_BUILD])dnl
ac_prog=ld
if test "$GCC" = yes; then
  # Check if gcc -print-prog-name=ld gives a path.
  AC_MSG_CHECKING([for ld used by $CC])
  case $host in
  *-*-mingw*)
    # gcc leaves a trailing carriage return which upsets mingw
    ac_prog=`($CC -print-prog-name=ld) 2>&5 | tr -d '\015'` ;;
  *)
    ac_prog=`($CC -print-prog-name=ld) 2>&5` ;;
  esac
  case $ac_prog in
    # Accept absolute paths.
    [[\\/]]* | ?:[[\\/]]*)
      re_direlt='/[[^/]][[^/]]*/\.\./'
      # Canonicalize the path of ld
      ac_prog=`echo $ac_prog| $SED 's%\\\\%/%g'`
      while echo $ac_prog | grep "$re_direlt" > /dev/null 2>&1; do
	ac_prog=`echo $ac_prog| $SED "s%$re_direlt%/%"`
      done
      test -z "$LD" && LD="$ac_prog"
      ;;
  "")
    # If it fails, then pretend we aren't using GCC.
    ac_prog=ld
    ;;
  *)
    # If it is relative, then search for the first ld in PATH.
    with_gnu_ld=unknown
    ;;
  esac
elif test "$with_gnu_ld" = yes; then
  AC_MSG_CHECKING([for GNU ld])
else
  AC_MSG_CHECKING([for non-GNU ld])
fi
AC_CACHE_VAL(lt_cv_path_LD,
[if test -z "$LD"; then
  lt_save_ifs="$IFS"; IFS=$PATH_SEPARATOR
  for ac_dir in $PATH; do
    IFS="$lt_save_ifs"
    test -z "$ac_dir" && ac_dir=.
    if test -f "$ac_dir/$ac_prog" || test -f "$ac_dir/$ac_prog$ac_exeext"; then
      lt_cv_path_LD="$ac_dir/$ac_prog"
      # Check to see if the program is GNU ld.  I'd rather use --version,
      # but apparently some GNU ld's only accept -v.
      # Break only if it was the GNU/non-GNU ld that we prefer.
      case `"$lt_cv_path_LD" -v 2>&1 </dev/null` in
      *GNU* | *'with BFD'*)
	test "$with_gnu_ld" != no && break
	;;
      *)
	test "$with_gnu_ld" != yes && break
	;;
      esac
    fi
  done
  IFS="$lt_save_ifs"
else
  lt_cv_path_LD="$LD" # Let the user override the test with a path.
fi])
LD="$lt_cv_path_LD"
if test -n "$LD"; then
  AC_MSG_RESULT($LD)
else
  AC_MSG_RESULT(no)
fi
test -z "$LD" && AC_MSG_ERROR([no acceptable ld found in \$PATH])
AC_PROG_LD_GNU
])# AC_PROG_LD


# AC_PROG_LD_GNU
# --------------
AC_DEFUN([AC_PROG_LD_GNU],
[AC_REQUIRE([AC_PROG_EGREP])dnl
AC_CACHE_CHECK([if the linker ($LD) is GNU ld], lt_cv_prog_gnu_ld,
[# I'd rather use --version here, but apparently some GNU ld's only accept -v.
case `"$LD" -v 2>&1 </dev/null` in
*GNU* | *'with BFD'*)
  lt_cv_prog_gnu_ld=yes
  ;;
*)
  lt_cv_prog_gnu_ld=no
  ;;
esac])
with_gnu_ld=$lt_cv_prog_gnu_ld
])# AC_PROG_LD_GNU


# AC_PROG_LD_RELOAD_FLAG
# ----------------------
# find reload flag for linker
#   -- PORTME Some linkers may need a different reload flag.
AC_DEFUN([AC_PROG_LD_RELOAD_FLAG],
[AC_CACHE_CHECK([for $LD option to reload object files],
  lt_cv_ld_reload_flag,
  [lt_cv_ld_reload_flag='-r'])
reload_flag=$lt_cv_ld_reload_flag
case $reload_flag in
"" | " "*) ;;
*) reload_flag=" $reload_flag" ;;
esac
reload_cmds='$CC -nostdlib -Xlinker$reload_flag $archargs -o $output$reload_objs'
])# AC_PROG_LD_RELOAD_FLAG


# AC_DEPLIBS_CHECK_METHOD
# -----------------------
# how to check for library dependencies
#  -- PORTME fill in with the dynamic library characteristics
AC_DEFUN([AC_DEPLIBS_CHECK_METHOD],
[AC_CACHE_CHECK([how to recognise dependent libraries],
lt_cv_deplibs_check_method,
[lt_cv_file_magic_cmd='$MAGIC_CMD'
lt_cv_file_magic_test_file=
lt_cv_deplibs_check_method='unknown'
# Need to set the preceding variable on all platforms that support
# interlibrary dependencies.
# 'none' -- dependencies not supported.
# `unknown' -- same as none, but documents that we really don't know.
# 'pass_all' -- all dependencies passed with no checks.
# 'test_compile' -- check by making test program.
# 'file_magic [[regex]]' -- check by looking for files in library path
# which responds to the $file_magic_cmd with a given extended regex.
# If you have `file' or equivalent on your system and you're not sure
# whether `pass_all' will *always* work, you probably want this one.

case $host_os in
aix4* | aix5*)
  lt_cv_deplibs_check_method=pass_all
  ;;

beos*)
  lt_cv_deplibs_check_method=pass_all
  ;;

bsdi4*)
  lt_cv_deplibs_check_method='file_magic ELF [[0-9]][[0-9]]*-bit [[ML]]SB (shared object|dynamic lib)'
  lt_cv_file_magic_cmd='/usr/bin/file -L'
  lt_cv_file_magic_test_file=/shlib/libc.so
  ;;

cygwin* | mingw* | pw32*)
  # win32_libid is a shell function defined in ltmain.sh
  lt_cv_deplibs_check_method='file_magic ^x86 archive import|^x86 DLL'
  lt_cv_file_magic_cmd='win32_libid'
  ;;

darwin* | rhapsody*)
  # this will be overwritten by pass_all, but leave it in just in case
  lt_cv_deplibs_check_method='file_magic Mach-O dynamically linked shared library'
  lt_cv_file_magic_cmd='/usr/bin/file -L'
  case "$host_os" in
  rhapsody* | darwin1.[[012]])
    lt_cv_file_magic_test_file=`/System/Library/Frameworks/System.framework/System`
    ;;
  *) # Darwin 1.3 on
    lt_cv_file_magic_test_file='/usr/lib/libSystem.dylib'
    ;;
  esac
  lt_cv_deplibs_check_method=pass_all
  ;;

freebsd*)
  if echo __ELF__ | $CC -E - | grep __ELF__ > /dev/null; then
    case $host_cpu in
    i*86 )
      # Not sure whether the presence of OpenBSD here was a mistake.
      # Let's accept both of them until this is cleared up.
      lt_cv_deplibs_check_method='file_magic (FreeBSD|OpenBSD)/i[[3-9]]86 (compact )?demand paged shared library'
      lt_cv_file_magic_cmd=/usr/bin/file
      lt_cv_file_magic_test_file=`echo /usr/lib/libc.so.*`
      ;;
    esac
  else
    lt_cv_deplibs_check_method=pass_all
  fi
  ;;

gnu*)
  lt_cv_deplibs_check_method=pass_all
  ;;

hpux10.20* | hpux11*)
  lt_cv_file_magic_cmd=/usr/bin/file
  case "$host_cpu" in
  ia64*)
    lt_cv_deplibs_check_method='file_magic (s[[0-9]][[0-9]][[0-9]]|ELF-[[0-9]][[0-9]]) shared object file - IA64'
    lt_cv_file_magic_test_file=/usr/lib/hpux32/libc.so
    ;;
  hppa*64*)
    [lt_cv_deplibs_check_method='file_magic (s[0-9][0-9][0-9]|ELF-[0-9][0-9]) shared object file - PA-RISC [0-9].[0-9]']
    lt_cv_file_magic_test_file=/usr/lib/pa20_64/libc.sl
    ;;
  *)
    lt_cv_deplibs_check_method='file_magic (s[[0-9]][[0-9]][[0-9]]|PA-RISC[[0-9]].[[0-9]]) shared library'
    lt_cv_file_magic_test_file=/usr/lib/libc.sl
    ;;
  esac
  ;;

irix5* | irix6* | nonstopux*)
  case $host_os in
  irix5* | nonstopux*)
    # this will be overridden with pass_all, but let us keep it just in case
    lt_cv_deplibs_check_method="file_magic ELF 32-bit MSB dynamic lib MIPS - version 1"
    ;;
  *)
    case $LD in
    *-32|*"-32 ") libmagic=32-bit;;
    *-n32|*"-n32 ") libmagic=N32;;
    *-64|*"-64 ") libmagic=64-bit;;
    *) libmagic=never-match;;
    esac
    # this will be overridden with pass_all, but let us keep it just in case
    lt_cv_deplibs_check_method="file_magic ELF ${libmagic} MSB mips-[[1234]] dynamic lib MIPS - version 1"
    ;;
  esac
  lt_cv_file_magic_test_file=`echo /lib${libsuff}/libc.so*`
  lt_cv_deplibs_check_method=pass_all
  ;;

# This must be Linux ELF.
linux*)
  case $host_cpu in
  alpha* | hppa* | i*86 | ia64* | m68* | mips | mipsel | powerpc* | sparc* | s390* | sh*)
    lt_cv_deplibs_check_method=pass_all ;;
  *)
    # glibc up to 2.1.1 does not perform some relocations on ARM
    lt_cv_deplibs_check_method='file_magic ELF [[0-9]][[0-9]]*-bit [[LM]]SB (shared object|dynamic lib )' ;;
  esac
  lt_cv_file_magic_test_file=`echo /lib/libc.so* /lib/libc-*.so`
  ;;

netbsd*)
  if echo __ELF__ | $CC -E - | grep __ELF__ > /dev/null; then
    lt_cv_deplibs_check_method='match_pattern /lib[[^/]]+(\.so\.[[0-9]]+\.[[0-9]]+|_pic\.a)$'
  else
    lt_cv_deplibs_check_method='match_pattern /lib[[^/]]+(\.so|_pic\.a)$'
  fi
  ;;

newos6*)
  lt_cv_deplibs_check_method='file_magic ELF [[0-9]][[0-9]]*-bit [[ML]]SB (executable|dynamic lib)'
  lt_cv_file_magic_cmd=/usr/bin/file
  lt_cv_file_magic_test_file=/usr/lib/libnls.so
  ;;

nto-qnx)
  lt_cv_deplibs_check_method=unknown
  ;;

openbsd*)
  lt_cv_file_magic_cmd=/usr/bin/file
  lt_cv_file_magic_test_file=`echo /usr/lib/libc.so.*`
  if test -z "`echo __ELF__ | $CC -E - | grep __ELF__`" || test "$host_os-$host_cpu" = "openbsd2.8-powerpc"; then
    lt_cv_deplibs_check_method='file_magic ELF [[0-9]][[0-9]]*-bit [[LM]]SB shared object'
  else
    lt_cv_deplibs_check_method='file_magic OpenBSD.* shared library'
  fi
  ;;

osf3* | osf4* | osf5*)
  # this will be overridden with pass_all, but let us keep it just in case
  lt_cv_deplibs_check_method='file_magic COFF format alpha shared library'
  lt_cv_file_magic_test_file=/shlib/libc.so
  lt_cv_deplibs_check_method=pass_all
  ;;

sco3.2v5*)
  lt_cv_deplibs_check_method=pass_all
  ;;

solaris*)
  lt_cv_deplibs_check_method=pass_all
  lt_cv_file_magic_test_file=/lib/libc.so
  ;;

sysv4 | sysv4.2uw2* | sysv4.3* | sysv5*)
  case $host_vendor in
  motorola)
    lt_cv_deplibs_check_method='file_magic ELF [[0-9]][[0-9]]*-bit [[ML]]SB (shared object|dynamic lib) M[[0-9]][[0-9]]* Version [[0-9]]'
    lt_cv_file_magic_test_file=`echo /usr/lib/libc.so*`
    ;;
  ncr)
    lt_cv_deplibs_check_method=pass_all
    ;;
  sequent)
    lt_cv_file_magic_cmd='/bin/file'
    lt_cv_deplibs_check_method='file_magic ELF [[0-9]][[0-9]]*-bit [[LM]]SB (shared object|dynamic lib )'
    ;;
  sni)
    lt_cv_file_magic_cmd='/bin/file'
    lt_cv_deplibs_check_method="file_magic ELF [[0-9]][[0-9]]*-bit [[LM]]SB dynamic lib"
    lt_cv_file_magic_test_file=/lib/libc.so
    ;;
  siemens)
    lt_cv_deplibs_check_method=pass_all
    ;;
  esac
  ;;

sysv5OpenUNIX8* | sysv5UnixWare7* | sysv5uw[[78]]* | unixware7* | sysv4*uw2*)
  lt_cv_deplibs_check_method=pass_all
  ;;
esac
])
file_magic_cmd=$lt_cv_file_magic_cmd
deplibs_check_method=$lt_cv_deplibs_check_method
test -z "$deplibs_check_method" && deplibs_check_method=unknown
])# AC_DEPLIBS_CHECK_METHOD


# AC_PROG_NM
# ----------
# find the path to a BSD-compatible name lister
AC_DEFUN([AC_PROG_NM],
[AC_CACHE_CHECK([for BSD-compatible nm], lt_cv_path_NM,
[if test -n "$NM"; then
  # Let the user override the test.
  lt_cv_path_NM="$NM"
else
  lt_save_ifs="$IFS"; IFS=$PATH_SEPARATOR
  for ac_dir in $PATH /usr/ccs/bin /usr/ucb /bin; do
    IFS="$lt_save_ifs"
    test -z "$ac_dir" && ac_dir=.
    tmp_nm="$ac_dir/${ac_tool_prefix}nm"
    if test -f "$tmp_nm" || test -f "$tmp_nm$ac_exeext" ; then
      # Check to see if the nm accepts a BSD-compat flag.
      # Adding the `sed 1q' prevents false positives on HP-UX, which says:
      #   nm: unknown option "B" ignored
      # Tru64's nm complains that /dev/null is an invalid object file
      case `"$tmp_nm" -B /dev/null 2>&1 | sed '1q'` in
      */dev/null* | *'Invalid file or object type'*)
	lt_cv_path_NM="$tmp_nm -B"
	break
        ;;
      *)
	case `"$tmp_nm" -p /dev/null 2>&1 | sed '1q'` in
	*/dev/null*)
	  lt_cv_path_NM="$tmp_nm -p"
	  break
	  ;;
	*)
	  lt_cv_path_NM=${lt_cv_path_NM="$tmp_nm"} # keep the first match, but
	  continue # so that we can try to find one that supports BSD flags
	  ;;
	esac
      esac
    fi
  done
  IFS="$lt_save_ifs"
  test -z "$lt_cv_path_NM" && lt_cv_path_NM=nm
fi])
NM="$lt_cv_path_NM"
])# AC_PROG_NM


# AC_CHECK_LIBM
# -------------
# check for math library
AC_DEFUN([AC_CHECK_LIBM],
[AC_REQUIRE([AC_CANONICAL_HOST])dnl
LIBM=
case $host in
*-*-beos* | *-*-cygwin* | *-*-pw32* | *-*-darwin*)
  # These system don't have libm, or don't need it
  ;;
*-ncr-sysv4.3*)
  AC_CHECK_LIB(mw, _mwvalidcheckl, LIBM="-lmw")
  AC_CHECK_LIB(m, cos, LIBM="$LIBM -lm")
  ;;
*)
  AC_CHECK_LIB(m, cos, LIBM="-lm")
  ;;
esac
])# AC_CHECK_LIBM


# AC_LIBLTDL_CONVENIENCE([DIRECTORY])
# -----------------------------------
# sets LIBLTDL to the link flags for the libltdl convenience library and
# LTDLINCL to the include flags for the libltdl header and adds
# --enable-ltdl-convenience to the configure arguments.  Note that LIBLTDL
# and LTDLINCL are not AC_SUBSTed, nor is AC_CONFIG_SUBDIRS called.  If
# DIRECTORY is not provided, it is assumed to be `libltdl'.  LIBLTDL will
# be prefixed with '${top_builddir}/' and LTDLINCL will be prefixed with
# '${top_srcdir}/' (note the single quotes!).  If your package is not
# flat and you're not using automake, define top_builddir and
# top_srcdir appropriately in the Makefiles.
AC_DEFUN([AC_LIBLTDL_CONVENIENCE],
[AC_BEFORE([$0],[AC_LIBTOOL_SETUP])dnl
  case $enable_ltdl_convenience in
  no) AC_MSG_ERROR([this package needs a convenience libltdl]) ;;
  "") enable_ltdl_convenience=yes
      ac_configure_args="$ac_configure_args --enable-ltdl-convenience" ;;
  esac
  LIBLTDL='${top_builddir}/'ifelse($#,1,[$1],['libltdl'])/libltdlc.la
  LTDLINCL='-I${top_srcdir}/'ifelse($#,1,[$1],['libltdl'])
  # For backwards non-gettext consistent compatibility...
  INCLTDL="$LTDLINCL"
])# AC_LIBLTDL_CONVENIENCE


# AC_LIBLTDL_INSTALLABLE([DIRECTORY])
# -----------------------------------
# sets LIBLTDL to the link flags for the libltdl installable library and
# LTDLINCL to the include flags for the libltdl header and adds
# --enable-ltdl-install to the configure arguments.  Note that LIBLTDL
# and LTDLINCL are not AC_SUBSTed, nor is AC_CONFIG_SUBDIRS called.  If
# DIRECTORY is not provided and an installed libltdl is not found, it is
# assumed to be `libltdl'.  LIBLTDL will be prefixed with '${top_builddir}/'
# and LTDLINCL will be prefixed with '${top_srcdir}/' (note the single
# quotes!).  If your package is not flat and you're not using automake,
# define top_builddir and top_srcdir appropriately in the Makefiles.
# In the future, this macro may have to be called after AC_PROG_LIBTOOL.
AC_DEFUN([AC_LIBLTDL_INSTALLABLE],
[AC_BEFORE([$0],[AC_LIBTOOL_SETUP])dnl
  AC_CHECK_LIB(ltdl, lt_dlinit,
  [test x"$enable_ltdl_install" != xyes && enable_ltdl_install=no],
  [if test x"$enable_ltdl_install" = xno; then
     AC_MSG_WARN([libltdl not installed, but installation disabled])
   else
     enable_ltdl_install=yes
   fi
  ])
  if test x"$enable_ltdl_install" = x"yes"; then
    ac_configure_args="$ac_configure_args --enable-ltdl-install"
    LIBLTDL='${top_builddir}/'ifelse($#,1,[$1],['libltdl'])/libltdl.la
    LTDLINCL='-I${top_srcdir}/'ifelse($#,1,[$1],['libltdl'])
  else
    ac_configure_args="$ac_configure_args --enable-ltdl-install=no"
    LIBLTDL="-lltdl"
    LTDLINCL=
  fi
  # For backwards non-gettext consistent compatibility...
  INCLTDL="$LTDLINCL"
])# AC_LIBLTDL_INSTALLABLE


# AC_LIBTOOL_CXX
# --------------
# enable support for C++ libraries
AC_DEFUN([AC_LIBTOOL_CXX],
[AC_REQUIRE([_LT_AC_LANG_CXX])
])# AC_LIBTOOL_CXX


# _LT_AC_LANG_CXX
# ---------------
AC_DEFUN([_LT_AC_LANG_CXX],
[AC_REQUIRE([AC_PROG_CXX])
AC_REQUIRE([AC_PROG_CXXCPP])
_LT_AC_SHELL_INIT([tagnames=`echo "$tagnames,CXX" | sed 's/^,//'`])
])# _LT_AC_LANG_CXX


# AC_LIBTOOL_F77
# --------------
# enable support for Fortran 77 libraries
AC_DEFUN([AC_LIBTOOL_F77],
[AC_REQUIRE([_LT_AC_LANG_F77])
])# AC_LIBTOOL_F77


# _LT_AC_LANG_F77
# ---------------
AC_DEFUN([_LT_AC_LANG_F77],
[AC_REQUIRE([AC_PROG_F77])
_LT_AC_SHELL_INIT([tagnames=`echo "$tagnames,F77" | sed 's/^,//'`])
])# _LT_AC_LANG_F77


# AC_LIBTOOL_GCJ
# --------------
# enable support for GCJ libraries
AC_DEFUN([AC_LIBTOOL_GCJ],
[AC_REQUIRE([_LT_AC_LANG_GCJ])
])# AC_LIBTOOL_GCJ


# _LT_AC_LANG_GCJ
# ---------------
AC_DEFUN([_LT_AC_LANG_GCJ],
[AC_PROVIDE_IFELSE([AC_PROG_GCJ],[],
  [AC_PROVIDE_IFELSE([A][M_PROG_GCJ],[],
    [AC_PROVIDE_IFELSE([LT_AC_PROG_GCJ],[],
      [ifdef([AC_PROG_GCJ],[AC_REQUIRE([AC_PROG_GCJ])],
	 [ifdef([A][M_PROG_GCJ],[AC_REQUIRE([A][M_PROG_GCJ])],
	   [AC_REQUIRE([A][C_PROG_GCJ_OR_A][M_PROG_GCJ])])])])])])
_LT_AC_SHELL_INIT([tagnames=`echo "$tagnames,GCJ" | sed 's/^,//'`])
])# _LT_AC_LANG_GCJ


# AC_LIBTOOL_RC
# --------------
# enable support for Windows resource files
AC_DEFUN([AC_LIBTOOL_RC],
[AC_REQUIRE([LT_AC_PROG_RC])
_LT_AC_SHELL_INIT([tagnames=`echo "$tagnames,RC" | sed 's/^,//'`])
])# AC_LIBTOOL_RC


# AC_LIBTOOL_LANG_C_CONFIG
# ------------------------
# Ensure that the configuration vars for the C compiler are
# suitably defined.  Those variables are subsequently used by
# AC_LIBTOOL_CONFIG to write the compiler configuration to `libtool'.
AC_DEFUN([AC_LIBTOOL_LANG_C_CONFIG], [_LT_AC_LANG_C_CONFIG])
AC_DEFUN([_LT_AC_LANG_C_CONFIG],
[lt_save_CC="$CC"
AC_LANG_PUSH(C)

# Source file extension for C test sources.
ac_ext=c

# Object file extension for compiled C test sources.
objext=o
_LT_AC_TAGVAR(objext, $1)=$objext

# Code to be used in simple compile tests
lt_simple_compile_test_code="int some_variable = 0;\n"

# Code to be used in simple link tests
lt_simple_link_test_code='int main(){return(0);}\n'

_LT_AC_SYS_COMPILER

#
# Check for any special shared library compilation flags.
#
_LT_AC_TAGVAR(lt_prog_cc_shlib, $1)=
if test "$GCC" = no; then
  case $host_os in
  sco3.2v5*)
    _LT_AC_TAGVAR(lt_prog_cc_shlib, $1)='-belf'
    ;;
  esac
fi
if test -n "$_LT_AC_TAGVAR(lt_prog_cc_shlib, $1)"; then
  AC_MSG_WARN([`$CC' requires `$_LT_AC_TAGVAR(lt_prog_cc_shlib, $1)' to build shared libraries])
  if echo "$old_CC $old_CFLAGS " | grep "[[ 	]]$]_LT_AC_TAGVAR(lt_prog_cc_shlib, $1)[[[ 	]]" >/dev/null; then :
  else
    AC_MSG_WARN([add `$_LT_AC_TAGVAR(lt_prog_cc_shlib, $1)' to the CC or CFLAGS env variable and reconfigure])
    _LT_AC_TAGVAR(lt_cv_prog_cc_can_build_shared, $1)=no
  fi
fi


#
# Check to make sure the static flag actually works.
#
AC_LIBTOOL_LINKER_OPTION([if $compiler static flag $_LT_AC_TAGVAR(lt_prog_compiler_static, $1) works],
  _LT_AC_TAGVAR(lt_prog_compiler_static_works, $1),
  $_LT_AC_TAGVAR(lt_prog_compiler_static, $1),
  [],
  [_LT_AC_TAGVAR(lt_prog_compiler_static, $1)=])


AC_LIBTOOL_PROG_COMPILER_NO_RTTI($1)
AC_LIBTOOL_PROG_COMPILER_PIC($1)
AC_LIBTOOL_PROG_CC_C_O($1)
AC_LIBTOOL_SYS_HARD_LINK_LOCKS($1)
AC_LIBTOOL_PROG_LD_SHLIBS($1)
AC_LIBTOOL_PROG_LD_HARDCODE_LIBPATH($1)
AC_LIBTOOL_SYS_LIB_STRIP
AC_LIBTOOL_SYS_DYNAMIC_LINKER($1)
AC_LIBTOOL_DLOPEN_SELF($1)

# Report which librarie types wil actually be built
AC_MSG_CHECKING([if libtool supports shared libraries])
AC_MSG_RESULT([$can_build_shared])

AC_MSG_CHECKING([whether to build shared libraries])
test "$can_build_shared" = "no" && enable_shared=no

# On AIX, shared libraries and static libraries use the same namespace, and
# are all built from PIC.
case "$host_os" in
aix3*)
  test "$enable_shared" = yes && enable_static=no
  if test -n "$RANLIB"; then
    archive_cmds="$archive_cmds\${_S_}\$RANLIB \$lib"
    postinstall_cmds='$RANLIB $lib'
  fi
  ;;

aix4*)
  if test "$host_cpu" != ia64 && test "$aix_use_runtimelinking" = no ; then
    test "$enable_shared" = yes && enable_static=no
  fi
  ;;
  darwin* | rhapsody*)
  if $CC -v 2>&1 | grep 'Apple' >/dev/null ; then 
    _LT_AC_TAGVAR(archive_cmds_need_lc, $1)=no
    case "$host_os" in
    rhapsody* | darwin1.[[012]])
      _LT_AC_TAGVAR(allow_undefined_flag, $1)='-undefined suppress'
      ;;
    darwin1.* | darwin[[2-6]].*) # Darwin 1.3 on, but less than 7.0
      test -z ${LD_TWOLEVEL_NAMESPACE} && _LT_AC_TAGVAR(allow_undefined_flag, $1)='-flat_namespace -undefined suppress'
      ;;
    *) # Darwin 7.0 on
      case "${MACOSX_DEPLOYMENT_TARGET-10.1}" in
      10.[[012]])
	test -z ${LD_TWOLEVEL_NAMESPACE} && _LT_AC_TAGVAR(allow_undefined_flag, $1)='-flat_namespace -undefined suppress'
	;;
      *) # 10.3 on
	if test -z ${LD_TWOLEVEL_NAMESPACE}; then
	  _LT_AC_TAGVAR(allow_undefined_flag, $1)='-flat_namespace -undefined suppress'
	else
	  _LT_AC_TAGVAR(allow_undefined_flag, $1)='-undefined dynamic_lookup'
	fi
	;;
      esac
      ;;
    esac
    # FIXME: Relying on posixy $() will cause problems for
    #        cross-compilation, but unfortunately the echo tests do not
    #        yet detect zsh echo's removal of \ escapes.  Also zsh mangles
    #	       `"' quotes if we put them in here... so don't!
		output_verbose_link_cmd='echo'
    _LT_AC_TAGVAR(archive_cmds, $1)='$CC -dynamiclib $archargs $allow_undefined_flag -o $lib $libobjs $deplibs$compiler_flags -install_name $rpath/$soname $verstring'
    _LT_AC_TAGVAR(module_cmds, $1)='$CC -bundle $archargs $allow_undefined_flag -o $lib $libobjs $deplibs$compiler_flags'
    # Don't fix this by using the ld -exported_symbols_list flag, it doesn't exist in older darwin ld's
		  _LT_AC_TAGVAR(archive_expsym_cmds, $1)='sed -e "s,#.*,," -e "s,^[    ]*,," -e "s,^\(..*\),_&," < $export_symbols > $output_objdir/${libname}-symbols.expsym${_S_}$CC -dynamiclib $archargs $allow_undefined_flag  -o $lib $libobjs $deplibs$compiler_flags -install_name $rpath/$soname $verstring${_S_}nmedit -s $output_objdir/${libname}-symbols.expsym ${lib}'
		  _LT_AC_TAGVAR(module_expsym_cmds, $1)='sed -e "s,#.*,," -e "s,^[    ]*,," -e "s,^\(..*\),_&," < $export_symbols > $output_objdir/${libname}-symbols.expsym${_S_}$CC -bundle $archargs $allow_undefined_flag  -o $lib $libobjs $deplibs$compiler_flags${_S_}nmedit -s $output_objdir/${libname}-symbols.expsym ${lib}'
    _LT_AC_TAGVAR(hardcode_direct, $1)=no
    _LT_AC_TAGVAR(hardcode_automatic, $1)=yes
    _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=unsupported
    _LT_AC_TAGVAR(whole_archive_flag_spec, $1)='-all_load $convenience'
    _LT_AC_TAGVAR(link_all_deplibs, $1)=yes
    fi
    ;;  
esac
AC_MSG_RESULT([$enable_shared])

AC_MSG_CHECKING([whether to build static libraries])
# Make sure either enable_shared or enable_static is yes.
test "$enable_shared" = yes || enable_static=yes
AC_MSG_RESULT([$enable_static])

AC_LIBTOOL_CONFIG($1)

AC_LANG_POP
CC="$lt_save_CC"
])# AC_LIBTOOL_LANG_C_CONFIG


# AC_LIBTOOL_LANG_CXX_CONFIG
# --------------------------
# Ensure that the configuration vars for the C compiler are
# suitably defined.  Those variables are subsequently used by
# AC_LIBTOOL_CONFIG to write the compiler configuration to `libtool'.
AC_DEFUN([AC_LIBTOOL_LANG_CXX_CONFIG], [_LT_AC_LANG_CXX_CONFIG(CXX)])
AC_DEFUN([_LT_AC_LANG_CXX_CONFIG],
[AC_LANG_PUSH(C++)
AC_REQUIRE([AC_PROG_CXX])
AC_REQUIRE([AC_PROG_CXXCPP])

_LT_AC_TAGVAR(archive_cmds_need_lc, $1)=no
_LT_AC_TAGVAR(allow_undefined_flag, $1)=
_LT_AC_TAGVAR(always_export_symbols, $1)=no
_LT_AC_TAGVAR(archive_expsym_cmds, $1)=
_LT_AC_TAGVAR(export_dynamic_flag_spec, $1)=
_LT_AC_TAGVAR(hardcode_direct, $1)=no
_LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)=
_LT_AC_TAGVAR(hardcode_libdir_flag_spec_ld, $1)=
_LT_AC_TAGVAR(hardcode_libdir_separator, $1)=
_LT_AC_TAGVAR(hardcode_minus_L, $1)=no
_LT_AC_TAGVAR(hardcode_automatic, $1)=no
_LT_AC_TAGVAR(module_cmds, $1)=
_LT_AC_TAGVAR(module_expsym_cmds, $1)=
_LT_AC_TAGVAR(link_all_deplibs, $1)=unknown
_LT_AC_TAGVAR(old_archive_cmds, $1)=$old_archive_cmds
_LT_AC_TAGVAR(no_undefined_flag, $1)=
_LT_AC_TAGVAR(whole_archive_flag_spec, $1)=
_LT_AC_TAGVAR(enable_shared_with_static_runtimes, $1)=no

# Dependencies to place before and after the object being linked:
_LT_AC_TAGVAR(predep_objects, $1)=
_LT_AC_TAGVAR(postdep_objects, $1)=
_LT_AC_TAGVAR(predeps, $1)=
_LT_AC_TAGVAR(postdeps, $1)=
_LT_AC_TAGVAR(compiler_lib_search_path, $1)=

# Source file extension for C++ test sources.
ac_ext=cc

# Object file extension for compiled C++ test sources.
objext=o
_LT_AC_TAGVAR(objext, $1)=$objext

# Code to be used in simple compile tests
lt_simple_compile_test_code="int some_variable = 0;\n"

# Code to be used in simple link tests
lt_simple_link_test_code='int main(int, char *[]) { return(0); }\n'

# ltmain only uses $CC for tagged configurations so make sure $CC is set.
_LT_AC_SYS_COMPILER

# Allow CC to be a program name with arguments.
lt_save_CC=$CC
lt_save_LD=$LD
lt_save_GCC=$GCC
GCC=$GXX
lt_save_with_gnu_ld=$with_gnu_ld
lt_save_path_LD=$lt_cv_path_LD
if test -n "${lt_cv_prog_gnu_ldcxx+set}"; then
  lt_cv_prog_gnu_ld=$lt_cv_prog_gnu_ldcxx
else
  unset lt_cv_prog_gnu_ld
fi
if test -n "${lt_cv_path_LDCXX+set}"; then
  lt_cv_path_LD=$lt_cv_path_LDCXX
else
  unset lt_cv_path_LD
fi
test -z "${LDCXX+set}" || LD=$LDCXX
CC=${CXX-"c++"}
compiler=$CC
_LT_AC_TAGVAR(compiler, $1)=$CC
cc_basename=`$echo X"$compiler" | $Xsed -e 's%^.*/%%'`

# We don't want -fno-exception wen compiling C++ code, so set the
# no_builtin_flag separately
if test "$GXX" = yes; then
  _LT_AC_TAGVAR(lt_prog_compiler_no_builtin_flag, $1)=' -fno-builtin'
else
  _LT_AC_TAGVAR(lt_prog_compiler_no_builtin_flag, $1)=
fi

if test "$GXX" = yes; then
  # Set up default GNU C++ configuration

  AC_PROG_LD

  # Check if GNU C++ uses GNU ld as the underlying linker, since the
  # archiving commands below assume that GNU ld is being used.
  if test "$with_gnu_ld" = yes; then
    _LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared -nostdlib $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags ${wl}-soname $wl$soname -o $lib'
    _LT_AC_TAGVAR(archive_expsym_cmds, $1)='$CC -shared -nostdlib $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags ${wl}-soname $wl$soname ${wl}-retain-symbols-file $wl$export_symbols -o $lib'

    _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}--rpath ${wl}$libdir'
    _LT_AC_TAGVAR(export_dynamic_flag_spec, $1)='${wl}--export-dynamic'

    # If archive_cmds runs LD, not CC, wlarc should be empty
    # XXX I think wlarc can be eliminated in ltcf-cxx, but I need to
    #     investigate it a little bit more. (MM)
    wlarc='${wl}'

    # ancient GNU ld didn't support --whole-archive et. al.
    if eval "`$CC -print-prog-name=ld` --help 2>&1" | \
	grep 'no-whole-archive' > /dev/null; then
      _LT_AC_TAGVAR(whole_archive_flag_spec, $1)="$wlarc"'--whole-archive$convenience '"$wlarc"'--no-whole-archive'
    else
      _LT_AC_TAGVAR(whole_archive_flag_spec, $1)=
    fi
  else
    with_gnu_ld=no
    wlarc=

    # A generic and very simple default shared library creation
    # command for GNU C++ for the case where it uses the native
    # linker, instead of GNU ld.  If possible, this setting should
    # overridden to take advantage of the native linker features on
    # the platform it is being used on.
    _LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags -o $lib'
  fi

  # Commands to make compiler produce verbose output that lists
  # what "hidden" libraries, object files and flags are used when
  # linking a shared library.
  output_verbose_link_cmd='$CC -shared $CFLAGS -v conftest.$objext 2>&1 | grep "\-L"'

else
  GXX=no
  with_gnu_ld=no
  wlarc=
fi

# PORTME: fill in a description of your system's C++ link characteristics
AC_MSG_CHECKING([whether the $compiler linker ($LD) supports shared libraries])
_LT_AC_TAGVAR(ld_shlibs, $1)=yes
case $host_os in
  aix3*)
    # FIXME: insert proper C++ library support
    _LT_AC_TAGVAR(ld_shlibs, $1)=no
    ;;
  aix4* | aix5*)
    if test "$host_cpu" = ia64; then
      # On IA64, the linker does run time linking by default, so we don't
      # have to do anything special.
      aix_use_runtimelinking=no
      exp_sym_flag='-Bexport'
      no_entry_flag=""
    else
      aix_use_runtimelinking=no

      # Test if we are trying to use run time linking or normal
      # AIX style linking. If -brtl is somewhere in LDFLAGS, we
      # need to do runtime linking.
      case $host_os in aix4.[[23]]|aix4.[[23]].*|aix5*)
	for ld_flag in $LDFLAGS; do
	  case $ld_flag in
	  *-brtl*)
	    aix_use_runtimelinking=yes
	    break
	    ;;
	  esac
	done
      esac

      exp_sym_flag='-bexport'
      no_entry_flag='-bnoentry'
    fi

    # When large executables or shared objects are built, AIX ld can
    # have problems creating the table of contents.  If linking a library
    # or program results in "error TOC overflow" add -mminimal-toc to
    # CXXFLAGS/CFLAGS for g++/gcc.  In the cases where that is not
    # enough to fix the problem, add -Wl,-bbigtoc to LDFLAGS.

    _LT_AC_TAGVAR(archive_cmds, $1)=''
    _LT_AC_TAGVAR(hardcode_direct, $1)=yes
    _LT_AC_TAGVAR(hardcode_libdir_separator, $1)=':'
    _LT_AC_TAGVAR(link_all_deplibs, $1)=yes

    if test "$GXX" = yes; then
      case $host_os in aix4.[012]|aix4.[012].*)
      # We only want to do this on AIX 4.2 and lower, the check
      # below for broken collect2 doesn't work under 4.3+
	collect2name=`${CC} -print-prog-name=collect2`
	if test -f "$collect2name" && \
	   strings "$collect2name" | grep resolve_lib_name >/dev/null
	then
	  # We have reworked collect2
	  _LT_AC_TAGVAR(hardcode_direct, $1)=yes
	else
	  # We have old collect2
	  _LT_AC_TAGVAR(hardcode_direct, $1)=unsupported
	  # It fails to find uninstalled libraries when the uninstalled
	  # path is not listed in the libpath.  Setting hardcode_minus_L
	  # to unsupported forces relinking
	  _LT_AC_TAGVAR(hardcode_minus_L, $1)=yes
	  _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='-L$libdir'
	  _LT_AC_TAGVAR(hardcode_libdir_separator, $1)=
	fi
      esac
      shared_flag='-shared'
    else
      # not using gcc
      if test "$host_cpu" = ia64; then
	# VisualAge C++, Version 5.5 for AIX 5L for IA-64, Beta 3 Release
	# chokes on -Wl,-G. The following line is correct:
	shared_flag='-G'
      else
	if test "$aix_use_runtimelinking" = yes; then
	  shared_flag='${wl}-G'
	else
	  shared_flag='${wl}-bM:SRE'
	fi
      fi
    fi

    # It seems that -bexpall does not export symbols beginning with
    # underscore (_), so it is better to generate a list of symbols to export.
    _LT_AC_TAGVAR(always_export_symbols, $1)=yes
    if test "$aix_use_runtimelinking" = yes; then
      # Warning - without using the other runtime loading flags (-brtl),
      # -berok will link without error, but may produce a broken library.
      _LT_AC_TAGVAR(allow_undefined_flag, $1)='-berok'
      # Determine the default libpath from the value encoded in an empty executable.
      _LT_AC_SYS_LIBPATH_AIX
      _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}-blibpath:$libdir:'"$aix_libpath"

      _LT_AC_TAGVAR(archive_expsym_cmds, $1)="\$CC"' -o $output_objdir/$soname $libobjs $deplibs $compiler_flags `if test "x${allow_undefined_flag}" != "x"; then echo "${wl}${allow_undefined_flag}"; else :; fi` '"\${wl}$no_entry_flag \${wl}$exp_sym_flag:\$export_symbols $shared_flag"
     else
      if test "$host_cpu" = ia64; then
	_LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}-R $libdir:/usr/lib:/lib'
	_LT_AC_TAGVAR(allow_undefined_flag, $1)="-z nodefs"
	_LT_AC_TAGVAR(archive_expsym_cmds, $1)="\$CC $shared_flag"' -o $output_objdir/$soname $libobjs $deplibs $compiler_flags ${wl}${allow_undefined_flag} '"\${wl}$no_entry_flag \${wl}$exp_sym_flag:\$export_symbols"
      else
	# Determine the default libpath from the value encoded in an empty executable.
	_LT_AC_SYS_LIBPATH_AIX
	_LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}-blibpath:$libdir:'"$aix_libpath"
	# Warning - without using the other run time loading flags,
	# -berok will link without error, but may produce a broken library.
	_LT_AC_TAGVAR(no_undefined_flag, $1)=' ${wl}-bernotok'
	_LT_AC_TAGVAR(allow_undefined_flag, $1)=' ${wl}-berok'
	# -bexpall does not export symbols beginning with underscore (_)
	_LT_AC_TAGVAR(always_export_symbols, $1)=yes
	# Exported symbols can be pulled into shared objects from archives
	_LT_AC_TAGVAR(whole_archive_flag_spec, $1)=' '
	_LT_AC_TAGVAR(archive_cmds_need_lc, $1)=yes
	# This is similar to how AIX traditionally builds it's shared libraries.
	_LT_AC_TAGVAR(archive_expsym_cmds, $1)="\$CC $shared_flag"' -o $output_objdir/$soname $libobjs $deplibs $compiler_flags ${wl}-bE:$export_symbols ${wl}-bnoentry${allow_undefined_flag}\${_S_}$AR $AR_FLAGS $output_objdir/$libname$release.a $output_objdir/$soname'
      fi
    fi
    ;;
  chorus*)
    case $cc_basename in
      *)
	# FIXME: insert proper C++ library support
	_LT_AC_TAGVAR(ld_shlibs, $1)=no
	;;
    esac
    ;;

  cygwin* | mingw* | pw32*)
    # _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1) is actually meaningless,
    # as there is no search path for DLLs.
    _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='-L$libdir'
    _LT_AC_TAGVAR(allow_undefined_flag, $1)=unsupported
    _LT_AC_TAGVAR(always_export_symbols, $1)=no
    _LT_AC_TAGVAR(enable_shared_with_static_runtimes, $1)=yes

    if $LD --help 2>&1 | grep 'auto-import' > /dev/null; then
      _LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared -nostdlib $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags -o $output_objdir/$soname ${wl}--image-base=0x10000000 ${wl}--out-implib,$lib'
      # If the export-symbols file already is a .def file (1st line
      # is EXPORTS), use it as is; otherwise, prepend...
      _LT_AC_TAGVAR(archive_expsym_cmds, $1)='if test "x`$SED 1q $export_symbols`" = xEXPORTS; then
	cp $export_symbols $output_objdir/$soname.def;
      else
	echo EXPORTS > $output_objdir/$soname.def;
	cat $export_symbols >> $output_objdir/$soname.def;
      fi${_S_}
      $CC -shared -nostdlib $output_objdir/$soname.def $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags -o $output_objdir/$soname ${wl}--image-base=0x10000000 ${wl}--out-implib,$lib'
    else
      _LT_AC_TAGVAR(ld_shlibs, $1)=no
    fi
  ;;

  darwin* | rhapsody*)
   if $CC -v 2>&1 | grep 'Apple' >/dev/null ; then 
    _LT_AC_TAGVAR(archive_cmds_need_lc, $1)=no
    case "$host_os" in
    rhapsody* | darwin1.[[012]])
      _LT_AC_TAGVAR(allow_undefined_flag, $1)='-undefined suppress'
      ;;
    darwin1.* | darwin[[2-6]].*) # Darwin 1.3 on, but less than 7.0
      test -z ${LD_TWOLEVEL_NAMESPACE} && _LT_AC_TAGVAR(allow_undefined_flag, $1)='-flat_namespace -undefined suppress'
      ;;
    *) # Darwin 7.0 on
      case "${MACOSX_DEPLOYMENT_TARGET-10.1}" in
      10.[[012]])
	test -z ${LD_TWOLEVEL_NAMESPACE} && _LT_AC_TAGVAR(allow_undefined_flag, $1)='-flat_namespace -undefined suppress'
	;;
      *) # 10.3 on
	if test -z ${LD_TWOLEVEL_NAMESPACE}; then
	  _LT_AC_TAGVAR(allow_undefined_flag, $1)='-flat_namespace -undefined suppress'
	else
	  _LT_AC_TAGVAR(allow_undefined_flag, $1)='-undefined dynamic_lookup'
	fi
	;;
      esac
      ;;
    esac
    	lt_int_apple_cc_single_mod=no
    	output_verbose_link_cmd='echo'
    	if $CC -dumpspecs 2>&1 | grep 'single_module' >/dev/null ; then
    	  lt_int_apple_cc_single_mod=yes
    	fi
    	if test "X$lt_int_apple_cc_single_mod" = Xyes ; then
    	  _LT_AC_TAGVAR(archive_cmds, $1)='$CC -dynamiclib $archargs -single_module $allow_undefined_flag -o $lib $libobjs $deplibs $compiler_flags -install_name $rpath/$soname $verstring'
    	else
        _LT_AC_TAGVAR(archive_cmds, $1)='$CC -r ${wl}-bind_at_load -keep_private_externs -nostdlib -o ${lib}-master.o $libobjs${_S_}$CC -dynamiclib $archargs $allow_undefined_flag -o $lib ${lib}-master.o $deplibs $compiler_flags -install_name $rpath/$soname $verstring'
      fi
      _LT_AC_TAGVAR(module_cmds, $1)='$CC -bundle $archargs ${wl}-bind_at_load $allow_undefined_flag -o $lib $libobjs $deplibs$compiler_flags'

    # Don't fix this by using the ld -exported_symbols_list flag, it doesn't exist in older darwin ld's
      if test "X$lt_int_apple_cc_single_mod" = Xyes ; then
        _LT_AC_TAGVAR(archive_expsym_cmds, $1)='sed -e "s,#.*,," -e "s,^[    ]*,," -e "s,^\(..*\),_&," < $export_symbols > $output_objdir/${libname}-symbols.expsym${_S_}$CC -dynamiclib $archargs -single_module $allow_undefined_flag -o $lib $libobjs $deplibs $compiler_flags -install_name $rpath/$soname $verstring${_S_}nmedit -s $output_objdir/${libname}-symbols.expsym ${lib}'
      else
        _LT_AC_TAGVAR(archive_expsym_cmds, $1)='sed -e "s,#.*,," -e "s,^[    ]*,," -e "s,^\(..*\),_&," < $export_symbols > $output_objdir/${libname}-symbols.expsym${_S_}$CC -r ${wl}-bind_at_load -keep_private_externs -nostdlib -o ${lib}-master.o $libobjs${_S_}$CC -dynamiclib $archargs $allow_undefined_flag -o $lib ${lib}-master.o $deplibs $compiler_flags -install_name $rpath/$soname $verstring${_S_}nmedit -s $output_objdir/${libname}-symbols.expsym ${lib}'
      fi
        _LT_AC_TAGVAR(module_expsym_cmds, $1)='sed -e "s,#.*,," -e "s,^[    ]*,," -e "s,^\(..*\),_&," < $export_symbols > $output_objdir/${libname}-symbols.expsym${_S_}$CC -bundle $archargs $allow_undefined_flag  -o $lib $libobjs $deplibs$compiler_flags${_S_}nmedit -s $output_objdir/${libname}-symbols.expsym ${lib}'
    _LT_AC_TAGVAR(hardcode_direct, $1)=no
    _LT_AC_TAGVAR(hardcode_automatic, $1)=yes
    _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=unsupported
    _LT_AC_TAGVAR(whole_archive_flag_spec, $1)='-all_load $convenience'
    _LT_AC_TAGVAR(link_all_deplibs, $1)=yes
   fi 
    ;;

  dgux*)
    case $cc_basename in
      ec++)
	# FIXME: insert proper C++ library support
	_LT_AC_TAGVAR(ld_shlibs, $1)=no
	;;
      ghcx)
	# Green Hills C++ Compiler
	# FIXME: insert proper C++ library support
	_LT_AC_TAGVAR(ld_shlibs, $1)=no
	;;
      *)
	# FIXME: insert proper C++ library support
	_LT_AC_TAGVAR(ld_shlibs, $1)=no
	;;
    esac
    ;;
  freebsd[12]*)
    # C++ shared libraries reported to be fairly broken before switch to ELF
    _LT_AC_TAGVAR(ld_shlibs, $1)=no
    ;;
  freebsd-elf*)
    _LT_AC_TAGVAR(archive_cmds_need_lc, $1)=no
    ;;
  freebsd*)
    # FreeBSD 3 and later use GNU C++ and GNU ld with standard ELF
    # conventions
    _LT_AC_TAGVAR(ld_shlibs, $1)=yes
    ;;
  gnu*)
    ;;
  hpux9*)
    _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}+b ${wl}$libdir'
    _LT_AC_TAGVAR(hardcode_libdir_separator, $1)=:
    _LT_AC_TAGVAR(export_dynamic_flag_spec, $1)='${wl}-E'
    _LT_AC_TAGVAR(hardcode_direct, $1)=yes
    _LT_AC_TAGVAR(hardcode_minus_L, $1)=yes # Not in the search PATH,
				# but as the default
				# location of the library.

    case $cc_basename in
    CC)
      # FIXME: insert proper C++ library support
      _LT_AC_TAGVAR(ld_shlibs, $1)=no
      ;;
    aCC)
      _LT_AC_TAGVAR(archive_cmds, $1)='$rm $output_objdir/$soname${_S_}$CC -b ${wl}+b ${wl}$install_libdir -o $output_objdir/$soname $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags${_S_}test $output_objdir/$soname = $lib || mv $output_objdir/$soname $lib'
      # Commands to make compiler produce verbose output that lists
      # what "hidden" libraries, object files and flags are used when
      # linking a shared library.
      #
      # There doesn't appear to be a way to prevent this compiler from
      # explicitly linking system object files so we need to strip them
      # from the output so that they don't get included in the library
      # dependencies.
      output_verbose_link_cmd='templist=`($CC -b $CFLAGS -v conftest.$objext 2>&1) | egrep "\-L"`; list=""; for z in $templist; do case $z in conftest.$objext) list="$list $z";; *.$objext);; *) list="$list $z";;esac; done; echo $list'
      ;;
    *)
      if test "$GXX" = yes; then
        _LT_AC_TAGVAR(archive_cmds, $1)='$rm $output_objdir/$soname${_S_}$CC -shared -nostdlib -fPIC ${wl}+b ${wl}$install_libdir -o $output_objdir/$soname $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags${_S_}test $output_objdir/$soname = $lib || mv $output_objdir/$soname $lib'
      else
        # FIXME: insert proper C++ library support
        _LT_AC_TAGVAR(ld_shlibs, $1)=no
      fi
      ;;
    esac
    ;;
  hpux10*|hpux11*)
    if test $with_gnu_ld = no; then
      case "$host_cpu" in
      hppa*64*)
	_LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}+b ${wl}$libdir'
	_LT_AC_TAGVAR(hardcode_libdir_flag_spec_ld, $1)='+b $libdir'
	_LT_AC_TAGVAR(hardcode_libdir_separator, $1)=:
        ;;
      ia64*)
	_LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='-L$libdir'
        ;;
      *)
	_LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}+b ${wl}$libdir'
	_LT_AC_TAGVAR(hardcode_libdir_separator, $1)=:
	_LT_AC_TAGVAR(export_dynamic_flag_spec, $1)='${wl}-E'
        ;;
      esac
    fi
    case "$host_cpu" in
    hppa*64*)
      _LT_AC_TAGVAR(hardcode_direct, $1)=no
      _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=no
      ;;
    ia64*)
      _LT_AC_TAGVAR(hardcode_direct, $1)=no
      _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=no
      _LT_AC_TAGVAR(hardcode_minus_L, $1)=yes # Not in the search PATH,
					      # but as the default
					      # location of the library.
      ;;
    *)
      _LT_AC_TAGVAR(hardcode_direct, $1)=yes
      _LT_AC_TAGVAR(hardcode_minus_L, $1)=yes # Not in the search PATH,
					      # but as the default
					      # location of the library.
      ;;
    esac

    case $cc_basename in
      CC)
	# FIXME: insert proper C++ library support
	_LT_AC_TAGVAR(ld_shlibs, $1)=no
	;;
      aCC)
	case "$host_cpu" in
	hppa*64*|ia64*)
	  _LT_AC_TAGVAR(archive_cmds, $1)='$LD -b +h $soname -o $lib $linker_flags $libobjs $deplibs'
	  ;;
	*)
	  _LT_AC_TAGVAR(archive_cmds, $1)='$CC -b ${wl}+h ${wl}$soname ${wl}+b ${wl}$install_libdir -o $lib $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags'
	  ;;
	esac
	# Commands to make compiler produce verbose output that lists
	# what "hidden" libraries, object files and flags are used when
	# linking a shared library.
	#
	# There doesn't appear to be a way to prevent this compiler from
	# explicitly linking system object files so we need to strip them
	# from the output so that they don't get included in the library
	# dependencies.
	output_verbose_link_cmd='templist=`($CC -b $CFLAGS -v conftest.$objext 2>&1) | grep "\-L"`; list=""; for z in $templist; do case $z in conftest.$objext) list="$list $z";; *.$objext);; *) list="$list $z";;esac; done; echo $list'
	;;
      *)
	if test "$GXX" = yes; then
	  if test $with_gnu_ld = no; then
	    case "$host_cpu" in
	    ia64*|hppa*64*)
	      _LT_AC_TAGVAR(archive_cmds, $1)='$LD -b +h $soname -o $lib $linker_flags $libobjs $deplibs'
	      ;;
	    *)
	      _LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared -nostdlib -fPIC ${wl}+h ${wl}$soname ${wl}+b ${wl}$install_libdir -o $lib $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags'
	      ;;
	    esac
	  fi
	else
	  # FIXME: insert proper C++ library support
	  _LT_AC_TAGVAR(ld_shlibs, $1)=no
	fi
	;;
    esac
    ;;
  irix5* | irix6*)
    case $cc_basename in
      CC)
	# SGI C++
	_LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared -all -multigot $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags -soname $soname `test -n "$verstring" && echo -set_version $verstring` -update_registry ${objdir}/so_locations -o $lib'

	# Archives containing C++ object files must be created using
	# "CC -ar", where "CC" is the IRIX C++ compiler.  This is
	# necessary to make sure instantiated templates are included
	# in the archive.
	_LT_AC_TAGVAR(old_archive_cmds, $1)='$CC -ar -WR,-u -o $oldlib $oldobjs'
	;;
      *)
	if test "$GXX" = yes; then
	  if test "$with_gnu_ld" = no; then
	    _LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared -nostdlib $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags ${wl}-soname ${wl}$soname `test -n "$verstring" && echo ${wl}-set_version ${wl}$verstring` ${wl}-update_registry ${wl}${objdir}/so_locations -o $lib'
	  else
	    _LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared -nostdlib $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags ${wl}-soname ${wl}$soname `test -n "$verstring" && echo ${wl}-set_version ${wl}$verstring` -o $lib'
	  fi
	fi
	_LT_AC_TAGVAR(link_all_deplibs, $1)=yes
	;;
    esac
    _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}-rpath ${wl}$libdir'
    _LT_AC_TAGVAR(hardcode_libdir_separator, $1)=:
    ;;
  linux*)
    case $cc_basename in
      KCC)
	# Kuck and Associates, Inc. (KAI) C++ Compiler

	# KCC will only create a shared library if the output file
	# ends with ".so" (or ".sl" for HP-UX), so rename the library
	# to its proper name (with version) after linking.
	_LT_AC_TAGVAR(archive_cmds, $1)='tempext=`echo $shared_ext | $SED -e '\''s/\([[^()0-9A-Za-z{}]]\)/\\\\\1/g'\''`; templib=`echo $lib | $SED -e "s/\${tempext}\..*/.so/"`; $CC $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags --soname $soname -o \$templib; mv \$templib $lib'
	_LT_AC_TAGVAR(archive_expsym_cmds, $1)='tempext=`echo $shared_ext | $SED -e '\''s/\([[^()0-9A-Za-z{}]]\)/\\\\\1/g'\''`; templib=`echo $lib | $SED -e "s/\${tempext}\..*/.so/"`; $CC $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags --soname $soname -o \$templib ${wl}-retain-symbols-file,$export_symbols; mv \$templib $lib'
	# Commands to make compiler produce verbose output that lists
	# what "hidden" libraries, object files and flags are used when
	# linking a shared library.
	#
	# There doesn't appear to be a way to prevent this compiler from
	# explicitly linking system object files so we need to strip them
	# from the output so that they don't get included in the library
	# dependencies.
	output_verbose_link_cmd='templist=`$CC $CFLAGS -v conftest.$objext -o libconftest$shared_ext 2>&1 | grep "ld"`; rm -f libconftest$shared_ext; list=""; for z in $templist; do case $z in conftest.$objext) list="$list $z";; *.$objext);; *) list="$list $z";;esac; done; echo $list'

	_LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}--rpath,$libdir'
	_LT_AC_TAGVAR(export_dynamic_flag_spec, $1)='${wl}--export-dynamic'

	# Archives containing C++ object files must be created using
	# "CC -Bstatic", where "CC" is the KAI C++ compiler.
	_LT_AC_TAGVAR(old_archive_cmds, $1)='$CC -Bstatic -o $oldlib $oldobjs'
	;;
      icpc)
	# Intel C++
	with_gnu_ld=yes
	_LT_AC_TAGVAR(archive_cmds_need_lc, $1)=no
	_LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags ${wl}-soname $wl$soname -o $lib'
	_LT_AC_TAGVAR(archive_expsym_cmds, $1)='$CC -shared $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags ${wl}-soname $wl$soname ${wl}-retain-symbols-file $wl$export_symbols -o $lib'
	_LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}-rpath,$libdir'
	_LT_AC_TAGVAR(export_dynamic_flag_spec, $1)='${wl}--export-dynamic'
	_LT_AC_TAGVAR(whole_archive_flag_spec, $1)='${wl}--whole-archive$convenience ${wl}--no-whole-archive'
	;;
      cxx)
	# Compaq C++
	_LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags ${wl}-soname $wl$soname -o $lib'
	_LT_AC_TAGVAR(archive_expsym_cmds, $1)='$CC -shared $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags ${wl}-soname $wl$soname  -o $lib ${wl}-retain-symbols-file $wl$export_symbols'

	runpath_var=LD_RUN_PATH
	_LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='-rpath $libdir'
	_LT_AC_TAGVAR(hardcode_libdir_separator, $1)=:

	# Commands to make compiler produce verbose output that lists
	# what "hidden" libraries, object files and flags are used when
	# linking a shared library.
	#
	# There doesn't appear to be a way to prevent this compiler from
	# explicitly linking system object files so we need to strip them
	# from the output so that they don't get included in the library
	# dependencies.
	output_verbose_link_cmd='templist=`$CC -shared $CFLAGS -v conftest.$objext 2>&1 | grep "ld"`; templist=`echo $templist | $SED "s/\(^.*ld.*\)\( .*ld .*$\)/\1/"`; list=""; for z in $templist; do case $z in conftest.$objext) list="$list $z";; *.$objext);; *) list="$list $z";;esac; done; echo $list'
	;;
    esac
    ;;
  lynxos*)
    # FIXME: insert proper C++ library support
    _LT_AC_TAGVAR(ld_shlibs, $1)=no
    ;;
  m88k*)
    # FIXME: insert proper C++ library support
    _LT_AC_TAGVAR(ld_shlibs, $1)=no
    ;;
  mvs*)
    case $cc_basename in
      cxx)
	# FIXME: insert proper C++ library support
	_LT_AC_TAGVAR(ld_shlibs, $1)=no
	;;
      *)
	# FIXME: insert proper C++ library support
	_LT_AC_TAGVAR(ld_shlibs, $1)=no
	;;
    esac
    ;;
  netbsd*)
    if echo __ELF__ | $CC -E - | grep __ELF__ >/dev/null; then
      _LT_AC_TAGVAR(archive_cmds, $1)='$LD -Bshareable  -o $lib $predep_objects $libobjs $deplibs $postdep_objects $linker_flags'
      wlarc=
      _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='-R$libdir'
      _LT_AC_TAGVAR(hardcode_direct, $1)=yes
      _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=no
    fi
    # Workaround some broken pre-1.5 toolchains
    output_verbose_link_cmd='$CC -shared $CFLAGS -v conftest.$objext 2>&1 | grep conftest.$objext | $SED -e "s:-lgcc -lc -lgcc::"'
    ;;
  osf3*)
    case $cc_basename in
      KCC)
	# Kuck and Associates, Inc. (KAI) C++ Compiler

	# KCC will only create a shared library if the output file
	# ends with ".so" (or ".sl" for HP-UX), so rename the library
	# to its proper name (with version) after linking.
	_LT_AC_TAGVAR(archive_cmds, $1)='tempext=`echo $shared_ext | $SED -e '\''s/\([[^()0-9A-Za-z{}]]\)/\\\\\1/g'\''`; templib=`echo $lib | $SED -e "s/\${tempext}\..*/.so/"`; $CC $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags --soname $soname -o \$templib; mv \$templib $lib'

	_LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}-rpath,$libdir'
	_LT_AC_TAGVAR(hardcode_libdir_separator, $1)=:

	# Archives containing C++ object files must be created using
	# "CC -Bstatic", where "CC" is the KAI C++ compiler.
	_LT_AC_TAGVAR(old_archive_cmds, $1)='$CC -Bstatic -o $oldlib $oldobjs'

	;;
      RCC)
	# Rational C++ 2.4.1
	# FIXME: insert proper C++ library support
	_LT_AC_TAGVAR(ld_shlibs, $1)=no
	;;
      cxx)
	_LT_AC_TAGVAR(allow_undefined_flag, $1)=' ${wl}-expect_unresolved ${wl}\*'
	_LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared${allow_undefined_flag} $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags ${wl}-soname $soname `test -n "$verstring" && echo ${wl}-set_version $verstring` -update_registry ${objdir}/so_locations -o $lib'

	_LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}-rpath ${wl}$libdir'
	_LT_AC_TAGVAR(hardcode_libdir_separator, $1)=:

	# Commands to make compiler produce verbose output that lists
	# what "hidden" libraries, object files and flags are used when
	# linking a shared library.
	#
	# There doesn't appear to be a way to prevent this compiler from
	# explicitly linking system object files so we need to strip them
	# from the output so that they don't get included in the library
	# dependencies.
	output_verbose_link_cmd='templist=`$CC -shared $CFLAGS -v conftest.$objext 2>&1 | grep "ld" | grep -v "ld:"`; templist=`echo $templist | $SED "s/\(^.*ld.*\)\( .*ld.*$\)/\1/"`; list=""; for z in $templist; do case $z in conftest.$objext) list="$list $z";; *.$objext);; *) list="$list $z";;esac; done; echo $list'
	;;
      *)
	if test "$GXX" = yes && test "$with_gnu_ld" = no; then
	  _LT_AC_TAGVAR(allow_undefined_flag, $1)=' ${wl}-expect_unresolved ${wl}\*'
	  _LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared -nostdlib ${allow_undefined_flag} $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags ${wl}-soname ${wl}$soname `test -n "$verstring" && echo ${wl}-set_version ${wl}$verstring` ${wl}-update_registry ${wl}${objdir}/so_locations -o $lib'

	  _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}-rpath ${wl}$libdir'
	  _LT_AC_TAGVAR(hardcode_libdir_separator, $1)=:

	  # Commands to make compiler produce verbose output that lists
	  # what "hidden" libraries, object files and flags are used when
	  # linking a shared library.
	  output_verbose_link_cmd='$CC -shared $CFLAGS -v conftest.$objext 2>&1 | grep "\-L"'

	else
	  # FIXME: insert proper C++ library support
	  _LT_AC_TAGVAR(ld_shlibs, $1)=no
	fi
	;;
    esac
    ;;
  osf4* | osf5*)
    case $cc_basename in
      KCC)
	# Kuck and Associates, Inc. (KAI) C++ Compiler

	# KCC will only create a shared library if the output file
	# ends with ".so" (or ".sl" for HP-UX), so rename the library
	# to its proper name (with version) after linking.
	_LT_AC_TAGVAR(archive_cmds, $1)='tempext=`echo $shared_ext | $SED -e '\''s/\([[^()0-9A-Za-z{}]]\)/\\\\\1/g'\''`; templib=`echo $lib | $SED -e "s/\${tempext}\..*/.so/"`; $CC $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags --soname $soname -o \$templib; mv \$templib $lib'

	_LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}-rpath,$libdir'
	_LT_AC_TAGVAR(hardcode_libdir_separator, $1)=:

	# Archives containing C++ object files must be created using
	# the KAI C++ compiler.
	_LT_AC_TAGVAR(old_archive_cmds, $1)='$CC -o $oldlib $oldobjs'
	;;
      RCC)
	# Rational C++ 2.4.1
	# FIXME: insert proper C++ library support
	_LT_AC_TAGVAR(ld_shlibs, $1)=no
	;;
      cxx)
	_LT_AC_TAGVAR(allow_undefined_flag, $1)=' -expect_unresolved \*'
	_LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared${allow_undefined_flag} $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags -msym -soname $soname `test -n "$verstring" && echo -set_version $verstring` -update_registry ${objdir}/so_locations -o $lib'
	_LT_AC_TAGVAR(archive_expsym_cmds, $1)='for i in `cat $export_symbols`; do printf "%s %s\\n" -exported_symbol "\$i" >> $lib.exp; done${_S_}
	  echo "-hidden">> $lib.exp${_S_}
	  $CC -shared$allow_undefined_flag $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags -msym -soname $soname -Wl,-input -Wl,$lib.exp  `test -n "$verstring" && echo -set_version	$verstring` -update_registry $objdir/so_locations -o $lib${_S_}
	  $rm $lib.exp'

	_LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='-rpath $libdir'
	_LT_AC_TAGVAR(hardcode_libdir_separator, $1)=:

	# Commands to make compiler produce verbose output that lists
	# what "hidden" libraries, object files and flags are used when
	# linking a shared library.
	#
	# There doesn't appear to be a way to prevent this compiler from
	# explicitly linking system object files so we need to strip them
	# from the output so that they don't get included in the library
	# dependencies.
	output_verbose_link_cmd='templist=`$CC -shared $CFLAGS -v conftest.$objext 2>&1 | grep "ld" | grep -v "ld:"`; templist=`echo $templist | $SED "s/\(^.*ld.*\)\( .*ld.*$\)/\1/"`; list=""; for z in $templist; do case $z in conftest.$objext) list="$list $z";; *.$objext);; *) list="$list $z";;esac; done; echo $list'
	;;
      *)
	if test "$GXX" = yes && test "$with_gnu_ld" = no; then
	  _LT_AC_TAGVAR(allow_undefined_flag, $1)=' ${wl}-expect_unresolved ${wl}\*'
	 _LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared -nostdlib ${allow_undefined_flag} $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags ${wl}-msym ${wl}-soname ${wl}$soname `test -n "$verstring" && echo ${wl}-set_version ${wl}$verstring` ${wl}-update_registry ${wl}${objdir}/so_locations -o $lib'

	  _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}-rpath ${wl}$libdir'
	  _LT_AC_TAGVAR(hardcode_libdir_separator, $1)=:

	  # Commands to make compiler produce verbose output that lists
	  # what "hidden" libraries, object files and flags are used when
	  # linking a shared library.
	  output_verbose_link_cmd='$CC -shared $CFLAGS -v conftest.$objext 2>&1 | grep "\-L"'

	else
	  # FIXME: insert proper C++ library support
	  _LT_AC_TAGVAR(ld_shlibs, $1)=no
	fi
	;;
    esac
    ;;
  psos*)
    # FIXME: insert proper C++ library support
    _LT_AC_TAGVAR(ld_shlibs, $1)=no
    ;;
  sco*)
    _LT_AC_TAGVAR(archive_cmds_need_lc, $1)=no
    case $cc_basename in
      CC)
	# FIXME: insert proper C++ library support
	_LT_AC_TAGVAR(ld_shlibs, $1)=no
	;;
      *)
	# FIXME: insert proper C++ library support
	_LT_AC_TAGVAR(ld_shlibs, $1)=no
	;;
    esac
    ;;
  sunos4*)
    case $cc_basename in
      CC)
	# Sun C++ 4.x
	# FIXME: insert proper C++ library support
	_LT_AC_TAGVAR(ld_shlibs, $1)=no
	;;
      lcc)
	# Lucid
	# FIXME: insert proper C++ library support
	_LT_AC_TAGVAR(ld_shlibs, $1)=no
	;;
      *)
	# FIXME: insert proper C++ library support
	_LT_AC_TAGVAR(ld_shlibs, $1)=no
	;;
    esac
    ;;
  solaris*)
    case $cc_basename in
      CC)
	# Sun C++ 4.2, 5.x and Centerline C++
	_LT_AC_TAGVAR(no_undefined_flag, $1)=' -zdefs'
	_LT_AC_TAGVAR(archive_cmds, $1)='$CC -G${allow_undefined_flag} -nolib -h$soname -o $lib $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags'
	_LT_AC_TAGVAR(archive_expsym_cmds, $1)='$echo "{ global:" > $lib.exp${_S_}cat $export_symbols | $SED -e "s/\(.*\)/\1;/" >> $lib.exp${_S_}$echo "local: *; };" >> $lib.exp${_S_}
	$CC -G${allow_undefined_flag} -nolib ${wl}-M ${wl}$lib.exp -h$soname -o $lib $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags${_S_}$rm $lib.exp'

	_LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='-R$libdir'
	_LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=no
	case $host_os in
	  solaris2.[0-5] | solaris2.[0-5].*) ;;
	  *)
	    # The C++ compiler is used as linker so we must use $wl
	    # flag to pass the commands to the underlying system
	    # linker.
	    # Supported since Solaris 2.6 (maybe 2.5.1?)
	    _LT_AC_TAGVAR(whole_archive_flag_spec, $1)='${wl}-z ${wl}allextract$convenience ${wl}-z ${wl}defaultextract'
	    ;;
	esac
	_LT_AC_TAGVAR(link_all_deplibs, $1)=yes

	# Commands to make compiler produce verbose output that lists
	# what "hidden" libraries, object files and flags are used when
	# linking a shared library.
	#
	# There doesn't appear to be a way to prevent this compiler from
	# explicitly linking system object files so we need to strip them
	# from the output so that they don't get included in the library
	# dependencies.
	output_verbose_link_cmd='templist=`$CC -G $CFLAGS -v conftest.$objext 2>&1 | grep "\-[[LR]]"`; list=""; for z in $templist; do case $z in conftest.$objext) list="$list $z";; *.$objext);; *) list="$list $z";;esac; done; echo $list'

	# Archives containing C++ object files must be created using
	# "CC -xar", where "CC" is the Sun C++ compiler.  This is
	# necessary to make sure instantiated templates are included
	# in the archive.
	_LT_AC_TAGVAR(old_archive_cmds, $1)='$CC -xar -o $oldlib $oldobjs'
	;;
      gcx)
	# Green Hills C++ Compiler
	_LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags ${wl}-h $wl$soname -o $lib'

	# The C++ compiler must be used to create the archive.
	_LT_AC_TAGVAR(old_archive_cmds, $1)='$CC $LDFLAGS -archive -o $oldlib $oldobjs'
	;;
      *)
	# GNU C++ compiler with Solaris linker
	if test "$GXX" = yes && test "$with_gnu_ld" = no; then
	  _LT_AC_TAGVAR(no_undefined_flag, $1)=' ${wl}-z ${wl}defs'
	  if $CC --version | grep -v '^2\.7' > /dev/null; then
	    _LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared -nostdlib $LDFLAGS $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags ${wl}-h $wl$soname -o $lib'
	    _LT_AC_TAGVAR(archive_expsym_cmds, $1)='$echo "{ global:" > $lib.exp${_S_}cat $export_symbols | $SED -e "s/\(.*\)/\1;/" >> $lib.exp${_S_}$echo "local: *; };" >> $lib.exp${_S_}
		$CC -shared -nostdlib ${wl}-M $wl$lib.exp -o $lib $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags${_S_}$rm $lib.exp'

	    # Commands to make compiler produce verbose output that lists
	    # what "hidden" libraries, object files and flags are used when
	    # linking a shared library.
	    output_verbose_link_cmd="$CC -shared $CFLAGS -v conftest.$objext 2>&1 | grep \"\-L\""
	  else
	    # g++ 2.7 appears to require `-G' NOT `-shared' on this
	    # platform.
	    _LT_AC_TAGVAR(archive_cmds, $1)='$CC -G -nostdlib $LDFLAGS $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags ${wl}-h $wl$soname -o $lib'
	    _LT_AC_TAGVAR(archive_expsym_cmds, $1)='$echo "{ global:" > $lib.exp${_S_}cat $export_symbols | $SED -e "s/\(.*\)/\1;/" >> $lib.exp${_S_}$echo "local: *; };" >> $lib.exp${_S_}
		$CC -G -nostdlib ${wl}-M $wl$lib.exp -o $lib $predep_objects $libobjs $deplibs $postdep_objects $compiler_flags${_S_}$rm $lib.exp'

	    # Commands to make compiler produce verbose output that lists
	    # what "hidden" libraries, object files and flags are used when
	    # linking a shared library.
	    output_verbose_link_cmd="$CC -G $CFLAGS -v conftest.$objext 2>&1 | grep \"\-L\""
	  fi

	  _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}-R $wl$libdir'
	fi
	;;
    esac
    ;;
  sysv5OpenUNIX8* | sysv5UnixWare7* | sysv5uw[[78]]* | unixware7*)
    _LT_AC_TAGVAR(archive_cmds_need_lc, $1)=no
    ;;
  tandem*)
    case $cc_basename in
      NCC)
	# NonStop-UX NCC 3.20
	# FIXME: insert proper C++ library support
	_LT_AC_TAGVAR(ld_shlibs, $1)=no
	;;
      *)
	# FIXME: insert proper C++ library support
	_LT_AC_TAGVAR(ld_shlibs, $1)=no
	;;
    esac
    ;;
  vxworks*)
    # FIXME: insert proper C++ library support
    _LT_AC_TAGVAR(ld_shlibs, $1)=no
    ;;
  *)
    # FIXME: insert proper C++ library support
    _LT_AC_TAGVAR(ld_shlibs, $1)=no
    ;;
esac
AC_MSG_RESULT([$_LT_AC_TAGVAR(ld_shlibs, $1)])
test "$_LT_AC_TAGVAR(ld_shlibs, $1)" = no && can_build_shared=no

_LT_AC_TAGVAR(GCC, $1)="$GXX"
_LT_AC_TAGVAR(LD, $1)="$LD"

AC_LIBTOOL_POSTDEP_PREDEP($1)
AC_LIBTOOL_PROG_COMPILER_PIC($1)
AC_LIBTOOL_PROG_CC_C_O($1)
AC_LIBTOOL_SYS_HARD_LINK_LOCKS($1)
AC_LIBTOOL_PROG_LD_SHLIBS($1)
AC_LIBTOOL_PROG_LD_HARDCODE_LIBPATH($1)
AC_LIBTOOL_SYS_LIB_STRIP
AC_LIBTOOL_SYS_DYNAMIC_LINKER($1)
AC_LIBTOOL_DLOPEN_SELF($1)

AC_LIBTOOL_CONFIG($1)

AC_LANG_POP
CC=$lt_save_CC
LDCXX=$LD
LD=$lt_save_LD
GCC=$lt_save_GCC
with_gnu_ldcxx=$with_gnu_ld
with_gnu_ld=$lt_save_with_gnu_ld
lt_cv_path_LDCXX=$lt_cv_path_LD
lt_cv_path_LD=$lt_save_path_LD
lt_cv_prog_gnu_ldcxx=$lt_cv_prog_gnu_ld
lt_cv_prog_gnu_ld=$lt_save_with_gnu_ld
])# AC_LIBTOOL_LANG_CXX_CONFIG

# AC_LIBTOOL_POSTDEP_PREDEP([TAGNAME])
# ------------------------
# Figure out "hidden" library dependencies from verbose
# compiler output when linking a shared library.
# Parse the compiler output and extract the necessary
# objects, libraries and library flags.
AC_DEFUN([AC_LIBTOOL_POSTDEP_PREDEP],[
dnl we can't use the lt_simple_compile_test_code here,
dnl because it contains code intended for an executable,
dnl not a library.  It's possible we should let each
dnl tag define a new lt_????_link_test_code variable,
dnl but it's only used here...
ifelse([$1],[],[cat > conftest.$ac_ext <<EOF
int a;
void foo (void) { a = 0; }
EOF
],[$1],[CXX],[cat > conftest.$ac_ext <<EOF
class Foo
{
public:
  Foo (void) { a = 0; }
private:
  int a;
};
EOF
],[$1],[F77],[cat > conftest.$ac_ext <<EOF
      subroutine foo
      implicit none
      integer*4 a
      a=0
      return
      end
EOF
],[$1],[GCJ],[cat > conftest.$ac_ext <<EOF
public class foo {
  private int a;
  public void bar (void) {
    a = 0;
  }
};
EOF
])
dnl Parse the compiler output and extract the necessary
dnl objects, libraries and library flags.
if AC_TRY_EVAL(ac_compile); then
  # Parse the compiler output and extract the necessary
  # objects, libraries and library flags.

  # Sentinel used to keep track of whether or not we are before
  # the conftest object file.
  pre_test_object_deps_done=no

  # The `*' in the case matches for architectures that use `case' in
  # $output_verbose_cmd can trigger glob expansion during the loop
  # eval without this substitution.
  output_verbose_link_cmd="`$echo \"X$output_verbose_link_cmd\" | $Xsed -e \"$no_glob_subst\"`"

  for p in `eval $output_verbose_link_cmd`; do
    case $p in

    -L* | -R* | -l*)
       # Some compilers place space between "-{L,R}" and the path.
       # Remove the space.
       if test $p = "-L" \
	  || test $p = "-R"; then
	 prev=$p
	 continue
       else
	 prev=
       fi

       if test "$pre_test_object_deps_done" = no; then
	 case $p in
	 -L* | -R*)
	   # Internal compiler library paths should come after those
	   # provided the user.  The postdeps already come after the
	   # user supplied libs so there is no need to process them.
	   if test -z "$_LT_AC_TAGVAR(compiler_lib_search_path, $1)"; then
	     _LT_AC_TAGVAR(compiler_lib_search_path, $1)="${prev}${p}"
	   else
	     _LT_AC_TAGVAR(compiler_lib_search_path, $1)="${_LT_AC_TAGVAR(compiler_lib_search_path, $1)} ${prev}${p}"
	   fi
	   ;;
	 # The "-l" case would never come before the object being
	 # linked, so don't bother handling this case.
	 esac
       else
	 if test -z "$_LT_AC_TAGVAR(postdeps, $1)"; then
	   _LT_AC_TAGVAR(postdeps, $1)="${prev}${p}"
	 else
	   _LT_AC_TAGVAR(postdeps, $1)="${_LT_AC_TAGVAR(postdeps, $1)} ${prev}${p}"
	 fi
       fi
       ;;

    *.$objext)
       # This assumes that the test object file only shows up
       # once in the compiler output.
       if test "$p" = "conftest.$objext"; then
	 pre_test_object_deps_done=yes
	 continue
       fi

       if test "$pre_test_object_deps_done" = no; then
	 if test -z "$_LT_AC_TAGVAR(predep_objects, $1)"; then
	   _LT_AC_TAGVAR(predep_objects, $1)="$p"
	 else
	   _LT_AC_TAGVAR(predep_objects, $1)="$_LT_AC_TAGVAR(predep_objects, $1) $p"
	 fi
       else
	 if test -z "$_LT_AC_TAGVAR(postdep_objects, $1)"; then
	   _LT_AC_TAGVAR(postdep_objects, $1)="$p"
	 else
	   _LT_AC_TAGVAR(postdep_objects, $1)="$_LT_AC_TAGVAR(postdep_objects, $1) $p"
	 fi
       fi
       ;;

    *) ;; # Ignore the rest.

    esac
  done

  # Clean up.
  rm -f a.out a.exe
else
  echo "libtool.m4: error: problem compiling $1 test program"
fi

$rm -f confest.$objext

case " $_LT_AC_TAGVAR(postdeps, $1) " in
*" -lc "*) _LT_AC_TAGVAR(archive_cmds_need_lc, $1)=no ;;
esac
])# AC_LIBTOOL_POSTDEP_PREDEP

# AC_LIBTOOL_LANG_F77_CONFIG
# ------------------------
# Ensure that the configuration vars for the C compiler are
# suitably defined.  Those variables are subsequently used by
# AC_LIBTOOL_CONFIG to write the compiler configuration to `libtool'.
AC_DEFUN([AC_LIBTOOL_LANG_F77_CONFIG], [_LT_AC_LANG_F77_CONFIG(F77)])
AC_DEFUN([_LT_AC_LANG_F77_CONFIG],
[AC_REQUIRE([AC_PROG_F77])
AC_LANG_PUSH(Fortran 77)

_LT_AC_TAGVAR(archive_cmds_need_lc, $1)=no
_LT_AC_TAGVAR(allow_undefined_flag, $1)=
_LT_AC_TAGVAR(always_export_symbols, $1)=no
_LT_AC_TAGVAR(archive_expsym_cmds, $1)=
_LT_AC_TAGVAR(export_dynamic_flag_spec, $1)=
_LT_AC_TAGVAR(hardcode_direct, $1)=no
_LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)=
_LT_AC_TAGVAR(hardcode_libdir_flag_spec_ld, $1)=
_LT_AC_TAGVAR(hardcode_libdir_separator, $1)=
_LT_AC_TAGVAR(hardcode_minus_L, $1)=no
_LT_AC_TAGVAR(hardcode_automatic, $1)=no
_LT_AC_TAGVAR(module_cmds, $1)=
_LT_AC_TAGVAR(module_expsym_cmds, $1)=
_LT_AC_TAGVAR(link_all_deplibs, $1)=unknown
_LT_AC_TAGVAR(old_archive_cmds, $1)=$old_archive_cmds
_LT_AC_TAGVAR(no_undefined_flag, $1)=
_LT_AC_TAGVAR(whole_archive_flag_spec, $1)=
_LT_AC_TAGVAR(enable_shared_with_static_runtimes, $1)=no

# Source file extension for f77 test sources.
ac_ext=f

# Object file extension for compiled f77 test sources.
objext=o
_LT_AC_TAGVAR(objext, $1)=$objext

# Code to be used in simple compile tests
lt_simple_compile_test_code="      subroutine t\n      return\n      end\n"

# Code to be used in simple link tests
lt_simple_link_test_code="      program t\n      end\n"

# ltmain only uses $CC for tagged configurations so make sure $CC is set.
_LT_AC_SYS_COMPILER

# Allow CC to be a program name with arguments.
lt_save_CC="$CC"
CC=${F77-"f77"}
compiler=$CC
_LT_AC_TAGVAR(compiler, $1)=$CC
cc_basename=`$echo X"$compiler" | $Xsed -e 's%^.*/%%'`

AC_MSG_CHECKING([if libtool supports shared libraries])
AC_MSG_RESULT([$can_build_shared])

AC_MSG_CHECKING([whether to build shared libraries])
test "$can_build_shared" = "no" && enable_shared=no

# On AIX, shared libraries and static libraries use the same namespace, and
# are all built from PIC.
case "$host_os" in
aix3*)
  test "$enable_shared" = yes && enable_static=no
  if test -n "$RANLIB"; then
    archive_cmds="$archive_cmds\${_S_}\$RANLIB \$lib"
    postinstall_cmds='$RANLIB $lib'
  fi
  ;;
aix4*)
  test "$enable_shared" = yes && enable_static=no
  ;;
esac
AC_MSG_RESULT([$enable_shared])

AC_MSG_CHECKING([whether to build static libraries])
# Make sure either enable_shared or enable_static is yes.
test "$enable_shared" = yes || enable_static=yes
AC_MSG_RESULT([$enable_static])

test "$_LT_AC_TAGVAR(ld_shlibs, $1)" = no && can_build_shared=no

_LT_AC_TAGVAR(GCC, $1)="$G77"
_LT_AC_TAGVAR(LD, $1)="$LD"

AC_LIBTOOL_PROG_COMPILER_PIC($1)
AC_LIBTOOL_PROG_CC_C_O($1)
AC_LIBTOOL_SYS_HARD_LINK_LOCKS($1)
AC_LIBTOOL_PROG_LD_SHLIBS($1)
AC_LIBTOOL_PROG_LD_HARDCODE_LIBPATH($1)
AC_LIBTOOL_SYS_LIB_STRIP
AC_LIBTOOL_SYS_DYNAMIC_LINKER($1)

AC_LIBTOOL_CONFIG($1)

AC_LANG_POP
CC="$lt_save_CC"
])# AC_LIBTOOL_LANG_F77_CONFIG


# AC_LIBTOOL_LANG_GCJ_CONFIG
# --------------------------
# Ensure that the configuration vars for the C compiler are
# suitably defined.  Those variables are subsequently used by
# AC_LIBTOOL_CONFIG to write the compiler configuration to `libtool'.
AC_DEFUN([AC_LIBTOOL_LANG_GCJ_CONFIG], [_LT_AC_LANG_GCJ_CONFIG(GCJ)])
AC_DEFUN([_LT_AC_LANG_GCJ_CONFIG],
[AC_LANG_SAVE

# Source file extension for Java test sources.
ac_ext=java

# Object file extension for compiled Java test sources.
objext=o
_LT_AC_TAGVAR(objext, $1)=$objext

# Code to be used in simple compile tests
lt_simple_compile_test_code="class foo {}\n"

# Code to be used in simple link tests
lt_simple_link_test_code='public class conftest { public static void main(String[] argv) {}; }\n'

# ltmain only uses $CC for tagged configurations so make sure $CC is set.
_LT_AC_SYS_COMPILER

# Allow CC to be a program name with arguments.
lt_save_CC="$CC"
CC=${GCJ-"gcj"}
compiler=$CC
_LT_AC_TAGVAR(compiler, $1)=$CC

# GCJ did not exist at the time GCC didn't implicitly link libc in.
_LT_AC_TAGVAR(archive_cmds_need_lc, $1)=no

AC_LIBTOOL_PROG_COMPILER_NO_RTTI($1)
AC_LIBTOOL_PROG_COMPILER_PIC($1)
AC_LIBTOOL_PROG_CC_C_O($1)
AC_LIBTOOL_SYS_HARD_LINK_LOCKS($1)
AC_LIBTOOL_PROG_LD_SHLIBS($1)
AC_LIBTOOL_PROG_LD_HARDCODE_LIBPATH($1)
AC_LIBTOOL_SYS_LIB_STRIP
AC_LIBTOOL_SYS_DYNAMIC_LINKER($1)
AC_LIBTOOL_DLOPEN_SELF($1)

AC_LIBTOOL_CONFIG($1)

AC_LANG_RESTORE
CC="$lt_save_CC"
])# AC_LIBTOOL_LANG_GCJ_CONFIG


# AC_LIBTOOL_LANG_RC_CONFIG
# --------------------------
# Ensure that the configuration vars for the Windows resource compiler are
# suitably defined.  Those variables are subsequently used by
# AC_LIBTOOL_CONFIG to write the compiler configuration to `libtool'.
AC_DEFUN([AC_LIBTOOL_LANG_RC_CONFIG], [_LT_AC_LANG_RC_CONFIG(RC)])
AC_DEFUN([_LT_AC_LANG_RC_CONFIG],
[AC_LANG_SAVE

# Source file extension for RC test sources.
ac_ext=rc

# Object file extension for compiled RC test sources.
objext=o
_LT_AC_TAGVAR(objext, $1)=$objext

# Code to be used in simple compile tests
lt_simple_compile_test_code='sample MENU { MENUITEM "&Soup", 100, CHECKED }\n'

# Code to be used in simple link tests
lt_simple_link_test_code="$lt_simple_compile_test_code"

# ltmain only uses $CC for tagged configurations so make sure $CC is set.
_LT_AC_SYS_COMPILER

# Allow CC to be a program name with arguments.
lt_save_CC="$CC"
CC=${RC-"windres"}
compiler=$CC
_LT_AC_TAGVAR(compiler, $1)=$CC
_LT_AC_TAGVAR(lt_cv_prog_compiler_c_o, $1)=yes

AC_LIBTOOL_CONFIG($1)

AC_LANG_RESTORE
CC="$lt_save_CC"
])# AC_LIBTOOL_LANG_RC_CONFIG


# AC_LIBTOOL_CONFIG([TAGNAME])
# ----------------------------
# If TAGNAME is not passed, then create an initial libtool script
# with a default configuration from the untagged config vars.  Otherwise
# add code to config.status for appending the configuration named by
# TAGNAME from the matching tagged config vars.
AC_DEFUN([AC_LIBTOOL_CONFIG],
[# The else clause should only fire when bootstrapping the
# libtool distribution, otherwise you forgot to ship ltmain.sh
# with your package, and you will get complaints that there are
# no rules to generate ltmain.sh.
if test -f "$ltmain"; then
  # See if we are running on zsh, and set the options which allow our commands through
  # without removal of \ escapes.
  if test -n "${ZSH_VERSION+set}" ; then
    setopt NO_GLOB_SUBST
  fi  
  # Now quote all the things that may contain metacharacters while being
  # careful not to overquote the AC_SUBSTed values.  We take copies of the
  # variables and quote the copies for generation of the libtool script.
  for var in echo old_CC old_CFLAGS AR AR_FLAGS EGREP RANLIB LN_S LTCC NM SED SHELL \
    libname_spec library_names_spec soname_spec extract_expsyms_cmds \
    old_striplib striplib file_magic_cmd finish_cmds finish_eval \
    deplibs_check_method reload_flag reload_cmds need_locks \
    lt_cv_sys_global_symbol_pipe lt_cv_sys_global_symbol_to_cdecl \
    lt_cv_sys_global_symbol_to_c_name_address \
    sys_lib_search_path_spec sys_lib_dlsearch_path_spec \
    old_postinstall_cmds old_postuninstall_cmds \
    _LT_AC_TAGVAR(compiler, $1) \
    _LT_AC_TAGVAR(CC, $1) \
    _LT_AC_TAGVAR(LD, $1) \
    _LT_AC_TAGVAR(lt_prog_compiler_wl, $1) \
    _LT_AC_TAGVAR(lt_prog_compiler_pic, $1) \
    _LT_AC_TAGVAR(lt_prog_compiler_static, $1) \
    _LT_AC_TAGVAR(lt_prog_compiler_no_builtin_flag, $1) \
    _LT_AC_TAGVAR(export_dynamic_flag_spec, $1) \
    _LT_AC_TAGVAR(thread_safe_flag_spec, $1) \
    _LT_AC_TAGVAR(whole_archive_flag_spec, $1) \
    _LT_AC_TAGVAR(enable_shared_with_static_runtimes, $1) \
    _LT_AC_TAGVAR(old_archive_cmds, $1) \
    _LT_AC_TAGVAR(old_archive_from_new_cmds, $1) \
    _LT_AC_TAGVAR(predep_objects, $1) \
    _LT_AC_TAGVAR(postdep_objects, $1) \
    _LT_AC_TAGVAR(predeps, $1) \
    _LT_AC_TAGVAR(postdeps, $1) \
    _LT_AC_TAGVAR(compiler_lib_search_path, $1) \
    _LT_AC_TAGVAR(archive_cmds, $1) \
    _LT_AC_TAGVAR(archive_expsym_cmds, $1) \
    _LT_AC_TAGVAR(postinstall_cmds, $1) \
    _LT_AC_TAGVAR(postuninstall_cmds, $1) \
    _LT_AC_TAGVAR(old_archive_from_expsyms_cmds, $1) \
    _LT_AC_TAGVAR(allow_undefined_flag, $1) \
    _LT_AC_TAGVAR(no_undefined_flag, $1) \
    _LT_AC_TAGVAR(export_symbols_cmds, $1) \
    _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1) \
    _LT_AC_TAGVAR(hardcode_libdir_flag_spec_ld, $1) \
    _LT_AC_TAGVAR(hardcode_libdir_separator, $1) \
    _LT_AC_TAGVAR(hardcode_automatic, $1) \
    _LT_AC_TAGVAR(module_cmds, $1) \
    _LT_AC_TAGVAR(module_expsym_cmds, $1) \
    _LT_AC_TAGVAR(lt_cv_prog_compiler_c_o, $1) \
    _LT_AC_TAGVAR(exclude_expsyms, $1) \
    _LT_AC_TAGVAR(include_expsyms, $1); do

    case $var in
    _LT_AC_TAGVAR(old_archive_cmds, $1) | \
    _LT_AC_TAGVAR(old_archive_from_new_cmds, $1) | \
    _LT_AC_TAGVAR(archive_cmds, $1) | \
    _LT_AC_TAGVAR(archive_expsym_cmds, $1) | \
    _LT_AC_TAGVAR(module_cmds, $1) | \
    _LT_AC_TAGVAR(module_expsym_cmds, $1) | \   
    _LT_AC_TAGVAR(old_archive_from_expsyms_cmds, $1) | \
    _LT_AC_TAGVAR(export_symbols_cmds, $1) | \
    extract_expsyms_cmds | reload_cmds | finish_cmds | \
    postinstall_cmds | postuninstall_cmds | \
    old_postinstall_cmds | old_postuninstall_cmds | \
    sys_lib_search_path_spec | sys_lib_dlsearch_path_spec)
      # Double-quote double-evaled strings.
      eval "lt_$var=\\\"\`\$echo \"X\$$var\" | \$Xsed -e \"\$double_quote_subst\" -e \"\$sed_quote_subst\" -e \"\$delay_variable_subst\" -e \"\$unescape_variable_subst\"\`\\\""
      ;;
    *)
      eval "lt_$var=\\\"\`\$echo \"X\$$var\" | \$Xsed -e \"\$sed_quote_subst\"\`\\\""
      ;;
    esac
  done

  case $lt_echo in
  *'\[$]0 --fallback-echo"')
    lt_echo=`$echo "X$lt_echo" | $Xsed -e 's/\\\\\\\[$]0 --fallback-echo"[$]/[$]0 --fallback-echo"/'`
    ;;
  esac

ifelse([$1], [],
  [cfgfile="${ofile}T"
  trap "$rm \"$cfgfile\"; exit 1" 1 2 15
  $rm -f "$cfgfile"
  AC_MSG_NOTICE([creating $ofile])],
  [cfgfile="$ofile"])

  cat <<__EOF__ >> "$cfgfile"
ifelse([$1], [],
[#! $SHELL

# `$echo "$cfgfile" | sed 's%^.*/%%'` - Provide generalized library-building support services.
# Generated automatically by $PROGRAM (GNU $PACKAGE $VERSION$TIMESTAMP)
# NOTE: Changes made to this file will be lost: look at ltmain.sh.
#
# Copyright (C) 1996, 1997, 1998, 1999, 2000, 2001
# Free Software Foundation, Inc.
#
# This file is part of GNU Libtool:
# Originally by Gordon Matzigkeit <gord@gnu.ai.mit.edu>, 1996
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
# As a special exception to the GNU General Public License, if you
# distribute this file as part of a program that contains a
# configuration script generated by Autoconf, you may include it under
# the same distribution terms that you use for the rest of that program.

# A sed program that does not truncate output.
SED=$lt_SED

# Sed that helps us avoid accidentally triggering echo(1) options like -n.
Xsed="$SED -e s/^X//"

# The HP-UX ksh and POSIX shell print the target directory to stdout
# if CDPATH is set.
if test "X\${CDPATH+set}" = Xset; then CDPATH=:; export CDPATH; fi

# The names of the tagged configurations supported by this script.
available_tags=

# ### BEGIN LIBTOOL CONFIG],
[# ### BEGIN LIBTOOL TAG CONFIG: $tagname])

# Libtool was configured on host `(hostname || uname -n) 2>/dev/null | sed 1q`:

# Set the command separator (default: ~)
_S_=\${LIBTOOL_CMD_SEP-\~}

# Shell to use when invoking shell scripts.
SHELL=$lt_SHELL

# Whether or not to build shared libraries.
build_libtool_libs=$enable_shared

# Whether or not to build static libraries.
build_old_libs=$enable_static

# Whether or not to add -lc for building shared libraries.
build_libtool_need_lc=$_LT_AC_TAGVAR(archive_cmds_need_lc, $1)

# Whether or not to disallow shared libs when runtime libs are static
allow_libtool_libs_with_static_runtimes=$_LT_AC_TAGVAR(enable_shared_with_static_runtimes, $1)

# Whether or not to optimize for fast installation.
fast_install=$enable_fast_install

# The host system.
host_alias=$host_alias
host=$host

# An echo program that does not interpret backslashes.
echo=$lt_echo

# The archiver.
AR=$lt_AR
AR_FLAGS=$lt_AR_FLAGS

# A C compiler.
LTCC=$lt_LTCC

# A language-specific compiler.
CC=$lt_[]_LT_AC_TAGVAR(compiler, $1)

# Is the compiler the GNU C compiler?
with_gcc=$_LT_AC_TAGVAR(GCC, $1)

# An ERE matcher.
EGREP=$lt_EGREP

# The linker used to build libraries.
LD=$lt_[]_LT_AC_TAGVAR(LD, $1)

# Whether we need hard or soft links.
LN_S=$lt_LN_S

# A BSD-compatible nm program.
NM=$lt_NM

# A symbol stripping program
STRIP=$STRIP

# Used to examine libraries when file_magic_cmd begins "file"
MAGIC_CMD=$MAGIC_CMD

# Used on cygwin: DLL creation program.
DLLTOOL="$DLLTOOL"

# Used on cygwin: object dumper.
OBJDUMP="$OBJDUMP"

# Used on cygwin: assembler.
AS="$AS"

# The name of the directory that contains temporary libtool files.
objdir=$objdir

# How to create reloadable object files.
reload_flag=$lt_reload_flag
reload_cmds=$lt_reload_cmds

# How to pass a linker flag through the compiler.
wl=$lt_[]_LT_AC_TAGVAR(lt_prog_compiler_wl, $1)

# Object file suffix (normally "o").
objext="$ac_objext"

# Old archive suffix (normally "a").
libext="$libext"

# Shared library suffix (normally ".so").
shrext='$shrext'

# Executable file suffix (normally "").
exeext="$exeext"

# Additional compiler flags for building library objects.
pic_flag=$lt_[]_LT_AC_TAGVAR(lt_prog_compiler_pic, $1)
pic_mode=$pic_mode

# What is the maximum length of a command?
max_cmd_len=$lt_cv_sys_max_cmd_len

# Does compiler simultaneously support -c and -o options?
compiler_c_o=$lt_[]_LT_AC_TAGVAR(lt_cv_prog_compiler_c_o, $1)

# Must we lock files when doing compilation ?
need_locks=$lt_need_locks

# Do we need the lib prefix for modules?
need_lib_prefix=$need_lib_prefix

# Do we need a version for libraries?
need_version=$need_version

# Whether dlopen is supported.
dlopen_support=$enable_dlopen

# Whether dlopen of programs is supported.
dlopen_self=$enable_dlopen_self

# Whether dlopen of statically linked programs is supported.
dlopen_self_static=$enable_dlopen_self_static

# Compiler flag to prevent dynamic linking.
link_static_flag=$lt_[]_LT_AC_TAGVAR(lt_prog_compiler_static, $1)

# Compiler flag to turn off builtin functions.
no_builtin_flag=$lt_[]_LT_AC_TAGVAR(lt_prog_compiler_no_builtin_flag, $1)

# Compiler flag to allow reflexive dlopens.
export_dynamic_flag_spec=$lt_[]_LT_AC_TAGVAR(export_dynamic_flag_spec, $1)

# Compiler flag to generate shared objects directly from archives.
whole_archive_flag_spec=$lt_[]_LT_AC_TAGVAR(whole_archive_flag_spec, $1)

# Compiler flag to generate thread-safe objects.
thread_safe_flag_spec=$lt_[]_LT_AC_TAGVAR(thread_safe_flag_spec, $1)

# Library versioning type.
version_type=$version_type

# Format of library name prefix.
libname_spec=$lt_libname_spec

# List of archive names.  First name is the real one, the rest are links.
# The last name is the one that the linker finds with -lNAME.
library_names_spec=$lt_library_names_spec

# The coded name of the library, if different from the real name.
soname_spec=$lt_soname_spec

# Commands used to build and install an old-style archive.
RANLIB=$lt_RANLIB
old_archive_cmds=$lt_[]_LT_AC_TAGVAR(old_archive_cmds, $1)
old_postinstall_cmds=$lt_old_postinstall_cmds
old_postuninstall_cmds=$lt_old_postuninstall_cmds

# Create an old-style archive from a shared archive.
old_archive_from_new_cmds=$lt_[]_LT_AC_TAGVAR(old_archive_from_new_cmds, $1)

# Create a temporary old-style archive to link instead of a shared archive.
old_archive_from_expsyms_cmds=$lt_[]_LT_AC_TAGVAR(old_archive_from_expsyms_cmds, $1)

# Commands used to build and install a shared archive.
archive_cmds=$lt_[]_LT_AC_TAGVAR(archive_cmds, $1)
archive_expsym_cmds=$lt_[]_LT_AC_TAGVAR(archive_expsym_cmds, $1)
postinstall_cmds=$lt_postinstall_cmds
postuninstall_cmds=$lt_postuninstall_cmds

# Commands used to build a loadable module (assumed same as above if empty)
module_cmds=$lt_[]_LT_AC_TAGVAR(module_cmds, $1)
module_expsym_cmds=$lt_[]_LT_AC_TAGVAR(module_expsym_cmds, $1)

# Commands to strip libraries.
old_striplib=$lt_old_striplib
striplib=$lt_striplib

# Dependencies to place before the objects being linked to create a
# shared library.
predep_objects=$lt_[]_LT_AC_TAGVAR(predep_objects, $1)

# Dependencies to place after the objects being linked to create a
# shared library.
postdep_objects=$lt_[]_LT_AC_TAGVAR(postdep_objects, $1)

# Dependencies to place before the objects being linked to create a
# shared library.
predeps=$lt_[]_LT_AC_TAGVAR(predeps, $1)

# Dependencies to place after the objects being linked to create a
# shared library.
postdeps=$lt_[]_LT_AC_TAGVAR(postdeps, $1)

# The library search path used internally by the compiler when linking
# a shared library.
compiler_lib_search_path=$lt_[]_LT_AC_TAGVAR(compiler_lib_search_path, $1)

# Method to check whether dependent libraries are shared objects.
deplibs_check_method=$lt_deplibs_check_method

# Command to use when deplibs_check_method == file_magic.
file_magic_cmd=$lt_file_magic_cmd

# Flag that allows shared libraries with undefined symbols to be built.
allow_undefined_flag=$lt_[]_LT_AC_TAGVAR(allow_undefined_flag, $1)

# Flag that forces no undefined symbols.
no_undefined_flag=$lt_[]_LT_AC_TAGVAR(no_undefined_flag, $1)

# Commands used to finish a libtool library installation in a directory.
finish_cmds=$lt_finish_cmds

# Same as above, but a single script fragment to be evaled but not shown.
finish_eval=$lt_finish_eval

# Take the output of nm and produce a listing of raw symbols and C names.
global_symbol_pipe=$lt_lt_cv_sys_global_symbol_pipe

# Transform the output of nm in a proper C declaration
global_symbol_to_cdecl=$lt_lt_cv_sys_global_symbol_to_cdecl

# Transform the output of nm in a C name address pair
global_symbol_to_c_name_address=$lt_lt_cv_sys_global_symbol_to_c_name_address

# This is the shared library runtime path variable.
runpath_var=$runpath_var

# This is the shared library path variable.
shlibpath_var=$shlibpath_var

# Is shlibpath searched before the hard-coded library search path?
shlibpath_overrides_runpath=$shlibpath_overrides_runpath

# How to hardcode a shared library path into an executable.
hardcode_action=$_LT_AC_TAGVAR(hardcode_action, $1)

# Whether we should hardcode library paths into libraries.
hardcode_into_libs=$hardcode_into_libs

# Flag to hardcode \$libdir into a binary during linking.
# This must work even if \$libdir does not exist.
hardcode_libdir_flag_spec=$lt_[]_LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)

# If ld is used when linking, flag to hardcode \$libdir into
# a binary during linking. This must work even if \$libdir does
# not exist.
hardcode_libdir_flag_spec_ld=$lt_[]_LT_AC_TAGVAR(hardcode_libdir_flag_spec_ld, $1)

# Whether we need a single -rpath flag with a separated argument.
hardcode_libdir_separator=$lt_[]_LT_AC_TAGVAR(hardcode_libdir_separator, $1)

# Set to yes if using DIR/libNAME${shared_ext} during linking hardcodes DIR into the
# resulting binary.
hardcode_direct=$_LT_AC_TAGVAR(hardcode_direct, $1)

# Set to yes if using the -LDIR flag during linking hardcodes DIR into the
# resulting binary.
hardcode_minus_L=$_LT_AC_TAGVAR(hardcode_minus_L, $1)

# Set to yes if using SHLIBPATH_VAR=DIR during linking hardcodes DIR into
# the resulting binary.
hardcode_shlibpath_var=$_LT_AC_TAGVAR(hardcode_shlibpath_var, $1)

# Set to yes if building a shared library automatically hardcodes DIR into the library
# and all subsequent libraries and executables linked against it.
hardcode_automatic=$_LT_AC_TAGVAR(hardcode_automatic, $1)

# Variables whose values should be saved in libtool wrapper scripts and
# restored at relink time.
variables_saved_for_relink="$variables_saved_for_relink"

# Whether libtool must link a program against all its dependency libraries.
link_all_deplibs=$_LT_AC_TAGVAR(link_all_deplibs, $1)

# Compile-time system search path for libraries
sys_lib_search_path_spec=$lt_sys_lib_search_path_spec

# Run-time system search path for libraries
sys_lib_dlsearch_path_spec=$lt_sys_lib_dlsearch_path_spec

# Fix the shell variable \$srcfile for the compiler.
fix_srcfile_path="$_LT_AC_TAGVAR(fix_srcfile_path, $1)"

# Set to yes if exported symbols are required.
always_export_symbols=$_LT_AC_TAGVAR(always_export_symbols, $1)

# The commands to list exported symbols.
export_symbols_cmds=$lt_[]_LT_AC_TAGVAR(export_symbols_cmds, $1)

# The commands to extract the exported symbol list from a shared archive.
extract_expsyms_cmds=$lt_extract_expsyms_cmds

# Symbols that should not be listed in the preloaded symbols.
exclude_expsyms=$lt_[]_LT_AC_TAGVAR(exclude_expsyms, $1)

# Symbols that must always be exported.
include_expsyms=$lt_[]_LT_AC_TAGVAR(include_expsyms, $1)

ifelse([$1],[],
[# ### END LIBTOOL CONFIG],
[# ### END LIBTOOL TAG CONFIG: $tagname])

__EOF__

ifelse([$1],[], [
  case $host_os in
  aix3*)
    cat <<\EOF >> "$cfgfile"

# AIX sometimes has problems with the GCC collect2 program.  For some
# reason, if we set the COLLECT_NAMES environment variable, the problems
# vanish in a puff of smoke.
if test "X${COLLECT_NAMES+set}" != Xset; then
  COLLECT_NAMES=
  export COLLECT_NAMES
fi
EOF
    ;;
  esac

  # We use sed instead of cat because bash on DJGPP gets confused if
  # if finds mixed CR/LF and LF-only lines.  Since sed operates in
  # text mode, it properly converts lines to CR/LF.  This bash problem
  # is reportedly fixed, but why not run on old versions too?
  sed '$q' "$ltmain" >> "$cfgfile" || (rm -f "$cfgfile"; exit 1)

  mv -f "$cfgfile" "$ofile" || \
    (rm -f "$ofile" && cp "$cfgfile" "$ofile" && rm -f "$cfgfile")
  chmod +x "$ofile"
])
else
  # If there is no Makefile yet, we rely on a make rule to execute
  # `config.status --recheck' to rerun these tests and create the
  # libtool script then.
  test -f Makefile && make "$ltmain"
fi
])# AC_LIBTOOL_CONFIG


# AC_LIBTOOL_PROG_COMPILER_NO_RTTI([TAGNAME])
# -------------------------------------------
AC_DEFUN([AC_LIBTOOL_PROG_COMPILER_NO_RTTI],
[AC_REQUIRE([_LT_AC_SYS_COMPILER])dnl

_LT_AC_TAGVAR(lt_prog_compiler_no_builtin_flag, $1)=

if test "$GCC" = yes; then
  _LT_AC_TAGVAR(lt_prog_compiler_no_builtin_flag, $1)=' -fno-builtin'

  AC_LIBTOOL_COMPILER_OPTION([if $compiler supports -fno-rtti -fno-exceptions],
    lt_cv_prog_compiler_rtti_exceptions,
    [-fno-rtti -fno-exceptions], [],
    [_LT_AC_TAGVAR(lt_prog_compiler_no_builtin_flag, $1)="$_LT_AC_TAGVAR(lt_prog_compiler_no_builtin_flag, $1) -fno-rtti -fno-exceptions"])
fi
])# AC_LIBTOOL_PROG_COMPILER_NO_RTTI


# AC_LIBTOOL_SYS_GLOBAL_SYMBOL_PIPE
# ---------------------------------
AC_DEFUN([AC_LIBTOOL_SYS_GLOBAL_SYMBOL_PIPE],
[AC_REQUIRE([AC_CANONICAL_HOST])
AC_REQUIRE([AC_PROG_NM])
AC_REQUIRE([AC_OBJEXT])
# Check for command to grab the raw symbol name followed by C symbol from nm.
AC_MSG_CHECKING([command to parse $NM output from $compiler object])
AC_CACHE_VAL([lt_cv_sys_global_symbol_pipe],
[
# These are sane defaults that work on at least a few old systems.
# [They come from Ultrix.  What could be older than Ultrix?!! ;)]

# Character class describing NM global symbol codes.
symcode='[[BCDEGRST]]'

# Regexp to match symbols that can be accessed directly from C.
sympat='\([[_A-Za-z]][[_A-Za-z0-9]]*\)'

# Transform the above into a raw symbol and a C symbol.
symxfrm='\1 \2\3 \3'

# Transform an extracted symbol line into a proper C declaration
lt_cv_sys_global_symbol_to_cdecl="sed -n -e 's/^. .* \(.*\)$/extern int \1;/p'"

# Transform an extracted symbol line into symbol name and symbol address
lt_cv_sys_global_symbol_to_c_name_address="sed -n -e 's/^: \([[^ ]]*\) $/  {\\\"\1\\\", (lt_ptr) 0},/p' -e 's/^$symcode \([[^ ]]*\) \([[^ ]]*\)$/  {\"\2\", (lt_ptr) \&\2},/p'"

# Define system-specific variables.
case $host_os in
aix*)
  symcode='[[BCDT]]'
  ;;
cygwin* | mingw* | pw32*)
  symcode='[[ABCDGISTW]]'
  ;;
hpux*) # Its linker distinguishes data from code symbols
  if test "$host_cpu" = ia64; then
    symcode='[[ABCDEGRST]]'
  fi
  lt_cv_sys_global_symbol_to_cdecl="sed -n -e 's/^T .* \(.*\)$/extern int \1();/p' -e 's/^$symcode* .* \(.*\)$/extern char \1;/p'"
  lt_cv_sys_global_symbol_to_c_name_address="sed -n -e 's/^: \([[^ ]]*\) $/  {\\\"\1\\\", (lt_ptr) 0},/p' -e 's/^$symcode* \([[^ ]]*\) \([[^ ]]*\)$/  {\"\2\", (lt_ptr) \&\2},/p'"
  ;;
irix* | nonstopux*)
  symcode='[[BCDEGRST]]'
  ;;
osf*)
  symcode='[[BCDEGQRST]]'
  ;;
solaris* | sysv5*)
  symcode='[[BDT]]'
  ;;
sysv4)
  symcode='[[DFNSTU]]'
  ;;
esac

# Handle CRLF in mingw tool chain
opt_cr=
case $build_os in
mingw*)
  opt_cr=`echo 'x\{0,1\}' | tr x '\015'` # option cr in regexp
  ;;
esac

# If we're using GNU nm, then use its standard symbol codes.
case `$NM -V 2>&1` in
*GNU* | *'with BFD'*)
  symcode='[[ABCDGISTW]]' ;;
esac

# Try without a prefix undercore, then with it.
for ac_symprfx in "" "_"; do

  # Write the raw and C identifiers.
  lt_cv_sys_global_symbol_pipe="sed -n -e 's/^.*[[ 	]]\($symcode$symcode*\)[[ 	]][[ 	]]*\($ac_symprfx\)$sympat$opt_cr$/$symxfrm/p'"

  # Check to see that the pipe works correctly.
  pipe_works=no

  rm -f conftest*
  cat > conftest.$ac_ext <<EOF
#ifdef __cplusplus
extern "C" {
#endif
char nm_test_var;
void nm_test_func(){}
#ifdef __cplusplus
}
#endif
int main(){nm_test_var='a';nm_test_func();return(0);}
EOF

  if AC_TRY_EVAL(ac_compile); then
    # Now try to grab the symbols.
    nlist=conftest.nm
    if AC_TRY_EVAL(NM conftest.$ac_objext \| $lt_cv_sys_global_symbol_pipe \> $nlist) && test -s "$nlist"; then
      # Try sorting and uniquifying the output.
      if sort "$nlist" | uniq > "$nlist"T; then
	mv -f "$nlist"T "$nlist"
      else
	rm -f "$nlist"T
      fi

      # Make sure that we snagged all the symbols we need.
      if grep ' nm_test_var$' "$nlist" >/dev/null; then
	if grep ' nm_test_func$' "$nlist" >/dev/null; then
	  cat <<EOF > conftest.$ac_ext
#ifdef __cplusplus
extern "C" {
#endif

EOF
	  # Now generate the symbol file.
	  eval "$lt_cv_sys_global_symbol_to_cdecl"' < "$nlist" | grep -v main >> conftest.$ac_ext'

	  cat <<EOF >> conftest.$ac_ext
#if defined (__STDC__) && __STDC__
# define lt_ptr_t void *
#else
# define lt_ptr_t char *
# define const
#endif

/* The mapping between symbol names and symbols. */
const struct {
  const char *name;
  lt_ptr_t address;
}
lt_preloaded_symbols[[]] =
{
EOF
	  $SED "s/^$symcode$symcode* \(.*\) \(.*\)$/  {\"\2\", (lt_ptr_t) \&\2},/" < "$nlist" | grep -v main >> conftest.$ac_ext
	  cat <<\EOF >> conftest.$ac_ext
  {0, (lt_ptr_t) 0}
};

#ifdef __cplusplus
}
#endif
EOF
	  # Now try linking the two files.
	  mv conftest.$ac_objext conftstm.$ac_objext
	  lt_save_LIBS="$LIBS"
	  lt_save_CFLAGS="$CFLAGS"
	  LIBS="conftstm.$ac_objext"
	  CFLAGS="$CFLAGS$_LT_AC_TAGVAR(lt_prog_compiler_no_builtin_flag, $1)"
	  if AC_TRY_EVAL(ac_link) && test -s conftest${ac_exeext}; then
	    pipe_works=yes
	  fi
	  LIBS="$lt_save_LIBS"
	  CFLAGS="$lt_save_CFLAGS"
	else
	  echo "cannot find nm_test_func in $nlist" >&AS_MESSAGE_LOG_FD
	fi
      else
	echo "cannot find nm_test_var in $nlist" >&AS_MESSAGE_LOG_FD
      fi
    else
      echo "cannot run $lt_cv_sys_global_symbol_pipe" >&AS_MESSAGE_LOG_FD
    fi
  else
    echo "$progname: failed program was:" >&AS_MESSAGE_LOG_FD
    cat conftest.$ac_ext >&5
  fi
  rm -f conftest* conftst*

  # Do not use the global_symbol_pipe unless it works.
  if test "$pipe_works" = yes; then
    break
  else
    lt_cv_sys_global_symbol_pipe=
  fi
done
])
if test -z "$lt_cv_sys_global_symbol_pipe"; then
  lt_cv_sys_global_symbol_to_cdecl=
fi
if test -z "$lt_cv_sys_global_symbol_pipe$lt_cv_sys_global_symbol_to_cdecl"; then
  AC_MSG_RESULT(failed)
else
  AC_MSG_RESULT(ok)
fi
]) # AC_LIBTOOL_SYS_GLOBAL_SYMBOL_PIPE


# AC_LIBTOOL_PROG_COMPILER_PIC([TAGNAME])
# ---------------------------------------
AC_DEFUN([AC_LIBTOOL_PROG_COMPILER_PIC],
[_LT_AC_TAGVAR(lt_prog_compiler_wl, $1)=
_LT_AC_TAGVAR(lt_prog_compiler_pic, $1)=
_LT_AC_TAGVAR(lt_prog_compiler_static, $1)=

AC_MSG_CHECKING([for $compiler option to produce PIC])
 ifelse([$1],[CXX],[
  # C++ specific cases for pic, static, wl, etc.
  if test "$GXX" = yes; then
    _LT_AC_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
    _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-static'

    case $host_os in
    aix*)
      # All AIX code is PIC.
      if test "$host_cpu" = ia64; then
	# AIX 5 now supports IA64 processor
	_LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-Bstatic'
      fi
      ;;
    amigaos*)
      # FIXME: we need at least 68020 code to build shared libraries, but
      # adding the `-m68020' flag to GCC prevents building anything better,
      # like `-m68040'.
      _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-m68020 -resident32 -malways-restore-a4'
      ;;
    beos* | cygwin* | irix5* | irix6* | nonstopux* | osf3* | osf4* | osf5*)
      # PIC is the default for these OSes.
      ;;
    mingw* | os2* | pw32*)
      # This hack is so that the source file can tell whether it is being
      # built for inclusion in a dll (and should export symbols for example).
      _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-DDLL_EXPORT'
      ;;
    darwin* | rhapsody*)
      # PIC is the default on this platform
      # Common symbols not allowed in MH_DYLIB files
      _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-fno-common'
      ;;
    *djgpp*)
      # DJGPP does not support shared libraries at all
      _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)=
      ;;
    sysv4*MP*)
      if test -d /usr/nec; then
	_LT_AC_TAGVAR(lt_prog_compiler_pic, $1)=-Kconform_pic
      fi
      ;;
    hpux*)
      # PIC is the default for IA64 HP-UX and 64-bit HP-UX, but
      # not for PA HP-UX.
      case "$host_cpu" in
      hppa*64*|ia64*)
	;;
      *)
	_LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-fPIC'
	;;
      esac
      ;;
    *)
      _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-fPIC'
      ;;
    esac
  else
    case $host_os in
      aix4* | aix5*)
	# All AIX code is PIC.
	if test "$host_cpu" = ia64; then
	  # AIX 5 now supports IA64 processor
	  _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-Bstatic'
	else
	  _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-bnso -bI:/lib/syscalls.exp'
	fi
	;;
      chorus*)
	case $cc_basename in
	cxch68)
	  # Green Hills C++ Compiler
	  # _LT_AC_TAGVAR(lt_prog_compiler_static, $1)="--no_auto_instantiation -u __main -u __premain -u _abort -r $COOL_DIR/lib/libOrb.a $MVME_DIR/lib/CC/libC.a $MVME_DIR/lib/classix/libcx.s.a"
	  ;;
	esac
	;;
      dgux*)
	case $cc_basename in
	  ec++)
	    _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-KPIC'
	    ;;
	  ghcx)
	    # Green Hills C++ Compiler
	    _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-pic'
	    ;;
	  *)
	    ;;
	esac
	;;
      freebsd*)
	# FreeBSD uses GNU C++
	;;
      hpux9* | hpux10* | hpux11*)
	case $cc_basename in
	  CC)
	    _LT_AC_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
	    _LT_AC_TAGVAR(lt_prog_compiler_static, $1)="${ac_cv_prog_cc_wl}-a ${ac_cv_prog_cc_wl}archive"
	    if test "$host_cpu" != ia64; then
	      _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='+Z'
	    fi
	    ;;
	  aCC)
	    _LT_AC_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
	    _LT_AC_TAGVAR(lt_prog_compiler_static, $1)="${ac_cv_prog_cc_wl}-a ${ac_cv_prog_cc_wl}archive"
	    case "$host_cpu" in
	    hppa*64*|ia64*)
	      # +Z the default
	      ;;
	    *)
	      _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='+Z'
	      ;;
	    esac
	    ;;
	  *)
	    ;;
	esac
	;;
      irix5* | irix6* | nonstopux*)
	case $cc_basename in
	  CC)
	    _LT_AC_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
	    _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-non_shared'
	    # CC pic flag -KPIC is the default.
	    ;;
	  *)
	    ;;
	esac
	;;
      linux*)
	case $cc_basename in
	  KCC)
	    # KAI C++ Compiler
	    _LT_AC_TAGVAR(lt_prog_compiler_wl, $1)='--backend -Wl,'
	    _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-fPIC'
	    ;;
	  icpc)
	    # Intel C++
	    _LT_AC_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
	    _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-KPIC'
	    _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-static'
	    ;; 
	  cxx)
	    # Compaq C++
	    # Make sure the PIC flag is empty.  It appears that all Alpha
	    # Linux and Compaq Tru64 Unix objects are PIC.
	    _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)=
	    _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-non_shared'
	    ;;
	  *)
	    ;;
	esac
	;;
      lynxos*)
	;;
      m88k*)
	;;
      mvs*)
	case $cc_basename in
	  cxx)
	    _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-W c,exportall'
	    ;;
	  *)
	    ;;
	esac
	;;
      netbsd*)
	;;
      osf3* | osf4* | osf5*)
	case $cc_basename in
	  KCC)
	    _LT_AC_TAGVAR(lt_prog_compiler_wl, $1)='--backend -Wl,'
	    ;;
	  RCC)
	    # Rational C++ 2.4.1
	    _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-pic'
	    ;;
	  cxx)
	    # Digital/Compaq C++
	    _LT_AC_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
	    # Make sure the PIC flag is empty.  It appears that all Alpha
	    # Linux and Compaq Tru64 Unix objects are PIC.
	    _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)=
	    _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-non_shared'
	    ;;
	  *)
	    ;;
	esac
	;;
      psos*)
	;;
      sco*)
	case $cc_basename in
	  CC)
	    _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-fPIC'
	    ;;
	  *)
	    ;;
	esac
	;;
      solaris*)
	case $cc_basename in
	  CC)
	    # Sun C++ 4.2, 5.x and Centerline C++
	    _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-KPIC'
	    _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-Bstatic'
	    _LT_AC_TAGVAR(lt_prog_compiler_wl, $1)='-Qoption ld '
	    ;;
	  gcx)
	    # Green Hills C++ Compiler
	    _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-PIC'
	    ;;
	  *)
	    ;;
	esac
	;;
      sunos4*)
	case $cc_basename in
	  CC)
	    # Sun C++ 4.x
	    _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-pic'
	    _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-Bstatic'
	    ;;
	  lcc)
	    # Lucid
	    _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-pic'
	    ;;
	  *)
	    ;;
	esac
	;;
      tandem*)
	case $cc_basename in
	  NCC)
	    # NonStop-UX NCC 3.20
	    _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-KPIC'
	    ;;
	  *)
	    ;;
	esac
	;;
      unixware*)
	;;
      vxworks*)
	;;
      *)
	_LT_AC_TAGVAR(lt_prog_compiler_can_build_shared, $1)=no
	;;
    esac
  fi
],
[
  if test "$GCC" = yes; then
    _LT_AC_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
    _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-static'

    case $host_os in
      aix*)
      # All AIX code is PIC.
      if test "$host_cpu" = ia64; then
	# AIX 5 now supports IA64 processor
	_LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-Bstatic'
      fi
      ;;

    amigaos*)
      # FIXME: we need at least 68020 code to build shared libraries, but
      # adding the `-m68020' flag to GCC prevents building anything better,
      # like `-m68040'.
      _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-m68020 -resident32 -malways-restore-a4'
      ;;

    beos* | cygwin* | irix5* | irix6* | nonstopux* | osf3* | osf4* | osf5*)
      # PIC is the default for these OSes.
      ;;

    mingw* | pw32* | os2*)
      # This hack is so that the source file can tell whether it is being
      # built for inclusion in a dll (and should export symbols for example).
      _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-DDLL_EXPORT'
      ;;

    darwin* | rhapsody*)
      # PIC is the default on this platform
      # Common symbols not allowed in MH_DYLIB files
      _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-fno-common'
      ;;

    msdosdjgpp*)
      # Just because we use GCC doesn't mean we suddenly get shared libraries
      # on systems that don't support them.
      _LT_AC_TAGVAR(lt_prog_compiler_can_build_shared, $1)=no
      enable_shared=no
      ;;

    sysv4*MP*)
      if test -d /usr/nec; then
	_LT_AC_TAGVAR(lt_prog_compiler_pic, $1)=-Kconform_pic
      fi
      ;;

    hpux*)
      # PIC is the default for IA64 HP-UX and 64-bit HP-UX, but
      # not for PA HP-UX.
      case "$host_cpu" in
      hppa*64*|ia64*)
	# +Z the default
	;;
      *)
	_LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-fPIC'
	;;
      esac
      ;;

    *)
      _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-fPIC'
      ;;
    esac
  else
    # PORTME Check for flag to pass linker flags through the system compiler.
    case $host_os in
    aix*)
      _LT_AC_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
      if test "$host_cpu" = ia64; then
	# AIX 5 now supports IA64 processor
	_LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-Bstatic'
      else
	_LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-bnso -bI:/lib/syscalls.exp'
      fi
      ;;

    mingw* | pw32* | os2*)
      # This hack is so that the source file can tell whether it is being
      # built for inclusion in a dll (and should export symbols for example).
      _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-DDLL_EXPORT'
      ;;

    hpux9* | hpux10* | hpux11*)
      _LT_AC_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
      # PIC is the default for IA64 HP-UX and 64-bit HP-UX, but
      # not for PA HP-UX.
      case "$host_cpu" in
      hppa*64*|ia64*)
	# +Z the default
	;;
      *)
	_LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='+Z'
	;;
      esac
      # Is there a better lt_prog_compiler_static that works with the bundled CC?
      _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='${wl}-a ${wl}archive'
      ;;

    irix5* | irix6* | nonstopux*)
      _LT_AC_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
      # PIC (with -KPIC) is the default.
      _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-non_shared'
      ;;

    newsos6)
      _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-KPIC'
      _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-Bstatic'
      ;;

    linux*)
      case $CC in
      icc|ecc)
	_LT_AC_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
	_LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-KPIC'
	_LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-static'
        ;;
      ccc)
        _LT_AC_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
        # All Alpha code is PIC.
        _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-non_shared'
        ;;
      esac
      ;;

    osf3* | osf4* | osf5*)
      _LT_AC_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
      # All OSF/1 code is PIC.
      _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-non_shared'
      ;;

    sco3.2v5*)
      _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-Kpic'
      _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-dn'
      ;;

    solaris*)
      _LT_AC_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
      _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-KPIC'
      _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-Bstatic'
      ;;

    sunos4*)
      _LT_AC_TAGVAR(lt_prog_compiler_wl, $1)='-Qoption ld '
      _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-PIC'
      _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-Bstatic'
      ;;

    sysv4 | sysv4.2uw2* | sysv4.3* | sysv5*)
      _LT_AC_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
      _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-KPIC'
      _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-Bstatic'
      ;;

    sysv4*MP*)
      if test -d /usr/nec ;then
	_LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-Kconform_pic'
	_LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-Bstatic'
      fi
      ;;

    uts4*)
      _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-pic'
      _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-Bstatic'
      ;;

    *)
      _LT_AC_TAGVAR(lt_prog_compiler_can_build_shared, $1)=no
      ;;
    esac
  fi
])
AC_MSG_RESULT([$_LT_AC_TAGVAR(lt_prog_compiler_pic, $1)])

#
# Check to make sure the PIC flag actually works.
#
if test -n "$_LT_AC_TAGVAR(lt_prog_compiler_pic, $1)"; then
  AC_LIBTOOL_COMPILER_OPTION([if $compiler PIC flag $_LT_AC_TAGVAR(lt_prog_compiler_pic, $1) works],
    _LT_AC_TAGVAR(lt_prog_compiler_pic_works, $1),
    [$_LT_AC_TAGVAR(lt_prog_compiler_pic, $1)ifelse([$1],[],[ -DPIC],[ifelse([$1],[CXX],[ -DPIC],[])])], [],
    [case $_LT_AC_TAGVAR(lt_prog_compiler_pic, $1) in
     "" | " "*) ;;
     *) _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)=" $_LT_AC_TAGVAR(lt_prog_compiler_pic, $1)" ;;
     esac],
    [_LT_AC_TAGVAR(lt_prog_compiler_pic, $1)=
     _LT_AC_TAGVAR(lt_prog_compiler_can_build_shared, $1)=no])
fi
case "$host_os" in
  # For platforms which do not support PIC, -DPIC is meaningless:
  *djgpp*)
    _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)=
    ;;
  *)
    _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)="$_LT_AC_TAGVAR(lt_prog_compiler_pic, $1)ifelse([$1],[],[ -DPIC],[ifelse([$1],[CXX],[ -DPIC],[])])"
    ;;
esac
])


# AC_LIBTOOL_PROG_LD_SHLIBS([TAGNAME])
# ------------------------------------
# See if the linker supports building shared libraries.
AC_DEFUN([AC_LIBTOOL_PROG_LD_SHLIBS],
[AC_MSG_CHECKING([whether the $compiler linker ($LD) supports shared libraries])
ifelse([$1],[CXX],[
  _LT_AC_TAGVAR(export_symbols_cmds, $1)='$NM $libobjs $convenience | $global_symbol_pipe | $SED '\''s/.* //'\'' | sort | uniq > $export_symbols'
  case $host_os in
  aix4* | aix5*)
    # If we're using GNU nm, then we don't want the "-C" option.
    # -C means demangle to AIX nm, but means don't demangle with GNU nm
    if $NM -V 2>&1 | grep 'GNU' > /dev/null; then
      _LT_AC_TAGVAR(export_symbols_cmds, $1)='$NM -Bpg $libobjs $convenience | awk '\''{ if (((\[$]2 == "T") || (\[$]2 == "D") || (\[$]2 == "B")) && ([substr](\[$]3,1,1) != ".")) { print \[$]3 } }'\'' | sort -u > $export_symbols'
    else
      _LT_AC_TAGVAR(export_symbols_cmds, $1)='$NM -BCpg $libobjs $convenience | awk '\''{ if (((\[$]2 == "T") || (\[$]2 == "D") || (\[$]2 == "B")) && ([substr](\[$]3,1,1) != ".")) { print \[$]3 } }'\'' | sort -u > $export_symbols'
    fi
    ;;
  pw32*)
    _LT_AC_TAGVAR(export_symbols_cmds, $1)="$ltdll_cmds"
  ;;
  cygwin* | mingw*)
    _LT_AC_TAGVAR(export_symbols_cmds, $1)='$NM $libobjs $convenience | $global_symbol_pipe | $SED -e '\''/^[[BCDGS]] /s/.* \([[^ ]]*\)/\1 DATA/'\'' | $SED -e '\''/^[[AITW]] /s/.* //'\'' | sort | uniq > $export_symbols'
  ;;
  *)
    _LT_AC_TAGVAR(export_symbols_cmds, $1)='$NM $libobjs $convenience | $global_symbol_pipe | $SED '\''s/.* //'\'' | sort | uniq > $export_symbols'
  ;;
  esac
],[
  runpath_var=
  _LT_AC_TAGVAR(allow_undefined_flag, $1)=
  _LT_AC_TAGVAR(enable_shared_with_static_runtimes, $1)=no
  _LT_AC_TAGVAR(archive_cmds, $1)=
  _LT_AC_TAGVAR(archive_expsym_cmds, $1)=
  _LT_AC_TAGVAR(old_archive_From_new_cmds, $1)=
  _LT_AC_TAGVAR(old_archive_from_expsyms_cmds, $1)=
  _LT_AC_TAGVAR(export_dynamic_flag_spec, $1)=
  _LT_AC_TAGVAR(whole_archive_flag_spec, $1)=
  _LT_AC_TAGVAR(thread_safe_flag_spec, $1)=
  _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)=
  _LT_AC_TAGVAR(hardcode_libdir_flag_spec_ld, $1)=
  _LT_AC_TAGVAR(hardcode_libdir_separator, $1)=
  _LT_AC_TAGVAR(hardcode_direct, $1)=no
  _LT_AC_TAGVAR(hardcode_minus_L, $1)=no
  _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=unsupported
  _LT_AC_TAGVAR(link_all_deplibs, $1)=unknown
  _LT_AC_TAGVAR(hardcode_automatic, $1)=no
  _LT_AC_TAGVAR(module_cmds, $1)=
  _LT_AC_TAGVAR(module_expsym_cmds, $1)=  
  _LT_AC_TAGVAR(always_export_symbols, $1)=no
  _LT_AC_TAGVAR(export_symbols_cmds, $1)='$NM $libobjs $convenience | $global_symbol_pipe | $SED '\''s/.* //'\'' | sort | uniq > $export_symbols'
  # include_expsyms should be a list of space-separated symbols to be *always*
  # included in the symbol list
  _LT_AC_TAGVAR(include_expsyms, $1)=
  # exclude_expsyms can be an extended regexp of symbols to exclude
  # it will be wrapped by ` (' and `)$', so one must not match beginning or
  # end of line.  Example: `a|bc|.*d.*' will exclude the symbols `a' and `bc',
  # as well as any symbol that contains `d'.
  _LT_AC_TAGVAR(exclude_expsyms, $1)="_GLOBAL_OFFSET_TABLE_"
  # Although _GLOBAL_OFFSET_TABLE_ is a valid symbol C name, most a.out
  # platforms (ab)use it in PIC code, but their linkers get confused if
  # the symbol is explicitly referenced.  Since portable code cannot
  # rely on this symbol name, it's probably fine to never include it in
  # preloaded symbol tables.
  extract_expsyms_cmds=

  case $host_os in
  cygwin* | mingw* | pw32*)
    # FIXME: the MSVC++ port hasn't been tested in a loooong time
    # When not using gcc, we currently assume that we are using
    # Microsoft Visual C++.
    if test "$GCC" != yes; then
      with_gnu_ld=no
    fi
    ;;
  openbsd*)
    with_gnu_ld=no
    ;;
  esac

  _LT_AC_TAGVAR(ld_shlibs, $1)=yes
  if test "$with_gnu_ld" = yes; then
    # If archive_cmds runs LD, not CC, wlarc should be empty
    wlarc='${wl}'

    # See if GNU ld supports shared libraries.
    case $host_os in
    aix3* | aix4* | aix5*)
      # On AIX/PPC, the GNU linker is very broken
      if test "$host_cpu" != ia64; then
	_LT_AC_TAGVAR(ld_shlibs, $1)=no
	cat <<EOF 1>&2

*** Warning: the GNU linker, at least up to release 2.9.1, is reported
*** to be unable to reliably create shared libraries on AIX.
*** Therefore, libtool is disabling shared libraries support.  If you
*** really care for shared libraries, you may want to modify your PATH
*** so that a non-GNU linker is found, and then restart.

EOF
      fi
      ;;

    amigaos*)
      _LT_AC_TAGVAR(archive_cmds, $1)='$rm $output_objdir/a2ixlibrary.data${_S_}$echo "#define NAME $libname" > $output_objdir/a2ixlibrary.data${_S_}$echo "#define LIBRARY_ID 1" >> $output_objdir/a2ixlibrary.data${_S_}$echo "#define VERSION $major" >> $output_objdir/a2ixlibrary.data${_S_}$echo "#define REVISION $revision" >> $output_objdir/a2ixlibrary.data${_S_}$AR $AR_FLAGS $lib $libobjs${_S_}$RANLIB $lib${_S_}(cd $output_objdir && a2ixlibrary -32)'
      _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='-L$libdir'
      _LT_AC_TAGVAR(hardcode_minus_L, $1)=yes

      # Samuel A. Falvo II <kc5tja@dolphin.openprojects.net> reports
      # that the semantics of dynamic libraries on AmigaOS, at least up
      # to version 4, is to share data among multiple programs linked
      # with the same dynamic library.  Since this doesn't match the
      # behavior of shared libraries on other platforms, we can't use
      # them.
      _LT_AC_TAGVAR(ld_shlibs, $1)=no
      ;;

    beos*)
      if $LD --help 2>&1 | grep ': supported targets:.* elf' > /dev/null; then
	_LT_AC_TAGVAR(allow_undefined_flag, $1)=unsupported
	# Joseph Beckenbach <jrb3@best.com> says some releases of gcc
	# support --undefined.  This deserves some investigation.  FIXME
	_LT_AC_TAGVAR(archive_cmds, $1)='$CC -nostart $libobjs $deplibs $compiler_flags ${wl}-soname $wl$soname -o $lib'
      else
	_LT_AC_TAGVAR(ld_shlibs, $1)=no
      fi
      ;;

    cygwin* | mingw* | pw32*)
      # _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1) is actually meaningless,
      # as there is no search path for DLLs.
      _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='-L$libdir'
      _LT_AC_TAGVAR(allow_undefined_flag, $1)=unsupported
      _LT_AC_TAGVAR(always_export_symbols, $1)=no
      _LT_AC_TAGVAR(enable_shared_with_static_runtimes, $1)=yes
      _LT_AC_TAGVAR(export_symbols_cmds, $1)='$NM $libobjs $convenience | $global_symbol_pipe | $SED -e '\''/^[[BCDGS]] /s/.* \([[^ ]]*\)/\1 DATA/'\'' | $SED -e '\''/^[[AITW]] /s/.* //'\'' | sort | uniq > $export_symbols'

      if $LD --help 2>&1 | grep 'auto-import' > /dev/null; then
        _LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared $libobjs $deplibs $compiler_flags -o $output_objdir/$soname ${wl}--image-base=0x10000000 ${wl}--out-implib,$lib'
	# If the export-symbols file already is a .def file (1st line
	# is EXPORTS), use it as is; otherwise, prepend...
	_LT_AC_TAGVAR(archive_expsym_cmds, $1)='if test "x`$SED 1q $export_symbols`" = xEXPORTS; then
	  cp $export_symbols $output_objdir/$soname.def;
	else
	  echo EXPORTS > $output_objdir/$soname.def;
	  cat $export_symbols >> $output_objdir/$soname.def;
	fi${_S_}
	$CC -shared $output_objdir/$soname.def $libobjs $deplibs $compiler_flags -o $output_objdir/$soname ${wl}--image-base=0x10000000  ${wl}--out-implib,$lib'
      else
	ld_shlibs=no
      fi
      ;;

    netbsd*)
      if echo __ELF__ | $CC -E - | grep __ELF__ >/dev/null; then
	_LT_AC_TAGVAR(archive_cmds, $1)='$LD -Bshareable $libobjs $deplibs $linker_flags -o $lib'
	wlarc=
      else
	_LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared $libobjs $deplibs $compiler_flags ${wl}-soname $wl$soname -o $lib'
	_LT_AC_TAGVAR(archive_expsym_cmds, $1)='$CC -shared $libobjs $deplibs $compiler_flags ${wl}-soname $wl$soname ${wl}-retain-symbols-file $wl$export_symbols -o $lib'
      fi
      ;;

    solaris* | sysv5*)
      if $LD -v 2>&1 | grep 'BFD 2\.8' > /dev/null; then
	_LT_AC_TAGVAR(ld_shlibs, $1)=no
	cat <<EOF 1>&2

*** Warning: The releases 2.8.* of the GNU linker cannot reliably
*** create shared libraries on Solaris systems.  Therefore, libtool
*** is disabling shared libraries support.  We urge you to upgrade GNU
*** binutils to release 2.9.1 or newer.  Another option is to modify
*** your PATH or compiler configuration so that the native linker is
*** used, and then restart.

EOF
      elif $LD --help 2>&1 | grep ': supported targets:.* elf' > /dev/null; then
	_LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared $libobjs $deplibs $compiler_flags ${wl}-soname $wl$soname -o $lib'
	_LT_AC_TAGVAR(archive_expsym_cmds, $1)='$CC -shared $libobjs $deplibs $compiler_flags ${wl}-soname $wl$soname ${wl}-retain-symbols-file $wl$export_symbols -o $lib'
      else
	_LT_AC_TAGVAR(ld_shlibs, $1)=no
      fi
      ;;

    sunos4*)
      _LT_AC_TAGVAR(archive_cmds, $1)='$LD -assert pure-text -Bshareable -o $lib $libobjs $deplibs $linker_flags'
      wlarc=
      _LT_AC_TAGVAR(hardcode_direct, $1)=yes
      _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=no
      ;;

    *)
      if $LD --help 2>&1 | grep ': supported targets:.* elf' > /dev/null; then
	_LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared $libobjs $deplibs $compiler_flags ${wl}-soname $wl$soname -o $lib'
	_LT_AC_TAGVAR(archive_expsym_cmds, $1)='$CC -shared $libobjs $deplibs $compiler_flags ${wl}-soname $wl$soname ${wl}-retain-symbols-file $wl$export_symbols -o $lib'
      else
	_LT_AC_TAGVAR(ld_shlibs, $1)=no
      fi
      ;;
    esac

    if test "$_LT_AC_TAGVAR(ld_shlibs, $1)" = yes; then
      runpath_var=LD_RUN_PATH
      _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}--rpath ${wl}$libdir'
      _LT_AC_TAGVAR(export_dynamic_flag_spec, $1)='${wl}--export-dynamic'
      # ancient GNU ld didn't support --whole-archive et. al.
      if $LD --help 2>&1 | grep 'no-whole-archive' > /dev/null; then
 	_LT_AC_TAGVAR(whole_archive_flag_spec, $1)="$wlarc"'--whole-archive$convenience '"$wlarc"'--no-whole-archive'
      else
  	_LT_AC_TAGVAR(whole_archive_flag_spec, $1)=
      fi
    fi
  else
    # PORTME fill in a description of your system's linker (not GNU ld)
    case $host_os in
    aix3*)
      _LT_AC_TAGVAR(allow_undefined_flag, $1)=unsupported
      _LT_AC_TAGVAR(always_export_symbols, $1)=yes
      _LT_AC_TAGVAR(archive_expsym_cmds, $1)='$LD -o $output_objdir/$soname $libobjs $deplibs $linker_flags -bE:$export_symbols -T512 -H512 -bM:SRE${_S_}$AR $AR_FLAGS $lib $output_objdir/$soname'
      # Note: this linker hardcodes the directories in LIBPATH if there
      # are no directories specified by -L.
      _LT_AC_TAGVAR(hardcode_minus_L, $1)=yes
      if test "$GCC" = yes && test -z "$link_static_flag"; then
	# Neither direct hardcoding nor static linking is supported with a
	# broken collect2.
	_LT_AC_TAGVAR(hardcode_direct, $1)=unsupported
      fi
      ;;

    aix4* | aix5*)
      if test "$host_cpu" = ia64; then
	# On IA64, the linker does run time linking by default, so we don't
	# have to do anything special.
	aix_use_runtimelinking=no
	exp_sym_flag='-Bexport'
	no_entry_flag=""
      else
	# If we're using GNU nm, then we don't want the "-C" option.
	# -C means demangle to AIX nm, but means don't demangle with GNU nm
	if $NM -V 2>&1 | grep 'GNU' > /dev/null; then
	  _LT_AC_TAGVAR(export_symbols_cmds, $1)='$NM -Bpg $libobjs $convenience | awk '\''{ if (((\[$]2 == "T") || (\[$]2 == "D") || (\[$]2 == "B")) && ([substr](\[$]3,1,1) != ".")) { print \[$]3 } }'\'' | sort -u > $export_symbols'
	else
	  _LT_AC_TAGVAR(export_symbols_cmds, $1)='$NM -BCpg $libobjs $convenience | awk '\''{ if (((\[$]2 == "T") || (\[$]2 == "D") || (\[$]2 == "B")) && ([substr](\[$]3,1,1) != ".")) { print \[$]3 } }'\'' | sort -u > $export_symbols'
	fi
	aix_use_runtimelinking=no

	# Test if we are trying to use run time linking or normal
	# AIX style linking. If -brtl is somewhere in LDFLAGS, we
	# need to do runtime linking.
	case $host_os in aix4.[[23]]|aix4.[[23]].*|aix5*)
	  for ld_flag in $LDFLAGS; do
  	  if (test $ld_flag = "-brtl" || test $ld_flag = "-Wl,-brtl"); then
  	    aix_use_runtimelinking=yes
  	    break
  	  fi
	  done
	esac

	exp_sym_flag='-bexport'
	no_entry_flag='-bnoentry'
      fi

      # When large executables or shared objects are built, AIX ld can
      # have problems creating the table of contents.  If linking a library
      # or program results in "error TOC overflow" add -mminimal-toc to
      # CXXFLAGS/CFLAGS for g++/gcc.  In the cases where that is not
      # enough to fix the problem, add -Wl,-bbigtoc to LDFLAGS.

      _LT_AC_TAGVAR(archive_cmds, $1)=''
      _LT_AC_TAGVAR(hardcode_direct, $1)=yes
      _LT_AC_TAGVAR(hardcode_libdir_separator, $1)=':'
      _LT_AC_TAGVAR(link_all_deplibs, $1)=yes

      if test "$GCC" = yes; then
	case $host_os in aix4.[012]|aix4.[012].*)
	# We only want to do this on AIX 4.2 and lower, the check
	# below for broken collect2 doesn't work under 4.3+
	  collect2name=`${CC} -print-prog-name=collect2`
	  if test -f "$collect2name" && \
  	   strings "$collect2name" | grep resolve_lib_name >/dev/null
	  then
  	  # We have reworked collect2
  	  _LT_AC_TAGVAR(hardcode_direct, $1)=yes
	  else
  	  # We have old collect2
  	  _LT_AC_TAGVAR(hardcode_direct, $1)=unsupported
  	  # It fails to find uninstalled libraries when the uninstalled
  	  # path is not listed in the libpath.  Setting hardcode_minus_L
  	  # to unsupported forces relinking
  	  _LT_AC_TAGVAR(hardcode_minus_L, $1)=yes
  	  _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='-L$libdir'
  	  _LT_AC_TAGVAR(hardcode_libdir_separator, $1)=
	  fi
	esac
	shared_flag='-shared'
      else
	# not using gcc
	if test "$host_cpu" = ia64; then
  	# VisualAge C++, Version 5.5 for AIX 5L for IA-64, Beta 3 Release
  	# chokes on -Wl,-G. The following line is correct:
	  shared_flag='-G'
	else
  	if test "$aix_use_runtimelinking" = yes; then
	    shared_flag='${wl}-G'
	  else
	    shared_flag='${wl}-bM:SRE'
  	fi
	fi
      fi

      # It seems that -bexpall does not export symbols beginning with
      # underscore (_), so it is better to generate a list of symbols to export.
      _LT_AC_TAGVAR(always_export_symbols, $1)=yes
      if test "$aix_use_runtimelinking" = yes; then
	# Warning - without using the other runtime loading flags (-brtl),
	# -berok will link without error, but may produce a broken library.
	_LT_AC_TAGVAR(allow_undefined_flag, $1)='-berok'
       # Determine the default libpath from the value encoded in an empty executable.
       _LT_AC_SYS_LIBPATH_AIX
       _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}-blibpath:$libdir:'"$aix_libpath"
	_LT_AC_TAGVAR(archive_expsym_cmds, $1)="\$CC"' -o $output_objdir/$soname $libobjs $deplibs $compiler_flags `if test "x${allow_undefined_flag}" != "x"; then echo "${wl}${allow_undefined_flag}"; else :; fi` '"\${wl}$no_entry_flag \${wl}$exp_sym_flag:\$export_symbols $shared_flag"
       else
	if test "$host_cpu" = ia64; then
	  _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}-R $libdir:/usr/lib:/lib'
	  _LT_AC_TAGVAR(allow_undefined_flag, $1)="-z nodefs"
	  _LT_AC_TAGVAR(archive_expsym_cmds, $1)="\$CC $shared_flag"' -o $output_objdir/$soname $libobjs $deplibs $compiler_flags ${wl}${allow_undefined_flag} '"\${wl}$no_entry_flag \${wl}$exp_sym_flag:\$export_symbols"
	else
	 # Determine the default libpath from the value encoded in an empty executable.
	 _LT_AC_SYS_LIBPATH_AIX
	 _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}-blibpath:$libdir:'"$aix_libpath"
	  # Warning - without using the other run time loading flags,
	  # -berok will link without error, but may produce a broken library.
	  _LT_AC_TAGVAR(no_undefined_flag, $1)=' ${wl}-bernotok'
	  _LT_AC_TAGVAR(allow_undefined_flag, $1)=' ${wl}-berok'
	  # -bexpall does not export symbols beginning with underscore (_)
	  _LT_AC_TAGVAR(always_export_symbols, $1)=yes
	  # Exported symbols can be pulled into shared objects from archives
	  _LT_AC_TAGVAR(whole_archive_flag_spec, $1)=' '
	  _LT_AC_TAGVAR(archive_cmds_need_lc, $1)=yes
	  # This is similar to how AIX traditionally builds it's shared libraries.
	  _LT_AC_TAGVAR(archive_expsym_cmds, $1)="\$CC $shared_flag"' -o $output_objdir/$soname $libobjs $deplibs $compiler_flags ${wl}-bE:$export_symbols ${wl}-bnoentry${allow_undefined_flag}\${_S_}$AR $AR_FLAGS $output_objdir/$libname$release.a $output_objdir/$soname'
	fi
      fi
      ;;

    amigaos*)
      _LT_AC_TAGVAR(archive_cmds, $1)='$rm $output_objdir/a2ixlibrary.data${_S_}$echo "#define NAME $libname" > $output_objdir/a2ixlibrary.data${_S_}$echo "#define LIBRARY_ID 1" >> $output_objdir/a2ixlibrary.data${_S_}$echo "#define VERSION $major" >> $output_objdir/a2ixlibrary.data${_S_}$echo "#define REVISION $revision" >> $output_objdir/a2ixlibrary.data${_S_}$AR $AR_FLAGS $lib $libobjs${_S_}$RANLIB $lib${_S_}(cd $output_objdir && a2ixlibrary -32)'
      _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='-L$libdir'
      _LT_AC_TAGVAR(hardcode_minus_L, $1)=yes
      # see comment about different semantics on the GNU ld section
      _LT_AC_TAGVAR(ld_shlibs, $1)=no
      ;;

    bsdi4*)
      _LT_AC_TAGVAR(export_dynamic_flag_spec, $1)=-rdynamic
      ;;

    cygwin* | mingw* | pw32*)
      # When not using gcc, we currently assume that we are using
      # Microsoft Visual C++.
      # hardcode_libdir_flag_spec is actually meaningless, as there is
      # no search path for DLLs.
      _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)=' '
      _LT_AC_TAGVAR(allow_undefined_flag, $1)=unsupported
      # Tell ltmain to make .lib files, not .a files.
      libext=lib
      # Tell ltmain to make .dll files, not .so files.
      shrext=".dll"
      # FIXME: Setting linknames here is a bad hack.
      _LT_AC_TAGVAR(archive_cmds, $1)='$CC -o $lib $libobjs $compiler_flags `echo "$deplibs" | $SED -e '\''s/ -lc$//'\''` -link -dll${_S_}linknames='
      # The linker will automatically build a .lib file if we build a DLL.
      _LT_AC_TAGVAR(old_archive_From_new_cmds, $1)='true'
      # FIXME: Should let the user specify the lib program.
      _LT_AC_TAGVAR(old_archive_cmds, $1)='lib /OUT:$oldlib$oldobjs$old_deplibs'
      fix_srcfile_path='`cygpath -w "$srcfile"`'
      _LT_AC_TAGVAR(enable_shared_with_static_runtimes, $1)=yes
      ;;

    darwin* | rhapsody*)
    if $CC -v 2>&1 | grep 'Apple' >/dev/null ; then 
      _LT_AC_TAGVAR(archive_cmds_need_lc, $1)=no
      case "$host_os" in
      rhapsody* | darwin1.[[012]])
	_LT_AC_TAGVAR(allow_undefined_flag, $1)='-undefined suppress'
	;;
      darwin1.* | darwin[[2-6]].*) # Darwin 1.3 on, but less than 7.0
	test -z ${LD_TWOLEVEL_NAMESPACE} && _LT_AC_TAGVAR(allow_undefined_flag, $1)='-flat_namespace -undefined suppress'
	;;
      *) # Darwin 7.0 on
	case "${MACOSX_DEPLOYMENT_TARGET-10.1}" in
	10.[[012]])
	  test -z ${LD_TWOLEVEL_NAMESPACE} && _LT_AC_TAGVAR(allow_undefined_flag, $1)='-flat_namespace -undefined suppress'
	  ;;
	*) # 10.3 on
	  if test -z ${LD_TWOLEVEL_NAMESPACE}; then
	    _LT_AC_TAGVAR(allow_undefined_flag, $1)='-flat_namespace -undefined suppress'
	  else
	    _LT_AC_TAGVAR(allow_undefined_flag, $1)='-undefined dynamic_lookup'
	  fi
	  ;;
	esac
	;;
      esac
      # FIXME: Relying on posixy $() will cause problems for
      #        cross-compilation, but unfortunately the echo tests do not
      #        yet detect zsh echo's removal of \ escapes.  Also zsh mangles
      #	       `"' quotes if we put them in here... so don't!
    	lt_int_apple_cc_single_mod=no
    	output_verbose_link_cmd='echo'
    	if $CC -dumpspecs 2>&1 | grep 'single_module' >/dev/null ; then
    	  lt_int_apple_cc_single_mod=yes
    	fi
    	if test "X$lt_int_apple_cc_single_mod" = Xyes ; then
    	  _LT_AC_TAGVAR(archive_cmds, $1)='$CC -dynamiclib $archargs -single_module $allow_undefined_flag -o $lib $libobjs $deplibs $compiler_flags -install_name $rpath/$soname $verstring'
    	else
        _LT_AC_TAGVAR(archive_cmds, $1)='$CC -r ${wl}-bind_at_load -keep_private_externs -nostdlib -o ${lib}-master.o $libobjs${_S_}$CC -dynamiclib $archargs $allow_undefined_flag -o $lib ${lib}-master.o $deplibs $compiler_flags -install_name $rpath/$soname $verstring'
      fi
      _LT_AC_TAGVAR(module_cmds, $1)='$CC -bundle $archargs ${wl}-bind_at_load $allow_undefined_flag -o $lib $libobjs $deplibs$compiler_flags'
      # Don't fix this by using the ld -exported_symbols_list flag, it doesn't exist in older darwin ld's
        if test "X$lt_int_apple_cc_single_mod" = Xyes ; then
          _LT_AC_TAGVAR(archive_expsym_cmds, $1)='sed -e "s,#.*,," -e "s,^[    ]*,," -e "s,^\(..*\),_&," < $export_symbols > $output_objdir/${libname}-symbols.expsym${_S_}$CC -dynamiclib $archargs -single_module $allow_undefined_flag -o $lib $libobjs $deplibs $compiler_flags -install_name $rpath/$soname $verstring${_S_}nmedit -s $output_objdir/${libname}-symbols.expsym ${lib}'
        else
          _LT_AC_TAGVAR(archive_expsym_cmds, $1)='sed -e "s,#.*,," -e "s,^[    ]*,," -e "s,^\(..*\),_&," < $export_symbols > $output_objdir/${libname}-symbols.expsym${_S_}$CC -r ${wl}-bind_at_load -keep_private_externs -nostdlib -o ${lib}-master.o $libobjs${_S_}$CC -dynamiclib $archargs $allow_undefined_flag -o $lib ${lib}-master.o $deplibs $compiler_flags -install_name $rpath/$soname $verstring${_S_}nmedit -s $output_objdir/${libname}-symbols.expsym ${lib}'
        fi
          _LT_AC_TAGVAR(module_expsym_cmds, $1)='sed -e "s,#.*,," -e "s,^[    ]*,," -e "s,^\(..*\),_&," < $export_symbols > $output_objdir/${libname}-symbols.expsym${_S_}$CC -bundle $archargs $allow_undefined_flag  -o $lib $libobjs $deplibs$compiler_flags${_S_}nmedit -s $output_objdir/${libname}-symbols.expsym ${lib}'
      _LT_AC_TAGVAR(hardcode_direct, $1)=no
      _LT_AC_TAGVAR(hardcode_automatic, $1)=yes
      _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=unsupported
      _LT_AC_TAGVAR(whole_archive_flag_spec, $1)='-all_load $convenience'
      _LT_AC_TAGVAR(link_all_deplibs, $1)=yes
    fi  
      ;;

    dgux*)
      _LT_AC_TAGVAR(archive_cmds, $1)='$LD -G -h $soname -o $lib $libobjs $deplibs $linker_flags'
      _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='-L$libdir'
      _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=no
      ;;

    freebsd1*)
      _LT_AC_TAGVAR(ld_shlibs, $1)=no
      ;;

    # FreeBSD 2.2.[012] allows us to include c++rt0.o to get C++ constructor
    # support.  Future versions do this automatically, but an explicit c++rt0.o
    # does not break anything, and helps significantly (at the cost of a little
    # extra space).
    freebsd2.2*)
      _LT_AC_TAGVAR(archive_cmds, $1)='$LD -Bshareable -o $lib $libobjs $deplibs $linker_flags /usr/lib/c++rt0.o'
      _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='-R$libdir'
      _LT_AC_TAGVAR(hardcode_direct, $1)=yes
      _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=no
      ;;

    # Unfortunately, older versions of FreeBSD 2 do not have this feature.
    freebsd2*)
      _LT_AC_TAGVAR(archive_cmds, $1)='$LD -Bshareable -o $lib $libobjs $deplibs $linker_flags'
      _LT_AC_TAGVAR(hardcode_direct, $1)=yes
      _LT_AC_TAGVAR(hardcode_minus_L, $1)=yes
      _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=no
      ;;

    # FreeBSD 3 and greater uses gcc -shared to do shared libraries.
    freebsd*)
      _LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared -o $lib $libobjs $deplibs $compiler_flags'
      _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='-R$libdir'
      _LT_AC_TAGVAR(hardcode_direct, $1)=yes
      _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=no
      ;;

    hpux9*)
      if test "$GCC" = yes; then
	_LT_AC_TAGVAR(archive_cmds, $1)='$rm $output_objdir/$soname${_S_}$CC -shared -fPIC ${wl}+b ${wl}$install_libdir -o $output_objdir/$soname $libobjs $deplibs $compiler_flags${_S_}test $output_objdir/$soname = $lib || mv $output_objdir/$soname $lib'
      else
	_LT_AC_TAGVAR(archive_cmds, $1)='$rm $output_objdir/$soname${_S_}$LD -b +b $install_libdir -o $output_objdir/$soname $libobjs $deplibs $linker_flags${_S_}test $output_objdir/$soname = $lib || mv $output_objdir/$soname $lib'
      fi
      _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}+b ${wl}$libdir'
      _LT_AC_TAGVAR(hardcode_libdir_separator, $1)=:
      _LT_AC_TAGVAR(hardcode_direct, $1)=yes

      # hardcode_minus_L: Not really in the search PATH,
      # but as the default location of the library.
      _LT_AC_TAGVAR(hardcode_minus_L, $1)=yes
      _LT_AC_TAGVAR(export_dynamic_flag_spec, $1)='${wl}-E'
      ;;

    hpux10* | hpux11*)
      if test "$GCC" = yes -a "$with_gnu_ld" = no; then
	case "$host_cpu" in
	hppa*64*|ia64*)
	  _LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared ${wl}+h ${wl}$soname -o $lib $libobjs $deplibs $compiler_flags'
	  ;;
	*)
	  _LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared -fPIC ${wl}+h ${wl}$soname ${wl}+b ${wl}$install_libdir -o $lib $libobjs $deplibs $compiler_flags'
	  ;;
	esac
      else
	case "$host_cpu" in
	hppa*64*|ia64*)
	  _LT_AC_TAGVAR(archive_cmds, $1)='$LD -b +h $soname -o $lib $libobjs $deplibs $linker_flags'
	  ;;
	*)
	  _LT_AC_TAGVAR(archive_cmds, $1)='$LD -b +h $soname +b $install_libdir -o $lib $libobjs $deplibs $linker_flags'
	  ;;
	esac
      fi
      if test "$with_gnu_ld" = no; then
	case "$host_cpu" in
	hppa*64*)
	  _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}+b ${wl}$libdir'
	  _LT_AC_TAGVAR(hardcode_libdir_flag_spec_ld, $1)='+b $libdir'
	  _LT_AC_TAGVAR(hardcode_libdir_separator, $1)=:
	  _LT_AC_TAGVAR(hardcode_direct, $1)=no
	  _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=no
	  ;;
	ia64*)
	  _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='-L$libdir'
	  _LT_AC_TAGVAR(hardcode_direct, $1)=no
	  _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=no

	  # hardcode_minus_L: Not really in the search PATH,
	  # but as the default location of the library.
	  _LT_AC_TAGVAR(hardcode_minus_L, $1)=yes
	  ;;
	*)
	  _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}+b ${wl}$libdir'
	  _LT_AC_TAGVAR(hardcode_libdir_separator, $1)=:
	  _LT_AC_TAGVAR(hardcode_direct, $1)=yes
	  _LT_AC_TAGVAR(export_dynamic_flag_spec, $1)='${wl}-E'

	  # hardcode_minus_L: Not really in the search PATH,
	  # but as the default location of the library.
	  _LT_AC_TAGVAR(hardcode_minus_L, $1)=yes
	  ;;
	esac
      fi
      ;;

    irix5* | irix6* | nonstopux*)
      if test "$GCC" = yes; then
	_LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared $libobjs $deplibs $compiler_flags ${wl}-soname ${wl}$soname `test -n "$verstring" && echo ${wl}-set_version ${wl}$verstring` ${wl}-update_registry ${wl}${output_objdir}/so_locations -o $lib'
      else
	_LT_AC_TAGVAR(archive_cmds, $1)='$LD -shared $libobjs $deplibs $linker_flags -soname $soname `test -n "$verstring" && echo -set_version $verstring` -update_registry ${output_objdir}/so_locations -o $lib'
	_LT_AC_TAGVAR(hardcode_libdir_flag_spec_ld, $1)='-rpath $libdir'
      fi
      _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}-rpath ${wl}$libdir'
      _LT_AC_TAGVAR(hardcode_libdir_separator, $1)=:
      _LT_AC_TAGVAR(link_all_deplibs, $1)=yes
      ;;

    netbsd*)
      if echo __ELF__ | $CC -E - | grep __ELF__ >/dev/null; then
	_LT_AC_TAGVAR(archive_cmds, $1)='$LD -Bshareable -o $lib $libobjs $deplibs $linker_flags'  # a.out
      else
	_LT_AC_TAGVAR(archive_cmds, $1)='$LD -shared -o $lib $libobjs $deplibs $linker_flags'      # ELF
      fi
      _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='-R$libdir'
      _LT_AC_TAGVAR(hardcode_direct, $1)=yes
      _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=no
      ;;

    newsos6)
      _LT_AC_TAGVAR(archive_cmds, $1)='$LD -G -h $soname -o $lib $libobjs $deplibs $linker_flags'
      _LT_AC_TAGVAR(hardcode_direct, $1)=yes
      _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}-rpath ${wl}$libdir'
      _LT_AC_TAGVAR(hardcode_libdir_separator, $1)=:
      _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=no
      ;;

    openbsd*)
      _LT_AC_TAGVAR(hardcode_direct, $1)=yes
      _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=no
      if test -z "`echo __ELF__ | $CC -E - | grep __ELF__`" || test "$host_os-$host_cpu" = "openbsd2.8-powerpc"; then
	_LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared $pic_flag -o $lib $libobjs $deplibs $compiler_flags'
	_LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}-rpath,$libdir'
	_LT_AC_TAGVAR(export_dynamic_flag_spec, $1)='${wl}-E'
      else
       case $host_os in
	 openbsd[[01]].* | openbsd2.[[0-7]] | openbsd2.[[0-7]].*)
	   _LT_AC_TAGVAR(archive_cmds, $1)='$LD -Bshareable -o $lib $libobjs $deplibs $linker_flags'
	   _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='-R$libdir'
	   ;;
	 *)
	   _LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared $pic_flag -o $lib $libobjs $deplibs $compiler_flags'
	   _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}-rpath,$libdir'
	   ;;
       esac
      fi
      ;;

    os2*)
      _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='-L$libdir'
      _LT_AC_TAGVAR(hardcode_minus_L, $1)=yes
      _LT_AC_TAGVAR(allow_undefined_flag, $1)=unsupported
      _LT_AC_TAGVAR(archive_cmds, $1)='$echo "LIBRARY $libname INITINSTANCE" > $output_objdir/$libname.def${_S_}$echo "DESCRIPTION \"$libname\"" >> $output_objdir/$libname.def${_S_}$echo DATA >> $output_objdir/$libname.def${_S_}$echo " SINGLE NONSHARED" >> $output_objdir/$libname.def${_S_}$echo EXPORTS >> $output_objdir/$libname.def${_S_}emxexp $libobjs >> $output_objdir/$libname.def${_S_}$CC -Zdll -Zcrtdll -o $lib $libobjs $deplibs $compiler_flags $output_objdir/$libname.def'
      _LT_AC_TAGVAR(old_archive_From_new_cmds, $1)='emximp -o $output_objdir/$libname.a $output_objdir/$libname.def'
      ;;

    osf3*)
      if test "$GCC" = yes; then
	_LT_AC_TAGVAR(allow_undefined_flag, $1)=' ${wl}-expect_unresolved ${wl}\*'
	_LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared${allow_undefined_flag} $libobjs $deplibs $compiler_flags ${wl}-soname ${wl}$soname `test -n "$verstring" && echo ${wl}-set_version ${wl}$verstring` ${wl}-update_registry ${wl}${output_objdir}/so_locations -o $lib'
      else
	_LT_AC_TAGVAR(allow_undefined_flag, $1)=' -expect_unresolved \*'
	_LT_AC_TAGVAR(archive_cmds, $1)='$LD -shared${allow_undefined_flag} $libobjs $deplibs $linker_flags -soname $soname `test -n "$verstring" && echo -set_version $verstring` -update_registry ${output_objdir}/so_locations -o $lib'
      fi
      _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}-rpath ${wl}$libdir'
      _LT_AC_TAGVAR(hardcode_libdir_separator, $1)=:
      ;;

    osf4* | osf5*)	# as osf3* with the addition of -msym flag
      if test "$GCC" = yes; then
	_LT_AC_TAGVAR(allow_undefined_flag, $1)=' ${wl}-expect_unresolved ${wl}\*'
	_LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared${allow_undefined_flag} $libobjs $deplibs $compiler_flags ${wl}-msym ${wl}-soname ${wl}$soname `test -n "$verstring" && echo ${wl}-set_version ${wl}$verstring` ${wl}-update_registry ${wl}${output_objdir}/so_locations -o $lib'
	_LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='${wl}-rpath ${wl}$libdir'
      else
	_LT_AC_TAGVAR(allow_undefined_flag, $1)=' -expect_unresolved \*'
	_LT_AC_TAGVAR(archive_cmds, $1)='$LD -shared${allow_undefined_flag} $libobjs $deplibs $linker_flags -msym -soname $soname `test -n "$verstring" && echo -set_version $verstring` -update_registry ${output_objdir}/so_locations -o $lib'
	_LT_AC_TAGVAR(archive_expsym_cmds, $1)='for i in `cat $export_symbols`; do printf "%s %s\\n" -exported_symbol "\$i" >> $lib.exp; done; echo "-hidden">> $lib.exp${_S_}
	$LD -shared${allow_undefined_flag} -input $lib.exp $linker_flags $libobjs $deplibs -soname $soname `test -n "$verstring" && echo -set_version $verstring` -update_registry ${objdir}/so_locations -o $lib${_S_}$rm $lib.exp'

	# Both c and cxx compiler support -rpath directly
	_LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='-rpath $libdir'
      fi
      _LT_AC_TAGVAR(hardcode_libdir_separator, $1)=:
      ;;

    sco3.2v5*)
      _LT_AC_TAGVAR(archive_cmds, $1)='$LD -G -h $soname -o $lib $libobjs $deplibs $linker_flags'
      _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=no
      _LT_AC_TAGVAR(export_dynamic_flag_spec, $1)='${wl}-Bexport'
      runpath_var=LD_RUN_PATH
      hardcode_runpath_var=yes
      ;;

    solaris*)
      _LT_AC_TAGVAR(no_undefined_flag, $1)=' -z text'
      if test "$GCC" = yes; then
	_LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared ${wl}-h ${wl}$soname -o $lib $libobjs $deplibs $compiler_flags'
	_LT_AC_TAGVAR(archive_expsym_cmds, $1)='$echo "{ global:" > $lib.exp${_S_}cat $export_symbols | $SED -e "s/\(.*\)/\1;/" >> $lib.exp${_S_}$echo "local: *; };" >> $lib.exp${_S_}
	  $CC -shared ${wl}-M ${wl}$lib.exp ${wl}-h ${wl}$soname -o $lib $libobjs $deplibs $compiler_flags${_S_}$rm $lib.exp'
      else
	_LT_AC_TAGVAR(archive_cmds, $1)='$LD -G${allow_undefined_flag} -h $soname -o $lib $libobjs $deplibs $linker_flags'
	_LT_AC_TAGVAR(archive_expsym_cmds, $1)='$echo "{ global:" > $lib.exp${_S_}cat $export_symbols | $SED -e "s/\(.*\)/\1;/" >> $lib.exp${_S_}$echo "local: *; };" >> $lib.exp${_S_}
  	$LD -G${allow_undefined_flag} -M $lib.exp -h $soname -o $lib $libobjs $deplibs $linker_flags${_S_}$rm $lib.exp'
      fi
      _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='-R$libdir'
      _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=no
      case $host_os in
      solaris2.[[0-5]] | solaris2.[[0-5]].*) ;;
      *) # Supported since Solaris 2.6 (maybe 2.5.1?)
	_LT_AC_TAGVAR(whole_archive_flag_spec, $1)='-z allextract$convenience -z defaultextract' ;;
      esac
      _LT_AC_TAGVAR(link_all_deplibs, $1)=yes
      ;;

    sunos4*)
      if test "x$host_vendor" = xsequent; then
	# Use $CC to link under sequent, because it throws in some extra .o
	# files that make .init and .fini sections work.
	_LT_AC_TAGVAR(archive_cmds, $1)='$CC -G ${wl}-h $soname -o $lib $libobjs $deplibs $compiler_flags'
      else
	_LT_AC_TAGVAR(archive_cmds, $1)='$LD -assert pure-text -Bstatic -o $lib $libobjs $deplibs $linker_flags'
      fi
      _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='-L$libdir'
      _LT_AC_TAGVAR(hardcode_direct, $1)=yes
      _LT_AC_TAGVAR(hardcode_minus_L, $1)=yes
      _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=no
      ;;

    sysv4)
      case $host_vendor in
	sni)
	  _LT_AC_TAGVAR(archive_cmds, $1)='$LD -G -h $soname -o $lib $libobjs $deplibs $linker_flags'
	  _LT_AC_TAGVAR(hardcode_direct, $1)=yes # is this really true???
	;;
	siemens)
	  ## LD is ld it makes a PLAMLIB
	  ## CC just makes a GrossModule.
	  _LT_AC_TAGVAR(archive_cmds, $1)='$LD -G -o $lib $libobjs $deplibs $linker_flags'
	  _LT_AC_TAGVAR(reload_cmds, $1)='$CC -r -o $output$reload_objs'
	  _LT_AC_TAGVAR(hardcode_direct, $1)=no
        ;;
	motorola)
	  _LT_AC_TAGVAR(archive_cmds, $1)='$LD -G -h $soname -o $lib $libobjs $deplibs $linker_flags'
	  _LT_AC_TAGVAR(hardcode_direct, $1)=no #Motorola manual says yes, but my tests say they lie
	;;
      esac
      runpath_var='LD_RUN_PATH'
      _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=no
      ;;

    sysv4.3*)
      _LT_AC_TAGVAR(archive_cmds, $1)='$LD -G -h $soname -o $lib $libobjs $deplibs $linker_flags'
      _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=no
      _LT_AC_TAGVAR(export_dynamic_flag_spec, $1)='-Bexport'
      ;;

    sysv4*MP*)
      if test -d /usr/nec; then
	_LT_AC_TAGVAR(archive_cmds, $1)='$LD -G -h $soname -o $lib $libobjs $deplibs $linker_flags'
	_LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=no
	runpath_var=LD_RUN_PATH
	hardcode_runpath_var=yes
	_LT_AC_TAGVAR(ld_shlibs, $1)=yes
      fi
      ;;

    sysv4.2uw2*)
      _LT_AC_TAGVAR(archive_cmds, $1)='$LD -G -o $lib $libobjs $deplibs $linker_flags'
      _LT_AC_TAGVAR(hardcode_direct, $1)=yes
      _LT_AC_TAGVAR(hardcode_minus_L, $1)=no
      _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=no
      hardcode_runpath_var=yes
      runpath_var=LD_RUN_PATH
      ;;

   sysv5OpenUNIX8* | sysv5UnixWare7* |  sysv5uw[[78]]* | unixware7*)
      _LT_AC_TAGVAR(no_undefined_flag, $1)='${wl}-z ${wl}text'
      if test "$GCC" = yes; then
	_LT_AC_TAGVAR(archive_cmds, $1)='$CC -shared ${wl}-h ${wl}$soname -o $lib $libobjs $deplibs $compiler_flags'
      else
	_LT_AC_TAGVAR(archive_cmds, $1)='$CC -G ${wl}-h ${wl}$soname -o $lib $libobjs $deplibs $compiler_flags'
      fi
      runpath_var='LD_RUN_PATH'
      _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=no
      ;;

    sysv5*)
      _LT_AC_TAGVAR(no_undefined_flag, $1)=' -z text'
      # $CC -shared without GNU ld will not create a library from C++
      # object files and a static libstdc++, better avoid it by now
      _LT_AC_TAGVAR(archive_cmds, $1)='$LD -G${allow_undefined_flag} -h $soname -o $lib $libobjs $deplibs $linker_flags'
      _LT_AC_TAGVAR(archive_expsym_cmds, $1)='$echo "{ global:" > $lib.exp${_S_}cat $export_symbols | $SED -e "s/\(.*\)/\1;/" >> $lib.exp${_S_}$echo "local: *; };" >> $lib.exp${_S_}
  		$LD -G${allow_undefined_flag} -M $lib.exp -h $soname -o $lib $libobjs $deplibs $linker_flags${_S_}$rm $lib.exp'
      _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)=
      _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=no
      runpath_var='LD_RUN_PATH'
      ;;

    uts4*)
      _LT_AC_TAGVAR(archive_cmds, $1)='$LD -G -h $soname -o $lib $libobjs $deplibs $linker_flags'
      _LT_AC_TAGVAR(hardcode_libdir_flag_spec, $1)='-L$libdir'
      _LT_AC_TAGVAR(hardcode_shlibpath_var, $1)=no
      ;;

    *)
      _LT_AC_TAGVAR(ld_shlibs, $1)=no
      ;;
    esac
  fi
])
AC_MSG_RESULT([$_LT_AC_TAGVAR(ld_shlibs, $1)])
test "$_LT_AC_TAGVAR(ld_shlibs, $1)" = no && can_build_shared=no

variables_saved_for_relink="PATH $shlibpath_var $runpath_var"
if test "$GCC" = yes; then
  variables_saved_for_relink="$variables_saved_for_relink GCC_EXEC_PREFIX COMPILER_PATH LIBRARY_PATH"
fi

#
# Do we need to explicitly link libc?
#
case "x$_LT_AC_TAGVAR(archive_cmds_need_lc, $1)" in
x|xyes)
  # Assume -lc should be added
  _LT_AC_TAGVAR(archive_cmds_need_lc, $1)=yes

  if test "$enable_shared" = yes && test "$GCC" = yes; then
    case $_LT_AC_TAGVAR(archive_cmds, $1) in
    *"$_S_"*)
      # FIXME: we may have to deal with multi-command sequences.
      ;;
    '$CC '*)
      # Test whether the compiler implicitly links with -lc since on some
      # systems, -lgcc has to come before -lc. If gcc already passes -lc
      # to ld, don't add -lc before -lgcc.
      AC_MSG_CHECKING([whether -lc should be explicitly linked in])
      $rm conftest*
      printf "$lt_simple_compile_test_code" > conftest.$ac_ext
  
      if AC_TRY_EVAL(ac_compile) 2>conftest.err; then
        soname=conftest
        lib=conftest
        libobjs=conftest.$ac_objext
        deplibs=
        wl=$_LT_AC_TAGVAR(lt_prog_compiler_wl, $1)
        compiler_flags=-v
        linker_flags=-v
        verstring=
        output_objdir=.
        libname=conftest
        lt_save_allow_undefined_flag=$_LT_AC_TAGVAR(allow_undefined_flag, $1)
        _LT_AC_TAGVAR(allow_undefined_flag, $1)=
        if AC_TRY_EVAL(_LT_AC_TAGVAR(archive_cmds, $1) 2\>\&1 \| grep \" -lc \" \>/dev/null 2\>\&1)
        then
	  _LT_AC_TAGVAR(archive_cmds_need_lc, $1)=no
        else
	  _LT_AC_TAGVAR(archive_cmds_need_lc, $1)=yes
        fi
        _LT_AC_TAGVAR(allow_undefined_flag, $1)=$lt_save_allow_undefined_flag
      else
        cat conftest.err 1>&5
      fi
      $rm conftest*
      AC_MSG_RESULT([$_LT_AC_TAGVAR(archive_cmds_need_lc, $1)])
      ;;
    esac
  fi
  ;;
esac
])# AC_LIBTOOL_PROG_LD_SHLIBS


# _LT_AC_FILE_LTDLL_C
# -------------------
# Be careful that the start marker always follows a newline.
AC_DEFUN([_LT_AC_FILE_LTDLL_C], [
# /* ltdll.c starts here */
# #define WIN32_LEAN_AND_MEAN
# #include <windows.h>
# #undef WIN32_LEAN_AND_MEAN
# #include <stdio.h>
#
# #ifndef __CYGWIN__
# #  ifdef __CYGWIN32__
# #    define __CYGWIN__ __CYGWIN32__
# #  endif
# #endif
#
# #ifdef __cplusplus
# extern "C" {
# #endif
# BOOL APIENTRY DllMain (HINSTANCE hInst, DWORD reason, LPVOID reserved);
# #ifdef __cplusplus
# }
# #endif
#
# #ifdef __CYGWIN__
# #include <cygwin/cygwin_dll.h>
# DECLARE_CYGWIN_DLL( DllMain );
# #endif
# HINSTANCE __hDllInstance_base;
#
# BOOL APIENTRY
# DllMain (HINSTANCE hInst, DWORD reason, LPVOID reserved)
# {
#   __hDllInstance_base = hInst;
#   return TRUE;
# }
# /* ltdll.c ends here */
])# _LT_AC_FILE_LTDLL_C


# _LT_AC_TAGVAR(VARNAME, [TAGNAME])
# ---------------------------------
AC_DEFUN([_LT_AC_TAGVAR], [ifelse([$2], [], [$1], [$1_$2])])


# old names
AC_DEFUN([AM_PROG_LIBTOOL],   [AC_PROG_LIBTOOL])
AC_DEFUN([AM_ENABLE_SHARED],  [AC_ENABLE_SHARED($@)])
AC_DEFUN([AM_ENABLE_STATIC],  [AC_ENABLE_STATIC($@)])
AC_DEFUN([AM_DISABLE_SHARED], [AC_DISABLE_SHARED($@)])
AC_DEFUN([AM_DISABLE_STATIC], [AC_DISABLE_STATIC($@)])
AC_DEFUN([AM_PROG_LD],        [AC_PROG_LD])
AC_DEFUN([AM_PROG_NM],        [AC_PROG_NM])

# This is just to silence aclocal about the macro not being used
ifelse([AC_DISABLE_FAST_INSTALL])

AC_DEFUN([LT_AC_PROG_GCJ],
[AC_CHECK_TOOL(GCJ, gcj, no)
  test "x${GCJFLAGS+set}" = xset || GCJFLAGS="-g -O2"
  AC_SUBST(GCJFLAGS)
])

AC_DEFUN([LT_AC_PROG_RC],
[AC_CHECK_TOOL(RC, windres, no)
])

# NOTE: This macro has been submitted for inclusion into   #
#  GNU Autoconf as AC_PROG_SED.  When it is available in   #
#  a released version of Autoconf we should remove this    #
#  macro and use it instead.                               #
# LT_AC_PROG_SED
# --------------
# Check for a fully-functional sed program, that truncates
# as few characters as possible.  Prefer GNU sed if found.
AC_DEFUN([LT_AC_PROG_SED],
[AC_MSG_CHECKING([for a sed that does not truncate output])
AC_CACHE_VAL(lt_cv_path_SED,
[# Loop through the user's path and test for sed and gsed.
# Then use that list of sed's as ones to test for truncation.
as_save_IFS=$IFS; IFS=$PATH_SEPARATOR
for as_dir in $PATH
do
  IFS=$as_save_IFS
  test -z "$as_dir" && as_dir=.
  for lt_ac_prog in sed gsed; do
    for ac_exec_ext in '' $ac_executable_extensions; do
      if $as_executable_p "$as_dir/$lt_ac_prog$ac_exec_ext"; then
        lt_ac_sed_list="$lt_ac_sed_list $as_dir/$lt_ac_prog$ac_exec_ext"
      fi
    done
  done
done
lt_ac_max=0
lt_ac_count=0
# Add /usr/xpg4/bin/sed as it is typically found on Solaris
# along with /bin/sed that truncates output.
for lt_ac_sed in $lt_ac_sed_list /usr/xpg4/bin/sed; do
  test ! -f $lt_ac_sed && break
  cat /dev/null > conftest.in
  lt_ac_count=0
  echo $ECHO_N "0123456789$ECHO_C" >conftest.in
  # Check for GNU sed and select it if it is found.
  if "$lt_ac_sed" --version 2>&1 < /dev/null | grep 'GNU' > /dev/null; then
    lt_cv_path_SED=$lt_ac_sed
    break
  fi
  while true; do
    cat conftest.in conftest.in >conftest.tmp
    mv conftest.tmp conftest.in
    cp conftest.in conftest.nl
    echo >>conftest.nl
    $lt_ac_sed -e 's/a$//' < conftest.nl >conftest.out || break
    cmp -s conftest.out conftest.nl || break
    # 10000 chars as input seems more than enough
    test $lt_ac_count -gt 10 && break
    lt_ac_count=`expr $lt_ac_count + 1`
    if test $lt_ac_count -gt $lt_ac_max; then
      lt_ac_max=$lt_ac_count
      lt_cv_path_SED=$lt_ac_sed
    fi
  done
done
SED=$lt_cv_path_SED
])
AC_MSG_RESULT([$SED])
])

# Do all the work for Automake.                            -*- Autoconf -*-

# This macro actually does too much some checks are only needed if
# your package does certain things.  But this isn't really a big deal.

# Copyright 1996, 1997, 1998, 1999, 2000, 2001, 2002
# Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# serial 8

# There are a few dirty hacks below to avoid letting `AC_PROG_CC' be
# written in clear, in which case automake, when reading aclocal.m4,
# will think it sees a *use*, and therefore will trigger all it's
# C support machinery.  Also note that it means that autoscan, seeing
# CC etc. in the Makefile, will ask for an AC_PROG_CC use...


AC_PREREQ([2.52])

# Autoconf 2.50 wants to disallow AM_ names.  We explicitly allow
# the ones we care about.
m4_pattern_allow([^AM_[A-Z]+FLAGS$])dnl

# AM_INIT_AUTOMAKE(PACKAGE, VERSION, [NO-DEFINE])
# AM_INIT_AUTOMAKE([OPTIONS])
# -----------------------------------------------
# The call with PACKAGE and VERSION arguments is the old style
# call (pre autoconf-2.50), which is being phased out.  PACKAGE
# and VERSION should now be passed to AC_INIT and removed from
# the call to AM_INIT_AUTOMAKE.
# We support both call styles for the transition.  After
# the next Automake release, Autoconf can make the AC_INIT
# arguments mandatory, and then we can depend on a new Autoconf
# release and drop the old call support.
AC_DEFUN([AM_INIT_AUTOMAKE],
[AC_REQUIRE([AM_SET_CURRENT_AUTOMAKE_VERSION])dnl
 AC_REQUIRE([AC_PROG_INSTALL])dnl
# test to see if srcdir already configured
if test "`cd $srcdir && pwd`" != "`pwd`" &&
   test -f $srcdir/config.status; then
  AC_MSG_ERROR([source directory already configured; run "make distclean" there first])
fi

# Define the identity of the package.
dnl Distinguish between old-style and new-style calls.
m4_ifval([$2],
[m4_ifval([$3], [_AM_SET_OPTION([no-define])])dnl
 AC_SUBST([PACKAGE], [$1])dnl
 AC_SUBST([VERSION], [$2])],
[_AM_SET_OPTIONS([$1])dnl
 AC_SUBST([PACKAGE], [AC_PACKAGE_TARNAME])dnl
 AC_SUBST([VERSION], [AC_PACKAGE_VERSION])])dnl

_AM_IF_OPTION([no-define],,
[AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE", [Name of package])
 AC_DEFINE_UNQUOTED(VERSION, "$VERSION", [Version number of package])])dnl

# Some tools Automake needs.
AC_REQUIRE([AM_SANITY_CHECK])dnl
AC_REQUIRE([AC_ARG_PROGRAM])dnl
AM_MISSING_PROG(ACLOCAL, aclocal-${am__api_version})
AM_MISSING_PROG(AUTOCONF, autoconf)
AM_MISSING_PROG(AUTOMAKE, automake-${am__api_version})
AM_MISSING_PROG(AUTOHEADER, autoheader)
AM_MISSING_PROG(MAKEINFO, makeinfo)
AM_MISSING_PROG(AMTAR, tar)
AM_PROG_INSTALL_SH
AM_PROG_INSTALL_STRIP
# We need awk for the "check" target.  The system "awk" is bad on
# some platforms.
AC_REQUIRE([AC_PROG_AWK])dnl
AC_REQUIRE([AC_PROG_MAKE_SET])dnl

_AM_IF_OPTION([no-dependencies],,
[AC_PROVIDE_IFELSE([AC_PROG_][CC],
                  [_AM_DEPENDENCIES(CC)],
                  [define([AC_PROG_][CC],
                          defn([AC_PROG_][CC])[_AM_DEPENDENCIES(CC)])])dnl
AC_PROVIDE_IFELSE([AC_PROG_][CXX],
                  [_AM_DEPENDENCIES(CXX)],
                  [define([AC_PROG_][CXX],
                          defn([AC_PROG_][CXX])[_AM_DEPENDENCIES(CXX)])])dnl
])
])

# Copyright 2002  Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA

# AM_AUTOMAKE_VERSION(VERSION)
# ----------------------------
# Automake X.Y traces this macro to ensure aclocal.m4 has been
# generated from the m4 files accompanying Automake X.Y.
AC_DEFUN([AM_AUTOMAKE_VERSION],[am__api_version="1.6"])

# AM_SET_CURRENT_AUTOMAKE_VERSION
# -------------------------------
# Call AM_AUTOMAKE_VERSION so it can be traced.
# This function is AC_REQUIREd by AC_INIT_AUTOMAKE.
AC_DEFUN([AM_SET_CURRENT_AUTOMAKE_VERSION],
	 [AM_AUTOMAKE_VERSION([1.6.3])])

# Helper functions for option handling.                    -*- Autoconf -*-

# Copyright 2001, 2002  Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# serial 2

# _AM_MANGLE_OPTION(NAME)
# -----------------------
AC_DEFUN([_AM_MANGLE_OPTION],
[[_AM_OPTION_]m4_bpatsubst($1, [[^a-zA-Z0-9_]], [_])])

# _AM_SET_OPTION(NAME)
# ------------------------------
# Set option NAME.  Presently that only means defining a flag for this option.
AC_DEFUN([_AM_SET_OPTION],
[m4_define(_AM_MANGLE_OPTION([$1]), 1)])

# _AM_SET_OPTIONS(OPTIONS)
# ----------------------------------
# OPTIONS is a space-separated list of Automake options.
AC_DEFUN([_AM_SET_OPTIONS],
[AC_FOREACH([_AM_Option], [$1], [_AM_SET_OPTION(_AM_Option)])])

# _AM_IF_OPTION(OPTION, IF-SET, [IF-NOT-SET])
# -------------------------------------------
# Execute IF-SET if OPTION is set, IF-NOT-SET otherwise.
AC_DEFUN([_AM_IF_OPTION],
[m4_ifset(_AM_MANGLE_OPTION([$1]), [$2], [$3])])

#
# Check to make sure that the build environment is sane.
#

# Copyright 1996, 1997, 2000, 2001 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# serial 3

# AM_SANITY_CHECK
# ---------------
AC_DEFUN([AM_SANITY_CHECK],
[AC_MSG_CHECKING([whether build environment is sane])
# Just in case
sleep 1
echo timestamp > conftest.file
# Do `set' in a subshell so we don't clobber the current shell's
# arguments.  Must try -L first in case configure is actually a
# symlink; some systems play weird games with the mod time of symlinks
# (eg FreeBSD returns the mod time of the symlink's containing
# directory).
if (
   set X `ls -Lt $srcdir/configure conftest.file 2> /dev/null`
   if test "$[*]" = "X"; then
      # -L didn't work.
      set X `ls -t $srcdir/configure conftest.file`
   fi
   rm -f conftest.file
   if test "$[*]" != "X $srcdir/configure conftest.file" \
      && test "$[*]" != "X conftest.file $srcdir/configure"; then

      # If neither matched, then we have a broken ls.  This can happen
      # if, for instance, CONFIG_SHELL is bash and it inherits a
      # broken ls alias from the environment.  This has actually
      # happened.  Such a system could not be considered "sane".
      AC_MSG_ERROR([ls -t appears to fail.  Make sure there is not a broken
alias in your environment])
   fi

   test "$[2]" = conftest.file
   )
then
   # Ok.
   :
else
   AC_MSG_ERROR([newly created file is older than distributed files!
Check your system clock])
fi
AC_MSG_RESULT(yes)])

#  -*- Autoconf -*-


# Copyright 1997, 1999, 2000, 2001 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# serial 3

# AM_MISSING_PROG(NAME, PROGRAM)
# ------------------------------
AC_DEFUN([AM_MISSING_PROG],
[AC_REQUIRE([AM_MISSING_HAS_RUN])
$1=${$1-"${am_missing_run}$2"}
AC_SUBST($1)])


# AM_MISSING_HAS_RUN
# ------------------
# Define MISSING if not defined so far and test if it supports --run.
# If it does, set am_missing_run to use it, otherwise, to nothing.
AC_DEFUN([AM_MISSING_HAS_RUN],
[AC_REQUIRE([AM_AUX_DIR_EXPAND])dnl
test x"${MISSING+set}" = xset || MISSING="\${SHELL} $am_aux_dir/missing"
# Use eval to expand $SHELL
if eval "$MISSING --run true"; then
  am_missing_run="$MISSING --run "
else
  am_missing_run=
  AC_MSG_WARN([`missing' script is too old or missing])
fi
])

# AM_AUX_DIR_EXPAND

# Copyright 2001 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# For projects using AC_CONFIG_AUX_DIR([foo]), Autoconf sets
# $ac_aux_dir to `$srcdir/foo'.  In other projects, it is set to
# `$srcdir', `$srcdir/..', or `$srcdir/../..'.
#
# Of course, Automake must honor this variable whenever it calls a
# tool from the auxiliary directory.  The problem is that $srcdir (and
# therefore $ac_aux_dir as well) can be either absolute or relative,
# depending on how configure is run.  This is pretty annoying, since
# it makes $ac_aux_dir quite unusable in subdirectories: in the top
# source directory, any form will work fine, but in subdirectories a
# relative path needs to be adjusted first.
#
# $ac_aux_dir/missing
#    fails when called from a subdirectory if $ac_aux_dir is relative
# $top_srcdir/$ac_aux_dir/missing
#    fails if $ac_aux_dir is absolute,
#    fails when called from a subdirectory in a VPATH build with
#          a relative $ac_aux_dir
#
# The reason of the latter failure is that $top_srcdir and $ac_aux_dir
# are both prefixed by $srcdir.  In an in-source build this is usually
# harmless because $srcdir is `.', but things will broke when you
# start a VPATH build or use an absolute $srcdir.
#
# So we could use something similar to $top_srcdir/$ac_aux_dir/missing,
# iff we strip the leading $srcdir from $ac_aux_dir.  That would be:
#   am_aux_dir='\$(top_srcdir)/'`expr "$ac_aux_dir" : "$srcdir//*\(.*\)"`
# and then we would define $MISSING as
#   MISSING="\${SHELL} $am_aux_dir/missing"
# This will work as long as MISSING is not called from configure, because
# unfortunately $(top_srcdir) has no meaning in configure.
# However there are other variables, like CC, which are often used in
# configure, and could therefore not use this "fixed" $ac_aux_dir.
#
# Another solution, used here, is to always expand $ac_aux_dir to an
# absolute PATH.  The drawback is that using absolute paths prevent a
# configured tree to be moved without reconfiguration.

# Rely on autoconf to set up CDPATH properly.
AC_PREREQ([2.50])

AC_DEFUN([AM_AUX_DIR_EXPAND], [
# expand $ac_aux_dir to an absolute path
am_aux_dir=`cd $ac_aux_dir && pwd`
])

# AM_PROG_INSTALL_SH
# ------------------
# Define $install_sh.

# Copyright 2001 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

AC_DEFUN([AM_PROG_INSTALL_SH],
[AC_REQUIRE([AM_AUX_DIR_EXPAND])dnl
install_sh=${install_sh-"$am_aux_dir/install-sh"}
AC_SUBST(install_sh)])

# AM_PROG_INSTALL_STRIP

# Copyright 2001 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# One issue with vendor `install' (even GNU) is that you can't
# specify the program used to strip binaries.  This is especially
# annoying in cross-compiling environments, where the build's strip
# is unlikely to handle the host's binaries.
# Fortunately install-sh will honor a STRIPPROG variable, so we
# always use install-sh in `make install-strip', and initialize
# STRIPPROG with the value of the STRIP variable (set by the user).
AC_DEFUN([AM_PROG_INSTALL_STRIP],
[AC_REQUIRE([AM_PROG_INSTALL_SH])dnl
# Installed binaries are usually stripped using `strip' when the user
# run `make install-strip'.  However `strip' might not be the right
# tool to use in cross-compilation environments, therefore Automake
# will honor the `STRIP' environment variable to overrule this program.
dnl Don't test for $cross_compiling = yes, because it might be `maybe'.
if test "$cross_compiling" != no; then
  AC_CHECK_TOOL([STRIP], [strip], :)
fi
INSTALL_STRIP_PROGRAM="\${SHELL} \$(install_sh) -c -s"
AC_SUBST([INSTALL_STRIP_PROGRAM])])

# serial 4						-*- Autoconf -*-

# Copyright 1999, 2000, 2001 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.


# There are a few dirty hacks below to avoid letting `AC_PROG_CC' be
# written in clear, in which case automake, when reading aclocal.m4,
# will think it sees a *use*, and therefore will trigger all it's
# C support machinery.  Also note that it means that autoscan, seeing
# CC etc. in the Makefile, will ask for an AC_PROG_CC use...



# _AM_DEPENDENCIES(NAME)
# ----------------------
# See how the compiler implements dependency checking.
# NAME is "CC", "CXX", "GCJ", or "OBJC".
# We try a few techniques and use that to set a single cache variable.
#
# We don't AC_REQUIRE the corresponding AC_PROG_CC since the latter was
# modified to invoke _AM_DEPENDENCIES(CC); we would have a circular
# dependency, and given that the user is not expected to run this macro,
# just rely on AC_PROG_CC.
AC_DEFUN([_AM_DEPENDENCIES],
[AC_REQUIRE([AM_SET_DEPDIR])dnl
AC_REQUIRE([AM_OUTPUT_DEPENDENCY_COMMANDS])dnl
AC_REQUIRE([AM_MAKE_INCLUDE])dnl
AC_REQUIRE([AM_DEP_TRACK])dnl

ifelse([$1], CC,   [depcc="$CC"   am_compiler_list=],
       [$1], CXX,  [depcc="$CXX"  am_compiler_list=],
       [$1], OBJC, [depcc="$OBJC" am_compiler_list='gcc3 gcc'],
       [$1], GCJ,  [depcc="$GCJ"  am_compiler_list='gcc3 gcc'],
                   [depcc="$$1"   am_compiler_list=])

AC_CACHE_CHECK([dependency style of $depcc],
               [am_cv_$1_dependencies_compiler_type],
[if test -z "$AMDEP_TRUE" && test -f "$am_depcomp"; then
  # We make a subdir and do the tests there.  Otherwise we can end up
  # making bogus files that we don't know about and never remove.  For
  # instance it was reported that on HP-UX the gcc test will end up
  # making a dummy file named `D' -- because `-MD' means `put the output
  # in D'.
  mkdir conftest.dir
  # Copy depcomp to subdir because otherwise we won't find it if we're
  # using a relative directory.
  cp "$am_depcomp" conftest.dir
  cd conftest.dir

  am_cv_$1_dependencies_compiler_type=none
  if test "$am_compiler_list" = ""; then
     am_compiler_list=`sed -n ['s/^#*\([a-zA-Z0-9]*\))$/\1/p'] < ./depcomp`
  fi
  for depmode in $am_compiler_list; do
    # We need to recreate these files for each test, as the compiler may
    # overwrite some of them when testing with obscure command lines.
    # This happens at least with the AIX C compiler.
    echo '#include "conftest.h"' > conftest.c
    echo 'int i;' > conftest.h
    echo "${am__include} ${am__quote}conftest.Po${am__quote}" > confmf

    case $depmode in
    nosideeffect)
      # after this tag, mechanisms are not by side-effect, so they'll
      # only be used when explicitly requested
      if test "x$enable_dependency_tracking" = xyes; then
	continue
      else
	break
      fi
      ;;
    none) break ;;
    esac
    # We check with `-c' and `-o' for the sake of the "dashmstdout"
    # mode.  It turns out that the SunPro C++ compiler does not properly
    # handle `-M -o', and we need to detect this.
    if depmode=$depmode \
       source=conftest.c object=conftest.o \
       depfile=conftest.Po tmpdepfile=conftest.TPo \
       $SHELL ./depcomp $depcc -c conftest.c -o conftest.o >/dev/null 2>&1 &&
       grep conftest.h conftest.Po > /dev/null 2>&1 &&
       ${MAKE-make} -s -f confmf > /dev/null 2>&1; then
      am_cv_$1_dependencies_compiler_type=$depmode
      break
    fi
  done

  cd ..
  rm -rf conftest.dir
else
  am_cv_$1_dependencies_compiler_type=none
fi
])
AC_SUBST([$1DEPMODE], [depmode=$am_cv_$1_dependencies_compiler_type])
])


# AM_SET_DEPDIR
# -------------
# Choose a directory name for dependency files.
# This macro is AC_REQUIREd in _AM_DEPENDENCIES
AC_DEFUN([AM_SET_DEPDIR],
[rm -f .deps 2>/dev/null
mkdir .deps 2>/dev/null
if test -d .deps; then
  DEPDIR=.deps
else
  # MS-DOS does not allow filenames that begin with a dot.
  DEPDIR=_deps
fi
rmdir .deps 2>/dev/null
AC_SUBST([DEPDIR])
])


# AM_DEP_TRACK
# ------------
AC_DEFUN([AM_DEP_TRACK],
[AC_ARG_ENABLE(dependency-tracking,
[  --disable-dependency-tracking Speeds up one-time builds
  --enable-dependency-tracking  Do not reject slow dependency extractors])
if test "x$enable_dependency_tracking" != xno; then
  am_depcomp="$ac_aux_dir/depcomp"
  AMDEPBACKSLASH='\'
fi
AM_CONDITIONAL([AMDEP], [test "x$enable_dependency_tracking" != xno])
AC_SUBST([AMDEPBACKSLASH])
])

# Generate code to set up dependency tracking.   -*- Autoconf -*-

# Copyright 1999, 2000, 2001, 2002 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

#serial 2

# _AM_OUTPUT_DEPENDENCY_COMMANDS
# ------------------------------
AC_DEFUN([_AM_OUTPUT_DEPENDENCY_COMMANDS],
[for mf in $CONFIG_FILES; do
  # Strip MF so we end up with the name of the file.
  mf=`echo "$mf" | sed -e 's/:.*$//'`
  # Check whether this is an Automake generated Makefile or not.
  # We used to match only the files named `Makefile.in', but
  # some people rename them; so instead we look at the file content.
  # Grep'ing the first line is not enough: some people post-process
  # each Makefile.in and add a new line on top of each file to say so.
  # So let's grep whole file.
  if grep '^#.*generated by automake' $mf > /dev/null 2>&1; then
    dirpart=`AS_DIRNAME("$mf")`
  else
    continue
  fi
  grep '^DEP_FILES *= *[[^ @%:@]]' < "$mf" > /dev/null || continue
  # Extract the definition of DEP_FILES from the Makefile without
  # running `make'.
  DEPDIR=`sed -n -e '/^DEPDIR = / s///p' < "$mf"`
  test -z "$DEPDIR" && continue
  # When using ansi2knr, U may be empty or an underscore; expand it
  U=`sed -n -e '/^U = / s///p' < "$mf"`
  test -d "$dirpart/$DEPDIR" || mkdir "$dirpart/$DEPDIR"
  # We invoke sed twice because it is the simplest approach to
  # changing $(DEPDIR) to its actual value in the expansion.
  for file in `sed -n -e '
    /^DEP_FILES = .*\\\\$/ {
      s/^DEP_FILES = //
      :loop
	s/\\\\$//
	p
	n
	/\\\\$/ b loop
      p
    }
    /^DEP_FILES = / s/^DEP_FILES = //p' < "$mf" | \
       sed -e 's/\$(DEPDIR)/'"$DEPDIR"'/g' -e 's/\$U/'"$U"'/g'`; do
    # Make sure the directory exists.
    test -f "$dirpart/$file" && continue
    fdir=`AS_DIRNAME(["$file"])`
    AS_MKDIR_P([$dirpart/$fdir])
    # echo "creating $dirpart/$file"
    echo '# dummy' > "$dirpart/$file"
  done
done
])# _AM_OUTPUT_DEPENDENCY_COMMANDS


# AM_OUTPUT_DEPENDENCY_COMMANDS
# -----------------------------
# This macro should only be invoked once -- use via AC_REQUIRE.
#
# This code is only required when automatic dependency tracking
# is enabled.  FIXME.  This creates each `.P' file that we will
# need in order to bootstrap the dependency handling code.
AC_DEFUN([AM_OUTPUT_DEPENDENCY_COMMANDS],
[AC_CONFIG_COMMANDS([depfiles],
     [test x"$AMDEP_TRUE" != x"" || _AM_OUTPUT_DEPENDENCY_COMMANDS],
     [AMDEP_TRUE="$AMDEP_TRUE" ac_aux_dir="$ac_aux_dir"])
])

# Copyright 2001 Free Software Foundation, Inc.             -*- Autoconf -*-

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# serial 2

# AM_MAKE_INCLUDE()
# -----------------
# Check to see how make treats includes.
AC_DEFUN([AM_MAKE_INCLUDE],
[am_make=${MAKE-make}
cat > confinc << 'END'
doit:
	@echo done
END
# If we don't find an include directive, just comment out the code.
AC_MSG_CHECKING([for style of include used by $am_make])
am__include="#"
am__quote=
_am_result=none
# First try GNU make style include.
echo "include confinc" > confmf
# We grep out `Entering directory' and `Leaving directory'
# messages which can occur if `w' ends up in MAKEFLAGS.
# In particular we don't look at `^make:' because GNU make might
# be invoked under some other name (usually "gmake"), in which
# case it prints its new name instead of `make'.
if test "`$am_make -s -f confmf 2> /dev/null | fgrep -v 'ing directory'`" = "done"; then
   am__include=include
   am__quote=
   _am_result=GNU
fi
# Now try BSD make style include.
if test "$am__include" = "#"; then
   echo '.include "confinc"' > confmf
   if test "`$am_make -s -f confmf 2> /dev/null`" = "done"; then
      am__include=.include
      am__quote="\""
      _am_result=BSD
   fi
fi
AC_SUBST(am__include)
AC_SUBST(am__quote)
AC_MSG_RESULT($_am_result)
rm -f confinc confmf
])

# AM_CONDITIONAL                                              -*- Autoconf -*-

# Copyright 1997, 2000, 2001 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# serial 5

AC_PREREQ(2.52)

# AM_CONDITIONAL(NAME, SHELL-CONDITION)
# -------------------------------------
# Define a conditional.
AC_DEFUN([AM_CONDITIONAL],
[ifelse([$1], [TRUE],  [AC_FATAL([$0: invalid condition: $1])],
        [$1], [FALSE], [AC_FATAL([$0: invalid condition: $1])])dnl
AC_SUBST([$1_TRUE])
AC_SUBST([$1_FALSE])
if $2; then
  $1_TRUE=
  $1_FALSE='#'
else
  $1_TRUE='#'
  $1_FALSE=
fi
AC_CONFIG_COMMANDS_PRE(
[if test -z "${$1_TRUE}" && test -z "${$1_FALSE}"; then
  AC_MSG_ERROR([conditional \"$1\" was never defined.
Usually this means the macro was only invoked conditionally.])
fi])])

# Like AC_CONFIG_HEADER, but automatically create stamp file. -*- Autoconf -*-

# Copyright 1996, 1997, 2000, 2001 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

AC_PREREQ([2.52])

# serial 6

# When config.status generates a header, we must update the stamp-h file.
# This file resides in the same directory as the config header
# that is generated.  We must strip everything past the first ":",
# and everything past the last "/".

# _AM_DIRNAME(PATH)
# -----------------
# Like AS_DIRNAME, only do it during macro expansion
AC_DEFUN([_AM_DIRNAME],
       [m4_if(regexp([$1], [^.*[^/]//*[^/][^/]*/*$]), -1,
	      m4_if(regexp([$1], [^//\([^/]\|$\)]), -1,
		    m4_if(regexp([$1], [^/.*]), -1,
			  [.],
			  patsubst([$1], [^\(/\).*], [\1])),
		    patsubst([$1], [^\(//\)\([^/].*\|$\)], [\1])),
	      patsubst([$1], [^\(.*[^/]\)//*[^/][^/]*/*$], [\1]))[]dnl
])# _AM_DIRNAME


# The stamp files are numbered to have different names.
# We could number them on a directory basis, but that's additional
# complications, let's have a unique counter.
m4_define([_AM_STAMP_Count], [0])


# _AM_STAMP(HEADER)
# -----------------
# The name of the stamp file for HEADER.
AC_DEFUN([_AM_STAMP],
[m4_define([_AM_STAMP_Count], m4_incr(_AM_STAMP_Count))dnl
AS_ESCAPE(_AM_DIRNAME(patsubst([$1],
                               [:.*])))/stamp-h[]_AM_STAMP_Count])


# _AM_CONFIG_HEADER(HEADER[:SOURCES], COMMANDS, INIT-COMMANDS)
# ------------------------------------------------------------
# We used to try to get a real timestamp in stamp-h.  But the fear is that
# that will cause unnecessary cvs conflicts.
AC_DEFUN([_AM_CONFIG_HEADER],
[# Add the stamp file to the list of files AC keeps track of,
# along with our hook.
AC_CONFIG_HEADERS([$1],
                  [# update the timestamp
echo 'timestamp for $1' >"_AM_STAMP([$1])"
$2],
                  [$3])
])# _AM_CONFIG_HEADER


# AM_CONFIG_HEADER(HEADER[:SOURCES]..., COMMANDS, INIT-COMMANDS)
# --------------------------------------------------------------
AC_DEFUN([AM_CONFIG_HEADER],
[AC_FOREACH([_AM_File], [$1], [_AM_CONFIG_HEADER(_AM_File, [$2], [$3])])
])# AM_CONFIG_HEADER

# Add --enable-maintainer-mode option to configure.
# From Jim Meyering

# Copyright 1996, 1998, 2000, 2001 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# serial 1

AC_DEFUN([AM_MAINTAINER_MODE],
[AC_MSG_CHECKING([whether to enable maintainer-specific portions of Makefiles])
  dnl maintainer-mode is disabled by default
  AC_ARG_ENABLE(maintainer-mode,
[  --enable-maintainer-mode enable make rules and dependencies not useful
                          (and sometimes confusing) to the casual installer],
      USE_MAINTAINER_MODE=$enableval,
      USE_MAINTAINER_MODE=no)
  AC_MSG_RESULT([$USE_MAINTAINER_MODE])
  AM_CONDITIONAL(MAINTAINER_MODE, [test $USE_MAINTAINER_MODE = yes])
  MAINT=$MAINTAINER_MODE_TRUE
  AC_SUBST(MAINT)dnl
]
)


# Copyright 1996, 1997, 1998, 2000, 2001 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# serial 1

AC_DEFUN([AM_C_PROTOTYPES],
[AC_REQUIRE([AM_PROG_CC_STDC])
AC_REQUIRE([AC_PROG_CPP])
AC_MSG_CHECKING([for function prototypes])
if test "$am_cv_prog_cc_stdc" != no; then
  AC_MSG_RESULT(yes)
  AC_DEFINE(PROTOTYPES,1,[Define if compiler has function prototypes])
  U= ANSI2KNR=
else
  AC_MSG_RESULT(no)
  U=_ ANSI2KNR=./ansi2knr
fi
# Ensure some checks needed by ansi2knr itself.
AC_HEADER_STDC
AC_CHECK_HEADERS(string.h)
AC_SUBST(U)dnl
AC_SUBST(ANSI2KNR)dnl
])


# Copyright 1996, 1997, 1999, 2000, 2001 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# serial 1

# @defmac AC_PROG_CC_STDC
# @maindex PROG_CC_STDC
# @ovindex CC
# If the C compiler in not in ANSI C mode by default, try to add an option
# to output variable @code{CC} to make it so.  This macro tries various
# options that select ANSI C on some system or another.  It considers the
# compiler to be in ANSI C mode if it handles function prototypes correctly.
#
# If you use this macro, you should check after calling it whether the C
# compiler has been set to accept ANSI C; if not, the shell variable
# @code{am_cv_prog_cc_stdc} is set to @samp{no}.  If you wrote your source
# code in ANSI C, you can make an un-ANSIfied copy of it by using the
# program @code{ansi2knr}, which comes with Ghostscript.
# @end defmac

AC_DEFUN([AM_PROG_CC_STDC],
[AC_REQUIRE([AC_PROG_CC])
AC_BEFORE([$0], [AC_C_INLINE])
AC_BEFORE([$0], [AC_C_CONST])
dnl Force this before AC_PROG_CPP.  Some cpp's, eg on HPUX, require
dnl a magic option to avoid problems with ANSI preprocessor commands
dnl like #elif.
dnl FIXME: can't do this because then AC_AIX won't work due to a
dnl circular dependency.
dnl AC_BEFORE([$0], [AC_PROG_CPP])
AC_MSG_CHECKING([for ${CC-cc} option to accept ANSI C])
AC_CACHE_VAL(am_cv_prog_cc_stdc,
[am_cv_prog_cc_stdc=no
ac_save_CC="$CC"
# Don't try gcc -ansi; that turns off useful extensions and
# breaks some systems' header files.
# AIX			-qlanglvl=ansi
# Ultrix and OSF/1	-std1
# HP-UX 10.20 and later	-Ae
# HP-UX older versions	-Aa -D_HPUX_SOURCE
# SVR4			-Xc -D__EXTENSIONS__
for ac_arg in "" -qlanglvl=ansi -std1 -Ae "-Aa -D_HPUX_SOURCE" "-Xc -D__EXTENSIONS__"
do
  CC="$ac_save_CC $ac_arg"
  AC_TRY_COMPILE(
[#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
/* Most of the following tests are stolen from RCS 5.7's src/conf.sh.  */
struct buf { int x; };
FILE * (*rcsopen) (struct buf *, struct stat *, int);
static char *e (p, i)
     char **p;
     int i;
{
  return p[i];
}
static char *f (char * (*g) (char **, int), char **p, ...)
{
  char *s;
  va_list v;
  va_start (v,p);
  s = g (p, va_arg (v,int));
  va_end (v);
  return s;
}
int test (int i, double x);
struct s1 {int (*f) (int a);};
struct s2 {int (*f) (double a);};
int pairnames (int, char **, FILE *(*)(struct buf *, struct stat *, int), int, int);
int argc;
char **argv;
], [
return f (e, argv, 0) != argv[0]  ||  f (e, argv, 1) != argv[1];
],
[am_cv_prog_cc_stdc="$ac_arg"; break])
done
CC="$ac_save_CC"
])
if test -z "$am_cv_prog_cc_stdc"; then
  AC_MSG_RESULT([none needed])
else
  AC_MSG_RESULT([$am_cv_prog_cc_stdc])
fi
case "x$am_cv_prog_cc_stdc" in
  x|xno) ;;
  *) CC="$CC $am_cv_prog_cc_stdc" ;;
esac
])


# Copyright 1998, 1999, 2000, 2001, 2002 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# serial 3

AC_PREREQ(2.50)

# AM_PROG_LEX
# -----------
# Autoconf leaves LEX=: if lex or flex can't be found.  Change that to a
# "missing" invocation, for better error output.
AC_DEFUN([AM_PROG_LEX],
[AC_REQUIRE([AM_MISSING_HAS_RUN])dnl
AC_REQUIRE([AC_PROG_LEX])dnl
if test "$LEX" = :; then
  LEX=${am_missing_run}flex
fi])

