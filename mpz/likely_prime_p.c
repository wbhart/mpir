/*
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

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"

#if GMP_LIMB_BITS == 32 || GMP_LIMB_BITS == 64

#define D_BITS 53

typedef struct pair_s
{
	mp_limb_t x, y;
} n_pair_t;

#define r_shift(in, shift) \
   ((shift == GMP_LIMB_BITS) ? 0L : ((in)>>(shift)))

mp_limb_t n_sqrt(mp_limb_t r)
{
#if GMP_LIMB_BITS == 32
    float x, z;
	union {
	  float f;
      mp_limb_t l;
	} temp;

	temp.f = (float) r;
	temp.l = (0xbe6ec85eUL - temp.l)>>1; // estimate of 1/sqrt(y) 
	x =  temp.f;
	z =  (float) r*0.5;                        
    x = (1.5*x) - (x*x)*(x*z);
	x = (1.5*x) - (x*x)*(x*z);
	x = (1.5*x) - (x*x)*(x*z);
	x = (1.5*x) - (x*x)*(x*z);
    mp_limb_t res =  is + ((is+1)*(is+1) <= r);
    return res - (res*res > r);
#else
    mp_limb_t is;
	
	double x, z;
	union {
	  double f;
      mp_limb_t l;
	} temp;

	temp.f = (double) r;
	temp.l = (0xbfcdd90a00000000UL - temp.l)>>1; // estimate of 1/sqrt(y) 
	x =  temp.f;
	z =  (double) r*0.5;                        
    x = (1.5*x) - (x*x)*(x*z);
	x = (1.5*x) - (x*x)*(x*z);
	x = (1.5*x) - (x*x)*(x*z);
	x = (1.5*x) - (x*x)*(x*z);
	x = (1.5*x) - (x*x)*(x*z);
    is = (mp_limb_t) (x*(double) r);
    mp_limb_t res =  is + ((is+1)*(is+1) <= r);
    return res - (res*res > r);
#endif
}

double n_precompute_inverse(mp_limb_t n)
{
   return (double) 1 / (double) n;
}

unsigned int BIT_COUNT(mp_limb_t x)
{
   unsigned int zeros = GMP_LIMB_BITS;
   if (x) count_leading_zeros(zeros, x);
   return GMP_LIMB_BITS - zeros;
}

int mod64[64] = {1,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,
                 0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,
                 0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0}; 

int mod65[65] = {1,1,0,0,1,0,0,0,0,1,1,0,0,0,1,0,1,0,0,0,0,0,
                 0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,
                 0,0,0,0,0,1,0,1,0,0,0,1,1,0,0,0,0,1,0,0,1};

int mod63[63] = {1,1,0,0,1,0,0,1,0,1,0,0,0,0,1,0,1,0,1,0,0,
                 0,1,0,0,1,0,0,1,0,0,0,0,0,0,1,1,1,0,0,0,0,
                 0,1,0,0,1,0,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0};
   
int n_is_square(mp_limb_t x)
{
   if (!mod64[x%64UL]) return 0;
   if (!mod63[x%63UL]) return 0;
   if (!mod65[x%65UL]) return 0;

   mp_limb_t sq = n_sqrt(x); 
   
   return (x == sq*sq);
}

mp_limb_t n_preinvert_limb(mp_limb_t n)
{
   mp_limb_t norm, ninv;

   count_leading_zeros(norm, n);
   invert_limb(ninv, n<<norm);

   return ninv;
}

mp_limb_t n_addmod(mp_limb_t x, mp_limb_t y, mp_limb_t n)
{
   if (n - y > x) return x + y;
   else return x + y - n;
}

static inline
mp_limb_t n_submod(mp_limb_t x, mp_limb_t y, mp_limb_t n)
{
   if (y > x) return x - y + n;
   else return x - y;
}

mp_limb_t n_mod2_preinv(mp_limb_t a, mp_limb_t n, mp_limb_t ninv)
{
   unsigned int norm;
   mp_limb_t q, r;

   count_leading_zeros(norm, n);
   udiv_qrnnd_preinv(q, r, r_shift(a, GMP_LIMB_BITS-norm), a<<norm, n<<norm, ninv);
   return (r>>norm);
}

mp_limb_t n_ll_mod_preinv(mp_limb_t a_hi, mp_limb_t a_lo, 
                                             mp_limb_t n, mp_limb_t ninv)
{
   mp_limb_t q, r, norm;
   
   if (a_hi > n) a_hi = n_mod2_preinv(a_hi, n, ninv);

   count_leading_zeros(norm, n);
   
   udiv_qrnnd_preinv(q, r, (a_hi<<norm) + r_shift(a_lo, GMP_LIMB_BITS-norm), a_lo<<norm, n<<norm, ninv);

   return (r>>norm);
}

mp_limb_t n_mulmod_precomp(mp_limb_t a, mp_limb_t b, mp_limb_t n, double npre)
{
   mp_limb_t quot = (mp_limb_t) ((double) a * (double) b * npre);
   mp_limb_signed_t rem = a*b - quot*n;
   if (rem < 0) 
   {
      rem += n;
      if (rem < 0) return rem + n;
   } else if (rem >= n) return rem - n;
   return rem;
}

mp_limb_t n_mulmod2_preinv(mp_limb_t a, mp_limb_t b, mp_limb_t n, mp_limb_t ninv)
{
   mp_limb_t p1, p2;
   
   umul_ppmm(p1, p2, a, b);
   
   return n_ll_mod_preinv(p1, p2, n, ninv);
}

mp_limb_t n_powmod_precomp(mp_limb_t a, mp_limb_t exp, mp_limb_t n, double npre)
{
   if (n == 1UL) return 0L;
   
   mp_limb_t x, y;
   mp_limb_t e;

   e = exp;
   
   x = 1UL;
   y = a;

   while (e) 
   {
      if (e & 1L) x = n_mulmod_precomp(x, y, n, npre);
      e >>= 1;
      if (e) y = n_mulmod_precomp(y, y, n, npre);
   }

   return x;
}

mp_limb_t n_powmod2_preinv(mp_limb_t a, mp_limb_t exp, mp_limb_t n, mp_limb_t ninv)
{
   if (n == 1UL) return 0UL;
   
   mp_limb_t x, y;
   mp_limb_t e;
   
   e = exp;
   
   x = 1UL;
   y = a;
   while (e) 
   {
      if (e & 1) x = n_mulmod2_preinv(x, y, n, ninv);
      e = e >> 1;
      if (e) y = n_mulmod2_preinv(y, y, n, ninv);
   }

   return x;
} 

mp_limb_t n_powmod(mp_limb_t a, mp_limb_signed_t exp, mp_limb_t n)
{
   double npre = n_precompute_inverse(n);

   return n_powmod_precomp(a, exp, n, npre);
}

mp_limb_t n_powmod2(mp_limb_t a, mp_limb_signed_t exp, mp_limb_t n)
{
   mp_limb_t ninv = n_preinvert_limb(n);

   return n_powmod2_preinv(a, exp, n, ninv);
}

mp_limb_t n_gcd(mp_limb_t x, mp_limb_t y)
{
   mp_limb_t u3, v3;
   mp_limb_t quot, rem;
   
   u3 = x; v3 = y;
   
   if ((mp_limb_signed_t) (x & y) < 0L) /* x and y both have top bit set */ 
   {
     quot=u3-v3;
     u3 = v3;
     v3 = quot;
   }

   while ((mp_limb_signed_t) (v3<<1) < 0L) /* second value has second msb set */
   {
     quot=u3-v3;
     if (quot < v3)
     {
        u3 = v3;
        v3 = quot;
     } else if (quot < (v3<<1))
     {  
        u3 = v3;
        v3 = quot-u3;
     } else
     {
        u3 = v3;
        v3 = quot-(u3<<1);
     }
   }
   
   while (v3) {
      quot=u3-v3;
      if (u3 < (v3<<2)) /* overflow not possible due to top 2 bits of v3 not being set */
      {
         if (quot < v3)
         {
            u3 = v3;
            v3 = quot;
         } else if (quot < (v3<<1))
         {  
            u3 = v3;
            v3 = quot-u3;
         } else
         {
            u3 = v3;
            v3 = quot-(u3<<1);
         }
      } else
      {
         quot=u3/v3;
         rem = u3 - v3*quot;
         u3 = v3;
         v3 = rem;
      }
   }
   
   return u3;
}

mp_limb_t n_invmod(mp_limb_t x, mp_limb_t y)
{
   mp_limb_signed_t v1 = 0UL; 
   mp_limb_signed_t v2 = 1UL; 
   mp_limb_signed_t t2; 
   mp_limb_t u3, v3;
   mp_limb_t quot, rem;
   
   u3 = y, v3 = x;
   
   if (v3 > u3)
   {
         rem = u3;
         u3 = v3;
         t2 = v2; v2 = v1; v1 = t2; v3 = rem;
   }
   
   if ((mp_limb_signed_t) (y & x) < 0L) // y and x both have top bit set  
   {
     quot=u3-v3;
     t2 = v2; 
     u3 = v3;
     v2 = v1 - v2; v1 = t2; v3 = quot;
   }

   while ((mp_limb_signed_t) (v3<<1) < 0L) // second value has second msb set 
   {
     quot=u3-v3;
     if (quot < v3)
     {
        t2 = v2; 
        u3 = v3;
        v2 = v1 - v2; v1 = t2; v3 = quot;
     } else if (quot < (v3<<1))
     {  
        u3 = v3;
        t2 = v2; v2 = v1 - (v2<<1); v1 = t2; v3 = quot-u3;
     } else
     {
        u3 = v3;
        t2 = v2; v2 = v1 - 3*v2; v1 = t2; v3 = quot-(u3<<1);
     }
   }
   
   while (v3) {
      quot=u3-v3;
      if (u3 < (v3<<2)) // overflow not possible due to top 2 bits of v3 not being set 
      {
         if (quot < v3)
         {
            t2 = v2; 
            u3 = v3;
            v2 = v1 - v2; v1 = t2; v3 = quot;
         } else if (quot < (v3<<1))
         {  
            u3 = v3;
            t2 = v2; v2 = v1 - (v2<<1); v1 = t2; v3 = quot-u3;
         } else
         {
            u3 = v3;
            t2 = v2; v2 = v1 - 3*v2; v1 = t2; v3 = quot-(u3<<1);
         }
      } else
      {
         quot=u3/v3;
         rem = u3 - v3*quot;
         u3 = v3;
         t2 = v2; v2 = v1 - quot*v2; v1 = t2; v3 = rem;
      }
   }

   if (v1 < 0L) v1 += y;
      
   return v1;
}

int n_jacobi(mp_limb_signed_t x, mp_limb_t y)
{
	mp_limb_t a, b, temp;
	int s, exp;
	
   a = x;
	b = y;
	s = 1;

	if (x < 0L)
	{
		if (((b - 1)/2)%2 == 1UL)
		   s = -s;
		a = -x;
	} 

   if ((a < b) && (b != 1UL))
   {
      if (a == 0UL) return 0;
      
      temp = a;
      a = b;
      b = temp;

      count_trailing_zeros(exp, b);
	   b>>=exp;

      if (((exp*(a*a - 1))/8)%2 == 1UL) // we are only interested in values mod 8, 
		   s = -s;                        //so overflows don't matter here

		if ((((a - 1)*(b - 1))/4)%2 == 1UL) // we are only interested in values mod 4, 
		   s = -s;                          //so overflows don't matter here
   }

	while (b != 1UL)
	{
      if ((a>>2) < b)
      {
         temp = a - b;
         a = b;         
         if (temp < b)
            b = temp;
         else if (temp < (b<<1)) 
            b = temp - a;
         else
            b = temp - (a<<1);
      } else
      {
         temp = (a%b);
         a = b;
         b = temp;
      }

      if (b == 0UL) return 0;
      
      count_trailing_zeros(exp, b);
	   b>>=exp;

      if (((exp*(a*a - 1))/8)%2 == 1UL) // we are only interested in values mod 8, 
		   s = -s;                        //so overflows don't matter here

		if ((((a - 1)*(b - 1))/4)%2 == 1UL) // we are only interested in values mod 4, 
		   s = -s;                          //so overflows don't matter here
	}

	return s;
}

int n_is_pseudoprime_fermat(mp_limb_t n, mp_limb_t i)
{
	if (BIT_COUNT(n) <= D_BITS) return (n_powmod(i, n - 1, n) == 1UL);
   else 
   {
      if ((mp_limb_signed_t) (n - 1) < 0L)
      {
         mp_limb_t temp = n_powmod2(i, (n - 1)/2, n);
         return (n_powmod2(temp, 2, n) == 1UL);
      } else
      {
         return (n_powmod2(i, n - 1, n) == 1UL);
      }
   }
}

int n_is_strong_pseudoprime_precomp(mp_limb_t n, double npre, mp_limb_t a, mp_limb_t d)
{
   mp_limb_t t = d;
   mp_limb_t y;
      
   y = n_powmod_precomp(a, t, n, npre);

   if (y == 1UL) return 1;
   t <<= 1;

   while ((t != n - 1) && (y != n - 1))
   {
      y = n_mulmod_precomp(y, y, n, npre);
      t <<= 1;
   }
   
   return (y == n - 1);
}

int n_is_strong_pseudoprime2_preinv(mp_limb_t n, mp_limb_t ninv, mp_limb_t a, mp_limb_t d)
{
   mp_limb_t t = d;
   mp_limb_t y;
      
   y = n_powmod2_preinv(a, t, n, ninv);

   if (y == 1UL) return 1;
   t <<= 1;

   while ((t != n - 1) && (y != n - 1))
   {
      y = n_mulmod2_preinv(y, y, n, ninv);
      t <<= 1;
   }
   
   return (y == n - 1);
}

n_pair_t fchain_precomp(mp_limb_t m, mp_limb_t n, double npre)
{
	n_pair_t current, old;
	int length;
	mp_limb_t power, xy, xx, yy;
	
	old.x = 2UL;
	old.y = n - 3UL;
	
	length = BIT_COUNT(m);
	power = (1UL<<(length-1));

	for ( ; length > 0; length--)
	{
		xy = n_mulmod_precomp(old.x, old.y, n, npre);
		
		xy = n_addmod(xy, 3UL, n);
		
		if (m & power)
		{
			current.y = n_submod(n_mulmod_precomp(old.y, old.y, n, npre), 2UL, n);
			current.x = xy;
		} else 
		{
			current.x = n_submod(n_mulmod_precomp(old.x, old.x, n, npre), 2UL, n);
			current.y = xy;
		}

		power >>= 1;
		old = current;
	}

	return current;
}

n_pair_t fchain2_preinv(mp_limb_t m, mp_limb_t n, mp_limb_t ninv)
{
	n_pair_t current, old;
	int length;
	mp_limb_t power, xy, xx, yy;
	
	old.x = 2UL;
	old.y = n - 3UL;
	
	length = BIT_COUNT(m);
	power = (1UL<<(length-1));

	for ( ; length > 0; length--)
	{
		xy = n_mulmod2_preinv(old.x, old.y, n, ninv);
		
		xy = n_addmod(xy, 3UL, n);
		
		if (m & power)
		{
			current.y = n_submod(n_mulmod2_preinv(old.y, old.y, n, ninv), 2UL, n);
			current.x = xy;
		} else 
		{
			current.x = n_submod(n_mulmod2_preinv(old.x, old.x, n, ninv), 2UL, n);
			current.y = xy;
		}

		power >>= 1;
		old = current;
	}

	return current;
}

int n_is_pseudoprime_fibonacci(mp_limb_t n)
{
	mp_limb_t m, left, right;
	n_pair_t V;

   if (ABS((mp_limb_signed_t) n) <= 3UL)
   {
      if (n >= 2UL) return 1;
      return 0;
   }

	m = (n - n_jacobi(5L, n))/2; // cannot overflow as (5/n) = 0 for n = 2^64-1

   if (BIT_COUNT(n) <= D_BITS)
   {
      double npre = n_precompute_inverse(n);
	
      V = fchain_precomp(m, n, npre);
	   return (n_mulmod_precomp(n - 3UL, V.x, n, npre) == n_mulmod_precomp(2UL, V.y, n, npre));
   } else
   {
      mp_limb_t ninv = n_preinvert_limb(n);
	
      V = fchain2_preinv(m, n, ninv);
	   return (n_mulmod2_preinv(n - 3UL, V.x, n, ninv) == n_mulmod2_preinv(2UL, V.y, n, ninv));
  }
}

n_pair_t lchain_precomp(mp_limb_t m, mp_limb_t a, mp_limb_t n, double npre)
{
	n_pair_t current, old;
	int length, i;
	mp_limb_t power, xy, xx, yy;
	
	old.x = 2UL;
	old.y = a;
	
	length = BIT_COUNT(m);
	power = (1UL<<(length - 1));

	for (i = 0; i < length; i++)
	{
		xy = n_submod(n_mulmod_precomp(old.x, old.y, n, npre), a, n);
		
      if (m & power)
		{
			yy = n_submod(n_mulmod_precomp(old.y, old.y, n, npre), 2UL, n);
			current.x = xy;
			current.y = yy;
		} else 
		{
			xx = n_submod(n_mulmod_precomp(old.x, old.x, n, npre), 2UL, n);
			current.x = xx;
			current.y = xy;
		}

		power >>= 1;
		old = current;
	}

	return current;
}

n_pair_t lchain2_preinv(mp_limb_t m, mp_limb_t a, mp_limb_t n, mp_limb_t ninv)
{
	n_pair_t current, old;
	int length, i;
	mp_limb_t power, xy, xx, yy;
	
	old.x = 2UL;
	old.y = a;
	
	length = BIT_COUNT(m);
	power = (1UL<<(length - 1));

	for (i = 0; i < length; i++)
	{
		xy = n_submod(n_mulmod2_preinv(old.x, old.y, n, ninv), a, n);
		
      if (m & power)
		{
			yy = n_submod(n_mulmod2_preinv(old.y, old.y, n, ninv), 2UL, n);
			current.x = xy;
			current.y = yy;
		} else 
		{
			xx = n_submod(n_mulmod2_preinv(old.x, old.x, n, ninv), 2UL, n);
			current.x = xx;
			current.y = xy;
		}

		power >>= 1;
		old = current;
	}

	return current;
}

int n_is_pseudoprime_lucas(mp_limb_t n)
{
	int i, D, Q;
	mp_limb_t A;
	mp_limb_t left, right;
	n_pair_t V;

	D = 0;
	Q = 0;
	
	if (((n % 2) == 0) || (ABS((mp_limb_signed_t) n) <= 2))
	{
		if (n == 2UL) return 1;
		else return 0;
	}
	
	for (i = 0; i < 100; i++)
	{
		D = 5 + 2*i;
		if (n_gcd(D, n%D) != 1UL) 
      { 
         if (n == D) continue;
         else return 0;
      }
		if (i % 2 == 1) D = -D;
		if (n_jacobi(D, n) == -1) break;
	}

	if (i == 100)
	{
		if (n_is_square(n)) return -1;
		else return 1;
	}
   
   Q = (1 - D)/4;
   if (Q < 0)
   {
      if (n < 52UL)
      {
         while (Q < 0) Q += n;
         A = n_submod(n_invmod(Q, n), 2UL, n);
      } else
         A = n_submod(n_invmod(Q + n, n), 2UL, n);
   } else
   {
      if (n < 52UL)
      {
         while (Q >= n) Q -= n;
         A = n_submod(n_invmod(Q, n), 2UL, n);
      } else
         A = n_submod(n_invmod(Q, n), 2UL, n);
   }

   if (BIT_COUNT(n) <= D_BITS)
   {
      double npre = n_precompute_inverse(n);
	   V = lchain_precomp(n + 1, A, n, npre);
	
	   left = n_mulmod_precomp(A, V.x, n, npre);
	   right = n_mulmod_precomp(2, V.y, n, npre);
   } else
   {
      mp_limb_t ninv = n_preinvert_limb(n);
	   V = lchain2_preinv(n + 1, A, n, ninv);
	
	   left = n_mulmod_precomp(A, V.x, n, ninv);
	   right = n_mulmod_precomp(2, V.y, n, ninv);
   }
	
	return (left == right);
}

int is_likely_prime_BPSW(mp_limb_t n)
{
	if (n <= 1UL) return 0;

   if ((n & 1UL) == 0UL)
   {
      if (n == 2UL) return 1;
      return 0;
   }
   
   if (((n % 10) == 3) || ((n % 10) == 7))
	{
		if (n_is_pseudoprime_fermat(n, 2) == 0) return 0;
      
		return n_is_pseudoprime_fibonacci(n);	
	} else
	{
		mp_limb_t d;

		d = n - 1UL;
		while ((d & 1UL) == 0UL) d >>= 1;

		if (BIT_COUNT(n) <= D_BITS)
      {
         double npre = n_precompute_inverse(n);
         if (n_is_strong_pseudoprime_precomp(n, npre, 2L, d) == 0) return 0;
      } else
      {
         mp_limb_t ninv = n_preinvert_limb(n);
         if (n_is_strong_pseudoprime2_preinv(n, ninv, 2L, d) == 0) return 0;
      }

		return (n_is_pseudoprime_lucas(n) == 1);
	}
}

#endif // GMP_LIMB_BITS

// could have another parameter to specify what likely means
// ie for factoring , for RSA 
// or to state that we have already done trial div

// could call it mpz_likely_composite_p then when true we could return more info about it , ie a factor
int
mpz_likely_prime_p (mpz_srcptr N, gmp_randstate_t STATE, unsigned long td)
{
  int d, t, r;
  unsigned long tdlim;
  mpz_t base, nm1, x, e, n;

  ALLOC (n) = ALLOC (N);
  SIZ (n) = ABSIZ (N);
  PTR (n) = PTR (N);		// fake up an absolute value that we dont have de-allocate
// algorithm does not handle small values , get rid of them here
  if (mpz_cmp_ui (n, 2) == 0 || mpz_cmp_ui (n, 3) == 0)
    return 1;
  if (mpz_cmp_ui (n, 5) < 0 || mpz_even_p (n))
    return 0;

#if GMP_LIMB_BITS == 64 || GMP_LIMB_BITS == 32
  if (SIZ(n) == 1)
  {
     return is_likely_prime_BPSW(PTR(n)[0]);
  }
#endif

// for factoring purpoises 
// we assume we know nothing about N ie it is a random integer
// therefore it has a good chance of factoring by small divisiors , so try trial division as its fast and it checks small divisors
// checking for other divisors is not worth it even if the test is fast as we have random integer so only small divisors are common
// enough , remember this is not exact so it doesn't matter if we miss a few divisors
  tdlim=mpz_sizeinbase(n,2);
  tdlim=MAX(1000,tdlim);
  d=mpz_trial_division(n,3,tdlim);
  if(d!=0)
    {if(mpz_cmp_ui(n, d) == 0)return 1;
     return 0;}
  if (mpz_cmp_ui (n, tdlim * tdlim) < 0)
    return 1;	// if tdlim*tdlim overflows then n is not a single limb so cant be true anyway
  ASSERT (mpz_odd_p (n));
  ASSERT (mpz_cmp_ui (n, 5) >= 0);	// so we can choose a base
// now do strong pseudoprime test 
// get random base , for now choose any size , later choose a small one
  mpz_init (base);
  mpz_init_set (nm1, n);
  mpz_sub_ui (nm1, nm1, 1);
  do
    {
      mpz_urandomm (base, STATE, nm1);
    }
  while (mpz_cmp_ui (base, 1) <= 0);
// so base is 2 to n-2  which implys n>=4 , only really want a small base , and ignore the rare base=n-1 condition etc
//printf("base is ");mpz_out_str(stdout,10,base);printf(" ");
  mpz_init (e);
  mpz_init (x);
  t = mpz_scan1 (nm1, 0);	// so 2^t divides nm1
  ASSERT (t > 0);
  mpz_tdiv_q_2exp (e, nm1, t);	// so e=nm1/2^t
  mpz_powm (x, base, e, n);	// x=base^e mod n
  mpz_clear (e);
  mpz_clear (base);
  if (mpz_cmp_ui (x, 1) == 0)
    {
      mpz_clear (nm1);
      mpz_clear (x);
      return 1;
    }
  if (mpz_cmp (x, nm1) == 0)
    {
      mpz_clear (nm1);
      mpz_clear (x);
      return 1;
    }
  for (r = 0, t = t - 1; t > 0; t--)
    {
      mpz_mul (x, x, x);
      mpz_mod (x, x, n);
      if (mpz_cmp (x, nm1) == 0)
	{
	  r = 1;
	  break;
	}
      if (mpz_cmp_ui (x, 1) == 0)
	break;
    }
  mpz_clear (nm1);
  mpz_clear (x);
  return r;
}
