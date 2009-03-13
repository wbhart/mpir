
; AMD64 mpn_submul_1 -- multiply and subtract
; Version 1.0.3.
;
;  Copyright 2008 Jason Moxham
;
;  Windows Conversion Copyright 2008 Brian Gladman
;
;  This file is part of the MPIR Library.
;  The MPIR Library is free software; you can redistribute it and/or modify
;  it under the terms of the GNU Lesser General Public License as published
;  by the Free Software Foundation; either version 2.1 of the License, or (at
;  your option) any later version.
;  The MPIR Library is distributed in the hope that it will be useful, but
;  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
;  License for more details.
;  You should have received a copy of the GNU Lesser General Public License
;  along with the MPIR Library; see the file COPYING.LIB.  If not, write
;  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
;  Boston, MA 02110-1301, USA.
;
;  Calling interface:
;
;  mp_limb_t __gmpn_<op>mul_1(          <op> = add or sub
;     mp_ptr dst,               rcx
;     mp_srcptr src,            rdx
;     mp_size_t size,            r8
;     mp_limb_t mult             r9
;  )
;
; Calculate src[size] multiplied by mult[1] and add to /subtract from
; dst[size] and return the carry or borrow from the top of the result
;
; These are SEH frame functions with two leaf prologues

%include "..\yasm_mac.inc"

    BITS 64

%define reg_save_list r12, r13, r14

    LEAF_PROC mpn_submul_1
    movsxd  r8, r8d
    mov     rax, [rdx]
    cmp     r8, 1
    jnz     .1
    mul     r9
    sub     [rcx], rax
    adc     rdx, 0
    mov     rax, rdx
    ret

    align   16
.1:
    FRAME_PROC mpn_submul_1x, 0, reg_save_list
    mov     r10, rdx
    sub     r8, 5
    lea     r10, [r10+r8*8]
    lea     rcx, [rcx+r8*8]
    neg     r8
    mul     r9
    mov     r11, rax
    mov     rax, [8+r10+r8*8]
    mov     r13, rdx
    cmp     r8, 0
    jge     .3

    alignb  16, nop
.2: mov     r12d, 0
    mul     r9
    sub     [rcx+r8*8], r11
    adc     r13, rax
    adc     r12, rdx
    mov     rax, [16+r10+r8*8]
    mul     r9
    sub     [8+rcx+r8*8], r13
    adc     r12, rax
    mov     r14d, 0
    adc     r14, rdx
    mov     rax, [24+r10+r8*8]
    mov     r11d, 0
    mov     r13d, 0
    mul     r9
    sub     [16+rcx+r8*8], r12
    adc     r14, rax
    adc     r11, rdx
    mov     rax, [32+r10+r8*8]
    mul     r9
    sub     [24+rcx+r8*8], r14
    adc     r11, rax
    adc     r13, rdx
    add     r8, 4
    mov     rax, [8+r10+r8*8]
    jnc     .2

.3: jz      .7
    jp      .5
    cmp     r8, 1
    je      .6

.4: mov     r12d, 0
    mul     r9
    sub     [rcx+r8*8],r11
    adc     r13, rax
    adc     r12, rdx
    mov     rax, [16+r10+r8*8]
    mul     r9
    sub     [8+rcx+r8*8], r13
    adc     r12, rax
    mov     r14d, 0
    adc     r14, rdx
    sub     [16+rcx+r8*8], r12
    adc     r14, 0
    mov     rax, r14
    jmp     .8

    alignb  16, nop
.5: mov     r12d, 0
    mul     r9
    sub     [rcx+r8*8], r11
    adc     r13, rax
    adc     r12, rdx
    sub     [8+rcx+r8*8], r13
    adc     r12, 0
    mov     rax, r12
    jmp     .8

    alignb  16, nop
.6: mov     r12d, 0
    mul     r9
    sub     [rcx+r8*8], r11
    adc     r13, rax
    adc     r12, rdx
    mov     rax, [16+r10+r8*8]
    mul     r9
    sub     [8+rcx+r8*8], r13
    adc     r12, rax
    mov     r14d, 0
    adc     r14, rdx
    mov     rax, [24+r10+r8*8]
    mov     r11d, 0
    mul     r9
    sub     [16+rcx+r8*8], r12
    adc     r14, rax
    adc     r11, rdx
    sub     [24+rcx+r8*8], r14
    adc     r11, 0
    mov     rax, r11
    jmp     .8

    alignb  16, nop
.7: mov     r12d, 0
    mul     r9
    sub     [rcx+r8*8], r11
    adc     r13, rax
    adc     r12, rdx
    mov     rax, [16+r10+r8*8]
    mul     r9
    sub     [8+rcx+r8*8], r13
    adc     r12, rax
    mov     r14d, 0
    adc     r14, rdx
    mov     rax, [24+r10+r8*8]
    mov     r11d, 0
    mov     r13d, 0
    mul     r9
    sub     [16+rcx+r8*8], r12
    adc     r14, rax
    adc     r11, rdx
    mov     rax, [32+r10+r8*8]
    mul     r9
    sub     [24+rcx+r8*8], r14
    adc     r11, rax
    adc     r13, rdx
    sub     [32+rcx+r8*8], r11
    adc     r13, 0
    mov     rax, r13
.8:
    END_PROC reg_save_list

    end
