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

; AMD64 mpn_add_n/mpn_sub_n -- mpn add or subtract.
;
;  Calling interface:
;
;  mp_limb_t __gmpn_<op>mul_1(          <op> = add or sub
;     mp_ptr dst,               rdi
;     mp_srcptr src,            rsi
;     mp_size_t size,           rdx
;     mp_limb_t mult            rcx
;  )
;
;  mp_limb_t __gmpn_<op>mul_1c(
;     mp_ptr dst,               rdi
;     mp_srcptr src,            rsi
;     mp_size_t size,           rdx
;     mp_limb_t mult,           rcx
;     mp_limb_t carry            r8
;  )
;
; Calculate src[size] multiplied by mult[1] and add to /subtract from dst[size] and
; return the carry or borrow from the top of the result

%include '../yasm_mac.inc'

%define UNROLL_LOG2        4
%define UNROLL_COUNT       (1 << UNROLL_LOG2)
%define UNROLL_MASK        (UNROLL_COUNT - 1)
%define  UNROLL_BYTES      (8 * UNROLL_COUNT)
%ifdef PIC
%define UNROLL_THRESHOLD   9
%else
%define UNROLL_THRESHOLD   6
%endif

%if UNROLL_BYTES >= 256
%error unroll count is too large
%elif UNROLL_BYTES >= 128
%define off 128
%else
%define off 0
%endif

%macro   mac_sub  4

    G_EXPORT %1%3
    G_EXPORT %1%4

    align 32

G_LABEL %1%3
    mov     r9, rcx
    mov     r10, rbx
    mov     r11, rbp
    
    mov     rax, rsi
    xor     rcx, rcx       ; carry = 0
    
    dec     rdx             ; test for one limb only
    jnz     %%5             ; if more than one

    mov     rax,[rax]       ; get limb value
    mov     rcx, rdi
    
    mul     r9              ; rax * mlt -> rdx (hi), rax (lo)

    %2      [rcx],rax        ; add/sub from destination
    adc     rdx, 0          ; add any carry into high word
    mov     rax,rdx         ; and return the carry value

    ret

    align 16

G_LABEL %1%4
    mov     r9, rcx
    mov     r10, rbx
    mov     r11, rbp
    
    mov     rax,rsi         ; source pointer
    
    dec     rdx             ; test for one limb
    jnz     %%1             ; if more than one
    
    mov     rax,[rax]       ; get limb value
    mov     rcx, rdi
    
    mul     r9              ; rax * mlt -> rdx (hi), rax (lo)
    
    add     rax,r8          ; add in input carry
    
    adc     rdx, 0          ; propagate it into rdx
    %2      [rcx],rax       ; add or subtract rax from dest limb
    
    adc     rdx, 0          ; propagate carry into high word
    mov     rax,rdx
    ret

    align 16
%%1:
    mov     rcx, r8
   
    align 32
%%5:
    mov     rbx, rdx
    cmp     rdx, UNROLL_THRESHOLD
    mov     rbp, r9
    mov     rax,[rsi]           ; first limb of source
    ja      %%3                 ; unroll for many limbs
    
    lea     rsi,[rsi+rbx*8+8]   ; next source limb
    lea     rdi,[rdi+rbx*8]     ; current dst limb
    neg     rbx

; simple loop

%%2:
    mul     rbp                 ; multiply current src limb -> rxx, rax
    add     rcx,rax             ; add in carry
    adc     rdx, 0              ; propagate carry into rdx
    
    %2      [rdi+rbx*8],rcx     ; add or subtract rax from dest limb
    mov     rax,[rsi+rbx*8]     ; get next source limb
    adc     rdx, 0              ; add carry or borrow into high word
    
    inc     rbx                 ; go to next limb
    mov     rcx,rdx             ; high word -> carry
    jnz     %%2


    mul     rbp                 ; one more limb to do
    
    mov     rbx, r10
    mov     rbp, r11

    add     rcx,rax
    adc     rdx, 0

    %2      [rdi],rcx
    adc     rdx, 0

    mov     rax,rdx             ; return carry value as a limb
    ret

    align 32

%%3:
    sub     rbx, 2
    dec     rdx

    shr     rbx,UNROLL_LOG2
    neg     rdx

    mov     r9, rbx
    and     rdx,UNROLL_MASK
    
    mov     r8,rdx
    mov     rbx,rdx                 ; cry_hi and jmp_val are temporary
    shl     r8,3                    ; values for calculating the jump
    shl     rdx,4                ; offset into the unrolled code

%ifdef PIC
    call    .pic_calc
.unroll_here:
..@unroll_here1:

%else
    lea     rdx,[rel %%4 + rdx + r8]
%endif

    neg     rbx
    mov     r8, rdx

    mul     rbp

    add     rcx,rax                 ; initial carry, becomes low carry
    adc     rdx, 0
    test    bl,1
    
    mov     rax,[rsi+8]             ; src second limb
    lea     rsi,[rsi+rbx*8+off+16]
    lea     rdi,[rdi+rbx*8+off]
    
    mov     rbx,rdx
    cmovnz  rbx,rcx              ; high, low carry other way around
    cmovnz  rcx,rdx
    
    jmp     r8

%ifdef PIC

.pic_calc:

	lea     rdx, [rdx+r8]
	add     rdx, ..@unroll_entry1 - ..@unroll_here1
	add     rdx, [rsp]
	ret

%endif

      align 32

.unroll_entry1:
..@unroll_entry1:
%%4:
%define CHUNK_COUNT  2
%assign i 0
%rep  UNROLL_COUNT / CHUNK_COUNT
%assign  disp0 8 * i * CHUNK_COUNT - off
%assign  disp1 disp0 + 8

    mul     rbp

    %2      [byte rdi+disp0],rcx
    mov     rcx, 0                  ; len = 0
    
    adc     rbx,rax
    
    adc     rcx,rdx

    mov     rax,[byte rsi+disp0]
    
    mul     rbp

    %2      [byte rdi+disp1],rbx

    mov     rbx,0                   ; len = 0
    
    adc     rcx,rax
    
    adc     rbx,rdx
    mov     rax,[byte rsi+disp1]
    
%assign i   i + 1
%endrep

    dec     r9
    lea     rsi,[rsi+UNROLL_BYTES]
    lea     rdi,[rdi+UNROLL_BYTES]
    
    jns     %%4

    mul     rbp

    %2      [rdi-off],rcx
    mov     rbp, r11

    adc     rax,rbx
    mov     rbx, r10

    adc     rdx,0
    %2      [rdi-off+8],rax

    adc     rdx,0

    mov     rax,rdx
    ret

%endmacro

    bits    64
    section .text

    mac_sub __g,sub,mpn_submul_1,mpn_submul_1c

    end
