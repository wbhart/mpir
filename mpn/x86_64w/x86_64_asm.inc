
; Windows x64 prologue macro
;
;   prologue name, register list, stack space 
;
;   name:           routine name
;   register list:  comma separated list of registers to save
;   stack space:    stack space needed in 8 byte units

; Windows x64 epilogue macro
;
;   epilogue register list, stack space
;
;   register list:  comma separated list of registers to save
;                   in same order used in prologue
;   stack space:    stack space used in 8 byte units

%ifdef _WIN64_ABI

%macro prologue 1-*
PROC_FRAME %1
%rotate 1  
%rep %0 - 2
    push_reg  %1
    %rotate 1  
%endrep
%if ((%0 + %1) & 1) == 0
    alloc_stack 8 * (%1 + 1)
%define stack_use 8 * (%0 + %1 - 1)  
%elif %1 > 0
    alloc_stack 8 * %1
%define stack_use 8 * (%0 + %1 - 2)
%endif
END_PROLOGUE
%endmacro

%macro epilogue 1-*
%rotate -1  
%if (%0 + %1) & 1
    add rsp, 8 * (%1 + 1)
%else
    add rsp, 8 * %1     ; is this needed if %1 == 0 ?
%endif
%rep %0 - 1
    %rotate -1
    pop  %1  
%endrep
    ret
ENDPROC_FRAME
%endmacro

%else

%macro prologue 1-*
%1:
%rotate 1  
%rep %0 - 2
    push  %1
    %rotate 1  
%endrep
%if %1 > 0
    sub rsp, 8 * %1
%endif
%endmacro

%macro epilogue 1-*
%rotate -1
%if %1 > 0  
    add rsp, 8 * %1
%endif
%rep %0 - 1
    %rotate -1
    pop  %1  
%endrep
    ret
%endmacro

%endif

;  Microsoft -> GCC Calling Convention
;
; function( MSVC  GCC
;   p1       rcx  rdi
;   p2       rdx  rsi
;   p3        r8  rdx
;   p4        r9  rcx
;   p5  [rsp+40]   r8
;   p6  [rsp+48]   r9
;   ...
;   
;   save    rsi, rdi in prologue
;   mov     rdi, rcx
;   mov     rsi, rdx
;   mov     rdx, r8
;   mov     rcx, r9
;   mov     r8, [rsp + stack_use + 40]
;   mov     r9, [rsp + stack_use + 48]
;
; 32 bit parameters must be loaded into 32-bit registers