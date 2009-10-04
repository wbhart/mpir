dnl  X86_64 mpn_rsh_divrem_hensel_qr_1_2

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

C	(rdi,rdx)=( (rsi,rdx)-r9 / rcx ) >> r8    rdx>=1
C	rax=hensel remainder from div 

C	This is divrem_hensel_1_2 with shifting on the output of the quotient

ASM_START()
PROLOGUE(mpn_rsh_divrem_hensel_qr_1_2)
#// 3limb minimum for the mo
mov %r9,%r10
mov $2,%r9
sub %rdx,%r9
lea -16(%rdi,%rdx,8),%rdi
lea -16(%rsi,%rdx,8),%rsi	#// last use of rdx

push %r12
push %r13
push %r14

mov %rcx,%rdx	#// rdx is 3 bit inverse

mov $64,%rax
sub %r8,%rax
movq %r8,%mm0
movq %rax,%mm1

mov %rdx,%rax
imul %ecx,%edx
mov $2,%r11
sub %rdx,%r11
imul %eax,%r11d	#//r11 has 4 bits

mov %r11,%rax
imul %ecx,%r11d 
mov $2,%rdx
sub %r11,%rdx		
imul %eax,%edx	#//rdx has 8 bits

mov %rdx,%rax
imul %ecx,%edx
mov $2,%r11
sub %rdx,%r11
imul %eax,%r11d	#//r11 has 16 bits

mov %r11,%rax
imul %ecx,%r11d 
mov $2,%rdx
sub %r11,%rdx		
imul %eax,%edx	#// rdx has 32 bits

mov %rdx,%rax
imul %rcx,%rdx
mov $2,%r11
sub %rdx,%r11
imul %rax,%r11	#//r11 has 64 bits

mov %r11,%rax
mov %r11,%r12
mul %rcx
neg %rdx
imul %rdx,%r12	#// r12,r11 has 128 bits

#// for the first limb we can not store (as we have to shift) so we need to
#// do first limb separately , we could do it as normal as an extention of
#// the loop , but if we do it as a 1 limb inverse then we can start it
#// eailer , ie interleave it with the calculation of the 2limb inverse

mov %r11,%r13
mov %r12,%r14


mov (%rsi,%r9,8),%r11
sub %r10,%r11
sbb %r10,%r10

imul %r13,%r11
movq %r11,%mm2
psrlq %mm0,%mm2
mov %rcx,%rax
mul %r11
mov 8(%rsi,%r9,8),%r11
mov 16(%rsi,%r9,8),%r12
add %r10,%r10
sbb %rdx,%r11
sbb $0,%r12
sbb %r10,%r10


#mov $0,%r10
add $2,%r9
jc skiplp
ALIGN(16)
lp:
	mov %r12,%r8
	mov %r13,%rax		#// r13 low inverse
	mul %r11

	#mov %rax,-16(%rdi,%r9,8)	#// store low quotient
	movq %rax,%mm3
	movq %mm3,%mm4
	psllq %mm1,%mm3
	psrlq %mm0,%mm4
	por %mm3,%mm2
	movq %mm2,-16(%rdi,%r9,8)

	imul %r14,%r11		#// r14 high inverse
	imul %r13,%r12
	add %r11,%rdx
	add %r12,%rdx
		mov 8(%rsi,%r9,8),%r11
		mov 16(%rsi,%r9,8),%r12

	#mov %rdx,-8(%rdi,%r9,8)	#// store high quotient
	movq %rdx,%mm3
	movq %mm3,%mm2
	psllq %mm1,%mm3
	psrlq %mm0,%mm2
	por %mm3,%mm4
	movq %mm4,-8(%rdi,%r9,8)

	mov %rcx,%rax		#// rcx is divisor
	mul %rdx		#// (*) divisor*high quotient
		add %r10,%r10
		sbb $0,%r11
		sbb $0,%r12
		sbb %r10,%r10
	cmp %rax,%r8		#// (*) and corrrection
		sbb %rdx,%r11	#// (*) to give our "carry" not including old r10
		sbb $0,%r12
		sbb $0,%r10
	add $2,%r9
	jnc lp
skiplp:
mov %r12,%r8
mov %r13,%rax		#// r13 low inverse
mul %r11

#mov %rax,-16(%rdi,%r9,8)	#// store low quotient
movq %rax,%mm3
movq %mm3,%mm4
psllq %mm1,%mm3
psrlq %mm0,%mm4
por %mm3,%mm2
movq %mm2,-16(%rdi,%r9,8)

imul %r14,%r11		#// r14 high inverse
imul %r13,%r12
add %r11,%rdx
add %r12,%rdx
cmp $0,%r9
jne case0
case1:
		mov 8(%rsi,%r9,8),%r11
	#mov %rdx,-8(%rdi,%r9,8)	#// store high quotient
	movq %rdx,%mm3
	movq %mm3,%mm2
	psllq %mm1,%mm3
	psrlq %mm0,%mm2
	por %mm3,%mm4
	movq %mm4,-8(%rdi,%r9,8)

	mov %rcx,%rax		#// rcx is divisor
	mul %rdx		#// (*) divisor*high quotient
		add %r10,%r10
		sbb $0,%r11
		sbb %r10,%r10
	cmp %rax,%r8		#// (*) and corrrection
		sbb %rdx,%r11	#// (*) to give our "carry" not including old r10
		sbb $0,%r10
	mov %r11,%rax
	imul %r13,%rax
	#mov %rax,(%rdi,%r9,8)
	movq %rax,%mm3
	movq %mm3,%mm4
	psllq %mm1,%mm3
	psrlq %mm0,%mm4
	por %mm3,%mm2
	movq %mm2,(%rdi,%r9,8)
	movq %mm4,8(%rdi,%r9,8)

	mul %rcx
	add %r10,%r10
	mov $0,%rax
	adc %rdx,%rax
	pop %r14
	pop %r13
	pop %r12
	emms
	ret
case0:
	#mov %rdx,-8(%rdi,%r9,8)	#// store high quotient
	movq %rdx,%mm3
	movq %mm3,%mm2
	psllq %mm1,%mm3
	psrlq %mm0,%mm2
	por %mm3,%mm4
	movq %mm4,-8(%rdi,%r9,8)
	movq %mm2,(%rdi,%r9,8)

	mov %rcx,%rax		#// rcx is divisor
	mul %rdx		#// (*) divisor*high quotient
	cmp %rax,%r8		#// (*) and corrrection
	mov $0,%rax
	adc %rdx,%rax		#// (*) to give our "carry" not including old r10
	sub %r10,%rax
	pop %r14
	pop %r13
	pop %r12
	emms
	ret
EPILOGUE()
