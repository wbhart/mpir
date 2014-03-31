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
            positiveOp(destination, destination, (mpir_ui)Right);                                       \
        else                                                                                            \
            negativeOp(destination, destination, (mpir_ui)-Right);                                      \
    }                                                                     

#define DEFINE_BINARY_ASSIGNMENT_SI_REF(name, leftTypeAbbr, rightTypeAbbr, positiveOp, negativeOp1, negativeOp2)   \
    DEFINE_ASSIGNMENT_PROLOG(name##leftTypeAbbr##rightTypeAbbr)                                                    \
    {                                                                                                              \
        Right->AssignTo(destination);                                                                              \
        if (Left >= 0)                                                                                             \
            positiveOp(destination, (mpir_ui)Left, destination);                                                   \
        else                                                                                                       \
        {                                                                                                          \
            negativeOp1(destination, destination, (mpir_ui)-Left);                                                 \
            negativeOp2(destination, destination);                                                                 \
        }                                                                                                          \
    }                                                                   

#define DEFINE_TERNARY_ASSIGNMENT_REF_REF_REF(name, typeAbbr, operation)               \
    DEFINE_ASSIGNMENT_PROLOG(name##typeAbbr##typeAbbr##typeAbbr)                       \
    {                                                                                  \
        EvaluationContext context;                                                     \
        Left->AssignTo(context);                                                       \
        Middle->AssignTo(context);                                                     \
        Right->AssignTo(context);                                                      \
        operation(destination, context.Args[0], context.Args[1], context.Args[2]);     \
    }                                                                                  \

#define DEFINE_TERNARY_ASSIGNMENT_REF_VAL_REF(name, leftT, middleT, rightT, operation) \
    DEFINE_ASSIGNMENT_PROLOG(name##leftT##middleT##rightT)                             \
    {                                                                                  \
        EvaluationContext context;                                                     \
        Left->AssignTo(context);                                                       \
        Right->AssignTo(context);                                                      \
        operation(destination, context.Args[0], Middle, context.Args[1]);              \
    }                                                                                  \

void dummy_ternary(mpz_ptr d, mpz_srcptr a, mpz_srcptr b, mpz_srcptr c) { };

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

    void HugeInt::FromString(String^ value, int base)
    {
        AllocateStruct();

        IntPtr ptr = Marshal::StringToHGlobalAnsi(value);
        bool success = 0 == mpz_init_set_str(_value, (char*)(void*)ptr, base);
        Marshal::FreeHGlobal(ptr);

        if(!success)
        {
            DeallocateStruct();
            throw gcnew ArgumentException("Invalid number", "value");
        }
    }

    void HugeInt::SetTo(String^ value, int base)
    {
        IntPtr ptr = Marshal::StringToHGlobalAnsi(value);
        bool success = 0 == mpz_set_str(_value, (char*)(void*)ptr, base);
        Marshal::FreeHGlobal(ptr);

        if(!success)
            throw gcnew ArgumentException("Invalid number", "value");
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

    HugeInt^ HugeInt::FromDouble(double value)
    {
        auto result = gcnew HugeInt();
        mpz_set_d(result->_value, value);
        return result;
    }

    HugeInt^ HugeInt::FromPower(mpir_ui value, mpir_ui power)
    {
        auto result = gcnew HugeInt();
        mpz_ui_pow_ui(result->_value, value, power);
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

    int MpirExpression::GetHashCode()
    {
        EvaluationContext context;
        AssignTo(context);

        mp_limb_t hash = 0;
        mp_limb_t* ptr = context.Args[0]->_mp_d;
        for(int i = abs(context.Args[0]->_mp_size); i > 0; i--)
            hash ^= *ptr++;

        if(context.Args[0]->_mp_size < 0)
            hash = (mp_limb_t)-(mpir_si)hash;

        return hash.GetHashCode();
    }

    #pragma endregion

    #pragma region Interface implementations

    int MpirExpression::CompareTo(Object^ a, bool& valid)
    {
        valid = true;

        if (IS_NULL(a))
            return 1;

        MpirExpression^ expr = dynamic_cast<MpirExpression^>(a);
        if(!IS_NULL(expr))
            return CompareTo(expr);

        EvaluationContext context;

        if(a->GetType() == mpir_ui::typeid)
        {
            AssignTo(context);
            return mpz_cmp_ui(context.Args[0], (mpir_ui)a);
        }

        if(a->GetType() == mpir_si::typeid)
        {
            AssignTo(context);
            return mpz_cmp_si(context.Args[0], (mpir_si)a);
        }

        if(a->GetType() == double::typeid)
        {
            AssignTo(context);
            return mpz_cmp_d(context.Args[0], (double)a);
        }

        valid = false;
        return 0;
    }

    int MpirExpression::CompareTo(Object^ a)
    {
        bool valid;
        auto result = CompareTo(a, valid);

        if (valid)
            return result;

        throw gcnew ArgumentException("Invalid argument type", "a");
    }

    int MpirExpression::CompareTo(MpirExpression^ a)
    {
        if (IS_NULL(a))
            return 1;

        EvaluationContext context;
        AssignTo(context);
        a->AssignTo(context);
        return mpz_cmp(context.Args[0], context.Args[1]);
    }

    bool MpirExpression::Equals(Object^ a)
    {
        bool valid;
        auto result = CompareTo(a, valid);

        return valid && result == 0;
    }

    bool MpirExpression::Equals(MpirExpression^ a)
    {
        return CompareTo(a) == 0;
    }

    #pragma endregion

    #pragma region expression special cases

    void MpirDivideExpression::custom_mpz_div(mpz_ptr q, mpz_srcptr n, mpz_srcptr d)
    {
        switch((rounding == RoundingModes::Default) ? MpirSettings::RoundingMode : rounding)
        {
            case RoundingModes::Floor:
                (_remainder == nullptr)
                    ? mpz_fdiv_q(q, n, d)
                    : mpz_fdiv_qr(q, _remainder->_value, n, d);
                break;

            case RoundingModes::Ceiling:
                (_remainder == nullptr)
                    ? mpz_cdiv_q(q, n, d)
                    : mpz_cdiv_qr(q, _remainder->_value, n, d);
                break;

            default:
                (_remainder == nullptr)
                    ? mpz_tdiv_q(q, n, d)
                    : mpz_tdiv_qr(q, _remainder->_value, n, d);
                break;
        }
    };

    void MpirDivideUiExpression::custom_mpz_div_ui(mpz_ptr q, mpz_srcptr n, mpir_ui d)
    {
        mpir_ui limb;

        switch((rounding == RoundingModes::Default) ? MpirSettings::RoundingMode : rounding)
        {
            case RoundingModes::Floor:
                limb = (_remainder == nullptr)
                    ? mpz_fdiv_q_ui(q, n, d)
                    : mpz_fdiv_qr_ui(q, _remainder->_value, n, d);
                break;

            case RoundingModes::Ceiling:
                limb = (_remainder == nullptr)
                    ? mpz_cdiv_q_ui(q, n, d)
                    : mpz_cdiv_qr_ui(q, _remainder->_value, n, d);
                break;

            default:
                limb = (_remainder == nullptr)
                    ? mpz_tdiv_q_ui(q, n, d)
                    : mpz_tdiv_qr_ui(q, _remainder->_value, n, d);
                break;
        }

        if(_limbRemainder != nullptr)
            _limbRemainder(limb);
    };

    void MpirShiftRightExpression::custom_mpz_div_2exp(mpz_ptr q, mpz_srcptr n, mp_bitcnt_t d)
    {
        switch((rounding == RoundingModes::Default) ? MpirSettings::RoundingMode : rounding)
        {
            case RoundingModes::Floor:
                _remainder
                    ? mpz_fdiv_r_2exp(q, n, d)
                    : mpz_fdiv_q_2exp(q, n, d);
                break;

            case RoundingModes::Ceiling:
                _remainder
                    ? mpz_cdiv_r_2exp(q, n, d)
                    : mpz_cdiv_q_2exp(q, n, d);
                break;

            default:
                _remainder
                    ? mpz_tdiv_r_2exp(q, n, d)
                    : mpz_tdiv_q_2exp(q, n, d);
                break;
        }
    };

    void MpirModExpression::custom_mpz_mod(mpz_ptr r, mpz_srcptr n, mpz_srcptr d)
    {
        switch((rounding == RoundingModes::Default) ? MpirSettings::RoundingMode : rounding)
        {
            case RoundingModes::Floor:
                (_quotient == nullptr)
                    ? mpz_fdiv_r(r, n, d)
                    : mpz_fdiv_qr(_quotient->_value, r, n, d);
                break;

            case RoundingModes::Ceiling:
                (_quotient == nullptr)
                    ? mpz_cdiv_r(r, n, d)
                    : mpz_cdiv_qr(_quotient->_value, r, n, d);
                break;

            default:
                (_quotient == nullptr)
                    ? mpz_tdiv_r(r, n, d)
                    : mpz_tdiv_qr(_quotient->_value, r, n, d);
                break;
        }
    };

    void MpirModUiExpression::custom_mpz_mod_ui(mpz_ptr r, mpz_srcptr n, mpir_ui d)
    {
        mpir_ui limb;

        switch((rounding == RoundingModes::Default) ? MpirSettings::RoundingMode : rounding)
        {
            case RoundingModes::Floor:
                limb = (_quotient == nullptr)
                    ? mpz_fdiv_r_ui(r, n, d)
                    : mpz_fdiv_qr_ui(_quotient->_value, r, n, d);
                break;

            case RoundingModes::Ceiling:
                limb = (_quotient == nullptr)
                    ? mpz_cdiv_r_ui(r, n, d)
                    : mpz_cdiv_qr_ui(_quotient->_value, r, n, d);
                break;

            default:
                limb = (_quotient == nullptr)
                    ? mpz_tdiv_r_ui(r, n, d)
                    : mpz_tdiv_qr_ui(_quotient->_value, r, n, d);
                break;
        }

        if(_limbRemainder != nullptr)
            _limbRemainder(limb);
    };

    void MpirRootExpression::custom_mpz_root(mpz_ptr dest, mpz_srcptr oper, mpir_ui power)
    {
        if(_remainder != nullptr)
            mpz_rootrem(dest, _remainder->_value, oper, power);
        else if (_exact != nullptr)
            _exact(mpz_root(dest, oper, power) != 0);
        else
            mpz_nthroot(dest, oper, power);
    };

    void MpirSquareRootExpression::custom_mpz_sqrt(mpz_ptr dest, mpz_srcptr oper)
    {
        (_remainder != nullptr)
            ? mpz_sqrtrem(dest, _remainder->_value, oper)
            : mpz_sqrt(dest, oper);
    };

    #pragma endregion

    #pragma region Arithmetic

    DEFINE_UNARY_ASSIGNMENT_REF(Negate, Int, mpz_neg)
    DEFINE_UNARY_ASSIGNMENT_REF(Abs, Int, mpz_abs)

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
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Divide, Int, Ui, custom_mpz_div_ui)
    DEFINE_BINARY_ASSIGNMENT_REF_REF(Mod, Int, custom_mpz_mod)
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Mod, Int, Ui, custom_mpz_mod_ui)

    DEFINE_BINARY_ASSIGNMENT_REF_VAL(ShiftLeft, Int, Bits, mpz_mul_2exp)
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(ShiftRight, Int, Bits, custom_mpz_div_2exp)

    DEFINE_BINARY_ASSIGNMENT_REF_REF(DivideExactly, Int, mpz_divexact)
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(DivideExactly, Int, Ui, mpz_divexact_ui)
    
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Power, Int, Ui, mpz_pow_ui)

    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Root, Int, Ui, custom_mpz_root)
    DEFINE_UNARY_ASSIGNMENT_REF     (SquareRoot, Int, custom_mpz_sqrt)

    DEFINE_TERNARY_ASSIGNMENT_REF_REF_REF(PowerMod, Int, mpz_powm);
    DEFINE_TERNARY_ASSIGNMENT_REF_VAL_REF(PowerMod, Int, Ui, Int, mpz_powm_ui)

    mpir_ui MpirExpression::Mod(mpir_ui d, RoundingModes rounding)
    {
        EvaluationContext context;
        AssignTo(context);

        switch((rounding == RoundingModes::Default) ? MpirSettings::RoundingMode : rounding)
        {
            case RoundingModes::Floor:
                return mpz_fdiv_ui(context.Args[0], d);

            case RoundingModes::Ceiling:
                return mpz_cdiv_ui(context.Args[0], d);

            default:
                return mpz_tdiv_ui(context.Args[0], d);
        }
    }

    int MpirExpression::CompareAbsTo(MpirExpression^ a)
    {
        EvaluationContext context;
        AssignTo(context);
        a->AssignTo(context);
        return mpz_cmpabs(context.Args[0], context.Args[1]);
    }

    int MpirExpression::CompareAbsTo(mpir_ui a)
    {
        EvaluationContext context;
        AssignTo(context);
        return mpz_cmpabs_ui(context.Args[0], a);
    }

    int MpirExpression::CompareAbsTo(double a)
    {
        EvaluationContext context;
        AssignTo(context);
        return mpz_cmpabs_d(context.Args[0], a);
    }

    #pragma endregion
};