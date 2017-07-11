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
#include "Random.h"

using namespace System::Runtime::InteropServices;
using namespace System::Text;

__mpz_struct HugeIntConst1;

namespace MPIR
{
    #pragma region MpirSettings

    void MpirSettings::ToStringDigits::set(int value)
    {
        _toStringDigits = value;
        _toStringModulo = gcnew MPTYPE(MPTYPE::Power(10, value));
    }

    #pragma endregion

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

    MPTYPE^ MPTYPE::Allocate(mp_bitcnt_t bits)
    {
        auto result = gcnew MPTYPE(false);
        MP(init2)(result->_value, bits);
        return result;
    }

    void MPTYPE::FromString(String^ value, int base)
    {
        AllocateStruct();

        IntPtr ptr = Marshal::StringToHGlobalAnsi(value);
        bool success = 0 == MP(init_set_str)(_value, (char*)(void*)ptr, base);
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
        bool negative = false;
        bool truncated = false;

        EvaluationContext context;
        if(maxDigits > 0 && CompareAbsTo(MpirSettings::_toStringModulo) >= 0)
        {
            (this->Abs() % MpirSettings::_toStringModulo)->Rounding(RoundingModes::Truncate)->ASSIGN_TO(context);
            truncated = true;
            negative = this->Sign() < 0;
            allocated = maxDigits + 5;
        }
        else
        {
            ASSIGN_TO(context);
            allocated = MP(sizeinbase)(_value, base == 0 ? 10 : base) + 2;
        }

        char* allocatedStr = (char*)(*__gmp_allocate_func)(allocated);
        char* str = allocatedStr;

        if(negative)
            *str++ = '-';
        if(truncated)
        {
            *str++ = '.';
            *str++ = '.';
            *str++ = '.';
        }

        MP(get_str)(str, (base <= 36 && !lowercase) ? -base : base, CTXT(0));
        auto result = gcnew String(allocatedStr);
        (*__gmp_free_func)(allocatedStr, allocated);

        return result;
    }

    int MPEXPR_NAME::GetHashCode()
    {
        IN_CONTEXT(this);

        mp_limb_t hash = 0;
        mp_limb_t* ptr = CTXT(0)->_mp_d;
        for(int i = abs(CTXT(0)->_mp_size); i > 0; i--)
            hash ^= *ptr++;

        if(CTXT(0)->_mp_size < 0)
            hash = (mp_limb_t)-(mpir_si)hash;

        return hash.GetHashCode();
    }

    #pragma endregion

    #pragma region Interface implementations

    //CompareTo has to be defined in HugeRational.cpp because it depends on HugeRational.h

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

    #pragma region expression special cases

    void MPEXPR(Divide)::CUSTOM_MP(div)(MP(ptr) q, MP(srcptr) n, MP(srcptr) d)
    {
        switch((rounding == RoundingModes::Default) ? MpirSettings::RoundingMode : rounding)
        {
            case RoundingModes::Floor:
                IS_NULL(_remainder)
                    ? MP(fdiv_q)(q, n, d)
                    : MP(fdiv_qr)(q, _remainder->_value, n, d);
                break;

            case RoundingModes::Ceiling:
                IS_NULL(_remainder)
                    ? MP(cdiv_q)(q, n, d)
                    : MP(cdiv_qr)(q, _remainder->_value, n, d);
                break;

            default:
                IS_NULL(_remainder)
                    ? MP(tdiv_q)(q, n, d)
                    : MP(tdiv_qr)(q, _remainder->_value, n, d);
                break;
        }
    }

    void MPEXPR(DivideUi)::CUSTOM_MP(div_ui)(MP(ptr) q, MP(srcptr) n, mpir_ui d)
    {
        mpir_ui limb;

        switch((rounding == RoundingModes::Default) ? MpirSettings::RoundingMode : rounding)
        {
            case RoundingModes::Floor:
                limb = IS_NULL(_remainder)
                    ? MP(fdiv_q_ui)(q, n, d)
                    : MP(fdiv_qr_ui)(q, _remainder->_value, n, d);
                break;

            case RoundingModes::Ceiling:
                limb = IS_NULL(_remainder)
                    ? MP(cdiv_q_ui)(q, n, d)
                    : MP(cdiv_qr_ui)(q, _remainder->_value, n, d);
                break;

            default:
                limb = IS_NULL(_remainder)
                    ? MP(tdiv_q_ui)(q, n, d)
                    : MP(tdiv_qr_ui)(q, _remainder->_value, n, d);
                break;
        }

        if(!IS_NULL(_limbRemainder))
            _limbRemainder(limb);
    }

    void MPEXPR(ShiftRight)::CUSTOM_MP(div_2exp)(MP(ptr) q, MP(srcptr) n, mp_bitcnt_t d)
    {
        switch((rounding == RoundingModes::Default) ? MpirSettings::RoundingMode : rounding)
        {
            case RoundingModes::Floor:
                _remainder
                    ? MP(fdiv_r_2exp)(q, n, d)
                    : MP(fdiv_q_2exp)(q, n, d);
                break;

            case RoundingModes::Ceiling:
                _remainder
                    ? MP(cdiv_r_2exp)(q, n, d)
                    : MP(cdiv_q_2exp)(q, n, d);
                break;

            default:
                _remainder
                    ? MP(tdiv_r_2exp)(q, n, d)
                    : MP(tdiv_q_2exp)(q, n, d);
                break;
        }
    }

    void MPEXPR(Mod)::CUSTOM_MP(mod)(MP(ptr) r, MP(srcptr) n, MP(srcptr) d)
    {
        switch((rounding == RoundingModes::Default) ? MpirSettings::RoundingMode : rounding)
        {
            case RoundingModes::Floor:
                IS_NULL(_quotient)
                    ? MP(fdiv_r)(r, n, d)
                    : MP(fdiv_qr)(_quotient->_value, r, n, d);
                break;

            case RoundingModes::Ceiling:
                IS_NULL(_quotient)
                    ? MP(cdiv_r)(r, n, d)
                    : MP(cdiv_qr)(_quotient->_value, r, n, d);
                break;

            default:
                IS_NULL(_quotient)
                    ? MP(tdiv_r)(r, n, d)
                    : MP(tdiv_qr)(_quotient->_value, r, n, d);
                break;
        }
    }

    void MPEXPR(ModUi)::CUSTOM_MP(mod_ui)(MP(ptr) r, MP(srcptr) n, mpir_ui d)
    {
        mpir_ui limb;

        switch((rounding == RoundingModes::Default) ? MpirSettings::RoundingMode : rounding)
        {
            case RoundingModes::Floor:
                limb = IS_NULL(_quotient)
                    ? MP(fdiv_r_ui)(r, n, d)
                    : MP(fdiv_qr_ui)(_quotient->_value, r, n, d);
                break;

            case RoundingModes::Ceiling:
                limb = IS_NULL(_quotient)
                    ? MP(cdiv_r_ui)(r, n, d)
                    : MP(cdiv_qr_ui)(_quotient->_value, r, n, d);
                break;

            default:
                limb = IS_NULL(_quotient)
                    ? MP(tdiv_r_ui)(r, n, d)
                    : MP(tdiv_qr_ui)(_quotient->_value, r, n, d);
                break;
        }

        if(!IS_NULL(_limbRemainder))
            _limbRemainder(limb);
    }

    void MPEXPR(Root)::CUSTOM_MP(root)(MP(ptr) dest, MP(srcptr) oper, mpir_ui power)
    {
        if(!IS_NULL(_remainder))
            MP(rootrem)(dest, _remainder->_value, oper, power);
        else if (IS_NULL(_exact))
            MP(nthroot)(dest, oper, power);
        else
            _exact(MP(root)(dest, oper, power) != 0);
    }

    void MPEXPR(SquareRoot)::CUSTOM_MP(sqrt)(MP(ptr) dest, MP(srcptr) oper)
    {
        IS_NULL(_remainder)
            ? MP(sqrt)(dest, oper)
            : MP(sqrtrem)(dest, _remainder->_value, oper);
    }

    void MPEXPR(Gcd)::CUSTOM_MP(gcd)(MP(ptr) dest, MP(srcptr) a, MP(srcptr) b)
    {
        switch ((IS_NULL(_s) ? 0 : 1) + (IS_NULL(_t) ? 0 : 2))
        {
            case 0:
               MP(gcd)(dest, a, b);
               break;

            case 1:
               MP(gcdext)(dest, _s->_value, NULL, a, b);
               break;

            case 2:
               MP(gcdext)(dest, _t->_value, NULL, b, a);
               break;

            case 3:
               MP(gcdext)(dest, _s->_value, _t->_value, a, b);
               break;
        }
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

    MAKE_BINARY_OPERATOR_STANDARD  (MPEXPR(Multiply),   DEFINE, *, Multiply, Int, Int)      
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR(Multiply),   DEFINE, *, Multiply, Int, Ui)       
    MAKE_BINARY_OPERATOR_LLIMB_R   (MPEXPR(Multiply),   DEFINE, *, Multiply, Int, Ui)       
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR(Multiply),   DEFINE, *, Multiply, Int, Si)       
    MAKE_BINARY_OPERATOR_LLIMB_R   (MPEXPR(Multiply),   DEFINE, *, Multiply, Int, Si)       

    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, <<, ShiftLeft, Int, Bits)   
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR(ShiftRight), DEFINE, >>, ShiftRight, Int, Bits)  
                                                                                                           
    MAKE_UNARY_OPERATOR            (MPEXPR_NAME,        DEFINE, -, Negate, Int)             
                                                                                                        
    MAKE_VOID_FUNCTION             (MPEXPR_NAME,        DEFINE, Abs, Int)                   
                                                                                                           
    MAKE_BINARY_OPERATOR_STANDARD  (MPEXPR(Divide),     DEFINE, /, Divide, Int, Int)        
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR(DivideUi),   DEFINE, /, Divide, Int, Ui)         
                                                                                                           
    MAKE_BINARY_OPERATOR_STANDARD  (MPEXPR(Mod),        DEFINE, %, Mod, Int, Int)           
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR(ModUi),      DEFINE, %, Mod, Int, Ui)            
                                                                                                           
    MAKE_BINARY_OPERATOR_RLIMB     (MPEXPR_NAME,        DEFINE, ^, Power, Int, Ui)          
                                                                                                        
    MAKE_FUNCTION_WITH_ONE         (MPEXPR_NAME,        DEFINE, DivideExactly, Int)         
    MAKE_FUNCTION_WITH_LIMB        (MPEXPR_NAME,        DEFINE, DivideExactly, Ui)          
                                                                                                        
    MAKE_FUNCTION_WITH_TWO         (MPEXPR_NAME,        DEFINE, PowerMod, Int, Int)         
    MAKE_FUNCTION_WITH_TWO_LLIMB   (MPEXPR_NAME,        DEFINE, PowerMod, Ui, Int)          
                                                                                                           
    MAKE_FUNCTION_WITH_LIMB        (MPEXPR(Root),       DEFINE, Root, Ui)                   
    MAKE_VOID_FUNCTION             (MPEXPR(SquareRoot), DEFINE, SquareRoot, Int)            
                                                                                                           
    MAKE_BINARY_OPERATOR_STANDARD  (MPEXPR_NAME,        DEFINE, &, And, Int, Int)           
    MAKE_BINARY_OPERATOR_STANDARD  (MPEXPR_NAME,        DEFINE, |, Or, Int, Int)            
    MAKE_BINARY_OPERATOR_STANDARD  (MPEXPR_NAME,        DEFINE, ^, Xor, Int, Int)           
    MAKE_UNARY_OPERATOR            (MPEXPR_NAME,        DEFINE, ~, Complement, Int)         

    DEFINE_UNARY_ASSIGNMENT_REF(Complement, Int, MP(com))
    DEFINE_UNARY_ASSIGNMENT_REF(Negate, Int, MP(neg))
    DEFINE_UNARY_ASSIGNMENT_REF(Abs, Int, MP(abs))

    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Add, Int, Ui, MP(add_ui))
    DEFINE_BINARY_ASSIGNMENT_REF_SI (Add, Int, Si, MP(add_ui), MP(sub_ui))

    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Subtract, Int, Ui, MP(sub_ui))
    DEFINE_BINARY_ASSIGNMENT_VAL_REF(Subtract, Ui, Int, MP(ui_sub))
    DEFINE_BINARY_ASSIGNMENT_REF_SI (Subtract, Int, Si, MP(sub_ui), MP(add_ui))
    DEFINE_BINARY_ASSIGNMENT_SI_REF (Subtract, Si, Int, MP(ui_sub), MP(add_ui), MP(neg))

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Multiply, Int, MP(mul))
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Multiply, Int, Ui, MP(mul_ui))
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Multiply, Int, Si, MP(mul_si))

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Divide, Int, CUSTOM_MP(div))
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Divide, Int, Ui, CUSTOM_MP(div_ui))
    DEFINE_BINARY_ASSIGNMENT_REF_REF(Mod, Int, CUSTOM_MP(mod))
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Mod, Int, Ui, CUSTOM_MP(mod_ui))

    DEFINE_BINARY_ASSIGNMENT_REF_VAL(ShiftLeft, Int, Bits, MP(mul_2exp))
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(ShiftRight, Int, Bits, CUSTOM_MP(div_2exp))

    DEFINE_BINARY_ASSIGNMENT_REF_REF(DivideExactly, Int, MP(divexact))
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(DivideExactly, Int, Ui, MP(divexact_ui))
    
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Power, Int, Ui, MP(pow_ui))

    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Root, Int, Ui, CUSTOM_MP(root))
    DEFINE_UNARY_ASSIGNMENT_REF     (SquareRoot, Int, CUSTOM_MP(sqrt))

    DEFINE_BINARY_ASSIGNMENT_REF_REF(And, Int, MP(and))
    DEFINE_BINARY_ASSIGNMENT_REF_REF(Or, Int, MP(ior))
    DEFINE_BINARY_ASSIGNMENT_REF_REF(Xor, Int, MP(xor))

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Gcd, Int, CUSTOM_MP(gcd))
    DEFINE_BINARY_ASSIGNMENT_REF_REF(Lcm, Int, MP(lcm))
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Lcm, Int, Ui, MP(lcm_ui))

    DEFINE_TERNARY_ASSIGNMENT_REF_REF_REF(PowerMod, Int, MP(powm));
    DEFINE_TERNARY_ASSIGNMENT_REF_VAL_REF(PowerMod, Int, Ui, Int, MP(powm_ui))

    void MPEXPR(MultiplyIntInt)::AddTo(MP(ptr) destination)
    {
        IN_CONTEXT(Left, Right);
        MP(addmul)(destination, CTXT(0), CTXT(1));
    }

    void MPEXPR(MultiplyIntUi)::AddTo(MP(ptr) destination)
    {
        IN_CONTEXT(Left);
        MP(addmul_ui)(destination, CTXT(0), Right);
    }

    void MPEXPR(MultiplyIntSi)::AddTo(MP(ptr) destination)
    {
        IN_CONTEXT(Left);
        if (Right < 0)
            MP(submul_ui)(destination, CTXT(0), (mpir_ui)-Right);
        else
            MP(addmul_ui)(destination, CTXT(0), (mpir_ui)Right);
        return;
    }

    void MPEXPR(MultiplyIntInt)::SubtractFrom(MP(ptr) destination)
    {
        IN_CONTEXT(Left, Right);
        MP(submul)(destination, CTXT(0), CTXT(1));
    }

    void MPEXPR(MultiplyIntUi)::SubtractFrom(MP(ptr) destination)
    {
        IN_CONTEXT(Left);
        MP(submul_ui)(destination, CTXT(0), Right);
    }

    void MPEXPR(MultiplyIntSi)::SubtractFrom(MP(ptr) destination)
    {
        IN_CONTEXT(Left);
        if (Right < 0)
            MP(addmul_ui)(destination, CTXT(0), (mpir_ui)-Right);
        else
            MP(submul_ui)(destination, CTXT(0), (mpir_ui)Right);
    }

    DEFINE_ASSIGNMENT_PROLOG(AddIntInt)
    {
        WHEN_IS_DEST(1, Left)
        {
            WHEN_IS(2, Right, MPEXPR(Multiply))
            {
                x2->AddTo(destination);
                return;
            }
        }
        WHEN_IS_DEST(3, Right)
        {
            WHEN_IS(4, Left, MPEXPR(Multiply))
            {
                x4->AddTo(destination);
                return;
            }
        }

        IN_CONTEXT(Left, Right);
        MP(add)(destination, CTXT(0), CTXT(1));
    }

    DEFINE_ASSIGNMENT_PROLOG(SubtractIntInt)
    {
        WHEN_IS_DEST(1, Left)
        {
            WHEN_IS(2, Right, MPEXPR(Multiply))
            {
                x2->SubtractFrom(destination);
                return;
            }
        }
        WHEN_IS_DEST(3, Right)
        {
            WHEN_IS(4, Left, MPEXPR(Multiply))
            {
                x4->SubtractFrom(destination);
                MP(neg)(destination, destination);
                return;
            }
        }

        IN_CONTEXT(Left, Right);
        MP(sub)(destination, CTXT(0), CTXT(1));
    }

    mpir_ui MPEXPR_NAME::Mod(mpir_ui d, RoundingModes rounding)
    {
        IN_CONTEXT(this);

        switch((rounding == RoundingModes::Default) ? MpirSettings::RoundingMode : rounding)
        {
            case RoundingModes::Floor:
                return MP(fdiv_ui)(CTXT(0), d);

            case RoundingModes::Ceiling:
                return MP(cdiv_ui)(CTXT(0), d);

            default:
                return MP(tdiv_ui)(CTXT(0), d);
        }
    }
    
    #pragma endregion

    #pragma region IO

    #define chunkSize 1024

    size_t MPTYPE::Write(Stream^ stream)
    {
        mpir_out_struct out;
        MP(out_raw_m)(out, _value);

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

        MP(inp_raw_p)(_value, csize_bytes, out);

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

            MP(inp_raw_m)(_value, out);
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

        return ReadNoWhite(reader, base, nread);
    }

    // adapted from inp_str, which is shared by mpq_inp_str
    size_t MPTYPE::ReadNoWhite(TextReader^ reader, int base, size_t nread)
    {
        char *str;
        size_t alloc_size, str_size;
        bool negative = false;
        mp_size_t xsize;
        const unsigned char* digit_value = __gmp_digit_value_tab;
        int c = reader->Peek();

        if (base > 36)
        {
            // For bases > 36, use the collating sequence
            // 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz
            digit_value += 224;
            if (base > 62)
                throw gcnew ArgumentException("Invalid base", "base");
        }

        if (c == '-')
        {
            negative = true;
            PEEK_NEXT_CHAR;
        }

        if (c == EOF || digit_value[c] >= (base == 0 ? 10 : base))
            throw gcnew Exception("No digits found");

        // If BASE is 0, try to find out the base by looking at the initial characters.
        if (base == 0)
        {
            base = 10;
            if (c == '0')
            {
                base = 8;
                PEEK_NEXT_CHAR;

                switch(c = reader->Peek())
                {
                    case 'x':
                    case 'X':
                        base = 16;
                        PEEK_NEXT_CHAR;
                        break;

                    case 'b':
                    case 'B':
                        base = 2;
                        PEEK_NEXT_CHAR;
                }
            }
        }

        // Skip leading zeros
        while (c == '0')
        {
            PEEK_NEXT_CHAR;
        }

        alloc_size = 100;
        str = (char *) (*__gmp_allocate_func) (alloc_size);
        str_size = 0;

        while (c != EOF)
        {
            int dig = digit_value[c];
            if (dig >= base)
                break;

            if (str_size >= alloc_size)
            {
                size_t old_alloc_size = alloc_size;
                alloc_size = alloc_size * 3 / 2;
                str = (char *) (*__gmp_reallocate_func) (str, old_alloc_size, alloc_size);
            }
            str[str_size++] = dig;
            reader->Read();
            c = reader->Peek();
        }
        nread += str_size;

        // Make sure the string is not empty, mpn_set_str would fail.
        if (str_size == 0)
        {
            _value ->_mp_size = 0;
        }
        else
        {
            xsize = (((mp_size_t)
                (str_size / __mp_bases[base].chars_per_bit_exactly))
                / GMP_NUMB_BITS + 2);
            MP(realloc) (_value, xsize);

            // Convert the byte array in base BASE to our bignum format.
            xsize = mpn_set_str (_value->_mp_d, (unsigned char *) str, str_size, base);
            _value->_mp_size = (int)(negative ? -xsize : xsize);
        }
        (*__gmp_free_func) (str, alloc_size);
        return nread;
    }

    void MPTYPE::ReadLimbs(array<mpir_ui>^ destination, int start, int length, int destinationIndex)
    {
        if (start < 0) throw gcnew ArgumentException("Invalid start", "start");
        if (length <= 0) throw gcnew ArgumentException("Invalid length", "length");
        if (_value->_mp_alloc < start + length) throw gcnew ArgumentException("Insufficient allocated limb data", "length");
        if (destinationIndex < 0) throw gcnew ArgumentException("Invalid destination index", "destinationIndex");
        if (destination->Length < destinationIndex + length) throw gcnew ArgumentException("Insufficient destination array size", "destinationIndex");

        auto src = &MP(limbs_read)(_value)[start];
        for (int i = 0; i < length; i++)
        {
            destination[destinationIndex++] = *src++;
        }
    }

    void MPTYPE::ModifyLimbs(array<mpir_ui>^ source, int start, int length, int sourceIndex, bool negative)
    {
        if (start < 0) throw gcnew ArgumentException("Invalid start", "start");
        if (length <= 0) throw gcnew ArgumentException("Invalid length", "length");
        if (sourceIndex < 0) throw gcnew ArgumentException("Invalid source index", "sourceIndex");
        if (source->Length < sourceIndex + length) throw gcnew ArgumentException("Insufficient source array size", "sourceIndex");

        auto oldAbsSize = ABS(_value->_mp_size);
        auto newSize = MAX(_value->_mp_alloc, start + length);

        auto ptr = MP(limbs_modify)(_value, newSize);
        auto dest = &ptr[oldAbsSize];
        for (int i = oldAbsSize; i < start; i++)
            *dest++ = 0;

        dest = &ptr[start];
        for (int i = 0; i < length; i++)
            *dest++ = source[sourceIndex++];

        MP(limbs_finish)(_value, negative ? -newSize : newSize);
    }

    void MPTYPE::WriteLimbs(array<mpir_ui>^ source, int sourceIndex, mp_size_t newSize, bool negative)
    {
        auto length = newSize;
        if (length == 0) throw gcnew ArgumentException("Invalid new size", "newSize");
        if (sourceIndex < 0) throw gcnew ArgumentException("Invalid source index", "sourceIndex");
        if (source->Length < sourceIndex + length) throw gcnew ArgumentException("Insufficient source array size", "sourceIndex");

        auto dest = MP(limbs_write)(_value, length);
        for (mp_size_t i = 0; i < length; i++)
            *dest++ = source[sourceIndex++];

        MP(limbs_finish)(_value, negative ? -newSize : newSize);
    }

    #pragma endregion

    #pragma region number-theoretic

    bool MPTYPE::IsProbablePrime(MpirRandom^ random, int probability, mpir_ui pretested)
    {
        return MP(probable_prime_p)(_value, random->_value, probability, pretested) != 0;
    }
            
    bool MPTYPE::IsLikelyPrime(MpirRandom^ random, mpir_ui pretested)
    {
        return MP(likely_prime_p)(_value, random->_value, pretested) != 0;
    }

    MAKE_FUNCTION_WITH_LIMB (MPEXPR_NAME, DEFINE, NextPrimeCandidate, Rnd)
    DEFINE_ASSIGNMENT_PROLOG(NextPrimeCandidateIntRnd)
    {
        IN_CONTEXT(Left);
        MP(next_prime_candidate)(destination, CTXT(0), Right->_value);
    }

    MAKE_FUNCTION_WITH_ONE (MPEXPR(Gcd), DEFINE, Gcd, Int)
    MAKE_FUNCTION_WITH_ONE (MPEXPR_NAME, DEFINE, Lcm, Int)
    MAKE_FUNCTION_WITH_LIMB (MPEXPR_NAME, DEFINE, Lcm, Ui)

    MAKE_FUNCTION_WITH_ONE (MPEXPR_NAME, DEFINE, Invert, Int)
    DEFINE_ASSIGNMENT_PROLOG(InvertIntInt)
    {
        IN_CONTEXT(Left, Right);
        if (MP(invert)(destination, CTXT(0), CTXT(1)) == 0)
            throw gcnew ArgumentException("Inverse does not exist");
    }

    MAKE_FUNCTION_WITH_ONE (MPEXPR(RemoveFactors), DEFINE, RemoveFactors, Int)
    DEFINE_ASSIGNMENT_PROLOG(RemoveFactorsIntInt)
    {
        IN_CONTEXT(Left, Right);
        auto result = MP(remove)(destination, CTXT(0), CTXT(1));
        if(!IS_NULL(_count))
            _count(result);
    }

    DEFINE_BINARY_ASSIGNMENT_VAL_VAL(Power, Ui, Ui, MP(ui_pow_ui))

    DEFINE_ASSIGNMENT_PROLOG(FactorialUiUi)
    {
        switch (Right)
        {
            case 1:
                MP(fac_ui)(destination, Left);
                break;

            case 2:
                MP(2fac_ui)(destination, Left);
                break;

            default:
                MP(mfac_uiui)(destination, Left, Right);
                break;
        }
    }

    DEFINE_UNARY_ASSIGNMENT_VAL(Primorial, Ui, MP(primorial_ui))
    DEFINE_BINARY_ASSIGNMENT_VAL_VAL(Binomial, Ui, Ui, MP(bin_uiui))
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Binomial, Int, Ui, MP(bin_ui))

    DEFINE_ASSIGNMENT_PROLOG(FibonacciUi)
    {
        IS_NULL(_previous)
            ? MP(fib_ui)(destination, Operand)
            : MP(fib2_ui)(destination, _previous->_value, Operand);
    }

    DEFINE_ASSIGNMENT_PROLOG(LucasUi)
    {
        IS_NULL(_previous)
            ? MP(lucnum_ui)(destination, Operand)
            : MP(lucnum2_ui)(destination, _previous->_value, Operand);
    }

    #pragma endregion
};