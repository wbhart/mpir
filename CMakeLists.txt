cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0048 NEW)
project(MPIR VERSION 3.0.0)


option(MPIR_GMP "Enable GMP compatibility and generate gmp.h header" OFF)
option(MPIR_TESTS "Enable MPIR tests" OFF)


include(CheckTypeSize)
include(CheckSymbolExists)
set(CMAKE_REQUIRED_DEFINITIONS -D__GMP_WITHIN_CONFIGURE=1)
set(CMAKE_REQUIRED_INCLUDES "${CMAKE_SOURCE_DIR}")
set(CMAKE_EXTRA_INCLUDE_FILES gmp-h.in stdint.h)
check_type_size("unsigned long" SIZEOF_UNSIGNED_LONG)
check_type_size("mp_limb_t" SIZEOF_MP_LIMB_T)

math(EXPR BITS_PER_MP_LIMB "${SIZEOF_MP_LIMB_T} * 8")
set(GMP_NAIL_BITS "0")
set(DEFN_LONG_LONG_LIMB "/* #undef _LONG_LONG_LIMB */")
set(LIBGMP_DLL "0")
configure_file(gmp-h.in mpir.h)

if(MPIR_GMP)
  # TODO: I guess we can copy mpir.h instead of configuring it again.
  configure_file(gmp-h.in gmp.h)
endif()

check_type_size(intmax_t SIZEOF_INTMAX_T)
check_symbol_exists(strnlen string.h HAVE_STRNLEN)
check_symbol_exists(strchr string.h HAVE_STRCHR)
string(COMPARE NOTEQUAL "${SIZEOF_INTMAX_T}" "" HAVE_INTMAX_T)
check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(unistd.h HAVE_UNISTD_H)
check_include_file(stdarg.h HAVE_STDARG)
set(WANT_TMP_ALLOCA "1")
message("VERSION: ${PROJECT_VERSION}")
set(VERSION "${PROJECT_VERSION}")
configure_file(config.h.in config.h)

configure_file(gmp-mparam.h.in gmp-mparam.h)

configure_file(longlong.h.in longlong.h)


file(GLOB FFT_SOURCES "fft/*.c")
file(GLOB MPF_SOURCES "mpf/*.c")

file(GLOB MPN_SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "mpn/generic/*.c")
list(REMOVE_ITEM MPN_SOURCES "mpn/generic/udiv_w_sdiv.c")

file(GLOB MPQ_SOURCES "mpq/*.c")
file(GLOB MPZ_SOURCES "mpz/*.c")
file(GLOB PRINTF_SOURCES "printf/*.c")
file(GLOB SCANF_SOURCES "scanf/*.c")
set(SOURCES assert.c compat.c errno.c invalid.c memory.c mp_bpl.c mp_clz_tab.c mp_dv_tab.c mp_get_fns.c mp_minv_tab.c
mp_set_fns.c nextprime.c primesieve.c randbui.c randclr.c randdef.c randiset.c randlc2s.c randlc2x.c randmt.c randmts.c
randmui.c rands.c randsd.c randsdui.c tal-reent.c version.c)

add_library(
    mpir
    ${SOURCES}
    ${FFT_SOURCES}
    ${MPF_SOURCES}
    ${MPN_SOURCES}
    ${MPQ_SOURCES}
    ${MPZ_SOURCES}
    ${PRINTF_SOURCES}
    ${SCANF_SOURCES}
)
target_include_directories(mpir PUBLIC ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR})


if(MPIR_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

include(GNUInstallDirs)
install(FILES ${CMAKE_BINARY_DIR}/mpir.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
if(MPIR_GMP)
  install(FILES ${CMAKE_BINARY_DIR}/gmp.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()
install(TARGETS mpir ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
