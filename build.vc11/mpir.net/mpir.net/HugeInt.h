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
using namespace System::Runtime::InteropServices;

#define IS_NULL(a) (Object::ReferenceEquals(a, nullptr))

//defines a unary expression class
#define DEFINE_UNARY_EXPRESSION(base, name, type)                \
private ref class Mpir##name##Expression : base                  \
{                                                                \
    internal:                                                    \
        type Operand;                                            \
        virtual void AssignTo(mpz_ptr destination) override;     \
        Mpir##name##Expression(type operand)                     \
        {                                                        \
            Operand = operand;                                   \
        }                                                        \
};

//defines a binary expression class
#define DEFINE_BINARY_EXPRESSION(base, name, leftType, rightType) \
private ref class Mpir##name##Expression : base                   \
{                                                                 \
    internal:                                                     \
        leftType Left;                                            \
        rightType Right;                                          \
        virtual void AssignTo(mpz_ptr destination) override;      \
        Mpir##name##Expression(leftType left, rightType right)    \
        {                                                         \
            Left = left;                                          \
            Right = right;                                        \
        }                                                         \
};

//defines a ternary expression class
#define DEFINE_TERNARY_EXPRESSION(base, name, leftType, middleType, rightType)     \
private ref class Mpir##name##Expression : base                                    \
{                                                                                  \
    internal:                                                                      \
        leftType Left;                                                             \
        middleType Middle;                                                         \
        rightType Right;                                                           \
        virtual void AssignTo(mpz_ptr destination) override;                       \
        Mpir##name##Expression(leftType left, middleType middle, rightType right)  \
        {                                                                          \
            Left = left;                                                           \
            Middle = middle;                                                       \
            Right = right;                                                         \
        }                                                                          \
};

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

#define TYPE_FOR_ABBR_Int HugeInt^
#define TYPE_FOR_ABBR_Expr MpirExpression^
#define TYPE_FOR_ABBR_Si mpir_si
#define TYPE_FOR_ABBR_Ui mpir_ui
#define TYPE_FOR_ABBR_Bits mp_bitcnt_t

//unary expressions
#define DEFINE_UNARY_EXPRESSION_WITH_ONE(base, name, typeAbbr) \
    DEFINE_UNARY_EXPRESSION(base, name##typeAbbr, MpirExpression^)           

//binary expressions
#define DEFINE_BINARY_EXPRESSION_WITH_TWO(base, name, typeAbbr) \
    DEFINE_BINARY_EXPRESSION(base, name##typeAbbr##typeAbbr, MpirExpression^, MpirExpression^)

#define DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(base, name, leftTypeAbbr, rightTypeAbbr)    \
    DEFINE_BINARY_EXPRESSION(base, name##leftTypeAbbr##rightTypeAbbr, MpirExpression^, TYPE_FOR_ABBR_##rightTypeAbbr) 

#define DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT(base, name, leftTypeAbbr, rightTypeAbbr)     \
    DEFINE_BINARY_EXPRESSION(base, name##leftTypeAbbr##rightTypeAbbr, TYPE_FOR_ABBR_##leftTypeAbbr, MpirExpression^)

//ternary expressions
#define DEFINE_TERNARY_EXPRESSION_WITH_THREE(base, name, typeAbbr) \
    DEFINE_TERNARY_EXPRESSION(base, name##typeAbbr##typeAbbr##typeAbbr, MpirExpression^, MpirExpression^, MpirExpression^)

#define DEFINE_TERNARY_EXPRESSION_WITH_BUILT_IN_MIDDLE(base, name, leftTypeAbbr, middleTypeAbbr, rightTypeAbbr)    \
    DEFINE_TERNARY_EXPRESSION(base, name##leftTypeAbbr##middleTypeAbbr##rightTypeAbbr, MpirExpression^, TYPE_FOR_ABBR_##middleTypeAbbr, MpirExpression^)

//void functions
#define MAKE_VOID_FUNCTION(base, action, op, type)  \
    MAKE_VOID_FUNCTION_##action(base, op, op##type)

#define MAKE_VOID_FUNCTION_DECLARE(base, op, result)     \
    base^ op();

#define MAKE_VOID_FUNCTION_DEFINE(base, op, result)      \
    base^ MpirExpression::op() { return gcnew Mpir##result##Expression(this); }

//one-arg functions
#define MAKE_FUNCTION_WITH_ONE(base, action, op, argTypeAbbr)  \
    MAKE_FUNCTION_WITH_ONE_##action(base, op, Expr, op##Int##argTypeAbbr)

#define MAKE_FUNCTION_WITH_LIMB(base, action, op, argTypeAbbr)  \
    MAKE_FUNCTION_WITH_ONE_##action(base, op, argTypeAbbr, op##Int##argTypeAbbr)

#define MAKE_FUNCTION_WITH_ONE_DECLARE(base, op, argTypeAbbr, result)     \
    base^ op(TYPE_FOR_ABBR_##argTypeAbbr a);

#define MAKE_FUNCTION_WITH_ONE_DEFINE(base, op, argTypeAbbr, result)      \
    base^ MpirExpression::op(TYPE_FOR_ABBR_##argTypeAbbr a) { return gcnew Mpir##result##Expression(this, a); }

//two-arg functions
#define MAKE_FUNCTION_WITH_TWO(base, action, op, leftTypeAbbr, rightTypeAbbr)  \
    MAKE_FUNCTION_WITH_TWO_##action(base, op, Expr, Expr, op##Int##leftTypeAbbr##rightTypeAbbr)

#define MAKE_FUNCTION_WITH_TWO_LLIMB(base, action, op, leftTypeAbbr, rightTypeAbbr)  \
    MAKE_FUNCTION_WITH_TWO_##action(base, op, leftTypeAbbr, Expr, op##Int##leftTypeAbbr##rightTypeAbbr)

#define MAKE_FUNCTION_WITH_TWO_DECLARE(base, op, leftTypeAbbr, rightTypeAbbr, result)     \
    base^ op(TYPE_FOR_ABBR_##leftTypeAbbr a, TYPE_FOR_ABBR_##rightTypeAbbr b);

#define MAKE_FUNCTION_WITH_TWO_DEFINE(base, op, leftTypeAbbr, rightTypeAbbr, result)      \
    base^ MpirExpression::op(TYPE_FOR_ABBR_##leftTypeAbbr a, TYPE_FOR_ABBR_##rightTypeAbbr b) { return gcnew Mpir##result##Expression(this, a, b); }

//functions with one argument and simple result
//#define MAKE_SIMPLE_FUNCTION_WITH_ONE(base, action, op, resultType, argType) \
//    MAKE_SIMPLE_FUNCTION_WITH_ONE_##action(base, op, resultType, Expr)
//
//#define MAKE_SIMPLE_FUNCTION_WITH_LIMB(base, action, op, resultType, argType) \
//    MAKE_SIMPLE_FUNCTION_WITH_ONE_##action(base, op, resultType, argType)
//
//#define MAKE_SIMPLE_FUNCTION_WITH_ONE_DECLARE(base, op, resultTypeAbbr, argTypeAbbr)     \
//    TYPE_FOR_ABBR_##resultTypeAbbr op(TYPE_FOR_ABBR_##argTypeAbbr a);
//
//#define MAKE_SIMPLE_FUNCTION_WITH_ONE_DEFINE(base, op, resultTypeAbbr, argTypeAbbr)      \
//    TYPE_FOR_ABBR_##resultTypeAbbr HugeInt::op(TYPE_FOR_ABBR_##argTypeAbbr a) { return gcnew Mpir##result##Expression(this, a); }

//unary operators
#define MAKE_UNARY_OPERATOR(base, action, op, result, mpType) \
    MAKE_UNARY_OPERATOR_##action(base, op, result##mpType, Expr)

#define MAKE_UNARY_OPERATOR_DECLARE(base, op, result, type)     \
    static base^ operator op(TYPE_FOR_ABBR_##type a);

#define MAKE_UNARY_OPERATOR_DEFINE(base, op, result, type)      \
    base^ MpirExpression::operator op(TYPE_FOR_ABBR_##type a) { return gcnew Mpir##result##Expression(a); }

//binary operators
#define MAKE_BINARY_OPERATOR_DECLARE(base, op, result, leftType, rightType, left, right)     \
    static base^ operator op(TYPE_FOR_ABBR_##leftType a, TYPE_FOR_ABBR_##rightType b);

#define MAKE_BINARY_OPERATOR_DEFINE(base, op, result, leftType, rightType, left, right)      \
    base^ MpirExpression::operator op(TYPE_FOR_ABBR_##leftType a, TYPE_FOR_ABBR_##rightType b) { return gcnew Mpir##result##Expression(left, right); }

#define MAKE_BINARY_OPERATOR_STANDARD(base, action, op, result, leftType, rightType)        \
    MAKE_BINARY_OPERATOR_##action(base, op, result##leftType##rightType, Expr, Expr, a, b)      

#define MAKE_BINARY_OPERATOR_RLIMB(base, action, op, result, mpType, limbType)  \
    MAKE_BINARY_OPERATOR_##action(base, op, result##mpType##limbType, Expr, limbType, a, b)          

#define MAKE_BINARY_OPERATOR_LLIMB(base, action, op, result, mpType, limbType)  \
    MAKE_BINARY_OPERATOR_##action(base, op, result##limbType##mpType, limbType, Expr, a, b)           

#define MAKE_BINARY_OPERATOR_LLIMB_R(base, action, op, result, mpType, limbType) \
    MAKE_BINARY_OPERATOR_##action(base, op, result##mpType##limbType, limbType, Expr, b, a)

//master operators/functions definition
#define DEFINE_OPERATIONS(action)                                                                 \
    MAKE_BINARY_OPERATOR_STANDARD  (MpirExpression,           action, +, Add, Int, Int)           \
    MAKE_BINARY_OPERATOR_RLIMB     (MpirExpression,           action, +, Add, Int, Ui)            \
    MAKE_BINARY_OPERATOR_LLIMB_R   (MpirExpression,           action, +, Add, Int, Ui)            \
    MAKE_BINARY_OPERATOR_RLIMB     (MpirExpression,           action, +, Add, Int, Si)            \
    MAKE_BINARY_OPERATOR_LLIMB_R   (MpirExpression,           action, +, Add, Int, Si)            \
                                                                                                  \
    MAKE_BINARY_OPERATOR_STANDARD  (MpirExpression,           action, -, Subtract, Int, Int)      \
    MAKE_BINARY_OPERATOR_RLIMB     (MpirExpression,           action, -, Subtract, Int, Ui)       \
    MAKE_BINARY_OPERATOR_LLIMB     (MpirExpression,           action, -, Subtract, Int, Ui)       \
    MAKE_BINARY_OPERATOR_RLIMB     (MpirExpression,           action, -, Subtract, Int, Si)       \
    MAKE_BINARY_OPERATOR_LLIMB     (MpirExpression,           action, -, Subtract, Int, Si)       \
                                                                                                  \
    MAKE_BINARY_OPERATOR_STANDARD  (MpirExpression,           action, *, Multiply, Int, Int)      \
    MAKE_BINARY_OPERATOR_RLIMB     (MpirExpression,           action, *, Multiply, Int, Ui)       \
    MAKE_BINARY_OPERATOR_LLIMB_R   (MpirExpression,           action, *, Multiply, Int, Ui)       \
    MAKE_BINARY_OPERATOR_RLIMB     (MpirExpression,           action, *, Multiply, Int, Si)       \
    MAKE_BINARY_OPERATOR_LLIMB_R   (MpirExpression,           action, *, Multiply, Int, Si)       \
                                                                                                  \
    MAKE_BINARY_OPERATOR_RLIMB     (MpirExpression,           action, <<, ShiftLeft, Int, Bits)   \
    MAKE_BINARY_OPERATOR_RLIMB     (MpirShiftRightExpression, action, >>, ShiftRight, Int, Bits)  \
                                                                                                  \
    MAKE_UNARY_OPERATOR            (MpirExpression,           action, -, Negate, Int)             \
                                                                                                  \
    MAKE_VOID_FUNCTION             (MpirExpression,           action, Abs, Int)                   \
                                                                                                  \
    MAKE_BINARY_OPERATOR_STANDARD  (MpirDivideExpression,     action, /, Divide, Int, Int)        \
    MAKE_BINARY_OPERATOR_RLIMB     (MpirDivideUiExpression,   action, /, Divide, Int, Ui)         \
                                                                                                  \
    MAKE_BINARY_OPERATOR_STANDARD  (MpirModExpression,        action, %, Mod, Int, Int)           \
    MAKE_BINARY_OPERATOR_RLIMB     (MpirModUiExpression,      action, %, Mod, Int, Ui)            \
                                                                                                  \
    MAKE_BINARY_OPERATOR_RLIMB     (MpirExpression,           action, ^, Power, Int, Ui)          \
                                                                                                  \
    MAKE_FUNCTION_WITH_ONE         (MpirExpression,           action, DivideExactly, Int)         \
    MAKE_FUNCTION_WITH_LIMB        (MpirExpression,           action, DivideExactly, Ui)          \
                                                                                                  \
    MAKE_FUNCTION_WITH_TWO         (MpirExpression,           action, PowerMod, Int, Int)         \
    MAKE_FUNCTION_WITH_TWO_LLIMB   (MpirExpression,           action, PowerMod, Ui, Int)          \
                                                                                                  \
    MAKE_FUNCTION_WITH_LIMB        (MpirRootExpression,       action, Root, Ui)                   \
    MAKE_VOID_FUNCTION             (MpirSquareRootExpression, action, SquareRoot, Int)            \
                                                                                                  \
    MAKE_BINARY_OPERATOR_STANDARD  (MpirExpression,           action, &, And, Int, Int)           \
    MAKE_BINARY_OPERATOR_STANDARD  (MpirExpression,           action, |, Or, Int, Int)            \
    MAKE_BINARY_OPERATOR_STANDARD  (MpirExpression,           action, ^, Xor, Int, Int)           \
    MAKE_UNARY_OPERATOR            (MpirExpression,           action, ~, Complement, Int)         \

namespace MPIR
{
    ref class HugeInt;
    ref class MpirDivideExpression;
    ref class MpirDivideUiExpression;
    ref class MpirModExpression;
    ref class MpirDivModExpression;
    ref class MpirModUiExpression;
    ref class MpirShiftRightExpression;
    ref class MpirRootExpression;
    ref class MpirSquareRootExpression;

    /// <summary>
    /// This enum defines the rounding modes MPIR supports.  Division and modulo operations take an optional rounding mode parameter, or use the default, which is set in the static MpirSettings class.
    /// </summary>
    public enum class RoundingModes
    {
        /// <summary>Rounding mode is unspecified.  Use a higher level default if available, fall back to Truncate.</summary>
        Default,
        /// <summary>Truncate to zero.</summary>
        Truncate,
        /// <summary>Round up to +infinity.</summary>
        Ceiling,
        /// <summary>Round down to -infinity.</summary>
        Floor,
    };

    /// <summary>
    /// Base class for all expressions resulting from many operations on MPIR types. Expressions can be arbitrarily nested, and are lazily evaluated 
    /// when they are either assigned to the Value property of an MPIR object, or are consumed by a function or operator that returns a primitive type.
    /// Assignment to the Value property is necessary because .Net does not support overloading the assignment operator.
    /// </summary>
    public ref class MpirExpression abstract : public IComparable, IComparable<MpirExpression^>, IEquatable<MpirExpression^>
    {
        internal:
            MpirExpression() { }
            virtual void AssignTo(mpz_ptr destination) abstract;
            virtual void AssignTo(EvaluationContext& destination)
            {
                destination.Options = (EvaluationOptions) (destination.Options | (1 << destination.Index));
                auto ptr = &destination.Temp[destination.Index];
                destination.Args[destination.Index++] = ptr;
                mpz_init(ptr);
                AssignTo(ptr); 
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
            static MpirExpression^ operator + (MpirExpression^ a, MpirExpression^ b);

            /// <summary>Adds two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to add to</param>
            /// <param name="b">Source value to add</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ operator + (MpirExpression^ a, mpir_ui b);

            /// <summary>Adds two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to add to</param>
            /// <param name="b">Source value to add</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ operator + (mpir_ui a, MpirExpression^ b);

            /// <summary>Adds two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to add to</param>
            /// <param name="b">Source value to add</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ operator + (MpirExpression^ a, mpir_si b);

            /// <summary>Adds two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to add to</param>
            /// <param name="b">Source value to add</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ operator + (mpir_si a, MpirExpression^ b);
                                                                                                          
            /// <summary>Subtracts two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to subtract from</param>
            /// <param name="b">Source value to subtract</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ operator - (MpirExpression^ a, MpirExpression^ b);
                                                                                                          
            /// <summary>Subtracts two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to subtract from</param>
            /// <param name="b">Source value to subtract</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ operator - (MpirExpression^ a, mpir_ui b);
                                                                                                          
            /// <summary>Subtracts two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to subtract from</param>
            /// <param name="b">Source value to subtract</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ operator - (mpir_ui a, MpirExpression^ b);
                                                                                                          
            /// <summary>Subtracts two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to subtract from</param>
            /// <param name="b">Source value to subtract</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ operator - (MpirExpression^ a, mpir_si b);
                                                                                                          
            /// <summary>Subtracts two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to subtract from</param>
            /// <param name="b">Source value to subtract</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ operator - (mpir_si a, MpirExpression^ b);
                                                                                                          
            /// <summary>Multiplies two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to multiply</param>
            /// <param name="b">Source value to multiply by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ operator * (MpirExpression^ a, MpirExpression^ b);
                                                                                                          
            /// <summary>Multiplies two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to multiply</param>
            /// <param name="b">Source value to multiply by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ operator * (MpirExpression^ a, mpir_ui b);
                                                                                                          
            /// <summary>Multiplies two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to multiply</param>
            /// <param name="b">Source value to multiply by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ operator * (mpir_ui a, MpirExpression^ b);
                                                                                                          
            /// <summary>Multiplies two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to multiply</param>
            /// <param name="b">Source value to multiply by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ operator * (MpirExpression^ a, mpir_si b);
                                                                                                          
            /// <summary>Multiplies two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to multiply</param>
            /// <param name="b">Source value to multiply by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ operator * (mpir_si a, MpirExpression^ b);
                                                                                                          
            /// <summary>Shifts the <paramref name="a"/> source operand to the left by <paramref name="bits"/>, i.e. multiplies <paramref name="a"/> by 2^<paramref name="bits"/>.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to multiply</param>
            /// <param name="bits">Number of bits to shift <paramref name="a"/> by, i.e. power of 2 to multiply <paramref name="a"/> by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ operator << (MpirExpression^ a, mp_bitcnt_t bits);
                                                                                                          
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
            static MpirShiftRightExpression^ operator >> (MpirExpression^ a, mp_bitcnt_t bits);

            /// <summary>Negates the source value.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to negate</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ operator - (MpirExpression^ a);
                                                                                                          
            /// <summary>Divides two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para>You can optionally save the remainder or specify the rounding mode to use for the division by calling methods on the resulting expression, before assigning it.</summary>
            /// <param name="a">Source value to divide</param>
            /// <param name="b">Source value to divide <paramref name="a"/> by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation.
            /// <para>The expression exposes methods you can call optionally if you need to save the remainder of the division, and/or to set the rounding mode.
            /// </para>By default, the remainder is not computed and the rounding mode defaults to the static MpirSettings.DefaultRoundingMode.
            /// </returns>
            static MpirDivideExpression^ operator / (MpirExpression^ a, MpirExpression^ b);
                                                                                                          
            /// <summary>Divides two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para>You can optionally save the remainder or specify the rounding mode to use for the division by calling methods on the resulting expression, before assigning it.</summary>
            /// <param name="a">Source value to divide</param>
            /// <param name="b">Source value to divide <paramref name="a"/> by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation.
            /// <para>The expression exposes methods you can call optionally if you need to save the remainder of the division, and/or to set the rounding mode.
            /// </para>By default, the remainder is not computed and the rounding mode defaults to the static MpirSettings.DefaultRoundingMode.
            /// </returns>
            static MpirDivideUiExpression^ operator / (MpirExpression^ a, mpir_ui b);
                                                                                                          
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
            static MpirModExpression^ operator % (MpirExpression^ a, MpirExpression^ b);
                                                                                                          
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
            static MpirModUiExpression^ operator % (MpirExpression^ a, mpir_ui b);
                                                                                                          
            /// <summary>Raises the source value to the specified power.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to multiply</param>
            /// <param name="power">Power to raise <paramref name="a"/> to</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ operator ^ (MpirExpression^ a, mpir_ui power);
                                                                                                          
            /// <summary>Computes the bitwise AND of two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to AND</param>
            /// <param name="b">Source value to AND with</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ operator & (MpirExpression^ a, MpirExpression^ b);
                                                                                                          
            /// <summary>Computes the bitwise (inclusive) OR of two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to OR</param>
            /// <param name="b">Source value to OR with</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ operator | (MpirExpression^ a, MpirExpression^ b);
                                                                                                          
            /// <summary>Computes the bitwise XOR (exclusive or) of two numbers.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to XOR</param>
            /// <param name="b">Source value to XOR with</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ operator ^ (MpirExpression^ a, MpirExpression^ b);
                                                                                                          
            /// <summary>Computes the bitwise complement of a number.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to complement</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ operator ~ (MpirExpression^ a);
                                                                                                          
            /// <summary>Computes the absolute value of the source number.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MpirExpression^ Abs();
                                                                                                          
            /// <summary>Divides two numbers where it is known in advance that the division is exact.  This method is faster than normal division,
            /// but produces an undefined result when the division is not exact.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to divide by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation.
            /// </returns>
            MpirExpression^ DivideExactly(MpirExpression^ a);
                                                                                                          
            /// <summary>Divides two numbers where it is known in advance that the division is exact.  This method is faster than normal division,
            /// but produces an undefined result when the division is not exact.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to divide by</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation.
            /// </returns>
            MpirExpression^ DivideExactly(mpir_ui a);
                                                                                                          
            /// <summary>Raises the source value to the specified <paramref name="power"/> modulo <paramref name="modulo"/>.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="power">Power to raise the source value to</param>
            /// <param name="modulo">Modulo to perform the powering operation with</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MpirExpression^ PowerMod(MpirExpression^ power, MpirExpression^ modulo);
                                                                                                          
            /// <summary>Raises the source value to the specified <paramref name="power"/> modulo <paramref name="modulo"/>.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="power">Power to raise the source value to</param>
            /// <param name="modulo">Modulo to perform the powering operation with</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MpirExpression^ PowerMod(mpir_ui power, MpirExpression^ modulo);

            /// <summary>Computes the root of the specified <paramref name="power"/> from the source value.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para>You can optionally save the remainder from the root operation, or a flag indicating whether the root was exact, 
            /// by calling a method on the resulting expression, before assigning it.</summary>
            /// <param name="power">Power of the root to compute.</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MpirRootExpression^ Root(mpir_ui power);

            /// <summary>Computes the square root of the source value.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para>You can optionally save the remainder from the root operation
            /// by calling a method on the resulting expression, before assigning it.</summary>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MpirSquareRootExpression^ SquareRoot();

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
            /// </para></summary>
            /// <param name="a">Value to compare the source with</param>
            /// <returns>A positive number if the source is greater than <paramref name="a"/>, negative if less, and zero if they are equal.</returns>
            virtual int CompareTo(Object^ a) sealed;

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Value to compare the source with</param>
            /// <returns>A positive number if the source is greater than <paramref name="a"/>, negative if less, and zero if they are equal.</returns>
            virtual int CompareTo(MpirExpression^ a) sealed;

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Value to compare the source with</param>
            /// <returns>true if the values of the source and <paramref name="a"/> are equal, false otherwise.</returns>
            virtual bool Equals(MpirExpression^ a) sealed;

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
            static bool operator <  (MpirExpression^ a, MpirExpression^ b) { return  IS_NULL(a) ? !IS_NULL(b) : a->CompareTo(b) < 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >= (MpirExpression^ a, MpirExpression^ b) { return  IS_NULL(a) ?  IS_NULL(b) : a->CompareTo(b) >= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator == (MpirExpression^ a, MpirExpression^ b) { return  IS_NULL(a) ?  IS_NULL(b) : a->CompareTo(b) == 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator != (MpirExpression^ a, MpirExpression^ b) { return  IS_NULL(a) ? !IS_NULL(b) : a->CompareTo(b) != 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >  (MpirExpression^ a, MpirExpression^ b) { return !IS_NULL(a) && a->CompareTo(b) > 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <= (MpirExpression^ a, MpirExpression^ b) { return  IS_NULL(a) || a->CompareTo(b) <= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <  (MpirExpression^ a, mpir_ui b) { return  IS_NULL(a) || a->CompareTo(b) < 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >= (MpirExpression^ a, mpir_ui b) { return !IS_NULL(a) && a->CompareTo(b) >= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >  (MpirExpression^ a, mpir_ui b) { return !IS_NULL(a) && a->CompareTo(b) > 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <= (MpirExpression^ a, mpir_ui b) { return  IS_NULL(a) || a->CompareTo(b) <= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator != (MpirExpression^ a, mpir_ui b) { return  IS_NULL(a) || a->CompareTo(b) != 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator == (MpirExpression^ a, mpir_ui b) { return !IS_NULL(a) && a->CompareTo(b) == 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <  (mpir_ui b, MpirExpression^ a) { return !IS_NULL(a) && a->CompareTo(b) > 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >= (mpir_ui b, MpirExpression^ a) { return  IS_NULL(a) || a->CompareTo(b) <= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >  (mpir_ui b, MpirExpression^ a) { return  IS_NULL(a) || a->CompareTo(b) < 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <= (mpir_ui b, MpirExpression^ a) { return !IS_NULL(a) && a->CompareTo(b) >= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator != (mpir_ui b, MpirExpression^ a) { return  IS_NULL(a) || a->CompareTo(b) != 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator == (mpir_ui b, MpirExpression^ a) { return !IS_NULL(a) && a->CompareTo(b) == 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <  (MpirExpression^ a, mpir_si b) { return  IS_NULL(a) || a->CompareTo(b) < 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >= (MpirExpression^ a, mpir_si b) { return !IS_NULL(a) && a->CompareTo(b) >= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >  (MpirExpression^ a, mpir_si b) { return !IS_NULL(a) && a->CompareTo(b) > 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <= (MpirExpression^ a, mpir_si b) { return  IS_NULL(a) || a->CompareTo(b) <= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator != (MpirExpression^ a, mpir_si b) { return  IS_NULL(a) || a->CompareTo(b) != 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator == (MpirExpression^ a, mpir_si b) { return !IS_NULL(a) && a->CompareTo(b) == 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <  (mpir_si b, MpirExpression^ a) { return !IS_NULL(a) && a->CompareTo(b) > 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >= (mpir_si b, MpirExpression^ a) { return  IS_NULL(a) || a->CompareTo(b) <= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >  (mpir_si b, MpirExpression^ a) { return  IS_NULL(a) || a->CompareTo(b) < 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <= (mpir_si b, MpirExpression^ a) { return !IS_NULL(a) && a->CompareTo(b) >= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator != (mpir_si b, MpirExpression^ a) { return  IS_NULL(a) || a->CompareTo(b) != 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator == (mpir_si b, MpirExpression^ a) { return !IS_NULL(a) && a->CompareTo(b) == 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <  (MpirExpression^ a, double b) { return  IS_NULL(a) || a->CompareTo(b) < 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >= (MpirExpression^ a, double b) { return !IS_NULL(a) && a->CompareTo(b) >= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >  (MpirExpression^ a, double b) { return !IS_NULL(a) && a->CompareTo(b) > 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <= (MpirExpression^ a, double b) { return  IS_NULL(a) || a->CompareTo(b) <= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator != (MpirExpression^ a, double b) { return  IS_NULL(a) || a->CompareTo(b) != 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator == (MpirExpression^ a, double b) { return !IS_NULL(a) && a->CompareTo(b) == 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <  (double b, MpirExpression^ a) { return !IS_NULL(a) && a->CompareTo(b) > 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >= (double b, MpirExpression^ a) { return  IS_NULL(a) || a->CompareTo(b) <= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator >  (double b, MpirExpression^ a) { return  IS_NULL(a) || a->CompareTo(b) < 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator <= (double b, MpirExpression^ a) { return !IS_NULL(a) && a->CompareTo(b) >= 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator != (double b, MpirExpression^ a) { return  IS_NULL(a) || a->CompareTo(b) != 0; }

            /// <summary>Compares two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Source value to compare</param>
            /// <param name="b">Source value to compare with</param>
            /// <returns>A boolean result of the comparison.</returns>
            static bool operator == (double b, MpirExpression^ a) { return !IS_NULL(a) && a->CompareTo(b) == 0; }

            /// <summary>Compares the absolute values of two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Value to compare the source with</param>
            /// <returns>A positive number if the absolute value of the source is greater than the absolute value of <paramref name="a"/>, negative if less, and zero if they are equal.</returns>
            int CompareAbsTo(MpirExpression^ a) { IN_CONTEXT_2(this, a); return mpz_cmpabs(context.Args[0], context.Args[1]); }

            /// <summary>Compares the absolute values of two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Value to compare the source with</param>
            /// <returns>A positive number if the absolute value of the source is greater than <paramref name="a"/>, negative if less, and zero if they are equal.</returns>
            int CompareAbsTo(mpir_ui a) { IN_CONTEXT_1(this); return mpz_cmpabs_ui(context.Args[0], a); }

            /// <summary>Compares the absolute values of two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Value to compare the source with</param>
            /// <returns>A positive number if the absolute value of the source is greater than the absolute value of <paramref name="a"/>, negative if less, and zero if they are equal.</returns>
            int CompareAbsTo(double a) { IN_CONTEXT_1(this); return mpz_cmpabs_d(context.Args[0], a); }

            /// <summary>Calculates the sign (+1, 0, or -1) of the source value.
            /// <para>If the source is an expression, it is evaluated into a temporary variable before the sign is computed.
            /// </para></summary>
            /// <returns>+1 if the source is positive, -1 if negative, and 0 if zero.</returns>
            int Sign() { IN_CONTEXT_1(this); return mpz_sgn(context.Args[0]); }

            #pragma endregion

            #pragma region Utility methods

            /// <summary>
            /// Checks if the source is evenly divisible by <paramref name="a"/>.
            /// Because this method returns a primitive type, it is computed immediately.
            /// </summary>
            /// <param name="a">Divisor to test with</param>
            /// <returns>True if the source is evenly divisible by <paramref name="a"/></returns>
            bool IsDivisibleBy(MpirExpression^ a) { IN_CONTEXT_2(this, a); return mpz_divisible_p(context.Args[0], context.Args[1]) != 0; }

            /// <summary>
            /// Checks if the source is evenly divisible by <paramref name="a"/>.
            /// Because this method returns a primitive type, it is computed immediately.
            /// </summary>
            /// <param name="a">Divisor to test with</param>
            /// <returns>True if the source is evenly divisible by <paramref name="a"/></returns>
            bool IsDivisibleBy(mpir_ui a) { IN_CONTEXT_1(this); return mpz_divisible_ui_p(context.Args[0], a) != 0; }

            /// <summary>
            /// Checks if the source is evenly divisible by 2^<paramref name="power"/>.
            /// Because this method returns a primitive type, it is computed immediately.
            /// </summary>
            /// <param name="power">Power of 2 to use for the divisor</param>
            /// <returns>True if the source is evenly divisible by 2^<paramref name="power"/></returns>
            bool IsDivisibleByPowerOf2(mp_bitcnt_t power) { IN_CONTEXT_1(this); return mpz_divisible_2exp_p(context.Args[0], power) != 0; }

            /// <summary>
            /// Checks if the source is congruent to <paramref name="a"/> modulo <paramref name="mod"/>.
            /// Because this method returns a primitive type, it is computed immediately.
            /// </summary>
            /// <param name="a">Divisor to test with</param>
            /// <param name="mod">Modulo with respect to which to test for congruency</param>
            /// <returns>True if the source is congruent to <paramref name="a"/> modulo <paramref name="mod"/></returns>
            bool IsCongruentTo(MpirExpression^ a, MpirExpression^ mod) { IN_CONTEXT_3(this, a, mod); return mpz_congruent_p(context.Args[0], context.Args[1], context.Args[2]) != 0; }

            /// <summary>
            /// Checks if the source is congruent to <paramref name="a"/> modulo <paramref name="mod"/>.
            /// Because this method returns a primitive type, it is computed immediately.
            /// </summary>
            /// <param name="a">Divisor to test with</param>
            /// <param name="mod">Modulo with respect to which to test for congruency</param>
            /// <returns>True if the source is congruent to <paramref name="a"/> modulo <paramref name="mod"/></returns>
            bool IsCongruentTo(mpir_ui a, mpir_ui mod) { IN_CONTEXT_1(this); return mpz_congruent_ui_p(context.Args[0], a, mod) != 0; }

            /// <summary>
            /// Checks if the source is congruent to <paramref name="a"/> modulo 2^<paramref name="power"/>.
            /// Because this method returns a primitive type, it is computed immediately.
            /// </summary>
            /// <param name="a">Divisor to test with</param>
            /// <param name="power">Power of 2 to use for the modulo</param>
            /// <returns>True if the source is congruent to <paramref name="a"/> modulo 2^<paramref name="power"/></returns>
            bool IsCongruentToModPowerOf2(MpirExpression^ a, mp_bitcnt_t power) { IN_CONTEXT_2(this, a); return mpz_congruent_2exp_p(context.Args[0], context.Args[1], power) != 0; }

            /// <summary>
            /// Checks if the source is a perfect power.
            /// Because this method returns a primitive type, it is computed immediately.
            /// </summary>
            /// <returns>True if the source is a perfect power</returns>
            bool IsPerfectPower() { IN_CONTEXT_1(this); return mpz_perfect_power_p(context.Args[0]) != 0; }

            /// <summary>
            /// Checks if the source is a perfect square.
            /// Because this method returns a primitive type, it is computed immediately.
            /// </summary>
            /// <returns>True if the source is a perfect square</returns>
            bool IsPerfectSquare() { IN_CONTEXT_1(this); return mpz_perfect_square_p(context.Args[0]) != 0; }

            #pragma endregion
    };

    /// <summary>
    /// Static class for MPIR settings such as rounding defaults
    /// </summary>
    public ref class MpirSettings abstract sealed
    {
        public:
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
            static property int ToStringDigits;

            static MpirSettings()
            {
                RoundingMode = RoundingModes::Truncate;
                ToStringDigits = 256;
            }
    };

    #pragma region mid-level abstract expression specializations

    /// <summary>
    /// Expression that results from a division or modulo operator.  Allows to set the rounding mode for the division.
    /// </summary>
    public ref class MpirDivModExpression abstract : MpirExpression 
    {
        internal:
            MpirDivModExpression() { }
            RoundingModes rounding;

        public:
            /// <summary>
            /// Optionally sets the rounding mode for the division.  If not set, the static MpirSettings.RoundingMode will be used.
            /// </summary>
            /// <param name="mode">the mode to use.  If this is Default, the static MpirSettings.Rounding mode is used.</param>
            /// <returns>An updated expression, with its internal state updated to use the specified rounding mode.</returns>
            MpirExpression^ Rounding(RoundingModes mode)
            {
                rounding = mode;
                return this;
            }
    };

    /// <summary>
    /// Expression that results from a division operator.  Allows to save the remainder to a separate result object, and/or set the rounding mode for the division.
    /// </summary>
    public ref class MpirDivideExpression abstract : MpirDivModExpression 
    {
        internal:
            MpirDivideExpression() { }
            HugeInt^ _remainder;
            void custom_mpz_div(mpz_ptr q, mpz_srcptr n, mpz_srcptr d);

        public:
            /// <summary>
            /// Optionally save the remainder of the division to a separate result.  This cannot be the same object the resulting division quotient is being assigned to.
            /// </summary>
            /// <param name="destination">destination for the remainder.  This cannot be the same object the resulting division quotient is being assigned to.</param>
            /// <returns>An updated expression, with its internal state updated to compute the remainder.</returns>
            MpirDivModExpression^ SavingRemainderTo(HugeInt^ destination)
            {
                _remainder = destination;
                return this;
            }
    };

    /// <summary>
    /// Expression that results from a division operator.  Allows to save the remainder, and/or set the rounding mode for the division.
    /// </summary>
    public ref class MpirDivideUiExpression abstract : MpirDivideExpression 
    {
        private:
            Action<mpir_ui>^ _limbRemainder;

        internal:
            MpirDivideUiExpression() { }
            void custom_mpz_div_ui(mpz_ptr q, mpz_srcptr n, mpir_ui d);

        public:
            /// <summary>
            /// Optionally saves the remainder of the division to a separate result.
            /// </summary>
            /// <param name="callback">The delegate that will be called with the remainder of the division.  
            /// The delegate is called when the division is evaluated, i.e. is assigned to the Value property or consumed by a method that returns a primitive type.</param>
            /// <returns>An updated expression, with its internal state updated to compute the remainder.</returns>
            MpirDivideExpression^ SettingRemainderTo(Action<mpir_ui>^ callback)
            {
                _limbRemainder = callback;
                return this;
            }
    };

    /// <summary>
    /// Expression that results from a modulo operator.  Allows to save the division result to a separate object, and/or set the rounding mode for the division.
    /// </summary>
    public ref class MpirModExpression abstract : MpirDivModExpression 
    {
        internal:
            MpirModExpression() { }
            HugeInt^ _quotient;
            void custom_mpz_mod(mpz_ptr r, mpz_srcptr n, mpz_srcptr d);

        public:
            /// <summary>
            /// Optionally save the quotient of the division to a separate result.  This cannot be the same object the resulting division modulo is being assigned to.
            /// </summary>
            /// <param name="destination">destination for the quotient.  This cannot be the same object the resulting division modulo is being assigned to.</param>
            /// <returns>An updated expression, with its internal state updated to compute the quotient.</returns>
            MpirDivModExpression^ SavingQuotientTo(HugeInt^ destination)
            {
                _quotient = destination;
                return this;
            }
    };

    /// <summary>
    /// Expression that results from a modulo operator.  Allows to save the division result to a separate object, and/or set the rounding mode for the division.
    /// </summary>
    public ref class MpirModUiExpression abstract : MpirModExpression 
    {
        private:
            Action<mpir_ui>^ _limbRemainder;

        internal:
            MpirModUiExpression() { }
            void custom_mpz_mod_ui(mpz_ptr r, mpz_srcptr n, mpir_ui d);

        public:
            /// <summary>
            /// Optionally saves the remainder of the division to a separate result.
            /// </summary>
            /// <param name="callback">The delegate that will be called with the remainder of the division.  
            /// The delegate is called when the division is evaluated, i.e. is assigned to the Value property or consumed by a method that returns a primitive type.</param>
            /// <returns>An updated expression, with its internal state updated to compute the remainder.</returns>
            MpirModExpression^ SettingRemainderTo(Action<mpir_ui>^ callback)
            {
                _limbRemainder = callback;
                return this;
            }
    };

    /// <summary>
    /// Expression that results from a right shift operator.  Allows to save the remainder to a separate result object, and/or set the rounding mode for the division.
    /// </summary>
    public ref class MpirShiftRightExpression abstract : MpirDivModExpression
    {
        private:
            bool _remainder;

        internal:
            MpirShiftRightExpression() { }
            void custom_mpz_div_2exp(mpz_ptr q, mpz_srcptr n, mp_bitcnt_t bits);

        public:
            /// <summary>
            /// Computes the remainder of the division, rather than the quotient, which is the default.
            /// </summary>
            /// <returns>An updated expression, with its internal state updated to compute the remainder, rather than the quotient.</returns>
            MpirDivModExpression^ Remainder()
            {
                _remainder = true;
                return this;
            }
    };

    /// <summary>
    /// Expression that results from a square root function.  Allows to save the remainder to a separate result object.
    /// </summary>
    public ref class MpirSquareRootExpression abstract : MpirExpression 
    {
        internal:
            MpirSquareRootExpression() { }
            HugeInt^ _remainder;
            void custom_mpz_sqrt(mpz_ptr dest, mpz_srcptr oper);

        public:
            /// <summary>
            /// Optionally saves the remainder of the root operation to a separate result.
            /// </summary>
            /// <param name="destination">destination for the remainder.  This cannot be the same object the result of the root operation is being assigned to.</param>
            /// <returns>An updated expression, with its internal state updated to save the remainder.</returns>
            MpirExpression^ SavingRemainderTo(HugeInt^ destination)
            {
                _remainder = destination;
                return this;
            }
    };

    /// <summary>
    /// Expression that results from a root function.  Allows to save a flag indicating whether the root was exact, or to save the remainder to a separate result object.
    /// </summary>
    public ref class MpirRootExpression abstract : MpirSquareRootExpression 
    {
        private:
            Action<bool>^ _exact;

        internal:
            MpirRootExpression() { }
            void custom_mpz_root(mpz_ptr dest, mpz_srcptr oper, mpir_ui power);

        public:
            /// <summary>
            /// Optionally gets a flag indicating whether the root operation was exact.
            /// </summary>
            /// <param name="callback">Delegate that will be called with the exact flag.
            /// The delegate is called when the root operation is evaluated, i.e. is assigned to the Value property or consumed by a method that returns a primitive type.</param>
            /// <returns>An updated expression, with its internal state updated to compute the exact flag.</returns>
            MpirExpression^ SettingExactTo(Action<bool>^ callback)
            {
                _exact = callback;
                return this;
            }
    };

    #pragma endregion

    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MpirExpression, Add, Int)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MpirExpression, Add, Int, Ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MpirExpression, Add, Int, Si)
                                                   
    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MpirExpression, Subtract, Int)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MpirExpression, Subtract, Int, Ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT    (MpirExpression, Subtract, Ui, Int)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MpirExpression, Subtract, Int, Si)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT    (MpirExpression, Subtract, Si, Int)
                                                   
    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MpirExpression, Multiply, Int)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MpirExpression, Multiply, Int, Ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MpirExpression, Multiply, Int, Si)
                                                   
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MpirExpression, ShiftLeft, Int, Bits)
                                                   
    DEFINE_UNARY_EXPRESSION_WITH_ONE               (MpirExpression, Negate, Int)
    DEFINE_UNARY_EXPRESSION_WITH_ONE               (MpirExpression, Abs, Int)
                                                   
    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MpirDivideExpression, Divide, Int)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MpirDivideUiExpression, Divide, Int, Ui)
                                                   
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MpirShiftRightExpression, ShiftRight, Int, Bits)
                                                   
    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MpirModExpression, Mod, Int)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MpirModUiExpression, Mod, Int, Ui)
                                                   
    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MpirExpression, DivideExactly, Int)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MpirExpression, DivideExactly, Int, Ui)

    DEFINE_TERNARY_EXPRESSION_WITH_THREE           (MpirExpression, PowerMod, Int)
    DEFINE_TERNARY_EXPRESSION_WITH_BUILT_IN_MIDDLE (MpirExpression, PowerMod, Int, Ui, Int)

    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MpirExpression, Power, Int, Ui)

    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MpirRootExpression, Root, Int, Ui)
    DEFINE_UNARY_EXPRESSION_WITH_ONE               (MpirSquareRootExpression, SquareRoot, Int)

    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MpirExpression, And, Int)
    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MpirExpression, Or, Int)
    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MpirExpression, Xor, Int)
    DEFINE_UNARY_EXPRESSION_WITH_ONE               (MpirExpression, Complement, Int)

    DEFINE_OPERATIONS(DEFINE)

    /// <summary>
    /// Multi-precision Integer class.
    /// </summary>
    public ref class HugeInt sealed : MpirExpression
    {
        internal:
            //fields
            mpz_ptr _value;

        private:
            //construction
            void AllocateStruct()
            {
                _value = (mpz_ptr)CustomAllocate(sizeof(__mpz_struct));
            }
            void DeallocateStruct()
            {
                mpz_clear(_value);
                CustomFree(_value);
                _value = nullptr;
            }
            void FromString(String^ value, int base);

        internal:
            //assignment
            virtual void AssignTo(mpz_ptr destination) override
            {
                if(destination != _value)
                    mpz_set(destination, _value);
            }
            virtual void AssignTo(EvaluationContext& destination) override
            {
                destination.Args[destination.Index++] = _value;
            }

        public:
            #pragma region construction and disposal

            /// <summary>
            /// Initializes a new integer instance and sets its value to 0
            /// </summary>
            HugeInt();

            /// <summary>
            /// Initializes a new integer instance, allocating enough memory to hold at least <paramref name="bits"/> bits, and sets its value to 0
            /// </summary>
            /// <param name="bits">Minimum number of bits the initially allocated memory should hold</param>
            HugeInt(mp_bitcnt_t bits);

            /// <summary>
            /// Initializes a new integer instance and sets its value from the specified string, using leading characters to recognize the base:
            /// 0x and 0X for hexadecimal, 0b and 0B for binary, 0 for octal, or decimal otherwise.
            /// </summary>
            /// <param name="value">string representing the initial value for the new instance.  Whitespace in the string is ignored.</param>
            HugeInt(String^ value) { FromString(value, 0); }

            /// <summary>
            /// Initializes a new integer instance and sets its value from the specified string
            /// </summary>
            /// <param name="value">string representing the initial value for the new instance.  Whitespace in the string is ignored.</param>
            /// <param name="base">base the <paramref name="value"/> string is in.
            /// <para>The base may vary from 2 to 62, or if base is 0, then the leading characters are used: 0x and 0X for hexadecimal, 0b and 0B for binary, 0 for octal, or decimal otherwise.
            /// </para>For bases up to 36, case is ignored; upper-case and lower-case letters have the same value. 
            /// For bases 37 to 62, upper-case letter represent the usual 10..35 while lower-case letter represent 36..61.</param>
            HugeInt(String^ value, int base) { FromString(value, base); }

            /// <summary>
            /// Constructs and returns a new integer instance with its value set to the <paramref name="value"/> parameter.
            /// </summary>
            /// <param name="value">Initial value for the new integer instance</param>
            /// <returns>the newly constructed instance</returns>
            static HugeInt^ FromLong(mpir_si value);

            /// <summary>
            /// Constructs and returns a new integer instance with its value set to the <paramref name="value"/> parameter.
            /// </summary>
            /// <param name="value">Initial value for the new integer instance</param>
            /// <returns>the newly constructed instance</returns>
            static HugeInt^ FromUlong(mpir_ui value);

            /// <summary>
            /// Constructs and returns a new integer instance with its value set to the <paramref name="value"/> parameter.
            /// </summary>
            /// <param name="value">Initial value for the new integer instance.  Any fractional portion is truncated.</param>
            /// <returns>the newly constructed instance</returns>
            static HugeInt^ FromDouble(double value);

            /// <summary>
            /// Constructs and returns a new integer instance with its value set to <paramref name="value"/> raised to the specified <paramref name="power"/>.
            /// </summary>
            /// <param name="value">Base for the initial value for the new integer instance</param>
            /// <param name="power">Power to raise the <paramref name="value"/> to when calculating the initial value for the new instance</param>
            /// <returns>the newly constructed instance</returns>
            static HugeInt^ FromPower(mpir_ui value, mpir_ui power);

            /// <summary>
            /// Change the space allocated for integer to <paramref name="bits"/> bits. The value in integer is preserved if it fits, or is set to 0 if not.
            /// <para>This function can be used to increase the space for a variable in order to avoid repeated automatic reallocations, or to decrease it to give memory back to the heap.
            /// </para></summary>
            /// <param name="bits">Minimum number of bits the allocated memory should hold</param>
            void Reallocate(mp_bitcnt_t bits) { mpz_realloc2(_value, bits); }

            //disposal

            //creating a destructor in C++ implements IDisposable.

            /// <summary>
            /// Frees all memory allocated by the instance.
            /// <para>To minimize memory footprint, multi-precision objects should be disposed of when no longer used, instead of relying on the garbage collector to free the memory.
            /// </para></summary>
            ~HugeInt() { this->!HugeInt(); }

            /// <summary>
            /// Frees all memory allocated by the instance.
            /// <para>To minimize memory footprint, multi-precision objects should be disposed of when no longer used, instead of relying on the garbage collector to free the memory.
            /// </para></summary>
            !HugeInt() { if(_value != 0) DeallocateStruct(); }

            #pragma endregion

            #pragma region conversions

            /// <summary>
            /// Converts the number to a string.
            /// <para>To avoid debugging performance problems, this method outputs at most the number of digits specified in MpirSettings.ToStringDigits.
            /// If the number is larger, the least significant digits are shown with a leading ellipsis (i.e., [-]...NNNNN)
            /// </para>Setting MpirSettings.ToStringDigits to 0 removes the upper limit.
            /// </summary>
            /// <returns>A string representation of the number in decimal, possibly cut off if the number has more digits than MpirSettings.ToStringDigits.</returns>
            virtual String^ ToString() override;

            /// <summary>
            /// Converts the number to a string in the specified base.
            /// <para>This method always produces the complete output regardless of the MpirSettings.ToStringDigits setting.
            /// </para></summary>
            /// <param name="base">The base to use for the output.  The base can be from 2 to 62; uppercase letters represent digits 10-35 and lowercase letters represent digits 36-61.</param>
            /// <returns>A string representation of the number in the specified base.</returns>
            String^ ToString(int base) { return ToString(base, false); }

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
            String^ ToString(int base, bool lowercase);

            /// <summary>
            /// Returns the absolute value of the number of the number as a ulong.
            /// <para>If the number is too big, then just the least significant bits that do fit are returned.
            /// </para>The sign of the number is ignored, only the absolute value is used.
            /// </summary>
            /// <returns>The absolute value as a ulong, possibly truncated to the least significant bits only.</returns>
            mpir_ui ToUlong() { return mpz_get_ui(_value); }

            /// <summary>
            /// Returns the value of the number of the number as a long.
            /// <para>If the number is too big, then just the least significant bits that do fit are returned, with the same sign as the number.
            /// </para>When truncation occurs, the result is propobly not very useful.  Call FitsLong() to check if the number will fit.
            /// </summary>
            /// <returns>The value as a ulong, possibly truncated to the least significant bits only.</returns>
            mpir_si ToLong() { return mpz_get_si(_value); }

            /// <summary>
            /// Returns the value of the number of the number as a double, truncating if necessary (rounding towards zero).
            /// <para>If the exponent from the conversion is too big, the result is system dependent. An infinity is returned where available.             /// A hardware overflow trap may or may not occur.
            /// </para></summary>
            /// <returns>The value as a double, possibly truncated.</returns>
            double ToDouble() { return mpz_get_d(_value); }

            /// <summary>
            /// Returns the value of the number of the number as a double, truncating if necessary (rounding towards zero), and returning the exponent separately.
            /// <para>The return is the mantissa, its absolute value will be in the range [0.5 - 1).            /// </para>If the source value is zero, both mantissa and exponent are returned as 0.
            /// </summary>
            /// <param name="exp">variable to store the exponent in.</param>
            /// <returns>The mantissa of the value as a double, possibly truncated.</returns>
            double ToDouble([Out] mpir_si% exp) 
            { 
                mpir_si x; 
                auto result = mpz_get_d_2exp(&x, _value); 
                exp = x; 
                return result; 
            }

            #pragma endregion

            #pragma region assignment

            /// <summary>
            /// Sets the value of the integer object.
            /// <para>MPIR types are implemented as reference types with value semantics.
            /// Like Strings, the objects themselves are just lightweight pointers to data allocated elsewhere.
            /// Unlike Strings, MPIR types are mutable.  Value semantics requires you to be able to code, a = b + c.
            /// However, .Net (outside of C++) does not allow overloading the assignment operator,
            /// and assigning references would necessitate some unnecessary duplication and extra memory allocations.
            /// </para>To solve this problem, MPIR.Net uses the property assignment.  
            /// The setter of the Value property does what an overloaded assignment operator would do in C++.
            /// The syntax is a little different: a.Value = b + c, but it is fluent enough to become a quick habit,
            /// and additionally reinforces the concept that an existing object can change its value while reusing internally allocated memory.
            /// <para>To this end, all overloaded operators and most functions that operate on MPIR types,
            /// instead of eagerly computing a result, instead produce and return an expression that is basically a formula for the computation.
            /// Expressions can then be composed using additional operators to achieve expression trees of arbitrary complexity.
            /// All computations are deferred until an expression is assigned to the Value property of an MPIR object,
            /// or is consumed by a method or operator that returns a primitive type.
            /// </para>Do not set the Value of an object while it is contained in a hash table, because that changes its hash code.
            /// </summary>
            property MpirExpression^ Value
            {
                void set(MpirExpression^ expr) { expr->AssignTo(_value); }
            }

            /// <summary>
            /// Sets the value of the integer object.
            /// <para>Do not change the value of an object while it is contained in a hash table, because that changes its hash code.
            /// </para></summary>
            /// <param name="value">new value for the object</param>
            void SetTo(mpir_ui value) { mpz_set_ui(_value, value); }

            /// <summary>
            /// Sets the value of the integer object.
            /// <para>Do not change the value of an object while it is contained in a hash table, because that changes its hash code.
            /// </para></summary>
            /// <param name="value">new value for the object</param>
            void SetTo(mpir_si value) { mpz_set_si(_value, value); }

            /// <summary>
            /// Sets the value of the integer object.  Any fractional portion is truncated.
            /// <para>Do not change the value of an object while it is contained in a hash table, because that changes its hash code.
            /// </para></summary>
            /// <param name="value">new value for the object</param>
            void SetTo(double value) { mpz_set_d(_value, value); }

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
            /// Swaps the values of two integers.
            /// <para>This operation is a pointer swap and doesn't affect allocated memory.
            /// </para>Do not call this method while either object is contained in a hash table, because this would change their hash codes.
            /// </summary>
            /// <param name="a">Source number to swap this instance's value with</param>
            void Swap(HugeInt^ a) 
            { 
                mpz_ptr temp = a->_value;
                a->_value = _value;
                _value = temp; 
            }

            #pragma endregion
    };
};