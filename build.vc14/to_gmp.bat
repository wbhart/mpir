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
if exist mpir.dll   (ren mpir.dll   gmp.dll)
if exist mpir.lib   (ren mpir.lib   gmp.lib)
if exist mpir.pdb   (ren mpir.pdb   gmp.pdb)
if exist mpir.exp   (ren mpir.exp   gmp.exp)
if exist mpirxx.lib (ren mpirxx.lib gmpxx.lib)
if exist mpirxx.pdb (ren mpirxx.pdb gmpxx.pdb)
popd
exit /b 0
