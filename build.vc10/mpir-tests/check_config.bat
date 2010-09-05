rem lib x64 Release "lib\x64\Release"
@echo off
call :clrerr
set plf=
for /f "tokens=1,2,3,4" %%a in (..\lastbuild.txt) do ((set tpe=%%a) & (set plf=%%b) & (set cnf=%%c) & (set odir=%%d))
if /i "%plf%" EQU "" (call :seterr & echo ERROR: 'mpir-tests\lastbuild.txt' not found & exit /b %errorlevel%)
echo %odir%"
set in_tpe=
fc ..\%tpe%-%cnf%-config.props ..\test-config.props > nul && ((set in_tpe=%tpe%) & goto cont)
call :seterr & echo ERROR: cannot determine library type (static or DLL) to test & exit /b %errorlevel%

:cont
call :no_spc in_plf, %1
call :no_spc in_cnf, %2

if "%tpe%" EQU "lib" if not exist ..\..\%odir%\mpirxx.lib (call :seterr & echo ERROR: static library tests need 'mpirxx.lib' & exit /b %errorlevel%)
if /i "%in_plf%" EQU "%plf%" if /i "%in_cnf%" EQU "%cnf%" if /i "%in_tpe%" EQU "%tpe%" echo OK & exit /b 0

call :seterr
echo ERROR Last MPIR build was %tpe%\%plf%\%cnf%, not %in_tpe%\%in_plf%\%in_cnf%
exit /b %errorlevel%

call :seterr
echo ERROR Last MPIR build was %plf%\%cnf%, not %in_plf%\%in_cnf%
exit /b %errorlevel%

:clrerr
exit /b 0

:seterr
exit /b 1

:no_spc
set _t=#%2#
set _tt=%_t: =%
set %1=%_tt:~1,-1%
