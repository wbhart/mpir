/*  mpn_rootrem  

Copyright 2009 Jason Moxham

This file is part of the MPIR Library.

The MPIR Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation; either version 2.1 of the License, or (at
your option) any later version.

The MPIR Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPIR Library; see the file COPYING.LIB.  If not, write
to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.
*/


/*
   HERE IS A COPY OF THE OLD GMP ROOTREM WHICH WE RENAMED MPN-ROOTREM_BASECASE
   WE USE THIS FOR SMALL SIZES
   AND OF THE COURSE THE OLD GMP BOILERPLATE
*/

/* mpn_rootrem(rootp,remp,ap,an,nth) -- Compute the nth root of {ap,an}, and
   store the truncated integer part at rootp and the remainder at remp.

   THE FUNCTIONS IN THIS FILE ARE INTERNAL FUNCTIONS WITH MUTABLE
   INTERFACES.  IT IS ONLY SAFE TO REACH THEM THROUGH DOCUMENTED INTERFACES.
   IN FACT, IT IS ALMOST GUARANTEED THAT THEY'LL CHANGE OR DISAPPEAR IN A
   FUTURE GNU MP RELEASE.


Copyright 2002, 2005 Free Software Foundation, Inc.

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

/*
  We use Newton's method to compute the root of a:

           n
  f(x) := x  - a


            n - 1
  f'(x) := x      n


                                       n-1            n-1           n-1
                                x - a/x            a/x   - x     a/x   + (n-1)x
  new x = x - f(x)/f'(x) =  x - ----------  =  x + ---------  =  --------------
                                     n                 n                n

*/

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"

mp_size_t
mpn_rootrem_basecase (mp_ptr rootp, mp_ptr remp,mp_srcptr up, mp_size_t un, mp_limb_t nth)
{
  mp_ptr pp, qp, xp;
  mp_size_t pn, xn, qn;
  unsigned long int unb, xnb, bit;
  unsigned int cnt;
  mp_size_t i;
  unsigned long int n_valid_bits, adj;
  TMP_DECL;

  TMP_MARK;

  /* The extra factor 1.585 = log(3)/log(2) here is for the worst case
     overestimate of the root, i.e., when the code rounds a root that is
     2+epsilon to 3, and then powers this to a potentially huge power.  We
     could generalize the code for detecting root=1 a few lines below to deal
     with xnb <= k, for some small k.  For example, when xnb <= 2, meaning
     the root should be 1, 2, or 3, we could replace this factor by the much
     smaller log(5)/log(4).  */

#define PP_ALLOC (2 + (mp_size_t) (un*1.585))
  pp = TMP_ALLOC_LIMBS (PP_ALLOC);

  count_leading_zeros (cnt, up[un - 1]);
  unb = un * GMP_NUMB_BITS - cnt + GMP_NAIL_BITS;

  xnb = (unb - 1) / nth + 1;
  if (xnb == 1)
    {
      if (remp == 0)
	remp = pp;
      mpn_sub_1 (remp, up, un, (mp_limb_t) 1);
      MPN_NORMALIZE (remp, un);
      rootp[0] = 1;
      TMP_FREE;
      return un;
    }

  xn = (xnb + GMP_NUMB_BITS - 1) / GMP_NUMB_BITS;

  qp = TMP_ALLOC_LIMBS (PP_ALLOC);
  xp = TMP_ALLOC_LIMBS (xn + 1);

  /* Set initial root to only ones.  This is an overestimate of the actual root
     by less than a factor of 2.  */
  for (i = 0; i < xn; i++)
    xp[i] = GMP_NUMB_MAX;
  xp[xnb / GMP_NUMB_BITS] = ((mp_limb_t) 1 << (xnb % GMP_NUMB_BITS)) - 1;

  /* Improve the initial approximation, one bit at a time.  Keep the
     approximations >= root(U,nth).  */
  bit = xnb - 2;
  n_valid_bits = 0;
  for (i = 0; (nth >> i) != 0; i++)
    {
      mp_limb_t xl = xp[bit / GMP_NUMB_BITS];
      xp[bit / GMP_NUMB_BITS] = xl ^ (mp_limb_t) 1 << bit % GMP_NUMB_BITS;
      pn = mpn_pow_1 (pp, xp, xn, nth, qp);
      ASSERT_ALWAYS (pn < PP_ALLOC);
      /* If the new root approximation is too small, restore old value.  */
      if (! (un < pn || (un == pn && mpn_cmp (up, pp, pn) < 0)))
	xp[bit / GMP_NUMB_BITS] = xl;		/* restore old value */
      n_valid_bits += 1;
      if (bit == 0)
	goto done;
      bit--;
    }

  adj = n_valid_bits - 1;

  /* Newton loop.  Converges downwards towards root(U,nth).  Currently we use
     full precision from iteration 1.  Clearly, we should use just n_valid_bits
     of precision in each step, and thus save most of the computations.  */
  while (n_valid_bits <= xnb)
    {
      mp_limb_t cy;

      pn = mpn_pow_1 (pp, xp, xn, nth - 1, qp);
      ASSERT_ALWAYS (pn < PP_ALLOC);
      qp[xn - 1] = 0;		/* pad quotient to make it always xn limbs */
      mpn_tdiv_qr (qp, pp, (mp_size_t) 0, up, un, pp, pn); /* junk remainder */
      cy = mpn_addmul_1 (qp, xp, xn, nth - 1);
      if (un - pn == xn)
	{
	  cy += qp[xn];
	  if (cy == nth)
	    {
	      for (i = xn - 1; i >= 0; i--)
		qp[i] = GMP_NUMB_MAX;
	      cy = nth - 1;
	    }
	}

      qp[xn] = cy;
      qn = xn + (cy != 0);

      mpn_divrem_1 (xp, 0, qp, qn, nth);
      n_valid_bits = n_valid_bits * 2 - adj;
    }

  /* The computed result might be one unit too large.  Adjust as necessary.  */
 done:
  pn = mpn_pow_1 (pp, xp, xn, nth, qp);
  ASSERT_ALWAYS (pn < PP_ALLOC);
  if (un < pn || (un == pn && mpn_cmp (up, pp, pn) < 0))
    {
      mpn_decr_u (xp, 1);
      pn = mpn_pow_1 (pp, xp, xn, nth, qp);
      ASSERT_ALWAYS (pn < PP_ALLOC);

      ASSERT_ALWAYS (! (un < pn || (un == pn && mpn_cmp (up, pp, pn) < 0)));
    }

  if (remp == 0)
    remp = pp;
  mpn_sub (remp, up, un, pp, pn);
  MPN_NORMALIZE (remp, un);
  MPN_COPY (rootp, xp, xn);
  TMP_FREE;
  return un;
}


#if 0 /* this code needs more work to be faster than that in rootrem.c */

/* HERE IS THE NEW CODE */

/*
   TODO

   For large k we can calulate x^k faster as a float ie exp(k*ln(x)) or 
   x^(1/k) = exp(ln(x)/k).

   rather than doing it bitwise , round up all the truncation to the next limb,
   this should save quite a lot of shifts, don't know how much this will save (if 
   any) in practice.
        
   The powering is now a base2 left to right binary expansion , we could the usual 
   sliding base 2^k expansion, although the most common roots are small so this is 
   not likely to give us much in the common case.
        
   As most roots are for small k , we can do the powering via an optimized addition 
   chain, ie some sort of table lookup.
        
   Merge this reciprocal with our reciprocal used in our barratt (and/or newton 
   division).
        
   Currently we calc x^(1/k) as	(x^(-1/k))^(-1/1)
   or (x^(-1/1))^(-1/k)  
   could also try x(x^(-1/k)^(k-1))	(*)
   or (x^(-1/a))^(-1/b)  where k=ab    
   this last one is SLOWER as hi.gh k is fast as so make out computation as small as 
   possible as fast as possible
        
   So (*) is the only alternative, which I guess is only faster for small k ???
        
   Rewrite in term of mpf (or similar) like it was when I started, but I lost it, 
   will make the code below much clearer and smaller.
        
   multrunc can use high half mul.

   if k < 496 (32 bit cpus) then nroot_vsmall can be further reduced for a nroot_vvsmall.
        
   change signed long etc to mp_size_t ?  mainly for MSVC.
        
   At the moment we have just one threshold, need a separate one for each k, and some 
   sort of rule for large k.
*/


/* Algortihms from "Detecting Perfect Powers in Essentially Linear Time" ,
   Daniel J Bernstein  http://cr.yp.to/papers.html   */

/* define this to 1 to test the nroot_small code */
#define TESTSMALL 0

/* if k<=floor((2^(GMP_LIMB_BITS-1)-33)/66) &&  k<=2^(GMP_LIMB_BITS-4) then call vsmall
         ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ is always the smallest */
#define NROOT_VSMALL_MIN (((((mp_limb_t)1)<<(GMP_LIMB_BITS - 1)) - 33)/66)

/* shiftrights requires an extra gmp_numb_bits */
#define shiftright(x,xn,c)							\
   do { \
      (xn)=(xn)-(c)/GMP_NUMB_BITS;							\
      if((c)%GMP_NUMB_BITS != 0)	{ \
         mpn_rshift((x),(x) + (c)/GMP_NUMB_BITS, (xn), (c)%GMP_NUMB_BITS);		\
         if((x)[(xn) - 1] == 0) (xn)--; \
      }						\
      else \
      { \
      if ((c)/GMP_NUMB_BITS != 0) \
         MPN_COPY_INCR((x),(x) + (c)/GMP_NUMB_BITS,(xn)); \
      } \
   } while(0)

// shiftrights requires an extra gmp_numb_bits
#define shiftrights(x, xn, y, yn, c)					\
   do { \
      (xn) = (yn) - (c)/GMP_NUMB_BITS;						\
      if ((c)%GMP_NUMB_BITS != 0) { \
         mpn_rshift((x), (y) + (c)/GMP_NUMB_BITS, (xn), (c)%GMP_NUMB_BITS);	\
         if((x)[(xn) - 1] == 0) (xn)--; \
      }					\
      else  \
      { \
         MPN_COPY_INCR((x), (y) + (c)/GMP_NUMB_BITS, (xn)); \
      }			\
   } while(0)

#define shiftleft(x, xn, c)						\
   do { \
      mp_limb_t __t;							\
      if ((c)%GMP_NUMB_BITS != 0) { \
         __t = mpn_lshift((x) + (c)/GMP_NUMB_BITS, (x), (xn), (c)%GMP_NUMB_BITS);	\
         (xn) = (xn) + (c)/GMP_NUMB_BITS;					\
         if (__t != 0) \
            (x)[(xn)] = __t;(xn)++; \
      } \
      else									\
      { 
         if ((c)/GMP_NUMB_BITS != 0) {
            MPN_COPY_DECR((x) + (c)/GMP_NUMB_BITS, (x), (xn));			\
            (xn) = (xn) + (c)/GMP_NUMB_BITS; \
         } \
      }					\
      if ((c)/GMP_NUMB_BITS != 0) \
         MPN_ZERO((x), (c)/GMP_NUMB_BITS);		\
   } while(0)

#define shiftlefts(x, xn, y, yn, c)						\
   do { \
      mp_limb_t __t;							\
      if ((c)%GMP_NUMB_BITS != 0) { \
         __t = mpn_lshift((x) + (c)/GMP_NUMB_BITS, (y), (yn), (c)%GMP_NUMB_BITS);	\
         (xn) = (yn) + (c)/GMP_NUMB_BITS;					\
         if (__t != 0) \
           (x)[(xn)]=__t;(xn)++; \
      }				\
      else \
      {     \
         MPN_COPY_DECR((x) + (c)/GMP_NUMB_BITS, (y), (yn));			\
        (xn) = (yn) + (c)/GMP_NUMB_BITS; \
      }					\
      if ((c)/GMP_NUMB_BITS != 0) \
         MPN_ZERO((x), (c)/GMP_NUMB_BITS);		\
   } while(0)

#define mul_ui(x, xn, k) \
   do { \
      mp_limb_t __t = mpn_mul_1((x), (x), (xn), (k)); \
      if (__t != 0) { \
         (x)[(xn)] = __t; \
         (xn)++; \
      } \
   }while(0)

/* tdiv_q_ui requires an extra gmp_numb_bits */
#define tdiv_q_ui(x, xn, k) \
   do { \
      mpn_divrem_1((x), 0, (x), (xn), (k)); \
      if ((x)[(xn)-1] == 0) \
      (xn)--; \
   } while(0)

/* bigmultrunc requires an extra gmp_numb_bits */
#define bigmultrunc(xv, xn, xp, yv, yn, yp, B)		\
   do { \
      signed long __f; \
      mp_limb_t __t;			\
      (xp) += (yp);						\
      if ((xn) >= (yn))  \
         __t = mpn_mul(t1, (xv), (xn), (yv), (yn));	\
      else \
         __t = mpn_mul(t1, (yv), (yn), (xv), (xn));		\
      t1n = (xn) + (yn); \
      if (__t == 0) t1n--;			\
      __f = sizetwo(t1,t1n); \
      __f = __f - (B);			\
      if (__f > 0) { \
         shiftrights((xv), (xn), t1, t1n, __f); \
         (xp) += __f; \
      }	\
      else \
      { \
         MPN_COPY_INCR((xv), t1, t1n); \
         (xn) = t1n; \
      }		\
   } while(0)

/* bigsqrtrunc requires an extra gmp_numb_bits */
#define bigsqrtrunc(xv, xn, xp, B)				\
   do { \
      signed long __f;					\
      (xp) += (xp);						\
      mpn_sqr(t1, (xv), (xn));				\
      t1n = (xn)*2; \
      if (t1[t1n - 1] == 0) t1n--;			\
      __f = sizetwo(t1,t1n); \
      __f = __f - (B);			\
      if (__f > 0) { \
         shiftrights((xv), (xn), t1, t1n, __f); \
         (xp) += __f; \
      }	\
      else \
      { \
         MPN_COPY_INCR((xv), t1, t1n); \
         (xn) = t1n; \
      }		\
   } while(0)

/* must have y > z value wise */
#define subtract(x, xn, y, yn, z, zn)			\
   do { \
      mpn_sub((x), (y), (yn), (z), (zn)); /* no carry */	\
      (xn) = (yn); \
      while ((x)[(xn) - 1] == 0)(xn)--;		\
   } while(0)

/* returns ceil(lg(x)) where x != 0 */
signed long
clg (unsigned long x)
{
   mp_limb_t t;

   ASSERT (x != 0);

#if BITS_PER_ULONG <= GMP_LIMB_BITS
   if (x == 1)
      return 0;
   
   count_leading_zeros (t, (mp_limb_t) (x - 1));
   
   return GMP_LIMB_BITS - t;
#endif

#if BITS_PER_ULONG > GMP_LIMB_BITS
#error FIXME
#endif
}

// returns sizeinbase(x,2) x!=0
static inline signed long
sizetwo (mp_srcptr x, mp_size_t xn)
{
  signed long r;
  count_leading_zeros (r, x[xn - 1]);
  return xn * GMP_NUMB_BITS + GMP_NAIL_BITS - r;
}

//          returns sizeinbase(x-1,2) and returns 0 if x=1
static inline signed long
sizetwom1 (mp_srcptr x, mp_size_t xn)
{
  signed long r, i;
  mp_limb_t v;

  ASSERT (xn > 1 || (xn == 1 && x[0] != 0));
  if (xn == 1 && x[0] == 1)
    return 0;
  r = sizetwo (x, xn);
  i = xn - 1;
  v = x[i];
  if ((v & (v - 1)) != 0)
    return r;
  for (i--; i >= 0; i--)
    if (x[i] != 0)
      return r;
  return r - 1;
}


/* Algorithm B

   Calculates Z such that Z*(1-2^(-b)) < Y^(-1/k) < Z*(1+2^(-b)) 
   ie a b bit approximation the reciprocal of the kth root of Y
   where Z,Y>0 are real , 1<=b<=3+ceil(lg(k)) is an int , k>=1 is an int
   
   Z={z,zn}*2^zp	where zp is the return value , and zn is modified
   Y={y,yn}*2^yp	where {y,yn}>=2 and leading limb of {y,yn} is not zero
   {z,zn} requires space for GMP_LIMB_BITS+4 bits
   {z,zn} and {y,yn} must be completly distinct
*/
static signed long
nroot_small (mp_ptr z, mp_size_t * zn, mp_srcptr y, mp_size_t yn,
	     signed long yp, mp_limb_t b, mp_limb_t k)
{
  signed long zp, f, j, g, B, t2p, t3p, t4p;
  int ret;
  mp_limb_t mask;
  mp_size_t t1n, t2n, t3n, t4n;
  mp_limb_t t1[BITS_TO_LIMBS (2 * (GMP_LIMB_BITS + 8) + GMP_NUMB_BITS)];
  mp_limb_t t2[BITS_TO_LIMBS (GMP_LIMB_BITS + 8 + GMP_NUMB_BITS)];
  mp_limb_t t3[BITS_TO_LIMBS (GMP_LIMB_BITS + 8 + 2 + GMP_NUMB_BITS)];
  mp_limb_t t4[BITS_TO_LIMBS (GMP_LIMB_BITS + 8 + GMP_NUMB_BITS)];

  ASSERT (k != 0);
  ASSERT (b >= 1);
  ASSERT (b <= (mp_limb_t) (clg (k) + 3));	// bit counts are maximums , ie can have less
  ASSERT (yn > 1 || (yn == 1 && y[0] >= 2));
  ASSERT (y[yn - 1] != 0);
  g = sizetwom1 (y, yn);
  g = g + yp;
  g = -g;
  if (g >= 0)
    {
      g = g / k;
    }
  else
    {
      g = -((k - 1 - g) / k);
    }
  B = 66 * (2 * k + 1);
  B = clg (B);
  ASSERT (B <= GMP_LIMB_BITS + 8);
  ASSERT (b + 1 <= GMP_LIMB_BITS + 4);
  f = sizetwo (y, yn);
  if (f > B)
    {
      shiftrights (t4, t4n, y, yn, f - B);
      t4p = yp + f - B;
    }
  else
    {
      MPN_COPY_INCR (t4, y, yn);
      t4n = yn;
      t4p = yp;
    }				// t4 has B bits+numb space
  *zn = 1;
  z[0] = 3;
  zp = g - 1;			// z has 2 bits
  for (j = 1; (unsigned long) j < b; j++)
    {
      f = sizetwo (z, *zn);
      if (f > B)
	{
	  shiftrights (t2, t2n, z, *zn, f - B);
	  t2p = zp + f - B;
	}
      else
	{
	  MPN_COPY_INCR (t2, z, *zn);
	  t2n = *zn;
	  t2p = zp;
	}			// t2 has B bits+numb space
      if (k != 1)
	{
	  MPN_COPY_INCR (t3, t2, t2n);
	  t3n = t2n;
	  t3p = t2p;		// t3 has B bits
	  mask = (((mp_limb_t) 1) << (GMP_LIMB_BITS - 1));
	  while ((mask & k) == 0)
	    mask >>= 1;
	  mask >>= 1;
	  for (; mask != 0; mask >>= 1)
	    {
	      bigsqrtrunc (t2, t2n, t2p, B);	// t2 has B bits+numb space , t1 has 2*B bits+numb space
	      if ((k & mask) != 0)
		{
		  bigmultrunc (t2, t2n, t2p, t3, t3n, t3p, B);
		}
	    }
	}			// t2 has B bits+numb space , t1 has 2*B bits+numb space
      bigmultrunc (t2, t2n, t2p, t4, t4n, t4p, B);	// t2 has B bits+numb space , t1 has 2*B bits+numb space
      ret = 0;
      f = sizetwo (t2, t2n);
      if (f - 1 <= 8 - (t2p + 10))
	ret = 1;
      if (f - 1 >= 10 - (t2p + 10))
	ret = 0;
      if (f - 1 == 9 - (t2p + 10))
	{			// so 512 <= t2.2^(t2p+10) < 1024
	  if (t2p + 10 >= 0)
	    {
	      shiftlefts (t3, t3n, t2, t2n, t2p + 10);
	    }			// t3 has 10 bits
	  else
	    {
	      shiftrights (t3, t3n, t2, t2n, -t2p - 10);
	    }			// t3 has 10 bits+numb space
	  if (t3n == 1 && t3[0] <= 993)
	    ret = 1;
	}
      if (ret != 0)
	{
	  shiftleft (z, *zn, zp - (g - j - 1));	// z has j+2 bits
	  {
	    mp_limb_t __t;
	    __t = mpn_add_1 (z, z, *zn, 1);
	    if (__t != 0)
	      {
		z[*zn] = 1;
		(*zn)++;
	      }
	  }
	  zp = g - j - 1;
	  continue;
	}
      f = sizetwom1 (t2, t2n);
      if (f + t2p >= 1)
	{
	  shiftleft (z, *zn, zp - (g - j - 1));
	  mpn_sub_1 (z, z, *zn, 1);
	  zp = g - j - 1;
	}
    }				// z has j+2 bits
  return zp;
}				// z has b+1 bits


/* Algorithm B for k<=NROOT_VSMALL_MIN=(((((mp_limb_t)1)<<(GMP_LIMB_BITS-1))-33)/66)

   Calculates Z such that Z*(1-2^(-b)) < Y^(-1/k) < Z*(1+2^(-b)) 
   ie a b bit approximation the reciprocal of the kth root of Y
   where Z,Y>0 are real , 1<=b<=3+ceil(lg(k)) is an int , k>=1 is an int
   
   Z=z[0]*2^zp		where zp is the return value
   Y={y,yn}*2^yp	where {y,yn}>=2 and leading limb of {y,yn} is not zero
   {z,1} and {y,yn} must be completly distinct
   
   Note : the restriction on k allows calculations to be less than limb sized
   assumes GMP_LIMB_BITS>=10
   
*/
static signed long
nroot_vsmall (mp_ptr z, mp_srcptr y, mp_size_t yn, signed long yp,
	      mp_limb_t b, mp_limb_t k)
{
  signed long f1, zp, f, j, g, B, t1p, t2p, t3p;
  int ret;
  mp_limb_t t1, t2, t3, qh, ql, mask;

  ASSERT (k != 0);
  ASSERT (b >= 1);
  ASSERT (b <= (mp_limb_t) (clg (k) + 3));
  ASSERT (yn > 1 || (yn == 1 && y[0] >= 2));
  ASSERT (y[yn - 1] != 0);
  ASSERT (GMP_LIMB_BITS >= 10);
  ASSERT (k <= NROOT_VSMALL_MIN);

  g = sizetwom1 (y, yn);
  B = 66 * (2 * k + 1);
  B = clg (B);

  ASSERT (B <= GMP_LIMB_BITS);
  ASSERT (b <= GMP_LIMB_BITS - 1);

#if GMP_NAIL_BITS == 0
  t3p = yp;
  t3 = y[yn - 1];
  count_leading_zeros (f1, t3);
  f = yn * GMP_NUMB_BITS + GMP_NAIL_BITS - f1;	//related to g(internally)
  f1 = GMP_LIMB_BITS - f1;
  if (f1 >= B)
    {
      t3 >>= f1 - B;
      t3p += f - B;
    }
  else
    {
      if (yn != 1)
	{
	  t3 = (t3 << (B - f1)) | ((y[yn - 2]) >> (GMP_LIMB_BITS - B + f1));
	  t3p += f - B;
	}
    }
#endif

#if GMP_NAIL_BITS != 0
#if GMP_NUMB_BITS*2 < GMP_LIMB_BITS
#error not supported
#endif

  f = sizetwo (y, yn);
  if (f > B)
    {
      mp_limb_t t3t[2];
      mp_size_t t3n;
      t3p = yp + f - B;
      shiftrights (t3t, t3n, y, yn, f - B);
      t3 = t3t[0];
    }
  else
    {
      t3p = yp;
      if (f <= GMP_NUMB_BITS)
	{
	  t3 = y[0];
	}
      else
	{
	  t3 = (y[0] | (y[1] << (GMP_NUMB_BITS)));
	}
    }
#endif

  g = g + yp;
  g = -g;
  if (g >= 0)
    {
      g = g / k;
    }
  else
    {
      g = -((k - 1 - g) / k);
    }
  z[0] = 3;
  zp = g - 1;
  for (j = 1; (unsigned long) j < b; j++)
    {
      count_leading_zeros (f, z[0]);
      f = GMP_LIMB_BITS - f;
      if (f > B)
	{
	  t1 = (z[0] >> (f - B));
	  t1p = zp + f - B;
	}
      else
	{
	  t1 = z[0];
	  t1p = zp;
	}
      if (k != 1)
	{
	  t2 = t1;
	  t2p = t1p;
	  mask = (((mp_limb_t) 1) << (GMP_LIMB_BITS - 1));
	  while ((mask & k) == 0)
	    mask >>= 1;
	  mask >>= 1;
	  for (; mask != 0; mask >>= 1)
	    {
	      umul_ppmm (qh, ql, t1, t1);
	      t1p += t1p;
	      if (qh == 0)
		{		//count_leading_zeros(f,ql);f=GMP_LIMB_BITS-f;f=f-B;if(f>0){t1=(ql>>f);t1p+=f;}else{t1=ql;}
		  t1 = ql;	// be lazy
		}
	      else
		{
		  count_leading_zeros (f, qh);
		  f = 2 * GMP_LIMB_BITS - f;
		  f = f - B;
		  t1p += f;	//only need these cases when B>=16
		  if (f < GMP_LIMB_BITS)
		    {
		      t1 = (ql >> f);
		      t1 |= (qh << (GMP_LIMB_BITS - f));
		    }
		  else
		    {
		      t1 = (qh >> (f - GMP_LIMB_BITS));
		    }
		}
	      if ((k & mask) != 0)
		{
		  umul_ppmm (qh, ql, t1, t2);
		  t1p += t2p;
		  if (qh == 0)
		    {		//count_leading_zeros(f,ql);f=GMP_LIMB_BITS-f;f=f-B;if(f>0){t1=(ql>>f);t1p+=f;}else{t1=ql;}
		      t1 = ql;	// be lazy
		    }
		  else
		    {
		      count_leading_zeros (f, qh);
		      f = 2 * GMP_LIMB_BITS - f;
		      f = f - B;
		      t1p += f;
		      if (f < GMP_LIMB_BITS)
			{
			  t1 = (ql >> f);
			  t1 |= (qh << (GMP_LIMB_BITS - f));
			}
		      else
			{
			  t1 = (qh >> (f - GMP_LIMB_BITS));
			}
		    }
		}
	    }
	}
      umul_ppmm (qh, ql, t1, t3);
      t1p += t3p;
      if (qh == 0)
	{
	  count_leading_zeros (f, ql);
	  f = GMP_LIMB_BITS - f;
	  f = f - B;
	  if (f > 0)
	    {
	      t1 = (ql >> f);
	      t1p += f;
	    }
	  else
	    {
	      t1 = ql;
	    }
	  // dont be lazy here as it could screw up the compairison below
	}
      else
	{
	  count_leading_zeros (f, qh);
	  f = 2 * GMP_LIMB_BITS - f;
	  f = f - B;
	  t1p += f;
	  if (f < GMP_LIMB_BITS)
	    {
	      t1 = (ql >> f);
	      t1 |= (qh << (GMP_LIMB_BITS - f));
	    }
	  else
	    {
	      t1 = (qh >> (f - GMP_LIMB_BITS));
	    }
	}
      ret = 0;
      ASSERT (t1 != 0);
      count_leading_zeros (f, t1);
      f = GMP_LIMB_BITS - f;
      if (f - 1 <= 8 - (t1p + 10))
	ret = 1;
      if (f - 1 >= 10 - (t1p + 10))
	ret = 0;
      if (f - 1 == 9 - (t1p + 10))
	{			// so 512 <= t1.2^(t1p+10) < 1024
	  if (t1p + 10 >= 0)
	    {
	      t2 = (t1 << (t1p + 10));
	    }
	  else
	    {
	      t2 = (t1 >> (-t1p - 10));
	    }
	  if (t2 <= 993)
	    ret = 1;
	}
      if (ret != 0)
	{
	  z[0] = (z[0] << (zp - (g - j - 1)));
	  z[0]++;
	  zp = g - j - 1;
	  continue;
	}
      if (t1 == 1)
	{
	  f = 0;
	}
      else
	{
	  count_leading_zeros (f, t1 - 1);
	  f = GMP_LIMB_BITS - f;
	}
      if (f + t1p >= 1)
	{
	  z[0] = (z[0] << (zp - (g - j - 1)));
	  z[0]--;
	  zp = g - j - 1;
	}
    }
  return zp;
}

/* Algorithm N

   Calculates Z such that Z*(1 - 2^(-b)) < Y^(-1/k) < Z*(1 + 2^(-b)) 
   ie a b bit approximation the reciprocal of the kth root of Y
   where Z,Y > 0 are real , b >= 1 is an int, k >= 1 is an int
   
   Z={z, zn}*2^zp	where zp is the return value , and zn is modified
   Y={y, yn}*2^yp	where {y, yn} >= 2 and leading limb of {y, yn} is not zero

   z  satisfies 1 <= z < 2^(b + 7)
   zp satisfies -lg(Y)/k - b - 7 - lg(3/2) < zp < -lg(Y)/k + 1
   
   {z, zn} and {y, yn} and temps t1, t2, t3 must be completely distinct
   z  requires b + 6 + GMP_NUMB_BITS + max(1, clgk)
   t1 requires max(2*b + 12 + GMP_NUMB_BITS, b + 6 + clg(k + 1))   
   t2 requires b + 6 + GMP_NUMB_BITS
   t3 requires b + 6 + GMP_NUMB_BITS
*/
static signed long
nroot(mp_ptr z, mp_size_t * zn, mp_srcptr y, mp_size_t yn, signed long yp,
       mp_limb_t b, mp_limb_t k, signed long clgk, mp_ptr t1, mp_ptr t2, mp_ptr t3)
{ 
   mp_size_t t1n, t2n, t3n; 
   mp_limb_t mask, kpow2, k1pow2;
   signed long t1p, zp, t2p, t3p, f, bd, bs[GMP_LIMB_BITS * 2], c;	/* FIXME how many */
  
   ASSERT(k != 0);
   ASSERT(yn > 1 || (yn == 1 && y[0] >= 2));
   ASSERT (y[yn - 1] != 0);
  
   bs[0] = b;			/* bit counts are maximums, i.e. can have less */
  
   for (c = 0; ; c++) / *bs[c] <= 3 + clgk */
   { 
      if (bs[c] <= 3 + clgk)
	      break;
      
      bs[c + 1] = 1 + (bs[c] + clgk) / 2;   
   }		

#if GMP_LIMB_BITS >= 10 && TESTSMALL == 0
  if (k <= NROOT_VSMALL_MIN)
  { 
     zp = nroot_vsmall(z, y, yn, yp, bs[c], k); 
     *zn = 1; 
  }
  else
  { 
     zp = nroot_small(z, (mp_limb_t *) zn, y, yn, yp, bs[c], k); 
  }
#endif

/* bs[1] = 1 + floor((b + clgk)/2) max bd = b + 6, z has bs[c]+1 bits */
#if GMP_LIMB_BITS < 10 || TESTSMALL == 1 
  zp = nroot_small (z, zn, y, yn, yp, bs[c], k);
#endif 
  
  kpow2 = 0;
  k1pow2 = 0; /* shortcut for div, mul to a shift instead */
  
  if (POW2_P(k)) /* k=2^(kpow2 - 1) */
  {
     count_leading_zeros(kpow2, k);
     kpow2 = GMP_LIMB_BITS - kpow2;
  }

  if (POW2_P(k + 1)) /* k + 1 = 2^(k1pow2-1) */
  {
     count_leading_zeros(k1pow2, k + 1);
     k1pow2 = GMP_LIMB_BITS - k1pow2;
  }	
  
  for ( ; c != 0; c--)
  {  bd = 2 * bs[c] + 4 - clgk;
     f = sizetwo (z, *zn);	/* is this trunc ever going to do something real? */
     if (f > bd) 
     { 
        shiftright (z, *zn, f - bd); 
        zp = zp + f - bd;
     }			
     /* z has bd bits + numb space */

     MPN_COPY_INCR (t3, z, *zn); t3n = *zn;t3p = zp;
      
     mask = (((mp_limb_t) 1) << (GMP_LIMB_BITS - 1));	/* t3 has bd bits */
     while ((mask & (k + 1)) == 0) mask >>= 1;
      
     for (mask >>= 1; mask != 0; mask >>= 1)
	  { 
        /* t3 has bd bits + numb space, t1 has 2*bd bits + numb space */
        bigsqrtrunc (t3, t3n, t3p, bd);	
	  
        if (((k + 1) & mask) != 0)
        {
           bigmultrunc (t3, t3n, t3p, z, *zn, zp, bd);
        }
     }
     
     /* t3 has bd bits + numb space t1 has 2*bd bits + numb space */
      
     if (k1pow2)
     {
        shiftleft(z, *zn, k1pow2 - 1);
     } else
     {
        mul_ui(z, *zn, k + 1);
     }
     
     /* z has bd+clg(k + 1) bits */
      
     f = sizetwo (y, yn);
     if (f > bd) { 
        shiftrights (t2, t2n, y, yn, f - bd);
        t2p = yp + f - bd;

        /* t2 has bd bits + numb space */
     }	   
     else /* this case may not happen if this is only called by mpn_root */
     { 
        MPN_COPY_INCR (t2, y, yn);
        t2n = yn;
        t2p = yp;
     }	

     bigmultrunc (t3, t3n, t3p, t2, t2n, t2p, bd);	
     
     /* t3 has bd bits + numb space t1 has 2*bd bits + numb space */
      
     if (zp <= t3p) /* which branch depends on yp ? and only want top bd + clgk bits exactly */
	  { 
        shiftlefts(t1, t1n, t3, t3n, t3p - zp);	/* t1 has bd + clg(k + 1) bits */
	     subtract(t1, t1n, z, *zn, t1, t1n);
	     t1p = zp;
        
        /* t1 has bd + clg(k + 1) bits */
     } else
	  { 
        ASSERT(zp - t3p + sizetwo (z, *zn) <= 2 * b + 12 + GMP_NUMB_BITS); //* not allocated enough mem */
	  
        shiftlefts(t1, t1n, z, *zn, zp - t3p);	/* t1 has 2*b+12+numb */
	     subtract(t1, t1n, t1, t1n, t3, t3n);
	     t1p = t3p;
     }			
     
     /* t1 has 2*b + 12 + numb */
      
     f = sizetwo (t1, t1n);
     if (f >= bd + clgk)
        shiftrights(z, *zn, t1, t1n, f - bd - clgk);
     else
        shiftlefts(z, *zn, t1, t1n, bd + clgk - f); 
       
     /* z has bd + clgk bits + numb space */

     zp = t1p + f - bd - clgk;
     if (kpow2)
        shiftright(z, *zn, kpow2 - 1);
     else
        tdiv_q_ui (z, *zn, k);
    }	
  
     /* z has bd + 1 bits + numb space (maybe prove just bd bits ?) */
  return zp;
}

/* same as Algorithm N but for k = 1

   Calculates Z such that Z*(1 - 2^(-b)) < Y^(-1/k) < Z*(1 + 2^(-b)) 
   ie a b bit approximation the reciprocal of the kth root of Y
   where Z,Y > 0 are real, b >= 1 is an int, k >= 1 is an int
   
   Z={z, zn}*2^zp	where zp is the return value, and zn is modified
   Y={y, yn}*2^yp	where {y, yn} >= 2 and leading limb of {y, yn} is not zero

   and z  satisfies 2^b <= z <= 2^(b + 1)
   and zp satisfies zp = -sizetwo(y, yn) - b - yp

   {z, zn} and {y, yn} and temps t1, t2 must be completely distinct
   z  requires 2 + floor(((sizetwo(y, yn) + b + 1)/GMP_NUMB_BITS) - yn limbs
   t1 requires 1 + floor((sizetwo(y, yn) + b + 1)/GMP_NUMB_BITS) limbs
   t2 requires yn limbs   
*/
static signed long
finv_fast (mp_ptr z, int *zn, mp_srcptr y, mp_size_t yn, signed long yp,
	   unsigned long b, mp_ptr t1, mp_ptr t2)
{
  signed long c;
  signed long zp;
  mp_size_t t1n;

  c = sizetwo (y, yn) + b;
  MPN_COPY_INCR (t1, y, yn);
  t1n = yn;			/* t1 has yn limbs */
  
  MPN_ZERO(t1 + t1n, (c + 1) / GMP_NUMB_BITS + 1 - t1n);	 
  /* t1 has 1 + floor((c+1)/numb) limbs */
  
  t1[(c + 1) / GMP_NUMB_BITS] = (((mp_limb_t) 1) << ((c + 1) % GMP_NUMB_BITS));	
  /*  t1 has 1+floor((c+1)/numb) limbs */
  
  t1n = (c + 1) / GMP_NUMB_BITS + 1;
  
  ASSERT (y[yn - 1] != 0);
  
  mpn_tdiv_qr (z, t2, 0, t1, t1n, y, yn);	/*bdivmod could be faster */
  /* z has 2 + floor((c + 1)/numb) - yn, t2 has yn limbs */

  *zn = t1n - yn + 1;
  
  while (*zn != 0 && z[*zn - 1] == 0)
    (*zn)--;
  
  shiftright (z, *zn, 1);
  zp = -c - yp;
  
  return zp;
}


/* calculates X and R such that X^k <= Y and (X + 1)^k > Y
   where X = {x, xn}, Y = {y, yn}, R = {r, rn}, only calculates R if r != NULL
   
   R satisfies R < (X+1)^k-X^k
   X satisfies X^k <= Y
   
   X needs ceil(yn/k) limb space
   R needs yn limb space if r != 0
   return sizeof remainder if r != 0
*/
mp_size_t mpn_rootrem(mp_ptr xp, mp_ptr r, mp_srcptr y,mp_size_t yn, mp_limb_t k)
{
  unsigned long b, clgk;
  signed long d, tp, zp;
  mpz_t t4, t3;
  mp_ptr x, t1, t2;
  mp_size_t t2n, xn, rn;
  mp_limb_t val;
  mp_size_t pos, bit;
  
  if (BELOW_THRESHOLD(yn, ROOTREM_THRESHOLD))
     return mpn_rootrem_basecase(xp, r, y, yn, k);

  d = 8;			/* any d >= 1 will do, for testing to its limits use d = 1 TUNEME */
  b = sizetwo (y, yn);
  b = (b + k - 1) / k + 2 + d;
  clgk = clg(k);

  x = __GMP_ALLOCATE_FUNC_LIMBS(BITS_TO_LIMBS(b + 7 + GMP_NUMB_BITS));
  t1 = __GMP_ALLOCATE_FUNC_LIMBS(BITS_TO_LIMBS (2 * b + 12 + GMP_NUMB_BITS));
  t2 = __GMP_ALLOCATE_FUNC_LIMBS(BITS_TO_LIMBS (b + 6 + clgk + 1 + GMP_NUMB_BITS));
  
  mpz_init2(t3, b + 6 + GMP_NUMB_BITS * 2);
  mpz_init2(t4, b + 6 + GMP_NUMB_BITS);
  
  zp = nroot(t2, &t2n, y, yn, 0, b, k, clgk, t1, PTR (t3), PTR (t4));

  /*  1 <= t2 < 2^(b+7), -lg(Y)/k - b - 7 - lg(3/2) < zp < -lg(Y)/k + 1  where Y = {y,yn} */
  tp = finv_fast (PTR (t3), &SIZ (t3), t2, t2n, zp, b, t1, PTR (t4)); /* t3 is our approx root */

  /*  2^b <= t3 <= 2^(b+1)    tp=-sizetwo(t2,t2n)-b-zp  */
  ASSERT (tp <= -d - 1);
  
  pos = (-tp - d - 1 + 1) / GMP_NUMB_BITS;
  bit = (-tp - d - 1 + 1) % GMP_NUMB_BITS;
  val = (((mp_limb_t) 1) << bit);
  mpn_sub_1 (PTR (t3) + pos, PTR (t3) + pos, SIZ (t3) - pos, val);

  if (PTR (t3)[SIZ (t3) - 1] == 0)
    SIZ (t3)--;

  shiftrights (PTR (t4), SIZ (t4), PTR (t3), SIZ (t3), -tp);
  
  if (mpn_add_1 (PTR (t3) + pos, PTR (t3) + pos, SIZ (t3) - pos, val))
  {
     PTR (t3)[SIZ (t3)] = 1;
     SIZ (t3)++;
  }
  
  pos = (-tp - d - 1) / GMP_NUMB_BITS;
  bit = (-tp - d - 1) % GMP_NUMB_BITS;
  val = (((mp_limb_t) 1) << bit);
  
  if (mpn_add_1 (PTR (t3) + pos, PTR (t3) + pos, SIZ (t3) - pos, val))
  {
     PTR (t3)[SIZ (t3)] = 1;
     SIZ (t3)++;
  }
 
  shiftright (PTR (t3), SIZ (t3), -tp);

  if (mpz_cmp (t4, t3) == 0)
  {
     xn = SIZ (t3);
     MPN_COPY_INCR (x, PTR (t3), xn);
     
     if (r != 0)
	  {
	     mpz_pow_ui (t4, t3, k);
	     mpn_sub (r, y, yn, PTR (t4), SIZ (t4));	/* no carry */
	     rn = yn;
	     while (rn != 0 && r[rn - 1] == 0) rn--;
	  }
     
     mpz_clear (t4);
     mpz_clear (t3);
     
     MPN_COPY(xp, x, (yn + k - 1)/k);
      
     __GMP_FREE_FUNC_LIMBS(x, BITS_TO_LIMBS(b + 7 + GMP_NUMB_BITS));
     __GMP_FREE_FUNC_LIMBS(t1, BITS_TO_LIMBS(2*b + 12 + GMP_NUMB_BITS));
     __GMP_FREE_FUNC_LIMBS(t2, BITS_TO_LIMBS(b + 6 + clgk + 1 + GMP_NUMB_BITS));
      
     return rn;
  }

  mpz_pow_ui (t4, t3, k);

  if (SIZ (t4) > yn || (SIZ (t4) == yn && mpn_cmp(PTR (t4), y, yn) > 0))
  {
     mpz_sub_ui (t3, t3, 1);
     xn = SIZ (t3);
     MPN_COPY_INCR(x, PTR (t3), xn);
     if (r != 0)
	  {
	     mpz_pow_ui(t4, t3, k);
	     mpn_sub(r, y, yn, PTR (t4), SIZ (t4));	/* no carry */
	     rn = yn;
	     while (rn != 0 && r[rn - 1] == 0) rn--;
     }
      
     mpz_clear (t4);
     mpz_clear (t3);
      
     MPN_COPY(xp,x,(yn+k-1)/k);
      
     __GMP_FREE_FUNC_LIMBS(x, BITS_TO_LIMBS(b + 7 + GMP_NUMB_BITS));
     __GMP_FREE_FUNC_LIMBS(t1,BITS_TO_LIMBS(2*b+ 12 + GMP_NUMB_BITS));
     __GMP_FREE_FUNC_LIMBS(t2,BITS_TO_LIMBS(b + 6 + clgk + 1 + GMP_NUMB_BITS));
     
     return rn;
  }
  
  xn = SIZ(t3);
  MPN_COPY_INCR(x, PTR (t3), xn);
  
  if (r != 0)
  {
     mpn_sub(r, y, yn, PTR (t4), SIZ (t4));	/* no carry */
     rn = yn;
     
     while(rn != 0 && r[rn - 1] == 0) rn--;
  }
  
  mpz_clear(t4);
  mpz_clear(t3);
  
  MPN_COPY(xp, x, (yn + k - 1)/k);
  
  __GMP_FREE_FUNC_LIMBS(x, BITS_TO_LIMBS(b + 7 + GMP_NUMB_BITS));
  __GMP_FREE_FUNC_LIMBS(t1, BITS_TO_LIMBS(2*b + 12 + GMP_NUMB_BITS));
  __GMP_FREE_FUNC_LIMBS(t2, BITS_TO_LIMBS(b + 6 + clgk + 1 + GMP_NUMB_BITS));
  
  return rn;
}

#endif
