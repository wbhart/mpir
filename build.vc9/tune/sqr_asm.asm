
%define _NEHALEM_
%undef _CORE2_

%define SQR_KARATSUBA_THRESHOLD_OVERRIDE SQR_KARATSUBA_THRESHOLD_MAX

%ifdef _NEHALEM_ 

%include "..\..\mpn\x86_64w\nehalem\sqr_basecase.asm"

%elifdef _CORE2_

%include "..\..\mpn\x86_64w\core2\sqr_basecase.asm"

%else

%error define _NEHALEM_ or _CORE2_ in sqr_asm.asm

%endif