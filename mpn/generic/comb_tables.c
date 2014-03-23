/* Const tables shared among combinatoric functions.

   THE CONTENTS OF THIS FILE ARE FOR INTERNAL USE AND ARE ALMOST CERTAIN TO
   BE SUBJECT TO INCOMPATIBLE CHANGES IN FUTURE GNU MP RELEASES.

Copyright 2012 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library.  If not, see http://www.gnu.org/licenses/.  */

#include "mpir.h"
#include "gmp-impl.h"

#if GMP_LIMB_BITS == 64

/* This table is 0!,1!,2!/2,3!/2,...,n!/2^sn where n!/2^sn is an */
/* odd integer for each n, and n!/2^sn has <= GMP_NUMB_BITS bits */
#define ONE_LIMB_ODD_FACTORIAL_TABLE CNST_LIMB(0x1),CNST_LIMB(0x1),CNST_LIMB(0x1),CNST_LIMB(0x3),CNST_LIMB(0x3),CNST_LIMB(0xf),CNST_LIMB(0x2d),CNST_LIMB(0x13b),CNST_LIMB(0x13b),CNST_LIMB(0xb13),CNST_LIMB(0x375f),CNST_LIMB(0x26115),CNST_LIMB(0x7233f),CNST_LIMB(0x5cca33),CNST_LIMB(0x2898765),CNST_LIMB(0x260eeeeb),CNST_LIMB(0x260eeeeb),CNST_LIMB(0x286fddd9b),CNST_LIMB(0x16beecca73),CNST_LIMB(0x1b02b930689),CNST_LIMB(0x870d9df20ad),CNST_LIMB(0xb141df4dae31),CNST_LIMB(0x79dd498567c1b),CNST_LIMB(0xaf2e19afc5266d),CNST_LIMB(0x20d8a4d0f4f7347),CNST_LIMB(0x335281867ec241ef)

/* Previous table, continued, values modulo 2^GMP_NUMB_BITS */
#define ONE_LIMB_ODD_FACTORIAL_EXTTABLE CNST_LIMB(0x9b3093d46fdd5923),CNST_LIMB(0x5e1f9767cc5866b1),CNST_LIMB(0x92dd23d6966aced7),CNST_LIMB(0xa30d0f4f0a196e5b),CNST_LIMB(0x8dc3e5a1977d7755),CNST_LIMB(0x2ab8ce915831734b),CNST_LIMB(0x2ab8ce915831734b),CNST_LIMB(0x81d2a0bc5e5fdcab),CNST_LIMB(0x9efcac82445da75b),CNST_LIMB(0xbc8b95cf58cde171),CNST_LIMB(0xa0e8444a1f3cecf9),CNST_LIMB(0x4191deb683ce3ffd),CNST_LIMB(0xddd3878bc84ebfc7),CNST_LIMB(0xcb39a64b83ff3751),CNST_LIMB(0xf8203f7993fc1495),CNST_LIMB(0xbd2a2a78b35f4bdd),CNST_LIMB(0x84757be6b6d13921),CNST_LIMB(0x3fbbcfc0b524988b),CNST_LIMB(0xbd11ed47c8928df9),CNST_LIMB(0x3c26b59e41c2f4c5),CNST_LIMB(0x677a5137e883fdb3),CNST_LIMB(0xff74e943b03b93dd),CNST_LIMB(0xfe5ebbcb10b2bb97),CNST_LIMB(0xb021f1de3235e7e7),CNST_LIMB(0x33509eb2e743a58f),CNST_LIMB(0x390f9da41279fb7d),CNST_LIMB(0xe5cb0154f031c559),CNST_LIMB(0x93074695ba4ddb6d),CNST_LIMB(0x81c471caa636247f),CNST_LIMB(0xe1347289b5a1d749),CNST_LIMB(0x286f21c3f76ce2ff),CNST_LIMB(0xbe84a2173e8ac7),CNST_LIMB(0x1595065ca215b88b),CNST_LIMB(0xf95877595b018809),CNST_LIMB(0x9c2efe3c5516f887),CNST_LIMB(0x373294604679382b),CNST_LIMB(0xaf1ff7a888adcd35),CNST_LIMB(0x18ddf279a2c5800b),CNST_LIMB(0x18ddf279a2c5800b),CNST_LIMB(0x505a90e2542582cb),CNST_LIMB(0x5bacad2cd8d5dc2b),CNST_LIMB(0xfe3152bcbff89f41)

/* This table is 1!!,3!!,...,(2n+1)!! where (2n+1)!! has <= GMP_NUMB_BITS bits */
#define ONE_LIMB_ODD_DOUBLEFACTORIAL_TABLE CNST_LIMB(0x1),CNST_LIMB(0x3),CNST_LIMB(0xf),CNST_LIMB(0x69),CNST_LIMB(0x3b1),CNST_LIMB(0x289b),CNST_LIMB(0x20fdf),CNST_LIMB(0x1eee11),CNST_LIMB(0x20dcf21),CNST_LIMB(0x27065f73),CNST_LIMB(0x33385d46f),CNST_LIMB(0x49a10615f9),CNST_LIMB(0x730b9982551),CNST_LIMB(0xc223930bef8b),CNST_LIMB(0x15fe07a85a22bf),CNST_LIMB(0x2a9c2ed62ea3521),CNST_LIMB(0x57e22099c030d941)

/* This table contains 2n-popc(2n) for small n */

/* It begins with 2-1=1 (n=1) */
#define TABLE_2N_MINUS_POPC_2N 1,3,4,7,8,10,11,15,16,18,19,22,23,25,26,31,32,34,35,38,39,41,42,46,47,49,50,53,54,56,57,63,64,66,67,70,71,73,74,78

/* This table x_1, x_2,... contains values s.t. x_n^n has <= GMP_NUMB_BITS bits */
#define NTH_ROOT_NUMB_MASK_TABLE (GMP_NUMB_MASK),CNST_LIMB(0xffffffff),CNST_LIMB(0x285145),CNST_LIMB(0xffff),CNST_LIMB(0x1bdb),CNST_LIMB(0x659),CNST_LIMB(0x235),CNST_LIMB(0xff)

#else

/* This table is 0!,1!,2!/2,3!/2,...,n!/2^sn where n!/2^sn is an */
/* odd integer for each n, and n!/2^sn has <= GMP_NUMB_BITS bits */
#define ONE_LIMB_ODD_FACTORIAL_TABLE CNST_LIMB(0x1),CNST_LIMB(0x1),CNST_LIMB(0x1),CNST_LIMB(0x3),CNST_LIMB(0x3),CNST_LIMB(0xf),CNST_LIMB(0x2d),CNST_LIMB(0x13b),CNST_LIMB(0x13b),CNST_LIMB(0xb13),CNST_LIMB(0x375f),CNST_LIMB(0x26115),CNST_LIMB(0x7233f),CNST_LIMB(0x5cca33),CNST_LIMB(0x2898765),CNST_LIMB(0x260eeeeb),CNST_LIMB(0x260eeeeb)

/* Previous table, continued, values modulo 2^GMP_NUMB_BITS */
#define ONE_LIMB_ODD_FACTORIAL_EXTTABLE CNST_LIMB(0x86fddd9b),CNST_LIMB(0xbeecca73),CNST_LIMB(0x2b930689),CNST_LIMB(0xd9df20ad),CNST_LIMB(0xdf4dae31),CNST_LIMB(0x98567c1b),CNST_LIMB(0xafc5266d),CNST_LIMB(0xf4f7347),CNST_LIMB(0x7ec241ef),CNST_LIMB(0x6fdd5923),CNST_LIMB(0xcc5866b1),CNST_LIMB(0x966aced7),CNST_LIMB(0xa196e5b),CNST_LIMB(0x977d7755),CNST_LIMB(0x5831734b),CNST_LIMB(0x5831734b),CNST_LIMB(0x5e5fdcab),CNST_LIMB(0x445da75b)

/* This table is 1!!,3!!,...,(2n+1)!! where (2n+1)!! has <= GMP_NUMB_BITS bits */
#define ONE_LIMB_ODD_DOUBLEFACTORIAL_TABLE CNST_LIMB(0x1),CNST_LIMB(0x3),CNST_LIMB(0xf),CNST_LIMB(0x69),CNST_LIMB(0x3b1),CNST_LIMB(0x289b),CNST_LIMB(0x20fdf),CNST_LIMB(0x1eee11),CNST_LIMB(0x20dcf21),CNST_LIMB(0x27065f73)

/* This table contains 2n-popc(2n) for small n */

/* It begins with 2-1=1 (n=1) */
#define TABLE_2N_MINUS_POPC_2N 1,3,4,7,8,10,11,15,16,18,19,22,23,25,26,31,32,34,35,38,39,41,42,46

/* This table x_1, x_2,... contains values s.t. x_n^n has <= GMP_NUMB_BITS bits */
#define NTH_ROOT_NUMB_MASK_TABLE (GMP_NUMB_MASK),CNST_LIMB(0xffff),CNST_LIMB(0x659),CNST_LIMB(0xff),CNST_LIMB(0x54),CNST_LIMB(0x28),CNST_LIMB(0x17),CNST_LIMB(0xf)

#endif

/* Entry i contains (i!/2^t) where t is chosen such that the parenthesis
   is an odd integer. */
const mp_limb_t __gmp_oddfac_table[] = { ONE_LIMB_ODD_FACTORIAL_TABLE, ONE_LIMB_ODD_FACTORIAL_EXTTABLE };

/* Entry i contains ((2i+1)!!/2^t) where t is chosen such that the parenthesis
   is an odd integer. */
const mp_limb_t __gmp_odd2fac_table[] = { ONE_LIMB_ODD_DOUBLEFACTORIAL_TABLE };

/* Entry i contains 2i-popc(2i). */
const unsigned char __gmp_fac2cnt_table[] = { TABLE_2N_MINUS_POPC_2N };

const mp_limb_t __gmp_limbroots_table[] = { NTH_ROOT_NUMB_MASK_TABLE };
