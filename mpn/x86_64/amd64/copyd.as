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
;  not, write to the Free Software Foundation, Inc., 51 Franklin Street,
;  Fifth Floor, Boston, MA 02110-1301, USA.
;
;  Provided by Brian Gladman AMD64 using the Microsoft VC++ v8 64-bit
;  compiler and the YASM assembler.

; AMD64 mpn_copyd -- decrementing copy limb vector
;
;  Calling interface:
;
; void mpn_copyd(
;     mp_ptr dst,    rdi
;     mp_srcptr src, rsi
;     mp_size_t size rdx
; )

%include 'yasm_mac.inc'

%define    UNROLL_THRESHOLD 16

%define d_ptr  rdi
%define s_ptr  rsi
%define s_len  rdx
%define s_lend edx

    BITS    64

GLOBAL_FUNC mpn_copyd
    movsxd  s_len,s_lend
    cmp     s_len,byte UNROLL_THRESHOLD
    jge     label2                      ; if many limbs to move
    dec     s_len
    jl      label1
label0: 
    mov     rax,[s_ptr+s_len*8]         ; short move via rax
    mov     [d_ptr+s_len*8],rax
    dec     s_len
    jge     label0                      ; avoid single byte ret that
label1: 
    rep     ret                         ; interferes with branch prediction

label2: 
    mov     rax,s_ptr                   ; find relative alignment of
    xor     rax,d_ptr                   ; source and destination (min
    test    al,8
    jnz     label7                      ; not 16 byte aligned
    lea     rax,[s_ptr+s_len*8]
    test    al,8                        ; see if src is on 16 byte
    jz      label3                      ; boundary
    dec     s_len
    mov     rax,[rax-8]                 ; if not do a one limb copy
    mov     [d_ptr+s_len*8],rax
label3: 
    lea     s_len,[s_len-4]             ; now 16 byte aligned
label4: 
    prefetchnta [s_ptr+s_len*8+16-3*64] ; should this be -4*64 ??
    movdqa  xmm0,[s_ptr+s_len*8+16]     ; move 32 bytes at a time
    movntdq [d_ptr+s_len*8+16],xmm0
    movdqa  xmm0,[s_ptr+s_len*8]
    movntdq [d_ptr+s_len*8],xmm0
    sub     s_len,4
    jge     label4
    sfence
    test    s_len,2
    jz      label5
    movdqa  xmm0,[s_ptr+s_len*8+16]     ; move 16 bytes if necessary
    movdqa  [d_ptr+s_len*8+16],xmm0
label5: 
    test    s_len,1
    jz      label6
    movq    xmm0,[s_ptr]                ; move 8 bytes if necessary
    movq    [d_ptr],xmm0
label6: 
    ret

label7: 
    lea     s_len,[s_len-2]             ; move 8 bytes at a time
label8: 
    movq    xmm0,[s_ptr+s_len*8+8]
    movq    xmm1,[s_ptr+s_len*8]
    movq    [d_ptr+s_len*8+8],xmm0
    movq    [d_ptr+s_len*8],xmm1
    sub     s_len,2
    jge     label8
    test    s_len,1
    jz      label9
    movq    xmm0,[s_ptr]
    movq    [d_ptr],xmm0
label9: 
    ret
