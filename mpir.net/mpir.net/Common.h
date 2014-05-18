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

#pragma once

#pragma region misc macros

#define LIT2(x) x
#define LIT(x) LIT2(x)
#define IS_NULL(a) (Object::ReferenceEquals(a, nullptr))
#define PIN(x) pin_ptr<T> pinptr##x = &x[0]; void* pinned_##x = pinptr##x;
#define SGN(Z) ((Z) < 0 ? -1 : (Z) > 0)

#define IN_CONTEXT_1(a)        \
    EvaluationContext context; \
    a->AssignTo(context)

#define IN_CONTEXT_2(a, b)     \
    EvaluationContext context; \
    a->AssignTo(context);      \
    b->AssignTo(context)

#define IN_CONTEXT_3(a, b, c)  \
    EvaluationContext context; \
    a->AssignTo(context);      \
    b->AssignTo(context);      \
    c->AssignTo(context)

#define IN_CONTEXT_THIS_AND_RATIONAL(n, d)  \
    EvaluationContext context;              \
    this->AssignTo(context);                \
    context.AddRational(n, d)

#define COUNT_ARGS_IMPL2(_1, _2, _3, name, ...) name
#define COUNT_ARGS_IMPL(args) COUNT_ARGS_IMPL2 args
#define COUNT_ARGS(...) COUNT_ARGS_IMPL((__VA_ARGS__, 3, 2, 1))
#define MACRO_CHOOSE2(prefix, number) prefix##number
#define MACRO_CHOOSE1(prefix, number) MACRO_CHOOSE2(prefix, number)
#define MACRO_CHOOSE(prefix, number) MACRO_CHOOSE1(prefix, number)
#define MACRO_GLUE(x, y) x y
#define IN_CONTEXT(...) MACRO_GLUE(MACRO_CHOOSE(IN_CONTEXT_, COUNT_ARGS(__VA_ARGS__)), (__VA_ARGS__))

#define TYPE_FOR_ABBR_Int MPTYPE^
#define TYPE_FOR_ABBR_Expr MPEXPR_NAME^
#define TYPE_FOR_ABBR_Si mpir_si
#define TYPE_FOR_ABBR_Ui mpir_ui
#define TYPE_FOR_ABBR_Bits mp_bitcnt_t
#define TYPE_FOR_ABBR_Rnd MpirRandom^

#pragma endregion

#pragma region enums

namespace MPIR
{
    /// <summary>
    /// This enum defines the rounding modes MPIR supports.  Division and modulo operations take an optional rounding mode parameter, or use the default, which is set in the static MpirSettings class.
    /// </summary>
    public enum class RoundingModes
    {
        /// <summary>Rounding mode is unspecified.  Use a higher level default if available, fall back to Truncate.</summary>
        Default,
        /// <summary>Truncate.  Quotient is rounded toward zero, and remainder has the same sign as the source number.</summary>
        Truncate,
        /// <summary>Round up.  Quotient is rounded toward +infinity, and remainder has the opposite sign to the divisor.</summary>
        Ceiling,
        /// <summary>Round down.  Quotient is rounded toward -infinity, and remainder has the sames sign as the divisor.</summary>
        Floor,
    };

    /// <summary>
    /// This enum defines the limb order used when importing or exporting a number.
    /// </summary>
    public enum class LimbOrder : __int8
    {
        /// <summary>Most significant limb comes first.</summary>
        MostSignificantFirst = 1,
        /// <summary>Least significant limb comes first.</summary>
        LeastSignificantFirst = -1,
    };

    /// <summary>
    /// This enum defines the byte order within each limb when importing or exporting a number.
    /// </summary>
    public enum class Endianness : __int8
    {
        /// <summary>The native byte order of the CPU is used.</summary>
        Native = 0,
        /// <summary>Most significant byte comes first in a limb.</summary>
        BigEndian = 1,
        /// <summary>Least significant byte comes first in a limb.</summary>
        LittleEndian = -1,
    };
}

enum EvaluationOptions : __int32
{
    None = 0x0,

    IntInitialized = 0x1, 
    Temp1InitializedInt = IntInitialized,
    Temp2InitializedInt = IntInitialized << 1,
    Temp3InitializedInt = IntInitialized << 2,

    RationalInitialized = 0x10, 
    Temp1InitializedRational = RationalInitialized,
    Temp2InitializedRational = RationalInitialized << 1,
    Temp3InitializedRational = RationalInitialized << 2,

    FloatInitialized = 0x100, 
    Temp1InitializedFloat = FloatInitialized,
    Temp2InitializedFloat = FloatInitialized << 1,
    Temp3InitializedFloat = FloatInitialized << 2,
};

#pragma endregion

struct EvaluationContext
{
    public:
        union
        {
            __mpz_struct Integer;
            __mpq_struct Rational;
            __mpf_struct Float;
        } 
        Temp[3];
        union
        {
            mpz_ptr IntArgs[3];
            mpq_ptr RationalArgs[3];
            mpf_ptr FloatArgs[3];
        };
        union
        {
            struct
            {
                EvaluationOptions Options;
                unsigned __int8 Index;
            };
            __int64 Zero;
        };

        void inline Initialized(EvaluationOptions flag)
        {
            Options = (EvaluationOptions) (Options | (flag << Index));
        }

        EvaluationContext()
        {
            Zero = 0;
        }

#define CTXT_ADD_RATIONAL(numerator, denominator)     \
    auto ptr = &context.Temp[context.Index].Rational; \
    context.RationalArgs[context.Index++] = ptr;      \
                                                      \
    auto _n = (mpir_ui)ABS(numerator);                \
    ptr->_mp_num._mp_alloc = 1;                       \
    ptr->_mp_num._mp_size = (int)SGN(numerator);      \
    ptr->_mp_num._mp_d = &_n;                         \
                                                      \
    auto _d = (mpir_ui)denominator;                   \
    ptr->_mp_den._mp_alloc = 1;                       \
    ptr->_mp_den._mp_size = (int)SGN(denominator);    \
    ptr->_mp_den._mp_d = &_d;

#define CTXT_ADD_RATIONAL_DOUBLE(value)               \
    context.Initialized(RationalInitialized);         \
    auto ptr = &context.Temp[context.Index].Rational; \
    context.RationalArgs[context.Index++] = ptr;      \
    mpq_init(ptr);                                    \
    mpq_set_d(ptr, value);

        ~EvaluationContext()
        {
            if(Options & EvaluationOptions::Temp1InitializedInt)
                mpz_clear(IntArgs[0]);
            if(Options & EvaluationOptions::Temp2InitializedInt)
                mpz_clear(IntArgs[1]);
            if(Options & EvaluationOptions::Temp3InitializedInt)
                mpz_clear(IntArgs[2]);

            if(Options & EvaluationOptions::Temp1InitializedRational)
                mpq_clear(RationalArgs[0]);
            if(Options & EvaluationOptions::Temp2InitializedRational)
                mpq_clear(RationalArgs[1]);
            if(Options & EvaluationOptions::Temp3InitializedRational)
                mpq_clear(RationalArgs[2]);

            if(Options & EvaluationOptions::Temp1InitializedFloat)
                mpf_clear(FloatArgs[0]);
            if(Options & EvaluationOptions::Temp2InitializedFloat)
                mpf_clear(FloatArgs[1]);
            if(Options & EvaluationOptions::Temp3InitializedFloat)
                mpf_clear(FloatArgs[2]);
        }
};