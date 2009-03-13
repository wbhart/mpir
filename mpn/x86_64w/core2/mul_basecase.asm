;
; Core2 mpn_mul_basecase -- multiply two mpn numbers.
; Version 1.0.5.
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
;  l1 = r8d , l2 = dword [rsp+0x28] ; rcx[l1 + l2] = rdx[l1] * r9[l2]
;
;  void __gmpn_mul_basecase(
;     mp_ptr rp,          rcx
;     mp_srcptr xp,       rdx
;     mp_size_t xn,        r8
;     mp_srcptr yp,        r9
;     mp_size_t yn [rsp+0x28]    as a *** 32-bit *** word
; )
;
; Multiply xp[xn] by yp[yn] and write the result to rp[un+vn] with xn >= yn on
; entry.
;
; This is an SEH frame function with a leaf prologue

%include "..\yasm_mac.inc"

%define reg_save_list   rbx, rsi, rdi, r12, r13, r14

    BITS 64

%macro mulloop 1

    alignb  16, nop
%%1:
    mov     r10, 0
    mul     r13
    mov     [rdi+r11*8], r12
    add     r9, rax
    db      0x26
    adc     r10, rdx
    mov     rax, [rsi+r11*8+16]
    mul     r13
    mov     [rdi+r11*8+8], r9
    add     r10, rax
    mov     ebx, 0
    adc     rbx, rdx
    mov     rax, [rsi+r11*8+24]
    mov     r12, 0
    mov     r9, 0
    mul     r13
    mov     [rdi+r11*8+16], r10
    db      0x26
    add     rbx, rax
    db      0x26
    adc     r12, rdx
    mov     rax, [rsi+r11*8+32]
    mul     r13
    mov     [rdi+r11*8+24], rbx
    db      0x26
    add     r12, rax
    db      0x26
    adc     r9, rdx
    add     r11, 4
    mov     rax, [rsi+r11*8+8]
    jnc     %%1

%endmacro

%macro mulnext0 0

    mov     rax, [rsi+r11*8+16]
    mul     r13
    mov     [rdi+r11*8+8], r9
    add     r10, rax
    mov     ebx, 0
    adc     rbx, rdx
    mov     rax, [rsi+r11*8+24]
    mov     r12d, 0
    mul     r13
    mov     [rdi+r11*8+16], r10
    add     rbx, rax
    adc     r12, rdx
    mov     rax, [rsi+r11*8+32]
    mul     r13
    mov     [rdi+r11*8+24], rbx
    add     r12, rax
    adc     rdx, 0
    mov     [rdi+r11*8+32], r12
    mov     [rdi+r11*8+40], rdx
    add     r8, 1               ; ***
    mov     rax, [rsi+r14*8]
    mov     r11, r14
%endmacro

%macro mulnext1 0
    mov     rax, [rsi+r11*8+16]
    mul     r13
    mov     [rdi+r11*8+8], r9
    add     r10, rax
    mov     r12d, 0
    adc     r12, rdx
    mov     rax, [rsi+r11*8+24]
    mul     r13
    mov     [rdi+r11*8+16], r10
    add     r12, rax
    adc     rdx, 0
    mov     [rdi+r11*8+24], r12
    mov     [rdi+r11*8+32], rdx
    add     r8, 1               ; ***
    lea     rdi, [rdi+8]

%endmacro

%macro mulnext2 0

    mov     rax, [rsi+r11*8+16]
    mul     r13
    mov     [rdi+r11*8+8], r9
    add     r10, rax
    mov     ebx, 0
    adc     rbx, rdx
    mov     [rdi+r11*8+16], r10
    mov     [rdi+r11*8+24], rbx
    add     r8, 1               ; ***
    mov     rax, [rsi+r14*8]
    mov     r11, r14

%endmacro

%macro mulnext3 0

    mov     [rdi+r11*8+8], r9
    mov     [rdi+r11*8+16], r10
    add     r8, 1               ; ***
    lea     rdi, [rdi+8]

%endmacro

; changes from standard addmul
; change  r8 to r12   and rcx to r13
; reemove ret and write last limb

%macro addmulloop 1
    alignb  16, nop
%%1:
    mov     r10, 0
    mul     r13
    add     [rdi+r11*8], r12
    adc     r9, rax
    db      0x26
    adc     r10, rdx
    mov     rax, [rsi+r11*8+16]
    mul     r13
    add     [rdi+r11*8+8], r9
    adc     r10, rax
    mov     ebx, 0
    adc     rbx, rdx
    mov     rax, [rsi+r11*8+24]
    mov     r12, 0
    mov     r9, 0
    mul     r13
    add     [rdi+r11*8+16], r10
    db      0x26
    adc     rbx, rax
    db      0x26
    adc     r12, rdx
    mov     rax, [rsi+r11*8+32]
    mul     r13
    add     [rdi+r11*8+24], rbx
    db      0x26
    adc     r12, rax
    db      0x26
    adc     r9, rdx
    add     r11, 4
    mov     rax, [rsi+r11*8+8]
    jnc     %%1

%endmacro

%macro addmulpro0 0

; mov (%rsi,%r14,8),%rax
    mov     r13, [rcx+r8*8]
    lea     rdi, [rdi+8]
; mov %r14,%r11
    mul     r13
    mov     r12, rax
    mov     rax, [rsi+r14*8+8]
    mov     r9, rdx
    cmp     r14, 0

%endmacro

%macro addmulnext0 0
    mov     r10d, 0
    mul     r13
    add     [rdi+r11*8], r12
    adc     r9, rax
    adc     r10, rdx
    mov     rax, [rsi+r11*8+16]
    mul     r13
    add     [rdi+r11*8+8], r9
    adc     r10, rax
    mov     ebx, 0
    adc     rbx, rdx
    mov     rax, [rsi+r11*8+24]
    mov     r12d, 0
    mul     r13
    add     [rdi+r11*8+16], r10
    adc     rbx, rax
    adc     r12, rdx
    mov     rax, [rsi+r11*8+32]
    mul     r13
    add     [rdi+r11*8+24], rbx
    adc     r12, rax
    adc     rdx, 0
    add     [rdi+r11*8+32], r12
    mov     rax, [rsi+r14*8]
    adc     rdx, 0
    add     r8, 1               ; ***
    mov     [rdi+r11*8+40], rdx
    mov     r11, r14
    ; lea 8(%rdi),%rdi

%endmacro

%macro addmulpro1 0

    mov     rax, [rsi+r14*8]
    mov     r13, [rcx+r8*8]
    mov     r11, r14
    mul     r13
    mov     r12, rax
    mov     rax, [rsi+r14*8+8]
    mov     r9, rdx
    cmp     r14, 0

%endmacro

%macro addmulnext1 0

    mov     r10d, 0
    mul     r13
    add     [rdi+r11*8], r12
    adc     r9, rax
    adc     r10, rdx
    mov     rax, [rsi+r11*8+16]
    mul     r13
    add     [rdi+r11*8+8], r9
    adc     r10, rax
    mov     r12d, 0
    adc     r12, rdx
    mov     rax, [rsi+r11*8+24]
    mul     r13
    add     [rdi+r11*8+16], r10
    adc     r12, rax
    adc     rdx, 0
    add     [rdi+r11*8+24], r12
    adc     rdx, 0
    mov     [rdi+r11*8+32], rdx
    add     r8, 1               ; ***
    lea     rdi, [rdi+8]

%endmacro

%macro addmulpro2 0

    mov     r13, [rcx+r8*8]
    lea     rdi, [rdi+8]
    mul     r13
    mov     r12, rax
    mov     rax, [rsi+r14*8+8]
    mov     r9, rdx
    cmp     r14, 0

%endmacro

%macro addmulnext2 0

    mov     r10d, 0
    mul     r13
    add     [rdi+r11*8], r12
    adc     r9, rax
    adc     r10, rdx
    mov     rax, [rsi+r11*8+16]
    mul     r13
    mov     ebx, 0
    add     [rdi+r11*8+8], r9
    adc     r10, rax
    adc     rbx, rdx
    mov     rax, [rsi+r14*8]
    add     [rdi+r11*8+16], r10
    adc     rbx, 0
    mov     [rdi+r11*8+24], rbx
    add     r8, 1               ; ***
    mov     r11, r14

%endmacro

%macro addmulpro3 0

    mov     rax, [rsi+r14*8]
    mov     r13, [rcx+r8*8]
    mov     r11, r14
    mul     r13
    mov     r12, rax
    mov     rax, [rsi+r14*8+8]
    mov     r9, rdx
    cmp     r14, 0

%endmacro

%macro addmulnext3 0
    mul     r13
    add     [rdi+r11*8], r12
    adc     r9, rax
    mov     r10d, 0
    adc     r10, rdx
    add     [rdi+r11*8+8], r9
    adc     r10, 0
    mov     [rdi+r11*8+16], r10
    add     r8, 1               ; ***
    lea     rdi, [rdi+8]

%endmacro

%macro mpn_muladdmul_1_int 1
    mulnext%1
    jz      %%3
    alignb  16, nop
%%1:
    addmulpro%1
    jge     %%2
    addmulloop %1
%%2:
    addmulnext%1
    jnz     %%1
%%3:

%endmacro

    LEAF_PROC mpn_mul_basecase
    cmp     r8d, 2
    jz      two
    jb      one

    FRAME_PROC mul_m_by_n, 0, reg_save_list
    mov     rdi, rcx
    mov     rsi, rdx
    movsxd  rdx, r8d
    mov     rcx, r9
    mov     r8d, dword [rsp + stack_use + 40]

    mov     r14, 5
    sub     r14, rdx
    lea     rdi, [rdi+rdx*8-40]
    lea     rcx, [rcx+r8*8]
    neg     r8
    lea     rsi, [rsi+rdx*8-40]
    mov     rax, [rsi+r14*8]
    mov     r13, [rcx+r8*8]
    mov     r11, r14
    mul     r13
    mov     r12, rax
    mov     rax, [rsi+r14*8+8]
    mov     r9, rdx
    cmp     r14, 0
    jge     mulskiploop
    mulloop 1
mulskiploop:
    mov     r10d, 0
    mul     r13
    mov     [rdi+r11*8], r12
    add     r9, rax
    adc     r10, rdx
    cmp     r11, 2
    ja      case3
    jz      case2
    jp      case1
case0:
    mpn_muladdmul_1_int 0
    jmp     xit

    alignb  16, nop
case1:
    mpn_muladdmul_1_int 1
    jmp     xit

    alignb  16, nop
case2:
    mpn_muladdmul_1_int 2
    jmp     xit

    alignb  16, nop
case3:
    mpn_muladdmul_1_int 3
xit:
    END_PROC reg_save_list

    alignb  16, nop
one:mov     rax, [rdx]
    mul     qword [r9]
    mov     [rcx], rax
    mov     [rcx+8], rdx
    ret

    alignb  16, nop
two:mov     r10, rdx
    mov     r11, [r9]
    mov     rax, [r10]
    mul     r11
    mov     [rcx], rax
    mov     rax, [r10+8]
    mov     r8, rdx
    mul     r11
    add     r8, rax
    adc     rdx, 0
    mov     [rcx+8], r8
    mov     [rcx+16], rdx
    cmp     dword [rsp+0x28], 2
    jz      twobytwo
    ret
twobytwo:
    mov     r8d, 0
    mov     r11, [r9+8]
    mov     rax, [r10]
    mul     r11
    add     [rcx+8], rax
    mov     rax, [r10+8]
    adc     r8, rdx
    mul     r11
    add     r8, rax
    adc     rdx, 0
    add     [rcx+16], r8
    adc     rdx, 0
    mov     [rcx+24], rdx
    ret

    end
