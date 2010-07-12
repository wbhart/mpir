rem lib x64 Release "..\lib\x64\Release"
@echo off
call :clrerr
set plf=
for /f "tokens=1,2,3,4" %%a in (..\lastbuild.txt) do ((set tpe=%%a) & (set plf=%%b) & (set cnf=%%c) & (set odir=%%d))
if /i "%plf%" EQU "" (call :seterr & echo ERROR: 'mpir-tests\lastbuild.txt' not found & exit /b %errorlevel%)

set in_tpe=
fc ..\lib-test-config.props ..\test-config.props > nul && ((set in_tpe=lib) & goto cont)
fc ..\dll-test-config.props ..\test-config.props > nul && ((set in_tpe=dll) & goto cont)
call :seterr & echo ERROR: cannot determine library type (static or DLL) to test & exit /b %errorlevel%

:cont
call :no_spc %1
set in_plf=%_res%
call :no_spc %2
set in_cnf=%_res%
echo %odir%\mpirxx.lib
if "%tpe%" NEQ "%in_tpe%" (call :seterr & echo ERROR: last MPIR build was a %tpe%, not a %in_tpe% & exit /b %errorlevel%)
if "%tpe%" EQU "lib" if not exist ..\%odir%\mpirxx.lib (call :seterr & echo ERROR: static library tests need 'mpirxx.lib' & exit /b %errorlevel%)
if "%in_plf%" EQU "%plf%" if "%in_cnf%" EQU "%cnf%" echo OK & exit /b 0

call :seterr
echo ERROR Last MPIR build was %plf%\%cnf%, not %in_plf%\%in_cnf%
exit /b %errorlevel%

:clrerr
exit /b 0

:seterr
exit /b 1

:no_spc
set _t=%1#
set _tt=%_t: =%
set _res=%_tt:~0,-1%
