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
if "%0" == "--ABI32" ( set ABI=32)
if "%0" == "--ABI64" ( set ABI=64)
if "%0" == "--cpu-none" ( set CPU=none)
if "%0" == "--cpu-x86_64" ( set CPU=x86_64)
if "%0" == "--cpu-i486" ( set CPU=i486)
if "%0" == "--cpu-pentium" ( set CPU=pentium)
if "%0" == "--cpu-pentiummmx" ( set CPU=pentiummmx)
if "%0" == "--cpu-pentiumpro" ( set CPU=pentiumpro)
if "%0" == "--cpu-pentium2" ( set CPU=pentium2)
if "%0" == "--cpu-pentium3" ( set CPU=pentium3)
if "%0" == "--cpu-core" ( set CPU=core)
if "%0" == "--cpu-core2" ( set CPU=core2)
if "%0" == "--cpu-penryn" ( set CPU=penryn)
if "%0" == "--cpu-nehalem" ( set CPU=nehalem)
if "%0" == "--cpu-atom" ( set CPU=atom)
if "%0" == "--cpu-netburst" ( set CPU=netburst)
if "%0" == "--cpu-netburstlahf" ( set CPU=netburstlahf)
if "%0" == "--cpu-pentium4" ( set CPU=pentium4)
if "%0" == "--cpu-prescott" ( set CPU=prescott)
if "%0" == "--cpu-k5" ( set CPU=k5)
if "%0" == "--cpu-k6" ( set CPU=k6)
if "%0" == "--cpu-k62" ( set CPU=k62)
if "%0" == "--cpu-k63" ( set CPU=k63)
if "%0" == "--cpu-k7" ( set CPU=k7)
if "%0" == "--cpu-k8" ( set CPU=k8)
if "%0" == "--cpu-k10" ( set CPU=k10)
if "%0" == "--cpu-viac3" ( set CPU=viac3)
if "%0" == "--cpu-viac32" ( set CPU=viac32)
goto :lp
:exitlp

:: ARCH is native ABI
set ARCH=64
if %PROCESSOR_ARCHITECTURE% == x86 ( set ARCH=32)
set VCTARGET=
if %ARCH% == 64 (
	if %ABI% == 64 (set VCTARGET=amd64)
	if %ABI% == 32 (set VCTARGET=x86_amd64)
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

:: now find yasm , not needed for C build or? 32bit build?
if %CPU% == none goto :gotyasm
set YASMEXE=yasm.exe
yasm.exe --version > nul 2>&1
if not errorlevel 1 goto :gotyasm
set YASMEXE=vsyasm.exe
vsyasm.exe --version > nul 2>&1
if not errorlevel 1 goto :gotyasm
if not "%YASMPATH%" == "" (
	set PATH=%PATH%;%YASMPATH%
	yasm.exe --version > nul 2>&1
	if not errorlevel 1 (set YASMEXE=yasm.exe)
	goto :gotyasm
)
if exist "%VS100COMNTOOLS%\..\..\VC\bin\vsyasm.exe" (
	set PATH=%PATH%;%VS100COMNTOOLS%\..\..\VC\bin\
	goto :gotyasm
)
if exist "c:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\BuildCustomizations\vsyasm.exe" (
	set PATH=%PATH%;c:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\BuildCustomizations\
	goto :gotyasm
)
set YASMEXE=yasm.exe
if exist "%VS90COMNTOOLS%\..\..\VC\bin\vsyasm.exe" (
	set PATH=%PATH%;%VS90COMNTOOLS%\..\..\VC\bin\
	goto :gotyasm
)
echo cant find yasm
exit /b 1
:gotyasm

:: set config.params.bat to the settings needed by make etc
echo (set LIBTYPE=%LIBTYPE%) > config.params.bat
if %ABI% == 32 (
	if %CPU% == k8      (set MPNPATH=x86w x86w\k7)
	if %CPU% == nehalem (set MPNPATH=x86w)
)
if %ABI% == 64 (
	if %CPU% == k8      (set MPNPATH=x86_64w x86_64w\k8 x86_64w\k8\k8only)
	if %CPU% == nehalem (set MPNPATH=x86_64w x86_64w\nehalem)
)
echo (set MPNPATH=%MPNPATH%) >> config.params.bat
echo (set ABI=%ABI%) >> config.params.bat
echo setting params to
type config.params.bat
exit /b 0
