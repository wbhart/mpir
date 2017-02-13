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
#include "HugeRational.h"
#include "HugeFloat.h"

namespace MPIR
{
    MPEXPR_NAME^ MpirRandom::GetFloat() { return gcnew FloatRandomExpression(this); }
    MPEXPR_NAME^ MpirRandom::GetFloatBits(mp_bitcnt_t bitCount) { return gcnew FloatRandomBitsExpression(this, bitCount); }
    MPEXPR_NAME^ MpirRandom::GetFloatChunky(mp_exp_t maxExponent) { return gcnew FloatRandomChunkyExpression(this, maxExponent); }
    MPEXPR_NAME^ MpirRandom::GetFloatLimbsChunky(mp_size_t limbCount, mp_exp_t maxExponent) { return gcnew FloatRandomLimbsChunkyExpression(this, limbCount, maxExponent); }

    DEFINE_ASSIGNMENT_PROLOG(Random)
    {
        MP(urandomb)(destination, Operand->_value, MP(get_prec)(destination));
    }

    DEFINE_ASSIGNMENT_PROLOG(RandomBits)
    {
        MP(urandomb)(destination, Left->_value, Right);
    }

    DEFINE_ASSIGNMENT_PROLOG(RandomChunky)
    {
        MP(rrandomb)(destination, Left->_value, BITS_TO_LIMBS(MP(get_prec)(destination)), Right);
    }

    //TODO investigate implementing raw IO for floats

    DEFINE_ASSIGNMENT_PROLOG(RandomLimbsChunky)
    {
        MP(rrandomb)(destination, Left->_value, Middle, Right);
    }
};
