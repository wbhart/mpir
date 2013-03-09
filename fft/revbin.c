/* 

Copyright 2009 William Hart. All rights reserved.

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

const mp_limb_t revtab0[1] = { 0 };
const mp_limb_t revtab1[2] = { 0, 1 };
const mp_limb_t revtab2[4] = { 0, 2, 1, 3 };
const mp_limb_t revtab3[8] = { 0, 4, 2, 6, 1, 5, 3, 7 };
const mp_limb_t revtab4[16] = { 0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15 };

const mp_limb_t * revtab[5] = { revtab0, revtab1, revtab2, revtab3, revtab4 };

/*
   computes the reverse binary of a binary number of the given number of bits
 */
mp_limb_t mpir_revbin(mp_limb_t in, mp_limb_t bits)
{
    mp_limb_t out = 0, i;
    
    if (bits <= 4)
        return revtab[bits][in];

    for (i = 0; i < bits; i++)
    {   
       out <<= 1;
       out += (in & 1);
       in >>= 1;
    }

    return out;
}
