dnl  mpn_sumdiff

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
PROLOGUE(mpn_sumdiff_n)
push %rbx
xor %r9,%r9
push %rbp
xor %rax,%rax
push %r12
mov $3,%r10d
push %r13
lea -24(%rdi,%r8,8),%rdi
push %r14
lea -24(%rsi,%r8,8),%rsi
push %r15
sub %r8,%r10
lea -24(%rdx,%r8,8),%rdx
lea -24(%rcx,%r8,8),%rcx
jnc skiplp
.align 16
lp:     .byte 0x9e      # sahf	
	mov (%rdx,%r10,8),%r8
	mov 24(%rdx,%r10,8),%r12
	mov %r8,%r11
	adc (%rcx,%r10,8),%r8
	mov 8(%rdx,%r10,8),%rbx
	mov %rbx,%r13
	adc 8(%rcx,%r10,8),%rbx
	mov 16(%rdx,%r10,8),%rbp
	mov %rbp,%r14
	adc 16(%rcx,%r10,8),%rbp
	mov %r12,%r15
	adc 24(%rcx,%r10,8),%r12
        .byte 0x9f      # lahf	
	add $255,%r9b
	sbb (%rcx,%r10,8),%r11
	mov %r11,(%rsi,%r10,8)
	sbb 8(%rcx,%r10,8),%r13
	sbb 16(%rcx,%r10,8),%r14
	sbb 24(%rcx,%r10,8),%r15
	setc %r9b
	add $4,%r10
	mov %r8,-32(%rdi,%r10,8)
	mov %rbp,16-32(%rdi,%r10,8)
	mov %r13,8-32(%rsi,%r10,8)
	mov %r15,24-32(%rsi,%r10,8)
	mov %r12,24-32(%rdi,%r10,8)
	mov %r14,16-32(%rsi,%r10,8)
	mov %rbx,8-32(%rdi,%r10,8)
	jnc lp
skiplp:
cmp $2,%r10
jg case0
je case1
jp case2
case3:  .byte 0x9e      # sahf	
	mov (%rdx),%r8
	mov %r8,%r11
	adc (%rcx),%r8
	mov 8(%rdx),%rbx
	mov %rbx,%r13
	adc 8(%rcx),%rbx
	mov 16(%rdx),%rbp
	mov %rbp,%r14
	adc 16(%rcx),%rbp
        .byte 0x9f      # lahf	
	add $255,%r9b
	sbb (%rcx),%r11
	mov %r11,(%rsi)
	sbb 8(%rcx),%r13
	sbb 16(%rcx),%r14
	setc %r9b
	mov %r8,(%rdi)
	mov %rbp,16(%rdi)
	mov %r13,8(%rsi)
	mov %r14,16(%rsi)
	mov %rbx,8(%rdi)
        .byte 0x9e      # sahf	
	mov $0,%rax
	adc $0,%rax
	add $255,%r9b
	rcl $1,%rax
	pop %r15
	pop %r14
	pop %r13
	pop %r12
	pop %rbp
	pop %rbx
	ret
case2:  .byte 0x9e      # sahf	
	mov 8(%rdx),%r8
	mov %r8,%r11
	adc 8(%rcx),%r8
	mov 16(%rdx),%rbx
	mov %rbx,%r13
	adc 16(%rcx),%rbx
        .byte 0x9f      # lahf
	add $255,%r9b
	sbb 8(%rcx),%r11
	mov %r11,8(%rsi)
	sbb 16(%rcx),%r13
	setc %r9b
	mov %r8,8(%rdi)
	mov %r13,16(%rsi)
	mov %rbx,16(%rdi)
        .byte 0x9e      # sahf	
	mov $0,%rax
	adc $0,%rax
	add $255,%r9b
	rcl $1,%rax
	pop %r15
	pop %r14
	pop %r13
	pop %r12
	pop %rbp
	pop %rbx
	ret
case1:  .byte 0x9e      # sahf	
	mov 16(%rdx),%r8
	mov %r8,%r11
	adc 16(%rcx),%r8
        .byte 0x9f      # lahf
	add $255,%r9b
	sbb 16(%rcx),%r11
	mov %r11,16(%rsi)
	setc %r9b
	mov %r8,16(%rdi)
case0:  .byte 0x9e      # sahf	
	mov $0,%rax
	adc $0,%rax
	add $255,%r9b
	rcl $1,%rax
	pop %r15
	pop %r14
	pop %r13
	pop %r12
	pop %rbp
	pop %rbx
	ret
EPILOGUE()
