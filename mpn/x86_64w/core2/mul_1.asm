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

;  Intel64 mpn_mul_1 -- Multiply a limb vector with a limb and
;  store the result.
;
;  Calling interface:
;  mp_limb_t mpn_mul_1 (
;     mp_ptr dst,               rcx
;     mp_srcptr src,            rdx
;     mp_size_t size,            r8
;     mp_limb_t multiplier       r9
;  )
;
;  mp_limb_t mpn_mul_1c (
;     mp_ptr dst,               rcx
;     mp_srcptr src,            rdx
;     mp_size_t size,            r8
;     mp_limb_t multiplier,      r9
;     mp_limb_t carry    [rsp+0x28]
;  )
;
; Multiply src[size] by mult and store the result in dst[size].  Return the
; carry limb from the top of the result.
;
; mpn_mul_1c() accepts an initial carry for the calculation, it's added into
; the low limb of the destination.
;
; BPL is bytes per limb, which is 8 in the 64-bit code here

%define BPL                 8
%define UNROLL_EXPONENT     4
%define UNROLL_SIZE         (1 << UNROLL_EXPONENT)
%define UNROLL_MASK         (UNROLL_SIZE - 1)
%define ADDR(p,i,d)         (d*BPL)(p, i, BPL)

; Register  Usage
; --------  -----
; rax       low word from mul
; rbx
; s2limb    s2limb
; rdx       high word from mul
; s1p       s1p
; rp        rp
; rbp       Base Pointer
; rsp       Stack Pointer
; a_x       A_x
; a_y       A_y
; a_z       A_z
; b_x       B_x
; b_y       B_y
; b_z       B_z
; temp      temp
; index     index

%include "..\x86_64_asm.inc"

%define reg_save_list       rsi, rdi, r12, r13, r14, r15

%define s2limb  rcx
%define s1p     rsi
%define rp      rdi
%define a_x      r8
%define a_y      r9
%define a_z     r10
%define b_x     r11
%define b_y     r12
%define b_z     r13
%define temp    r14
%define index   r15

    bits    64
    section .text
    align   16
    global  __gmpn_mul_1
    global  __gmpn_mul_1c

%ifdef DLL
    export  __gmpn_mul_1
    export  __gmpn_mul_1c
%endif

__gmpn_mul_1:
    xor     r10, r10
    jmp     entry
    
__gmpn_mul_1c:
    mov     a_z, [rsp+0x28]

prologue    entry, reg_save_list, 0
    mov     rdi, rcx
    mov     rsi, rdx
    xor     rdx, rdx
    mov     edx, r8d
    mov     rcx, r9

    lea     s1p, [s1p+rdx*8]
    lea     rp, [rp+rdx*8]
    xor     index, index
    sub     index, rdx
    cmp     rdx, 4
    jge     .mul_1_main_loop_prep
    lea     rax, [rip+0xa]
    add     rax, [rax+rdx*8]
    jmp     rax

   align 8
.mul_1_jmptbl0:
    dq  .mul_1_small_0 - .mul_1_jmptbl0
    dq  .mul_1_small_1 - .mul_1_jmptbl0
    dq  .mul_1_small_2 - .mul_1_jmptbl0
    dq  .mul_1_small_3 - .mul_1_jmptbl0

.mul_1_small_0:
    mov     rax, a_z
    jmp     .mul_1_exit

.mul_1_small_1:
    mov     rax, [s1p+index*8]
    mul     s2limb
    add     rax, a_z
    adc     rdx, 0
    mov     [rp+index*8], rax
    mov     rax, rdx
    jmp     .mul_1_exit

.mul_1_small_2:
    mov     rax, [s1p+index*8]
    mul     s2limb
    add     rax, a_z
    adc     rdx, 0
    mov     a_x, rax
    mov     a_y, rdx
    mov     rax, [s1p+index*8+8]
    mul     s2limb
    add     rax, a_y
    mov     [rp+index*8], a_x
    adc     rdx, 0
    mov     [rp+index*8+8], rax
    mov     rax, rdx
    jmp     .mul_1_exit

.mul_1_small_3:
    mov     rax, [s1p+index*8]
    mul     s2limb
    add     rax, a_z
    adc     rdx, 0
    mov     a_x, rax
    mov     a_y, rdx
    mov     rax, [s1p+index*8+8]
    mul     s2limb
    mov     b_x, rax
    mov     b_y, rdx
    mov     rax, [s1p+index*8+16]
    mul     s2limb
    add     b_x, a_y
    mov     [rp+index*8], a_x
    adc     b_y, 0
    add     rax, b_y
    mov     [rp+index*8+8], b_x
    adc     rdx, 0
    mov     [rp+index*8+16], rax
    mov     rax, rdx
    jmp     .mul_1_exit

.mul_1_main_loop_prep:
    mov     temp, rdx
    test    rdx, 1
    jz      .mul_1_main_loop_prep_n1
    mov     rax, [s1p+index*8]
    mul     s2limb
    add     rax, a_z
    adc     rdx, 0
    mov     a_x, rax
    mov     a_y, rdx
    mov     rax, [s1p+index*8+8]
    mul     s2limb
    mov     b_x, rax
    mov     b_y, rdx
    jmp     .mul_1_main_loop_prep_n2

.mul_1_main_loop_prep_n1:
    mov     rax, [s1p+index*8]
    mul     s2limb
    add     rax, a_z
    adc     rdx, 0
    mov     b_x, rax
    mov     b_y, rdx
    mov     rax, [s1p+index*8+8]
    mul     s2limb
    mov     a_x, rax
    mov     a_y, rdx

.mul_1_main_loop_prep_n2:
    sub     temp, 4
    and     temp, UNROLL_MASK
    inc     temp
    mov     rax, (.unroll_end - .unroll_start) >> UNROLL_EXPONENT
    mul     temp
    lea     rdx, [rel .unroll_end]
    sub     rdx, rax
    mov     rax, [s1p+index*8+16]
    lea     index, [index+temp+3-UNROLL_SIZE]
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

    align   16
.unroll_start:
%assign i  0
%rep 16
%if (i & 1)
    seq_1   b_x, b_y, b_z, a_x, a_y, i
%else
    seq_1   a_x, a_y, a_z, b_x, b_y, i
%endif
%assign i i + 1
%endrep
.unroll_end

    add     index, UNROLL_SIZE
    jnz     .unroll_start

.mul_1_finish:
    mul     s2limb
    mov     [rp+index*8-24], a_x
    add     b_x, a_y
    adc     b_y, 0
    mov     [rp+index*8-16], b_x
    add     rax, b_y
    adc     rdx, 0
    mov     [rp+index*8-8], rax
    mov     rax, rdx
.mul_1_exit:
    epilogue    reg_save_list,0 

    end
