;  Copyright 1999, 2000, 2001, 2002 Free Software Foundation, Inc.
;
;  This file is part of the GNU MP Library.
;
;  The GNU MP Library is free software; you can redistribute it and/or
;  modify it under the terms of the GNU Lesser General Public License as
;  published by the Free Software Foundation; either version 2.1 of the
;  License, or (at your option) any later version.
;
;  The GNU MP Library is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;  Lesser General Public License for more details.
;
;  You should have received a copy of the GNU Lesser General Public
;  License along with the GNU MP Library; see the file COPYING.LIB.  If
;  not, write to the Free Software Foundation, Inc., 59 Temple Place -
;  Suite 330, Boston, MA 02111-1307, USA.
;
;  Adapted by Brian Gladman AMD64 using the Microsoft VC++ v8 64-bit
;  compiler and the YASM assembler.

;  AMD64 mpn_rshift -- mpn right shift
;
;  Calling interface:
;
; mp_limb_t mpn_rshift(
;     mp_ptr dst,       rdi
;     mp_srcptr src,    rsi
;     mp_size_t size,   rdx
;     unsigned shift    rcx
; )

%include '../yasm_mac.inc'


%define src rsi
%define dst rdi
%define r_tmpd ecx
%define s_len rdx

   BITS 64

GLOBAL_FUNC mpn_rshift
    movq    mm7, [src]
    movd    mm1, r_tmpd
    mov     eax, 64
    sub     eax, r_tmpd
    movd    mm0, eax
    movq    mm3, mm7
    psllq   mm7, mm0
    movd    rax, mm7
    lea     src, [src+s_len*8]
    lea     dst, [dst+s_len*8]
    neg     s_len
    add     s_len, 2
    jg      label1

    align   8
label0: 
    movq    mm6, [src+s_len*8-8]
    movq    mm2, mm6
    psllq   mm6, mm0
    psrlq   mm3, mm1
    por     mm3, mm6
    movq    [dst+s_len*8-16], mm3
    je      label2
    movq    mm7, [src+s_len*8]
    movq    mm3, mm7
    psllq   mm7, mm0
    psrlq   mm2, mm1
    por     mm2, mm7
    movq    [dst+s_len*8-8], mm2
    add     s_len, 2
    jle     label0
label1: 
    movq    mm2, mm3
label2: 
    psrlq   mm2, mm1
    movq    [dst-8], mm2
    emms
    ret
