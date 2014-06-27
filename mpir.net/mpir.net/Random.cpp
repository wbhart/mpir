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

#include "StdAfx.h"
#include "HugeInt.h"
#include "Random.h"

namespace MPIR
{
    DEFINE_ASSIGNMENT_PROLOG(Random)
    {
        IN_CONTEXT(Right);
        MP(urandomm)(destination, Left->_value, context.IntArgs[0]);
    }

    DEFINE_ASSIGNMENT_PROLOG(RandomBits)
    {
        MP(urandomb)(destination, Left->_value, Right);
    }

    DEFINE_ASSIGNMENT_PROLOG(RandomBitsChunky)
    {
        MP(rrandomb)(destination, Left->_value, Right);
    }
};
