dnl  mpn_store

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
PROLOGUE(mpn_store)
mov %rsi,%rcx
and $7,%rsi
mov $8,%r8
sub %rsi,%r8		#// r8=8-n%8
lea (%r8,%r8,4),%r8	
lea lp(%rip),%rax	#// rax is lpjmp
add %r8,%rax		#// rax=5*(8-n%8)+lp
and $-8,%rcx
add $48,%rdi
jmp  *%rax
ALIGN(16)
lp:	mov %rdx,8(%rdi,%rcx,8)		#// 5 bytes	// n%8=0 NOT HERE
	mov %rdx, (%rdi,%rcx,8)		#		// n%8=7	// 5 bytes with nop
	nop
	mov %rdx, -8(%rdi,%rcx,8)	#		// n%8=6
	mov %rdx,-16(%rdi,%rcx,8)	#		// n%8=5
	mov %rdx,-24(%rdi,%rcx,8)	#		// n%8=4
	mov %rdx,-32(%rdi,%rcx,8)	#		// n%8=3
	mov %rdx,-40(%rdi,%rcx,8)	#		// n%8=2
	mov %rdx,-48(%rdi,%rcx,8)	#		// n%8=1
	nop				#		// n%8=0
	sub $8,%rcx
	jnc lp				#// 2 bytes
nop
ret
EPILOGUE()
