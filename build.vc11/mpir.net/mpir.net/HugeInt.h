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

//defines a unary expression class
#define DEFINE_UNARY_EXPRESSION(name, type)                      \
public ref class Mpir##name##Expression : BASE_EXPRESSION        \
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
#define DEFINE_BINARY_EXPRESSION(name, leftType, rightType)      \
public ref class Mpir##name##Expression : BASE_EXPRESSION        \
{                                                                \
    internal:                                                    \
        leftType Left;                                           \
        rightType Right;                                         \
        virtual void AssignTo(mpz_ptr destination) override;     \
                                                                 \
    public:                                                      \
        Mpir##name##Expression(leftType left, rightType right)   \
        {                                                        \
            Left = left;                                         \
            Right = right;                                       \
        }                                                        \
};

#define TYPE_FOR_ABBR_Int HugeInt^
#define TYPE_FOR_ABBR_Expr MpirExpression^
#define TYPE_FOR_ABBR_Si mpir_si
#define TYPE_FOR_ABBR_Ui mpir_ui
#define TYPE_FOR_ABBR_Bits mp_bitcnt_t

//unary expressions
#define DEFINE_UNARY_EXPRESSION_WITH_ONE(name, typeAbbr, type) \
    DEFINE_UNARY_EXPRESSION(name##typeAbbr, type)              \
    DEFINE_UNARY_EXPRESSION(name##Expr, MpirExpression^)           

//binary expressions
#define DEFINE_BINARY_EXPRESSION_WITH_TWO(name, typeAbbr, type)                     \
    DEFINE_BINARY_EXPRESSION(name##typeAbbr##typeAbbr, type, type)                  \
    DEFINE_BINARY_EXPRESSION(name##typeAbbr##Expr, type, MpirExpression^)           \
    DEFINE_BINARY_EXPRESSION(name##Expr##typeAbbr, MpirExpression^, type)           \
    DEFINE_BINARY_EXPRESSION(name##ExprExpr, MpirExpression^, MpirExpression^)    

#define DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(name, leftTypeAbbr, rightTypeAbbr, leftType, rightType)    \
    DEFINE_BINARY_EXPRESSION(name##leftTypeAbbr##rightTypeAbbr, leftType, rightType)                            \
    DEFINE_BINARY_EXPRESSION(name##Expr##rightTypeAbbr, MpirExpression^, rightType)                            

#define DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT(name, leftTypeAbbr, rightTypeAbbr, leftType, rightType)    \
    DEFINE_BINARY_EXPRESSION(name##leftTypeAbbr##rightTypeAbbr, leftType, rightType)                           \
    DEFINE_BINARY_EXPRESSION(name##leftTypeAbbr##Expr, leftType, MpirExpression^)                            
    
#define DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT_OR_RIGHT(name, leftTypeAbbr, rightTypeAbbr, leftType, rightType)    \
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(name, leftTypeAbbr, rightTypeAbbr, leftType, rightType)                \
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT(name, rightTypeAbbr, leftTypeAbbr, rightType, leftType)

//void functions
#define MAKE_VOID_FUNCTION(action, kind, op, type)    MAKE_VOID_FUNCTION_##kind(action, op, type)
#define MAKE_VOID_FUNCTION_Int(action, op, type)      MAKE_VOID_FUNCTION_##action(op, op##type)
#define MAKE_VOID_FUNCTION_Expr(action, op, type)     MAKE_VOID_FUNCTION_##action(op, op##Expr)

#define MAKE_VOID_FUNCTION_MAKE(op, result)   \
    MpirExpression^ op() { return gcnew Mpir##result##Expression(this); }

#define MAKE_VOID_FUNCTION_DECLARE(op, result)     \
    MpirExpression^ op();

#define MAKE_VOID_FUNCTION_DEFINE(op, result)      \
    MpirExpression^ MpirExpression::op() { return gcnew Mpir##result##Expression(this); }

//unary operators
#define MAKE_UNARY_OPERATOR(action, kind, op, result, type)   MAKE_UNARY_OPERATOR_##kind(action, op, result, type)
#define MAKE_UNARY_OPERATOR_Int(action, op, result, type)     MAKE_UNARY_OPERATOR_##action(op, result##type, type)
#define MAKE_UNARY_OPERATOR_Expr(action, op, result, mpType)  MAKE_UNARY_OPERATOR_##action(op, result##Expr, Expr)

#define MAKE_UNARY_OPERATOR_MAKE(op, result, type)   \
    static BASE_EXPRESSION^ operator op(TYPE_FOR_ABBR_##type a) { return gcnew Mpir##result##Expression(a); }

#define MAKE_UNARY_OPERATOR_DECLARE(op, result, type)     \
    static BASE_EXPRESSION^ operator op(TYPE_FOR_ABBR_##type a);

#define MAKE_UNARY_OPERATOR_DEFINE(op, result, type)      \
    BASE_EXPRESSION^ MpirExpression::operator op(TYPE_FOR_ABBR_##type a) { return gcnew Mpir##result##Expression(a); }

//binary operators
#define MAKE_BINARY_OPERATOR_MAKE(op, result, leftType, rightType, left, right)       \
    static BASE_EXPRESSION^ operator op(TYPE_FOR_ABBR_##leftType a, TYPE_FOR_ABBR_##rightType b) { return gcnew Mpir##result##Expression(left, right); }

#define MAKE_BINARY_OPERATOR_DECLARE(op, result, leftType, rightType, left, right)     \
    static BASE_EXPRESSION^ operator op(TYPE_FOR_ABBR_##leftType a, TYPE_FOR_ABBR_##rightType b);

#define MAKE_BINARY_OPERATOR_DEFINE(op, result, leftType, rightType, left, right)      \
    BASE_EXPRESSION^ MpirExpression::operator op(TYPE_FOR_ABBR_##leftType a, TYPE_FOR_ABBR_##rightType b) { return gcnew Mpir##result##Expression(left, right); }

#define MAKE_BINARY_OPERATOR_STANDARD(action, kind, op, result, leftType, rightType)   \
    MAKE_BINARY_OPERATOR_STANDARD_##kind(action, op, result, leftType, rightType)

#define MAKE_BINARY_OPERATOR_SIMPLE(action, kind, op, result, mpType, limbType)     \
    MAKE_BINARY_OPERATOR_SIMPLE_##kind(action, op, result, mpType, limbType)

#define MAKE_BINARY_OPERATOR_RLIMB(action, kind, op, result, mpType, limbType)      \
    MAKE_BINARY_OPERATOR_RLIMB_##kind(action, op, result, mpType, limbType)

#define MAKE_BINARY_OPERATOR_LLIMB(action, kind, op, result, mpType, limbType)      \
    MAKE_BINARY_OPERATOR_LLIMB_##kind(action, op, result, limbType, mpType)

#define MAKE_BINARY_OPERATOR_LLIMB_R(action, kind, op, result, mpType, limbType)      \
    MAKE_BINARY_OPERATOR_LLIMB_R_##kind(action, op, result, limbType, mpType)

#define MAKE_BINARY_OPERATOR_STANDARD_Int(action, op, result, leftType, rightType) \
    MAKE_BINARY_OPERATOR_##action(op, result##leftType##rightType, leftType, rightType, a, b)

#define MAKE_BINARY_OPERATOR_SIMPLE_Int   MAKE_BINARY_OPERATOR_STANDARD_Int
#define MAKE_BINARY_OPERATOR_RLIMB_Int    MAKE_BINARY_OPERATOR_STANDARD_Int
#define MAKE_BINARY_OPERATOR_LLIMB_Int    MAKE_BINARY_OPERATOR_STANDARD_Int

#define MAKE_BINARY_OPERATOR_LLIMB_R_Int(action, op, result, limbType, mpType) \
    MAKE_BINARY_OPERATOR_##action(op, result##mpType##limbType, limbType, mpType, b, a)

#define MAKE_BINARY_OPERATOR_STANDARD_Expr(action, op, result, leftType, rightType)        \
    MAKE_BINARY_OPERATOR_##action(op, result##Expr##rightType, Expr, rightType, a, b)      \
    MAKE_BINARY_OPERATOR_##action(op, result##leftType##Expr, leftType, Expr, a, b)        \
    MAKE_BINARY_OPERATOR_##action(op, result##Expr##Expr, Expr, Expr, a, b)               

#define MAKE_BINARY_OPERATOR_RLIMB_Expr(action, op, result, mpType, limbType)  \
    MAKE_BINARY_OPERATOR_##action(op, result##Expr##limbType, Expr, limbType, a, b)          

#define MAKE_BINARY_OPERATOR_LLIMB_Expr(action, op, result, limbType, mpType)  \
    MAKE_BINARY_OPERATOR_##action(op, result##limbType##Expr, limbType, Expr, a, b)           

#define MAKE_BINARY_OPERATOR_LLIMB_R_Expr(action, op, result, limbType, mpType) \
    MAKE_BINARY_OPERATOR_##action(op, result##Expr##limbType, limbType, Expr, b, a)

#define MAKE_BINARY_OPERATOR_SIMPLE_Expr(action, op, result, mpType, limbType)  \
    MAKE_BINARY_OPERATOR_##action(op, result##Expr##limbType, Expr, limbType, a, b)          

//master operators/functions definition
#define DEFINE_OPERATIONS(action, kind)                                       \
    MAKE_BINARY_OPERATOR_STANDARD  (action, kind, +, Add, Int, Int)           \
    MAKE_BINARY_OPERATOR_RLIMB     (action, kind, +, Add, Int, Ui)            \
    MAKE_BINARY_OPERATOR_LLIMB_R   (action, kind, +, Add, Int, Ui)            \
    MAKE_BINARY_OPERATOR_RLIMB     (action, kind, +, Add, Int, Si)            \
    MAKE_BINARY_OPERATOR_LLIMB_R   (action, kind, +, Add, Int, Si)            \
                                                                              \
    MAKE_BINARY_OPERATOR_STANDARD  (action, kind, -, Subtract, Int, Int)      \
    MAKE_BINARY_OPERATOR_RLIMB     (action, kind, -, Subtract, Int, Ui)       \
    MAKE_BINARY_OPERATOR_LLIMB     (action, kind, -, Subtract, Int, Ui)       \
    MAKE_BINARY_OPERATOR_RLIMB     (action, kind, -, Subtract, Int, Si)       \
    MAKE_BINARY_OPERATOR_LLIMB     (action, kind, -, Subtract, Int, Si)       \
                                                                              \
    MAKE_BINARY_OPERATOR_STANDARD  (action, kind, *, Multiply, Int, Int)      \
    MAKE_BINARY_OPERATOR_RLIMB     (action, kind, *, Multiply, Int, Ui)       \
    MAKE_BINARY_OPERATOR_LLIMB_R   (action, kind, *, Multiply, Int, Ui)       \
    MAKE_BINARY_OPERATOR_RLIMB     (action, kind, *, Multiply, Int, Si)       \
    MAKE_BINARY_OPERATOR_LLIMB_R   (action, kind, *, Multiply, Int, Si)       \
                                                                              \
    MAKE_BINARY_OPERATOR_SIMPLE    (action, kind, <<, ShiftLeft, Int, Bits)   \
                                                                              \
    MAKE_UNARY_OPERATOR(action, kind, -, Negate, Int)                         \
                                                                              \
    MAKE_VOID_FUNCTION(action, kind, Abs, Int)                                \

#define DEFINE_DIV_OPERATIONS(action, kind)                                   \
    MAKE_BINARY_OPERATOR_STANDARD  (action, kind, /, Divide, Int, Int)        \
    /*MAKE_LIMB_OPERATOR_RLIMB       (action, kind, /, Divide, Int, Ui)    */     \

#define DEFINE_MOD_OPERATIONS(action, kind)                                   \
    MAKE_BINARY_OPERATOR_STANDARD  (action, kind, %, Mod, Int, Int)           \
    /*MAKE_LIMB_OPERATOR_RLIMB       (action, kind, %, Mod, Int, Ui)       */     \

namespace MPIR
{
    ref class HugeInt;
    ref class MpirDivideExpression;
    ref class MpirModExpression;

    public ref class MpirExpression abstract
    {
        internal:
            virtual void AssignTo(mpz_ptr destination) abstract;

        public:
#define BASE_EXPRESSION MpirExpression
            DEFINE_OPERATIONS(DECLARE, Expr)
#define BASE_EXPRESSION MpirDivideExpression
            DEFINE_DIV_OPERATIONS(DECLARE, Expr)
#define BASE_EXPRESSION MpirModExpression
            DEFINE_MOD_OPERATIONS(DECLARE, Expr)
    };

    public enum class RoundingModes
    {
        Default,
        Truncate,
        Ceiling,
        Floor,
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
        private:
            HugeInt^ remainder;

        protected:
            void custom_mpz_div(mpz_ptr q, mpz_srcptr n, mpz_srcptr d);

        public:
            MpirDivModExpression^ SavingRemainderTo(HugeInt^ destination)
            {
                remainder = destination;
                return this;
            }
    };

    public ref class MpirModExpression abstract : MpirDivModExpression 
    {
        private:
            HugeInt^ quotient;
        
        protected:
            void custom_mpz_mod(mpz_ptr r, mpz_srcptr n, mpz_srcptr d);

        public:
            MpirDivModExpression^ SavingQuotientTo(HugeInt^ destination)
            {
                quotient = destination;
                return this;
            }
    };

#define BASE_EXPRESSION MpirExpression
    DEFINE_BINARY_EXPRESSION_WITH_TWO(Add, Int, HugeInt^)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(Add, Int, Ui, HugeInt^, mpir_ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(Add, Int, Si, HugeInt^, mpir_si)

    DEFINE_BINARY_EXPRESSION_WITH_TWO(Subtract, Int, HugeInt^)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT_OR_RIGHT(Subtract, Int, Ui, HugeInt^, mpir_ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT_OR_RIGHT(Subtract, Int, Si, HugeInt^, mpir_si)

    DEFINE_BINARY_EXPRESSION_WITH_TWO(Multiply, Int, HugeInt^)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(Multiply, Int, Ui, HugeInt^, mpir_ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(Multiply, Int, Si, HugeInt^, mpir_si)

    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(ShiftLeft, Int, Bits, HugeInt^, mp_bitcnt_t)
    
    DEFINE_UNARY_EXPRESSION_WITH_ONE(Negate, Int, HugeInt^)
    DEFINE_UNARY_EXPRESSION_WITH_ONE(Abs, Int, HugeInt^)

#define BASE_EXPRESSION MpirDivideExpression
    DEFINE_BINARY_EXPRESSION_WITH_TWO(Divide, Int, HugeInt^)
    //DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(Divide, Int, Ui, HugeInt^, mpir_ui)

#define BASE_EXPRESSION MpirModExpression
    DEFINE_BINARY_EXPRESSION_WITH_TWO(Mod, Int, HugeInt^)

#define BASE_EXPRESSION MpirExpression
            DEFINE_OPERATIONS(DEFINE, Expr)
#define BASE_EXPRESSION MpirDivideExpression
            DEFINE_DIV_OPERATIONS(DEFINE, Expr)
#define BASE_EXPRESSION MpirModExpression
            DEFINE_MOD_OPERATIONS(DEFINE, Expr)

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
            //HugeInt(mpz_srcptr src);
            void FromString(String^ value, int base);

        internal:
            virtual void AssignTo(mpz_ptr destination) override
            {
                if(destination != _value)
                    mpz_set(destination, _value);
            }

        public:
            //construction
            HugeInt();
            HugeInt(mp_bitcnt_t bits);
            HugeInt(String^ value);
            HugeInt(String^ value, int base);
            static HugeInt^ FromLong(mpir_si value);
            static HugeInt^ FromUlong(mpir_ui value);

            //disposal
            ~HugeInt();
            !HugeInt();

            //object overrides
            virtual String^ ToString() override;
            String^ ToString(int base);

            //properties
            property MpirExpression^ Value
            {
                void set(MpirExpression^ expr);
            }

            //arithmetic
#define BASE_EXPRESSION MpirExpression
            DEFINE_OPERATIONS(MAKE, Int)
#define BASE_EXPRESSION MpirDivideExpression
            DEFINE_DIV_OPERATIONS(MAKE, Int)
#define BASE_EXPRESSION MpirModExpression
            DEFINE_MOD_OPERATIONS(MAKE, Int)
    };
};