dnl  mpn_popcount

dnl  Copyright 2010 The Code Cavern

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
PROLOGUE(mpn_popcount)
xor %eax,%eax
popcnt -8(%rdi,%rsi,8),%r9
sub $3,%rsi
jc skiplp
ALIGN(16)
lp:	popcnt 8(%rdi,%rsi,8),%r8
	add %r9,%rax
	add %r8,%rax
	popcnt (%rdi,%rsi,8),%r9
	sub $2,%rsi
	jnc lp
skiplp:
jnp case0
case1:	popcnt (%rdi),%r8
	add %r8,%rax
case0:	add %r9,%rax
	ret
EPILOGUE()
