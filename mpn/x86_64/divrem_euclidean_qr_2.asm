dnl  mpn_divrem_euclidean_qr_2

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

C	ret mpn_test_ppn(mp_ptr,mp_ptr,mp_size_t)
C	rax                 rdi,   rsi,      rdx

PROLOGUE(mpn_divrem_euclidean_qr_2)
#	rax	MULTEMP
#	rbx	qn
#	rcx	dp
#	rdx	MULTEMP
#	rdi	qp
#	rsi	xp
#	rbp	i
#	r8	l
#	r9	h
#	r10	t[0],xh
#	r11	
#	r12	t1[0]
#	r13	t1[1]
#	r14	qf
#	r15	q,mask,temp
push %rbx
push %rbp
push %r12
push %r13
push %r14
push %r15
mov %rdx,%rbx
dec %rbx
mov $0,%r9
mov 8(%rcx),%rdx
not %rdx
mov $-1,%rax
divq 8(%rcx)
mov %rax,%rbp
mov (%rsi,%rbx,8),%r8
dec %rbx
mov (%rsi,%rbx,8),%r10
cmp 8(%rcx),%r8
jae j2
   mov %r8,%rdx
   mov %r8,%r9
   mov (%rsi,%rbx,8),%r10
   mov %r10,%r8
   mov $0,%r14
   jmp j1
j2:
   mov $1,%r14
   mov %r8,%rdx
   sub 8(%rcx),%rdx
   sub (%rcx),%r10
   sbb $0,%rdx
   jnc j3
      dec %r14
      add (%rcx),%r10
      adc 8(%rcx),%rdx
   j3:
   mov %rdx,%r9
   mov %r10,%r8
j1:
dec %rbx
mov %r9,%rax
js skiplp
ALIGN(16)
lp:    
    cmp 8(%rcx),%rax
    je j4
        mov %rax,%r9
        bt $63,%r8
	adc $0,%rax
	mov %r8,%r15
	mov $0,%r13
	sar $63,%r15
	and 8(%rcx),%r15
	add %r8,%r15
	mul %rbp
	add %r15,%rax
	mov (%rcx),%rax
	adc %r9,%rdx
	mov %rdx,%r10
	mul %rdx
	mov (%rcx),%r12
	add %rax,%r12
	adc %rdx,%r13	
	mov 8(%rcx),%rax
	not %r10
	mul %r10
	sub 8(%rcx),%r9
	add %r8,%rax
	adc %r9,%rdx
	mov %rdx,%r9
	and 8(%rcx),%r9
	# swapped r9,rax from here
	add %r9,%rax
	mov %rdx,%r15
	sub %r10,%r15
	and (%rcx),%rdx
	sub %rdx,%r12
	sbb $0,%r13
	mov (%rsi,%rbx,8),%r8
	sub %r12,%r8
	sbb %r13,%rax
	mov (%rcx),%r10
	mov 8(%rcx),%r11
	sbb %r9,%r9
	and %r9,%r10
	and %r9,%r11
	add %r9,%r15
	add %r10,%r8
	adc %r11,%rax
	adc $0,%r9
	and %r9,%r10
	and %r9,%r11
	add %r9,%r15
	add %r10,%r8
	adc %r11,%rax
    j6:
    mov %r15,(%rdi,%rbx,8)
    dec %rbx
    jns lp
skiplp:
mov %rax,8(%rsi)
mov %r8,(%rsi)
mov %r14,%rax
pop %r15
pop %r14
pop %r13
pop %r12
pop %rbp
pop %rbx
ret
j4:
       mov (%rsi,%rbx,8),%r10
       mov $-1,%r15
       mov %r8,%rdx
       add (%rcx),%r10
       adc %rax,%rdx
       sbb %rax,%rax
       sub (%rcx),%rdx
       adc $0,%rax
       jz j5
 	    dec %r15
            add (%rcx),%r10
	    adc 8(%rcx),%rdx	    
       j5:
       mov %rdx,%rax
       mov %r10,%r8
       jmp j6
EPILOGUE()
