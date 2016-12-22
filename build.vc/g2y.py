'''
---------------------------------------------------------------------------
Copyright (c) 1998-2008, Brian Gladman, Worcester, UK. All rights reserved.

LICENSE TERMS

The redistribution and use of this software (with or without changes)
is allowed without the payment of fees or royalties provided that:

 1. source code distributions include the above copyright notice, this
    list of conditions and the following disclaimer;

 2. binary distributions include the above copyright notice, this list
    of conditions and the following disclaimer in their documentation;

 3. the name of the copyright holder is not used to endorse products
    built using this software without specific written permission.

DISCLAIMER

This software is provided 'as is' with no explicit or implied warranties
in respect of its properties, including, but not limited to, correctness
and/or fitness for purpose.
---------------------------------------------------------------------------
'''

from __future__ import print_function

import string, re, os, sys, shutil

debug = False
dot_labels = True
remove_comment_lines = True
is_linux = False

# translate 64 bit registers to 8 bit form

r08 = { 'rax' :   'al', 'rbx' :   'bl', 'rcx' :   'cl', 'rdx' :   'dl',
        'rsi' :  'sil', 'rdi' :  'dil', 'rbp' :  'bpl',
         'r8' :  'r8b',  'r9' :  'r9b', 'r10' : 'r10b', 'r11' : 'r11b',
        'r12' : 'r12b', 'r13' : 'r13b', 'r14' : 'r14b', 'r15' : 'r15b' }

# translate 64 bit registers to 32 bit form

r32 = { 'rax' :  'eax', 'rbx' :  'ebx', 'rcx' :  'ecx', 'rdx' :  'edx',
        'rsi' :  'esi', 'rdi' :  'edi', 'rbp' :  'ebp',
         'r8' :  'r8d',  'r9' :  'r9d', 'r10' : 'r10d', 'r11' : 'r11d',
        'r12' : 'r12d', 'r13' : 'r13d', 'r14' : 'r14d', 'r15' : 'r15d' }

# regular expression for registers

r_q = r'(?:r[abcd]x|r[sd]i|r[bsi]p)|(?:r8|r9|r1[0-5])|'  # 64 bit
r_d = r'(?:e[abcd]x|e[sd]i|e[bsi]p|r[89]d|r1[0-5]d)|'    # 32 bit
r_w = r'(?:[abcd]x|[sd]i|[bsi]p|r[89]w|r1[0-5]w)|'       # 16 bit
r_b = r'(?:[abcd]l|[ds]il|[bsi]pl|r[89]b|r1[0-5]b)|'     #  8 bit
r_x = r'(?:x?mm1[0-5]|x?mm\d)|(?:mmx1[0-5]|mmx\d)|(?:st\([0-7]\))'

p_r1 = r'(?:\s*%(?P<reg1>' + r_b + r_w + r_d + r_q + r_x + r'))'
p_r2 = r'(?:\s*%(?P<reg2>' + r_b + r_w + r_d + r_q + r_x + r'))'
p_r3 = r'(?:\s*%(?P<reg3>' + r_b + r_w + r_d + r_q + r_x + r'))'
p_r4 = r'(?:\s*%(?P<reg4>' + r_b + r_w + r_d + r_q + r_x + r'))'

# regular expression for immediate (numeric, not symbolic)

p_im = r'\s+\`?\$\'?(?P<imm>[\+\-]?[0-9]+|0x[a-zA-Z0-9]+)'

# regular expressions for labels

p_l1 = r'(?P<lab1>[a-zA-Z$_][a-zA-Z0-9\$_]*|\.[0-9]+)'
p_l2 = r'L\((?P<lab2>[a-zA-Z0-9\$_]+)\)'
p_l3 = r'(?P<lab3>[a-zA-Z$_][a-zA-Z0-9\$_]*|\.[0-9]+)'
p_l4 = r'L\((?P<lab4>[a-zA-Z0-9\$_]+)\)'

p_lr = r'\s*(?:' + p_l2 + r'|' + p_l1 + r')'
p_la = r'\s*(?:' + p_l2 + r'|' + p_l1 + r')\s*(:)'
p_ri = p_lr + r'\(\%rip\)'
p_34 = r'\s*(?:' + p_l4 + r'|' + p_l3 + r')'
p_jt = r'\s+\.long\s+' + p_lr + r'\s*\-\s*' + p_34

# regular expression for dis(r1, r2, mul) forms

p_di = r'(?:(?:\s*(?P<dis>[-+]?[0-9\+\-]*)(?=\())?' + '|' + p_lr + ')' # numeric displacement
p_mu = r'(?:\s*,\s*(?P<mul>[1248]))?\s*(?=\))'   # multiplier (1, 2, 4, 8)
p_t1 = p_di + r'\s*\(' + p_r1 + r'(?:\s*\)|\s*,' + p_r2 + p_mu + r'\s*\))'

# regular expression for instructions

p_in = r'\s*(?P<ins>[a-zA-Z][a-zA-Z0-9]*)'

p_def = r'define\s*\(\s*\`([a-zA-Z_][a-zA-Z0-9_]*)\'\s*\,\s*\`' + p_r1 + '\'\s*\)'

m_f1 = re.compile(p_in + p_r3 + r'\s*,' + p_t1)
m_f2 = re.compile(p_in + p_t1 + r'\s*,' + p_r3)
m_f3 = re.compile(p_in + p_t1)
m_f4 = re.compile(p_in + p_im + r'\s*,' + p_t1)
m_f5 = re.compile(p_in + p_im + r'\s*,' + p_r1)
m_f6 = re.compile(p_in + p_r1 + r'\s*,' + p_r2)
m_f7 = re.compile(p_in + p_r1)
m_g7 = re.compile(p_in + r'\s+\*' + p_r1)
m_f8 = re.compile(p_in + p_im)
m_f9 = re.compile(p_in + p_lr)
m_fa = re.compile(p_in + '(?:' + p_im + '|' + p_r1 + r')\s*,' + p_r2 + r'\s*,' + p_r3)
m_fb = re.compile(p_in + p_t1 +  r'\s*,' + p_r3 +  r'\s*,' + p_r4)

m_la = re.compile(p_la)
m_jt = re.compile(p_jt)
m_ri = re.compile(p_in + p_ri)

r_mac = re.compile(r"^\s*(?:define|DEFINE)\s*\(\s*`"
                   "([A-Z$_][A-Z0-9$_]*)'\s*,\s*`\s*$")
r_mnd = re.compile(r"^\s*'\s*\)\s*$")
r_dlr = re.compile(r'\$([0-9]+)')
r_def = re.compile(p_def)

r1 = r'\s*(\%{0,1}[0-9]+){0,1}\s*'
r2 = r'(?:,\s*(\%{0,1}[0-9]+))?\s*'
r_mrz = re.compile(r'([A-Z$_][A-Z0-9$_]*)\s*$')
r_mrf = re.compile(r'([A-Z$_][A-Z0-9$_]*)\s*\(' + r1 + r2 +
                  r2 + r2 + r2 + r2 + r2 + r2 + r2 + r2 + r'\)')

def lab_ref(v, labels, macros, mac_name):
  if mac_name and v in macros[mac_name][3]:
    return '%%{0}'.format(macros[mac_name][3][v])
  elif dot_labels:
    return '.{0}'.format(labels.index(v))
  else:
    return 'L_{0}'.format(re.sub('\$', '%', v))

def pass_one(code):
  labels = []
  for l in code:
    m = re.match(r'(\s*)(?:C|#|;|dnl)\s*(.*)', l)
    if m:
      v = list(m.groups())
      continue
    m = m_la.match(l)
    if m:
      lb = m.group(1) if m.group(1) else m.group(2)
      if lb:
        labels += [lb]
  return labels

def pass_two(code, labels):

  mac_name = ''
  mac_dict = {}
  lab_idx = 0
  for ln in range(len(code)):
    l = code[ln]

    m = r_mac.search(l)
    if m and not mac_name:
      mac_name = m.group(1)
      mac_dict[mac_name] = [ln, 0, 0, {}]
      continue

    m = r_mnd.search(l)
    if m and mac_name:
      mac_dict[mac_name][1] = ln
      mac_name = ''
      lab_idx = 0
      continue

    if mac_name:
      m = r_dlr.findall(l)
      if m:
        mn = max(int(i) for i in m)
        if mn > mac_dict[mac_name][2]:
          mac_dict[mac_name][2] = mn

      m = m_la.match(l)
      if m:
        lb = m.group(1) if m.group(1) else m.group(2)
        if lb in labels:
          lab_idx += 1
          mac_dict[mac_name][3][lb] = lab_idx

  return mac_dict

def addr(dd, labels, macros, mac_name):
  pfx = '['
  ss = ']'
  if dd['lab1'] or dd['lab2']:
    lb = dd['lab1'] if dd['lab1'] else dd['lab2']
    if lb in labels:
      ss = lab_ref(lb, labels, macros, mac_name) + ']'
      pfx = '[rel '
      if dd['reg1'] == 'rip':
        return pfx + ss
      ss = '+' + ss
  elif dd['dis']:
    ss = (dd['dis'] if not dd['dis'] or dd['dis'][0] in '+-'
            else '+' + dd['dis']) + ']'
  ss = ('' if not dd['mul'] else '*' + dd['mul']) + ss
  if dd['reg2']:
    ss = '+' + dd['reg2'] + ss
  if dd['reg1']:
    ss = dd['reg1'] + ss
  return pfx + ss

def pass_three(code, labels, macros, level):

  lo = []
  mac_name = ''
  for l in code:

    m = re.match(r'(\s*)(?:C|#|;|dnl)\s*(.*)', l)
    if m:
      v = list(m.groups())
      if not remove_comment_lines:
        lo += ['{0[0]}; {0[1]}'.format(v)]
      continue

    m = re.search(r'(.*)\s+#(.*)$', l)
    sfx = ''
    if m:
      if m.group(1):
        l = m.group(1) + ' '
      else:
        continue
      if m.group(2):
        sfx = '\t;' + m.group(2)

    # labels
    lp = ''
    m = m_la.match(l)
    if m:
      lb = m.group(1) if m.group(1) else m.group(2)
      l = l[ l.find(':') + 1 : -1 ]
      if mac_name:
        if lb in macros[mac_name][3]:
          ii = macros[mac_name][3][lb]
        else:
          print('internal error')
      else:
        ii = labels.index(lb)
      if lb:
        lb = re.sub('\$', '%', lb)
        if not l:
          if mac_name:
            lo += ['\n%%{0}:'.format(ii)]
          elif dot_labels:
            lo += ['\n.{0}:'.format(ii)]
          else:
            lo += ['\nL_{0}:'.format(lb)]
            continue
        else:
          if mac_name:
            lp = '\n%%{0}:'.format(ii)
          elif dot_labels:
            lp = '\n.{0}:'.format(ii)
          else:
            lp = '\nL_{0}:'.format(lb)

    if not re.search(r'(\S+)', l):
      if lp:
        lo += [lp]
        continue
    elif not lp:
      lp = '\n'

    # three operand instructions
    m = m_fa.search(l)
    if m:
      d = m.groupdict()
      if debug:
        print(l, end = '')
      e = d['imm'] if d['imm'] else d['reg1']
      lo += [lp + '\t{0[ins]:7s} {0[reg3]}, {0[reg2]}, {1}'.format(d, e)]
      continue
    
    m = m_fb.search(l)
    if m:
      d = m.groupdict()
      if debug:
        print(l, end = '')
      s = addr(d, labels, macros, mac_name)
      lo += [lp + '\t{0[ins]:7s} {0[reg4]}, {0[reg3]}, {1}'.format(d, s)]
      continue

    # ins reg, dis(reg, reg, off)
    m = m_f1.search(l)
    if m:
      if debug:
        print(l, end = '')
      d = m.groupdict()
      s = addr(d, labels, macros, mac_name)
      lo += [lp + '\t{0[ins]:7s} {1}, {0[reg3]}'.format(d, s)]
      continue

    # ins dis(reg, reg, off), reg
    m = m_f2.search(l)
    if m:
      if debug:
        print(l, end = '')
      d = m.groupdict()
      s = addr(d, labels, macros, mac_name)
      lo += [lp + '\t{0[ins]:7s} {0[reg3]}, {1}'.format(d, s)]
      continue

    # ins dis(reg, reg, off)
    m = m_f3.search(l)
    if m:
      if debug:
        print(l, end = '')
      d = m.groupdict()
      s = addr(d, labels, macros, mac_name)
      lo += [lp + '\t{0[ins]:7s} {1}'.format(d, s)]
      continue

    # ins imm, dis(reg, reg, off)
    m = m_f4.search(l)
    if m:
      if debug:
        print(l, end = '')
      d = m.groupdict()
      s = addr(d, labels, macros, mac_name)
      lo += [lp + '\t{0[ins]:7s} {1}, {0[imm]}'.format(d, s)]
      continue

    # ins imm, reg
    m = m_f5.search(l)
    if m:
      d = m.groupdict()
      if debug:
        print(l, end = '')
      lo += [lp + '\t{0[ins]:7s} {0[reg1]}, {0[imm]}'.format(d)]
      continue

    # ins reg, reg
    m = m_f6.search(l)
    if m:
      d = m.groupdict()
      if debug:
        print(l, end = '')
      lo += [lp + '\t{0[ins]:7s} {0[reg2]}, {0[reg1]}'.format(d)]
      continue

    # ins reg
    m = m_f7.search(l)
    if m:
      d = m.groupdict()
      if debug:
        print(l, end = '')
      lo += [lp + '\t{0[ins]:7s} {0[reg1]}'.format(d)]
      continue

    # ins *reg
    m = m_g7.search(l)
    if m:
      d = m.groupdict()
      if debug:
        print(l, end = '')
      lo += [lp + '\t{0[ins]:7s} {0[reg1]}'.format(d)]
      continue

    # ins imm
    m = m_f8.search(l)
    if m:
      d = m.groupdict()
      if debug:
        print(l, end = '')
      lo += [lp + '\t{0[ins]:7s} {0[imm]}'.format(d)]
      continue

    # jump table
    m = m_jt.search(l)
    if m:
      d = m.groupdict()
      lb0 = d['lab1'] if d['lab1'] else d['lab2']
      lb1 = d['lab3'] if d['lab3'] else d['lab4']
      if lb0 in labels and lb1 in labels:
        if debug:
          print(l, end = '')
        st1 = lab_ref(lb0, labels, macros, mac_name)
        st2 = lab_ref(lb1, labels, macros, mac_name)
        lo += [lp + '\tdd     ' + st1 + ' - ' + st2]
        continue

    # jump label
    m = m_f9.search(l)
    if m:
      d = m.groupdict()
      lb = d['lab1'] if d['lab1'] else d['lab2']
      if lb and lb in labels:
        if debug:
          print(l, end = '')
        ss = lab_ref(lb, labels, macros, mac_name)
        lo += [lp + '\t{0[ins]:7s} {1}'.format(d, ss)]
        continue

    m = re.search(r'\s*\.byte\s+((?:0x|0X)[0-9a-fA-F]+|[0-9]+)\s*', l)
    if m:
      v = list(m.groups())
      lo += [lp + '\tdb      {0[0]}'.format(v)]
      continue

    # macro definitions
    m = r_mac.search(l)
    if m:
      mac_name = m.group(1)
      if mac_name in macros:
        lo += [lp + '%macro ' + mac_name.lower() + ' '
                        + str(macros[mac_name][2])]
        continue

    m = r_mnd.search(l)
    if m and mac_name:
      mac_name = ''
      lab_ofs = 0
      lo += [lp + '%endmacro']
      continue

    m = r_dlr.findall(l)
    if m:
      l = re.sub(r'\$([0-9]+)', r'%\1', l)

    m = re.search('PROLOGUE\(([a-zA-Z$_][a-zA-Z0-9$_]*)\)', l)
    if m:
      if is_linux:
        lo += [lp + '\tGLOBAL_FUNC {0}'.format(m.group(1))]
      else:
        lo += [lp + '\tWIN64_GCC_PROC {0}'.format(m.group(1))]
      continue

    m = re.search('EPILOGUE\(\)', l)
    if m:
      if is_linux:
        lo += [lp + '\tend']
      else:
        lo += [lp + '\tWIN64_GCC_END']
      continue

    # macro calls
    m = r_mrz.search(l)
    if not m:
      m = r_mrf.search(l)
    if m:
      if m.group(1).lower() == 'align':
        fs = '\talign   {0}' if is_linux else '\txalign  {0}'
        lo += [lp + fs.format(m.group(2))]
        continue
      elif m.group(1) in macros:
        lp += '\t{0:7s}'.format(m.group(1).lower())
        i = 2
        while i <= m.lastindex and m.group(i):
          lp += ' {0}'.format(m.group(i))
          i += 1
        lo += [lp]
        continue
      elif m.group(1) and (m.lastindex == 1 or not m.group(2)):
        lo += [lp + '\t{0}'.format(m.group(1))]
        continue

    if mac_name:
      m = re.search(r'\s*([^%]+)%([0-9]+)\s*', l)
      if m and m.lastindex == 2 and int(m.group(2)) <= macros[mac_name][2]:
        lo += [lp + '\t{0}%{1}'.format(m.group(1).lower(),m.group(2))]
        continue

    # ins
    m = re.search(p_in + r'\s+(.*)', l)
    if m:
      v = list(m.groups())
      if debug:
        print(l, end = '')
      if len(v) == 1 or len(v) == 2 and v[1] == '':
        if v[0] in macros:
          lo += [lp + '\t{0}'.format(v[0].lower())]
        else:
          lo += [lp + '\t{0[0]}'.format(v)]
        continue
      elif v[0] == 'C':
        lo += [lp + ';\t{0}'.format(v[1])]
        continue

    m = re.search(r'include\(.+config.m4.+\)', l)
    if m:
      if is_linux:
        lo += [lp + "%include 'yasm_mac.inc'"]
      else:
        lo += [lp + "%include '"
               + ''.join(['..\\'] * level) + "yasm_mac.inc'"]
      continue

    m = re.search(r'\s*(ret\b.*)', l)
    if m:
      lo += [lp + '{0}'.format(l.rstrip(string.whitespace))]
      continue

    m = re.search(r'\s*\.(align\s+[0-9]*)', l)
    if m:
      lo += [lp + '\t{0}'.format(m.group(1))]
      continue
    
    m = re.search(r'\s*(\S+)', l)
    if m:
      if len(l):
        lo += [lp + '{0} ; < not translated >'
               .format(l.rstrip(string.whitespace))]
      else:
        lo += [lp]
    elif lp:
      lo += [lp]

  return lo + ['\n']

def form_path(p):
  n = str.rfind(p, '\\', 0, -1)
  if n != -1:
    if not os.path.exists(p[ : n + 1]):
      form_path(p[ : n + 1])
  if p[-1] == '\\' and not os.path.exists(p):
    os.mkdir(p)

def conv_lines(code, l):
  labels = pass_one(code)
  macros = pass_two(code, labels)
  code = pass_three(code, labels, macros, l)
  return (labels, macros, code)

def conv_file(f_in, f_out, l):
  f = open(f_in, 'r')
  code = f.readlines()
  f.close()
  labels, macros, code = conv_lines(code, l)
  f = open(f_out, 'w')
  f.writelines(code)
  f.close()

def conv_dirs(s, d, l):
  fd = os.listdir(s)
  for f in fd:
    sp = os.path.join(s,f)
    dp = os.path.join(d,f)
    if os.path.isdir(sp):
      conv_dirs(sp, dp, l + 1)
    elif os.path.isfile(sp):
      if sp != dp and os.path.exists(dp):
        continue
      fn = os.path.basename(f)
      x = os.path.splitext(fn)
      if x[1] == '.asm':
        form_path(dp)
        print("translating '{0}'".format(sp))
        f = open(sp, 'r')
        code = f.readlines()
        f.close()
        if sp == dp:
          rp = os.path.join(s, x[0] + ('.as' if is_linux else '.old'))
          os.rename(sp, rp)
        labels, macros, code = conv_lines(code, l)
        f = open(dp, 'w')
        f.writelines(code)
        f.close()
      elif False:
        form_path(dp)
        shutil.copyfile(sp, dp)

if len(sys.argv) == 1:
  cd = os.getcwd()                   # if run in the build.vc10 directory
  if cd.endswith('build.vc10'):
    cd1 = cd + '\\..\\mpn\\x86_64'   # the GAS assembler directory
    cd2 = cd + '\\..\\mpn\\x86_64w'  # the YASM (Windows) assembler directory
  elif cd.endswith('x86_64'):        # if run in the GAS assembler directory
    cd1 = cd
    cd2 = cd + '\\..\\x86_64w'
  elif cd.endswith('x86_64w'):       # if run in the YASM assembler directory
    cd2 = cd
    cd1 = cd + '\\..\\x86_64'
  else:
    print("cannot locate assembler source directory")
    sys.exit(-1)
elif len(sys.argv) == 3:
  cd1 = sys.argv[1]
  cd2 = sys.argv[2]
else:
  print("invalid program location or input")
  sys.exit(-1)

if cd1 and os.path.exists(cd1):
  if os.path.isdir(cd1):
    if os.path.exists(cd2) and os.path.isdir(cd2):
      print("warning: output directory '{0}' already exists".format(cd2))
    if cd1 != cd2:
      conv_dirs(cd1, cd2, 0)         # convert format from GAS to YASM
  elif os.path.isfile(cd1):
    if not os.path.exists(cd2):
      conv_file(cd1, cd2, 0)
else:
  print('{0} is not a file or directory'.format(cd1))
