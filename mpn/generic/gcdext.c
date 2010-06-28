/* mpn_gcdext -- Extended Greatest Common Divisor.

Copyright 1996, 1998, 2000, 2001, 2002 Free Software Foundation, Inc.
Copyright 2004, 2005 Niels Möller
Copyright 2010 William Hart

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library; see the file COPYING.LIB.  If not, write
to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA. */

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"

/*---------------------------------------------------------

    Asymptotically fast xgcd based on Niels Mohler's ngcd

----------------------------------------------------------*/

/* Needs temporary storage for the division and multiplication
   The division has quotient of size an - bn + 1
	product needs an + un at most
	Thus we need space at most n + un + 1

	If the gcd is found, stores it in gp and *gn, and the associated 
   cofactor in {sp, *un} and returns zero.
   Otherwise, compute the reduced a and b, update u0p and u1p,
	and return the new size. /*
 
 *
 * To make this code work with "make tune" we need to conditionally
 * exclude the Moller code when this file gets included inside of
 * gcdext*.c in ../tune.
 */
#ifndef INSIDE_TUNE_GCDEXT_BIN

#define P_SIZE(n) (n/3)

mp_size_t
mpn_ngcdext_subdiv_step (mp_ptr gp, mp_size_t *gn, mp_ptr s0p, mp_ptr u0, mp_ptr u1, 
		             mp_size_t *un, mp_ptr ap, mp_ptr bp, mp_size_t n, mp_ptr tp)
{
  /* Called when nhgcd or mpn_nhgcd2 has failed. Then either one of a or b
     is very small, or the difference is very small. Perform one
     subtraction followed by one division. */

  mp_size_t an, bn, cy, qn, qn2, u0n, u1n;
  int negate = 0;
  int c;

  ASSERT (n > 0);
  ASSERT (ap[n-1] > 0 || bp[n-1] > 0);

  /* See to what extend ap and bp are the same */
  for (an = n; an > 0; an--)
    if (ap[an-1] != bp[an-1])
      break;

  if (an == 0)
    {
      /* ap OR bp is the gcd, two possible normalisations
	     u1 or -u0, pick the smallest
	  */
      MPN_COPY (gp, ap, n);
	  (*gn) = n;

      MPN_CMP(c, u1, u0, *un);
	  if (c <= 0) // u1 is smallest
	  {
		 MPN_NORMALIZE(u1, (*un));
         MPN_COPY (s0p, u1, (*un));
	  } else // -u0 is smallest
	  {
		 MPN_NORMALIZE(u0, (*un));
         MPN_COPY (s0p, u0, (*un));
		 (*un) = -(*un);
	  }
	  
	  return 0;
    }

  if (ap[an-1] < bp[an-1]) /* swap so that ap >= bp */
  {
	 MP_PTR_SWAP (ap, bp);
    MP_PTR_SWAP (u0, u1);
	 negate = ~negate;
  }

  bn = n;
  MPN_NORMALIZE (bp, bn);
  if (bn == 0)
    {
      /* ap is the gcd */
		MPN_COPY (gp, ap, n);
      MPN_NORMALIZE(u1, (*un));
      MPN_COPY (s0p, u1, (*un));
		if (negate) (*un) = -(*un);
      (*gn) = n;
	  
	  return 0;
    }

  ASSERT_NOCARRY (mpn_sub_n (ap, ap, bp, an)); /* ap -= bp, u1 += u0 */
  MPN_NORMALIZE (ap, an);
  
  ASSERT (an > 0);
	
  cy = mpn_add_n(u1, u1, u0, *un);
  if (cy) u1[(*un)++] = cy;

  if (an < bn) /* make an >= bn */
  {
	  MPN_PTR_SWAP (ap, an, bp, bn);
	  MP_PTR_SWAP(u0, u1);
	  negate = ~negate;
  }
  else if (an == bn)
    {
      MPN_CMP (c, ap, bp, an);
      if (c < 0)
		{
			MP_PTR_SWAP (ap, bp);
		   MP_PTR_SWAP(u0, u1);
	      negate = ~negate;
      } else if (c == 0) /* gcd is ap OR bp */
		{
		 /* this case seems to never occur 
			it should happen only if ap = 2*bp
		 */
		 MPN_COPY (gp, ap, an);
         (*gn) = an;
		 /* As the gcd is ap OR bp, there are two possible 
		    cofactors here u1 or -u0, and we want the 
			least of the two.
		 */
		 MPN_CMP(c, u1, u0, *un);
		 if (c < 0) // u1 is less
		 {
			MPN_NORMALIZE(u1, (*un));
            MPN_COPY (s0p, u1, (*un));
            if (negate) (*un) = -(*un);
		 } else if (c > 0) // -u0 is less
		 {
			MPN_NORMALIZE(u0, (*un));
            MPN_COPY (s0p, u0, (*un));
            if (!negate) (*un) = -(*un);
		 } else // same
		 {
		    MPN_NORMALIZE(u0, (*un));
            MPN_COPY (s0p, u0, (*un));
		 }
         
		 return 0;
		}
    }

  ASSERT (an >= bn);

  qn = an - bn + 1;
  mpn_tdiv_qr (tp, ap, 0, ap, an, bp, bn); /* ap -= q * bp, u1 += q * u0 */

  /* Normalizing seems to be the simplest way to test if the remainder
     is zero. */
  an = bn;
  MPN_NORMALIZE (ap, an);
  if (an == 0)
    {
      /* this case never seems to occur*/
	  /* gcd = bp */
	  MPN_COPY (gp, bp, bn);
      MPN_NORMALIZE(u0, (*un));
      MPN_COPY (s0p, u0, (*un));
      if (!negate) (*un) = -(*un);
      (*gn) = bn;
      
	  return 0;
    }

  qn2 = qn;
  u0n = (*un);
  MPN_NORMALIZE (tp, qn2);
  MPN_NORMALIZE (u0, u0n);

  if (u0n > 0)
  {

  if (qn2 > u0n) mpn_mul(tp + qn, tp, qn2, u0, u0n);
  else mpn_mul(tp + qn, u0, u0n, tp, qn2);

  u0n += qn2;
  MPN_NORMALIZE(tp + qn, u0n);

  if ((*un) >= u0n) 
  {
	  cy = mpn_add(u1, u1, (*un), tp + qn, u0n);
	  if (cy) u1[(*un)++] = cy;
  } else
  {
	  cy = mpn_add(u1, tp + qn, u0n, u1, (*un));
	  (*un) = u0n;
	  if (cy) u1[(*un)++] = cy;
  }
  }

  return bn;
}

/*
   Set (u0, u1) = (u0, u1) M 
	Requires temporary space un + un  + M->n = 2*un + M->n
*/
void ngcdext_cofactor_adjust(mp_ptr u0, mp_ptr u1, mp_size_t * un, struct ngcd_matrix *M, mp_ptr tp)
{
	/* Let M = (r00, r01)
	           (r10, r11)
	   We want u0 = u0 * r00 + u1 * r10
		        u1 = u0 * r01 + u1 * r11
	   We make a copy of u0 at tp and update u0 first
	*/

   mp_limb_t cy, cy2;
	mp_ptr t2p =(tp + (*un)); /* second temporary space */
	ASSERT(tp > M->p[1][1] + M->n);


	MPN_COPY(tp, u0, *un);

	if (M->n >= (*un))
	{
		mpn_mul(t2p, M->p[1][0], M->n, u1, *un); /* t2p = r10 * u1 */
		mpn_mul(u0, M->p[0][0], M->n, tp, *un);  /* u0 = r00 * u0 */
	} else
	{
		mpn_mul(t2p, u1, *un, M->p[1][0], M->n); 
		mpn_mul(u0, tp, *un, M->p[0][0], M->n); 
	}
	
	cy = mpn_add_n(u0, u0, t2p, M->n + (*un)); /* u0 += t2p */
	
	
	if (M->n >= (*un))
	{
		mpn_mul(t2p, M->p[1][1], M->n, u1, *un); /* t2p = r11 * u1 */
		mpn_mul(u1, M->p[0][1], M->n, tp, *un);  /* u1 = r01 * u0 */
	} else
	{
		mpn_mul(t2p, u1, *un, M->p[1][1], M->n); 
		mpn_mul(u1, tp, *un, M->p[0][1], M->n); 
	}
	
	cy2 = mpn_add_n(u1, u1, t2p, M->n + (*un)); /* u1 += t2p */

	if ((cy) || (cy2)) /* normalise u0, u1 */
	{
		u0[M->n + (*un)] = cy;
		u1[M->n + (*un)] = cy2;
		(*un) += (M->n + 1);
	} else 
	{
      (*un) += M->n;
		while ((u0[*un - 1] == 0) && (u1[*un - 1] == 0)) (*un)--; /* both cannot be zero, so this won't overrun */
	}
}

/*
   Computes |t| where t = (gp - s*ap)/bp 
	Requires temporary space sn + an
*/

void gcdext_get_t(mp_ptr t, mp_size_t * tn, mp_ptr gp, mp_size_t gn, mp_ptr ap, 
					mp_size_t an, mp_ptr bp, mp_size_t n, mp_ptr s, mp_size_t sn, mp_ptr tp)
{
	mp_size_t ss = ABS(sn);
	mp_limb_t cy;
	
	if (ss >= an)
		mpn_mul(tp, s, ss, ap, an);
	else
      mpn_mul(tp, ap, an, s, ss);

	(*tn) = ss + an;
	(*tn) -= (tp[(*tn) - 1] == 0);

	/* We must have s*ap >= gp and we really want to compute -t */

	if (sn > 0)
	{
		mpn_sub(tp, tp, *tn, gp, gn);
	   MPN_NORMALIZE(tp, (*tn));
	} else 
	{
		cy = mpn_add(tp, tp, *tn, gp, gn);
		if (cy) tp[(*tn)++] = cy;
	}

	if ((*tn) == 0) 
	{
		return;
	}

	mpn_tdiv_qr(t, tp, 0, tp, (*tn), bp, n);

	ASSERT_MPN_ZERO_P(tp, n);

   (*tn) -= (n - 1);
	(*tn) -= (t[(*tn) - 1] == 0);
}

mp_limb_t mpn_gcdinv_1(mp_limb_signed_t * a, mp_limb_t x, mp_limb_t y)
{
   mp_limb_signed_t u1 = CNST_LIMB(1); 
   mp_limb_signed_t u2 = CNST_LIMB(0); 
   mp_limb_signed_t t1; 
   mp_limb_t u3, v3;
   mp_limb_t quot, rem;
   
   u3 = x, v3 = y;
   
   if ((mp_limb_signed_t) (x & y) < (mp_limb_signed_t) CNST_LIMB(0)) /* x and y both have top bit set */ 
   {
     quot=u3-v3;
     t1 = u2; u2 = u1 - u2; u1 = t1; u3 = v3;
     v3 = quot;
   }

   while ((mp_limb_signed_t) (v3<<1) < (mp_limb_signed_t) CNST_LIMB(0)) /* second value has second msb set */
   {
     quot=u3-v3;
     if (quot < v3)
     {
        t1 = u2; u2 = u1 - u2; u1 = t1; u3 = v3;
        v3 = quot;
     } else if (quot < (v3<<1))
     {  
        t1 = u2; u2 = u1 - (u2<<1); u1 = t1; u3 = v3;
        v3 = quot-u3;
     } else
     {
        t1 = u2; u2 = u1 - 3*u2; u1 = t1; u3 = v3;
        v3 = quot-(u3<<1);
     }
   }
   
   while (v3) {
      quot=u3-v3;
      if (u3 < (v3<<2)) /* overflow not possible due to top 2 bits of v3 not being set */
      {
         if (quot < v3)
         {
            t1 = u2; u2 = u1 - u2; u1 = t1; u3 = v3;
            v3 = quot;
         } else if (quot < (v3<<1))
         {  
            t1 = u2; u2 = u1 - (u2<<1); u1 = t1; u3 = v3;
            v3 = quot-u3;
         } else
         {
            t1 = u2; u2 = u1 - 3*u2; u1 = t1; u3 = v3;
            v3 = quot-(u3<<1);
         }
      } else
      {
         quot=u3/v3;
         rem = u3 - v3*quot;
         t1 = u2; u2 = u1 - quot*u2; u1 = t1; u3 = v3;
         v3 = rem;
      }
   }
   
   /* Quite remarkably, this always has |u1| < x/2 at this point, thus comparison with 0 is valid */
   //if (u1 < (mp_limb_signed_t) 0) u1 += y;
   (*a) = u1;
   
   return u3;
}


mp_size_t
mpn_gcdext (mp_ptr gp, mp_ptr s0p, mp_size_t *s0size,
	    mp_ptr ap, mp_size_t an, mp_ptr bp, mp_size_t n)
{
  mp_size_t init_scratch, orig_n = n;
  mp_size_t scratch, un, u0n, u1n;
  mp_limb_t t;
  mp_ptr tp, u0, u1;
  int swapped = 0;
    struct ngcd_matrix M;
    mp_size_t p;
    mp_size_t nn;
  mp_limb_signed_t a;
  int c;
  TMP_DECL;
  
  ASSERT (an >= n);
  
  if (an == 1)
  {
    if (!n)
    {
       /* shouldn't ever occur, but we include for completeness */
		gp[0] = ap[0];
       s0p[0] = 1;
       *s0size = 1;
       
	   return 1;
    }
    
	gp[0] = mpn_gcdinv_1(&a, ap[0], bp[0]);
    if (a < (mp_limb_signed_t) 0)
	{
	   s0p[0] = -a;
       (*s0size) = -1;
	} else
    {
	   s0p[0] = a;
       (*s0size) = 1 - (s0p[0] == 0);
	}
	
	return 1;
  }

  init_scratch = MPN_NGCD_MATRIX_INIT_ITCH (n-P_SIZE(n));
  scratch = mpn_nhgcd_itch ((n+1)/2);

  /* Space needed for mpn_ngcd_matrix_adjust */
  if (scratch < 2*n)
    scratch = 2*n;
  if (scratch < an - n + 1) /* the first division can sometimes be selfish!! */
	 scratch = an - n + 1;

 /* Space needed for cofactor adjust */
  scratch = MAX(scratch, 2*(n+1) + P_SIZE(n) + 1);

  TMP_MARK;
  
  if (5*n + 2 + MPN_GCD_LEHMER_N_ITCH(n) > init_scratch + scratch) 
    tp = TMP_ALLOC_LIMBS (7*n+4+MPN_GCD_LEHMER_N_ITCH(n)); /* 2n+2 for u0, u1, 5*n+2 + MPN_GCD_LEHMER_N_ITCH(n) for Lehmer
                                                              and copies of ap and bp and s (and finally 3*n+1 for t and get_t) */
  else
    tp = TMP_ALLOC_LIMBS (2*(n+1) + init_scratch + scratch);
    
  if (an > n)
    {
      mp_ptr qp = tp;

      mpn_tdiv_qr (qp, ap, 0, ap, an, bp, n);
      
      an = n;
      MPN_NORMALIZE (ap, an);
      if (an == 0)
	{	  
	  MPN_COPY (gp, bp, n);
	  TMP_FREE;
	  (*s0size) = 0;
	  
	  return n;
	}
    }
    
    if (BELOW_THRESHOLD (n, GCDEXT_THRESHOLD))
    {
      n = mpn_ngcdext_lehmer (gp, s0p, s0size, ap, bp, n, tp);
      TMP_FREE;
      
	  return n;
    }
  
    u0 = tp; /* Cofactor space */
    u1 = tp + n + 1;

    MPN_ZERO(tp, 2*(n+1));

    tp += 2*(n+1);
  
    /* First iteration, setup u0 and u1 */

    p = P_SIZE(n);
  
    mpn_ngcd_matrix_init (&M, n - p, tp);
	 ASSERT(tp + init_scratch > M.p[1][1] + M.n);
	 nn = mpn_nhgcd (ap + p, bp + p, n - p, &M, tp + init_scratch);
  if (nn > 0)
	 {
		 n = mpn_ngcd_matrix_adjust (&M, p + nn, ap, bp, p, tp + init_scratch);
		 
		 /* 
            (ap'', bp'')^T = M^-1(ap', bp')^T 
		    and (ap', bp') = (1*ap + ?*bp, 0*ap + ?*bp) 
		    We let u0 be minus the factor of ap appearing 
            in the expression for bp'' and u1 be the 
            factor of ap appearing in the expression for ap''
        */

       MPN_COPY(u0, M.p[1][0], M.n);
	    MPN_COPY(u1, M.p[1][1], M.n);

	    un = M.n;
	    while ((u0[un-1] == 0) && (u1[un-1] == 0)) un--; /* normalise u0, u1, both cannot be zero as det = 1*/
     }
  else	
	 {
	   mp_size_t gn;

		un = 1;
	   u0[0] = 0; /* bp = 0*ap + ?*bp, thus u0 = -0 */
	   u1[0] = 1; /* ap = 1*ap + ?*bp, thus u1 = 1 */
   
	   n = mpn_ngcdext_subdiv_step (gp, &gn, s0p, u0, u1, &un, ap, bp, n, tp);
	 if (n == 0)
	   {
	      /* never observed to occur */
		   (*s0size) = un;
			ASSERT(s0p[*s0size - 1] != 0);
		   TMP_FREE;
	       
		   return gn;
	   }
	 } 

  while (ABOVE_THRESHOLD (n, GCDEXT_THRESHOLD))
    {
      struct ngcd_matrix M;
      mp_size_t p = P_SIZE(n);
      mp_size_t nn;
      
      mpn_ngcd_matrix_init (&M, n - p, tp);
      nn = mpn_nhgcd (ap + p, bp + p, n - p, &M, tp + init_scratch);
		if (nn > 0)
	{
	   n = mpn_ngcd_matrix_adjust (&M, p + nn, ap, bp, p, tp + init_scratch);

		ngcdext_cofactor_adjust(u0, u1, &un, &M, tp + init_scratch);
		
		/* 
            (ap'', bp'')^T = M^-1(ap', bp')^T 
		    and (ap', bp') = (u1*ap + ?*bp, -u0*ap + ?*bp) 
		    So we need u0' = -(-c*u1 + a*-u0) = a*u0 + c*u1
            and we need u1' = (d*u1 -b*-u0) = b*u0 + d*u1 
        */

     
		ASSERT(un <= orig_n + 1);

	}  else	
	{
	  mp_size_t gn;
	  n = mpn_ngcdext_subdiv_step (gp, &gn, s0p, u0, u1, &un, ap, bp, n, tp);
	  ASSERT(un <= orig_n + 1);
	  if (n == 0)
	    {
	      (*s0size) = un;
			ASSERT(((*s0size) == 0) || (s0p[ABS(*s0size) - 1] != 0));
		   TMP_FREE;
		   
		   return gn;
	    }
	}
    }

  ASSERT (ap[n-1] > 0 || bp[n-1] > 0);
  ASSERT (u0[un-1] > 0 || u1[un-1] > 0);

  if (ap[n-1] < bp[n-1])
  {
	  MP_PTR_SWAP (ap, bp);
	  MP_PTR_SWAP (u0, u1);
	  swapped = 1;
  }
   
  an = n; /* {ap, an} and {bp, bn} are normalised, {ap, an} >= {bp, bn} */
  MPN_NORMALIZE (bp, n);

  if (n == 0)
    {
      /* If bp == 0 then gp = ap
		   with cofactor u1
			If we swapped then cofactor is -u1
			This case never seems to happen
		*/
		MPN_COPY (gp, ap, an);
		MPN_NORMALIZE(u1, un);
		MPN_COPY(s0p, u1, un);
      (*s0size) = un;
		if (swapped) (*s0size) = -(*s0size);
      TMP_FREE;
      
	  return an;
    }

  /* 
     If at this point we have s*ap' + t*bp' = gp where gp is the gcd
	  and (ap', bp') = (u1*ap + ?*bp, -u0*ap + ?*bp)
	  then gp = s*u1*ap - t*u0*ap + ?*bp
	  and the cofactor we want is (s*u1-t*u0).

	  First there is the special case u0 = 0, u1 = 1 in which case we do not need 
	  to compute t...
  */
    
  ASSERT(u1 + un <= tp);
  u0n = un;
  MPN_NORMALIZE(u0, u0n);  /* {u0, u0n} is now normalised */

  if (u0n == 0) /* u1 = 1 case is rare*/
  {
	  mp_size_t gn;
	 
	  gn = mpn_ngcdext_lehmer (gp, s0p, s0size, ap, bp, n, tp);
	  if (swapped) (*s0size) = -(*s0size);
	  TMP_FREE;
	  
	  return gn;
  }
  else
  {
	  /* Compute final gcd. */
  
	  mp_size_t gn, sn, tn;
	  mp_ptr s, t;
	  mp_limb_t cy;
	  int negate = 0;
	  
      /* Save an, bn first as gcdext destroys inputs */
	  s = tp;
	  tp += an;
	  
     MPN_COPY(tp, ap, an);
	  MPN_COPY(tp + an, bp, an);
	  
	  if (mpn_cmp(tp, tp + an, an) == 0) 
	  {
	     /* gcd is tp or tp + an 
		    return smallest cofactor, either -u0 or u1
		 */
	     gn = an;
		 MPN_NORMALIZE(tp, gn);
		 MPN_COPY(gp, tp, gn);
		 
		 MPN_CMP(c, u0, u1, un);
		 if (c < (mp_limb_signed_t) 0)
		 {
		    MPN_COPY(s0p, u0, u0n);
			(*s0size) = -u0n;
		 } else
		 {
		    MPN_NORMALIZE(u1, un);
			MPN_COPY(s0p, u1, un);
			(*s0size) = un;
		 }
		 TMP_FREE;
		  
		 return gn;
	  }

      gn = mpn_ngcdext_lehmer (gp, s, &sn, tp, tp + an, an, tp + 2*an);
      
	  /* Special case, s == 0, t == 1, cofactor = -u0 case is rare*/

	  if (sn == 0)
	  {
		  MPN_COPY(s0p, u0, u0n);
		  (*s0size) = -u0n;
		  if (swapped) (*s0size) = -(*s0size);
		  TMP_FREE;
		  
		  return gn;
	  }

	  /* We'll need the other cofactor t = (gp - s*ap)/bp 
		*/

	  t = tp;
	  tp += (an + 1);
		 
	  gcdext_get_t(t, &tn, gp, gn, ap, an, bp, n, s, sn, tp);

	  ASSERT((tn == 0) || (t[tn - 1] > 0)); /* {t, tn} is normalised */

	  ASSERT(tn <= an + 1);

	  /* We want to compute s*u1 - t*u0, so if s is negative
	     t will be positive, so we'd be dealing with negative
		  numbers. We fix that here.
	  */

	  if (sn < 0)
	  {
		  sn = -sn;
		  negate = 1;
	  }

	  /* Now we can deal with the special case u1 = 0 */

	  u1n = un; 
	  MPN_NORMALIZE(u1, u1n); /* {u1, u1n} is now normalised */
     
	  if (u1n == 0) /* case is rare */
	  {
		  MPN_COPY(s0p, t, tn);
		  (*s0size) = -tn;
		  if (swapped ^ negate) (*s0size) = -(*s0size);
		  TMP_FREE;
		  
		  return gn;
	  }

	  /* t may be zero, but we need to compute s*u1 anyway */
	  if (sn >= u1n)
		  mpn_mul(s0p, s, sn, u1, u1n);
	  else
		  mpn_mul(s0p, u1, u1n, s, sn);

	  (*s0size) = sn + u1n;
	  (*s0size) -= (s0p[sn + u1n - 1] == 0);

	  ASSERT(s0p[*s0size - 1] > 0); /* {s0p, *s0size} is normalised now */

	  if (tn == 0) /* case is rare */
	  {
		  if (swapped ^ negate) (*s0size) = -(*s0size);
        TMP_FREE;
	    
		return gn;
	  }

	  /* Now compute the rest of the cofactor, t*u0
	     and subtract it
		  We're done with u1 and s which happen to be
		  consecutive, so use that space
	  */

	  ASSERT(u1 + tn + u0n <= t);

     if (tn > u0n)
		  mpn_mul(u1, t, tn, u0, u0n);
	  else
		  mpn_mul(u1, u0, u0n, t, tn);

	  u1n = tn + u0n;
	  u1n -= (u1[tn + u0n - 1] == 0);

	  ASSERT(u1[u1n - 1] > 0);

	  /* Recall t is now negated so s*u1 - t*u0 
	     involves an *addition* 
	  */

	  if ((*s0size) >= u1n)
	  {
		  cy = mpn_add(s0p, s0p, *s0size, u1, u1n);
		  if (cy) s0p[(*s0size)++] = cy;
	  }
	  else
	  {
		  cy = mpn_add(s0p, u1, u1n, s0p, *s0size);
        (*s0size) = u1n;
	     if (cy) s0p[(*s0size)++] = cy;
	  }

	  if (swapped ^ negate) (*s0size) = -(*s0size);
     TMP_FREE;  
     
	 return gn;
  }
}
#endif
