/* mpn_sb_divappr_q -- Schoolbook division using the Möller-Granlund 3/2
   division algorithm, returning approximate quotient.  The quotient returned
   is either correct, or one too large.

   Contributed to the GNU project by Torbjorn Granlund.

   THE FUNCTION IN THIS FILE IS INTERNAL WITH A MUTABLE INTERFACE.  IT IS ONLY
   SAFE TO REACH IT THROUGH DOCUMENTED INTERFACES.  IN FACT, IT IS ALMOST
   GUARANTEED THAT IT WILL CHANGE OR DISAPPEAR IN A FUTURE GMP RELEASE.

Copyright 2007, 2009 Free Software Foundation, Inc.

Copyright 2010, 2013 William Hart

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library.  If not, see http://www.gnu.org/licenses/.  */


#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"

#define SB_DIVAPPR_Q_SMALL_THRESHOLD 30

void __divappr_helper(mp_ptr qp, mp_ptr np, mp_srcptr dp, mp_size_t qn)
{   
   mpn_sub_n(np + 1, np + 1, dp, qn + 1);
   add_ssaaaa(np[2], np[1], np[2], np[1], 0, dp[qn]);
   
   for (qn--; qn >= 0; qn--)
   {
      qp[qn] = ~CNST_LIMB(0);
      mpn_add_1(np, np, 3, dp[qn]);
   }
}

mp_limb_t
mpn_sb_divappr_q (mp_ptr qp,
		     mp_ptr np, mp_size_t nn,
		     mp_srcptr dp, mp_size_t dn,
		     mp_limb_t dinv)
{
  mp_limb_t qh;
  mp_size_t qn, i;
  mp_limb_t n1, n0;
  mp_limb_t d1, d0, d11, d01, r1, r2;
  mp_limb_t cy, cy1;
  mp_limb_t q;
  mp_limb_t flag;

  ASSERT (dn > 2);
  ASSERT (nn >= dn);
  ASSERT ((dp[dn-1] & GMP_NUMB_HIGHBIT) != 0);

  np += nn;

  qn = nn - dn;
  if (qn + 1 < dn)
    {
      dp += dn - (qn + 1);
      dn = qn + 1;
    }

  qh = mpn_cmp (np - dn, dp, dn) >= 0;
  if (qh != 0)
    mpn_sub_n (np - dn, np - dn, dp, dn);

  if (dn <= SB_DIVAPPR_Q_SMALL_THRESHOLD)
     {
   /* Reduce until dn - 2 >= qn */
   for (qn--, np--; qn > dn - 2; qn--)
     {
       /* fetch next word */
       cy = np[0];

       np--;
       mpir_divapprox32_preinv2(q, cy, np[0], dinv);
      
	    /* np -= dp*q */
       cy -= mpn_submul_1(np - dn + 1, dp, dn, q);

       /* correct if remainder is too large */
       if (UNLIKELY(cy || np[0] >= dp[dn - 1]))
         {
       if (cy || mpn_cmp(np - dn + 1, dp, dn) >= 0)
       {
          q++;
          cy -= mpn_sub_n(np - dn + 1, np - dn + 1, dp, dn);
       }
       }

       qp[qn] = q;
     }
   
   qn++;
   dp = dp + dn - qn - 1; /* make dp length qn + 1 */
   
   for ( ; qn > 0; qn--)
     {
       /* fetch next word */
       cy = np[0];
 
       np--;
       /* rare case where truncation ruins normalisation */
       if (cy > dp[qn] || (cy == dp[qn] && mpn_cmp(np - qn + 1, dp, qn) >= 0))
         {
       __divappr_helper(qp, np - qn, dp, qn);
       return qh;
         }
       
       mpir_divapprox32_preinv2(q, cy, np[0], dinv);
         
       /* np -= dp*q */
       cy -= mpn_submul_1(np - qn, dp, qn + 1, q);

       /* correct if remainder is too large */
       if (UNLIKELY(cy || np[0] >= dp[qn]))
         {
       if (cy || mpn_cmp(np - qn, dp, qn + 1) >= 0)
         {
       q++;
       cy -= mpn_sub_n(np - qn, np - qn, dp, qn + 1);
         }
         }
       
       qp[qn - 1] = q;
       dp++;
     }
  np[1] = cy;
     } 
  else
     {
  d1 = dp[dn - 1];
  d0 = dp[dn - 2];
  d01 = d0 + 1;
  d11 = d1 + (d01 < d0);

    /* Reduce until dn - 2 >= qn */
   for (qn--, np--; qn > dn - 2; qn--)
     {
       /* fetch next word */
       cy = np[0];

       np --;
       if (UNLIKELY(cy == d1 && np[0] == d0))
          q = ~CNST_LIMB(0);
       else
          mpir_divrem32_preinv2(q, r1, r2, cy, np[0], np[-1], d11, d01, d1, d0, dinv);
 
	    /* np -= dp*q */
       cy -= mpn_submul_1(np - dn + 1, dp, dn, q);

       /* correct if remainder is too large */
       if (UNLIKELY(cy != 0))
         {
          q--;
          cy += mpn_add_n(np - dn + 1, np - dn + 1, dp, dn);
         }
       
       qp[qn] = q;
     }
   
   qn++;
   dp = dp + dn - qn - 1; /* make dp length qn + 1 */
   
   for ( ; qn > 0; qn--)
     {
       /* fetch next word */
       cy = np[0];
 
       np--;
       /* rare case where truncation ruins normalisation */
       if (UNLIKELY(cy >= d1))
         {
       if (cy > d1 || (cy == d1 && mpn_cmp(np - qn + 1, dp, qn) >= 0))
         {
       __divappr_helper(qp, np - qn, dp, qn);
       return qh;
         }
       if (np[0] >= d0)
          q = ~CNST_LIMB(0);
       else
          mpir_divrem32_preinv2(q, r1, r2, cy, np[0], np[-1], d11, d01, d1, d0, dinv);
         }
       else
          mpir_divrem32_preinv2(q, r1, r2, cy, np[0], np[-1], d11, d01, d1, d0, dinv);
         
       /* np -= dp*q */
       cy -= mpn_submul_1(np - qn, dp, qn + 1, q);

       /* correct if remainder is too large */
       if (UNLIKELY(cy != 0))
         {
       q--;
       cy += mpn_add_n(np - qn, np - qn, dp, qn + 1);
         }
       
       qp[qn - 1] = q;
       dp++;
     }
  np[1] = cy;
  }

  return qh;
}
