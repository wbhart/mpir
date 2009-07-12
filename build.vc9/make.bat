@echo off
if not exist config_params.bat (
	echo run configure first
	goto :EOF
)
call config_params.bat
if "%1" == ""        goto :make
if "%1" == "clean"   goto :clean
if "%1" == "install" goto :install
if "%1" == "check"   goto :check
if "%1" == "speed"   goto :speed
if "%1" == "tune"    goto :speed
if "%1" == "try"     goto :try
if "%1" == "help" (
	echo Usage : make [clean|install|check|speed|tune|try|help]
	goto :EOF
)
echo Unkwown option
goto :EOF

:make
vcbuild gen-mpir\gen-mpir.vcproj "Release|Win32"
vcbuild gen-bases\gen-bases.vcproj "Release|Win32"
vcbuild gen-fac_ui\gen-fac_ui.vcproj "Release|Win32%"
vcbuild gen-fib\gen-fib.vcproj "Release|Win32"
vcbuild gen-psqr\gen-psqr.vcproj "Release|Win32"
set LIBBUILD=%LIBTYPE%_mpir_%BCPU%
vcbuild %LIBBUILD%\%LIBBUILD%.vcproj "Release|%ARCHW%"
:: c++ to build  if static
if %LIBTYPE% == lib (
	vcbuild lib_mpir_cpp\lib_mpir_cpp.vcproj "Release|%ARCHW%"
)
goto :EOF

:check
:: this gives an error if we dont build the c++ stuff
vcbuild mpir-tests.sln "Release|%ARCHW%"
::copy gmp-mparam.h %LIBTYPE%\%ARCHW%\Release\gmp-mparam.h
::vcbuild /override:mpir-tests\test-config.vsprops mpir-tests\add-test-lib.vcproj "Release|%ARCHW%"
::vcbuild mpir-tests\add-test-lib.vcproj "Release|%ARCHW%"
cd mpir-tests
python --version >nul 2>&1
if not errorlevel 1 goto :got
if exist c:\Python26 (
        set PATH=%PATH%;c:\Python26
        python --version >nul 2>&1
        if not errorlevel 1 goto :got
)
if exist c:\Python30 (
        set PATH=%PATH%;c:\Python30
        python --version >nul 2>&1
        if not errorlevel 1 goto :got
)
echo ERROR Could not find PYTHON
goto :EOF
:got
python run-tests.py
cd ..
goto :EOF

:install
goto :EOF

:speed
vcbuild speed.sln "Release|%ARCHW%"
echo tune.exe and speed.exe are in %ARCHW%\Release\
goto :EOF

:try
vcbuild try\try.vcproj "Release|%ARCHW%"
echo try.exe is in try\%ARCHW%\Release\
goto :EOF

:clean
del config_params.bat config.guess.bat config.guess.exe config.guess.obj last_build.txt
set LIBBUILD=%LIBTYPE%_mpir_%BCPU%
del /q/s gen-psqr\gen-psqr.exe gen-fac_ui\gen-fac_ui.exe gen-bases\gen-bases.exe
del /q/s gen-fib\gen-fib.exe gen-mpir\gen-mpir.exe gen-mpir\gen-mpir.pdb
rmdir /s/q x64 win32 %LIBBUILD%\%ARCHW% lib_mpir_cpp\%ARCHW% lib\%ARCHW% dll\%ARCHW% mpir-tests\%ARCHW%
rmdir /s/q gen-psqr\%ARCHW% gen-fac_ui\%ARCHW% gen-bases\%ARCHW% gen-fib\%ARCHW% gen-mpir\%ARCHW%
rmdir /s/q lib_speed\%ARCHW% speed\%ARCHW% tune\%ARCHW% try\%ARCHW%
goto :EOF

:EOF
