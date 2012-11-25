
from os.path import exists

names = [ 'divrem_2.c', 'gcd.c', 'gcdext.c', 'get_str.c',
          'mul_n.c', 'mullow_n.c', 'mulhigh_n.c', 'mul_fft.c',
	  'mul.c', 'tdiv_qr.c', 'toom4_mul_n.c', 'toom4_mul.c',
	  'toom3_mul.c', 'toom3_mul_n.c', 'toom8h_mul.c', 
	  'toom8_sqr_n.c', 'mulmod_2expm1.c', 'rootrem.c',
	  'divrem_euclidean_r_1.c', 'divrem_hensel_qr_1.c',
	  'rsh_divrem_hensel_qr_1.c', 'dc_divappr_q.c',
          'dc_div_qr.c', 'inv_divappr_q.c', 'inv_div_qr.c',
          'tdiv_q.c', 'dc_bdiv_qr.c', 'dc_bdiv_q.c' ]
		   
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
