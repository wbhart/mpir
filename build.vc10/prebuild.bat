@echo off
if "%2" EQU "x64" ((set platform=x64) & (set bdir=x86_64w\)) else ((set platform=win32) & (set bdir=x86w\))
set sdir=
if /i "%1" EQU "gc" ((set sdir=generic) & (set bdir=generic)) else (set sdir=%bdir%%1)
if exist ..\mpn\%sdir% (echo.%sdir%) else (call :seterr & echo ERROR: %1 is not supported & exit /b %errorlevel%)
echo building MPIR for %1 (%platform%) from directory mpn\%sdir%
set sdir=..\mpn\%sdir%
set bdir=..\mpn\%bdir%

call gen_mpir_h %platform%
call gen_config_h %sdir%

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
