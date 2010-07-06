
#if GMP_LIMB_BITS == 32 && GMP_NAIL_BITS == 0

/* Non-zero bit indicates a quadratic residue mod 0x100.
   This test identifies 82.81% as non-squares (212/256). */
static const mp_limb_t
sq_res_0x100[8] = {
  CNST_LIMB(0x2030213),
  CNST_LIMB(0x2020212),
  CNST_LIMB(0x2020213),
  CNST_LIMB(0x2020212),
  CNST_LIMB(0x2030212),
  CNST_LIMB(0x2020212),
  CNST_LIMB(0x2020212),
  CNST_LIMB(0x2020212),
};

/* 2^24-1 = 3^2 * 5 * 7 * 13 * 17 ... */
#define PERFSQR_MOD_BITS  25

/* This test identifies 95.66% as non-squares. */
#define PERFSQR_MOD_TEST(up, usize) \
  do {                              \
    mp_limb_t  r;                   \
    PERFSQR_MOD_34 (r, up, usize);  \
                                    \
    /* 73.33% */                    \
    PERFSQR_MOD_2 (r, CNST_LIMB(45), CNST_LIMB(0xfa4fa5), \
                   CNST_LIMB(0x920), CNST_LIMB(0x1a442481)); \
                                    \
    /* 47.06% */                    \
    PERFSQR_MOD_1 (r, CNST_LIMB(17), CNST_LIMB(0xf0f0f1), \
                   CNST_LIMB(0x1a317)); \
                                    \
    /* 46.15% */                    \
    PERFSQR_MOD_1 (r, CNST_LIMB(13), CNST_LIMB(0xec4ec5), \
                   CNST_LIMB(0x9e5)); \
                                    \
    /* 42.86% */                    \
    PERFSQR_MOD_1 (r, CNST_LIMB( 7), CNST_LIMB(0xdb6db7), \
                   CNST_LIMB(0x69)); \
  } while (0)

/* Grand total sq_res_0x100 and PERFSQR_MOD_TEST, 99.25% non-squares. */

/* helper for tests/mpz/t-perfsqr.c */
#define PERFSQR_DIVISORS  { 256, 45, 17, 13, 7, }

#elif GMP_LIMB_BITS == 64 && GMP_NAIL_BITS == 0

/* Non-zero bit indicates a quadratic residue mod 0x100.
   This test identifies 82.81% as non-squares (212/256). */
static const mp_limb_t
sq_res_0x100[4] = {
  CNST_LIMB(0x202021202030213),
  CNST_LIMB(0x202021202020213),
  CNST_LIMB(0x202021202030212),
  CNST_LIMB(0x202021202020212),
};

/* 2^48-1 = 3^2 * 5 * 7 * 13 * 17 * 97 ... */
#define PERFSQR_MOD_BITS  49

/* This test identifies 97.81% as non-squares. */
#define PERFSQR_MOD_TEST(up, usize) \
  do {                              \
    mp_limb_t  r;                   \
    PERFSQR_MOD_34 (r, up, usize);  \
                                    \
    /* 69.23% */                    \
    PERFSQR_MOD_2 (r, CNST_LIMB(91), CNST_LIMB(0xfd2fd2fd2fd3), \
                   CNST_LIMB(0x2191240), CNST_LIMB(0x8850a206953820e1)); \
                                    \
    /* 68.24% */                    \
    PERFSQR_MOD_2 (r, CNST_LIMB(85), CNST_LIMB(0xfcfcfcfcfcfd), \
                   CNST_LIMB(0x82158), CNST_LIMB(0x10b48c4b4206a105)); \
                                    \
    /* 55.56% */                    \
    PERFSQR_MOD_1 (r, CNST_LIMB( 9), CNST_LIMB(0xe38e38e38e39), \
                   CNST_LIMB(0x93)); \
                                    \
    /* 49.48% */                    \
    PERFSQR_MOD_2 (r, CNST_LIMB(97), CNST_LIMB(0xfd5c5f02a3a1), \
                   CNST_LIMB(0x1eb628b47), CNST_LIMB(0x6067981b8b451b5f)); \
  } while (0)

/* Grand total sq_res_0x100 and PERFSQR_MOD_TEST, 99.62% non-squares. */

/* helper for tests/mpz/t-perfsqr.c */
#define PERFSQR_DIVISORS  { 256, 91, 85, 9, 97, }

#else
#error no data available for this limb size in perfsqr.h
#endif
