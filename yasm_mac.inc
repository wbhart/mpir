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
