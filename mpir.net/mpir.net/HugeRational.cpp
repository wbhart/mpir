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
#include "Random.h"
#include "HugeRational.h"

using namespace System::Runtime::InteropServices;
using namespace System::Text;

namespace MPIR
{
    #pragma region construction

    HugeRational::HugeRational()
    {
        AllocateStruct();
        mpq_init(_value);
    }
    /*
    HugeRational::HugeRational(RationalExpression^ value)
    {
        AllocateStruct();
        mpq_init(_value);
        value->AssignTo(_value);
    }

    HugeRational::HugeRational(mp_bitcnt_t bits)
    {
        AllocateStruct();
        mpq_init2(_value, bits);
    }

    void HugeRational::FromString(String^ value, int base)
    {
        AllocateStruct();

        IntPtr ptr = Marshal::StringToHGlobalAnsi(value);
        bool success = 0 == mpq_init_set_str(_value, (char*)(void*)ptr, base);
        Marshal::FreeHGlobal(ptr);

        if(!success)
        {
            DeallocateStruct();
            throw gcnew ArgumentException("Invalid number", "value");
        }
    }

    void HugeRational::SetTo(String^ value, int base)
    {
        IntPtr ptr = Marshal::StringToHGlobalAnsi(value);
        bool success = 0 == mpq_set_str(_value, (char*)(void*)ptr, base);
        Marshal::FreeHGlobal(ptr);

        if(!success)
            throw gcnew ArgumentException("Invalid number", "value");
    }

    HugeRational^ HugeRational::FromLong(mpir_si value)
    {
        auto result = gcnew HugeRational();
        mpq_set_si(result->_value, value);
        return result;
    }

    HugeRational^ HugeRational::FromUlong(mpir_ui value)
    {
        auto result = gcnew HugeRational();
        mpq_set_ui(result->_value, value);
        return result;
    }

    HugeRational^ HugeRational::FromDouble(double value)
    {
        auto result = gcnew HugeRational();
        mpq_set_d(result->_value, value);
        return result;
    }

    #pragma endregion

    #pragma region object overrides

    String^ HugeRational::ToString(int base, bool lowercase, int maxDigits)
    {
        size_t allocated;
        bool negative = false;
        bool truncated = false;

        EvaluationContext context;
        if(maxDigits > 0 && CompareAbsTo(MpirSettings::_toStringModulo) >= 0)
        {
            (this->Abs() % MpirSettings::_toStringModulo)->Rounding(RoundingModes::Truncate)->AssignTo(context);
            truncated = true;
            negative = this->Sign() < 0;
            allocated = maxDigits + 5;
        }
        else
        {
            AssignTo(context);
            allocated = mpq_sizeinbase(_value, base == 0 ? 10 : base) + 2;
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

        mpq_get_str(str, (base <= 36 && !lowercase) ? -base : base, context.Args[0]);
        auto result = gcnew String(allocatedStr);
        (*__gmp_free_func)(allocatedStr, allocated);

        return result;
    }
    */

    int RationalExpression::GetHashCode()
    {
        /*
        IN_CONTEXT(this);
        */

        mp_limb_t hash = 0;
        /*
        mp_limb_t* ptr = context.Args[0]->_mp_d;
        for(int i = abs(context.Args[0]->_mp_size); i > 0; i--)
            hash ^= *ptr++;

        if(context.Args[0]->_mp_size < 0)
            hash = (mp_limb_t)-(mpir_si)hash;
        */

        return hash.GetHashCode();
    }
    /*
    #pragma endregion

    #pragma region Interface implementations
    */
    int RationalExpression::CompareTo(Object^ a, bool& valid)
    {
        valid = true;

        if (IS_NULL(a))
            return 1;

        RationalExpression^ expr = dynamic_cast<RationalExpression^>(a);
        if(!IS_NULL(expr))
            return CompareTo(expr);

        EvaluationContext context;
/*
        if(a->GetType() == mpir_ui::typeid)
        {
            AssignTo(context);
            return mpq_cmp_ui(context.Args[0], (mpir_ui)a);
        }

        if(a->GetType() == mpir_si::typeid)
        {
            AssignTo(context);
            return mpq_cmp_si(context.Args[0], (mpir_si)a);
        }

        if(a->GetType() == double::typeid)
        {
            AssignTo(context);
            return mpq_cmp_d(context.Args[0], (double)a);
        }
        */
        valid = false;
        return 0;
    }

    int RationalExpression::CompareTo(Object^ a)
    {
        bool valid;
        auto result = CompareTo(a, valid);

        if (valid)
            return result;

        throw gcnew ArgumentException("Invalid argument type", "a");
    }

    int RationalExpression::CompareTo(RationalExpression^ a)
    {
        /*
        if (IS_NULL(a))
        */
            return 1;
/*
        IN_CONTEXT(this, a);
        return mpq_cmp(context.Args[0], context.Args[1]);*/
    }

    bool RationalExpression::Equals(Object^ a)
    {
        bool valid;
        auto result = CompareTo(a, valid);

        return valid && result == 0;
    }

    bool RationalExpression::Equals(RationalExpression^ a)
    {
        return CompareTo(a) == 0;
    }

    #pragma endregion
    /*
    #pragma region expression special cases

    void RationalDivideExpression::custom_mpq_div(mpq_ptr q, mpq_srcptr n, mpq_srcptr d)
    {
        switch((rounding == RoundingModes::Default) ? MpirSettings::RoundingMode : rounding)
        {
            case RoundingModes::Floor:
                IS_NULL(_remainder)
                    ? mpq_fdiv_q(q, n, d)
                    : mpq_fdiv_qr(q, _remainder->_value, n, d);
                break;

            case RoundingModes::Ceiling:
                IS_NULL(_remainder)
                    ? mpq_cdiv_q(q, n, d)
                    : mpq_cdiv_qr(q, _remainder->_value, n, d);
                break;

            default:
                IS_NULL(_remainder)
                    ? mpq_tdiv_q(q, n, d)
                    : mpq_tdiv_qr(q, _remainder->_value, n, d);
                break;
        }
    }

    void RationalDivideUiExpression::custom_mpq_div_ui(mpq_ptr q, mpq_srcptr n, mpir_ui d)
    {
        mpir_ui limb;

        switch((rounding == RoundingModes::Default) ? MpirSettings::RoundingMode : rounding)
        {
            case RoundingModes::Floor:
                limb = IS_NULL(_remainder)
                    ? mpq_fdiv_q_ui(q, n, d)
                    : mpq_fdiv_qr_ui(q, _remainder->_value, n, d);
                break;

            case RoundingModes::Ceiling:
                limb = IS_NULL(_remainder)
                    ? mpq_cdiv_q_ui(q, n, d)
                    : mpq_cdiv_qr_ui(q, _remainder->_value, n, d);
                break;

            default:
                limb = IS_NULL(_remainder)
                    ? mpq_tdiv_q_ui(q, n, d)
                    : mpq_tdiv_qr_ui(q, _remainder->_value, n, d);
                break;
        }

        if(!IS_NULL(_limbRemainder))
            _limbRemainder(limb);
    }

    void RationalShiftRightExpression::custom_mpq_div_2exp(mpq_ptr q, mpq_srcptr n, mp_bitcnt_t d)
    {
        switch((rounding == RoundingModes::Default) ? MpirSettings::RoundingMode : rounding)
        {
            case RoundingModes::Floor:
                _remainder
                    ? mpq_fdiv_r_2exp(q, n, d)
                    : mpq_fdiv_q_2exp(q, n, d);
                break;

            case RoundingModes::Ceiling:
                _remainder
                    ? mpq_cdiv_r_2exp(q, n, d)
                    : mpq_cdiv_q_2exp(q, n, d);
                break;

            default:
                _remainder
                    ? mpq_tdiv_r_2exp(q, n, d)
                    : mpq_tdiv_q_2exp(q, n, d);
                break;
        }
    }

    void RationalModExpression::custom_mpq_mod(mpq_ptr r, mpq_srcptr n, mpq_srcptr d)
    {
        switch((rounding == RoundingModes::Default) ? MpirSettings::RoundingMode : rounding)
        {
            case RoundingModes::Floor:
                IS_NULL(_quotient)
                    ? mpq_fdiv_r(r, n, d)
                    : mpq_fdiv_qr(_quotient->_value, r, n, d);
                break;

            case RoundingModes::Ceiling:
                IS_NULL(_quotient)
                    ? mpq_cdiv_r(r, n, d)
                    : mpq_cdiv_qr(_quotient->_value, r, n, d);
                break;

            default:
                IS_NULL(_quotient)
                    ? mpq_tdiv_r(r, n, d)
                    : mpq_tdiv_qr(_quotient->_value, r, n, d);
                break;
        }
    }

    void RationalModUiExpression::custom_mpq_mod_ui(mpq_ptr r, mpq_srcptr n, mpir_ui d)
    {
        mpir_ui limb;

        switch((rounding == RoundingModes::Default) ? MpirSettings::RoundingMode : rounding)
        {
            case RoundingModes::Floor:
                limb = IS_NULL(_quotient)
                    ? mpq_fdiv_r_ui(r, n, d)
                    : mpq_fdiv_qr_ui(_quotient->_value, r, n, d);
                break;

            case RoundingModes::Ceiling:
                limb = IS_NULL(_quotient)
                    ? mpq_cdiv_r_ui(r, n, d)
                    : mpq_cdiv_qr_ui(_quotient->_value, r, n, d);
                break;

            default:
                limb = IS_NULL(_quotient)
                    ? mpq_tdiv_r_ui(r, n, d)
                    : mpq_tdiv_qr_ui(_quotient->_value, r, n, d);
                break;
        }

        if(!IS_NULL(_limbRemainder))
            _limbRemainder(limb);
    }

    void RationalRootExpression::custom_mpq_root(mpq_ptr dest, mpq_srcptr oper, mpir_ui power)
    {
        if(!IS_NULL(_remainder))
            mpq_rootrem(dest, _remainder->_value, oper, power);
        else if (IS_NULL(_exact))
            mpq_nthroot(dest, oper, power);
        else
            _exact(mpq_root(dest, oper, power) != 0);
    }

    void RationalSquareRootExpression::custom_mpq_sqrt(mpq_ptr dest, mpq_srcptr oper)
    {
        IS_NULL(_remainder)
            ? mpq_sqrt(dest, oper)
            : mpq_sqrtrem(dest, _remainder->_value, oper);
    }

    void RationalGcdExpression::custom_mpq_gcd(mpq_ptr dest, mpq_srcptr a, mpq_srcptr b)
    {
        switch ((IS_NULL(_s) ? 0 : 1) + (IS_NULL(_t) ? 0 : 2))
        {
            case 0:
               mpq_gcd(dest, a, b);
               break;

            case 1:
               mpq_gcdext(dest, _s->_value, NULL, a, b);
               break;

            case 2:
               mpq_gcdext(dest, _t->_value, NULL, b, a);
               break;

            case 3:
               mpq_gcdext(dest, _s->_value, _t->_value, a, b);
               break;
        }
    }

    #pragma endregion

    #pragma region Arithmetic

    DEFINE_OPERATIONS(DEFINE)

    DEFINE_UNARY_ASSIGNMENT_REF(Complement, Int, mpq_com)
    DEFINE_UNARY_ASSIGNMENT_REF(Negate, Int, mpq_neg)
    DEFINE_UNARY_ASSIGNMENT_REF(Abs, Int, mpq_abs)

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Add, Int, mpq_add)
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Add, Int, Ui, mpq_add_ui)
    DEFINE_BINARY_ASSIGNMENT_REF_SI (Add, Int, Si, mpq_add_ui, mpq_sub_ui)

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Subtract, Int, mpq_sub)
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Subtract, Int, Ui, mpq_sub_ui)
    DEFINE_BINARY_ASSIGNMENT_VAL_REF(Subtract, Ui, Int, mpq_ui_sub)
    DEFINE_BINARY_ASSIGNMENT_REF_SI (Subtract, Int, Si, mpq_sub_ui, mpq_add_ui)
    DEFINE_BINARY_ASSIGNMENT_SI_REF (Subtract, Si, Int, mpq_ui_sub, mpq_add_ui, mpq_neg)

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Multiply, Int, mpq_mul)
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Multiply, Int, Ui, mpq_mul_ui)
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Multiply, Int, Si, mpq_mul_si)

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Divide, Int, custom_mpq_div)
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Divide, Int, Ui, custom_mpq_div_ui)
    DEFINE_BINARY_ASSIGNMENT_REF_REF(Mod, Int, custom_mpq_mod)
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Mod, Int, Ui, custom_mpq_mod_ui)

    DEFINE_BINARY_ASSIGNMENT_REF_VAL(ShiftLeft, Int, Bits, mpq_mul_2exp)
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(ShiftRight, Int, Bits, custom_mpq_div_2exp)

    DEFINE_BINARY_ASSIGNMENT_REF_REF(DivideExactly, Int, mpq_divexact)
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(DivideExactly, Int, Ui, mpq_divexact_ui)
    
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Power, Int, Ui, mpq_pow_ui)

    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Root, Int, Ui, custom_mpq_root)
    DEFINE_UNARY_ASSIGNMENT_REF     (SquareRoot, Int, custom_mpq_sqrt)

    DEFINE_BINARY_ASSIGNMENT_REF_REF(And, Int, mpq_and)
    DEFINE_BINARY_ASSIGNMENT_REF_REF(Or, Int, mpq_ior)
    DEFINE_BINARY_ASSIGNMENT_REF_REF(Xor, Int, mpq_xor)

    DEFINE_BINARY_ASSIGNMENT_REF_REF(Gcd, Int, custom_mpq_gcd)
    DEFINE_BINARY_ASSIGNMENT_REF_REF(Lcm, Int, mpq_lcm)
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Lcm, Int, Ui, mpq_lcm_ui)

    DEFINE_TERNARY_ASSIGNMENT_REF_REF_REF(PowerMod, Int, mpq_powm);
    DEFINE_TERNARY_ASSIGNMENT_REF_VAL_REF(PowerMod, Int, Ui, Int, mpq_powm_ui)

    mpir_ui RationalExpression::Mod(mpir_ui d, RoundingModes rounding)
    {
        IN_CONTEXT(this);

        switch((rounding == RoundingModes::Default) ? MpirSettings::RoundingMode : rounding)
        {
            case RoundingModes::Floor:
                return mpq_fdiv_ui(context.Args[0], d);

            case RoundingModes::Ceiling:
                return mpq_cdiv_ui(context.Args[0], d);

            default:
                return mpq_tdiv_ui(context.Args[0], d);
        }
    }
    
    #pragma endregion

    #pragma region IO

    #define chunkSize 1024

    size_t HugeRational::Write(Stream^ stream)
    {
        mpir_out_struct out;
        mpq_out_raw_m(out, _value);

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

    size_t HugeRational::Read(Stream^ stream)
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

        mpq_inp_raw_p(_value, csize_bytes, out);

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

            mpq_inp_raw_m(_value, out);
        }

        return 4 + out->writtenSize;
    }

    size_t HugeRational::Write(TextWriter^ writer, int base, bool lowercase)
    {
        auto str = ToString(base, lowercase);
        writer->Write(str);
        return str->Length;
    }

    size_t HugeRational::Read(TextReader^ reader, int base)
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

#define PEEK_NEXT_CHAR  \
    reader->Read();     \
    c = reader->Peek(); \
    nread++;

    // adapted from inp_str, which is shared by mpq_inp_str
    size_t HugeRational::ReadNoWhite(TextReader^ reader, int base, size_t nread)
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
            MPZ_REALLOC (_value, xsize);

            // Convert the byte array in base BASE to our bignum format.
            xsize = mpn_set_str (_value->_mp_d, (unsigned char *) str, str_size, base);
            _value->_mp_size = (int)(negative ? -xsize : xsize);
        }
        (*__gmp_free_func) (str, alloc_size);
        return nread;
    }

    #pragma endregion

    #pragma region number-theoretic

    bool HugeRational::IsProbablePrime(MpirRandom^ random, int probability, mpir_ui pretested)
    {
        return mpq_probable_prime_p(_value, random->_value, probability, pretested) != 0;
    }
            
    bool HugeRational::IsLikelyPrime(MpirRandom^ random, mpir_ui pretested)
    {
        return mpq_likely_prime_p(_value, random->_value, pretested) != 0;
    }

    MAKE_FUNCTION_WITH_LIMB (RationalExpression, DEFINE, NextPrimeCandidate, Rnd)
    DEFINE_ASSIGNMENT_PROLOG(NextPrimeCandidateIntRnd)
    {
        IN_CONTEXT(Left);
        mpq_next_prime_candidate(destination, context.Args[0], Right->_value);
    }

    MAKE_FUNCTION_WITH_ONE (RationalGcdExpression, DEFINE, Gcd, Int)
    MAKE_FUNCTION_WITH_ONE (RationalExpression, DEFINE, Lcm, Int)
    MAKE_FUNCTION_WITH_LIMB (RationalExpression, DEFINE, Lcm, Ui)

    MAKE_FUNCTION_WITH_ONE (RationalExpression, DEFINE, Invert, Int)
    DEFINE_ASSIGNMENT_PROLOG(InvertIntInt)
    {
        IN_CONTEXT(Left, Right);
        if (mpq_invert(destination, context.Args[0], context.Args[1]) == 0)
            throw gcnew ArgumentException("Inverse does not exist");
    }

    MAKE_FUNCTION_WITH_ONE (RationalRemoveFactorsExpression, DEFINE, RemoveFactors, Int)
    DEFINE_ASSIGNMENT_PROLOG(RemoveFactorsIntInt)
    {
        IN_CONTEXT(Left, Right);
        auto result = mpq_remove(destination, context.Args[0], context.Args[1]);
        if(!IS_NULL(_count))
            _count(result);
    }

    DEFINE_BINARY_ASSIGNMENT_VAL_VAL(Power, Ui, Ui, mpq_ui_pow_ui)

    DEFINE_ASSIGNMENT_PROLOG(FactorialUiUi)
    {
        switch (Right)
        {
            case 1:
                mpq_fac_ui(destination, Left);
                break;

            case 2:
                mpq_2fac_ui(destination, Left);
                break;

            default:
                mpq_mfac_uiui(destination, Left, Right);
                break;
        }
    }

    DEFINE_UNARY_ASSIGNMENT_VAL(Primorial, Ui, mpq_primorial_ui)
    DEFINE_BINARY_ASSIGNMENT_VAL_VAL(Binomial, Ui, Ui, mpq_bin_uiui)
    DEFINE_BINARY_ASSIGNMENT_REF_VAL(Binomial, Int, Ui, mpq_bin_ui)

    DEFINE_ASSIGNMENT_PROLOG(FibonacciUi)
    {
        IS_NULL(_previous)
            ? mpq_fib_ui(destination, Operand)
            : mpq_fib2_ui(destination, _previous->_value, Operand);
    }

    DEFINE_ASSIGNMENT_PROLOG(LucasUi)
    {
        IS_NULL(_previous)
            ? mpq_lucnum_ui(destination, Operand)
            : mpq_lucnum2_ui(destination, _previous->_value, Operand);
    }

    #pragma endregion

    */
};