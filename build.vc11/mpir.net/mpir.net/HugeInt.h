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

typedef __mpz_struct StructType;
static const int StructSize = sizeof(StructType);

//makes a local var from the managed mpz struct data of the specified instance
#define SRC_PTR(x) mpz_t src_##x; src_##x[0] = *x->_value

//makes a local var from the managed mpz struct data of this instance
#define THIS_PTR mpz_t src_this; src_this[0] = *_value

//converts from allocated limbs pointer to the mpz struct pointer the class will keep
#define ToStructPtr(x) (StructType*)((char*)x - StructSize)

//updates the managed mpz struct data for the specified instance from a local var
#define SAVE_PTR(x)                                     \
    x->_value = ToStructPtr(src_##x->_mp_d);            \
    *x->_value = src_##x[0]

//updates the managed mpz struct data for this instance from a local var
#define SAVE_THIS                                       \
    _value = ToStructPtr(src_this->_mp_d);              \
    *_value = src_this[0]


//defines a binary expression class
#define DEFINE_BINARY_EXPRESSION(name, leftType, rightType)      \
public ref class Mpir##name##Expression : IMpirExpression        \
{                                                                \
    public:                                                      \
        leftType Left;                                           \
        rightType Right;                                         \
                                                                 \
        Mpir##name##Expression(leftType left, rightType right)   \
        {                                                        \
            Left = left;                                         \
            Right = right;                                       \
        }                                                        \
        virtual void AssignTo(HugeInt^ destination);             \
};

namespace MPIR
{
    ref class HugeInt;

    public interface class IMpirExpression
    {
        public:
            void AssignTo(HugeInt^ destination);
    };

    DEFINE_BINARY_EXPRESSION(AddIntInt, HugeInt^, HugeInt^)
    DEFINE_BINARY_EXPRESSION(AddIntUi, HugeInt^, mpir_ui)

    DEFINE_BINARY_EXPRESSION(SubtractIntInt, HugeInt^, HugeInt^)
    DEFINE_BINARY_EXPRESSION(SubtractIntUi, HugeInt^, mpir_ui)
    DEFINE_BINARY_EXPRESSION(SubtractUiInt, mpir_ui, HugeInt^)

    DEFINE_BINARY_EXPRESSION(MultiplyIntInt, HugeInt^, HugeInt^)
    DEFINE_BINARY_EXPRESSION(MultiplyIntUi, HugeInt^, mpir_ui)
    DEFINE_BINARY_EXPRESSION(MultiplyIntSi, HugeInt^, mpir_ui)

    public ref class HugeInt sealed : IMpirExpression
    {
        internal:
            //fields
            __mpz_struct* _value;

        private:
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

            //properties
            property IMpirExpression^ Value
            {
                void set(IMpirExpression^ expr)
                {
                    expr->AssignTo(this);
                };
            }

            virtual void AssignTo(HugeInt^ destination)
            {
                SRC_PTR(destination);
                mpz_set(src_destination, _value);
                SAVE_PTR(destination);
            }

            //arithmetic
            static MpirAddIntIntExpression^ operator+(HugeInt^ a, HugeInt^ b);
            static MpirAddIntUiExpression^  operator+(HugeInt^ a, mpir_ui b);
            static MpirAddIntUiExpression^  operator+(mpir_ui a, HugeInt^ b);

            static MpirSubtractIntIntExpression^ operator-(HugeInt^ a, HugeInt^ b);
            static MpirSubtractIntUiExpression^  operator-(HugeInt^ a, mpir_ui b);
            static MpirSubtractUiIntExpression^  operator-(mpir_ui a, HugeInt^ b);

            static MpirMultiplyIntIntExpression^ operator*(HugeInt^ a, HugeInt^ b);
            static MpirMultiplyIntUiExpression^  operator*(HugeInt^ a, mpir_ui b);
            static MpirMultiplyIntUiExpression^  operator*(mpir_ui a, HugeInt^ b);
            static MpirMultiplyIntSiExpression^  operator*(HugeInt^ a, mpir_si b);
            static MpirMultiplyIntSiExpression^  operator*(mpir_si a, HugeInt^ b);

            //DECLARE_VOID_FROM_MPZ_OR_UI(Add)
            //DECLARE_VOID_FROM_MPZ_OR_UI(Subtract)
            //DECLARE_VOID_FROM_UI(SubtractFrom)
            //DECLARE_VOID_FROM_MPZ_OR_UI_OR_SI(MultiplyBy)
            DECLARE_VOID_FROM_MPZ_MPZ_OR_UI(AddProduct)
            DECLARE_VOID_FROM_MPZ_MPZ_OR_UI(SubtractProduct)
            DECLARE_VOID_FROM_2EXP(ShiftLeft)
            DECLARE_VOID_FROM_NONE(Negate)
            DECLARE_VOID_FROM_NONE(MakeAbsolute)
    };
};