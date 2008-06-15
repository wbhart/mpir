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
