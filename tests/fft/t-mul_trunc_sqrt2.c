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

#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>
#include "gmp-impl.h"
#include "tests.h"

int
main(void)
{
    mp_bitcnt_t depth, w;
    
    gmp_randstate_t state;

    tests_start();
    fflush(stdout);

    gmp_randinit_default(state);

    for (depth = 6; depth <= 12; depth++)
    {
        for (w = 1; w <= 5; w++)
        {
            mp_size_t n = (((mp_limb_t)1)<<depth);
            mp_bitcnt_t bits1 = (n*w - (depth + 1))/2; 
            mp_limb_t trunc;
            mp_bitcnt_t bits;
            mp_size_t int_limbs;
            mp_size_t j;
            mp_limb_t * i1, *i2, *r1, *r2;
        
            mpn_rrandom(&trunc, state, 1);
            trunc = 2 * n + 2 * (trunc % n) + 2; /* trunc is even */
            bits = (trunc/2)*bits1;
            int_limbs = (bits - 1)/GMP_LIMB_BITS + 1;

            i1 = malloc(6*int_limbs*sizeof(mp_limb_t));
            i2 = i1 + int_limbs;
            r1 = i2 + int_limbs;
            r2 = r1 + 2*int_limbs;
   
            mpn_urandomb(i1, state, int_limbs*GMP_LIMB_BITS);
            mpn_urandomb(i2, state, int_limbs*GMP_LIMB_BITS);
  
            if (ABOVE_THRESHOLD (int_limbs, MUL_FFT_FULL_THRESHOLD))
               mpn_toom8h_mul(r2, i1, int_limbs, i2, int_limbs);
            else
               mpn_mul(r2, i1, int_limbs, i2, int_limbs);
            mpn_mul_trunc_sqrt2(r1, i1, int_limbs, i2, int_limbs, depth, w);
            
            for (j = 0; j < 2*int_limbs; j++)
            {
                if (r1[j] != r2[j]) 
                {
                    gmp_printf("error in limb %ld, %Mx != %Mx\n", j, r1[j], r2[j]);
                    abort();
                }
            }

            free(i1);
        }
    }

    /* test squaring */
    for (depth = 6; depth <= 12; depth++)
    {
        for (w = 1; w <= 5; w++)
        {
            mp_size_t n = (((mp_limb_t)1)<<depth);
            mp_bitcnt_t bits1 = (n*w - (depth + 1))/2; 
            mp_limb_t trunc;
            mp_bitcnt_t bits;
            mp_size_t int_limbs;
            mp_size_t j;
            mp_limb_t * i1, *r1, *r2;
        
            mpn_rrandom(&trunc, state, 1);
            trunc = 2*n + 2 * (trunc % n) + 2; /* trunc is even */
            bits = (trunc/2)*bits1;
            int_limbs = (bits - 1)/GMP_LIMB_BITS + 1;

            i1 = malloc(5*int_limbs*sizeof(mp_limb_t));
            r1 = i1 + int_limbs;
            r2 = r1 + 2*int_limbs;
   
            mpn_urandomb(i1, state, int_limbs*GMP_LIMB_BITS);
            
            if (ABOVE_THRESHOLD (int_limbs, MUL_FFT_FULL_THRESHOLD))
               mpn_toom8h_mul(r2, i1, int_limbs, i1, int_limbs);
            else
               mpn_mul(r2, i1, int_limbs, i1, int_limbs);
            mpn_mul_trunc_sqrt2(r1, i1, int_limbs, i1, int_limbs, depth, w);
            
            for (j = 0; j < 2*int_limbs; j++)
            {
                if (r1[j] != r2[j]) 
                {
                    gmp_printf("error in limb %ld, %Mx != %Mx\n", j, r1[j], r2[j]);
                    abort();
                }
            }

            free(i1);
        }
    }

    gmp_randclear(state);
    
    tests_end();
    return 0;
}
