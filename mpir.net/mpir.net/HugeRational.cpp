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

    MPTYPE::MPTYPE(MPEXPR_NAME^ value)
    {
        AllocateStruct();
        MP(init)(_value);
        value->AssignTo(_value);
    }

    MPTYPE::MPTYPE(mp_bitcnt_t numeratorBits, mp_bitcnt_t denominatorBits)
    {
        AllocateStruct();
        mpz_init(&_value->_mp_num);
        mpz_init_set_ui(&_value->_mp_den, 1);
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

    MPTYPE^ MPTYPE::FromLong(mpir_si numerator, mpir_ui denominator)
    {
        auto result = gcnew MPTYPE();
        MP(set_si)(result->_value, numerator, denominator);
        return result;
    }

    MPTYPE^ MPTYPE::FromUlong(mpir_ui numerator, mpir_ui denominator)
    {
        auto result = gcnew MPTYPE();
        MP(set_ui)(result->_value, numerator, denominator);
        return result;
    }

    MPTYPE^ MPTYPE::FromDouble(double value)
    {
        auto result = gcnew MPTYPE();
        MP(set_d)(result->_value, value);
        return result;
    }

    #pragma endregion

    #pragma region object overrides

    String^ MPTYPE::ToString(int base, bool lowercase, int maxDigits)
    {
        auto result = gcnew StringBuilder();
        result->Append(this->Numerator->ToString(base, lowercase, maxDigits));
        result->Append('/');
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

        //todo
        //if(a->GetType() == mpir_ui::typeid)
        //{
        //    AssignTo(context);
        //    return MP(cmp_ui)(CTXT(0), (mpir_ui)a);
        //}

        //if(a->GetType() == mpir_si::typeid)
        //{
        //    AssignTo(context);
        //    return MP(cmp_si)(CTXT(0), (mpir_si)a);
        //}

        //if(a->GetType() == double::typeid)
        //{
        //    AssignTo(context);
        //    return MP(cmp_d)(CTXT(0), (double)a);
        //}

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

    MAKE_BINARY_OPERATOR_STANDARD  (MPEXPR_NAME,        DEFINE, +, Add, Int, Int)           
    //MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, +, Add, Int, Ui)            
    //MAKE_BINARY_OPERATOR_LLIMB_R   (MPEXPR_NAME,        DEFINE, +, Add, Int, Ui)            
    //MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, +, Add, Int, Si)            
    //MAKE_BINARY_OPERATOR_LLIMB_R   (MPEXPR_NAME,        DEFINE, +, Add, Int, Si)            
                                                                                                        
    MAKE_BINARY_OPERATOR_STANDARD  (MPEXPR_NAME,        DEFINE, -, Subtract, Int, Int)      
    //MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, -, Subtract, Int, Ui)       
    //MAKE_BINARY_OPERATOR_LLIMB     (MPEXPR_NAME,        DEFINE, -, Subtract, Int, Ui)       
    //MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, -, Subtract, Int, Si)       
    //MAKE_BINARY_OPERATOR_LLIMB     (MPEXPR_NAME,        DEFINE, -, Subtract, Int, Si)       
                                                                                                        
    MAKE_BINARY_OPERATOR_STANDARD  (MPEXPR_NAME,        DEFINE, *, Multiply, Int, Int)      
    //MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, *, Multiply, Int, Ui)       
    //MAKE_BINARY_OPERATOR_LLIMB_R   (MPEXPR_NAME,        DEFINE, *, Multiply, Int, Ui)       
    //MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, *, Multiply, Int, Si)       
    //MAKE_BINARY_OPERATOR_LLIMB_R   (MPEXPR_NAME,        DEFINE, *, Multiply, Int, Si)       
                                                                                                        
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, <<, ShiftLeft, Int, Bits)   
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, >>, ShiftRight, Int, Bits)  
                                                                                                           
    MAKE_UNARY_OPERATOR            (MPEXPR_NAME,        DEFINE, -, Negate, Int)             
    MAKE_VOID_FUNCTION             (MPEXPR_NAME,        DEFINE, Abs, Int)                   
    MAKE_VOID_FUNCTION             (MPEXPR_NAME,        DEFINE, Invert, Int)
                                                                                                           
    MAKE_BINARY_OPERATOR_STANDARD  (MPEXPR_NAME,        DEFINE, /, Divide, Int, Int)        
//    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, /, Divide, Int, Ui)         
                                                                                                           
                                                                                                           
    DEFINE_UNARY_ASSIGNMENT_REF(Negate, Int, MP(neg))
    DEFINE_UNARY_ASSIGNMENT_REF(Invert, Int, MP(inv))
    DEFINE_UNARY_ASSIGNMENT_REF(Abs, Int, MP(abs))

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Add, Int, MP(add))
    //DEFINE_BINARY_ASSIGNMENT_REF_VAL(Add, Int, Ui, MP(add_ui))
    //DEFINE_BINARY_ASSIGNMENT_REF_SI (Add, Int, Si, MP(add_ui), MP(sub_ui))

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Subtract, Int, MP(sub))
    //DEFINE_BINARY_ASSIGNMENT_REF_VAL(Subtract, Int, Ui, MP(sub_ui))
    //DEFINE_BINARY_ASSIGNMENT_VAL_REF(Subtract, Ui, Int, MP(ui_sub))
    //DEFINE_BINARY_ASSIGNMENT_REF_SI (Subtract, Int, Si, MP(sub_ui), MP(add_ui))
    //DEFINE_BINARY_ASSIGNMENT_SI_REF (Subtract, Si, Int, MP(ui_sub), MP(add_ui), MP(neg))

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Multiply, Int, MP(mul))
    //DEFINE_BINARY_ASSIGNMENT_REF_VAL(Multiply, Int, Ui, MP(mul_ui))
    //DEFINE_BINARY_ASSIGNMENT_REF_VAL(Multiply, Int, Si, MP(mul_si))

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Divide, Int, MP(div))
    //DEFINE_BINARY_ASSIGNMENT_REF_VAL(Divide, Int, Ui, CUSTOM_MP(div_ui))

    DEFINE_BINARY_ASSIGNMENT_REF_VAL(ShiftLeft, Int, Bits, MP(mul_2exp))
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(ShiftRight, Int, Bits, MP(div_2exp))

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