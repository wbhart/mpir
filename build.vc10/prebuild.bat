@echo off
echo building MPIR for %1
set sdir=
if /i "%1" EQU "gc" (set sdir=generic) && if "%2" EQU "x64" (set platform=x64) else (set platform=win32)

if /i "%1" EQU "core2"  ((set sdir=x86_64w\core2) & (set platform=x64))
if /i "%1" EQU "k8" ((set sdir=x86_64w\k8) & (set platform=x64)) 
if /i "%1" EQU "k10" ((set sdir=x86_64w\k8\k10) & (set platform=x64))
if /i "%1" EQU "nehalem" ((set sdir=x86_64w\nehalem) & (set platform=x64))

if /i "%1" EQU "p0" ((set sdir=x86w\p0) & (set platform=win32))
if /i "%1" EQU "p3" ((set sdir=x86w\p3) & (set platform=win32))
if /i "%1" EQU "p4" ((set sdir=x86w\p4) & (set platform=win32))

if /i "%sdir%" EQU "" (call :seterr & echo ERROR: %1 is not supported & exit /b %errorlevel%)

call gen_mpir_h %platform%
call gen_config_h  ..\mpn\%sdir%\
call out_copy_rename ..\mpn\%sdir%\gmp-mparam.h ..\ gmp-mparam.h
exit /b 0

:seterr
exit /b 1
