;  Copyright (C) 2009, 2008 Brian Gladman
;
;  All rights reserved.
;
;  Redistribution and use in source and binary forms, with or without
;  modification, are permitted provided that the following conditions are
;  met:
;  1. Redistributions of source code must retain the above copyright notice,
;  this list of conditions and the following disclaimer.
;
;  2. Redistributions in binary form must reproduce the above copyright
;  notice, this list of conditions and the following disclaimer in the
;  documentation and/or other materials provided with the distribution.
;
;  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
;  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
;  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
;  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
;  HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
;  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
;  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
;  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
;  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
;  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
;  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;

; Standardised register numbering scheme

%define r0  rax
%define r1  rdx
%define r2  rcx
%define r3  rbx
%define r4  rsi
%define r5  rdi
%define r6  rbp
%define r7  rsp

%define r0d eax
%define r1d edx
%define r2d ecx
%define r3d ebx
%define r4d esi
%define r5d edi
%define r6d ebp
%define r7d esp

%define r0w ax
%define r1w dx
%define r2w cx
%define r3w bx
%define r4w si
%define r5w di
%define r6w bp
%define r7w sp

%define r0b al
%define r1b dl
%define r2b cl
%define r3b bl
%define r4b sil
%define r5b dil
%define r6b bpl
%define r7b spl

; Standard macro for alignment (used to allow easy subsititution of
; alternative padding schemes)

%macro xalign 1
    align %1
%endmacro

; YASM macros for handling Windows Prologues and Epilogues
;
; Copyright 2008, 2009 Brian Gladman
;
; Windows x64 prologue macro
;
;   FRAME_PROC name, stack_slots, register_save_list 
;
;   name:                routine name
;   register_save_list:  comma separated list of registers to save
;   stack_slots:         stack space needed in 8 byte units

; Windows x64 epilogue macro
;
;   EXIT_PROC register_save_list
;
;   register_save_list:  comma separated list of registers to save
;                        in same order used in prologue
;
; On return the macro variable 'stack_use' gives the total number 
; of bytes used on the stack. This allows the function parameters
; to be accessed at [rsp + stack_use + 8 * n] where n starts at 1
; (for n = 1..4 this is shadow space for a register parameter)  
;
; Windows x64 epilogue and end procedure macro
;
;   END_PROC register_save_list
;
; Details as above but used at the end of a procedure to signal 
; the end of its source code.

%macro FRAME_PROC 2-*

    global  __g%1
    
%ifdef DLL
    export  __g%1
%endif

    PROC_FRAME __g%1
    %rotate 1
      
    %if %1 < 0
        %error Negative stack allocation not allowed
    %else
        %if (%0 & 1) == (%1 & 1)
            %assign stack_use 8 * (%1 + 1)
        %else
            %assign stack_use 8 * %1
        %endif
    %endif
    %rotate 1

    %if %0 > 2
        %rep %0 - 2
            push_reg  %1
            %rotate 1
        %endrep
    %endif

    %if stack_use > 0
        alloc_stack stack_use
    %endif
    
    %assign stack_use stack_use + 8 * (%0 - 2)
    END_PROLOGUE

%endmacro

%macro EXIT_PROC 0-*

    add rsp, stack_use - 8 * %0
    %if %0 > 0
        %rep %0
            %rotate -1
            pop  %1  
        %endrep
    %endif
    ret

%endmacro

%macro END_PROC 0-*

    add rsp, stack_use - 8 * %0
    %if %0 > 0
        %rep %0
            %rotate -1
            pop  %1  
        %endrep
    %endif
    ret
    ENDPROC_FRAME

%endmacro

%macro LEAF_PROC 1
    
    global  __g%1
    
%ifdef DLL
    export  __g%1
%endif

__g%1:

%endmacro

; Macros for using assembler code using the GCC Calling
; Conventions in Windows.  These macros move the first
; six integer parameters from Microsoft ABI calling
; calling conventions to those used by GCC: 
;
;   function(     MSVC --> GCC
;       p1,       rcx      rdi
;       p2,       rdx      rsi
;       p3,        r8      rdx
;       p4,        r9      rcx
;       p5,  [rsp+40]       r8
;       p6,  [rsp+48]       r9
;
;   WIN64_GCC_PROC name, n_parms, (frame | leaf)
;
;   WIN64_GCC_EXIT frame | leaf
;
;   WIN64_GCC_END  frame | leaf
; 
;     name    subroutine name
;     n_parms number of parameters (default 6)
;     type    frame or leaf function (default frame)
;
; These defines are also used:
;
;     reg_save_list   list of registers to be saved
;                     and restored
;     stack_slots     number of 8 byte slots needed
;                     on the stack (excluding the 
;                     register save/restore space)

%macro WIN64_GCC_PROC 1-3 6, frame

    %ifidn %3, frame

        %ifndef reg_save_list
            %define reg_save_list rsi, rdi
        %endif

        %ifndef stack_slots
            %define stack_slots 0
        %endif

        FRAME_PROC %1, stack_slots, reg_save_list
        
    %elifidn %3, leaf

        LEAF_PROC %1

    %else

        %error no (or wrong) function type defined 

    %endif

        %if %2 > 0
            mov     rdi, rcx
        %endif
        %if %2 > 1
            mov     rsi, rdx
        %endif
        %if %2 > 2
            mov     rdx, r8
        %endif
        %if %2 > 3
            mov     rcx, r9
        %endif
        %if %2 > 4
            mov     r8, [rsp + stack_use + 40]
        %endif
        %if %2 > 5
            mov     r9, [rsp + stack_use + 48]
        %endif
        
%endmacro

%macro WIN64_GCC_EXIT 0-1 frame

    %ifidn %1, frame
        EXIT_PROC reg_save_list
    %elifidn %1, leaf
        ret
    %else
        %error no (or wrong) function type defined 
    %endif

%endmacro

%macro WIN64_GCC_END 0-1 frame

    %ifidn %1, frame
        END_PROC reg_save_list
    %elifidn %1, leaf
        ret
    %else
        %error no (or wrong) function type defined 
    %endif

%endmacro
