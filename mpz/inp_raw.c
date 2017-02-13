/* mpz_inp_raw -- read an mpz_t in raw format.

Copyright 2001, 2002, 2005 Free Software Foundation, Inc.

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

#include <stdio.h>
#include "mpir.h"
#include "gmp-impl.h"


/* NTOH_LIMB_FETCH fetches a limb which is in network byte order (ie. big
   endian) and produces a normal host byte order result. */

#if HAVE_LIMB_BIG_ENDIAN
#define NTOH_LIMB_FETCH(limb, src)  do { (limb) = *(src); } while (0)
#endif

#if HAVE_LIMB_LITTLE_ENDIAN
#define NTOH_LIMB_FETCH(limb, src)  BSWAP_LIMB_FETCH (limb, src)
#endif

#ifndef NTOH_LIMB_FETCH
#define NTOH_LIMB_FETCH(limb, src)                              \
  do {                                                          \
    const unsigned char  *__p = (const unsigned char *) (src);  \
    mp_limb_t  __limb;                                          \
    int        __i;                                             \
    __limb = 0;                                                 \
    for (__i = 0; __i < BYTES_PER_MP_LIMB; __i++)               \
      __limb = (__limb << 8) | __p[__i];                        \
    (limb) = __limb;                                            \
  } while (0)
#endif

/* In order to allow mpz_inp_raw() to be called from MPIR.Net, its implementation has been refactored into three separate functions.
   Both the contract and implementation of mpz_inp_raw() were unchanged, the split was made in order for MPIR.Net to access intermediate variables.
   The basic flow of mpz_inp_raw is to 1) read a 4-byte size from file, 2) re-allocate the destination __mpz_struct accordingly, 
   3) read raw limb data from file, and 4) reconstitute the limb data from raw format.
   Of these, steps 2 and 4 represent the (in-memory) bulk of the entire operation.
   The new mpz_inp_raw_p() function below performs step 2.  mpz_inp_raw_m() performs step 4.
   To pass internal state from step to step an mpir_out struct is used.
   mpz_inp_raw() now performs file I/O for steps 1 and 3, and calls into mpz_inp_raw_m() and mpz_inp_raw_p() for steps 2 and 4 respectively.
   MPIR.Net performs its own file I/O using different infrastructure, and calls mpz_inp_raw_m() and mpz_inp_raw_p() for the rest. */
void mpz_inp_raw_p (mpz_ptr x, unsigned char* csize_bytes, mpir_out_ptr out)
{
  mp_size_t      csize, abs_xsize, i;
  size_t         abs_csize;
  mp_ptr         xp;

  csize =
    (  (mp_size_t) csize_bytes[0] << 24)
    + ((mp_size_t) csize_bytes[1] << 16)
    + ((mp_size_t) csize_bytes[2] << 8)
    + ((mp_size_t) csize_bytes[3]);

  /* Sign extend if necessary.
     Could write "csize -= ((csize & 0x80000000L) << 1)", but that tickles a
     bug in gcc 3.0 for powerpc64 on AIX.  */
/* We exculde win32 as msvc gives a spurious warning */
#if !defined(_WIN32) || defined(_WIN64)
  if (sizeof (csize) > 4 && csize & 0x80000000L)
    csize |= (mp_size_t)(-1) << 32;
#endif

  abs_csize = ABS (csize);

  /* round up to a multiple of limbs */
  abs_xsize = (abs_csize*8 + GMP_NUMB_BITS-1) / GMP_NUMB_BITS;

  if (abs_xsize != 0)
    {
      MPZ_REALLOC (x, abs_xsize);
      xp = PTR(x);

      /* Get limb boundaries right in the read, for the benefit of the
         non-nails case.  */
      xp[0] = 0;
      out->written = (char *) (xp + abs_xsize) - abs_csize;
    }
  out->writtenSize = abs_csize;
  out->allocatedSize = abs_xsize;
  SIZ(x) = (csize >= 0 ? abs_xsize : -abs_xsize);
}

void mpz_inp_raw_m(mpz_ptr x, mpir_out_ptr out)
{
      mp_ptr         xp, sp, ep;
      mp_size_t      abs_xsize, i;
      mp_limb_t      slimb, elimb;

      abs_xsize = out->allocatedSize;

      xp = PTR(x);
      if (GMP_NAIL_BITS == 0)
        {
          /* Reverse limbs to least significant first, and byte swap.  If
             abs_xsize is odd then on the last iteration elimb and slimb are
             the same.  It doesn't seem extra code to handle that case
             separately, to save an NTOH.  */
          sp = xp;
          ep = xp + abs_xsize-1;
          for (i = 0; i < (abs_xsize+1)/2; i++)
            {
              NTOH_LIMB_FETCH (elimb, ep);
              NTOH_LIMB_FETCH (slimb, sp);
              *sp++ = elimb;
              *ep-- = slimb;
            }
        }
      else
        {
          /* It ought to be possible to do the transformation in-place, but
             for now it's easier to use an extra temporary area.  */
          mp_limb_t  byte, limb;
          int        bits;
          mp_size_t  tpos;
          mp_ptr     tp;
          TMP_DECL;

          TMP_MARK;
          tp = TMP_ALLOC_LIMBS (abs_xsize);
          limb = 0;
          bits = 0;
          tpos = 0;
          for (i = out->writtenSize-1; i >= 0; i--)
            {
              byte = out->written[i];
              limb |= (byte << bits);
              bits += 8;
              if (bits >= GMP_NUMB_BITS)
                {
                  ASSERT (tpos < abs_xsize);
                  tp[tpos++] = limb & GMP_NUMB_MASK;
                  bits -= GMP_NUMB_BITS;
                  ASSERT (bits < 8);
                  limb = byte >> (8 - bits);
                }
            }
          if (bits != 0)
            {
              ASSERT (tpos < abs_xsize);
              tp[tpos++] = limb;
            }
          ASSERT (tpos == abs_xsize);

          MPN_COPY (xp, tp, abs_xsize);
          TMP_FREE;
        }

      /* GMP 1.x mpz_out_raw wrote high zero bytes, strip any high zero
         limbs resulting from this.  Should be a non-zero value here, but
         for safety don't assume that. */
      MPN_NORMALIZE (xp, abs_xsize);
      SIZ(x) = (SIZ(x) >= 0 ? abs_xsize : -abs_xsize);
}

size_t
mpz_inp_raw(mpz_ptr x, FILE *fp)
{
    unsigned char  csize_bytes[4];
    mpir_out_struct out;

    if (fp == 0)
        fp = stdin;

    /* 4 bytes for size */
    if (fread(csize_bytes, sizeof(csize_bytes), 1, fp) != 1)
        return 0;

    mpz_inp_raw_p(x, csize_bytes, out);

    if (out->writtenSize != 0)
    {
        if (fread(out->written, out->writtenSize, 1, fp) != 1)
            return 0;

        mpz_inp_raw_m(x, out);
    }
    return out->writtenSize + 4;
}
