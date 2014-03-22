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

#define DEFINE_ASSIGNMENT_PROLOG(name) void Mpir##name##Expression::AssignTo(mpz_ptr destination)

#define DEFINE_UNARY_ASSIGNMENT_REF(name, typeAbbr, operation)     \
    DEFINE_ASSIGNMENT_PROLOG(name##typeAbbr)                       \
    {                                                              \
        Operand->AssignTo(destination);                            \
        operation(destination, destination);                       \
    }

#define DEFINE_BINARY_ASSIGNMENT_REF_REF(name, typeAbbr, operation)   \
    DEFINE_ASSIGNMENT_PROLOG(name##typeAbbr##typeAbbr)                \
    {                                                                 \
        EvaluationContext context;                                    \
        Left->AssignTo(context);                                      \
        Right->AssignTo(context);                                     \
        operation(destination, context.Args[0], context.Args[1]);     \
    }                                                                 \

#define DEFINE_BINARY_ASSIGNMENT_REF_VAL(name, leftTypeAbbr, rightTypeAbbr, operation)    \
    DEFINE_ASSIGNMENT_PROLOG(name##leftTypeAbbr##rightTypeAbbr)                           \
    {                                                                                     \
        Left->AssignTo(destination);                                                      \
        operation(destination, destination, Right);                                       \
    }

#define DEFINE_BINARY_ASSIGNMENT_VAL_REF(name, leftTypeAbbr, rightTypeAbbr, operation)    \
    DEFINE_ASSIGNMENT_PROLOG(name##leftTypeAbbr##rightTypeAbbr)                           \
    {                                                                                     \
        Right->AssignTo(destination);                                                     \
        operation(destination, Left, destination);                                        \
    }

#define DEFINE_BINARY_ASSIGNMENT_REF_SI(name, leftTypeAbbr, rightTypeAbbr, positiveOp, negativeOp)      \
    DEFINE_ASSIGNMENT_PROLOG(name##leftTypeAbbr##rightTypeAbbr)                                         \
    {                                                                                                   \
        Left->AssignTo(destination);                                                                    \
        if (Right >= 0)                                                                                 \
            positiveOp(destination, destination, static_cast<mpir_ui>(Right));                          \
        else                                                                                            \
            negativeOp(destination, destination, -static_cast<mpir_ui>(Right));                         \
    }                                                                     

#define DEFINE_BINARY_ASSIGNMENT_SI_REF(name, leftTypeAbbr, rightTypeAbbr, positiveOp, negativeOp1, negativeOp2)   \
    DEFINE_ASSIGNMENT_PROLOG(name##leftTypeAbbr##rightTypeAbbr)                                                    \
    {                                                                                                              \
        Right->AssignTo(destination);                                                                              \
        if (Left >= 0)                                                                                             \
            positiveOp(destination, static_cast<mpir_ui>(Left), destination);                                      \
        else                                                                                                       \
        {                                                                                                          \
            negativeOp1(destination, destination, -static_cast<mpir_ui>(Left));                                    \
            negativeOp2(destination, destination);                                                                 \
        }                                                                                                          \
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
        expr->AssignTo(_value);
    }

    #pragma region expression special cases

    void MpirDivideExpression::custom_mpz_div(mpz_ptr q, mpz_srcptr n, mpz_srcptr d)
    {
        switch((rounding == RoundingModes::Default) ? MpirSettings::RoundingMode : rounding)
        {
            case RoundingModes::Floor:
                (remainder == nullptr)
                    ? mpz_fdiv_q(q, n, d)
                    : mpz_fdiv_qr(q, remainder->_value, n, d);
                break;

            case RoundingModes::Ceiling:
                (remainder == nullptr)
                    ? mpz_cdiv_q(q, n, d)
                    : mpz_cdiv_qr(q, remainder->_value, n, d);
                break;

            default:
                (remainder == nullptr)
                    ? mpz_tdiv_q(q, n, d)
                    : mpz_tdiv_qr(q, remainder->_value, n, d);
                break;
        }
    };

    void MpirModExpression::custom_mpz_mod(mpz_ptr r, mpz_srcptr n, mpz_srcptr d)
    {
        switch((rounding == RoundingModes::Default) ? MpirSettings::RoundingMode : rounding)
        {
            case RoundingModes::Floor:
                (quotient == nullptr)
                    ? mpz_fdiv_r(r, n, d)
                    : mpz_fdiv_qr(quotient->_value, r, n, d);
                break;

            case RoundingModes::Ceiling:
                (quotient == nullptr)
                    ? mpz_cdiv_r(r, n, d)
                    : mpz_cdiv_qr(quotient->_value, r, n, d);
                break;

            default:
                (quotient == nullptr)
                    ? mpz_tdiv_r(r, n, d)
                    : mpz_tdiv_qr(quotient->_value, r, n, d);
                break;
        }
    };

    #pragma endregion

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

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Divide, Int, custom_mpz_div)
    DEFINE_BINARY_ASSIGNMENT_REF_REF(Mod, Int, custom_mpz_mod)

    DEFINE_BINARY_ASSIGNMENT_REF_VAL(ShiftLeft, Int, Bits, mpz_mul_2exp)

    DEFINE_UNARY_ASSIGNMENT_REF(Negate, Int, mpz_neg)
    DEFINE_UNARY_ASSIGNMENT_REF(Abs, Int, mpz_abs)

    #pragma endregion
};