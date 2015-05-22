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
#include "ExpressionMacros.h"

namespace MPIR
{
    ref class MpirRandom;
    ref class MPTYPE;
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
                MP(init)(ptr);
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

            /// <summary>Computes the absolute value of the source number.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MPEXPR_NAME^ Abs();

            #pragma endregion

            #pragma region Comparisons

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Value to compare the source with</param>
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
            /// </para></summary>
            /// <param name="a">Value to compare the source with.  This can be a multi-precision number, an expression, or a supported primitive type (long, ulong, or double).</param>
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

            /// <summary>Calculates the sign (+1, 0, or -1) of the source value.
            /// <para>If the source is an expression, it is evaluated into a temporary variable before the sign is computed.
            /// </para></summary>
            /// <returns>+1 if the source is positive, -1 if negative, and 0 if zero.</returns>
            int Sign() { IN_CONTEXT(this); return MP(sgn)(CTXT(0)); }

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

        private:
            //construction
            void AllocateStruct()
            {
                _value = (MP(ptr))((*__gmp_allocate_func)(sizeof(MPSTRUCT)));
            }
            void FromString(String^ value, int base);
            MPTYPE(bool initialize);
            String^ ToString(int base, bool lowercase, int maxDigits);

        internal:
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
            /// Gets or sets the default precision (in bits) assigned to new instances created with the default constructor
            /// </summary>
            static property mp_bitcnt_t DefaultPrecision
            {
                mp_bitcnt_t get()
                {
                    return MP(get_default_prec)();
                }
                void set(mp_bitcnt_t value)
                {
                    MP(set_default_prec)(value);
                }
            }

            /// <summary>
            /// Initializes a new float instance and sets its value to 0/1
            /// </summary>
            MPTYPE();

            /// <summary>
            /// Initializes a new float instance, allocating enough memory to hold at least <paramref name="precision"/> bits, and sets its value to 0.
            /// <para>All float operations are performed to the precision of the destination.
            /// </para></summary>
            /// <param name="precision">Minimum number of bits the initially allocated memory should hold for the mantissa</param>
            /// <returns>the newly constructed instance</returns>
            static MPTYPE^ Allocate(mp_bitcnt_t precision);

            /// <summary>
            /// Initializes a new float instance and sets its value from the specified string, using leading characters to recognize the base:
            /// 0x and 0X for hexadecimal, 0b and 0B for binary, 0 for octal, or decimal otherwise.
            /// </summary>
            /// <param name="value">string representing the initial value for the new instance.  Whitespace in the string is ignored.</param>
            MPTYPE(String^ value) { FromString(value, 0); }

            /// <summary>
            /// Initializes a new float instance and sets its value from the specified string
            /// </summary>
            /// <param name="value">string representing the initial value for the new instance.  Whitespace in the string is ignored.</param>
            /// <param name="base">base the <paramref name="value"/> string is in.
            /// <para>The base may vary from 2 to 62, or if base is 0, then the leading characters are used: 0x and 0X for hexadecimal, 0b and 0B for binary, 0 for octal, or decimal otherwise.
            /// </para>For bases up to 36, case is ignored; upper-case and lower-case letters have the same value. 
            /// For bases 37 to 62, upper-case letter represent the usual 10..35 while lower-case letter represent 36..61.</param>
            MPTYPE(String^ value, int base) { FromString(value, base); }

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

            /// <summary>
            /// Returns the absolute value of the number as a ulong, truncating any fractional part.
            /// <para>If the number is too big, the result is undefined.  Call FitsUlong() to check if the number will fit.
            /// </para>The sign of the number is ignored, only the absolute value is used.
            /// </summary>
            /// <returns>The absolute value as a ulong, with any fractional part truncated.</returns>
            mpir_ui ToUlong() { return MP(get_ui)(_value); }

            /// <summary>
            /// Returns the value of the number as a long.
            /// <para>If the number is too big, the result is undefined.  Call FitsLong() to check if the number will fit.
            /// </para>
            /// </summary>
            /// <returns>The value as a long, possibly truncated to the least significant bits only.</returns>
            mpir_si ToLong() { return MP(get_si)(_value); }

            /// <summary>
            /// Returns the value of the number as a double, truncating if necessary (rounding towards zero).
            /// <para>If the exponent from the conversion is too big, the result is system dependent. An infinity is returned where available.             /// A hardware overflow trap may or may not occur.
            /// </para></summary>
            /// <returns>The value as a double, possibly truncated.</returns>
            double ToDouble() { return MP(get_d)(_value); }

            /// <summary>
            /// Returns the value of the number as a double, truncating if necessary (rounding towards zero), and returning the exponent separately.
            /// <para>The return is the mantissa, its absolute value will be in the range [0.5 - 1).            /// </para>The exponent is binary, i.e. mantissa * 2^exp is the value of the source number.
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
            /// <param name="base">base the <paramref name="value"/> string is in.
            /// <para>The base may vary from 2 to 62, or if base is 0, then the leading characters are used: 0x and 0X for hexadecimal, 0b and 0B for binary, 0 for octal, or decimal otherwise.
            /// </para>For bases up to 36, case is ignored; upper-case and lower-case letters have the same value. 
            /// For bases 37 to 62, upper-case letter represent the usual 10..35 while lower-case letter represent 36..61.</param>
            void SetTo(String^ value, int base);

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
                MP(ptr) temp = a->_value;
                a->_value = _value;
                _value = temp; 
            }

            #pragma endregion

            #pragma region Size checks

            /// <summary>
            /// Returns true if the value of the float, when truncated to an integer, is in the ulong range.
            /// </summary>
            /// <returns>true if the value will fit in a ulong</returns>
            bool FitsUlong() { return MP(fits_ui_p)(_value) != 0; }

            /// <summary>
            /// Returns true if the value of the float, when truncated to an integer, is in the long range.
            /// </summary>
            /// <returns>true if the value will fit in a long</returns>
            bool FitsLong() { return MP(fits_si_p)(_value) != 0; }

            /// <summary>
            /// Returns true if the value of the float, when truncated to an integer, is in the uint range.
            /// </summary>
            /// <returns>true if the value will fit in a uint</returns>
            bool FitsUint() { return MP(fits_uint_p)(_value) != 0; }

            /// <summary>
            /// Returns true if the value of the float, when truncated to an integer, is in the int range.
            /// </summary>
            /// <returns>true if the value will fit in a int</returns>
            bool FitsInt() { return MP(fits_sint_p)(_value) != 0; }

            /// <summary>
            /// Returns true if the value of the float, when truncated to an integer, is in the ushort range.
            /// </summary>
            /// <returns>true if the value will fit in a ushort</returns>
            bool FitsUshort() { return MP(fits_ushort_p)(_value) != 0; }

            /// <summary>
            /// Returns true if the value of the float, when truncated to an integer, is in the short range.
            /// </summary>
            /// <returns>true if the value will fit in a short</returns>
            bool FitsShort() { return MP(fits_sshort_p)(_value) != 0; }

            #pragma endregion

            #pragma region IO

            /// <summary>
            /// Outputs the float to the <paramref name="stream"/> in raw binary format.
            /// <para>The number's numerator and denominator are written in sequence, each in a portable format,
            /// with 4 bytes of size information, and that many bytes of limbs.
            /// </para>Both the size and the limbs are written in decreasing significance order (i.e., in big-endian).
            /// <para>The output can be read with Read(Stream).
            /// </para>The output cannot be read by MP(inp_raw) from GMP 1, because of changes necessary
            /// for compatibility between 32-bit and 64-bit machines.
            /// </summary>
            /// <param name="stream">Stream to output the number to</param>
            /// <returns>the number of bytes written, or 0 if an error occurs.</returns>
            size_t Write(Stream^ stream);

            /// <summary>
            /// Reads the float value from the <paramref name="stream"/> in raw binary format, as it would have been written by Write(Stream).
            /// <para>The number's numerator and denominator are read in sequence, each in a portable format,
            /// with 4 bytes of size information, and that many bytes of limbs.
            /// </para>Both the size and the limbs are written in decreasing significance order (i.e., in big-endian).
            /// <para>This routine can read the output from MP(out_raw) also from GMP 1, in spite of changes
            /// necessary for compatibility between 32-bit and 64-bit machines.
            /// </para></summary>
            /// <param name="stream">Stream to input the number from</param>
            /// <returns>the number of bytes read, or 0 if an error occurs.</returns>
            size_t Read(Stream^ stream);

            /// <summary>
            /// Outputs the float to the <paramref name="writer"/> as a string of digits in decimal.
            /// <para>When writing multiple numbers that are to be read back with the Read(TextReader) method,
            /// it is useful to separate the numbers with a character that is not a valid decimal digit.
            /// </para>This is because the Read method stops reading when it encounters a character that cannot represent a digit.
            /// </summary>
            /// <param name="writer">Text writer to output the number to</param>
            /// <returns>the number of characters written</returns>
            size_t Write(TextWriter^ writer) { return Write(writer, 0, false); }

            /// <summary>
            /// Outputs the float to the <paramref name="writer"/> as a string of digits in base <paramref name="base"/>.
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
            /// Outputs the float to the <paramref name="writer"/> as a string of digits in base <paramref name="base"/>.
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

///// <summary>
///// Imports the number from arbitrary words of binary data.
///// <para>No sign information is taken from the data, the imported number will be positive or zero.</para>
///// </summary>
///// <typeparam name="T">Type of element in the data array.  This must be a value type, but does not need to represent a single limb.  Data is interpreted as a flat byte array.</typeparam>
///// <param name="data">Array of binary "limbs" to import from.
///// <para>Elements don't necessarily need to be of the <paramref name="bytesPerLimb"/> size; the data is interpreted as a flat byte array.</para></param>
///// <param name="limbCount">Number of "limbs" to import</param>
///// <param name="bytesPerLimb">Number of bytes per "limb."</param>
///// <param name="limbOrder">Specifies the order of the "limbs."</param>
///// <param name="endianness">Specifies the byte order within each "limb."</param>
///// <param name="nails">The number of most-significant bits to ignore in each "limb."</param>
//generic<typename T> where T : value class void Import(array<T>^ data, size_t limbCount, int bytesPerLimb, LimbOrder limbOrder, Endianness endianness, int nails)
//{
//    if(limbCount == 0)
//    {
//        MP(set_ui)(_value, 0);
//        return;
//    }

//    PIN(data);
//    MP(import)(_value, limbCount, (int)limbOrder, bytesPerLimb, (int)endianness, nails, pinned_data);
//}

///// <summary>
///// Exports the absolute value of the number to arbitrary words of binary data.
///// <para>The sign of op is ignored.
///// </para></summary>
///// <typeparam name="T">Type of element in the data array.  This must be a value type, but does not need to represent a single limb.  Data is interpreted as a flat byte array.</typeparam>
///// <param name="data">Array of binary "limbs" to export to.
///// <para>Elements don't necessarily need to be of the <paramref name="bytesPerLimb"/> size; the data is interpreted as a flat byte array.
///// </para>The total size of the array in bytes must be sufficient for the export.</param>
///// <param name="bytesPerLimb">Number of bytes per "limb."</param>
///// <param name="limbOrder">Specifies the order of the "limbs."</param>
///// <param name="endianness">Specifies the byte order within each "limb."</param>
///// <param name="nails">The number of most-significant bits to reserve, and set to zero, in each "limb."</param>
///// <returns>The number of limbs exported.
///// <para>If the number is non-zero, then the most significant word produced will be non-zero.
///// </para>If the number is zero, then the count returned will be zero and nothing written to the data.</returns>
//generic<typename T> where T : value class size_t Export(array<T>^ data, int bytesPerLimb, LimbOrder limbOrder, Endianness endianness, int nails)
//{
//    PIN(data);
//    size_t limbCount;
//    MP(export)(pinned_data, &limbCount, (int)limbOrder, bytesPerLimb, (int)endianness, nails, _value);
//    return limbCount;
//}

//    /// <summary>
//    /// Exports the absolute value of the number to arbitrary words of binary data.  An array of type T is allocated for the export.
//    /// <para>The sign of op is ignored.
//    /// </para></summary>
//    /// <typeparam name="T">Type of element in the data array.  This must be a value type, but does not need to represent a single limb.  Data is interpreted as a flat byte array.</typeparam>
//    /// <param name="bytesPerLimb">Number of bytes per "limb."</param>
//    /// <param name="limbOrder">Specifies the order of the "limbs."</param>
//    /// <param name="endianness">Specifies the byte order within each "limb."</param>
//    /// <param name="nails">The number of most-significant bits to reserve, and set to zero, in each "limb."</param>
//    /// <returns>An array of type T containing the exported limb data.
//    /// <para>If the number is non-zero, then the most significant word produced will be non-zero.
//    /// </para>If the number is zero, then a zero-length array is returned.</returns>
//    generic<typename T> where T : value class array<T>^ Export(int bytesPerLimb, LimbOrder limbOrder, Endianness endianness, int nails)
//    {
//        if(this->Sign() == 0)
//            return gcnew array<T>(0);

//        auto bitsPerLimb = 8 * bytesPerLimb - nails;
//        auto limbCount = (MP(sizeinbase)(_value, 2) - 1) / bitsPerLimb + 1;
//        auto arrayCount = (limbCount * bytesPerLimb - 1) / sizeof(T) + 1;
//        auto data = gcnew array<T>(arrayCount);

//        PIN(data);
//        MP(export)(pinned_data, &limbCount, (int)limbOrder, bytesPerLimb, (int)endianness, nails, _value);
//        return data;
//    }

//internal:
//    size_t ReadNoWhite(TextReader^ reader, int base, size_t nread);

//public:

//    /// <summary>
//    /// Returns the specified limb of the number.
//    /// <para>The least significant limb is zero.
//    /// </para>The sign of the number is ignored.
//    /// </summary>
//    /// <param name="index">The index of the limb to return.
//    /// <para>The least significant limb is zero.
//    /// </para>If the index is outside the range 0 to Size()-1, zero is returned.</param>
//    /// <returns>The specified limb, or zero if <paramref name="index"/> is outside of the valid range.</returns>
//    size_t GetLimb(mp_size_t index) { return MP(getlimbn)(_value, index); }

            #pragma endregion
    };

    #pragma endregion
};
