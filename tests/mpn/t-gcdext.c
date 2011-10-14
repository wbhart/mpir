/* Test mpn_gcdext.

Copyright 2010 William Hart

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

#include <stdio.h>
#include <stdlib.h>

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"
#include "tests.h"

#if defined(_MSC_VER) || defined(__MINGW32__)
#define random rand
#endif

#define MAX_LIMBS 2000
#define ITERS 1000

void check_normalisation(const mpz_t G, const mpz_t S, const mpz_t U, const mpz_t V, 
						                                   mpz_t T, mpz_t T1, mpz_t T2)
{
   /* compute T */
   mpz_mul(T1, U, S);
   mpz_sub(T, G, T1);
   mpz_divexact(T, T, V);

   /* T2 = V/(2G) */
   mpz_divexact(T2, V, G);
   mpz_cdiv_q_ui(T2, T2, 2);

   /* T1 = abs(S) */
   mpz_abs(T1, S);
      
   if ((mpz_cmp(T1, T2) >= 0) && (mpz_cmp_ui(S, 1) != 0))
   {
	   printf("FAIL\n");
	   gmp_printf("U = %Zd\n", U);
	   gmp_printf("V = %Zd\n", V);
	   gmp_printf("G = %Zd\n", G);
	   gmp_printf("S = %Zd\n", S);
	   gmp_printf("T = %Zd\n", T);
	   abort();
   }
}

/* Check extended gcd routine. */
void check_gcdext(void)
{
   mp_limb_t up[MAX_LIMBS + 1];
   mp_limb_t vp[MAX_LIMBS + 1];
   mp_limb_t up2[MAX_LIMBS];
   mp_limb_t vp2[MAX_LIMBS];
   mp_limb_t sp[MAX_LIMBS + 1];
   mp_limb_t tp[MAX_LIMBS];
   mp_limb_t gp[MAX_LIMBS + 1];
   
   mp_size_t un, vn, sn, tn, bits, gn;
   mp_bitcnt_t u_bits, v_bits, g_bits;
   long i, j;
   mpz_t U, V, S, T, G, T1, T2;

   gmp_randstate_t rands;
   gmp_randinit_default(rands);

   mpz_init(T);
   mpz_init(T1);
   mpz_init(T2);
  
   for (i = 0; i < ITERS; i++)
   {
      u_bits = (random()%(GMP_LIMB_BITS*MAX_LIMBS)) + 1;
      v_bits = (random()%u_bits) + 1;
	  g_bits = (random()%v_bits) + 1;
      
	  gn = (g_bits + GMP_LIMB_BITS - 1)/GMP_LIMB_BITS;
	  sn = (u_bits - g_bits + GMP_LIMB_BITS - 1)/GMP_LIMB_BITS;
	  tn = (v_bits - g_bits + GMP_LIMB_BITS - 1)/GMP_LIMB_BITS;

	  /* generate quasi gcd gp*/
	  do{
	     mpn_urandomb(gp, rands, g_bits);
         gn = (g_bits + GMP_LIMB_BITS - 1)/GMP_LIMB_BITS;
	     MPN_NORMALIZE(gp, gn);
	  } while (gn == 0);

	  /* 
	     generate random {sp, sn}
	     set {up, un} = {gp, gn}*{sp, sn} 
	  */
	  if (u_bits > g_bits)
	  {
		 mpn_urandomb(sp, rands, u_bits - g_bits);
         up[0] |= (mp_limb_t) 1;

		 if (gn >= sn)
			mpn_mul(up, gp, gn, sp, sn);
		 else
            mpn_mul(up, sp, sn, gp, gn);
		 un = gn + sn;
		 MPN_NORMALIZE(up, un);
	  } else
	  {
	     MPN_COPY(up, gp, gn);
		 un = gn;
		 MPN_NORMALIZE(up, un);
	  }
      
	  do
	  {
	  /* 
	     generate random odd {tp, tn}
	     set {vp, vn} = {gp, gn}*{tp, tn} 
	  */
	  if (v_bits > g_bits)
	  {
		 mpn_urandomb(tp, rands, v_bits - g_bits);
		 tp[0] |= (mp_limb_t) 1;

         if (gn >= tn)
			mpn_mul(vp, gp, gn, tp, tn);
		 else
            mpn_mul(vp, tp, tn, gp, gn);
		 vn = gn + tn;
		 MPN_NORMALIZE(vp, vn);
	  } else
	  {
	     MPN_COPY(vp, gp, gn);
		 vn = gn;
		 MPN_NORMALIZE(vp, vn);
	  }
	  } while ((un == vn) && (mpn_cmp(up, vp, un) < 0));

	  /* Save a copy of up and vp */
	  MPN_COPY(up2, up, un);
	  MPN_COPY(vp2, vp, vn);
          if(un<vn)continue;
	  gn = mpn_gcdext(gp, sp, &sn, up, un, vp, vn);
      
	  U->_mp_d = up2;
	  U->_mp_size = un;
      V->_mp_d = vp2;
	  V->_mp_size = vn;
      S->_mp_d = sp;
	  S->_mp_size = sn;
      G->_mp_d = gp;
	  G->_mp_size = gn;

	  check_normalisation(G, S, U, V, T, T1, T2);
   }

   mpz_clear(T);
   mpz_clear(T1);
   mpz_clear(T2);
   gmp_randclear(rands);
}

int main(void)
{
  tests_start();
  check_gcdext();
  tests_end();

  return 0;
}
