;
; YASM macros for handling Windows Prologues and Epilogues
;
; Copyright 2008, 2009 Brian Gladman
;
; Windows x64 prologue macro
;
;   prologue name, stack_slots, register_save_list 
;
;   name:                routine name
;   register_save_list:  comma separated list of registers to save
;   stack_slots:         stack space needed in 8 byte units

; Windows x64 epilogue macro
;
;   epilogue register_save_list
;
;   register_save_list:  comma separated list of registers to save
;                        in same order used in prologue

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
; Conventions in Windows.

; Note that these macros loads integer parameters that 
; are 32-bits as 64-bit integers with no zero or sign
; extension. For such parameters either 'mov rxd, rxd'
; (for unsigned) or 'movsxd rx, rxd' (for signed) will
; be needed before they are used as 64-bit integers.  
; These macros move the first six integer parameters 
; from Microsoft ABI calling calling conventions to 
; those used by GCC: 
;
;   function(     MSVC --> GCC
;       p1,       rcx      rdi
;       p2,       rdx      rsi
;       p3,        r8      rdx
;       p4,        r9      rcx
;       p5,  [rsp+40]       r8
;       p6,  [rsp+48]       r9
;
; As already discussed, care must be taken with 32-bit 
; values in 64-bit registers or on the stack because 
; the upper 32-bits of such parameters are undefined.
;
;   WIN64_GCC_PROC name, (frame | leaf), parms
;
;   WIN64_GCC_END frame | leaf
; 
;     name    subroutine name
;     type    subroutine type (frame or leaf)
;     parms   number of parameters
;
%define leaf_fun    0
%define frame_fun   1

%macro WIN64_GCC_PROC 1-3 frame_fun, 6

    %if %3 == frame_fun

        %ifndef reg_save_list
            %define reg_save_list rsi, rdi
        %endif

        %ifndef stack_slots
            %define stack_slots 0
        %endif

        FRAME_PROC %1, stack_slots, reg_save_list
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
        
    %elif %3 == leaf_fun
        LEAF_PROC %1
    %else
        %error no (or wrong) function type defined 
    %endif

%endmacro

%macro WIN64_GCC_END 0-1 frame_fun

    %if %1 == frame_fun
        proc_end reg_save_list
    %elif %1 == leaf_fun
        ret
    %else
        %error no (or wrong) function type defined 
    %endif

%endmacro
