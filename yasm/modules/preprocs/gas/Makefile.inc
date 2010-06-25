# $Id: Makefile.inc 2268 2010-01-07 06:55:03Z peter $

libyasm_a_SOURCES += modules/preprocs/gas/gas-preproc.c
libyasm_a_SOURCES += modules/preprocs/gas/gas-eval.h
libyasm_a_SOURCES += modules/preprocs/gas/gas-eval.c

YASM_MODULES += preproc_gas

EXTRA_DIST += modules/preprocs/gas/tests/Makefile.inc

include modules/preprocs/gas/tests/Makefile.inc
