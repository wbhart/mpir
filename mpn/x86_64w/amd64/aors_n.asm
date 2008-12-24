;
; AMD64 mpn_add_n/mpn_sub_n -- mpn add or subtract 
; Version 1.0.3.
;
; Copyright 2008 Jason Moxham 
; Windows conversion by Brian Gladman
;
; Windows x64 ABI
;
; Calling interface:
;
;  mp_limb_t __gmpn_<op>_n(    <op> = add OR sub
;     mp_ptr dst,              rcx
;     mp_srcptr src1,          rdx
;     mp_srcptr src2,           r8
;     mp_size_t  len            r9
;  )
;
;  mp_limb_t __gmpn_<op>_nc(   <op> = add OR sub
;     mp_ptr dst,              rcx
;     mp_srcptr src1,          rdx
;     mp_srcptr src2,           r8
;     mp_size_t len,            r9
;     mp_limb_t carry   [rsp+0x28]
;  )
;
;  Calculate src1[size] plus(minus) src2[size] and store the result in
;  dst[size].  The return value is the carry bit from the top of the result
;  (1 or 0).  The _nc version accepts 1 or 0 for an initial carry into the
;  low limb of the calculation.  Note values other than 1 or 0 here will
;  lead to garbage results.
;
;  This is an SEH Leaf Function (no unwind support needed)
;
;   %1 = __g, %2 = adc, %3 = mpn_add_n, %4 = mpn_add_nc
;   %1 = __g, %2 = sbb, %3 = mpn_sub_n, %4 = mpn_sub_nc

%macro   mac_sub  4

    global  %1%4
    global  %1%3

%ifdef DLL
    export  %1%4
    export  %1%3
%endif

%1%4:
    mov     r10,[rsp+0x28]
    jmp     %%0
%1%3:
    xor     r10, r10

%%0:movsxd  r9, r9d
    mov	    rax, r9
    and	    rax, 3
    shr	    r9, 2
    lea     r9,[r10+r9*2]
    shr     r9, 1
    jnz	    %%2

    mov	    r10, [rdx]
    %2      r10, [r8]
    mov	    [rcx], r10
    dec	    rax
    jz	    %%1
    mov	    r10, [rdx+8]
    %2      r10, [r8+8]
    mov	    [rcx+8], r10
    dec	    rax
    jz	    %%1
    mov	    r10, [rdx+16]
    %2      r10, [r8+16]
    mov	    [rcx+16], r10
    dec	    rax
%%1:adc	    rax, rax
    ret

    align   8
%%2:mov	    r10, [rdx]
	mov	    r11, [rdx+8]
	lea	    rdx, [rdx+32]
	%2	    r10, [r8]
	%2	    r11, [r8+8]
	lea	    r8, [r8+32]
	mov	    [rcx], r10
	mov	    [rcx+8], r11
	lea	    rcx, [rcx+32]
	mov	    r10, [rdx-16]
	mov	    r11, [rdx-8]
	%2	    r10, [r8-16]
	%2	    r11, [r8-8]
	mov	    [rcx-16], r10
	dec	    r9
	mov	    [rcx-8], r11
	jnz	    %%2

    inc	    rax
    dec	    rax
    jz	    %%3
    mov	    r10, [rdx]
    %2      r10, [r8]
    mov	    [rcx], r10
    dec	    rax
    jz	    %%3
    mov	    r10, [rdx+8]
    %2      r10, [r8+8]
    mov	    [rcx+8], r10
    dec	    rax
    jz	    %%3
    mov	    r10, [rdx+16]
    %2      r10, [r8+16]
    mov	    [rcx+16], r10
    dec	    rax
%%3:adc	    rax, rax
    ret

%endmacro

    bits    64
    section .text

    mac_sub __g,adc,mpn_add_n,mpn_add_nc
    mac_sub __g,sbb,mpn_sub_n,mpn_sub_nc

    end
