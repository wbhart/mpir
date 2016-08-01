@echo off
rem %1 = mpn directory (generic, x86\... or x86_64\...) 
rem %2 = platform (win32 or x64)
rem %3 = MSVC version number (e.g. 14)

if /i "%2" EQU "win32" ((set platform=win32) & (set bdir=x86w\)) else ((set platform=x64) & (set bdir=x86_64w\))
set sdir=
if /i "%1" EQU "gc" ((set sdir=generic) & (set bdir=generic)) else (set sdir=%bdir%%1)
if not exist ..\mpn\%sdir% (call :seterr & echo ERROR: %1 is not supported & exit /b %errorlevel%)

echo building MPIR for %1 (%platform%) from directory mpn\%sdir%

set cdir=..\build.vc%3\cdata\mpn\%sdir%\
set sdir=..\mpn\%sdir%\
set bdir=..\mpn\%bdir%\

call gen_mpir_h %platform%
call gen_config_h %cdir%

if exist %sdir%\gmp-mparam.h (call out_copy_rename %sdir%\gmp-mparam.h ..\ gmp-mparam.h) else (
    call out_copy_rename %bdir%\gmp-mparam.h ..\ gmp-mparam.h)

type ..\longlong_pre.h >tmp.h
type %bdir%\longlong_inc.h >>tmp.h
type ..\longlong_post.h >>tmp.h
call out_copy_rename tmp.h ..\ longlong.h
del tmp.h

exit /b 0

:seterr
exit /b 1
