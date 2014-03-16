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

void CustomFree (void* ptr)
{
    void (*freeFunc) (void*, size_t);
    mp_get_memory_functions (NULL, NULL, &freeFunc);
    freeFunc(ptr, 0);
}

void* CustomAllocate(size_t size)
{
    void* (*allocateFunc) (size_t);
    mp_get_memory_functions(&allocateFunc, NULL, NULL);
    return allocateFunc(size);
}

void* CustomReallocate(void* old, size_t size)
{
    void* (*reallocateFunc) (void*, size_t, size_t);
    mp_get_memory_functions(NULL, &reallocateFunc, NULL);
    return reallocateFunc(old, 0, size);
}
