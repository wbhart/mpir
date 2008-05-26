
# Convert between Visual Studio 2008 and 2005 Project Files
# (with thanks to Tommi Vainikainen)

import os
import shutil
import string
import fileinput, sys

def fd_convert(sp):
  for l in fileinput.input(sp, inplace = 1) :
    p1 = l.find("Version=\"8.00\"")
    p2 = l.find("Version=\"9.00\"")
    if p1 != -1 or p2 != -1 :
      if p1 != -1 :
        l = l[ : p1 + 9] + '9' + l[ p1 + 10 : ]
      else :
        l = l[ : p2 + 9] + '8' + l[ p2 + 10 : ]
    sys.stdout.write(l)

if os.getcwd().endswith("build.vc9") :
  for root, dirs, files in os.walk("./") :
    for file in files :
      if file.endswith(".vcproj") :
        fd_convert(os.path.join(root, file))
else :
  print "This script must be run in the 'build.vc9' directory"
