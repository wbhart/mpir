@echo off

set plf=
if exist %1 (call :parse %1) else (call :parse "%1")
if /i "%plf%" NEQ "" (goto next)

call :seterr & echo ERROR: %1 is not supported & exit /b %errorlevel%

:next
set extn=%fnx%#
set extn=%extn:~-4,-1%
set tdir="%plf%\%cnf%"

rem The Output Directory
set odir="%extn%\%plf%\%cnf%"

echo %extn% %plf% %cnf% %odir% >mpir-tests\lastbuild.txt
echo copying outputs from %tdir% to %odir%
if not exist %odir% md %odir%
call :copyh %odir%

if "%extn%" EQU "dll" (
	copy %tdir%\mpir.dll %odir%\mpir.dll
	copy %tdir%\mpir.exp %odir%\mpir.exp
	copy %tdir%\mpir.lib %odir%\mpir.lib
	if exist %tdir%\mpir.pdb (copy %tdir%\mpir.pdb %odir%\mpir.pdb)
	copy mpir-tests\dll-test-config.props mpir-tests\test-config.props
) else if "%extn%" EQU "lib" (
	copy %tdir%\mpir.lib %odir%\mpir.lib
	if exist %tdir%\mpir.pdb (copy %tdir%\mpir.pdb %odir%\mpir.pdb)
	copy mpir-tests\lib-test-config.props mpir-tests\test-config.props
) else (
	call :seterr & echo ERROR: illegal library type %extn%  & exit /b %errorlevel%
)
exit /b 0

:copyh
copy ..\config.h %1\config.h
copy ..\gmp-mparam.h %1\gmp-mparam.h
copy ..\mpir.h %1\mpir.h
copy ..\mpir.h %1\gmp.h
copy ..\mpirxx.h %1\mpirxx.h
copy ..\mpirxx.h %1\gmpxx.h
exit /b 0

:parse
for /f "tokens=1-12 delims=\" %%a in (%1) do (
  if "%%a" EQU "build.vc10" ((set plf=%%b) & (set cnf=%%c) & (set fnx=%%d))
  if "%%b" EQU "build.vc10" ((set plf=%%c) & (set cnf=%%d) & (set fnx=%%e))
  if "%%c" EQU "build.vc10" ((set plf=%%d) & (set cnf=%%e) & (set fnx=%%f))
  if "%%d" EQU "build.vc10" ((set plf=%%e) & (set cnf=%%f) & (set fnx=%%g))
  if "%%e" EQU "build.vc10" ((set plf=%%f) & (set cnf=%%g) & (set fnx=%%h))
  if "%%f" EQU "build.vc10" ((set plf=%%g) & (set cnf=%%h) & (set fnx=%%i))
  if "%%g" EQU "build.vc10" ((set plf=%%h) & (set cnf=%%i) & (set fnx=%%j))
  if "%%h" EQU "build.vc10" ((set plf=%%i) & (set cnf=%%j) & (set fnx=%%k))
  if "%%i" EQU "build.vc10" ((set plf=%%j) & (set cnf=%%k) & (set fnx=%%l))
  if "%%j" EQU "build.vc10" ((set plf=%%k) & (set cnf=%%l) & (set fnx=%%m))
  if "%%k" EQU "build.vc10" ((set plf=%%l) & (set cnf=%%m) & (set fnx=%%n))
  if "%%l" EQU "build.vc10" ((set plf=%%m) & (set cnf=%%n) & (set fnx=%%o))
)
exit /b 0

:seterr
exit /b 1
