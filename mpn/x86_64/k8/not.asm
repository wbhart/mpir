dnl  mpn_not

dnl  Copyright 2011 The Code Cavern

dnl  This file is part of the MPIR Library.

dnl  The MPIR Library is free software; you can redistribute it and/or modify
dnl  it under the terms of the GNU Lesser General Public License as published
dnl  by the Free Software Foundation; either version 2.1 of the License, or (at
dnl  your option) any later version.

dnl  The MPIR Library is distributed in the hope that it will be useful, but
dnl  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
dnl  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
dnl  License for more details.

dnl  You should have received a copy of the GNU Lesser General Public License
dnl  along with the MPIR Library; see the file COPYING.LIB.  If not, write
dnl  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
dnl  Boston, MA 02110-1301, USA.

include(`../config.m4')

ASM_START()
PROLOGUE(mpn_not)
mov $1,%ecx
lea -8(%rdi,%rsi,8),%rdi
sub %rsi,%rcx
jnc skiplp
ALIGN(16)
lp:	notq (%rdi,%rcx,8)
	notq 8(%rdi,%rcx,8)
	add $2,%rcx
	jnc lp
skiplp:
jnz case0
case1:	notq (%rdi,%rcx,8)
case0:	ret
EPILOGUE()
