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

        MPEXPR_NAME^ expr = dynamic_cast<MPEXPR_NAME^>(a);
        if(!IS_NULL(expr))
            return CompareTo(expr);

        EvaluationContext context;

        if(a->GetType() == mpir_ui::typeid)
        {
            AssignTo(context);
            return MP(cmp_ui)(CTXT(0), (mpir_ui)a, 1);
        }

        if(a->GetType() == mpir_si::typeid)
        {
            AssignTo(context);
            return MP(cmp_si)(CTXT(0), (mpir_si)a, 1);
        }

        if(a->GetType() == double::typeid)
        {
            AssignTo(context);
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

        MPEXPR_NAME^ expr = dynamic_cast<MPEXPR_NAME^>(a);
        if(!IS_NULL(expr))
            return Equals(expr);

        EvaluationContext context;

        if(a->GetType() == mpir_ui::typeid)
        {
            AssignTo(context);
            CTXT_ADD_RATIONAL((mpir_ui)a, 1);
            return MP(equal)(CTXT(0), CTXT(1)) != 0;
        }

        if(a->GetType() == mpir_si::typeid)
        {
            AssignTo(context);
            CTXT_ADD_RATIONAL((mpir_si)a, 1);
            return MP(equal)(CTXT(0), CTXT(1)) != 0;
        }

        if(a->GetType() == double::typeid)
        {
            AssignTo(context);
            CTXT_ADD_RATIONAL_DOUBLE((double)a);
            return MP(equal)(CTXT(0), CTXT(1)) != 0;
        }

        return false;
    }

    bool MPEXPR_NAME::Equals(MPEXPR_NAME^ a)
    {
        IN_CONTEXT(this, a);
        return MP(equal)(CTXT(0), CTXT(1)) != 0;
    }

    bool MPEXPR_NAME::Equals(mpir_si numerator, mpir_ui denominator)
    {
        IN_CONTEXT(this);
        CTXT_ADD_RATIONAL(numerator, denominator);
        return MP(equal)(CTXT(0), CTXT(1)) != 0;
    }

    bool MPEXPR_NAME::Equals(mpir_ui numerator, mpir_ui denominator)
    {
        IN_CONTEXT(this);
        CTXT_ADD_RATIONAL(numerator, denominator);
        return MP(equal)(CTXT(0), CTXT(1)) != 0;
    }

    #pragma endregion

    #pragma region Arithmetic

    MAKE_BINARY_OPERATOR_STANDARD  (MPEXPR_NAME,        DEFINE, +, Add, Int, Int)           
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, +, Add, Int, Ui)            
    MAKE_BINARY_OPERATOR_LLIMB_R   (MPEXPR_NAME,        DEFINE, +, Add, Int, Ui)            
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, +, Add, Int, Si)            
    MAKE_BINARY_OPERATOR_LLIMB_R   (MPEXPR_NAME,        DEFINE, +, Add, Int, Si)            
                                                                                                        
    MAKE_BINARY_OPERATOR_STANDARD  (MPEXPR_NAME,        DEFINE, -, Subtract, Int, Int)      
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, -, Subtract, Int, Ui)       
    MAKE_BINARY_OPERATOR_LLIMB     (MPEXPR_NAME,        DEFINE, -, Subtract, Int, Ui)       
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, -, Subtract, Int, Si)       
    MAKE_BINARY_OPERATOR_LLIMB     (MPEXPR_NAME,        DEFINE, -, Subtract, Int, Si)       
                                                                                                        
    MAKE_BINARY_OPERATOR_STANDARD  (MPEXPR_NAME,        DEFINE, *, Multiply, Int, Int)      
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, *, Multiply, Int, Ui)       
    MAKE_BINARY_OPERATOR_LLIMB_R   (MPEXPR_NAME,        DEFINE, *, Multiply, Int, Ui)       
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, *, Multiply, Int, Si)       
    MAKE_BINARY_OPERATOR_LLIMB_R   (MPEXPR_NAME,        DEFINE, *, Multiply, Int, Si)       
                                                                                                        
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, <<, ShiftLeft, Int, Bits)   
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, >>, ShiftRight, Int, Bits)  

    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, ^, Power, Int, Ui)          
                                                                                                           
    MAKE_UNARY_OPERATOR            (MPEXPR_NAME,        DEFINE, -, Negate, Int)             
    MAKE_VOID_FUNCTION             (MPEXPR_NAME,        DEFINE, Abs, Int)                   
    MAKE_VOID_FUNCTION             (MPEXPR_NAME,        DEFINE, Invert, Int)
                                                                                                           
    MAKE_BINARY_OPERATOR_STANDARD  (MPEXPR_NAME,        DEFINE, /, Divide, Int, Int)        
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, /, Divide, Int, Ui)       
    MAKE_BINARY_OPERATOR_LLIMB     (MPEXPR_NAME,        DEFINE, /, Divide, Int, Ui)       
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, /, Divide, Int, Si)       
    MAKE_BINARY_OPERATOR_LLIMB     (MPEXPR_NAME,        DEFINE, /, Divide, Int, Si)       
                                                                                                           
                                                                                                           
    DEFINE_UNARY_ASSIGNMENT_REF(Negate, Int, MP(neg))
    DEFINE_UNARY_ASSIGNMENT_REF(Invert, Int, MP(inv))
    DEFINE_UNARY_ASSIGNMENT_REF(Abs, Int, MP(abs))

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Add, Int, MP(add))
    DEFINE_BINARY_ASSIGNMENT_REF_RATVAL(Add, Int, Ui, MP(add))
    DEFINE_BINARY_ASSIGNMENT_REF_RATVAL(Add, Int, Si, MP(add))

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Subtract, Int, MP(sub))
    DEFINE_BINARY_ASSIGNMENT_REF_RATVAL(Subtract, Int, Ui, MP(sub))
    DEFINE_BINARY_ASSIGNMENT_RATVAL_REF(Subtract, Ui, Int, MP(sub))
    DEFINE_BINARY_ASSIGNMENT_REF_RATVAL(Subtract, Int, Si, MP(sub))
    DEFINE_BINARY_ASSIGNMENT_RATVAL_REF(Subtract, Si, Int, MP(sub))

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Multiply, Int, MP(mul))
    DEFINE_BINARY_ASSIGNMENT_REF_RATVAL(Multiply, Int, Ui, MP(mul))
    DEFINE_BINARY_ASSIGNMENT_REF_RATVAL(Multiply, Int, Si, MP(mul))

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Divide, Int, MP(div))
    DEFINE_BINARY_ASSIGNMENT_REF_RATVAL(Divide, Int, Ui, MP(div))
    DEFINE_BINARY_ASSIGNMENT_RATVAL_REF(Divide, Ui, Int, MP(div))
    DEFINE_BINARY_ASSIGNMENT_REF_RATVAL(Divide, Int, Si, MP(div))
    DEFINE_BINARY_ASSIGNMENT_RATVAL_REF(Divide, Si, Int, MP(div))

    DEFINE_BINARY_ASSIGNMENT_REF_VAL(ShiftLeft, Int, Bits, MP(mul_2exp))
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(ShiftRight, Int, Bits, MP(div_2exp))

    DEFINE_ASSIGNMENT_PROLOG(PowerIntUi)
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
        mpir_out_struct out;
        //todo
        //MP(out_raw_m)(out, _value);

        auto buffer = gcnew array<unsigned char>(chunkSize);
        auto ptr = out->written;
        auto toWrite = (int)out->writtenSize;

        while(toWrite > 0)
        {
            auto len = Math::Min(chunkSize, toWrite);
            Marshal::Copy(IntPtr(ptr), buffer, 0, len);
            stream->Write(buffer, 0, len);
            ptr += len;
            toWrite -= len;
        }

        (*__gmp_free_func)(out->allocated, out->allocatedSize);

        return out->writtenSize;
    }

    size_t MPTYPE::Read(Stream^ stream)
    {
        unsigned char csize_bytes[4];
        mpir_out_struct out;

        // 4 bytes for size
        for(int i = 0; i < 4; i++)
        {
            auto byte = stream->ReadByte();
            if(byte < 0)
                throw gcnew Exception("Unexpected end of stream");

            csize_bytes[i] = byte;
        }

        //todo
        //MP(inp_raw_p)(_value, csize_bytes, out);

        if(out->writtenSize != 0)
        {
            auto buffer = gcnew array<unsigned char>(chunkSize);
            auto ptr = out->written;
            auto toRead = (int)out->writtenSize;

            while(toRead > 0)
            {
                auto len = Math::Min(chunkSize, toRead);
                if (len != stream->Read(buffer, 0, len))
                    throw gcnew Exception("Unexpected end of stream");

                Marshal::Copy(buffer, 0, IntPtr(ptr), len);
                ptr += len;
                toRead -= len;
            }

            //todo
            //MP(inp_raw_m)(_value, out);
        }

        return 4 + out->writtenSize;
    }

    size_t MPTYPE::Write(TextWriter^ writer, int base, bool lowercase)
    {
        auto str = ToString(base, lowercase);
        writer->Write(str);
        return str->Length;
    }

    size_t MPTYPE::Read(TextReader^ reader, int base)
    {
        int c;
        size_t nread = 0;

        // Skip whitespace
        while ((c = reader->Peek()) >= 0 && Char::IsWhiteSpace(c))
        {
            nread++;
            reader->Read();
        }

        //todo
//        return ReadNoWhite(reader, base, nread);
        return 0;
    }

//#define PEEK_NEXT_CHAR  \
//    reader->Read();     \
//    c = reader->Peek(); \
//    nread++;
//
//    // adapted from inp_str, which is shared by mpq_inp_str
//    size_t MPTYPE::ReadNoWhite(TextReader^ reader, int base, size_t nread)
//    {
//        char *str;
//        size_t alloc_size, str_size;
//        bool negative = false;
//        mp_size_t xsize;
//        const unsigned char* digit_value = __gmp_digit_value_tab;
//        int c = reader->Peek();
//
//        if (base > 36)
//        {
//            // For bases > 36, use the collating sequence
//            // 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz
//            digit_value += 224;
//            if (base > 62)
//                throw gcnew ArgumentException("Invalid base", "base");
//        }
//
//        if (c == '-')
//        {
//            negative = true;
//            PEEK_NEXT_CHAR;
//        }
//
//        if (c == EOF || digit_value[c] >= (base == 0 ? 10 : base))
//            throw gcnew Exception("No digits found");
//
//        // If BASE is 0, try to find out the base by looking at the initial characters.
//        if (base == 0)
//        {
//            base = 10;
//            if (c == '0')
//            {
//                base = 8;
//                PEEK_NEXT_CHAR;
//
//                switch(c = reader->Peek())
//                {
//                    case 'x':
//                    case 'X':
//                        base = 16;
//                        PEEK_NEXT_CHAR;
//                        break;
//
//                    case 'b':
//                    case 'B':
//                        base = 2;
//                        PEEK_NEXT_CHAR;
//                }
//            }
//        }
//
//        // Skip leading zeros
//        while (c == '0')
//        {
//            PEEK_NEXT_CHAR;
//        }
//
//        alloc_size = 100;
//        str = (char *) (*__gmp_allocate_func) (alloc_size);
//        str_size = 0;
//
//        while (c != EOF)
//        {
//            int dig = digit_value[c];
//            if (dig >= base)
//                break;
//
//            if (str_size >= alloc_size)
//            {
//                size_t old_alloc_size = alloc_size;
//                alloc_size = alloc_size * 3 / 2;
//                str = (char *) (*__gmp_reallocate_func) (str, old_alloc_size, alloc_size);
//            }
//            str[str_size++] = dig;
//            reader->Read();
//            c = reader->Peek();
//        }
//        nread += str_size;
//
//        // Make sure the string is not empty, mpn_set_str would fail.
//        if (str_size == 0)
//        {
//            _value ->_mp_size = 0;
//        }
//        else
//        {
//            xsize = (((mp_size_t)
//                (str_size / __mp_bases[base].chars_per_bit_exactly))
//                / GMP_NUMB_BITS + 2);
//            MP(realloc) (_value, xsize);
//
//            // Convert the byte array in base BASE to our bignum format.
//            xsize = mpn_set_str (_value->_mp_d, (unsigned char *) str, str_size, base);
//            _value->_mp_size = (int)(negative ? -xsize : xsize);
//        }
//        (*__gmp_free_func) (str, alloc_size);
//        return nread;
//    }

    #pragma endregion
};