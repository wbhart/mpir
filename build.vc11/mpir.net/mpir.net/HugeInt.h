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

typedef __mpz_struct StructType;
static const int StructSize = sizeof(StructType);

//makes a local var from the managed mpz struct data of the specified instance.
//this must be done when we're about to call an MPIR method that may realloc
//the limb data, since we're using a custom alloc scheme where the referencing MP* struct 
//is stored in the same allocated block.
//Local var must subsequently be saved with SAVE_PTR.
//all other pointers used in the same method that are "const" for MPIR
//must be wrapped in CONST_PTR
#define EDIT_PTR(x) mpz_t src_##x; src_##x[0] = *x->_value

////makes a local var from the managed mpz struct data of this instance
//#define THIS_PTR EDIT_PTR(this)

//converts from allocated limbs pointer to the mpz struct pointer the class will keep
#define ToStructPtr(x) (StructType*)((char*)x - StructSize)

//updates the managed mpz struct data for the specified instance from a local var
#define SAVE_PTR(x)                                     \
    x->_value = ToStructPtr(src_##x->_mp_d);            \
    *x->_value = src_##x[0]

////updates the managed mpz struct data for this instance from a local var
//#define SAVE_THIS SAVE_PTR(this)                                       

//wraps a "const" pointer for safe calling into MPIR methods.
//The underlying MP* structs for any editable pointers are copied to local vars.
//"const" pointers distinct from those are used unchanged;
//but if a "const" pointer is the same as one that was copied,
//it must reference the same local var because some MPIR methods compare them and
//optimize for such equality.
#define CONST_PTR(x) (x != destination ? x->_value : src_destination)

//defines a unary expression class
#define DEFINE_UNARY_EXPRESSION(name, type)                      \
public ref class Mpir##name##Expression : IMpirExpression        \
{                                                                \
    internal:                                                    \
        type Operand;                                            \
                                                                 \
    public:                                                      \
        Mpir##name##Expression(type operand)                     \
        {                                                        \
            Operand = operand;                                   \
        }                                                        \
        virtual void AssignTo(HugeInt^ destination);             \
};

//defines a binary expression class
#define DEFINE_BINARY_EXPRESSION(name, leftType, rightType)      \
public ref class Mpir##name##Expression : IMpirExpression        \
{                                                                \
    internal:                                                    \
        leftType Left;                                           \
        rightType Right;                                         \
                                                                 \
    public:                                                      \
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
    DEFINE_BINARY_EXPRESSION(AddIntSi, HugeInt^, mpir_si)

    DEFINE_BINARY_EXPRESSION(SubtractIntInt, HugeInt^, HugeInt^)
    DEFINE_BINARY_EXPRESSION(SubtractIntUi, HugeInt^, mpir_ui)
    DEFINE_BINARY_EXPRESSION(SubtractUiInt, mpir_ui, HugeInt^)
    DEFINE_BINARY_EXPRESSION(SubtractIntSi, HugeInt^, mpir_si)
    DEFINE_BINARY_EXPRESSION(SubtractSiInt, mpir_si, HugeInt^)

    DEFINE_BINARY_EXPRESSION(MultiplyIntInt, HugeInt^, HugeInt^)
    DEFINE_BINARY_EXPRESSION(MultiplyIntUi, HugeInt^, mpir_ui)
    DEFINE_BINARY_EXPRESSION(MultiplyIntSi, HugeInt^, mpir_si)

    DEFINE_BINARY_EXPRESSION(AddProductIntInt, HugeInt^, MpirMultiplyIntIntExpression^)
    DEFINE_BINARY_EXPRESSION(AddProductIntUi, HugeInt^, MpirMultiplyIntUiExpression^)
    DEFINE_BINARY_EXPRESSION(AddProductIntSi, HugeInt^, MpirMultiplyIntSiExpression^)

    DEFINE_BINARY_EXPRESSION(SubtractProductIntInt, HugeInt^, MpirMultiplyIntIntExpression^)
    DEFINE_BINARY_EXPRESSION(SubtractProductIntUi, HugeInt^, MpirMultiplyIntUiExpression^)
    DEFINE_BINARY_EXPRESSION(SubtractProductIntSi, HugeInt^, MpirMultiplyIntSiExpression^)

    DEFINE_BINARY_EXPRESSION(ShiftLeft, HugeInt^, mp_bitcnt_t)
    
    DEFINE_UNARY_EXPRESSION(Negate, HugeInt^)
    DEFINE_UNARY_EXPRESSION(Abs, HugeInt^)

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
                if(this != destination)
                {
                    EDIT_PTR(destination);
                    mpz_set(src_destination, _value);
                    SAVE_PTR(destination);
                }
            }

            //arithmetic
            static MpirAddIntIntExpression^ operator+(HugeInt^ a, HugeInt^ b) { return gcnew MpirAddIntIntExpression(a, b); }
            static MpirAddIntUiExpression^  operator+(HugeInt^ a, mpir_ui  b) { return gcnew MpirAddIntUiExpression (a, b); }
            static MpirAddIntUiExpression^  operator+(mpir_ui  a, HugeInt^ b) { return gcnew MpirAddIntUiExpression (b, a); }
            static MpirAddIntSiExpression^  operator+(HugeInt^ a, mpir_si  b) { return gcnew MpirAddIntSiExpression (a, b); }
            static MpirAddIntSiExpression^  operator+(mpir_si  a, HugeInt^ b) { return gcnew MpirAddIntSiExpression (b, a); }

            static MpirAddProductIntIntExpression^ operator+(HugeInt^ a, MpirMultiplyIntIntExpression^ b) { return gcnew MpirAddProductIntIntExpression(a, b); }
            static MpirAddProductIntIntExpression^ operator+(MpirMultiplyIntIntExpression^ a, HugeInt^ b) { return gcnew MpirAddProductIntIntExpression(b, a); }
            static MpirAddProductIntUiExpression^  operator+(HugeInt^ a, MpirMultiplyIntUiExpression^ b)  { return gcnew MpirAddProductIntUiExpression(a, b); }
            static MpirAddProductIntUiExpression^  operator+(MpirMultiplyIntUiExpression^ a, HugeInt^ b)  { return gcnew MpirAddProductIntUiExpression(b, a); }
            static MpirAddProductIntSiExpression^  operator+(HugeInt^ a, MpirMultiplyIntSiExpression^ b)  { return gcnew MpirAddProductIntSiExpression(a, b); }
            static MpirAddProductIntSiExpression^  operator+(MpirMultiplyIntSiExpression^ a, HugeInt^ b)  { return gcnew MpirAddProductIntSiExpression(b, a); }

            static MpirSubtractIntIntExpression^ operator-(HugeInt^ a, HugeInt^ b) { return gcnew MpirSubtractIntIntExpression(a, b); }
            static MpirSubtractIntUiExpression^  operator-(HugeInt^ a, mpir_ui  b) { return gcnew MpirSubtractIntUiExpression (a, b); }
            static MpirSubtractUiIntExpression^  operator-(mpir_ui  a, HugeInt^ b) { return gcnew MpirSubtractUiIntExpression (a, b); }
            static MpirSubtractIntSiExpression^  operator-(HugeInt^ a, mpir_si  b) { return gcnew MpirSubtractIntSiExpression (a, b); }
            static MpirSubtractSiIntExpression^  operator-(mpir_si  a, HugeInt^ b) { return gcnew MpirSubtractSiIntExpression (a, b); }

            static MpirSubtractProductIntIntExpression^ operator-(HugeInt^ a, MpirMultiplyIntIntExpression^ b) { return gcnew MpirSubtractProductIntIntExpression(a, b); }
            static MpirSubtractProductIntUiExpression^  operator-(HugeInt^ a, MpirMultiplyIntUiExpression^ b)  { return gcnew MpirSubtractProductIntUiExpression(a, b); }
            static MpirSubtractProductIntSiExpression^  operator-(HugeInt^ a, MpirMultiplyIntSiExpression^ b)  { return gcnew MpirSubtractProductIntSiExpression(a, b); }

            static MpirMultiplyIntIntExpression^ operator*(HugeInt^ a, HugeInt^ b) { return gcnew MpirMultiplyIntIntExpression(a, b); }
            static MpirMultiplyIntUiExpression^  operator*(HugeInt^ a, mpir_ui  b) { return gcnew MpirMultiplyIntUiExpression (a, b); }
            static MpirMultiplyIntUiExpression^  operator*(mpir_ui  a, HugeInt^ b) { return gcnew MpirMultiplyIntUiExpression (b, a); }
            static MpirMultiplyIntSiExpression^  operator*(HugeInt^ a, mpir_si  b) { return gcnew MpirMultiplyIntSiExpression (a, b); }
            static MpirMultiplyIntSiExpression^  operator*(mpir_si  a, HugeInt^ b) { return gcnew MpirMultiplyIntSiExpression (b, a); }

            static MpirShiftLeftExpression^ operator<<(HugeInt^ a, mp_bitcnt_t b) { return gcnew MpirShiftLeftExpression(a, b); }

            static MpirNegateExpression^ operator-(HugeInt^ a) { return gcnew MpirNegateExpression(a); }

            MpirAbsExpression^ Abs() { return gcnew MpirAbsExpression(this); }
    };
};