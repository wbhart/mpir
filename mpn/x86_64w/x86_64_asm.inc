
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

%ifdef _WIN64_ABI

%macro prologue 2-*

    PROC_FRAME %1
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

%macro epilogue 0-*

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
;   win64_gcc_start name, frame | leaf, no_of_parms
;
;   win64_gcc_end frame | leaf

%define leaf_proc    0
%define frame_proc   1

%macro win64_gcc_start 1-3 frame_proc, 6

    global  %1
    
    %ifdef DLL
        export %1
    %endif
    
    %if %2 == frame_proc

        %ifndef reg_save_list
            %define reg_save_list rsi, rdi
        %endif

        %ifndef stack_slots
            %define stack_slots 0
        %endif
        %if %3 != 6
        %error NO
        %endif
        ; 64-bit parameters may need truncation
        prologue %1, stack_slots, reg_save_list
        %if %3 > 0
            mov     rdi, rcx
        %endif
        %if %3 > 1
            mov     rsi, rdx
        %endif
        %if %3 > 2
            mov     rdx, r8
        %endif
        %if %3 > 3
            mov     rcx, r9
        %endif
        %if %3 > 4
            mov     r8, [rsp + stack_use + 40]
        %endif
        %if %3 > 5
            mov     r9, [rsp + stack_use + 48]
        %endif
        
    %elif %2 == leaf_proc
        %1:
    %else
        %error no (or wrong) function type defined 
    %endif

%endmacro

%macro win64_gcc_end 0-1 frame_proc

    %if %1 == frame_proc
        epilogue reg_save_list
    %elif %1 == leaf_proc
        ret
    %else
        %error no (or wrong) function type defined 
    %endif

%endmacro
