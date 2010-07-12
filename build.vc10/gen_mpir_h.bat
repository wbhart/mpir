@echo off
echo /* generated from gmp-h.in by gen_mpt_h.bat */ >mpt.h
if /i "%1" == "win32" set bits=32
if /i "%1" == "x64" set bits=64
for /f "tokens=1,2 delims=@" %%a in (..\..\gmp-h.in) do (
 if "%%a" NEQ "DEFN_LONG_LONG_LIMB" if "%%a" NEQ "GMP_CC" if "%%a" NEQ "GMP_CFLAGS" (
  if "%%b" NEQ "LIBGMP_DLL" if "%%b" NEQ "CC" if "%%b" NEQ "CFLAGS" (
   if "%%b" NEQ "HAVE_HOST_CPU_FAMILY_power" if "%%b" NEQ "HAVE_HOST_CPU_FAMILY_powerpc" (
    if "%%b" EQU "BITS_PER_MP_LIMB" (
        echo %%a %bits%>>mpt.h
    ) else if "%%b" EQU "GMP_NAIL_BITS" (
        echo %%a 0 >>mpt.h
        echo #define SIZEOF_MP_LIMB_T ^(GMP_LIMB_BITS ^>^> 3^)>>mpt.h 
        if "%1" == "win32" (
            echo #if defined _LONG_LONG_LIMB>>mpt.h
            echo #  undef _LONG_LONG_LIMB>>mpt.h
            echo #endif>>mpt.h
        )
        if "%1" == "x64" (
            echo #if !defined _LONG_LONG_LIMB>>mpt.h
            echo #  define _LONG_LONG_LIMB 1 >>mpt.h
            echo #endif>>.\mpt.h
        )
    ) else (echo %%a%%b>>mpt.h)
   )
  )
 )
)
call ..\out_copy_rename mpt.h ..\..\ mpir.h
del mpt.h
