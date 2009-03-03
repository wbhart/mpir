
from __future__ import print_function

import string, re, os, sys, shutil

# regular expression for registers 

r_q = r"(?:r[abcd]x|r[sd]i|r[bsi]p)|(?:r8|r9|r1[012345])|"
r_d = r"(?:e[abcd]x|e[sd]i|e[bsi]p|r[89]d|r1[012345]d)|"
r_w = r"(?:[abcd]x|[sd]i|[bsi]p|r[89]w|r1[012345]w)|"
r_b = r"(?:[abcd]l|[ds]il|[bsi]pl|r[89]l|r1[012345]l)|"
r_x = r"(?:x?mm\d|x?mm1[0-5])|(?:mmx\d|mmx1[0-5])|(?:st\([0-7]\))"  

p_rg = r"(?:\s*%(" + r_b + r_w + r_d + r_q + r_x + r"))"

# regular expression for dis(r1, r2, mul) forms

p_mu = r"(?:\s*,\s*([1248])){0,1}\s*(?=\))"
p_di = r"(?:\s*([\+\-]{0,1}[0-9]*)(?=\()){0,1}"
p_t1 = p_di + r"\s*\(" + p_rg + r"(?:\s*\)|\s*,*" + p_rg + p_mu + r"\s*\))"

# regular expression for immediate (numeric, not symbolic)

p_im = r"\s+\`{0,1}\$\'{0,1}([\+\-]{0,1}[0-9]+|0x[a-zA-Z0-9]+)"

# regular expression for labels

p_la = r"\s*([a-zA-Z$_][a-zA-Z0-9\$_]*|\.[0-9]+)(:)"
p_lr = r"\s+([a-zA-Z$_][a-zA-Z0-9\$_]*|\.[0-9]+)"

# regular expression for instructions

p_in = r"\s*([a-zA-Z][a-zA-Z0-9]*)"

m_f1 = re.compile(p_in + p_rg + r"\s*," + p_t1)
m_f2 = re.compile(p_in + p_t1 + r"\s*," + p_rg)
m_f3 = re.compile(p_in + p_t1)
m_f4 = re.compile(p_in + p_im + r"\s*," + p_t1)
m_f5 = re.compile(p_in + p_im + r"\s*," + p_rg)
m_f6 = re.compile(p_in + p_rg + r"\s*," + p_rg)
m_f7 = re.compile(p_in + p_rg)
m_f8 = re.compile(p_in + p_im)
m_f9 = re.compile(p_in + p_lr)
m_fa = re.compile(p_in + p_im + r"\s*," + p_rg + r"\s*," + p_rg)

m_la = re.compile(p_la)

r_mac = re.compile(r"^\s*(?:define|DEFINE)\s*\(\s*`"
                   "([A-Z$_][A-Z0-9$_]*)'\s*,\s*`\s*$")
r_mnd = re.compile(r"^\s*'\s*\)\s*$")
r_dlr = re.compile(r"\$([0-9]+)")

r1 = r"\s*(\%{0,1}[0-9]+){0,1}\s*"
r2 = r"(?:,\s*(\%{0,1}[0-9]+))?\s*"
r_mrf = re.compile(r"([A-Z$_][A-Z0-9$_]*)\s*\(" + r1 + r2 +
                  r2 + r2 + r2 + r2 + r2 + r2 + r2 + r2 + r"\)")

def pass_one(code) :
  labels = []
  for l in code :
    m = m_la.search(l)
    if m :
      labels += [m.group(1)]
  return labels

def pass_two(code, labels) :

  mac_name = ''
  mac_dict = {}
  lab_idx = 0
  for ln in range(len(code)) :
    l = code[ln]
    
    m = r_mac.search(l)
    if m and not mac_name :
      mac_name = m.group(1)
      mac_dict[mac_name] = [ln, 0, 0, {}]
      continue

    m = r_mnd.search(l)
    if m and mac_name:
      mac_dict[mac_name][1] = ln
      mac_name = ''
      lab_idx = 0
      continue

    if mac_name :
      m = r_dlr.findall(l)
      if m :
        mn = max(int(i) for i in m)
        if mn > mac_dict[mac_name][2] :
          mac_dict[mac_name][2] = mn
    
      m = m_la.search(l)
      if m and m.group(1) in labels :
        lab_idx += 1
        mac_dict[mac_name][3][m.group(1)] = lab_idx

  return mac_dict

def proc_m(m, i, j) :
  v = list(m.groups())
  n = len(v)
  if n <= i :
    return ([], '')
  ss = (v[i] if not v[i] or v[i][0] in '+-' else '+' + v[i]) + ']'
    
  if n > i + 3 and i + 3 < j and v[i + 3] :
    ss = ('' if not v[i + 3] else '*' + v[i + 3]) + ss
  if n > i + 2 and i + 2 < j and v[i + 2] :
    ss = '+' + v[i + 2] + ss
  if n > i + 1 and i + 1 < j and v[i + 1] :
    ss = v[i + 1] + ss
  return (v[ 0 : i ] + v[ j : ], '[' + ss)

def pass_three(code, labels, macros) :

  lo = []
  debug = False
  dot_labels = False
  mac_name = ''
  for l in code :
    lp = '\n'
    # labels
    m = m_la.search(l)
    if m :
      l = l[ m.end(2) : -1 ]
      if mac_name :
        if m.group(1) in macros[mac_name][3] :
          ii = macros[mac_name][3][m.group(1)]
        else :
          print("internal error")
      else :      
        ii = labels.index(m.group(1))
      lab = re.sub('\$', '%', m.group(1))
      if not l :
        if mac_name :
          lo += ['\n%%{0}:'.format(ii)]          
        elif dot_labels :
          lo += ['\n.{0}:'.format(ii)]
        else :
          lo += ['\n{0}:'.format(lab)]
        continue
      else :
        if mac_name :
          lp = '\n%%{0}:'.format(ii)
        elif dot_labels :
          lp = '\n.{0}:'.format(ii)
        else :
          lp = '\n{0}:'.format(lab)

    m = re.search(r"(\s*)#\s*(.*)", l)
    if m :
      v = list(m.groups())
      lo += [lp + '{0[0]}; {0[1]}'.format(v)]
      continue

    m = re.search(r"dnl(.*)", l)
    if m :
      lo += [lp + ';{0}'.format(m.group(1))]
      continue

    m = m_fa.search(l)
    if m :
      v = list(m.groups())
      if debug :
        print(l, end = '')
      lo += [lp + '\t{0[0]:7s} {0[3]}, {0[2]}, {0[1]}'.format(v)]
      continue

    # ins reg, dis(reg, reg, off)
    m = m_f1.search(l)
    if m :
      if debug :
        print(l, end = '')
      v, s = proc_m(m, 2, 6)
      lo += [lp + '\t{0[0]:7s} {1}, {0[1]}'.format(v, s)]
      continue

    # ins dis(reg, reg, off), reg  
    m = m_f2.search(l)
    if m :
      if debug :
        print(l, end = '')
      v, s = proc_m(m, 1, 5)
      lo += [lp + '\t{0[0]:7s} {0[1]}, {1}'.format(v, s)]
      continue

    # ins dis(reg, reg, off)  
    m = m_f3.search(l)
    if m :
      if debug :
        print(l, end = '')
      v, s = proc_m(m, 1, 5)
      lo += [lp + '\t{0[0]:7s} {1}'.format(v, s)]
      continue

    # ins imm, dis(reg, reg, off)  
    m = m_f4.search(l)
    if m :
      if debug :
        print(l, end = '')
      v, s = proc_m(m, 2, 6)
      lo += [lp + '\t{0[0]:7s} {1}, {0[1]}'.format(v, s)]
      continue

    # ins imm, reg  
    m = m_f5.search(l)
    if m :
      v = list(m.groups())
      if debug :
        print(l, end = '')
      lo += [lp + '\t{0[0]:7s} {0[2]}, {0[1]}'.format(v)]
      continue

    # ins reg, reg  
    m = m_f6.search(l)
    if m :
      v = list(m.groups())
      if debug :
        print(l, end = '')
      lo += [lp + '\t{0[0]:7s} {0[2]}, {0[1]}'.format(v)]
      continue

    # ins reg  
    m = m_f7.search(l)
    if m :
      v = list(m.groups())
      if debug :
        print(l, end = '')
      lo += [lp + '\t{0[0]:7s} {0[1]}'.format(v)]
      continue

    # ins imm  
    m = m_f8.search(l)
    if m :
      v = list(m.groups())
      if debug :
        print(l, end = '')
      lo += [lp + '\t{0[0]:7s} {0[1]}'.format(v)]
      continue

    # jump label  
    m = m_f9.search(l)
    if m :
      v = list(m.groups())
      if v[1] in labels :
        if debug :
          print(l, end = '')
        if mac_name and v[1] in macros[mac_name][3] :
          ii = macros[mac_name][3][v[1]]
          lo += [lp + '\t{0[0]:7s} %%{1}'.format(v, ii)]
          continue
        if dot_labels :
          ii = labels.index(v[1])
          lo += [lp + '\t{0[0]:7s} .{1}'.format(v, ii)]
        else :
          lab = re.sub('\$', '%', v[1])
          lo += [lp + '\t{0[0]:7s} {1}'.format(v, lab)]
        continue

    m = re.search(r"\s*\.byte\s+((?:0x|0X)[0-9a-fA-F]+|[0-9]+)\s*", l)
    if m :
      v = list(m.groups())
      lo += [lp + '\tdb      {0[0]}'.format(v)]
      continue
    
    # macro definitions
    m = r_mac.search(l)
    if m :
      mac_name = m.group(1)
      if mac_name in macros :
        lo += [lp + '%macro ' + mac_name.lower() + ' '
                        + str(macros[mac_name][2])]
        continue
      
    m = r_mnd.search(l)
    if m and mac_name :
      mac_name = ''
      lab_ofs = 0
      lo += [lp + "%endmacro"]
      continue

    m = r_dlr.findall(l)
    if m :
      l = re.sub(r"\$([0-9]+)", r"%\1", l)

    m = re.search("PROLOGUE\(([a-zA-Z$_][a-zA-Z0-9$_]*)\)", l)
    if m :
      lo += [lp + "\twin64_gcc_start {0}".format(m.group(1))]
      continue

    m = re.search("EPILOGUE\(\)", l)
    if m :
      lo += [lp + "\twin64_gcc_end"]
      continue

    # macro calls 
    m = r_mrf.search(l)
    if m :
      if m.group(1).lower() == 'align' :
        lo += [lp + '\talign   {0}'.format(m.group(2))]
        continue
      elif m.group(1) in macros :
        lp += '\t{0:7s}'.format(m.group(1).lower())
        i = 2
        while i <= m.lastindex and m.group(i) :
          lp += ' {0}'.format(m.group(i))
          i += 1
        lo += [lp]
        continue
      elif m.group(1) and not m.group(2) :
        lo += [lp + '\t{0}'.format(m.group(1))]
        continue
      
    if mac_name :
      m = re.search(r"\s*([^%]+)%([0-9]+)\s*", l)
      if m and m.lastindex == 2 and int(m.group(2)) <= macros[mac_name][2] :
        lo += [lp + '\t{0}%{1}'.format(m.group(1).lower(),m.group(2))]
        continue

    # ins  
    m = re.search(p_in + r"\s+(.*)", l)
    if m :
      v = list(m.groups())
      if debug :
        print(l, end = '')
      if len(v) == 1 or len(v) == 2 and v[1] == '' :
        lo += [lp + '\t{0[0]}'.format(v)]
        continue
      elif v[0] == 'C' :
        lo += [lp + ';\t{0}'.format(v[1])]
        continue
 
    m = re.search(r"\s*(\S+)", l)
    if m :
      lo += [lp + '{0} ; < not translated >'.format(l[:-1])]
    else :
      lo += ['']
  return lo

def form_path(p) :
  n = str.rfind(p, '\\', 0, -1)
  if n != -1 :
    if not os.path.exists(p[ : n + 1]) :
      form_path(p[ : n + 1])
  if p[-1] == '\\' and not os.path.exists(p) :
    os.mkdir(p)
    
def convert(s, d, l) :
  fd = os.listdir(s)
  for f in fd :
    sp = os.path.join(s,f)
    dp = os.path.join(d,f)
    if os.path.isdir(sp) :
      convert(sp, dp, l + 1)
    elif os.path.isfile(sp) :
      if sp != dp and os.path.exists(dp) :
        continue
      fn = os.path.basename(f)
      x = os.path.splitext(fn)
      if x[1] == '.asm' :
        form_path(dp)
        print("translating '{0}'".format(sp))
        f = open(sp, "r")
        code = f.readlines()
        f.close()
        if sp == dp :
          rp = os.path.join(s, x[0] + '.old')
          os.rename(sp, rp)
        labels = pass_one(code)
        macros = pass_two(code, labels)
        code = pass_three(code, labels, macros)
        f = open(dp, "w")
        f.writelines(code)
        f.close()
      elif False :
        form_path(dp)
        shutil.copyfile(sp, dp)

cd = os.getcwd()                    # it must run in build.vc9
if cd.endswith("build.vc9") :
  cd1 = cd + "\\..\\mpn\\x86_64"    # the GCC assembler directory
  cd2 = cd + "\\..\\mpn\\x86_64w"   # the YASM (Windows) assembler directory
elif cd.endswith("x86_64") :
  if os.path.exists(cd + "\\..\\x86_64w") :
    cd1 = cd
    cd2 = cd + "\\..\\x86_64w"
  else :
    cd1 = cd2 = cd
else:
  cd1 = cd2 = None

if cd1 and os.path.exists(cd1) :
  if os.path.exists(cd2) :
    print("warning: output directory '{0}' already exists".format(cd2))
  convert(cd1, cd2, 0)            # convert format from GAS to YASM 
elif cd1 :
  print("cannot find input directory: '{0}'".format(cd1))
else :
  convert(cd, cd + "\\convert\\", 0)
  if False :
    # for testing -- translates to directory amd64c rather than amd64
    f = open("..\\mpn\\x86_64\\amd64\\mul_basecase.asm", "r")
    code = f.readlines()
    f.close()
    labels = pass_one(code)
    macros = pass_two(code, labels)
    code = pass_three(code, labels, macros)
    f = open("..\\mpn\\x86_64w\\amd64c\\mul_basecase.asm", "w")
    f.writelines(code)
    f.close()
