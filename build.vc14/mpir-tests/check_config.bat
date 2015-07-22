@echo off
rem output_params.bat contains:
rem (set ldir=architecture)   
rem (set libr=lib) 
rem (set plat=x64) 
rem (set conf=Release) 

call :clrerr
if exist ..\..\output_params.bat (call ..\..\output_params.bat) else (call :seterr & echo ERROR: 'output_params.bat' not found & exit /b %errorlevel%)
if /i "%libr%" EQU "lib" if not exist ..\..\lib_mpir_cxx\%plat%\%conf%\mpirxx.lib (call :seterr & echo ERROR: static library tests need 'mpirxx.lib' & exit /b %errorlevel%)

if /i "%1" EQU "%plat%" if /i "%2" EQU "%conf%" (exit /b 0)

call :seterr
echo ERROR The last MPIR build was for \%plat%\%conf%, not %1\%2
echo Please set the correct platform and configuration for the tests
exit /b %errorlevel%

:clrerr
exit /b 0

:seterr
exit /b 1
