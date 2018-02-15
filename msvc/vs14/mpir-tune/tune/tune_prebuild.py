
from os.path import exists

print('Performing tune prebuild ...')

basic = [
  'dc_bdiv_qr.c', 'dc_bdiv_qr_n.c', 'dc_bdiv_q.c', 'dc_divappr_q.c',
  'dc_div_qr.c', 'dc_div_qr_n.c', 'divrem_2.c',  'divrem_euclidean_r_1.c',
  'divrem_hensel_qr_1.c',  'gcd.c', 'gcdext.c', 'get_str.c', 'hgcd.c',
  'hgcd_appr.c', 'hgcd_reduce.c', 'inv_divappr_q.c', 'inv_div_qr.c',
  'matrix22_mul.c', 'mul.c', 'mulhigh_n.c', 'mullow_n.c', 'mulmod_2expm1.c',
  'mulmod_2expp1_basecase.c', 'mul_n.c', 'rootrem.c', 'rsh_divrem_hensel_qr_1.c',
  'sb_divappr_q.c', 'sb_div_qr.c', 'set_str.c', 'tdiv_q.c', 'tdiv_qr.c',
  'toom3_mul.c', 'toom3_mul_n.c', 'toom4_mul.c', 'toom4_mul_n.c',
  'toom8h_mul.c', 'toom8_sqr_n.c'
]

fft_basic = [
  'adjust.c', 'adjust_sqrt2.c', 'butterfly_lshB.c', 'butterfly_rshB.c',
  'combine_bits.c', 'div_2expmod_2expp1.c', 'fermat_to_mpz.c',
  'fft_mfa_trunc_sqrt2.c', 'fft_mfa_trunc_sqrt2_inner.c', 'fft_negacyclic.c',
  'fft_radix2.c', 'fft_trunc.c', 'ifft_negacyclic.c', 'ifft_radix2.c',
  'ifft_trunc.c', 'ifft_trunc_sqrt2.c', 'mul_2expmod_2expp1.c',
  'mul_fft_main.c', 'mul_mfa_trunc_sqrt2.c', 'mulmod_2expp1.c',
  'mul_trunc_sqrt2.c', 'normmod_2expp1.c', 'revbin.c', 'split_bits.c'
]

for n in basic:
  if not exists(n) :
    with open(n, "wb") as f :
      lines = [('#define TUNE_PROGRAM_BUILD 1\r\n').encode()]
      lines += [('#include "..\\..\\..\\mpn\\generic\\' + n + '"\r\n').encode()]
      f.writelines(lines)

for n in fft_basic:
  if not exists(n) :
    with open(n, "wb") as f :
      lines = [('#define TUNE_PROGRAM_BUILD 1\r\n').encode()]
      lines += [('#include "..\\..\\..\\fft\\' + n + '"\r\n').encode()]
      f.writelines(lines)

n = 'divrem_1.c'
if not exists(n) :
  with open(n, "wb") as f :
    lines = [('#define TUNE_PROGRAM_BUILD 1\r\n').encode()]
    lines += [('#define __gmpn_divrem_1  mpn_divrem_1_tune\r\n').encode()]
    lines += [('#include "..\\..\\..\\mpn\\generic\\' + n + '"\r\n').encode()]
    f.writelines(lines)

n = 'mod_1.c'
if not exists(n) :
  with open(n, "wb") as f :
    lines = [('#define TUNE_PROGRAM_BUILD 1\r\n').encode()]
    lines += [('#define __gmpn_mod_1  mpn_mod_1_tune\r\n').encode()]
    lines += [('#include "..\\..\\..\\mpn\\generic\\' + n + '"\r\n').encode()]
    f.writelines(lines)


n = 'sqr_basecase.asm'
if not exists(n) :
  with open(n, "wb") as f :
    lines = [('%define SQR_KARATSUBA_THRESHOLD_OVERRIDE SQR_KARATSUBA_THRESHOLD_MAX\r\n').encode()]
    lines += [('%include "..\\..\\..\\mpn\\x86_64w\\' + n + '"\r\n').encode()]
    f.writelines(lines)

n = 'fac_ui.c'
if not exists(n) :
  with open(n, "wb") as f :
    lines = [('#define TUNE_PROGRAM_BUILD 1\r\n').encode()]
    lines += [('#define __gmpz_fac_ui mpz_fac_ui_tune\r\n').encode()]
    lines += [('#define __gmpz_oddfac_1 mpz_oddfac_1_tune\r\n').encode()]
    lines += [('#include "..\\..\\..\\mpz\\' + 'oddfac_1.c' + '"\r\n').encode()]
    lines += [('#include "..\\..\\..\\mpz\\' + 'fac_ui.c' + '"\r\n').encode()]
    f.writelines(lines)
