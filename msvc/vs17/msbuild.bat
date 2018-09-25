@echo off
rem %1 = architecture
rem %2 = library type (LIB|DLL)
rem %3 = platform (Win32|x64)
rem %4 = configuration (Release|Debug)
rem %5 = Windows SDK Version
rem %6 = build tests (|+tests)

set vsw_exe="%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"

rem This is the directory where Microsoft MSBUILD is installed 
for /f "usebackq tokens=*" %%i in (`%vsw_exe% -prerelease -latest -products * -requires Microsoft.Component.MSBuild -property installationPath`) do (
  set InstallDir=%%i
)

if exist "%InstallDir%\MSBuild\15.0\Bin\MSBuild.exe" (
  set msbdir="%InstallDir%\MSBuild\15.0\Bin"
)

if "%4" NEQ "" if "%3" NEQ "" if "%2" NEQ "" if "%1" NEQ "" goto cont
call :get_architectures -
echo usage: msbuild architecture=^<%architectures:|=^|%^> library_type=^<LIB^|DLL^> platform=^<Win32^|x64^> configuration=^<Release^|Debug^> [Windows_SDK_Version=^<n^>] [+tests]
goto :eof

:cont
rem example use: msbuild sandybridge_ivybridge dll x64 release

if not exist "lib_mpir_%1" (call :get_architectures & call :seterr & echo ERROR: architecture is one of ^(%architectures%^) ^(not %1^) & exit /b %errorlevel%)
if /i "%2" EQU "DLL" (set libp=dll) else (if /i "%2" EQU "LIB" (set libp=lib) else ((call :seterr & echo ERROR: library type is "lib" or "dll" ^(not "%2"^) & exit /b %errorlevel%)))
if /i "%3" EQU "x64" (set plat=x64) else (if /i "%3" EQU "Win32" (set plat=win32) else (call :seterr & echo ERROR: platform is "Win32" or "x64" ^(not "%3"^) & exit /b %errorlevel%))
if /i "%4" EQU "Debug" (set conf=Debug) else (if /i "%4" EQU "Release" (set conf=Release) else (call :seterr & echo ERROR: configuration is "Release" or "Debug" ^(not "%4"^) & exit /b %errorlevel%))
if /i "%5" NEQ "" if "%5" EQU "+tests" (set run_tests=y) else (set win_sdk=%5)
if /i "%6" NEQ "" if "%6" EQU "+tests" (set run_tests=y) 

set src=%libp%_mpir_%1

rem This is the Visual Studio build directory (within the MPIR directory) 
set srcdir=.

echo %msbdir%\msbuild.exe /p:Platform=%plat% /p:Configuration=%conf% /p:"Windows%20SDK%20Version=%win_sdk%" %srcdir%\%src%\%src%.vcxproj
%msbdir%\msbuild.exe /p:Platform=%plat% /p:Configuration=%conf% /p:"Windows%20SDK%20Version=%win_sdk%" %srcdir%\%src%\%src%.vcxproj

if /i "%libp%" == "LIB" (
  %msbdir%\msbuild.exe /p:Platform=%plat% /p:Configuration=%conf% /p:"Windows%20SDK%20Version=%win_sdk%" %srcdir%\lib_mpir_cxx\lib_mpir_cxx.vcxproj
)

if /i "%run_tests%" NEQ "" (
  for /d %%d in (.\mpir-tests\*) do (
    for %%f in (%%d\*.vcxproj) do (
      %msbdir%\msbuild.exe /property:SolutionDir=..\..\ /property:OutDir=..\..\%plat%\%conf%\ /p:Platform=%plat% /p:Configuration=%conf% /p:"Windows%20SDK%20Version=%win_sdk%" %%f
    )
  )
)

:get_architectures
set architectures=
for /d %%a in (lib_mpir_*) do (call :add_architecture %%a %1)
goto :eof

:add_architecture
set xx=%1
set arch=%xx:~9%
if /i "%arch%" EQU "cxx" goto :eof
if _%2_ == __ (
   set "delim= "
   set arch=%arch%
) else (
   set delim=^|
)

if "%architectures%" == "" (
   set "architectures=%arch%"
) else (
   set "architectures=%architectures%%delim%%arch%"
)
goto :eof

:set_arch
set arch=%1
goto :eof

:clrerr
exit /b 0

:seterr
exit /b 1
