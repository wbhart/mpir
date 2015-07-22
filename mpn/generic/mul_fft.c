
#include "mpir.h"
#include "gmp-impl.h"

int 
mpn_mul_fft(mp_ptr rp, mp_size_t rn, mp_srcptr ap, mp_size_t an,
    mp_srcptr bp, mp_size_t bn, int k)
{
    mp_ptr rpp, app, bpp, tpp;
    mp_size_t t = rn + 1;
    TMP_DECL;
    TMP_MARK;

    rpp = (mp_ptr)TMP_ALLOC_LIMBS(t);
    tpp = (mp_ptr)TMP_ALLOC_LIMBS(t);
    app = (mp_ptr)TMP_ALLOC_LIMBS(t);
    bpp = (mp_ptr)TMP_ALLOC_LIMBS(t);

    mpn_copyi(app, ap, an); mpn_zero(app + an, t - an);
    mpn_copyi(bpp, bp, bn); mpn_zero(bpp + bn, t - bn);

    mpn_mulmod_Bexpp1(rpp, app, bpp, rn, tpp);
    mpn_copyi(rp, rpp, rn);
    t = rpp[rn];

    TMP_FREE;
    return t;
}
