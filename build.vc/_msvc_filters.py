# generate Visual Studio IDE Filter

from os import makedirs
from os.path import dirname, normpath, join, split, relpath
from errno import EEXIST

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
  f2 = r'''    <ClCompile Include="{}{}">
      <Filter>Source Files</Filter>
    </ClCompile>
'''
  f3 = r'''    <ClCompile Include="{}{}\{}">
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
  f2 = r'''    <YASM Include="{0:s}{2:s}\{1:s}">
      <Filter>Source Files\mpn\yasm</Filter>
    </YASM>
'''
  f3 = r'''  </ItemGroup>
'''
  outf.write(f1)
  for i in af_list:
    outf.write(f2.format(relp, i[0] + i[1], i[2], i[2]))
  outf.write(f3)

def gen_filter(path, root_dir, hf_list, cf_list, af_list, tools_ver):

  f1 = r'''<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="{0}" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
'''.format(tools_ver)

  f2 = r'''  <ItemGroup>
    <None Include="..\..\gmp-h.in" />
  </ItemGroup>
</Project>
'''

  relp = split(relpath(root_dir, path))[0] + '\\'
  try:
    makedirs(split(path)[0])
  except IOError as e:
    if e.errno != EEXIST:
      raise
    else:
      pass

  with open(path, 'w') as outf:

    outf.write(f1)
    filter_folders(cf_list, af_list, outf)
    if hf_list:
      filter_headers(hf_list, relp, outf)
    filter_csrc(cf_list, relp, outf)
    if af_list:
      filter_asrc(af_list, relp, outf)
    outf.write(f2)
