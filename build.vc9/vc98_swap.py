
import os
import shutil
import string

def convert(sp) :
  f = open(sp, 'r+')
  for l in f :
    p = l.find("Version=\"9.00\"\r\n")
    if p != -1
      l = l[ : l + 9] + '8' + l[ l + 10 : ]
      f.write(l)
    p = l.find("Version=\"8.00\"\r\n")
    if p != -1
      l = l[ : l + 9] + '9' + l[ l + 10 : ]
      f.write(l)
  f.close()
  return

def fd_search(s) :
  fd = os.listdir(s)
  for f in fd :
    sp = os.path.join(s,f)
    if os.path.isdir(sp) :
      fd_convert(sp)
    elif os.path.isfile(sp) :
      fn = os.path.basename(f)
      x = os.path.splitext(fn)
      if x[1] == ".vcproj" :
        convert(sp)
  return

cd = os.getcwd()           # it must run in build.vc9 
cd += "\\..\\build.vc9"    # the current vc9 build directory
if os.path.exists(cd) :
    fd_search(cd)
else :
  print "This script must be run in the 'build.vc9' directory"
