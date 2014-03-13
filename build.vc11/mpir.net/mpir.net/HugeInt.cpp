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

#include "Stdafx.h"
#include "Common.h"

#define DEFINE_VOID_FROM_MPZ(x, impl)                   \
    void HugeInt::x(HugeInt^ a)                         \
    {                                                   \
        THIS_PTR;                                       \
        mpz_##impl(src_this, src_this, a->_value);      \
        SAVE_THIS;                                      \
    }

#define DEFINE_VOID_FROM_MPZ_MPZ(x, impl)               \
    void HugeInt::x(HugeInt^ a, HugeInt^ b)             \
    {                                                   \
        THIS_PTR;                                       \
        mpz_##impl(src_this, a->_value, b->_value);     \
        SAVE_THIS;                                      \
    }

#define DEFINE_VOID_FROM_UI(x, impl)                    \
    void HugeInt::x(mpir_ui a)                          \
    {                                                   \
        THIS_PTR;                                       \
        mpz_##impl##_ui(src_this, src_this, a);         \
        SAVE_THIS;                                      \
    }

#define DEFINE_VOID_FROM_NONE(x, impl)                  \
    void HugeInt::x()                                   \
    {                                                   \
        THIS_PTR;                                       \
        mpz_##impl##(src_this, src_this);               \
        SAVE_THIS;                                      \
    }

#define DEFINE_VOID_FROM_2EXP(x, impl)                  \
    void HugeInt::x(mp_bitcnt_t a)                      \
    {                                                   \
        THIS_PTR;                                       \
        mpz_##impl##_2exp(src_this, src_this, a);       \
        SAVE_THIS;                                      \
    }

#define DEFINE_VOID_FROM_MPZ_UI(x, impl)                \
    void HugeInt::x(HugeInt^ a, mpir_ui b)              \
    {                                                   \
        THIS_PTR;                                       \
        mpz_##impl##_ui(src_this, a->_value, b);        \
        SAVE_THIS;                                      \
    }

#define DEFINE_VOID_UI_FROM(x, impl)                    \
    void HugeInt::x(mpir_ui a)                          \
    {                                                   \
        THIS_PTR;                                       \
        mpz_ui_##impl##(src_this, a, src_this);         \
        SAVE_THIS;                                      \
    }

#define DEFINE_VOID_FROM_SI(x, impl)                    \
    void HugeInt::x(mpir_si a)                          \
    {                                                   \
        THIS_PTR;                                       \
        mpz_##impl##_si(src_this, src_this, a);         \
        SAVE_THIS;                                      \
    }

#define DEFINE_VOID_FROM_MPZ_SI(x, impl)                \
    void HugeInt::x(HugeInt^ a, mpir_si b)              \
    {                                                   \
        THIS_PTR;                                       \
        mpz_##impl##_si(src_this, a->_value, b);        \
        SAVE_THIS;                                      \
    }

#define DEFINE_VOID_FROM_MPZ_OR_UI_OR_SI(x, impl)       \
        DEFINE_VOID_FROM_MPZ(x, impl)                   \
        DEFINE_VOID_FROM_UI(x, impl)                    \
        DEFINE_VOID_FROM_SI(x, impl)

#define DEFINE_VOID_FROM_MPZ_OR_UI(x, impl)             \
        DEFINE_VOID_FROM_MPZ(x, impl)                   \
        DEFINE_VOID_FROM_UI(x, impl)

#define DEFINE_VOID_FROM_MPZ_MPZ_OR_UI(x, impl)         \
        DEFINE_VOID_FROM_MPZ_MPZ(x, impl)               \
        DEFINE_VOID_FROM_MPZ_UI(x, impl)

using namespace System::Runtime::InteropServices;

namespace MPIR
{
    #pragma region construction

    static HugeInt::HugeInt()
    {
        SetCustomAllocationFunctions();
    }

    HugeInt::HugeInt(mpz_srcptr src_this)
    {
        SAVE_THIS;
    }

    HugeInt::HugeInt()
    {
        mpz_t src_this;
        mpz_init(src_this);
        SAVE_THIS;
    }

    HugeInt::HugeInt(mp_bitcnt_t value)
    {
        mpz_t src_this;
        mpz_init2(src_this, value);
        SAVE_THIS;
    }

    HugeInt::HugeInt(String^ value)
    {
        FromString(value, 10);
    }

    HugeInt::HugeInt(String^ value, int base)
    {
        FromString(value, base);
    }

    void HugeInt::FromString(String^ value, int base)
    {
        mpz_t src_this;

        IntPtr ptr = Marshal::StringToHGlobalAnsi(value);
        bool success = 0 == mpz_init_set_str(src_this, (char*)(void*)ptr, base);
        Marshal::FreeHGlobal(ptr);

        if(!success)
        {
            mpz_clear(src_this);
            throw gcnew ArgumentException("InvalidNumber", "value");
        }

        SAVE_THIS;
    }

    HugeInt^ HugeInt::FromLong(mpir_si value)
    {
        mpz_t src_a;
        mpz_init_set_si(src_a, value);
        return gcnew HugeInt(src_a);
    }

    HugeInt^ HugeInt::FromUlong(mpir_ui value)
    {
        mpz_t src_a;
        mpz_init_set_ui(src_a, value);
        return gcnew HugeInt(src_a);
    }

    //unmanaged destructor makes the compiler implement IDisposable
    //delegates the actual destruction to the finalizer
    HugeInt::~HugeInt()
    {
        this->!HugeInt();
    }

    //managed finalizer
    HugeInt::!HugeInt()
    {
        if(_value != 0)
        {
            THIS_PTR;
            mpz_clear(src_this);
            _value = 0;
        }
    }

    #pragma endregion

    #pragma region object overrides

    String^ HugeInt::ToString()
    {
        return ToString(10);
    }

    String^ HugeInt::ToString(int base)
    {
        THIS_PTR;
        char* str = mpz_get_str(NULL, base, src_this);
        String^ result = gcnew String(str);
        CustomFree(str, 0);

        return result;
    }

    #pragma endregion

    #pragma region Arithmetic

    IMpirExpression^ HugeInt::operator+(HugeInt^ a, HugeInt^ b) { return gcnew MpirAddIntIntExpression(a, b); }
    IMpirExpression^ HugeInt::operator+(HugeInt^ a, mpir_ui b) { return gcnew MpirAddIntUiExpression(a, b); }
    IMpirExpression^ HugeInt::operator+(mpir_ui a, HugeInt^ b) { return gcnew MpirAddIntUiExpression(b, a); }

    void MpirAddIntIntExpression::AssignTo(HugeInt^ destination)
    {
        SRC_PTR(destination);
        mpz_add(src_destination, Left->_value, Right->_value);
        SAVE_PTR(destination);
    }

    void MpirAddIntUiExpression::AssignTo(HugeInt^ destination)
    {
        SRC_PTR(destination);
        mpz_add_ui(src_destination, Left->_value, Right);
        SAVE_PTR(destination);
    }

    //DEFINE_VOID_FROM_MPZ_OR_UI(Add, add)
    DEFINE_VOID_FROM_MPZ_OR_UI(Subtract, sub)
    DEFINE_VOID_UI_FROM(SubtractFrom, sub)
    DEFINE_VOID_FROM_MPZ_OR_UI_OR_SI(MultiplyBy, mul)
    DEFINE_VOID_FROM_MPZ_MPZ_OR_UI(AddProduct, addmul)
    DEFINE_VOID_FROM_MPZ_MPZ_OR_UI(SubtractProduct, submul)
    DEFINE_VOID_FROM_2EXP(ShiftLeft, mul)
    DEFINE_VOID_FROM_NONE(Negate, neg)
    DEFINE_VOID_FROM_NONE(MakeAbsolute, abs)

    #pragma endregion
};