dnl  X86_64 mpn_divrem_hensel_r_1

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

C	 hensel divide (rdi,rsi) / rdx
C	rax=hensel remainder from div 

C	This is divrem_hensel_1 with shifting on the output of the quotient
C	On k8/k10 the shifting comes for free so no need to have different
C	fn for that. And on K8/K10 this runs at 10c/l which is optimal
C	This function "replaces" divexact_1 and modexact_1_odd
C	This is same as the shifting version but with  no shifting

ASM_START()
PROLOGUE(mpn_divrem_hensel_r_1)
mov $0,%r9
sub %rsi,%r9
lea (%rdi,%rsi,8),%rdi

mov %rdx,%rcx

mov %rdx,%rax
imul %ecx,%edx
mov $2,%r11
sub %rdx,%r11
imul %eax,%r11d

mov %r11,%rax
imul %ecx,%r11d 
mov $2,%rdx
sub %r11,%rdx		
imul %eax,%edx

mov %rdx,%rax
imul %ecx,%edx
mov $2,%r11
sub %rdx,%r11
imul %eax,%r11d

mov %r11,%rax
imul %ecx,%r11d 
mov $2,%rdx
sub %r11,%rdx		
imul %eax,%edx

mov %rdx,%rax
imul %rcx,%rdx
mov $2,%r11
sub %rdx,%r11
imul %rax,%r11

C //clear carry
xor %rdx,%rdx
ALIGN(16)
loop:
    mov (%rdi,%r9,8),%rax
    sbb %rdx,%rax
    sbb %r8,%r8
    imul %r11,%rax
    mul %rcx
    add $1,%r8
    inc %r9
    jnz loop
mov $0,%rax
adc %rdx,%rax
ret
EPILOGUE()
