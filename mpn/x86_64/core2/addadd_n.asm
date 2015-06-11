dnl  mpn_addadd

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
PROLOGUE(mpn_addadd_n)
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
lp:     .byte 0x9e      # sahf	
	mov (%rdx,%r10,8),%r8
	adc (%rcx,%r10,8),%r8
	mov 8(%rdx,%r10,8),%rbx
	adc 8(%rcx,%r10,8),%rbx
	mov 24(%rdx,%r10,8),%r11
	mov 16(%rdx,%r10,8),%rbp
	adc 16(%rcx,%r10,8),%rbp
	adc 24(%rcx,%r10,8),%r11
        .byte 0x9f      # lahf	
	add $255,%r9b
	adc (%rsi,%r10,8),%r8
	adc 8(%rsi,%r10,8),%rbx
	mov %r8,(%rdi,%r10,8)
	adc 16(%rsi,%r10,8),%rbp
	adc 24(%rsi,%r10,8),%r11
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
case3:  .byte 0x9e      # sahf	
	mov (%rdx),%r8
	adc (%rcx),%r8
	mov 8(%rdx),%rbx
	adc 8(%rcx),%rbx
	mov 16(%rdx),%rbp
	adc 16(%rcx),%rbp
        .byte 0x9f      # lahf	
	add $255,%r9b
	adc (%rsi),%r8
	adc 8(%rsi),%rbx
	mov %r8,(%rdi)
	adc 16(%rsi),%rbp
	setc %r9b
	mov %rbp,16(%rdi)
	mov %rbx,8(%rdi)
        .byte 0x9e      # sahf	
	mov $0,%eax
	adc $0,%rax
	add $255,%r9b
	adc $0,%rax	
	pop %rbp
	pop %rbx
	ret
case2:  .byte 0x9e      # sahf	
	mov 8(%rdx),%r8
	adc 8(%rcx),%r8
	mov 16(%rdx),%rbx
	adc 16(%rcx),%rbx
        .byte 0x9f      # lahf	
	add $255,%r9b
	adc 8(%rsi),%r8
	adc 16(%rsi),%rbx
	mov %r8,8(%rdi)
	setc %r9b
	mov %rbx,16(%rdi)
        .byte 0x9e      # sahf	
	mov $0,%eax
	adc $0,%rax
	add $255,%r9b
	adc $0,%rax	
	pop %rbp
	pop %rbx
	ret
case1:  .byte 0x9e      # sahf	
	mov 16(%rdx),%r8
	adc 16(%rcx),%r8
        .byte 0x9f      # lahf	
	add $255,%r9b
	adc 16(%rsi),%r8
	mov %r8,16(%rdi)
	setc %r9b
case0:  .byte 0x9e      # sahf	
	mov $0,%eax
	adc $0,%rax
	add $255,%r9b
	adc $0,%rax	
	pop %rbp
	pop %rbx
	ret
EPILOGUE()
