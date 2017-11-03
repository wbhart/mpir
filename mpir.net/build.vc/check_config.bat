@echo off
rem %1 = platform
rem %2 = configuration
rem %3 - msvc version

rem output_params.bat contains:
rem (set ldir=architecture)   
rem (set libr=lib) 
rem (set plat=x64) 
rem (set conf=Release) 

call :clrerr
if exist ..\..\build.vc\output_params.bat (call ..\..\build.vc\output_params.bat) else (call :seterr & echo ERROR: 'output_params.bat' not found & exit /b %errorlevel%)

if /i "%1" EQU "%plat%" if /i "%2" EQU "%conf%" (exit /b 0)

call :seterr
echo ERROR The last MPIR build was for \%plat%\%conf%, not %1\%2
echo Please set the correct platform and configuration to build MPIR.Net
exit /b %errorlevel%

:clrerr
exit /b 0

:seterr
exit /b 1
