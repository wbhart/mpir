@echo off
::  Copyright 2009,2011 Jason Moxham
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


:: set default lib type as static
set LIBTYPE=lib
:: set default abi to ?
set ABI=?
:: set default cpu to ?
set CPU=?

:: parse params
:lp
shift
if "%0" == "" goto :exitlp
if "%0" == "--enable-shared" ( set LIBTYPE=dll)
if "%0" == "--enable-static" ( set LIBTYPE=lib)
if "%0" == "--disable-shared" ( set LIBTYPE=lib)
if "%0" == "--disable-static" ( set LIBTYPE=dll)
if "%0" == "ABI" (
	shift
	set ABI=%1
)
if "%0" == "--cpu" (
	shift
	set CPU=%1
)
goto :lp
:exitlp

:: ARCH is native ABI
set ARCH=64
if %PROCESSOR_ARCHITECTURE% == x86 ( set ARCH=32)
set VCTARGET=
if %ARCH% == 64 (
	if %ABI% == 64 (set VCTARGET=amd64)
	if %ABI% == 32 (set VCTARGET=x86)
)
if %ARCH% == 32 (
	if %ABI% == 64 (set VCTARGET=amd64_x86)
	if %ABI% == 32 (set VCTARGET=x86)
)

:: now we have to find a compiler that matches the required abi or default=?
copy ..\build.vc10\config.guess.c . > nul 2>&1
cl /nologo config.guess.c > nul 2>&1
if errorlevel 1 goto :step1
config.guess.exe print > config.guess.bat
call config.guess.bat
if %ABI% == ? goto :gotcc
if %GBITS% == %ABI% goto :gotcc
:step1
if exist "%VS100COMNTOOLS%\..\..\VC\vcvarsall.bat" (
	call "%VS100COMNTOOLS%\..\..\VC\vcvarsall.bat" %VCTARGET%
	goto :checkcc
)
if exist "%VS90COMNTOOLS%\..\..\VC\vcvarsall.bat" (
	call "%VS90COMNTOOLS%\..\..\VC\vcvarsall.bat" %VCTARGET%
	goto :checkcc
)

:nocc
echo cant find a suitible compiler
exit /b 1

:checkcc
cl /nologo config.guess.c > nul 2>&1
if errorlevel 1 goto :nocc
config.guess.exe print > config.guess.bat
call config.guess.bat
if %ABI% == ? goto :gotcc
if %GBITS% == %ABI% goto :gotcc
goto :nocc
:gotcc
if %ABI% == ? ( set ABI=%GBITS%)
if %CPU% == ? ( set CPU=%GCPU%)

:: now find yasm , not needed for C build?
if %CPU% == none goto :gotyasm
set YASMEXE=yasm.exe
yasm.exe --version > nul 2>&1
if not errorlevel 1 goto :gotyasm
set YASMEXE=vsyasm.exe
vsyasm.exe --version > nul 2>&1
if not errorlevel 1 goto :gotyasm
if exist "%YASMPATH%\yasm.exe" (
	set YASMEXE="%YASMPATH%\yasm.exe"
	goto :gotyasm
)
if exist "%YASMPATH%\vsyasm.exe" (
	set YASMEXE="%YASMPATH%\vsyasm.exe"
	goto :gotyasm
)
if exist "%VS100COMNTOOLS%\..\..\VC\bin\vsyasm.exe" (
	set YASMEXE="%VS100COMNTOOLS%\..\..\VC\bin\vsyasm.exe"
	goto :gotyasm
)
if exist "c:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\BuildCustomizations\vsyasm.exe" (
	set YASMEXE="c:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\BuildCustomizations\vsyasm.exe"
	goto :gotyasm
)
if exist "%VS90COMNTOOLS%\..\..\VC\bin\yasm.exe" (
	set YASMEXE="%VS90COMNTOOLS%\..\..\VC\bin\yasm.exe"
	goto :gotyasm
)
echo cant find yasm
exit /b 1
:gotyasm

:: set config.params.bat to the settings needed by make etc
echo (set LIBTYPE=%LIBTYPE%) > config.params.bat
echo (set FLAGS=/Ox /Ot /D "NDEBUG" /D "HAVE_CONFIG_H" /nologo /D "_MBCS" /GS-) >> config.params.bat
if %LIBTYPE% == lib (set FLAGS1=/Oi /D "_LIB" /D "PIC" /MT)
if %LIBTYPE% == dll (set FLAGS1=/D "__GMP_LIBGMP_DLL" /D "_WINDLL" /GF /EHsc /MD)
echo (set FLAGS1=%FLAGS1%) >> config.params.bat
set MPNPATH=.
if %ABI% == 32 (
	if %CPU% == x86		(set MPNPATH=x86w)
	if %CPU% == i386	(set MPNPATH=x86w)
	if %CPU% == i486	(set MPNPATH=x86w)
	if %CPU% == i586	(set MPNPATH=x86w)
	if %CPU% == pentium	(set MPNPATH=x86w)
	if %CPU% == pentiummmx	(set MPNPATH=x86w)
	if %CPU% == pentiumpro	(set MPNPATH=x86w x86w\p6)
	if %CPU% == i686	(set MPNPATH=x86w x86w\p6)
	if %CPU% == pentium2	(set MPNPATH=x86w x86w\p6 x86w\p6\mmx)
	if %CPU% == pentium3	(set MPNPATH=x86w x86w\p6 x86w\p6\mmx x86w\p6\p3mmx)
	if %CPU% == k6		(set MPNPATH=x86w)
	if %CPU% == k62		(set MPNPATH=x86w)
	if %CPU% == k63		(set MPNPATH=x86w)
	if %CPU% == k7		(set MPNPATH=x86w)
	if %CPU% == athlon	(set MPNPATH=x86w)
	if %CPU% == pentium4	(set MPNPATH=x86w x86w\pentium4 x86w\pentium4\mmx x86w\pentium4\sse2)
	if %CPU% == prescott	(set MPNPATH=x86w x86w\pentium4 x86w\pentium4\mmx x86w\pentium4\sse2)
	if %CPU% == core	(set MPNPATH=x86w x86w\pentium4 x86w\pentium4\mmx x86w\pentium4\sse2)
	if %CPU% == viac3	(set MPNPATH=x86w)
	if %CPU% == viac32	(set MPNPATH=x86w)
	if %CPU% == x86_64	(set MPNPATH=x86w)
	if %CPU% == netburst	(set MPNPATH=x86w x86w\pentium4 x86w\pentium4\mmx x86w\pentium4\sse2)
	if %CPU% == netburstlahf	(set MPNPATH=x86w x86w\pentium4 x86w\pentium4\mmx x86w\pentium4\sse2)
	if %CPU% == k8           (set MPNPATH=x86w x86w\p6 x86w\p6\mmx x86w\p6\p3mmx)
	if %CPU% == k10          (set MPNPATH=x86w x86w\p6 x86w\p6\mmx x86w\p6\p3mmx)
	if %CPU% == k102         (set MPNPATH=x86w x86w\p6 x86w\p6\mmx x86w\p6\p3mmx)
	if %CPU% == k103         (set MPNPATH=x86w x86w\p6 x86w\p6\mmx x86w\p6\p3mmx)
	if %CPU% == bulldozer    (set MPNPATH=x86w x86w\p6 x86w\p6\mmx x86w\p6\p3mmx)
	if %CPU% == bobcat       (set MPNPATH=x86w x86w\p6 x86w\p6\mmx x86w\p6\p3mmx)
	if %CPU% == core2        (set MPNPATH=x86w x86w\p6 x86w\p6\mmx x86w\p6\p3mmx)
	if %CPU% == penryn       (set MPNPATH=x86w x86w\p6 x86w\p6\mmx x86w\p6\p3mmx)
	if %CPU% == nehalem      (set MPNPATH=x86w x86w\p6 x86w\p6\mmx x86w\p6\p3mmx)
	if %CPU% == westmere     (set MPNPATH=x86w x86w\p6 x86w\p6\mmx x86w\p6\p3mmx)
	if %CPU% == sandybridge  (set MPNPATH=x86w x86w\p6 x86w\p6\mmx x86w\p6\p3mmx)
	if %CPU% == atom         (set MPNPATH=x86w x86w\p6 x86w\p6\mmx x86w\p6\p3mmx)
	if %CPU% == nano         (set MPNPATH=x86w x86w\p6 x86w\p6\mmx x86w\p6\p3mmx)
)
if %ABI% == 64 (
	if %CPU% == x86_64       (set MPNPATH=x86_64w)
	if %CPU% == netburst     (set MPNPATH=x86_64w x86_64w\netburst)
	if %CPU% == netburstlahf (set MPNPATH=x86_64w x86_64w\netburst)
	if %CPU% == k8           (set MPNPATH=x86_64w x86_64w\k8 x86_64w\k8\k8only)
	if %CPU% == k10          (set MPNPATH=x86_64w x86_64w\k8 x86_64w\k8\k10)
	if %CPU% == k102         (set MPNPATH=x86_64w x86_64w\k8 x86_64w\k8\k10 x86_64w\k8\k10\k102)
	if %CPU% == k103         (set MPNPATH=x86_64w x86_64w\k8 x86_64w\k8\k10 x86_64w\k8\k10\k102)
	if %CPU% == bulldozer    (set MPNPATH=x86_64w x86_64w\k8 x86_64w\k8\k10 x86_64w\k8\k10\k102)
	if %CPU% == bobcat       (set MPNPATH=x86_64w x86_64w\bobcat)
	if %CPU% == core2        (set MPNPATH=x86_64w x86_64w\core2)
	if %CPU% == penryn       (set MPNPATH=x86_64w x86_64w\core2 x86_64w\core2\penryn)
	if %CPU% == nehalem      (set MPNPATH=x86_64w x86_64w\nehalem)
	if %CPU% == westmere     (set MPNPATH=x86_64w x86_64w\nehalem x86_64w\nehalem\westmere)
	if %CPU% == sandybridge  (set MPNPATH=x86_64w x86_64w\sandybridge)
	if %CPU% == atom         (set MPNPATH=x86_64w x86_64w\atom)
	if %CPU% == nano         (set MPNPATH=x86_64w x86_64w\k8 x86_64w\k8\k8only)
)
echo (set MPNPATH=%MPNPATH%) >> config.params.bat
echo (set ABI=%ABI%) >> config.params.bat
echo setting params to
type config.params.bat
exit /b 0
