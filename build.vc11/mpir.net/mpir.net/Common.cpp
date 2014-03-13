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

#include "Stdafx.h"
#include "Common.h"

static bool AllocationFunctionsConfigured = false;

void* CustomAllocate (size_t alloc_size)
{
    auto ptr = malloc(alloc_size + StructSize);
    return (char*)ptr + StructSize;
}

void* CustomReallocate (void* ptr, size_t old_size, size_t new_size)
{
    auto newPtr = realloc((char*)ptr - StructSize, new_size + StructSize);
    return (char*)newPtr + StructSize;
}

void CustomFree (void* ptr, size_t size)
{
    free((char*)ptr - StructSize);
}

void SetCustomAllocationFunctions()
{
    if(!AllocationFunctionsConfigured)
    {
        mp_set_memory_functions(CustomAllocate, CustomReallocate, CustomFree);
        AllocationFunctionsConfigured = true;
    }
}