; PROLOGUE(mpn_lshift)

;  Version 1.0.3.
;
;  Copyright 2008 Jason Moxham
;
;  Windows Conversion Copyright 2008 Brian Gladman
;
;  This file is part of the MPIR Library.
;
;  The MPIR Library is free software; you can redistribute it and/or modify
;  it under the terms of the GNU Lesser General Public License as published
;  by the Free Software Foundation; either version 2.1 of the License, or (at
;  your option) any later version.
;
;  The MPIR Library is distributed in the hope that it will be useful, but
;  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
;  License for more details.
;
;  You should have received a copy of the GNU Lesser General Public License
;  along with the MPIR Library; see the file COPYING.LIB.  If not, write
;  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
;  Boston, MA 02110-1301, USA.
;
;  mp_limb_t mpn_lshift(mp_ptr, mp_ptr, mp_size_t, mp_uint)
;  rax                     rdi     rsi        rdx      rcx
;  rax                     rcx     rdx         r8      r9d

%include "yasm_mac.inc"

%define reg_save_list rbx, rsi, rdi

    CPU  Athlon64
    BITS 64

    FRAME_PROC mpn_lshift, 0, reg_save_list
; odd and even n seem to have different runtimes
    mov     rbx, r8
    lea     rsi, [rdx+24]
    lea     rdi, [rcx+24]
    mov     ecx, r9d

    mov     rdx, [rsi+rbx*8-32]
    xor     rax, rax
    shld    rax, rdx, cl
    sub     rbx, 5
    js      .2
    
	xalign  16
.1:	mov     r8, [rsi+rbx*8]
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
    jns     .1
.2:	cmp     rbx, -2
    ja      .4
    je      .5
    jp      .6
; ALIGN(16)
.3:	shl     rdx, cl
    mov     [rdi+rbx*8+8], rdx
    EXIT_PROC reg_save_list

    xalign  16
.4:	mov     r8, [rsi+rbx*8]
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
    EXIT_PROC reg_save_list

    xalign  16
.5:	mov     r8, [rsi+rbx*8]
    mov     r9, [rsi+rbx*8-8]
    shld    rdx, r8, cl
    mov     [rdi+rbx*8+8], rdx
    shld    r8, r9, cl
    shl     r9, cl
    mov     [rdi+rbx*8], r8
    mov     [rdi+rbx*8-8], r9
    EXIT_PROC reg_save_list

    xalign  16
.6:	mov     r8, [rsi+rbx*8]
    shld    rdx, r8, cl
    mov     [rdi+rbx*8+8], rdx
    shl     r8, cl
    mov     [rdi+rbx*8], r8
.7:	END_PROC reg_save_list

    end
