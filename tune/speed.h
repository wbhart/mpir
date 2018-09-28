/* Header for speed and threshold things.

Copyright 1999, 2000, 2001, 2002, 2003, 2005, 2006, 2008, 2009, 2010, 2011,
2012 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library.  If not, see http://www.gnu.org/licenses/.  

*/

#ifndef __SPEED_H__
#define __SPEED_H__


/* Pad ptr,oldsize with zero limbs (at the most significant end) to make it
   newsize long. */
#define MPN_ZERO_EXTEND(ptr, oldsize, newsize)		\
  do {							\
    ASSERT ((newsize) >= (oldsize));			\
    MPN_ZERO ((ptr)+(oldsize), (newsize)-(oldsize));	\
  } while (0)

/* A mask of the least significant n bits.  Note 1<<32 doesn't give zero on
   x86 family CPUs, hence the separate case for BITS_PER_MP_LIMB. */
#define MP_LIMB_T_LOWBITMASK(n)	\
  ((n) == BITS_PER_MP_LIMB ? MP_LIMB_T_MAX : ((mp_limb_t) 1 << (n)) - 1)


/* align must be a power of 2 here, usually CACHE_LINE_SIZE is a good choice */

#define TMP_ALLOC_ALIGNED(bytes, align)	\
  align_pointer (TMP_ALLOC ((bytes) + (align)-1), (align))
#define TMP_ALLOC_LIMBS_ALIGNED(limbs, align)	\
  ((mp_ptr) TMP_ALLOC_ALIGNED ((limbs)*sizeof(mp_limb_t), align))

/* CACHE_LINE_SIZE is our default alignment for speed operands, and the
   limit on what s->align_xp etc and then request for off-alignment.  Maybe
   this should be an option of some sort, but in any case here are some line
   sizes,

       bytes
	 32   pentium
	 64   athlon
	 64   itanium-2 L1
	128   itanium-2 L2
*/
#define CACHE_LINE_SIZE   64 /* bytes */

#define SPEED_TMP_ALLOC_ADJUST_MASK  (CACHE_LINE_SIZE/BYTES_PER_MP_LIMB - 1)

/* Set ptr to a TMP_ALLOC block of the given limbs, with the given limb
   alignment.  */
#define SPEED_TMP_ALLOC_LIMBS(ptr, limbs, align)			\
  do {									\
    mp_ptr     __ptr;							\
    mp_size_t  __ptr_align, __ptr_add;					\
									\
    ASSERT ((CACHE_LINE_SIZE % BYTES_PER_MP_LIMB) == 0);		\
    __ptr = TMP_ALLOC_LIMBS ((limbs) + SPEED_TMP_ALLOC_ADJUST_MASK);	\
    __ptr_align = (__ptr - (mp_ptr) NULL);				\
    __ptr_add = ((align) - __ptr_align) & SPEED_TMP_ALLOC_ADJUST_MASK;	\
    (ptr) = __ptr + __ptr_add;						\
  } while (0)


/* This is the size for s->xp_block and s->yp_block, used in certain
   routines that want to run across many different data values and use
   s->size for a different purpose, eg. SPEED_ROUTINE_MPN_GCD_1.

   512 means 2kbytes of data for each of xp_block and yp_block, making 4k
   total, which should fit easily in any L1 data cache. */

#define SPEED_BLOCK_SIZE   512 /* limbs */

#ifdef _MSC_VER
#include "win_timing.h"
#else
extern double  speed_unittime;
extern double  speed_cycletime;
extern int     speed_precision;
extern char    speed_time_string[];
void speed_time_init(void);
void speed_cycletime_fail(const char *str);
void speed_cycletime_init(void);
void speed_cycletime_need_cycles(void);
void speed_cycletime_need_seconds(void);
void speed_starttime(void);
double speed_endtime(void);
#endif

struct speed_params {
  unsigned   reps;	/* how many times to run the routine */
  mp_ptr     xp;	/* first argument */
  mp_ptr     yp;	/* second argument */
  mp_size_t  size;	/* size of both arguments */
  mp_limb_t  r;		/* user supplied parameter */
  mp_size_t  align_xp;	/* alignment of xp */
  mp_size_t  align_yp;	/* alignment of yp */
  mp_size_t  align_wp;	/* intended alignment of wp */
  mp_size_t  align_wp2; /* intended alignment of wp2 */
  mp_ptr     xp_block;	/* first special SPEED_BLOCK_SIZE block */
  mp_ptr     yp_block;	/* second special SPEED_BLOCK_SIZE block */

  double     time_divisor; /* optionally set by the speed routine */

  /* used by the cache priming things */
  int	     cache;
  unsigned   src_num, dst_num;
  struct {
    mp_ptr    ptr;
    mp_size_t size;
  } src[5], dst[5];
};

typedef double (*speed_function_t)(struct speed_params *s);

double speed_measure(speed_function_t fun, struct speed_params *s);

/* Prototypes for speed measuring routines */

double speed_back_to_back(struct speed_params *s);
double speed_count_leading_zeros(struct speed_params *s);
double speed_count_trailing_zeros(struct speed_params *s);
double speed_gmp_allocate_free(struct speed_params *s);
double speed_gmp_allocate_reallocate_free(struct speed_params *s);
double speed_invert_limb(struct speed_params *s);
double speed_malloc_free(struct speed_params *s);
double speed_malloc_realloc_free(struct speed_params *s);
double speed_memcpy(struct speed_params *s);
double speed_modlimb_invert(struct speed_params *s);
double speed_modlimb_invert_mul1(struct speed_params *s);
double speed_modlimb_invert_loop(struct speed_params *s);
double speed_modlimb_invert_cond(struct speed_params *s);
double speed_modlimb_invert_arith(struct speed_params *s);

double speed_mpf_init_clear(struct speed_params *s);
double speed_mpn_add_err1_n(struct speed_params *s);
double speed_mpn_sub_err1_n(struct speed_params *s);

double speed_mpn_add_n(struct speed_params *s);
double speed_mpn_addadd_n(struct speed_params *s);
double speed_mpn_subadd_n(struct speed_params *s);
double speed_mpn_addlsh1_n(struct speed_params *s);
double speed_mpn_addlsh_n(struct speed_params *s);
double speed_mpn_sublsh_n(struct speed_params *s);
double speed_mpn_declsh_n(struct speed_params *s);
double speed_mpn_inclsh_n(struct speed_params *s);
double speed_mpn_sumdiff_n(struct speed_params *s);
double speed_mpn_nsumdiff_n(struct speed_params *s);
double speed_mpn_and_n(struct speed_params *s);
double speed_mpn_andn_n(struct speed_params *s);
double speed_mpn_addmul_1(struct speed_params *s);
double speed_mpn_addmul_2(struct speed_params *s);
double speed_mpn_addmul_3(struct speed_params *s);
double speed_mpn_addmul_4(struct speed_params *s);
double speed_mpn_addmul_5(struct speed_params *s);
double speed_mpn_addmul_6(struct speed_params *s);
double speed_mpn_addmul_7(struct speed_params *s);
double speed_mpn_addmul_8(struct speed_params *s);
double speed_mpn_addsub_n(struct speed_params *s);
double speed_mpn_com_n(struct speed_params *s);
double speed_mpn_not(struct speed_params *s);
double speed_mpn_copyd(struct speed_params *s);
double speed_mpn_copyi(struct speed_params *s);
double speed_mpn_dc_tdiv_qr(struct speed_params *s);
double speed_mpn_dc_div_qr_n(struct speed_params *s);
double speed_mpn_tdiv_q(struct speed_params *s);
double speed_mpn_tdiv_q1(struct speed_params *s);
double speed_mpn_sb_divappr_q(struct speed_params *s);
double speed_mpn_sb_div_qr(struct speed_params *s);
double speed_mpn_dc_divappr_q(struct speed_params *s);
double speed_mpn_dc_bdiv_q(struct speed_params *s);
double speed_mpn_dc_bdiv_qr_n(struct speed_params *s);
double speed_mpn_inv_div_qr(struct speed_params *s);
double speed_mpn_inv_divappr_q(struct speed_params *s);
double speed_mpn_tdiv_q(struct speed_params *s);
double speed_mpn_tdiv_q2(struct speed_params *s);
double speed_MPN_COPY(struct speed_params *s);
double speed_MPN_COPY_DECR(struct speed_params *s);
double speed_MPN_COPY_INCR(struct speed_params *s);
double speed_mpn_divexact_1(struct speed_params *s);
double speed_mpn_divexact_by3(struct speed_params *s);
double speed_mpn_divexact_byff(struct speed_params *s);
double speed_mpn_divexact_byfobm1(struct speed_params *s);
double speed_mpn_divrem_euclidean_qr_1(struct speed_params *s);
double speed_mpn_divrem_euclidean_qr_2(struct speed_params *s);
double speed_mpn_divrem_euclidean_r_1(struct speed_params *s);
double speed_mpn_divrem_hensel_qr_1(struct speed_params *s);
double speed_mpn_divrem_hensel_qr_1_1(struct speed_params *s);
double speed_mpn_divrem_hensel_qr_1_2(struct speed_params *s);
double speed_mpn_divrem_hensel_r_1(struct speed_params *s);
double speed_mpn_rsh_divrem_hensel_qr_1(struct speed_params *s);
double speed_mpn_rsh_divrem_hensel_qr_1_1(struct speed_params *s);
double speed_mpn_rsh_divrem_hensel_qr_1_2(struct speed_params *s);
double speed_mpn_divrem_hensel_rsh_qr_1(struct speed_params *s);
double speed_mpn_divrem_1(struct speed_params *s);
double speed_mpn_divrem_1f(struct speed_params *s);
double speed_mpn_divrem_1c(struct speed_params *s);
double speed_mpn_divrem_1cf(struct speed_params *s);
double speed_mpn_divrem_1_div(struct speed_params *s);
double speed_mpn_divrem_1f_div(struct speed_params *s);
double speed_mpn_divrem_1_inv(struct speed_params *s);
double speed_mpn_divrem_1f_inv(struct speed_params *s);
double speed_mpn_divrem_2(struct speed_params *s);
double speed_mpn_divrem_2_div(struct speed_params *s);
double speed_mpn_divrem_2_inv(struct speed_params *s);
double speed_mpn_double(struct speed_params *s);
double speed_mpn_half(struct speed_params *s);
double speed_mpn_fib2_ui(struct speed_params *s);
double speed_mpn_matrix22_mul (struct speed_params *);
double speed_mpn_hgcd (struct speed_params *);
double speed_mpn_hgcd_lehmer (struct speed_params *);
double speed_mpn_hgcd_appr (struct speed_params *);
double speed_mpn_hgcd_appr_lehmer (struct speed_params *);
double speed_mpn_hgcd_reduce (struct speed_params *);
double speed_mpn_hgcd_reduce_1 (struct speed_params *);
double speed_mpn_hgcd_reduce_2 (struct speed_params *);
double speed_mpn_gcd (struct speed_params *);
double speed_mpn_gcd_1 (struct speed_params *);
double speed_mpn_gcd_1N (struct speed_params *);
double speed_mpn_gcdext (struct speed_params *);
double speed_mpn_gcdext_double (struct speed_params *);
double speed_mpn_gcdext_one_double (struct speed_params *);
double speed_mpn_gcdext_one_single (struct speed_params *);
double speed_mpn_gcdext_single (struct speed_params *);
double speed_mpn_get_str(struct speed_params *s);
double speed_mpn_hamdist(struct speed_params *s);
double speed_mpn_ior_n(struct speed_params *s);
double speed_mpn_iorn_n(struct speed_params *s);
double speed_mpn_jacobi_base(struct speed_params *s);
double speed_mpn_jacobi_base_1(struct speed_params *s);
double speed_mpn_jacobi_base_2(struct speed_params *s);
double speed_mpn_jacobi_base_3(struct speed_params *s);
double speed_mpn_jacobi_base_4(struct speed_params *s);
double speed_mpn_kara_mul_n(struct speed_params *s);
double speed_mpn_kara_sqr_n(struct speed_params *s);
double speed_mpn_karaadd(struct speed_params *s);
double speed_mpn_karasub(struct speed_params *s);
double speed_mpn_lshift(struct speed_params *s);
double speed_mpn_lshift1(struct speed_params *s);
double speed_mpn_lshift2(struct speed_params *s);
double speed_mpn_lshiftc(struct speed_params *s);
double speed_mpn_mod_1(struct speed_params *s);
double speed_mpn_mod_1_1(struct speed_params *s);
double speed_mpn_mod_1_2(struct speed_params *s);
double speed_mpn_mod_1_3(struct speed_params *s);
double speed_mpn_mod_1_k(struct speed_params *s);
double speed_mpn_mod_1c(struct speed_params *s);
double speed_mpn_mod_1_div(struct speed_params *s);
double speed_mpn_mod_1_inv(struct speed_params *s);
double speed_mpn_mod_34lsub1(struct speed_params *s);
double speed_mpn_modexact_1_odd(struct speed_params *s);
double speed_mpn_modexact_1c_odd(struct speed_params *s);
double speed_mpn_mul_1(struct speed_params *s);
double speed_mpn_mul_1_inplace(struct speed_params *s);
double speed_mpn_mul_2(struct speed_params *s);
double speed_mpn_mul_basecase(struct speed_params *s);
double speed_mpn_mul_fft_main(struct speed_params *s);
double speed_mpn_sqr_fft_main(struct speed_params *s);
double speed_mpn_mul_n(struct speed_params *s);
double speed_mpn_mul_n_sqr(struct speed_params *s);
double speed_mpn_mullow_n(struct speed_params *s);
double speed_mpn_mulhigh_n(struct speed_params *s);
double speed_mpn_mulmod_2expm1(struct speed_params *s);
double speed_mpn_mulmod_2expp1_basecase(struct speed_params *s);
double speed_mpn_mullow_n_basecase(struct speed_params *s);
double speed_mpn_mulmid(struct speed_params *s);
double speed_mpn_mulmid_basecase(struct speed_params *s);
double speed_mpn_mulmid_n(struct speed_params *s);
double speed_mpn_toom42_mulmid(struct speed_params *s);
double speed_mpn_nand_n(struct speed_params *s);
double speed_mpn_nior_n(struct speed_params *s);
double speed_mpn_popcount(struct speed_params *s);
double speed_mpn_preinv_divrem_1(struct speed_params *s);
double speed_mpn_preinv_divrem_1f(struct speed_params *s);
double speed_mpn_preinv_mod_1(struct speed_params *s);
double speed_mpn_binvert (struct speed_params *);
double speed_mpn_redc_1 (struct speed_params *);
double speed_mpn_redc_2 (struct speed_params *);
double speed_mpn_redc_n (struct speed_params *);
double speed_mpn_rsh1add_n(struct speed_params *s);
double speed_mpn_rsh1sub_n(struct speed_params *s);
double speed_mpn_rshift(struct speed_params *s);
double speed_mpn_rshift1(struct speed_params *s);
double speed_mpn_rshift2(struct speed_params *s);
double speed_mpn_set_str(struct speed_params *s);
double speed_mpn_set_str_basecase(struct speed_params *s);
double speed_mpn_set_str_subquad(struct speed_params *s);
double speed_mpn_sqr_basecase(struct speed_params *s);
double speed_mpn_sqr_diagonal(struct speed_params *s);
double speed_mpn_sqr(struct speed_params *s);
double speed_mpn_sqrtrem(struct speed_params *s);
double speed_mpn_rootrem(struct speed_params *s);
double speed_mpn_sub_n(struct speed_params *s);
double speed_mpn_sublsh1_n(struct speed_params *s);
double speed_mpn_submul_1(struct speed_params *s);
double speed_mpn_toom3_mul_n(struct speed_params *s);
double speed_mpn_toom4_mul_n(struct speed_params *s);
double speed_mpn_toom8h_mul(struct speed_params *s);
double speed_mpn_toom3_sqr_n(struct speed_params *s);
double speed_mpn_toom4_sqr_n(struct speed_params *s);
double speed_mpn_toom8_sqr_n(struct speed_params *s);
double speed_mpn_udiv_qrnnd(struct speed_params *s);
double speed_mpn_udiv_qrnnd_r(struct speed_params *s);
double speed_mpn_umul_ppmm(struct speed_params *s);
double speed_mpn_umul_ppmm_r(struct speed_params *s);
double speed_mpn_xnor_n(struct speed_params *s);
double speed_mpn_xor_n(struct speed_params *s);
double speed_MPN_ZERO(struct speed_params *s);
double speed_mpn_store(struct speed_params *s);

double speed_mpq_init_clear(struct speed_params *s);

double speed_mpz_add(struct speed_params *s);
double speed_mpz_bin_uiui(struct speed_params *s);
double speed_mpz_fac_ui(struct speed_params *s);
double speed_mpz_fac_ui_small(struct speed_params *s);
double speed_mpz_fac_ui_large(struct speed_params *s);
double speed_mpz_fib_ui(struct speed_params *s);
double speed_mpz_fib2_ui(struct speed_params *s);
double speed_mpz_init_clear(struct speed_params *s);
double speed_mpz_init_realloc_clear(struct speed_params *s);
double speed_mpz_jacobi(struct speed_params *s);
double speed_mpz_lucnum_ui(struct speed_params *s);
double speed_mpz_lucnum2_ui(struct speed_params *s);
double speed_mpz_mod(struct speed_params *s);
double speed_mpz_powm(struct speed_params *s);
double speed_mpz_powm_mod(struct speed_params *s);
double speed_mpz_powm_redc(struct speed_params *s);
double speed_mpz_powm_ui(struct speed_params *s);
double speed_mpz_urandomb(struct speed_params *s);

double speed_gmp_randseed(struct speed_params *s);
double speed_gmp_randseed_ui(struct speed_params *s);

double speed_noop(struct speed_params *s);
double speed_noop_wxs(struct speed_params *s);
double speed_noop_wxys(struct speed_params *s);

double speed_operator_div(struct speed_params *s);
double speed_operator_mod(struct speed_params *s);

double speed_udiv_qrnnd(struct speed_params *s);
double speed_udiv_qrnnd_preinv1(struct speed_params *s);
double speed_udiv_qrnnd_preinv2(struct speed_params *s);
double speed_udiv_qrnnd_c(struct speed_params *s);
double speed_umul_ppmm(struct speed_params *s);


/* Prototypes for other routines */

/* low 32-bits in p[0], high 32-bits in p[1] */
void speed_cyclecounter(unsigned p[2]);

void mftb_function(unsigned p[2]);

/* In i386 gcc -fPIC, ebx is a fixed register and can't be declared a dummy
   output or a clobber for the cpuid, hence an explicit save and restore.  A
   clobber as such doesn't provoke an error unfortunately (gcc 3.0), so use
   the dummy output style in non-PIC, so there's an error if somehow -fPIC
   is used without a -DPIC to tell us about it.	 */
#if defined(__GNUC__) && (defined (__i386__) || defined (__i486__))
#ifdef PIC
#define speed_cyclecounter(p)						\
  do {									\
    int	 __speed_cyclecounter__save_ebx;				\
    int	 __speed_cyclecounter__dummy;					\
    __asm__ __volatile__ ("movl %%ebx, %1\n"				\
			  "cpuid\n"					\
			  "movl %1, %%ebx\n"				\
			  "rdtsc"					\
			  : "=a"   ((p)[0]),				\
			    "=&rm" (__speed_cyclecounter__save_ebx),	\
			    "=c"   (__speed_cyclecounter__dummy),	\
			    "=d"   ((p)[1]));				\
  } while (0)
#else
#define speed_cyclecounter(p)						\
  do {									\
    int	 __speed_cyclecounter__dummy1;					\
    int	 __speed_cyclecounter__dummy2;					\
    __asm__ __volatile__ ("cpuid\n"					\
			  "rdtsc"					\
			  : "=a" ((p)[0]),				\
			    "=b" (__speed_cyclecounter__dummy1),	\
			    "=c" (__speed_cyclecounter__dummy2),	\
			    "=d" ((p)[1]));				\
  } while (0)
#endif
#elif defined( _MSC_VER )
#include <intrin.h>
#pragma intrinsic(__rdtsc)
__inline void speed_cyclecounter(unsigned long p[2]) { *(unsigned long long*)p = __rdtsc(); }
#endif

double speed_cyclecounter_diff(const unsigned end[2], const unsigned start[2]);
int gettimeofday_microseconds_p(void);
int getrusage_microseconds_p(void);
int cycles_works_p(void);
long clk_tck(void);
double freq_measure(const char *, double (*)(void));

int double_cmp_ptr(const double *p, const double *q);
void pentium_wbinvd(void);
typedef int (*qsort_function_t)(const void *, const void *);

void noop(void);
void noop_1(mp_limb_t n);
void noop_wxs(mp_ptr wp, mp_srcptr xp, mp_size_t size);
void noop_wxys(mp_ptr wp, mp_srcptr xp, mp_srcptr yp, mp_size_t size);
void mpn_cache_fill(mp_srcptr ptr, mp_size_t size);
void mpn_cache_fill_dummy(mp_limb_t n);
void speed_cache_fill(struct speed_params *s);
void speed_operand_src(struct speed_params *s, mp_ptr ptr, mp_size_t size);
void speed_operand_dst(struct speed_params *s, mp_ptr ptr, mp_size_t size);

extern int  speed_option_addrs;
extern int  speed_option_verbose;
void speed_option_set(const char *s);

mp_limb_t mpn_divrem_1_div(mp_ptr qp, mp_size_t xsize, mp_srcptr ap, mp_size_t size, mp_limb_t d);
mp_limb_t mpn_divrem_1_inv(mp_ptr qp, mp_size_t xsize, mp_srcptr ap, mp_size_t size, mp_limb_t d);
mp_limb_t mpn_divrem_2_div(mp_ptr qp, mp_size_t qxn, mp_ptr np, mp_size_t nsize, mp_srcptr dp);
mp_limb_t mpn_divrem_2_inv(mp_ptr qp, mp_size_t qxn, mp_ptr np, mp_size_t nsize, mp_srcptr dp);

int mpn_jacobi_base_1(mp_limb_t a, mp_limb_t b, int result_bit1);
int mpn_jacobi_base_2(mp_limb_t a, mp_limb_t b, int result_bit1);
int mpn_jacobi_base_3(mp_limb_t a, mp_limb_t b, int result_bit1);
int mpn_jacobi_base_4(mp_limb_t a, mp_limb_t b, int result_bit1);

mp_limb_t mpn_mod_1_div(mp_srcptr ap, mp_size_t size, mp_limb_t d);
mp_limb_t mpn_mod_1_inv(mp_srcptr ap, mp_size_t size, mp_limb_t d);

mp_size_t mpn_gcdext_one_double (mp_ptr, mp_ptr, mp_size_t *, mp_ptr, mp_size_t, mp_ptr, mp_size_t);
mp_size_t mpn_gcdext_one_single (mp_ptr, mp_ptr, mp_size_t *, mp_ptr, mp_size_t, mp_ptr, mp_size_t);
mp_size_t mpn_gcdext_single (mp_ptr, mp_ptr, mp_size_t *, mp_ptr, mp_size_t, mp_ptr, mp_size_t);
mp_size_t mpn_gcdext_double (mp_ptr, mp_ptr, mp_size_t *, mp_ptr, mp_size_t, mp_ptr, mp_size_t);
mp_size_t mpn_hgcd_lehmer (mp_ptr, mp_ptr, mp_size_t, struct hgcd_matrix *, mp_ptr);
mp_size_t mpn_hgcd_lehmer_itch (mp_size_t);

mp_size_t mpn_hgcd_appr_lehmer (mp_ptr, mp_ptr, mp_size_t, struct hgcd_matrix *, mp_ptr);
mp_size_t mpn_hgcd_appr_lehmer_itch (mp_size_t);

mp_size_t mpn_hgcd_reduce_1 (struct hgcd_matrix *, mp_ptr, mp_ptr, mp_size_t, mp_size_t, mp_ptr);
mp_size_t mpn_hgcd_reduce_1_itch (mp_size_t, mp_size_t);

mp_size_t mpn_hgcd_reduce_2 (struct hgcd_matrix *, mp_ptr, mp_ptr, mp_size_t, mp_size_t, mp_ptr);
mp_size_t mpn_hgcd_reduce_2_itch (mp_size_t, mp_size_t);

mp_size_t mpn_set_str_basecase(mp_ptr, const unsigned char *, size_t, int);
mp_size_t mpn_set_str_subquad(mp_ptr, const unsigned char *, size_t, int);

void mpn_toom3_mul_n_open(mp_ptr, mp_srcptr, mp_srcptr, mp_size_t, mp_ptr);
void mpn_toom4_mul_n_open(mp_ptr, mp_srcptr, mp_srcptr, mp_size_t);
void mpn_toom8h_mul_open(mp_ptr, mp_srcptr, mp_size_t, mp_srcptr, mp_size_t);

void mpn_toom3_sqr_n_open(mp_ptr, mp_srcptr, mp_size_t, mp_ptr);
void mpn_toom4_sqr_n_open(mp_ptr, mp_srcptr, mp_size_t, mp_ptr);
void mpn_toom8_sqr_n_open(mp_ptr, mp_srcptr, mp_size_t, mp_ptr);

void mpn_toom3_mul_n_mpn(mp_ptr, mp_srcptr, mp_srcptr, mp_size_t, mp_ptr);
void mpn_toom4_mul_n_mpn(mp_ptr, mp_srcptr, mp_srcptr, mp_size_t);
void mpn_toom8h_mul_mpn(mp_ptr, mp_srcptr, mp_size_t, mp_srcptr, mp_size_t);

void mpn_toom3_sqr_n_mpn(mp_ptr, mp_srcptr, mp_size_t, mp_ptr);
void mpn_toom4_sqr_n_mpn(mp_ptr, mp_srcptr, mp_size_t, mp_ptr);
void mpn_toom8_sqr_n_mpn(mp_ptr, mp_srcptr, mp_size_t, mp_ptr);

void mpz_powm_mod(mpz_ptr res, mpz_srcptr base, mpz_srcptr e, mpz_srcptr mod);
void mpz_powm_redc(mpz_ptr res, mpz_srcptr base, mpz_srcptr e, mpz_srcptr mod);
void redc(mp_ptr cp, mp_ptr tp, mp_srcptr mp, mp_size_t n, mp_limb_t Nprim);
void mpz_fac_ui_small(mpz_ptr,unsigned long);
void mpz_fac_ui_large(mpz_ptr,unsigned long);

int speed_routine_count_zeros_setup(struct speed_params *s, mp_ptr xp, int leading, int zero);


/* "get" is called repeatedly until it ticks over, just in case on a fast
   processor it takes less than a microsecond, though this is probably
   unlikely if it's a system call.

   speed_cyclecounter is called on the same side of the "get" for the start
   and end measurements.  It doesn't matter how long it takes from the "get"
   sample to the cycles sample, since that period will cancel out in the
   difference calculation (assuming it's the same each time).

   Letting the test run for more than a process time slice is probably only
   going to reduce accuracy, especially for getrusage when the cycle counter
   is real time, or for gettimeofday if the cycle counter is in fact process
   time.  Use CLK_TCK/2 as a reasonable stop.

   It'd be desirable to be quite accurate here.  The default speed_precision
   for a cycle counter is 10000 cycles, so to mix that with getrusage or
   gettimeofday the frequency should be at least that accurate.  But running
   measurements for 10000 microseconds (or more) is too long.  Be satisfied
   with just a half clock tick (5000 microseconds usually).  */

#define FREQ_MEASURE_ONE(name, type, get, getc, sec, usec)		\
  do {									\
    type      st1, st, et1, et;						\
    unsigned  sc[2], ec[2];						\
    long      dt, half_tick;						\
    double    dc, cyc;							\
									\
    half_tick = (1000000L / clk_tck()) / 2;				\
									\
    get (st1);								\
    do {								\
      get (st);								\
    } while (usec(st) == usec(st1) && sec(st) == sec(st1));		\
									\
    getc (sc);								\
									\
    for (;;)								\
      {									\
	get (et1);							\
	do {								\
	  get (et);							\
	} while (usec(et) == usec(et1) && sec(et) == sec(et1));		\
									\
	getc (ec);							\
									\
	dc = speed_cyclecounter_diff (ec, sc);				\
									\
	/* allow secs to cancel before multiplying */			\
	dt = sec(et) - sec(st);						\
	dt = dt * 1000000L + (usec(et) - usec(st));			\
									\
	if (dt >= half_tick)						\
	  break;							\
      }									\
									\
    cyc = dt * 1e-6 / dc;						\
									\
    if (speed_option_verbose >= 2)					\
      printf ("freq_measure_%s_one() dc=%.6g dt=%ld cyc=%.6g\n",	\
	      name, dc, dt, cyc);					\
									\
    return dt * 1e-6 / dc;						\
									\
  } while (0)




/* The measuring routines use these big macros to save duplication for
   similar forms.  They also get used for some automatically generated
   measuring of new implementations of functions.

   Having something like SPEED_ROUTINE_BINARY_N as a subroutine accepting a
   function pointer is considered undesirable since it's not the way a
   normal application will be calling, and some processors might do
   different things with an indirect call, like not branch predicting, or
   doing a full pipe flush.  At least some of the "functions" measured are
   actually macros too.

   The net effect is to bloat the object code, possibly in a big way, but
   only what's being measured is being run, so that doesn't matter.

   The loop forms don't try to cope with __GMP_ATTRIBUTE_PURE or
   ATTRIBUTE_CONST on the called functions.  Adding a cast to a non-pure
   function pointer doesn't work in gcc 3.2.  Using an actual non-pure
   function pointer variable works, but stands a real risk of a
   non-optimizing compiler generating unnecessary overheads in the call.
   Currently the best idea is not to use those attributes for a timing
   program build.  __GMP_NO_ATTRIBUTE_CONST_PURE will tell mpir.h and
   gmp-impl.h to omit them from routines there.  */

#define SPEED_RESTRICT_COND(cond)   if (!(cond)) return -1.0;

/* For mpn_copy or similar. */
#define SPEED_ROUTINE_MPN_COPY(function)				\
  {									\
    mp_ptr    wp;							\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 0);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (wp, s->size, s->align_wp);			\
									\
    speed_operand_src (s, s->xp, s->size);				\
    speed_operand_dst (s, wp, s->size);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function (wp, s->xp, s->size);					\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_COPYC(function)				\
  {									\
    mp_ptr    wp;							\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 0);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (wp, s->size, s->align_wp);			\
									\
    speed_operand_src (s, s->xp, s->size);				\
    speed_operand_dst (s, wp, s->size);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function (wp, s->xp, s->size, 0);					\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

/* s->size is still in limbs, and it's limbs which are copied, but
   "function" takes a size in bytes not limbs.	*/
#define SPEED_ROUTINE_MPN_COPY_BYTES(function)				\
  {									\
    mp_ptr    wp;							\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 0);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (wp, s->size, s->align_wp);			\
									\
    speed_operand_src (s, s->xp, s->size);				\
    speed_operand_dst (s, wp, s->size);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function (wp, s->xp, s->size * BYTES_PER_MP_LIMB);		\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }


/* For mpn_add_n, mpn_sub_n, or similar. */
#define SPEED_ROUTINE_MPN_BINARY_N_CALL(call)				\
  {									\
    mp_ptr     wp;							\
    mp_ptr     xp, yp;							\
    unsigned   i;							\
    double     t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (wp, s->size, s->align_wp);			\
									\
    xp = s->xp;								\
    yp = s->yp;								\
									\
    if (s->r == 0)	;						\
    else if (s->r == 1) { xp = wp;	    }				\
    else if (s->r == 2) {	   yp = wp; }				\
    else if (s->r == 3) { xp = wp; yp = wp; }				\
    else if (s->r == 4) {     yp = xp;	    }				\
    else		{						\
      TMP_FREE;								\
      return -1.0;							\
    }									\
									\
    /* initialize wp if operand overlap */				\
    if (xp == wp || yp == wp)						\
      MPN_COPY (wp, s->xp, s->size);					\
									\
    speed_operand_src (s, xp, s->size);					\
    speed_operand_src (s, yp, s->size);					\
    speed_operand_dst (s, wp, s->size);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      call;								\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

/* For mpn_add_n, mpn_sub_n, or similar. */
#define SPEED_ROUTINE_MPN_SUMDIFF_N_CALL(call)				\
  {									\
    mp_ptr     ap, sp;							\
    mp_ptr     xp, yp;							\
    unsigned   i;							\
    double     t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (ap, s->size, s->align_wp);			\
    SPEED_TMP_ALLOC_LIMBS (sp, s->size, s->align_wp);			\
									\
    xp = s->xp;								\
    yp = s->yp;								\
									\
    if ((s->r & 1) != 0) { xp = ap; }					\
    if ((s->r & 2) != 0) { yp = ap; }					\
    if ((s->r & 4) != 0) { xp = sp; }					\
    if ((s->r & 8) != 0) { yp = sp; }					\
    if ((s->r & 3) == 3  ||  (s->r & 12) == 12)				\
      {									\
	TMP_FREE;							\
	return -1.0;							\
      }									\
									\
    /* initialize ap if operand overlap */				\
    if (xp == ap || yp == ap)						\
      MPN_COPY (ap, s->xp, s->size);					\
    /* initialize sp if operand overlap */				\
    if (xp == sp || yp == sp)						\
      MPN_COPY (sp, s->xp, s->size);					\
									\
    speed_operand_src (s, xp, s->size);					\
    speed_operand_src (s, yp, s->size);					\
    speed_operand_dst (s, ap, s->size);					\
    speed_operand_dst (s, sp, s->size);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      call;								\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

// for addadd or addsub or subadd 
#define SPEED_ROUTINE_MPN_TRINARY_N(call)				\
  {									\
    mp_ptr     ap, sp;							\
    mp_ptr     xp, yp;							\
    unsigned   i;							\
    double     t;							\
    TMP_DECL;								\
    SPEED_RESTRICT_COND (s->size >= 1);					\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (ap, s->size, s->align_wp);			\
    SPEED_TMP_ALLOC_LIMBS (sp, s->size, s->align_wp2);			\
    xp = s->xp;								\
    yp = s->yp;								\
    speed_operand_src (s, xp, s->size);					\
    speed_operand_src (s, yp, s->size);					\
    speed_operand_src (s, sp, s->size);					\
    speed_operand_dst (s, ap, s->size);					\
    speed_cache_fill (s);						\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      call(ap,sp,xp,yp,s->size);					\
    while (--i != 0);							\
    t = speed_endtime ();						\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_ADD_ERR1_N(call)				\
  {									\
    mp_ptr     ap, sp;							\
    mp_ptr     xp, yp;							\
    mp_limb_t  ep[2];							\
    unsigned   i;							\
    double     t;							\
    TMP_DECL;								\
    SPEED_RESTRICT_COND (s->size >= 1);					\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (ap, s->size, s->align_wp);			\
    SPEED_TMP_ALLOC_LIMBS (sp, s->size, s->align_wp2);			\
    xp = s->xp;								\
    yp = s->yp;								\
    speed_operand_src (s, xp, s->size);					\
    speed_operand_src (s, yp, s->size);					\
    speed_operand_src (s, sp, s->size);					\
    speed_operand_dst (s, ap, s->size);					\
    speed_cache_fill (s);						\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      call(ap,sp,xp,ep,yp,s->size,0);					\
    while (--i != 0);							\
    t = speed_endtime ();						\
    TMP_FREE;								\
    return t;								\
  }


#define SPEED_ROUTINE_MPN_BINARY_N(function)				\
   SPEED_ROUTINE_MPN_BINARY_N_CALL ((*function) (wp, xp, yp, s->size))

#define SPEED_ROUTINE_MPN_BINARY_NC(function)				\
   SPEED_ROUTINE_MPN_BINARY_N_CALL ((*function) (wp, xp, yp, s->size, 0))


/* For mpn_lshift, mpn_rshift, mpn_mul_1, with r, or similar. */
#define SPEED_ROUTINE_MPN_UNARY_1_CALL(call)				\
  {									\
    mp_ptr    wp;							\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (wp, s->size, s->align_wp);			\
									\
    speed_operand_src (s, s->xp, s->size);				\
    speed_operand_dst (s, wp, s->size);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      call;								\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_SHIFTX(call)					\
  {									\
    mp_ptr    wp;							\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (wp, s->size, s->align_wp);			\
									\
    speed_operand_src (s, s->xp, s->size);				\
    speed_operand_dst (s, wp, s->size);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      call(wp,s->xp,s->size);						\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_UNARY_1(function)				\
  SPEED_ROUTINE_MPN_UNARY_1_CALL ((*function) (wp, s->xp, s->size, s->r))

#define SPEED_ROUTINE_MPN_UNARY_1C(function)				\
  SPEED_ROUTINE_MPN_UNARY_1_CALL ((*function) (wp, s->xp, s->size, s->r, 0))

/* FIXME: wp is uninitialized here, should start it off from xp */
#define SPEED_ROUTINE_MPN_UNARY_1_INPLACE(function)			\
  SPEED_ROUTINE_MPN_UNARY_1_CALL ((*function) (wp, wp, s->size, s->r))

#define SPEED_ROUTINE_MPN_DIVEXACT_1(function)				\
  SPEED_ROUTINE_MPN_UNARY_1_CALL ((*function) (wp, s->xp, s->size, s->r))

#define SPEED_ROUTINE_MPN_DIVEXACT_BYFOBM1(function)			\
  {									\
    mp_ptr    wp;							\
    unsigned  i;							\
    double    t;							\
    mp_limb_t inv=MP_LIMB_T_MAX/s->r;					\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (wp, s->size, s->align_wp);			\
									\
    speed_operand_src (s, s->xp, s->size);				\
    speed_operand_dst (s, wp, s->size);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      (*function) (wp, s->xp, s->size, s->r,inv);			\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_DIVREM_1(function)				\
  SPEED_ROUTINE_MPN_UNARY_1_CALL ((*function) (wp, 0, s->xp, s->size, s->r))

#define SPEED_ROUTINE_MPN_DIVREM_1C(function)				\
  SPEED_ROUTINE_MPN_UNARY_1_CALL ((*function) (wp, 0, s->xp, s->size, s->r, 0))

#define SPEED_ROUTINE_MPN_DIVREM_1F(function)				\
  SPEED_ROUTINE_MPN_UNARY_1_CALL ((*function) (wp, s->size, s->xp, 0, s->r))

#define SPEED_ROUTINE_MPN_DIVREM_1CF(function)				\
  SPEED_ROUTINE_MPN_UNARY_1_CALL ((*function) (wp, s->size, s->xp, 0, s->r, 0))


#define SPEED_ROUTINE_MPN_PREINV_DIVREM_1_CALL(call)			\
  {									\
    unsigned   shift;							\
    mp_limb_t  dinv;							\
									\
    SPEED_RESTRICT_COND (s->size >= 0);					\
    SPEED_RESTRICT_COND (s->r != 0);					\
									\
    count_leading_zeros (shift, s->r);					\
    invert_limb (dinv, s->r << shift);					\
									\
    SPEED_ROUTINE_MPN_UNARY_1_CALL (call);				\
  }									\

#define SPEED_ROUTINE_MPN_PREINV_DIVREM_1(function)			\
  SPEED_ROUTINE_MPN_PREINV_DIVREM_1_CALL				\
  ((*function) (wp, 0, s->xp, s->size, s->r, dinv, shift))

/* s->size limbs worth of fraction part */
#define SPEED_ROUTINE_MPN_PREINV_DIVREM_1F(function)			\
  SPEED_ROUTINE_MPN_PREINV_DIVREM_1_CALL				\
  ((*function) (wp, s->size, s->xp, 0, s->r, dinv, shift))


/* s->r is duplicated to form the multiplier, defaulting to
   MP_BASES_BIG_BASE_10.  Not sure if that's particularly useful, but at
   least it provides some control.  */
#define SPEED_ROUTINE_MPN_UNARY_N(function,N)				\
  {									\
    mp_ptr     wp;							\
    mp_size_t  wn;							\
    unsigned   i;							\
    double     t;							\
    mp_limb_t  yp[N];							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= N);					\
									\
    TMP_MARK;								\
    wn = s->size + N-1;							\
    SPEED_TMP_ALLOC_LIMBS (wp, wn, s->align_wp);			\
    for (i = 0; i < N; i++)						\
      yp[i] = (s->r != 0 ? s->r : MP_BASES_BIG_BASE_10);		\
									\
    speed_operand_src (s, s->xp, s->size);				\
    speed_operand_src (s, yp, (mp_size_t) N);				\
    speed_operand_dst (s, wp, wn);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function (wp, s->xp, s->size, yp);				\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_UNARY_2(function)				\
  SPEED_ROUTINE_MPN_UNARY_N (function, 2)
#define SPEED_ROUTINE_MPN_UNARY_3(function)				\
  SPEED_ROUTINE_MPN_UNARY_N (function, 3)
#define SPEED_ROUTINE_MPN_UNARY_4(function)				\
  SPEED_ROUTINE_MPN_UNARY_N (function, 4)
#define SPEED_ROUTINE_MPN_UNARY_5(function)				\
  SPEED_ROUTINE_MPN_UNARY_N (function, 5)
#define SPEED_ROUTINE_MPN_UNARY_6(function)				\
  SPEED_ROUTINE_MPN_UNARY_N (function, 6)
#define SPEED_ROUTINE_MPN_UNARY_7(function)				\
  SPEED_ROUTINE_MPN_UNARY_N (function, 7)
#define SPEED_ROUTINE_MPN_UNARY_8(function)				\
  SPEED_ROUTINE_MPN_UNARY_N (function, 8)


/* For mpn_mul_basecase, xsize=r, ysize=s->size. */
#define SPEED_ROUTINE_MPN_MUL_BASECASE(function)			\
  {									\
    mp_ptr    wp;							\
    mp_size_t size1;							\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    size1 = (s->r == 0 ? s->size : s->r);				\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
    SPEED_RESTRICT_COND (size1 >= s->size);				\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (wp, size1 + s->size, s->align_wp);		\
									\
    speed_operand_src (s, s->xp, size1);				\
    speed_operand_src (s, s->yp, s->size);				\
    speed_operand_dst (s, wp, size1 + s->size);				\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function (wp, s->xp, size1, s->yp, s->size);			\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_MUL_CALL(call, minsize)			\
  {									\
    mp_ptr    wp;							\
    mp_size_t size1;							\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    size1 = (s->r == 0 ? s->size : s->r);				\
									\
    SPEED_RESTRICT_COND (s->size >= minsize);					\
    SPEED_RESTRICT_COND (size1 >= s->size);				\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (wp, size1 + s->size, s->align_wp);		\
									\
    speed_operand_src (s, s->xp, size1);				\
    speed_operand_src (s, s->yp, s->size);				\
    speed_operand_dst (s, wp, size1 + s->size);				\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      call;			\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_MUL_N_CALL(call)				\
  {									\
    mp_ptr    wp;							\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (wp, 2*s->size, s->align_wp);			\
									\
    speed_operand_src (s, s->xp, s->size);				\
    speed_operand_src (s, s->yp, s->size);				\
    speed_operand_dst (s, wp, 2*s->size);				\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      call;								\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_MUL_N(function)				\
  SPEED_ROUTINE_MPN_MUL_N_CALL (function (wp, s->xp, s->yp, s->size));

#define SPEED_ROUTINE_MPN_MUL_N_TSPACE(call, tsize, minsize)		\
  {									\
    mp_ptr    wp, tspace;						\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= minsize);				\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (wp, 2*s->size, s->align_wp);			\
    SPEED_TMP_ALLOC_LIMBS (tspace, tsize, s->align_wp2);		\
									\
    speed_operand_src (s, s->xp, s->size);				\
    speed_operand_src (s, s->yp, s->size);				\
    speed_operand_dst (s, wp, 2*s->size);				\
    speed_operand_dst (s, tspace, tsize);				\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      call;								\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_MUL_N_SIZE(call, minsize)		\
  {									\
    mp_ptr    wp;						\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= minsize);				\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (wp, 2*s->size, s->align_wp);			\
    								\
    speed_operand_src (s, s->xp, s->size);				\
    speed_operand_src (s, s->yp, s->size);				\
    speed_operand_dst (s, wp, 2*s->size);				\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      call;								\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

/* For mpn_mulmid, mpn_mulmid_basecase, xsize=r, ysize=s->size. */
#define SPEED_ROUTINE_MPN_MULMID(function)				\
  {									\
    mp_ptr    wp, xp;							\
    mp_size_t size1;							\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    size1 = (s->r == 0 ? (2 * s->size - 1) : s->r);			\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
    SPEED_RESTRICT_COND (size1 >= s->size);				\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (wp, size1 - s->size + 3, s->align_wp);	\
    SPEED_TMP_ALLOC_LIMBS (xp, size1, s->align_xp);			\
									\
    speed_operand_src (s, xp, size1);					\
    speed_operand_src (s, s->yp, s->size);				\
    speed_operand_dst (s, wp, size1 - s->size + 3);			\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function (wp, xp, size1, s->yp, s->size);				\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_MULMID_N(function)				\
  {									\
    mp_ptr    wp, xp;							\
    mp_size_t size1;							\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    size1 = 2 * s->size - 1;						\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (wp, size1 - s->size + 3, s->align_wp);	\
    SPEED_TMP_ALLOC_LIMBS (xp, size1, s->align_xp);			\
									\
    speed_operand_src (s, xp, size1);					\
    speed_operand_src (s, s->yp, s->size);				\
    speed_operand_dst (s, wp, size1 - s->size + 3);			\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function (wp, xp, s->yp, s->size);				\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_TOOM42_MULMID(function)			\
  {									\
    mp_ptr    wp, xp, scratch;						\
    mp_size_t size1, scratch_size;					\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    size1 = 2 * s->size - 1;						\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (wp, size1 - s->size + 3, s->align_wp);	\
    SPEED_TMP_ALLOC_LIMBS (xp, size1, s->align_xp);			\
    scratch_size = mpn_toom42_mulmid_itch (s->size);			\
    SPEED_TMP_ALLOC_LIMBS (scratch, scratch_size, 0);			\
									\
    speed_operand_src (s, xp, size1);					\
    speed_operand_src (s, s->yp, s->size);				\
    speed_operand_dst (s, wp, size1 - s->size + 3);			\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function (wp, xp, s->yp, s->size, scratch);			\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

/* For mpn_mulmod_2expm1 , xsize=r, ysize=s->size. */
#define SPEED_ROUTINE_MPN_MULMOD_2EXPM1(function)			\
  {									\
    mp_ptr    wp,temps;							\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (wp, s->size, s->align_wp);			\
    SPEED_TMP_ALLOC_LIMBS (temps,5*s->size+64, s->align_wp);		\
									\
    speed_operand_src (s, s->xp, s->size);				\
    speed_operand_src (s, s->yp, s->size);				\
    speed_operand_dst (s, wp, s->size);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function (wp, s->xp, s->yp, GMP_NUMB_BITS*s->size,temps);		\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

/* For mpn_mulmod_2expp1_basecase , xsize=r, ysize=s->size. */
#define SPEED_ROUTINE_MPN_MULMOD_2EXPP1_BASECASE(function)			\
  {									\
    mp_ptr    wp,temps;							\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (wp, s->size, s->align_wp);			\
    SPEED_TMP_ALLOC_LIMBS (temps,2*s->size, s->align_wp);		\
									\
    speed_operand_src (s, s->xp, s->size);				\
    speed_operand_src (s, s->yp, s->size);				\
    speed_operand_dst (s, wp, s->size);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function (wp, s->xp, s->yp,0, GMP_NUMB_BITS*s->size,temps);		\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_KARA_MUL_N(function)				\
  SPEED_ROUTINE_MPN_MUL_N_TSPACE					\
    (function (wp, s->xp, s->xp, s->size, tspace),			\
     MPN_KARA_MUL_N_TSIZE (s->size),					\
     MPN_KARA_MUL_N_MINSIZE)

#define SPEED_ROUTINE_MPN_TOOM3_MUL_N(function)				\
  SPEED_ROUTINE_MPN_MUL_N_TSPACE					\
    (function (wp, s->xp, s->yp, s->size, tspace),			\
     MPN_TOOM3_MUL_N_TSIZE (s->size),					\
     MPN_TOOM3_MUL_N_MINSIZE)

#define SPEED_ROUTINE_MPN_TOOM4_MUL_N(function)				\
  SPEED_ROUTINE_MPN_MUL_N_SIZE					                  \
    (function (wp, s->xp, s->yp, s->size),			\
     MPN_TOOM4_MUL_N_MINSIZE)

#define SPEED_ROUTINE_MPN_TOOM8H_MUL(function)				\
  SPEED_ROUTINE_MPN_MUL_CALL					                  \
    (function (wp, s->xp, size1, s->yp, s->size),			\
     MPN_TOOM8H_MUL_MINSIZE)


#define SPEED_ROUTINE_MPN_SQR_CALL(call)				\
  {									\
    mp_ptr    wp;							\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (wp, 2*s->size, s->align_wp);			\
									\
    speed_operand_src (s, s->xp, s->size);				\
    speed_operand_dst (s, wp, 2*s->size);				\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      call;								\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_SQR(function)					\
  SPEED_ROUTINE_MPN_SQR_CALL (function (wp, s->xp, s->size))

#define SPEED_ROUTINE_MPN_SQR_DIAGONAL(function)			\
  SPEED_ROUTINE_MPN_SQR (function)


#define SPEED_ROUTINE_MPN_SQR_TSPACE(call, tsize, minsize)		\
  {									\
    mp_ptr    wp, tspace;						\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= minsize);				\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (wp, 2*s->size, s->align_wp);			\
    SPEED_TMP_ALLOC_LIMBS (tspace, tsize, s->align_wp2);		\
									\
    speed_operand_src (s, s->xp, s->size);				\
    speed_operand_dst (s, wp, 2*s->size);				\
    speed_operand_dst (s, tspace, tsize);				\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      call;								\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_SQR_N_SIZE(call, minsize)		\
  {									\
    mp_ptr    wp;						\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= minsize);				\
								\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (wp, 2*s->size, s->align_wp);			\
    speed_operand_src (s, s->xp, s->size);				\
    speed_operand_dst (s, wp, 2*s->size);				\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      call;								\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_KARA_SQR_N(function)				\
  SPEED_ROUTINE_MPN_SQR_TSPACE (function (wp, s->xp, s->size, tspace),	\
				MPN_KARA_SQR_N_TSIZE (s->size),		\
				MPN_KARA_SQR_N_MINSIZE)

#define SPEED_ROUTINE_MPN_TOOM3_SQR_N(function)				\
  SPEED_ROUTINE_MPN_SQR_TSPACE (function (wp, s->xp, s->size, tspace),	\
				MPN_TOOM3_SQR_N_TSIZE (s->size),	\
				MPN_TOOM3_SQR_N_MINSIZE)

#define SPEED_ROUTINE_MPN_TOOM4_SQR_N(function)				\
  SPEED_ROUTINE_MPN_SQR_N_SIZE (function (wp, s->xp, s->size),	\
				MPN_TOOM4_SQR_N_MINSIZE)

#define SPEED_ROUTINE_MPN_TOOM8_SQR_N(function)				\
  SPEED_ROUTINE_MPN_SQR_N_SIZE (function (wp, s->xp, s->size),	\
				MPN_TOOM8_SQR_N_MINSIZE)

#define SPEED_ROUTINE_MPN_MOD_CALL(call)				\
  {									\
    unsigned   i;							\
									\
    SPEED_RESTRICT_COND (s->size >= 0);					\
									\
    speed_operand_src (s, s->xp, s->size);				\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      call;								\
    while (--i != 0);							\
									\
    return speed_endtime ();						\
  }

#define SPEED_ROUTINE_MPN_MOD_1_K(call)				\
  {									\
    unsigned   i;							\
									\
    SPEED_RESTRICT_COND (s->size >= 0);					\
									\
    speed_operand_src (s, s->xp, s->size);				\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      mpn_mod_1_k(s->xp,s->size,2333,s->r);				\
    while (--i != 0);							\
									\
    return speed_endtime ();						\
  }


#define SPEED_ROUTINE_MPN_MOD_1(function)				\
   SPEED_ROUTINE_MPN_MOD_CALL ((*function) (s->xp, s->size, s->r))

#define SPEED_ROUTINE_MPN_MOD_1C(function)				\
   SPEED_ROUTINE_MPN_MOD_CALL ((*function)(s->xp, s->size, s->r, CNST_LIMB(0)))

#define SPEED_ROUTINE_MPN_MODEXACT_1_ODD(function)			\
  SPEED_ROUTINE_MPN_MOD_CALL (function (s->xp, s->size, s->r));

#define SPEED_ROUTINE_MPN_MODEXACT_1C_ODD(function)			\
  SPEED_ROUTINE_MPN_MOD_CALL (function (s->xp, s->size, s->r, CNST_LIMB(0)));

#define SPEED_ROUTINE_MPN_MOD_34LSUB1(function)				\
   SPEED_ROUTINE_MPN_MOD_CALL ((*function) (s->xp, s->size))

#define SPEED_ROUTINE_MPN_PREINV_MOD_1(function)			\
  {									\
    unsigned   i;							\
    mp_limb_t  inv;							\
									\
    SPEED_RESTRICT_COND (s->size >= 0);					\
    SPEED_RESTRICT_COND (s->r & GMP_LIMB_HIGHBIT);			\
									\
    invert_limb (inv, s->r);						\
    speed_operand_src (s, s->xp, s->size);				\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      (*function) (s->xp, s->size, s->r, inv);				\
    while (--i != 0);							\
									\
    return speed_endtime ();						\
  }


/* A division of 2*s->size by s->size limbs */

#define SPEED_ROUTINE_MPN_DC_DIVREM_CALL(call)				\
  {									\
    unsigned  i;							\
    mp_ptr    a, d, q, r;						\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (a, 2*s->size, s->align_xp);			\
    SPEED_TMP_ALLOC_LIMBS (d, s->size,   s->align_yp);			\
    SPEED_TMP_ALLOC_LIMBS (q, s->size+1, s->align_wp);			\
    SPEED_TMP_ALLOC_LIMBS (r, s->size,   s->align_wp2);			\
									\
    MPN_COPY (a, s->xp, s->size);					\
    MPN_COPY (a+s->size, s->xp, s->size);				\
									\
    MPN_COPY (d, s->yp, s->size);					\
									\
    /* normalize the data */						\
    d[s->size-1] |= GMP_NUMB_HIGHBIT;					\
    a[2*s->size-1] = d[s->size-1] - 1;					\
									\
    speed_operand_src (s, a, 2*s->size);				\
    speed_operand_src (s, d, s->size);					\
    speed_operand_dst (s, q, s->size+1);				\
    speed_operand_dst (s, r, s->size);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      call;								\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_DC_DIV_N_TSIZE(function, tsize)				\
  {									\
    unsigned  i;							\
    mp_ptr    a, d, q, tmp;						\
    mp_limb_t inv;                                             \
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 2);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (a, 4*s->size, s->align_xp);			\
    SPEED_TMP_ALLOC_LIMBS (d, 2*s->size,   s->align_yp);			\
    SPEED_TMP_ALLOC_LIMBS (q, 2*s->size+1, s->align_wp);			\
    SPEED_TMP_ALLOC_LIMBS (tmp, 2*tsize,   s->align_wp2);			\
    								\
    MPN_COPY (a, s->xp, s->size);					\
    MPN_COPY (a+s->size, s->xp, s->size);				\
    MPN_COPY (a+2*s->size, s->xp, s->size);				\
    MPN_COPY (a+3*s->size, s->xp, s->size);				\
									\
    MPN_COPY (d, s->yp, s->size);					\
    MPN_COPY (d+s->size, s->yp, s->size);					\
									\
    /* normalize the data */						\
    d[2*s->size-1] |= GMP_NUMB_HIGHBIT;					\
    a[4*s->size-1] = d[2*s->size-1] - 1;					\
									\
    speed_operand_src (s, a, 4*s->size);				\
    speed_operand_src (s, d, 2*s->size);					\
    speed_operand_dst (s, q, 2*s->size+1);				\
    speed_cache_fill (s);						\
	                                                         \
    mpir_invert_pi1(inv, d[2*s->size-1], d[2*s->size-2]);                        \
								\
    speed_starttime ();							\
    i = s->reps;							\
    do	{								\
      MPN_COPY (a, s->xp, s->size);					\
      MPN_COPY (a+s->size, s->xp, s->size);				\
      MPN_COPY (a+2*s->size, s->xp, s->size);				\
      MPN_COPY (a+3*s->size, s->xp, s->size);				\
      function(q, a, d, 2*s->size, inv, tmp);								\
    } while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }
#define SPEED_ROUTINE_MPN_TDIV_Q1(function)          \
{                                 \
    unsigned   i;                           \
    mp_ptr     dp, ap, tp, qp;                  \
    double     t;                           \
    mp_size_t itch, size1;                          \
    TMP_DECL;                               \
                                            \
    size1 = 3 * s->size;               \
                                        \
    TMP_MARK;                               \
    SPEED_TMP_ALLOC_LIMBS (dp, s->size, s->align_yp);           \
    SPEED_TMP_ALLOC_LIMBS (qp, 2*s->size, s->align_wp);           \
    SPEED_TMP_ALLOC_LIMBS (tp, size1, s->align_xp);     \
    SPEED_TMP_ALLOC_LIMBS (ap, size1, s->align_xp);     \
                                                         \
    MPN_COPY (ap,         s->xp, s->size);              \
    MPN_COPY (ap+s->size, s->xp, s->size);                \
    MPN_COPY (ap+2*s->size, s->xp, s->size);                \
                                                            \
    /* normalize the data */                        \
    dp[s->size-1] |= GMP_NUMB_HIGHBIT;                  \
    ap[size1-1] = dp[s->size-1] - 1;                \
                                                    \
    speed_operand_dst (s, qp, 2*s->size);                 \
    speed_operand_src (s, tp, size1);               \
    speed_operand_src (s, dp, s->size);                 \
    speed_cache_fill (s);                       \
                                                  \
    speed_starttime ();                         \
    i = s->reps;                            \
    do {                                \
        MPN_COPY(tp, ap, size1); \
        function (qp, tp, size1, dp, s->size);        \
    } while (--i != 0);                         \
    t = speed_endtime ();                       \
                                             \
    TMP_FREE;                               \
    return t;                               \
}
#define SPEED_ROUTINE_MPN_TDIV_Q(function)			\
  {									\
    unsigned   i;							\
    mp_ptr     dp, ap, tp, qp;					\
    double     t;							\
    mp_size_t itch, size1;							\
    TMP_DECL;								\
									\
    size1 = (s->r == 0 ? 2 * s->size : s->r);				\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (dp, s->size, s->align_yp);			\
    SPEED_TMP_ALLOC_LIMBS (qp, s->size, s->align_wp);			\
    SPEED_TMP_ALLOC_LIMBS (tp, size1, s->align_xp);		\
    SPEED_TMP_ALLOC_LIMBS (ap, size1, s->align_xp);		\
    								\
    MPN_COPY (ap,         s->xp, s->size);				\
    MPN_COPY (ap+size1-s->size, s->xp, s->size);				\
									\
    /* normalize the data */						\
    dp[s->size-1] |= GMP_NUMB_HIGHBIT;					\
    ap[size1-1] = dp[s->size-1] - 1;				\
									\
    speed_operand_dst (s, qp, s->size);					\
    speed_operand_src (s, tp, size1);				\
    speed_operand_src (s, dp, s->size);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do {								\
      MPN_COPY(tp, ap, size1); \
      function (qp, tp, size1, dp, s->size);		\
    } while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }
#define SPEED_ROUTINE_MPN_DC_DIV_SMALL_Q(function)				\
  {									\
    unsigned  i;							\
    mp_ptr    a, d, t, q;						\
    mp_limb_t inv;                                                \
    mp_size_t size1;   \
    double    td;       						\
    TMP_DECL;								\
									\
    size1 = (s->r == 0 ? 2 * s->size : s->r);				\
	 SPEED_RESTRICT_COND (s->size >= 2);					\
	 SPEED_RESTRICT_COND (size1 >= s->size);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (a, size1, s->align_xp);			\
    SPEED_TMP_ALLOC_LIMBS (t, size1, s->align_wp2);                 \
    SPEED_TMP_ALLOC_LIMBS (d, s->size,   s->align_yp);			\
    SPEED_TMP_ALLOC_LIMBS (q, size1 - s->size + 1, s->align_wp);			\
    						         		\
    MPN_COPY (a, s->xp, s->size);					\
    MPN_COPY (a+size1-s->size, s->xp, s->size);				\
									\
    MPN_COPY (d, s->yp, s->size);					\
									\
    /* normalize the data */						\
    d[s->size-1] |= GMP_NUMB_HIGHBIT;					\
    a[size1-1] = d[s->size-1] - 1;					\
									\
    speed_operand_src (s, a, size1);				\
    speed_operand_dst (s, t, size1);                                \
    speed_operand_src (s, d, s->size);					\
    speed_operand_dst (s, q, size1-s->size+1);				\
    speed_cache_fill (s);						\
	                                                                \
    mpir_invert_pi1(inv, d[s->size-1], d[s->size-2]);             \
								        \
    speed_starttime ();							\
    i = s->reps;							\
    do	{								\
      MPN_COPY (t, a, size1);					\
      function(q, t, size1, d, s->size, inv);		        \
    } while (--i != 0);							\
    td = speed_endtime ();						\
									\
    TMP_FREE;								\
    return td;								\
  }
#define SPEED_ROUTINE_MPN_SB_DIV_SMALL_Q(function)				\
  {									\
    unsigned  i;							\
    mp_ptr    a, d, t, q;						\
    mp_limb_t inv;                                                \
    mp_size_t size1;   \
    double    td;       						\
    TMP_DECL;								\
									\
    size1 = 2 * s->size;				\
	 SPEED_RESTRICT_COND (s->size >= 2);					\
	 SPEED_RESTRICT_COND (size1 >= s->size);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (a, size1, s->align_xp);			\
    SPEED_TMP_ALLOC_LIMBS (t, size1, s->align_wp2);                 \
    SPEED_TMP_ALLOC_LIMBS (d, s->size,   s->align_yp);			\
    SPEED_TMP_ALLOC_LIMBS (q, size1 - s->size + 1, s->align_wp);			\
    						         		\
    MPN_COPY (a, s->xp, s->size);					\
    MPN_COPY (a+size1-s->size, s->xp, s->size);				\
									\
    MPN_COPY (d, s->yp, s->size);					\
									\
    /* normalize the data */						\
    d[s->size-1] |= GMP_NUMB_HIGHBIT;					\
    a[size1-1] = d[s->size-1] - 1;					\
									\
    speed_operand_src (s, a, size1);				\
    speed_operand_dst (s, t, size1);                                \
    speed_operand_src (s, d, s->size);					\
    speed_operand_dst (s, q, size1-s->size+1);				\
    speed_cache_fill (s);						\
	                                                                \
    mpir_invert_pi1(inv, d[s->size-1], d[s->size-2]);             \
								        \
    speed_starttime ();							\
    i = s->reps;							\
    do	{								\
      MPN_COPY (t, a, size1);					\
      function(q, t, size1, d, s->size, inv);		        \
    } while (--i != 0);							\
    td = speed_endtime ();						\
									\
    TMP_FREE;								\
    return td;								\
  }
#define SPEED_ROUTINE_MPN_INV_DIV(function)				\
  {									\
    unsigned  i;							\
    mp_ptr    a, d, q, inv;						\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 2);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (a, 2*s->size, s->align_xp);			\
    SPEED_TMP_ALLOC_LIMBS (d, s->size,   s->align_yp);			\
    SPEED_TMP_ALLOC_LIMBS (q, s->size+1, s->align_wp);			\
    SPEED_TMP_ALLOC_LIMBS (inv, s->size, s->align_wp2);			\
    								\
    MPN_COPY (a, s->xp, s->size);					\
    MPN_COPY (a+s->size, s->xp, s->size);				\
									\
    MPN_COPY (d, s->yp, s->size);					\
									\
    /* normalize the data */						\
    d[s->size-1] |= GMP_NUMB_HIGHBIT;					\
    a[2*s->size-1] = d[s->size-1] - 1;					\
									\
    speed_operand_src (s, a, 2*s->size);				\
    speed_operand_src (s, d, s->size);					\
    speed_operand_dst (s, q, s->size+1);				\
    speed_cache_fill (s);						\
	                                                         \
    mpn_invert(inv, d, s->size);                        \
								\
    speed_starttime ();							\
    i = s->reps;							\
    do	{								\
      MPN_COPY (a, s->xp, s->size);					\
      MPN_COPY (a+s->size, s->xp, s->size);				\
      function(q, a, 2*s->size, d, s->size, inv);								\
    } while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_INV_DIV_SMALL_Q(function)				\
  {									\
    unsigned  i;							\
    mp_ptr    a, d, q, inv;						\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 2);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (a, 3*s->size, s->align_xp);			\
    SPEED_TMP_ALLOC_LIMBS (d, 2*s->size,   s->align_yp);			\
    SPEED_TMP_ALLOC_LIMBS (q, s->size+1, s->align_wp);			\
    SPEED_TMP_ALLOC_LIMBS (inv, 2*s->size, s->align_wp2);			\
    								\
    MPN_COPY (a, s->xp, s->size);					\
    MPN_COPY (a+s->size, s->xp, s->size);				\
    MPN_COPY (a+2*s->size, s->xp, s->size);				\
									\
    MPN_COPY (d, s->yp, s->size);					\
    MPN_COPY (d+s->size, s->yp, s->size);					\
									\
    /* normalize the data */						\
    d[2*s->size-1] |= GMP_NUMB_HIGHBIT;					\
    a[3*s->size-1] = d[2*s->size-1] - 1;					\
									\
    speed_operand_src (s, a, 3*s->size);				\
    speed_operand_src (s, d, 2*s->size);					\
    speed_operand_dst (s, q, s->size+1);				\
    speed_cache_fill (s);						\
	                                                         \
    mpn_invert(inv, d, 2*s->size);                        \
								\
    speed_starttime ();							\
    i = s->reps;							\
    do	{								\
      MPN_COPY (a, s->xp, s->size);					\
      MPN_COPY (a+s->size, s->xp, s->size);				\
      MPN_COPY (a+2*s->size, s->xp, s->size);				\
      function(q, a, 3*s->size, d, 2*s->size, inv);								\
    } while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_TDIV_SMALL_Q(function)				\
  {									\
    unsigned  i;							\
    mp_ptr    a, d, q;						\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 2);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (a, 3*s->size, s->align_xp);			\
    SPEED_TMP_ALLOC_LIMBS (d, 2*s->size,   s->align_yp);			\
    SPEED_TMP_ALLOC_LIMBS (q, s->size+1, s->align_wp);			\
    								\
    MPN_COPY (a, s->xp, s->size);					\
    MPN_COPY (a+s->size, s->xp, s->size);				\
    MPN_COPY (a+2*s->size, s->xp, s->size);				\
									\
    MPN_COPY (d, s->yp, s->size);					\
    MPN_COPY (d+s->size, s->yp, s->size);					\
									\
    /* normalize the data */						\
    d[2*s->size-1] |= GMP_NUMB_HIGHBIT;					\
    a[3*s->size-1] = d[2*s->size-1] - 1;					\
									\
    speed_operand_src (s, a, 3*s->size);				\
    speed_operand_src (s, d, 2*s->size);					\
    speed_operand_dst (s, q, s->size+1);				\
    speed_cache_fill (s);						\
	                                                         \
    speed_starttime ();							\
    i = s->reps;							\
    do	{								\
      MPN_COPY (a, s->xp, s->size);					\
      MPN_COPY (a+s->size, s->xp, s->size);				\
      MPN_COPY (a+2*s->size, s->xp, s->size);				\
      function(q, a, 3*s->size, d, 2*s->size);								\
    } while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_DC_DIVREM_N(function)				\
  SPEED_ROUTINE_MPN_DC_DIVREM_CALL((*function) (q, a, d, s->size))

#define SPEED_ROUTINE_MPN_DC_DIVREM_SB(function)			\
  SPEED_ROUTINE_MPN_DC_DIVREM_CALL					\
    ((*function) (q, a, 2*s->size, d, s->size))

#define SPEED_ROUTINE_MPN_DC_TDIV_QR(function)				\
  SPEED_ROUTINE_MPN_DC_DIVREM_CALL					\
    ((*function) (q, r, 0, a, 2*s->size, d, s->size))

#define SPEED_ROUTINE_MPN_DC_BDIV_N_TSIZE(function, tsize)				\
  {									\
    unsigned  i;							\
    mp_ptr    a, d, q, tmp;						\
    mp_limb_t inv;                                             \
    double    t;							\
    TMP_DECL;								\
	                                      \
    SPEED_RESTRICT_COND (s->size >= 2);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (a, 4*s->size, s->align_xp);			\
    SPEED_TMP_ALLOC_LIMBS (d, 2*s->size,   s->align_yp);			\
    SPEED_TMP_ALLOC_LIMBS (q, 2*s->size, s->align_wp);			\
    SPEED_TMP_ALLOC_LIMBS (tmp, tsize,   s->align_wp2);			\
    								\
    MPN_COPY (a, s->xp, s->size);					\
    MPN_COPY (a+s->size, s->xp, s->size);				\
    MPN_COPY (a+2*s->size, s->xp, s->size);				\
    MPN_COPY (a+3*s->size, s->xp, s->size);				\
									\
    MPN_COPY (d, s->yp, s->size);					\
    MPN_COPY (d+s->size, s->yp, s->size);					\
									\
    /* normalize the data */						\
    d[0] |= 1;					\
    								\
    speed_operand_src (s, a, 4*s->size);				\
    speed_operand_src (s, d, 2*s->size);					\
    speed_operand_dst (s, q, 2*s->size);				\
    speed_cache_fill (s);						\
	                                                         \
    modlimb_invert(inv, d[0]);                        \
								\
    speed_starttime ();							\
    i = s->reps;							\
    do	{								\
      MPN_COPY (a, s->xp, s->size);					\
      MPN_COPY (a+s->size, s->xp, s->size);				\
      MPN_COPY (a+2*s->size, s->xp, s->size);				\
      MPN_COPY (a+3*s->size, s->xp, s->size);				\
      function(q, a, d, 2*s->size, inv, tmp);								\
    } while (--i != 0);							\
    t = speed_endtime ();						\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_DC_BDIV_Q(function)				\
  {									\
    unsigned  i;							\
    mp_ptr    a, d, q;						\
    mp_limb_t inv;                                             \
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 2);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (a, s->size, s->align_xp);			\
    SPEED_TMP_ALLOC_LIMBS (d, s->size,   s->align_yp);			\
    SPEED_TMP_ALLOC_LIMBS (q, s->size, s->align_wp);			\
    								\
    MPN_COPY (a, s->xp, s->size);					\
    MPN_COPY (d, s->yp, s->size);					\
    								\
    /* normalize the data */						\
    d[0] |= 1;					\
    								\
    speed_operand_src (s, a, s->size);				\
    speed_operand_src (s, d, s->size);					\
    speed_operand_dst (s, q, s->size);				\
    speed_cache_fill (s);						\
	                                                         \
    modlimb_invert(inv, d[0]);                        \
								\
    speed_starttime ();							\
    i = s->reps;							\
    do	{								\
      MPN_COPY (a, s->xp, s->size);					\
      function(q, a, s->size, d, s->size, inv);								\
    } while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

/* A division of s->size by 3 limbs */

#define SPEED_ROUTINE_MPN_SB_DIVREM_M3(function)			\
  {									\
    unsigned   i;							\
    mp_ptr     a, d, q;							\
    mp_size_t  qsize;							\
    double     t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 3);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (a, s->size, s->align_xp);			\
									\
    SPEED_TMP_ALLOC_LIMBS (d, 3, s->align_yp);				\
    MPN_COPY (d, s->yp, 3);						\
    d[2] |= GMP_NUMB_HIGHBIT;						\
									\
    qsize = s->size - 3;						\
    SPEED_TMP_ALLOC_LIMBS (q, qsize, s->align_wp);			\
									\
    speed_operand_dst (s, a, s->size);					\
    speed_operand_src (s, d, 3);					\
    speed_operand_dst (s, q, qsize);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      {									\
	MPN_COPY (a, s->xp, s->size);					\
	function (q, a, s->size, d, 3);					\
      }									\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }


/* A remainder 2*s->size by s->size limbs */

#define SPEED_ROUTINE_MPZ_MOD(function)					\
  {									\
    unsigned   i;							\
    mpz_t      a, d, r;							\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    mpz_init_set_n (d, s->yp, s->size);					\
									\
    /* high part less than d, low part a duplicate copied in */		\
    mpz_init_set_n (a, s->xp, s->size);					\
    mpz_mod (a, a, d);							\
    mpz_mul_2exp (a, a, BITS_PER_MP_LIMB * s->size);			\
    MPN_COPY (PTR(a), s->xp, s->size);					\
									\
    mpz_init (r);							\
									\
    speed_operand_src (s, PTR(a), SIZ(a));				\
    speed_operand_src (s, PTR(d), SIZ(d));				\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function (r, a, d);						\
    while (--i != 0);							\
    return speed_endtime ();						\
  }

#define SPEED_ROUTINE_MPN_KARA(function)					\
  {									\
    unsigned   i;							\
    mp_ptr     rp, tp;						\
    double     t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 8);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (rp, 2*s->size, s->align_xp);		\
    SPEED_TMP_ALLOC_LIMBS (tp, s->size+1,     s->align_yp);		\
        								\
    MPN_COPY (rp,         s->xp, s->size);				\
    MPN_COPY (rp+s->size, s->yp, s->size);				\
    MPN_COPY (tp , s->yp, s->size);				\
									\
    speed_operand_dst (s, rp, 2*s->size);				\
    speed_operand_src (s, tp, s->size+1);				\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do {								\
      function (rp, tp, s->size);				\
    } while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_BINVERT(function,itchfn)			\
  {									\
    long  i;								\
    mp_ptr    up, tp, ip;						\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (ip, s->size, s->align_xp);			\
    SPEED_TMP_ALLOC_LIMBS (up, s->size,   s->align_yp);			\
    SPEED_TMP_ALLOC_LIMBS (tp, itchfn (s->size), s->align_wp);		\
									\
    MPN_COPY (up, s->xp, s->size);					\
									\
    /* normalize the data */						\
    up[0] |= 1;								\
									\
    speed_operand_src (s, up, s->size);					\
    speed_operand_dst (s, tp, s->size);					\
    speed_operand_dst (s, ip, s->size);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function (ip, up, s->size, tp);					\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_REDC_1(function)					\
  {									\
    unsigned   i;							\
    mp_ptr     cp, mp, tp, ap;						\
    mp_limb_t  inv;							\
    double     t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (ap, 2*s->size+1, s->align_xp);		\
    SPEED_TMP_ALLOC_LIMBS (mp, s->size,     s->align_yp);		\
    SPEED_TMP_ALLOC_LIMBS (cp, s->size,     s->align_wp);		\
    SPEED_TMP_ALLOC_LIMBS (tp, 2*s->size+1, s->align_wp2);		\
									\
    MPN_COPY (ap,         s->xp, s->size);				\
    MPN_COPY (ap+s->size, s->xp, s->size);				\
									\
    /* modulus must be odd */						\
    MPN_COPY (mp, s->yp, s->size);					\
    mp[0] |= 1;								\
    modlimb_invert (inv, mp[0]);						\
    inv = -inv;								\
									\
    speed_operand_src (s, ap, 2*s->size+1);				\
    speed_operand_dst (s, tp, 2*s->size+1);				\
    speed_operand_src (s, mp, s->size);					\
    speed_operand_dst (s, cp, s->size);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do {								\
      MPN_COPY (tp, ap, 2*s->size);					\
      function (cp, tp, mp, s->size, inv);				\
    } while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_REDC_2(function)					\
  {									\
    unsigned   i;							\
    mp_ptr     cp, mp, tp, ap;						\
    mp_limb_t  invp[2];							\
    double     t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (ap, 2*s->size+1, s->align_xp);		\
    SPEED_TMP_ALLOC_LIMBS (mp, s->size,     s->align_yp);		\
    SPEED_TMP_ALLOC_LIMBS (cp, s->size,     s->align_wp);		\
    SPEED_TMP_ALLOC_LIMBS (tp, 2*s->size+1, s->align_wp2);		\
									\
    MPN_COPY (ap,         s->xp, s->size);				\
    MPN_COPY (ap+s->size, s->xp, s->size);				\
									\
    /* modulus must be odd */						\
    MPN_COPY (mp, s->yp, s->size);					\
    mp[0] |= 1;								\
    mpn_binvert (invp, mp, 2, tp);					\
    invp[0] = -invp[0]; invp[1] = ~invp[1];				\
									\
    speed_operand_src (s, ap, 2*s->size+1);				\
    speed_operand_dst (s, tp, 2*s->size+1);				\
    speed_operand_src (s, mp, s->size);					\
    speed_operand_dst (s, cp, s->size);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do {								\
      MPN_COPY (tp, ap, 2*s->size);					\
      function (cp, tp, mp, s->size, invp);				\
    } while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }
#define SPEED_ROUTINE_REDC_N(function)					\
  {									\
    unsigned   i;							\
    mp_ptr     cp, mp, tp, ap, invp;					\
    double     t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size > 8);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (ap, 2*s->size+1, s->align_xp);		\
    SPEED_TMP_ALLOC_LIMBS (mp, s->size,     s->align_yp);		\
    SPEED_TMP_ALLOC_LIMBS (cp, s->size,     s->align_wp);		\
    SPEED_TMP_ALLOC_LIMBS (tp, 2*s->size+1, s->align_wp2);		\
    SPEED_TMP_ALLOC_LIMBS (invp, s->size,   s->align_wp2); /* align? */	\
									\
    MPN_COPY (ap,         s->xp, s->size);				\
    MPN_COPY (ap+s->size, s->xp, s->size);				\
									\
    /* modulus must be odd */						\
    MPN_COPY (mp, s->yp, s->size);					\
    mp[0] |= 1;								\
    mpn_binvert (invp, mp, s->size, tp);				\
									\
    speed_operand_src (s, ap, 2*s->size+1);				\
    speed_operand_dst (s, tp, 2*s->size+1);				\
    speed_operand_src (s, mp, s->size);					\
    speed_operand_dst (s, cp, s->size);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do {								\
      MPN_COPY (tp, ap, 2*s->size);					\
      function (cp, tp, mp, s->size, invp);				\
    } while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_POPCOUNT(function)				\
  {									\
    unsigned i;								\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    speed_operand_src (s, s->xp, s->size);				\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function (s->xp, s->size);					\
    while (--i != 0);							\
									\
    return speed_endtime ();						\
  }

#define SPEED_ROUTINE_MPN_HAMDIST(function)				\
  {									\
    unsigned i;								\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    speed_operand_src (s, s->xp, s->size);				\
    speed_operand_src (s, s->yp, s->size);				\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function (s->xp, s->yp, s->size);					\
    while (--i != 0);							\
									\
    return speed_endtime ();						\
  }


#define SPEED_ROUTINE_MPZ_UI(function)					\
  {									\
    mpz_t     z;							\
    unsigned  i;							\
    double    t;							\
									\
    SPEED_RESTRICT_COND (s->size >= 0);					\
									\
    mpz_init (z);							\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function (z, s->size);						\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    mpz_clear (z);							\
    return t;								\
  }

#define SPEED_ROUTINE_MPZ_FAC_UI(function)    SPEED_ROUTINE_MPZ_UI(function)
#define SPEED_ROUTINE_MPZ_FIB_UI(function)    SPEED_ROUTINE_MPZ_UI(function)
#define SPEED_ROUTINE_MPZ_LUCNUM_UI(function) SPEED_ROUTINE_MPZ_UI(function)


#define SPEED_ROUTINE_MPZ_2_UI(function)				\
  {									\
    mpz_t     z, z2;							\
    unsigned  i;							\
    double    t;							\
									\
    SPEED_RESTRICT_COND (s->size >= 0);					\
									\
    mpz_init (z);							\
    mpz_init (z2);							\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function (z, z2, s->size);					\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    mpz_clear (z);							\
    mpz_clear (z2);							\
    return t;								\
  }

#define SPEED_ROUTINE_MPZ_FIB2_UI(function)    SPEED_ROUTINE_MPZ_2_UI(function)
#define SPEED_ROUTINE_MPZ_LUCNUM2_UI(function) SPEED_ROUTINE_MPZ_2_UI(function)


#define SPEED_ROUTINE_MPN_FIB2_UI(function)				\
  {									\
    mp_ptr     fp, f1p;							\
    mp_size_t  alloc;							\
    unsigned   i;							\
    double     t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 0);					\
									\
    TMP_MARK;								\
    alloc = MPN_FIB2_SIZE (s->size);					\
    SPEED_TMP_ALLOC_LIMBS (fp,	alloc, s->align_xp);			\
    SPEED_TMP_ALLOC_LIMBS (f1p, alloc, s->align_yp);			\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function (fp, f1p, s->size);					\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }



/* Calculate b^e mod m for random b and m of s->size limbs and random e of 6
   limbs.  m is forced to odd so that redc can be used.  e is limited in
   size so the calculation doesn't take too long. */
#define SPEED_ROUTINE_MPZ_POWM(function)				\
  {									\
    mpz_t     r, b, e, m;						\
    unsigned  i;							\
    double    t;							\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    mpz_init (r);							\
    mpz_init_set_n (b, s->xp, s->size);					\
    mpz_init_set_n (m, s->yp, s->size);					\
    mpz_setbit (m, 0);	/* force m to odd */				\
    mpz_init_set_n (e, s->xp_block, 6);					\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function (r, b, e, m);						\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    mpz_clear (r);							\
    mpz_clear (b);							\
    mpz_clear (e);							\
    mpz_clear (m);							\
    return t;								\
  }

/* (m-2)^0xAAAAAAAA mod m */
#define SPEED_ROUTINE_MPZ_POWM_UI(function)				\
  {									\
    mpz_t     r, b, m;							\
    unsigned  long  e = (~ (unsigned long) 0) / 3;			\
    unsigned  i;							\
    double    t;							\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    mpz_init (r);							\
									\
    /* force m to odd */						\
    mpz_init (m);							\
    mpz_set_n (m, s->xp, s->size);					\
    PTR(m)[0] |= 1;							\
									\
    mpz_init_set (b, m);						\
    mpz_sub_ui (b, b, 2);						\
/* printf ("%X\n", mpz_get_ui(m)); */					\
    i = s->reps;							\
    speed_starttime ();							\
    do									\
      function (r, b, e, m);						\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    mpz_clear (r);							\
    mpz_clear (b);							\
    mpz_clear (m);							\
    return t;								\
  }


#define SPEED_ROUTINE_MPN_SUMDIFF_CALL(call)				\
  {									\
    mp_ptr    wp, wp2, xp, yp;						\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 0);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (wp,	s->size, s->align_wp);			\
    SPEED_TMP_ALLOC_LIMBS (wp2, s->size, s->align_wp2);			\
    xp = s->xp;								\
    yp = s->yp;								\
									\
    if (s->r == 0)	;						\
    else if (s->r == 1) { xp = wp;	      }				\
    else if (s->r == 2) {	    yp = wp2; }				\
    else if (s->r == 3) { xp = wp;  yp = wp2; }				\
    else if (s->r == 4) { xp = wp2; yp = wp;  }				\
    else {								\
      TMP_FREE;								\
      return -1.0;							\
    }									\
    if (xp != s->xp) MPN_COPY (xp, s->xp, s->size);			\
    if (yp != s->yp) MPN_COPY (yp, s->yp, s->size);			\
									\
    speed_operand_src (s, xp, s->size);					\
    speed_operand_src (s, yp, s->size);					\
    speed_operand_dst (s, wp, s->size);					\
    speed_operand_dst (s, wp2, s->size);				\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      call;								\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_SUMDIFF_N(function)				\
  SPEED_ROUTINE_MPN_SUMDIFF_CALL						\
    (function (wp, wp2, xp, yp, s->size));

#define SPEED_ROUTINE_MPN_SUMDIFF_NC(function)				\
  SPEED_ROUTINE_MPN_SUMDIFF_CALL						\
    (function (wp, wp2, xp, yp, s->size, 0));


/* Doing an Nx1 gcd with the given r. */
#define SPEED_ROUTINE_MPN_GCD_1N(function)				\
  {									\
    mp_ptr    xp;							\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
    SPEED_RESTRICT_COND (s->r != 0);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (xp, s->size, s->align_xp);			\
    MPN_COPY (xp, s->xp, s->size);					\
    xp[0] |= refmpn_zero_p (xp, s->size);				\
									\
    speed_operand_src (s, s->xp, s->size);				\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function (xp, s->size, s->r);					\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }


/* SPEED_BLOCK_SIZE many one GCDs of s->size bits each. */

#define SPEED_ROUTINE_MPN_GCD_1_CALL(setup, call)			\
  {									\
    unsigned  i, j;							\
    mp_ptr    px, py;							\
    mp_limb_t x_mask, y_mask;						\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
    SPEED_RESTRICT_COND (s->size <= mp_bits_per_limb);			\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (px, SPEED_BLOCK_SIZE, s->align_xp);		\
    SPEED_TMP_ALLOC_LIMBS (py, SPEED_BLOCK_SIZE, s->align_yp);		\
    MPN_COPY (px, s->xp_block, SPEED_BLOCK_SIZE);			\
    MPN_COPY (py, s->yp_block, SPEED_BLOCK_SIZE);			\
									\
    x_mask = MP_LIMB_T_LOWBITMASK (s->size);				\
    y_mask = MP_LIMB_T_LOWBITMASK (s->r != 0 ? s->r : s->size);		\
    for (i = 0; i < SPEED_BLOCK_SIZE; i++)				\
      {									\
	px[i] &= x_mask; px[i] += (px[i] == 0);				\
	py[i] &= y_mask; py[i] += (py[i] == 0);				\
	setup;								\
      }									\
									\
    speed_operand_src (s, px, SPEED_BLOCK_SIZE);			\
    speed_operand_src (s, py, SPEED_BLOCK_SIZE);			\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      {									\
	j = SPEED_BLOCK_SIZE;						\
	do								\
	  {								\
	    call;							\
	  }								\
	while (--j != 0);						\
      }									\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
									\
    s->time_divisor = SPEED_BLOCK_SIZE;					\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_GCD_1(function)				\
  SPEED_ROUTINE_MPN_GCD_1_CALL( , function (&px[j-1], 1, py[j-1]))

#define SPEED_ROUTINE_MPN_JACBASE(function)				\
  SPEED_ROUTINE_MPN_GCD_1_CALL						\
    ({									\
       /* require x<y, y odd, y!=1 */					\
       px[i] %= py[i];							\
       px[i] |= 1;							\
       py[i] |= 1;							\
       if (py[i]==1) py[i]=3;						\
     },									\
     function (px[j-1], py[j-1], 0))


#define SPEED_ROUTINE_MPN_HGCD_CALL(func, itchfunc)			\
  {									\
    mp_size_t hgcd_init_itch, hgcd_itch;				\
    mp_ptr ap, bp, wp, tmp1;						\
    struct hgcd_matrix hgcd;						\
    int res;								\
    unsigned i;								\
    double t;								\
    TMP_DECL;								\
									\
    if (s->size < 2)							\
      return -1;							\
									\
    TMP_MARK;								\
									\
    SPEED_TMP_ALLOC_LIMBS (ap, s->size + 1, s->align_xp);		\
    SPEED_TMP_ALLOC_LIMBS (bp, s->size + 1, s->align_yp);		\
									\
    s->xp[s->size - 1] |= 1;						\
    s->yp[s->size - 1] |= 1;						\
									\
    hgcd_init_itch = MPN_HGCD_MATRIX_INIT_ITCH (s->size);		\
    hgcd_itch = itchfunc (s->size);					\
									\
    SPEED_TMP_ALLOC_LIMBS (tmp1, hgcd_init_itch, s->align_wp);		\
    SPEED_TMP_ALLOC_LIMBS (wp, hgcd_itch, s->align_wp);			\
									\
    speed_operand_src (s, s->xp, s->size);				\
    speed_operand_src (s, s->yp, s->size);				\
    speed_operand_dst (s, ap, s->size + 1);				\
    speed_operand_dst (s, bp, s->size + 1);				\
    speed_operand_dst (s, wp, hgcd_itch);				\
    speed_operand_dst (s, tmp1, hgcd_init_itch);			\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      {									\
	MPN_COPY (ap, s->xp, s->size);					\
	MPN_COPY (bp, s->yp, s->size);					\
	mpn_hgcd_matrix_init (&hgcd, s->size, tmp1);			\
	res = func (ap, bp, s->size, &hgcd, wp);			\
      }									\
    while (--i != 0);							\
    t = speed_endtime ();						\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_HGCD_REDUCE_CALL(func, itchfunc)		\
  {									\
    mp_size_t hgcd_init_itch, hgcd_step_itch;				\
    mp_ptr ap, bp, wp, tmp1;						\
    struct hgcd_matrix hgcd;						\
    mp_size_t p = s->size/2;						\
    int res;								\
    unsigned i;								\
    double t;								\
    TMP_DECL;								\
									\
    if (s->size < 2)							\
      return -1;							\
									\
    TMP_MARK;								\
									\
    SPEED_TMP_ALLOC_LIMBS (ap, s->size + 1, s->align_xp);		\
    SPEED_TMP_ALLOC_LIMBS (bp, s->size + 1, s->align_yp);		\
									\
    s->xp[s->size - 1] |= 1;						\
    s->yp[s->size - 1] |= 1;						\
									\
    hgcd_init_itch = MPN_HGCD_MATRIX_INIT_ITCH (s->size);		\
    hgcd_step_itch = itchfunc (s->size, p);				\
									\
    SPEED_TMP_ALLOC_LIMBS (tmp1, hgcd_init_itch, s->align_wp);		\
    SPEED_TMP_ALLOC_LIMBS (wp, hgcd_step_itch, s->align_wp);			\
									\
    speed_operand_src (s, s->xp, s->size);				\
    speed_operand_src (s, s->yp, s->size);				\
    speed_operand_dst (s, ap, s->size + 1);				\
    speed_operand_dst (s, bp, s->size + 1);				\
    speed_operand_dst (s, wp, hgcd_step_itch);				\
    speed_operand_dst (s, tmp1, hgcd_init_itch);			\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      {									\
	MPN_COPY (ap, s->xp, s->size);					\
	MPN_COPY (bp, s->yp, s->size);					\
	mpn_hgcd_matrix_init (&hgcd, s->size, tmp1);			\
	res = func (&hgcd, ap, bp, s->size, p, wp);			\
      }									\
    while (--i != 0);							\
    t = speed_endtime ();						\
    TMP_FREE;								\
    return t;								\
  }

/* Run some GCDs of s->size limbs each.  The number of different data values
   is decreased as s->size**2, since GCD is a quadratic algorithm.
   SPEED_ROUTINE_MPN_GCD runs more times than SPEED_ROUTINE_MPN_GCDEXT
   though, because the plain gcd is about twice as fast as gcdext.  */

#define SPEED_ROUTINE_MPN_GCD_CALL(datafactor, call)			\
  {									\
    unsigned  i;							\
    mp_size_t j, pieces, psize;						\
    mp_ptr    wp, wp2, xtmp, ytmp, px, py;				\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (xtmp, s->size+1, s->align_xp);		\
    SPEED_TMP_ALLOC_LIMBS (ytmp, s->size+1, s->align_yp);		\
    SPEED_TMP_ALLOC_LIMBS (wp,   s->size+1, s->align_wp);		\
    SPEED_TMP_ALLOC_LIMBS (wp2,  s->size+1, s->align_wp2);		\
									\
    pieces = SPEED_BLOCK_SIZE * datafactor / s->size / s->size;		\
    pieces = MIN (pieces, SPEED_BLOCK_SIZE / s->size);			\
    pieces = MAX (pieces, 1);						\
									\
    psize = pieces * s->size;						\
    px = TMP_ALLOC_LIMBS (psize);					\
    py = TMP_ALLOC_LIMBS (psize);					\
    MPN_COPY (px, pieces==1 ? s->xp : s->xp_block, psize);		\
    MPN_COPY (py, pieces==1 ? s->yp : s->yp_block, psize);		\
									\
    /* Requirements: x >= y, y must be odd, high limbs != 0.		\
       No need to ensure random numbers are really great.  */		\
    for (j = 0; j < pieces; j++)					\
      {									\
	mp_ptr	x = px + j * s->size;					\
	mp_ptr	y = py + j * s->size;					\
	if (x[s->size - 1] == 0) x[s->size - 1] = 1;			\
	if (y[s->size - 1] == 0) y[s->size - 1] = 1;			\
									\
	if (x[s->size - 1] < y[s->size - 1])				\
	  MP_LIMB_T_SWAP (x[s->size - 1], y[s->size - 1]);		\
	else if (x[s->size - 1] == y[s->size - 1])			\
	  {								\
	    x[s->size - 1] = 2;						\
	    y[s->size - 1] = 1;						\
	  }								\
	y[0] |= 1;							\
      }									\
									\
    speed_operand_src (s, px, psize);					\
    speed_operand_src (s, py, psize);					\
    speed_operand_dst (s, xtmp, s->size);				\
    speed_operand_dst (s, ytmp, s->size);				\
    speed_operand_dst (s, wp, s->size);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      {									\
	j = pieces;							\
	do								\
	  {								\
	    MPN_COPY (xtmp, px+(j - 1)*s->size, s->size);		\
	    MPN_COPY (ytmp, py+(j - 1)*s->size, s->size);		\
	    call;							\
	  }								\
	while (--j != 0);						\
      }									\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
									\
    s->time_divisor = pieces;						\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_GCD(function)	\
  SPEED_ROUTINE_MPN_GCD_CALL (8, function (wp, xtmp, s->size, ytmp, s->size))

#define SPEED_ROUTINE_MPN_GCDEXT(function)				\
  SPEED_ROUTINE_MPN_GCD_CALL						\
    (4, { mp_size_t  wp2size;						\
	  function (wp, wp2, &wp2size, xtmp, s->size, ytmp, s->size); })



#define SPEED_ROUTINE_MPN_GCDEXT_ONE(function)				\
  {									\
    unsigned  i;							\
    mp_size_t j, pieces, psize, wp2size;				\
    mp_ptr    wp, wp2, xtmp, ytmp, px, py;				\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    TMP_MARK;								\
									\
    SPEED_TMP_ALLOC_LIMBS (xtmp, s->size+1, s->align_xp);		\
    SPEED_TMP_ALLOC_LIMBS (ytmp, s->size+1, s->align_yp);		\
    MPN_COPY (xtmp, s->xp, s->size);					\
    MPN_COPY (ytmp, s->yp, s->size);					\
									\
    SPEED_TMP_ALLOC_LIMBS (wp,	s->size+1, s->align_wp);		\
    SPEED_TMP_ALLOC_LIMBS (wp2, s->size+1, s->align_wp2);		\
									\
    pieces = SPEED_BLOCK_SIZE / 3;					\
    psize = 3 * pieces;							\
    px = TMP_ALLOC_LIMBS (psize);					\
    py = TMP_ALLOC_LIMBS (psize);					\
    MPN_COPY (px, s->xp_block, psize);					\
    MPN_COPY (py, s->yp_block, psize);					\
									\
    /* x must have at least as many bits as y,				\
       high limbs must be non-zero */					\
    for (j = 0; j < pieces; j++)					\
      {									\
	mp_ptr	x = px+3*j;						\
	mp_ptr	y = py+3*j;						\
	x[2] += (x[2] == 0);						\
	y[2] += (y[2] == 0);						\
	if (x[2] < y[2])						\
	  MP_LIMB_T_SWAP (x[2], y[2]);					\
      }									\
									\
    speed_operand_src (s, px, psize);					\
    speed_operand_src (s, py, psize);					\
    speed_operand_dst (s, xtmp, s->size);				\
    speed_operand_dst (s, ytmp, s->size);				\
    speed_operand_dst (s, wp, s->size);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      {									\
	mp_ptr	x = px;							\
	mp_ptr	y = py;							\
	mp_ptr	xth = &xtmp[s->size-3];					\
	mp_ptr	yth = &ytmp[s->size-3];					\
	j = pieces;							\
	do								\
	  {								\
	    xth[0] = x[0], xth[1] = x[1], xth[2] = x[2];		\
	    yth[0] = y[0], yth[1] = y[1], yth[2] = y[2];		\
									\
	    ytmp[0] |= 1; /* y must be odd, */				\
									\
	    function (wp, wp2, &wp2size, xtmp, s->size, ytmp, s->size);	\
									\
	    x += 3;							\
	    y += 3;							\
	  }								\
	while (--j != 0);						\
      }									\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
									\
    s->time_divisor = pieces;						\
    return t;								\
  }

#define SPEED_ROUTINE_MPZ_JACOBI(function)				\
  {									\
    mpz_t     a, b;							\
    unsigned  i;							\
    mp_size_t j, pieces, psize;						\
    mp_ptr    px, py;							\
    double    t;							\
    TMP_DECL;								\
									\
    TMP_MARK;								\
    pieces = SPEED_BLOCK_SIZE / MAX (s->size, 1);			\
    pieces = MAX (pieces, 1);						\
    s->time_divisor = pieces;						\
									\
    psize = pieces * s->size;						\
    px = TMP_ALLOC_LIMBS (psize);					\
    py = TMP_ALLOC_LIMBS (psize);					\
    MPN_COPY (px, pieces==1 ? s->xp : s->xp_block, psize);		\
    MPN_COPY (py, pieces==1 ? s->yp : s->yp_block, psize);		\
									\
    for (j = 0; j < pieces; j++)					\
      {									\
	mp_ptr	x = px+j*s->size;					\
	mp_ptr	y = py+j*s->size;					\
									\
	/* y odd */							\
	y[0] |= 1;							\
									\
	/* high limbs non-zero */					\
	if (x[s->size-1] == 0) x[s->size-1] = 1;			\
	if (y[s->size-1] == 0) y[s->size-1] = 1;			\
      }									\
									\
    SIZ(a) = s->size;							\
    SIZ(b) = s->size;							\
									\
    speed_operand_src (s, px, psize);					\
    speed_operand_src (s, py, psize);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      {									\
	j = pieces;							\
	do								\
	  {								\
	    PTR(a) = px+(j-1)*s->size;					\
	    PTR(b) = py+(j-1)*s->size;					\
	    function (a, b);						\
	  }								\
	while (--j != 0);						\
      }									\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_DIVREM_2(function)				\
  {									\
    mp_ptr    wp, xp;							\
    mp_limb_t yp[2];							\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 2);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (xp, s->size, s->align_xp);			\
    SPEED_TMP_ALLOC_LIMBS (wp, s->size, s->align_wp);			\
									\
    /* source is destroyed */						\
    MPN_COPY (xp, s->xp, s->size);					\
									\
    /* divisor must be normalized */					\
    MPN_COPY (yp, s->yp_block, 2);					\
    yp[1] |= GMP_NUMB_HIGHBIT;						\
									\
    speed_operand_src (s, xp, s->size);					\
    speed_operand_src (s, yp, 2);					\
    speed_operand_dst (s, wp, s->size);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function (wp, 0, xp, s->size, yp);				\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_DIVREME_2(function)				\
  {									\
    mp_ptr    wp, xp;							\
    mp_limb_t yp[2];							\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 2);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (xp, s->size, s->align_xp);			\
    SPEED_TMP_ALLOC_LIMBS (wp, s->size, s->align_wp);			\
									\
    /* source is destroyed */						\
    MPN_COPY (xp, s->xp, s->size);					\
									\
    /* divisor must be normalized */					\
    MPN_COPY (yp, s->yp_block, 2);					\
    yp[1] |= GMP_NUMB_HIGHBIT;						\
									\
    speed_operand_src (s, xp, s->size);					\
    speed_operand_src (s, yp, 2);					\
    speed_operand_dst (s, wp, s->size);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function (wp, xp, s->size, yp);					\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }


#define SPEED_ROUTINE_MODLIMB_INVERT(function)				\
  {									\
    unsigned   i, j;							\
    mp_ptr     xp;							\
    mp_limb_t  n = 1;							\
    double     t;							\
									\
    xp = s->xp_block-1;							\
									\
    speed_operand_src (s, s->xp_block, SPEED_BLOCK_SIZE);		\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      {									\
	j = SPEED_BLOCK_SIZE;						\
	do								\
	  {								\
	    /* randomized but successively dependent */			\
	    n += (xp[j] << 1);						\
									\
	    function (n, n);						\
	  }								\
	while (--j != 0);						\
      }									\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    /* make sure the compiler won't optimize away n */			\
    noop_1 (n);								\
									\
    s->time_divisor = SPEED_BLOCK_SIZE;					\
    return t;								\
  }


#define SPEED_ROUTINE_MPN_SQRTREM(function)				\
  {									\
    mp_ptr    wp, wp2;							\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (wp,	s->size, s->align_wp);			\
    SPEED_TMP_ALLOC_LIMBS (wp2, s->size, s->align_wp2);			\
									\
    speed_operand_src (s, s->xp, s->size);				\
    speed_operand_dst (s, wp, s->size);					\
    speed_operand_dst (s, wp2, s->size);				\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function (wp, wp2, s->xp, s->size);				\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_ROOTREM(function)				\
  {									\
    mp_ptr    wp, wp2;							\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (wp,	s->size, s->align_wp);			\
    SPEED_TMP_ALLOC_LIMBS (wp2, s->size, s->align_wp2);			\
									\
    speed_operand_src (s, s->xp, s->size);				\
    speed_operand_dst (s, wp, s->size);					\
    speed_operand_dst (s, wp2, s->size);				\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function (wp, wp2, s->xp, s->size, s->r);				\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }


/* s->size controls the number of limbs in the input, s->r is the base, or
   decimal by default. */
#define SPEED_ROUTINE_MPN_GET_STR(function)				\
  {									\
    unsigned char *wp;							\
    mp_size_t wn;							\
    mp_ptr xp;								\
    int base;								\
    unsigned i;								\
    double t;								\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    base = s->r == 0 ? 10 : s->r;					\
    SPEED_RESTRICT_COND (base >= 2 && base <= 256);			\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (xp, s->size + 1, s->align_xp);		\
									\
    MPN_SIZEINBASE (wn, s->xp, s->size, base);				\
    wp = TMP_ALLOC (wn);						\
									\
    /* use this during development to guard against overflowing wp */	\
    /*									\
    MPN_COPY (xp, s->xp, s->size);					\
    ASSERT_ALWAYS (mpn_get_str (wp, base, xp, s->size) <= wn);		\
    */									\
									\
    speed_operand_src (s, s->xp, s->size);				\
    speed_operand_dst (s, xp, s->size);					\
    speed_operand_dst (s, (mp_ptr) wp, wn/BYTES_PER_MP_LIMB);		\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      {									\
	MPN_COPY (xp, s->xp, s->size);					\
	function (wp, base, xp, s->size);				\
      }									\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

/* s->size controls the number of digits in the input, s->r is the base, or
   decimal by default. */
#define SPEED_ROUTINE_MPN_SET_STR(function)				\
  {									\
    unsigned char *xp;							\
    mp_ptr     wp;							\
    mp_size_t  wn;							\
    unsigned   i;							\
    int        base;							\
    double     t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 1);					\
									\
    base = s->r == 0 ? 10 : s->r;					\
    SPEED_RESTRICT_COND (base >= 2 && base <= 256);			\
									\
    TMP_MARK;								\
									\
    xp = TMP_ALLOC (s->size);						\
    for (i = 0; i < s->size; i++)					\
      xp[i] = s->xp[i] % base;						\
									\
    wn = ((mp_size_t) (s->size / __mp_bases[base].chars_per_bit_exactly)) \
      / BITS_PER_MP_LIMB + 2;						\
    SPEED_TMP_ALLOC_LIMBS (wp, wn, s->align_wp);			\
									\
    /* use this during development to check wn is big enough */		\
    /*									\
    ASSERT_ALWAYS (mpn_set_str (wp, xp, s->size, base) <= wn);		\
    */									\
									\
    speed_operand_src (s, (mp_ptr) xp, s->size/BYTES_PER_MP_LIMB);	\
    speed_operand_dst (s, wp, wn);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function (wp, xp, s->size, base);					\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }


/* "call" should do "count_foo_zeros(c,n)".
   Give leading=1 if foo is leading zeros, leading=0 for trailing.
   Give zero=1 if n=0 is allowed in the call, zero=0 if not.  */

#define SPEED_ROUTINE_COUNT_ZEROS_A(leading, zero)			\
  {									\
    mp_ptr     xp;							\
    int        i, c;							\
    unsigned   j;							\
    mp_limb_t  n;							\
    double     t;							\
    TMP_DECL;								\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (xp, SPEED_BLOCK_SIZE, s->align_xp);		\
									\
    if (! speed_routine_count_zeros_setup (s, xp, leading, zero))	\
      return -1.0;							\
    speed_operand_src (s, xp, SPEED_BLOCK_SIZE);			\
    speed_cache_fill (s);						\
									\
    c = 0;								\
    speed_starttime ();							\
    j = s->reps;							\
    do {								\
      for (i = 0; i < SPEED_BLOCK_SIZE; i++)				\
	{								\
	  n = xp[i];							\
	  n ^= c;							\

#define SPEED_ROUTINE_COUNT_ZEROS_B()					\
	}								\
    } while (--j != 0);							\
    t = speed_endtime ();						\
									\
    /* don't let c go dead */						\
    noop_1 (c);								\
									\
    s->time_divisor = SPEED_BLOCK_SIZE;					\
									\
    TMP_FREE;								\
    return t;								\
  }									\

#define SPEED_ROUTINE_COUNT_ZEROS_C(call, leading, zero)		\
  do {									\
    SPEED_ROUTINE_COUNT_ZEROS_A (leading, zero);			\
    call;								\
    SPEED_ROUTINE_COUNT_ZEROS_B ();					\
  } while (0)								\

#define SPEED_ROUTINE_COUNT_LEADING_ZEROS_C(call,zero)			\
  SPEED_ROUTINE_COUNT_ZEROS_C (call, 1, zero)
#define SPEED_ROUTINE_COUNT_LEADING_ZEROS(fun)				\
  SPEED_ROUTINE_COUNT_ZEROS_C (fun (c, n), 1, 0)

#define SPEED_ROUTINE_COUNT_TRAILING_ZEROS_C(call,zero)			\
  SPEED_ROUTINE_COUNT_ZEROS_C (call, 0, zero)
#define SPEED_ROUTINE_COUNT_TRAILING_ZEROS(call)			\
  SPEED_ROUTINE_COUNT_ZEROS_C (fun (c, n), 0, 0)


#define SPEED_ROUTINE_INVERT_LIMB_CALL(call)				\
  {									\
    unsigned   i, j;							\
    mp_limb_t  d, dinv=0;						\
    mp_ptr     xp = s->xp_block - 1;					\
									\
    s->time_divisor = SPEED_BLOCK_SIZE;					\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      {									\
	j = SPEED_BLOCK_SIZE;						\
	do								\
	  {								\
	    d = dinv ^ xp[j];						\
	    d |= GMP_LIMB_HIGHBIT;					\
	    do { call; } while (0);					\
	  }								\
	while (--j != 0);						\
      }									\
    while (--i != 0);							\
									\
    /* don't let the compiler optimize everything away */		\
    noop_1 (dinv);							\
									\
    return speed_endtime();						\
  }


#endif


#define SPEED_ROUTINE_MPN_BACK_TO_BACK(function)			\
  {									\
    unsigned  i;							\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      function ();							\
    while (--i != 0);							\
    return speed_endtime ();						\
  }


#define SPEED_ROUTINE_MPN_ZERO_CALL(call)				\
  {									\
    mp_ptr    wp;							\
    unsigned  i;							\
    double    t;							\
    TMP_DECL;								\
									\
    SPEED_RESTRICT_COND (s->size >= 0);					\
									\
    TMP_MARK;								\
    SPEED_TMP_ALLOC_LIMBS (wp, s->size, s->align_wp);			\
    speed_operand_dst (s, wp, s->size);					\
    speed_cache_fill (s);						\
									\
    speed_starttime ();							\
    i = s->reps;							\
    do									\
      call;								\
    while (--i != 0);							\
    t = speed_endtime ();						\
									\
    TMP_FREE;								\
    return t;								\
  }

#define SPEED_ROUTINE_MPN_ZERO(function)				\
  SPEED_ROUTINE_MPN_ZERO_CALL (function (wp, s->size))
