@echo off

if exist lib\win32\release call :sub lib\win32\release
if exist lib\win32\debug   call :sub lib\win32\debug
if exist lib\x64\release   call :sub lib\x64\release
if exist lib\x64\debug     call :sub lib\x64\debug
if exist dll\win32\release call :sub dll\win32\release
if exist dll\win32\debug   call :sub dll\win32\debug
if exist dll\x64\release   call :sub dll\x64\release
if exist dll\x64\debug     call :sub dll\x64\debug

exit /b 0

:sub
echo off
if not exist %1 exit /b 0
pushd %1
if exist gmp.dll   (ren gmp.dll   mpir.dll)
if exist gmp.lib   (ren gmp.lib   mpir.lib)
if exist gmp.pdb   (ren gmp.pdb   mpir.pdb)
if exist gmp.exp   (ren gmp.exp   mpir.exp)
if exist gmpxx.lib (ren gmpxx.lib mpirxx.lib)
if exist gmpxx.pdb (ren gmpxx.pdb mpirxx.pdb)
popd
exit /b 0
