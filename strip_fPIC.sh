#!/bin/sh
#
# This file was found distributed in numerous places on the 
# web, including, but not limited to NetBSD, which is covered 
# by the three clause BSD license. No accompanying copyright 
# information was found in it. Since the file has been so
# widely distributed without a copyright notice, I assume
# the file is actually in the public domain. I have modified
# it considerably since and hereby place my modifications
# in the public domain.
#
# libtool assumes that the compiler can handle the -fPIC flag
# This isn't always true (for example, yasm can't handle it)
command=""
while [ $# -gt 0 ]; do
   case "$1" in
		-DPIC)
			command="$command -D PIC"
			;;
		-fPIC)
			;;
		-fno-common)
			;;
		*)
			command="$command $1"
			;;
		esac
		shift
done
echo $command
exec $command
