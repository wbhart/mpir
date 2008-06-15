;  AMD K8 mpn_mul_basecase -- multiply two mpn numbers.
;
;  This file is just an adaptation of similar file in the k7 directory.
;  Adapted by P. Gaudry in April 2005.
;  Here is the copyright of the original k7 version:
;
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

; void mpn_mul_basecase (mp_ptr wp,
;                        mp_srcptr xp, mp_size_t xsize,
;                        mp_srcptr yp, mp_size_t ysize);
;
; Calculate xp,xsize multiplied by yp,ysize, storing the result in
; wp,xsize+ysize.
;
; This routine is essentially the same as mpn/generic/mul_basecase.c, but
; it's faster because it does most of the mpn_addmul_1() startup
; calculations only once.  The saving is 15-25% on typical sizes coming from
; the Karatsuba multiply code.

%include '../yasm_mac.inc'

%define UNROLL_LOG2         4
%define UNROLL_COUNT        (1 << UNROLL_LOG2)
%define UNROLL_MASK         (UNROLL_COUNT - 1)
%define UNROLL_BYTES        (8 * UNROLL_COUNT)
%define UNROLL_THRESHOLD    5

%define ysize   r8  
%define yp     rcx 
%define xsize  rdx 
%define xp     rsi 
%define wp     rdi 


    bits    64
    section .text
    align   32

    G_EXPORT __gmpn_mul_basecase

G_LABEL __gmpn_mul_basecase
	cmp     xsize, 2	
	ja	    .xsize_more_than_two
	je	    .two_by_something

; one limb by one limb
	
	mov     rax, [yp]	; rax <- y0
	mul	    qword [xp]        ; [ax:dx] <- y0*x0

	mov	    [wp], rax  
	mov	    [wp+8], rdx
	ret


;-----------------------------------------------------------------------------

.two_by_something:

; xsize = 2, hence rdx is free for usage

	dec	     ysize	        ; YSIZE--
	
	mov	     r9, [yp]		; r9 <- y0
	mov      rax, [xp]	    ; rax <- x0	
	jnz	     .two_by_two


; two limbs by one limb

	mul	    r9			    ;  [ax:dx] <- x0*y0	

	mov     [wp], rax     	;  w0 <- low_prod
	mov     rax, [xp+8]   	;  rax <- x1   (rsi is now free)
	mov	    rsi, rdx		;  rsi <- carry

	mul     r9	    		;  [ax:dx] <- x1*y0

	add     rsi, rax		;  rsi <- ax+carry   ( --> carry_flag)
	mov     [wp+8], rsi   	;  w1 <- rsi

	adc     rdx, 0	    	;  dx <- dx+carry
	mov     [wp+16], rdx  	;  w2 <- dx

	ret
	

; -----------------------------------------------------------------------------

	align 16

.two_by_two:
	; rax	x0			r8      
	; rbx	**untouched**		r9      y0
	; rcx	yp                      r10-11  
	; rdx   
	; rsi	xp
	; rdi   wp
	; rbp

	mul     r9			    ; [ax:dx]  <- x0*y0

	mov     r10, rdx	    ; r10 <- carry for w1

	mov     [wp], rax	    ; w0 <- ax
	mov     rax, [xp+8]	    ; ax <- x1

	mul     r9			    ; [ax:dx]  <- x1*y0

	add     r10, rax	    ; r10 <- r10 + ax  for w1

	adc     rdx, 0		    ; dx <- carry for w2
	mov     rcx, [yp+8]	    ; cx <- y1
	mov     [wp+8], r10	    ; w1 <- r10
	
	mov     rax, [xp+8]	    ; ax <- x1
	mov     r10, rdx	    ; carry, for w2

	mul     rcx			    ; [ax:dx] <- x1*y1

	add     r10, rax        ; r10 <- for w2

	adc     rdx, 0		    ; for w3
	mov     rax, [xp]	    ; x0

	mov     rsi, rdx	    ; carry, for w3

	mul     rcx			    ; x0*y1

	add     [wp+8], rax	    ; w1 += ax
	adc     r10, rdx	    ; for w2
	mov     [wp+16], r10    ; w2 <- r10

	adc     rsi, 0
	mov     [wp+24], rsi	; w3 <- carry in rsi

	ret

	
; -----------------------------------------------------------------------------

	    align 16

.xsize_more_than_two:

; The first limb of yp is processed with a simple mpn_mul_1 style loop
; inline.  Unrolling this doesn't seem worthwhile since it's only run once
; (whereas the addmul below is run ysize-1 many times).  A call to the
; actual mpn_mul_1 will be slowed down by the call and parameter pushing and
; popping, and doesn't seem likely to be worthwhile on the typical 13-26
; limb operations the Karatsuba code calls here with.

	; rax			r8	ysize
	; rbx   
	; rcx	yp
	; rdx	xsize
	; rsi	xp
	; rdi	wp
	; rbp


%define YSIZE  r8    ; already there
%define YP     r9    ;  init : rcx
%define XSIZE r10    ;  init : rdx
%define XP    r11    ;  init : rsi
%define WP    r12    ;  init : rdi     r12 should be saved!


;  FRAME doesn't carry on from previous, no pushes yet here

%define SAVE_RBX  [rsp+16]
%define SAVE_R12  [rsp+8]
%define SAVE_RBP  [rsp]

	sub     rsp, 24

	mov     SAVE_RBX, rbx
	mov     SAVE_R12, r12
	mov     SAVE_RBP, rbp
	
	mov     YP, rcx
	mov     XSIZE, rdx
	mov     XP, rsi
	mov     WP, rdi
	
	mov     rbp, [YP]
	mov     rcx, XSIZE

	xor     rbx, rbx
	lea     rsi, [XP+XSIZE*8]	; xp end

	lea     rdi, [WP+XSIZE*8]	; wp end of mul1
	neg     rcx


.mul1:
	; rax	scratch
	; rbx	carry
	; rcx	counter, negative
	; rdx	scratch
	; rsi	xp end
	; rdi	wp end of mul1
	; rbp	multiplier

	mov     rax, [rsi+rcx*8]

	mul     rbp

	add     rax, rbx
	mov     [rdi+rcx*8], rax
	mov     rbx, 0

	adc     rbx, rdx
	inc     rcx
	jnz     .mul1


	mov     rdx, YSIZE
	mov     rcx, XSIZE

	mov     [rdi], rbx		; final carry
	dec     rdx

	jnz     .ysize_more_than_one


	mov     rbx, SAVE_RBX
	mov     rbp, SAVE_RBP
	mov     r12, SAVE_R12
	add     rsp, 24

	ret


.ysize_more_than_one:
	cmp     rcx, UNROLL_THRESHOLD
	mov     rax, YP

	jae     .unroll


; -----------------------------------------------------------------------------
	; simple addmul looping
	;
	; rax	yp
	; rbx
	; rcx	xsize
	; rdx	ysize-1
	; rsi	xp end
	; rdi	wp end of mul1
	; rbp

	lea     rbp, [rax+rdx*8+8]	; yp end
	neg     rcx
	neg     rdx

	mov     RAX, [rsi+rcx*8]	; xp low limb
	mov     YSIZE, rdx      	; -(ysize-1)
	inc     rcx

	xor     rbx, rbx	    	; initial carry
	mov     XSIZE, rcx	        ; -(xsize-1)
	mov     YP, rbp

	mov     rbp, [rbp+rdx*8]	; yp second lowest limb - multiplier
	jmp     .simple_outer_entry


	; this is offset ????  Align ?
.simple_outer_top:	
	; rbp	ysize counter, negative

	mov     rdx, YP
	mov     rcx, XSIZE      	; -(xsize-1)
	xor     rbx, rbx	    	; carry

	mov     YSIZE, rbp
	add     rdi, 8		        ; next position in wp

	mov     rbp, [rdx+rbp*8]	; yp limb - multiplier
	mov     rax, [rsi+rcx*8-8]	; xp low limb


.simple_outer_entry:

.simple_inner:
	; rax	xp limb
	; rbx	carry limb
	; rcx	loop counter (negative)
	; rdx	scratch
	; rsi	xp end
	; rdi	wp end
	; rbp	multiplier

	mul     rbp

	add     rbx, rax
	adc     rdx, 0

	add     [rdi+rcx*8], rbx
	mov     rax, [rsi+rcx*8]
	adc     rdx, 0

	inc     rcx
	mov     rbx, rdx
	jnz     .simple_inner

	mul     rbp

	mov     rbp, YSIZE
	add     rbx, rax

	adc     rdx, 0
	add     [rdi], rbx

	adc     rdx, 0
	inc     rbp

	mov     [rdi+8], rdx
	jnz     .simple_outer_top


	mov     rbx, SAVE_RBX
	mov     rbp, SAVE_RBP
	mov     r12, SAVE_R12
	add     rsp, 24

	ret



; -----------------------------------------------------------------------------
;
; The unrolled loop is the same as in mpn_addmul_1(), see that code for some
; comments.
;
; VAR_ADJUST is the negative of how many limbs the leals in the inner loop
; increment xp and wp.  This is used to adjust back xp and wp, and rshifted
; to given an initial VAR_COUNTER at the top of the outer loop.
;
; VAR_COUNTER is for the unrolled loop, running from VAR_ADJUST/UNROLL_COUNT
; up to -1, inclusive.
;
; VAR_JMP is the computed jump into the unrolled loop.
;
; VAR_XP_LOW is the least significant limb of xp, which is needed at the
; start of the unrolled loop.
;
; YSIZE is the outer loop counter, going from -(ysize-1) up to -1,
; inclusive.
;
; YP is offset appropriately so that the YSIZE counter can be
; added to give the location of the next limb of yp, which is the multiplier
; in the unrolled loop.
;
; The trick with VAR_ADJUST means it's only necessary to do one fetch in the
; outer loop to take care of xp, wp and the inner loop counter.

%define VAR_COUNTER      [rsp+16]
%define VAR_ADJUST       [rsp+8]
%define VAR_XP_LOW       [rsp]
%define VAR_EXTRA_SPACE  24


.unroll: 
	; rax	yp
	; rbx
	; rcx	xsize
	; rdx	ysize-1
	; rsi	xp end
	; rdi	wp end of mul1
	; rbp

	mov     rsi, XP             ; from here, XP not used
	mov     rbp, [rax+8]		; multiplier (yp second limb)
	lea     rax, [rax+rdx*8+8]	; yp adjust for ysize indexing

	mov     rdi, WP
	mov     YP, rax
	neg     rdx

		;  From here, only YP and YSIZE are used
		;  Hence r10, r11, r12 are free for use

	mov     YSIZE, rdx
	lea     rbx, [rcx+UNROLL_COUNT-2]	; (xsize-1)+UNROLL_COUNT-1
	dec     rcx				            ; xsize-1

	mov     rax, [rsi]		            ; xp low limb
	and     rbx, -UNROLL_MASK-1
	neg     rcx

	sub     rsp, VAR_EXTRA_SPACE

	neg     rbx
	and     rcx, UNROLL_MASK
	mov     r12, rcx            		; for later parity test

	mov     VAR_ADJUST, rbx
	mov     rdx, rcx
	mov     r10, rcx
	shl     rcx, 4
	shl     r10, 3

	sar     rbx, UNROLL_LOG2

; 24=16+8 code bytes per limb
%ifdef PIC

	call    .pic_calc

.unroll_here:
..@unroll_here1:

%else
    lea     rcx, [rcx + r10 + ..@unroll_entry1]
%endif

	neg     rdx

	mov     VAR_XP_LOW, rax
	mov     XP, rcx	                	; XP used for VAR_JUMP
	lea     rdi, [rdi+rdx*8+8]      	; wp and xp, adjust for unrolling,
	lea     rsi, [rsi+rdx*8+8]      	;  and start at second limb
	jmp     .unroll_outer_entry


%ifdef PIC

.pic_calc:

	lea     rcx, [rcx+r10]
	add     rcx, ..@unroll_entry1 - ..@unroll_here1
	add     rcx, [rsp]
	ret

%endif


; --------------------------------------------------------------------------

 	align 32

.unroll_outer_top:
	; ebp	ysize counter, negative

	mov     rbx, VAR_ADJUST
	mov     rdx, YP

	mov     rax, VAR_XP_LOW
	mov     YSIZE, rbp              	; store incremented ysize counter

	lea     rdi, [rdi+rbx*8+8]
	lea     rsi, [rsi+rbx*8]
	sar     rbx, UNROLL_LOG2

	mov     rbp, [rdx+rbp*8]        	; yp next multiplier
	mov     rcx, XP

.unroll_outer_entry:
	mul     rbp

	mov     rcx, r12
	test	cl, 1               		; and clear carry bit
	mov     VAR_COUNTER, rbx
	mov     rbx, 0

	mov     rcx, 0
	cmovz	rcx, rax	; eax into low carry, zero into high carry limb
	cmovnz	rbx, rax

	; Extra fetch of VAR_JMP is bad, but registers are tight
	; TODO: we have more registers, now!!!!
	jmp     XP


; -----------------------------------------------------------------------------
	
    align 32

.unroll_top:
	; rax	xp limb
	; rbx	carry high
	; rcx	carry low
	; rdx	scratch
	; rsi	xp+8
	; rdi	wp
	; rbp	yp multiplier limb
	;
	; VAR_COUNTER  loop counter, negative
	;
	; 24 bytes each limb

.unroll_entry:
..@unroll_entry1:

%define CHUNK_COUNT  2
%assign i 0
%rep UNROLL_COUNT / CHUNK_COUNT
%define  disp0 8 * i * CHUNK_COUNT

	adc     rbx, rdx
    mov     rax, [byte rsi+disp0]

	mul     rbp

    add     [byte rdi+disp0], rcx
	mov     rcx, 0

	adc     rbx, rax


	adc     rcx, rdx	
	mov     rax, [byte rsi+disp0+8]

	mul     rbp

	add     [byte rdi+disp0+8], rbx
	mov     rbx, 0

	adc     rcx, rax

%assign  i i + 1
%endrep

	inc     qword VAR_COUNTER
	lea     rsi, [rsi+UNROLL_BYTES]
	lea     rdi, [rdi+UNROLL_BYTES]

	jnz	    .unroll_top


	; rax
	; rbx	zero
	; rcx	low
	; rdx	high
	; rsi
	; rdi	wp, pointing at second last limb)
	; rbp
	;
	; carry flag to be added to high

	mov     rbp, YSIZE
	adc     rdx, 0
	add     [rdi], rcx

	adc     rdx, 0
	inc     rbp

	mov     [rdi+8], rdx
	jnz	    .unroll_outer_top

    add     rsp, 24

	mov  	rbp, SAVE_RBP
	mov     r12, SAVE_R12
	mov     rbx, SAVE_RBX
	add     rsp, 24

	ret

  end
