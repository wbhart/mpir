/* x86 fat binary initializers.

   THE FUNCTIONS AND VARIABLES IN THIS FILE ARE FOR INTERNAL USE ONLY.
   THEY'RE ALMOST CERTAIN TO BE SUBJECT TO INCOMPATIBLE CHANGES OR DISAPPEAR
   COMPLETELY IN FUTURE GNU MP RELEASES.

Copyright 2003, 2004 Free Software Foundation, Inc.

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

#include <stdio.h>    /* for printf */
#include <stdlib.h>   /* for getenv */
#include <string.h>

#include "mpir.h"
#include "gmp-impl.h"

/* Change this to "#define TRACE(x) x" for some traces. */
#define TRACE(x)

/* Change this to 1 to take the cpuid from GMP_CPU_TYPE env var. */
#define WANT_FAKE_CPUID  0


/* fat_entry.asm */
long __gmpn_cpuid __GMP_PROTO ((char dst[12], int id));
int  __gmpn_cpuid_available __GMP_PROTO ((void));



#if WANT_FAKE_CPUID
/* The "name"s in the table are values for the GMP_CPU_TYPE environment
   variable.  Anything can be used, but for now it's the canonical cpu types
   as per config.guess/config.sub.  */

#define __gmpn_cpuid            fake_cpuid
#define __gmpn_cpuid_available  fake_cpuid_available

#define MAKE_FMS(family, model) \
  (((family) << 8) + ((model << 4)))

static struct {
  const char  *name;
  const char  vendor[13];
  unsigned    fms;
} fake_cpuid_table[] = {
  { "i386",       "" },
  { "i486",       "GenuineIntel", MAKE_FMS (4, 0) },
  { "pentium",    "GenuineIntel", MAKE_FMS (5, 0) },
  { "pentiummmx", "GenuineIntel", MAKE_FMS (5, 4) },
  { "pentiumpro", "GenuineIntel", MAKE_FMS (6, 0) },
  { "pentium2",   "GenuineIntel", MAKE_FMS (6, 2) },
  { "pentium3",   "GenuineIntel", MAKE_FMS (6, 7) },
  { "pentium4",   "GenuineIntel", MAKE_FMS (7, 0) },

  { "k5",         "AuthenticAMD", MAKE_FMS (5, 0) },
  { "k6",         "AuthenticAMD", MAKE_FMS (5, 3) },
  { "k62",        "AuthenticAMD", MAKE_FMS (5, 8) },
  { "k63",        "AuthenticAMD", MAKE_FMS (5, 9) },
  { "k7",         "AuthenticAMD", MAKE_FMS (6, 0) },
  { "x86_64",     "AuthenticAMD", MAKE_FMS (15, 0) },

  { "viac3",      "CentaurHauls", MAKE_FMS (6, 0) },
  { "viac32",     "CentaurHauls", MAKE_FMS (6, 9) },
};

static int
fake_cpuid_lookup (void)
{
  char  *s;
  int   i;

  s = getenv ("GMP_CPU_TYPE");
  if (s == NULL)
    {
      printf ("Need GMP_CPU_TYPE environment variable for fake cpuid\n");
      abort ();
    }

  for (i = 0; i < numberof (fake_cpuid_table); i++)
    if (strcmp (s, fake_cpuid_table[i].name) == 0)
      return i;

  printf ("GMP_CPU_TYPE=%s unknown\n", s);
  abort ();
}

static int
fake_cpuid_available (void)
{
  return fake_cpuid_table[fake_cpuid_lookup()].vendor[0] != '\0';
}

static long
fake_cpuid (char dst[12], int id)
{
  int  i = fake_cpuid_lookup();

  switch (id) {
  case 0:
    memcpy (dst, fake_cpuid_table[i].vendor, 12);
    return 0;
  case 1:
    return fake_cpuid_table[i].fms;
  default:
    printf ("fake_cpuid(): oops, unknown id %d\n", id);
    abort ();
  }
}
#endif


typedef DECL_preinv_divrem_1 ((*preinv_divrem_1_t));
typedef DECL_preinv_mod_1    ((*preinv_mod_1_t));

struct cpuvec_t __gmpn_cpuvec = {
  __MPN(add_n_init),
  __MPN(addmul_1_init),
  __MPN(copyd_init),
  __MPN(copyi_init),
  __MPN(divexact_1_init),
  __MPN(divexact_by3c_init),
  __MPN(divexact_byfobm1_init),
  __MPN(divrem_1_init),
  __MPN(divrem_2_init),
  __MPN(divrem_euclidean_qr_1_init),
  __MPN(divrem_euclidean_qr_2_init),
  __MPN(gcd_1_init),
  __MPN(lshift_init),
  __MPN(mod_1_init),
  __MPN(mod_34lsub1_init),
  __MPN(modexact_1c_odd_init),
  __MPN(mul_1_init),
  __MPN(mul_basecase_init),
  __MPN(preinv_divrem_1_init),
  __MPN(preinv_mod_1_init),
  __MPN(redc_1_init),
  __MPN(rshift_init),
  __MPN(sqr_basecase_init),
  __MPN(sub_n_init),
  __MPN(submul_1_init),
  __MPN(sumdiff_n_init),
  0
};


/* The following setups start with generic x86, then overwrite with
   specifics for a chip, and higher versions of that chip.

   The arrangement of the setups here will normally be the same as the $path
   selections in configure.in for the respective chips.

   This code is reentrant and thread safe.  We always calculate the same
   decided_cpuvec, so if two copies of the code are running it doesn't
   matter which completes first, both write the same to __gmpn_cpuvec.

   We need to go via decided_cpuvec because if one thread has completed
   __gmpn_cpuvec then it may be making use of the threshold values in that
   vector.  If another thread is still running __gmpn_cpuvec_init then we
   don't want it to write different values to those fields since some of the
   asm routines only operate correctly up to their own defined threshold,
   not an arbitrary value.  */

void
__gmpn_cpuvec_init (void)
{
  struct cpuvec_t  decided_cpuvec;

  TRACE (printf ("__gmpn_cpuvec_init:\n"));

  memset (&decided_cpuvec, '\0', sizeof (decided_cpuvec));

  CPUVEC_SETUP_x86;
  CPUVEC_SETUP_fat;

  if (! __gmpn_cpuid_available ())
    {
      TRACE (printf ("  80386, or early 80486 without cpuid\n"));
    }
  else
    {


#define CONFIG_GUESS            0
#define CONFIG_GUESS_32BIT      0
#define CONFIG_GUESS_64BIT      0
#define FAT32                   1
#define FAT64                   0
#define INFAT                   1

#define CPUSETUP_pentium	CPUVEC_SETUP_pentium
#define CPUSETUP_pentiummmx	CPUVEC_SETUP_pentium;CPUVEC_SETUP_pentium_mmx
#define CPUSETUP_pentiumpro	CPUVEC_SETUP_p6
#define CPUSETUP_pentium2	CPUVEC_SETUP_p6;CPUVEC_SETUP_p6_mmx
#define CPUSETUP_pentium3	CPUVEC_SETUP_p6;CPUVEC_SETUP_p6_mmx;CPUVEC_SETUP_p6_p3mmx
#define CPUSETUP_core		CPUVEC_SETUP_p6;CPUVEC_SETUP_p6_mmx;CPUVEC_SETUP_p6_p3mmx
#define CPUSETUP_core2		CPUVEC_SETUP_core2
#define CPUSETUP_penryn		CPUVEC_SETUP_p6;CPUVEC_SETUP_p6_mmx;CPUVEC_SETUP_p6_p3mmx
#define CPUSETUP_nehalem	CPUVEC_SETUP_nehalem
#define CPUSETUP_westmere	CPUVEC_SETUP_nehalem
#define CPUSETUP_sandybridge	CPUVEC_SETUP_nehalem
#define CPUSETUP_atom		CPUVEC_SETUP_p6;CPUVEC_SETUP_p6_mmx;CPUVEC_SETUP_p6_p3mmx
#define CPUSETUP_nano		CPUVEC_SETUP_p6;CPUVEC_SETUP_p6_mmx;CPUVEC_SETUP_p6_p3mmx
#define CPUSETUP_pentium4	CPUVEC_SETUP_pentium4;CPUVEC_SETUP_pentium4_mmx;CPUVEC_SETUP_pentium4_sse2
#define CPUSETUP_prescott	CPUVEC_SETUP_pentium4;CPUVEC_SETUP_pentium4_mmx;CPUVEC_SETUP_pentium4_sse2
#define CPUSETUP_k5		do{}while(0)
#define CPUSETUP_k6		CPUVEC_SETUP_k6;CPUVEC_SETUP_k6_mmx
#define CPUSETUP_k62		CPUVEC_SETUP_k6;CPUVEC_SETUP_k6_mmx;CPUVEC_SETUP_k6_k62mmx
#define CPUSETUP_k63		CPUVEC_SETUP_k6;CPUVEC_SETUP_k6_mmx;CPUVEC_SETUP_k6_k62mmx
#define CPUSETUP_k7		CPUVEC_SETUP_k7;CPUVEC_SETUP_k7_mmx
#define CPUSETUP_k8		CPUVEC_SETUP_k7;CPUVEC_SETUP_k7_mmx;CPUVEC_SETUP_k7_mmx_k8
#define CPUSETUP_k10		CPUVEC_SETUP_k7;CPUVEC_SETUP_k7_mmx;CPUVEC_SETUP_k7_mmx_k8;CPUVEC_SETUP_k7_mmx_k8_k10
#define CPUSETUP_k102		CPUVEC_SETUP_k7;CPUVEC_SETUP_k7_mmx;CPUVEC_SETUP_k7_mmx_k8;CPUVEC_SETUP_k7_mmx_k8_k10;CPUVEC_SETUP_k7_mmx_k8_k10_k102
#define CPUSETUP_viac3		do{}while(0)
#define CPUSETUP_viac32		do{}while(0)

#include "cpuid.c"

    }

  /* There's no x86 generic mpn_preinv_divrem_1 or mpn_preinv_mod_1.
     Instead default to the plain versions from whichever CPU we detected.
     The function arguments are compatible, no need for any glue code.  */
  if (decided_cpuvec.preinv_divrem_1 == NULL)
    decided_cpuvec.preinv_divrem_1 =(preinv_divrem_1_t)decided_cpuvec.divrem_1;
  if (decided_cpuvec.preinv_mod_1 == NULL)
    decided_cpuvec.preinv_mod_1    =(preinv_mod_1_t)   decided_cpuvec.mod_1;

  ASSERT_CPUVEC (decided_cpuvec);
  CPUVEC_INSTALL (decided_cpuvec);

  /* Set this once the threshold fields are ready.
     Use volatile to prevent it getting moved.  */
  ((volatile struct cpuvec_t *) &__gmpn_cpuvec)->initialized = 1;
}
