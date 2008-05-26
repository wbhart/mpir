# $Id: Makefile.inc 1183 2004-12-01 07:49:18Z peter $

libyasm_a_SOURCES += modules/objfmts/bin/bin-objfmt.c

YASM_MODULES += objfmt_bin

EXTRA_DIST += modules/objfmts/bin/tests/Makefile.inc

include modules/objfmts/bin/tests/Makefile.inc
