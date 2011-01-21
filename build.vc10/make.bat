@echo off
::  Copyright 2009 Jason Moxham
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
if not exist config_params.bat (
	echo run configure first
	exit /b 1
)
call config_params.bat
if "%1" == ""        goto :make
if "%1" == "clean"   goto :clean
if "%1" == "install" goto :install
if "%1" == "check"   goto :check
if "%1" == "speed"   goto :speed
if "%1" == "tune"    goto :tune
if "%1" == "try"     goto :try
if "%1" == "help" (
	echo Usage : make [clean^|install^|check^|speed^|tune^|try^|help]
	exit /b 0
)
echo Unkwown option
exit /b 1

:make

set MS_BUILD="C:\Windows\Microsoft.NET\Framework64\v4.0.30319\msbuild.exe"

set LIBBUILD=%LIBTYPE%_mpir_%BCPU%

if errorlevel 1 (
	echo "ERROR PREBUILD"
	exit /b 1
)
%MS_BUILD% %LIBBUILD%\%LIBBUILD%.vcxproj /p:Platform=%ARCHW%;Configuration=Release

if errorlevel 1 (
	echo "ERROR BUILDING"
	exit /b 1
)
:: c++ to build  if static
if %LIBTYPE% == lib (
	%MS_BUILD% lib_mpir_cxx\lib_mpir_cxx.vcxproj /p:Platform=%ARCHW%;Configuration=Release
	if errorlevel 1 (
		echo "ERROR BUILDING CXX"
		exit /b 1
	)
)
exit /b 0

:check
:: this gives an error if we dont build the c++ stuff
%MS_BUILD% mpir-tests.sln  /p:Platform=%ARCHW%;Configuration=Release"
if errorlevel 1 (
	echo "ERROR BUILDING TESTS"
	exit /b 1
)
cd mpir-tests
@echo off
for /f "usebackq tokens=1*" %%f in (`reg query HKCR\Python.File\shell\open\command`) do (set _my_=%%f %%g)
if not errorlevel 1 goto :got
echo ERROR Could not find PYTHON
exit /b 1
:got
set _res_=%_my_:*REG_SZ=%
set _end_=%_res_:*exe"=%
call set _python_=%%_res_:%_end_%=%%
call %_python_% run-tests.py noenter
set RET=%ERRORLEVEL%
cd ..
exit /b %RET%

:install
echo HOW???
exit /b 1

:speed
%MS_BUILD% speed\speed.vcxproj /p:Platform=%ARCHW%;Configuration=Release
echo speed.exe is in %ARCHW%\Release\
exit /b 0

:tune
%MS_BUILD% tune\tune.vcxproj /p:Platform=%ARCHW%;Configuration=Release
echo tune.exe is in %ARCHW%\Release\
exit /b 0

:try
%MS_BUILD% try\try.vcxproj /p:Platform=%ARCHW%;Configuration=Release
echo try.exe is in try\%ARCHW%\Release\
exit /b 0

:clean
del config_params.bat config.guess.bat config.guess.exe config.guess.obj last_build.txt >nul 2>&1
del ..\config.h ..\mpir.h ..\gmp.h ..\gmp-mparam.h >nul 2>&1
del gen-psqr\gen-psqr.exe gen-fac_ui\gen-fac_ui.exe gen-bases\gen-bases.exe >nul 2>&1
del gen-fib\gen-fib.exe gen-mpir\gen-mpir.exe gen-mpir\gen-mpir.pdb >nul 2>&1
rmdir /s/q x64 win32 %LIBTYPE%_mpir_%BCPU%\%ARCHW% lib_mpir_cxx\%ARCHW% lib\%ARCHW% dll\%ARCHW% mpir-tests\%ARCHW% >nul 2>&1
rmdir /s/q gen-psqr\Win32 gen-fac_ui\Win32 gen-bases\Win32 gen-fib\Win32 gen-mpir\Win32 >nul 2>&1
rmdir /s/q lib_speed\%ARCHW% speed\%ARCHW% tune\%ARCHW% try\%ARCHW% >nul 2>&1
exit /b 0
