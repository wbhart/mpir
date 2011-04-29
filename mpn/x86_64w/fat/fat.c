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

/* fat_entry.asm */
long __gmpn_cpuid __GMP_PROTO ((char dst[12], int id));

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

#define CONFIG_GUESS            0
#define CONFIG_GUESS_32BIT      0
#define CONFIG_GUESS_64BIT      0
#define FAT32                   0
#define FAT64                   1
#define INFAT                   1

#define CPUSETUP_core2		CPUVEC_SETUP_core2
#define CPUSETUP_penryn		CPUVEC_SETUP_core2;CPUVEC_SETUP_core2_penryn
#define CPUSETUP_nehalem	CPUVEC_SETUP_nehalem
#define CPUSETUP_westmere	CPUVEC_SETUP_nehalem;CPUVEC_SETUP_nehalem_westmere
#define CPUSETUP_sandybridge	CPUVEC_SETUP_sandybridge
#define CPUSETUP_atom		CPUVEC_SETUP_atom
#define CPUSETUP_nano		CPUVEC_SETUP_k8;CPUVEC_SETUP_k8_k8only
#define CPUSETUP_netburst	CPUVEC_SETUP_netburst
#define CPUSETUP_netburstlahf	CPUVEC_SETUP_netburst;
#define CPUSETUP_k8		CPUVEC_SETUP_k8;CPUVEC_SETUP_k8_k8only
#define CPUSETUP_k10		CPUVEC_SETUP_k8;CPUVEC_SETUP_k8_k10
#define CPUSETUP_k102		CPUVEC_SETUP_k8;CPUVEC_SETUP_k8_k10;CPUVEC_SETUP_k8_k10_k102
#define CPUSETUP_k103		CPUVEC_SETUP_k8;CPUVEC_SETUP_k8_k10;CPUVEC_SETUP_k8_k10_k102
#define CPUSETUP_bulldozer	CPUVEC_SETUP_k8;CPUVEC_SETUP_k8_k10;CPUVEC_SETUP_k8_k10_k102
#define CPUSETUP_bobcat		CPUVEC_SETUP_bobcat

#include "cpuid.c"

void
__gmpn_cpuvec_init (void)
{
  struct cpuvec_t  decided_cpuvec;

  TRACE (printf ("__gmpn_cpuvec_init:\n"));

  __gmpn_cpu(&decided_cpuvec);

  ASSERT_CPUVEC (decided_cpuvec);
  CPUVEC_INSTALL (decided_cpuvec);

  /* Set this once the threshold fields are ready.
     Use volatile to prevent it getting moved.  */
  ((volatile struct cpuvec_t *) &__gmpn_cpuvec)->initialized = 1;
}
