;  AMD64 and Core 2 calling conventions checking.

;  Copyright 2000, 2003, 2004 Free Software Foundation, Inc.
;
;  Copyright 2008 Jason Worth-Martin
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


%include 'yasm_mac.inc'

%ifdef GSYM_PREFIX
%define G_EX(a) _ %+ a
%else
%define G_EX(a) a
%endif

G_EXTERN calling_conventions_save_rbx
G_EXTERN calling_conventions_save_rbp
G_EXTERN calling_conventions_save_r12
G_EXTERN calling_conventions_save_r13
G_EXTERN calling_conventions_save_r14
G_EXTERN calling_conventions_save_r15
G_EXTERN calling_conventions_function
G_EXTERN calling_conventions_rbx
G_EXTERN calling_conventions_rbp
G_EXTERN calling_conventions_r12
G_EXTERN calling_conventions_r13
G_EXTERN calling_conventions_r14
G_EXTERN calling_conventions_r15
G_EXTERN calling_conventions_rflags
G_EXTERN calling_conventions_fenv
G_EXTERN calling_conventions_retaddr


     BITS 64


; int calling_conventions (...);
; 
; The global variable "calling_conventions_function" is the function to
; call, with the arguments as passed here.
; 
; Perhaps the finit should be done only if the tags word isn't clear, but
; nothing uses the rounding mode or anything at the moment.


     default rel
     ALIGN	32
GLOBAL_FUNC calling_conventions
     mov	rax, [rsp]
     mov	[G_EX(calling_conventions_retaddr)], rax

     lea	rax, [return]
     mov	[rsp], rax

     mov	[G_EX(calling_conventions_save_rbx)], rbx
     mov	[G_EX(calling_conventions_save_rbp)], rbp
     mov	[G_EX(calling_conventions_save_r12)], r12
     mov	[G_EX(calling_conventions_save_r13)], r13
     mov	[G_EX(calling_conventions_save_r14)], r14
     mov	[G_EX(calling_conventions_save_r15)], r15

     mov	rbx, [G_EX(calling_conventions_save_rbx)]
     mov	rbp, [G_EX(calling_conventions_save_rbp)]
     mov	r12, [G_EX(calling_conventions_save_r12)]
     mov	r13, [G_EX(calling_conventions_save_r13)]
     mov	r14, [G_EX(calling_conventions_save_r14)]
     mov	r15, [G_EX(calling_conventions_save_r15)]

; values we expect to see unchanged, as per amd64check.c

     mov	rbx, 0x1234567887654321
     mov	rbp, 0x89ABCDEFFEDCBA98
     mov	r12, 0xDEADBEEFBADECAFE
     mov	r13, 0xFFEEDDCCBBAA9988
     mov	r14, 0x0011223344556677
     mov	r15, 0x1234432156788765

; Try to provoke a problem by starting with junk in the registers,
; especially %rax which will be the return value.
;
; ENHANCE-ME: If we knew how many of the parameter registers were
; actually being used we could put junk in the rest.  Maybe we could
; get try.c to communicate this to us.
; 

     mov	rax, 0xFEEDABBACAAFBEED
     mov	r10, 0xAB78DE89FF5125BB
     mov	r11, 0x1238901890189031

     jmp	[G_EX(calling_conventions_function)]

return:
     mov	[G_EX(calling_conventions_rbx)], rbx
     mov	[G_EX(calling_conventions_rbp)], rbp
     mov	[G_EX(calling_conventions_r12)], r12
     mov	[G_EX(calling_conventions_r13)], r13
     mov	[G_EX(calling_conventions_r14)], r14
     mov	[G_EX(calling_conventions_r15)], r15

     pushf
     pop	rbx
     mov	[G_EX(calling_conventions_rflags)], rbx

     fstenv	[G_EX(calling_conventions_fenv)]
     finit

     mov	rbx, [G_EX(calling_conventions_save_rbx)]
     mov	rbp, [G_EX(calling_conventions_save_rbp)]
     mov	r12, [G_EX(calling_conventions_save_r12)]
     mov	r13, [G_EX(calling_conventions_save_r13)]
     mov	r14, [G_EX(calling_conventions_save_r14)]
     mov	r15, [G_EX(calling_conventions_save_r15)]

     jmp	[G_EX(calling_conventions_retaddr)]
