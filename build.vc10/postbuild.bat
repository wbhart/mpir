@echo off
rem remove any quotes around the full file path
set str=%1
for /f "useback tokens=*" %%a in ('%str%') do set str=%%~a

rem delete anything from path before 'build.vc10'
:dele
set str=%str:~1%
set str2=%str:~0,10%
if "%str2%" NEQ "build.vc10" goto dele

rem we now have: build.vc10\<win32|x64>\<debug|release>\mpir.<lib|dll>

rem extract platform (plat=<win32|x64>), configuration (conf=<debug|release>) anbd file name     
set file=
for /f "tokens=1,2,3,4 delims=\" %%a in ("%str%") do set plat=%%b&set conf=%%c&set file=%%d
if /i "%file%" NEQ "" (goto next)
call :seterr & echo ERROR: %1 is not supported & exit /b %errorlevel%

:next
rem echo platform= %plat% configuration= %conf%, file= %file%

rem get the filename extension (lib/dll) to set the output directory
set extn=%file%#
set extn=%extn:~-4,3%

rem set the target aand output directories
set source="%plat%\%conf%"
set dest="%extn%\%plat%\%conf%"

rem output parametrers for the MPIR tests
echo (set libr=%extn%)  > output_params.bat
echo (set plat=%plat%) >> output_params.bat
echo (set conf=%conf%) >> output_params.bat

echo copying outputs from %source% to %dest%
if not exist %dest% md %dest%
call :copyh %dest%
call :copyb %source% %dest% %conf% %extn%
exit /b 0

rem copy binaries to final destination directory
rem %1 = source directory  %2 = destination directory
rem %3 = library (lib/dll) %4 = configuration (debug/releaase) 
:copyb
if "%4" EQU "dll" (
	copy %1\mpir.dll %2\mpir.dll
	copy %1\mpir.exp %2\mpir.exp
	copy %1\mpir.lib %2\mpir.lib
	if exist %1\mpir.pdb (copy %1\mpir.pdb %2\mpir.pdb)
	copy mpir-tests\%4-%3-config.props mpir-tests\test-config.props
) else if "%4" EQU "lib" (
	copy %1\mpir.lib %2\mpir.lib
	if exist %1\mpir.pdb (copy %1\mpir.pdb %2\mpir.pdb)
	copy mpir-tests\%4-%3-config.props mpir-tests\test-config.props
) else (
	call :seterr & echo ERROR: illegal library type %4  & exit /b %errorlevel%
)
exit /b 0

rem copy headers to final destination directory
:copyh
copy ..\config.h %1\config.h
copy ..\gmp-mparam.h %1\gmp-mparam.h
copy ..\mpir.h %1\mpir.h
copy ..\mpir.h %1\gmp.h
copy ..\mpirxx.h %1\mpirxx.h
copy ..\mpirxx.h %1\gmpxx.h
exit /b 0

:seterr
exit /b 1
