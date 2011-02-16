/* middle product test code

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

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"
#include "tests.h"

#if defined(_MSC_VER) || defined(__MINGW32__)
#define random rand
#endif

int compare_ul(const void* a, const void* b)
{
  unsigned long aa = * (unsigned long*) a;
  unsigned long bb = * (unsigned long*) b;
  if (aa < bb)
    return -1;
  if (aa > bb)
    return 1;
  return 0;
}

#define SIZE 400
#define REPEAT 500
#define SAFETY 16

int
main (int argc, char* argv[])
{
  mp_limb_t up[2*SIZE];
  mp_limb_t vp[SIZE];
  mp_limb_t buf[SIZE + 2*SAFETY + 3];
  mp_ptr rp = buf + SAFETY;
  mp_limb_t ref[SIZE + 3];
  mp_limb_t scratch[10*SIZE + 100];
  mp_size_t un, vn;
  gmp_randstate_t rands;
  
  const mp_limb_t sentry = 0x012345678;
  
  int i, j;

  tests_start();
  gmp_randinit_default(rands);
  
  for (i = 0; i < REPEAT; i++)
    {
      vn = (random() % SIZE) + 1;
      un = (random() % (SIZE + 1 - vn)) + vn;

      mpn_rrandom (up, rands, un);
      mpn_rrandom (vp, rands, vn);

      refmpn_mulmid (ref, up, un, vp, vn);

      for (j = 0; j < SAFETY; j++)
        rp[-j - 1] = rp[un - vn + 3 + j] = sentry;
      mpn_mulmid_basecase (rp, up, un, vp, vn);

      for (j = 0; j < SAFETY; j++)
        if (rp[-j - 1] != sentry || rp[un - vn + 3 + j] != sentry)
		{
		  printf ("sentry overwritten\n");
		  abort();
		}
      if (mpn_cmp (rp, ref, un - vn + 3) != 0)
        {
          printf ("failed:\n");
          printf ("un = %lu, vn = %lu\n\n", un, vn);
          gmp_printf (" up: %Nx\n\n", up, un);
          gmp_printf (" vp: %Nx\n\n", vp, vn);
          gmp_printf (" rp: %Nx\n\n", rp, un - vn + 3);
          gmp_printf ("ref: %Nx\n\n", ref, un - vn + 3);
          abort ();
        }
    }

  for (i = 0; i < REPEAT; i++)
    {
      vn = (random() % SIZE) + 1;
      un = (random() % (SIZE + 1 - vn)) + vn;

      mpn_rrandom (up, rands, un);
      mpn_rrandom (vp, rands, vn);

      refmpn_mulmid (ref, up, un, vp, vn);

      for (j = 0; j < SAFETY; j++)
        rp[-j - 1] = rp[un - vn + 3 + j] = sentry;
      mpn_mulmid (rp, up, un, vp, vn);

      for (j = 0; j < SAFETY; j++)
        if (rp[-j - 1] != sentry || rp[un - vn + 3 + j] != sentry)
		{
		  printf ("sentry overwritten\n");
		  abort();
		}
      if (mpn_cmp (rp, ref, un - vn + 3) != 0)
        {
          printf ("failed:\n");
          printf ("un = %lu, vn = %lu\n\n", un, vn);
          gmp_printf (" up: %Nx\n\n", up, un);
          gmp_printf (" vp: %Nx\n\n", vp, vn);
          gmp_printf (" rp: %Nx\n\n", rp, un - vn + 3);
          gmp_printf ("ref: %Nx\n\n", ref, un - vn + 3);
          abort ();
        }
    }

  for (i = 0; i < REPEAT; i++)
    {
      vn = (random() % (SIZE - 4)) + 4;
      un = 2*vn - 1;

      mpn_rrandom (up, rands, un);
      mpn_rrandom (vp, rands, vn);

      refmpn_mulmid (ref, up, un, vp, vn);

      for (j = 0; j < SAFETY; j++)
        rp[-j - 1] = rp[un - vn + 3 + j] = sentry;
      mpn_toom42_mulmid (rp, up, vp, vn, scratch);

      for (j = 0; j < SAFETY; j++)
        if (rp[-j - 1] != sentry || rp[un - vn + 3 + j] != sentry)
		{
		  printf ("sentry overwritten\n");
		  abort();
		}
      if (mpn_cmp (rp, ref, un - vn + 3) != 0)
        {
          printf ("failed:\n");
          printf ("un = %lu, vn = %lu\n\n", un, vn);
          gmp_printf (" up: %Nx\n\n", up, un);
          gmp_printf (" vp: %Nx\n\n", vp, vn);
          gmp_printf (" rp: %Nx\n\n", rp, un - vn + 3);
          gmp_printf ("ref: %Nx\n\n", ref, un - vn + 3);
          abort ();
        }
    }
  gmp_randclear(rands);
  tests_end();
  return 0;
}
