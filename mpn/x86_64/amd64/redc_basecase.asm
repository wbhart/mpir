dnl  AMD64 mpn_redc_basecase

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

C Version 1.0.4

C	(rdi,rcx)=(rsi,rcx)+(rdx,rcx)   with the carry flag set for the carry
C this is the usual mpn_add_n with the final dec rax;adc rax,rax;ret  removed 
C and a jump where we have two rets
define(`MPN_ADD',`
mov	%rcx,%rax
and	`$'3,%rax
shr	`$'2,%rcx
cmp	`$'0,%rcx	
C carry flag is clear here
jnz	addloop
mov	(%rsi),%r11
add	(%rdx),%r11
mov	%r11,(%rdi)
dec	%rax
jz	addend
mov	8(%rsi),%r11
adc	8(%rdx),%r11
mov	%r11,8(%rdi)
dec	%rax
jz	addend
mov	16(%rsi),%r11
adc	16(%rdx),%r11
mov	%r11,16(%rdi)
jmp addend
ALIGN(16)
addloop:
	mov	(%rsi),%r11
	mov	8(%rsi),%r8
	lea	32(%rsi),%rsi
	adc	(%rdx),%r11
	adc	8(%rdx),%r8
	lea	32(%rdx),%rdx
	mov	%r11,(%rdi)
	mov	%r8,8(%rdi)
	lea	32(%rdi),%rdi
	mov	-16(%rsi),%r9
	mov	-8(%rsi),%r10
	adc	-16(%rdx),%r9
	adc	-8(%rdx),%r10
	mov	%r9,-16(%rdi)
	dec	%rcx
	mov	%r10,-8(%rdi)
	jnz	addloop
inc	%rax
dec	%rax
jz	addend
mov	(%rsi),%r11
adc	(%rdx),%r11
mov	%r11,(%rdi)
dec	%rax
jz	addend
mov	8(%rsi),%r11
adc	8(%rdx),%r11
mov	%r11,8(%rdi)
dec	%rax
jz	addend
mov	16(%rsi),%r11
adc	16(%rdx),%r11
mov	%r11,16(%rdi)
addend:
')

C (rbx,rbp)=(rsi,rbp)-(rdx,rbp)
define(`MPN_SUB',`
mov	%rbp,%rax
and	`$'3,%rax
shr	`$'2,%rbp
cmp	`$'0,%rbp	
C  carry flag is clear here
jnz	subloop
mov	(%rsi),%r11
sub	(%rdx),%r11
mov	%r11,(%rbx)
dec	%rax
jz	subend
mov	8(%rsi),%r11
sbb	8(%rdx),%r11
mov	%r11,8(%rbx)
dec	%rax
jz	subend
mov	16(%rsi),%r11
sbb	16(%rdx),%r11
mov	%r11,16(%rbx)
jmp subend
ALIGN(16)
subloop:
	mov	(%rsi),%r11
	mov	8(%rsi),%r8
	lea	32(%rsi),%rsi
	sbb	(%rdx),%r11
	sbb	8(%rdx),%r8
	lea	32(%rdx),%rdx
	mov	%r11,(%rbx)
	mov	%r8,8(%rbx)
	lea	32(%rbx),%rbx
	mov	-16(%rsi),%r9
	mov	-8(%rsi),%r10
	sbb	-16(%rdx),%r9
	sbb	-8(%rdx),%r10
	mov	%r9,-16(%rbx)
	dec	%rbp
	mov	%r10,-8(%rbx)
	jnz	subloop
inc	%rax
dec	%rax
jz	subend
mov	(%rsi),%r11
sbb	(%rdx),%r11
mov	%r11,(%rbx)
dec	%rax
jz	subend
mov	8(%rsi),%r11
sbb	8(%rdx),%r11
mov	%r11,8(%rbx)
dec	%rax
jz	subend
mov	16(%rsi),%r11
sbb	16(%rdx),%r11
mov	%r11,16(%rbx)
subend:
')

C changes from standard addmul
C change  r8 to r12   and rcx to r13   and rdi to r8
C reemove ret and write last limb but to beginning
define(`ADDMULLOOP',`
ALIGN(16)
addmulloop$1:
	mov `$'0,%r10
	mul %r13
	add %r12,(%r8,%r11,8)
	adc %rax,%r9
	.byte 0x26
	adc %rdx,%r10
	mov 16(%rsi,%r11,8),%rax
	mul %r13
	add %r9,8(%r8,%r11,8)
	adc %rax,%r10
	mov `$'0,%ebx
	adc %rdx,%rbx
	mov 24(%rsi,%r11,8),%rax
	mov `$'0,%r12
	mov `$'0,%r9
	mul %r13
	add %r10,16(%r8,%r11,8)
	.byte 0x26
	adc %rax,%rbx
	.byte 0x26
	adc %rdx,%r12
	mov 32(%rsi,%r11,8),%rax
 	mul %r13
	add %rbx,24(%r8,%r11,8)
	.byte 0x26
	adc %rax,%r12
	.byte 0x26
	adc %rdx,%r9
	add `$'4,%r11
	mov 8(%rsi,%r11,8),%rax
	jnc addmulloop$1
')

define(`ADDMULPROPRO0',`
imul %rcx,%r13
lea -8(%r8),%r8
')

define(`ADDMULPRO0',`
mov %r14,%r11	
lea 8(%r8),%r8
mov (%rsi,%r14,8),%rax
mul %r13
mov %rax,%r12
mov 8(%rsi,%r14,8),%rax
mov %rdx,%r9
cmp `$'0,%r14
')

define(`ADDMULNEXT0',`
mov `$'0,%r10d
mul %r13
add %r12,(%r8,%r11,8)
adc %rax,%r9
adc %rdx,%r10
mov 16(%rsi,%r11,8),%rax
mul %r13
add %r9,8(%r8,%r11,8)
adc %rax,%r10
mov `$'0,%ebx
adc %rdx,%rbx
mov 24(%rsi,%r11,8),%rax
mov `$'0,%r12d
mov `$'0,%r9d
mul %r13
add %r10,16(%r8,%r11,8)
adc %rax,%rbx
adc %rdx,%r12
mov 32(%rsi,%r11,8),%rax
mul %r13
add %rbx,24(%r8,%r11,8)
mov 8(%r8,%r14,8),%r13
adc %rax,%r12
adc %rdx,%r9
imul %rcx,%r13
add %r12,32(%r8,%r11,8)
adc `$'0,%r9
dec %r15
mov %r9,(%r8,%r14,8)
')

define(`ADDMULPROPRO1',`
')

define(`ADDMULPRO1',`
imul %rcx,%r13
mov (%rsi,%r14,8),%rax
mov %r14,%r11	
mul %r13
mov %rax,%r12
mov 8(%rsi,%r14,8),%rax
mov %rdx,%r9
cmp `$'0,%r14
')

define(`ADDMULNEXT1',`
mov `$'0,%r10d
mul %r13
add %r12,(%r8,%r11,8)
adc %rax,%r9
adc %rdx,%r10
mov 16(%rsi,%r11,8),%rax
mul %r13
add %r9,8(%r8,%r11,8)
adc %rax,%r10
mov `$'0,%ebx
adc %rdx,%rbx
mov 24(%rsi,%r11,8),%rax
mov `$'0,%r12d
mul %r13
add %r10,16(%r8,%r11,8)
adc %rax,%rbx
adc %rdx,%r12
add %rbx,24(%r8,%r11,8)
mov 8(%r8,%r14,8),%r13
adc `$'0,%r12
dec %r15
mov %r12,(%r8,%r14,8)
lea 8(%r8),%r8
')

define(`ADDMULPROPRO2',`
')

define(`ADDMULPRO2',`
imul %rcx,%r13
mov (%rsi,%r14,8),%rax
mov %r14,%r11	
mul %r13
mov %rax,%r12
mov 8(%rsi,%r14,8),%rax
mov %rdx,%r9
cmp `$'0,%r14
')

define(`ADDMULNEXT2',`
mul %r13
add %r12,(%r8,%r11,8)
adc %rax,%r9
mov `$'0,%r10d
adc %rdx,%r10
mov 16(%rsi,%r11,8),%rax
mul %r13
add %r9,8(%r8,%r11,8)
adc %rax,%r10
mov `$'0,%ebx	
adc %rdx,%rbx
mov 8(%r8,%r14,8),%r13
add %r10,16(%r8,%r11,8)
adc `$'0,%rbx
mov %rbx,(%r8,%r14,8)
dec %r15
lea 8(%r8),%r8
')

define(`ADDMULPROPRO3',`
')

define(`ADDMULPRO3',`
imul %rcx,%r13
mov (%rsi,%r14,8),%rax
mov %r14,%r11	
mul %r13
mov %rax,%r12
mov 8(%rsi,%r14,8),%rax
mov %rdx,%r9
cmp `$'0,%r14
')

define(`ADDMULNEXT3',`
mul %r13
add %r12,(%r8,%r11,8)
adc %rax,%r9
mov `$'0,%r10d
adc %rdx,%r10
add %r9,8(%r8,%r11,8)
adc `$'0,%r10
mov 8(%r8,%r14,8),%r13
mov %r10,(%r8,%r14,8)
lea 8(%r8),%r8
dec %r15
')

C change r8 to r12
C write top limb ax straight to mem dont return  (NOTE we WRITE NOT ADD)
define(`MPN_ADDMUL_1_INT',`
ADDMULPROPRO$1
ALIGN(16)
loopaddmul$1:
ADDMULPRO$1
jge addmulskiploop$1
ADDMULLOOP($1)
addmulskiploop$1:
ADDMULNEXT$1
jnz loopaddmul$1
jmp end
')

ASM_START()
PROLOGUE(mpn_redc_basecase)
cmp $1,%rdx
je one
push %r13
push %r14
push %rbx
push %r12
push %r15
push %rbp
mov $5,%r14
sub %rdx,%r14
C store copys
push %rsi
push %r8
lea -40(%r8,%rdx,8),%r8
lea -40(%rsi,%rdx,8),%rsi
mov %rdx,%rbp
mov %rdx,%r15
mov %r14,%rax
and $3,%rax
mov (%r8,%r14,8),%r13
je case0
jp case3
cmp $1,%rax
je case1
case2:
MPN_ADDMUL_1_INT(2)
ALIGN(16)
case0:
MPN_ADDMUL_1_INT(0)
ALIGN(16)
case1:
MPN_ADDMUL_1_INT(1)
ALIGN(16)
case3:
MPN_ADDMUL_1_INT(3)
ALIGN(16)
end:
mov %rbp,%rcx
pop %rdx
lea (%rdx,%rbp,8),%rsi
mov %rdi,%rbx
MPN_ADD()
C     mpnadd(rdi,rsi,rdx,rcx)
pop %rdx
jnc skip
mov %rbx,%rsi
MPN_SUB()
C    mpn_sub_n(rbx,rsi,rdx,rbp) we can certainly improve this sub
skip:
pop %rbp
pop %r15
pop %r12
pop %rbx
pop %r14
pop %r13
ret
ALIGN(16)
one:
	mov (%r8),%r9
	mov (%rsi),%r11
	imul %r9,%rcx
	mov %rcx,%rax
	mul %r11
	add %r9,%rax
	C rax is zero here
	adc 8(%r8),%rdx
	cmovnc %rax,%r11
	sub %r11,%rdx
	mov %rdx,(%rdi)
	ret
EPILOGUE()
