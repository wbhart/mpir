dnl  AMD64 mpn_mul_basecase

dnl  Copyright 2008,2009 Jason Moxham

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

C	(rdi,rdx+r8)=(rsi,rdx)*(rcx,r8)
# Version 1.0.5

# same as the addmul for now
# changes from standard mul
# change  r8 to r12   and rcx to r13
# reemove ret and write last limb
define(`MULLOOP',`
ALIGN(16)
mulloop$1:
	mov `$'0,%r10
	mul %r13
	mov %r12,(%rdi,%r11,8)
	add %rax,%r9
	.byte 0x26
	adc %rdx,%r10
	mov 16(%rsi,%r11,8),%rax
	mul %r13
	mov %r9,8(%rdi,%r11,8)
	add %rax,%r10
	mov `$'0,%ebx
	adc %rdx,%rbx
	mov 24(%rsi,%r11,8),%rax
	mov `$'0,%r12
	mov `$'0,%r9
	mul %r13
	mov %r10,16(%rdi,%r11,8)
	.byte 0x26
	add %rax,%rbx
	.byte 0x26
	adc %rdx,%r12
	mov 32(%rsi,%r11,8),%rax
 	mul %r13
	mov %rbx,24(%rdi,%r11,8)
	.byte 0x26
	add %rax,%r12
	.byte 0x26
	adc %rdx,%r9
	add `$'4,%r11
	mov 8(%rsi,%r11,8),%rax
	jnc mulloop$1
')

define(`MULNEXT0',`
	mov 16(%rsi,%r11,8),%rax
	mul %r13
	mov %r9,8(%rdi,%r11,8)
	add %rax,%r10
	mov `$'0,%ebx
	adc %rdx,%rbx
	mov 24(%rsi,%r11,8),%rax
	mov `$'0,%r12d
	mul %r13
	mov %r10,16(%rdi,%r11,8)
	add %rax,%rbx
	adc %rdx,%r12
	mov 32(%rsi,%r11,8),%rax
	mul %r13
	mov %rbx,24(%rdi,%r11,8)
	add %rax,%r12
	adc `$'0,%rdx
	mov %r12,32(%rdi,%r11,8)
	mov %rdx,40(%rdi,%r11,8)
	inc %r8
	mov (%rsi,%r14,8),%rax
	mov %r14,%r11
')

define(`MULNEXT1',`
	mov 16(%rsi,%r11,8),%rax
	mul %r13
	mov %r9,8(%rdi,%r11,8)
	add %rax,%r10
	mov `$'0,%r12d
	adc %rdx,%r12
	mov 24(%rsi,%r11,8),%rax
	mul %r13
	mov %r10,16(%rdi,%r11,8)
	add %rax,%r12
	adc `$'0,%rdx
	mov %r12,24(%rdi,%r11,8)
	mov %rdx,32(%rdi,%r11,8)
	inc %r8
	lea 8(%rdi),%rdi
')

define(`MULNEXT2',`
	mov 16(%rsi,%r11,8),%rax
	mul %r13
	mov %r9,8(%rdi,%r11,8)
	add %rax,%r10
	mov `$'0,%ebx
	adc %rdx,%rbx
	mov %r10,16(%rdi,%r11,8)
	mov %rbx,24(%rdi,%r11,8)
	inc %r8
	mov (%rsi,%r14,8),%rax
	mov %r14,%r11
')

define(`MULNEXT3',`
	mov %r9,8(%rdi,%r11,8)
	mov %r10,16(%rdi,%r11,8)
	inc %r8
	lea 8(%rdi),%rdi
')

# changes from standard addmul
# change  r8 to r12   and rcx to r13
# reemove ret and write last limb
define(`ADDMULLOOP',`
ALIGN(16)
addmulloop$1:
	mov `$'0,%r10
	mul %r13
	add %r12,(%rdi,%r11,8)
	adc %rax,%r9
	.byte 0x26
	adc %rdx,%r10
	mov 16(%rsi,%r11,8),%rax
	mul %r13
	add %r9,8(%rdi,%r11,8)
	adc %rax,%r10
	mov `$'0,%ebx
	adc %rdx,%rbx
	mov 24(%rsi,%r11,8),%rax
	mov `$'0,%r12
	mov `$'0,%r9
	mul %r13
	add %r10,16(%rdi,%r11,8)
	.byte 0x26
	adc %rax,%rbx
	.byte 0x26
	adc %rdx,%r12
	mov 32(%rsi,%r11,8),%rax
 	mul %r13
	add %rbx,24(%rdi,%r11,8)
	.byte 0x26
	adc %rax,%r12
	.byte 0x26
	adc %rdx,%r9
	add `$'4,%r11
	mov 8(%rsi,%r11,8),%rax
	jnc addmulloop$1
')

define(`ADDMULPRO0',`
#mov (%rsi,%r14,8),%rax
mov (%rcx,%r8,8),%r13
lea 8(%rdi),%rdi
#mov %r14,%r11	
mul %r13
mov %rax,%r12
mov 8(%rsi,%r14,8),%rax
mov %rdx,%r9
cmp `$'0,%r14
')

define(`ADDMULNEXT0',`
mov `$'0,%r10d
mul %r13
add %r12,(%rdi,%r11,8)
adc %rax,%r9
adc %rdx,%r10
	mov 16(%rsi,%r11,8),%rax
	mul %r13
	add %r9,8(%rdi,%r11,8)
	adc %rax,%r10
	mov `$'0,%ebx
	adc %rdx,%rbx
	mov 24(%rsi,%r11,8),%rax
	mov `$'0,%r12d
	mul %r13
	add %r10,16(%rdi,%r11,8)
	adc %rax,%rbx
	adc %rdx,%r12
	mov 32(%rsi,%r11,8),%rax
	mul %r13
	add %rbx,24(%rdi,%r11,8)
	adc %rax,%r12
	adc `$'0,%rdx
	add %r12,32(%rdi,%r11,8)
	mov (%rsi,%r14,8),%rax
	adc `$'0,%rdx
	inc %r8
	mov %rdx,40(%rdi,%r11,8)
	mov %r14,%r11	
	#lea 8(%rdi),%rdi
')

define(`ADDMULPRO1',`
mov (%rsi,%r14,8),%rax
mov (%rcx,%r8,8),%r13
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
add %r12,(%rdi,%r11,8)
adc %rax,%r9
adc %rdx,%r10
	mov 16(%rsi,%r11,8),%rax
	mul %r13
	add %r9,8(%rdi,%r11,8)
	adc %rax,%r10
	mov `$'0,%r12d
	adc %rdx,%r12
	mov 24(%rsi,%r11,8),%rax
	mul %r13
	add %r10,16(%rdi,%r11,8)
	adc %rax,%r12
	adc `$'0,%rdx
	add %r12,24(%rdi,%r11,8)
	adc `$'0,%rdx
	mov %rdx,32(%rdi,%r11,8)
	inc %r8
	lea 8(%rdi),%rdi
')

define(`ADDMULPRO2',`
mov (%rcx,%r8,8),%r13
lea 8(%rdi),%rdi
mul %r13
mov %rax,%r12
mov 8(%rsi,%r14,8),%rax
mov %rdx,%r9
cmp `$'0,%r14
')

define(`ADDMULNEXT2',`
mov `$'0,%r10d
mul %r13
add %r12,(%rdi,%r11,8)
adc %rax,%r9
adc %rdx,%r10
	mov 16(%rsi,%r11,8),%rax
	mul %r13
	mov `$'0,%ebx
	add %r9,8(%rdi,%r11,8)
	adc %rax,%r10
	adc %rdx,%rbx
	mov (%rsi,%r14,8),%rax
	add %r10,16(%rdi,%r11,8)
	adc `$'0,%rbx
	mov %rbx,24(%rdi,%r11,8)
	inc %r8
	mov %r14,%r11	
')

define(`ADDMULPRO3',`
mov (%rsi,%r14,8),%rax
mov (%rcx,%r8,8),%r13
mov %r14,%r11	
mul %r13
mov %rax,%r12
mov 8(%rsi,%r14,8),%rax
mov %rdx,%r9
cmp `$'0,%r14
')

define(`ADDMULNEXT3',`
mul %r13
add %r12,(%rdi,%r11,8)
adc %rax,%r9
mov `$'0,%r10d
adc %rdx,%r10
	add %r9,8(%rdi,%r11,8)
	adc `$'0,%r10
	mov %r10,16(%rdi,%r11,8)
	inc %r8
	lea 8(%rdi),%rdi
')

define(`MPN_MULADDMUL_1_INT',`
MULNEXT$1
jz end$1
ALIGN(16)
loopaddmul$1:
ADDMULPRO$1
jge addmulskiploop$1
ADDMULLOOP($1)
addmulskiploop$1:
ADDMULNEXT$1
jnz loopaddmul$1
end$1:
mov -8(%rsp),%r13
mov -16(%rsp),%r14
mov -24(%rsp),%rbx
mov -32(%rsp),%r12
ret
')

ASM_START()
PROLOGUE(mpn_mul_basecase)
# the current mul does not handle case one 
cmp $2,%rdx
jz two
jb one
mov %r13,-8(%rsp)
mov %r14,-16(%rsp)
mov %rbx,-24(%rsp)
mov %r12,-32(%rsp)
mov $5,%r14
sub %rdx,%r14
lea -40(%rdi,%rdx,8),%rdi
lea (%rcx,%r8,8),%rcx
neg %r8
lea -40(%rsi,%rdx,8),%rsi
mov (%rsi,%r14,8),%rax
mov (%rcx,%r8,8),%r13
mov %r14,%r11	
mul %r13
mov %rax,%r12
mov 8(%rsi,%r14,8),%rax
mov %rdx,%r9
cmp $0,%r14
jge mulskiploop
MULLOOP(1)
mulskiploop:
mov $0,%r10d
mul %r13
mov %r12,(%rdi,%r11,8)
add %rax,%r9
adc %rdx,%r10
cmp $2,%r11
ja case3
jz case2
jp case1
case0:
MPN_MULADDMUL_1_INT(0)
ALIGN(16)
case1:
MPN_MULADDMUL_1_INT(1)
ALIGN(16)
case2:
MPN_MULADDMUL_1_INT(2)
ALIGN(16)
case3:
MPN_MULADDMUL_1_INT(3)
ALIGN(16)
one:
	mov (%rsi),%rax
	mulq (%rcx)
	mov %rax,(%rdi)
	mov %rdx,8(%rdi)
	ret
ALIGN(16)
two:
	mov (%rcx),%r10
	mov (%rsi),%rax
	mul %r10
	mov %rax,(%rdi)
	mov 8(%rsi),%rax
	mov %rdx,%r9
	mul %r10
	add %rax,%r9
	adc $0,%rdx
	mov %r9,8(%rdi)
	mov %rdx,16(%rdi)
	cmp $2,%r8
	jz twobytwo
	ret
twobytwo:
	mov $0,%r9d
	mov 8(%rcx),%r10
	mov (%rsi),%rax
	mul %r10
	add %rax,8(%rdi)
	mov 8(%rsi),%rax
	adc %rdx,%r9
	mul %r10
	add %rax,%r9
	adc $0,%rdx
	add %r9,16(%rdi)
	adc $0,%rdx
	mov %rdx,24(%rdi)
	ret	
EPILOGUE()
