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

#define DEFINE_ASSIGNMENT_PROLOG(name) void Mpir##name##Expression::AssignTo(mpz_ptr destination)

#define DEFINE_UNARY_ASSIGNMENT_REF(name, typeAbbr, operation)     \
    DEFINE_ASSIGNMENT_PROLOG(name##typeAbbr)                       \
    {                                                              \
        Operand->AssignTo(destination);                            \
        operation(destination, destination);                       \
    }

#define DEFINE_BINARY_ASSIGNMENT_REF_REF(name, typeAbbr, operation)   \
    DEFINE_ASSIGNMENT_PROLOG(name##typeAbbr##typeAbbr)                \
    {                                                                 \
        IN_CONTEXT(Left, Right);                                      \
        operation(destination, context.Args[0], context.Args[1]);     \
    }

#define DEFINE_BINARY_ASSIGNMENT_REF_VAL(name, leftTypeAbbr, rightTypeAbbr, operation)    \
    DEFINE_ASSIGNMENT_PROLOG(name##leftTypeAbbr##rightTypeAbbr)                           \
    {                                                                                     \
        Left->AssignTo(destination);                                                      \
        operation(destination, destination, Right);                                       \
    }

#define DEFINE_BINARY_ASSIGNMENT_VAL_REF(name, leftTypeAbbr, rightTypeAbbr, operation)    \
    DEFINE_ASSIGNMENT_PROLOG(name##leftTypeAbbr##rightTypeAbbr)                           \
    {                                                                                     \
        Right->AssignTo(destination);                                                     \
        operation(destination, Left, destination);                                        \
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

#define DEFINE_TERNARY_ASSIGNMENT_REF_REF_REF(name, typeAbbr, operation)               \
    DEFINE_ASSIGNMENT_PROLOG(name##typeAbbr##typeAbbr##typeAbbr)                       \
    {                                                                                  \
        IN_CONTEXT(Left, Middle, Right);                                               \
        operation(destination, context.Args[0], context.Args[1], context.Args[2]);     \
    }

#define DEFINE_TERNARY_ASSIGNMENT_REF_VAL_REF(name, leftT, middleT, rightT, operation) \
    DEFINE_ASSIGNMENT_PROLOG(name##leftT##middleT##rightT)                             \
    {                                                                                  \
        IN_CONTEXT(Left, Right);                                                       \
        operation(destination, context.Args[0], Middle, context.Args[1]);              \
    }

enum EvaluationOptions : __int8
{
    None = 0x0,
    Temp1Initialized = 0x1,
    Temp2Initialized = 0x2,
    Temp3Initialized = 0x4,
};

struct EvaluationContext
{
    public:
        __mpz_struct Temp[3];
        mpz_ptr Args[3];
        union
        {
            struct
            {
                unsigned __int8 Index;
                EvaluationOptions Options;
            };
            __int64 Zero;
        };

        EvaluationContext()
        {
            Zero = 0;
        }

        ~EvaluationContext()
        {
            if(Options & EvaluationOptions::Temp1Initialized)
                mpz_clear(Args[0]);
            if(Options & EvaluationOptions::Temp2Initialized)
                mpz_clear(Args[1]);
            if(Options & EvaluationOptions::Temp3Initialized)
                mpz_clear(Args[2]);
        }
};