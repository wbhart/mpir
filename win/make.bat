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
md mpn mpz mpq mpf printf scanf > nul 2>&1

copy ..\build.vc10\gen_mpir_h.bat .
copy ..\build.vc10\out_copy_rename.bat .
copy ..\build.vc10\gen_config_h.bat .
copy ..\build.vc10\cfg.h .

del ..\mpir.h
call gen_mpir_h x64
del ..\config.h
call gen_config_h %MPIRDIR%
copy %MPIRDIR%gmp-mparam.h ..

del gen_mpir_h.bat out_copy_rename.bat gen_config_h.bat cfg.h

echo int main(void){return 0;} > comptest.c
cl /nologo comptest.c > nul 2>&1
if errorlevel 1 (
	call "%VS90COMNTOOLS%\..\..\VC\vcvarsall.bat" amd64
)
del comptest.*
:: dont set yasm path until after MSVC
set PATH=%PATH%;%YASM%

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

::static
set OPT=/Ox /Oi /Ot /D "NDEBUG" /D "_LIB" /D "HAVE_CONFIG_H" /D "PIC" /D "_MBCS" /MT /GS- /FD /nologo /c /Zi /favor:INTEL64
::dll
::set OPT=/Ox         /D "NDEBUG"           /D "HAVE_CONFIG_H" /D "__GMP_LIBGMP_DLL" /D "__GMP_WITHIN_GMP" /D "__GMP_WITHIN_GMPXX" /D "_WINDLL" /D "_MBCS" /GF /FD /EHsc /MD /GS- /W3 /nologo /c /Zi /Gd


::cd mpn
::for %%X in ( ..\mpn\generic\*.c) do (
::        for /f "tokens=4 delims=\ " %%a in ("%%X") do (
::                for /f "tokens=1 delims=. " %%b in ("%%a") do (
::                        if not exist %%b.obj (
::				cl %OPT% -I..\.. %%X
::			)
::                )
::        )
::)
::cd ..


:: or just compile all generic and just overwrite with asm
cd mpn
for %%X in ( ..\..\mpn\generic\*.c) do (
	cl %OPT% -I..\.. %%X
)
for %%X in ( ..\%MPIRDIR%\..\modexact_1c_odd.asm ) do (
	yasm -I ..\..\mpn\x86_64w -f x64 %%X
	echo assemblin %%X
)
for %%X in ( ..\%MPIRDIR%\*.asm ) do (
	yasm -I ..\..\mpn\x86_64w -f x64 %%X
	echo assemblin %%X
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

::del mpz\get_ux.obj mpz\get_sx.obj mpz\set_ux.obj mpz\set_sx.obj

lib /nologo scanf\*.obj printf\*.obj mpz\*.obj mpq\*.obj mpf\*.obj mpn\*.obj *.obj /out:mpir.lib


:fin
