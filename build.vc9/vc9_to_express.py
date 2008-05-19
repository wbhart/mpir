
import os
import shutil
import string

def process(sp, dp) :
  fr = open(sp, 'rb')
  fw = open(dp, 'wb')
  
  in_files = False
  in_filter = False
  in_file = False
  in_fconfig = False

  for l in fr :
    
    if l.find('</Filter>') != -1 :
      in_filter = False
      continue
    elif l.find('<Filter') != -1 :
      in_filter = True
      continue
    elif l.find('</FileConfiguration>') != -1 :
      in_fconfig = False
      fw.write(l)
      continue      
    elif l.find('<FileConfiguration') != -1 :
      in_fconfig = True
      fw.write(l)
      continue      
    elif l.find('</Files>') != -1 :
      in_files = False
      fw.write(l)
      continue      
    elif l.find('<Files') != -1 :
      in_files = True
      fw.write(l)
      continue      
    elif l.find('</File>') != -1 :
      in_file = False
      fw.write(l)
      continue      
    elif l.find('<File') != -1 :
      in_file = True
      fw.write(l)
      continue      
      
    if in_files and not in_file :
      continue
    if in_filter and not in_file :
      continue

    if in_file :
      if in_fconfig :
        p = l.find('ObjectFile="$(IntDir)')
        if p != -1 :
          h = -1
          while l[h] == '\r' or l[h] == '\n' or l[h] == '\"' :
            h = h - 1
          l = l[ : h] + '.' + n + '.obj\"\r\n'
      else :
        p = l.find('RelativePath=')
        if p != -1 :
          n = os.path.splitext(os.path.basename(l[p + 22 : -1]))[0]
    fw.write(l)  
  
  fr.close()
  fw.close()
  return

def form_path(p) :
  n = string.rfind(p, '\\', 0, -1)
  if n != -1 :
    if not os.path.exists(p[ : n + 1]) :
      form_path(p[ : n + 1])
  if p[-1] == '\\' and not os.path.exists(p) :
    os.mkdir(p)
  
copy_f = [ '.', '.c', '.vcproj', '.vsprops', '.ps1', '.py' ]
exc_f  = [ '.exe', '.pdb', '.ncb' ]
exc_d  = [ 'win32', 'Win32', 'x64', 'X64' ]

def fd_copy(s, d, l) :
  fd = os.listdir(s)
  for f in fd :
    sp = os.path.join(s,f)
    dp = os.path.join(d,f)
    if os.path.isdir(sp) and f not in exc_d :
      fd_copy(sp, dp, l + 1)
    elif os.path.isfile(sp) :
      fn = os.path.basename(f)
      x = os.path.splitext(fn)
      if l == 0 and x[1] not in exc_f or x[1] in copy_f :
        if x[1] == ".vcproj" :
            if x[0].find('amd64') == -1 :
              form_path(dp)
              process(sp, dp)
        else :
          form_path(dp)
          shutil.copyfile(sp, dp)

cd = os.getcwd()                # it must run in build.vc9 
cd1 = cd + "\\..\\build.vc9"    # the current vc9 build directory
cd2 = cd + "\\..\\build.vc9x"   # the new vc9x build directory
if os.path.exists(cd1) :
  if not os.path.exists(cd2) :
    print "Creating 'build.vc9x' directory structure and "
    print "Converting vcproj files for VC++ Express"
    fd_copy(cd1, cd2, 0)
  else :
    print "The directory 'build.vc9x' already exists"
else :
  print "Cannot find the directory: %s" % cd1
