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
#// t[0] is r8		t[1] is r9
#//  l is   r10 		h is r11
#//      divisor is rcx
#// qn is rbx
#// i is bp
#// qf is r12
#// mask,q is r13
push %rbx
push %rbp
push %r12
push %r13
mov %rdx,%rbx
sub $3,%rbx
xor %r11,%r11
mov $-1,%rax
mov 8(%rcx),%rdx
not %rdx
divq 8(%rcx)
mov %rax,%rbp
mov 16(%rsi,%rbx,8),%r10
mov 8(%rsi,%rbx,8),%r8
cmp 8(%rcx),%r10
jae j4
   mov %r10,%r11
   mov %r10,%r9
   mov 8(%rsi,%rbx,8),%r10
   mov %r10,%r8
   mov $0,%r12
   jmp j5
j4:
   mov $1,%r12
   mov %r10,%r9
   sub 8(%rcx),%r9
   sub (%rcx),%r8
   sbb $0,%r9
   jnc j3
      dec %r12
      add (%rcx),%r8
      adc 8(%rcx),%r9
   j3:
   mov %r9,%r11
   mov %r8,%r10
j5:
cmp $0,%rbx
js skiplp
.align 16
lp:
    mov (%rsi,%rbx,8),%r8
    cmp 8(%rcx),%r11
    je j2
       mov %r10,%r13
       sar $63,%r13
       mov %r11,%rax
       sub %r13,%rax
       and 8(%rcx),%r13
       add %r10,%r13
       mul %rbp
       add %r13,%rax
       adc %r11,%rdx
       mov %rdx,%r13
       not %rdx
       mov 8(%rcx),%rax
       mul %rdx
       add %r10,%rax
       adc %r11,%rdx
       sub 8(%rcx),%rdx
       sbb $-1,%r13
       and 8(%rcx),%rdx
       add %rax,%rdx
       mov (%rcx),%rax
       mov %rdx,%r9
       mov %r13,(%rdi,%rbx,8)
       mul %r13
       sub %rax,%r8
       sbb %rdx,%r9
       jnc j1
          decq (%rdi,%rbx,8)
          add (%rcx),%r8
          adc 8(%rcx),%r9
          jc j1
                decq (%rdi,%rbx,8)
                add (%rcx),%r8
                adc 8(%rcx),%r9
                jmp j1
       j2:
       movq $-1,(%rdi,%rbx,8)
       mov %r10,%r9
       add (%rcx),%r8
       adc %r11,%r9
       sbb %rax,%rax
       sub (%rcx),%r9
       adc $0,%rax
       jz j1
          decq (%rdi,%rbx,8)
          add (%rcx),%r8
          adc %r11,%r9
    j1:
    mov %r9,%r11
    mov %r8,%r10
    dec %rbx
    jns lp
skiplp:
mov %r9,8(%rsi)
mov %r8,(%rsi)
mov %r12,%rax
pop %r13
pop %r12
pop %rbp
pop %rbx
ret
EPILOGUE()
