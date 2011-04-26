dnl  mpn_mod_1_1

dnl  Copyright 2009 Jason Moxham

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

C	(rdi,2)= not fully reduced remainder of (rsi,rdx) / divisor , and top limb <d
C	where (rcx,2)  contains B^i % divisor


ASM_START()
PROLOGUE(mpn_mod_1_1)
push %r13
mov -8(%rsi,%rdx,8),%r13
mov -16(%rsi,%rdx,8),%rax
mov (%rcx),%r8
mov 8(%rcx),%r9
mov %rdx,%rcx
sub $2,%rcx
ALIGN(16)
lp:
	mov -8(%rsi,%rcx,8),%r10
	mul %r8
	add %rax,%r10
	mov $0,%r11
	adc %rdx,%r11
	mov %r13,%rax
	mul %r9
	add %r10,%rax
	mov %r11,%r13
	adc %rdx,%r13
	dec %rcx
	jnz lp
C // r13,rax
mov %rax,(%rdi)
mov %r8,%rax
mul %r13
add %rax,(%rdi)
adc $0,%rdx
mov %rdx,8(%rdi)
pop %r13
ret
EPILOGUE()



