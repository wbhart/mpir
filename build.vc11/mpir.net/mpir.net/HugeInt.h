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

using namespace System;

#define DECLARE_VOID_FROM_MPZ(x) void x(HugeInt^ a);
#define DECLARE_VOID_FROM_UI(x) void x(mpir_ui a);
#define DECLARE_VOID_FROM_SI(x) void x(mpir_si a);

#define DECLARE_VOID_FROM_MPZ_OR_UI(x)          \
        DECLARE_VOID_FROM_MPZ(x)                \
        DECLARE_VOID_FROM_UI(x)

#define DECLARE_VOID_FROM_MPZ_OR_UI_OR_SI(x)    \
        DECLARE_VOID_FROM_MPZ(x)                \
        DECLARE_VOID_FROM_UI(x)                 \
        DECLARE_VOID_FROM_SI(x)

namespace MPIR
{
    public ref class HugeInt sealed
    {
        private:

            //fields
            int _numberOfLimbsAllocated;
            int _numberOfLimbsUsed;
            mp_limb_t* _limbs;

            //construction
            HugeInt(mpz_srcptr src);
            void FromString(String^ value, int base);

        public:
            //construction
            HugeInt();
            HugeInt(mp_bitcnt_t bits);
            HugeInt(String^ value);
            HugeInt(String^ value, int base);
            static HugeInt^ FromLong(mpir_si value);
            static HugeInt^ FromUlong(mpir_ui value);

            //disposal
            ~HugeInt();
            !HugeInt();

            //object overrides
            virtual String^ ToString() override;
            String^ ToString(int base);

            //arithmetic
            DECLARE_VOID_FROM_MPZ_OR_UI(Add)
            DECLARE_VOID_FROM_MPZ_OR_UI(Subtract)
            DECLARE_VOID_FROM_UI(SubtractFrom)
            DECLARE_VOID_FROM_MPZ_OR_UI_OR_SI(MultiplyBy)

            //void Add(HugeInt^ a);
    };
};