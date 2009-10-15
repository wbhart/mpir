dnl  X86_64 mpn_divrem_hensel_qr_1_2

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

C	(rdi,rdx)=(rsi,rdx) / rcx      rdx>=1
C	rax=hensel remainder from div 

C	This is divrem_hensel_1 with shifting on the output of the quotient
C	On k8/k10 the shifting comes for free so no need to have different
C	fn for that. And on K8/K10 this runs at 10c/l which is optimal
C	This function "replaces" divexact_1 and modexact_1_odd
C	This is same as the shifting version but with  no shifting

ASM_START()
PROLOGUE(mpn_divrem_hensel_qr_1_2)
mov $1,%r9
sub %rdx,%r9
lea -8(%rdi,%rdx,8),%rdi
lea -8(%rsi,%rdx,8),%rsi

push %r12
push %r13
push %r14

mov %rcx,%rdx	
C // rdx is 3 bit inverse

mov %rdx,%rax
imul %ecx,%edx
mov $2,%r11
sub %rdx,%r11
imul %eax,%r11d	
C //r11 has 4 bits

mov %r11,%rax
imul %ecx,%r11d 
mov $2,%rdx
sub %r11,%rdx		
imul %eax,%edx	
C //rdx has 8 bits

mov %rdx,%rax
imul %ecx,%edx
mov $2,%r11
sub %rdx,%r11
imul %eax,%r11d	
C //r11 has 16 bits

mov %r11,%rax
imul %ecx,%r11d 
mov $2,%rdx
sub %r11,%rdx		
imul %eax,%edx	
C // rdx has 32 bits

mov %rdx,%rax
imul %rcx,%rdx
mov $2,%r11
sub %rdx,%r11
imul %rax,%r11	
C //r11 has 64 bits

mov %r11,%rax
mov %r11,%r12
mul %rcx
neg %rdx
imul %rdx,%r12	
C // r12,r11 has 128 bits

mov %r11,%r13
mov %r12,%r14

mov (%rsi,%r9,8),%r11
mov 8(%rsi,%r9,8),%r12
mov $0,%r10
add $2,%r9
jc skiplp
ALIGN(16)
lp:
	mov %r12,%r8
	mov %r13,%rax
	mul %r11
	mov %rax,-16(%rdi,%r9,8)
	imul %r14,%r11
	imul %r13,%r12
	add %r11,%rdx
	add %r12,%rdx
		mov (%rsi,%r9,8),%r11
		mov 8(%rsi,%r9,8),%r12
	mov %rdx,-8(%rdi,%r9,8)
	mov %rcx,%rax
	mul %rdx
		add %r10,%r10
		sbb $0,%r11
		sbb $0,%r12
		sbb %r10,%r10
	cmp %rax,%r8
		sbb %rdx,%r11
		sbb $0,%r12
		sbb $0,%r10
	add $2,%r9
	jnc lp
skiplp:
mov %r12,%r8
mov %r13,%rax	
mul %r11
mov %rax,-16(%rdi,%r9,8)
imul %r14,%r11	
imul %r13,%r12
add %r11,%rdx
add %r12,%rdx
cmp $0,%r9
jne case0
case1:
		mov (%rsi,%r9,8),%r11
	mov %rdx,-8(%rdi,%r9,8)
	mov %rcx,%rax
	mul %rdx
		add %r10,%r10
		sbb $0,%r11
		sbb %r10,%r10
	cmp %rax,%r8
		sbb %rdx,%r11
		sbb $0,%r10
	mov %r11,%rax
	imul %r13,%rax
	mov %rax,(%rdi,%r9,8)
	mul %rcx
	add %r10,%r10
	mov $0,%rax
	adc %rdx,%rax
	pop %r14
	pop %r13
	pop %r12
	ret
case0:
	mov %rdx,-8(%rdi,%r9,8)	
	mov %rcx,%rax	
	mul %rdx	
	cmp %rax,%r8	
	mov $0,%rax
	adc %rdx,%rax
	sub %r10,%rax
	pop %r14
	pop %r13
	pop %r12
	ret
EPILOGUE()
