
# A Python program to run speed and evaluate the performance of MPIR
# routines.
#
# Copyright (c) 2009, Brian Gladman, Worcester, UK.
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

from __future__ import print_function
import sys
import os
import shutil
import string
import copy
import code
import math
import platform
from subprocess import Popen, PIPE, STDOUT

if sys.platform.startswith('win'):
  dir = '.\\x64\\release\\'
else :
  dir = './'

ll = [
     '-c -s 10(10)1000 noop',
     '-c -s 10(10)1000 noop_wxs',
     '-c -s 10(10)1000 noop_wxys',
     '-c -s 10(10)1000 mpn_add_n',
     '-c -s 10(10)1000 mpn_sub_n',
     '-c -s 10(10)1000 mpn_addadd_n',
     '-c -s 10(10)1000 mpn_subadd_n',
     '-c -s 10(10)1000 mpn_addsub_n',
     '-c -s 10(10)1000 mpn_karaadd',
     '-c -s 10(10)1000 mpn_karasub',
     '-c -s 10(10)1000 mpn_addmul_1.3333',
     '-c -s 10(10)1000 mpn_submul_1.3333',
     '-c -s 10(10)1000 mpn_submul_2',
     '-c -s 10(10)1000 mpn_mul_1.3333',
     '-c -s 10(10)1000 mpn_mul_1_inplace.3333',
     '-c -s 10(10)1000 mpn_mul_2',
      '-c -s 10(10)1000 mpn_divrem_euclidean_qr_1.3333',
      '-c -s 10(10)1000 mpn_divrem_euclidean_qr_2',
      '-c -s 10(10)1000 mpn_divrem_euclidean_r_1.3333',
      '-c -s 10(10)1000 mpn_divrem_hensel_qr_1.3333',
      '-c -s 10(10)1000 mpn_divrem_hensel_qr_1_1.3333',
      '-c -s 10(10)1000 mpn_divrem_hensel_qr_1_2.3333',
      '-c -s 10(10)1000 mpn_divrem_hensel_r_1.3333',
      '-c -s 10(10)1000 mpn_rsh_divrem_hensel_qr_1.3333',
      '-c -s 10(10)1000 mpn_rsh_divrem_hensel_qr_1_1.3333',
      '-c -s 10(10)1000 mpn_rsh_divrem_hensel_qr_1_2.3333',
      '-c -s 10(10)1000 mpn_divrem_hensel_rsh_qr_1.3333',
      '-c -s 10(10)1000 mpn_divrem_1.3333',
      '-c -s 10(10)1000 mpn_divrem_1f.3333',
      '-c -s 10(10)1000 mpn_mod_1.3333',
      '-c -s 10(10)1000 mpn_mod_1_1',
      '-c -s 10(10)1000 mpn_mod_1_2',
      '-c -s 10(10)1000 mpn_mod_1_3',
      '-c -s 10(10)1000 mpn_mod_1_k.3',
      '-c -s 10(10)1000 mpn_preinv_divrem_1.3333',
      '-c -s 10(10)1000 mpn_preinv_divrem_1f.3333',
      '-c -s 10(10)1000 mpn_preinv_mod_1.3333',
      '-c -s 10(10)1000 mpn_add_err1_n',
      '-c -s 10(10)1000 mpn_sub_err1_n',
      '-c -s 10(10)1000 mpn_inv_divappr_q',
      '-c -s 10(10)1000 mpn_inv_div_qr',
      '-c -s 10(10)1000 mpn_dc_divappr_q',
      '-c -s 10(10)1000 mpn_dc_div_qr_n',
      '-c -s 10(10)1000 mpn_divrem_1_inv.3333',
      '-c -s 10(10)1000 mpn_divrem_1f_div.3333',
      '-c -s 10(10)1000 mpn_divrem_1f_inv.3333',
      '-c -s 10(10)1000 mpn_mod_1_div.3333',
      '-c -s 10(10)1000 mpn_mod_1_inv.3333',
      '-c -s 10(10)1000 mpn_divrem_2',
      '-c -s 10(10)1000 mpn_divrem_2_div',
      '-c -s 10(10)1000 mpn_divrem_2_inv',
      '-c -s 10(10)1000 mpn_divexact_1.3333',
      '-c -s 10(10)1000 mpn_divexact_by3',
      '-c -s 10(10)1000 mpn_divexact_byff',
      '-c -s 10(10)1000 mpn_divexact_byfobm1.3333',
      '-c -s 10(10)1000 mpn_modexact_1_odd.333',
      '-c -s 10(10)1000 mpn_modexact_1c_odd.333',
      '-c -s 10(10)1000 mpn_mod_34lsub1',
      '-c -s 10(10)1000 mpn_dc_tdiv_qr',
      '-c -s 10(10)1000 mpn_lshift.33',
      '-c -s 10(10)1000 mpn_rshift.33',
      '-c -s 10(10)1000 mpn_lshift1',
      '-c -s 10(10)1000 mpn_rshift1',
      '-c -s 10(10)1000 mpn_double',
      '-c -s 10(10)1000 mpn_half',
      '-c -s 10(10)1000 mpn_lshift2',
      '-c -s 10(10)1000 mpn_rshift2',
      '-c -s 10(10)1000 mpn_and_n',
      '-c -s 10(10)1000 mpn_andn_n',
      '-c -s 10(10)1000 mpn_nand_n',
      '-c -s 10(10)1000 mpn_ior_n',
      '-c -s 10(10)1000 mpn_iorn_n',
      '-c -s 10(10)1000 mpn_nior_n',
      '-c -s 10(10)1000 mpn_xor_n',
      '-c -s 10(10)1000 mpn_xnor_n',
      '-c -s 10(10)1000 mpn_com_n',
      '-c -s 10(10)1000 mpn_not',
      '-c -s 10(10)1000 mpn_popcount',
      '-c -s 10(10)1000 mpn_hamdist',

     '-c -s 10(10)1000 MPN_ZERO',
     '-c -s 10(10)1000 MPN_COPY',
     '-c -s 10(10)1000 MPN_COPY_INCR',
     '-c -s 10(10)1000 MPN_COPY_DECR',

     '-c -s 10(10)1000 count_leading_zeros',
     '-c -s 10(10)1000 gmp_allocate_free',
     '-c -s 10(10)1000 malloc_realloc_free',
     '-c -s 10(10)1000 gmp_allocate_reallocate_free',
     '-c -s 10(10)1000 malloc_free',
     '-c -s 10(10)1000 mpn_umul_ppmm',
     '-c -s 10(10)1000 mpz_add',
     '-c -s 10(10)1000 mpz_init_realloc_clear',
     '-c -s 10(10)1000 mpz_init_clear',
     '-c -s 10(10)1000 udiv_qrnnd',
     '-c -s 10(10)1000 udiv_qrnnd_c',
     '-c -s 10(10)1000 udiv_qrnnd_preinv1',
     '-c -s 10(10)1000 udiv_qrnnd_preinv2',
     '-c -s 10(10)1000 umul_ppmm',
     '-c -s 10(10)1000 mpn_popcount',
     '-c -s 10(10)1000 mpn_hamdist',

     ]

lq = [
     '-c -s 10(10)1000 mpn_dc_divrem_n',
     '-c -s 10(10)1000 mpn_dc_divrem_sb',
     '-c -s 10(10)1000 mpn_dc_tdiv_qr',
     '-c -s 10(10)1000 mpn_kara_mul_n',
     '-c -s 10(10)1000 mpn_kara_sqr_n',
     '-c -s 10(10)1000 mpn_mul_basecase',
     '-c -s 1000(500)10000 -t 10 mpn_mul_fft_full',
     '-c -s 10(10)1000 mpn_mul_n',
     '-c -s 10(10)1000 mpn_sqr_basecase',
     '-c -s 10(10)1000 mpn_sqr_n',
     '-c -s 50(10)1000 mpn_toom3_mul_n',
     '-c -s 50(10)1000 mpn_toom3_sqr_n',
     '-c -s 1(5)100 mpz_powm',
     ]

# run an executable and return its error return value and any output
def run_exe(exe, args, inp) :
  al = {'stdin' : PIPE, 'stdout' : PIPE, 'stderr' : STDOUT }
  if sys.platform.startswith('win') :
    al['creationflags'] = 0x08000000
  p = Popen([exe] + args.split(' '), **al)
  res = p.communicate(inp.encode())[0].decode()
  ret = p.poll()
  return (ret, res)

# output a matrix implemented as a dictionary
def mout(m, n) :
  for r in range(n) :
    print('\n{0:3d}'.format(r), end='')
    for c in range(n) :
      print('{0:18.4f}'.format(m[(r,c)]) , end='')
  print

# output a vector
def vout(v) :
  print('   ' , end='')
  for c in v :
    print('{0:18.4f}'.format(c) , end='')
  print()

# In-place LU matrix decomposition. The diagonal
# elements of the upper triangular matrix U are
# all 1 and are not stored. Pivoting is used and
# the matrix is implemented as a dictionary. It
# is only intended for use with small matrices.

def LU_decompose(A, n) :
  p = [0] * n
  for k in range(n) :
    # find pivot
    p[k] = k
    max = math.fabs(A[(k,k)])
    for j in range(k + 1, n) :
      if max < math.fabs(A[(j,k)]) :
        max = math.fabs(A[(j,k)])
        p[k] = j
    # exchange rows if necessary
    if p[k] != k :
      for j in range(n) :
        A[(k,j)], A[(p[k],j)] = A[(p[k],j)], A[(k,j)]

    # exit if matrix is singular
    if A[(k,k)] == 0.0 :
      return None

    # set upper triangular elements
    for j in range(k + 1,n) :
      A[(k,j)] /= A[(k,k)]

    # update remaining part of original matrix
    for i in range(k + 1, n) :
      for j in range(k + 1, n) :
        A[(i,j)] -= A[(i,k)] * A[(k,j)]

  # return pivot array
  return p

# Use the LU decomposition above to solve the matrix
# equation A x = b for x given A and b

def LU_solve(A, p, b) :
  n = len(p)
  x = [0] * n

  # calculate U x = L^-1 b
  for k in range(n) :
    if p[k] != k :
      b[k], b[p[k]] = b[p[k]], b[k]
    x[k] = b[k]
    for i in range(k) :
      x[k] -= x[i] * A[(k,i)]
    x[k] /= A[(k,k)]

  # back substitute for x = U^-1 (L^-1 b)
  for k in reversed(range(n)) :
    if p[k] != k :
      b[k], b[p[k]], b[p[k]], b[k]
    for i in range(k + 1, n) :
      x[k] -= x[i] *  A[(k,i)]
  return x

def lsq_solve(x, y, n) :
  m = {}  # matrix as dictionary
  v = []  # vector as list
  # set up matrix and vectors for least squares
  for i in range(n) :
    v.append(sum(xx ** i * yy for xx, yy in zip(x, y)))
    for j in range(i, n) :
      m[(i,j)] = m[(j,i)] = sum(xx ** (i + j) for xx in x)
  # decompose the matrix into lower and upper triangular
  # matrices
  p = LU_decompose(m, n)
  if p != None :
    return LU_solve(m, p, v)
  else :
    return None

def do_lsq(x, y, lsq_size) :
  # get least squares coefficients
  f = lsq_solve(x, y, lsq_size)

  # now find the standard deviation from the curve
  s = 0
  for i in range(len(x)) :
    t = sum(f[j] * x[i] ** j for j in range(lsq_size))
    s += (y[i] - t) ** 2
  sd =  2 * math.sqrt(s / len(x))

  # now remove 'outliers' - data points outside twice
  # the standard deviation
  sc = 0
  for i in reversed(range(len(x))) :
    t = sum(f[j] * x[i] ** j for j in range(lsq_size))
    if math.fabs(y[i] - t) > sd :
      del x[i]
      del y[i]
      sc += 1

  # if we had to remove more than 10% of measurements
  # declare that the result is not stable
  if 10 * sc > len(x) :
    return None
  else :
    return f

print('Machine:', platform.processor())
print('Running:', platform.platform())
print('SPEED CURVE (l: no of limbs) cycles: c[0] + c[1] * l + c[2] * l^2')
print('ROUTINE                      ', end = '')
print('        c[0]        c[1]        c[2]')
lines = ''
cnt = 0
lsq_size = 4
for args in ll + lq :
  cnt += 1
  # run speed for each routine in the list above
  ret = run_exe(os.path.join(dir, 'speed'), args, '')
  # parse the output to produce limbs[] and times[]
  x = []
  y = []
  lines = ret[1].split('\n')
  for l in lines :
    if len(l) :
      s = l.split()
      try :
        t = [float(i) for i in s]
      except :
        continue
      x += [t[0]]
      y += [t[1]]

# output the name of the routine
  nn = args.split(' ')[-1]
  print('{0:<30s}'.format(nn) , end='')
  if not len(x) :
#   print(ret[1].strip(), '(failed to parse output)')
    print('(failed to parse output)')
    continue

  q = 0 if args in ll else 1
  rep = q
  while rep < 3 :
    rep += 1
    f = do_lsq(x, y, lsq_size)
    if f != None :
      break
  else :
    print('not stable')
    continue
  if args in lq :
    print('{0[0]:11.1f} {0[1]:11.1f} {0[2]:11.1f}'.format(f))
  else :
    print('{0[0]:11.1f} {0[1]:11.1f}'.format(f))
