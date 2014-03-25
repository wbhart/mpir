/*
Copyright 2014 Alex Dyachenko

This file is part of the MPIR Library.

The MPIR Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation; either version 3 of the License, or (at
your option) any later version.

The MPIR Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPIR Library.  If not, see http://www.gnu.org/licenses/.  
*/

#pragma once

void* CustomAllocate (size_t alloc_size);
void* CustomReallocate (void* ptr, size_t new_size);
void CustomFree (void* ptr);

enum EvaluationOptions : __int8
{
    None = 0x0,
    Temp1Initialized = 0x1,
    Temp2Initialized = 0x2,
    Temp3Initialized = 0x4,
};

struct EvaluationContext
{
    public:
        __mpz_struct Temp[3];
        mpz_ptr Args[3];
        union
        {
            struct
            {
                unsigned __int8 Index;
                EvaluationOptions Options;
            };
            __int64 Zero;
        };

        EvaluationContext()
        {
            Zero = 0;
        }

        ~EvaluationContext()
        {
            if(Options & EvaluationOptions::Temp1Initialized)
                mpz_clear(Args[0]);
            if(Options & EvaluationOptions::Temp2Initialized)
                mpz_clear(Args[1]);
            if(Options & EvaluationOptions::Temp3Initialized)
                mpz_clear(Args[2]);
        }
};