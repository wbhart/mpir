;  Copyright 2001, 2002 Free Software Foundation, Inc.
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
;  not, write to the Free Software Foundation, Inc., 51 Franklin Street,
;  Fifth Floor, Boston, MA 02110-1301, USA.
;
; CREDITS
;
; The code used here is derived from that provided by ct35z at:
;
;    http://www.geocities.jp/ct35z/gmp-core2-en.html
;
; This code is based largely on Pierrick Gaudry's excellent assembly
; support for the AMD64 architecture.  (Note that Intel64 and AMD64,
; while using the same instruction set, have very different
; microarchitectures.  So, this code performs very poorly on AMD64
; machines even though it is near-optimal on Intel64.)
;
; Roger Golliver works for Intel and provided insightful improvements
; particularly in using the "lea" instruction to perform additions
; and register-to-register moves.
;
; Jason Worth Martin's excellent assembly support for the Intel64
; architecture has been used where appropriate.
;
; Eric Bainville has a brilliant exposition of optimizing arithmetic for
; AMD64 (http://www.bealto.it).  I adapted many of the ideas he
; describes to Intel64.
;
; Agner Fog is a demigod in the x86 world.  If you are reading assembly
; code files and you haven't heard of Agner Fog, then take a minute to
; look over his software optimization manuals (http://www.agner.org/).
; They are superb.
;
; Adapted for use with VC++ and YASM using a special mode in which NASM
; preprocessing is used with AT&T assembler syntax. I am very grateful
; for the support that Peter Johnson (one of the authors of YASM) has
; provided in getting this special YASM mode working.  Without his
; support this port would have been a great deal more difficult.
;
; The principle issues that I have had to address is the difference
; between GCC and MSVC in their register saving and parameter passing
; conventions.  Registers that have to be preserved across function
; calls are:
;
; GCC:             rbx, rbp, r12..r15
; MSVC:  rsi, rdi, rbx, rbp, r12..r15 xmm6..xmm15
;
; Parameter passing conventions for non floating point parameters:
;
;   function(   GCC     MSVC
;       p1,     rdi      rcx
;       p2,     rsi      rdx
;       p3,     rdx       r8
;       p4,     rcx       r9
;       p5,      r8 [rsp+40]
;       p6,      r9 [rsp+48]
;
; Care must be taken with 32-bit values in 64-bit register or on the
; stack because the upper 32-bits of such parameters are undefined.
;
;       Brian Gladman

; Intel64 mpn_mul_basecase -- mpn by mpn multiplication.
;
; void __gmpn_mul_basecase(  MSVC   GCC
;     mp_ptr rp,              rcx   rdi
;     mp_srcptr xp,           rdx   rsi
;     mp_size_t xn,            r8   rdx
;     mp_srcptr yp,            r9   rcx
;     mp_size_t yn     [rsp+0x28]    r8    as a *** 32-bit *** word
; )
;
; Multiply xp[xn] by yp[yn] and write the result to rp[un+vn] with
; xn >= yn on entry.
;
; BPL is bytes per limb, which is 8 in the 64-bit code here

%include "..\x86_64_asm.inc"

%define reg_save_list       rsi, rdi, rbx, rbp, r12, r13, r14, r15

%define BPL                 8
%define UNROLL_EXPONENT     4
%define UNROLL_SIZE         (1 << UNROLL_EXPONENT)
%define UNROLL_MASK         (UNROLL_SIZE - 1)
%define ADDR(p,i,d)         (d*BPL)(p, i, BPL)

%define s2limb  rbp
%define s1p     rsi
%define rp      rdi
%define a_x     rbx
%define a_y      r9
%define a_z     r10
%define b_x     r11
%define b_y     r12
%define b_z     r13
%define _un     r14
%define index   r15
%define vn       r8
%define vp      rcx

    bits    64
    section .text
    align   16
    global  __gmpn_mul_basecase

%ifdef DLL
    export  __gmpn_mul_basecase
%endif

prologue    __gmpn_mul_basecase, 0, reg_save_list
    mov     rdi, rcx
    mov     rsi, rdx
    mov     rdx, r8
    mov     rcx, r9
    xor     r8, r8
    mov     r8d, [rsp+9*8+40]

    xor     _un, _un
    sub     _un, rdx
    mov     s2limb, [vp]
    lea     s1p, [s1p+rdx*8]
    lea     rp, [rp+rdx*8]
    mov     index, _un
    cmp     rdx, 4
    jge     main_loop
    lea     rax, [rel .0]
    add     rax, [rax+rdx*8]
    jmp     rax

   align 8
.0:
    dq      .1 - .0
    dq      .2 - .0
    dq      .3 - .0
    dq      .4 - .0

.1: xor     rax, rax
    jmp     mul_1_exit

.2: mov     rax, [s1p+index*8]
    mul     s2limb
    mov     [rp+index*8], rax
    mov     rax, rdx
    jmp     mul_1_exit

.3: mov     rax, [s1p+index*8]
    mul     s2limb
    mov     a_x, rax
    mov     a_y, rdx
    mov     rax, [s1p+index*8+8]
    mul     s2limb
    add     rax, a_y
    mov     [rp+index*8], a_x
    adc     rdx, 0
    mov     [rp+index*8+8], rax
    mov     rax, rdx
    jmp     mul_1_exit

.4: mov     rax, [s1p+index*8]
    mul     s2limb
    mov     a_x, rax
    mov     a_y, rdx
    mov     rax, [s1p+index*8+8]
    mul     s2limb
    mov     b_x, rax
    mov     b_y, rdx
    mov     rax, [s1p+index*8+016]
    mul     s2limb
    add     b_x, a_y
    mov     [rp+index*8], a_x
    adc     b_y, 0
    add     rax, b_y
    mov     [rp+index*8+8], b_x
    adc     rdx, 0
    mov     [rp+index*8+16], rax
    mov     rax, rdx
    jmp     mul_1_exit

main_loop:
    mov     a_z, rdx
    test    rdx, 1
    jz      .1
    mov     rax, [s1p+index*8]
    mul     s2limb
    mov     a_x, rax
    mov     a_y, rdx
    mov     rax, [s1p+index*8+8]
    mul     s2limb
    mov     b_x, rax
    mov     b_y, rdx
    jmp     .2

.1: mov     rax, [s1p+index*8]
    mul     s2limb
    mov     b_x, rax
    mov     b_y, rdx
    mov     rax, [s1p+index*8+8]
    mul     s2limb
    mov     a_x, rax
    mov     a_y, rdx

.2: sub     a_z, 4
    and     a_z, UNROLL_MASK
    inc     a_z
    mov     rax, (l1_end - l1_start) >> UNROLL_EXPONENT
    mul     a_z
    lea     rdx, [rel l1_end]
    sub     rdx, rax
    mov     rax, [s1p+index*8+16]
    lea     index, [index+a_z+3-UNROLL_SIZE]
    jmp     rdx

%macro seq_1 6
    mul     s2limb
    mov     %3, %1
    lea     %1, [rax]
    mov     rax, [byte s1p+index*8+8*%6]
    add     %4, %2
    mov     [byte rp+index*8+8*(%6-3)], %3
    lea     %2, [rdx]
    adc     %5, 0
%endmacro

   align 16
l1_start:
%assign i 0
%rep 16
%if (i & 1)
    seq_1 b_x, b_y, b_z, a_x, a_y, i
%else
    seq_1 a_x, a_y, a_z, b_x, b_y, i
%endif
%assign i i + 1
%endrep
l1_end:

    add     index, UNROLL_SIZE
    jnz     l1_start

    mul     s2limb
    mov     [rp+index*8-24], a_x
    add     b_x, a_y
    adc     b_y, 0
    mov     [rp+index*8-16], b_x
    add     rax, b_y
    adc     rdx, 0
    mov     [rp+index*8-8], rax
    mov     rax, rdx

mul_1_exit:
    mov     [rp], rax
    add     rp, BPL
    add     vp, BPL
    dec     vn
    jz      exit
    xor     rdx, rdx
    sub     rdx, _un
    cmp     _un, -4
    jle     .0
    lea     rax, [rel jt2]
    add     rax, [rax+rdx*8]
    mov     [rsp], rax
    jmp     .1

.0: sub     rdx, 4
    and     rdx, UNROLL_MASK
    inc     rdx
    lea     rax, [rdx+_un+(3-UNROLL_SIZE)]
    movq    xmm0, rax
    mov     rax, (l2_end - l2_start) >> UNROLL_EXPONENT
    mul     rdx
    lea     rdx, [rel l2_end]
    sub     rdx, rax
    mov     [rsp], rdx
.1:
    align   16
mul_loop:
    mov     s2limb, [vp]
    mov     index, _un
    cmp     _un, -4
    jle     loop_2
    jmp     [rsp]

   align 8
jt2:dq      .0 - jt2
    dq      .1 - jt2
    dq      .2 - jt2
    dq      .3 - jt2

.0: xor     rax, rax
    jmp     mul_loop_end

.1: mov     rax, [s1p+index*8]
    mul     s2limb
    mov     a_z, [rp+index*8]
    add     a_z, rax
    mov     rax, 0
    mov     [rp+index*8], a_z
    adc     rax, rdx
    jmp     mul_loop_end

.2: mov     rax, [s1p+index*8]
    mul     s2limb
    mov     a_z, [rp+index*8]
    mov     a_x, rax
    mov     a_y, rdx
    mov     rax, [s1p+index*8+8]
    mul     s2limb
    mov     b_z, [rp+index*8+8]
    add     a_z, a_x
    adc     rax, a_y
    mov     [rp+index*8], a_z
    adc     rdx, 0
    add     b_z, rax
    mov     rax, 0
    mov     [rp+index*8+8], b_z
    adc     rax, rdx
    jmp     mul_loop_end

.3: mov     rax, [s1p+index*8]
    mul     s2limb
    mov     a_z, [rp+index*8]
    mov     a_x, rax
    mov     a_y, rdx
    mov     rax, [s1p+index*8+8]
    mul     s2limb
    mov     b_z, [rp+index*8+8]
    mov     b_x, rax
    mov     b_y, rdx
    mov     rax, [s1p+index*8+16]
    mul     s2limb
    add     a_z, a_x
    adc     b_x, a_y
    mov     [rp+index*8], a_z
    mov     a_z, [rp+index*8+16]
    adc     b_y, 0
    add     b_z, b_x
    adc     rax, b_y
    mov     [rp+index*8+8], b_z
    adc     rdx, 0
    add     a_z, rax
    mov     rax, 0
    mov     [rp+index*8+16], a_z
    adc     rax, rdx
    jmp     mul_loop_end

loop_2:
    test    _un, 1
    jz      .1

    mov     rax, [s1p+index*8]
    mul     s2limb
    mov     a_z, [rp+index*8]
    mov     a_x, rax
    mov     a_y, rdx
    mov     rax, [s1p+index*8+8]
    mul     s2limb
    mov     b_z, [rp+index*8+8]
    mov     b_x, rax
    mov     b_y, rdx
    jmp     .2

.1: mov     rax, [s1p+index*8]
    mul     s2limb
    mov     b_z, [rp+index*8]
    mov     b_x, rax
    mov     b_y, rdx
    mov     rax, [s1p+index*8+8]
    mul     s2limb
    mov     a_z, [rp+index*8+8]
    mov     a_x, rax
    mov     a_y, rdx

.2: mov     rax, [s1p+index*8+16]
    movq    index, xmm0
    jmp     [rsp]

%macro seq_2 6
    mul     s2limb
    add     %3, %1
    lea     %1, [rax]
    mov     rax, [byte s1p+index*8+8*%6]
    adc     %4, %2
    mov     [byte rp+index*8+8*(%6-3)], %3
    mov     %3, [byte rp+index*8+8*(%6-1)]
    lea     %2, [rdx]
    adc     %5, 0
%endmacro

   align    16
l2_start:
%assign i 0
%rep 16
%if (i & 1)
    seq_2 b_x, b_y, b_z, a_x, a_y, i
%else
    seq_2 a_x, a_y, a_z, b_x, b_y, i
%endif
%assign i i + 1
%endrep
l2_end:
    add     index, UNROLL_SIZE
    jnz     l2_start

    mul     s2limb
    add     a_z, a_x
    mov     [rp+index*8-24], a_z
    mov     a_z, [rp+index*8-8]
    adc     b_x, a_y
    adc     b_y, 0
    add     b_z, b_x
    mov     [rp+index*8-16], b_z
    adc     rax, b_y
    adc     rdx, 0
    add     a_z, rax
    mov     rax, 0
    mov     [rp+index*8-8], a_z
    adc     rax, rdx

mul_loop_end:
    mov     [rp], rax
    add     rp, BPL
    add     vp, BPL
    dec     vn
    jnz     mul_loop

exit:
    epilogue    reg_save_list

    end
