;
;  AMD64 mpn_add_err2_n, mpn_sub_err2_n
;
;  Copyright (C) 2009, David Harvey
;
;  Windows Conversion Copyright 2008 Brian Gladman
;
;  All rights reserved.
;
;  Redistribution and use in source and binary forms, with or without
;  modification, are permitted provided that the following conditions are
;  met:
;
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
;  mp_limb_t mpn_add_err2_n (mp_ptr, mp_ptr, mp_ptr, mp_ptr, mp_ptr,  mp_ptr, mp_size_t, mp_limb_t);
;  mp_limb_t mpn_sub_err2_n (mp_ptr, mp_ptr, mp_ptr, mp_ptr, mp_ptr,  mp_ptr, mp_size_t, mp_limb_t);
;  rax                          rdi     rsi     rdx     rcx       r8      r9     8(rsp)    16(rsp)
;  rax                          rcx     rdx      r8      r9 [rsp+40] [rsp+48]  [rsp+56]   [rsp+64]

%include "aors_err2_n.inc"

    CPU  Athlon64
    BITS 64

    fun mpn_sub_err2_n, sbb
    
    end
