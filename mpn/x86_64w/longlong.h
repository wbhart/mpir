#ifdef _MSC_VER
#  include <intrin.h>

#define COUNT_LEADING_ZEROS_NEED_CLZ_TAB

#  if defined( _WIN64 )

#pragma intrinsic(_BitScanForward64)
#pragma intrinsic(_BitScanReverse64)
#pragma intrinsic(_umul128)

#    define count_leading_zeros(c,x)        \
      do { unsigned long _z;		    	\
        ASSERT ((x) != 0);                  \
        _BitScanReverse64(&_z, (x));        \
        c = 63 - _z;                        \
      } while (0)
#    define count_trailing_zeros(c,x)       \
      do { unsigned long _z;		    	\
        ASSERT ((x) != 0);                  \
        _BitScanForward64(&_z, (x));        \
		c = _z;								\
      } while (0)
#    define umul_ppmm(xh, xl, m0, m1)       \
      do {                                  \
        xl = _umul128( (m0), (m1), &xh);    \
      } while (0)
#  endif

#endif

/* We need to put the the gcc inline asm for MinGW64 here as well */

