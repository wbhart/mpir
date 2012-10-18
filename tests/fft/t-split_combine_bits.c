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
    int i;
    mp_size_t j;

    gmp_randstate_t state;

    tests_start();
    fflush(stdout);

    gmp_randinit_default(state);

    for (i = 0; i < 10000; i++)
    {
        mp_limb_t total_limbs;
        mp_limb_t * in;
        mp_limb_t * out;
        mp_bitcnt_t bits;
        mp_size_t limbs;
        long length;
        mp_limb_t ** poly;
        
        mpn_rrandom(&total_limbs, state, 1);
        total_limbs = total_limbs % 1000 + 1;
        in = malloc(total_limbs*sizeof(mp_limb_t));
        out = calloc(total_limbs, sizeof(mp_limb_t));
        mpn_rrandom(&bits, state, 1);
        bits = bits % 200 + 1;
        limbs = (2*bits - 1)/GMP_LIMB_BITS + 1;
        length = (total_limbs*GMP_LIMB_BITS - 1)/bits + 1;
        
        poly = malloc(length*sizeof(mp_limb_t *));
        for (j = 0; j < length; j++)
           poly[j] = malloc((limbs + 1)*sizeof(mp_limb_t));

        mpn_urandomb(in, state, total_limbs*GMP_LIMB_BITS);

        mpir_fft_split_bits(poly, in, total_limbs, bits, limbs);
        mpir_fft_combine_bits(out, poly, length, bits, limbs, total_limbs);
        
        for (j = 0; j < total_limbs; j++)
        {
           if (in[j] != out[j])
           {
              printf("FAIL:\n");
              gmp_printf("Error in limb %ld, %Mu != %Mu\n", j, in[j], out[j]);
              abort();
           }
        }

        free(in);
        free(out);

        for (j = 0; j < length; j++)
           free(poly[j]);

        free(poly);
    }

    gmp_randclear(state);
    
    tests_end();
    return 0;
}
