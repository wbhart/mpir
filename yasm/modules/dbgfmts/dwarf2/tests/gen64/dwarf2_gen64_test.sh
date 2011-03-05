#! /bin/sh
# $Id: dwarf2_gen64_test.sh 2367 2010-09-06 19:33:25Z peter $
${srcdir}/out_test.sh dwarf2_gen64_test modules/dbgfmts/dwarf2/tests/gen64 "dwarf2 dbgfmt gen64" "-f elf64 -g dwarf2" ".o"
exit $?
