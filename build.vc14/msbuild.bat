@echo off
rem %1 = architecture
rem %2 = library type (LIB|DLL)
rem %3 = platform (Win32|x64)
rem %4 = configuration (Release|Debug)
rem %5 = build tests (|+tests)

if "%4" NEQ "" if "%3" NEQ "" if "%2" NEQ "" if "%1" NEQ "" goto cont
echo usage: msbuild arcitecture library_type=^<LIB^|DLL^> platform=^<Win32^|x64^> configuration=^<Release^|Debug^>
goto :eof

:cont
rem example use: msbuild sandybridge_ivybridge dll x64 release

if /i "%2" EQU "DLL" (set libp=dll) else (if /i "%2" EQU "LIB" (set libp=lib) else ((call :seterr & echo ERROR: library type is "lib" or "dll" ^(not "%2"^) & exit /b %errorlevel%)))
if /i "%3" EQU "x64" (set plat=x64) else (if /i "%3" EQU "Win32" (set plat=win32) else (call :seterr & echo ERROR: platform is "Win32" or "x64" ^(not "%3"^) & exit /b %errorlevel%))
if /i "%4" EQU "Debug" (set conf=Debug) else (if /i "%4" EQU "Release" (set conf=Release) else (call :seterr & echo ERROR: configuration is "Release" or "Debug" ^(not "%4"^) & exit /b %errorlevel%))

set src=%libp%_mpir_%1

rem This is the directory where Microsoft MSBUILD is installed 
set msbdir="C:\Program Files (x86)\MSBuild\14.0\Bin"
rem This is the Visual Studio build directory (within the MPIR directory) 
set srcdir="."

%msbdir%\msbuild.exe /p:Platform=%plat% /p:Configuration=%conf% %srcdir%\%src%\%src%.vcxproj

if /i "%libp%" == "LIB" (
  %msbdir%\msbuild.exe /p:Platform=%plat% /p:Configuration=%conf% %srcdir%\lib_mpir_cxx\lib_mpir_cxx.vcxproj
)

if /i "%5" EQU "+tests" (
  for /d %%d in (.\mpir-tests\*) do (
    for %%f in (%%d\*.vcxproj) do (
      %msbdir%\msbuild.exe /property:SolutionDir=..\..\ /property:OutDir=..\..\%plat%\%conf%\ /p:Platform=%plat% /p:Configuration=%conf% %%f
    )
  )
)

:clrerr
exit /b 0

:seterr
exit /b 1
