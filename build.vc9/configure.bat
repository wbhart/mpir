@echo off
set LIBTYPE=lib
set CPU=?
set BITS=?
set GBITS=?
set GCPU=?
:lp
shift
if "%0" == "" goto :exitlp
if "%0" == "--enable-shared" ( set LIBTYPE=dll)
if "%0" == "--enable-static" ( set LIBTYPE=lib)
if "%0" == "--disable-shared" ( set LIBTYPE=lib)
if "%0" == "--disable-static" ( set LIBTYPE=dll)
if "%0" == "--ABI32" ( set BITS=32)
if "%0" == "--ABI64" ( set BITS=64)
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
if %BITS% == ? ( 
	set BITS=64
	if %PROCESSOR_ARCHITECTURE% == x86 (set BITS=32)
)
set ARCH=x86
set ARCHW=Win32
if %BITS% == 64 (
	set ARCH=amd64
	set ARCHW=x64
)
cl config.guess.c > nul 2>&1
if errorlevel 1 goto :nocc1
config.guess.exe print > config.guess.bat
call config.guess.bat
if %GBITS% == %BITS% goto :gotcc
:nocc1
if exist "%VS90COMNTOOLS%\..\..\VC\vcvarsall.bat" (
	call "%VS90COMNTOOLS%\..\..\VC\vcvarsall.bat" %ARCH%
)
cl config.guess.c > nul 2>&1
if errorlevel 1 goto :nocc
config.guess.exe print > config.guess.bat
call config.guess.bat
if %GBITS% == %BITS% goto :gotcc
:nocc
echo Cant find a compiler for %ARCH%
goto :EOF
:gotcc

set YASM="%VCINSTALLDIR%\bin\yasm.exe"
if not exist %YASM% (
	echo Cant find %YASM%
	echo Download from http://www.tortall.net/projects/yasm/ and install to above
	goto :EOF
)
set YASMRULES="%VCINSTALLDIR%\VCProjectDefaults\yasm.rules"
if not exist %YASMRULES% (
	echo Cant find %YASMRULES%
	echo copy from this directory to above
	goto :EOF
)

if %CPU% == ? (	set CPU=%GCPU%)
set BCPU=none
if %BITS% == 64 (
	if %CPU% == x86_64 ( set BCPU=gc)
	if %CPU% == core2 ( set BCPU=core2)
	if %CPU% == penryn ( set BCPU=core2)
	if %CPU% == nehalem ( set BCPU=core2)
	if %CPU% == atom ( set BCPU=gc)
	if %CPU% == netburst ( set BCPU=gc)
	if %CPU% == netburstlahf ( set BCPU=gc)
	if %CPU% == k8 ( set BCPU=amd64)
	if %CPU% == k10 ( set BCPU=amd64)
)
if %BITS% == 32 (
	if %CPU% == i486 ( set BCPU=gc)
	if %CPU% == pentium ( set BCPU=p0)
	if %CPU% == pentiummmx ( set BCPU=p0)
	if %CPU% == pentiumpro ( set BCPU=p0)
	if %CPU% == pentium2 ( set BCPU=p0)
	if %CPU% == pentium3 ( set BCPU=p3)
	if %CPU% == core ( set BCPU=p3)
	if %CPU% == core2 ( set BCPU=p3)
	if %CPU% == penryn ( set BCPU=p3)
	if %CPU% == nehalem ( set BCPU=p3)
	if %CPU% == atom ( set BCPU=p3)
	if %CPU% == netburst ( set BCPU=p4)
	if %CPU% == netburstlahf ( set BCPU=p4)
	if %CPU% == pentium4 ( set BCPU=p4)
	if %CPU% == prescott ( set BCPU=p4)
	if %CPU% == k5 ( set BCPU=p0)
	if %CPU% == k6 ( set BCPU=p0)
	if %CPU% == k62 ( set BCPU=p0)
	if %CPU% == k63 ( set BCPU=p0)
	if %CPU% == k7 ( set BCPU=p3)
	if %CPU% == k8 ( set BCPU=p3)
	if %CPU% == k10 ( set BCPU=p3)
	if %CPU% == viac3 ( set BCPU=p0)
	if %CPU% == viac32 ( set BCPU=p0)
)
if %CPU% == none ( set BCPU=gc)
echo Detected %GCPU%
echo (set LIBTYPE=%LIBTYPE%) > config_params.bat
echo (set ARCHW=%ARCHW%) >> config_params.bat
echo (set BCPU=%BCPU%) >> config_params.bat
echo setting params to
type config_params.bat
:EOF
