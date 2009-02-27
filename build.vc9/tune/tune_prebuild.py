
from os.path import exists

names = ['dc_divrem_n.c', 'divrem_2.c', 'gcd.c', 'gcdext.c',
         'get_str.c', 'mul_n.c', 'mullow_n.c', 'mul_fft.c',
         'mul.c', 'sb_divrem_mn.c', 'tdiv_qr.c' ]

for n in names :
  if not exists(n) :
    with open(n, "wb") as f :
      lines = ['#define TUNE_PROGRAM_BUILD 1\r\n']
      lines += ['#include "..\\..\\mpn\\generic\\' + n + '"\r\n']
      f.writelines(lines)

n = 'divrem_1.c'
if not exists(n) :
  with open(n, "wb") as f :
    lines = ['#define TUNE_PROGRAM_BUILD 1\r\n']
    lines += ['#define __gmpn_divrem_1  mpn_divrem_1_tune\r\n']
    lines += ['#include "..\\..\\mpn\\generic\\' + n + '"\r\n']
    f.writelines(lines)

n = 'mod_1.c'
if not exists(n) :
  with open(n, "wb") as f :
    lines = ['#define TUNE_PROGRAM_BUILD 1\r\n']
    lines += ['#define __gmpn_mod_1  mpn_mod_1_tune\r\n']
    lines += ['#include "..\\..\\mpn\\generic\\' + n + '"\r\n']
    f.writelines(lines)
