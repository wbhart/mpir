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
# Version 1.0.4

# The M4 use here is just for clarity , think of them as functions which are
# getting inlined , with the param to differentiate the lables

# changes from standard mpn_mul_1
# change lables
# change r8 to r11
# write top limb ax straight to mem dont return
# change lea offsets to match addmul
# reorder mod4 tail parts so have "only one ret"
# remove ret
# todo --------  could ignore small values ie mulskiploop for dx=1,2,3 we save one branch
# todo ------------- alignment of jumps etc
define(`MPN_MUL_1_INT',`
mov `$'4,%r10d
sub %rdx,%r10
mov `$'0,%r11d
jnc mulskiploop
ALIGN(16)
mulloop:
	mov 16(%rsi,%r10,8),%rax
	mov `$'0,%r9d
	mul %r13
	add %rax,%r11
	mov %r11,16(%rdi,%r10,8)
	mov 24(%rsi,%r10,8),%rax
	adc %rdx,%r9
	mul %r13
	mov `$'0,%r11d
	add %rax,%r9
	mov 32(%rsi,%r10,8),%rax
	adc %rdx,%r11
	mul %r13
	mov %r9,24(%rdi,%r10,8)
	add %rax,%r11
	mov `$'0,%r9d
	mov %r11,32(%rdi,%r10,8)
	mov 40(%rsi,%r10,8),%rax
	mov `$'0,%r11d
	adc %rdx,%r9
	mul %r13
	add %rax,%r9
	mov %r9,40(%rdi,%r10,8)
	adc %rdx,%r11
	add `$'4,%r10
	jnc mulloop
mulskiploop:
test `$'2,%r10
jnz mulskip
	mov 16(%rsi,%r10,8),%rax
	mov `$'0,%r9d
	mul %r13
	add %rax,%r11
	mov %r11,16(%rdi,%r10,8)
	mov 24(%rsi,%r10,8),%rax
	adc %rdx,%r9
	mul %r13
	mov `$'0,%r11d
	add %rax,%r9
	adc %rdx,%r11
	add `$'2,%r10
	mov %r9,8(%rdi,%r10,8)
mulskip:
test `$'1,%r10
jnz mulend
	mov 16(%rsi,%r10,8),%rax
	mov `$'0,%r9d
	mul %r13
	add %rax,%r11
	adc %rdx,%r9
	mov %r9,24(%rdi,%r10,8)
mulend:
	mov %r11,16(%rdi,%r10,8)
')


# changes from standard mpn_addmul_1 internal loop
# change lables
# change r8 to r12   , rcx to r13
define(`ADDMULLOOP',`
ALIGN(16)
addmulloop$1:
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
	mov `$'0,%r9d
	mul %r13
	add %r10,16(%rdi,%r11,8)
	adc %rax,%rbx
	adc %rdx,%r12
	mov 32(%rsi,%r11,8),%rax
 	mul %r13
	add %rbx,24(%rdi,%r11,8)
	adc %rax,%r12
	adc %rdx,%r9
	add `$'4,%r11
	mov 8(%rsi,%r11,8),%rax
	jnc addmulloop$1
')

define(`ADDMULNEXT0',`
#here is 3 loads ie if r11=0
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
	mov `$'0,%r9d
	mul %r13
	add %r10,16(%rdi,%r11,8)
	adc %rax,%rbx
	adc %rdx,%r12
	mov 32(%rsi,%r11,8),%rax
	mul %r13
	add %rbx,24(%rdi,%r11,8)
	adc %rax,%r12
	adc %rdx,%r9
	add %r12,32(%rdi,%r11,8)
	adc `$'0,%r9
	mov %r9,40(%rdi,%r11,8)
')

define(`ADDMULNEXT1',`
#here is 2 loads ie if r11=1
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
	add %rbx,24(%rdi,%r11,8)
	adc `$'0,%r12
	mov %r12,32(%rdi,%r11,8)
')

define(`ADDMULNEXT2',`
#here is 1 load ie if r11=2
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
	add %r10,16(%rdi,%r11,8)
	adc `$'0,%rbx
	mov %rbx,24(%rdi,%r11,8)
')

define(`ADDMULNEXT3',`
#here is 0 loads ie if r11=3
	mov `$'0,%r10d
	mul %r13
	add %r12,(%rdi,%r11,8)
	adc %rax,%r9
	adc %rdx,%r10
	add %r9,8(%rdi,%r11,8)
	adc `$'0,%r10
	mov %r10,16(%rdi,%r11,8)
')

# changes from standard addmul_1
# change lables
# change r8 to r12
# write top limb ax straight to mem dont return  (NOTE we WRITE NOT ADD)
# remove one limb special case
# remove push/pop , basecase must save rbx
# split into mod4 types and remove rets
# surround with outer loop and pop ret
# todo ----------- can ignore small values
# this addmul MUST have a param whic is 0123 which is our r11
define(`MPN_ADDMUL_1_INT',`
loopaddmul$1:
mov (%rsi,%r14,8),%rax
mov -8(%rsi,%r14,8),%r13
mov %r14,%r11	
mul %r13
mov %rax,%r12
mov 8(%rsi,%r14,8),%rax
mov %rdx,%r9
cmp `$'0,%r14
jge addmulskiploop$1
ADDMULLOOP($1)
addmulskiploop$1:
ADDMULNEXT$1
inc %r14
lea 8(%rdi),%rdi
cmp `$'4,%r14
jz theend
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
        mov $3,%ecx
	xor %r10,%r10
	xor %r11,%r11
	lea 24(%rsi),%rsi
	mov %r11,(%rdi)
	mov %r11,40(%rdi)
	neg %rcx
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
fourormore:
# this code can not handle cases 3,2,1
push %r12
push %r13
push %r14
push %rbx
# save 
push %rdi
push %rsi
push %rdx
mov (%rsi),%r13
mov $7,%r14d
sub %rdx,%r14
lea -40(%rdi,%rdx,8),%rdi
lea -40(%rsi,%rdx,8),%rsi
MPN_MUL_1_INT()
lea 8(%rdi),%rdi
mov %r14,%rax
and $3,%rax
je case0
jp case3
cmp $1,%rax
je case1
ALIGN(16)
theloop:
case2:
MPN_ADDMUL_1_INT(2)
case3:
MPN_ADDMUL_1_INT(3)
case0:
MPN_ADDMUL_1_INT(0)
case1:
MPN_ADDMUL_1_INT(1)
jmp theloop
theend:
# only need to add one more line here
mov (%rsi,%r14,8),%rax
mov -8(%rsi,%r14,8),%r13
mul %r13
add %rax,(%rdi,%r14,8)
adc $0,%rdx
mov %rdx,8(%rdi,%r14,8)
# now lsh by 1 and add in the diagonal
pop %rcx
pop %rsi
pop %rdi
endish:
xor %rbx,%rbx
xor %r14,%r14
lea (%rsi,%rcx,8),%rsi
mov %r14,(%rdi)
lea (%rdi,%rcx,8),%r10
mov %r14,-8(%r10,%rcx,8)
neg %rcx
dialoop:
	mov (%rsi,%rcx,8),%rax
	mul %rax
	mov (%rdi),%r8
	mov 8(%rdi),%r9	
	add $1,%rbx
	adc %r8,%r8
	adc %r9,%r9
	sbb %rbx,%rbx	
	add $1,%r14
	adc %rax,%r8
	adc %rdx,%r9
	sbb %r14,%r14	
	mov %r8,(%rdi)
	mov %r9,8(%rdi)
	inc %rcx
	lea 16(%rdi),%rdi
	jnz dialoop
pop %rbx
pop %r14
pop %r13
pop %r12
ret
EPILOGUE()
# if we do the diag first then addmul we could remove the mul and save space
