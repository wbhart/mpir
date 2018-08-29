/* Tests support prototypes etc.

Copyright 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA. */


#ifndef __TESTS_H__
#define __TESTS_H__

#include "config.h"

#include <setjmp.h>  /* for jmp_buf */

#if defined (__cplusplus)
extern "C" {
#endif


#ifdef __cplusplus
#define ANYARGS  ...
#else
#define ANYARGS
#endif


void tests_start(void);
void tests_end(void);

void tests_memory_start(void);
void tests_memory_end(void);
void *tests_allocate(size_t size);
void *tests_reallocate(void *ptr, size_t old_size, size_t new_size);
void tests_free(void *ptr, size_t size);
void tests_free_nosize(void *ptr);
int tests_memory_valid(void *ptr);

void tests_rand_start(void);
void tests_rand_end(void);

double tests_infinity_d();
int tests_hardware_getround(void);
int tests_hardware_setround(int);
int tests_isinf(double);
int tests_dbl_mant_bits(void);

/* tests_setjmp_sigfpe is like a setjmp, establishing a trap for SIGFPE.
   The initial return is 0, if SIGFPE is trapped execution goes back there
   with return value 1.

   tests_sigfpe_done puts SIGFPE back to SIG_DFL, which should be used once
   the setjmp point is out of scope, so a later SIGFPE won't try to go back
   there.  */

#define tests_setjmp_sigfpe()                   \
  (signal (SIGFPE, tests_sigfpe_handler),       \
   setjmp (tests_sigfpe_target))

void tests_sigfpe_handler(int);
void tests_sigfpe_done(void);
extern jmp_buf  tests_sigfpe_target;


#if HAVE_CALLING_CONVENTIONS
extern mp_limb_t (*calling_conventions_function)(ANYARGS);
mp_limb_t calling_conventions(ANYARGS);
int calling_conventions_check(void);
#define CALLING_CONVENTIONS(function) \
  (calling_conventions_function = (function), calling_conventions)
#define CALLING_CONVENTIONS_CHECK()    (calling_conventions_check())
#else
#define CALLING_CONVENTIONS(function)  (function)
#define CALLING_CONVENTIONS_CHECK()    1 /* always ok */
#endif


extern int mp_trace_base;
void mp_limb_trace(const char *, mp_limb_t);
void mpn_trace(const char *name, mp_srcptr ptr, mp_size_t size);
void mpn_tracea(const char *name, const mp_ptr *a, int count, mp_size_t size);
void mpn_tracen(const char *name, int num, mp_srcptr ptr, mp_size_t size);
void mpn_trace_file(const char *filename, mp_srcptr ptr, mp_size_t size);
void mpn_tracea_file(const char *filename, const mp_ptr *a, int count, mp_size_t size);
void mpf_trace(const char *name, mpf_srcptr z);
void mpq_trace(const char *name, mpq_srcptr q);
void mpz_trace(const char *name, mpz_srcptr z);
void mpz_tracen(const char *name, int num, mpz_srcptr z);
void byte_trace(const char *, const void *, mp_size_t);
void byte_tracen(const char *, int, const void *, mp_size_t);
void d_trace(const char *, double);


void spinner(void);
extern unsigned long  spinner_count;
extern int  spinner_wanted;
extern int  spinner_tick;


void *align_pointer(void *p, size_t align);
void *__gmp_allocate_func_aligned(size_t bytes, size_t align);
void *__gmp_allocate_or_reallocate(void *ptr, size_t oldsize, size_t newsize);
char *__gmp_allocate_strdup(const char *s);
char *strtoupper(char *s_orig);
mp_limb_t urandom(gmp_randstate_t);
void call_rand_algs(void (*func) (const char *, gmp_randstate_t));


void mpf_set_str_or_abort(mpf_ptr f, const char *str, int base);


void mpq_set_str_or_abort(mpq_ptr q, const char *str, int base);


void mpz_erandomb(mpz_ptr rop, gmp_randstate_t rstate, unsigned long nbits);
void mpz_erandomb_nonzero(mpz_ptr rop, gmp_randstate_t rstate, unsigned long nbits);
void mpz_errandomb(mpz_ptr rop, gmp_randstate_t rstate, unsigned long nbits);
void mpz_errandomb_nonzero(mpz_ptr rop, gmp_randstate_t rstate, unsigned long nbits);
void mpz_init_set_n(mpz_ptr z, mp_srcptr p, mp_size_t size);
void mpz_negrandom(mpz_ptr rop, gmp_randstate_t rstate);
int mpz_pow2abs_p(mpz_srcptr z) __GMP_ATTRIBUTE_PURE;
void mpz_set_n(mpz_ptr z, mp_srcptr p, mp_size_t size);
void mpz_set_str_or_abort(mpz_ptr z, const char *str, int base);

mp_size_t mpn_diff_highest(mp_srcptr p1, mp_srcptr p2, mp_size_t n) __GMP_ATTRIBUTE_PURE;
mp_size_t mpn_diff_lowest(mp_srcptr p1, mp_srcptr p2, mp_size_t n) __GMP_ATTRIBUTE_PURE;
mp_size_t byte_diff_highest(const void *p1, const void *p2, mp_size_t size) __GMP_ATTRIBUTE_PURE;
mp_size_t byte_diff_lowest(const void *p1, const void *p2, mp_size_t size) __GMP_ATTRIBUTE_PURE;


mp_limb_t ref_addc_limb(mp_limb_t *, mp_limb_t, mp_limb_t);
mp_limb_t ref_bswap_limb(mp_limb_t src);
unsigned long ref_popc_limb(mp_limb_t src);
mp_limb_t ref_subc_limb(mp_limb_t *, mp_limb_t, mp_limb_t);


void refmpf_add(mpf_ptr, mpf_srcptr, mpf_srcptr);
void refmpf_add_ulp(mpf_ptr f);
void refmpf_fill(mpf_ptr f, mp_size_t size, mp_limb_t value);
void refmpf_normalize(mpf_ptr f);
void refmpf_set_prec_limbs(mpf_ptr f, unsigned long prec);
unsigned long refmpf_set_overlap(mpf_ptr dst, mpf_srcptr src);
void refmpf_sub(mpf_ptr, mpf_srcptr, mpf_srcptr);
int refmpf_validate(const char *name, mpf_srcptr got, mpf_srcptr want);
int refmpf_validate_division(const char *name, mpf_srcptr got, mpf_srcptr n, mpf_srcptr d);


mp_limb_t refmpn_add(mp_ptr rp, mp_srcptr s1p, mp_size_t s1size, mp_srcptr s2p, mp_size_t s2size);
mp_limb_t refmpn_add_1(mp_ptr rp, mp_srcptr sp, mp_size_t size, mp_limb_t n);
mp_limb_t refmpn_add_n(mp_ptr wp, mp_srcptr xp, mp_srcptr yp, mp_size_t size);
mp_limb_t refmpn_add_nc(mp_ptr wp, mp_srcptr xp, mp_srcptr yp, mp_size_t size, mp_limb_t carry);
mp_limb_t refmpn_addadd_n(mp_ptr rp, mp_srcptr xp, mp_srcptr yp,mp_srcptr zp, mp_size_t n);

mp_limb_t refmpn_subadd_n(mp_ptr rp, mp_srcptr xp, mp_srcptr yp,mp_srcptr zp, mp_size_t n);

int refmpn_addsub_n(mp_ptr rp, mp_srcptr xp, mp_srcptr yp, mp_srcptr zp, mp_size_t n);

mp_limb_t refmpn_addlsh1_n(mp_ptr wp, mp_srcptr xp, mp_srcptr yp, mp_size_t size);
mp_limb_t refmpn_addlsh_n(mp_ptr wp, mp_srcptr xp, mp_srcptr yp, mp_size_t size,unsigned int);
mp_limb_t refmpn_sublsh_n(mp_ptr wp, mp_srcptr xp, mp_srcptr yp, mp_size_t size,unsigned int);
mp_limb_t refmpn_addlsh_nc(mp_ptr wp, mp_srcptr xp, mp_srcptr yp, mp_size_t size,unsigned int,mp_limb_t);
mp_limb_t refmpn_sublsh_nc(mp_ptr wp, mp_srcptr xp, mp_srcptr yp, mp_size_t size,unsigned int,mp_limb_t);
mp_limb_t refmpn_inclsh_n(mp_ptr wp, mp_srcptr xp, mp_size_t size,unsigned int);
mp_limb_t refmpn_declsh_n(mp_ptr wp, mp_srcptr xp, mp_size_t size,unsigned int);
mp_limb_t refmpn_addmul_1(mp_ptr wp, mp_srcptr xp, mp_size_t size, mp_limb_t multiplier);
mp_limb_t refmpn_addmul_1c(mp_ptr wp, mp_srcptr xp, mp_size_t size, mp_limb_t multiplier, mp_limb_t carry);
mp_limb_t refmpn_addmul_2(mp_ptr dst, mp_srcptr src, mp_size_t size, mp_srcptr mult);
mp_limb_t refmpn_addmul_3(mp_ptr dst, mp_srcptr src, mp_size_t size, mp_srcptr mult);
mp_limb_t refmpn_addmul_4(mp_ptr dst, mp_srcptr src, mp_size_t size, mp_srcptr mult);
mp_limb_t refmpn_addmul_5(mp_ptr dst, mp_srcptr src, mp_size_t size, mp_srcptr mult);
mp_limb_t refmpn_addmul_6(mp_ptr dst, mp_srcptr src, mp_size_t size, mp_srcptr mult);
mp_limb_t refmpn_addmul_7(mp_ptr dst, mp_srcptr src, mp_size_t size, mp_srcptr mult);
mp_limb_t refmpn_addmul_8(mp_ptr dst, mp_srcptr src, mp_size_t size, mp_srcptr mult);

mp_limb_t refmpn_add_err1_n(mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_ptr ep, mp_srcptr yp,mp_size_t n, mp_limb_t cy);
mp_limb_t refmpn_sub_err1_n(mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_ptr ep, mp_srcptr yp,mp_size_t n, mp_limb_t cy);

mp_limb_t refmpn_add_err2_n(mp_ptr rp, mp_srcptr up, mp_srcptr vp,mp_ptr ep, mp_srcptr yp1, mp_srcptr yp2,mp_size_t n, mp_limb_t cy);
mp_limb_t refmpn_sub_err2_n(mp_ptr rp, mp_srcptr up, mp_srcptr vp,mp_ptr ep, mp_srcptr yp1, mp_srcptr yp2,mp_size_t n, mp_limb_t cy);

mp_limb_t refmpn_sumdiff_n(mp_ptr r1p, mp_ptr r2p, mp_srcptr s1p, mp_srcptr s2p, mp_size_t size);
mp_limb_t refmpn_nsumdiff_n(mp_ptr r1p, mp_ptr r2p, mp_srcptr s1p, mp_srcptr s2p, mp_size_t size);
mp_limb_t refmpn_sumdiff_nc(mp_ptr r1p, mp_ptr r2p, mp_srcptr s1p, mp_srcptr s2p, mp_size_t size, mp_limb_t carry);

void refmpn_and_n(mp_ptr wp, mp_srcptr xp, mp_srcptr yp, mp_size_t size);
void refmpn_andn_n(mp_ptr wp, mp_srcptr xp, mp_srcptr yp, mp_size_t size);

mp_limb_t refmpn_big_base(int);

int refmpn_chars_per_limb(int);
void refmpn_clrbit(mp_ptr, unsigned long);
int refmpn_cmp(mp_srcptr s1p, mp_srcptr s2p, mp_size_t size);
int refmpn_cmp_allowzero(mp_srcptr, mp_srcptr, mp_size_t);
int refmpn_cmp_twosizes(mp_srcptr xp, mp_size_t xsize, mp_srcptr yp, mp_size_t ysize);

void refmpn_com_n(mp_ptr rp, mp_srcptr sp, mp_size_t size);
void refmpn_not(mp_ptr rp, mp_size_t size);
void refmpn_copy (mp_ptr rp, mp_srcptr sp, mp_size_t size);
void refmpn_copyi(mp_ptr rp, mp_srcptr sp, mp_size_t size);
void refmpn_copyd(mp_ptr rp, mp_srcptr sp, mp_size_t size);
void refmpn_copy_extend(mp_ptr wp, mp_size_t wsize, mp_srcptr xp, mp_size_t xsize);

unsigned refmpn_count_leading_zeros(mp_limb_t x);
unsigned refmpn_count_trailing_zeros(mp_limb_t x);

mp_limb_t refmpn_divexact_by3(mp_ptr rp, mp_srcptr sp, mp_size_t size);
mp_limb_t refmpn_divexact_by3c(mp_ptr rp, mp_srcptr sp, mp_size_t size, mp_limb_t carry);
mp_limb_t refmpn_divexact_byff(mp_ptr rp, mp_srcptr xp, mp_size_t n);

mp_limb_t refmpn_divexact_byfobm1(mp_ptr rp, mp_srcptr xp, mp_size_t n, mp_limb_t,mp_limb_t);

mp_limb_t refmpn_divmod_1(mp_ptr rp, mp_srcptr sp, mp_size_t size, mp_limb_t divisor);
mp_limb_t refmpn_divrem_euclidean_qr_1(mp_ptr rp, mp_srcptr sp, mp_size_t size,mp_limb_t divisor);
mp_limb_t refmpn_divrem_euclidean_r_1(mp_srcptr sp, mp_size_t size,mp_limb_t divisor);

mp_limb_t refmpn_divrem_hensel_qr_1(mp_ptr rp, mp_srcptr sp, mp_size_t size,mp_limb_t divisor);
mp_limb_t refmpn_divrem_hensel_rsh_qr_1(mp_ptr rp, mp_srcptr sp, mp_size_t size,mp_limb_t divisor,int);
mp_limb_t refmpn_rsh_divrem_hensel_qr_1(mp_ptr rp, mp_srcptr sp, mp_size_t size,mp_limb_t divisor,int,mp_limb_t);
mp_limb_t refmpn_divrem_hensel_r_1(mp_srcptr sp, mp_size_t size,mp_limb_t divisor);

mp_limb_t refmpn_divmod_1c(mp_ptr rp, mp_srcptr sp, mp_size_t size, mp_limb_t divisor, mp_limb_t carry);
mp_limb_t refmpn_divrem_1(mp_ptr rp, mp_size_t xsize, mp_srcptr sp, mp_size_t size, mp_limb_t divisor);
mp_limb_t refmpn_divrem_1c(mp_ptr rp, mp_size_t xsize, mp_srcptr sp, mp_size_t size, mp_limb_t divisor, mp_limb_t carry);

int refmpn_equal_anynail(mp_srcptr, mp_srcptr, mp_size_t);

void refmpn_fill(mp_ptr p, mp_size_t s, mp_limb_t v);

mp_limb_t refmpn_gcd_1(mp_srcptr xp, mp_size_t xsize, mp_limb_t y);
mp_limb_t refmpn_gcd(mp_ptr gp, mp_ptr xp, mp_size_t xsize, mp_ptr yp, mp_size_t ysize);

size_t refmpn_get_str(unsigned char *, int, mp_ptr, mp_size_t);

unsigned long refmpn_hamdist(mp_srcptr s1p, mp_srcptr s2p, mp_size_t size);

mp_limb_t refmpn_invert_limb(mp_limb_t d);
void refmpn_ior_n(mp_ptr wp, mp_srcptr xp, mp_srcptr yp, mp_size_t size);
void refmpn_iorn_n(mp_ptr wp, mp_srcptr xp, mp_srcptr yp, mp_size_t size);

mp_limb_t refmpn_lshift(mp_ptr wp, mp_srcptr xp, mp_size_t size, unsigned shift);
mp_limb_t refmpn_lshiftc(mp_ptr wp, mp_srcptr xp, mp_size_t size, unsigned shift);
mp_limb_t refmpn_lshift_or_copy(mp_ptr wp, mp_srcptr xp, mp_size_t size, unsigned shift);
mp_limb_t refmpn_lshift_or_copy_any(mp_ptr wp, mp_srcptr xp, mp_size_t size, unsigned shift);
mp_limb_t refmpn_lshift1(mp_ptr rp, mp_srcptr xp, mp_size_t n);
mp_limb_t refmpn_lshift2(mp_ptr rp, mp_srcptr xp, mp_size_t n);

mp_limb_t refmpn_double(mp_ptr rp, mp_size_t n);
mp_limb_t refmpn_half(mp_ptr rp,  mp_size_t n);

mp_ptr refmpn_malloc_limbs(mp_size_t size);
mp_ptr refmpn_malloc_limbs_aligned(mp_size_t n, size_t m);
void refmpn_free_limbs(mp_ptr p);
mp_limb_t refmpn_msbone(mp_limb_t x);
mp_limb_t refmpn_msbone_mask(mp_limb_t x);
mp_ptr refmpn_memdup_limbs(mp_srcptr ptr, mp_size_t size);

mp_limb_t refmpn_mod_1(mp_srcptr sp, mp_size_t size, mp_limb_t divisor);
mp_limb_t refmpn_mod_1c(mp_srcptr sp, mp_size_t size, mp_limb_t divisor, mp_limb_t carry);
mp_limb_t refmpn_mod_34lsub1(mp_srcptr p, mp_size_t n);

mp_limb_t refmpn_mul_1(mp_ptr wp, mp_srcptr xp, mp_size_t size, mp_limb_t multiplier);
mp_limb_t refmpn_mul_1c(mp_ptr wp, mp_srcptr xp, mp_size_t size, mp_limb_t multiplier, mp_limb_t carry);
mp_limb_t refmpn_mul_2(mp_ptr dst, mp_srcptr src, mp_size_t size, mp_srcptr mult);

void refmpn_mul_basecase(mp_ptr prodp, mp_srcptr up, mp_size_t usize, mp_srcptr vp, mp_size_t vsize);
void refmpn_mul_any(mp_ptr prodp, mp_srcptr up, mp_size_t usize, mp_srcptr vp, mp_size_t vsize);
void refmpn_mul_n(mp_ptr prodp, mp_srcptr up, mp_srcptr vp, mp_size_t size);
void refmpn_mulmid_basecase(mp_ptr rp, mp_srcptr up, mp_size_t un, mp_srcptr vp, mp_size_t vn);
void refmpn_mulmid(mp_ptr rp, mp_srcptr up, mp_size_t un, mp_srcptr vp, mp_size_t vn);
void refmpn_mulmid_n(mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_size_t n);
void refmpn_nand_n(mp_ptr wp, mp_srcptr xp, mp_srcptr yp, mp_size_t size);
void refmpn_nior_n(mp_ptr wp, mp_srcptr xp, mp_srcptr yp, mp_size_t size);
mp_limb_t refmpn_neg_n(mp_ptr dst, mp_srcptr src, mp_size_t size);
mp_size_t refmpn_normalize(mp_srcptr, mp_size_t);

unsigned long refmpn_popcount(mp_srcptr sp, mp_size_t size);
mp_limb_t refmpn_preinv_divrem_1(mp_ptr rp, mp_size_t xsize, mp_srcptr sp, mp_size_t size, mp_limb_t divisor, mp_limb_t inverse, unsigned shift);
mp_limb_t refmpn_preinv_mod_1(mp_srcptr sp, mp_size_t size, mp_limb_t divisor, mp_limb_t divisor_inverse);

void refmpn_random(mp_ptr, mp_size_t);
void refmpn_random2(mp_ptr, mp_size_t);
mp_limb_t refmpn_random_limb(void);

mp_limb_t refmpn_rsh1add_n(mp_ptr wp, mp_srcptr xp, mp_srcptr yp, mp_size_t size);
mp_limb_t refmpn_rsh1sub_n(mp_ptr wp, mp_srcptr xp, mp_srcptr yp, mp_size_t size);
mp_limb_t refmpn_rshift(mp_ptr wp, mp_srcptr xp, mp_size_t size, unsigned shift);
mp_limb_t refmpn_rshift_or_copy(mp_ptr wp, mp_srcptr xp, mp_size_t size, unsigned shift);
mp_limb_t refmpn_rshift_or_copy_any(mp_ptr wp, mp_srcptr xp, mp_size_t size, unsigned shift);
mp_limb_t refmpn_rshift1(mp_ptr rp, mp_srcptr xp, mp_size_t n);
mp_limb_t refmpn_rshift2(mp_ptr rp, mp_srcptr xp, mp_size_t n);

mp_limb_t refmpn_sb_divrem_mn(mp_ptr qp, mp_ptr np, mp_size_t nsize, mp_srcptr dp, mp_size_t dsize);
unsigned long refmpn_scan0(mp_srcptr, unsigned long);
unsigned long refmpn_scan1(mp_srcptr, unsigned long);
void refmpn_setbit(mp_ptr, unsigned long);
void refmpn_sqr(mp_ptr dst, mp_srcptr src, mp_size_t size);
mp_size_t refmpn_sqrtrem(mp_ptr, mp_ptr, mp_srcptr, mp_size_t);

void refmpn_sub_ddmmss(mp_limb_t *, mp_limb_t *, mp_limb_t, mp_limb_t, mp_limb_t, mp_limb_t);
mp_limb_t refmpn_sub(mp_ptr rp, mp_srcptr s1p, mp_size_t s1size, mp_srcptr s2p, mp_size_t s2size);
mp_limb_t refmpn_sub_1(mp_ptr rp, mp_srcptr sp, mp_size_t size, mp_limb_t n);
mp_limb_t refmpn_sub_n(mp_ptr wp, mp_srcptr xp, mp_srcptr yp, mp_size_t size);
mp_limb_t refmpn_sub_nc(mp_ptr wp, mp_srcptr xp, mp_srcptr yp, mp_size_t size, mp_limb_t carry);
mp_limb_t refmpn_sublsh1_n(mp_ptr wp, mp_srcptr xp, mp_srcptr yp, mp_size_t size);
mp_limb_t refmpn_submul_1(mp_ptr wp, mp_srcptr xp, mp_size_t size, mp_limb_t multiplier);
mp_limb_t refmpn_submul_1c(mp_ptr wp, mp_srcptr xp, mp_size_t size, mp_limb_t multiplier, mp_limb_t carry);

void refmpn_tdiv_qr(mp_ptr qp, mp_ptr rp, mp_size_t qxn, mp_ptr np, mp_size_t nsize, mp_srcptr dp, mp_size_t dsize);
void refmpn_tdiv_q(mp_ptr qp, mp_ptr np, mp_size_t nsize, mp_srcptr dp, mp_size_t dsize);
int refmpn_tstbit(mp_srcptr, unsigned long);

mp_limb_t refmpn_udiv_qrnnd(mp_limb_t *, mp_limb_t, mp_limb_t, mp_limb_t);
mp_limb_t refmpn_udiv_qrnnd_r(mp_limb_t, mp_limb_t, mp_limb_t, mp_limb_t *);
mp_limb_t refmpn_umul_ppmm(mp_limb_t *, mp_limb_t, mp_limb_t);
mp_limb_t refmpn_umul_ppmm_r(mp_limb_t, mp_limb_t, mp_limb_t *);

void refmpn_xnor_n(mp_ptr wp, mp_srcptr xp, mp_srcptr yp, mp_size_t size);
void refmpn_xor_n(mp_ptr wp, mp_srcptr xp, mp_srcptr yp, mp_size_t size);

void refmpn_zero(mp_ptr p, mp_size_t s);
void refmpn_zero_extend(mp_ptr, mp_size_t, mp_size_t);
int refmpn_zero_p(mp_srcptr ptr, mp_size_t size);
void refmpn_store(mp_ptr p, mp_size_t s,mp_limb_t);

void refmpq_add(mpq_ptr w, mpq_srcptr x, mpq_srcptr y);
void refmpq_sub(mpq_ptr w, mpq_srcptr x, mpq_srcptr y);


void refmpz_combit(mpz_ptr r, unsigned long bit);
unsigned long refmpz_hamdist(mpz_srcptr x, mpz_srcptr y);
int refmpz_kronecker(mpz_srcptr a_orig, mpz_srcptr b_orig);
int refmpz_jacobi(mpz_srcptr a_orig, mpz_srcptr b_orig);
int refmpz_legendre(mpz_srcptr a_orig, mpz_srcptr b_orig);
int refmpz_kronecker_si(mpz_srcptr, long);
int refmpz_kronecker_ui(mpz_srcptr, unsigned long);
int refmpz_si_kronecker(long, mpz_srcptr);
int refmpz_ui_kronecker(unsigned long, mpz_srcptr);

void refmpz_pow_ui(mpz_ptr w, mpz_srcptr b, unsigned long e);

void refmpn_redc_1(mp_ptr cp, mp_ptr tp,mp_srcptr mp, mp_size_t n, mp_limb_t Nd);

#if defined (__cplusplus)
}
#endif


/* Establish ostringstream and istringstream.  Do this here so as to hide
   the conditionals, rather than putting stuff in each test program.

   Oldish versions of g++, like 2.95.2, don't have <sstream>, only
   <strstream>.  Fake up ostringstream and istringstream classes, but not a
   full implementation, just enough for our purposes.  */

#ifdef __cplusplus
#if HAVE_SSTREAM
#include <sstream>
#else /* ! HAVE_SSTREAM */
#include <string>
#include <strstream>
class
ostringstream : public std::ostrstream {
 public:
  string str() {
    int  pcount = ostrstream::pcount ();
    char *s = (char *) (*__gmp_allocate_func) (pcount + 1);
    memcpy (s, ostrstream::str(), pcount);
    s[pcount] = '\0';
    string ret = string(s);
    (*__gmp_free_func) (s, pcount + 1);
    return ret; }
};
class
istringstream : public std::istrstream {
 public:
  istringstream (const char *s) : istrstream (s) { };
};
#endif /* ! HAVE_SSTREAM */
#endif /* __cplusplus */


#endif /* __TESTS_H__ */
