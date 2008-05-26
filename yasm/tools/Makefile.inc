# $Id: Makefile.inc 1929 2007-09-07 22:05:33Z peter $

EXTRA_DIST += tools/re2c/Makefile.inc
EXTRA_DIST += tools/genperf/Makefile.inc
EXTRA_DIST += tools/python-yasm/Makefile.inc

include tools/re2c/Makefile.inc
include tools/genperf/Makefile.inc
include tools/python-yasm/Makefile.inc
