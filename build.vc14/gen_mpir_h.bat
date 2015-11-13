@echo off
rem %1 = platform

echo creating mpir.h for %1
echo /* generated from gmp-h.in by gen_mpir_h.bat */ >tmp.h

rem if /i "%1" EQU "x64" (
rem   echo ^#ifndef _WIN64 >>tmp.h
rem   echo ^#define _WIN64 >>tmp.h
rem   echo ^#endif >>tmp.h
rem )

for /f "tokens=1,2 delims=@" %%a in (..\gmp-h.in) do (
  if "%%a" NEQ "DEFN_LONG_LONG_LIMB" if "%%a" NEQ "DEFN_LONG_LONG_LIMB" if "%%a" NEQ "GMP_CC" if "%%a" NEQ "GMP_CFLAGS" (
    if "%%b" EQU "GMP_NAIL_BITS" (
       echo ^#ifdef _WIN32 >>tmp.h
       echo ^#  ifdef _WIN64 >>tmp.h
       echo ^#    define _LONG_LONG_LIMB  1 >>tmp.h
       echo ^#    define GMP_LIMB_BITS   64 >>tmp.h
       echo ^#  else >>tmp.h
       echo ^#    define GMP_LIMB_BITS   32 >>tmp.h
       echo ^#  endif >>tmp.h
       echo ^#  define __GMP_BITS_PER_MP_LIMB  GMP_LIMB_BITS >>tmp.h
       echo ^#  define SIZEOF_MP_LIMB_T ^(GMP_LIMB_BITS ^>^> 3^) >>tmp.h
       echo ^#  define GMP_NAIL_BITS                       0 >>tmp.h
       echo ^#endif >>tmp.h
    ) else if "%%b" EQU "" echo %%a%%b>>tmp.h
  )
)
call out_copy_rename tmp.h ..\ mpir.h
call out_copy_rename tmp.h ..\ gmp.h
del tmp.h
