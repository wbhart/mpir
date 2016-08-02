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
#undef CTXTI
#undef ASSIGN_TO
#undef Mpt
#undef SET_CONTEXT_PRECISION
#endif
#define SPECIALIZE_EXPRESSIONS
#define Mpt Flt
#define CUSTOM_MP(x) custom_mpf_##x
#define MPSTRUCT __mpf_struct
#define MP(x) mpf_##x
#define MPTYPE HugeFloat
#define MPTYPE_NAME Float
#define MPEXPR_NAME LIT(MPTYPE_NAME)Expression
#define MPEXPR(x) LIT(MPTYPE_NAME)##x##Expression
#define CTXT(x) context.FloatArgs[x]
#define CTXTI(x) context.IntArgs[x]
#define CTXTR(x) context.RationalArgs[x]
#define ASSIGN_TO CONCAT(AssignTo, LIT(MPTYPE_NAME))
#define SET_CONTEXT_PRECISION context.FloatPrecision = mpf_get_prec(destination);
#include "ExpressionMacros.h"

namespace MPIR
{
    ref class MpirRandom;
    ref class MPTYPE;

    #pragma region FloatExpression

    /// <summary>
    /// Base class for all float expressions resulting from many float operations on MPIR types.
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
                context.Initialized(FloatInitialized);
                auto ptr = &context.Temp[context.Index].MPTYPE_NAME;
                CTXT(context.Index++) = ptr;
                MP(init2)(ptr, context.FloatPrecision);
                AssignTo(ptr);
            }

        private:
            int CompareTo(Object^ a, bool& valid);

        public:
            #pragma region Arithmetic

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
            static MPEXPR_NAME^ operator * (MPEXPR_NAME^ a, MPEXPR_NAME^ b);
                                                                                              
            /// <summary>Multiplies two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to multiply</param>
            /// <param name="b">Source value to multiply by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ operator * (MPEXPR_NAME^ a, mpir_ui b);
                                                                                              
            /// <summary>Multiplies two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to multiply</param>
            /// <param name="b">Source value to multiply by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ operator * (mpir_ui a, MPEXPR_NAME^ b);
                                                                                              
            /// <summary>Multiplies two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to multiply</param>
            /// <param name="b">Source value to multiply by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ operator * (MPEXPR_NAME^ a, mpir_si b);
                                                                                              
            /// <summary>Multiplies two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to multiply</param>
            /// <param name="b">Source value to multiply by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ operator * (mpir_si a, MPEXPR_NAME^ b);
                                                                                                          
            /// <summary>Shifts the <paramref name="a"/> source operand to the left by <paramref name="bits"/>, i.e. multiplies <paramref name="a"/> by 2^<paramref name="bits"/>.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to multiply</param>
            /// <param name="bits">Number of bits to shift <paramref name="a"/> by, i.e. power of 2 to multiply <paramref name="a"/> by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ operator << (MPEXPR_NAME^ a, mp_bitcnt_t bits);
                                                                                                          
            /// <summary>Shifts the <paramref name="a"/> source operand to the right by <paramref name="bits"/>, i.e. divides <paramref name="a"/> by 2^<paramref name="bits"/>.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to divide</param>
            /// <param name="bits">Number of bits to shift <paramref name="a"/> by, i.e. power of 2 to divide <paramref name="a"/> by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation.
            /// </returns>
            static MPEXPR_NAME^ operator >> (MPEXPR_NAME^ a, mp_bitcnt_t bits);

            /// <summary>Negates the source value.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to negate</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ operator - (MPEXPR_NAME^ a);
                                                                                                          
            /// <summary>Divides two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to divide</param>
            /// <param name="b">Source value to divide <paramref name="a"/> by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation.
            /// </returns>
            static MPEXPR_NAME^ operator / (MPEXPR_NAME^ a, MPEXPR_NAME^ b);
                                                                                              
            /// <summary>Divides two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to divide</param>
            /// <param name="b">Source value to divide <paramref name="a"/> by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation.
            /// </returns>
            static MPEXPR_NAME^ operator / (MPEXPR_NAME^ a, mpir_ui b);
                                                                                                          
            /// <summary>Divides two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to divide</param>
            /// <param name="b">Source value to divide <paramref name="a"/> by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation.
            /// </returns>
            static MPEXPR_NAME^ operator / (mpir_ui a, MPEXPR_NAME^ b);
                                                                                              
            /// <summary>Divides two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to divide</param>
            /// <param name="b">Source value to divide <paramref name="a"/> by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation.
            /// </returns>
            static MPEXPR_NAME^ operator / (MPEXPR_NAME^ a, mpir_si b);
                                                                                                          
            /// <summary>Divides two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to divide</param>
            /// <param name="b">Source value to divide <paramref name="a"/> by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation.
            /// </returns>
            static MPEXPR_NAME^ operator / (mpir_si a, MPEXPR_NAME^ b);
                                                                                                          
            /// <summary>Raises the source value to the specified power.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to multiply</param>
            /// <param name="power">Power to raise <paramref name="a"/> to</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ operator ^ (MPEXPR_NAME^ a, mpir_ui power);

            #pragma endregion

            #pragma region Math

            /// <summary>Computes the absolute value of the source number.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MPEXPR_NAME^ Abs();

            /// <summary>Computes the square root of the source number.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MPEXPR_NAME^ SquareRoot();

            /// <summary>Rounds the source number down to the next integer.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MPEXPR_NAME^ Floor();

            /// <summary>Rounds the source number up to the next integer.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MPEXPR_NAME^ Ceiling();

            /// <summary>Rounds the source number to the next integer toward zero.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MPEXPR_NAME^ Truncate();

            #pragma endregion

            #pragma region Comparisons

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para>The precision of the calculation is the precision of this instance if it is a computed number, otherwise the precision of <paramref name="a"/> if that is a computed number,
            /// otherwise the current default float precision.
            /// <para>When the argument is a double, it may be an infinity, but results are undefined for a NaN.</para></summary>
            /// <param name="a">Value to compare the source with</param>
            /// <returns>A positive number if the source is greater than <paramref name="a"/>, negative if less, and zero if they are equal.</returns>
            virtual int CompareTo(Object^ a) sealed;

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para>The precision of the calculation is the precision of this instance if it is a computed number, otherwise the precision of <paramref name="a"/> if that is a computed number,
            /// otherwise the current default float precision if both are expressions.</summary>
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
            /// </para>When the argument is a double, it may be an infinity, but results are undefined for a NaN.</summary>
            /// <param name="a">Value to compare the source with.  This can be a multi-precision number, an expression, or a supported primitive type (long, ulong, or double).</param>
            /// <returns>true if the values of the source and <paramref name="a"/> are equal, false otherwise.</returns>
            virtual bool Equals(Object^ a) override sealed;

            /// <summary>Compares two numbers approximately, taking into account <paramref name="precision"/> most significant bits of the mantissa.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable with the specified <paramref name="precision"/> before the comparison is performed.
            /// </para>In the future values like 1000 and 0111 may be considered the same to 3 bits (on the basis that their difference is that small).
            /// </summary>
            /// <param name="a">Value to compare the source with</param>
            /// <param name="precision">The number of most significant bits that must match for the two numbers to be considered equal</param>
            /// <returns>true if the values of the source and <paramref name="a"/> are equal to <paramref name="precision"/>, false otherwise.</returns>
            bool Equals(MPEXPR_NAME^ a, mp_bitcnt_t precision) { IN_SPECIFIC_CONTEXT(precision, this, a); return MP(eq)(CTXT(0), CTXT(1), precision) != 0; }

            /// <summary>Computes the hash code of the source value.
            /// <para>If called on an expression, it is evaluated into a temporary variable with the current default float precision before the calculation is performed.
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
            /// </para>Can be called with an infinity, but results are undefined for a NaN.</summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <  (MPEXPR_NAME^ a, double b) { return  IS_NULL(a) || a->CompareTo(b) < 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para>Can be called with an infinity, but results are undefined for a NaN.</summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >= (MPEXPR_NAME^ a, double b) { return !IS_NULL(a) && a->CompareTo(b) >= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para>Can be called with an infinity, but results are undefined for a NaN.</summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >  (MPEXPR_NAME^ a, double b) { return !IS_NULL(a) && a->CompareTo(b) > 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para>Can be called with an infinity, but results are undefined for a NaN.</summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <= (MPEXPR_NAME^ a, double b) { return  IS_NULL(a) || a->CompareTo(b) <= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para>Can be called with an infinity, but results are undefined for a NaN.</summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator != (MPEXPR_NAME^ a, double b) { return  IS_NULL(a) || a->CompareTo(b) != 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para>Can be called with an infinity, but results are undefined for a NaN.</summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator == (MPEXPR_NAME^ a, double b) { return !IS_NULL(a) && a->CompareTo(b) == 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para>Can be called with an infinity, but results are undefined for a NaN.</summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <  (double b, MPEXPR_NAME^ a) { return !IS_NULL(a) && a->CompareTo(b) > 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para>Can be called with an infinity, but results are undefined for a NaN.</summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >= (double b, MPEXPR_NAME^ a) { return  IS_NULL(a) || a->CompareTo(b) <= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para>Can be called with an infinity, but results are undefined for a NaN.</summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >  (double b, MPEXPR_NAME^ a) { return  IS_NULL(a) || a->CompareTo(b) < 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para>Can be called with an infinity, but results are undefined for a NaN.</summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <= (double b, MPEXPR_NAME^ a) { return !IS_NULL(a) && a->CompareTo(b) >= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para>Can be called with an infinity, but results are undefined for a NaN.</summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator != (double b, MPEXPR_NAME^ a) { return  IS_NULL(a) || a->CompareTo(b) != 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para>Can be called with an infinity, but results are undefined for a NaN.</summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator == (double b, MPEXPR_NAME^ a) { return !IS_NULL(a) && a->CompareTo(b) == 0; }

            /// <summary>Calculates the sign (+1, 0, or -1) of the source value.
            /// <para>If the source is an expression, it is evaluated into a temporary variable with the current default float precision before the sign is computed.
            /// </para></summary>
            /// <returns>+1 if the source is positive, -1 if negative, and 0 if zero.</returns>
            int Sign();

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para>The result equals | source - a | / source.</summary>
            /// <param name="a">Source value to compare with</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MPEXPR_NAME^ RelativeDifferenceFrom(MPEXPR_NAME^ a);

            #pragma endregion
    };

    #pragma endregion

    #pragma region concrete expressions

    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MPEXPR_NAME, Add, Flt)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR_NAME, Add, Flt, Ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR_NAME, Add, Flt, Si)
                                                   
    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MPEXPR_NAME, Subtract, Flt)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR_NAME, Subtract, Flt, Ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT    (MPEXPR_NAME, Subtract, Ui, Flt)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR_NAME, Subtract, Flt, Si)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT    (MPEXPR_NAME, Subtract, Si, Flt)
                                                   
    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MPEXPR_NAME, Multiply, Flt)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR_NAME, Multiply, Flt, Ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR_NAME, Multiply, Flt, Si)
                                                   
    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MPEXPR_NAME, Divide, Flt)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR_NAME, Divide, Flt, Ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT    (MPEXPR_NAME, Divide, Ui, Flt)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR_NAME, Divide, Flt, Si)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT    (MPEXPR_NAME, Divide, Si, Flt)
                                                   
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR_NAME, ShiftLeft, Flt, Bits)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR_NAME, ShiftRight, Flt, Bits)
                                                   
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MPEXPR_NAME, Power, Flt, Ui)

    DEFINE_UNARY_EXPRESSION_WITH_ONE               (MPEXPR_NAME, Negate, Flt)
    DEFINE_UNARY_EXPRESSION_WITH_ONE               (MPEXPR_NAME, Abs, Flt)
    DEFINE_UNARY_EXPRESSION_WITH_ONE               (MPEXPR_NAME, SquareRoot, Flt)
    DEFINE_UNARY_EXPRESSION_WITH_BUILT_INS_ONLY    (MPEXPR_NAME, SquareRoot, Ui)
                                                   
    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MPEXPR_NAME, RelativeDifferenceFrom, Flt)

    DEFINE_UNARY_EXPRESSION_WITH_ONE               (MPEXPR_NAME, Floor, Flt)
    DEFINE_UNARY_EXPRESSION_WITH_ONE               (MPEXPR_NAME, Ceiling, Flt)
    DEFINE_UNARY_EXPRESSION_WITH_ONE               (MPEXPR_NAME, Truncate, Flt)

    DEFINE_UNARY_EXPRESSION                        (MPEXPR_NAME, Random, MpirRandom^)
    DEFINE_BINARY_EXPRESSION                       (MPEXPR_NAME, RandomBits, MpirRandom^, mp_bitcnt_t)
    DEFINE_BINARY_EXPRESSION                       (MPEXPR_NAME, RandomChunky, MpirRandom^, mp_exp_t)
    DEFINE_TERNARY_EXPRESSION                      (MPEXPR_NAME, RandomLimbsChunky, MpirRandom^, mp_size_t, mp_exp_t)

    #pragma endregion

    #pragma region HugeFloat class

    /// <summary>
    /// Multi-precision Float class.
    /// </summary>
    public ref class MPTYPE : MPEXPR_NAME
    {
        internal:
            //fields
            MP(ptr) _value;
            mp_bitcnt_t _allocatedPrecision;

        private:
            //construction
            void AllocateStruct()
            {
                _value = (MP(ptr))((*__gmp_allocate_func)(sizeof(MPSTRUCT)));
            }
            void FromString(String^ value, int base, bool exponentInDecimal);
            MPTYPE(bool initialize);
            String^ ToString(int base, bool lowercase, int maxDigits, bool exponentInDecimal);

        internal:
            virtual void DeallocateStruct()
            {
                MP(set_prec_raw)(_value, _allocatedPrecision);
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

            static MPTYPE()
            {
                DefaultPrecision = sizeof(mpir_ui) * 8 * 2; //2 limbs
            }

            /// <summary>
            /// Gets or sets the default precision of a float variable in bits.
            /// <para>The actual precision may be slightly greater if the value is not a whole limb multiple.
            /// </para>All subsequently constructed variables will use this precision, but previously constructed variables are unaffected.
            /// <para>The initial default value is 2 limbs.
            /// </para></summary>
            static property mp_bitcnt_t DefaultPrecision
            {
                mp_bitcnt_t get() { return MP(get_default_prec)(); }
                void set(mp_bitcnt_t value) { MP(set_default_prec)(value); }
            }

            /// <summary>
            /// Initializes a new float instance and sets its value to 0.
            /// <para>The precision of the new variable's mantissa is set from the static DefaultPrecision property.
            /// </para></summary>
            MPTYPE();

            /// <summary>
            /// Initializes a new float instance, allocating enough memory to hold at least <paramref name="precision"/> bits, and sets its value to 0.
            /// <para>All float operations are performed to the precision of the destination.
            /// </para></summary>
            /// <param name="precision">Minimum number of bits the initially allocated memory should hold for the mantissa</param>
            /// <returns>the newly constructed instance</returns>
            static MPTYPE^ Allocate(mp_bitcnt_t precision);

            /// <summary>
            /// Initializes a new float instance and sets its value from the specified string.
            /// <para>No leading base characters are allowed.
            /// </para>The exponent is always in decimal.
            /// </summary>
            /// <param name="value">string representing the initial value for the new instance.  Whitespace in the string is ignored.</param>
            MPTYPE(String^ value) { FromString(value, 0, true); }

            /// <summary>
            /// Initializes a new float instance and sets its value from the specified string
            /// </summary>
            /// <param name="value">string representing the initial value for the new instance.  Whitespace in the string is ignored.</param>
            /// <param name="base">base the <paramref name="value"/> string is in.
            /// <para>The base may vary from 2 to 62.  No leading base characters are allowed.  The exponent is always in decimal
            /// </para>For bases up to 36, case is ignored; upper-case and lower-case letters have the same value. 
            /// For bases 37 to 62, upper-case letter represent the usual 10..35 while lower-case letter represent 36..61.</param>
            MPTYPE(String^ value, int base) { FromString(value, base, true); }

            /// <summary>
            /// Initializes a new float instance and sets its value from the specified string
            /// </summary>
            /// <param name="value">string representing the initial value for the new instance.  Whitespace in the string is ignored.</param>
            /// <param name="base">base the <paramref name="value"/> string is in.
            /// <para>The base may vary from 2 to 62.  No leading base characters are allowed.
            /// </para>For bases up to 36, case is ignored; upper-case and lower-case letters have the same value. 
            /// For bases 37 to 62, upper-case letter represent the usual 10..35 while lower-case letter represent 36..61.</param>
            /// <param name="exponentInDecimal">True if the exponent is in decimal, false to use the same base as the mantissa.</param>
            MPTYPE(String^ value, int base, bool exponentInDecimal) { FromString(value, base, exponentInDecimal); }

            /// <summary>
            /// Initializes a new float instance and sets its value to the result of computing the source expression.
            /// </summary>
            /// <param name="value">the expression that will be computed, and the result set as the initial value of the new instance.</param>
            MPTYPE(MPEXPR_NAME^ value);

            /// <summary>
            /// Initializes a new float instance and sets its value to the result of computing the source expression.
            /// </summary>
            /// <param name="value">the expression that will be computed, and the result set as the initial value of the new instance.</param>
            MPTYPE(IntegerExpression^ value);

            /// <summary>
            /// Initializes a new float instance and sets its value to the result of computing the source expression.
            /// </summary>
            /// <param name="value">the expression that will be computed, and the result set as the initial value of the new instance.</param>
            MPTYPE(RationalExpression^ value);

            /// <summary>
            /// Constructs and returns a new float instance with its value set to <paramref name="value"/>.
            /// </summary>
            /// <param name="value">value for the initial value for the new float instance</param>
            MPTYPE(mpir_si value);

            /// <summary>
            /// Constructs and returns a new float instance with its value set to <paramref name="value"/>.
            /// </summary>
            /// <param name="value">value for the initial value for the new float instance</param>
            MPTYPE(mpir_ui value);

            /// <summary>
            /// Constructs and returns a new float instance with its value set to the <paramref name="value"/> parameter.
            /// <para>There is no rounding, this conversion is exact.</para>
            /// </summary>
            /// <param name="value">Initial value for the new float instance.  This is an exact conversion.</param>
            MPTYPE(double value);

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

            #pragma region conversions

            /// <summary>
            /// Converts the number to a string.
            /// <para>To avoid debugging performance problems, this method outputs at most the number of digits specified in MpirSettings.ToStringDigits.
            /// If the number is larger, the least significant digits are shown with a leading ellipsis (i.e., [-]...NNNNN)
            /// </para>Setting MpirSettings.ToStringDigits to 0 removes the upper limit.
            /// </summary>
            /// <returns>A string representation of the number in decimal, possibly cut off if the number has more digits than MpirSettings.ToStringDigits.</returns>
            virtual String^ ToString() override { return ToString(10, false, MpirSettings::ToStringDigits, false); }

            /// <summary>
            /// Converts the number to a string in the specified base.
            /// <para>This method always produces the complete output regardless of the MpirSettings.ToStringDigits setting.
            /// </para></summary>
            /// <param name="base">The base to use for the output.  The base can be from 2 to 62; uppercase letters represent digits 10-35 and lowercase letters represent digits 36-61.</param>
            /// <returns>A string representation of the number in the specified base.</returns>
            String^ ToString(int base) { return ToString(base, false, 0, false); }

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
            String^ ToString(int base, bool lowercase) { return ToString(base, lowercase, 0, false); }

            /// <summary>
            /// Converts the number to a string in the specified base.
            /// <para>This method always produces the complete output regardless of the MpirSettings.ToStringDigits setting.
            /// </para></summary>
            /// <param name="base">The base to use for the output.
            /// <para>The base can be from 2 to 62; Bases up to 36 use uppercase or lowercase letters based on the <paramref name="lowercase"/> argument.
            /// </para>For bases larger than 36, the <paramref name="lowercase"/> argument is ignored and uppercase letters represent digits 10-35 while lowercase letters represent digits 36-61.</param>
            /// <param name="lowercase">Indicates if lowercase or uppercase letters should be used for the output.
            /// <para>This argument is ignored for bases larger than 36, where both uppercase and lowercase letters are used.</para></param>
            /// <param name="exponentInDecimal">True to always show the exponent in decimal, False to use the same base as the mantissa</param>
            /// <returns>A string representation of the number in the specified base.</returns>
            String^ ToString(int base, bool lowercase, bool exponentInDecimal) { return ToString(base, lowercase, 0, exponentInDecimal); }

#if BITS_PER_MP_LIMB == 64
            /// <summary>
            /// Returns the absolute value of the number as a ulong, truncating any fractional part.
            /// <para>If the number is too big, the result is undefined.  Call FitsUlong() to check if the number will fit.
            /// </para>The sign of the number is ignored, only the absolute value is used.
            /// <para>This method is supported only on 64-bit builds</para>
            /// </summary>
            /// <returns>The absolute value as a ulong, with any fractional part truncated.</returns>
            mpir_ui ToUlong() { return MP(get_ui)(_value); }

            /// <summary>
            /// Returns the value of the number as a long.
            /// <para>If the number is too big, the result is undefined.  Call FitsLong() to check if the number will fit.
            /// </para>This method is supported only on 64-bit builds
            /// </summary>
            /// <returns>The value as a long, with any fractional part truncated.</returns>
            mpir_si ToLong() { return MP(get_si)(_value); }
#else
            /// <summary>
            /// Returns the absolute value of the number as a uint, truncating any fractional part.
            /// <para>If the number is too big, the result is undefined.  Call FitsUint() to check if the number will fit.
            /// </para>The sign of the number is ignored, only the absolute value is used.
            /// <para>This method is supported only on 32-bit builds</para>
            /// </summary>
            /// <returns>The absolute value as a uint, with any fractional part truncated.</returns>
            mpir_ui ToUint() { return MP(get_ui)(_value); }

            /// <summary>
            /// Returns the value of the number as an int.
            /// <para>If the number is too big, the result is undefined.  Call FitsInt() to check if the number will fit.
            /// </para>This method is supported only on 32-bit builds
            /// </summary>
            /// <returns>The value as an int, with any fractional part truncated.</returns>
            mpir_si ToInt() { return MP(get_si)(_value); }
#endif

            /// <summary>
            /// Returns the value of the number as a double, truncating if necessary (rounding towards zero).
            /// <para>If the exponent from the conversion is too big, the result is system dependent. An infinity is returned where available. 
            /// A hardware overflow trap may or may not occur.
            /// </para></summary>
            /// <returns>The value as a double, possibly truncated.</returns>
            double ToDouble() { return MP(get_d)(_value); }

            /// <summary>
            /// Returns the value of the number as a double, truncating if necessary (rounding towards zero), and returning the exponent separately.
            /// <para>The return is the mantissa, its absolute value will be in the range [0.5 - 1).           
            /// </para>The exponent is binary, i.e. mantissa * 2^exp is the value of the source number.
            /// <para>If the source value is zero, both mantissa and exponent are returned as 0.
            /// </para></summary>
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
            /// When getting, returns this float.
            /// <para>When setting, sets the value of the float object to the value resulting from computing the supplied expression.
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
            /// </para>It would also not compile if the source were a "using" variable, as all method-local floats should be.
            /// </remarks>
            property MPEXPR_NAME^ Value
            {
                void set(MPEXPR_NAME^ expr) { expr->AssignTo(_value); }
                MPEXPR_NAME^ get() { return this; }
            }

            /// <summary>
            /// Sets the value of the float object.
            /// <para>Do not change the value of an object while it is contained in a hash table, because that changes its hash code.
            /// </para></summary>
            /// <param name="value">value for the new value for the object</param>
            void SetTo(mpir_ui value) { MP(set_ui)(_value, value); }

            /// <summary>
            /// Sets the value of the float object.
            /// <para>Do not change the value of an object while it is contained in a hash table, because that changes its hash code.
            /// </para></summary>
            /// <param name="value">value for the new value for the object</param>
            void SetTo(mpir_si value) { MP(set_si)(_value, value); }

            /// <summary>
            /// Sets the value of the float object.  This is an exact conversion, there is no rounting.
            /// <para>Do not change the value of an object while it is contained in a hash table, because that changes its hash code.
            /// </para></summary>
            /// <param name="value">new value for the object</param>
            void SetTo(double value) { MP(set_d)(_value, value); }

            /// <summary>
            /// Sets the value of the float object.
            /// <para>Do not change the value of an object while it is contained in a hash table, because that changes its hash code.
            /// </para></summary>
            /// <param name="value">new value for the object.
            /// <para>May be an float or a pair of floats separated by a slash.
            /// </para>The string's leading characters may indicate base:
            /// 0x and 0X for hexadecimal, 0b and 0B for binary, 0 for octal, or decimal otherwise</param>
            void SetTo(String^ value) { SetTo(value, 0); }

            /// <summary>
            /// Sets the value of the float object.
            /// <para>Do not change the value of an object while it is contained in a hash table, because that changes its hash code.
            /// </para></summary>
            /// <param name="value">new value for the object</param>
            /// <param name="base">base the mantissa in the <paramref name="value"/> string is in.  Exponent is always in decimcal.
            /// <para>The base may vary from 2 to 62.  No leading leading base characters are allowed.
            /// </para>For bases up to 36, case is ignored; upper-case and lower-case letters have the same value. 
            /// For bases 37 to 62, upper-case letter represent the usual 10..35 while lower-case letter represent 36..61.</param>
            void SetTo(String^ value, int base) { SetTo(value, base, true); }

            /// <summary>
            /// Sets the value of the float object.
            /// <para>Do not change the value of an object while it is contained in a hash table, because that changes its hash code.
            /// </para>If the fraction is not in canonical form, Canonicalize() must be called.
            /// </summary>
            /// <param name="value">new value for the object</param>
            /// <param name="base">base the <paramref name="value"/> string is in.
            /// <para>The base may vary from 2 to 62.  No leading leading base characters are allowed.
            /// </para>For bases up to 36, case is ignored; upper-case and lower-case letters have the same value. 
            /// For bases 37 to 62, upper-case letter represent the usual 10..35 while lower-case letter represent 36..61.</param>
            /// <param name="exponentInDecimal">If true, the exponent is in decimal; otherwise it is in the same base as the mantissa</param>
            void SetTo(String^ value, int base, bool exponentInDecimal);

            /// <summary>
            /// Sets the value of the float object.
            /// <para>Do not change the value of an object while it is contained in a hash table, because that changes its hash code.
            /// </para></summary>
            /// <param name="value">new value for the object</param>
            void SetTo(IntegerExpression^ value)
            {
                EvaluationContext context;
                value->AssignToInteger(context);
                MP(set_z)(_value, CTXTI(0));
            }

            /// <summary>
            /// Sets the value of the float object.
            /// <para>Do not change the value of an object while it is contained in a hash table, because that changes its hash code.
            /// </para></summary>
            /// <param name="value">new value for the object</param>
            void SetTo(RationalExpression^ value);

            /// <summary>
            /// Swaps the values of two floats.
            /// <para>This operation is a pointer swap and doesn't affect allocated memory.
            /// </para>Do not call this method while either object is contained in a hash table, because this would change their hash codes.
            /// </summary>
            /// <param name="a">Source number to swap this instance's value with</param>
            void Swap(MPTYPE^ a) 
            { 
                MP(swap)(_value, a->_value);

                mp_bitcnt_t prec = a->_allocatedPrecision;
                a->_allocatedPrecision = _allocatedPrecision;
                _allocatedPrecision = prec;
            }

            #pragma endregion

            #pragma region Size checks

            /// <summary>
            /// Gets or sets the effective precision of the number without changing the memory allocated.
            /// <para>The number of bits cannot exceed the precision with which the variable was initialized or last reallocated.
            /// </para>The value of the number is unchanged, and in particular if it previously had a higher precision it will retain that higher precision.
            /// <para>New values assigned to the Value property will use the new precision.
            /// </para>This is an efficient way to use a float variable at different precisions during a calculation,
            /// perhaps to gradually increase precision in an iteration, or just to use various different 
            /// precisions for different purposes during a calculation.
            /// <para>The number can be safely disposed after modifying its precision (this would not be the case in unmanaged MPIR).
            /// </para></summary>
            property mp_bitcnt_t Precision
            {
                mp_bitcnt_t get() { return MP(get_prec)(_value); }
                void set(mp_bitcnt_t value) 
                {
                    if(value > _allocatedPrecision)
                        throw gcnew ArgumentException("Cannot set precision higher than allocated");

                    MP(set_prec_raw)(_value, value);
                }
            }

            /// <summary>
            /// Gets the precision in bits that is currently allocated for internal storage of the mantissa.
            /// <para>The precision actually in effect, used in calculations, is initially the same but may be reduced by setting the Precision property.
            /// </para>However Precision cannot exceed AllocatedPrecision.
            /// <para>To change AllocatedPrecision, call Reallocate().
            /// </para>The value actually allocated may be slightly more than the number of bits requested by Allocate() or Reallocate().
            /// </summary>
            property mp_bitcnt_t AllocatedPrecision
            {
                mp_bitcnt_t get() { return _allocatedPrecision; }
            }

            /// <summary>
            /// Set the precision of this instance to be at least prec bits, reallocating its limbs data.
            /// <para>The value of this instance will be truncated to the new precision.
            /// </para>This function requires a call to realloc, and so should not be used in a tight loop.
            /// </summary>
            /// <param name="precision">Minimum number of bits the allocated memory should hold for the mantissa.</param>
            void Reallocate(mp_bitcnt_t precision)
            { 
                MP(set_prec_raw)(_value, _allocatedPrecision);
                MP(set_prec)(_value, precision); 
                _allocatedPrecision = precision; 
            }

#if BITS_PER_MP_LIMB == 64
            /// <summary>
            /// Returns true if the value of the source number, if truncated to an integer, is in the long range.
            /// <para>This method is supported only on 64-bit builds</para>
            /// </summary>
            /// <returns>true if the value will fit in a long</returns>
            bool FitsLong() { return MP(fits_si_p)(_value) != 0; }

            /// <summary>
            /// Returns true if the value of the source number, if truncated to an integer, is in the ulong range.
            /// <para>This method is supported only on 64-bit builds</para>
            /// </summary>
            /// <returns>true if the value will fit in a long</returns>
            bool FitsUlong() { return MP(fits_ui_p)(_value) != 0; }
#endif

            /// <summary>
            /// Returns true if the value of the source number, if truncated to an integer, is in the int range.
            /// </summary>
            /// <returns>true if the value will fit in an int</returns>
            bool FitsInt() { return MP(fits_sint_p)(_value) != 0; }

            /// <summary>
            /// Returns true if the value of the source number, if truncated to an integer, is in the uint range.
            /// </summary>
            /// <returns>true if the value will fit in an int</returns>
            bool FitsUint() { return MP(fits_uint_p)(_value) != 0; }

            /// <summary>
            /// Returns true if the value of the source number, if truncated to an integer, is in the short range.
            /// </summary>
            /// <returns>true if the value will fit in a short</returns>
            bool FitsShort() { return MP(fits_sshort_p)(_value) != 0; }

            /// <summary>
            /// Returns true if the value of the source number, if truncated to an integer, is in the ushort range.
            /// </summary>
            /// <returns>true if the value will fit in a short</returns>
            bool FitsUshort() { return MP(fits_ushort_p)(_value) != 0; }

            /// <summary>
            /// Returns true if the source number is a whole integer.
            /// </summary>
            /// <returns>true if the value is an integer</returns>
            bool IsInteger() { return MP(integer_p)(_value) != 0; }

            #pragma endregion

            #pragma region IO

            /// <summary>
            /// Outputs the float to the <paramref name="writer"/> as a string of digits in decimal.
            /// <para>The mantissa is prefixed with an '0.', and is followed by '@' and an integer exponent.
            /// </para>When writing multiple numbers that are to be read back with the Read(TextReader) method,
            /// it is useful to separate the numbers with a character that is not valid for this format.
            /// <para>This is because the Read method stops reading when it encounters an invalid character.
            /// </para></summary>
            /// <param name="writer">Text writer to output the number to</param>
            /// <returns>the number of characters written</returns>
            size_t Write(TextWriter^ writer) { return Write(writer, 10, 0, false, true); }

            /// <summary>
            /// Outputs the float to the <paramref name="writer"/> as a string of digits in base <paramref name="base"/>.
            /// <para>The mantissa is prefixed with an '0.', and is followed by '@' and an integer exponent in the same base.
            /// </para>When writing multiple numbers that are to be read back with the Read(TextReader) method,
            /// it is useful to separate the numbers with a character that is not valid for this format.
            /// <para>This is because the Read method stops reading when it encounters an invalid character.
            /// </para></summary>
            /// <param name="writer">Text writer to output the number to</param>
            /// <param name="base">The base to use for the output.
            /// <para>The base can be from 2 to 62; uppercase letters represent digits 10-35 while lowercase letters represent digits 36-61.</para></param>
            /// <returns>the number of characters written</returns>
            size_t Write(TextWriter^ writer, int base) { return Write(writer, base, 0, false, true); }

            /// <summary>
            /// Outputs the float to the <paramref name="writer"/> as a string of digits in base <paramref name="base"/>.
            /// <para>The mantissa is prefixed with an '0.', and is followed by '@' and an integer exponent.
            /// </para>When writing multiple numbers that are to be read back with the Read(TextReader) method,
            /// it is useful to separate the numbers with a character that is not valid for this format.
            /// <para>This is because the Read method stops reading when it encounters an invalid character.
            /// </para></summary>
            /// <param name="writer">Text writer to output the number to</param>
            /// <param name="lowercase">Indicates if lowercase or uppercase letters should be used for the output.
            /// <para>This argument is ignored for bases larger than 36, where both uppercase and lowercase letters are used.</para></param>
            /// <param name="base">The base to use for the output.
            /// <para>The base can be from 2 to 62; Bases up to 36 use uppercase or lowercase letters based on the <paramref name="lowercase"/> argument.
            /// </para>For bases larger than 36, the <paramref name="lowercase"/> argument is ignored and uppercase letters represent digits 10-35 while lowercase letters represent digits 36-61.</param>
            /// <returns>the number of characters written</returns>
            size_t Write(TextWriter^ writer, int base, bool lowercase) { return Write(writer, base, 0, lowercase, true); }

            /// <summary>
            /// Outputs the float to the <paramref name="writer"/> as a string of digits in base <paramref name="base"/>.
            /// <para>The mantissa is prefixed with an '0.', and is followed by '@' and an integer exponent.
            /// </para>When writing multiple numbers that are to be read back with the Read(TextReader) method,
            /// it is useful to separate the numbers with a character that is not valid for this format.
            /// <para>This is because the Read method stops reading when it encounters an invalid character.
            /// </para></summary>
            /// <param name="writer">Text writer to output the number to</param>
            /// <param name="base">The base to use for the output.
            /// <para>The base can be from 2 to 62; Bases up to 36 use uppercase or lowercase letters based on the <paramref name="lowercase"/> argument.
            /// </para>For bases larger than 36, the <paramref name="lowercase"/> argument is ignored and uppercase letters represent digits 10-35 while lowercase letters represent digits 36-61.</param>
            /// <param name="lowercase">Indicates if lowercase or uppercase letters should be used for the output.
            /// <para>This argument is ignored for bases larger than 36, where both uppercase and lowercase letters are used.</para></param>
            /// <param name="exponentInDecimal">Whether the exponent should always be written in decimal, or use the same base as the mantissa.</param>
            /// <param name="maxDigits">Maximum number of digits for the mantissa.
            /// <para>No more digits than are accurately representable by the number's precision will be printed.
            /// </para>Use zero to select that accurate maximum.</param>
            /// <returns>the number of characters written</returns>
            size_t Write(TextWriter^ writer, int base, int maxDigits, bool lowercase, bool exponentInDecimal);

            /// <summary>
            /// Inputs the number as a possibly white-space preceeded string.
            /// <para>The string is in the form 'Mantissa@Exponent' or, if the base is 10 or less, alternatively 'MeN'.
            /// </para>The mantissa and exponent are always in decimal, no leading base characters are allowed.
            /// <para>Reading terminates at end-of-stream, or up to but not including a character that is not valid for this format.
            /// </para>This method reads the output of a Write(TextWriter) when decimal base is used.
            /// </summary>
            /// <param name="reader">Text reader to input the number from</param>
            /// <returns>the number of characters read</returns>
            size_t Read(TextReader^ reader) { return Read(reader, 0, true); }

            /// <summary>
            /// Inputs the number as a possibly white-space preceeded string.
            /// <para>The string is in the form 'Mantissa@Exponent' or, if the base is 10 or less, alternatively 'MeN'.
            /// </para>The mantissa is always in the specified base, the exponent is always in decimal.  No leading base characters are allowed.
            /// <para>Reading terminates at end-of-stream, or up to but not including a character that is not valid for this format.
            /// </para>This method reads the output of a Write(TextWriter) when decimal base is used.
            /// </summary>
            /// <param name="reader">Text reader to input the number from</param>
            /// <param name="base">The base to use for the mantissa.
            /// <para>The base can be from 2 to 62; uppercase letters represent digits 10-35 while lowercase letters represent digits 36-61.
            /// </para>For bases 36 and less, uppercase and lowercase letters are equivalent.</param>
            /// <returns>the number of characters read</returns>
            size_t Read(TextReader^ reader, int base) { return Read(reader, base, true); }

            /// <summary>
            /// <para>The string is in the form 'Mantissa@Exponent' or, if the base is 10 or less, alternatively 'MeN'.
            /// </para>The mantissa is always in the specified base, the exponent is either in the same base or in decimal.  No leading base characters are allowed.
            /// <para>Reading terminates at end-of-stream, or up to but not including a character that is not valid for this format.
            /// </para>This method reads the output of a Write(TextWriter, ...) method with the same base.
            /// </summary>
            /// <param name="reader">Text reader to input the number from</param>
            /// <param name="base">The base to use for the input.
            /// <para>The base can be from 2 to 62; For bases up to 36 case is ignored.
            /// </para>For bases larger than 36, uppercase letters represent digits 10-35 while lowercase letters represent digits 36-61.</param>
            /// <param name="exponentInDecimal">Whether the exponent should always be written in decimal, or use the same base as the mantissa.</param>
            /// <returns>the number of characters read</returns>
            size_t Read(TextReader^ reader, int base, bool exponentInDecimal);

            #pragma endregion

            #pragma region Arithmetic

            /// <summary>Computes the square root of the source number.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">source number to take the square root of</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MPEXPR_NAME^ SquareRoot(mpir_ui a) { return gcnew MPEXPR(SquareRootUi)(a); }

            #pragma endregion
    };

    #pragma endregion
};
