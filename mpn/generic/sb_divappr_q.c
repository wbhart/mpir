/* mpn_sb_divappr_q - schoolbook approximate quotient.

   THE FUNCTIONS IN THIS FILE ARE INTERNAL FUNCTIONS WITH MUTABLE
   INTERFACES.  IT IS ONLY SAFE TO REACH THEM THROUGH DOCUMENTED INTERFACES.
   IN FACT, IT IS ALMOST GUARANTEED THAT THEY'LL CHANGE OR DISAPPEAR IN A
   FUTURE GNU MP RELEASE.


Copyright 2009 William Hart.

This file is part of the MPIR Library.

The MPIR Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The MPIR Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPIR Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA. */

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"

/*
   Given n = {np, nn} and d = {dp, dn} and a 2 limb inverse
   x = {dip, 2} (with implicit top bit), satisfying 
   x*d0 < B^4 <= (x+1)*d0 where d0 = {dp + dn - 2, 2} is the
   top two limbs of the denominator, returns an approximate
   quotient q = {qp, nn - dn + 1} such that d*q + r = n for
   some remainder r with -d < r < d.

   Requires d = {dp, dn} to be normalised, i.e. the most 
   significant bit of the most significant limb must be set.
   Also requires that d is at least two limbs. 

   n = {np, nn} is destroyed.
*/

mp_limb_t
mpn_sb_divappr_q (mp_ptr qp, mp_ptr np, mp_size_t nn,
		  mp_srcptr dp, mp_size_t dn, mp_srcptr dip)
{
  /*
     In order to make use of the the two limb inverse we
	 use the following theorem of Torbjorn Granlund and
	 Peter Montgomery from their paper, "Division by 
	 invariant integers using multiplication" (restated
	 here for clarity):

     Lemma 8.1: Let d be normalised, d < B^2 (i.e. 
	 fits in two words), and suppose that 
	   m*d < B^4 <= (m+1)*d.
	 Let 0 <= n <= B^2*d - 1. Write
	   n = n2*B^2 + n1*B^2/2 + n0 
	 with n1 = 0 or 1 and n0 < B^2/2.
	 Suppose 
	   q1*B^2 + q0 = n2*B^2 + (n2 + n1)*(m-B^2) 
	               + n1*(d-B^2/2) + n0
	 and 0 <= q0 < B^2. 
     Then 0 <= q1 < B^2 and 0 <= n - q1*d < 2d.

	 We apply the theorem as follows. Note that
	 n0 and n1*(d-B^2/2) are both less than B^2/2.
	 Also note that n1*(m-B^2) < B^2. Thus the sum
	 of all these terms contributes at most 1 to q1.

	 We are left with n2*B^2 + n2*(m-B^2). But note
	 that (m-B^2) is precisely our precomputed inverse
	 without the implied leading bit. If we write
	 q1*B^2 + q0 = n2*B^2 + n2*(m-B^2), then from the
	 theorem, we have 0 <= n-q1*d < 3d.
  */

  mp_limb_t ret, di0, di1, p1, p2, p3, p4, q, q0, n21, n20, cy;
  mp_size_t qn = nn - dn + 1;
  mp_size_t i;
  
  /* 
     We only need to use the top qn limbs of the 
	 denominator and the same applies for the 
	 numerator.
  */
  
  if (qn < dn)
  {
	 dp += (dn - qn);
	 dn = qn;
  }

  if (qn < nn)
  {
     np += (nn - qn);
	 nn = qn;
  }

  /* 
     It may be that the top limbs of the numerator
     are bigger than the denominator, in which case
     we return the high top limb of the quotient as 
	 1 instead of 0.
  */

  if (mpn_cmp(np + nn - dn, dp, dn) >= 0)
  {
     ret = CNST_LIMB(1);
	 mpn_sub_n(np + nn - dn, np + nn - dn, dp, dn);
  } else 
     ret = CNST_LIMB(0);

  di1 = dip[1]; 
  di0 = dip[0];
  for (i = qn - 2; i >= 0L; i--)
  {
     /*
	    Compute n2 + top two limbs of n2*di, but
		caring only about the top limb q, which we
		allow to be off by up to 1.
	 */
     n21 = np[nn - 1];
	  n20 = np[nn - 2];
     umul_ppmm(p2, p1, di0, n21);
	 umul_ppmm(p4, p3, di1, n20);
	 add_ssaaaa(q, q0, n21, p2, CNST_LIMB(0), p4);
	 umul_ppmm(p1, p2, di1, n21);
	 add_ssaaaa(q, q0, q, q0, p1, p2);
     add_ssaaaa(q, q0, q, q0, CNST_LIMB(0), n20);

	 cy = mpn_submul_1(np + nn - dn - 1, dp, dn, q);

	 /* Either q was correct or too small by 1 */

	 if ((np[nn-1] > cy) || (mpn_cmp(np + nn - dn - 1, dp, dn) >= 0))
	 {
	    q++; /* beware: q *can* overflow */
       if (q == 0) 
          q--;
       else
          mpn_sub_n(np + nn - dn - 1, np + nn - dn - 1, dp, dn);
	 }
    
	 qp[i] = q;

	 if (dn > i + 1)
    {
       dp++;
       dn--;
    }

    nn--;

    /* If what is left is equal to the denominator then the 
       rest of the quotient has to be all 1's. Without this 
       step the remainder gets screwed up
    */
    if (mpn_cmp(np + nn - dn, dp, dn) == 0)
    {
       i--;
       for ( ; i >= 0L; i--) qp[i] = ~CNST_LIMB(0);
       break;
    }
  }
	
  return ret;
}

