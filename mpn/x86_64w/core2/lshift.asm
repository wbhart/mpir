
;  mpn_lshift

;  Copyright 2009 Jason Moxham

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
;   mpn_lshift(mp_ptr rdi, mp_ptr rsi, mp_size_t rdx, mp_limb_t rcx)
;                     rcx         rdx            r8d             r9
;   rax=carry

%include "..\yasm_mac.inc"

%define reg_save_list rbx, rsi, rdi

    CPU  Athlon64
    BITS 64

    FRAME_PROC mpn_lshift, 0, reg_save_list
; odd and even n seem to have different runtimes
    movsxd  rbx, r8d
    lea     rsi, [rdx+24]
    lea     rdi, [rcx+24]
    mov     rcx, r9

    mov     rdx, [rsi+rbx*8-32]
    xor     rax, rax
    shld    rax, rdx, cl
    sub     rbx, 5
    js      L_skiplp
    xalign  16
L_lp:
    mov     r8, [rsi+rbx*8]
    mov     r11, [rsi+rbx*8-24]
    mov     r9, [rsi+rbx*8-8]
    shld    rdx, r8, cl
    mov     [rdi+rbx*8+8], rdx
    mov     rdx, r11
    mov     r10, [rsi+rbx*8-16]
    shld    r8, r9, cl
    shld    r9, r10, cl
    mov     [rdi+rbx*8], r8
    mov     [rdi+rbx*8-8], r9
    shld    r10, r11, cl
    sub     rbx, 4
    mov     [rdi+rbx*8+16], r10
    jns     L_lp
L_skiplp:
    cmp     rbx, -2
    ja      L_case3
    je      L_case2
    jp      L_case1
; ALIGN(16)
L_case0:
    shl     rdx, cl
    mov     [rdi+rbx*8+8], rdx
    jmp     L_xit

    xalign  16
L_case3:
    mov     r8, [rsi+rbx*8]
    mov     r9, [rsi+rbx*8-8]
    shld    rdx, r8, cl
    mov     [rdi+rbx*8+8], rdx
    mov     r10, [rsi+rbx*8-16]
    shld    r8, r9, cl
    shld    r9, r10, cl
    mov     [rdi+rbx*8], r8
    mov     [rdi+rbx*8-8], r9
    shl     r10, cl
    mov     [rdi+rbx*8-16], r10
    jmp     L_xit

    xalign  16
L_case2:
    mov     r8, [rsi+rbx*8]
    mov     r9, [rsi+rbx*8-8]
    shld    rdx, r8, cl
    mov     [rdi+rbx*8+8], rdx
    shld    r8, r9, cl
    shl     r9, cl
    mov     [rdi+rbx*8], r8
    mov     [rdi+rbx*8-8], r9
    jmp     L_xit

    xalign  16
L_case1:
    mov     r8, [rsi+rbx*8]
    shld    rdx, r8, cl
    mov     [rdi+rbx*8+8], rdx
    shl     r8, cl
    mov     [rdi+rbx*8], r8
L_xit:
    END_PROC reg_save_list

    end
