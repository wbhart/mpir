
#include "mpir.h"
#include "gmp-impl.h"

__GMP_DECLSPEC void __gmpn_sqr_n(mp_limb_t *rp, const mp_limb_t *s1p, mp_size_t n)
{
	mpn_sqr(rp, s1p, n);
}

__GMP_DECLSPEC void __gmp_randinit(gmp_randstate_t state, gmp_randalg_t alg, ...)
{
	gmp_randinit_lc_2exp_size(state, GMP_LIMB_BITS);
}

__GMP_DECLSPEC void __gmpz_random(mpz_t rop, mp_size_t max_size)
{	gmp_randstate_t state;
	mp_size_t bits = GMP_LIMB_BITS * (max_size < 0 ? -max_size : max_size);
	gmp_randinit_lc_2exp_size(state, GMP_LIMB_BITS);
	mpz_urandomb(rop, state, bits);
	if(max_size < 0)
		rop->_mp_size = -rop->_mp_size;
}

__GMP_DECLSPEC void __gmpz_random2 (mpz_t rop, mp_size_t max_size)
{	gmp_randstate_t state;
	mp_size_t bits = GMP_LIMB_BITS * (max_size < 0 ? -max_size : max_size);
	gmp_randinit_lc_2exp_size(state, GMP_LIMB_BITS);
	mpz_rrandomb(rop, state, bits);
	if(max_size < 0)
		rop->_mp_size = -rop->_mp_size;
}

