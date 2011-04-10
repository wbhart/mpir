/* This form encourages gcc (pre-release 3.4 at least) to emit predicated
   "sub r=r,r" and "sub r=r,r,1", giving a 2 cycle latency.  The generic
   code using "al<bl" arithmetically comes out making an actual 0 or 1 in a
   register, which takes an extra cycle.  */
#define sub_ddmmss(sh, sl, ah, al, bh, bl)      \
  do {                                          \
    UWtype __x;                                 \
    __x = (al) - (bl);                          \
    if ((al) < (bl))                            \
      (sh) = (ah) - (bh) - 1;                   \
    else                                        \
      (sh) = (ah) - (bh);                       \
    (sl) = __x;                                 \
  } while (0)
#if defined (__GNUC__) && ! defined (__INTEL_COMPILER)
/* Do both product parts in assembly, since that gives better code with
   all gcc versions.  Some callers will just use the upper part, and in
   that situation we waste an instruction, but not any cycles.  */
#define umul_ppmm(ph, pl, m0, m1) \
    __asm__ ("xma.hu %0 = %2, %3, f0\n\txma.l %1 = %2, %3, f0"		\
	     : "=&f" (ph), "=f" (pl)					\
	     : "f" (m0), "f" (m1))
#define UMUL_TIME 14
#define count_leading_zeros(count, x) \
  do {									\
    UWtype _x = (x), _y, _a, _c;					\
    __asm__ ("mux1 %0 = %1, @rev" : "=r" (_y) : "r" (_x));		\
    __asm__ ("czx1.l %0 = %1" : "=r" (_a) : "r" (-_y | _y));		\
    _c = (_a - 1) << 3;							\
    _x >>= _c;								\
    if (_x >= 1 << 4)							\
      _x >>= 4, _c += 4;						\
    if (_x >= 1 << 2)							\
      _x >>= 2, _c += 2;						\
    _c += _x >> 1;							\
    (count) =  W_TYPE_SIZE - 1 - _c;					\
  } while (0)
/* similar to what gcc does for __builtin_ffs, but 0 based rather than 1
   based, and we don't need a special case for x==0 here */
#define count_trailing_zeros(count, x)					\
  do {									\
    UWtype __ctz_x = (x);						\
    __asm__ ("popcnt %0 = %1"						\
	     : "=r" (count)						\
	     : "r" ((__ctz_x-1) & ~__ctz_x));				\
  } while (0)
#endif
#if defined (__INTEL_COMPILER)
#include <ia64intrin.h>
#define umul_ppmm(ph, pl, m0, m1)					\
  do {									\
    UWtype _m0 = (m0), _m1 = (m1);					\
    ph = _m64_xmahu (_m0, _m1, 0);					\
    pl = _m0 * _m1;							\
  } while (0)
#endif
#ifndef LONGLONG_STANDALONE
#define udiv_qrnnd(q, r, n1, n0, d) \
  do { UWtype __di;							\
    __di = __MPN(invert_limb) (d);					\
    udiv_qrnnd_preinv (q, r, n1, n0, d, __di);				\
  } while (0)
#define UDIV_PREINV_ALWAYS  1
#define UDIV_NEEDS_NORMALIZATION 1
#endif
#define UDIV_TIME 220
