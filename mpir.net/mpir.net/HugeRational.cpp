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
#include "HugeFloat.h"
#include "HugeRational.h"
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

    MPTYPE::MPTYPE(FloatExpression^ value)
    {
        AllocateStruct();
        MP(init)(_value);
        SetTo(value);
    }

    MPTYPE::MPTYPE(IntegerExpression^ numerator, IntegerExpression^ denominator)
    {
        AllocateStruct();
        MP(init)(_value);
        SetTo(numerator, denominator);
    }
    
    MPTYPE^ MPTYPE::Allocate(mp_bitcnt_t numeratorBits, mp_bitcnt_t denominatorBits)
    {
        auto result = gcnew MPTYPE(false);
        mpz_init2(&result->_value->_mp_num, numeratorBits);
        mpz_init2(&result->_value->_mp_den, denominatorBits);
        mpz_set_ui(&result->_value->_mp_den, 1);
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

    MPTYPE::MPTYPE(mpir_si numerator, mpir_ui denominator)
    {
        AllocateStruct();
        MP(init)(_value);
        MP(set_si)(_value, numerator, denominator);
    }

    MPTYPE::MPTYPE(mpir_ui numerator, mpir_ui denominator)
    {
        AllocateStruct();
        MP(init)(_value);
        MP(set_ui)(_value, numerator, denominator);
    }

    MPTYPE::MPTYPE(double value)
    {
        AllocateStruct();
        MP(init)(_value);
        MP(set_d)(_value, value);
    }

    #pragma endregion

    #pragma region object overrides

    String^ MPTYPE::ToString(int base, bool lowercase, int maxDigits)
    {
        auto result = gcnew StringBuilder();
        result->Append(this->Numerator->ToString(base, lowercase, maxDigits));
        result->Append((wchar_t)'/');
        result->Append(this->Denominator->ToString(base, lowercase, maxDigits));
        return result->ToString();
    }

    int MPEXPR_NAME::GetHashCode()
    {
        IN_CONTEXT(this);
        auto n = gcnew HugeIntComponent(&CTXT(0)->_mp_num);
        auto d = gcnew HugeIntComponent(&CTXT(0)->_mp_den);
        return n->GetHashCode() ^ d->GetHashCode();
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

        EvaluationContext context;

        WHEN_IS(2, a, IntegerExpression)
        {
            ASSIGN_TO(context);
            x2->AssignToInteger(context);
            return MP(cmp_z)(CTXT(0), CTXTI(1));
        }

        if(a->GetType() == mpir_ui::typeid)
        {
            ASSIGN_TO(context);
            return MP(cmp_ui)(CTXT(0), (mpir_ui)a, 1);
        }

        if(a->GetType() == mpir_si::typeid)
        {
            ASSIGN_TO(context);
            return MP(cmp_si)(CTXT(0), (mpir_si)a, 1);
        }

        if(a->GetType() == double::typeid)
        {
            ASSIGN_TO(context);
            CTXT_ADD_RATIONAL_DOUBLE((double)a);
            return MP(cmp)(CTXT(0), CTXT(1));
        }

        valid = false;
        return 0;
    }

    int MPEXPR_NAME::CompareTo(mpir_si numerator, mpir_ui denominator)
    {
        IN_CONTEXT(this);
        return MP(cmp_si)(CTXT(0), numerator, denominator);
    }

    int MPEXPR_NAME::CompareTo(mpir_ui numerator, mpir_ui denominator)
    {
        IN_CONTEXT(this);
        return MP(cmp_ui)(CTXT(0), numerator, denominator);
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
        if (IS_NULL(a))
            return false;

        WHEN_IS(1, a, MPEXPR_NAME)
            return Equals(x1);

        EvaluationContext context;

        WHEN_IS(2, a, IntegerExpression)
        {
            ASSIGN_TO(context);
            x2->AssignToRational(context);
            return MP(equal)(CTXT(0), CTXT(1)) != 0;
        }

        if(a->GetType() == mpir_ui::typeid)
        {
            ASSIGN_TO(context);
            CTXT_ADD_RATIONAL_UI((mpir_ui)a, 1);
            return MP(equal)(CTXT(0), CTXT(1)) != 0;
        }

        if(a->GetType() == mpir_si::typeid)
        {
            ASSIGN_TO(context);
            CTXT_ADD_RATIONAL_SI((mpir_si)a, 1);
            return MP(equal)(CTXT(0), CTXT(1)) != 0;
        }

        if(a->GetType() == double::typeid)
        {
            ASSIGN_TO(context);
            CTXT_ADD_RATIONAL_DOUBLE((double)a);
            return MP(equal)(CTXT(0), CTXT(1)) != 0;
        }

        return false;
    }

    bool MPEXPR_NAME::Equals(MPEXPR_NAME^ a)
    {
        if (IS_NULL(a))
            return false;

        IN_CONTEXT(this, a);
        return MP(equal)(CTXT(0), CTXT(1)) != 0;
    }

    bool MPEXPR_NAME::Equals(mpir_si numerator, mpir_ui denominator)
    {
        IN_CONTEXT(this);
        CTXT_ADD_RATIONAL_SI(numerator, denominator);
        return MP(equal)(CTXT(0), CTXT(1)) != 0;
    }

    bool MPEXPR_NAME::Equals(mpir_ui numerator, mpir_ui denominator)
    {
        IN_CONTEXT(this);
        CTXT_ADD_RATIONAL_UI(numerator, denominator);
        return MP(equal)(CTXT(0), CTXT(1)) != 0;
    }

    #pragma endregion

    #pragma region Arithmetic

    MAKE_BINARY_OPERATOR_STANDARD  (MPEXPR_NAME,        DEFINE, +, Add, Rat, Rat)           
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, +, Add, Rat, Ui)            
    MAKE_BINARY_OPERATOR_LLIMB_R   (MPEXPR_NAME,        DEFINE, +, Add, Rat, Ui)            
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, +, Add, Rat, Si)            
    MAKE_BINARY_OPERATOR_LLIMB_R   (MPEXPR_NAME,        DEFINE, +, Add, Rat, Si)            
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, +, Add, Rat, IExpr)            
    MAKE_BINARY_OPERATOR_LLIMB_R   (MPEXPR_NAME,        DEFINE, +, Add, Rat, IExpr)            
                                                                                                        
    MAKE_BINARY_OPERATOR_STANDARD  (MPEXPR_NAME,        DEFINE, -, Subtract, Rat, Rat)      
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, -, Subtract, Rat, Ui)       
    MAKE_BINARY_OPERATOR_LLIMB     (MPEXPR_NAME,        DEFINE, -, Subtract, Rat, Ui)       
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, -, Subtract, Rat, Si)
    MAKE_BINARY_OPERATOR_LLIMB     (MPEXPR_NAME,        DEFINE, -, Subtract, Rat, Si)
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, -, Subtract, Rat, IExpr)
    MAKE_BINARY_OPERATOR_LLIMB     (MPEXPR_NAME,        DEFINE, -, Subtract, Rat, IExpr)
                                                                                                        
    MAKE_BINARY_OPERATOR_STANDARD  (MPEXPR_NAME,        DEFINE, *, Multiply, Rat, Rat)
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, *, Multiply, Rat, Ui)
    MAKE_BINARY_OPERATOR_LLIMB_R   (MPEXPR_NAME,        DEFINE, *, Multiply, Rat, Ui)
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, *, Multiply, Rat, Si)
    MAKE_BINARY_OPERATOR_LLIMB_R   (MPEXPR_NAME,        DEFINE, *, Multiply, Rat, Si)
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, *, Multiply, Rat, IExpr)
    MAKE_BINARY_OPERATOR_LLIMB_R   (MPEXPR_NAME,        DEFINE, *, Multiply, Rat, IExpr)
                                                                                                        
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, <<, ShiftLeft, Rat, Bits)   
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, >>, ShiftRight, Rat, Bits)  

    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, ^, Power, Rat, Ui)          
                                                                                                           
    MAKE_UNARY_OPERATOR            (MPEXPR_NAME,        DEFINE, -, Negate, Rat)             
    MAKE_VOID_FUNCTION             (MPEXPR_NAME,        DEFINE, Abs, Rat)                   
    MAKE_VOID_FUNCTION             (MPEXPR_NAME,        DEFINE, Invert, Rat)
                                                                                                           
    MAKE_BINARY_OPERATOR_STANDARD  (MPEXPR_NAME,        DEFINE, /, Divide, Rat, Rat)        
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, /, Divide, Rat, Ui)       
    MAKE_BINARY_OPERATOR_LLIMB     (MPEXPR_NAME,        DEFINE, /, Divide, Rat, Ui)       
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, /, Divide, Rat, Si)       
    MAKE_BINARY_OPERATOR_LLIMB     (MPEXPR_NAME,        DEFINE, /, Divide, Rat, Si)       
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, /, Divide, Rat, IExpr)
    MAKE_BINARY_OPERATOR_LLIMB     (MPEXPR_NAME,        DEFINE, /, Divide, Rat, IExpr)
                                                                                                           
    DEFINE_UNARY_ASSIGNMENT_REF(Negate, Rat, MP(neg))
    DEFINE_UNARY_ASSIGNMENT_REF(Invert, Rat, MP(inv))
    DEFINE_UNARY_ASSIGNMENT_REF(Abs, Rat, MP(abs))

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Add, Rat, MP(add))
    DEFINE_BINARY_ASSIGNMENT_REF_RATUI(Add, Rat, Ui, MP(add))
    DEFINE_BINARY_ASSIGNMENT_REF_RATSI(Add, Rat, Si, MP(add))
    DEFINE_BINARY_ASSIGNMENT_REF_INTVAL(Add, Rat, IExpr, MP(add))

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Subtract, Rat, MP(sub))
    DEFINE_BINARY_ASSIGNMENT_REF_RATUI(Subtract, Rat, Ui, MP(sub))
    DEFINE_BINARY_ASSIGNMENT_RATUI_REF(Subtract, Ui, Rat, MP(sub))
    DEFINE_BINARY_ASSIGNMENT_REF_RATSI(Subtract, Rat, Si, MP(sub))
    DEFINE_BINARY_ASSIGNMENT_RATSI_REF(Subtract, Si, Rat, MP(sub))
    DEFINE_BINARY_ASSIGNMENT_REF_INTVAL(Subtract, Rat, IExpr, MP(sub))
    DEFINE_BINARY_ASSIGNMENT_REF_INTVAL(Subtract, IExpr, Rat, MP(sub))

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Multiply, Rat, MP(mul))
    DEFINE_BINARY_ASSIGNMENT_REF_RATUI(Multiply, Rat, Ui, MP(mul))
    DEFINE_BINARY_ASSIGNMENT_REF_RATSI(Multiply, Rat, Si, MP(mul))
    DEFINE_BINARY_ASSIGNMENT_REF_INTVAL(Multiply, Rat, IExpr, MP(mul))

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Divide, Rat, MP(div))
    DEFINE_BINARY_ASSIGNMENT_REF_RATUI(Divide, Rat, Ui, MP(div))
    DEFINE_BINARY_ASSIGNMENT_RATUI_REF(Divide, Ui, Rat, MP(div))
    DEFINE_BINARY_ASSIGNMENT_REF_RATSI(Divide, Rat, Si, MP(div))
    DEFINE_BINARY_ASSIGNMENT_RATSI_REF(Divide, Si, Rat, MP(div))
    DEFINE_BINARY_ASSIGNMENT_REF_INTVAL(Divide, Rat, IExpr, MP(div))
    DEFINE_BINARY_ASSIGNMENT_REF_INTVAL(Divide, IExpr, Rat, MP(div))

    DEFINE_BINARY_ASSIGNMENT_REF_VAL(ShiftLeft, Rat, Bits, MP(mul_2exp))
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(ShiftRight, Rat, Bits, MP(div_2exp))

    DEFINE_ASSIGNMENT_PROLOG(PowerRatUi)
    { 
        IN_CONTEXT(Left);
        mpz_pow_ui(&destination->_mp_num, &CTXT(0)->_mp_num, Right);
        mpz_pow_ui(&destination->_mp_den, &CTXT(0)->_mp_den, Right);
    }

    #pragma endregion

    #pragma region IO

    #define chunkSize 1024

    size_t MPTYPE::Write(Stream^ stream)
    {
        auto writtenNumerator = Numerator->Write(stream);
        if(writtenNumerator == 0)
            return 0;

        auto writtenDenominator = Denominator->Write(stream);
        if(writtenDenominator == 0)
            return 0;

        return writtenNumerator + writtenDenominator;
    }

    size_t MPTYPE::Read(Stream^ stream)
    {
        auto readNumerator = Numerator->Read(stream);
        if(readNumerator == 0)
            return 0;

        auto readDenominator = Denominator->Read(stream);
        if(readDenominator == 0)
            return 0;

        return readNumerator + readDenominator;
    }

    size_t MPTYPE::Write(TextWriter^ writer, int base, bool lowercase)
    {
        auto str = ToString(base, lowercase);
        writer->Write(str);
        return str->Length;
    }

    size_t MPTYPE::Read(TextReader^ reader, int base)
    {
        auto readNumerator = Numerator->Read(reader, base);
        if(readNumerator == 0)
            return 0;

        size_t readDenominator = 0;
        char c = reader->Peek();
        if (c == '/')
        {
            reader->Read();
            readDenominator = 1 + Denominator->Read(reader, base);
            if(readDenominator == 1)
                return 0;
        }

        return readNumerator + readDenominator;
    }

    #pragma endregion

    #pragma region methods in other classes with rational parameters

    void HugeInt::SetTo(MPEXPR_NAME^ value)
    {
        IN_CONTEXT(value);
        mpz_set_q(_value, CTXT(0));
    }

    void HugeFloat::SetTo(MPEXPR_NAME^ value)
    {
        IN_CONTEXT(value);
        mpf_set_q(_value, CTXT(0));
    }

    int IntegerExpression::CompareTo(Object^ a, bool& valid)
    {
        valid = true;

        if (IS_NULL(a))
            return 1;

        WHEN_IS(1, a, IntegerExpression)
            return CompareTo(x1);

        EvaluationContext context;

        WHEN_IS(2, a, MPEXPR_NAME)
        {
            x2->AssignToRational(context);
            AssignToInteger(context);
            return -MP(cmp_z)(CTXT(0), CTXTI(1));
        }

        if (a->GetType() == mpir_ui::typeid)
        {
            AssignToInteger(context);
            return mpz_cmp_ui(CTXTI(0), (mpir_ui)a);
        }

        if (a->GetType() == mpir_si::typeid)
        {
            AssignToInteger(context);
            return mpz_cmp_si(CTXTI(0), (mpir_si)a);
        }

        if (a->GetType() == double::typeid)
        {
            AssignToInteger(context);
            return mpz_cmp_d(CTXTI(0), (double)a);
        }

        valid = false;
        return 0;
    }

    #pragma endregion
};