; PROLOGUE(mpn_addsub_n)
;        
;  Copyright 2011 The Code Cavern
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
;  mp_limb_t mpn_addsub_n(mp_ptr, mp_ptr, mp_ptr, mp_ptr, mp_size_t)
;  rax                       rdi     rsi     rdx     rcx         r8
;  rax                       rcx     rdx      r8      r9   [rsp+40]

%include "yasm_mac.inc"

%define reg_save_list   rbx, rbp, rsi, rdi

        CPU  Nehalem
        BITS 64

        FRAME_PROC mpn_addsub_n, 0, reg_save_list
        mov     rbx, [rsp+stack_use+40]
        xor     rax, rax
        mov     r10d, 3
        sub     r10, rbx
        lea     rdi, [rcx+rbx*8-24]
        lea     rsi, [rdx+rbx*8-24]
        lea     rdx, [r8+rbx*8-24]
        lea     rcx, [r9+rbx*8-24]
        mov     r9, rax
        jnc     .2

        align   16
.1:     sahf    
        mov     r8, [rdx+r10*8]
        sbb     r8, [rcx+r10*8]
        mov     rbx, [rdx+r10*8+8]
        sbb     rbx, [rcx+r10*8+8]
        mov     r11, [rdx+r10*8+24]
        mov     rbp, [rdx+r10*8+16]
        sbb     rbp, [rcx+r10*8+16]
        sbb     r11, [rcx+r10*8+24]
        lahf    
        add     r9b, 255
        adc     r8, [rsi+r10*8]
        adc     rbx, [rsi+r10*8+8]
        mov     [rdi+r10*8], r8
        adc     rbp, [rsi+r10*8+16]
        adc     r11, [rsi+r10*8+24]
        setc    r9b
        mov     [rdi+r10*8+24], r11
        mov     [rdi+r10*8+16], rbp
        mov     [rdi+r10*8+8], rbx
        add     r10, 4
        jnc     .1
.2:     
        cmp     r10, 2
        jg      .6
        je      .5
        jp      .4
.3:     sahf    
        mov     r8, [rdx]
        sbb     r8, [rcx]
        mov     rbx, [rdx+8]
        sbb     rbx, [rcx+8]
        mov     rbp, [rdx+16]
        sbb     rbp, [rcx+16]
        lahf    
        add     r9b, 255
        adc     r8, [rsi]
        adc     rbx, [rsi+8]
        mov     [rdi], r8
        adc     rbp, [rsi+16]
        setc    r9b
        mov     [rdi+16], rbp
        mov     [rdi+8], rbx
        sahf    
        mov     eax, 0
        sbb     rax, 0
        add     r9b, 255
        adc     rax, 0
        EXIT_PROC reg_save_list

.4:     sahf    
        mov     r8, [rdx+8]
        sbb     r8, [rcx+8]
        mov     rbx, [rdx+16]
        sbb     rbx, [rcx+16]
        lahf    
        add     r9b, 255
        adc     r8, [rsi+8]
        adc     rbx, [rsi+16]
        mov     [rdi+8], r8
        setc    r9b
        mov     [rdi+16], rbx
        sahf    
        mov     eax, 0
        sbb     rax, 0
        add     r9b, 255
        adc     rax, 0
        EXIT_PROC reg_save_list

.5:     sahf    
        mov     r8, [rdx+16]
        sbb     r8, [rcx+16]
        lahf    
        add     r9b, 255
        adc     r8, [rsi+16]
        mov     [rdi+16], r8
        setc    r9b
.6:     sahf    
        mov     eax, 0
        sbb     rax, 0
        add     r9b, 255
        adc     rax, 0
        END_PROC reg_save_list

        end