@echo off
rem %1 = full target path
rem %2 = MSVC version number (e.g. 14)

set msvc_ver=%2
set str=%~1

rem delete anything from the path before 'build.vc'
:dele
set str=%str:~1%
set str2=%str:~0,8%
if "%str2%" NEQ "build.vc" goto dele

rem we now have: build.vc<nn>\<project_directory>\<win32|x64>\<debug|release>\mpir.<lib|dll>
rem extract: project_directory, platform (plat=<win32|x64>), configuration (conf=<debug|release>) and file name  
set file=
for /f "tokens=1,2,3,4,5 delims=\" %%a in ("%str%") do set tloc=%%b&set plat=%%c&set conf=%%d&set file=%%e
if /i "%file%" NEQ "" (goto next)
call :seterr & echo ERROR: %1 is not supported & exit /b %errorlevel%

:next
rem echo target=%tloc%, platform=%plat%, configuration=%conf%, file=%file%

rem get the filename extension (lib/dll) to set the output directory
set loc=%tloc%\
set extn=%file%#
set filename=%extn:~0,-5%
set extn=%extn:~-4,3%
if "%extn%" EQU "lib" (goto is2nd)
if "%extn%" EQU "dll" (goto is2nd)
call :seterr & echo "postbuild copy error ERROR: target=%tloc%, plat=%plat%, conf=%conf%, file=%file%, filename=%filename%, extn=%extn%" & exit /b %errorlevel%

:is2nd:
rem set the target and final binary output directories
set tgt_dir="..\build.vc%msvc_ver%\%loc%%plat%\%conf%\"
set bin_dir="..\%extn%\%plat%\%conf%\"
set hdr_dir="..\%extn%\%plat%\%conf%\"

rem output parametrers for the MPIR tests
if /i "%filename%" EQU "mpirxx" goto skip 
echo (set ldir=%loc%)   > output_params.bat
echo (set libr=%extn%) >> output_params.bat
echo (set plat=%plat%) >> output_params.bat
echo (set conf=%conf%) >> output_params.bat
:skip

echo copying outputs from %tgt_dir% to %bin_dir%
if not exist %bin_dir% md %bin_dir%
call :copyh %tgt_dir%
call :copyh %hdr_dir%
call :copyb %tgt_dir% %bin_dir% %conf% %extn% %filename%
exit /b 0

rem copy binaries to final bin_dirination directory
rem %1 = target (build output) directory
rem %2 = binary destination directory
rem %3 = configuration (debug/release) 
rem %4 = library (lib/dll)
rem %5 = file name
:copyb
if "%4" EQU "dll" (
	copy %1mpir.dll %2mpir.dll > nul 2>&1
	copy %1mpir.exp %2mpir.exp > nul 2>&1
	copy %1mpir.lib %2mpir.lib > nul 2>&1
	if exist %1mpir.pdb (copy %1mpir.pdb %2mpir.pdb  > nul 2>&1)
) else if "%4" EQU "lib" (
    if "%5" EQU "mpir" ( 
  	    if exist %1mpir.lib (
        copy %1mpir.lib %2mpir.lib > nul 2>&1
	    if exist %1mpir.pdb (copy %1mpir.pdb %2mpir.pdb > nul 2>&1)
        )
    ) else if "%5" EQU "mpirxx" (
	    if exist %1mpirxx.lib (
        copy %1mpirxx.lib %2mpirxx.lib > nul 2>&1
	    if exist %1mpirxx.pdb (copy %1mpirxx.pdb %2mpirxx.pdb > nul 2>&1)
        )
    )
) else (
	call :seterr & echo ERROR: illegal library type %4  & exit /b %errorlevel%
)

rem set configuration for the tests
call gen_test_config_props %plat% %conf% %msvc_ver%
exit /b 0

rem copy headers to final destination directory
:copyh
copy ..\config.h %1config.h > nul 2>&1
copy ..\gmp-mparam.h %1gmp-mparam.h > nul 2>&1
copy ..\mpir.h %1mpir.h > nul 2>&1
copy ..\mpir.h %1gmp.h > nul 2>&1
copy ..\gmp-impl.h %1gmp-impl.h > nul 2>&1
copy ..\longlong.h %1longlong.h > nul 2>&1
copy ..\mpirxx.h %1mpirxx.h > nul 2>&1
copy ..\mpirxx.h %1gmpxx.h > nul 2>&1
exit /b 0

:seterr
exit /b 1
