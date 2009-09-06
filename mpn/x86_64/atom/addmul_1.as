; **************************************************************************
;  Intel64 mpn_addmul_1 -- Multiply a limb vector with a limb and
;  add the result to a second limb vector.
;
;  Copyright (C) 2006  Jason Worth Martin <jason.worth.martin@gmail.com>
;
;  This program is free software; you can redistribute it and/or modify
;  it under the terms of the GNU Lesser General Public License as published
;  by the Free Software Foundation; either version 2 of the License, or
;  (at your option) any later version.
;
;  This program is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU Lesser General Public License
;  along with this program; if not, write to the Free Software Foundation,
;  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
;
; **************************************************************************
;
;
; CREDITS
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
; Eric Bainville has a brilliant exposition of optimizing arithmetic for
; AMD64 (http://www.bealto.it).  I adapted many of the ideas he
; describes to Intel64.
;
; Agner Fog is a demigod in the x86 world.  If you are reading assembly
; code files and you haven't heard of Agner Fog, then take a minute to
; look over his software optimization manuals (http://www.agner.org/).
; They are superb.
;
; *********************************************************************


; With a 4-way unroll the code has
;
;         	cycles/limb
; Hammer:           4.8
; Woodcrest:        4.6
;
; With increased unrolling, it appears to converge to 4 cycles/limb
; on Intel Core 2 machines.  I believe that this is optimal, however
; it requires such absurd unrolling that it becomes unusable for all
; but the largest inputs.  A 4-way unroll seems like a good balance
; to me because then commonly used input sizes (e.g. 1024bit Public
; keys) still benifit from the speed up.

;
; This is just a check to see if we are in my code testing sandbox
; or if we are actually in the GMP source tree

%include 'yasm_mac.inc'


; *********************************************************************
; *********************************************************************
;
; Here are the important macro parameters for the code
;
;      BpL is Bytes per Limb (8 since this is 64bit code)
;
;	UNROLL_SIZE is a power of 2 for which we unroll the code.
;                  possible values are 2,4,8,15,..., 256.  A reasonable
;                  value is probably 4.  If really large inputs
;                  are expected, then 16 is probably good.  Larger
;                  values are really only useful for flashy
;                  benchmarks and testing asymptotic behavior.
;
;      THRESHOLD is the minimum number of limbs needed before we bother
;                using the complicated loop.  A reasonable value is
;                2*UNROLL_SIZE + 6
;
; *********************************************************************
; *********************************************************************
%define	BpL	8
%define	UNROLL_SIZE	4
%define	UNROLL_MASK	UNROLL_SIZE-1
%define	THRESHOLD	2*UNROLL_SIZE+6

; Here is a convenient Macro for addressing
; memory.  Entries of the form
;
;      ADDR(ptr,index,displacement)
;
; get converted to
;
;      [displacement*BpL + ptr + index*BpL]
;
%define	ADDR(a,b,c)	[c*BpL+a+b*BpL]


; Register	Usage
; --------	-----
; rax		low word from mul
; rbx*
; rcx		s2limb
; rdx		high word from mul
; rsi		s1p
; rdi		rp
; rbp*		Base Pointer
; rsp*		Stack Pointer
; r8		A_x
; r9		A_y
; r10		A_z
; r11		B_x
; r12*		B_y
; r13*		B_z
; r14*		temp
; r15*		index
; 
; * indicates that the register must be
; preserved for the caller.
%define	s2limb	rcx
%define	s1p	rsi
%define	rp	rdi
%define	A_x	r8
%define	A_y	r9
%define	A_z	r10
%define	B_x	r11
%define	B_y	r12
%define	B_z	r13
%define	temp	r14
%define	index	r15

	
; INPUT PARAMETERS
; rp		rdi
; s1p		rsi
; n		rdx
; s2limb	rcx
	BITS	64
GLOBAL_FUNC mpn_addmul_1
					; Compare the limb count
					; with the threshold value.
					; If the limb count is small
					; we just use the small loop,
					; otherwise we jump to the
					; more complicated loop.
	cmp	rdx,THRESHOLD
	jge	L_mpn_addmul_1_main_loop_prep
	mov	r11,rdx
	lea	rsi,[rsi+rdx*8]
	lea	rdi,[rdi+rdx*8]
	neg	r11
	xor	r8, r8
	xor	r10, r10
	jmp	L_mpn_addmul_1_small_loop
	
	align	16
L_mpn_addmul_1_small_loop:
	mov	rax,[rsi+r11*8]
	mul	rcx
	add	rax,[rdi+r11*8]
	adc	rdx,r10
	add	rax,r8
	mov	r8,r10
	mov	[rdi+r11*8],rax
	adc	r8,rdx
	inc	r11
	jne	L_mpn_addmul_1_small_loop

	mov	rax,r8
	ret

L_mpn_addmul_1_main_loop_prep:
	push	r15
	push	r14
	push	r13
	push	r12
				; If n is even, we need to do three
				; pre-multiplies, if n is odd we only
				; need to do two.
	mov	temp,rdx
	mov	index,0
	mov	A_x,0
	mov	A_y,0
	and	rdx,1
	jnz	L_mpn_addmul_1_odd_n

					; Case n is even
	mov	rax,ADDR(s1p,index,0)
	mul	s2limb
	add	ADDR(rp,index,0),rax
	adc	A_x,rdx
	add	index,1
					; At this point
					;  temp = n (even)
					; index = 1

L_mpn_addmul_1_odd_n:
					; Now
					; temp = n
					; index = 1 if n even
					;       = 0 if n odd
					;
	mov	rax,ADDR(s1p,index,0)
	mul	s2limb
	mov	A_z,ADDR(rp,index,0)
	add	A_x,rax
	adc	A_y,rdx

	mov	rax,ADDR(s1p,index,1)
	mul	s2limb
	mov	B_z,ADDR(rp,index,1)
	mov	B_x,rax
	mov	B_y,rdx
	mov	rax,ADDR(s1p,index,2)

	add	index,3
	lea	s1p,ADDR(s1p,temp,-1)
	lea	rp,ADDR(rp,temp,-1)
	neg	temp
	add	index,temp
				; At this point:
				; s1p   = address of last s1limb
				; rp    = address of last rplimb
				; temp  = -n
				; index = 4 - n if n even
				;       = 3 - n if n odd
				;
				; So, index is a (negative) even
				; number.
				;
				; *****************************************
				; ATTENTION:
				;
				; From here on, I will use array
				; indexing notation in the comments
				; because it is convenient.  So, I
				; will pretend that index is positive
				; because then a comment like
				;      B_z = rp[index-1]
				; is easier to read.
				; However, keep in mind that index is
				; actually a negative number indexing
				; back from the end of the array.
				; This is a common trick to remove one
				; compare operation from the main loop.
				; *****************************************

				;
				; Now we enter a spin-up loop the
				; will make sure that the index is
				; a multiple of UNROLL_SIZE before
				; going to our main unrolled loop.
	mov	temp,index
	neg	temp
	and	temp,UNROLL_MASK
	jz	L_mpn_addmul_1_main_loop
	shr	temp,1
L_mpn_addmul_1_main_loop_spin_up:
				; At this point we should have:
				;
				; A_x = low_mul[index-2] + carry_in
				; A_y = high_mul[index-2] + CF
				; A_z = rp[index-2]
				;
				; B_x = low_mul[index-1]
				; B_y = high_mul[index-1]
				; B_z = rp[index-1]
				;
				; rax = s1p[index]
	mul	s2limb
	add	A_z,A_x
	lea	A_x,[rax]
	mov	rax,ADDR(s1p,index,1)
	adc	B_x,A_y
	mov	ADDR(rp,index,-2),A_z
	mov	A_z,ADDR(rp,index,0)
	adc	B_y,0
	lea	A_y,[rdx]
				; At this point we should have:
				;
				; B_x = low_mul[index-1] + carry_in
				; B_y = high_mul[index-1] + CF
				; B_z = rp[index-1]
				;
				; A_x = low_mul[index]
				; A_y = high_mul[index]
				; A_z = rp[index]
				;
				; rax = s1p[index+1]
	mul	s2limb
	add	B_z,B_x
	lea	B_x,[rax]
	mov	rax,ADDR(s1p,index,2)
	adc	A_x,B_y
	mov	ADDR(rp,index,-1),B_z
	mov	B_z,ADDR(rp,index,1)
	adc	A_y,0
	lea	B_y,[rdx]

	add	index,2
	sub	temp,1
	jnz	L_mpn_addmul_1_main_loop_spin_up
	
	jmp	L_mpn_addmul_1_main_loop
	
	align	16
L_mpn_addmul_1_main_loop:
				; The code here is really the same
				; logic as the spin-up loop.  It's
				; just been unrolled.
%assign	unroll_index 0
%rep	UNROLL_SIZE/2
	mul	s2limb
	add	A_z,A_x
	lea	A_x,[rax]
	mov	rax,ADDR(s1p,index,(2*unroll_index+1))
	adc	B_x,A_y
	mov	ADDR(rp,index,(2*unroll_index-2)),A_z
	mov	A_z,ADDR(rp,index,(2*unroll_index))
	adc	B_y,0
	lea	A_y,[rdx]

	mul	s2limb
	add	B_z,B_x
	lea	B_x,[rax]
	mov	rax,ADDR(s1p,index,(2*unroll_index+2))
	adc	A_x,B_y
	mov	ADDR(rp,index,(2*unroll_index-1)),B_z
	mov	B_z,ADDR(rp,index,(2*unroll_index+1))
	adc	A_y,0
	lea	B_y,[rdx]
%assign	unroll_index	unroll_index+1
%endrep

	add	index,UNROLL_SIZE
	jnz	L_mpn_addmul_1_main_loop

L_mpn_addmul_1_finish:	
				; At this point we should have:
				;
				; index = n-1
				;
				; A_x = low_mul[index-2] + carry_in
				; A_y = high_mul[index-2] + CF
				; A_z = rp[index-2]
				; 
				; B_x = low_mul[index-1]
				; B_y = high_mul[index-1]
				; B_z = rp[index-1]
				;
				; rax = s1p[index]
	mul	s2limb
	add	A_z,A_x
	mov	A_x,rax
	mov	ADDR(rp,index,-2),A_z
	mov	A_z,ADDR(rp,index,0)
	adc	B_x,A_y
	adc	B_y,0
	mov	A_y,rdx
				; At this point we should have:
				;
				; index = n-1
				;
				; B_x = low_mul[index-1] + carry_in
				; B_y = high_mul[index-1] + CF
				; B_z = rp[index-1]
				;
				; A_x = low_mul[index]
				; A_y = high_mul[index]
				; A_z = rp[index]
	add	B_z,B_x
	mov	ADDR(rp,index,-1),B_z
	adc	A_x,B_y
	adc	A_y,0
				; At this point we should have:
				;
				; index = n-1
				; 
				; A_x = low_mul[index] + carry_in
				; A_y = high_mul[index] + CF
				; A_z = rp[index]
				; 
	add	A_z,A_x
	mov	ADDR(rp,index,0),A_z
	adc	A_y,0

	mov	rax,A_y
	pop	r12
	pop	r13
	pop	r14
	pop	r15
	ret
