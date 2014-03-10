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
#define DECLARE_VOID_FROM_UI(x)  void x(mpir_ui a);
#define DECLARE_VOID_FROM_SI(x)  void x(mpir_si a);
#define DECLARE_VOID_FROM_2EXP(x)  void x(mp_bitcnt_t a);
#define DECLARE_VOID_FROM_NONE(x)  void x();
#define DECLARE_VOID_FROM_MPZ_MPZ(x) void x(HugeInt^ a, HugeInt^ b);
#define DECLARE_VOID_FROM_MPZ_UI(x)  void x(HugeInt^ a, mpir_ui b);
#define DECLARE_VOID_FROM_MPZ_SI(x)  void x(HugeInt^ a, mpir_si b);

#define DECLARE_VOID_FROM_MPZ_OR_UI(x)          \
        DECLARE_VOID_FROM_MPZ(x)                \
        DECLARE_VOID_FROM_UI(x)

#define DECLARE_VOID_FROM_MPZ_MPZ_OR_UI(x)          \
        DECLARE_VOID_FROM_MPZ_MPZ(x)                \
        DECLARE_VOID_FROM_MPZ_UI(x)

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
            __mpz_struct* _value;

            //construction
            HugeInt(mpz_srcptr src);
            void FromString(String^ value, int base);

        public:
            //construction
            static HugeInt();
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
            static HugeInt^ operator+(HugeInt^ destination, HugeInt^ source);
            DECLARE_VOID_FROM_MPZ_OR_UI(Add)
            DECLARE_VOID_FROM_MPZ_OR_UI(Subtract)
            DECLARE_VOID_FROM_UI(SubtractFrom)
            DECLARE_VOID_FROM_MPZ_OR_UI_OR_SI(MultiplyBy)
            DECLARE_VOID_FROM_MPZ_MPZ_OR_UI(AddProduct)
            DECLARE_VOID_FROM_MPZ_MPZ_OR_UI(SubtractProduct)
            DECLARE_VOID_FROM_2EXP(ShiftLeft)
            DECLARE_VOID_FROM_NONE(Negate)
            DECLARE_VOID_FROM_NONE(MakeAbsolute)
    };
};