dnl  mpn_subadd

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
PROLOGUE(mpn_subadd_n)
push %rbx
xor %r9,%r9
push %rbp
xor %rax,%rax
mov $3,%r10d
sub %r8,%r10
lea -24(%rdi,%r8,8),%rdi
lea -24(%rsi,%r8,8),%rsi
lea -24(%rdx,%r8,8),%rdx
lea -24(%rcx,%r8,8),%rcx
jnc skiplp
.align 16
lp:	sahf
	mov (%rsi,%r10,8),%r8
	sbb (%rcx,%r10,8),%r8
	mov 8(%rsi,%r10,8),%rbx
	sbb 8(%rcx,%r10,8),%rbx
	mov 24(%rsi,%r10,8),%r11
	mov 16(%rsi,%r10,8),%rbp
	sbb 16(%rcx,%r10,8),%rbp
	sbb 24(%rcx,%r10,8),%r11
	lahf
	add $255,%r9b
	sbb (%rdx,%r10,8),%r8
	sbb 8(%rdx,%r10,8),%rbx
	mov %r8,(%rdi,%r10,8)
	sbb 16(%rdx,%r10,8),%rbp
	sbb 24(%rdx,%r10,8),%r11
	setc %r9b
	mov %r11,24(%rdi,%r10,8)
	mov %rbp,16(%rdi,%r10,8)
	mov %rbx,8(%rdi,%r10,8)
	add $4,%r10
	jnc lp
skiplp:
cmp $2,%r10
jg case0
je case1
jp case2
case3:	sahf
	mov (%rsi),%r8
	sbb (%rcx),%r8
	mov 8(%rsi),%rbx
	sbb 8(%rcx),%rbx
	mov 16(%rsi),%rbp
	sbb 16(%rcx),%rbp
	lahf
	add $255,%r9b
	sbb (%rdx),%r8
	sbb 8(%rdx),%rbx
	mov %r8,(%rdi)
	sbb 16(%rdx),%rbp
	setc %r9b
	mov %rbp,16(%rdi)
	mov %rbx,8(%rdi)
	sahf
	mov $0,%eax
	adc $0,%rax
	add $255,%r9b
	adc $0,%rax	
	pop %rbp
	pop %rbx
	ret
case2:	sahf
	mov 8(%rsi),%r8
	sbb 8(%rcx),%r8
	mov 16(%rsi),%rbx
	sbb 16(%rcx),%rbx
	lahf
	add $255,%r9b
	sbb 8(%rdx),%r8
	sbb 16(%rdx),%rbx
	mov %r8,8(%rdi)
	setc %r9b
	mov %rbx,16(%rdi)
	sahf
	mov $0,%eax
	adc $0,%rax
	add $255,%r9b
	adc $0,%rax	
	pop %rbp
	pop %rbx
	ret
case1:	sahf
	mov 16(%rsi),%r8
	sbb 16(%rcx),%r8
	lahf
	add $255,%r9b
	sbb 16(%rdx),%r8
	mov %r8,16(%rdi)
	setc %r9b
case0:	sahf
	mov $0,%eax
	adc $0,%rax
	add $255,%r9b
	adc $0,%rax	
	pop %rbp
	pop %rbx
	ret
EPILOGUE()
