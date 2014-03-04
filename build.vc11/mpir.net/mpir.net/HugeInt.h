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

namespace MPIR
{
	public ref class HugeInt sealed
	{
		private:
			int _numberOfLimbsAllocated;
			int _numberOfLimbsUsed;
			mp_limb_t* _limbs;

		//construction
		private:
			HugeInt(mpz_srcptr src);
			void FromString(String^ value, int base);
			
		public:
			HugeInt();
			HugeInt(mp_bitcnt_t bits);
			HugeInt(String^ value);
			HugeInt(String^ value, int base);
			static HugeInt^ FromLong(mpir_si value);
			static HugeInt^ FromUlong(mpir_ui value);

		//disposal
		public:
			~HugeInt();
			!HugeInt();
	};
};