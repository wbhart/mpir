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
//#include "Random.h"

using namespace System::Runtime::InteropServices;
using namespace System::Text;

namespace MPIR
{
    #pragma region construction

    MPTYPE::MPTYPE()
    {
        AllocateStruct();
        MP(init)(_value);
    }

    MPTYPE::MPTYPE(bool initialize)
    {
        AllocateStruct();
        if(initialize)
            MP(init)(_value);
    }

    MPTYPE::MPTYPE(MPEXPR_NAME^ value)
    {
        AllocateStruct();
        MP(init)(_value);
        value->AssignTo(_value);
    }

    MPTYPE::MPTYPE(IntegerExpression^ value)
    {
        AllocateStruct();
        MP(init)(_value);
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
        return result;
    }

    void MPTYPE::FromString(String^ value, int base)
    {
        AllocateStruct();
        MP(init)(_value);

        IntPtr ptr = Marshal::StringToHGlobalAnsi(value);
        bool success = 0 == MP(set_str)(_value, (char*)(void*)ptr, base);
        Marshal::FreeHGlobal(ptr);

        if(!success)
        {
            DeallocateStruct();
            throw gcnew ArgumentException("Invalid number", "value");
        }
    }

    void MPTYPE::SetTo(String^ value, int base)
    {
        IntPtr ptr = Marshal::StringToHGlobalAnsi(value);
        bool success = 0 == MP(set_str)(_value, (char*)(void*)ptr, base);
        Marshal::FreeHGlobal(ptr);

        if(!success)
            throw gcnew ArgumentException("Invalid number", "value");
    }

    MPTYPE::MPTYPE(mpir_si value)
    {
        AllocateStruct();
        MP(init_set_si)(_value, value);
    }

    MPTYPE::MPTYPE(mpir_ui value)
    {
        AllocateStruct();
        MP(init_set_ui)(_value, value);
    }

    MPTYPE::MPTYPE(double value)
    {
        AllocateStruct();
        MP(init_set_d)(_value, value);
    }

    #pragma endregion

    #pragma region object overrides

    String^ MPTYPE::ToString(int base, bool lowercase, int maxDigits)
    {
        size_t allocated;
        mp_exp_t exponent;
        bool negative = false;
        bool truncated = false;
        char* allocatedStr = NULL;
        EvaluationContext context;
        auto result = gcnew StringBuilder(maxDigits + 20);

        ASSIGN_TO(context);

        if(maxDigits > 0)
        {
            allocated = maxDigits + 2;
            allocatedStr = (char*)(*__gmp_allocate_func)(allocated);
        }
        else
        {
            allocated = 0;
        }

        allocatedStr = MP(get_str)(allocatedStr, &exponent, (base <= 36 && !lowercase) ? -base : base, maxDigits, CTXT(0));

        char* str = allocatedStr;

        if(*str == '-')
            result->Append((wchar_t)*str++);
        
        result->Append((wchar_t)'0');
        if (*str != 0)
        {
            result->Append((wchar_t)'.');

            while (*str != 0)
                result->Append((wchar_t)*str++);
        }

        if (exponent != 0)
            result->Append((wchar_t)'@')->Append(exponent);

        if (allocated == 0)
            allocated = str - allocatedStr + 1;

        (*__gmp_free_func)(allocatedStr, allocated);

        return result->ToString();
    }

    int MPEXPR_NAME::GetHashCode()
    {
        IN_CONTEXT(this);

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

        MPEXPR_NAME^ expr = dynamic_cast<MPEXPR_NAME^>(a);
        if(!IS_NULL(expr))
            return CompareTo(expr);

        EvaluationContext context;

        if(a->GetType() == mpir_ui::typeid)
        {
            ASSIGN_TO(context);
            return MP(cmp_ui)(CTXT(0), (mpir_ui)a);
        }

        if(a->GetType() == mpir_si::typeid)
        {
            ASSIGN_TO(context);
            return MP(cmp_si)(CTXT(0), (mpir_si)a);
        }

        if(a->GetType() == double::typeid)
        {
            ASSIGN_TO(context);
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

        IN_CONTEXT(this, a);
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
                                                                                                           
    MAKE_BINARY_OPERATOR_STANDARD  (MPEXPR_NAME,        DEFINE, /, Divide, Flt, Flt)        
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, /, Divide, Flt, Ui)       
    MAKE_BINARY_OPERATOR_LLIMB     (MPEXPR_NAME,        DEFINE, /, Divide, Flt, Ui)       
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, /, Divide, Flt, Si)       
    MAKE_BINARY_OPERATOR_LLIMB     (MPEXPR_NAME,        DEFINE, /, Divide, Flt, Si)       
                                                                                                           
    DEFINE_UNARY_ASSIGNMENT_REF(Negate, Flt, MP(neg))
    DEFINE_UNARY_ASSIGNMENT_REF(Abs, Flt, MP(abs))

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

    #pragma endregion

    #pragma region IO

    #define chunkSize 1024

    size_t MPTYPE::Write(Stream^ stream)
    {
        throw gcnew NotImplementedException();
    }

    size_t MPTYPE::Read(Stream^ stream)
    {
        throw gcnew NotImplementedException();
    }

    size_t MPTYPE::Write(TextWriter^ writer, int base, bool lowercase)
    {
        auto str = ToString(base, lowercase);
        writer->Write(str);
        return str->Length;
    }

    size_t MPTYPE::Read(TextReader^ reader, int base)
    {
        throw gcnew NotImplementedException();
    }

    #pragma endregion

    #pragma region methods in other classes with float parameters

    void HugeInt::SetTo(MPEXPR_NAME^ value)
    {
        IN_CONTEXT(value);
        mpz_set_f(_value, CTXT(0));
    }

    void HugeRational::SetTo(MPEXPR_NAME^ value)
    {
        IN_CONTEXT(value);
        mpq_set_f(_value, CTXT(0));
    }

    #pragma endregion
};