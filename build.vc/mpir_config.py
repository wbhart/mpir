'''
Set up Visual Sudio to build a specified MPIR configuration

Copyright (C) 2011, 2012, 2013, 2014 Brian Gladman
'''

from __future__ import print_function
from operator import itemgetter
from os import listdir, walk, unlink, makedirs, sep
from os.path import split, splitext, isdir, relpath, join, exists
from os.path import join, abspath, dirname, normpath
from copy import deepcopy
from sys import argv, exit, path
from filecmp import cmp
from shutil import copy
from re import compile, search
from collections import defaultdict
from time import sleep

from _msvc_filters import gen_filter
from _msvc_project import Project_Type, gen_vcxproj
from _msvc_solution import msvc_solution

try:
  input = raw_input
except NameError:
  pass

vs_version = 15
if len(argv) > 1:
  vs_version = int(argv[1])

solution_name = 'mpir.sln'
script_dir = dirname(__file__)
build_dir_name = 'build.vc{0:d}'.format(vs_version)
path.append(abspath(join(script_dir, '../' + build_dir_name)))
from version_info import vs_info

# for script debugging
debug = False
# either add a prebuild step to the project files or do it here
add_prebuild = True
# output a build project for the C++ static library
add_cpp_lib = True

# The path to the mpir root directory
cf_dir = './'
mpir_root_dir = '../'
build_dir = join(mpir_root_dir, build_dir_name)
solution_dir = join(mpir_root_dir, build_dir_name)
cfg_dir = join(solution_dir, 'cdata')

# paths that might include source files(*.c, *.h, *.asm)
c_directories = ('', 'build.vc', 'fft', 'mpf', 'mpq', 'mpz', 'printf', 'scanf')

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
    relr = relpath(root, mpir_root_dir)      # path from MPIR root
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
    for f in listdir(join(mpir_root_dir, d)):
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
    fn = join(mpir_root_dir, f[2], f[0] + f[1])
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
mpn_32 = find_asm(mpir_root_dir + 'mpn/x86w', gc_src_list)
syms32 = file_symbols(mpn_32)
del mpn_32['']

# prepare the list of x64 builds
mpn_64 = find_asm(mpir_root_dir + 'mpn/x86_64w', gc_src_list)
syms64 = file_symbols(mpn_64)
del mpn_64['']

nd_gc = len(mpn_gc)
nd_32 = nd_gc + len(mpn_32)
nd_nd = nd_32 + len(mpn_64)

# now ask user which builds they wish to generate

while True:
  cnt = 0
  for v in sorted(mpn_gc):
    cnt += 1
    print('{0:2d}. {1:24s}        '.format(cnt, v.replace('\\', '_')))
  for v in sorted(mpn_32):
    cnt += 1
    print('{0:2d}. {1:24s} (win32)'.format(cnt, v.replace('\\', '_')))
  for v in sorted(mpn_64):
    cnt += 1
    print('{0:2d}. {1:24s}   (x64)'.format(cnt, v.replace('\\', '_')))
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

# now generate the requested builds

# input any existing projects in the solution (*.sln) file
solc = msvc_solution(abspath(join(solution_dir, solution_name)))
hf_list = ('config.h', 'gmp-impl.h', 'longlong.h', 'mpir.h', 'gmp-mparam.h')
for n in n_list:

  if 0 < n <= nd_gc:
    config = sorted(mpn_gc)[n - 1]
    mode = ('Win32', 'x64')
    mpn_f = mpn_gc[config]
  elif nd_gc < n <= nd_32:
    config = sorted(mpn_32)[n - 1 - nd_gc]
    gen_have_list(mpn_32[config], syms32, cfg_dir)
    mode = ('Win32', )
    mpn_f = mpn_32[config]
  elif nd_32 < n <= nd_nd:
    config = sorted(mpn_64)[n - 1 - nd_32]
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
      lines = open(join(mpir_root_dir, 'gmp-h.in'), 'r').readlines()
    except IOError:
      print('error attempting to read from gmp_h.in')
      exit()
    try:
      tfile = join(mpir_root_dir, 'tmp.h')
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
      write_f(tfile, join(mpir_root_dir, 'mpir.h'))
      write_f(tfile, join(mpir_root_dir, 'gmp.h'))
      unlink(tfile)
    except IOError:
      print('error attempting to create mpir.h from gmp-h.in')
      exit()

    # generate config.h

    try:
      tfile = join(mpir_root_dir, 'tmp.h')

      if 5 < len(mpn_f) < 8:
        if len(mpn_f) == 6:
          t = sorted(mpn_f[5])
        else:
          t = sorted(mpn_f[5] + mpn_f[6])
        with open(tfile, 'w') as outf:
          for i in t:
            outf.writelines(['#define HAVE_NATIVE_{0:s} 1\n'.format(i)])

      append_f(join(cf_dir, 'cfg.h'), tfile)
      write_f(tfile, join(mpir_root_dir, 'config.h'))
      unlink(tfile)
    except IOError:
      print('error attempting to write to {0:s}'.format(tfile))
      exit()

    # generate longlong.h and copy gmp-mparam.h

    try:
      li_file = None
      for i in mpn_f[0]:
        if i[0] == 'longlong_inc':
          li_file = join(mpir_root_dir, join(i[2], r'longlong_inc.h'))
        if i[0] == 'gmp-mparam':
          write_f(join(mpir_root_dir, join(i[2], 'gmp-mparam.h')),
                  join(mpir_root_dir, 'gmp-mparam.h'))

      if not li_file or not exists(li_file):
        print('error attempting to read {0:s}'.format(li_file))
        exit()

      tfile = join(mpir_root_dir, 'tmp.h')
      write_f(join(mpir_root_dir, 'longlong_pre.h'), tfile)
      append_f(li_file, tfile)
      append_f(join(mpir_root_dir, 'longlong_post.h'), tfile)
      write_f(tfile, join(mpir_root_dir, 'longlong.h'))
      unlink(tfile)
    except IOError:
      print('error attempting to generate longlong.h')
      exit()

  # generate the vcxproj and the IDE filter files
  # and add/replace project in the solution file

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

  cf = config.replace('\\', '_')

  # set up DLL build
  proj_name = 'mpir'
  vcx_name = 'dll_mpir_' + cf
  vcx_path = abspath(join(build_dir, vcx_name, vcx_name + '.vcxproj'))
  guid = solc.get_project_guid(vcx_name, vcx_path)
  gen_filter(vcx_path + '.filters', mpir_root_dir, hf_list, c_src_list + cc_src_list + mpn_f[1], af_list, 12.0)
  gen_vcxproj(vcx_path, mpir_root_dir, proj_name, guid, mp_dir, mode, Project_Type.DLL,
              False, hf_list, c_src_list + cc_src_list + mpn_f[1], af_list, add_prebuild, vs_info)
  solc.add_project('', vcx_name, vcx_path, guid)

  # set up LIB build
  proj_name = 'mpir'
  vcx_name = 'lib_mpir_' + cf
  vcx_path = abspath(join(build_dir, vcx_name, vcx_name + '.vcxproj'))
  guid = solc.get_project_guid(vcx_name, vcx_path)
  gen_filter(vcx_path + '.filters', mpir_root_dir, hf_list, c_src_list + mpn_f[1], af_list, 12.0)
  gen_vcxproj(vcx_path, mpir_root_dir, proj_name, guid, mp_dir, mode, Project_Type.LIB,
              False, hf_list, c_src_list + mpn_f[1], af_list, add_prebuild, vs_info)
  solc.add_project('', vcx_name, vcx_path, guid)

# C++ library build

if add_cpp_lib:
  proj_name = 'mpirxx'
  mode = ('Win32', 'x64')
  vcx_name = 'lib_mpir_cxx'
  vcx_path = abspath(join(build_dir, vcx_name, vcx_name + '.vcxproj'))
  th = hf_list +  ('mpirxx.h',)
  guid = solc.get_project_guid(vcx_name, vcx_path)
  gen_filter(vcx_path + '.filters', mpir_root_dir, th, cc_src_list, '', 12.0)
  gen_vcxproj(vcx_path, mpir_root_dir, proj_name, guid, '', mode, Project_Type.LIB,
              True, th, cc_src_list, '', add_prebuild, vs_info)
  solc.add_project('', vcx_name, vcx_path, guid)

solc.write_solution(vs_info)

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
          for d in reversed(dirs):       # don't scan build.vc<nn> subdirectories
            dirs.remove(d)
        for f in files:
          if f.endswith(p):
            l += [(tuple(relp.split('\\')), f)]
    return sorted(l)

  hdr_list = findf(mpir_root_dir, c_directories, '.h')
  for x in hdr_list:
    print(x)
  print()

  src_list = findf(mpir_root_dir, c_directories, '.c')
  for x in src_list:
    print(x)
  print()

  cpp_list = findf(mpir_root_dir, ['cpp'], '.cc')
  for x in cpp_list:
    print(x)
  print()

  gnc_list = findf(mpir_root_dir + 'mpn/', ['generic'], '.c')
  for x in gnc_list:
    print(x)
  print()

  w32_list = findf(mpir_root_dir + 'mpn/', ['x86w'], '.asm')
  for x in w32_list:
    print(x)
  print()

  x64_list = findf(mpir_root_dir + 'mpn/', ['x86_64w'], '.asm')
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