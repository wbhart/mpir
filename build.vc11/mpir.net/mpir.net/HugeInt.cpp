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

#define DEFINE_ASSIGNMENT_PROLOG(name) void Mpir##name##Expression::AssignTo(HugeInt^ destination)

#define DEFINE_UNARY_ASSIGNMENT(name, operation)       \
    DEFINE_ASSIGNMENT_PROLOG(name)                     \
{                                                      \
    operation(destination->_value, Operand->_value);   \
}

#define DEFINE_BINARY_ASSIGNMENT(name, operation, left, right)    \
    DEFINE_ASSIGNMENT_PROLOG(name)                                \
{                                                                 \
    operation(destination->_value, left, right);                  \
}

#define DEFINE_BINARY_ASSIGNMENT_EVAL_LEFT(name, operation, right)    \
    DEFINE_ASSIGNMENT_PROLOG(name)                                    \
{                                                                     \
    HugeInt temp;                                                     \
    Left->AssignTo(%temp);                                            \
    operation(destination->_value, temp._value, right);               \
}

#define DEFINE_BINARY_ASSIGNMENT_EVAL_RIGHT(name, operation, left)    \
    DEFINE_ASSIGNMENT_PROLOG(name)                                    \
{                                                                     \
    HugeInt temp;                                                     \
    Right->AssignTo(%temp);                                           \
    operation(destination->_value, left, temp._value);                \
}

#define DEFINE_BINARY_ASSIGNMENT_EVAL_BOTH(name, operation)           \
    DEFINE_ASSIGNMENT_PROLOG(name)                                    \
{                                                                     \
    HugeInt temp1;                                                    \
    HugeInt temp2;                                                    \
    Left->AssignTo(%temp1);                                           \
    Right->AssignTo(%temp2);                                          \
    operation(destination->_value, temp1._value, temp2._value);       \
}

#define DEFINE_BINARY_ASSIGNMENT_REF_REF(name, typeAbbr, operation)                                     \
    DEFINE_BINARY_ASSIGNMENT(name##typeAbbr##typeAbbr, operation, Left->_value, Right->_value)          \
    DEFINE_BINARY_ASSIGNMENT_EVAL_LEFT(name##Expr##typeAbbr, operation, Right->_value)                  \
    DEFINE_BINARY_ASSIGNMENT_EVAL_RIGHT(name##typeAbbr##Expr, operation, Left->_value)                  \
    DEFINE_BINARY_ASSIGNMENT_EVAL_BOTH(name##Expr##Expr, operation)        

#define DEFINE_BINARY_ASSIGNMENT_REF_VAL(name, leftTypeAbbr, rightTypeAbbr, operation)                  \
    DEFINE_BINARY_ASSIGNMENT(name##leftTypeAbbr##rightTypeAbbr, operation, Left->_value, Right)         \
    DEFINE_BINARY_ASSIGNMENT_EVAL_LEFT(name##Expr##rightTypeAbbr, operation, Right)

#define DEFINE_BINARY_ASSIGNMENT_VAL_REF(name, leftTypeAbbr, rightTypeAbbr, operation)                  \
    DEFINE_BINARY_ASSIGNMENT(name##leftTypeAbbr##rightTypeAbbr, operation, Left, Right->_value)         \
    DEFINE_BINARY_ASSIGNMENT_EVAL_RIGHT(name##leftTypeAbbr##Expr, operation, Left)

#define DEFINE_BINARY_ASSIGNMENT_REF_SI(name, leftTypeAbbr, rightTypeAbbr, positiveOp, negativeOp)      \
    DEFINE_ASSIGNMENT_PROLOG(name##leftTypeAbbr##rightTypeAbbr)                                         \
{                                                                                                       \
    if (Right >= 0)                                                                                     \
        positiveOp(destination->_value, Left->_value, static_cast<mpir_ui>(Right));                     \
    else                                                                                                \
        negativeOp(destination->_value, Left->_value, -static_cast<mpir_ui>(Right));                    \
}                                                                                                       \
    DEFINE_ASSIGNMENT_PROLOG(name##Expr##rightTypeAbbr)                                                 \
{                                                                                                       \
    HugeInt temp;                                                                                       \
    Left->AssignTo(%temp);                                                                              \
    if (Right >= 0)                                                                                     \
        positiveOp(destination->_value, temp._value, static_cast<mpir_ui>(Right));                      \
    else                                                                                                \
        negativeOp(destination->_value, temp._value, -static_cast<mpir_ui>(Right));                     \
}

#define DEFINE_BINARY_ASSIGNMENT_SI_REF(name, leftTypeAbbr, rightTypeAbbr, positiveOp, negativeOp1, negativeOp2)    \
    DEFINE_ASSIGNMENT_PROLOG(name##leftTypeAbbr##rightTypeAbbr)                                                     \
{                                                                                                                   \
    if (Left >= 0)                                                                                                  \
        positiveOp(destination->_value, static_cast<mpir_ui>(Left), Right->_value);                                 \
    else                                                                                                            \
    {                                                                                                               \
        negativeOp1(destination->_value, Right->_value, -static_cast<mpir_ui>(Left));                               \
        negativeOp2(destination->_value, destination->_value);                                                      \
    }                                                                                                               \
}                                                                                                                   \
    DEFINE_ASSIGNMENT_PROLOG(name##leftTypeAbbr##Expr)                                                              \
{                                                                                                                   \
    HugeInt temp;                                                                                                   \
    Right->AssignTo(%temp);                                                                                         \
    if (Left >= 0)                                                                                                  \
        positiveOp(destination->_value, static_cast<mpir_ui>(Left), temp._value);                                   \
    else                                                                                                            \
    {                                                                                                               \
        negativeOp1(destination->_value, temp._value, -static_cast<mpir_ui>(Left));                                 \
        negativeOp2(destination->_value, destination->_value);                                                      \
    }                                                                                                               \
}                                                                                     

using namespace System::Runtime::InteropServices;

namespace MPIR
{
    #pragma region construction

    HugeInt::HugeInt()
    {
        AllocateStruct();
        mpz_init(_value);
    }

    HugeInt::HugeInt(mp_bitcnt_t bits)
    {
        AllocateStruct();
        mpz_init2(_value, bits);
    }

    HugeInt::HugeInt(String^ value)
    {
        FromString(value, 10);
    }

    HugeInt::HugeInt(String^ value, int base)
    {
        FromString(value, base);
    }

    void HugeInt::FromString(String^ value, int base)
    {
        AllocateStruct();

        IntPtr ptr = Marshal::StringToHGlobalAnsi(value);
        bool success = 0 == mpz_init_set_str(_value, (char*)(void*)ptr, base);
        Marshal::FreeHGlobal(ptr);

        if(!success)
        {
            DeallocateStruct();
            throw gcnew ArgumentException("InvalidNumber", "value");
        }
    }

    HugeInt^ HugeInt::FromLong(mpir_si value)
    {
        auto result = gcnew HugeInt();
        mpz_set_si(result->_value, value);
        return result;
    }

    HugeInt^ HugeInt::FromUlong(mpir_ui value)
    {
        auto result = gcnew HugeInt();
        mpz_set_ui(result->_value, value);
        return result;
    }

    //unmanaged destructor makes the compiler implement IDisposable
    //delegates the actual destruction to the finalizer
    HugeInt::~HugeInt()
    {
        this->!HugeInt();
    }

    //managed finalizer
    HugeInt::!HugeInt()
    {
        if(_value != 0)
            DeallocateStruct();
    }

    #pragma endregion

    #pragma region object overrides

    String^ HugeInt::ToString()
    {
        return ToString(10);
    }

    String^ HugeInt::ToString(int base)
    {
        char* str = mpz_get_str(NULL, base, _value);
        String^ result = gcnew String(str);
        CustomFree(str);

        return result;
    }

    #pragma endregion

    void HugeInt::Value::set(MpirExpression^ expr)
    {
        expr->AssignTo(this);
    }

    #pragma region Arithmetic

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Add, Int, mpz_add)
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Add, Int, Ui, mpz_add_ui)
    DEFINE_BINARY_ASSIGNMENT_REF_SI (Add, Int, Si, mpz_add_ui, mpz_sub_ui)

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Subtract, Int, mpz_sub)
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Subtract, Int, Ui, mpz_sub_ui)
    DEFINE_BINARY_ASSIGNMENT_VAL_REF(Subtract, Ui, Int, mpz_ui_sub)
    DEFINE_BINARY_ASSIGNMENT_REF_SI (Subtract, Int, Si, mpz_sub_ui, mpz_add_ui)
    DEFINE_BINARY_ASSIGNMENT_SI_REF (Subtract, Si, Int, mpz_ui_sub, mpz_add_ui, mpz_neg)

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Multiply, Int, mpz_mul)
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Multiply, Int, Ui, mpz_mul_ui)
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Multiply, Int, Si, mpz_mul_si)

    DEFINE_BINARY_ASSIGNMENT_REF_VAL(ShiftLeft, Int, Bits, mpz_mul_2exp)

    DEFINE_UNARY_ASSIGNMENT(Negate, mpz_neg)
    DEFINE_UNARY_ASSIGNMENT(Abs, mpz_abs)

    #pragma endregion
};