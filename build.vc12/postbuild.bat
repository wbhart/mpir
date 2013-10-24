@echo off
set str=%~1

rem delete anything from path before 'build.vc12'
:dele
set str=%str:~1%
set str2=%str:~0,10%
if "%str2%" NEQ "build.vc12" goto dele

rem we now have: build.vc12\<win32|x64>\<debug|release>\mpir.<lib|dll>

rem extract platform (plat=<win32|x64>), configuration (conf=<debug|release>) and file name 
rem for the current working directory = build.vc12\lib_mpir_nehalem
rem IDE gives:     build.vc12\x64\Release\mpir.lib   
rem MSBUILD gives: build.vc12\lib_mpir_nehalem\x64\Release\mpir.lib  
rem Python gives:  build.vc12\lib_mpir_nehalem\x64\Release\mpir.lib
 
set file=
for /f "tokens=1,2,3,4,5 delims=\" %%a in ("%str%") do set plat=%%b&set conf=%%c&set file=%%d&set msbf=%%e
if /i "%file%" NEQ "" (goto next)
call :seterr & echo ERROR: %1 is not supported & exit /b %errorlevel%

:next
rem echo platform= %plat% configuration= %conf%, file= %file%

rem get the filename extension (lib/dll) to set the output directory
set extn=%file%#
set filename=%extn:~0,-5%
set extn=%extn:~-4,3%
if "%extn%" EQU "lib" (goto is2nd)
if "%extn%" EQU "dll" (goto is2nd)
set extn=%msbf%#
set extn=%extn:~-4,3%
if "%extn%" EQU "lib" (goto is1st)
if "%extn%" EQU "dll" (goto is1st)
call :seterr & echo "postbuild copy error ERROR: file = %file%, msbf = %msbf% extn = %extn%" & exit /b %errorlevel%

:is1st
set plat=%conf%
set conf=%file%

:is2nd:
rem set the target aand output directories
set source="%plat%\%conf%"
set dest="..\%extn%\%plat%\%conf%"

rem output parametrers for the MPIR tests
echo (set libr=%extn%)  > output_params.bat
echo (set plat=%plat%) >> output_params.bat
echo (set conf=%conf%) >> output_params.bat

echo copying outputs from %source% to %dest%
if not exist %dest% md %dest%
call :copyh %dest%
call :copyb %source% %dest% %conf% %extn% %filename%
exit /b 0

rem copy binaries to final destination directory
rem %1 = source directory
rem %2 = destination directory
rem %3 = configuration (debug/release) 
rem %4 = library (lib/dll)
rem %5 = file naame

:copyb
if "%4" EQU "dll" (
	copy %1\mpir.dll %2\mpir.dll > nul 2>&1
	copy %1\mpir.exp %2\mpir.exp > nul 2>&1
	copy %1\mpir.lib %2\mpir.lib > nul 2>&1
	if exist %1\mpir.pdb (copy %1\mpir.pdb %2\mpir.pdb  > nul 2>&1)
	copy mpir-tests\%4-%3-config.props mpir-tests\test-config.props > nul 2>&1
) else if "%4" EQU "lib" (
    if "%5" EQU "mpir" ( 
  	    if exist %1\mpir.lib (
        copy %1\mpir.lib %2\mpir.lib > nul 2>&1
	    if exist %1\mpir.pdb (copy %1\mpir.pdb %2\mpir.pdb > nul 2>&1)
	    copy mpir-tests\%4-%3-config.props mpir-tests\test-config.props > nul 2>&1
        )
    ) else if "%5" EQU "mpirxx" (
	    if exist %1\mpirxx.lib (
        copy %1\mpirxx.lib %2\mpirxx.lib > nul 2>&1
	    if exist %1\mpirxx.pdb (copy %1\mpirxx.pdb %2\mpirxx.pdb > nul 2>&1)
        )
    )
) else (
	call :seterr & echo ERROR: illegal library type %4  & exit /b %errorlevel%
)
exit /b 0

rem copy headers to final destination directory
:copyh
copy ..\config.h %1\config.h > nul 2>&1
copy ..\gmp-mparam.h %1\gmp-mparam.h > nul 2>&1
copy ..\mpir.h %1\mpir.h > nul 2>&1
copy ..\mpir.h %1\gmp.h > nul 2>&1
copy ..\gmp-impl.h %1\gmp-impl.h > nul 2>&1
copy ..\longlong.h %1\longlong.h > nul 2>&1
copy ..\mpirxx.h %1\mpirxx.h > nul 2>&1
copy ..\mpirxx.h %1\gmpxx.h > nul 2>&1
exit /b 0

:seterr
exit /b 1
