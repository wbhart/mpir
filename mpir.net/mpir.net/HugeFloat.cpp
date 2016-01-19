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
#include "HugeInt.h"
#include "HugeRational.h"
#include "HugeFloat.h"

using namespace System::Runtime::InteropServices;
using namespace System::Text;

namespace MPIR
{
    #pragma region construction

    MPTYPE::MPTYPE()
    {
        AllocateStruct();
        MP(init)(_value);
        _allocatedPrecision = MP(get_prec)(_value);
    }

    MPTYPE::MPTYPE(bool initialize)
    {
        AllocateStruct();
        if(initialize)
        {
            MP(init)(_value);
            _allocatedPrecision = MP(get_prec)(_value);
        }
    }

    MPTYPE::MPTYPE(MPEXPR_NAME^ value)
    {
        AllocateStruct();
        MP(init)(_value);
        _allocatedPrecision = MP(get_prec)(_value);
        value->AssignTo(_value);
    }

    MPTYPE::MPTYPE(IntegerExpression^ value)
    {
        AllocateStruct();
        MP(init)(_value);
        _allocatedPrecision = MP(get_prec)(_value);
        SetTo(value);
    }

    MPTYPE::MPTYPE(RationalExpression^ value)
    {
        AllocateStruct();
        MP(init)(_value);
        SetTo(value);
    }
    
    MPTYPE^ MPTYPE::Allocate(mp_bitcnt_t precision)
    {
        auto result = gcnew MPTYPE(false);
        MP(init2)(result->_value, precision);
        result->_allocatedPrecision = MP(get_prec)(result->_value);
        return result;
    }

    void MPTYPE::FromString(String^ value, int base, bool exponentInDecimal)
    {
        AllocateStruct();
        MP(init)(_value);
        _allocatedPrecision = MP(get_prec)(_value);

        base = Math::Abs(base);
        IntPtr ptr = Marshal::StringToHGlobalAnsi(value);
        bool success = 0 == MP(set_str)(_value, (char*)(void*)ptr, exponentInDecimal ? -base : base);
        Marshal::FreeHGlobal(ptr);

        if(!success)
        {
            DeallocateStruct();
            throw gcnew ArgumentException("Invalid number", "value");
        }
    }

    void MPTYPE::SetTo(String^ value, int base, bool exponentInDecimal)
    {
        base = Math::Abs(base);
        IntPtr ptr = Marshal::StringToHGlobalAnsi(value);
        bool success = 0 == MP(set_str)(_value, (char*)(void*)ptr, exponentInDecimal ? -base : base);
        Marshal::FreeHGlobal(ptr);

        if(!success)
            throw gcnew ArgumentException("Invalid number", "value");
    }

    MPTYPE::MPTYPE(mpir_si value)
    {
        AllocateStruct();
        MP(init_set_si)(_value, value);
        _allocatedPrecision = MP(get_prec)(_value);
    }

    MPTYPE::MPTYPE(mpir_ui value)
    {
        AllocateStruct();
        MP(init_set_ui)(_value, value);
        _allocatedPrecision = MP(get_prec)(_value);
    }

    MPTYPE::MPTYPE(double value)
    {
        AllocateStruct();
        MP(init_set_d)(_value, value);
        _allocatedPrecision = MP(get_prec)(_value);
    }

    #pragma endregion

    #pragma region object overrides

    String^ MPTYPE::ToString(int base, bool lowercase, int maxDigits, bool exponentInDecimal)
    {
        if(base < 2 || base > 62)
            throw gcnew ArgumentOutOfRangeException("base", "Invalid base");

        mp_exp_t exponent;
        auto allocatedStr = MP(get_str)(NULL, &exponent, (!lowercase && base <= 36) ? -base : base, maxDigits, _value);
        auto str = allocatedStr;

        auto result = maxDigits > 0 ? gcnew StringBuilder(maxDigits + 70) : gcnew StringBuilder();
        
        size_t allocated = 1;
        if (*str == '-')
        {
            result->Append((wchar_t)'-');
            allocated++;
            str++;
        }
        result->Append((wchar_t)'0');

        if (*str != 0)
        {
            result->Append((wchar_t)'.');
            while (*str != 0)
            {
                result->Append((wchar_t)*str);
                allocated++;
                str++;
            }
            result->Append((wchar_t)'@');

            if(exponentInDecimal)
            {
                result->Append(exponent);
            }
            else
            {
                HugeInt exp((mpir_si) exponent);
                result->Append(exp.ToString(base, lowercase));
            }
        }

        (*__gmp_free_func)(allocatedStr, allocated);

        return result->ToString();
    }

    int MPEXPR_NAME::GetHashCode()
    {
        IN_DEFAULT_CONTEXT(this);

        mp_limb_t hash = CTXT(0)->_mp_exp;
        mp_limb_t* ptr = CTXT(0)->_mp_d;
        for(int i = abs(CTXT(0)->_mp_size); i > 0; i--)
            hash ^= *ptr++;

        if(CTXT(0)->_mp_size < 0)
            hash = (mp_limb_t)-(mpir_si)hash;

        return hash.GetHashCode();
    }

    #pragma endregion

    #pragma region Interface implementations

    int MPEXPR_NAME::CompareTo(Object^ a, bool& valid)
    {
        valid = true;

        if (IS_NULL(a))
            return 1;

        WHEN_IS(1, a, MPEXPR_NAME)
            return CompareTo(x1);

        auto f = dynamic_cast<MPTYPE^>(this);
        auto precision = IS_NULL(f) ? MPTYPE::DefaultPrecision : f->Precision;

        if(a->GetType() == mpir_ui::typeid)
        {
            IN_SPECIFIC_CONTEXT(precision, this);
            return MP(cmp_ui)(CTXT(0), (mpir_ui)a);
        }

        if(a->GetType() == mpir_si::typeid)
        {
            IN_SPECIFIC_CONTEXT(precision, this);
            return MP(cmp_si)(CTXT(0), (mpir_si)a);
        }

        if(a->GetType() == double::typeid)
        {
            IN_SPECIFIC_CONTEXT(precision, this);
            return MP(cmp_d)(CTXT(0), (double)a);
        }

        valid = false;
        return 0;
    }

    int MPEXPR_NAME::CompareTo(Object^ a)
    {
        bool valid;
        auto result = CompareTo(a, valid);

        if (valid)
            return result;

        throw gcnew ArgumentException("Invalid argument type", "a");
    }

    int MPEXPR_NAME::CompareTo(MPEXPR_NAME^ a)
    {
        if (IS_NULL(a))
            return 1;

        auto f = dynamic_cast<MPTYPE^>(this);
        if (IS_NULL(f)) f = dynamic_cast<MPTYPE^>(a);
        auto precision = IS_NULL(f) ? MPTYPE::DefaultPrecision : f->Precision;

        IN_SPECIFIC_CONTEXT(precision, this, a);
        return MP(cmp)(CTXT(0), CTXT(1));
    }

    bool MPEXPR_NAME::Equals(Object^ a)
    {
        bool valid;
        auto result = CompareTo(a, valid);

        return valid && result == 0;
    }

    bool MPEXPR_NAME::Equals(MPEXPR_NAME^ a)
    {
        return CompareTo(a) == 0;
    }

    #pragma endregion

    #pragma region Arithmetic

    MAKE_BINARY_OPERATOR_STANDARD  (MPEXPR_NAME,        DEFINE, +, Add, Flt, Flt)           
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, +, Add, Flt, Ui)            
    MAKE_BINARY_OPERATOR_LLIMB_R   (MPEXPR_NAME,        DEFINE, +, Add, Flt, Ui)            
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, +, Add, Flt, Si)            
    MAKE_BINARY_OPERATOR_LLIMB_R   (MPEXPR_NAME,        DEFINE, +, Add, Flt, Si)            
                                                                                                        
    MAKE_BINARY_OPERATOR_STANDARD  (MPEXPR_NAME,        DEFINE, -, Subtract, Flt, Flt)      
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, -, Subtract, Flt, Ui)       
    MAKE_BINARY_OPERATOR_LLIMB     (MPEXPR_NAME,        DEFINE, -, Subtract, Flt, Ui)       
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, -, Subtract, Flt, Si)
    MAKE_BINARY_OPERATOR_LLIMB     (MPEXPR_NAME,        DEFINE, -, Subtract, Flt, Si)
                                                                                                        
    MAKE_BINARY_OPERATOR_STANDARD  (MPEXPR_NAME,        DEFINE, *, Multiply, Flt, Flt)
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, *, Multiply, Flt, Ui)
    MAKE_BINARY_OPERATOR_LLIMB_R   (MPEXPR_NAME,        DEFINE, *, Multiply, Flt, Ui)
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, *, Multiply, Flt, Si)
    MAKE_BINARY_OPERATOR_LLIMB_R   (MPEXPR_NAME,        DEFINE, *, Multiply, Flt, Si)
                                                                                                        
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, <<, ShiftLeft, Flt, Bits)   
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, >>, ShiftRight, Flt, Bits)  

    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, ^, Power, Flt, Ui)          
                                                                                                           
    MAKE_UNARY_OPERATOR            (MPEXPR_NAME,        DEFINE, -, Negate, Flt)             
    MAKE_VOID_FUNCTION             (MPEXPR_NAME,        DEFINE, Abs, Flt)                   
    MAKE_VOID_FUNCTION             (MPEXPR_NAME,        DEFINE, Floor, Flt)                   
    MAKE_VOID_FUNCTION             (MPEXPR_NAME,        DEFINE, Ceiling, Flt)                   
    MAKE_VOID_FUNCTION             (MPEXPR_NAME,        DEFINE, Truncate, Flt)                   
    MAKE_VOID_FUNCTION             (MPEXPR_NAME,        DEFINE, SquareRoot, Flt)                   
    MAKE_FUNCTION_WITH_ONE         (MPEXPR_NAME,        DEFINE, RelativeDifferenceFrom, Flt)                   
                                                                                                           
    MAKE_BINARY_OPERATOR_STANDARD  (MPEXPR_NAME,        DEFINE, /, Divide, Flt, Flt)        
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, /, Divide, Flt, Ui)       
    MAKE_BINARY_OPERATOR_LLIMB     (MPEXPR_NAME,        DEFINE, /, Divide, Flt, Ui)       
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, /, Divide, Flt, Si)       
    MAKE_BINARY_OPERATOR_LLIMB     (MPEXPR_NAME,        DEFINE, /, Divide, Flt, Si)       
                                                                                                           
    DEFINE_UNARY_ASSIGNMENT_REF(Negate, Flt, MP(neg))
    DEFINE_UNARY_ASSIGNMENT_REF(Abs, Flt, MP(abs))
    DEFINE_UNARY_ASSIGNMENT_REF(Floor, Flt, MP(floor))
    DEFINE_UNARY_ASSIGNMENT_REF(Ceiling, Flt, MP(ceil))
    DEFINE_UNARY_ASSIGNMENT_REF(Truncate, Flt, MP(trunc))
    DEFINE_UNARY_ASSIGNMENT_REF(SquareRoot, Flt, MP(sqrt))
    DEFINE_UNARY_ASSIGNMENT_VAL(SquareRoot, Ui, MP(sqrt_ui))

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Add, Flt, MP(add))
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Add, Flt, Ui, MP(add_ui))
    DEFINE_BINARY_ASSIGNMENT_REF_SI (Add, Flt, Si, MP(add_ui), MP(sub_ui))

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Subtract, Flt, MP(sub))
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Subtract, Flt, Ui, MP(sub_ui))
    DEFINE_BINARY_ASSIGNMENT_VAL_REF(Subtract, Ui, Flt, MP(ui_sub))
    DEFINE_BINARY_ASSIGNMENT_REF_SI (Subtract, Flt, Si, MP(sub_ui), MP(add_ui))
    DEFINE_BINARY_ASSIGNMENT_SI_REF (Subtract, Si, Flt, MP(ui_sub), MP(add_ui), MP(neg))

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Multiply, Flt, MP(mul))
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Multiply, Flt, Ui, MP(mul_ui))
    DEFINE_BINARY_ASSIGNMENT_REF_SI2(Multiply, Flt, Si, MP(mul_ui), MP(neg))

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Divide, Flt, MP(div))
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Divide, Flt, Ui, MP(div_ui))
    DEFINE_BINARY_ASSIGNMENT_VAL_REF(Divide, Ui, Flt, MP(ui_div))
    DEFINE_BINARY_ASSIGNMENT_REF_SI2(Divide, Flt, Si, MP(div_ui), MP(neg))
    DEFINE_BINARY_ASSIGNMENT_SI_REF2(Divide, Si, Flt, MP(ui_div), MP(ui_div), MP(neg))

    DEFINE_BINARY_ASSIGNMENT_REF_VAL(ShiftLeft, Flt, Bits, MP(mul_2exp))
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(ShiftRight, Flt, Bits, MP(div_2exp))

    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Power, Flt, Ui, MP(pow_ui))
    DEFINE_BINARY_ASSIGNMENT_REF_REF(RelativeDifferenceFrom, Flt, MP(reldiff))

    int MPEXPR_NAME::Sign()
    {
        IN_DEFAULT_CONTEXT(this); 
        return MP(sgn)(CTXT(0));
    }

    #pragma endregion

    #pragma region IO

    size_t MPTYPE::Write(TextWriter^ writer, int base, int maxDigits, bool lowercase, bool exponentInDecimal)
    {
        auto str = ToString(base, lowercase, maxDigits, exponentInDecimal);
        writer->Write(str);
        return str->Length;
    }

    size_t MPTYPE::Read(TextReader^ reader, int base, bool exponentInDecimal)
    {
        StringBuilder str;
        
        int c;
        size_t nread = 0;
        const unsigned char* digit_value = __gmp_digit_value_tab;
        bool hasDecimal = false;
        bool inExponent = false;

        if (base < 2)
            throw gcnew ArgumentException("Invalid base", "base");

        if (base > 36)
        {
            // For bases > 36, use the collating sequence
            // 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz
            digit_value += 224;
            if (base > 62)
                throw gcnew ArgumentException("Invalid base", "base");
        }

        // Skip whitespace
        while ((c = reader->Peek()) >= 0 && Char::IsWhiteSpace(c))
        {
            nread++;
            reader->Read();
        }

        // possibly negative
        if (c == '-')
        {
            str.Append((wchar_t)c);
            PEEK_NEXT_CHAR;
        }

        while (c != EOF)
        {
            if (c == '.')
            {
                if (hasDecimal) break;
                
                hasDecimal = true;
                str.Append((wchar_t)c);
                PEEK_NEXT_CHAR;
                continue;
            }

            if ((base <= 10 && Char::ToLower(c) == 'e') || c == '@')
            {
                if (inExponent) break;
                
                inExponent = true;
                str.Append((wchar_t)c);
                PEEK_NEXT_CHAR;

                // possibly negative
                if (c == '-')
                {
                    str.Append((wchar_t)c);
                    PEEK_NEXT_CHAR;
                }
                continue;
            }

            if (inExponent && exponentInDecimal && !Char::IsDigit(c))
                break;

            int dig = digit_value[c];
            if (dig >= base)
                break;

            str.Append((wchar_t)c);
            PEEK_NEXT_CHAR;
        }

        SetTo(str.ToString(), base, exponentInDecimal);
        return str.Length + nread;
    }

    #pragma endregion

    #pragma region methods in other classes with float parameters

    void HugeInt::SetTo(MPEXPR_NAME^ value)
    {
        IN_DEFAULT_CONTEXT(value);
        mpz_set_f(_value, CTXT(0));
    }

    void HugeRational::SetTo(MPEXPR_NAME^ value)
    {
        IN_DEFAULT_CONTEXT(value);
        mpq_set_f(_value, CTXT(0));
    }

    #pragma endregion
};