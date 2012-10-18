/* 

Copyright 2009, 2011 William Hart. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of
      conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list
      of conditions and the following disclaimer in the documentation and/or other materials
      provided with the distribution.

THIS SOFTWARE IS PROVIDED BY William Hart ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL William Hart OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those of the
authors and should not be interpreted as representing official policies, either expressed
or implied, of William Hart.

*/

#include "mpir.h"
#include "gmp-impl.h"
      
void mpir_fft_trunc1(mp_ptr * ii, mp_size_t n, mp_bitcnt_t w, 
                   mp_ptr * t1, mp_ptr * t2, mp_size_t trunc)
{
    mp_size_t i;
    mp_size_t limbs = (w*n)/GMP_LIMB_BITS;
   
    if (trunc == 2*n)
        mpir_fft_radix2(ii, n, w, t1, t2);
    else if (trunc <= n)
    {
        for (i = 0; i < n; i++)
            mpn_add_n(ii[i], ii[i], ii[i+n], limbs + 1);
      
        mpir_fft_trunc1(ii, n/2, 2*w, t1, t2, trunc);
    } else
    {
        for (i = 0; i < n; i++) 
        {   
            mpir_fft_butterfly(*t1, *t2, ii[i], ii[n+i], i, limbs, w);
   
            MP_PTR_SWAP(ii[i],   *t1);
            MP_PTR_SWAP(ii[n+i], *t2);
        }

        mpir_fft_radix2(ii, n/2, 2*w, t1, t2);
        mpir_fft_trunc1(ii+n, n/2, 2*w, t1, t2, trunc - n);
   }
}

void mpir_fft_trunc(mp_ptr * ii,  mp_size_t n, mp_bitcnt_t w, 
                  mp_ptr * t1, mp_ptr * t2, mp_size_t trunc)
{
    mp_size_t i;
    mp_size_t limbs = (w*n)/GMP_LIMB_BITS;
   
    if (trunc == 2*n)
       mpir_fft_radix2(ii, n, w, t1, t2);
    else if (trunc <= n)
       mpir_fft_trunc(ii, n/2, 2*w, t1, t2, trunc);
    else
    {
        for (i = 0; i < trunc - n; i++) 
        {   
            mpir_fft_butterfly(*t1, *t2, ii[i], ii[n+i], i, limbs, w);
   
            MP_PTR_SWAP(ii[i],   *t1);
            MP_PTR_SWAP(ii[n+i], *t2);
        }

        for ( ; i < n; i++)
            mpir_fft_adjust(ii[i+n], ii[i], i, limbs, w); 
   
        mpir_fft_radix2(ii, n/2, 2*w, t1, t2);
        mpir_fft_trunc1(ii+n, n/2, 2*w, t1, t2, trunc - n);
   }
}
