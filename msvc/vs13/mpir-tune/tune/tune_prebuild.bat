@echo off

for /f "usebackq tokens=1*" %%f in (`reg query HKCR\Applications\python.exe\shell\open\command  2^>NUL`) do (set _my_=%%f %%g)
goto try1%errorlevel%

:try10
goto ok

:try11
for /f "usebackq tokens=1*" %%f in (`reg query HKCR\Python.File\shell\open\command 2^>NUL`) do (set _my_=%%f %%g)
goto try2%errorlevel%

:try20:
goto ok

:try21:
echo Error: Python not found ...
goto :eof

:ok
echo Building with Python ...
set _res_=%_my_:*REG_SZ=%
set _end_=%_res_:*exe"=%
call set _python_=%%_res_:%_end_%=%%
call %_python_% tune_prebuild.py


