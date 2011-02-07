@echo off
echo creating mpir.h for %1
echo /* generated from gmp-h.in by gen_mpir_h.bat */ >tmp.h
if /i "%1" EQU "win32" set bits=32
if /i "%1" EQU "x64" set bits=64
for /f "tokens=1,2 delims=@" %%a in (..\gmp-h.in) do (
 if "%%a" NEQ "DEFN_LONG_LONG_LIMB" if "%%a" NEQ "GMP_CC" if "%%a" NEQ "GMP_CFLAGS" (
  if "%%b" NEQ "LIBGMP_DLL" if "%%b" NEQ "CC" if "%%b" NEQ "CFLAGS" (
   if "%%b" NEQ "HAVE_HOST_CPU_FAMILY_power" if "%%b" NEQ "HAVE_HOST_CPU_FAMILY_powerpc" (
    if "%%b" EQU "BITS_PER_MP_LIMB" (
        echo %%a %bits%>>tmp.h
    ) else if "%%b" EQU "GMP_NAIL_BITS" (
        echo %%a 0 >>tmp.h
        echo #define SIZEOF_MP_LIMB_T ^(GMP_LIMB_BITS ^>^> 3^)>>tmp.h 
        if "%1" EQU "win32" (
            echo #if defined _LONG_LONG_LIMB>>tmp.h
            echo #  undef _LONG_LONG_LIMB>>tmp.h
            echo #endif>>tmp.h
        )
        if "%1" EQU "x64" (
            echo #if !defined _LONG_LONG_LIMB>>tmp.h
            echo #  define _LONG_LONG_LIMB 1 >>tmp.h
            echo #endif>>.\tmp.h
            echo #if !defined _WIN64>>tmp.h
            echo #define _WIN64 >>.\tmp.h
            echo #endif>>.\tmp.h
        )
    ) else (echo %%a%%b>>tmp.h)
   )
  )
 )
)
call out_copy_rename tmp.h ..\ mpir.h
del tmp.h
