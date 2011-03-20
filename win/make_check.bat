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
md tests
cd tests
md mpn mpz mpq mpf rand misc cxx > nul 2>&1
cd ..

echo int main(void){return 0;} > comptest.c
cl /nologo comptest.c > nul 2>&1
if errorlevel 1 (
	call "%VS90COMNTOOLS%\..\..\VC\vcvarsall.bat" amd64
)
del comptest.*
:: dont set yasm path until after MSVC
set PATH=%PATH%;%YASM%

::static
set OPT=/Ox /Oi /Ot /D "NDEBUG" /D "_LIB" /D "HAVE_CONFIG_H" /D "PIC" /D "_MBCS" /MT /GS- /FD /nologo /Zi /favor:INTEL64
::dll
::set OPT=/Ox         /D "NDEBUG"           /D "HAVE_CONFIG_H" /D "__GMP_LIBGMP_DLL" /D "__GMP_WITHIN_GMP" /D "__GMP_WITHIN_GMPXX" /D "_WINDLL" /D "_MBCS" /GF /FD /EHsc /MD /GS- /W3 /nologo /Zi /Gd


cd tests

cl %OPT% /c ..\..\tests\memory.c /I..\..
cl %OPT% /c ..\..\tests\misc.c   /I..\..
cl %OPT% /c ..\..\tests\trace.c  /I..\..
cl %OPT% /c ..\..\tests\refmpn.c /I..\..
cl %OPT% /c ..\..\tests\refmpz.c /I..\..
cl %OPT% /c ..\..\tests\refmpq.c /I..\..
cl %OPT% /c ..\..\tests\refmpf.c /I..\..

for %%X in ( ..\..\tests\t-*.c) do (
	cl %OPT% /I..\.. /I..\..\tests %%X misc.obj memory.obj trace.obj refmpn.obj ..\mpir.lib
	if errorlevel 1 ( echo %%X FAILS )	
)
for %%X in ( *.exe) do (
	echo testing %%X
	%%X
	if errorlevel 1 ( echo %%X FAILS )
)

cd mpn
for %%X in ( ..\..\..\tests\mpn\t-*.c) do (
	cl %OPT% /I..\..\.. /I..\..\..\tests %%X ..\misc.obj ..\memory.obj ..\trace.obj ..\refmpn.obj ..\..\mpir.lib
)
for %%X in ( *.exe) do (
	echo testing mpn_%%X
	%%X
	if errorlevel 1 ( echo %%X FAILS )
)
cd ..

cd mpz
for %%X in ( ..\..\..\tests\mpz\*.c) do (
	cl %OPT% /I..\..\.. /I..\..\..\tests %%X ..\misc.obj ..\memory.obj ..\trace.obj ..\refmpn.obj ..\refmpz.obj ..\..\mpir.lib
)
for %%X in ( *.exe) do (
	echo testing mpz_%%X
	%%X
	if errorlevel 1 ( echo %%X FAILS )
)
cd ..

cd mpq
for %%X in ( ..\..\..\tests\mpq\t-*.c) do (
	cl %OPT% /I..\..\.. /I..\..\..\tests %%X ..\misc.obj ..\memory.obj ..\trace.obj ..\refmpn.obj ..\refmpz.obj ..\refmpq.obj ..\..\mpir.lib
)
for %%X in ( *.exe) do (
	echo testing mpq_%%X
	%%X
	if errorlevel 1 ( echo %%X FAILS )
)
cd ..

cd mpf
for %%X in ( ..\..\..\tests\mpf\*.c) do (
	cl %OPT% /I..\..\.. /I..\..\..\tests %%X ..\misc.obj ..\memory.obj ..\trace.obj ..\refmpn.obj ..\refmpf.obj ..\..\mpir.lib
)
for %%X in ( *.exe) do (
	echo testing mpf_%%X
	%%X
	if errorlevel 1 ( echo %%X FAILS )
)
cd ..

cd rand
for %%X in ( ..\..\..\tests\rand\t-*.c) do (
	cl %OPT% /I..\..\.. /I..\..\..\tests %%X ..\misc.obj ..\memory.obj ..\trace.obj ..\refmpn.obj ..\..\mpir.lib
)
for %%X in ( *.exe) do (
	echo testing rand_%%X
	%%X
	if errorlevel 1 ( echo %%X FAILS )
)
cd ..

cd misc
for %%X in ( ..\..\..\tests\misc\t-*.c) do (
	cl %OPT% /I..\..\.. /I..\..\..\tests %%X ..\misc.obj ..\memory.obj ..\trace.obj ..\refmpn.obj ..\..\mpir.lib
)
for %%X in ( *.exe) do (
	echo testing misc_%%X
	%%X
	if errorlevel 1 ( echo %%X FAILS )
)
cd ..

cd cxx
for %%X in ( ..\..\..\tests\cxx\t-*.cc) do (
	cl /EHsc %OPT% /I..\..\.. /I..\..\..\tests %%X ..\misc.obj ..\memory.obj ..\trace.obj ..\refmpn.obj ..\..\mpir.lib
)
for %%X in ( *.exe) do (
	echo testing cxx_%%X
	%%X
	if errorlevel 1 ( echo %%X FAILS )
)
cd ..


:fin
cd ..
