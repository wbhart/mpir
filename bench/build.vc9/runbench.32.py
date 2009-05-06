
# A Python equivalent to gmpbench script 'runbench' for MPIR
#
# Copyright (c) 1998-2008, Brian Gladman, Worcester, UK.
#
# Support for Linux added with help from Case Van Horsen.
#
# This file is part of the MPIR Library.  The MPIR Library is free
# software; you can redistribute it and/or modify it under the terms
# of the GNU Lesser General Public License version 2.1 as published
# by the Free Software Foundation.
# 
# The MPIR Library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.  You should have
# received a copy of the GNU Lesser General Public License along
# with the MPIR Library; see the file COPYING.LIB.  If not, write to
# the Free Software Foundation, Inc., 51Franklin Street, Fifth Floor,
# Boston, MA 02110-1301, USA.
#
# To compile the test applications:
#
#  Windows:
#
#    Use the VC++ mpirbench project
#
#  Linux:
#
#    mkdir bin
#    gcc multiply.c -lmpir -o bin/multiply
#    gcc divide.c -lmpir -o bin/divide
#    gcc rsa.c -lmpir -o bin/rsa
#    gcc mpirver.c -lmpir -o bin/mpirver

from __future__ import print_function

import os
import sys
import string
import platform
from re import match
from subprocess import Popen, PIPE, STDOUT

if sys.platform.startswith('win') :
  dir = 'win32\\release\\'
else:
  dir = 'bin'

def run_exe(exe, args, inp) :
  al = {'stdin' : PIPE, 'stdout' : PIPE, 'stderr' : STDOUT }
  if sys.platform.startswith('win') :
    al['creationflags'] = 0x08000000
  p = Popen([exe] + args, **al)
  res = p.communicate(inp.encode())[0].decode()
  ret = p.poll()
  return (ret, res)

multiply_args = [(128,128),(512,512),(8192,8192),(131072,131072),\
                 (2097152,2097152)]
divide_args   = [(8192,32),(8192,64),(8192,128),(8192,4096),(8192,8064),\
                 (131072,8192),(131072,65536),(8388608,4194304)]
rsa_args      = [[512],[1024],[2048]]

base_tests    = ['multiply', 'divide']
app_tests     = ["rsa"]
tests         = ['base', 'app']

print('Machine:', platform.processor())
print('Running:', platform.platform())
print('Running benchmarks')
acc2 = 1.0
n2   = 0.0
for c in tests :
  print('  Category {0:s}'.format(c))
  acc1 = 1.0
  n1   = 0.0
  for t in globals()[c + '_tests'] :
    print('    Program {0:s}'.format(t))
    acc = 1.0
    n   = 0.0
    for pars in globals()[t + '_args'] :
      params = list(map(str,pars))
      print('      {0:s} {1:s}'.format(t,  ' '.join(params)))
      ss = (run_exe(os.path.join(dir, t), params, '')[1]).split('\n')
      for s in ss :
        if match('RESULT: [0-9]*', s) :
          res = s.split(' ')[1]
      print('      MPIRbench.{0:s}.{1:s}.{2:s} result: {3:s}' \
                              .format(c, t, '.'.join(params), res))
      acc *= eval(res)
      n += 1.0
    v = acc ** (1.0 / n)
    acc1 *= v
    print('    MPIRbench.{0:s}.{1:s} result: {2:.2f}'.format(c, t, v))
    n1 += 1.0
  v = acc1 ** (1.0 / n1)
  acc2 *= v
  print('  MPIRbench.{0:s} result {1:.2f}'.format(c, v))
  n2 += 1.0
print('MPIRbench result: {0:.2f}'.format(acc2 ** (1.0 / n2)))
input(".. completed - press ENTER")
