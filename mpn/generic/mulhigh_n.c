/* mpn_mulhigh_n

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

/*
Define the usual multiplication as 

let X=sum over 0<=i<n  of  x[i]B^i
let Y=sum over 0<=i<n  of  y[i]B^i
XY = sum over 0<=i<n 0<=j<n   x[i]y[j]B^(i+j)

Define short product as

XY_k = sum over i+j>=k  x[i]y[j]B^(i+j)

and approx short product as a superset of shortproduct and subset of usual product

Now consider the usual product XY 

XY = sum over {0<=i<n,0<=j<n}  x[i]y[j]B^(i+j)     from now we just show the sum bounds with these implicit limits on i and j
={0<=i<n,0<=j<n}
split into four pieces  (requires 0<=m<=n)
={i<n-m,j<n-m}{i>=n-m,j>=n-m}                     {i<n-m,j>=n-m}                      {i>=n-m,j<n-m}
split last two pieces again   (requires n-m<=m-1)
={i<n-m,j<n-m}{i>=n-m,j>=n-m}           {i<n-m,n-m<=j<m} {i<n-m,m<=j}        {n-m<=i<m,j<n-m} {m<=i,j<n-m}
rearrange
={i<n-m,j<n-m}{i>=n-m,j>=n-m}{i<n-m,m<=j}{m<=i,j<n-m}                  {i<n-m,n-m<=j<m}                    {n-m<=i<m,j<n-m}
split last two again      (requires  n-m<=m-2)
={i<n-m,j<n-m}{i>=n-m,j>=n-m}{i<n-m,m<=j}{m<=i,j<n-m}     {i<n-m,n-m<=j<=m-2} {i<n-m,m-2<j<m}    {n-m<=i<=m-2,j<n-m} {m-2<i<m,j<n-m}
rearrange
={i<n-m,j<n-m}{i>=n-m,j>=n-m}{i<n-m,m<=j}{m<=i,j<n-m}{i<n-m,n-m<=j<=m-2}{n-m<=i<=m-2,j<n-m}             {i<n-m,j=m-1}                    {i=m-1,j<n-m}
split last two again
={i<n-m,j<n-m}{i>=n-m,j>=n-m}{i<n-m,m<=j}{m<=i,j<n-m}{i<n-m,n-m<=j<=m-2}{n-m<=i<=m-2,j<n-m}    {i<n-m-1,j=m-1}{i=n-m-1,j=m-1}    {i=m-1,j<n-m-1}{i=m-1,j=n-m-1}

Now choose any m such that n+2<=2m  m<=n 
so n-m<=m-2  so our requirements above are satisfied 
Now consider the short product with k=n-2 , so we discard those with i+j<k=n-2 
={i<n-m,j<n-m}		i+j<=2(n-m)-2  as n+2<=2m so n<2m so 2n<2m+n so 2n-2m<n so i+j<n-2=k     so empty
 {i>=n-m,j>=n-m}	i+j>=2(n-m)		      so keep most
 {i<n-m,m<=j}						keep some
 {m<=i,j<n-m}						keep some
 {i<n-m,n-m<=j<=m-2}	i+j<=n-m-1+m-2=n-3<n-2=k      so empty
 {n-m<=i<=m-2,j<n-m}	i+j<=n-m-1+m-2=n-3<n-2=k      so empty
 {i<n-m-1,j=m-1}	i+j<=n-m-2+m-1=n-3<n-2=k      so empty
 {i=n-m-1,j=m-1}	i+j=n-m-1+m-1=n-2=k	      so keep all
 {i=m-1,j<n-m-1}	i+j<=m-1+n-m-2=n-3<n-2=k      so empty
 {i=m-1,j=n-m-1}	i+j=m-1+n-m-1=n-2=k	      so keep all
 
so the approx short product XY_k is {i>=n-m,j>=n-m} {i<n-m,m<=j} {m<=i,j<n-m} {i=n-m-1,j=m-1} {i=m-1,j=n-m-1}

Now for {i<n-m,m<=j} with i+j>=k=n-2 , let u=i,v=j-m  so we have {0<=u<n-m, 0<=v<n-m} with u+v>=n-m-2
which is the same short product 

Summary
-----------
Given n digit xp and yp , 
define mulshort_n(xp,yp,n) to be sum {i+j>=n-2,and perhaps some i+j<n-2} xp[i]yp[i]B^(i+j)
choose m such that n+2<=2m and m<n then from above , 
mulshort_n(xp,yp,n)=mul(xp+n-m,yp+n-m,m)B^(2n-2m)
                   +mulshort_n(xp+m,yp,n-m)B^m
                   +mulshort_n(xp,yp+m,n-m)B^m
                   +xp[n-m-1]yp[m-1]B^(n-2)
                   +xp[m-1]yp[n-m-1]B^(n-2)

and clearly when summing the above we can ignore any products from i+j<n-2

Theorem

Let (zp,2n)=mulshort_n(xp,yp,n) 
if zp[n-1]+n-2<B then mulhigh_n(xp,yp,n)=(zp,2n) 


*/

// (rp,2n)=(xp,n)*(yp,n) / B^n 
inline static void
mpn_mulshort_n_basecase (mp_ptr rp, mp_srcptr xp, mp_srcptr yp, mp_size_t n)
{
  mp_size_t i, k;
#if GMP_NAIL_BITS==0
  mp_limb_t t1, t2, t3;
#endif

  ASSERT (n >= 3);  // this restriction doesn't make a lot of sense in general
  ASSERT_MPN (xp, n);
  ASSERT_MPN (yp, n);
  ASSERT (!MPN_OVERLAP_P (rp, 2 * n, xp, n));
  ASSERT (!MPN_OVERLAP_P (rp, 2 * n, yp, n));
  k = n - 2;			// so want short product sum_(i+j>=k) x[i]y[j]B^(i+j)
#if GMP_NAIL_BITS!=0
  rp[n] = mpn_mul_1 (rp + k, xp + k, 2, yp[0]);
#else
  umul_ppmm (t1, rp[k], xp[k], yp[0]);
  umul_ppmm (t3, t2, xp[k + 1], yp[0]);
  add_ssaaaa (rp[n], rp[k + 1], t3, t2, 0, t1);
#endif
  for (i = 1; i <= n - 2; i++)
    rp[n + i] = mpn_addmul_1 (rp + k, xp + k - i, 2 + i, yp[i]);
  rp[n + n - 1] = mpn_addmul_1 (rp + n - 1, xp, n, yp[n - 1]);
  return;
}

// (rp,2n)=(xp,n)*(yp,n)
static void
mpn_mulshort_n (mp_ptr rp, mp_srcptr xp, mp_srcptr yp, mp_size_t n)
{
  mp_size_t m;
  mp_limb_t t;
  mp_ptr rpn2;

  ASSERT (n >= 1);
  ASSERT_MPN (xp, n);
  ASSERT_MPN (yp, n);
  ASSERT (!MPN_OVERLAP_P (rp, 2 * n, xp, n));
  ASSERT (!MPN_OVERLAP_P (rp, 2 * n, yp, n));
  if (BELOW_THRESHOLD (n, MULHIGH_BASECASE_THRESHOLD))
    {
      mpn_mul_basecase (rp, xp, n, yp, n);
      return;
    }
  if (BELOW_THRESHOLD (n, MULHIGH_DC_THRESHOLD))
    {
      mpn_mulshort_n_basecase (rp, xp, yp, n);
      return;
    }
// choose optimal m st n+2<=2m  m<n
  ASSERT (n >= 4);
  m = 87 * n / 128;
  if (2 * m < n + 2)
    m = (n + 1) / 2 + 1;
  if (m >= n)
    m = n - 1;
  ASSERT (n + 2 <= 2 * m);
  ASSERT (m < n);
  rpn2 = rp + n - 2;
  mpn_mul_n (rp + n - m + n - m, xp + n - m, yp + n - m, m);
  mpn_mulshort_n (rp, xp, yp + m, n - m);
  ASSERT_NOCARRY (mpn_add (rpn2, rpn2, n + 2, rpn2 - m, n - m + 2));
  mpn_mulshort_n (rp, xp + m, yp, n - m);
  ASSERT_NOCARRY (mpn_add (rpn2, rpn2, n + 2, rpn2 - m, n - m + 2));
  umul_ppmm (rp[1], t, xp[m - 1], yp[n - m - 1] << GMP_NAIL_BITS);
  rp[0] = t >> GMP_NAIL_BITS;
  ASSERT_NOCARRY (mpn_add (rpn2, rpn2, n + 2, rp, 2));
  umul_ppmm (rp[1], t, xp[n - m - 1], yp[m - 1] << GMP_NAIL_BITS);
  rp[0] = t >> GMP_NAIL_BITS;
  ASSERT_NOCARRY (mpn_add (rpn2, rpn2, n + 2, rp, 2));
  return;
}

// (rp,2n)=(xp,n)*(yp,n)
void
mpn_mulhigh_n (mp_ptr rp, mp_srcptr xp, mp_srcptr yp, mp_size_t n)
{
  mp_limb_t t;

  ASSERT (n > 0);
  ASSERT_MPN (xp, n);
  ASSERT_MPN (yp, n);
  ASSERT (!MPN_OVERLAP_P (rp, 2 * n, xp, n));
  ASSERT (!MPN_OVERLAP_P (rp, 2 * n, yp, n));
  if (BELOW_THRESHOLD (n, MULHIGH_BASECASE_THRESHOLD))
    {
      mpn_mul_basecase (rp, xp, n, yp, n);
      return;
    }
  if (ABOVE_THRESHOLD (n, MULHIGH_MUL_THRESHOLD))
    {
      mpn_mul_n (rp, xp, yp, n);
      return;
    }
  mpn_mulshort_n (rp, xp, yp, n);
  t = rp[n - 1] + n - 2;
  if (UNLIKELY (t < n - 2))
    mpn_mul_n (rp, xp, yp, n);
  return;
}
