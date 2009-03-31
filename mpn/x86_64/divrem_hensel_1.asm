dnl  X86_64 mpn_divrem_hensel_1

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

C	(rdi,rdx)=(rsi,rdx) / rcx     inverse is r8
C	rax=remainder

C	This is divrem_hensel_1 with shifting on the output of the quotient
C	On k8/k10 the shifting comes for free so no need to have different
C	fn for that. And on K8/K10 this runs at 10c/l which is optimal
C	This function "replaces" divexact_1 and modexact_1_odd

DONT USE until we have sorted out the return value


ASM_START()
PROLOGUE(mpn_divrem_hensel_1)
mov $1,%r9
sub %rdx,%r9
lea (%rdi,%rdx,8),%rdi
lea (%rsi,%rdx,8),%rsi
mov %rcx,%r10
mov %r8,%r11
mov $64,%rax
bsf %r10,%rcx
sub %rcx,%rax
movq %rcx,%mm0
movq %rax,%mm1
shr %cl,%r10
mov -8(%rsi,%r9,8),%rax
imul %r11,%rax
movq %rax,%mm4
movq %mm4,%mm5
psrlq %mm0,%mm4
psllq %mm1,%mm5
psrlq %mm1,%mm5
mul %r10
mov $1,%r8
# cmp below clears carry
cmp $0,%r9
jz skiploop
ALIGN(16)
loop:
    movq %mm4,%mm2
    mov (%rsi,%r9,8),%rax
    sbb %rdx,%rax
    sbb %r8,%r8
    imul %r11,%rax
    movq %rax,%mm3
    movq %mm3,%mm4
    psllq %mm1,%mm3
    psrlq %mm0,%mm4
    por %mm3,%mm2
    movq %mm2,-8(%rdi,%r9,8)
    mul %r10
    add $1,%r8
    inc %r9
    jnz loop
skiploop:
movq %mm4,-8(%rdi,%r9,8)
movq %mm5,%rax
dec %r8
sub %r8,%rdx
shl %cl,%rdx
add %rdx,%rax
emms
ret
# so   (xp,n) = low_s_bitsof(ret)*d/2^s + (qp,n)*(d) - (ret>>s)*B^n    and 0 <= (ret>>s) < d/2^s
# the return value is overly complicated , simplify this when we are clearer
# about what we need
EPILOGUE()
