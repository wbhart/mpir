dnl  AMD64 mpn_sqr_basecase

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

C	(rdi,2*rdx)=(rsi,rdx)^2
C Version 1.0.5

C same as the addmul for now
C changes from standard mul
C change  r8 to r12   and rcx to r13
C reemove ret and write last limb
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
	inc %r14
	lea 8(%rdi),%rdi
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
	inc %r14
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
	inc %r14
	lea 8(%rdi),%rdi
')

define(`MULNEXT3',`
	mov %r9,8(%rdi,%r11,8)
	mov %r10,16(%rdi,%r11,8)
	inc %r14
	lea 8(%rdi),%rdi
')

C changes from standard addmul
C change  r8 to r12   and rcx to r13
C reemove ret and write last limb
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
adc `$'0,%rdx
inc %r14
mov %rdx,40(%rdi,%r11,8)
lea 8(%rdi),%rdi
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
inc %r14
lea 8(%rdi),%rdi
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
add %r10,16(%rdi,%r11,8)
adc `$'0,%rbx
mov %rbx,24(%rdi,%r11,8)
inc %r14
lea 8(%rdi),%rdi
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
inc %r14
lea 8(%rdi),%rdi
cmp `$'4,%r14
jnz theloop
')


ASM_START()
PROLOGUE(mpn_sqr_basecase)
cmp $3,%rdx
ja fourormore
jz three
jp two
one:
	mov (%rsi),%rax
	mul %rax
	mov %rax,(%rdi)
	mov %rdx,8(%rdi)
	ret
ALIGN(16)
fourormore:
C this code can not handle cases 3,2,1
mov %r12,-8(%rsp)
mov %r13,-16(%rsp)
mov %r14,-24(%rsp)
mov %rbx,-32(%rsp)
C save data for later
mov %rdi,-40(%rsp)
mov %rsi,-48(%rsp)
mov %rdx,-56(%rsp)
mov (%rsi),%r13
mov 8(%rsi),%rax
mov $6,%r14d
sub %rdx,%r14
lea -40(%rdi,%rdx,8),%rdi
lea -40(%rsi,%rdx,8),%rsi
mov %r14,%r11	
mul %r13
mov %rax,%r12
mov 8(%rsi,%r14,8),%rax
mov %rdx,%r9
cmp `$'0,%r14
jge mulskiploop$1
MULLOOP($1)
mulskiploop$1:
mov $0,%r10d
mul %r13
mov %r12,(%rdi,%r11,8)
add %rax,%r9
adc %rdx,%r10
C could save r9 here 
C could update here ie lea 8(rdi),rdi and inc r14 
cmp $2,%r11
je mcase2
ja mcase3
jp mcase1
mcase0:
MULNEXT0
jmp case1
ALIGN(16)
mcase1:
MULNEXT1
jmp case2
ALIGN(16)
mcase2:
MULNEXT2
jmp case3
ALIGN(16)
mcase3:
MULNEXT3
C jmp case0 just fall thru 
ALIGN(16)
theloop:
case0:
mov (%rsi,%r14,8),%rax
mov -8(%rsi,%r14,8),%r13
mov %r14,%r11	
mul %r13
mov %rax,%r12
mov 8(%rsi,%r14,8),%rax
mov %rdx,%r9
cmp $0,%r14
jge addmulskiploop0
ADDMULLOOP(0)
addmulskiploop0:
ADDMULNEXT0
case1:
mov (%rsi,%r14,8),%rax
mov -8(%rsi,%r14,8),%r13
mov %r14,%r11	
mul %r13
mov %rax,%r12
mov 8(%rsi,%r14,8),%rax
mov %rdx,%r9
cmp $0,%r14
jge addmulskiploop1
ADDMULLOOP(1)
addmulskiploop1:
ADDMULNEXT1
case2:
mov (%rsi,%r14,8),%rax
mov -8(%rsi,%r14,8),%r13
mov %r14,%r11	
mul %r13
mov %rax,%r12
mov 8(%rsi,%r14,8),%rax
mov %rdx,%r9
cmp $0,%r14
jge addmulskiploop2
ADDMULLOOP(2)
addmulskiploop2:
ADDMULNEXT2
case3:
mov (%rsi,%r14,8),%rax
mov -8(%rsi,%r14,8),%r13
mov %r14,%r11	
mul %r13
mov %rax,%r12
mov 8(%rsi,%r14,8),%rax
mov %rdx,%r9
cmp $0,%r14
jge addmulskiploop3
ADDMULLOOP(3)
addmulskiploop3:
ADDMULNEXT3
C only need to add one more line here
mov (%rsi,%r14,8),%rax
mov -8(%rsi,%r14,8),%r13
mul %r13
add %rax,(%rdi,%r14,8)
adc $0,%rdx
mov %rdx,8(%rdi,%r14,8)
C now lsh by 1 and add in the diagonal
mov -40(%rsp),%rdi
mov -48(%rsp),%rsi
mov -56(%rsp),%rcx
mov -8(%rsp),%r12
mov -16(%rsp),%r13
xor %rbx,%rbx
xor %r11,%r11
lea (%rsi,%rcx,8),%rsi
mov %r11,(%rdi)
lea (%rdi,%rcx,8),%r10
mov %r11,-8(%r10,%rcx,8)
neg %rcx
ALIGN(16)
dialoop:
	mov (%rsi,%rcx,8),%rax
	mul %rax
	mov (%rdi),%r8
	mov 8(%rdi),%r9	
	add $1,%rbx
	adc %r8,%r8
	adc %r9,%r9
	sbb %rbx,%rbx	
	add $1,%r11
	adc %rax,%r8
	adc %rdx,%r9
	sbb %r11,%r11	
	mov %r8,(%rdi)
	mov %r9,8(%rdi)
	inc %rcx
	lea 16(%rdi),%rdi
	jnz dialoop
mov -32(%rsp),%rbx
mov -24(%rsp),%r14
ret
ALIGN(16)
two:
	mov (%rsi),%rax
	mov 8(%rsi),%r9
	mov %rax,%r8
	mul %rax
	mov %rax,(%rdi)
	mov %r9,%rax
	mov %rdx,8(%rdi)
	mul %rax
	mov %rax,16(%rdi)
	mov %r8,%rax
	mov %rdx,%r10
	mul %r9
	add %rax,%rax
	adc %rdx,%rdx
	adc $0,%r10
	add %rax,8(%rdi)
	adc %rdx,16(%rdi)
	adc $0,%r10
	mov %r10,24(%rdi)
	ret
ALIGN(16)
three:
	mov (%rsi),%r8
	mov 8(%rsi),%rax
	mul %r8
	mov $0,%r11d
	mov %rax,8(%rdi)
	mov 16(%rsi),%rax
	mov %rdx,%r9
	mul %r8
	mov 8(%rsi),%r8
	add %rax,%r9
	mov 16(%rsi),%rax
	adc %rdx,%r11
	mul %r8
	mov %r9,16(%rdi)
	add %rax,%r11
	mov $0,%r9d
	mov %r11,24(%rdi)
	adc %rdx,%r9
	mov %r9,32(%rdi)
        mov $-3,%rcx
	xor %r10,%r10
	xor %r11,%r11
	lea 24(%rsi),%rsi
	mov %r11,(%rdi)
	mov %r11,40(%rdi)
	dialoop1:
		mov (%rsi,%rcx,8),%rax
		mul %rax
		mov (%rdi),%r8
		mov 8(%rdi),%r9	
		add $1,%r10
		adc %r8,%r8
		adc %r9,%r9
		sbb %r10,%r10	
		add $1,%r11
		adc %rax,%r8
		adc %rdx,%r9
		sbb %r11,%r11	
		mov %r8,(%rdi)
		mov %r9,8(%rdi)
		inc %rcx
		lea 16(%rdi),%rdi
		jnz dialoop1
	nop
	ret
EPILOGUE()
