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
#include "longlong.h"

static mp_size_t mulmod_2expp1_table_n[FFT_N_NUM] = MULMOD_TAB;

int mpn_mulmod_Bexpp1(mp_ptr r, mp_srcptr i1, mp_srcptr i2, mp_size_t limbs, mp_ptr tt)
{
   mp_size_t bits = limbs * GMP_LIMB_BITS;
   mp_bitcnt_t depth1, depth = 1;
   mp_size_t w1, off;

   mp_limb_t c = 2 * i1[limbs] + i2[limbs];
   
   if (c & 1)
   {
      mpn_neg_n(r, i1, limbs + 1);
      mpn_normmod_2expp1(r, limbs);
      return 0;
   } else if (c & 2)
   {
      mpn_neg_n(r, i2, limbs + 1);
      mpn_normmod_2expp1(r, limbs);
      return 0;
   }

   if (limbs <= FFT_MULMOD_2EXPP1_CUTOFF) 
   {
       if(bits)
          r[limbs] = mpn_mulmod_2expp1_basecase(r, i1, i2, c, bits, tt);
       else
          r[limbs] = 0;
       return r[limbs];
   }
   while ((((mp_limb_t)1)<<depth) < bits) depth++;
   
   if (depth < 12) off = mulmod_2expp1_table_n[0];
   else off = mulmod_2expp1_table_n[MIN(depth, FFT_N_NUM + 11) - 12];
   depth1 = depth/2 - off;
   
   w1 = bits/(((mp_limb_t)1)<<(2*depth1));

   mpir_fft_mulmod_2expp1(r, i1, i2, limbs, depth1, w1);

   return r[limbs];
}
