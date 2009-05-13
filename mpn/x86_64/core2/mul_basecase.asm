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
C Version 1.0.7


define(`ADDMUL2LP',`
ALIGN(16)
addmul2lp$1:
	mov (%rsi,%rbx,8),%rax
	mul %r8
	add %rax,%r9
	mov 8(%rsi,%rbx,8),%rax
	adc %rdx,%r10
	mov `$'0,%r11
	mul %rcx
	add %r12,(%rdi,%rbx,8)
	adc %rax,%r9
	mov `$'0,%r12
	adc %rdx,%r10
	mov 8(%rsi,%rbx,8),%rax
	adc `$'0,%r11
	mul %r8
	add %r9,8(%rdi,%rbx,8)
	adc %rax,%r10
	adc %rdx,%r11
	mov 16(%rsi,%rbx,8),%rax
	mul %rcx
	add %rax,%r10
	mov 16(%rsi,%rbx,8),%rax
	adc %rdx,%r11
	adc `$'0,%r12
	mul %r8
	add %r10,16(%rdi,%rbx,8)
	mov `$'0,%r9
	adc %rax,%r11
	mov `$'0,%r10
	mov 24(%rsi,%rbx,8),%rax
	adc %rdx,%r12
	mov %r15,%r15
	mul %rcx
	add %rax,%r11
	mov 24(%rsi,%rbx,8),%rax
	adc %rdx,%r12
	adc `$'0,%r9
	mul %r8
	add %r11,24(%rdi,%rbx,8)
	adc %rax,%r12
	adc %rdx,%r9
	mov 32(%rsi,%rbx,8),%rax
	mul %rcx
	add %rax,%r12
	adc %rdx,%r9
	adc `$'0,%r10
	add `$'4,%rbx
	jnc addmul2lp$1
')

define(`ADDMUL2PRO0',`
mov (%r13,%r15,8),%rcx
mul %rcx
mov %rax,%r12
mov %rdx,%r9
mov `$'0,%r10
mov 8(%r13,%r15,8),%r8
')

define(`ADDMUL2EPI0',`
mov %r14,%rbx
mov 24(%rsi),%rax
mul %r8
add %r12,24(%rdi)
adc %rax,%r9
adc %rdx,%r10
add `$'2,%r15
mov (%rsi,%r14,8),%rax
mov %r9,32(%rdi)
lea 16(%rdi),%rdi
mov %r10,24(%rdi)
')

define(`ADDMUL2PRO1',`
mov (%r13,%r15,8),%rcx
mul %rcx
mov %rax,%r12
mov `$'0,%r10
mov %rdx,%r9
mov 8(%r13,%r15,8),%r8
')

define(`ADDMUL2EPI1',`
mov 16(%rsi),%rax
lea 16(%rdi),%rdi
mul %r8
add %rax,%r9
mov 24(%rsi),%rax
mov `$'0,%r11
adc %rdx,%r10
mul %rcx
add %r12,(%rdi)
adc %rax,%r9
adc %rdx,%r10
adc `$'0,%r11
mov 24(%rsi),%rax
mul %r8
add %r9,8(%rdi)
adc %rax,%r10
adc %rdx,%r11
add `$'2,%r15
mov %r14,%rbx
mov (%rsi,%r14,8),%rax
mov %r11,24(%rdi)
mov %r10,16(%rdi)
')

define(`ADDMUL2PRO2',`
mov (%r13,%r15,8),%rcx
mul %rcx
mov `$'0,%r10
mov %rax,%r12
mov %rdx,%r9
mov 8(%r13,%r15,8),%r8
')

define(`ADDMUL2EPI2',`
mov 8(%rsi),%rax
lea 16(%rdi),%rdi
mul %r8
add %rax,%r9
mov 16(%rsi),%rax
adc %rdx,%r10
mov `$'0,%r11
mul %rcx
add %r12,-8(%rdi)
adc %rax,%r9
mov `$'0,%r12
adc %rdx,%r10
mov 16(%rsi),%rax
adc `$'0,%r11
mul %r8
add %r9,(%rdi)
adc %rax,%r10
adc %rdx,%r11
mov 24(%rsi),%rax
mul %rcx
add %rax,%r10
mov 24(%rsi),%rax
adc %rdx,%r11
adc `$'0,%r12
mul %r8
add %r10,8(%rdi)
adc %rax,%r11
adc %rdx,%r12
mov (%rsi,%r14,8),%rax
mov %r11,16(%rdi)
mov %r12,24(%rdi)
add `$'2,%r15
mov %r14,%rbx
')

define(`ADDMUL2PRO3',`
mov (%r13,%r15,8),%rcx
mul %rcx
mov %rax,%r12
mov %rdx,%r9
mov 8(%r13,%r15,8),%r8
mov `$'0,%r10
')

define(`ADDMUL2EPI3',`
mov (%rsi),%rax
lea 16(%rdi),%rdi
mul %r8
add %rax,%r9
mov 8(%rsi),%rax
adc %rdx,%r10
mov `$'0,%r11
mul %rcx
add %r12,-16(%rdi)
adc %rax,%r9
mov `$'0,%r12
adc %rdx,%r10
mov 8(%rsi),%rax
adc `$'0,%r11
mul %r8
add %r9,-8(%rdi)
adc %rax,%r10
adc %rdx,%r11
mov 16(%rsi),%rax
mul %rcx
add %rax,%r10
mov 16(%rsi),%rax
adc %rdx,%r11
adc `$'0,%r12
mul %r8
add %r10,(%rdi)
mov `$'0,%r9
adc %rax,%r11
mov `$'0,%r10
mov 24(%rsi),%rax
adc %rdx,%r12
mov %r15,%r15
mul %rcx
add %rax,%r11
mov 24(%rsi),%rax
adc %rdx,%r12
adc `$'0,%r9
mul %r8
add %r11,8(%rdi)
adc %rax,%r12
adc %rdx,%r9
mov (%rsi,%r14,8),%rax
mov %r12,16(%rdi)
mov %r9,24(%rdi)
add `$'2,%r15
mov %r14,%rbx
')

define(`MUL2LP',`
ALIGN(16)
mul2lp$1:
	mov (%rsi,%rbx,8),%rax
	mul %r8
	add %rax,%r9
	mov 8(%rsi,%rbx,8),%rax
	adc %rdx,%r10
	mov `$'0,%r11
	mul %rcx
	mov %r12,(%rdi,%rbx,8)
	add %rax,%r9
	mov `$'0,%r12
	adc %rdx,%r10
	mov 8(%rsi,%rbx,8),%rax
	adc `$'0,%r11
	mul %r8
	mov %r9,8(%rdi,%rbx,8)
	add %rax,%r10
	adc %rdx,%r11
	mov 16(%rsi,%rbx,8),%rax
	mul %rcx
	add %rax,%r10
	mov 16(%rsi,%rbx,8),%rax
	adc %rdx,%r11
	adc `$'0,%r12
	mul %r8
	mov %r10,16(%rdi,%rbx,8)
	mov `$'0,%r9
	add %rax,%r11
	mov `$'0,%r10
	mov 24(%rsi,%rbx,8),%rax
	adc %rdx,%r12
	mov %r15,%r15
	mul %rcx
	add %rax,%r11
	mov 24(%rsi,%rbx,8),%rax
	adc %rdx,%r12
	adc `$'0,%r9
	mul %r8
	mov %r11,24(%rdi,%rbx,8)
	add %rax,%r12
	adc %rdx,%r9
	mov 32(%rsi,%rbx,8),%rax
	mul %rcx
	add %rax,%r12
	adc %rdx,%r9
	adc `$'0,%r10
	add `$'4,%rbx
	jnc mul2lp$1
')

define(`MUL2PRO0',`
mov (%r13,%r15,8),%rcx
mul %rcx
mov %rax,%r12
mov %rdx,%r9
mov `$'0,%r10
mov 8(%r13,%r15,8),%r8
')

define(`MUL2EPI0',`
mov %r14,%rbx
mov 24(%rsi),%rax
mul %r8
mov %r12,24(%rdi)
add %rax,%r9
adc %rdx,%r10
add `$'2,%r15
mov (%rsi,%r14,8),%rax
mov %r9,32(%rdi)
lea 16(%rdi),%rdi
mov %r10,24(%rdi)
')

define(`MUL2PRO1',`
mov (%r13,%r15,8),%rcx
mul %rcx
mov %rax,%r12
mov `$'0,%r10
mov %rdx,%r9
mov 8(%r13,%r15,8),%r8
')

define(`MUL2EPI1',`
mov 16(%rsi),%rax
lea 16(%rdi),%rdi
mul %r8
add %rax,%r9
mov 24(%rsi),%rax
mov `$'0,%r11
adc %rdx,%r10
mul %rcx
mov %r12,(%rdi)
add %rax,%r9
adc %rdx,%r10
adc `$'0,%r11
mov 24(%rsi),%rax
mul %r8
mov %r9,8(%rdi)
add %rax,%r10
adc %rdx,%r11
add `$'2,%r15
mov %r14,%rbx
mov (%rsi,%r14,8),%rax
mov %r11,24(%rdi)
mov %r10,16(%rdi)
')

define(`MUL2PRO2',`
mov (%r13,%r15,8),%rcx
mul %rcx
mov `$'0,%r10
mov %rax,%r12
mov %rdx,%r9
mov 8(%r13,%r15,8),%r8
')

define(`MUL2EPI2',`
mov 8(%rsi),%rax
lea 16(%rdi),%rdi
mul %r8
add %rax,%r9
mov 16(%rsi),%rax
adc %rdx,%r10
mov `$'0,%r11
mul %rcx
mov %r12,-8(%rdi)
add %rax,%r9
mov `$'0,%r12
adc %rdx,%r10
mov 16(%rsi),%rax
adc `$'0,%r11
mul %r8
mov %r9,(%rdi)
add %rax,%r10
adc %rdx,%r11
mov 24(%rsi),%rax
mul %rcx
add %rax,%r10
mov 24(%rsi),%rax
adc %rdx,%r11
adc `$'0,%r12
mul %r8
mov %r10,8(%rdi)
add %rax,%r11
adc %rdx,%r12
mov (%rsi,%r14,8),%rax
mov %r11,16(%rdi)
mov %r12,24(%rdi)
add `$'2,%r15
mov %r14,%rbx
')

define(`MUL2PRO3',`
mov (%r13,%r15,8),%rcx
mul %rcx
mov %rax,%r12
mov %rdx,%r9
mov 8(%r13,%r15,8),%r8
mov `$'0,%r10
')

define(`MUL2EPI3',`
mov (%rsi),%rax
lea 16(%rdi),%rdi
mul %r8
add %rax,%r9
mov 8(%rsi),%rax
adc %rdx,%r10
mov `$'0,%r11
mul %rcx
mov %r12,-16(%rdi)
add %rax,%r9
mov `$'0,%r12
adc %rdx,%r10
mov 8(%rsi),%rax
adc `$'0,%r11
mul %r8
mov %r9,-8(%rdi)
add %rax,%r10
adc %rdx,%r11
mov 16(%rsi),%rax
mul %rcx
add %rax,%r10
mov 16(%rsi),%rax
adc %rdx,%r11
adc `$'0,%r12
mul %r8
mov %r10,(%rdi)
mov `$'0,%r9
add %rax,%r11
mov `$'0,%r10
mov 24(%rsi),%rax
adc %rdx,%r12
mov %r15,%r15
mul %rcx
add %rax,%r11
mov 24(%rsi),%rax
adc %rdx,%r12
adc `$'0,%r9
mul %r8
mov %r11,8(%rdi)
add %rax,%r12
adc %rdx,%r9
mov (%rsi,%r14,8),%rax
mov %r12,16(%rdi)
mov %r9,24(%rdi)
add `$'2,%r15
mov %r14,%rbx
')

define(`MUL1LP',`
ALIGN(16)
mul1lp:
	mov `$'0,%r10
	mul %r8
	mov %r12,-8(%rdi,%rbx,8)
	add %rax,%r9
	.byte 0x26
	adc %rdx,%r10
	mov 8(%rsi,%rbx,8),%rax
	mul %r8
	mov %r9,(%rdi,%rbx,8)
	add %rax,%r10
	mov `$'0,%r11d
	adc %rdx,%r11
	mov 16(%rsi,%rbx,8),%rax
	mov `$'0,%r12
	mov `$'0,%r9
	mul %r8
	mov %r10,8(%rdi,%rbx,8)
	.byte 0x26
	add %rax,%r11
	.byte 0x26
	adc %rdx,%r12
	mov 24(%rsi,%rbx,8),%rax
 	mul %r8
	mov %r11,16(%rdi,%rbx,8)
	.byte 0x26
	add %rax,%r12
	.byte 0x26
	adc %rdx,%r9
	add `$'4,%rbx
	mov (%rsi,%rbx,8),%rax
	jnc mul1lp
')

# rbx is 0
define(`MULNEXT0',`
	mov 8(%rsi),%rax
	mul %r8
	mov %r9,(%rdi)
	add %rax,%r10
	mov `$'0,%r11d
	adc %rdx,%r11
	mov 16(%rsi),%rax
	mov `$'0,%r12d
	mul %r8
	mov %r10,8(%rdi)
	add %rax,%r11
	adc %rdx,%r12
	mov 24(%rsi),%rax
	mul %r8
	mov %r11,16(%rdi)
	add %rax,%r12
	adc `$'0,%rdx
	mov %r12,24(%rdi)
	mov (%rsi,%r14,8),%rax
	mov %rdx,32(%rdi)
	inc %r15
	lea 8(%rdi),%rdi
	mov %r14,%rbx
')

# rbx is 1
define(`MULNEXT1',`
	mov 16(%rsi),%rax
	mul %r8
	mov %r9,8(%rdi)
	add %rax,%r10
	mov `$'0,%r12d
	adc %rdx,%r12
	mov 24(%rsi),%rax
	mul %r8
	mov %r10,16(%rdi)
	add %rax,%r12
	adc `$'0,%rdx
	mov %r12,24(%rdi)
	mov %rdx,32(%rdi)
	inc %r15
	lea 8(%rdi),%rdi
	mov %r14,%rbx
	mov (%rsi,%r14,8),%rax
')

# rbx is 2
define(`MULNEXT2',`
	mov 24(%rsi),%rax
	mul %r8
	mov %r9,16(%rdi)
	add %rax,%r10
	mov `$'0,%r11d
	adc %rdx,%r11
	mov %r10,24(%rdi)
	mov %r11,32(%rdi)
	inc %r15
	lea 8(%rdi),%rdi
	mov (%rsi,%r14,8),%rax
	mov %r14,%rbx
')

# rbx is 3
define(`MULNEXT3',`
	mov %r9,24(%rdi)
	mov %r10,32(%rdi)
	inc %r15
	lea 8(%rdi),%rdi
	mov (%rsi,%r14,8),%rax
	mov %r14,%rbx
')

define(`MPN_ADDMUL_2_INT',`
ALIGN(16)
loopaddmul$1:
ADDMUL2PRO$1
ADDMUL2LP($1)
ADDMUL2EPI$1
jnz loopaddmul$1
end$1:
mov -8(%rsp),%r13
mov -16(%rsp),%r14
mov -24(%rsp),%rbx
mov -32(%rsp),%r12
mov -40(%rsp),%r15
ret
')

define(`OLDMULLOOP',`
ALIGN(16)
oldmulloop:
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
	jnc oldmulloop
')

define(`OLDMULNEXT0',`
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
	mov (%rsi,%r14,8),%rax
	mov %rdx,40(%rdi,%r11,8)
	inc %r8	
	mov %r14,%r11
')

define(`OLDMULNEXT1',`
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
	mov %r14,%r11
	mov (%rsi,%r14,8),%rax
')

define(`OLDMULNEXT2',`
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

define(`OLDMULNEXT3',`
	mov %r9,8(%rdi,%r11,8)
	mov %r10,16(%rdi,%r11,8)
	inc %r8
	mov (%rsi,%r14,8),%rax
	mov %r14,%r11
')

C changes from standard addmul
C change  r8 to r12   and rcx to r13
C reemove ret and write last limb
define(`OLDADDMULLOOP',`
ALIGN(16)
oldaddmulloop$1:
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
	jnc oldaddmulloop$1
')

define(`OLDADDMULPRO0',`
mov (%rcx,%r8,8),%r13
.byte 0x26
mul %r13
.byte 0x26
mov %rax,%r12
mov 8(%rsi,%r14,8),%rax
.byte 0x26
mov %rdx,%r9
.byte 0x26
cmp `$'0,%r14
lea 8(%rdi),%rdi
')

define(`OLDADDMULNEXT0',`
mov `$'0,%r10d
mul %r13
add %r12,(%rdi)
adc %rax,%r9
adc %rdx,%r10
mov 16(%rsi),%rax
mul %r13
add %r9,8(%rdi)
adc %rax,%r10
mov `$'0,%ebx
adc %rdx,%rbx
mov 24(%rsi),%rax
mov `$'0,%r12d
mov %r14,%r11	
mul %r13
add %r10,16(%rdi)
adc %rax,%rbx
adc %rdx,%r12
mov 32(%rsi),%rax
mul %r13
add %rbx,24(%rdi)
adc %rax,%r12
adc `$'0,%rdx
add %r12,32(%rdi)
mov (%rsi,%r14,8),%rax
adc `$'0,%rdx
inc %r8
mov %rdx,40(%rdi)
')

define(`OLDADDMULPRO1',`
mov (%rcx,%r8,8),%r13
mul %r13
mov %rax,%r12
mov 8(%rsi,%r14,8),%rax
mov %rdx,%r9
cmp `$'0,%r14
')

define(`OLDADDMULNEXT1',`
mov `$'0,%r10d
mul %r13
add %r12,8(%rdi)
adc %rax,%r9
adc %rdx,%r10
mov 24(%rsi),%rax
mul %r13
lea 8(%rdi),%rdi
add %r9,16-8(%rdi)
adc %rax,%r10
mov `$'0,%r12d
mov 32(%rsi),%rax
adc %rdx,%r12
mov %r14,%r11	
mul %r13
add %r10,24-8(%rdi)
adc %rax,%r12
adc `$'0,%rdx
add %r12,32-8(%rdi)
adc `$'0,%rdx
mov %rdx,40-8(%rdi)
inc %r8
mov (%rsi,%r14,8),%rax
')

define(`OLDADDMULPRO2',`
mov (%rcx,%r8,8),%r13
lea 8(%rdi),%rdi
mul %r13
mov %rax,%r12
mov 8(%rsi,%r14,8),%rax
mov %rdx,%r9
cmp `$'0,%r14
')

define(`OLDADDMULNEXT2',`
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

define(`OLDADDMULPRO3',`
mov (%rcx,%r8,8),%r13
.byte 0x26
mul %r13
.byte 0x26
mov %rax,%r12
.byte 0x26
lea 8(%rdi),%rdi
.byte 0x26
mov %rdx,%r9
mov 8(%rsi,%r14,8),%rax
cmp `$'0,%r14
')

define(`OLDADDMULNEXT3',`
mov %r14,%r11	
mul %r13
add %r12,24(%rdi)
adc %rax,%r9
adc `$'0,%rdx
add %r9,32(%rdi)
mov (%rsi,%r14,8),%rax
adc `$'0,%rdx
inc %r8
mov %rdx,40(%rdi)
')

define(`OLDMPN_MULADDMUL_1_INT',`
OLDMULNEXT$1
jz oldend$1
ALIGN(16)
oldloopaddmul$1:
OLDADDMULPRO$1
jge oldaddmulskiploop$1
OLDADDMULLOOP($1)
oldaddmulskiploop$1:
OLDADDMULNEXT$1
jnz oldloopaddmul$1
oldend$1:
mov -8(%rsp),%r13
mov -16(%rsp),%r14
mov -24(%rsp),%rbx
mov -32(%rsp),%r12
ret
')

ASM_START()
PROLOGUE(mpn_mul_basecase)
# the current mul does not handle case one 
cmp $4,%rdx
jg fiveormore
cmp $1,%rdx
je one
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
jge oldmulskiploop
OLDMULLOOP
oldmulskiploop:
mov $0,%r10d
mul %r13
mov %r12,(%rdi,%r11,8)
add %rax,%r9
adc %rdx,%r10
cmp $2,%r11
ja oldcase3
jz oldcase2
jp oldcase1
oldcase0:
OLDMPN_MULADDMUL_1_INT(0)
ALIGN(16)
oldcase1:
OLDMPN_MULADDMUL_1_INT(1)
ALIGN(16)
oldcase2:
OLDMPN_MULADDMUL_1_INT(2)
ALIGN(16)
oldcase3:
OLDMPN_MULADDMUL_1_INT(3)
ALIGN(16)
fiveormore:
# rdx >= 5  as we dont have an inner jump
#  (rdi,rdx+r8)=(rsi,rdx)*(rcx,r8)
mov %r13,-8(%rsp)
mov %r14,-16(%rsp)
mov %rbx,-24(%rsp)
mov %r12,-32(%rsp)
mov %r15,-40(%rsp)
mov $4,%r14
sub %rdx,%r14
lea -32(%rdi,%rdx,8),%rdi
lea -32(%rsi,%rdx,8),%rsi
mov %rcx,%r13
mov %r8,%r15
lea (%r13,%r15,8),%r13
neg %r15
mov %r14,%rbx
mov (%rsi,%r14,8),%rax
bt $0,%r15
jnc even
odd:
	inc %rbx
	mov (%r13,%r15,8),%r8
	mul %r8
	mov %rax,%r12
	mov 8(%rsi,%r14,8),%rax
	mov %rdx,%r9
	cmp $0,%rbx
	jge mulskiploop
	MUL1LP
	mulskiploop:
	mov $0,%r10d
	mul %r8
	mov %r12,-8(%rdi,%rbx,8)
	add %rax,%r9
	adc %rdx,%r10
	cmp $2,%rbx
	ja mul1case3
	jz mul1case2
	jp mul1case1
	mul1case0:
	MULNEXT0
	jmp case0
	mul1case1:
	MULNEXT1
	jmp case3
	mul1case2:
	MULNEXT2
	jmp case2
	mul1case3:
	MULNEXT3
	jmp case1
even:
	# as all the mul2pro? are the same
	MUL2PRO0
	MUL2LP
	cmp $2,%rbx
	ja mul2case0
	jz mul2case1
	jp mul2case2
	mul2case3:
	MUL2EPI3
	case3:
	jz end3
	MPN_ADDMUL_2_INT(3)
	mul2case2:
	MUL2EPI2
	case2:
	jz end2
	MPN_ADDMUL_2_INT(2)
	mul2case1:
	MUL2EPI1
	case1:
	jz end1
	MPN_ADDMUL_2_INT(1)
	mul2case0:
	MUL2EPI0
	case0:
	jz end0
	MPN_ADDMUL_2_INT(0)
ALIGN(16)
one:
	mov (%rsi),%rax
	mulq (%rcx)
	mov %rax,(%rdi)
	mov %rdx,8(%rdi)
	ret
EPILOGUE()

