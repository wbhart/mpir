
# generate vcxproj file

from os.path import normpath, join, split, relpath
from enum import IntEnum

class Project_Type(IntEnum):
  APP = 0
  LIB = 1
  DLL = 2

app_type, lib_type, dll_type = 0, 1, 2
app_ext = ('.exe', '.lib', '.dll')
app_str = ('Application', 'StaticLibrary', 'DynamicLibrary')

def vcx_proj_cfg(plat, outf):

  f1 = r'''  <ItemGroup Label="ProjectConfigurations">
'''
  f2 = r'''    <ProjectConfiguration Include="{1:s}|{0:s}">
    <Configuration>{1:s}</Configuration>
    <Platform>{0:s}</Platform>
    </ProjectConfiguration>
'''
  f3 = r'''  </ItemGroup>
'''
  outf.write(f1)
  for pl in plat:
    for conf in ('Release', 'Debug'):
      outf.write(f2.format(pl, conf))
  outf.write(f3)

def vcx_globals(name, guid, outf):

  f1 = r'''  <PropertyGroup Label="Globals">
    <RootNamespace>{0:s}</RootNamespace>
    <Keyword>Win32Proj</Keyword>
    <ProjectGuid>{1:s}</ProjectGuid>
    </PropertyGroup>
'''
  outf.write(f1.format(name, guid))

def vcx_default_cpp_props(outf):

  f1 = r'''  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.Default.props" />
'''
  outf.write(f1)

def vcx_library_type(plat, proj_type, msvc_ver, outf):

  f1 = r'''  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='{1:s}|{0:s}'" Label="Configuration">
    <ConfigurationType>{2:s}</ConfigurationType>
    <UseDebugLibraries>{3:s}</UseDebugLibraries>
    <PlatformToolset>v{4:s}0</PlatformToolset>
    </PropertyGroup>
'''
  for pl in plat:
    for conf, bool in (('Release', 'false'), ('Debug', 'true')):
      outf.write(f1.format(pl, conf, app_str[proj_type], bool, msvc_ver))

def vcx_cpp_props(outf):

  f1 = r'''  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.props" />
'''
  outf.write(f1)

def vcx_extensions(outf):

  f1 = r'''  <ImportGroup Label="ExtensionSettings">
    <Import Project="../../build.vc/vsyasm.props" />
    </ImportGroup>
'''
  outf.write(f1)

def vcx_user_props(plat, proj_type, outf):

  f1 = r'''  <ImportGroup Condition="'$(Configuration)|$(Platform)'=='{1:s}|{0:s}'" Label="PropertySheets">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" />
    <Import Project="..\..\build.vc\common_{2:s}.props" />
  </ImportGroup>
'''
  for pl in plat:
    for conf in ('Release', 'Debug'):
      outf.write(f1.format(pl, conf, app_ext[proj_type][1:]))

def vcx_target_name_and_dirs(name, plat, proj_type, outf):

  f1 = r'''  <PropertyGroup>
    <_ProjectFileVersion>10.0.21006.1</_ProjectFileVersion>
'''
  f2 = r'''    <TargetName Condition="'$(Configuration)|$(Platform)'=='{1:s}|{0:s}'">{2:s}</TargetName>
'''
  f3 = r'''  </PropertyGroup>
'''
  outf.write(f1)
  for pl in plat:
    for conf in ('Release', 'Debug'):
      outf.write(f2.format(pl, conf, name))
  outf.write(f3)

def yasm_options(plat, proj_type, outf):

  f1 = r'''    <YASM>
    <Defines>{0:s}</Defines>
    <IncludePaths>..\..\mpn\x86{1:s}w\</IncludePaths>
    <Debug>true</Debug>
    <ObjectFileName>$(IntDir)mpn\</ObjectFileName>
    <ObjectFile>$(IntDir)mpn\</ObjectFile>
    </YASM>
'''

  outf.write(f1.format('DLL' if proj_type == dll_type else '', '' if plat == 'Win32' else '_64'))

def compiler_options(plat, proj_type, is_debug, outf):

  f1 = r'''    <ClCompile>
    <Optimization>{0:s}</Optimization>
    <IntrinsicFunctions>true</IntrinsicFunctions>
    <AdditionalIncludeDirectories>..\..\</AdditionalIncludeDirectories>
    <PreprocessorDefinitions>{1:s}%(PreprocessorDefinitions)</PreprocessorDefinitions>
    <RuntimeLibrary>MultiThreaded{2:s}</RuntimeLibrary>
    <ProgramDataBaseFileName>$(TargetDir)$(TargetName).pdb</ProgramDataBaseFileName>
    <DebugInformationFormat>ProgramDatabase</DebugInformationFormat>
    </ClCompile>
'''

  if proj_type == app_type:
    s1 = 'DEBUG;WIN32;_CONSOLE'
    s2 = ''
  if proj_type == dll_type:
    s1 = 'DEBUG;WIN32;HAVE_CONFIG_H;MSC_BUILD_DLL;'
    s2 = 'DLL'
  elif proj_type == lib_type:
    s1 = 'DEBUG;WIN32;_LIB;HAVE_CONFIG_H;'
    s2 = ''
  else:
    pass
  if plat == 'x64':
    s1 = s1 + '_WIN64;'
  if is_debug:
    opt, defines, crt = 'Disabled', '_' + s1, 'Debug' + s2
  else:
    opt, defines, crt = 'Full', 'N' + s1, s2
  outf.write(f1.format(opt, defines, crt))

def linker_options(outf):

  f1 = r'''    <Link>
    <GenerateDebugInformation>true</GenerateDebugInformation>
    <LargeAddressAware>true</LargeAddressAware>
    </Link>
'''
  outf.write(f1)

def vcx_pre_build(name, plat, outf):

  f1 = r'''    <PreBuildEvent>
    <Command>cd ../../build.vc
prebuild {0:s} {1:s} 14
    </Command>
    </PreBuildEvent>
'''
  outf.write(f1.format(name, plat))

def vcx_post_build(is_cpp, outf):

  f1 = r'''
    <PostBuildEvent>
    <Command>cd ../../build.vc
postbuild "$(TargetPath)" 14
    </Command>
    </PostBuildEvent>
'''

  outf.write(f1)

def vcx_tool_options(config, plat, proj_type, is_cpp, af_list, add_prebuild, outf):

  f1 = r'''  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='{1:s}|{0:s}'">
'''
  f2 = r'''  </ItemDefinitionGroup>
'''
  for pl in plat:
    for is_debug in (False, True):
      outf.write(f1.format(pl, 'Debug' if is_debug else 'Release'))
      if add_prebuild and not is_cpp:
        vcx_pre_build(config, pl, outf)
      if af_list:
        yasm_options(pl, proj_type, outf)
      compiler_options(pl, proj_type, is_debug, outf)
      if proj_type != lib_type:
        linker_options(outf)
      vcx_post_build(is_cpp, outf)
      outf.write(f2)

def vcx_hdr_items(hdr_list, relp, outf):

  f1 = r'''  <ItemGroup>
'''
  f2 = r'''    <ClInclude Include="{}{}" />
'''
  f3 = r'''  </ItemGroup>
'''
  outf.write(f1)
  for i in hdr_list:
    outf.write(f2.format(relp, i))
  outf.write(f3)

def vcx_c_items(cf_list, plat, relp, outf):

  f1 = r'''  <ItemGroup>
'''
  f2 = r'''    <ClCompile Include="{0:s}{1[0]:s}{1[1]:s}" />
'''
  f3 = r'''    <ClCompile Include="{0:s}{1[2]:s}\{1[0]:s}{1[1]:s}" />
'''
  f6 = r'''  </ItemGroup>
'''
  outf.write(f1)
  for nxd in cf_list:
    if nxd[2] == '':
      outf.write(f2.format(relp, nxd))
    else:
      outf.write(f3.format(relp, nxd))

  outf.write(f6)

def vcx_a_items(af_list, relp, outf):

  f1 = r'''  <ItemGroup>
'''
  f2 = r'''    <YASM Include="{0:s}{1[2]:s}\{1[0]:s}{1[1]:s}" />
'''
  f3 = r'''  </ItemGroup>
'''
  outf.write(f1)
  for nxd in af_list:
    outf.write(f2.format(relp, nxd))
  outf.write(f3)

def gen_vcxproj(proj_name, file_name, build_dir, mpir_dir, guid, config, plat, proj_type,
                is_cpp, hf_list, cf_list, af_list, add_prebuild, vs_info):

  f1 = r'''<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" ToolsVersion="{}" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
'''
  f2 = r'''  <PropertyGroup Label="UserMacros" />
'''
  f3 = r'''  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
'''
  f4 = r'''  <ImportGroup Label="ExtensionTargets">
    <Import Project="../../build.vc/vsyasm.targets" />
    </ImportGroup>
'''

  f5 = r'''<ItemGroup>
    <None Include="../../gmp-h.in" />
    </ItemGroup>
</Project>
'''

  fn = normpath(join(build_dir, file_name))
  relp = split(relpath(mpir_dir, fn))[0] + '\\'
  with open(fn, 'w') as outf:
    outf.write(f1.format(vs_info['vcx_tool']))
    vcx_proj_cfg(plat, outf)
    vcx_globals(proj_name, guid, outf)
    vcx_default_cpp_props(outf)
    vcx_library_type(plat, proj_type, vs_info['msvc'], outf)
    vcx_cpp_props(outf)
    if af_list:
      vcx_extensions(outf)
    vcx_user_props(plat, proj_type, outf)
    outf.write(f2)
    vcx_target_name_and_dirs(proj_name, plat, proj_type, outf)
    vcx_tool_options(config, plat, proj_type, is_cpp, af_list, add_prebuild, outf)
    if hf_list:
      vcx_hdr_items(hf_list, relp, outf)
    vcx_c_items(cf_list, plat, relp, outf)
    vcx_a_items(af_list, relp, outf)
    outf.write(f3)
    if af_list:
      outf.write(f4)
    outf.write(f5)

