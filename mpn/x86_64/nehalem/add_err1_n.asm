dnl  mpn_add_err1_n

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

C ret mpn_add_err1(mp_ptr rp,mp_ptr up,mp_ptr vp,mp_ptr ep,mp_ptr_t yp,mp_size_t n,mp_limb_t cy)
C rax                    rdi,      rsi,      rdx,      rcx,         r8           r9       8(rsp)=>r10

ASM_START()
PROLOGUE(mpn_add_err1_n)
	C // if we rearrange the params we could save some moves
	C //(rdi,r9)=(rsi,r9)+(rdx,r9)  sum=carry*(r8)
	
	mov 8(%rsp),%r10            C cy
	mov %rbp,-16(%rsp)          C save rbp
	lea -24(%rdi,%r9,8),%rdi    C rp += n - 3
	mov %r12,-24(%rsp)          C save r12
	mov %r13,-32(%rsp)          C save r13
	lea -24(%rsi,%r9,8),%rsi    C up += n - 3
	mov %r14,-40(%rsp)          C save r14
	mov %r15,-48(%rsp)          C save r15
	lea -24(%rdx,%r9,8),%rdx    C vp += n - 3
	mov %rcx,-56(%rsp)	       C save rcx
	mov %rbx,-8(%rsp)           C save rbx
	mov $3,%r11                 C i = 3
	shl $63,%r10
	lea (%r8,%r9,8),%r8         C yp += n
	sub %r9,%r11	              C i = 3 - n
	mov $0,%r9                  C t1 = 0
	mov $0,%rax                 C t2 = 0
	mov $0,%rbx                 C t3 = 0
	jnc skiplp                  C if done goto skiplp
ALIGN(16)
lp:
	mov (%rsi,%r11,8),%r12      C s1 = *(up + i + 0)
	mov 8(%rsi,%r11,8),%r13     C s2 = *(up + i + 1)
	mov 16(%rsi,%r11,8),%r14    C s3 = *(up + i + 2)
	mov 24(%rsi,%r11,8),%r15    C s4 = *(up + i + 3)
	mov $0,%rbp                 C t5 = 0
	shl $1,%r10                 C s1 += *(vp + i + 0) + (cy & 1)
	adc (%rdx,%r11,8),%r12
	cmovc -8(%r8),%rax          C if carry1, t2 = *(yp - 1)
	adc 8(%rdx,%r11,8),%r13     C s2 += *(vp + i + 1) + carry1
	cmovc -16(%r8),%rbx         C if carry2 t3 = *(yp - 2)
	mov $0,%rcx                 C t4 = 0
	adc 16(%rdx,%r11,8),%r14    C s3 += *(vp + i + 2) + carry2
	cmovc -24(%r8),%rcx         C if carry3 t4 = *(yp - 3)
	adc 24(%rdx,%r11,8),%r15    C s4 += *(vp + i + 3) + carry3
	cmovc -32(%r8),%rbp         C if carry4 t5 = *(yp - 4)
	rcr $1,%r10                 C high bit of cy = carry4
	add %rax,%r9                C t1 += t2
	adc $0,%r10                 C accumulate cy
	add %rbx,%r9                C t1 += t2
	adc $0,%r10                 C accumulate cy
	add %rcx,%r9                C t1 += t4
	mov $0,%rax                 C t2 = 0
	adc $0,%r10                 C accumulate cy
	lea -32(%r8),%r8            C yp -= 4
	add %rbp,%r9                C t1 += t5
	adc $0,%r10                 C accumulate cy
	mov %r12,(%rdi,%r11,8)      C *(rp + i + 0) = s1
	mov %r13,8(%rdi,%r11,8)     C *(rp + i + 1) = s2
	mov %r14,16(%rdi,%r11,8)    C *(rp + i + 2) = s3
	mov %r15,24(%rdi,%r11,8)    C *(rp + i + 3) = s4
	mov $0,%rbx                 C t3 = 0
	add $4,%r11                 C i += 4
	jnc  lp                     C not done, goto lp
skiplp:
	cmp $2,%r11                 C cmp(i, 2)
	mov -16(%rsp),%rbp          C restore rbp
	mov -48(%rsp),%r15          C restore r15
	ja case0                    C i == 3 goto case0
	je case1                    C i == 2 goto case1
	jp case2                    C i == 1 goto case2
case3:
	mov (%rsi,%r11,8),%r12         C s1 = *(up + i + 0)
	mov 8(%rsi,%r11,8),%r13        C s2 = *(up + i + 1)
	mov 16(%rsi,%r11,8),%r14       C s3 = *(up + i + 2)
	shl $1,%r10                    C restore carry1 from high bit of t1
	adc (%rdx,%r11,8),%r12         C s1 += *(vp + i + 0) + carry1
	cmovc -8(%r8),%rax             C if carry2 t2 = *(yp - 1)
	adc 8(%rdx,%r11,8),%r13        C s2 += *(vp + i + 1) + carry2
	cmovc -16(%r8),%rbx            C if carry3 t3 = *(yp - 2)
	mov $0,%rcx                    C t4 = 0
	adc 16(%rdx,%r11,8),%r14       C s3 += *(vp + i + 3) + carry3
	cmovc -24(%r8),%rcx            C if carry4 t4 = *(yp - 3)
	rcr $1,%r10                    C store carry4 in high bit of cy
	add %rax,%r9                   C t1 += t2
	adc $0,%r10                    C accumulate cy
	add %rbx,%r9                   C t1 += t3
	adc $0,%r10                    C accumulate cy
	add %rcx,%r9                   C t1 += t4
	adc $0,%r10                    C accumulate cy
	mov %r12,(%rdi,%r11,8)         C *(rp + i + 0) = s1
	mov %r13,8(%rdi,%r11,8)        C *(rp + i + 1) = s2
	mov %r14,16(%rdi,%r11,8)       C *(rp + i + 2) = s3
	mov -56(%rsp),%rcx             C restore rcx
	mov %r9,(%rcx)                 C ep[0] = t1
	btr $63,%r10                   C retrieve carry out and reset bit of cy
	mov %r10,8(%rcx)               C ep[1] = cy
	mov -40(%rsp),%r14             C restore r14
	mov $0,%rax
	mov -32(%rsp),%r13             C restore r13
	adc $0,%rax                    C return carry out
	mov -24(%rsp),%r12             C restore r12
	mov -8(%rsp),%rbx              C restore rbx
	ret
ALIGN(16)
case2:
	mov (%rsi,%r11,8),%r12     C s1 = *(up + i + 0)
	mov 8(%rsi,%r11,8),%r13    C s2 = *(up + i + 1)
	shl $1,%r10                C restore carry1 from high bit of t1
	adc (%rdx,%r11,8),%r12     C s1 += *(vp + i + 0) + carry1
	cmovc -8(%r8),%rax         C if carry2 t2 = *(yp - 1)
	adc 8(%rdx,%r11,8),%r13    C s2 += *(vp + i + 1) + carry2
	cmovc -16(%r8),%rbx        C if carry3 t3 = *(yp - 2)
	rcr $1,%r10                C store carry3 in high bit of cy
	add %rax,%r9               C t1 += t2
	adc $0,%r10                C accumulate cy
	add %rbx,%r9               C t1 += t3
	adc $0,%r10                C accumulate cy
	mov %r12,(%rdi,%r11,8)     C *(rp + i + 0) = s1
	mov %r13,8(%rdi,%r11,8)    C *(rp + i + 1) = s2
	mov -56(%rsp),%rcx         C restore rcx
	mov %r9,(%rcx)             C ep[0] = t1
	btr $63,%r10               C retrieve carry out and reset bit of cy
	mov %r10,8(%rcx)           C ep[1] = cy
	mov -40(%rsp),%r14         C restore r14
	mov $0,%rax
	mov -32(%rsp),%r13         C restore r13
	adc $0,%rax                C return carry out
	mov -24(%rsp),%r12         C restore r12
	mov -8(%rsp),%rbx          C restore rbx
	ret
ALIGN(16)
case1:
	mov (%rsi,%r11,8),%r12     C s1 = *(up + i + 0)
	shl $1,%r10                C restore carry1 from high bit of t1
	adc (%rdx,%r11,8),%r12     C s1 += *(vp + i + 0) + carry1
	cmovc -8(%r8),%rax         C if carry2 t2 = *(yp - 1)
	rcr $1,%r10                C store carry3 in high bit of cy
	add %rax,%r9               C t1 += t2
	adc $0,%r10                C accumulate cy
	mov %r12,(%rdi,%r11,8)     C *(rp + i + 0) = s1
case0:	mov -56(%rsp),%rcx         C restore rcx
	mov %r9,(%rcx)             C ep[0] = t1
	btr $63,%r10               C retrieve carry out and reset bit of cy
	mov %r10,8(%rcx)           C ep[1] = cy
	mov -40(%rsp),%r14         C restore r14
	mov $0,%rax                
	mov -32(%rsp),%r13         C restore r13
	adc $0,%rax                C return carry out
	mov -24(%rsp),%r12         C restore r12
	mov -8(%rsp),%rbx          C restore rbx
	ret
EPILOGUE()
