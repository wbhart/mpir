; PROLOGUE(mpn_sumdiff_n)
;
;  Copyright 2011 The Code Cavern
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
;  mp_limb_t mpn_sumdiff_n(mp_ptr, mp_ptr, mp_ptr, mp_ptr,  mp_size_t)
;  rax                        rdi     rsi     rdx     rcx          r8
;  rax                        rcx     rdx      r8      r9    [rsp+40]

%include "yasm_mac.inc"

%define reg_save_list   rbx, rbp, rsi, rdi, r12, r13, r14, r15

        CPU  Nehalem
        BITS 64

        FRAME_PROC mpn_sumdiff_n, 0, reg_save_list
        mov     rbx, [rsp+stack_use+40]
        xor     rax, rax
        mov     r10d, 3
        lea     rdi, [rcx+rbx*8-24]
        lea     rsi, [rdx+rbx*8-24]
        sub     r10, rbx
        lea     rdx, [r8+rbx*8-24]
        lea     rcx, [r9+rbx*8-24]
        mov     r9, rax
        jnc     .2

        align   16
.1:     sahf    
        mov     r8, [rdx+r10*8]
        mov     r11, r8
        adc     r8, [rcx+r10*8]
        mov     rbx, [rdx+r10*8+8]
        mov     r13, rbx
        adc     rbx, [rcx+r10*8+8]
        mov     r12, [rdx+r10*8+24]
        mov     rbp, [rdx+r10*8+16]
        mov     r15, r12
        mov     r14, rbp
        adc     rbp, [rcx+r10*8+16]
        adc     r12, [rcx+r10*8+24]
        lahf    
        add     r9b, 255
        sbb     r11, [rcx+r10*8]
        mov     [rdi+r10*8], r8
        sbb     r13, [rcx+r10*8+8]
        sbb     r14, [rcx+r10*8+16]
        mov     [rdi+r10*8+8], rbx
        mov     [rdi+r10*8+16], rbp
        sbb     r15, [rcx+r10*8+24]
        mov     [rdi+r10*8+24], r12
        setc    r9b
        mov     [rsi+r10*8+16], r14
        mov     [rsi+r10*8+24], r15
        add     r10, 4
        mov     [rsi+r10*8+8-32], r13
        mov     [rsi+r10*8-32], r11
        jnc     .1
.2:     
        cmp     r10, 2
        jg      .6
        je      .5
        jp      .4
.3:     sahf    
        mov     r8, [rdx]
        mov     r11, r8
        adc     r8, [rcx]
        mov     rbx, [rdx+8]
        mov     r13, rbx
        adc     rbx, [rcx+8]
        mov     rbp, [rdx+16]
        mov     r14, rbp
        adc     rbp, [rcx+16]
        lahf    
        add     r9b, 255
        sbb     r11, [rcx]
        mov     [rdi], r8
        sbb     r13, [rcx+8]
        sbb     r14, [rcx+16]
        mov     [rdi+8], rbx
        mov     [rdi+16], rbp
        setc    r9b
        mov     [rsi+16], r14
        mov     [rsi+8], r13
        mov     [rsi], r11
        sahf    
        mov     rax, 0
        adc     rax, 0
        add     r9b, 255
        rcl     rax, 1
        EXIT_PROC reg_save_list

.4:     sahf    
        mov     r8, [rdx+8]
        mov     r11, r8
        adc     r8, [rcx+8]
        mov     rbx, [rdx+8+8]
        mov     r13, rbx
        adc     rbx, [rcx+8+8]
        lahf    
        add     r9b, 255
        sbb     r11, [rcx+8]
        mov     [rdi+8], r8
        sbb     r13, [rcx+8+8]
        mov     [rdi+8+8], rbx
        setc    r9b
        mov     [rsi+8+8], r13
        mov     [rsi+8], r11
        sahf    
        mov     rax, 0
        adc     rax, 0
        add     r9b, 255
        rcl     rax, 1
        EXIT_PROC reg_save_list

.5:     sahf    
        mov     r8, [rdx+16]
        mov     r11, r8
        adc     r8, [rcx+16]
        lahf    
        add     r9b, 255
        sbb     r11, [rcx+16]
        mov     [rdi+16], r8
        setc    r9b
        mov     [rsi+16], r11
.6:     sahf    
        mov     rax, 0
        adc     rax, 0
        add     r9b, 255
        rcl     rax, 1
        END_PROC reg_save_list

        end
