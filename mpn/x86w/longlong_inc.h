
#ifdef _MSC_VER

#  include <intrin.h>

#  define COUNT_LEADING_ZEROS_NEED_CLZ_TAB

#  if !defined( _WIN64 )

#    pragma intrinsic(_BitScanForward)
#    pragma intrinsic(_BitScanReverse)
#    pragma intrinsic(__emulu)
#    pragma intrinsic(_byteswap_ulong)

#    define count_leading_zeros(c,x)        \
      do { unsigned long _z;		    	\
        ASSERT ((x) != 0);                  \
        _BitScanReverse(&_z, (x));          \
        c = 31 - _z;                        \
      } while (0)
#    define count_trailing_zeros(c,x)       \
      do { unsigned long _z;		    	\
        ASSERT ((x) != 0);                  \
        _BitScanForward(&_z, (x));          \
		c = _z;								\
      } while (0)
#    define umul_ppmm(xh, xl, m0, m1)       \
      do { unsigned __int64 _t;             \
        _t = __emulu( (m0), (m1));          \
        xl = _t & 0xffffffff;               \
        xh = _t >> 32;                      \
      } while (0)

#    if !defined( BSWAP_LIMB )
#      define BSWAP_LIMB
#      define BSWAP_LIMB(dst, src)  dst = _byteswap_ulong(src)
#    endif

#  endif    /* _WIN64 */

#endif      /* _MSC_VER */
