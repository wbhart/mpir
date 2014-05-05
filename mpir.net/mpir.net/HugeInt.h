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

#pragma region misc macros

#define IS_NULL(a) (Object::ReferenceEquals(a, nullptr))
#define PIN(x) pin_ptr<T> pinptr##x = &x[0]; void* pinned_##x = pinptr##x;

#pragma endregion

#pragma region Expression macros

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

#define COUNT_ARGS_IMPL2(_1, _2, _3, name, ...) name
#define COUNT_ARGS_IMPL(args) COUNT_ARGS_IMPL2 args
#define COUNT_ARGS(...) COUNT_ARGS_IMPL((__VA_ARGS__, 3, 2, 1))
#define MACRO_CHOOSE2(prefix, number) prefix##number
#define MACRO_CHOOSE1(prefix, number) MACRO_CHOOSE2(prefix, number)
#define MACRO_CHOOSE(prefix, number) MACRO_CHOOSE1(prefix, number)
#define MACRO_GLUE(x, y) x y
#define IN_CONTEXT(...) MACRO_GLUE(MACRO_CHOOSE(IN_CONTEXT_, COUNT_ARGS(__VA_ARGS__)), (__VA_ARGS__))

#define TYPE_FOR_ABBR_Int HugeInt^
#define TYPE_FOR_ABBR_Expr MpirExpression^
#define TYPE_FOR_ABBR_Si mpir_si
#define TYPE_FOR_ABBR_Ui mpir_ui
#define TYPE_FOR_ABBR_Bits mp_bitcnt_t
#define TYPE_FOR_ABBR_Rnd MpirRandom^

//unary expressions
#define DEFINE_UNARY_EXPRESSION_WITH_ONE(base, name, typeAbbr) \
    DEFINE_UNARY_EXPRESSION(base, name##typeAbbr, MpirExpression^)           

#define DEFINE_UNARY_EXPRESSION_WITH_BUILT_INS_ONLY(base, name, typeAbbr)    \
    DEFINE_UNARY_EXPRESSION(base, name##typeAbbr, TYPE_FOR_ABBR_##typeAbbr)

//binary expressions
#define DEFINE_BINARY_EXPRESSION_WITH_BUILT_INS_ONLY(base, name, leftTypeAbbr, rightTypeAbbr)    \
    DEFINE_BINARY_EXPRESSION(base, name##leftTypeAbbr##rightTypeAbbr, TYPE_FOR_ABBR_##leftTypeAbbr, TYPE_FOR_ABBR_##rightTypeAbbr)

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

#pragma endregion

#pragma region Method macros

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

#pragma endregion

namespace MPIR
{
    ref class MpirRandom;
    ref class HugeInt;
    ref class MpirDivideExpression;
    ref class MpirDivideUiExpression;
    ref class MpirModExpression;
    ref class MpirDivModExpression;
    ref class MpirModUiExpression;
    ref class MpirShiftRightExpression;
    ref class MpirRootExpression;
    ref class MpirSquareRootExpression;
    ref class MpirGcdExpression;
    ref class MpirRemoveFactorsExpression;
    ref class MpirSequenceExpression;

    #pragma region enums

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

    #pragma endregion

    #pragma region MpirExpression

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

            /// <summary>If the source is &gt;= 0, returns the population count of op, which is the number of 1 bits in the binary representation.
            /// <para>If the source is &lt; 0, the number of 1s is infinite, and the return value is ulong.MaxValue, the largest possible bit count.
            /// </para>Because the result is a primitive type, it is computed immediately.
            /// </summary>
            /// <returns>The population count for a non-negative number</returns>
            mp_bitcnt_t PopCount() { IN_CONTEXT(this); return mpz_popcount(context.Args[0]); }

            /// <summary>If this number and the operand are both &gt;= 0 or both &lt; 0, returns the hamming distance between them, which is the number of bit positions with different bit values.
            /// <para>If one operand is &gt;= 0 and the other &lt; 0 then the number of bits different is infinite, and the return value is ulong.MaxValue, the largest possible bit count.
            /// </para>Because the result is a primitive type, it is computed immediately.
            /// </summary>
            /// <param name="a">Source value to compute the hamming distance to</param>
            /// <returns>The hamming distance between this number and <paramref name="a"/></returns>
            mp_bitcnt_t HammingDistance(MpirExpression^ a) { IN_CONTEXT(this, a); return mpz_hamdist(context.Args[0], context.Args[1]); }

            /// <summary>Scans the source number, starting from the <paramref name="start"/> bit, towards more significant bits, until the first 0 or 1 bit
            /// (depending on the <paramref name="value"/> is found, and return the index of the found bit.
            /// <para>If the bit at the starting position is already what's sought, then <paramref name="start"/> is returned.
            /// </para>If there's no bit found, then ulong.MaxValue (the largest possible bit count) is returned. 
            /// This will happen with <paramref name="value"/> = true past the end of a non-negative number, or with <paramref name="value"/> = false past the end of a negative number.
            /// <para>A false bit will always be found at the <paramref name="start"/> position past the end of a non-negatitve number, and a true bit past the end of a negative number.
            /// </para></summary>
            /// <param name="value">Value of the bit to scan for, true for 1, false for 0</param>
            /// <param name="start">Starting bit position to search.  The least significant bit is zero.</param>
            /// <returns>The index of the found bit, or ulong.MaxValue if no bit found.</returns>
            mp_bitcnt_t FindBit(bool value, mp_bitcnt_t start) { IN_CONTEXT(this); return value ? mpz_scan1(context.Args[0], start) : mpz_scan0(context.Args[0], start); }

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
            /// <param name="power">Power to raise the source value to.
            /// <para>Negative power values are supported if an inverse mod <paramref name="modulo"/> exists, otherwise divide by zero is raised.</para></param>
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

            /// <summary>Computes the truncated integer part of the root of the specified <paramref name="power"/> from the source value.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para>You can optionally save the remainder from the root operation, or a flag indicating whether the root was exact, 
            /// by calling a method on the resulting expression, before assigning it.</summary>
            /// <param name="power">Power of the root to compute.</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MpirRootExpression^ Root(mpir_ui power);

            /// <summary>Computes the truncated integer part of the square root of the source value.
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
            int CompareAbsTo(MpirExpression^ a) { IN_CONTEXT(this, a); return mpz_cmpabs(context.Args[0], context.Args[1]); }

            /// <summary>Compares the absolute values of two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Value to compare the source with</param>
            /// <returns>A positive number if the absolute value of the source is greater than <paramref name="a"/>, negative if less, and zero if they are equal.</returns>
            int CompareAbsTo(mpir_ui a) { IN_CONTEXT(this); return mpz_cmpabs_ui(context.Args[0], a); }

            /// <summary>Compares the absolute values of two numbers.
            /// <para>If any argument is an expression, it is evaluated into a temporary variable before the comparison is performed.
            /// </para></summary>
            /// <param name="a">Value to compare the source with</param>
            /// <returns>A positive number if the absolute value of the source is greater than the absolute value of <paramref name="a"/>, negative if less, and zero if they are equal.</returns>
            int CompareAbsTo(double a) { IN_CONTEXT(this); return mpz_cmpabs_d(context.Args[0], a); }

            /// <summary>Calculates the sign (+1, 0, or -1) of the source value.
            /// <para>If the source is an expression, it is evaluated into a temporary variable before the sign is computed.
            /// </para></summary>
            /// <returns>+1 if the source is positive, -1 if negative, and 0 if zero.</returns>
            int Sign() { IN_CONTEXT(this); return mpz_sgn(context.Args[0]); }

            #pragma endregion

            #pragma region Utility methods

            /// <summary>
            /// Checks if the source is evenly divisible by <paramref name="a"/>.
            /// Because this method returns a primitive type, it is computed immediately.
            /// </summary>
            /// <param name="a">Divisor to test with.  This can be zero; only zero is considired divisible by zero.</param>
            /// <returns>True if the source is evenly divisible by <paramref name="a"/></returns>
            bool IsDivisibleBy(MpirExpression^ a) { IN_CONTEXT(this, a); return mpz_divisible_p(context.Args[0], context.Args[1]) != 0; }

            /// <summary>
            /// Checks if the source is evenly divisible by <paramref name="a"/>.
            /// Because this method returns a primitive type, it is computed immediately.
            /// </summary>
            /// <param name="a">Divisor to test with.  This can be zero; only zero is considired divisible by zero.</param>
            /// <returns>True if the source is evenly divisible by <paramref name="a"/></returns>
            bool IsDivisibleBy(mpir_ui a) { IN_CONTEXT(this); return mpz_divisible_ui_p(context.Args[0], a) != 0; }

            /// <summary>
            /// Checks if the source is evenly divisible by 2^<paramref name="power"/>.
            /// Because this method returns a primitive type, it is computed immediately.
            /// </summary>
            /// <param name="power">Power of 2 to use for the divisor</param>
            /// <returns>True if the source is evenly divisible by 2^<paramref name="power"/></returns>
            bool IsDivisibleByPowerOf2(mp_bitcnt_t power) { IN_CONTEXT(this); return mpz_divisible_2exp_p(context.Args[0], power) != 0; }

            /// <summary>
            /// Checks if the source is congruent to <paramref name="a"/> modulo <paramref name="mod"/>.
            /// Because this method returns a primitive type, it is computed immediately.
            /// </summary>
            /// <param name="a">Divisor to test with.  This can be zero; only zero is considired divisible by zero.</param>
            /// <param name="mod">Modulo with respect to which to test for congruency</param>
            /// <returns>True if the source is congruent to <paramref name="a"/> modulo <paramref name="mod"/></returns>
            bool IsCongruentTo(MpirExpression^ a, MpirExpression^ mod) { IN_CONTEXT(this, a, mod); return mpz_congruent_p(context.Args[0], context.Args[1], context.Args[2]) != 0; }

            /// <summary>
            /// Checks if the source is congruent to <paramref name="a"/> modulo <paramref name="mod"/>.
            /// Because this method returns a primitive type, it is computed immediately.
            /// </summary>
            /// <param name="a">Divisor to test with.  This can be zero; only zero is considired divisible by zero.</param>
            /// <param name="mod">Modulo with respect to which to test for congruency</param>
            /// <returns>True if the source is congruent to <paramref name="a"/> modulo <paramref name="mod"/></returns>
            bool IsCongruentTo(mpir_ui a, mpir_ui mod) { IN_CONTEXT(this); return mpz_congruent_ui_p(context.Args[0], a, mod) != 0; }

            /// <summary>
            /// Checks if the source is congruent to <paramref name="a"/> modulo 2^<paramref name="power"/>.
            /// Because this method returns a primitive type, it is computed immediately.
            /// </summary>
            /// <param name="a">Divisor to test with</param>
            /// <param name="power">Power of 2 to use for the modulo</param>
            /// <returns>True if the source is congruent to <paramref name="a"/> modulo 2^<paramref name="power"/></returns>
            bool IsCongruentToModPowerOf2(MpirExpression^ a, mp_bitcnt_t power) { IN_CONTEXT(this, a); return mpz_congruent_2exp_p(context.Args[0], context.Args[1], power) != 0; }

            /// <summary>
            /// Checks if the source is a perfect power.
            /// <para>Because this method returns a primitive type, it is computed immediately.
            /// </para>Both 0 and 1 are considered perfect powers.
            /// <para>Negative values are accepted, but of course can only be odd powers.
            /// </para></summary>
            /// <returns>True if the source is a perfect power</returns>
            bool IsPerfectPower() { IN_CONTEXT(this); return mpz_perfect_power_p(context.Args[0]) != 0; }

            /// <summary>
            /// Checks if the source is a perfect square.
            /// <para>Because this method returns a primitive type, it is computed immediately.
            /// </para>Both 0 and 1 are considered perfect squares.
            /// </summary>
            /// <returns>True if the source is a perfect square</returns>
            bool IsPerfectSquare() { IN_CONTEXT(this); return mpz_perfect_square_p(context.Args[0]) != 0; }

            /// <summary>
            /// Returns the size of the absolute value of the number, measured in number of limbs.
            /// <para>If op is zero, the returned value will be zero.
            /// </para>Because this method returns a primitive type, it is computed immediately.
            /// </summary>
            /// <returns>The number of limbs used to represent the number</returns>
            size_t Size() { IN_CONTEXT(this); return mpz_size(context.Args[0]); }

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
            MpirExpression^ NextPrimeCandidate(MpirRandom^ random);

            /// <summary>Computes the greatest common divisor of this number and <paramref name="a"/>.
            /// <para>The result is always positive even if one or both inputs are negative.
            /// </para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </summary>
            /// <param name="a">Source value to compute the GCD with</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MpirGcdExpression^ Gcd(MpirExpression^ a);

            /// <summary>
            /// Computes the greatest common divisor of this number and <paramref name="a"/>.
            /// <para>The result is always positive even if the source number is negative.
            /// </para>Because the result is a primitive type, it is computed immediately.
            /// </summary>
            /// <param name="a">Source value to compute the GCD with.  If zero, zero is returned.</param>
            /// <returns>The greatest common divisor of the absolute value of this number and <paramref name="a"/>.</returns>
            mpir_ui Gcd(mpir_ui a) { IN_CONTEXT(this); return mpz_gcd_ui(nullptr, context.Args[0], a); }
                                                                                                          
            /// <summary>Computes the least common multiple of this number and <paramref name="a"/>.
            /// <para>The result is always positive, irrespective of the signs of the source numbers.
            /// </para>The result will be zero if either source number is zero.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to compute the LCM with.</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MpirExpression^ Lcm(MpirExpression^ a);
                                                                                                          
            /// <summary>Computes the least common multiple of this number and <paramref name="a"/>.
            /// <para>The result is always positive, irrespective of the signs of the source numbers.
            /// </para>The result will be zero if either source number is zero.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">Source value to compute the LCM with.</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MpirExpression^ Lcm(mpir_ui a);
                                                                                                          
            /// <summary>Computes the inverse of this number modulo <paramref name="modulo"/>.
            /// <para>If the inverse exists, the result will satisfy 0 &lt;= result &lt; <paramref name="modulo"/>.
            /// </para>If an inverse doesn't exist an ArgumentException is thrown.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="modulo">Modulo with respect to which to invert the number.</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MpirExpression^ Invert(MpirExpression^ modulo);

            /// <summary>Remove all occurrences of the <paramref name="factor"/> from the source number.
            /// <para>You can optionally save the number of such occurrences that were removed.
            /// </para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </summary>
            /// <param name="factor">Factor to remove from the source number.</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            MpirRemoveFactorsExpression^ RemoveFactors(MpirExpression^ factor);

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
            static HugeInt^ _toStringModulo;

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

    /// <summary>
    /// Expression that results from a Gcd method.  Allows to additionally compute Diophantine equation multiplier(s).
    /// </summary>
    public ref class MpirGcdExpression abstract : MpirExpression 
    {
        internal:
            MpirGcdExpression() { }
            HugeInt^ _s;
            HugeInt^ _t;
            void custom_mpz_gcd(mpz_ptr dest, mpz_srcptr a, mpz_srcptr b);

        public:
            /// <summary>
            /// Optionally computes and saves the coefficients <paramref name="s"/> and <paramref name="t"/> such that x*s + y*t = gcd(x, y).
            /// <para>If only one of the coefficients is needed, use null for the other.
            /// </para></summary>
            /// <param name="s">destination for the first coefficient. Can be null if not needed.</param>
            /// <param name="t">destination for the second coefficient. Can be null if not needed.</param>
            /// <returns>An updated expression, with its internal state updated to save the coefficients.</returns>
            MpirExpression^ SavingDiophantineMultipliersTo(HugeInt^ s, HugeInt^ t)
            {
                _s = s;
                _t = t;
                return this;
            }
    };

    /// <summary>
    /// Expression that results from a RemoveFactors method.  Allows to additionally save the number of factors that were removed.
    /// </summary>
    public ref class MpirRemoveFactorsExpression abstract : MpirExpression 
    {
        internal:
            MpirRemoveFactorsExpression() { }
            Action<mp_bitcnt_t>^ _count;

        public:
            /// <summary>
            /// Optionally gets the number of factors removed.
            /// </summary>
            /// <param name="callback">Delegate that will be called with the number of factors that were removed.
            /// The delegate is called when the root operation is evaluated, i.e. is assigned to the Value property or consumed by a method that returns a primitive type.</param>
            /// <returns>An updated expression, with its internal state updated to save the number of factors.</returns>
            MpirExpression^ SavingCountRemovedTo(Action<mp_bitcnt_t>^ callback)
            {
                _count = callback;
                return this;
            }
    };

    /// <summary>
    /// Expression that results from a method calculating a single number from a sequence, such as a fibonacci or lucas number.  Allows to save the previous number in addition to the requested one, so that the sequence can be continued.
    /// </summary>
    public ref class MpirSequenceExpression abstract : MpirExpression 
    {
        internal:
            MpirSequenceExpression() { }
            HugeInt^ _previous;

        public:
            /// <summary>
            /// Optionally save the previous number in the sequence to a separate result.  This cannot be the same object to which the expression is assigned.
            /// </summary>
            /// <param name="destination">destination for the previous number.  This cannot be the same object to which the expression is assigned.</param>
            /// <returns>An updated expression, with its internal state updated to additionally compute the previous number.</returns>
            MpirExpression^ SavingPreviousTo(HugeInt^ destination)
            {
                _previous = destination;
                return this;
            }
    };

    #pragma endregion

    #pragma region concrete expressions

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

    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MpirExpression, Invert, Int)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MpirExpression, NextPrimeCandidate, Int, Rnd)
    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MpirGcdExpression, Gcd, Int)
    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MpirRemoveFactorsExpression, RemoveFactors, Int)

    DEFINE_BINARY_EXPRESSION_WITH_TWO              (MpirExpression, Lcm, Int)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MpirExpression, Lcm, Int, Ui)

    DEFINE_BINARY_EXPRESSION_WITH_BUILT_INS_ONLY   (MpirExpression, Power, Ui, Ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_INS_ONLY   (MpirExpression, Factorial, Ui, Ui)
    DEFINE_UNARY_EXPRESSION_WITH_BUILT_INS_ONLY    (MpirExpression, Primorial, Ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_INS_ONLY   (MpirExpression, Binomial, Ui, Ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT   (MpirExpression, Binomial, Int, Ui)
    DEFINE_UNARY_EXPRESSION_WITH_BUILT_INS_ONLY    (MpirSequenceExpression, Fibonacci, Ui)
    DEFINE_UNARY_EXPRESSION_WITH_BUILT_INS_ONLY    (MpirSequenceExpression, Lucas, Ui)

    #pragma endregion

    #pragma region HugeInt class

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
                _value = (mpz_ptr)((*__gmp_allocate_func)(sizeof(__mpz_struct)));
            }
            void DeallocateStruct()
            {
                mpz_clear(_value);
                (*__gmp_free_func)(_value, sizeof(__mpz_struct));
                _value = nullptr;
            }
            void FromString(String^ value, int base);
            String^ ToString(int base, bool lowercase, int maxDigits);

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
            /// Initializes a new integer instance, allocating enough memory to hold at least <paramref name="bits"/> bits, and sets its value to 0.
            /// <para>This is only the initial space, integer will grow automatically in the normal way, if necessary, for subsequent values stored.
            /// </para>This makes it possible to avoid repeated reallocations if a maximum size is known in advance.
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
            /// Initializes a new integer instance and sets its value to the result of computing the source expression.
            /// </summary>
            /// <param name="value">the expression that will be computed, and the result set as the initial value of the new instance.</param>
            HugeInt(MpirExpression^ value);

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
            property MpirExpression^ Value
            {
                void set(MpirExpression^ expr) { expr->AssignTo(_value); }
                MpirExpression^ get() { return this; }
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

            #pragma region bit operations

            /// <summary>
            /// Sets a single bit at the specified position.
            /// </summary>
            /// <param name="value">Value of the bit to set, true for 1, false for 0</param>
            /// <param name="position">Position of the bit to set.
            /// <para>The least significant bit is zero.
            /// </para>If position is beyond the current size of the number, the number is extended automatically.</param>
            void SetBit(mp_bitcnt_t position, bool value) { value ? mpz_setbit(_value, position) : mpz_clrbit(_value, position); }

            /// <summary>
            /// Gets a single bit at the specified position.
            /// </summary>
            /// <param name="position">Position of the bit to get.
            /// <para>The least significant bit is zero.
            /// </para>If position is beyond the current size of the number, returns true for negative number, false for non-negative; the number itself is not extended.</param>
            /// <returns>true if the specified bit is 1, false if zero.
            /// <para>If position is beyond the current size of the number, returns true for negative number, false for non-negative; the number itself is not extended.</para></returns>
            bool GetBit(mp_bitcnt_t position) { return mpz_tstbit(_value, position) != 0; }

            /// <summary>
            /// Complements (inverts) a single bit at the specified position.
            /// </summary>
            /// <param name="position">Position of the bit to flip.
            /// <para>The least significant bit is zero.
            /// </para>If position is beyond the current size of the number, the number is extended automatically.</param>
            void ComplementBit(mp_bitcnt_t position) { mpz_combit(_value, position); }

            #pragma endregion

            #pragma region size checks

            /// <summary>
            /// Returns true if the value of the integer is in the ulong range.
            /// </summary>
            /// <returns>true if the value will fit in a ulong</returns>
            bool FitsUlong() { return mpz_fits_ui_p(_value) != 0; }

            /// <summary>
            /// Returns true if the value of the integer is in the long range.
            /// </summary>
            /// <returns>true if the value will fit in a long</returns>
            bool FitsLong() { return mpz_fits_si_p(_value) != 0; }

            /// <summary>
            /// Returns true if the value of the integer is in the uint range.
            /// </summary>
            /// <returns>true if the value will fit in a uint</returns>
            bool FitsUint() { return mpz_fits_uint_p(_value) != 0; }

            /// <summary>
            /// Returns true if the value of the integer is in the int range.
            /// </summary>
            /// <returns>true if the value will fit in a int</returns>
            bool FitsInt() { return mpz_fits_sint_p(_value) != 0; }

            /// <summary>
            /// Returns true if the value of the integer is in the ushort range.
            /// </summary>
            /// <returns>true if the value will fit in a ushort</returns>
            bool FitsUshort() { return mpz_fits_ushort_p(_value) != 0; }

            /// <summary>
            /// Returns true if the value of the integer is in the short range.
            /// </summary>
            /// <returns>true if the value will fit in a short</returns>
            bool FitsShort() { return mpz_fits_sshort_p(_value) != 0; }

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
            mp_size_t ApproximateSizeInBase(int base) { return mpz_sizeinbase(_value, base); }

            #pragma endregion

            #pragma region IO

            /// <summary>
            /// Outputs the integer to the <paramref name="stream"/> in raw binary format.
            /// <para>The number is written in a portable format, with 4 bytes of size information, and that many bytes of limbs.
            /// Both the size and the limbs are written in decreasing significance order (i.e., in big-endian).
            /// </para>The output can be read with Read(Stream).
            /// <para>The output cannot be read by mpz_inp_raw from GMP 1, because of changes necessary
            /// for compatibility between 32-bit and 64-bit machines.
            /// </para></summary>
            /// <param name="stream">Stream to output the number to</param>
            /// <returns>the number of bytes written, or 0 if an error occurs.</returns>
            size_t Write(Stream^ stream);

            /// <summary>
            /// Reads the integer value from the <paramref name="stream"/> in raw binary format, as it would have been written by Write(Stream).
            /// <para>The number is read in a portable format, with 4 bytes of size information, and that many bytes of limbs.
            /// Both the size and the limbs are written in decreasing significance order (i.e., in big-endian).
            /// </para>This routine can read the output from mpz_out_raw also from GMP 1, in spite of changes
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
                    mpz_set_ui(_value, 0);
                    return;
                }

                PIN(data);
                mpz_import(_value, limbCount, (int)limbOrder, bytesPerLimb, (int)endianness, nails, pinned_data);
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
                mpz_export(pinned_data, &limbCount, (int)limbOrder, bytesPerLimb, (int)endianness, nails, _value);
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
                auto limbCount = (mpz_sizeinbase(_value, 2) - 1) / bitsPerLimb + 1;
                auto arrayCount = (limbCount * bytesPerLimb - 1) / sizeof(T) + 1;
                auto data = gcnew array<T>(arrayCount);

                PIN(data);
                mpz_export(pinned_data, &limbCount, (int)limbOrder, bytesPerLimb, (int)endianness, nails, _value);
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
            size_t GetLimb(mp_size_t index) { return mpz_getlimbn(_value, index); }

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
            static int Jacobi(HugeInt^ a, HugeInt^ b) { return mpz_jacobi(a->_value, b->_value); }

            /// <summary>
            /// Calculates the Legendre symbol (<paramref name="a"/>/<paramref name="b"/>).
            /// <para>This is defined only when <paramref name="b"/> is an odd prime.
            /// </para></summary>
            /// <param name="a">First source value for the Legendre symbol</param>
            /// <param name="b">Second source value for the Legendre symbol</param>
            /// <returns>The Legendre symbol (-1, 0, or 1).  Return is undefined unless <paramref name="b"/> is an odd prime.</returns>
            static int Legendre(HugeInt^ a, HugeInt^ b) { return mpz_legendre(a->_value, b->_value); }

            /// <summary>
            /// Calculates the Jacobi symbol (<paramref name="a"/>/<paramref name="b"/>) with the Kronecker extension
            /// (<paramref name="a"/>/2) = (2/<paramref name="a"/>) when a odd, or (<paramref name="a"/>/2) = 0 when a even.
            /// <para>When <paramref name="b"/> is odd the Jacobi symbol and Kronecker symbol are identical, so the various Kronecker overloads 
            /// can be used for mixed precision Jacobi symbols too.
            /// </para></summary>
            /// <param name="a">First source value for the Kronecker symbol</param>
            /// <param name="b">Second source value for the Kronecker symbol</param>
            /// <returns>The Kronecker symbol (-1, 0, or 1).</returns>
            static int Kronecker(HugeInt^ a, HugeInt^ b) { return mpz_kronecker(a->_value, b->_value); }

            /// <summary>
            /// Calculates the Jacobi symbol (<paramref name="a"/>/<paramref name="b"/>) with the Kronecker extension
            /// (<paramref name="a"/>/2) = (2/<paramref name="a"/>) when a odd, or (<paramref name="a"/>/2) = 0 when a even.
            /// <para>When <paramref name="b"/> is odd the Jacobi symbol and Kronecker symbol are identical, so the various Kronecker overloads 
            /// can be used for mixed precision Jacobi symbols too.
            /// </para></summary>
            /// <param name="a">First source value for the Kronecker symbol</param>
            /// <param name="b">Second source value for the Kronecker symbol</param>
            /// <returns>The Kronecker symbol (-1, 0, or 1).</returns>
            static int Kronecker(HugeInt^ a, mpir_ui b) { return mpz_kronecker_ui(a->_value, b); }

            /// <summary>
            /// Calculates the Jacobi symbol (<paramref name="a"/>/<paramref name="b"/>) with the Kronecker extension
            /// (<paramref name="a"/>/2) = (2/<paramref name="a"/>) when a odd, or (<paramref name="a"/>/2) = 0 when a even.
            /// <para>When <paramref name="b"/> is odd the Jacobi symbol and Kronecker symbol are identical, so the various Kronecker overloads 
            /// can be used for mixed precision Jacobi symbols too.
            /// </para></summary>
            /// <param name="a">First source value for the Kronecker symbol</param>
            /// <param name="b">Second source value for the Kronecker symbol</param>
            /// <returns>The Kronecker symbol (-1, 0, or 1).</returns>
            static int Kronecker(HugeInt^ a, mpir_si b) { return mpz_kronecker_si(a->_value, b); }

            /// <summary>
            /// Calculates the Jacobi symbol (<paramref name="a"/>/<paramref name="b"/>) with the Kronecker extension
            /// (<paramref name="a"/>/2) = (2/<paramref name="a"/>) when a odd, or (<paramref name="a"/>/2) = 0 when a even.
            /// <para>When <paramref name="b"/> is odd the Jacobi symbol and Kronecker symbol are identical, so the various Kronecker overloads 
            /// can be used for mixed precision Jacobi symbols too.
            /// </para></summary>
            /// <param name="a">First source value for the Kronecker symbol</param>
            /// <param name="b">Second source value for the Kronecker symbol</param>
            /// <returns>The Kronecker symbol (-1, 0, or 1).</returns>
            static int Kronecker(mpir_ui a, HugeInt^ b) { return mpz_ui_kronecker(a, b->_value); }

            /// <summary>
            /// Calculates the Jacobi symbol (<paramref name="a"/>/<paramref name="b"/>) with the Kronecker extension
            /// (<paramref name="a"/>/2) = (2/<paramref name="a"/>) when a odd, or (<paramref name="a"/>/2) = 0 when a even.
            /// <para>When <paramref name="b"/> is odd the Jacobi symbol and Kronecker symbol are identical, so the various Kronecker overloads 
            /// can be used for mixed precision Jacobi symbols too.
            /// </para></summary>
            /// <param name="a">First source value for the Kronecker symbol</param>
            /// <param name="b">Second source value for the Kronecker symbol</param>
            /// <returns>The Kronecker symbol (-1, 0, or 1).</returns>
            static int Kronecker(mpir_si a, HugeInt^ b) { return mpz_si_kronecker(a, b->_value); }

            /// <summary>
            /// Returns an expression for calculating <paramref name="value"/> raised to the specified <paramref name="power"/>.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="value">Base for the initial value for the new integer instance</param>
            /// <param name="power">Power to raise the <paramref name="value"/> to when calculating the initial value for the new instance</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ Power(mpir_ui value, mpir_ui power) { return gcnew MpirPowerUiUiExpression(value, power); }

            /// <summary>
            /// Returns an expression for calculating the factorial of <paramref name="a"/>.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">The source number to take the factorial of</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ Factorial(mpir_ui a) { return gcnew MpirFactorialUiUiExpression(a, 1); }

            /// <summary>
            /// Returns an expression for calculating the multifactorial of <paramref name="a"/> of the specified order.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">The source number to take the multifactorial of</param>
            /// <param name="order">The order of the multifactorial, i.e. 2 for <paramref name="a"/>!!, 3 for <paramref name="a"/>!!!, etc.</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ Factorial(mpir_ui a, mpir_ui order) { return gcnew MpirFactorialUiUiExpression(a, order); }

            /// <summary>
            /// Returns an expression for calculating the primorial of <paramref name="a"/>, i.e. the product of all positive prime numbers &lt;= a.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="a">The source number to take the primorial of</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ Primorial(mpir_ui a) { return gcnew MpirPrimorialUiExpression(a); }

            /// <summary>
            /// Returns an expression for calculating the binomial coefficient (<paramref name="n"/>, <paramref name="k"/>), a.k.a. number of k-element combinations out of an n-element set.
            /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
            /// </para></summary>
            /// <param name="n">The first source value of the binomial coefficient, a.k.a. set size</param>
            /// <param name="k">The second source value of the binomial coefficient, a.k.a. subset size</param>
            /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, computes the requested operation</returns>
            static MpirExpression^ Binomial(mpir_ui n, mpir_ui k) { return gcnew MpirBinomialUiUiExpression(n, k); }

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
            static MpirExpression^ Binomial(MpirExpression^ n, mpir_ui k) { return gcnew MpirBinomialIntUiExpression(n, k); }

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
            static MpirSequenceExpression^ Fibonacci(mpir_ui n) { return gcnew MpirFibonacciUiExpression(n); }

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
            static MpirSequenceExpression^ Lucas(mpir_ui n) { return gcnew MpirLucasUiExpression(n); }

            #pragma endregion
    };

    #pragma endregion
};