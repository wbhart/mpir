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

#pragma region Expression macros

#ifdef ADDITIONAL_MEMBERS 
#undef ADDITIONAL_MEMBERS
#endif
#define ADDITIONAL_MEMBERS

#define IN_CONTEXT_1(a)                \
    a->ASSIGN_TO(context)

#define IN_CONTEXT_2(a, b)             \
    a->ASSIGN_TO(context);             \
    b->ASSIGN_TO(context)

#define IN_CONTEXT_3(a, b, c)          \
    a->ASSIGN_TO(context);             \
    b->ASSIGN_TO(context);             \
    c->ASSIGN_TO(context)

#define COUNT_ARGS_IMPL2(_1, _2, _3, name, ...) name
#define COUNT_ARGS_IMPL(args) COUNT_ARGS_IMPL2 args
#define COUNT_ARGS(...) COUNT_ARGS_IMPL((__VA_ARGS__, 3, 2, 1))
#define MACRO_CHOOSE2(prefix, number) prefix##number
#define MACRO_CHOOSE1(prefix, number) MACRO_CHOOSE2(prefix, number)
#define MACRO_CHOOSE(prefix, number) MACRO_CHOOSE1(prefix, number)
#define MACRO_GLUE(x, y) x y

#define IN_CONTEXT(...)                                                           \
    EvaluationContext context;                                                    \
    SET_CONTEXT_PRECISION                                                         \
    MACRO_GLUE(MACRO_CHOOSE(IN_CONTEXT_, COUNT_ARGS(__VA_ARGS__)), (__VA_ARGS__))

#define IN_DEFAULT_CONTEXT(...)                                                   \
    EvaluationContext context;                                                    \
    context.FloatPrecision = HugeFloat::DefaultPrecision;                         \
    MACRO_GLUE(MACRO_CHOOSE(IN_CONTEXT_, COUNT_ARGS(__VA_ARGS__)), (__VA_ARGS__))

#define IN_SPECIFIC_CONTEXT(precision, ...)                                       \
    EvaluationContext context;                                                    \
    context.FloatPrecision = precision;                                           \
    MACRO_GLUE(MACRO_CHOOSE(IN_CONTEXT_, COUNT_ARGS(__VA_ARGS__)), (__VA_ARGS__))

//defines a unary expression class
#define DEFINE_UNARY_EXPRESSION(base, name, type)                \
private ref class MPEXPR(name) : base                            \
{                                                                \
    internal:                                                    \
        initonly type Operand;                                   \
        virtual void AssignTo(MP(ptr) destination) override;     \
        MPEXPR(name)(type operand)                               \
        {                                                        \
            Operand = operand;                                   \
        }                                                        \
};

//defines a binary expression class
#define DEFINE_BINARY_EXPRESSION(base, name, leftType, rightType)     \
private ref class MPEXPR(name) : base                                 \
{                                                                     \
    internal:                                                         \
        initonly leftType Left;                                       \
        initonly rightType Right;                                     \
        virtual void AssignTo(MP(ptr) destination) override;          \
        ADDITIONAL_MEMBERS                                            \
        MPEXPR(name)(leftType left, rightType right)                  \
        {                                                             \
            Left = left;                                              \
            Right = right;                                            \
        }                                                             \
};

//defines a ternary expression class
#define DEFINE_TERNARY_EXPRESSION(base, name, leftType, middleType, rightType)    \
private ref class MPEXPR(name) : base                                             \
{                                                                                 \
    internal:                                                                     \
        initonly leftType Left;                                                   \
        initonly middleType Middle;                                               \
        initonly rightType Right;                                                 \
        virtual void AssignTo(MP(ptr) destination) override;                      \
        MPEXPR(name)(leftType left, middleType middle, rightType right)           \
        {                                                                         \
            Left = left;                                                          \
            Middle = middle;                                                      \
            Right = right;                                                        \
        }                                                                         \
};

//unary expressions
#define DEFINE_UNARY_EXPRESSION_WITH_ONE(base, name, typeAbbr) \
    DEFINE_UNARY_EXPRESSION(base, name##typeAbbr, MPEXPR_NAME^)           

#define DEFINE_UNARY_EXPRESSION_WITH_BUILT_INS_ONLY(base, name, typeAbbr)    \
    DEFINE_UNARY_EXPRESSION(base, name##typeAbbr, TYPE_FOR_ABBR_##typeAbbr)

//binary expressions
#define DEFINE_BINARY_EXPRESSION_WITH_BUILT_INS_ONLY(base, name, leftTypeAbbr, rightTypeAbbr)    \
    DEFINE_BINARY_EXPRESSION(base, name##leftTypeAbbr##rightTypeAbbr, TYPE_FOR_ABBR_##leftTypeAbbr, TYPE_FOR_ABBR_##rightTypeAbbr)

#define DEFINE_BINARY_EXPRESSION_WITH_TWO(base, name, typeAbbr) \
    DEFINE_BINARY_EXPRESSION(base, name##typeAbbr##typeAbbr, MPEXPR_NAME^, MPEXPR_NAME^)

#define DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(base, name, leftTypeAbbr, rightTypeAbbr)    \
    DEFINE_BINARY_EXPRESSION(base, name##leftTypeAbbr##rightTypeAbbr, MPEXPR_NAME^, TYPE_FOR_ABBR_##rightTypeAbbr) 

#define DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT(base, name, leftTypeAbbr, rightTypeAbbr)     \
    DEFINE_BINARY_EXPRESSION(base, name##leftTypeAbbr##rightTypeAbbr, TYPE_FOR_ABBR_##leftTypeAbbr, MPEXPR_NAME^)

//ternary expressions
#define DEFINE_TERNARY_EXPRESSION_WITH_THREE(base, name, typeAbbr) \
    DEFINE_TERNARY_EXPRESSION(base, name##typeAbbr##typeAbbr##typeAbbr, MPEXPR_NAME^, MPEXPR_NAME^, MPEXPR_NAME^)

#define DEFINE_TERNARY_EXPRESSION_WITH_BUILT_IN_MIDDLE(base, name, leftTypeAbbr, middleTypeAbbr, rightTypeAbbr)    \
    DEFINE_TERNARY_EXPRESSION(base, name##leftTypeAbbr##middleTypeAbbr##rightTypeAbbr, MPEXPR_NAME^, TYPE_FOR_ABBR_##middleTypeAbbr, MPEXPR_NAME^)

#pragma endregion

#pragma region Method macros

//void functions
#define MAKE_VOID_FUNCTION(base, action, op, type)  \
    MAKE_VOID_FUNCTION_##action(base, op, op##type)

#define MAKE_VOID_FUNCTION_DECLARE(base, op, result)     \
    base^ op();

#define MAKE_VOID_FUNCTION_DEFINE(base, op, result)      \
    base^ MPEXPR_NAME::op() { return gcnew MPEXPR(result)(this); }

//one-arg functions
#define MAKE_FUNCTION_WITH_ONE(base, action, op, argTypeAbbr)  \
    MAKE_FUNCTION_WITH_ONE_##action(base, op, Expr, CONCAT(op, LIT(Mpt))argTypeAbbr)

#define MAKE_FUNCTION_WITH_LIMB(base, action, op, argTypeAbbr)  \
    MAKE_FUNCTION_WITH_ONE_##action(base, op, argTypeAbbr, CONCAT(op, LIT(Mpt))argTypeAbbr)

#define MAKE_FUNCTION_WITH_ONE_DECLARE(base, op, argTypeAbbr, result)     \
    base^ op(TYPE_FOR_ABBR_##argTypeAbbr a);

#define MAKE_FUNCTION_WITH_ONE_DEFINE(base, op, argTypeAbbr, result)      \
    base^ MPEXPR_NAME::op(TYPE_FOR_ABBR_##argTypeAbbr a) { return gcnew MPEXPR(result)(this, a); }

//two-arg functions
#define MAKE_FUNCTION_WITH_TWO(base, action, op, leftTypeAbbr, rightTypeAbbr)  \
    MAKE_FUNCTION_WITH_TWO_##action(base, op, Expr, Expr, CONCAT(op, LIT(Mpt))leftTypeAbbr##rightTypeAbbr)

#define MAKE_FUNCTION_WITH_TWO_LLIMB(base, action, op, leftTypeAbbr, rightTypeAbbr)  \
    MAKE_FUNCTION_WITH_TWO_##action(base, op, leftTypeAbbr, Expr, CONCAT(op, LIT(Mpt))leftTypeAbbr##rightTypeAbbr)

#define MAKE_FUNCTION_WITH_TWO_DECLARE(base, op, leftTypeAbbr, rightTypeAbbr, result)     \
    base^ op(TYPE_FOR_ABBR_##leftTypeAbbr a, TYPE_FOR_ABBR_##rightTypeAbbr b);

#define MAKE_FUNCTION_WITH_TWO_DEFINE(base, op, leftTypeAbbr, rightTypeAbbr, result)      \
    base^ MPEXPR_NAME::op(TYPE_FOR_ABBR_##leftTypeAbbr a, TYPE_FOR_ABBR_##rightTypeAbbr b) { return gcnew MPEXPR(result)(this, a, b); }

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
//    TYPE_FOR_ABBR_##resultTypeAbbr HugeInt::op(TYPE_FOR_ABBR_##argTypeAbbr a) { return gcnew LIT(MPTYPE_NAME)result##Expression(this, a); }

//unary operators
#define MAKE_UNARY_OPERATOR(base, action, op, result, mpType) \
    MAKE_UNARY_OPERATOR_##action(base, op, result##mpType, Expr)

#define MAKE_UNARY_OPERATOR_DECLARE(base, op, result, type)     \
    static base^ operator op(TYPE_FOR_ABBR_##type a);

#define MAKE_UNARY_OPERATOR_DEFINE(base, op, result, type)      \
    base^ MPEXPR_NAME::operator op(TYPE_FOR_ABBR_##type a) { return gcnew MPEXPR(result)(a); }

//binary operators
#define MAKE_BINARY_OPERATOR_DECLARE(base, op, result, leftType, rightType, left, right)     \
    static base^ operator op(TYPE_FOR_ABBR_##leftType a, TYPE_FOR_ABBR_##rightType b);

#define MAKE_BINARY_OPERATOR_DEFINE(base, op, result, leftType, rightType, left, right)      \
    base^ MPEXPR_NAME::operator op(TYPE_FOR_ABBR_##leftType a, TYPE_FOR_ABBR_##rightType b) { return gcnew MPEXPR(result)(left, right); }

#define MAKE_BINARY_OPERATOR_STANDARD(base, action, op, result, leftType, rightType)        \
    MAKE_BINARY_OPERATOR_##action(base, op, result##leftType##rightType, Expr, Expr, a, b)      

#define MAKE_BINARY_OPERATOR_RLIMB(base, action, op, result, mpType, limbType)  \
    MAKE_BINARY_OPERATOR_##action(base, op, result##mpType##limbType, Expr, limbType, a, b)          

#define MAKE_BINARY_OPERATOR_LLIMB(base, action, op, result, mpType, limbType)  \
    MAKE_BINARY_OPERATOR_##action(base, op, result##limbType##mpType, limbType, Expr, a, b)           

#define MAKE_BINARY_OPERATOR_LLIMB_R(base, action, op, result, mpType, limbType) \
    MAKE_BINARY_OPERATOR_##action(base, op, result##mpType##limbType, limbType, Expr, b, a)

#pragma endregion

#pragma region expression assignment macros

#define DEFINE_ASSIGNMENT_PROLOG(name) void MPEXPR(name)::AssignTo(MP(ptr) destination)

#define DEFINE_UNARY_ASSIGNMENT_REF(name, typeAbbr, operation)     \
    DEFINE_ASSIGNMENT_PROLOG(name##typeAbbr)                       \
    {                                                              \
        Operand->AssignTo(destination);                            \
        operation(destination, destination);                       \
    }

#define DEFINE_UNARY_ASSIGNMENT_VAL(name, typeAbbr, operation)     \
    DEFINE_ASSIGNMENT_PROLOG(name##typeAbbr)                       \
    {                                                              \
        operation(destination, Operand);                           \
    }

#define DEFINE_BINARY_ASSIGNMENT_REF_INTVAL(name, leftTypeAbbr, rightTypeAbbr, operation) \
    DEFINE_ASSIGNMENT_PROLOG(name##leftTypeAbbr##rightTypeAbbr)                           \
    {                                                                                     \
        IN_CONTEXT(Left, Right);                                                          \
        operation(destination, CTXT(0), CTXT(1));                                         \
    }

#define DEFINE_BINARY_ASSIGNMENT_REF_REF(name, typeAbbr, operation)                       \
    DEFINE_BINARY_ASSIGNMENT_REF_INTVAL(name, typeAbbr, typeAbbr, operation)

#define DEFINE_BINARY_ASSIGNMENT_REF_VAL(name, leftTypeAbbr, rightTypeAbbr, operation)    \
    DEFINE_ASSIGNMENT_PROLOG(name##leftTypeAbbr##rightTypeAbbr)                           \
    {                                                                                     \
        Left->AssignTo(destination);                                                      \
        operation(destination, destination, Right);                                       \
    }

#define DEFINE_BINARY_ASSIGNMENT_REF_RATUI(name, leftTypeAbbr, rightTypeAbbr, operation)  \
    DEFINE_ASSIGNMENT_PROLOG(name##leftTypeAbbr##rightTypeAbbr)                           \
    {                                                                                     \
        IN_CONTEXT(Left);                                                                 \
        CTXT_ADD_RATIONAL_UI(Right, 1);                                                   \
        operation(destination, CTXT(0), CTXT(1));                                         \
    }

#define DEFINE_BINARY_ASSIGNMENT_REF_RATSI(name, leftTypeAbbr, rightTypeAbbr, operation)  \
    DEFINE_ASSIGNMENT_PROLOG(name##leftTypeAbbr##rightTypeAbbr)                           \
    {                                                                                     \
        IN_CONTEXT(Left);                                                                 \
        CTXT_ADD_RATIONAL_SI(Right, 1);                                                   \
        operation(destination, CTXT(0), CTXT(1));                                         \
    }

#define DEFINE_BINARY_ASSIGNMENT_VAL_REF(name, leftTypeAbbr, rightTypeAbbr, operation)    \
    DEFINE_ASSIGNMENT_PROLOG(name##leftTypeAbbr##rightTypeAbbr)                           \
    {                                                                                     \
        Right->AssignTo(destination);                                                     \
        operation(destination, Left, destination);                                        \
    }

#define DEFINE_BINARY_ASSIGNMENT_RATUI_REF(name, leftTypeAbbr, rightTypeAbbr, operation)  \
    DEFINE_ASSIGNMENT_PROLOG(name##leftTypeAbbr##rightTypeAbbr)                           \
    {                                                                                     \
        IN_CONTEXT(Right);                                                                \
        CTXT_ADD_RATIONAL_UI(Left, 1);                                                    \
        operation(destination, CTXT(1), CTXT(0));                                         \
    }

#define DEFINE_BINARY_ASSIGNMENT_RATSI_REF(name, leftTypeAbbr, rightTypeAbbr, operation)  \
    DEFINE_ASSIGNMENT_PROLOG(name##leftTypeAbbr##rightTypeAbbr)                           \
    {                                                                                     \
        IN_CONTEXT(Right);                                                                \
        CTXT_ADD_RATIONAL_SI(Left, 1);                                                    \
        operation(destination, CTXT(1), CTXT(0));                                         \
    }

#define DEFINE_BINARY_ASSIGNMENT_VAL_VAL(name, leftTypeAbbr, rightTypeAbbr, operation)    \
    DEFINE_ASSIGNMENT_PROLOG(name##leftTypeAbbr##rightTypeAbbr)                           \
    {                                                                                     \
        operation(destination, Left, Right);                                              \
    }

#define DEFINE_BINARY_ASSIGNMENT_REF_SI(name, leftTypeAbbr, rightTypeAbbr, positiveOp, negativeOp)      \
    DEFINE_ASSIGNMENT_PROLOG(name##leftTypeAbbr##rightTypeAbbr)                                         \
    {                                                                                                   \
        Left->AssignTo(destination);                                                                    \
        if (Right >= 0)                                                                                 \
            positiveOp(destination, destination, (mpir_ui)Right);                                       \
        else                                                                                            \
            negativeOp(destination, destination, (mpir_ui)-Right);                                      \
    }

#define DEFINE_BINARY_ASSIGNMENT_REF_SI2(name, leftTypeAbbr, rightTypeAbbr, operation, negateOp)        \
    DEFINE_ASSIGNMENT_PROLOG(name##leftTypeAbbr##rightTypeAbbr)                                         \
    {                                                                                                   \
        Left->AssignTo(destination);                                                                    \
        operation(destination, destination, (Right >= 0) ? (mpir_ui)Right : (mpir_ui)-Right);           \
        if (Right < 0)                                                                                  \
            negateOp(destination, destination);                                                         \
    }

#define DEFINE_BINARY_ASSIGNMENT_SI_REF(name, leftTypeAbbr, rightTypeAbbr, positiveOp, negativeOp1, negativeOp2)   \
    DEFINE_ASSIGNMENT_PROLOG(name##leftTypeAbbr##rightTypeAbbr)                                                    \
    {                                                                                                              \
        Right->AssignTo(destination);                                                                              \
        if (Left >= 0)                                                                                             \
            positiveOp(destination, (mpir_ui)Left, destination);                                                   \
        else                                                                                                       \
        {                                                                                                          \
            negativeOp1(destination, destination, (mpir_ui)-Left);                                                 \
            negativeOp2(destination, destination);                                                                 \
        }                                                                                                          \
    }

#define DEFINE_BINARY_ASSIGNMENT_SI_REF2(name, leftTypeAbbr, rightTypeAbbr, positiveOp, negativeOp1, negativeOp2)  \
    DEFINE_ASSIGNMENT_PROLOG(name##leftTypeAbbr##rightTypeAbbr)                                                    \
    {                                                                                                              \
        Right->AssignTo(destination);                                                                              \
        if (Left >= 0)                                                                                             \
            positiveOp(destination, (mpir_ui)Left, destination);                                                   \
        else                                                                                                       \
        {                                                                                                          \
            negativeOp1(destination, (mpir_ui)-Left, destination);                                                 \
            negativeOp2(destination, destination);                                                                 \
        }                                                                                                          \
    }

#define DEFINE_TERNARY_ASSIGNMENT_REF_REF_REF(name, typeAbbr, operation)   \
    DEFINE_ASSIGNMENT_PROLOG(name##typeAbbr##typeAbbr##typeAbbr)           \
    {                                                                      \
        IN_CONTEXT(Left, Middle, Right);                                   \
        operation(destination, CTXT(0), CTXT(1), CTXT(2));                 \
    }

#define DEFINE_TERNARY_ASSIGNMENT_REF_VAL_REF(name, leftT, middleT, rightT, operation) \
    DEFINE_ASSIGNMENT_PROLOG(name##leftT##middleT##rightT)                             \
    {                                                                                  \
        IN_CONTEXT(Left, Right);                                                       \
        operation(destination, CTXT(0), Middle, CTXT(1));                              \
    }

#pragma endregion
