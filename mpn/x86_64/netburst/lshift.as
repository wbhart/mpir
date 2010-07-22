;  Copyright 1999, 2000, 2001, 2002 Free Software Foundation, Inc.
;
;  Copyright 2008 Brian Gladman, William Hart
;
;  This file is part of the MPIR Library.
;
;  The MPIR Library is free software; you can redistribute it and/or
;  modify it under the terms of the GNU Lesser General Public License as
;  published by the Free Software Foundation; either version 2.1 of the
;  License, or (at your option) any later version.
;
;  The MPIR Library is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;  Lesser General Public License for more details.
;
;  You should have received a copy of the GNU Lesser General Public
;  License along with the MPIR Library; see the file COPYING.LIB.  If
;  not, write to the Free Software Foundation, Inc., 59 Temple Place -
;  Suite 330, Boston, MA 02111-1307, USA.
;
;  Adapted by Brian Gladman for AMD64 using the Microsoft VC++ v8 64-bit
;  compiler and the YASM assembler.

;  AMD64 mpn_lshift -- mpn left shift
;
;  Calling interface:
;
; mp_limb_t mpn_lshift(
;     mp_ptr dst,       rdi
;     mp_srcptr src,    rsi
;     mp_size_t size,   rdx
;     unsigned shift    rcx
; )

%include 'yasm_mac.inc'

%define src    rsi
%define dst    rdi
%define s_len  rdx
%define r_tmpd ecx

    BITS    64

GLOBAL_FUNC mpn_lshift
    movq    mm7, [src+s_len*8-8]   ; put top source chunk in mm7
    movd    mm1, r_tmpd            ; put shift value in mm1
    mov     eax, 64                
    sub     eax, r_tmpd            
    movd    mm0, eax               ; put 64 - shift value in mm0
    movq    mm3, mm7               ; save original source chunk in mm3
    psrlq   mm7, mm0               ; shift 
    movd    rax, mm7               ; put part shifted out top in rax to be returned
    sub     s_len, 2
    jl      label1

    align   4
label0:  
    movq    mm6, [src+s_len*8]     ; put next source chunk in mm6
    movq    mm2, mm6               ; copy into mm2
    psrlq   mm6, mm0               ; shift mm6 right
    psllq   mm3, mm1               ; ...and mm3 left
    por     mm3, mm6               ; and combine
    movq    [dst+s_len*8+8], mm3   ; store result
    je      label2                
    movq    mm7, [src+s_len*8-8]   ; next source chunk
    movq    mm3, mm7               ; copy it
    psrlq   mm7, mm0               ; shift right
    psllq   mm2, mm1               ; ...and left
    por     mm2, mm7               ; and combine
    movq    [dst+s_len*8], mm2     ; and store result
    sub     s_len, 2
    jge     label0
label1:  
    movq    mm2, mm3
label2:  
    psllq   mm2, mm1               ; final shift
    movq    [dst], mm2             ; and store
    emms
    ret
