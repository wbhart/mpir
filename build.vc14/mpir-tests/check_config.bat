rem output_params.bat contains:
rem    (set libr=<lib|dll>) 
rem    (set plat=<win32|x64>) 
rem    (set conf=<debug|release>) 

set test_platform=%1
set test_config=%2

@echo off
call :clrerr

if exist ..\..\output_params.bat (call ..\..\output_params.bat) else (call :seterr & echo ERROR: 'output_params.bat' not found & exit /b %errorlevel%)
set out_dir=%libr%\%plat%\%conf%

if /i "%plat%" EQU "" (call :seterr & echo ERROR: 'output_params.bat' is corrupt & exit /b %errorlevel%)
fc ..\%libr%-%conf%-config.props ..\test-config.props > nul && ((set library=%libr%) & goto cont)
call :seterr & echo ERROR: cannot determine library type (static or DLL) to test & exit /b %errorlevel%

:cont
if "%libr%" EQU "lib" if not exist ..\..\..\%out_dir%\mpirxx.lib (call :seterr & echo ERROR: static library tests need 'mpirxx.lib' & exit /b %errorlevel%)
if /i "%test_platform%" EQU "%plat%" if /i "%test_config%" EQU "%conf%" if /i "%library%" EQU "%libr%" echo OK & exit /b 0

call :seterr
echo ERROR Last MPIR build was %lib_type%\%plat%\%conf%, not %library%\%test_platform%\%test_config%
exit /b %errorlevel%

:clrerr
exit /b 0

:seterr
exit /b 1
