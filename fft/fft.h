/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2009 William Hart

******************************************************************************/

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"

mp_limb_t n_revbin(mp_limb_t in, mp_limb_t bits);

void fft_adjust(mp_limb_t * r, mp_limb_t * i1, mp_size_t i, mp_size_t limbs, mp_bitcnt_t w);

void fft_adjust_sqrt2(mp_limb_t * r, mp_limb_t * i1, mp_size_t i, mp_size_t limbs, mp_bitcnt_t w, mp_limb_t * temp);

void butterfly_lshB(mp_limb_t * t, mp_limb_t * u, mp_limb_t * i1, mp_limb_t * i2, mp_size_t limbs, mp_size_t x, mp_size_t y);

void butterfly_rshB(mp_limb_t * t, mp_limb_t * u, mp_limb_t * i1, mp_limb_t * i2, mp_size_t limbs, mp_size_t x, mp_size_t y);

void fft_combine_limbs(mp_limb_t * res, mp_limb_t ** poly, long length, mp_size_t coeff_limbs, mp_size_t output_limbs, mp_size_t total_limbs);

mp_size_t fft_split_limbs(mp_limb_t ** poly, mp_limb_t * limbs, mp_size_t total_limbs, mp_size_t coeff_limbs, mp_size_t output_limbs);

void fermat_to_mpz(mpz_t m, mp_limb_t * i, mp_size_t limbs);

void fft_butterfly(mp_limb_t * s, mp_limb_t * t, mp_limb_t * i1, mp_limb_t * i2, mp_size_t i, mp_size_t limbs, mp_bitcnt_t w);

void ifft_butterfly(mp_limb_t * s, mp_limb_t * t, mp_limb_t * i1, mp_limb_t * i2, mp_size_t i, mp_size_t limbs, mp_bitcnt_t w);

void fft_truncate1(mp_limb_t ** ii, mp_size_t n, mp_bitcnt_t w, mp_limb_t ** t1, mp_limb_t ** t2, mp_size_t trunc);

void ifft_truncate1(mp_limb_t ** ii, mp_size_t n, mp_bitcnt_t w, mp_limb_t ** t1, mp_limb_t ** t2, mp_size_t trunc);

void fft_butterfly_sqrt2(mp_limb_t * s, mp_limb_t * t, mp_limb_t * i1, mp_limb_t * i2, mp_size_t i, 
                                mp_size_t limbs, mp_bitcnt_t w, mp_limb_t * temp);

void ifft_butterfly_sqrt2(mp_limb_t * s, mp_limb_t * t, mp_limb_t * i1, 
   mp_limb_t * i2, mp_size_t i, mp_size_t limbs, mp_bitcnt_t w, mp_limb_t * temp);

void fft_butterfly_twiddle (mp_limb_t * u, mp_limb_t * v, 
   mp_limb_t * s, mp_limb_t * t, mp_size_t limbs, mp_bitcnt_t b1, mp_bitcnt_t b2);

void ifft_butterfly_twiddle(mp_limb_t * u, mp_limb_t * v, 
   mp_limb_t * s, mp_limb_t * t, mp_size_t limbs, mp_bitcnt_t b1, mp_bitcnt_t b2);

void fft_radix2_twiddle(mp_limb_t ** ii, mp_size_t is, mp_size_t n, mp_bitcnt_t w, mp_limb_t ** t1, mp_limb_t ** t2,
                            mp_size_t ws, mp_size_t r, mp_size_t c, mp_size_t rs);

void ifft_radix2_twiddle(mp_limb_t ** ii, mp_size_t is, mp_size_t n, mp_bitcnt_t w, mp_limb_t ** t1, mp_limb_t ** t2,
                            mp_size_t ws, mp_size_t r, mp_size_t c, mp_size_t rs);

void fft_truncate1_twiddle(mp_limb_t ** ii, mp_size_t is, mp_size_t n, mp_bitcnt_t w, mp_limb_t ** t1, mp_limb_t ** t2,
           mp_size_t ws, mp_size_t r, mp_size_t c, mp_size_t rs, mp_size_t trunc);

void ifft_truncate1_twiddle(mp_limb_t ** ii, mp_size_t is, mp_size_t n, mp_bitcnt_t w, mp_limb_t ** t1, mp_limb_t ** t2,
           mp_size_t ws, mp_size_t r, mp_size_t c, mp_size_t rs, mp_size_t trunc);

void fft_naive_convolution_1(mp_limb_t * r, mp_limb_t * ii, mp_limb_t * jj, mp_size_t m);

void _fft_mulmod_2expp1(mp_limb_t * r1, mp_limb_t * i1, mp_limb_t * i2, mp_size_t r_limbs, mp_bitcnt_t depth, mp_bitcnt_t w);

/* keep what follows in case declarations have to be moved to gmp-impl.h */

#if 0

#define fft_adjust __MPN(fft_adjust)
__GMP_DECLSPEC void fft_adjust __GMP_PROTO ((mp_limb_t * r, mp_limb_t * i1, 
                                     mp_size_t i, mp_size_t limbs, mp_bitcnt_t w));

#define fft_adjust_sqrt2 __MPN(fft_adjust_sqrt2)
__GMP_DECLSPEC void fft_adjust_sqrt2 __GMP_PROTO ((mp_limb_t * r, mp_limb_t * i1, 
                   mp_size_t i, mp_size_t limbs, mp_bitcnt_t w, mp_limb_t * temp));

#define butterfly_lshB __MPN(butterfly_lshB)
__GMP_DECLSPEC void butterfly_lshB __GMP_PROTO ((mp_limb_t * t, mp_limb_t * u, mp_limb_t * i1, 
                       mp_limb_t * i2, mp_size_t limbs, mp_size_t x, mp_size_t y));

#define butterfly_rshB __MPN(butterfly_rshB)
__GMP_DECLSPEC void butterfly_rshB __GMP_PROTO ((mp_limb_t * t, mp_limb_t * u, mp_limb_t * i1, 
                       mp_limb_t * i2, mp_size_t limbs, mp_size_t x, mp_size_t y));

#define fft_combine_limbs __MPN(combine_limbs)
__GMP_DECLSPEC void fft_combine_limbs __GMP_PROTO ((mp_limb_t * res, mp_limb_t ** poly, long length, 
            mp_size_t coeff_limbs, mp_size_t output_limbs, mp_size_t total_limbs));

#define fft_split_limbs __MPN(fft_split_limbs)
__GMP_DECLSPEC mp_size_t fft_split_limbs __GMP_PROTO ((mp_limb_t ** poly, mp_limb_t * limbs, 
            mp_size_t total_limbs, mp_size_t coeff_limbs, mp_size_t output_limbs));

#define fermat_to_mpz __MPN(fermat_to_mpz)
__GMP_DECLSPEC void fermat_to_mpz __GMP_PROTO ((mpz_t m, mp_limb_t * i, mp_size_t limbs));

#define fft_butterfly __MPN(fft_butterfly)
__GMP_DECLSPEC void fft_butterfly __GMP_PROTO ((mp_limb_t * s, mp_limb_t * t, mp_limb_t * i1, 
                     mp_limb_t * i2, mp_size_t i, mp_size_t limbs, mp_bitcnt_t w));

#define ifft_butterfly __MPN(ifft_butterfly)
__GMP_DECLSPEC void ifft_butterfly __GMP_PROTO ((mp_limb_t * s, mp_limb_t * t, mp_limb_t * i1, 
                     mp_limb_t * i2, mp_size_t i, mp_size_t limbs, mp_bitcnt_t w));

#define fft_truncate1 __MPN(fft_truncate1)
__GMP_DECLSPEC void fft_truncate1 __GMP_PROTO ((mp_limb_t ** ii, mp_size_t n, mp_bitcnt_t w, 
                               mp_limb_t ** t1, mp_limb_t ** t2, mp_size_t trunc));

#define ifft_truncate1 __MPN(ifft_truncate1)
__GMP_DECLSPEC void ifft_truncate1 __GMP_PROTO ((mp_limb_t ** ii, mp_size_t n, mp_bitcnt_t w, 
                               mp_limb_t ** t1, mp_limb_t ** t2, mp_size_t trunc));

#define fft_butterfly_sqrt2 __MPN(fft_butterfly_sqrt2)
__GMP_DECLSPEC void fft_butterfly_sqrt2 __GMP_PROTO ((mp_limb_t * s, mp_limb_t * t, 
                         mp_limb_t * i1, mp_limb_t * i2, mp_size_t i, 
                                mp_size_t limbs, mp_bitcnt_t w, mp_limb_t * temp));

#define ifft_butterfly_sqrt2 __MPN(ifft_butterfly_sqrt2)
__GMP_DECLSPEC void ifft_butterfly_sqrt2 __GMP_PROTO ((mp_limb_t * s, mp_limb_t * t, mp_limb_t * i1, 
   mp_limb_t * i2, mp_size_t i, mp_size_t limbs, mp_bitcnt_t w, mp_limb_t * temp));

#define fft_butterfly_twiddle __MPN(fft_butterfly_twiddle)
__GMP_DECLSPEC void fft_butterfly_twiddle __GMP_PROTO ((mp_limb_t * u, mp_limb_t * v, 
   mp_limb_t * s, mp_limb_t * t, mp_size_t limbs, mp_bitcnt_t b1, mp_bitcnt_t b2));

#define ifft_butterfly_twiddle __MPN(ifft_butterfly_twiddle)
__GMP_DECLSPEC void ifft_butterfly_twiddle __GMP_PROTO ((mp_limb_t * u, mp_limb_t * v, 
   mp_limb_t * s, mp_limb_t * t, mp_size_t limbs, mp_bitcnt_t b1, mp_bitcnt_t b2));

#define fft_radix2_twiddle __MPN(fft_radix2_twiddle)
__GMP_DECLSPEC void fft_radix2_twiddle __GMP_PROTO ((mp_limb_t ** ii, mp_size_t is,
      mp_size_t n, mp_bitcnt_t w, mp_limb_t ** t1, mp_limb_t ** t2,
                            mp_size_t ws, mp_size_t r, mp_size_t c, mp_size_t rs));

#define ifft_radix2_twiddle __MPN(ifft_radix2_twiddle)
__GMP_DECLSPEC void ifft_radix2_twiddle __GMP_PROTO ((mp_limb_t ** ii, mp_size_t is,
        mp_size_t n, mp_bitcnt_t w, mp_limb_t ** t1, mp_limb_t ** t2,
                            mp_size_t ws, mp_size_t r, mp_size_t c, mp_size_t rs));

#define fft_truncate1_twiddle __MPN(fft_truncate1_twiddle)
__GMP_DECLSPEC void fft_truncate1_twiddle __GMP_PROTO ((mp_limb_t ** ii, mp_size_t is,
        mp_size_t n, mp_bitcnt_t w, mp_limb_t ** t1, mp_limb_t ** t2,
           mp_size_t ws, mp_size_t r, mp_size_t c, mp_size_t rs, mp_size_t trunc));

#define ifft_truncate1_twiddle __MPN(ifft_truncate1_twiddle)
__GMP_DECLSPEC void ifft_truncate1_twiddle __GMP_PROTO ((mp_limb_t ** ii, mp_size_t is,
        mp_size_t n, mp_bitcnt_t w, mp_limb_t ** t1, mp_limb_t ** t2,
           mp_size_t ws, mp_size_t r, mp_size_t c, mp_size_t rs, mp_size_t trunc));

#define fft_naive_convolution_1 __MPN(fft_naive_convolution_1)
__GMP_DECLSPEC void fft_naive_convolution_1 __GMP_PROTO ((mp_limb_t * r, mp_limb_t * ii, 
                                                     mp_limb_t * jj, mp_size_t m));

#define _fft_mulmod_2expp1 __MPN(_fft_mulmod_2expp1)
__GMP_DECLSPEC void _fft_mulmod_2expp1 __GMP_PROTO ((mp_limb_t * r1, mp_limb_t * i1, mp_limb_t * i2, 
                             mp_size_t r_limbs, mp_bitcnt_t depth, mp_bitcnt_t w));

#endif
