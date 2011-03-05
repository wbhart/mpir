# $Id: Makefile.inc 2367 2010-09-06 19:33:25Z peter $

EXTRA_DIST += modules/dbgfmts/dwarf2/tests/gen64/Makefile.inc
EXTRA_DIST += modules/dbgfmts/dwarf2/tests/pass32/Makefile.inc
EXTRA_DIST += modules/dbgfmts/dwarf2/tests/pass64/Makefile.inc
EXTRA_DIST += modules/dbgfmts/dwarf2/tests/passwin64/Makefile.inc
include modules/dbgfmts/dwarf2/tests/gen64/Makefile.inc
include modules/dbgfmts/dwarf2/tests/pass32/Makefile.inc
include modules/dbgfmts/dwarf2/tests/pass64/Makefile.inc
include modules/dbgfmts/dwarf2/tests/passwin64/Makefile.inc

