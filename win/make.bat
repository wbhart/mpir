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

if not exist config.params.bat (
	echo run configure first
	exit /b 1
)
call config.params.bat
if "%1" == "" goto :make
if "%1" == "check" goto :check
if "%1" == "clean" goto :clean
if "%1" == "tune" goto :tune
if "%1" == "speed" goto :speed
echo Usage: make [clean^|check^|tune^|speed]
exit /b 1

:make

md mpn mpz mpq mpf printf scanf cxx > nul 2>&1

copy ..\build.vc10\gen_mpir_h.bat .
copy ..\build.vc10\out_copy_rename.bat .
copy ..\build.vc10\gen_config_h.bat .
copy ..\build.vc10\cfg.h .

if %ABI% == 64 (set LOCALABI=x64)
if %ABI% == 32 (set LOCALABI=win32)
call gen_mpir_h %LOCALABI%
copy ..\mpn\generic\gmp-mparam.h .. > nul 2>&1
for %%X in ( %MPNPATH% ) do (
	copy ..\mpn\%%X\gmp-mparam.h .. > nul 2>&1
	call gen_config_h ..\mpn\%%X > nul 2>&1
)

echo #include ^<stdint.h^> > comptest.c
echo int main(void){return 0;} >> comptest.c
cl /nologo comptest.c > nul 2>&1
if errorlevel 1 (
	echo #undef HAVE_STDINT_H >> ..\config.h
	echo #undef HAVE_INTMAX_T >> ..\config.h
	echo #undef HAVE_UINTMAX_T >> ..\config.h
	echo #undef HAVE_PTRDIFF_T >> ..\config.h
	echo #undef HAVE_UINT_LEAST32_T >> ..\config.h
	echo #undef SIZEOF_UINTMAX_T >> ..\config.h
)
del comptest.*

set OPT=%FLAGS% %FLAGS1% /c

if %ABI% == 64 (set LOCALDIR=x86_64w)
if %ABI% == 32 (set LOCALDIR=x86w)
:: just compile all generic and just overwrite with asm
cd mpn
for %%X in ( ..\..\mpn\generic\*.c) do (
	cl %OPT% -I..\.. %%X
)
for %%X in ( %MPNPATH% ) do (
	for %%i in ( ..\..\mpn\%%X\*.asm ) do (
		%YASMEXE% -I ..\..\mpn\%LOCALDIR% -f %LOCALABI% %%i
		echo assemblin %%i
	)
)
cd ..

cd mpz
for %%X in ( ..\..\mpz\*.c) do (
	cl %OPT% -I..\.. %%X
)
cd ..

cd mpf
for %%X in ( ..\..\mpf\*.c) do (
	cl %OPT% -I..\.. %%X
)
cd ..

cd mpq
for %%X in ( ..\..\mpq\*.c) do (
	cl %OPT% -I..\.. %%X
)
cd ..

cd printf
for %%X in ( ..\..\printf\*.c) do (
	cl %OPT% -I..\.. %%X
)
cd ..

cd scanf
for %%X in ( ..\..\scanf\*.c) do (
	cl %OPT% -I..\.. %%X
)
cd ..

for %%X in ( ..\assert.c ..\compat.c ..\errno.c ..\extract-dbl.c ..\invalid.c ..\memory.c ..\mp_bpl.c ..\mp_clz_tab.c ..\mp_dv_tab.c ..\mp_get_fns.c ..\mp_minv_tab.c ..\mp_set_fns.c ..\randbui.c ..\randclr.c ..\randdef.c ..\randiset.c ..\randlc2s.c ..\randlc2x.c ..\randmt.c ..\randmts.c ..\randmui.c ..\rands.c ..\randsd.c ..\randsdui.c ..\tal-reent.c ..\version.c ) do (
	cl %OPT% -I.. %%X
)

cd cxx
for %%X in ( ..\..\cxx\*.cc) do (
	cl /EHsc %OPT% -I..\.. %%X
)
cd ..

if %LIBTYPE% == dll (
	link /DLL /NODEFAULTLIB:LIBCMT.lib /nologo scanf\*.obj printf\*.obj mpz\*.obj mpq\*.obj mpf\*.obj mpn\*.obj cxx\*.obj *.obj /out:mpir.%LIBTYPE%
)
if %LIBTYPE% == lib (
lib /nologo scanf\*.obj printf\*.obj mpz\*.obj mpq\*.obj mpf\*.obj mpn\*.obj cxx\*.obj *.obj /out:mpir.%LIBTYPE%
)
exit /b 0
















:check

md tests
cd tests
md mpn mpz mpq mpf rand misc cxx devel > nul 2>&1
cd ..

set OPT=%FLAGS% %FLAGS1%

set MPIRLIB=?????
if %LIBTYPE% == lib (set MPIRLIB=..\..\mpir.lib)

set MPIRLIB1=/link ..\mpir.lib
if %LIBTYPE% == lib (set MPIRLIB1=..\mpir.lib)

cd tests

cl %OPT% /c ..\..\tests\memory.c /I..\..
cl %OPT% /c ..\..\tests\misc.c   /I..\..
cl %OPT% /c ..\..\tests\trace.c  /I..\..
cl %OPT% /c ..\..\tests\refmpn.c /I..\..
cl %OPT% /c ..\..\tests\refmpz.c /I..\..
cl %OPT% /c ..\..\tests\refmpq.c /I..\..
cl %OPT% /c ..\..\tests\refmpf.c /I..\..
:: these only needed for try.exe
cl %OPT% /c ..\..\tests\spinner.c /I..\..
cl %OPT% /c ..\..\build.vc10\getopt.c /I..\..

for %%X in ( ..\..\tests\t-*.c) do (
	cl %OPT% /I..\.. /I..\..\tests %%X misc.obj memory.obj trace.obj refmpn.obj %MPIRLIB1%
	if errorlevel 1 ( echo %%X FAILS )	
)
for %%X in ( *.exe) do (
	echo testing %%X
	%%X
	if errorlevel 1 ( echo %%X FAILS )
)

cd mpn
for %%X in ( ..\..\..\tests\mpn\t-*.c) do (
	cl %OPT% /I..\..\.. /I..\..\..\tests %%X ..\misc.obj ..\memory.obj ..\trace.obj ..\refmpn.obj %MPIRLIB%
)
for %%X in ( *.exe) do (
	echo testing mpn_%%X
	%%X
	if errorlevel 1 ( echo %%X FAILS )
)
cd ..

cd mpz
for %%X in ( ..\..\..\tests\mpz\*.c) do (
	cl %OPT% /I..\..\.. /I..\..\..\tests %%X ..\misc.obj ..\memory.obj ..\trace.obj ..\refmpn.obj ..\refmpz.obj %MPIRLIB%
)
for %%X in ( *.exe) do (
	echo testing mpz_%%X
	%%X
	if errorlevel 1 ( echo %%X FAILS )
)
cd ..

cd mpq
for %%X in ( ..\..\..\tests\mpq\t-*.c) do (
	cl %OPT% /I..\..\.. /I..\..\..\tests %%X ..\misc.obj ..\memory.obj ..\trace.obj ..\refmpn.obj ..\refmpz.obj ..\refmpq.obj %MPIRLIB%
)
for %%X in ( *.exe) do (
	echo testing mpq_%%X
	%%X
	if errorlevel 1 ( echo %%X FAILS )
)
cd ..

cd mpf
for %%X in ( ..\..\..\tests\mpf\*.c) do (
	cl %OPT% /I..\..\.. /I..\..\..\tests %%X ..\misc.obj ..\memory.obj ..\trace.obj ..\refmpn.obj ..\refmpf.obj %MPIRLIB%
)
for %%X in ( *.exe) do (
	echo testing mpf_%%X
	%%X
	if errorlevel 1 ( echo %%X FAILS )
)
cd ..

cd rand
for %%X in ( ..\..\..\tests\rand\t-*.c) do (
	cl %OPT% /I..\..\.. /I..\..\..\tests %%X ..\misc.obj ..\memory.obj ..\trace.obj ..\refmpn.obj %MPIRLIB%
)
for %%X in ( *.exe) do (
	echo testing rand_%%X
	%%X
	if errorlevel 1 ( echo %%X FAILS )
)
cd ..

cd misc
for %%X in ( ..\..\..\tests\misc\t-*.c) do (
	cl %OPT% /I..\..\.. /I..\..\..\tests %%X ..\misc.obj ..\memory.obj ..\trace.obj ..\refmpn.obj %MPIRLIB%
)
for %%X in ( *.exe) do (
	echo testing misc_%%X
	%%X
	if errorlevel 1 ( echo %%X FAILS )
)
cd ..

cd cxx
for %%X in ( ..\..\..\tests\cxx\t-*.cc) do (
	cl /EHsc %OPT% /I..\..\.. /I..\..\..\tests %%X ..\misc.obj ..\memory.obj ..\trace.obj ..\refmpn.obj %MPIRLIB%
)
for %%X in ( *.exe) do (
	echo testing cxx_%%X
	%%X
	if errorlevel 1 ( echo %%X FAILS )
)
cd ..

cd devel
cl %OPT% ..\..\..\tests\devel\try.c /I..\..\..\ /I..\..\..\tests ..\refmpn.obj ..\refmpz.obj ..\trace.obj ..\spinner.obj ..\misc.obj ..\memory.obj ..\getopt.obj %MPIRLIB%
cd ..

cd ..

exit /b 0











:clean

del *.obj ..\mpir.h ..\config.h ..\gmp-mparam.h mpir.lib *.idb *.pdb > nul 2>&1
rmdir /S/Q mpn mpz mpq mpf scanf printf tests cxx tune speed > nul 2>&1
del gen_mpir_h.bat out_copy_rename.bat gen_config_h.bat cfg.h > nul 2>&1
del getopt.h getrusage.h gettimeofday.h unistd.h win_timing.h > nul 2>&1 
del config.guess.* config.params.bat mpir.dll > nul 2>&1

exit /b 0




:speed

md speed

set OPT=%FLAGS% %FLAGS1%
set MPIRLIB=
if %LIBTYPE% == lib (set MPIRLIB=mpir.lib)

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
cl %OPT% /c ..\..\build.vc10\getopt.c /I..\..
for %%X in (        ..\..\tune\common.c        ..\..\tune\mod_1_div.c        ..\..\tune\set_strb.c ..\..\tune\divrem1div.c    ..\..\tune\gcd_bin.c   ..\..\tune\mod_1_inv.c       ..\..\tune\divrem1inv.c    ..\..\tune\gcdextod.c  ..\..\tune\modlinv.c          ..\..\tune\set_strs.c ..\..\tune\divrem2div.c    ..\..\tune\gcdextos.c  ..\..\tune\noop.c             ..\..\tune\divrem2inv.c    ..\..\tune\jacbase1.c  ..\..\tune\powm_mod.c       ..\..\tune\fac_ui_large.c  ..\..\tune\jacbase2.c  ..\..\tune\powm_redc.c    ..\..\tune\fac_ui_small.c  ..\..\tune\jacbase3.c  ..\..\tune\preinv_divrem_1.c  ) do (
	cl -c %OPT% /I..\.. /I..\..\tests /I.. %%X
)
cl %OPT% /I..\.. /I.. /I..\..\tests ..\..\tune\speed.c *.obj ..\%MPIRLIB% advapi32.lib psapi.lib
cd ..

exit /b 0



:tune

md tune

set OPT=%FLAGS% %FLAGS1%
set MPIRLIB=
if %LIBTYPE% == lib (set MPIRLIB=mpir.lib)

copy ..\build.vc10\unistd.h .
copy ..\build.vc10\getopt.h .
copy ..\build.vc10\win_timing.h .

cd tune
cl %OPT% /c ..\..\tests\misc.c /I..\..
cl %OPT% /c ..\..\tests\memory.c /I..\..
cl %OPT% /c ..\..\tests\refmpn.c /I..\..
cl %OPT% /c ..\..\build.vc10\win_timing.c
cl %OPT% /c ..\..\build.vc10\getopt.c /I..\..
for %%X in (  ..\..\tune\set_strp.c      ..\..\tune\common.c        ..\..\tune\mod_1_div.c        ..\..\tune\set_strb.c ..\..\tune\divrem1div.c    ..\..\tune\gcd_bin.c   ..\..\tune\mod_1_inv.c       ..\..\tune\divrem1inv.c    ..\..\tune\gcdextod.c  ..\..\tune\modlinv.c          ..\..\tune\set_strs.c ..\..\tune\divrem2div.c    ..\..\tune\gcdextos.c  ..\..\tune\noop.c             ..\..\tune\divrem2inv.c    ..\..\tune\jacbase1.c  ..\..\tune\powm_mod.c       ..\..\tune\fac_ui_large.c  ..\..\tune\jacbase2.c  ..\..\tune\powm_redc.c    ..\..\tune\fac_ui_small.c  ..\..\tune\jacbase3.c  ..\..\tune\preinv_divrem_1.c  ..\..\build.vc10\tune\*.c) do (
	cl -c %OPT% /I..\.. /I..\..\tests /I.. %%X
)
cl %OPT% /I..\.. /I.. /I..\..\tests ..\..\tune\tuneup.c *.obj ..\%MPIRLIB% advapi32.lib psapi.lib
cd ..

exit /b 0


