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
#include "HugeInt.h"

//makes a local var from the managed mpz struct data of the specified instance
#define SRC_PTR(x) \
	mpz_t src_##x; \
	src_##x->_mp_alloc = x._numberOfLimbsAllocated; \
	src_##x->_mp_size = x._numberOfLimbsUsed; \
	src_##x->_mp_d = x._limbs

//makes a local var from the managed mpz struct data of this instance
#define THIS_PTR \
	mpz_t src_this; \
	src_this->_mp_alloc = _numberOfLimbsAllocated; \
	src_this->_mp_size = _numberOfLimbsUsed; \
	src_this->_mp_d = _limbs

////updated the managed mpz struct data for the specified instance from a local var
//#define SAVE_PTR(x) \
//	x._numberOfLimbsAllocated = src_##x->_mp_alloc; \
//	x._numberOfLimbsUsed = src_##x->_mp_size; \
//	x._limbs = src_##x->_mp_d;

//updated the managed mpz struct data for this instance from a local var
#define SAVE_THIS \
	_numberOfLimbsAllocated = src_this->_mp_alloc; \
	_numberOfLimbsUsed = src_this->_mp_size; \
	_limbs = src_this->_mp_d

namespace MPIR
{
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

	HugeInt::~HugeInt()
	{
		this->!HugeInt();
	}

	HugeInt::!HugeInt()
	{
		if(_limbs != 0)
		{
			THIS_PTR;
			mpz_clear(src_this);
			_numberOfLimbsAllocated = 0;
			_numberOfLimbsUsed = 0;
			_limbs = 0;
		}
	}
};
