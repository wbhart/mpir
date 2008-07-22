%macro G_LABEL 1
%ifdef GSYM_PREFIX
_%1:
%else
%1:
%endif
%endmacro

%macro G_EXPORT 1
%ifdef GSYM_PREFIX
    global _%1:function
%else
    global %1:function
%endif
%endmacro

%macro G_EXTERN 1
%ifdef GSYM_PREFIX
    extern _%1
%else
    extern %1
%endif
%endmacro

%macro GLOBAL_FUNC 1
G_EXPORT __g%1
G_EXPORT %1
G_LABEL __g%1
G_LABEL %1
%endmacro
