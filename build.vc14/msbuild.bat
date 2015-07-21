rem %1 = architecture
rem %2 = library type (LIB|DLL)
rem %3 = platform (Win32|x64)
rem %4 = configuration (Release|Debug)

rem example use: msbuild sandybridge_ivybridge dll x64 release

@echo off

if /i "%2" EQU "DLL" (set libp=dll) else (set libp=lib)
if /i "%3" EQU "x64" (set plat=x64) else (set plat=Win32)
if /i "%4" EQU "Debug" (set conf=Debug) else (set conf=Release)

set src=%libp%_mpir_%1

rem This is the directory where Microsoft MSBUILD is installed 
set msbdir="C:\Program Files (x86)\MSBuild\14.0\Bin"
rem This is the Visual Studio build directory (within the MPIR directory) 
set srcdir="."

%msbdir%\msbuild.exe /p:Platform=%plat% /p:Configuration=%conf% %srcdir%\%src%\%src%.vcxproj

if /i "%libp%" == "LIB" (
  %msbdir%\msbuild.exe /p:Platform=%plat% /p:Configuration=%conf% %srcdir%\lib_mpir_cxx\lib_mpir_cxx.vcxproj
)
