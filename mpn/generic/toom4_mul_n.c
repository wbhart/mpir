/* mpn_toom4_mul_n -- Internal routine to multiply two natural numbers
   of length n.

   THIS IS AN INTERNAL FUNCTION WITH A MUTABLE INTERFACE.  IT IS ONLY
   SAFE TO REACH THIS FUNCTION THROUGH DOCUMENTED INTERFACES.
*/

/* Implementation of the Bodrato-Zanoni algorithm for Toom-Cook 4-way.

Copyright 2001, 2002, 2004, 2005, 2006 Free Software Foundation, Inc.
Copyright 2009 William Hart

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
along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA. */

/*
   This implementation is based on that of Paul Zimmmermann, which is available
	for mpz_t's at http://www.loria.fr/~zimmerma/software/toom4.c
*/

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"

#define TC4_LIB 1 // library build only

#if !TC4_LIB
#include <stdio.h>
#include <stdlib.h>
#define TC4_TEST 0 // test code
#define TC4_TIME 1 // timing code
#else
#define TC4_TEST 0 
#define TC4_TIME 0 
#endif

void
mpn_toom4_mul_n (mp_ptr rp, mp_srcptr up,
		          mp_srcptr vp, mp_size_t n);

void 
toom4_interpolate(mp_ptr rp, mp_size_t * rpn, mp_size_t sn,  
		       mp_ptr tp, mp_size_t s4, mp_size_t n4, mp_size_t n6, mp_limb_t r30);

void _tc4_add(mp_ptr rp, mp_size_t * rn, mp_srcptr r1, mp_size_t r1n, mp_srcptr r2, mp_size_t r2n)
{
   mp_limb_t cy;
   mp_size_t s1 = ABS(r1n);
   mp_size_t s2 = ABS(r2n);
   
   if (!s1)
   {
      *rn = 0;
   } else if (!s2)
   {
      if (rp != r1) MPN_COPY(rp, r1, s1);
		*rn = r1n;
   } else if ((r1n ^ r2n) >= 0)
   {
      *rn = r1n;
      cy = mpn_add(rp, r1, s1, r2, s2);
      if (cy) 
      {
         rp[s1] = cy;
         if ((*rn) < 0) (*rn)--;
         else (*rn)++;
      }
   } else
   {
      mp_size_t ct;
		if (s1 != s2) ct = 1;
		else MPN_CMP(ct, r1, r2, s1); 
		    
      if (!ct) *rn = 0;
      else if (ct > 0) 
      {
         mpn_sub(rp, r1, s1, r2, s2);
         *rn = s1;
         MPN_NORMALIZE(rp, (*rn));
			if (r1n < 0) *rn = -(*rn);
      }
      else
      {
         mpn_sub_n(rp, r2, r1, s1);
         *rn = s1;
         MPN_NORMALIZE(rp, (*rn));
			if (r1n > 0) *rn = -(*rn);
      }
   }
}

void tc4_add(mp_ptr rp, mp_size_t * rn, mp_srcptr r1, mp_size_t r1n, mp_srcptr r2, mp_size_t r2n)
{
   mp_size_t s1 = ABS(r1n);
   mp_size_t s2 = ABS(r2n);
   
   if (s1 < s2) _tc4_add(rp, rn, r2, r2n, r1, r1n);
	else _tc4_add(rp, rn, r1, r1n, r2, r2n);
} 

#if HAVE_NATIVE_mpn_sumdiff_n
void tc4_sumdiff(mp_ptr rp, mp_size_t * rn, mp_ptr sp, mp_size_t * sn, mp_ptr r1, mp_size_t r1n, mp_ptr r2, mp_size_t r2n)
{
   mp_limb_t cy, cy2;
   mp_size_t s1 = ABS(r1n);
   mp_size_t s2 = ABS(r2n);
	int swapped = 0;
   
   if (s1 < s2) 
   {
      MPN_PTR_SWAP(r1, r1n, r2, r2n);
      MP_SIZE_T_SWAP(s1, s2);
		swapped = 1;
   } 
   
   if (!s1)
   {
      *rn = 0;
		*sn = 0;
   } else if (!s2)
   {
      if (rp != r1) MPN_COPY(rp, r1, s1);
		if (sp != r1) MPN_COPY(sp, r1, s1);
		*rn = r1n;
		*sn = (swapped ? -r1n : r1n);
   } else
   {
      mp_size_t ct;
		if (s1 != s2) ct = 1;
		else MPN_CMP(ct, r1, r2, s1); 
		    
      if (!ct) 
		{
			if ((r1n ^ r2n) >= 0)
			{
				*sn = 0;
				*rn = r1n;
            cy = mpn_lshift(rp, r1, s1, 1);
            if (cy) 
            {
               rp[s1] = cy;
               if ((*rn) < 0) (*rn)--;
               else (*rn)++;
				}
         } else
			{
				*rn = 0;
				*sn = (swapped ? -r1n : r1n);
            cy = mpn_lshift(sp, r1, s1, 1);
            if (cy) 
            {
               sp[s1] = cy;
               if ((*sn) < 0) (*sn)--;
               else (*sn)++;
				}
			}
		} else if (ct > 0) // r1 is bigger than r2
      {
         if ((r1n ^ r2n) >= 0) // both inputs are same sign
			{
				cy = mpn_sumdiff_n(rp, sp, r1, r2, s2);
				if (s1 > s2) // r1 has more limbs than r2
				{
               *rn = r1n;
               cy2 = mpn_add_1(rp + s2, r1 + s2, s1 - s2, cy>>1);
					if (cy2) 
               {
                  rp[s1] = cy2;
                  if ((*rn) < 0) (*rn)--;
                  else (*rn)++;
				   }
					mpn_sub_1(sp + s2, r1 + s2, s1 - s2, cy&1);
				} else // both inputs have same number of limbs
				{
               *rn = r1n;
               if (cy>>1) 
               {
                  rp[s1] = 1;
                  if ((*rn) < 0) (*rn)--;
                  else (*rn)++;
				   }
				}
				*sn = s1;
            MPN_NORMALIZE(sp, (*sn));
			   if (r1n ^ (~swapped) < 0) *sn = -(*sn);
			} else // inputs are different sign
			{
				cy = mpn_sumdiff_n(sp, rp, r1, r2, s2);
				if (s1 > s2) // r1 has more limbs than r2
				{
               *sn = r1n;
               cy2 = mpn_add_1(sp + s2, r1 + s2, s1 - s2, cy>>1);
					if (cy2) 
               {
                  sp[s1] = cy2;
                  if ((*sn) < 0) (*sn)--;
                  else (*sn)++;
				   }
					mpn_sub_1(rp + s2, r1 + s2, s1 - s2, cy&1);
				} else // both inputs have same number of limbs
				{
               *sn = r1n;
               if (cy>>1) 
               {
                  sp[s1] = 1;
                  if ((*sn) < 0) (*sn)--;
                  else (*sn)++;
				   }
				}
				*rn = s1;
            MPN_NORMALIZE(rp, (*rn));
			   if (r1n ^ (~swapped) < 0) *rn = -(*rn);
			}
      } else // r2 is bigger than r1 (but same number of limbs)
      {
         if ((r1n ^ r2n) >= 0) // both inputs are same sign
			{
				cy = mpn_sumdiff_n(rp, sp, r2, r1, s1);
				*rn = r1n;
            if (cy>>1) 
            {
               rp[s1] = 1;
               if ((*rn) < 0) (*rn)--;
               else (*rn)++;
				}
				*sn = s1;
            MPN_NORMALIZE(sp, (*sn));
			   if (r1n ^ (~swapped) > 0) *sn = -(*sn);
			} else // inputs are different sign
			{
				cy = mpn_sumdiff_n(sp, rp, r2, r1, s1);
				*sn = r1n;
            if (cy>>1) 
            {
               sp[s1] = 1;
               if ((*sn) < 0) (*sn)--;
               else (*sn)++;
				}
				*rn = s1;
            MPN_NORMALIZE(rp, (*rn));
			   if (r1n ^ (~swapped) > 0) *rn = -(*rn);
			}
      }
   }
}

void tc4_sumdiff_unsigned(mp_ptr rp, mp_size_t * rn, mp_ptr sp, mp_size_t * sn, mp_ptr r1, mp_size_t r1n, mp_ptr r2, mp_size_t r2n)
{
   mp_limb_t cy, cy2;
   mp_size_t s1 = ABS(r1n);
   mp_size_t s2 = ABS(r2n);
	int swapped = 0;
   
   if (s1 < s2) 
   {
      MPN_PTR_SWAP(r1, r1n, r2, r2n);
      MP_SIZE_T_SWAP(s1, s2);
		swapped = 1;
   } 
   
   if (!s1)
   {
      *rn = 0;
		*sn = 0;
   } else if (!s2)
   {
      if (rp != r1) MPN_COPY(rp, r1, s1);
		if (sp != r1) MPN_COPY(sp, r1, s1);
		*rn = r1n;
		*sn = (swapped ? -r1n : r1n);
   } else
   {
      mp_size_t ct;
		if (s1 != s2) ct = 1;
		else MPN_CMP(ct, r1, r2, s1); 
		    
      if (!ct) 
		{
			*sn = 0;
		   *rn = r1n;
         cy = mpn_lshift(rp, r1, s1, 1);
         if (cy) 
         {
            rp[s1] = cy;
            (*rn)++;
		   }
		} else if (ct > 0) // r1 is bigger than r2
      {
         cy = mpn_sumdiff_n(rp, sp, r1, r2, s2);
			if (s1 > s2) // r1 has more limbs than r2
			{
            *rn = r1n;
            cy2 = mpn_add_1(rp + s2, r1 + s2, s1 - s2, cy>>1);
				if (cy2) 
            {
               rp[s1] = cy2;
               (*rn)++;
				}
			   mpn_sub_1(sp + s2, r1 + s2, s1 - s2, cy&1);
			} else // both inputs have same number of limbs
			{
            *rn = r1n;
            if (cy>>1) 
            {
               rp[s1] = 1;
               (*rn)++;
				}
		   }
			*sn = s1;
         MPN_NORMALIZE(sp, (*sn));
			if (swapped) *sn = -(*sn);
      } else // r2 is bigger than r1 (but same number of limbs)
      {
         cy = mpn_sumdiff_n(rp, sp, r2, r1, s1);
			*rn = r1n;
         if (cy>>1) 
         {
            rp[s1] = 1;
            (*rn)++;
			}
			*sn = s1;
         MPN_NORMALIZE(sp, (*sn));
			if (!swapped) *sn = -(*sn);
      }
   }
}
#endif

/* ~~~~~~~~~~~~~~~~~~~~UNTESTED CODE~~~~~~~~~~~~~~~~~~~~~~~~~~

#define MPN_ADDSUB_CMP(cyxx, r1xx, r2xx, r3xx, snxx) \
	do { \
	   mp_limb_t t[2]; \
		add_ssaaaa(t[1], t[0], CNST_LIMB(0), r1xx[snxx - 1], CNST_LIMB(0), r2xx[snxx - 1]); \
      if (t[1]) cyxx = 1; \
	   else if (t[0] > r3xx[snxx - 1]) cyxx = 1; \
		else if (t[0] < r3xx[snxx - 1] - CNST_LIMB(1)) cyxx = -1; \
		else cyxx = 0; \
	} while (0)

void tc4_addadd(mp_ptr rp, mp_size_t * rn, mp_ptr r1, mp_size_t r1n, mp_ptr r2, mp_size_t r2n, mp_ptr r3, mp_size_t r3n)
{
	mp_size_t s1 = ABS(r1n);
	mp_size_t s2 = ABS(r2n);
	mp_size_t s3 = ABS(r3n);

	if ((s1 != s2) || (s1 != s3))
	{
			tc4_add(rp, rn, r1, r1n, r2, r2n);
		   tc4_add(rp, rn, rp, *rn, r3, r3n);
	} else
	{
      mp_limb_t cy;
		mp_size_t cy2;
		if (((r1n ^ r2n) >= 0) && ((r1n ^ r3n) >= 0)) // all same sign addadd
		{
         cy = mpn_addadd_n(rp, r1, r2, r3, s1);
			*rn = r1n;
			if (cy) 
         {
            rp[s1] = cy;
            if ((*rn) < 0) (*rn)--;
            else (*rn)++;
         }
		} else if (((r1n ^ r2n) >= 0) && ((r1n ^ r3n) < 0)) // addsub
		{
			MPN_ADDSUB_CMP(cy2, r1, r2, r3, s1);
			
			if (cy2 > 0)
			{
				cy = mpn_addsub_n(rp, r1, r2, r3, s1);
			   *rn = r1n;
			   if (cy) 
			   {
				   rp[s1] = cy;
               if ((*rn) < 0) (*rn)--;
               else (*rn)++;
			   }
			} else if (cy2 < 0)
			{
				cy = mpn_subadd_n(rp, r3, r1, r2, s1);
				if (cy) abort();
				*rn = s1;
			   MPN_NORMALIZE(rp, (*rn));
				if (r1n < 0) *rn = -(*rn);
			} else
			{
		      tc4_add(rp, rn, r1, r1n, r2, r2n);
		      tc4_add(rp, rn, rp, *rn, r3, r3n);
			}
		} else if (((r1n ^ r2n) < 0) && ((r1n ^ r3n) >= 0)) // subadd
		{
			MPN_ADDSUB_CMP(cy2, r1, r3, r2, s1);
			
			if (cy2 > 0)
			{
				cy = mpn_addsub_n(rp, r1, r3, r2, s1);
			   *rn = r1n;
			   if (cy) 
			   {
				   rp[s1] = cy;
               if ((*rn) < 0) (*rn)--;
               else (*rn)++;
			   }
			} else if (cy2 < 0)
			{
				mpn_subadd_n(rp, r2, r1, r3, s1);
			   *rn = s1;
			   MPN_NORMALIZE(rp, (*rn));
				if (r1n > 0) *rn = -(*rn);
			} else
			{
		      tc4_add(rp, rn, r1, r1n, r2, r2n);
		      tc4_add(rp, rn, rp, *rn, r3, r3n);
			}
		} else // add final two and subtract first  
		{
			MPN_ADDSUB_CMP(cy2, r2, r3, r1, s1);
			
			if (cy2 > 0)
			{
				cy = mpn_addsub_n(rp, r2, r3, r1, s1);
			   *rn = r1n;
			   if (cy) 
			   {
				   rp[s1] = cy;
               if ((*rn) < 0) (*rn)--;
               else (*rn)++;
			   }
			} else if (cy2 < 0)
			{
				mpn_subadd_n(rp, r1, r2, r3, s1);
			   *rn = s1;
			   MPN_NORMALIZE(rp, (*rn));
				if (r1n < 0) *rn = -(*rn);
   		} else
			{
		      tc4_add(rp, rn, r1, r1n, r2, r2n);
		      tc4_add(rp, rn, rp, *rn, r3, r3n);
			}
		}
	}
}

void tc4_addsub(mp_ptr rp, mp_size_t * rn, mp_ptr r1, mp_size_t r1n, mp_ptr r2, mp_size_t r2n, mp_ptr r3, mp_size_t r3n)
{
	tc4_addadd(rp, rn, r1, r1n, r2, r2n, r3, -r3n);
}

void tc4_subsub(mp_ptr rp, mp_size_t * rn, mp_ptr r1, mp_size_t r1n, mp_ptr r2, mp_size_t r2n, mp_ptr r3, mp_size_t r3n)
{
	tc4_addadd(rp, rn, r1, r1n, r2, -r2n, r3, -r3n);
}

*/

void _tc4_add_unsigned(mp_ptr rp, mp_size_t * rn, mp_srcptr r1, mp_size_t r1n, mp_srcptr r2, mp_size_t r2n)
{
   mp_limb_t cy;
   mp_size_t s1 = r1n;
   mp_size_t s2 = r2n;
   
   if (!s2)
   {
      if (!s1) *rn = 0;
      else
      {
         if (rp != r1) MPN_COPY(rp, r1, s1);
		   *rn = r1n;
		}
   } else
   {
      *rn = r1n;
      cy = mpn_add(rp, r1, s1, r2, s2);
      if (cy) 
      {
         rp[s1] = cy;
         if ((*rn) < 0) (*rn)--;
         else (*rn)++;
      }
   } 
}

void tc4_add_unsigned(mp_ptr rp, mp_size_t * rn, mp_srcptr r1, mp_size_t r1n, mp_srcptr r2, mp_size_t r2n)
{
   if (r1n < r2n) _tc4_add_unsigned(rp, rn, r2, r2n, r1, r1n);
	else _tc4_add_unsigned(rp, rn, r1, r1n, r2, r2n);
} 

void tc4_sub(mp_ptr rp, mp_size_t * rn, mp_ptr r1, mp_size_t r1n, mp_ptr r2, mp_size_t r2n)
{
   tc4_add(rp, rn, r1, r1n, r2, -r2n);
}
	
void tc4_lshift(mp_ptr rp, mp_size_t * rn, mp_srcptr xp, mp_size_t xn, mp_size_t bits)
{
   if (xn == 0) *rn = 0;
   else
	{
		mp_size_t xu = ABS(xn);
		mp_limb_t msl = mpn_lshift(rp, xp, xu, bits);
      if (msl) 
		{
			rp[xu] = msl;
			*rn = (xn >= 0 ? xn + 1 : xn - 1);
		} else
		   *rn = xn;
	}
}

void tc4_rshift_inplace(mp_ptr rp, mp_size_t * rn, mp_size_t bits)
{
   if (*rn)
	{
		if ((*rn) > 0) 
		{
			mpn_rshift(rp, rp, *rn, bits);
	      if (rp[(*rn) - 1] == CNST_LIMB(0)) (*rn)--;
		} else 
		{
			mpn_rshift(rp, rp, -(*rn), bits);
	      if (rp[-(*rn) - 1] == CNST_LIMB(0)) (*rn)++;
		}
	}
}

#if HAVE_NATIVE_mpn_addlsh1_n
void tc4_addlsh1_unsigned(mp_ptr rp, mp_size_t * rn, mp_srcptr xp, mp_size_t xn)
{
	if (xn)
	{
		if (xn >= *rn)
		{
            mp_limb_t cy;
			if (xn > *rn) MPN_ZERO(rp + *rn, xn - *rn);
			cy = mpn_addlsh1_n(rp, rp, xp, xn);
			if (cy) 
			{
				rp[xn] = cy;
				*rn = xn + 1;
			} else *rn = xn;
		} else
	   {
		   mp_limb_t cy = mpn_addlsh1_n(rp, rp, xp, xn);
	      if (cy) cy = mpn_add_1(rp + xn, rp + xn, *rn - xn, cy);
		   if (cy) 
		   {
			   rp[*rn] = cy;
			   (*rn)++;
		   }
		}
	}
}
#endif
/*
void tc4_divexact_ui(mp_ptr rp, mp_size_t * rn, mp_srcptr x, mp_size_t xn, mp_limb_t c)
{
	if (xn)
	{
		mp_size_t xu = ABS(xn);
		mp_limb_t cy = mpn_divmod_1(rp, x, xu, c);
		if (xn > 0)
		{
			if (rp[xu - 1] == 0) *rn = xn - 1;
			else *rn = xn;
		} else
		{
			if (rp[xu - 1] == 0) *rn = xn + 1;
			else *rn = xn;
		}	
	} else *rn = 0;
}
*/

void tc4_divexact_ui(mp_ptr rp, mp_size_t * rn, mp_ptr x, mp_size_t xn, mp_limb_t c)
{
  mp_size_t abs_size;
  if (xn == 0)
    {
      *rn = 0;
      return;
    }
  abs_size = ABS (xn);

  MPN_DIVREM_OR_DIVEXACT_1 (rp, x, abs_size, c);
  abs_size -= (rp[abs_size-1] == 0);
  *rn = (xn >= 0 ? abs_size : -abs_size);
}

void tc4_divexact_by3(mp_ptr rp, mp_size_t * rn, mp_ptr x, mp_size_t xn)
{
	if (xn)
	{
		mp_size_t xu = ABS(xn);
		mpn_divexact_by3(rp, x, xu);
		if (xn > 0)
		{
			if (rp[xu - 1] == CNST_LIMB(0)) *rn = xn - 1;
			else *rn = xn;
		} else
		{
			if (rp[xu - 1] == CNST_LIMB(0)) *rn = xn + 1;
			else *rn = xn;
		}	
	} else *rn = 0;
}

#if HAVE_NATIVE_mpn_divexact_byBm1of
void tc4_divexact_by15(mp_ptr rp, mp_size_t * rn, mp_ptr x, mp_size_t xn)
{
	if (xn)
	{
		mp_size_t xu = ABS(xn);
		mpn_divexact_byBm1of(rp, x, xu, CNST_LIMB(15), CNST_LIMB((~0)/15)); // works for 32 and 64 bits
		if (xn > 0)
		{
			if (rp[xu - 1] == CNST_LIMB(0)) *rn = xn - 1;
			else *rn = xn;
		} else
		{
			if (rp[xu - 1] == CNST_LIMB(0)) *rn = xn + 1;
			else *rn = xn;
		}	
	} else *rn = 0;
}
#endif

#if HAVE_NATIVE_mpn_mul_1c
#define MPN_MUL_1C(cout, dst, src, size, n, cin)        \
  do {                                                  \
    (cout) = mpn_mul_1c (dst, src, size, n, cin);       \
  } while (0)
#else
#define MPN_MUL_1C(cout, dst, src, size, n, cin)        \
  do {                                                  \
    mp_limb_t __cy;                                     \
    __cy = mpn_mul_1 (dst, src, size, n);               \
    (cout) = __cy + mpn_add_1 (dst, dst, size, cin);    \
  } while (0)
#endif

void tc4_addmul_1(mp_ptr wp, mp_size_t * wn, mp_srcptr xp, mp_size_t xn, mp_limb_t y)
{
  mp_size_t  sign, wu, xu, ws, new_wn, min_size, dsize;
  mp_limb_t  cy;

  /* w unaffected if x==0 or y==0 */
  if (xn == 0 || y == 0)
    return;

  sign = xn;
  xu = ABS (xn);

  ws = *wn;
  if (*wn == 0)
  {
      /* nothing to add to, just set x*y, "sign" gives the sign */
      cy = mpn_mul_1 (wp, xp, xu, y);
      if (cy)
		{
			wp[xu] = cy;
         xu = xu + 1;
		} 
      *wn = (sign >= 0 ? xu : -xu);
      return;
  }
  
  sign ^= *wn;
  wu = ABS (*wn);

  new_wn = MAX (wu, xu);
  min_size = MIN (wu, xu);

  if (sign >= 0)
  {
      /* addmul of absolute values */

      cy = mpn_addmul_1 (wp, xp, min_size, y);
      
      dsize = xu - wu;
#if HAVE_NATIVE_mpn_mul_1c
      if (dsize > 0)
        cy = mpn_mul_1c (wp + min_size, xp + min_size, dsize, y, cy);
      else if (dsize < 0)
      {
          dsize = -dsize;
          cy = mpn_add_1 (wp + min_size, wp + min_size, dsize, cy);
      }
#else
      if (dsize != 0)
      {
          mp_limb_t cy2;
          if (dsize > 0)
            cy2 = mpn_mul_1 (wp + min_size, xp + min_size, dsize, y);
          else
          {
              dsize = -dsize;
              cy2 = 0;
          }
          cy = cy2 + mpn_add_1 (wp + min_size, wp + min_size, dsize, cy);
      }
#endif

      if (cy)
		{
			wp[dsize + min_size] = cy;
         new_wn ++;
		}
   } else
   {
      /* submul of absolute values */

      cy = mpn_submul_1 (wp, xp, min_size, y);
      if (wu >= xu)
      {
          /* if w bigger than x, then propagate borrow through it */
          if (wu != xu)
            cy = mpn_sub_1 (wp + xu, wp + xu, wu - xu, cy);

          if (cy != 0)
          {
              /* Borrow out of w, take twos complement negative to get
                 absolute value, flip sign of w.  */
              wp[new_wn] = ~-cy;  /* extra limb is 0-cy */
              mpn_com_n (wp, wp, new_wn);
              new_wn++;
              MPN_INCR_U (wp, new_wn, CNST_LIMB(1));
              ws = -*wn;
          }
      } else /* wu < xu */
      {
          /* x bigger than w, so want x*y-w.  Submul has given w-x*y, so
             take twos complement and use an mpn_mul_1 for the rest.  */

          mp_limb_t  cy2;

          /* -(-cy*b^n + w-x*y) = (cy-1)*b^n + ~(w-x*y) + 1 */
          mpn_com_n (wp, wp, wu);
          cy += mpn_add_1 (wp, wp, wu, CNST_LIMB(1));
          cy -= 1;

          /* If cy-1 == -1 then hold that -1 for latter.  mpn_submul_1 never
             returns cy==MP_LIMB_T_MAX so that value always indicates a -1. */
          cy2 = (cy == MP_LIMB_T_MAX);
          cy += cy2;
          MPN_MUL_1C (cy, wp + wu, xp + wu, xu - wu, y, cy);
          wp[new_wn] = cy;
          new_wn += (cy != 0);

          /* Apply any -1 from above.  The value at wp+wsize is non-zero
             because y!=0 and the high limb of x will be non-zero.  */
          if (cy2)
            MPN_DECR_U (wp+wu, new_wn - wu, CNST_LIMB(1));

          ws = -*wn;
        }

      /* submul can produce high zero limbs due to cancellation, both when w
         has more limbs or x has more  */
      MPN_NORMALIZE (wp, new_wn);
  }

  *wn = (ws >= 0 ? new_wn : -new_wn);

  ASSERT (new_wn == 0 || wp[new_wn - 1] != 0);
}

void tc4_submul_1(mp_ptr wp, mp_size_t * wn, mp_srcptr x, mp_size_t xn, mp_limb_t y)
{
	tc4_addmul_1(wp, wn, x, -xn, y);
}

void tc4_copy (mp_ptr yp, mp_size_t * yn, mp_size_t offset, mp_srcptr xp, mp_size_t xn)
{
  mp_size_t yu = ABS(*yn);
  mp_size_t xu = ABS(xn);
  mp_limb_t cy = 0;

  if (xn == 0)
    return;

  if (offset < yu) /* low part of x overlaps with y */
  {
      if (offset + xu <= yu) /* x entirely inside y */
      {
          cy = mpn_add_n (yp + offset, yp + offset, xp, xu);
          if (offset + xu < yu)
            cy = mpn_add_1 (yp + offset + xu, yp + offset + xu,
                            yu - (offset + xu), cy);
      } else
        cy = mpn_add_n (yp + offset, yp + offset, xp, yu - offset);
      /* now cy is the carry at yp + yu */
      if (xu + offset > yu) /* high part of x exceeds y */
      {
          MPN_COPY (yp + yu, xp + yu - offset, xu + offset - yu);
          cy = mpn_add_1 (yp + yu, yp + yu, xu + offset - yu, cy);
          yu = xu + offset;
      }
      /* now cy is the carry at yp + yn */
      if (cy)
        yp[yu++] = cy;
      MPN_NORMALIZE(yp, yu);
      *yn = yu;
  } else /* x does not overlap */
  {
      if (offset > yu)
        MPN_ZERO (yp + yu, offset - yu);
      MPN_COPY (yp + offset, xp, xu);
      *yn = offset + xu;
  }
}

#define MUL_TC4_UNSIGNED(r3xx, n3xx, r1xx, n1xx, r2xx, n2xx) \
   do \
   { \
      if ((n1xx != 0) && (n2xx != 0)) \
      { mp_size_t len; \
	      if (n1xx == n2xx) \
		   { \
			   if (n1xx > MUL_TOOM4_THRESHOLD) mpn_toom4_mul_n(r3xx, r1xx, r2xx, n1xx); \
            else mpn_mul_n(r3xx, r1xx, r2xx, n1xx); \
		   } else if (n1xx > n2xx) \
		      mpn_mul(r3xx, r1xx, n1xx, r2xx, n2xx); \
		   else \
		      mpn_mul(r3xx, r2xx, n2xx, r1xx, n1xx); \
	      len = n1xx + n2xx; \
		   MPN_NORMALIZE(r3xx, len); \
		   n3xx = len; \
      } else \
         n3xx = 0; \
   } while (0)

#define MUL_TC4(r3xx, n3xx, r1xx, n1xx, r2xx, n2xx) \
	do \
	{ \
	   mp_size_t sign = n1xx ^ n2xx; \
	   mp_size_t un1 = ABS(n1xx); \
	   mp_size_t un2 = ABS(n2xx); \
		MUL_TC4_UNSIGNED(r3xx, n3xx, r1xx, un1, r2xx, un2); \
		if (sign < 0) n3xx = -n3xx; \
	} while (0)

#define SQR_TC4_UNSIGNED(r3xx, n3xx, r1xx, n1xx) \
   do \
   { \
      if (n1xx != 0) \
      { mp_size_t len; \
	      if (n1xx > MUL_TOOM4_THRESHOLD) mpn_toom4_sqr_n(r3xx, r1xx, n1xx); \
         else mpn_sqr_n(r3xx, r1xx, n1xx); \
		   len = 2*n1xx; \
		   MPN_NORMALIZE(r3xx, len); \
		   n3xx = len; \
      } else \
         n3xx = 0; \
   } while (0)

#define SQR_TC4(r3xx, n3xx, r1xx, n1xx) \
	do \
	{ \
	   mp_size_t un1 = ABS(n1xx); \
	   SQR_TC4_UNSIGNED(r3xx, n3xx, r1xx, un1); \
	} while (0)

#define TC4_NORM(rxx, nxx, sxx) \
	do \
	{ \
	   nxx = sxx; \
	   MPN_NORMALIZE(rxx, nxx); \
	} while(0)

#if TC4_TEST || TC4_TIME
#define p2(axx, anxx, bxx, bnxx) \
	do \
	{ \
	   printf("s1 = "); \
      if (anxx < 0) printf("-"); \
      if (anxx == 0) printf("0, "); \
      else printf("%ld, ", axx[0]); \
      printf("s2 = "); \
      if (bnxx < 0) printf("-"); \
      if (bnxx == 0) printf("0"); \
      else printf("%ld\n", bxx[0]); \
   } while (0)

#define p(axx, anxx) \
	do \
	{ \
	   printf("r = "); \
      if (anxx < 0) printf("-"); \
      if (anxx == 0) printf("0\n"); \
      else printf("%ld\n", axx[0]); \
   } while (0)
#endif

/* Zero out limbs to end of integer */
#define TC4_DENORM(rxx, nxx, sxx) \
	do { \
	MPN_ZERO(rxx + ABS(nxx), sxx - ABS(nxx)); \
	} while (0)

/* Two's complement divexact by power of 2 */
#define TC4_DIVEXACT_2EXP(rxx, nxx, sxx) \
	do { \
	   mp_limb_t sign = (LIMB_HIGHBIT_TO_MASK(rxx[nxx-1]) << (GMP_LIMB_BITS - sxx)); \
      mpn_rshift(rxx, rxx, nxx, sxx); \
	   rxx[nxx-1] |= sign; \
	} while (0)

#if HAVE_NATIVE_mpn_rshift1
#define TC4_RSHIFT1(rxx, nxx) \
	do { \
	   mp_limb_t sign = (LIMB_HIGHBIT_TO_MASK(rxx[nxx-1]) << (GMP_LIMB_BITS - 1)); \
       mpn_rshift1(rxx, rxx, nxx); \
	   rxx[nxx-1] |= sign; \
	} while (0)
#else
#define TC4_RSHIFT1(rxx, nxx) \
	do { \
	   mp_limb_t sign = (LIMB_HIGHBIT_TO_MASK(rxx[nxx-1]) << (GMP_LIMB_BITS - 1)); \
       mpn_rshift(rxx, rxx, nxx, 1); \
	   rxx[nxx-1] |= sign; \
	} while (0)
#endif

#define r1 (tp)
#define r2 (tp + t4)
#define r4 (tp + 2*t4)
#define r6 (tp + 3*t4)

#define r3 (rp + 4*sn)
#define r5 (rp + 2*sn)
#define r7 (rp)

/* Multiply {up, n} by {vp, n} and write the result to
   {prodp, 2n}.

   Note that prodp gets 2n limbs stored, even if the actual result
   only needs 2n - 1.
*/

#define mpn_clearit(rxx, nxx) \
  do { \
     mp_size_t ind = 0; \
     for ( ; ind < nxx; ind++) \
        (rxx)[ind] = CNST_LIMB(0); \
  } while (0)

void
mpn_toom4_mul_n (mp_ptr rp, mp_srcptr up,
		          mp_srcptr vp, mp_size_t n)
{
  mp_size_t len1, len2, ind;
  mp_limb_t cy, r30, r31;
  mp_ptr tp;
  mp_size_t a0n, a1n, a2n, a3n, b0n, b1n, b2n, b3n, sn, n1, n2, n3, n4, n5, n6, n7, n8, rpn, t4;

  len1 = n;
  len2 = n;
  ASSERT (n >= 1);

  MPN_NORMALIZE(up, len1);
  MPN_NORMALIZE(vp, len2);

  
  sn = (n - 1) / 4 + 1;
  
#define a0 (up)
#define a1 (up + sn)
#define a2 (up + 2*sn)
#define a3 (up + 3*sn)
#define b0 (vp)
#define b1 (vp + sn)
#define b2 (vp + 2*sn)
#define b3 (vp + 3*sn)

   TC4_NORM(a0, a0n, sn);
	TC4_NORM(a1, a1n, sn);
	TC4_NORM(a2, a2n, sn);
	TC4_NORM(a3, a3n, n - 3*sn); 
   TC4_NORM(b0, b0n, sn);
	TC4_NORM(b1, b1n, sn);
	TC4_NORM(b2, b2n, sn);
	TC4_NORM(b3, b3n, n - 3*sn); 

   t4 = 2*sn+2; // allows mult of 2 integers of sn + 1 limbs

   tp = __GMP_ALLOCATE_FUNC_LIMBS(4*t4 + 5*(sn + 1));

#define u2 (tp + 4*t4)
#define u3 (tp + 4*t4 + (sn+1))
#define u4 (tp + 4*t4 + 2*(sn+1))
#define u5 (tp + 4*t4 + 3*(sn+1))
#define u6 (tp + 4*t4 + 4*(sn+1))

   tc4_add_unsigned(u6, &n6, a3, a3n, a1, a1n); 
   tc4_add_unsigned(u5, &n5, a2, a2n, a0, a0n); 
#if HAVE_NATIVE_mpn_sumdiff_n
	tc4_sumdiff_unsigned(u3, &n3, u4, &n4, u5, n5, u6, n6); 
#else
	tc4_add_unsigned(u3, &n3, u5, n5, u6, n6); 
   tc4_sub(u4, &n4, u5, n5, u6, n6);
#endif
	tc4_add_unsigned(u6, &n6, b3, b3n, b1, b1n);
   tc4_add_unsigned(u5, &n5, b2, b2n, b0, b0n);
#if HAVE_NATIVE_mpn_sumdiff_n
   tc4_sumdiff_unsigned(r2, &n8, u5, &n5, u5, n5, u6, n6); 
#else
   tc4_add_unsigned(r2, &n8, u5, n5, u6, n6); 
   tc4_sub(u5, &n5, u5, n5, u6, n6);
#endif

	MUL_TC4_UNSIGNED(r3, n3, u3, n3, r2, n8);
	MUL_TC4(r4, n4, u4, n4, u5, n5);
   
	tc4_lshift(r1, &n1, a0, a0n, 3);
#if HAVE_NATIVE_mpn_addlsh1_n
	tc4_addlsh1_unsigned(r1, &n1, a2, a2n);
#else
	tc4_addmul_1(r1, &n1, a2, a2n, 2);
#endif
 	tc4_lshift(r2, &n8, a1, a1n, 2);
   tc4_add(r2, &n8, r2, n8, a3, a3n);
   tc4_add(u5, &n5, r1, n1, r2, n8);
   tc4_sub(u6, &n6, r1, n1, r2, n8);
   tc4_lshift(r1, &n1, b0, b0n, 3);
#if HAVE_NATIVE_mpn_addlsh1_n
	tc4_addlsh1_unsigned(r1, &n1, b2, b2n);
#else
	tc4_addmul_1(r1, &n1, b2, b2n, 2);
#endif
   tc4_lshift(r2, &n8, b1, b1n, 2);
   tc4_add(r2, &n8, r2, n8, b3, b3n);
   tc4_add(u2, &n2, r1, n1, r2, n8);
   tc4_sub(r2, &n8, r1, n1, r2, n8);
   
	r30 = r3[0];
	if (!n3) r30 = CNST_LIMB(0);
   r31 = r3[1];
	MUL_TC4_UNSIGNED(r5, n5, u5, n5, u2, n2);
   MUL_TC4(r6, n6, u6, n6, r2, n8);
   r3[1] = r31;

   tc4_lshift(u2, &n2, a3, a3n, 3);
   tc4_addmul_1(u2, &n2, a2, a2n, 4);
#if HAVE_NATIVE_mpn_addlsh1_n
	tc4_addlsh1_unsigned(u2, &n2, a1, a1n);
#else
	tc4_addmul_1(u2, &n2, a1, a1n, 2);
#endif
	tc4_add(u2, &n2, u2, n2, a0, a0n);
   tc4_lshift(r1, &n1, b3, b3n, 3);
	tc4_addmul_1(r1, &n1, b2, b2n, 4);
#if HAVE_NATIVE_mpn_addlsh1_n
   tc4_addlsh1_unsigned(r1, &n1, b1, b1n);
#else
   tc4_addmul_1(r1, &n1, b1, b1n, 2);
#endif
	tc4_add(r1, &n1, r1, n1, b0, b0n);
   
	MUL_TC4_UNSIGNED(r2, n2, u2, n2, r1, n1);
   MUL_TC4_UNSIGNED(r1, n1, a3, a3n, b3, b3n);
   MUL_TC4_UNSIGNED(r7, n7, a0, a0n, b0, b0n);

	TC4_DENORM(r1, n1,  t4 - 1);
   TC4_DENORM(r2, n2,  t4 - 1);
   if (n3)
     TC4_DENORM(r3, n3,  t4 - 1); 
   else {
     /* MPN_ZERO defeats gcc 4.1.2 here, hence the explicit for loop */
     for (ind = 1 ; ind < t4 - 1; ind++) 
        (r3)[ind] = CNST_LIMB(0); 
   }
   TC4_DENORM(r4, n4,  t4 - 1);
   TC4_DENORM(r5, n5,  t4 - 1);
   TC4_DENORM(r6, n6,  t4 - 1);
   TC4_DENORM(r7, n7,  t4 - 2); // we treat r7 differently (it cannot exceed t4-2 in length)

/*	rp        rp1          rp2           rp3          rp4           rp5         rp6           rp7
<----------- r7-----------><------------r5-------------->            
                                                       <-------------r3------------->

              <-------------r6------------->                        < -----------r2------------>{           }
                                         <-------------r4-------------->         <--------------r1---->
*/

	toom4_interpolate(rp, &rpn, sn, tp, t4 - 1, n4, n6, r30);

	if (rpn != 2*n) 
	{
		MPN_ZERO((rp + rpn), 2*n - rpn);
	}

   __GMP_FREE_FUNC_LIMBS (tp, 4*t4 + 5*(sn+1));
}

/* Square {up, n} and write the result to {prodp, 2n}.

   Note that prodp gets 2n limbs stored, even if the actual result
   only needs 2n - 1.
*/

void
mpn_toom4_sqr_n (mp_ptr rp, mp_srcptr up, mp_size_t n)
{
  mp_size_t len1, ind;
  mp_limb_t cy, r30, r31;
  mp_ptr tp;
  mp_size_t a0n, a1n, a2n, a3n, sn, n1, n2, n3, n4, n5, n6, n7, n8, n9, rpn, t4;

  len1 = n;
  ASSERT (n >= 1);

  MPN_NORMALIZE(up, len1);
  
  sn = (n - 1) / 4 + 1;

  /* a0 - a3 are defined in mpn_toom4_mul_n above */
  
   TC4_NORM(a0, a0n, sn);
	TC4_NORM(a1, a1n, sn);
	TC4_NORM(a2, a2n, sn);
	TC4_NORM(a3, a3n, n - 3*sn); 

   t4 = 2*sn+2; // allows mult of 2 integers of sn + 1 limbs

   tp = __GMP_ALLOCATE_FUNC_LIMBS(4*t4 + 4*(sn + 1));

   tc4_add_unsigned(u5, &n5, a3, a3n, a1, a1n); 
   tc4_add_unsigned(u4, &n4, a2, a2n, a0, a0n); 
#if HAVE_NATIVE_mpn_sumdiff_n
	tc4_sumdiff_unsigned(u2, &n2, u3, &n3, u4, n4, u5, n5); 
#else
	tc4_add_unsigned(u2, &n2, u4, n4, u5, n5); 
   tc4_sub(u3, &n3, u4, n4, u5, n5);
#endif

	SQR_TC4(r4, n4, u3, n3);
   SQR_TC4_UNSIGNED(r3, n3, u2, n2);
	
	tc4_lshift(r1, &n1, a0, a0n, 3);
#if HAVE_NATIVE_mpn_addlsh1_n
	tc4_addlsh1_unsigned(r1, &n1, a2, a2n);
#else
	tc4_addmul_1(r1, &n1, a2, a2n, 2);
#endif
 	tc4_lshift(r2, &n8, a1, a1n, 2);
   tc4_add(r2, &n8, r2, n8, a3, a3n);
   tc4_add(u4, &n9, r1, n1, r2, n8);
   tc4_sub(u5, &n5, r1, n1, r2, n8);
   
	r30 = r3[0];
	if (!n3) r30 = CNST_LIMB(0);
   r31 = r3[1];
	SQR_TC4(r6, n6, u5, n5);
   SQR_TC4_UNSIGNED(r5, n5, u4, n9);
   r3[1] = r31;

   tc4_lshift(u2, &n8, a3, a3n, 3);
   tc4_addmul_1(u2, &n8, a2, a2n, 4);
#if HAVE_NATIVE_mpn_addlsh1_n
	tc4_addlsh1_unsigned(u2, &n8, a1, a1n);
#else
	tc4_addmul_1(u2, &n8, a1, a1n, 2);
#endif
	tc4_add(u2, &n8, u2, n8, a0, a0n);
   
	SQR_TC4_UNSIGNED(r2, n2, u2, n8);
   SQR_TC4_UNSIGNED(r1, n1, a3, a3n);
   SQR_TC4_UNSIGNED(r7, n7, a0, a0n);

	TC4_DENORM(r1, n1,  t4 - 1);
   TC4_DENORM(r2, n2,  t4 - 1);
   if (n3)
     TC4_DENORM(r3, n3,  t4 - 1);
   else {
     /* MPN_ZERO defeats gcc 4.1.2 here, hence the explicit for loop */
     for (ind = 1 ; ind < t4 - 1; ind++)
        (r3)[ind] = CNST_LIMB(0);
   }
   TC4_DENORM(r4, n4,  t4 - 1);
   TC4_DENORM(r5, n5,  t4 - 1);
   TC4_DENORM(r6, n6,  t4 - 1);
   TC4_DENORM(r7, n7,  t4 - 2); // we treat r7 differently (it cannot exceed t4-2 in length)

/*	rp        rp1          rp2           rp3          rp4           rp5         rp6           rp7
<----------- r7-----------><------------r5-------------->            
                                                       <-------------r3------------->

              <-------------r6------------->                        < -----------r2------------>{           }
                                         <-------------r4-------------->         <--------------r1---->
*/

	toom4_interpolate(rp, &rpn, sn, tp, t4 - 1, n4, n6, r30);

	if (rpn != 2*n) 
	{
		MPN_ZERO((rp + rpn), 2*n - rpn);
	}

   __GMP_FREE_FUNC_LIMBS (tp, 4*t4 + 4*(sn+1));
}

/*
   Toom 4 interpolation. Interpolates the value at 2^(sn*B) of a 
	polynomial p(x) with 7 coefficients given the values 
	p(oo), p(2), p(1), p(-1), 2^6*p(1/2), 2^6*p(-1/2), p(0).
	The output is placed in rp and the final number of limbs of the
	output is given in rpn.
	The 4th and 6th values may be negative, and if so, n4 and n6 
	should be set to a negative value respectively.
   To save space we pass r3, r5, r7 in place in the output rp.
	The other r's are stored separately in space tp.
	The low limb of r3 is stored in r30, as it will be overwritten
	by the high limb of r5.

rp          rp1          rp2           rp3          rp4           rp5         rp6           rp7
<----------- r7-----------><------------r5-------------->            
                                                      <-------------r3------------->

   We assume that r1 is stored at tp, r2 at (tp + t4), r4 at (tp + 2*t4) 
	and r6 (tp + 3*t4). Each of these r's has t4 = s4 + 1 limbs allocated.
*/
void toom4_interpolate(mp_ptr rp, mp_size_t * rpn, mp_size_t sn,  
		       mp_ptr tp, mp_size_t s4, mp_size_t n4, mp_size_t n6, mp_limb_t r30)
{
	mp_size_t n1, n2, n3, n5, n7, t4;
	mp_limb_t saved, saved2, cy;

   t4 = s4 + 1; 
   
	mpn_add_n(r2, r2, r5, s4);

   if (n6 < 0) 
		mpn_add_n(r6, r5, r6, s4);
	else
      mpn_sub_n(r6, r5, r6, s4);
	/* r6 is now in twos complement format */

	saved = r3[0];
	r3[0] = r30;
	if (n4 < 0) 
		mpn_add_n(r4, r3, r4, s4);
	else
      mpn_sub_n(r4, r3, r4, s4);
	r3[0] = saved;
	/* r4 is now in twos complement format */
	
	mpn_sub_n(r5, r5, r1, s4);

	r5[s4-1] -= mpn_submul_1(r5, r7, s4-1, 64);
   
   TC4_RSHIFT1(r4, s4); 
	
	saved = r3[0];
	r3[0] = r30;
	mpn_sub_n(r3, r3, r4, s4);
	r30 = r3[0];
	r3[0] = saved;

	mpn_lshift1(r5, r5, s4); 

	mpn_sub_n(r5, r5, r6, s4);

   saved = r3[0];
	r3[0] = r30;
	mpn_submul_1(r2, r3, s4, 65);
   r3[0] = saved;
	
	saved2 = r7[s4-1];
	r7[s4-1] = CNST_LIMB(0); // r7 is always positive so no sign extend needed
	saved = r3[0];
	r3[0] = r30;
#if HAVE_NATIVE_mpn_subadd_n
	mpn_subadd_n(r3, r3, r7, r1, s4);
#else
	mpn_sub_n(r3, r3, r7, s4);
	mpn_sub_n(r3, r3, r1, s4);
#endif
	r7[s4-1] = saved2;
   r30 = r3[0];
	
   mpn_addmul_1(r2, r3, s4, 45);

   cy = mpn_submul_1(r5, r3, s4 - 1, 8);
   r3[0] = saved;
	r3[0] -= (cy + 8*r3[s4-1]);
   
	TC4_DIVEXACT_2EXP(r5, s4, 3); 

	mpn_divexact_by3(r5, r5, s4); 
   
	mpn_sub_n(r6, r6, r2, s4);

	mpn_submul_1(r2, r4, s4, 16);
   
   TC4_RSHIFT1(r2, s4); 

	mpn_divexact_by3(r2, r2, s4); 

   mpn_divexact_by3(r2, r2, s4); 
   
   saved = r3[0];
	r3[0] = r30;
   cy = mpn_sub_n(r3, r3, r5, s4 - 1);
   r30 = r3[0];
	r3[0] = saved;
	r3[s4-1] -= (cy + r5[s4-1]);
   
	mpn_sub_n(r4, r4, r2, s4);
	
	mpn_addmul_1(r6, r2, s4, 30);

   mpn_divexact_byBm1of(r6, r6, s4, CNST_LIMB(15), CNST_LIMB(~0/15));

	TC4_DIVEXACT_2EXP(r6, s4, 2);

	mpn_sub_n(r2, r2, r6, s4);

	TC4_NORM(r1, n1, s4);
   TC4_NORM(r2, n2, s4);
   
   (*rpn) = 6*sn+1;
	cy = mpn_add_1(r3, r3, *rpn - 4*sn, r30); /* don't forget to add r3[0] back in */
   if (cy) 
	{
		rp[*rpn] = cy;
	   (*rpn)++;
	}

	tc4_copy(rp, rpn, 5*sn, r2, n2);
   tc4_copy(rp, rpn, 6*sn, r1, n1);

	tc4_copy(rp, rpn, sn, r6, s4);
   tc4_copy(rp, rpn, 3*sn, r4, s4); 
}

#if TC4_TEST
int tc4_test(mp_ptr up, mp_ptr vp, mp_size_t n)
{
	mp_limb_t * rp1 = malloc(2*n*sizeof(mp_limb_t));
   mp_limb_t * rp2 = malloc(2*n*sizeof(mp_limb_t));

	mpn_mul_n(rp1, up, vp, n);
   mpn_toom4_mul_n(rp2, up, vp, n);

	mp_size_t i;
	for (i = 0; i < 2*n; i++)
	{
		if (rp1[i] != rp2[i]) 
		{
			printf("First error in limb %d\n", i);
			free(rp1);
	      free(rp2);
	      return 0;
		}
	}
	
	free(rp1);
	free(rp2);
	return 1;
}

mp_size_t randsize(mp_size_t limit) 
{
    static uint64_t randval = 4035456057U;
    randval = ((uint64_t)randval*(uint64_t)1025416097U+(uint64_t)286824430U)%(uint64_t)4294967311U;
    
    if (limit == 0L) return (mp_size_t) randval;
    
    return (mp_size_t) randval%limit;
}

int main(void)
{
   mp_limb_t * up = malloc(2000*sizeof(mp_limb_t));
   mp_limb_t * vp = malloc(2000*sizeof(mp_limb_t));
   
	mp_size_t i, n;
   for (i = 0; i < 20000; i++)
	{
	   n = randsize(1500) + 500;
		printf("n = %d\n", n);
		mpn_random2(up, n);
		mpn_random2(vp, n);
      if (!tc4_test(up, vp, n)) break;
	}

	free(up);
	free(vp);

	return 0;
}
#endif

#if TC4_TIME
int main(void)
{
   mp_limb_t * up = malloc(40096*sizeof(mp_limb_t));
   mp_limb_t * vp = malloc(40096*sizeof(mp_limb_t));
   mp_limb_t * rp = malloc(80192*sizeof(mp_limb_t));

	mp_size_t i, n;
   n = 2048;
	mpn_random(up, n);
	mpn_random(vp, n);
   for (i = 0; i < 50000; i++)
	{
	   if ((i & 31) == 0)
		{
			mpn_random(up, n);
	      mpn_random(vp, n);
		}
		//mpn_mul_n(rp, up, vp, n);
		mpn_toom4_mul_n(rp, up, vp, n);
	}

	free(up);
	free(vp);
   free(rp);

	return 0;
}
#endif
