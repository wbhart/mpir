@echo off
::  can use x86 or amd64 as param for the below batch file
call "%VS90COMNTOOLS%\..\..\VC\vcvarsall.bat" >nul
echo cl %*
cl %*


