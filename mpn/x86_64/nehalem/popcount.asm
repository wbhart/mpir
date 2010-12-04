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
mov $5,%rcx
lea -40(%rdi,%rsi,8),%rdi
xor %eax,%eax
sub %rsi,%rcx
jnc skiplp
ALIGN(16)
lp:
	popcnt (%rdi,%rcx,8),%r8
	popcnt 8(%rdi,%rcx,8),%r9
	popcnt 16(%rdi,%rcx,8),%r10
	popcnt 24(%rdi,%rcx,8),%rsi
	add %r8,%rax
	add %rsi,%rax
	add %r9,%rax
	popcnt 32(%rdi,%rcx,8),%r8
	popcnt 40(%rdi,%rcx,8),%r9
	add %r8,%rax
	add %r10,%rax
	add %r9,%rax
	add $6,%rcx
	jnc lp
skiplp:
lea case5(%rip),%rdx	#// in linux we can do this before the loop
lea (%rcx,%rcx,8),%rcx	#// rcx*9
add %rcx,%rdx
jmp *%rdx
case5:	#//rcx=0
	nop
	popcnt (%rdi),%r8	#// 5bytes
	add %r8,%rax		#// 3bytes
case4:	#//rcx=1
	popcnt 8(%rdi),%r9	#// 6bytes
	add %r9,%rax
case3:	popcnt 16(%rdi),%r10
	add %r10,%rax
case2:	popcnt 24(%rdi),%rsi
	add %rsi,%rax
case1:	popcnt 32(%rdi),%r8
	add %r8,%rax
case0:	ret
EPILOGUE()
