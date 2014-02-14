/* mpn_mulmid_n -- balanced middle product

Copyright (C) 2009, David Harvey

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "mpir.h"
#include "gmp-impl.h"

/*
   Let a = sum_0^{m-1} a_i B^i and b = sum_0^{n-1} b_j B^j

   then MP(a, m, b, n) = sum_{0<=i<m, 0<=j<n, n-1<=i+j<=m-1} a_ib_j B^{i+j-n+1}

   Note there are m-n+1 different values of i+j and each product a_ib_j will be two limbs. Thus when added together, the sum must take up n-m+3 limbs of space.
   
   This function computes MP(ap,2n-1,bp,n)

   It is required that n << GMP_NUMBMAX. 
*/

void
mpn_mulmid_n (mp_ptr rp, mp_srcptr ap, mp_srcptr bp, mp_size_t n)
{
  ASSERT (n >= 1);
  ASSERT (! MPN_OVERLAP_P (rp, n + 2, ap, 2*n - 1));
  ASSERT (! MPN_OVERLAP_P (rp, n + 2, bp, n));

  if (n < MULMID_TOOM42_THRESHOLD)
    {
      mpn_mulmid_basecase (rp, ap, 2*n - 1, bp, n);
    }
  else
    {
      mp_size_t k;
      mp_ptr scratch;
	   TMP_DECL;
	  
      k = mpn_toom42_mulmid_itch (n);

      if (k <= 1000) k = 1000;
	
	   TMP_MARK;
	   scratch = TMP_ALLOC_LIMBS (k);
	   mpn_toom42_mulmid (rp, ap, bp, n, scratch);
	   TMP_FREE;
    }
}
