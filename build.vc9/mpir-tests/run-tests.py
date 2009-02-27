#
# Python script for running GMP tests 
#
# Run this from the build.vc9\mpir-tests directory

from __future__ import print_function
import os
import shutil
import string
import copy
import subprocess
import code

f = open("..\\last_build.txt")
l = f.readline()
f.close()
d = l[ : -2].split('+')
print("Testing project", d[0][d[0].find('gmp') : - 1], "in", d[1])

prj_list = []
l = os.listdir(os.getcwd())
for f in l :
  x = os.path.splitext(f)
  if x[1] == '.vcproj' and x[0] != 'add-test-lib' :
    prj_list += [x[0]]
prj_list.sort()

exe_list = []
try :
  l = os.listdir(os.getcwd() + '\\' + d[1])
except :
  print("Tests have not been built for this configuration")
  os._exit(-1)

for f in l :
  x = os.path.splitext(f)
  if x[1] == '.exe' :
    exe_list += [x[0]]
exe_list.sort()
if len(exe_list) == 0 :
  print("No executable test files for this configuration")
  os._exit(-1)

build_fail = 0
run_ok = 0
run_fail = 0
for i in prj_list :
  if i in exe_list :
    ef = '.\\' + d[1] + '\\' + i + '.exe'
    prc = subprocess.Popen( ef, stdout = subprocess.PIPE,
      stderr = subprocess.STDOUT, creationflags = 0x08000000 )
    output = prc.communicate()[0]
    if prc.returncode :
      print(i, ': ERROR (', prc.returncode, ' )')
      run_fail += 1
    else :
      print(i, ': success')
      run_ok += 1
    if output :
      print(output, end='')
  else :
    print("Build failure for %s" % i)
    build_fail += 1
print(build_fail + run_ok + run_fail, "tests:")
if build_fail > 0 :
  print("\t%i failed to build" % build_fail)
if run_ok > 0 :
  print("\t%i ran correctly" % run_ok)
if run_fail > 0 :
  print("\t%i failed" % run_fail) 
raw_input(".. completed - press ENTER")
