'''
Set up Visual Sudio to build a specified MPIR configuration

Copyright (C) 2011, 2012, 2013, 2014 Brian Gladman
'''

from __future__ import print_function
from operator import itemgetter
from os import listdir, walk, unlink, makedirs
from os.path import split, splitext, isdir, relpath, join, exists
from os.path import dirname, normpath
from copy import deepcopy
from sys import argv, exit
from filecmp import cmp
from shutil import copy
from re import compile, search
from collections import defaultdict
from uuid import uuid4
from time import sleep

solution_name = 'mpir.sln'
try:
  input = raw_input
except NameError:
  pass
app_type, lib_type, dll_type = 0, 1, 2
app_str = ('Application', 'StaticLibrary', 'DynamicLibrary')
app_ext = ('.exe', '.lib', '.dll')

# for script debugging
debug = False
# either add a prebuild step to the project files or do it here
add_prebuild = True
# output a build project for the C++ static library
add_cpp_lib = False

# The path to the mpir root directory
build_vc = 'build.vc12/'
mpir_dir = '../'
build_dir = mpir_dir + build_vc
cfg_dir = './cdata'
solution_dir = join(mpir_dir, build_vc)

# paths that might include source files(*.c, *.h, *.asm)
c_directories = ('', 'build.vc12', 'fft', 'mpf', 'mpq', 'mpz',
                 'printf', 'scanf')

# files that are to be excluded from the build
exclude_file_list = ('config.guess', 'cfg', 'getopt', 'getrusage',
                     'gettimeofday', 'cpuid', 'obsolete', 'win_timing',
                     'gmp-mparam', 'tal-debug', 'tal-notreent', 'new_fft',
                     'new_fft_with_flint', 'compat', 'udiv_w_sdiv')

# copy from file ipath to file opath but avoid copying if
# opath exists and is the same as ipath (this is to avoid
# triggering an unecessary rebuild).

def write_f(ipath, opath):
  if exists(ipath) and not isdir(ipath):
    if exists(opath):
      if isdir(opath) or cmp(ipath, opath):
        return
    copy(ipath, opath)

# append a file (ipath) to an existing file (opath)

def append_f(ipath, opath):
  try:
    with open(opath, 'ab') as out_file:
      try:
        with open(ipath, 'rb') as in_file:
          buf = in_file.read(8192)
          while buf:
            out_file.write(buf)
            buf = in_file.read(8192)
      except IOError:
        print('error reading {0:s} for input'.format(f))
        return
  except IOError:
    print('error opening {0:s} for output'.format(opath))

# copy files in a list from in_dir to out_dir
def copy_files(file_list, in_dir, out_dir):
  try:
    makedirs(out_dir)
  except IOError:
    pass
  for f in file_list:
    copy(join(in_dir, f), out_dir)

# Recursively search a given directory tree to find header,
# C and assembler code files that either replace or augment
# the generic C source files in the input list 'src_list'.
# As the directory tree is searched, files in each directory
# become the source code files for the current directory and
# the default source code files for its child directories.
#
# Lists of default header, C and assembler source code files
# are maintained as the tree is traversed and if a file in
# the current directory matches the name of a file in the
# default file list (name matches ignore file extensions),
# the name in the list is removed and is replaced by the new
# file found. On return each directory in the tree had an
# entry in the returned dictionary that contains:
#
# 1. The list of header files
#
# 2. The list of C source code files for the directory
#
# 3. The list of assembler code files that replace C files
#
# 4. The list of assembler files that are not C replacements
#

def find_asm(path, cf_list):
  d = dict()
  for root, dirs, files in walk(path):
    if '.svn' in dirs:                  # ignore SVN directories
      dirs.remove('.svn')
    if 'fat' in dirs:                   # ignore fat directory
      dirs.remove('fat')
    relp = relpath(root, path)          # path from asm root
    relr = relpath(root, mpir_dir)      # path from MPIR root
    if relp == '.':                     # set C files as default
      relp = h = t = ''
      d[''] = [[], deepcopy(cf_list), [], [], relr]
    else:
      h, _ = split(relp)                # h = parent, t = this directory
      # copy defaults from this directories parent
      d[relp] = [deepcopy(d[h][0]), deepcopy(d[h][1]),
                 deepcopy(d[h][2]), deepcopy(d[h][3]), relr]
    for f in files:                     # for the files in this directory
      n, x = splitext(f)
      if x == '.h':                     # if it is a header file, remove
        for cf in reversed(d[relp][0]): # any matching default
          if cf[0] == n:
            d[relp][0].remove(cf)
        d[relp][0] += [(n, x, relr)]    # and add the local header file
      if x == '.c':                     # if it is a C file, remove
        for cf in reversed(d[relp][1]): # any matching default
          if cf[0] == n:
            d[relp][1].remove(cf)
        d[relp][1] += [(n, x, relr)]    # and add the local C file
      if x == '.asm':                   # if it is an assembler file
        match = False
        for cf in reversed(d[relp][1]): # remove any matching C file
          if cf[0] == n:
            d[relp][1].remove(cf)
            match = True
            break
        for cf in reversed(d[relp][2]): # and remove any matching
          if cf[0] == n:                # assembler file
            d[relp][2].remove(cf)
            match = True
            break
        if match:                       # if a match was found, put the
          d[relp][2] += [(n, x, relr)]  # file in the replacement list
        else:                           # otherwise look for it in the
          for cf in reversed(d[relp][3]):  # additional files list
            if cf[0] == n:
              d[relp][3].remove(cf)
              break
          d[relp][3] += [(n, x, relr)]
  for k in d:                           # additional assembler list
    for i in range(4):
      d[k][i].sort(key=itemgetter(0))   # sort the four file lists
  return d

# create 4 lists of c, h, cc (or cpp) and asm (or as) files in a directory

def find_src(dir_list):
  # list number from file extension
  di = {'.h': 0, '.c': 1, '.cc': 2, '.cpp': 2, '.asm': 3, '.as': 3}
  list = [[], [], [], []]
  for d in dir_list:
    for f in listdir(join(mpir_dir, d)):
      if f == '.svn':
        continue                        # ignore SVN directories
      if not isdir(f):
        n, x = splitext(f)              # split into name + extension
        if x in di and not n in exclude_file_list:
          list[di[x]] += [(n, x, d)]    # if of the right type and is
  for x in list:                        # not in the exclude list
    x.sort(key=itemgetter(2, 0, 1))     # add it to appropriate list
  return list

# scan the files in the input set and find the symbols
# defined in the files
fr_sym = compile(r'LEAF_PROC\s+(\w+)')
lf_sym = compile(r'FRAME_PROC\s+(\w+)')
wf_sym = compile(r'WIN64_GCC_PROC\s+(\w+)')
g3_sym = compile(r'global\s+___g(\w+)')
g2_sym = compile(r'global\s+__g(\w+)')

def get_symbols(setf, sym_dir):
  for f in setf:
    fn = join(mpir_dir, f[2], f[0] + f[1])
    with open(fn, 'r') as inf:
      lines = inf.readlines()
      for l in lines:
        m = fr_sym.search(l)
        if m:
          sym_dir[f] |= set((m.groups(1)[0],))
        m = lf_sym.search(l)
        if m:
          sym_dir[f] |= set((m.groups(1)[0],))
        m = wf_sym.search(l)
        if m:
          sym_dir[f] |= set((m.groups(1)[0],))
        m = g3_sym.search(l)
        if m:
          sym_dir[f] |= set((m.groups(1)[0],))
        else:
          m = g2_sym.search(l)
          if m:
            sym_dir[f] |= set((m.groups(1)[0],))

def file_symbols(cf):
  sym_dir = defaultdict(set)
  for c in cf:
    if c == 'fat':
      continue
    setf = set()
    for f in cf[c][2] + cf[c][3]:
      setf |= set((f,))
    get_symbols(setf, sym_dir)
  return sym_dir

def gen_have_list(c, sym_dir, out_dir):
  set_sym2 = set()
  for f in c[2]:
    set_sym2 |= sym_dir[f]
  set_sym3 = set()
  for f in c[3]:
    set_sym3 |= sym_dir[f]
  c += [sorted(list(set_sym2)), sorted(list(set_sym3))]
  fd = join(out_dir, c[4])
  try:
    makedirs(fd)
  except IOError:
    pass
  with open(join(fd, 'cfg.h'), 'w') as outf:
    for sym in sorted(set_sym2 | set_sym3):
      print(sym, file=outf)
#   print('/* assembler symbols also available in C files */', file=outf)
#   for sym in sorted(set_sym2):
#     print(sym, file=outf)
#   print('/* assembler symbols not available in C files */', file=outf)
#   for sym in sorted(set_sym3):
#     print(sym, file=outf)

# generate Visual Studio IDE Filter

def filter_folders(cf_list, af_list, outf):

  f1 = r'''  <ItemGroup>
    <Filter Include="Header Files" />
    <Filter Include="Source Files" />
'''
  f2 = r'''    <Filter Include="Source Files\{0:s}" />
'''
  f3 = r'''  </ItemGroup>
'''
  c_dirs = set(i[2] for i in cf_list)
  a_dirs = set(i[2] for i in af_list)
  if a_dirs:
    c_dirs |= set((r'mpn\yasm',))
  outf.write(f1)
  for d in sorted(c_dirs):
    if d:
      t = d if d != r'mpn\generic' else r'mpn'
      outf.write(f2.format(t))
  outf.write(f3)


def filter_headers(hdr_list, relp, outf):

  f1 = r'''  <ItemGroup>
'''
  f2 = r'''    <ClInclude Include="{}{}">
    <Filter>Header Files</Filter>
    </ClInclude>
'''
  f3 = r'''  </ItemGroup>
'''
  outf.write(f1)
  for h in hdr_list:
    outf.write(f2.format(relp, h))
  outf.write(f3)

def filter_csrc(cf_list, relp, outf):

  f1 = r'''  <ItemGroup>
'''
  f2 = r'''  <ClCompile Include="{}{}">
    <Filter>Source Files</Filter>
    </ClCompile>
'''
  f3 = r'''  <ClCompile Include="{}{}\{}">
    <Filter>Source Files\{}</Filter>
    </ClCompile>
'''
  f4 = r'''  </ItemGroup>
'''
  outf.write(f1)
  for i in cf_list:
    if not i[2]:
      outf.write(f2.format(relp, i[0] + i[1]))
    else:
      t = 'mpn' if i[2].endswith('generic') else i[2]
      outf.write(f3.format(relp, i[2], i[0] + i[1], t))
  outf.write(f4)

def filter_asrc(af_list, relp, outf):

  f1 = r'''  <ItemGroup>
'''
  f2 = r'''  <YASM Include="{0:s}{2:s}\{1:s}">
    <Filter>Source Files\mpn\yasm</Filter>
    </YASM>
'''
  f3 = r'''  </ItemGroup>
'''
  outf.write(f1)
  for i in af_list:
    outf.write(f2.format(relp, i[0] + i[1], i[2], i[2]))
  outf.write(f3)

def gen_filter(name, hf_list, cf_list, af_list):

  f1 = r'''<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="12.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
'''
  f2 = r'''  <ItemGroup>
    <None Include="..\..\gmp-h.in" />
    </ItemGroup>
</Project>
'''

  fn = normpath(join(build_dir, name))
  relp = split(relpath(mpir_dir, fn))[0] + '\\'
  try:
    makedirs(split(fn)[0])
  except IOError:
    pass
  with open(fn, 'w') as outf:

    outf.write(f1)
    filter_folders(cf_list, af_list, outf)
    if hf_list:
      filter_headers(hf_list, relp, outf)
    filter_csrc(cf_list, relp, outf)
    if af_list:
      filter_asrc(af_list, relp, outf)
    outf.write(f2)

# generate vcxproj file

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

def vcx_library_type(plat, proj_type, outf):

  f1 = r'''  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='{1:s}|{0:s}'" Label="Configuration">
    <ConfigurationType>{2:s}</ConfigurationType>
    <PlatformToolset>v120</PlatformToolset>
    </PropertyGroup>
'''
  for pl in plat:
    for conf in ('Release', 'Debug'):
      outf.write(f1.format(pl, conf, app_str[proj_type]))

def vcx_cpp_props(outf):

  f1 = r'''  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.props" />
'''
  outf.write(f1)

def vcx_extensions(outf):

  f1 = r'''  <ImportGroup Label="ExtensionSettings">
    <Import Project="..\vsyasm.props" />
    </ImportGroup>
'''
  outf.write(f1)

def vcx_user_props(plat, outf):

  f1 = r'''  <ImportGroup Condition="'$(Configuration)|$(Platform)'=='{1:s}|{0:s}'" Label="PropertySheets">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" />
    </ImportGroup>
'''
  for pl in plat:
    for conf in ('Release', 'Debug'):
      outf.write(f1.format(pl, conf))

def vcx_target_name_and_dirs(name, plat, proj_type, outf):

  f1 = r'''  <PropertyGroup>
    <_ProjectFileVersion>10.0.21006.1</_ProjectFileVersion>
'''
  f2 = r'''    <TargetName Condition="'$(Configuration)|$(Platform)'=='{1:s}|{0:s}'">{2:s}</TargetName>
    <IntDir Condition="'$(Configuration)|$(Platform)'=='{1:s}|{0:s}'">$(Platform)\$(Configuration)\</IntDir>
    <OutDir Condition="'$(Configuration)|$(Platform)'=='{1:s}|{0:s}'">$(SolutionDir)$(Platform)\$(Configuration)\</OutDir>
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
    <Command>cd ..\
prebuild {0:s} {1:s}
    </Command>
    </PreBuildEvent>
'''
  outf.write(f1.format(name, plat))

def vcx_post_build(is_cpp, outf):

  f1 = r'''
    <PostBuildEvent>
    <Command>cd ..\
postbuild "$(TargetPath)"
    </Command>
    </PostBuildEvent>
'''

  outf.write(f1)

def vcx_tool_options(config, plat, proj_type, is_cpp, af_list, outf):

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
        yasm_options(plat, proj_type, outf)
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
  f3 = r'''    <ClCompile Include="{0:s}{1[2]:s}\{1[0]:s}{1[1]:s}">
'''
  f4 = r'''        <ObjectFileName Condition="'$(Configuration)|$(Platform)'=='{0:s}|{1:s}'">$(IntDir){2:s}\</ObjectFileName>
'''
  f5 = r'''    </ClCompile>
'''
  f6 = r'''  </ItemGroup>
'''
  outf.write(f1)
  for nxd in cf_list:
    if nxd[2] == '':
      outf.write(f2.format(relp, nxd))
    else:
      outf.write(f3.format(relp, nxd))
      for cf in ('Release', 'Debug'):
        for pl in plat:
          outf.write(f4.format(cf, pl, 'mpn' if nxd[2].endswith('generic') else nxd[2]))
      outf.write(f5)
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

def gen_vcxproj(proj_name, file_name, guid, config, plat, proj_type,
                is_cpp, hf_list, cf_list, af_list):

  f1 = r'''<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
'''
  f2 = r'''  <PropertyGroup Label="UserMacros" />
'''
  f3 = r'''  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
'''
  f4 = r'''  <ImportGroup Label="ExtensionTargets">
    <Import Project="..\vsyasm.targets" />
    </ImportGroup>
'''

  f5 = r'''<ItemGroup>
    <None Include="..\..\gmp-h.in" />
    </ItemGroup>
</Project>
'''

  fn = normpath(join(build_dir, file_name))
  relp = split(relpath(mpir_dir, fn))[0] + '\\'
  with open(fn, 'w') as outf:
    outf.write(f1)
    vcx_proj_cfg(plat, outf)
    vcx_globals(proj_name, guid, outf)
    vcx_default_cpp_props(outf)
    vcx_library_type(plat, proj_type, outf)
    vcx_cpp_props(outf)
    if af_list:
      vcx_extensions(outf)
    vcx_user_props(plat, outf)
    outf.write(f2)
    vcx_target_name_and_dirs(proj_name, plat, proj_type, outf)
    vcx_tool_options(config, plat, proj_type, is_cpp, af_list, outf)
    if hf_list:
      vcx_hdr_items(hf_list, relp, outf)
    vcx_c_items(cf_list, plat, relp, outf)
    vcx_a_items(af_list, relp, outf)
    outf.write(f3)
    if af_list:
      outf.write(f4)
    outf.write(f5)

# add a project file to the solution

folder_guid = "{2150E333-8FDC-42A3-9474-1A3956D46DE8}"
vcxproject_guid = "{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}"

s_guid = r'\s*(\{\w{8}-\w{4}-\w{4}-\w{4}-\w{12}\})\s*'
s_name = r'\s*\"([a-zA-Z][-.\\_a-zA-Z0-9]*\s*)\"\s*'
re_guid = compile(r'\s*\"\s*' + s_guid + r'\"\s*')
re_proj = compile(r'Project\s*\(\s*\"' + s_guid + r'\"\)\s*=\s*'
                  + s_name + r'\s*,\s*' + s_name + r'\s*,\s*\"' + s_guid + r'\"')
re_fmap = compile(r'\s*' + s_guid + r'\s*=\s*' + s_guid)

def read_solution_file(soln_name):
  fd, pd, p2f = {}, {}, {}
  solution_path = join(solution_dir, soln_name)
  if exists(solution_path):
    lines = open(solution_path).readlines()
    for i, ln in enumerate(lines):
      m = re_proj.search(ln)
      if m:
        if m.group(1) == folder_guid and m.group(2) == m.group(3):
          fd[m.group(2)] = m.group(4)
        elif m.group(3).endswith('.vcxproj') or m.group(3).endswith('.pyproj'):
          pd[m.group(2)] = (m.group(1), m.group(3), m.group(4))
      m = re_fmap.search(ln)
      if m:
        p2f[m.group(1)] = m.group(2)
  return fd, pd, p2f

sol_1 = '''Microsoft Visual Studio Solution File, Format Version 12.00
# Visual Studio 2013
VisualStudioVersion = 12.0.30626.0
MinimumVisualStudioVersion = 10.0.40219.1
'''

sol_2 = '''Project("{}") = "{}", "{}", "{}"
EndProject
'''

sol_3 = '''Global
	GlobalSection(SolutionConfigurationPlatforms) = preSolution
		Debug|Win32 = Debug|Win32
		Debug|x64 = Debug|x64
		Release|Win32 = Release|Win32
		Release|x64 = Release|x64
	EndGlobalSection
	GlobalSection(SolutionProperties) = preSolution
		HideSolutionNode = FALSE
	EndGlobalSection
	GlobalSection(NestedProjects) = preSolution
'''

sol_4 = '''		{} = {}
'''

sol_5 = r'''	EndGlobalSection
EndGlobal
'''

def write_solution_file(file_name, fd, pd, p2f):
  with open(join(solution_dir, file_name), 'w') as outf:
    outf.write(sol_1)
    for f, g in fd.items():
      outf.write(sol_2.format(folder_guid, f, f, g))
    for f, (g1, pn, g2) in pd.items():
      outf.write(sol_2.format(g1, f, pn, g2))
    outf.write(sol_3)
    for f, g in p2f.items():
      outf.write(sol_4.format(f, g))
    outf.write(sol_5)

def add_proj_to_sln(soln_name, soln_folder, proj_name, file_name, guid):
  fd, pd, p2f = read_solution_file(soln_name)
  if soln_folder:
    if soln_folder in fd:
      f_guid = fd[soln_folder]
    else:
      f_guid = '{' + str(uuid4()).upper() + '}'
      fd[soln_folder] = f_guid
  pd[proj_name] = (vcxproject_guid, file_name, guid)
  if soln_folder:
    p2f[guid] = f_guid

  write_solution_file(soln_name, fd, pd, p2f)
# compile list of C files
t = find_src(c_directories)
c_hdr_list = t[0]
c_src_list = t[1]
if t[2] or t[3]:
  print('found C++ and/or assembler file(s) in a C directory')
  if t[2]:
    for f in t[2]:
      print(f)
    print()
  if t[3]:
    for f in t[3]:
      print(f)
    print()

# compile list of C++ files
t = find_src(['cxx'])
cc_hdr_list = t[0]
cc_src_list = t[2]
if t[1] or t[3]:
  print('found C and/or assembler file(s) in a C++ directory')
  if t[1]:
    for f in t[1]:
      print(f)
    print()
  if t[3]:
    for f in cc_src_list:
      print(f)
    print()

# compile list of C files in mpn\generic
t = find_src([r'mpn\generic'])
gc_hdr_list = t[0]
gc_src_list = t[1]
if t[2] or t[3]:
  print('found C++ and/or assembler file(s) in a C directory')
  if t[2]:
    for f in gc_hdr_list:
      print(f)
    print()
  if t[3]:
    for f in gc_src_list:
      print(f)
    print()

# prepare the generic C build
mpn_gc = dict((('gc', [gc_hdr_list, gc_src_list, [], []]),))

# prepare the list of Win32 builds
mpn_32 = find_asm(mpir_dir + 'mpn/x86w', gc_src_list)
syms32 = file_symbols(mpn_32)
del mpn_32['']

# prepare the list of x64 builds
mpn_64 = find_asm(mpir_dir + 'mpn/x86_64w', gc_src_list)
syms64 = file_symbols(mpn_64)
del mpn_64['']

if len(argv) != 1 and not int(argv[1]) & 2:
  exit()

nd_gc = len(mpn_gc)
nd_32 = nd_gc + len(mpn_32)
nd_nd = nd_32 + len(mpn_64)

# now ask user which builds they wish to generate

while True:
  cnt = 0
  for v in sorted(mpn_gc):
    cnt += 1
    print('{0:2d}. {1:24s}        '.format(cnt, v))
  for v in sorted(mpn_32):
    cnt += 1
    print('{0:2d}. {1:24s} (win32)'.format(cnt, v))
  for v in sorted(mpn_64):
    cnt += 1
    print('{0:2d}. {1:24s}   (x64)'.format(cnt, v))
  fs = 'Space separated list of builds (1..{0:d}, 0 to exit)? '
  s = input(fs.format(cnt))
  n_list = [int(c) for c in s.split()]
  if 0 in n_list:
    exit()
  if any(n < 1 or n > nd_nd for n in n_list):
    print('list contains invalid build numbers')
    sleep(2)
  else:
    break

# multiple builds must each have their own prebuilds
if len(n_list) > 1:
  add_prebuild = True

# now gnerate the requested builds
for n in n_list:

  if 0 < n <= nd_gc:
    config = sorted(mpn_gc)[n - 1]
    mode = ('Win32', 'x64')
    mpn_f = mpn_gc[config]
  elif nd_gc < n <= nd_32:
    config = sorted(mpn_32)[n - 1 - nd_gc]
    if len(argv) == 1 or int(argv[1]) & 1:
      gen_have_list(mpn_32[config], syms32, cfg_dir)
    mode = ('Win32', )
    mpn_f = mpn_32[config]
  elif nd_32 < n <= nd_nd:
    config = sorted(mpn_64)[n - 1 - nd_32]
    if len(argv) == 1 or int(argv[1]) & 1:
      gen_have_list(mpn_64[config], syms64, cfg_dir)
    mode = ('x64', )
    mpn_f = mpn_64[config]
  else:
    print('internal error')
    exit()

  if mode[0] == 'x64':
    for l in mpn_f[1:]:
      for t in l:
        if t[0].startswith('preinv_'):
          if 'x64' in mode and t[0] == 'preinv_divrem_1':
            l.remove(t)

  print(config, mode)

  if not add_prebuild:
    # generate mpir.h and gmp.h from gmp_h.in
    gmp_h = '''
    #ifdef _WIN32
    #  ifdef _WIN64
    #    define _LONG_LONG_LIMB  1
    #    define GMP_LIMB_BITS   64
    #  else
    #    define GMP_LIMB_BITS   32
    #  endif
    #  define __GMP_BITS_PER_MP_LIMB  GMP_LIMB_BITS
    #  define SIZEOF_MP_LIMB_T (GMP_LIMB_BITS >> 3)
    #  define GMP_NAIL_BITS                       0
    #endif
    '''

    try:
      lines = open(join(mpir_dir, 'gmp-h.in'), 'r').readlines()
    except IOError:
      print('error attempting to read from gmp_h.in')
      exit()
    try:
      tfile = join(mpir_dir, 'tmp.h')
      with open(tfile, 'w') as outf:
        first = True
        for line in lines:
          if search(r'@\w+@', line):
            if first:
              first = False
              outf.writelines(gmp_h)
          else:
            outf.writelines([line])

      # write result to mpir.h but only overwrite the existing
      # version if this version is different (don't trigger an
      # unnecessary rebuild)
      write_f(tfile, join(mpir_dir, 'mpir.h'))
      write_f(tfile, join(mpir_dir, 'gmp.h'))
      unlink(tfile)
    except IOError:
      print('error attempting to create mpir.h from gmp-h.in')
      exit()

    # generate config.h

    try:
      tfile = join(mpir_dir, 'tmp.h')
      with open(tfile, 'w') as outf:
        for i in sorted(mpn_f[5] + mpn_f[6]):
          outf.writelines(['#define HAVE_NATIVE_{0:s} 1\n'.format(i)])

      append_f(join(build_dir, 'cfg.h'), tfile)
      write_f(tfile, join(mpir_dir, 'config.h'))
      unlink(tfile)
    except IOError:
      print('error attempting to write to {0:s}'.format(tfile))
      exit()

    # generate longlong.h and copy gmp-mparam.h

    try:
      li_file = None
      for i in mpn_f[0]:
        if i[0] == 'longlong_inc':
          li_file = join(mpir_dir, join(i[2], r'longlong_inc.h'))
        if i[0] == 'gmp-mparam':
          write_f(join(mpir_dir, join(i[2], 'gmp-mparam.h')),
                  join(mpir_dir, 'gmp-mparam.h'))

      if not li_file or not exists(li_file):
        print('error attempting to read {0:s}'.format(li_file))
        exit()

      tfile = join(mpir_dir, 'tmp.h')
      write_f(join(mpir_dir, 'longlong_pre.h'), tfile)
      append_f(li_file, tfile)
      append_f(join(mpir_dir, 'longlong_post.h'), tfile)
      write_f(tfile, join(mpir_dir, 'longlong.h'))
      unlink(tfile)
    except IOError:
      print('error attempting to generate longlong.h')
      exit()

  # generate the vcxproj and the IDE filter files
  # and add/replace project in the solution file

  hf_list = ('config.h', 'gmp-impl.h', 'longlong.h', 'mpir.h', 'gmp-mparam.h')
  af_list = sorted(mpn_f[2] + mpn_f[3])

  # find the gmp-mparam.h file to be used
  for name, ty, loc in mpn_f[0]:
    if name == 'gmp-mparam':
      loc = loc.replace('mpn\\x86w', '', 1)
      loc = loc.replace('mpn\\x86_64w', '', 1)
      if loc.startswith('\\'):
        loc = loc[1:]
      mp_dir = loc if loc else config
      break
  else:
    mp_dir = config

  proj_name = 'mpir'
  cf = config.replace('\\', '_')

  # set up DLL build
  guid = '{' + str(uuid4()) + '}'
  vcx_name = 'dll_mpir_' + cf
  vcx_path = 'dll_mpir_' + cf + '\\' + vcx_name + '.vcxproj'
  gen_filter(vcx_path + '.filters', hf_list,
             c_src_list + cc_src_list + mpn_f[1], af_list)
  gen_vcxproj(proj_name, vcx_path, guid, mp_dir, mode, dll_type,
              False, hf_list, c_src_list + cc_src_list + mpn_f[1], af_list)
  add_proj_to_sln(solution_name, '', vcx_name, vcx_path, guid)

  # set up LIB build
  guid = '{' + str(uuid4()) + '}'
  vcx_name = 'lib_mpir_' + cf
  vcx_path = 'lib_mpir_' + cf + '\\' + vcx_name + '.vcxproj'
  gen_filter(vcx_path + '.filters', hf_list, c_src_list + mpn_f[1], af_list)
  gen_vcxproj(proj_name, vcx_path, guid, mp_dir, mode, lib_type,
              False, hf_list, c_src_list + mpn_f[1], af_list)
  add_proj_to_sln(solution_name, '', vcx_name, vcx_path, guid)

# C++ library build

if add_cpp_lib:
  guid = '{' + str(uuid4()) + '}'
  proj_name = 'mpirxx'
  mode = ('Win32', 'x64')
  vcx_name = 'lib_mpir_cxx'
  vcx_path = 'lib_mpir_cxx\\' + vcx_name + '.vcxproj'
  th = hf_list +  ('mpirxx.h',)
  gen_filter(vcx_path + '.filters', th, cc_src_list, '')
  gen_vcxproj(proj_name, vcx_path, guid, config, mode, lib_type, True, th, cc_src_list, '')
  add_proj_to_sln('mpir.sln', '', vcx_name, vcx_path, guid)

# the following code is for diagnostic purposes only
if debug:

  for x in sorted(mpn_f[0] + mpn_f[1]):
    print(x)
  print()
  for x in sorted(mpn_f[2] + mpn_f[3]):
    print(x)
  print()

  # mpn_files = dict()
  # mpn_files.update(mpn_32)
  # mpn_files.update(mpn_64)
  for x in mpn_f[config]:
    print(x)
    if False:
      print('1:')
      for y in mpn_files[x][0]:
        print(y)
      print('2:')
      for y in mpn_files[x][1]:
        print(y)
      print('3:')
      for y in mpn_files[x][2]:
        print(y)
      print('4:')
      for y in mpn_files[x][3]:
        print(y)
      print()
    for y in sorted(x[2] + x[3]):
      print(y)
    print()
    print()

if debug:

  mpn_dirs = ('mpn/generic', 'mpn/x86_64w', 'mpn/x86w')

  # compile a list of files in directories in 'dl' under root 'r' with extension 'p'
  def findf(r, dl, p):
    l = []
    for d in dl:
      for root, dirs, files in walk(r + d):
        relp = relpath(root, r)  # path relative to mpir root directory
        if '.svn' in dirs:
          dirs.remove('.svn')            # ignore SVN directories
        if d == '' or root.endswith(build_vc):
          for d in reversed(dirs):       # don't scan build.vc12 subdirectories
            dirs.remove(d)
        for f in files:
          if f.endswith(p):
            l += [(tuple(relp.split('\\')), f)]
    return sorted(l)

  hdr_list = findf(mpir_dir, c_directories, '.h')
  for x in hdr_list:
    print(x)
  print()

  src_list = findf(mpir_dir, c_directories, '.c')
  for x in src_list:
    print(x)
  print()

  cpp_list = findf(mpir_dir, ['cpp'], '.cc')
  for x in cpp_list:
    print(x)
  print()

  gnc_list = findf(mpir_dir + 'mpn/', ['generic'], '.c')
  for x in gnc_list:
    print(x)
  print()

  w32_list = findf(mpir_dir + 'mpn/', ['x86w'], '.asm')
  for x in w32_list:
    print(x)
  print()

  x64_list = findf(mpir_dir + 'mpn/', ['x86_64w'], '.asm')
  for x in x64_list:
    print(x)
  print()

  nd = dict([])
  for d, f in gnc_list:
    n, x = splitext(f)
    nd[n] = nd.get(n, []) + [(d, 'c')]
  for d, f in x64_list:
    n, x = splitext(f)
    nd[n] = nd.get(n, []) + [(d, 'asm')]
  for d, f in w32_list:
    n, x = splitext(f)
    nd[n] = nd.get(n, []) + [(d, 'asm')]
  for x in nd:
    print(x, nd[x])