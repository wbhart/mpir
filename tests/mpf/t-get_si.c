/* Exercise mpf_get_si.

Copyright 2000, 2001 Free Software Foundation, Inc.

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
#include <stdlib.h>
#include "mpir.h"
#include "gmp-impl.h"
#include "tests.h"

#define printf gmp_printf
void
check_data (void)
{
  static const struct {
    int         base;
    const char  *f;
    mpir_si      want;
  } data[] = {
    { 10, "0",      0 },
    { 10, "1",      1 },
    { 10, "-1",     -1 },
    { 10, "2",      2 },
    { 10, "-2",     -2 },
    { 10, "12345",  12345 },
    { 10, "-12345", -12345 },

    /* fraction bits ignored */
    { 10, "0.5",    0 },
    { 10, "-0.5",   0 },
    { 10, "1.1",    1 },
    { 10, "-1.1",   -1 },
    { 10, "1.9",    1 },
    { 10, "-1.9",   -1 },
    { 16, "1.000000000000000000000000000000000000000000000000001", 1L },
    { 16, "-1.000000000000000000000000000000000000000000000000001", -1L },

    /* low bits extracted (this is undocumented) */
    { 16, "1000000000000000000000000000000000000000000000000001", 1L },
    { 16, "-1000000000000000000000000000000000000000000000000001", -1L },
  };

  int    i;
  mpf_t  f;
  mpir_si   got;

  mpf_init2 (f, 2000);
  for (i = 0; i < numberof (data); i++)
    {
      mpf_set_str_or_abort (f, data[i].f, data[i].base);

      got = mpf_get_si (f);
      if (got != data[i].want)
        {
          printf ("mpf_get_si wrong at data[%d]\n", i); 
          printf ("   f     \"%s\"\n", data[i].f);
          printf ("     dec "); mpf_out_str (stdout, 10, 0, f); printf ("\n");
          printf ("     hex "); mpf_out_str (stdout, 16, 0, f); printf ("\n");
          printf ("     size %ld\n", (long) SIZ(f));
          printf ("     exp  %ld\n", (long) EXP(f));
          printf ("   got   %Md (%#Mx)\n", got, got);
          printf ("   want  %Md (%#Mx)\n", data[i].want, data[i].want);
          abort();                                    
        }
    }
  mpf_clear (f);
}


void
check_max (void)
{
  mpf_t  f;
  mpir_si   want;
  mpir_si   got;

  mpf_init2 (f, 200L);

#define CHECK_MAX(name)                                         \
  if (got != want)                                              \
    {                                                           \
      printf ("mpf_get_si wrong on %s\n", name);                \
      printf ("   f    ");                                      \
      mpf_out_str (stdout, 10, 0, f); printf (", hex ");        \
      mpf_out_str (stdout, 16, 0, f); printf ("\n");            \
      printf ("   got  %ld, hex %#Mx\n", got, got);             \
      printf ("   want %ld, hex %#Mx\n", want, want);           \
      abort();                                                  \
    }

  want = GMP_SI_MAX;
  mpf_set_si (f, want);
  got = mpf_get_si (f);
  CHECK_MAX ("GMP_SI_MAX");

  want = GMP_SI_MIN;
  mpf_set_si (f, want);
  got = mpf_get_si (f);
  CHECK_MAX ("GMP_SI_MIN");

  mpf_clear (f);
}


void
check_limbdata (void)
{
#define M  GMP_NUMB_MAX
  
  static const struct {
    mp_exp_t       exp;
    mp_size_t      size;
    mp_limb_t      d[10];
    mpir_si         want;

  } data[] = {

    /* in the comments here, a "_" indicates a digit (ie. limb) position not
       included in the d data, and therefore zero */

    { 0, 0, { 0 }, 0 },    /* 0 */

    { 1,  1, { 1 }, 1 },   /* 1 */
    { 1, -1, { 1 }, -1 },  /* -1 */

    { 0,  1, { 1 }, 0 },   /* .1 */
    { 0, -1, { 1 }, 0 },   /* -.1 */

    { -1,  1, { 1 }, 0 },  /* ._1 */
    { -1, -1, { 1 }, 0 },  /* -._1 */

    { -999,          1, { 1 }, 0 },   /* .___1 small */
    { MP_EXP_T_MIN,  1, { 1 }, 0 },   /* .____1 very small */

    { 999,          1, { 1 }, 0 },    /* 1____. big */
    { MP_EXP_T_MAX, 1, { 1 }, 0 },    /* 1_____. very big */

    { 1, 2, { 999, 2 }, 2L },                  /* 2.9 */
    { 5, 8, { 7, 8, 9, 3, 0, 0, 0, 1 }, 3 },  /* 10003.987 */

    { 2, 2, { M, M },    GMP_SI_MAX }, /* FF. */
    { 2, 2, { M, M, M }, GMP_SI_MAX }, /* FF.F */
    { 3, 3, { M, M, M }, GMP_SI_MAX }, /* FFF. */

#if GMP_NUMB_BITS >= BITS_PER_UI
    /* normal case, numb bigger than long */
    { 2,  1, { 1 },    0 },      /* 1_. */
    { 2,  2, { 0, 1 }, 0 },      /* 10. */
    { 2,  2, { 999, 1 }, 999 },  /* 19. */
    { 3,  2, { 999, 1 }, 0 },    /* 19_. */

#else
    /* nails case, numb smaller than long */
    { 2,  1, { 1 }, 1 << GMP_NUMB_BITS },  /* 1_. */
    { 3,  1, { 1 }, 0 },                   /* 1__. */

    { 2,  2, { 99, 1 },    99 + (1 << GMP_NUMB_BITS) },  /* 19. */
    { 3,  2, { 1, 99 },    1 << GMP_NUMB_BITS },          /* 91_. */
    { 3,  3, { 0, 1, 99 }, 1 << GMP_NUMB_BITS },          /* 910. */

#endif
  };

  mpf_t          f;
  mpir_si         got;
  int            i;
  mp_limb_t      buf[20 + numberof(data[i].d)];

  for (i = 0; i < numberof (data); i++)
    {
      refmpn_fill (buf, 10, CNST_LIMB(0xDEADBEEF));
      refmpn_copy (buf+10, data[i].d, ABS(data[i].size));
      refmpn_fill (buf+10+ABS(data[i].size), 10, CNST_LIMB(0xDEADBEEF));

      PTR(f) = buf+10;
      EXP(f) = data[i].exp;
      SIZ(f) = data[i].size;
      PREC(f) = numberof (data[i].d);
      MPF_CHECK_FORMAT (f);

      got = mpf_get_si (f);
      if (got != data[i].want)
        {
          printf    ("mpf_get_si wrong at limb data[%d]\n", i);
          mpf_trace ("  f", f);
          mpn_trace ("  d", data[i].d, data[i].size);
          printf    ("  size %ld\n", (mpir_si) data[i].size);
          printf    ("  exp %ld\n", (mpir_si) data[i].exp);
          printf    ("  got   %Mu (%#Mx)\n", got, got);
          printf    ("  want  %Mu (%#Mx)\n", data[i].want, data[i].want);
          abort();
        }
    }
}


int
main (void)
{
  tests_start ();

  check_data ();
  check_max ();
  check_limbdata ();

  tests_end ();
  exit (0);
}
