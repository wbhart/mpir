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

using namespace System;
using namespace System::IO;
using namespace System::Runtime::InteropServices;

#ifdef SPECIALIZE_EXPRESSIONS
#undef SPECIALIZE_EXPRESSIONS
#undef MP
#undef CUSTOM_MP
#undef MPSTRUCT
#undef MPTYPE
#undef MPTYPE_NAME
#undef MPEXPR_NAME
#undef MPEXPR
#undef CTXT
#undef ASSIGN_TO
#undef Mpt
#undef SET_CONTEXT_PRECISION
#endif
#define SPECIALIZE_EXPRESSIONS
#define Mpt Int
#define CUSTOM_MP(x) custom_mpz_##x
#define MPSTRUCT __mpz_struct
#define MP(x) mpz_##x
#define MPTYPE HugeInt
#define MPTYPE_NAME Integer
#define MPEXPR_NAME LIT(MPTYPE_NAME)Expression
#define MPEXPR(x) LIT(MPTYPE_NAME)##x##Expression
#define CTXT(x) context.IntArgs[x]
#define ASSIGN_TO CONCAT(AssignTo, LIT(MPTYPE_NAME))
#define SET_CONTEXT_PRECISION
#include "ExpressionMacros.h"

extern __mpz_struct HugeIntConst1;

namespace MPIR
{
    ref class MpirRandom;
    ref class RationalExpression;
    ref class FloatExpression;
    ref class MPTYPE;
    ref class MPEXPR(Multiply);
    ref class MPEXPR(Divide);
    ref class MPEXPR(DivideUi);
    ref class MPEXPR(Mod);
    ref class MPEXPR(DivMod);
    ref class MPEXPR(ModUi);
    ref class MPEXPR(ShiftRight);
    ref class MPEXPR(Root);
    ref class MPEXPR(SquareRoot);
    ref class MPEXPR(Gcd);
    ref class MPEXPR(RemoveFactors);
    ref class MPEXPR(Sequence);
    
    #pragma region IntegerExpression

    /// <summary>
    /// Base class for all integer expressions resulting from many integer operations on MPIR types.
    /// <para>Expressions can be arbitrarily nested, and are lazily evaluated 
    /// when they are either assigned to the Value property of an MPIR object, or are consumed by a function or operator that returns a primitive type.
    /// </para>Assignment to the Value property is necessary because .Net does not support overloading the assignment operator.
    /// </summary>
    public ref class MPEXPR_NAME abstract : public IComparable, IComparable<MPEXPR_NAME^>, IEquatable<MPEXPR_NAME^>
    {
        internal:
            MPEXPR_NAME() { }
            virtual void AssignTo(MP(ptr) destination) abstract;
            virtual void ASSIGN_TO(EvaluationContext& context)
            {
                context.Initialized(IntInitialized);
                auto ptr = &context.Temp[context.Index].MPTYPE_NAME;
                CTXT(context.Index++) = ptr;
                MP(init)(ptr);
                AssignTo(ptr); 
            }
            virtual void AssignToRational(EvaluationContext& context)
            {
                context.Initialized(IntInitialized);
                auto ptr = &context.Temp[context.Index].Rational;
                context.RationalArgs[context.Index++] = ptr;
                MP(init)(&ptr->_mp_num);
                AssignTo(&ptr->_mp_num); 
                ptr->_mp_den = HugeIntConst1;
            }
            static MPEXPR_NAME()
            {
                MP(init_set_ui)(&HugeIntConst1, 1);
            }

        private:
            int CompareTo(Object^ a, bool& valid);

        public:
            #pragma region Arithmetic and Bitwise logic

            /// <summary>Adds two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to add to</param>
            /// <param name="b">Source value to add</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ operator + (MPEXPR_NAME^ a, MPEXPR_NAME^ b);

            /// <summary>Adds two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to add to</param>
            /// <param name="b">Source value to add</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ operator + (MPEXPR_NAME^ a, mpir_ui b);

            /// <summary>Adds two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to add to</param>
            /// <param name="b">Source value to add</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ operator + (mpir_ui a, MPEXPR_NAME^ b);

            /// <summary>Adds two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to add to</param>
            /// <param name="b">Source value to add</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ operator + (MPEXPR_NAME^ a, mpir_si b);

            /// <summary>Adds two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to add to</param>
            /// <param name="b">Source value to add</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ operator + (mpir_si a, MPEXPR_NAME^ b);
                                                                                                          
            /// <summary>Subtracts two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to subtract from</param>
            /// <param name="b">Source value to subtract</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ operator - (MPEXPR_NAME^ a, MPEXPR_NAME^ b);
                                                                                                          
            /// <summary>Subtracts two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to subtract from</param>
            /// <param name="b">Source value to subtract</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ operator - (MPEXPR_NAME^ a, mpir_ui b);
                                                                                                          
            /// <summary>Subtracts two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to subtract from</param>
            /// <param name="b">Source value to subtract</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ operator - (mpir_ui a, MPEXPR_NAME^ b);
                                                                                                          
            /// <summary>Subtracts two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to subtract from</param>
            /// <param name="b">Source value to subtract</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ operator - (MPEXPR_NAME^ a, mpir_si b);
                                                                                                          
            /// <summary>Subtracts two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to subtract from</param>
            /// <param name="b">Source value to subtract</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ operator - (mpir_si a, MPEXPR_NAME^ b);
                                                                                                          
            /// <summary>Multiplies two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to multiply</param>
            /// <param name="b">Source value to multiply by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR(Multiply)^ operator * (MPEXPR_NAME^ a, MPEXPR_NAME^ b);
                                                                                                          
            /// <summary>Multiplies two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to multiply</param>
            /// <param name="b">Source value to multiply by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR(Multiply)^ operator * (MPEXPR_NAME^ a, mpir_ui b);
                                                                                                          
            /// <summary>Multiplies two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to multiply</param>
            /// <param name="b">Source value to multiply by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR(Multiply)^ operator * (mpir_ui a, MPEXPR_NAME^ b);
                                                                                                          
            /// <summary>Multiplies two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to multiply</param>
            /// <param name="b">Source value to multiply by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR(Multiply)^ operator * (MPEXPR_NAME^ a, mpir_si b);
                                                                                                          
            /// <summary>Multiplies two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to multiply</param>
            /// <param name="b">Source value to multiply by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR(Multiply)^ operator * (mpir_si a, MPEXPR_NAME^ b);
                                                                                                          
            /// <summary>Shifts the <paramref name="a"/> source operand to the left by <paramref name="bits"/>, i.e. multiplies <paramref name="a"/> by 2^<paramref name="bits"/>.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to multiply</param>
            /// <param name="bits">Number of bits to shift <paramref name="a"/> by, i.e. power of 2 to multiply <paramref name="a"/> by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ operator << (MPEXPR_NAME^ a, mp_bitcnt_t bits);
                                                                                                          
            /// <summary>Shifts the <paramref name="a"/> source operand to the right by <paramref name="bits"/>, i.e. divides <paramref name="a"/> by 2^<paramref name="bits"/>.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para>By default, the shifted value (i.e., the quotient of the division) is computed.  If instead the shifted bits (i.e., the remainder) is desired, you can call 
            /// the Remainder method on the resulting expression before you assign it.  You can also specify the rounding mode to use for the division by calling a method on the resulting expression.</summary>
            /// <param name="a">Source value to divide</param>
            /// <param name="bits">Number of bits to shift <paramref name="a"/> by, i.e. power of 2 to divide <paramref name="a"/> by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation.
            /// <para>The expression exposes methods you can call to select whether you need to compute the quotient or remainder of the division, and/or to set the rounding mode.
            /// </para>By default, the shifted value (i.e., quotient) is computed and the rounding mode defaults to the static MpirSettings.DefaultRoundingMode.
            /// </returns>
            static MPEXPR(ShiftRight)^ operator >> (MPEXPR_NAME^ a, mp_bitcnt_t bits);

            /// <summary>Negates the source value.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to negate</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ operator - (MPEXPR_NAME^ a);
                                                                                                          
            /// <summary>Divides two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para>You can optionally save the remainder or specify the rounding mode to use for the division by calling methods on the resulting expression, before assigning it.</summary>
            /// <param name="a">Source value to divide</param>
            /// <param name="b">Source value to divide <paramref name="a"/> by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation.
            /// <para>The expression exposes methods you can call optionally if you need to save the remainder of the division, and/or to set the rounding mode.
            /// </para>By default, the remainder is not computed and the rounding mode defaults to the static MpirSettings.DefaultRoundingMode.
            /// </returns>
            static MPEXPR(Divide)^ operator / (MPEXPR_NAME^ a, MPEXPR_NAME^ b);
                                                                                                          
            /// <summary>Divides two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para>You can optionally save the remainder or specify the rounding mode to use for the division by calling methods on the resulting expression, before assigning it.</summary>
            /// <param name="a">Source value to divide</param>
            /// <param name="b">Source value to divide <paramref name="a"/> by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation.
            /// <para>The expression exposes methods you can call optionally if you need to save the remainder of the division, and/or to set the rounding mode.
            /// </para>By default, the remainder is not computed and the rounding mode defaults to the static MpirSettings.DefaultRoundingMode.
            /// </returns>
            static MPEXPR(DivideUi)^ operator / (MPEXPR_NAME^ a, mpir_ui b);
                                                                                                          
            /// <summary>Calculates the remainder from the division of two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para>You can optionally save the quotient in addition to the remainder or specify the rounding mode to use for the division 
            /// by calling methods on the resulting expression, before assigning it.</summary>
            /// <param name="a">Source value to divide</param>
            /// <param name="b">Source value to divide <paramref name="a"/> by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation.
            /// <para>The expression exposes methods you can call optionally if you need to save the quotient in addition to the remainder, and/or to set the rounding mode.
            /// </para>By default, the remainder is not computed and the rounding mode defaults to the static MpirSettings.DefaultRoundingMode.
            /// </returns>
            static MPEXPR(Mod)^ operator % (MPEXPR_NAME^ a, MPEXPR_NAME^ b);
                                                                                                          
            /// <summary>Calculates the remainder from the division of two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para>You can optionally save the quotient in addition to the remainder or specify the rounding mode to use for the division 
            /// by calling methods on the resulting expression, before assigning it.</summary>
            /// <param name="a">Source value to divide</param>
            /// <param name="b">Source value to divide <paramref name="a"/> by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation.
            /// <para>The expression exposes methods you can call optionally if you need to save the quotient in addition to the remainder, and/or to set the rounding mode.
            /// </para>By default, the remainder is not computed and the rounding mode defaults to the static MpirSettings.DefaultRoundingMode.
            /// </returns>
            static MPEXPR(ModUi)^ operator % (MPEXPR_NAME^ a, mpir_ui b);
                                                                                                          
            /// <summary>Raises the source value to the specified power.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to multiply</param>
            /// <param name="power">Power to raise <paramref name="a"/> to</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ operator ^ (MPEXPR_NAME^ a, mpir_ui power);
                                                                                                          
            /// <summary>Computes the bitwise AND of two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to AND</param>
            /// <param name="b">Source value to AND with</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ operator & (MPEXPR_NAME^ a, MPEXPR_NAME^ b);
                                                                                                          
            /// <summary>Computes the bitwise (inclusive) OR of two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to OR</param>
            /// <param name="b">Source value to OR with</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ operator | (MPEXPR_NAME^ a, MPEXPR_NAME^ b);
                                                                                                          
            /// <summary>Computes the bitwise XOR (exclusive or) of two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to XOR</param>
            /// <param name="b">Source value to XOR with</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ operator ^ (MPEXPR_NAME^ a, MPEXPR_NAME^ b);
                                                                                                          
            /// <summary>Computes the bitwise complement of a number.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to complement</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ operator ~ (MPEXPR_NAME^ a);

            /// <summary>If the source is &gt;= 0, returns the population count of op, which is the number of 1 bits in the binary representation.
#if BITS_PER_MP_LIMB == 64
            /// <para>If the source is &lt; 0, the number of 1s is infinite, and the return value is ulong.MaxValue, the largest possible bit count.
#else
            /// <para>If the source is &lt; 0, the number of 1s is infinite, and the return value is uint.MaxValue, the largest possible bit count.
#endif
            /// </para>Because the result is a primitive type, it is computed immediately.
            /// </summary>
            /// <returns>The population count for a non-negative number</returns>
            mp_bitcnt_t PopCount() { IN_CONTEXT(this); return MP(popcount)(CTXT(0)); }

            /// <summary>If this number and the operand are both &gt;= 0 or both &lt; 0, returns the hamming distance between them, which is the number of bit positions with different bit values.
#if BITS_PER_MP_LIMB == 64
            /// <para>If one operand is &gt;= 0 and the other &lt; 0 then the number of bits different is infinite, and the return value is ulong.MaxValue, the largest possible bit count.
#else
            /// <para>If one operand is &gt;= 0 and the other &lt; 0 then the number of bits different is infinite, and the return value is uint.MaxValue, the largest possible bit count.
#endif
            /// </para>Because the result is a primitive type, it is computed immediately.
            /// </summary>
            /// <param name="a">Source value to compute the hamming distance to</param>
            /// <returns>The hamming distance between this number and <paramref name="a"/></returns>
            mp_bitcnt_t HammingDistance(MPEXPR_NAME^ a) { IN_CONTEXT(this, a); return MP(hamdist)(CTXT(0), CTXT(1)); }

            /// <summary>Scans the source number, starting from the <paramref name="start"/> bit, towards more significant bits, until the first 0 or 1 bit
            /// (depending on the <paramref name="value"/> is found, and return the index of the found bit.
            /// <para>If the bit at the starting position is already what's sought, then <paramref name="start"/> is returned.
#if BITS_PER_MP_LIMB == 64
            /// </para>If there's no bit found, then ulong.MaxValue (the largest possible bit count) is returned. 
#else
            /// </para>If there's no bit found, then uint.MaxValue (the largest possible bit count) is returned.
#endif
            /// This will happen with <paramref name="value"/> = true past the end of a non-negative number, or with <paramref name="value"/> = false past the end of a negative number.
            /// <para>A false bit will always be found at the <paramref name="start"/> position past the end of a non-negatitve number, and a true bit past the end of a negative number.
            /// </para></summary>
            /// <param name="value">Value of the bit to scan for, true for 1, false for 0</param>
            /// <param name="start">Starting bit position to search.  The least significant bit is zero.</param>
#if BITS_PER_MP_LIMB == 64
            /// <returns>The index of the found bit, or ulong.MaxValue if no bit found.</returns>
#else
            /// <returns>The index of the found bit, or uint.MaxValue if no bit found.</returns>
#endif
            mp_bitcnt_t FindBit(bool value, mp_bitcnt_t start) { IN_CONTEXT(this); return value ? MP(scan1)(CTXT(0), start) : MP(scan0)(CTXT(0), start); }

            /// <summary>Computes the absolute value of the source number.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MPEXPR_NAME^ Abs();
                                                                                                          
            /// <summary>Divides two numbers where it is known in advance that the division is exact.  This method is faster than normal division,
            /// but produces an undefined result when the division is not exact.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to divide by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation.
            /// </returns>
            MPEXPR_NAME^ DivideExactly(MPEXPR_NAME^ a);
                                                                                                          
            /// <summary>Divides two numbers where it is known in advance that the division is exact.  This method is faster than normal division,
            /// but produces an undefined result when the division is not exact.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to divide by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation.
            /// </returns>
            MPEXPR_NAME^ DivideExactly(mpir_ui a);
                                                                                                          
            /// <summary>Raises the source value to the specified <paramref name="power"/> modulo <paramref name="modulo"/>.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="power">Power to raise the source value to.
            /// <para>Negative power values are supported if an inverse mod <paramref name="modulo"/> exists, otherwise divide by zero is raised.</para></param>
            /// <param name="modulo">Modulo to perform the powering operation with</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MPEXPR_NAME^ PowerMod(MPEXPR_NAME^ power, MPEXPR_NAME^ modulo);
                                                                                                          
            /// <summary>Raises the source value to the specified <paramref name="power"/> modulo <paramref name="modulo"/>.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="power">Power to raise the source value to</param>
            /// <param name="modulo">Modulo to perform the powering operation with</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MPEXPR_NAME^ PowerMod(mpir_ui power, MPEXPR_NAME^ modulo);

            /// <summary>Computes the truncated integer part of the root of the specified <paramref name="power"/> from the source value.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para>You can optionally save the remainder from the root operation, or a flag indicating whether the root was exact, 
            /// by calling a method on the resulting expression, before assigning it.</summary>
            /// <param name="power">Power of the root to compute.</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MPEXPR(Root)^ Root(mpir_ui power);

            /// <summary>Computes the truncated integer part of the square root of the source value.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para>You can optionally save the remainder from the root operation
            /// by calling a method on the resulting expression, before assigning it.</summary>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MPEXPR(SquareRoot)^ SquareRoot();

            /// <summary>Calculates the remainder from the division of two numbers, using the rounding mode set in MpirSettings.RoundingMode.
            /// <para>Because the result is a primitive type, it is computed immediately.
            /// </para></summary>
            /// <param name="a">divisor to divide the source by</param>
            /// <returns>The remainder of the division</returns>
            mpir_ui Mod(mpir_ui a) { return Mod(a, RoundingModes::Default); }

            /// <summary>Calculates the remainder from the division of two numbers, using the specified rounding mode.
            /// <para>Because the result is a primitive type, it is computed immediately.
            /// </para></summary>
            /// <param name="a">divisor to divide the source by</param>
            /// <param name="roundingMode">rounding mode to use for the division</param>
            /// <returns>The remainder of the division</returns>
            mpir_ui Mod(mpir_ui a, RoundingModes roundingMode);

            #pragma endregion

            #pragma region Comparisons

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para>Both this method and Equals() allow the argument to be a RationalExpression, however we do not define mixed equality operators,
            /// because otherwise testing for a null/non-null expression would require an awkward explicit cast on the null.
            /// <para>Although this only applies to equality operators, while comparison operators could have possibly worked, we're leaving out all mixed operators for now.
            /// </para>Since comparison via CompareTo() or Equals() is possible between ints and rationals, operators would just be another way to do the same thing.</summary>
            /// <param name="a">Value to compare the source with.  This can be an integer or rational multi-precision number or expression, or a supported primitive type (long, ulong, or double).</param>
            /// <returns>A positive number if the source is greater than <paramref name="a"/>, negative if less, and zero if they are equal.</returns>
            virtual int CompareTo(Object^ a) sealed;

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Value to compare the source with</param>
            /// <returns>A positive number if the source is greater than <paramref name="a"/>, negative if less, and zero if they are equal.</returns>
            virtual int CompareTo(MPEXPR_NAME^ a) sealed;

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Value to compare the source with</param>
            /// <returns>true if the values of the source and <paramref name="a"/> are equal, false otherwise.</returns>
            virtual bool Equals(MPEXPR_NAME^ a) sealed;

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para>Both this method and CompareTo() allow the argument to be a RationalExpression, however we do not define mixed equality operators,
            /// because otherwise testing for a null/non-null expression would require an awkward explicit cast on the null.
            /// <para>Although this only applies to equality operators, while comparison operators could have possibly worked, we're leaving out all mixed operators for now.
            /// </para>Since comparison via CompareTo() or Equals() is possible between ints and rationals, operators would just be another way to do the same thing.</summary>
            /// <param name="a">Value to compare the source with.  This can be an integer or rational multi-precision number or expression, or a supported primitive type (long, ulong, or double).</param>
            /// <returns>true if the values of the source and <paramref name="a"/> are equal, false otherwise.</returns>
            virtual bool Equals(Object^ a) override sealed;

            /// <summary>Computes the hash code of the source value.
            /// <para>If called on an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para>Multi-precision classes are mutable with value semantics.  The hash code is based on the value, and will change if the value changes.
            /// For this reason, the value of an object must not be modified while the object is contained in a hash table.</summary>
            /// <returns>a signed integer hash code for the value.</returns>
            virtual int GetHashCode() override sealed;

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <  (MPEXPR_NAME^ a, MPEXPR_NAME^ b) { return  IS_NULL(a) ? !IS_NULL(b) : a->CompareTo(b) < 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >= (MPEXPR_NAME^ a, MPEXPR_NAME^ b) { return  IS_NULL(a) ?  IS_NULL(b) : a->CompareTo(b) >= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator == (MPEXPR_NAME^ a, MPEXPR_NAME^ b) { return  IS_NULL(a) ?  IS_NULL(b) : a->CompareTo(b) == 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator != (MPEXPR_NAME^ a, MPEXPR_NAME^ b) { return  IS_NULL(a) ? !IS_NULL(b) : a->CompareTo(b) != 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >  (MPEXPR_NAME^ a, MPEXPR_NAME^ b) { return !IS_NULL(a) && a->CompareTo(b) > 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <= (MPEXPR_NAME^ a, MPEXPR_NAME^ b) { return  IS_NULL(a) || a->CompareTo(b) <= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <  (MPEXPR_NAME^ a, mpir_ui b) { return  IS_NULL(a) || a->CompareTo(b) < 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >= (MPEXPR_NAME^ a, mpir_ui b) { return !IS_NULL(a) && a->CompareTo(b) >= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >  (MPEXPR_NAME^ a, mpir_ui b) { return !IS_NULL(a) && a->CompareTo(b) > 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <= (MPEXPR_NAME^ a, mpir_ui b) { return  IS_NULL(a) || a->CompareTo(b) <= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator != (MPEXPR_NAME^ a, mpir_ui b) { return  IS_NULL(a) || a->CompareTo(b) != 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator == (MPEXPR_NAME^ a, mpir_ui b) { return !IS_NULL(a) && a->CompareTo(b) == 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <  (mpir_ui b, MPEXPR_NAME^ a) { return !IS_NULL(a) && a->CompareTo(b) > 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >= (mpir_ui b, MPEXPR_NAME^ a) { return  IS_NULL(a) || a->CompareTo(b) <= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >  (mpir_ui b, MPEXPR_NAME^ a) { return  IS_NULL(a) || a->CompareTo(b) < 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <= (mpir_ui b, MPEXPR_NAME^ a) { return !IS_NULL(a) && a->CompareTo(b) >= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator != (mpir_ui b, MPEXPR_NAME^ a) { return  IS_NULL(a) || a->CompareTo(b) != 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator == (mpir_ui b, MPEXPR_NAME^ a) { return !IS_NULL(a) && a->CompareTo(b) == 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <  (MPEXPR_NAME^ a, mpir_si b) { return  IS_NULL(a) || a->CompareTo(b) < 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >= (MPEXPR_NAME^ a, mpir_si b) { return !IS_NULL(a) && a->CompareTo(b) >= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >  (MPEXPR_NAME^ a, mpir_si b) { return !IS_NULL(a) && a->CompareTo(b) > 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <= (MPEXPR_NAME^ a, mpir_si b) { return  IS_NULL(a) || a->CompareTo(b) <= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator != (MPEXPR_NAME^ a, mpir_si b) { return  IS_NULL(a) || a->CompareTo(b) != 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator == (MPEXPR_NAME^ a, mpir_si b) { return !IS_NULL(a) && a->CompareTo(b) == 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <  (mpir_si b, MPEXPR_NAME^ a) { return !IS_NULL(a) && a->CompareTo(b) > 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >= (mpir_si b, MPEXPR_NAME^ a) { return  IS_NULL(a) || a->CompareTo(b) <= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >  (mpir_si b, MPEXPR_NAME^ a) { return  IS_NULL(a) || a->CompareTo(b) < 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <= (mpir_si b, MPEXPR_NAME^ a) { return !IS_NULL(a) && a->CompareTo(b) >= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator != (mpir_si b, MPEXPR_NAME^ a) { return  IS_NULL(a) || a->CompareTo(b) != 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator == (mpir_si b, MPEXPR_NAME^ a) { return !IS_NULL(a) && a->CompareTo(b) == 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <  (MPEXPR_NAME^ a, double b) { return  IS_NULL(a) || a->CompareTo(b) < 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >= (MPEXPR_NAME^ a, double b) { return !IS_NULL(a) && a->CompareTo(b) >= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >  (MPEXPR_NAME^ a, double b) { return !IS_NULL(a) && a->CompareTo(b) > 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <= (MPEXPR_NAME^ a, double b) { return  IS_NULL(a) || a->CompareTo(b) <= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator != (MPEXPR_NAME^ a, double b) { return  IS_NULL(a) || a->CompareTo(b) != 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator == (MPEXPR_NAME^ a, double b) { return !IS_NULL(a) && a->CompareTo(b) == 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <  (double b, MPEXPR_NAME^ a) { return !IS_NULL(a) && a->CompareTo(b) > 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >= (double b, MPEXPR_NAME^ a) { return  IS_NULL(a) || a->CompareTo(b) <= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >  (double b, MPEXPR_NAME^ a) { return  IS_NULL(a) || a->CompareTo(b) < 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <= (double b, MPEXPR_NAME^ a) { return !IS_NULL(a) && a->CompareTo(b) >= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator != (double b, MPEXPR_NAME^ a) { return  IS_NULL(a) || a->CompareTo(b) != 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator == (double b, MPEXPR_NAME^ a) { return !IS_NULL(a) && a->CompareTo(b) == 0; }

            /// <summary>Compares the absolute values of two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Value to compare the source with</param>
            /// <returns>A positive number if the absolute value of the source is greater than the absolute value of <paramref name="a"/>, negative if less, and zero if they are equal.</returns>
            int CompareAbsTo(MPEXPR_NAME^ a) { IN_CONTEXT(this, a); return MP(cmpabs)(CTXT(0), CTXT(1)); }

            /// <summary>Compares the absolute values of two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Value to compare the source with</param>
            /// <returns>A positive number if the absolute value of the source is greater than <paramref name="a"/>, negative if less, and zero if they are equal.</returns>
            int CompareAbsTo(mpir_ui a) { IN_CONTEXT(this); return MP(cmpabs_ui)(CTXT(0), a); }

            /// <summary>Compares the absolute values of two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Value to compare the source with</param>
            /// <returns>A positive number if the absolute value of the source is greater than the absolute value of <paramref name="a"/>, negative if less, and zero if they are equal.</returns>
            int CompareAbsTo(double a) { IN_CONTEXT(this); return MP(cmpabs_d)(CTXT(0), a); }

            /// <summary>Calculates the sign (+1, 0, or -1) of the source value.
            /// <para>If the source is an expression, it is evaluated into a temporary variable before the sign is computed.
            /// </para></summary>
            /// <returns>+1 if the source is positive, -1 if negative, and 0 if zero.</returns>
            int Sign() { IN_CONTEXT(this); return MP(sgn)(CTXT(0)); }

            #pragma endregion

            #pragma region Utility methods

            /// <summary>
            /// Checks if the source is evenly divisible by <paramref name="a"/>.
            /// Because this method returns a primitive type, it is computed immediately.
            /// </summary>
            /// <param name="a">Divisor to test with.  This can be zero; only zero is considired divisible by zero.</param>
            /// <returns>True if the source is evenly divisible by <paramref name="a"/></returns>
            bool IsDivisibleBy(MPEXPR_NAME^ a) { IN_CONTEXT(this, a); return MP(divisible_p)(CTXT(0), CTXT(1)) != 0; }

            /// <summary>
            /// Checks if the source is evenly divisible by <paramref name="a"/>.
            /// Because this method returns a primitive type, it is computed immediately.
            /// </summary>
            /// <param name="a">Divisor to test with.  This can be zero; only zero is considired divisible by zero.</param>
            /// <returns>True if the source is evenly divisible by <paramref name="a"/></returns>
            bool IsDivisibleBy(mpir_ui a) { IN_CONTEXT(this); return MP(divisible_ui_p)(CTXT(0), a) != 0; }

            /// <summary>
            /// Checks if the source is evenly divisible by 2^<paramref name="power"/>.
            /// Because this method returns a primitive type, it is computed immediately.
            /// </summary>
            /// <param name="power">Power of 2 to use for the divisor</param>
            /// <returns>True if the source is evenly divisible by 2^<paramref name="power"/></returns>
            bool IsDivisibleByPowerOf2(mp_bitcnt_t power) { IN_CONTEXT(this); return MP(divisible_2exp_p)(CTXT(0), power) != 0; }

            /// <summary>
            /// Checks if the source is congruent to <paramref name="a"/> modulo <paramref name="mod"/>.
            /// Because this method returns a primitive type, it is computed immediately.
            /// </summary>
            /// <param name="a">Divisor to test with.  This can be zero; only zero is considired divisible by zero.</param>
            /// <param name="mod">Modulo with respect to which to test for congruency</param>
            /// <returns>True if the source is congruent to <paramref name="a"/> modulo <paramref name="mod"/></returns>
            bool IsCongruentTo(MPEXPR_NAME^ a, MPEXPR_NAME^ mod) { IN_CONTEXT(this, a, mod); return MP(congruent_p)(CTXT(0), CTXT(1), CTXT(2)) != 0; }

            /// <summary>
            /// Checks if the source is congruent to <paramref name="a"/> modulo <paramref name="mod"/>.
            /// Because this method returns a primitive type, it is computed immediately.
            /// </summary>
            /// <param name="a">Divisor to test with.  This can be zero; only zero is considired divisible by zero.</param>
            /// <param name="mod">Modulo with respect to which to test for congruency</param>
            /// <returns>True if the source is congruent to <paramref name="a"/> modulo <paramref name="mod"/></returns>
            bool IsCongruentTo(mpir_ui a, mpir_ui mod) { IN_CONTEXT(this); return MP(congruent_ui_p)(CTXT(0), a, mod) != 0; }

            /// <summary>
            /// Checks if the source is congruent to <paramref name="a"/> modulo 2^<paramref name="power"/>.
            /// Because this method returns a primitive type, it is computed immediately.
            /// </summary>
            /// <param name="a">Divisor to test with</param>
            /// <param name="power">Power of 2 to use for the modulo</param>
            /// <returns>True if the source is congruent to <paramref name="a"/> modulo 2^<paramref name="power"/></returns>
            bool IsCongruentToModPowerOf2(MPEXPR_NAME^ a, mp_bitcnt_t power) { IN_CONTEXT(this, a); return MP(congruent_2exp_p)(CTXT(0), CTXT(1), power) != 0; }

            /// <summary>
            /// Checks if the source is a perfect power.
            /// <para>Because this method returns a primitive type, it is computed immediately.
            /// </para>Both 0 and 1 are considered perfect powers.
            /// <para>Negative values are accepted, but of course can only be odd powers.
            /// </para></summary>
            /// <returns>True if the source is a perfect power</returns>
            bool IsPerfectPower() { IN_CONTEXT(this); return MP(perfect_power_p)(CTXT(0)) != 0; }

            /// <summary>
            /// Checks if the source is a perfect square.
            /// <para>Because this method returns a primitive type, it is computed immediately.
            /// </para>Both 0 and 1 are considered perfect squares.
            /// </summary>
            /// <returns>True if the source is a perfect square</returns>
            bool IsPerfectSquare() { IN_CONTEXT(this); return MP(perfect_square_p)(CTXT(0)) != 0; }

            /// <summary>
            /// Returns the size of the absolute value of the number, measured in number of limbs.
            /// <para>If op is zero, the returned value will be zero.
            /// </para>For a HugeInt object, this method simply retrieves the value from a stored private field.
            /// <para>However for an expression it can potentially be very expensive, since it requires evaluation of the expression result.
            /// </para>Because this method returns a primitive type, it is computed immediately.
            /// </summary>
            /// <returns>The number of limbs used to represent the number</returns>
            size_t Size() { IN_CONTEXT(this); return MP(size)(CTXT(0)); }

            #pragma endregion

            #pragma region Number-theoretic

            /// <summary>
            /// Looks for the next candidate prime greater than this number.
            /// <para>Note that this function will occasionally return composites.
            /// It is designed to give a quick method for generating numbers which do not have small prime factors (less than 1000)
            /// and which pass a small number of rounds of Miller-Rabin (just two rounds).
            /// </para>The test is designed for speed, assuming that a high quality followup test can then be run to ensure primality.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="random">Random number generator to use for probabilistic primality tests</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, performs the requested operation</returns>
            MPEXPR_NAME^ NextPrimeCandidate(MpirRandom^ random);

            /// <summary>Computes the greatest common divisor of this number and <paramref name="a"/>.
            /// <para>The result is always positive even if one or both inputs are negative (or zero if both inputs are zero).
            /// </para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </summary>
            /// <param name="a">Source value to compute the GCD with</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MPEXPR(Gcd)^ Gcd(MPEXPR_NAME^ a);

            /// <summary>
            /// Computes the greatest common divisor of this number and <paramref name="a"/>.
            /// <para>The result is always positive even if the source number is negative.
            /// </para>Because the result is a primitive type, it is computed immediately.
            /// </summary>
            /// <param name="a">Source value to compute the GCD with.  If zero, zero is returned.</param>
            /// <returns>The greatest common divisor of the absolute value of this number and <paramref name="a"/>.</returns>
            mpir_ui Gcd(mpir_ui a) { IN_CONTEXT(this); return MP(gcd_ui)(nullptr, CTXT(0), a); }
                                                                                                          
            /// <summary>Computes the least common multiple of this number and <paramref name="a"/>.
            /// <para>The result is always positive, irrespective of the signs of the source numbers.
            /// </para>The result will be zero if either source number is zero.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to compute the LCM with.</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MPEXPR_NAME^ Lcm(MPEXPR_NAME^ a);
                                                                                                          
            /// <summary>Computes the least common multiple of this number and <paramref name="a"/>.
            /// <para>The result is always positive, irrespective of the signs of the source numbers.
            /// </para>The result will be zero if either source number is zero.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to compute the LCM with.</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MPEXPR_NAME^ Lcm(mpir_ui a);
                                                                                                          
            /// <summary>Computes the inverse of this number modulo <paramref name="modulo"/>.
            /// <para>If the inverse exists, the result will satisfy 0 &lt;= result &lt; <paramref name="modulo"/>.
            /// </para>If an inverse doesn't exist an ArgumentException is thrown.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="modulo">Modulo with respect to which to invert the number.</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MPEXPR_NAME^ Invert(MPEXPR_NAME^ modulo);

            /// <summary>Remove all occurrences of the <paramref name="factor"/> from the source number.
            /// <para>You can optionally save the number of such occurrences that were removed.
            /// </para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </summary>
            /// <param name="factor">Factor to remove from the source number.</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MPEXPR(RemoveFactors)^ RemoveFactors(MPEXPR_NAME^ factor);

            #pragma endregion
    };

    #pragma endregion

    #pragma region MpirSettings

    /// <summary>
    /// Static class for MPIR settings such as rounding defaults
    /// </summary>
    public ref class MpirSettings abstract sealed
    {
        private:
            static int _toStringDigits;

        internal:
            static MPTYPE^ _toStringModulo;

        public:

#define _GMP_VERSION __GNU_MP_VERSION + "." + __GNU_MP_VERSION_MINOR + "." + __GNU_MP_VERSION_PATCHLEVEL
#undef GMP_VERSION

            /// <summary>
            /// Represents the total number of bits in a single MPIR limb, including data bits and nail bits
            /// </summary>
            literal int BITS_PER_LIMB = BITS_PER_UI;

            /// <summary>
            /// Represents the number of nail bits in a single MPIR limb.  Nail bits are used internally by MPIR
            /// </summary>
            literal int NAIL_BITS_PER_LIMB = GMP_NAIL_BITS;

            /// <summary>
            /// Represents the number of data bits in a single MPIR limb
            /// </summary>
            literal int USABLE_BITS_PER_LIMB = GMP_NUMB_BITS;

            /// <summary>
            /// Represents the version of GMP with which the underlying MPIR library is compatible
            /// </summary>
            static initonly const Version^ GMP_VERSION = gcnew Version(_GMP_VERSION);

            /// <summary>
            /// Represents the version of the underlying MPIR library
            /// </summary>
            static initonly const Version^ MPIR_VERSION = gcnew Version(_MSC_MPIR_VERSION);

            /// <summary>
            /// Gets or sets the default rounding mode used for MPIR division operations that don't explicitly specify a rounding mode.
            /// <para>Defaults to Truncate.
            /// </para></summary>
            static property RoundingModes RoundingMode;

            /// <summary>
            /// Gets or sets the maximum number of digits the object.ToString() method override will output.  
            /// <para>If a number is longer than this number of digits, it will be output as "[-]...NNNNN" with the least significant digits shown.
            /// </para>Defaults to 256.
            /// <para>Specifying 0 will cause all digits to be output.  This should be used with care, as for example, the debugger calls ToString()
            /// on watched objects, and may have performance issues with large objects.
            /// </para></summary>
            static property int ToStringDigits
            {
                int get() { return _toStringDigits; }
                void set(int value);
            }

            static MpirSettings()
            {
                RoundingMode = RoundingModes::Truncate;
                ToStringDigits = 256;
            }
    };

    #pragma endregion

    #pragma region mid-level abstract expression specializations

    /// <summary>
    /// Expression that results from a multiplication operator.  Used internally to optimize computations that can use addmul/submul MPIR calls.
    /// </summary>
    public ref class MPEXPR(Multiply) abstract : MPEXPR_NAME
    {
        internal:
            MPEXPR(Multiply)() { }
            virtual void AddTo(MP(ptr) destination) abstract;
            virtual void SubtractFrom(MP(ptr) destination) abstract;
    };

    /// <summary>
    /// Expression that results from a division or modulo operator.  Allows to set the rounding mode for the division.
    /// </summary>
    public ref class MPEXPR(DivMod) abstract : MPEXPR_NAME 
    {
        internal:
            MPEXPR(DivMod)() { }
            RoundingModes rounding;

        public:
            /// <summary>
            /// Optionally sets the rounding mode for the division.  If not set, the static MpirSettings.RoundingMode will be used.
            /// </summary>
            /// <param name="mode">the mode to use.  If this is Default, the static MpirSettings.Rounding mode is used.</param>
            /// <returns>An updated expression, with its internal state updated to use the specified rounding mode.</returns>
            MPEXPR_NAME^ Rounding(RoundingModes mode)
            {
                rounding = mode;
                return this;
            }
    };

    /// <summary>
    /// Expression that results from a division operator.  Allows to save the remainder to a separate result object, and/or set the rounding mode for the division.
    /// </summary>
    public ref class MPEXPR(Divide) abstract : MPEXPR(DivMod) 
    {
        internal:
            MPEXPR(Divide)() { }
            MPTYPE^ _remainder;
            void CUSTOM_MP(div)(MP(ptr) q, MP(srcptr) n, MP(srcptr) d);

        public:
            /// <summary>
            /// Optionally save the remainder of the division to a separate result.  This cannot be the same object the resulting division quotient is being assigned to.
            /// </summary>
            /// <param name="destination">destination for the remainder.  This cannot be the same object the resulting division quotient is being assigned to.</param>
            /// <returns>An updated expression, with its internal state updated to compute the remainder.</returns>
            MPEXPR(DivMod)^ SavingRemainderTo(MPTYPE^ destination)
            {
                _remainder = destination;
                return this;
            }
    };

    /// <summary>
    /// Expression that results from a division operator.  Allows to save the remainder, and/or set the rounding mode for the division.
    /// </summary>
    public ref class MPEXPR(DivideUi) abstract : MPEXPR(Divide) 
    {
        private:
            Action<mpir_ui>^ _limbRemainder;

        internal:
            MPEXPR(DivideUi)() { }
            void CUSTOM_MP(div_ui)(MP(ptr) q, MP(srcptr) n, mpir_ui d);

        public:
            /// <summary>
            /// Optionally saves the remainder of the division to a separate result.
            /// </summary>
            /// <param name="callback">The delegate that will be called with the remainder of the division.  
            /// The delegate is called when the division is evaluated, i.e. is assigned to the Value property or consumed by a method that returns a primitive type.</param>
            /// <returns>An updated expression, with its internal state updated to compute the remainder.</returns>
            MPEXPR(Divide)^ SettingRemainderTo(Action<mpir_ui>^ callback)
            {
                _limbRemainder = callback;
                return this;
            }
    };

    /// <summary>
    /// Expression that results from a modulo operator.  Allows to save the division result to a separate object, and/or set the rounding mode for the division.
    /// </summary>
    public ref class MPEXPR(Mod) abstract : MPEXPR(DivMod) 
    {
        internal:
            MPEXPR(Mod)() { }
            MPTYPE^ _quotient;
            void CUSTOM_MP(mod)(MP(ptr) r, MP(srcptr) n, MP(srcptr) d);

        public:
            /// <summary>
            /// Optionally save the quotient of the division to a separate result.  This cannot be the same object the resulting division modulo is being assigned to.
            /// </summary>
            /// <param name="destination">destination for the quotient.  This cannot be the same object the resulting division modulo is being assigned to.</param>
            /// <returns>An updated expression, with its internal state updated to compute the quotient.</returns>
            MPEXPR(DivMod)^ SavingQuotientTo(MPTYPE^ destination)
            {
                _quotient = destination;
                return this;
            }
    };

    /// <summary>
    /// Expression that results from a modulo operator.  Allows to save the division result to a separate object, and/or set the rounding mode for the division.
    /// </summary>
    public ref class MPEXPR(ModUi) abstract : MPEXPR(Mod) 
    {
        private:
            Action<mpir_ui>^ _limbRemainder;

        internal:
            MPEXPR(ModUi)() { }
            void CUSTOM_MP(mod_ui)(MP(ptr) r, MP(srcptr) n, mpir_ui d);

        public:
            /// <summary>
            /// Optionally saves the remainder of the division to a separate result.
            /// </summary>
            /// <param name="callback">The delegate that will be called with the remainder of the division.  
            /// The delegate is called when the division is evaluated, i.e. is assigned to the Value property or consumed by a method that returns a primitive type.</param>
            /// <returns>An updated expression, with its internal state updated to compute the remainder.</returns>
            MPEXPR(Mod)^ SettingRemainderTo(Action<mpir_ui>^ callback)
            {
                _limbRemainder = callback;
                return this;
            }
    };

    /// <summary>
    /// Expression that results from a right shift operator.  Allows to save the remainder to a separate result object, and/or set the rounding mode for the division.
    /// </summary>
    public ref class MPEXPR(ShiftRight) abstract : MPEXPR(DivMod)
    {
        private:
            bool _remainder;

        internal:
            MPEXPR(ShiftRight)() { }
            void CUSTOM_MP(div_2exp)(MP(ptr) q, MP(srcptr) n, mp_bitcnt_t bits);

        public:
            /// <summary>
            /// Computes the remainder of the division, rather than the quotient, which is the default.
            /// </summary>
            /// <returns>An updated expression, with its internal state updated to compute the remainder, rather than the quotient.</returns>
            MPEXPR(DivMod)^ Remainder()
            {
                _remainder = true;
                return this;
            }
    };

    /// <summary>
    /// Expression that results from a square root function.  Allows to save the remainder to a separate result object.
    /// </summary>
    public ref class MPEXPR(SquareRoot) abstract : MPEXPR_NAME 
    {
        internal:
            MPEXPR(SquareRoot)() { }
            MPTYPE^ _remainder;
            void CUSTOM_MP(sqrt)(MP(ptr) dest, MP(srcptr) oper);

        public:
            /// <summary>
            /// Optionally saves the remainder of the root operation to a separate result.
            /// </summary>
            /// <param name="destination">destination for the remainder.  This cannot be the same object the result of the root operation is being assigned to.</param>
            /// <returns>An updated expression, with its internal state updated to save the remainder.</returns>
            MPEXPR_NAME^ SavingRemainderTo(MPTYPE^ destination)
            {
                _remainder = destination;
                return this;
            }
    };

    /// <summary>
    /// Expression that results from a root function.  Allows to save a flag indicating whether the root was exact, or to save the remainder to a separate result object.
    /// </summary>
    public ref class MPEXPR(Root) abstract : MPEXPR(SquareRoot) 
    {
        private:
            Action<bool>^ _exact;

        internal:
            MPEXPR(Root)() { }
            void CUSTOM_MP(root)(MP(ptr) dest, MP(srcptr) oper, mpir_ui power);

        public:
            /// <summary>
            /// Optionally gets a flag indicating whether the root operation was exact.
            /// </summary>
            /// <param name="callback">Delegate that will be called with the exact flag.
            /// The delegate is called when the root operation is evaluated, i.e. is assigned to the Value property or consumed by a method that returns a primitive type.</param>
            /// <returns>An updated expression, with its internal state updated to compute the exact flag.</returns>
            MPEXPR_NAME^ SettingExactTo(Action<bool>^ callback)
            {
                _exact = callback;
                return this;
            }
    };

    /// <summary>
    /// Expression that results from a Gcd method.  Allows to additionally compute Diophantine equation multiplier(s).
    /// </summary>
    public ref class MPEXPR(Gcd) abstract : MPEXPR_NAME 
    {
        internal:
            MPEXPR(Gcd)() { }
            MPTYPE^ _s;
            MPTYPE^ _t;
            void CUSTOM_MP(gcd)(MP(ptr) dest, MP(srcptr) a, MP(srcptr) b);

        public:
            /// <summary>
            /// Optionally computes and saves the coefficients <paramref name="s"/> and <paramref name="t"/> such that a<paramref name="s"/> + b<paramref name="t"/> = g = gcd(a, b).
            /// <para>If only one of the coefficients is needed, use null for the other.
            /// </para>The values <paramref name="s"/> and <paramref name="t"/> are chosen such that normally, |<paramref name="s"/>| &lt; |b|/(2g) and |<paramref name="t"/>| &lt; |a|/(2g),
            /// and these relations define <paramref name="s"/> and <paramref name="t"/> uniquely.
            /// <para>There are a few exceptional cases:
            /// </para>If |a| = |b|, then <paramref name="s"/> = 0 and <paramref name="t"/> = sgn(b).
            /// <para>Otherwise, <paramref name="s"/> = sgn(a) if b = 0 or |b| = 2g, and <paramref name="t"/> = sgn(b) if a = 0 or |a| = 2g.
            /// </para>In all cases, <paramref name="s"/> = 0 if and only if g = |b|, i.e., if b divides a or a = b = 0.</summary>
            /// <param name="s">destination for the first coefficient. Can be null if not needed.</param>
            /// <param name="t">destination for the second coefficient. Can be null if not needed.</param>
            /// <returns>An updated expression, with its internal state updated to save the coefficients.</returns>
            MPEXPR_NAME^ SavingDiophantineMultipliersTo(MPTYPE^ s, MPTYPE^ t)
            {
                _s = s;
                _t = t;
                return this;
            }
    };

    /// <summary>
    /// Expression that results from a RemoveFactors method.  Allows to additionally save the number of factors that were removed.
    /// </summary>
    public ref class MPEXPR(RemoveFactors) abstract : MPEXPR_NAME 
    {
        internal:
            MPEXPR(RemoveFactors)() { }
            Action<mp_bitcnt_t>^ _count;

        public:
            /// <summary>
            /// Optionally gets the number of factors removed.
            /// </summary>
            /// <param name="callback">Delegate that will be called with the number of factors that were removed.
            /// The delegate is called when the root operation is evaluated, i.e. is assigned to the Value property or consumed by a method that returns a primitive type.</param>
            /// <returns>An updated expression, with its internal state updated to save the number of factors.</returns>
            MPEXPR_NAME^ SavingCountRemovedTo(Action<mp_bitcnt_t>^ callback)
            {
                _count = callback;
                return this;
            }
    };

    /// <summary>
    /// Expression that results from a method calculating a single number from a sequence, such as a fibonacci or lucas number.  Allows to save the previous number in addition to the requested one, so that the sequence can be continued.
    /// </summary>
    public ref class MPEXPR(Sequence) abstract : MPEXPR_NAME 
    {
        internal:
            MPEXPR(Sequence)() { }
            MPTYPE^ _previous;

        public:
            /// <summary>
            /// Optionally save the previous number in the sequence to a separate result.  This cannot be the same object to which the expression is assigned.
            /// </summary>
            /// <param name="destination">destination for the previous number.  This cannot be the same object to which the expression is assigned.</param>
            /// <returns>An updated expression, with its internal state updated to additionally compute the previous number.</returns>
            MPEXPR_NAME^ SavingPreviousTo(MPTYPE^ destination)
            {
                _previous = destination;
                return this;
            }
    };

    #pragma endregion

    #pragma region concrete expressions

    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MPEXPR_NAME, Add, Int)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR_NAME, Add, Int, Ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR_NAME, Add, Int, Si)
                                                   
    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MPEXPR_NAME, Subtract, Int)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR_NAME, Subtract, Int, Ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT    (MPEXPR_NAME, Subtract, Ui, Int)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR_NAME, Subtract, Int, Si)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT    (MPEXPR_NAME, Subtract, Si, Int)

#undef ADDITIONAL_MEMBERS
#define ADDITIONAL_MEMBERS                                   \
    void virtual AddTo(MP(ptr) destination) override;        \
    void virtual SubtractFrom(MP(ptr) destination) override;

    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MPEXPR(Multiply), Multiply, Int)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR(Multiply), Multiply, Int, Ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR(Multiply), Multiply, Int, Si)

#undef ADDITIONAL_MEMBERS
#define ADDITIONAL_MEMBERS

    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR_NAME, ShiftLeft, Int, Bits)
                                                   
    DEFINE_UNARY_EXPRESSION_WITH_ONE               (MPEXPR_NAME, Negate, Int)
    DEFINE_UNARY_EXPRESSION_WITH_ONE               (MPEXPR_NAME, Abs, Int)
                                                   
    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MPEXPR(Divide), Divide, Int)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR(DivideUi), Divide, Int, Ui)
                                                   
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR(ShiftRight), ShiftRight, Int, Bits)
                                                   
    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MPEXPR(Mod), Mod, Int)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR(ModUi), Mod, Int, Ui)
                                                   
    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MPEXPR_NAME, DivideExactly, Int)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR_NAME, DivideExactly, Int, Ui)

    DEFINE_TERNARY_EXPRESSION_WITH_THREE           (MPEXPR_NAME, PowerMod, Int)
    DEFINE_TERNARY_EXPRESSION_WITH_BUILT_IN_MIDDLE (MPEXPR_NAME, PowerMod, Int, Ui, Int)

    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR_NAME, Power, Int, Ui)

    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR(Root), Root, Int, Ui)
    DEFINE_UNARY_EXPRESSION_WITH_ONE               (MPEXPR(SquareRoot), SquareRoot, Int)

    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MPEXPR_NAME, And, Int)
    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MPEXPR_NAME, Or, Int)
    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MPEXPR_NAME, Xor, Int)
    DEFINE_UNARY_EXPRESSION_WITH_ONE               (MPEXPR_NAME, Complement, Int)

    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MPEXPR_NAME, Invert, Int)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR_NAME, NextPrimeCandidate, Int, Rnd)

    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MPEXPR(Gcd), Gcd, Int)
    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MPEXPR(RemoveFactors), RemoveFactors, Int)

    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MPEXPR_NAME, Lcm, Int)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR_NAME, Lcm, Int, Ui)

    DEFINE_BINARY_EXPRESSION_WITH_BUILT_INS_ONLY   (MPEXPR_NAME, Power, Ui, Ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_INS_ONLY   (MPEXPR_NAME, Factorial, Ui, Ui)
    DEFINE_UNARY_EXPRESSION_WITH_BUILT_INS_ONLY    (MPEXPR_NAME, Primorial, Ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_INS_ONLY   (MPEXPR_NAME, Binomial, Ui, Ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR_NAME, Binomial, Int, Ui)
    DEFINE_UNARY_EXPRESSION_WITH_BUILT_INS_ONLY    (MPEXPR(Sequence), Fibonacci, Ui)
    DEFINE_UNARY_EXPRESSION_WITH_BUILT_INS_ONLY    (MPEXPR(Sequence), Lucas, Ui)

    DEFINE_BINARY_EXPRESSION(MPEXPR_NAME, RandomBits, MpirRandom^, mp_bitcnt_t)
    DEFINE_BINARY_EXPRESSION(MPEXPR_NAME, RandomBitsChunky, MpirRandom^, mp_bitcnt_t)
    DEFINE_BINARY_EXPRESSION(MPEXPR_NAME, Random, MpirRandom^, MPEXPR_NAME^)

    #pragma endregion

    #pragma region HugeInt class

    /// <summary>
    /// Multi-precision Integer class.
    /// </summary>
    public ref class MPTYPE : MPEXPR_NAME
    {
        internal:
            //fields
            MP(ptr) _value;

        private:
            //construction
            void AllocateStruct()
            {
                _value = (MP(ptr))((*__gmp_allocate_func)(sizeof(MPSTRUCT)));
            }
            void FromString(String^ value, int base);
            MPTYPE(bool initialize);

        internal:
            String^ ToString(int base, bool lowercase, int maxDigits);
            virtual void DeallocateStruct()
            {
                MP(clear)(_value);
                (*__gmp_free_func)(_value, sizeof(MPSTRUCT));
                _value = nullptr;
            }

            //assignment
            virtual void AssignTo(MP(ptr) destination) override
            {
                if(destination != _value)
                    MP(set)(destination, _value);
            }
            virtual void ASSIGN_TO(EvaluationContext& context) override
            {
                CTXT(context.Index++) = _value;
            }

        public:
            #pragma region construction and disposal

            /// <summary>
            /// Initializes a new integer instance and sets its value to 0
            /// </summary>
            MPTYPE();

            /// <summary>
            /// Initializes a new integer instance, allocating enough memory to hold at least <paramref name="bits"/> bits, and sets its value to 0.
            /// <para>This is only the initial space, integer will grow automatically in the normal way, if necessary, for subsequent values stored.
            /// </para>This makes it possible to avoid repeated reallocations if a maximum size is known in advance.
            /// </summary>
            /// <param name="bits">Minimum number of bits the initially allocated memory should hold</param>
            /// <returns>the newly constructed instance</returns>
            static MPTYPE^ Allocate(mp_bitcnt_t bits);

            /// <summary>
            /// Initializes a new integer instance and sets its value from the specified string, using leading characters to recognize the base:
            /// 0x and 0X for hexadecimal, 0b and 0B for binary, 0 for octal, or decimal otherwise.
            /// </summary>
            /// <param name="value">string representing the initial value for the new instance.  Whitespace in the string is ignored.</param>
            MPTYPE(String^ value) { FromString(value, 0); }

            /// <summary>
            /// Initializes a new integer instance and sets its value from the specified string
            /// </summary>
            /// <param name="value">string representing the initial value for the new instance.  Whitespace in the string is ignored.</param>
            /// <param name="base">base the <paramref name="value"/> string is in.
            /// <para>The base may vary from 2 to 62, or if base is 0, then the leading characters are used: 0x and 0X for hexadecimal, 0b and 0B for binary, 0 for octal, or decimal otherwise.
            /// </para>For bases up to 36, case is ignored; upper-case and lower-case letters have the same value. 
            /// For bases 37 to 62, upper-case letter represent the usual 10..35 while lower-case letter represent 36..61.</param>
            MPTYPE(String^ value, int base) { FromString(value, base); }

            /// <summary>
            /// Initializes a new integer instance and sets its value to the result of computing the source expression.
            /// </summary>
            /// <param name="value">the expression that will be computed, and the result set as the initial value of the new instance.</param>
            MPTYPE(MPEXPR_NAME^ value);

            /// <summary>
            /// Constructs and returns a new integer instance with its value set to the <paramref name="value"/> parameter.
            /// </summary>
            /// <param name="value">Initial value for the new integer instance</param>
            MPTYPE(mpir_si value);

            /// <summary>
            /// Constructs and returns a new integer instance with its value set to the <paramref name="value"/> parameter.
            /// </summary>
            /// <param name="value">Initial value for the new integer instance</param>
            MPTYPE(mpir_ui value);

            /// <summary>
            /// Constructs and returns a new integer instance with its value set to the <paramref name="value"/> parameter.
            /// </summary>
            /// <param name="value">Initial value for the new integer instance.  Any fractional portion is truncated.</param>
            MPTYPE(double value);

            /// <summary>
            /// Change the space allocated for integer to <paramref name="bits"/> bits. The value in integer is preserved if it fits, or is set to 0 if not.
            /// <para>This function can be used to increase the space for a variable in order to avoid repeated automatic reallocations, or to decrease it to give memory back to the heap.
            /// </para></summary>
            /// <param name="bits">Minimum number of bits the allocated memory should hold</param>
            void Reallocate(mp_bitcnt_t bits) { MP(realloc2)(_value, bits); }

            //disposal

            //creating a destructor in C++ implements IDisposable.

            /// <summary>
            /// Frees all memory allocated by the instance.
            /// <para>To minimize memory footprint, multi-precision objects should be disposed of when no longer used, instead of relying on the garbage collector to free the memory.
            /// </para></summary>
            ~MPTYPE() { this->!MPTYPE(); }

            /// <summary>
            /// Frees all memory allocated by the instance.
            /// <para>To minimize memory footprint, multi-precision objects should be disposed of when no longer used, instead of relying on the garbage collector to free the memory.
            /// </para></summary>
            !MPTYPE() { if(_value != 0) DeallocateStruct(); }

            #pragma endregion

            #pragma region Properties

            /// <summary>
            /// Gets the number of limbs currently allocated. This value will never be less than Size().
            /// <para>When a new value is assigned to the object, more space is automatically allocated if necessary.
            /// </para>Reallocate() can also be used manually.
            /// </summary>
            property int AllocatedSize
            {
                int get() { return _value->_mp_alloc; }
            }

            #pragma endregion

            #pragma region conversions

            /// <summary>
            /// Converts the number to a string.
            /// <para>To avoid debugging performance problems, this method outputs at most the number of digits specified in MpirSettings.ToStringDigits.
            /// If the number is larger, the least significant digits are shown with a leading ellipsis (i.e., [-]...NNNNN)
            /// </para>Setting MpirSettings.ToStringDigits to 0 removes the upper limit.
            /// </summary>
            /// <returns>A string representation of the number in decimal, possibly cut off if the number has more digits than MpirSettings.ToStringDigits.</returns>
            virtual String^ ToString() override { return ToString(10, false, MpirSettings::ToStringDigits); }

            /// <summary>
            /// Converts the number to a string in the specified base.
            /// <para>This method always produces the complete output regardless of the MpirSettings.ToStringDigits setting.
            /// </para></summary>
            /// <param name="base">The base to use for the output.  The base can be from 2 to 62; uppercase letters represent digits 10-35 and lowercase letters represent digits 36-61.</param>
            /// <returns>A string representation of the number in the specified base.</returns>
            String^ ToString(int base) { return ToString(base, false, 0); }

            /// <summary>
            /// Converts the number to a string in the specified base.
            /// <para>This method always produces the complete output regardless of the MpirSettings.ToStringDigits setting.
            /// </para></summary>
            /// <param name="base">The base to use for the output.
            /// <para>The base can be from 2 to 62; Bases up to 36 use uppercase or lowercase letters based on the <paramref name="lowercase"/> argument.
            /// </para>For bases larger than 36, the <paramref name="lowercase"/> argument is ignored and uppercase letters represent digits 10-35 while lowercase letters represent digits 36-61.</param>
            /// <param name="lowercase">Indicates if lowercase or uppercase letters should be used for the output.
            /// <para>This argument is ignored for bases larger than 36, where both uppercase and lowercase letters are used.</para></param>
            /// <returns>A string representation of the number in the specified base.</returns>
            String^ ToString(int base, bool lowercase) { return ToString(base, lowercase, 0); }

#if BITS_PER_MP_LIMB == 64
            /// <summary>
            /// Returns the absolute value of the number as a ulong.
            /// <para>If the number is too big, then just the least significant bits that do fit are returned.
            /// </para>The sign of the number is ignored, only the absolute value is used.
            /// <para>This method is supported only on 64-bit builds</para>
            /// </summary>
            /// <returns>The absolute value as a ulong, possibly truncated to the least significant bits only.</returns>
            mpir_ui ToUlong() { return MP(get_ui)(_value); }

            /// <summary>
            /// Returns the value of the number as a long.
            /// <para>If the number is too big, then just the least significant bits that do fit are returned, with the same sign as the number.
            /// </para>When truncation occurs, the result is propobly not very useful.  Call FitsLong() to check if the number will fit.
            /// <para>This method is supported only on 64-bit builds</para>
            /// </summary>
            /// <returns>The value as a long, possibly truncated to the least significant bits only.</returns>
            mpir_si ToLong() { return MP(get_si)(_value); }
#else
            /// <summary>
            /// Returns the absolute value of the number as a uint.
            /// <para>If the number is too big, then just the least significant bits that do fit are returned.
            /// </para>The sign of the number is ignored, only the absolute value is used.
            /// <para>This method is supported only on 32-bit builds</para>
            /// </summary>
            /// <returns>The absolute value as a uint, possibly truncated to the least significant bits only.</returns>
            mpir_ui ToUint() { return MP(get_ui)(_value); }

            /// <summary>
            /// Returns the value of the number as an int.
            /// <para>If the number is too big, then just the least significant bits that do fit are returned, with the same sign as the number.
            /// </para>When truncation occurs, the result is propobly not very useful.  Call FitsInt() to check if the number will fit.
            /// <para>This method is supported only on 32-bit builds</para>
            /// </summary>
            /// <returns>The value as an int, possibly truncated to the least significant bits only.</returns>
            mpir_si ToInt() { return MP(get_si)(_value); }
#endif

            /// <summary>
            /// Returns the value of the number as a double, truncating if necessary (rounding towards zero).
            /// <para>If the exponent from the conversion is too big, the result is system dependent. An infinity is returned where available.             /// A hardware overflow trap may or may not occur.
            /// </para></summary>
            /// <returns>The value as a double, possibly truncated.</returns>
            double ToDouble() { return MP(get_d)(_value); }

            /// <summary>
            /// Returns the value of the number as a double, truncating if necessary (rounding towards zero), and returning the exponent separately.
            /// <para>The return is the mantissa, its absolute value will be in the range [0.5 - 1).            /// </para>If the source value is zero, both mantissa and exponent are returned as 0.
            /// </summary>
            /// <param name="exp">variable to store the exponent in.</param>
            /// <returns>The mantissa of the value as a double, possibly truncated.</returns>
            double ToDouble([Out] mpir_si% exp) 
            { 
                mpir_si x; 
                auto result = MP(get_d_2exp)(&x, _value); 
                exp = x; 
                return result; 
            }

            #pragma endregion

            #pragma region assignment

            /// <summary>
            /// When getting, returns this integer.
            /// <para>When setting, sets the value of the integer object to the value resulting from computing the supplied expression.
            /// </para>The getter is a no-op and never needs to be invoked directly, but makes compound operators such as +=, *=, etc. possible.
            /// <para>Do not set the Value of an object while it is contained in a hash table, because that changes its hash code.
            /// </para></summary>
            /// <remarks>
            /// MPIR types are implemented as reference types with value semantics.
            /// Like Strings, the objects themselves are just lightweight pointers to data allocated elsewhere.
            /// Unlike Strings, MPIR types are mutable.
            /// <para>Value semantics requires you to be able to code, a = b + c.
            /// However, .Net (outside of C++) does not allow overloading the assignment operator,
            /// and assigning references would necessitate some unnecessary duplication and extra memory allocations.
            /// </para>To solve this problem, MPIR.Net uses the property assignment.  
            /// The setter of the Value property does what an overloaded assignment operator would do in C++.
            /// The syntax is a little different: a.Value = b + c, but it is fluent enough to become a quick habit,
            /// and additionally reinforces the concept that an existing object can change its value while reusing internally allocated memory.
            /// <para>To this end, all overloaded operators and most functions that operate on MPIR types,
            /// instead of eagerly computing a result, produce and return an expression that is basically a formula for the computation.
            /// Expressions can then be composed using additional operators to achieve expression trees of arbitrary complexity.
            /// All computations are deferred until an expression is assigned to the Value property of an MPIR object,
            /// consumed by a method or operator that returns a primitive type,
            /// or supplied as an argument to an MPIR type constructor.
            /// </para>The getter is a no-op defined to make possible constructs such as a.Value += 5, a.Value *= 10, etc.
            /// <para>Direct assignments such as a = b + c, a *= 10 will not compile because there is no implicit conversion from an expression.
            /// Even if an implicit conversion were defined, such code would incur an extra allocation plus garbage collection,
            /// and would not perform as well as doing the same operations on a.Value.
            /// </para>It would also not compile if the source were a "using" variable, as all method-local integers should be.
            /// </remarks>
            property MPEXPR_NAME^ Value
            {
                void set(MPEXPR_NAME^ expr) { expr->AssignTo(_value); }
                MPEXPR_NAME^ get() { return this; }
            }

            /// <summary>
            /// Sets the value of the integer object.
            /// <para>Do not change the value of an object while it is contained in a hash table, because that changes its hash code.
            /// </para></summary>
            /// <param name="value">new value for the object</param>
            void SetTo(mpir_ui value) { MP(set_ui)(_value, value); }

            /// <summary>
            /// Sets the value of the integer object.
            /// <para>Do not change the value of an object while it is contained in a hash table, because that changes its hash code.
            /// </para></summary>
            /// <param name="value">new value for the object</param>
            void SetTo(mpir_si value) { MP(set_si)(_value, value); }

            /// <summary>
            /// Sets the value of the integer object.  Any fractional portion is truncated.
            /// <para>Do not change the value of an object while it is contained in a hash table, because that changes its hash code.
            /// </para></summary>
            /// <param name="value">new value for the object</param>
            void SetTo(double value) { MP(set_d)(_value, value); }

            /// <summary>
            /// Sets the value of the integer object.
            /// <para>Do not change the value of an object while it is contained in a hash table, because that changes its hash code.
            /// </para></summary>
            /// <param name="value">new value for the object.  The string's leading characters may indicate base:
            /// 0x and 0X for hexadecimal, 0b and 0B for binary, 0 for octal, or decimal otherwise</param>
            void SetTo(String^ value) { SetTo(value, 0); }

            /// <summary>
            /// Sets the value of the integer object.
            /// <para>Do not change the value of an object while it is contained in a hash table, because that changes its hash code.
            /// </para></summary>
            /// <param name="value">new value for the object</param>
            /// <param name="base">base the <paramref name="value"/> string is in.
            /// <para>The base may vary from 2 to 62, or if base is 0, then the leading characters are used: 0x and 0X for hexadecimal, 0b and 0B for binary, 0 for octal, or decimal otherwise.
            /// </para>For bases up to 36, case is ignored; upper-case and lower-case letters have the same value. 
            /// For bases 37 to 62, upper-case letter represent the usual 10..35 while lower-case letter represent 36..61.</param>
            void SetTo(String^ value, int base);

            /// <summary>
            /// Sets the value of the integer object.  Any fractional portion is truncated.
            /// <para>Do not change the value of an object while it is contained in a hash table, because that changes its hash code.
            /// </para></summary>
            /// <param name="value">new value for the object</param>
            void SetTo(RationalExpression^ value);

            /// <summary>
            /// Sets the value of the integer object.  Any fractional portion is truncated.
            /// <para>Do not change the value of an object while it is contained in a hash table, because that changes its hash code.
            /// </para>If the argument is an expression, it is evaluated with the current default float precision.</summary>
            /// <param name="value">new value for the object</param>
            void SetTo(FloatExpression^ value);

            /// <summary>
            /// Swaps the values of two integers.
            /// <para>This operation is a pointer swap and doesn't affect allocated memory.
            /// </para>Do not call this method while either object is contained in a hash table, because this would change their hash codes.
            /// </summary>
            /// <param name="a">Source number to swap this instance's value with</param>
            void Swap(MPTYPE^ a) 
            { 
                MP(swap)(_value, a->_value);
            }

            #pragma endregion

            #pragma region bit operations

            /// <summary>
            /// Sets a single bit at the specified position.
            /// </summary>
            /// <param name="value">Value of the bit to set, true for 1, false for 0</param>
            /// <param name="position">Position of the bit to set.
            /// <para>The least significant bit is zero.
            /// </para>If position is beyond the current size of the number, the number is extended automatically.</param>
            void SetBit(mp_bitcnt_t position, bool value) { value ? MP(setbit)(_value, position) : MP(clrbit)(_value, position); }

            /// <summary>
            /// Gets a single bit at the specified position.
            /// </summary>
            /// <param name="position">Position of the bit to get.
            /// <para>The least significant bit is zero.
            /// </para>If position is beyond the current size of the number, returns true for negative number, false for non-negative; the number itself is not extended.</param>
            /// <returns>true if the specified bit is 1, false if zero.
            /// <para>If position is beyond the current size of the number, returns true for negative number, false for non-negative; the number itself is not extended.</para></returns>
            bool GetBit(mp_bitcnt_t position) { return MP(tstbit)(_value, position) != 0; }

            /// <summary>
            /// Complements (inverts) a single bit at the specified position.
            /// </summary>
            /// <param name="position">Position of the bit to flip.
            /// <para>The least significant bit is zero.
            /// </para>If position is beyond the current size of the number, the number is extended automatically.</param>
            void ComplementBit(mp_bitcnt_t position) { MP(combit)(_value, position); }

            #pragma endregion

            #pragma region size checks

#if BITS_PER_MP_LIMB == 64
            /// <summary>
            /// Returns true if the value of the integer is in the ulong range.
            /// <para>This method is supported only on 64-bit builds</para>
            /// </summary>
            /// <returns>true if the value will fit in a ulong</returns>
            bool FitsUlong() { return MP(fits_ui_p)(_value) != 0; }

            /// <summary>
            /// Returns true if the value of the integer is in the long range.
            /// <para>This method is supported only on 64-bit builds</para>
            /// </summary>
            /// <returns>true if the value will fit in a long</returns>
            bool FitsLong() { return MP(fits_si_p)(_value) != 0; }
#endif

            /// <summary>
            /// Returns true if the value of the integer is in the uint range.
            /// </summary>
            /// <returns>true if the value will fit in a uint</returns>
            bool FitsUint() { return MP(fits_uint_p)(_value) != 0; }

            /// <summary>
            /// Returns true if the value of the integer is in the int range.
            /// </summary>
            /// <returns>true if the value will fit in a int</returns>
            bool FitsInt() { return MP(fits_sint_p)(_value) != 0; }

            /// <summary>
            /// Returns true if the value of the integer is in the ushort range.
            /// </summary>
            /// <returns>true if the value will fit in a ushort</returns>
            bool FitsUshort() { return MP(fits_ushort_p)(_value) != 0; }

            /// <summary>
            /// Returns true if the value of the integer is in the short range.
            /// </summary>
            /// <returns>true if the value will fit in a short</returns>
            bool FitsShort() { return MP(fits_sshort_p)(_value) != 0; }

            /// <summary>
            /// Returns the number of digits the number would take if written in the specified base.
            /// <para>The sign of the number is ignored, just the absolute value is used.
            /// </para>The result will be either exact or 1 too big.
            /// If <paramref name="base"/> is a power of 2, the result will always be exact.
            /// <para>If the number is 0, the result is always 1.
            /// </para>This function can be used to estimate the space required when converting to a string.
            /// The right amount of allocation is normally two more than the value returned,
            /// one extra for a minus sign and one for the null-terminator.
            /// <para>It will be noted that base=2 can be used to locate the most significant 1 bit in op,
            /// counting from 1 (unlike all bitwise functions, which start from 0).
            /// </para></summary>
            /// <param name="base">Numeric base for the would-be string conversion, in the range from 2 to 62.</param>
            /// <returns>The number of digits the number would take written in the specified base, possibly 1 too big, not counting a leading minus.</returns>
            mp_size_t ApproximateSizeInBase(int base) { return MP(sizeinbase)(_value, base); }

            #pragma endregion

            #pragma region IO

            /// <summary>
            /// Outputs the integer to the <paramref name="stream"/> in raw binary format.
            /// <para>The number is written in a portable format, with 4 bytes of size information, and that many bytes of limbs.
            /// Both the size and the limbs are written in decreasing significance order (i.e., in big-endian).
            /// </para>The output can be read with Read(Stream).
            /// <para>The output cannot be read by MP(inp_raw) from GMP 1, because of changes necessary
            /// for compatibility between 32-bit and 64-bit machines.
            /// </para></summary>
            /// <param name="stream">Stream to output the number to</param>
            /// <returns>the number of bytes written, or 0 if an error occurs.</returns>
            size_t Write(Stream^ stream);

            /// <summary>
            /// Reads the integer value from the <paramref name="stream"/> in raw binary format, as it would have been written by Write(Stream).
            /// <para>The number is read in a portable format, with 4 bytes of size information, and that many bytes of limbs.
            /// Both the size and the limbs are written in decreasing significance order (i.e., in big-endian).
            /// </para>This routine can read the output from MP(out_raw) also from GMP 1, in spite of changes
            /// necessary for compatibility between 32-bit and 64-bit machines.
            /// </summary>
            /// <param name="stream">Stream to input the number from</param>
            /// <returns>the number of bytes read, or 0 if an error occurs.</returns>
            size_t Read(Stream^ stream);

            /// <summary>
            /// Outputs the integer to the <paramref name="writer"/> as a string of digits in decimal.
            /// <para>When writing multiple numbers that are to be read back with the Read(TextReader) method,
            /// it is useful to separate the numbers with a character that is not a valid decimal digit.
            /// </para>This is because the Read method stops reading when it encounters a character that cannot represent a digit.
            /// </summary>
            /// <param name="writer">Text writer to output the number to</param>
            /// <returns>the number of characters written</returns>
            size_t Write(TextWriter^ writer) { return Write(writer, 0, false); }

            /// <summary>
            /// Outputs the integer to the <paramref name="writer"/> as a string of digits in base <paramref name="base"/>.
            /// <para>When writing multiple numbers that are to be read back with the Read(TextReader) method,
            /// it is useful to separate the numbers with a character that is not a valid digit in base <paramref name="base"/>.
            /// </para>This is because the Read method stops reading when it encounters a character that cannot represent a digit.
            /// <para>For hexadecimal, binary, or octal, no leading base indication is written.
            /// </para>Therefore, for bases other than 10, use the Read(reader, base) overload rather than Read(reader) to read the number back.
            /// </summary>
            /// <param name="writer">Text writer to output the number to</param>
            /// <param name="base">The base to use for the output.
            /// <para>The base can be from 2 to 62; uppercase letters represent digits 10-35 while lowercase letters represent digits 36-61.</para></param>
            /// <returns>the number of characters written</returns>
            size_t Write(TextWriter^ writer, int base) { return Write(writer, base, false); }

            /// <summary>
            /// Outputs the integer to the <paramref name="writer"/> as a string of digits in base <paramref name="base"/>.
            /// <para>When writing multiple numbers that are to be read back with the Read(TextReader) method,
            /// it is useful to separate the numbers with a character that is not a valid digit in base <paramref name="base"/>.
            /// </para>This is because the Read method stops reading when it encounters a character that cannot represent a digit.
            /// <para>For hexadecimal, binary, or octal, no leading base indication is written.
            /// </para>Therefore, for bases other than 10, use the Read(reader, base) overload rather than Read(reader) to read the number back.
            /// </summary>
            /// <param name="writer">Text writer to output the number to</param>
            /// <param name="base">The base to use for the output.
            /// <para>The base can be from 2 to 62; Bases up to 36 use uppercase or lowercase letters based on the <paramref name="lowercase"/> argument.
            /// </para>For bases larger than 36, the <paramref name="lowercase"/> argument is ignored and uppercase letters represent digits 10-35 while lowercase letters represent digits 36-61.</param>
            /// <param name="lowercase">Indicates if lowercase or uppercase letters should be used for the output.
            /// <para>This argument is ignored for bases larger than 36, where both uppercase and lowercase letters are used.</para></param>
            /// <returns>the number of characters written</returns>
            size_t Write(TextWriter^ writer, int base, bool lowercase);

            /// <summary>
            /// Inputs the number as a possibly white-space preceeded string.
            /// <para>The base of the number is determined from the leading characters: 0x or 0X for hexadecimal, 0b or 0B for binary, 0 for octal, decimal otherwise.
            /// </para>Reading terminates at end-of-stream, or up to but not including a character that is not a valid digit.
            /// <para>This method reads the output of a Write(TextWriter) when decimal base is used.
            /// </para>For hexadecimal, binary, or octal, because Write(TextWriter) doesn't write leading base indication characters, 
            /// using this overload of Read will fail to recognize the correct base.</summary>
            /// <param name="reader">Text reader to input the number from</param>
            /// <returns>the number of characters read</returns>
            size_t Read(TextReader^ reader) { return Read(reader, 0); }

            /// <summary>
            /// Inputs the number as a possibly white-space preceeded string in base <paramref name="base"/> from the <paramref name="reader"/>.
            /// <para>Reading terminates at end-of-stream, or up to but not including a character that is not a valid digit.
            /// </para>This method reads the output of a Write(TextWriter) method.
            /// </summary>
            /// <param name="reader">Text reader to input the number from</param>
            /// <param name="base">The base to use for the input.
            /// <para>The base can be from 2 to 62; For bases up to 36 case is ignored.
            /// </para>For bases larger than 36, uppercase letters represent digits 10-35 while lowercase letters represent digits 36-61.
            /// <para>If 0, the base of the number is determined from the leading characters: 0x or 0X for hexadecimal, 0b or 0B for binary, 0 for octal, decimal otherwise.
            /// </para>Note that the leading base characters are not written by the Write method.</param>
            /// <returns>the number of characters read</returns>
            size_t Read(TextReader^ reader, int base);

            /// <summary>
            /// Imports the number from arbitrary words of binary data.
            /// <para>No sign information is taken from the data, the imported number will be positive or zero.</para>
            /// </summary>
            /// <typeparam name="T">Type of element in the data array.  This must be a value type, but does not need to represent a single limb.  Data is interpreted as a flat byte array.</typeparam>
            /// <param name="data">Array of binary "limbs" to import from.
            /// <para>Elements don't necessarily need to be of the <paramref name="bytesPerLimb"/> size; the data is interpreted as a flat byte array.</para></param>
            /// <param name="limbCount">Number of "limbs" to import</param>
            /// <param name="bytesPerLimb">Number of bytes per "limb."</param>
            /// <param name="limbOrder">Specifies the order of the "limbs."</param>
            /// <param name="endianness">Specifies the byte order within each "limb."</param>
            /// <param name="nails">The number of most-significant bits to ignore in each "limb."</param>
            generic<typename T> where T : value class void Import(array<T>^ data, size_t limbCount, int bytesPerLimb, LimbOrder limbOrder, Endianness endianness, int nails)
            {
                if(limbCount == 0)
                {
                    MP(set_ui)(_value, 0);
                    return;
                }

                PIN(data);
                MP(import)(_value, limbCount, (int)limbOrder, bytesPerLimb, (int)endianness, nails, pinned_data);
            }

            /// <summary>
            /// Exports the absolute value of the number to arbitrary words of binary data.
            /// <para>The sign of op is ignored.
            /// </para></summary>
            /// <typeparam name="T">Type of element in the data array.  This must be a value type, but does not need to represent a single limb.  Data is interpreted as a flat byte array.</typeparam>
            /// <param name="data">Array of binary "limbs" to export to.
            /// <para>Elements don't necessarily need to be of the <paramref name="bytesPerLimb"/> size; the data is interpreted as a flat byte array.
            /// </para>The total size of the array in bytes must be sufficient for the export.</param>
            /// <param name="bytesPerLimb">Number of bytes per "limb."</param>
            /// <param name="limbOrder">Specifies the order of the "limbs."</param>
            /// <param name="endianness">Specifies the byte order within each "limb."</param>
            /// <param name="nails">The number of most-significant bits to reserve, and set to zero, in each "limb."</param>
            /// <returns>The number of limbs exported.
            /// <para>If the number is non-zero, then the most significant word produced will be non-zero.
            /// </para>If the number is zero, then the count returned will be zero and nothing written to the data.</returns>
            generic<typename T> where T : value class size_t Export(array<T>^ data, int bytesPerLimb, LimbOrder limbOrder, Endianness endianness, int nails)
            {
                PIN(data);
                size_t limbCount;
                MP(export)(pinned_data, &limbCount, (int)limbOrder, bytesPerLimb, (int)endianness, nails, _value);
                return limbCount;
            }

            /// <summary>
            /// Exports the absolute value of the number to arbitrary words of binary data.  An array of type T is allocated for the export.
            /// <para>The sign of op is ignored.
            /// </para></summary>
            /// <typeparam name="T">Type of element in the data array.  This must be a value type, but does not need to represent a single limb.  Data is interpreted as a flat byte array.</typeparam>
            /// <param name="bytesPerLimb">Number of bytes per "limb."</param>
            /// <param name="limbOrder">Specifies the order of the "limbs."</param>
            /// <param name="endianness">Specifies the byte order within each "limb."</param>
            /// <param name="nails">The number of most-significant bits to reserve, and set to zero, in each "limb."</param>
            /// <returns>An array of type T containing the exported limb data.
            /// <para>If the number is non-zero, then the most significant word produced will be non-zero.
            /// </para>If the number is zero, then a zero-length array is returned.</returns>
            generic<typename T> where T : value class array<T>^ Export(int bytesPerLimb, LimbOrder limbOrder, Endianness endianness, int nails)
            {
                if(this->Sign() == 0)
                    return gcnew array<T>(0);

                auto bitsPerLimb = 8 * bytesPerLimb - nails;
                auto limbCount = (MP(sizeinbase)(_value, 2) - 1) / bitsPerLimb + 1;
                auto arrayCount = (limbCount * bytesPerLimb - 1) / sizeof(T) + 1;
                auto data = gcnew array<T>((int)arrayCount);

                PIN(data);
                MP(export)(pinned_data, &limbCount, (int)limbOrder, bytesPerLimb, (int)endianness, nails, _value);
                return data;
            }

        internal:
            size_t ReadNoWhite(TextReader^ reader, int base, size_t nread);

        public:

            /// <summary>
            /// Returns the specified limb of the number.
            /// <para>The least significant limb is zero.
            /// </para>The sign of the number is ignored.
            /// </summary>
            /// <param name="index">The index of the limb to return.
            /// <para>The least significant limb is zero.
            /// </para>If the index is outside the range 0 to Size()-1, zero is returned.</param>
            /// <returns>The specified limb, or zero if <paramref name="index"/> is outside of the valid range.</returns>
            size_t GetLimb(mp_size_t index) { return MP(getlimbn)(_value, index); }

            #pragma endregion
    
            #pragma region number-theoretic

            /// <summary>
            /// Determines whether the number is a probable prime with the chance of error being at most 1 in 2^<paramref name="probability"/>.
            /// <para>This function does some trial divisions to speed up the average case, then some probabilistic
            /// primality tests to achieve the desired level of error.
            /// </para>This function interface is preliminary and may change in the future.
            /// </summary>
            /// <param name="random">Random number generator to use for probabilistic primality tests</param>
            /// <param name="probability">Defines the maximum allowed probability of a false positive.
            /// <para>The odds of a composite number being reported as a probable prime are at most 1 in 2^probability</para></param>
            /// <param name="pretested">Used to inform the function that trial division up to div has already been performed,
            /// and so the number is known to have NO divisors &lt;= pretested.
            /// <para>Use 0 to inform the function that no trial division has been done.</para></param>
            /// <returns>true if the number is probably prime, or false if it is definitely composite.</returns>
            bool IsProbablePrime(MpirRandom^ random, int probability, mpir_ui pretested);

            /// <summary>
            /// Determines whether the number is likely a prime, i.e. you can consider it a prime for practical purposes.
            /// <para>This function does some trial divisions to speed up the average case, then some probabilistic primality tests.
            /// </para>The term "likely" refers to the fact that the number will not have small factors.
            /// <para>This function interface is preliminary and may change in the future.
            /// </para></summary>
            /// <param name="random">Random number generator to use for probabilistic primality tests</param>
            /// <param name="pretested">Used to inform the function that trial division up to div has already been performed,
            /// and so the number is known to have NO divisors &lt;= pretested.
            /// <para>Use 0 to inform the function that no trial division has been done.</para></param>
            /// <returns>true if the number is likely prime, or false if it is definitely composite.</returns>
            bool IsLikelyPrime(MpirRandom^ random, mpir_ui pretested);

            /// <summary>
            /// Calculates the Jacobi symbol (<paramref name="a"/>/<paramref name="b"/>).
            /// <para>This is defined only for <paramref name="b"/> odd.
            /// </para></summary>
            /// <param name="a">First source value for the Jacobi symbol</param>
            /// <param name="b">Second source value for the Jacobi symbol</param>
            /// <returns>The Jacobi symbol (-1, 0, or 1).  Return is undefined unless <paramref name="b"/> is odd.</returns>
            static int Jacobi(MPTYPE^ a, MPTYPE^ b) { return MP(jacobi)(a->_value, b->_value); }

            /// <summary>
            /// Calculates the Legendre symbol (<paramref name="a"/>/<paramref name="b"/>).
            /// <para>This is defined only when <paramref name="b"/> is an odd prime.
            /// </para></summary>
            /// <param name="a">First source value for the Legendre symbol</param>
            /// <param name="b">Second source value for the Legendre symbol</param>
            /// <returns>The Legendre symbol (-1, 0, or 1).  Return is undefined unless <paramref name="b"/> is an odd prime.</returns>
            static int Legendre(MPTYPE^ a, MPTYPE^ b) { return MP(legendre)(a->_value, b->_value); }

            /// <summary>
            /// Calculates the Jacobi symbol (<paramref name="a"/>/<paramref name="b"/>) with the Kronecker extension
            /// (<paramref name="a"/>/2) = (2/<paramref name="a"/>) when a odd, or (<paramref name="a"/>/2) = 0 when a even.
            /// <para>When <paramref name="b"/> is odd the Jacobi symbol and Kronecker symbol are identical, so the various Kronecker overloads 
            /// can be used for mixed precision Jacobi symbols too.
            /// </para></summary>
            /// <param name="a">First source value for the Kronecker symbol</param>
            /// <param name="b">Second source value for the Kronecker symbol</param>
            /// <returns>The Kronecker symbol (-1, 0, or 1).</returns>
            static int Kronecker(MPTYPE^ a, MPTYPE^ b) { return MP(kronecker)(a->_value, b->_value); }

            /// <summary>
            /// Calculates the Jacobi symbol (<paramref name="a"/>/<paramref name="b"/>) with the Kronecker extension
            /// (<paramref name="a"/>/2) = (2/<paramref name="a"/>) when a odd, or (<paramref name="a"/>/2) = 0 when a even.
            /// <para>When <paramref name="b"/> is odd the Jacobi symbol and Kronecker symbol are identical, so the various Kronecker overloads 
            /// can be used for mixed precision Jacobi symbols too.
            /// </para></summary>
            /// <param name="a">First source value for the Kronecker symbol</param>
            /// <param name="b">Second source value for the Kronecker symbol</param>
            /// <returns>The Kronecker symbol (-1, 0, or 1).</returns>
            static int Kronecker(MPTYPE^ a, mpir_ui b) { return MP(kronecker_ui)(a->_value, b); }

            /// <summary>
            /// Calculates the Jacobi symbol (<paramref name="a"/>/<paramref name="b"/>) with the Kronecker extension
            /// (<paramref name="a"/>/2) = (2/<paramref name="a"/>) when a odd, or (<paramref name="a"/>/2) = 0 when a even.
            /// <para>When <paramref name="b"/> is odd the Jacobi symbol and Kronecker symbol are identical, so the various Kronecker overloads 
            /// can be used for mixed precision Jacobi symbols too.
            /// </para></summary>
            /// <param name="a">First source value for the Kronecker symbol</param>
            /// <param name="b">Second source value for the Kronecker symbol</param>
            /// <returns>The Kronecker symbol (-1, 0, or 1).</returns>
            static int Kronecker(MPTYPE^ a, mpir_si b) { return MP(kronecker_si)(a->_value, b); }

            /// <summary>
            /// Calculates the Jacobi symbol (<paramref name="a"/>/<paramref name="b"/>) with the Kronecker extension
            /// (<paramref name="a"/>/2) = (2/<paramref name="a"/>) when a odd, or (<paramref name="a"/>/2) = 0 when a even.
            /// <para>When <paramref name="b"/> is odd the Jacobi symbol and Kronecker symbol are identical, so the various Kronecker overloads 
            /// can be used for mixed precision Jacobi symbols too.
            /// </para></summary>
            /// <param name="a">First source value for the Kronecker symbol</param>
            /// <param name="b">Second source value for the Kronecker symbol</param>
            /// <returns>The Kronecker symbol (-1, 0, or 1).</returns>
            static int Kronecker(mpir_ui a, MPTYPE^ b) { return MP(ui_kronecker)(a, b->_value); }

            /// <summary>
            /// Calculates the Jacobi symbol (<paramref name="a"/>/<paramref name="b"/>) with the Kronecker extension
            /// (<paramref name="a"/>/2) = (2/<paramref name="a"/>) when a odd, or (<paramref name="a"/>/2) = 0 when a even.
            /// <para>When <paramref name="b"/> is odd the Jacobi symbol and Kronecker symbol are identical, so the various Kronecker overloads 
            /// can be used for mixed precision Jacobi symbols too.
            /// </para></summary>
            /// <param name="a">First source value for the Kronecker symbol</param>
            /// <param name="b">Second source value for the Kronecker symbol</param>
            /// <returns>The Kronecker symbol (-1, 0, or 1).</returns>
            static int Kronecker(mpir_si a, MPTYPE^ b) { return MP(si_kronecker)(a, b->_value); }

            /// <summary>
            /// Returns an expression for calculating <paramref name="value"/> raised to the specified <paramref name="power"/>.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="value">Base for the initial value for the new integer instance</param>
            /// <param name="power">Power to raise the <paramref name="value"/> to when calculating the initial value for the new instance</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ Power(mpir_ui value, mpir_ui power) { return gcnew MPEXPR(PowerUiUi)(value, power); }

            /// <summary>
            /// Returns an expression for calculating the factorial of <paramref name="a"/>.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">The source number to take the factorial of</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ Factorial(mpir_ui a) { return gcnew MPEXPR(FactorialUiUi)(a, 1); }

            /// <summary>
            /// Returns an expression for calculating the multifactorial of <paramref name="a"/> of the specified order.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">The source number to take the multifactorial of</param>
            /// <param name="order">The order of the multifactorial, i.e. 2 for <paramref name="a"/>!!, 3 for <paramref name="a"/>!!!, etc.</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ Factorial(mpir_ui a, mpir_ui order) { return gcnew MPEXPR(FactorialUiUi)(a, order); }

            /// <summary>
            /// Returns an expression for calculating the primorial of <paramref name="a"/>, i.e. the product of all positive prime numbers &lt;= a.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">The source number to take the primorial of</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ Primorial(mpir_ui a) { return gcnew MPEXPR(PrimorialUi)(a); }

            /// <summary>
            /// Returns an expression for calculating the binomial coefficient (<paramref name="n"/>, <paramref name="k"/>), a.k.a. number of k-element combinations out of an n-element set.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="n">The first source value of the binomial coefficient, a.k.a. set size</param>
            /// <param name="k">The second source value of the binomial coefficient, a.k.a. subset size</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ Binomial(mpir_ui n, mpir_ui k) { return gcnew MPEXPR(BinomialUiUi)(n, k); }

            /// <summary>
            /// Returns an expression for calculating the binomial coefficient (<paramref name="n"/>, <paramref name="k"/>), a.k.a. number of k-element combinations out of an n-element set.
            /// <para>Negative values of <paramref name="n"/> are supported, using the identity (-n, k) = (-1)^k * (n + k - 1, k).
            /// </para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </summary>
            /// <param name="n">The first source value of the binomial coefficient, a.k.a. set size
            /// <para>Negative values of <paramref name="n"/> are supported, using the identity (-n, k) = (-1)^k * (n + k - 1, k).
            /// </para></param>
            /// <param name="k">The second source value of the binomial coefficient, a.k.a. subset size</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ Binomial(MPEXPR_NAME^ n, mpir_ui k) { return gcnew MPEXPR(BinomialIntUi)(n, k); }

            /// <summary>
            /// Returns an expression for calculating the <paramref name="n"/>th Fibonacci number.
            /// <para>You can also optionally save the (<paramref name="n"/>-1)th number by calling a method on the resulting expression.
            /// </para>This method is designed for calculating isolated Fibonacci numbers. When a sequence of
            /// values is wanted it’s best to start with a pair of numbers (Fn and Fn-1) by calling SettingPreviousTo(),
            /// and then iterate the defining Fn+1 = Fn + Fn-1.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para>Fibonacci and Lucas numbers are closely related, and it's never necessary to calculate both Fn and Ln.
            /// </summary>
            /// <param name="n">The index of the Fibonacci number to calculate</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR(Sequence)^ Fibonacci(mpir_ui n) { return gcnew MPEXPR(FibonacciUi)(n); }

            /// <summary>
            /// Returns an expression for calculating the <paramref name="n"/>th Lucas number.
            /// <para>You can also optionally save the (<paramref name="n"/>-1)th number by calling a method on the resulting expression.
            /// </para>This method is designed for calculating isolated Lucas numbers. When a sequence of
            /// values is wanted it’s best to start with a pair of numbers (Ln and Ln-1) by calling SettingPreviousTo(),
            /// and then iterate the defining Ln+1 = Ln + Ln-1.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para>Fibonacci and Lucas numbers are closely related, and it's never necessary to calculate both Fn and Ln.
            /// </summary>
            /// <param name="n">The index of the Lucas number to calculate</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR(Sequence)^ Lucas(mpir_ui n) { return gcnew MPEXPR(LucasUi)(n); }

            #pragma endregion
    };

    #pragma endregion

    #pragma region HugeIntComponent

    /// <summary>This internal class is used to provide access to the numerator and denominator of a Rational number.
    /// <para>It is a thin override of HugeInt with the only changes being that it does not perform any allocation/cleanup, 
    /// it simply reuses an mpz_ptr from a Rational struct that is allocated and freed by HugeRational.
    /// </para>It inherits the IDisposable implementation from HugeInt, but overrides the DeallocateStruct worker with a no-op.
    /// </summary>
    private ref class HugeIntComponent sealed : MPTYPE
    {
        internal:
            virtual void DeallocateStruct() override { }
            HugeIntComponent(MP(ptr) value) { _value = value; }
    };

    #pragma endregion
};