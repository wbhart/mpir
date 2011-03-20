@echo off
::  Copyright 2011 Jason Moxham
::
::  This file is part of the MPIR Library.
::
::  The MPIR Library is free software; you can redistribute it and/or modify
::  it under the terms of the GNU Lesser General Public License as published
::  by the Free Software Foundation; either version 2.1 of the License, or (at
::  your option) any later version.
::
::  The MPIR Library is distributed in the hope that it will be useful, but
::  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
::  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
::  License for more details.
::
::  You should have received a copy of the GNU Lesser General Public License
::  along with the MPIR Library; see the file COPYING.LIB.  If not, write
::  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
::  Boston, MA 02110-1301, USA.
::

set MPIRDIR=..\mpn\x86_64w\nehalem\
set YASM="%VS90COMNTOOLS%\..\..\VC\bin\"
md speed

echo int main(void){return 0;} > comptest.c
cl /nologo comptest.c > nul 2>&1
if errorlevel 1 (
	call "%VS90COMNTOOLS%\..\..\VC\vcvarsall.bat" amd64
)
del comptest.*

::static
::set OPT=/Ox /Oi /Ot /D "NDEBUG" /D "_LIB" /D "HAVE_CONFIG_H" /D "PIC" /D "_MBCS" /MT /GS- /FD /nologo /Zi /favor:INTEL64
set OPT=/Od /Oi /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "HAVE_GETRUSAGE" /D "HAVE_GETTIMEOFDAY" /D "_UNICODE" /D "UNICODE" /FD /EHsc /MT /Gy /nologo /Zi
::dll
::set OPT=/Ox         /D "NDEBUG"           /D "HAVE_CONFIG_H" /D "__GMP_LIBGMP_DLL" /D "__GMP_WITHIN_GMP" /D "__GMP_WITHIN_GMPXX" /D "_WINDLL" /D "_MBCS" /GF /FD /EHsc /MD /GS- /W3 /nologo /Zi /Gd

copy ..\build.vc10\unistd.h .
copy ..\build.vc10\getopt.h .
copy ..\build.vc10\win_timing.h .
copy ..\build.vc10\getrusage.h .
copy ..\build.vc10\gettimeofday.h .

cd speed
cl %OPT% /c ..\..\tests\misc.c /I..\..
cl %OPT% /c ..\..\tests\memory.c /I..\..
cl %OPT% /c ..\..\tests\refmpn.c /I..\..
cl %OPT% /c ..\..\build.vc10\win_timing.c
cl %OPT% /c ..\..\build.vc10\getopt.c
for %%X in (        ..\..\tune/common.c        ..\..\tune/mod_1_div.c        ..\..\tune/set_strb.c ..\..\tune/divrem1div.c    ..\..\tune/gcd_bin.c   ..\..\tune/mod_1_inv.c       ..\..\tune/divrem1inv.c    ..\..\tune/gcdextod.c  ..\..\tune/modlinv.c          ..\..\tune/set_strs.c ..\..\tune/divrem2div.c    ..\..\tune/gcdextos.c  ..\..\tune/noop.c             ..\..\tune/divrem2inv.c    ..\..\tune/jacbase1.c  ..\..\tune/powm_mod.c       ..\..\tune/fac_ui_large.c  ..\..\tune/jacbase2.c  ..\..\tune/powm_redc.c    ..\..\tune/fac_ui_small.c  ..\..\tune/jacbase3.c  ..\..\tune/preinv_divrem_1.c  ) do (
	cl -c %OPT% /I..\.. /I..\..\tests /I.. %%X
)
cl %OPT% /I..\.. /I.. /I..\..\tests ..\..\tune\speed.c *.obj ..\mpir.lib advapi32.lib psapi.lib
cd ..
