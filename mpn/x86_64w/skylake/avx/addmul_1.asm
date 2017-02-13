;  AMD64 mpn_addmul_1 optimised for Intel Broadwell.
;
;  Copyright 2015 Free Software Foundation, Inc.
;
;  This file is part of the GNU MP Library.
;
;  The GNU MP Library is free software; you can redistribute it and/or modify
;  it under the terms of either:
;
;    * the GNU Lesser General Public License as published by the Free
;      Software Foundation; either version 3 of the License, or (at your
;      option) any later version.
;
;  or
;
;    * the GNU General Public License as published by the Free Software
;      Foundation; either version 2 of the License, or (at your option) any
;      later version.
;
;  or both in parallel, as here.
;
;  The GNU MP Library is distributed in the hope that it will be useful, but
;  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
;  for more details.
;
;  You should have received copies of the GNU General Public License and the
;  GNU Lesser General Public License along with the GNU MP Library.  If not,
;  see https://www.gnu.org/licenses/.
;
;  mp_limb_t mpn_addmul_1(mp_ptr, mp_ptr, mp_size_t, mp_limb_t)
;  rax                       rdi     rsi        rdx        rcx
;  rax                       rcx     rdx         r8        r9d

%include "yasm_mac.inc"

%define reg_save_list  rsi, rdi

    TEXT
    align   32
    FRAME_PROC mpn_addmul_1, 0, reg_save_list
    mov     rdi, rcx
    mov     rsi, rdx
    mov     rdx, r8
    mov     rcx, r9

    mov     r10, rcx
    mov     rcx, rdx
    mov     r8, rdx
    shr     rcx, 3
    and     r8, 7
    mov     rdx, r10
    lea     r10, [rel .1]
    movsxd  r8, dword [r10+r8*4]
    lea     r10, [r8+r10]
    jmp     r10

    align   8
.1: dd      .2 - .1
    dd      .7 - .1
    dd     .10 - .1
    dd      .3 - .1
    dd      .4 - .1
    dd      .5 - .1
    dd      .6 - .1
    dd     .19 - .1

.2: mulx    r8, r10, [rsi]
    lea     rsi, [rsi-8]
    lea     rdi, [rdi-8]
    lea     rcx, [rcx-1]
    jmp     .13
.3: mulx    rax, r9, [rsi]
    lea     rsi, [rsi+16]
    lea     rdi, [rdi-48]
    jmp     .18
.4: mulx    r8, r10, [rsi]
    lea     rsi, [rsi+24]
    lea     rdi, [rdi-40]
    jmp     .17
.5: mulx    rax, r9, [rsi]
    lea     rsi, [rsi+32]
    lea     rdi, [rdi-32]
    jmp     .16
.6: mulx    r8, r10, [rsi]
    lea     rsi, [rsi+40]
    lea     rdi, [rdi-24]
    jmp     .15
.7: mulx    rax, r9, [rsi]
    jrcxz   .8
    jmp     .12
.8: add     r9, [rdi]
    mov     [rdi], r9
    adc     rax, rcx
    EXIT_PROC reg_save_list

.9: adox    r9, [rdi]
    mov     [rdi], r9
    adox    rax, rcx
    adc     rax, rcx
    END_PROC reg_save_list

    nop
    nop
    nop
    nop
.10:mulx    r8, r10, [rsi]
    lea     rsi, [rsi+8]
    lea     rdi, [rdi+8]
    mulx    rax, r9, [rsi]

    align   32
.11:adox    r10, [rdi-8]
    adcx    r9, r8
    mov     [rdi-8], r10
    jrcxz   .9
.12:mulx    r8, r10, [rsi+8]
    adox    r9, [rdi]
    lea     rcx, [rcx-1]
    mov     [rdi], r9
    adcx    r10, rax
.13:mulx    rax, r9, [rsi+16]
    adcx    r9, r8
    adox    r10, [rdi+8]
    mov     [rdi+8], r10
.14:mulx    r8, r10, [rsi+24]
    lea     rsi, [rsi+64]
    adcx    r10, rax
    adox    r9, [rdi+16]
    mov     [rdi+16], r9
.15:mulx    rax, r9, [rsi-32]
    adox    r10, [rdi+24]
    adcx    r9, r8
    mov     [rdi+24], r10
.16:mulx    r8, r10, [rsi-24]
    adcx    r10, rax
    adox    r9, [rdi+32]
    mov     [rdi+32], r9
.17:mulx    rax, r9, [rsi-16]
    adox    r10, [rdi+40]
    adcx    r9, r8
    mov     [rdi+40], r10
.18:adox    r9, [rdi+48]
    mulx    r8, r10, [rsi-8]
    mov     [rdi+48], r9
    lea     rdi, [rdi+64]
    adcx    r10, rax
    mulx    rax, r9, [rsi]
    jmp     .11
.19:mulx    rax, r9, [rsi]
    lea     rsi, [rsi-16]
    lea     rdi, [rdi-16]
    jmp     .14

    end
