
set(test_support_sources
    memory.c misc.c refmpf.c refmpn.c refmpq.c refmpz.c spinner.c trace.c x86_64check.c x86check.c)

add_library(test-support STATIC ${test_support_sources})
target_link_libraries(test-support PUBLIC mpir)

file(GLOB mpn_tests "mpn/t-*.c")
foreach(test_source IN LISTS mpn_tests)
  message("TEST ${test_source}")
  get_filename_component(name ${test_source} NAME_WE)
  add_executable(${name} ${test_source})
  target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries(${name} PRIVATE test-support)
  add_test(${name} ${name})
endforeach()