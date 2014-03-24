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

//defines a unary expression class
#define DEFINE_UNARY_EXPRESSION(base, name, type)                \
public ref class Mpir##name##Expression : base                   \
{                                                                \
    internal:                                                    \
        type Operand;                                            \
        virtual void AssignTo(mpz_ptr destination) override;     \
                                                                 \
    public:                                                      \
        Mpir##name##Expression(type operand)                     \
        {                                                        \
            Operand = operand;                                   \
        }                                                        \
};

//defines a binary expression class
#define DEFINE_BINARY_EXPRESSION(base, name, leftType, rightType) \
public ref class Mpir##name##Expression : base                    \
{                                                                 \
    internal:                                                     \
        leftType Left;                                            \
        rightType Right;                                          \
        virtual void AssignTo(mpz_ptr destination) override;      \
                                                                  \
    public:                                                       \
        Mpir##name##Expression(leftType left, rightType right)    \
        {                                                         \
            Left = left;                                          \
            Right = right;                                        \
        }                                                         \
};

#define TYPE_FOR_ABBR_Int HugeInt^
#define TYPE_FOR_ABBR_Expr MpirExpression^
#define TYPE_FOR_ABBR_Si mpir_si
#define TYPE_FOR_ABBR_Ui mpir_ui
#define TYPE_FOR_ABBR_Bits mp_bitcnt_t

//unary expressions
#define DEFINE_UNARY_EXPRESSION_WITH_ONE(base, name, typeAbbr, type) \
    DEFINE_UNARY_EXPRESSION(base, name##typeAbbr, MpirExpression^)           

//binary expressions
#define DEFINE_BINARY_EXPRESSION_WITH_TWO(base, name, typeAbbr, type) \
    DEFINE_BINARY_EXPRESSION(base, name##typeAbbr##typeAbbr, MpirExpression^, MpirExpression^)

#define DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(base, name, leftTypeAbbr, rightTypeAbbr, leftType, rightType)    \
    DEFINE_BINARY_EXPRESSION(base, name##leftTypeAbbr##rightTypeAbbr, MpirExpression^, rightType) 

#define DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT(base, name, leftTypeAbbr, rightTypeAbbr, leftType, rightType)     \
    DEFINE_BINARY_EXPRESSION(base, name##leftTypeAbbr##rightTypeAbbr, leftType, MpirExpression^)
    
#define DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT_OR_RIGHT(base, name, leftTypeAbbr, rightTypeAbbr, leftType, rightType)    \
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(base, name, leftTypeAbbr, rightTypeAbbr, leftType, rightType)                \
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT(base, name, rightTypeAbbr, leftTypeAbbr, rightType, leftType)

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
#define DEFINE_OPERATIONS(action)                                                       \
    MAKE_BINARY_OPERATOR_STANDARD  (MpirExpression, action, +, Add, Int, Int)           \
    MAKE_BINARY_OPERATOR_RLIMB     (MpirExpression, action, +, Add, Int, Ui)            \
    MAKE_BINARY_OPERATOR_LLIMB_R   (MpirExpression, action, +, Add, Int, Ui)            \
    MAKE_BINARY_OPERATOR_RLIMB     (MpirExpression, action, +, Add, Int, Si)            \
    MAKE_BINARY_OPERATOR_LLIMB_R   (MpirExpression, action, +, Add, Int, Si)            \
                                                                                        \
    MAKE_BINARY_OPERATOR_STANDARD  (MpirExpression, action, -, Subtract, Int, Int)      \
    MAKE_BINARY_OPERATOR_RLIMB     (MpirExpression, action, -, Subtract, Int, Ui)       \
    MAKE_BINARY_OPERATOR_LLIMB     (MpirExpression, action, -, Subtract, Int, Ui)       \
    MAKE_BINARY_OPERATOR_RLIMB     (MpirExpression, action, -, Subtract, Int, Si)       \
    MAKE_BINARY_OPERATOR_LLIMB     (MpirExpression, action, -, Subtract, Int, Si)       \
                                                                                        \
    MAKE_BINARY_OPERATOR_STANDARD  (MpirExpression, action, *, Multiply, Int, Int)      \
    MAKE_BINARY_OPERATOR_RLIMB     (MpirExpression, action, *, Multiply, Int, Ui)       \
    MAKE_BINARY_OPERATOR_LLIMB_R   (MpirExpression, action, *, Multiply, Int, Ui)       \
    MAKE_BINARY_OPERATOR_RLIMB     (MpirExpression, action, *, Multiply, Int, Si)       \
    MAKE_BINARY_OPERATOR_LLIMB_R   (MpirExpression, action, *, Multiply, Int, Si)       \
                                                                                        \
    MAKE_BINARY_OPERATOR_RLIMB     (MpirExpression, action, <<, ShiftLeft, Int, Bits)   \
                                                                                        \
    MAKE_UNARY_OPERATOR(MpirExpression, action, -, Negate, Int)                         \
                                                                                        \
    MAKE_VOID_FUNCTION(MpirExpression, action, Abs, Int)                                \
                                                                                        \
    MAKE_BINARY_OPERATOR_STANDARD  (MpirDivideExpression, action, /, Divide, Int, Int)  \
    MAKE_BINARY_OPERATOR_RLIMB     (MpirDivideUiExpression, action, /, Divide, Int, Ui) \
                                                                                        \
    MAKE_BINARY_OPERATOR_STANDARD  (MpirModExpression, action, %, Mod, Int, Int)        \
    MAKE_BINARY_OPERATOR_RLIMB     (MpirModUiExpression, action, %, Mod, Int, Ui)       \
                                                                                        \
    MAKE_FUNCTION_WITH_ONE         (MpirExpression, action, DivideExactly, Int)         \
    MAKE_FUNCTION_WITH_LIMB        (MpirExpression, action, DivideExactly, Ui)          \

namespace MPIR
{
    ref class HugeInt;
    ref class MpirDivideExpression;
    ref class MpirDivideUiExpression;
    ref class MpirModExpression;
    ref class MpirModUiExpression;

    public enum class RoundingModes
    {
        Default,
        Truncate,
        Ceiling,
        Floor,
    };

    public ref class MpirExpression abstract
    {
        internal:
            virtual void AssignTo(mpz_ptr destination) abstract;
            virtual void AssignTo(EvaluationContext& destination)
            {
                destination.Options = (EvaluationOptions) (destination.Options | (1 << destination.Index));
                auto ptr = &destination.Temp[destination.Index];
                destination.Args[destination.Index++] = ptr;
                mpz_init(ptr);
                AssignTo(ptr); 
            }

        public:
            DEFINE_OPERATIONS(DECLARE)

            mpir_ui Mod(mpir_ui a) { return Mod(a, RoundingModes::Default); }
            mpir_ui Mod(mpir_ui a, RoundingModes roundingMode);
    };

    static public ref class MpirSettings
    {
        public:
            static property RoundingModes RoundingMode;

            static MpirSettings()
            {
                RoundingMode = RoundingModes::Truncate;
            }
    };

    public ref class MpirDivModExpression abstract : MpirExpression 
    {
        protected:
            RoundingModes rounding;

        public:
            MpirExpression^ Rounding(RoundingModes mode)
            {
                rounding = mode;
                return this;
            }
    };

    public ref class MpirDivideExpression abstract : MpirDivModExpression 
    {
        internal:
            HugeInt^ _remainder;
            void custom_mpz_div(mpz_ptr q, mpz_srcptr n, mpz_srcptr d);

        public:
            MpirDivModExpression^ SavingRemainderTo(HugeInt^ destination)
            {
                _remainder = destination;
                return this;
            }
    };

    public ref class MpirDivideUiExpression abstract : MpirDivideExpression 
    {
        private:
            Action<mpir_ui>^ _limbRemainder;

        internal:
            void custom_mpz_div_ui(mpz_ptr q, mpz_srcptr n, mpir_ui d);

        public:
            MpirDivideExpression^ SettingRemainderTo(Action<mpir_ui>^ callback)
            {
                _limbRemainder = callback;
                return this;
            }
    };

    public ref class MpirModExpression abstract : MpirDivModExpression 
    {
        internal:
            HugeInt^ _quotient;
            void custom_mpz_mod(mpz_ptr r, mpz_srcptr n, mpz_srcptr d);

        public:
            MpirDivModExpression^ SavingQuotientTo(HugeInt^ destination)
            {
                _quotient = destination;
                return this;
            }
    };

    public ref class MpirModUiExpression abstract : MpirModExpression 
    {
        private:
            Action<mpir_ui>^ _limbRemainder;

        internal:
            void custom_mpz_mod_ui(mpz_ptr r, mpz_srcptr n, mpir_ui d);

        public:
            MpirModExpression^ SettingRemainderTo(Action<mpir_ui>^ callback)
            {
                _limbRemainder = callback;
                return this;
            }
    };

    DEFINE_BINARY_EXPRESSION_WITH_TWO           (MpirExpression, Add, Int, HugeInt^)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(MpirExpression, Add, Int, Ui, HugeInt^, mpir_ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(MpirExpression, Add, Int, Si, HugeInt^, mpir_si)

    DEFINE_BINARY_EXPRESSION_WITH_TWO                   (MpirExpression, Subtract, Int, HugeInt^)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT_OR_RIGHT(MpirExpression, Subtract, Int, Ui, HugeInt^, mpir_ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT_OR_RIGHT(MpirExpression, Subtract, Int, Si, HugeInt^, mpir_si)

    DEFINE_BINARY_EXPRESSION_WITH_TWO           (MpirExpression, Multiply, Int, HugeInt^)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(MpirExpression, Multiply, Int, Ui, HugeInt^, mpir_ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(MpirExpression, Multiply, Int, Si, HugeInt^, mpir_si)

    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(MpirExpression, ShiftLeft, Int, Bits, HugeInt^, mp_bitcnt_t)
    
    DEFINE_UNARY_EXPRESSION_WITH_ONE(MpirExpression, Negate, Int, HugeInt^)
    DEFINE_UNARY_EXPRESSION_WITH_ONE(MpirExpression, Abs, Int, HugeInt^)

    DEFINE_BINARY_EXPRESSION_WITH_TWO           (MpirDivideExpression, Divide, Int, HugeInt^)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(MpirDivideUiExpression, Divide, Int, Ui, HugeInt^, mpir_ui)

    DEFINE_BINARY_EXPRESSION_WITH_TWO(MpirModExpression, Mod, Int, HugeInt^)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(MpirModUiExpression, Mod, Int, Ui, HugeInt^, mpir_ui)

    DEFINE_BINARY_EXPRESSION_WITH_TWO           (MpirExpression, DivideExactly, Int, HugeInt^)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(MpirExpression, DivideExactly, Int, Ui, HugeInt^, mpir_ui)

    DEFINE_OPERATIONS(DEFINE)

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
            //construction
            HugeInt();
            HugeInt(mp_bitcnt_t bits);
            HugeInt(String^ value) { FromString(value, 10); }
            HugeInt(String^ value, int base) { FromString(value, base); }
            static HugeInt^ FromLong(mpir_si value);
            static HugeInt^ FromUlong(mpir_ui value);
            static HugeInt^ FromDouble(double value);
            void Reallocate(mp_bitcnt_t bits) { mpz_realloc2(_value, bits); }

            //disposal
            ~HugeInt();
            !HugeInt();

            //object overrides
            virtual String^ ToString() override;
            String^ ToString(int base);

            //assignment
            property MpirExpression^ Value
            {
                void set(MpirExpression^ expr) { expr->AssignTo(_value); }
            }

            //conversions
            mpir_ui ToUlong() { return mpz_get_ui(_value); }
            mpir_si ToLong() { return mpz_get_si(_value); }
            double ToDouble() { return mpz_get_d(_value); }
            double ToDouble([Out] mpir_si% exp) 
            { 
                mpir_si x; 
                auto result = mpz_get_d_2exp(&x, _value); 
                exp = x; 
                return result; 
            }

            void SetTo(mpir_ui value) { mpz_set_ui(_value, value); }
            void SetTo(mpir_si value) { mpz_set_si(_value, value); }
            void SetTo(double value) { mpz_set_d(_value, value); }
            void SetTo(String^ value) { SetTo(value, 10); }
            void SetTo(String^ value, int base);

            //arithmetic
            bool IsDivisibleBy(HugeInt^ a) { return mpz_divisible_p(_value, a->_value) != 0; }
            bool IsDivisibleBy(mpir_ui a) { return mpz_divisible_ui_p(_value, a) != 0; }
            bool IsDivisibleByPowerOf2(mp_bitcnt_t bits) { return mpz_divisible_2exp_p(_value, bits) != 0; }

            //utility methods
            void Swap(HugeInt^ a) 
            { 
                mpz_ptr temp = a->_value;
                a->_value = _value;
                _value = temp; 
            }
    };
};