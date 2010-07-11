@echo off
set in1=%1#
set in3=%in1: =%
for /f %%a in (..\dir.txt) do set in2=%%a#
set in4=%in2: =%
call :clre
if /i %in3% NEQ %in4% call :sete
echo error last MPIR build configuration (%in4:~0,-1%) does not match this test configuration (%in3:~0,-1%) 
exit /b %errorlevel%

:clre
exit /b 0

:sete
exit /b 1
