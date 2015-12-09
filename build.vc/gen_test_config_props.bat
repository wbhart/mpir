@echo off
rem %1 = plat
rem %2 = conf
rem %3 = output directory

rem output_params.bat contains:
rem (set ldir=architecture)   
rem (set libr=lib) 
rem (set plat=x64) 
rem (set conf=Release) 

set prefix=..\..\
call :clrerr

if exist output_params.bat (call output_params.bat) else (call :seterr & echo ERROR: 'output_params.bat' not found & exit /b %errorlevel%)
set lib_dir=%prefix%%ldir%\%plat%\%conf%

if /i "%plat%" EQU "" (call :seterr & echo ERROR: 'output_params.bat' is corrupt & exit /b %errorlevel%)
if /i "%1" NEQ "%plat%" goto bad_config
if /i "%2" NEQ "%conf%" goto bad_config

set rt_lib=MultiThreaded
if /i "%conf%" EQU "Debug" (set rt_lib=%rt_lib%Debug)
if /i "%libr%" EQu "DLL" (set rt_lib=%rt_lib%DLL)

echo ^<?xml version="1.0" encoding="utf-8"?^> >tmp.props
echo ^<Project DefaultTargets="Build" ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003"^> >>tmp.props
echo   ^<PropertyGroup Label="UserMacros"^> >>tmp.props
echo     ^<MPDIR^>%prefix%%ldir%%plat%\%conf%\^</MPDIR^> >>tmp.props
if /i "%libr%" EQU "LIB" (
echo     ^<MPLIBS^>%prefix%%ldir%%plat%\%conf%\mpir.lib;%prefix%lib_mpir_cxx\%plat%\%conf%\mpirxx.lib^</MPLIBS^> >>tmp.props
    ) else (
echo     ^<MPLIBS^>%prefix%%ldir%%plat%\%conf%\mpir.lib^</MPLIBS^> >>tmp.props
)
echo   ^</PropertyGroup^> >>tmp.props
echo   ^<ItemDefinitionGroup^> >>tmp.props
echo     ^<ClCompile^> >>tmp.props
echo       ^<RuntimeLibrary^>%rt_lib%^</RuntimeLibrary^> >>tmp.props
echo       ^<AdditionalIncludeDirectories^>$(MPDIR);$(SolutionDir)..\tests^</AdditionalIncludeDirectories^> >>tmp.props
if /i "%libr%" EQU "DLL" (
echo       ^<PreprocessorDefinitions^>__GMP_LIBGMP_DLL^</PreprocessorDefinitions^> >>tmp.props
)
echo     ^</ClCompile^> >>tmp.props
echo     ^<Link^> >>tmp.props
echo       ^<AdditionalDependencies^>$(MPLIBS);$(OutDir)\add-test-lib.lib;%%(AdditionalDependencies)^</AdditionalDependencies^> >>tmp.props
echo     ^</Link^> >>tmp.props
echo   ^</ItemDefinitionGroup^> >>tmp.props
echo   ^<ItemGroup^> >>tmp.props
echo     ^<BuildMacro Include="MPDIR"^> >>tmp.props
echo       ^<Value^>$(MPDIR)^</Value^> >>tmp.props
echo       ^<EnvironmentVariable^>true^</EnvironmentVariable^> >>tmp.props
echo     ^</BuildMacro^> >>tmp.props
echo     ^<BuildMacro Include="MPLIBS"^> >>tmp.props
echo       ^<Value^>$(MPLIBS)^</Value^> >>tmp.props
echo       ^<EnvironmentVariable^>true^</EnvironmentVariable^> >>tmp.props
echo     ^</BuildMacro^> >>tmp.props
echo   ^</ItemGroup^> >>tmp.props
echo ^</Project^> >>tmp.props

call out_copy_rename tmp.props ..\build.vc%3 test-config.props verb
del tmp.props
goto clrerr

:bad_config
call :seterr
exit /b %errorlevel%

:clrerr
exit /b 0

:seterr
exit /b 1
