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
    /// <summary>
    /// This class encapsulates a random number generator algorithm and state
    /// </summary>
    public ref class MpirRandom
    {
        internal:
            //fields
            gmp_randstate_ptr _value;

        #pragma region Construction and disposal

        private:
            MpirRandom()
            {
                _value = (gmp_randstate_ptr)((*__gmp_allocate_func)(sizeof(__gmp_randstate_struct)));
            }

        public:

            //creating a destructor in C++ implements IDisposable.

            /// <summary>
            /// Frees all memory allocated by the instance.
            /// <para>To minimize memory footprint, MPIR objects should be disposed of when no longer used, instead of relying on the garbage collector to free the memory.
            /// </para></summary>
            ~MpirRandom() { this->!MpirRandom(); }

            /// <summary>
            /// Frees all memory allocated by the instance.
            /// <para>To minimize memory footprint, MPIR objects should be disposed of when no longer used, instead of relying on the garbage collector to free the memory.
            /// </para></summary>
            !MpirRandom()
            {
                if(_value != 0) 
                {
                    gmp_randclear(_value);
                    (*__gmp_free_func)(_value, sizeof(__gmp_randstate_struct));
                    _value = nullptr;
                }
            }

        #pragma endregion

        #pragma region Initialization factory methods

            /// <summary>
            /// Creates a new random generator with a copy of the algorithm and state from this instance
            /// </summary>
            /// <returns>The newly initialized instance</returns>
            MpirRandom^ Copy()
            {
                auto result = gcnew MpirRandom();
                gmp_randinit_set(result->_value, _value);
                return result;
            }

            /// <summary>
            /// Creates a new random generator with the default algorithm.
            /// <para>This will be a compromise between speed and randomness, and is recommended for applications with no special requirements.
            /// </para>Currently Mersenne Twister is the default algorithm.
            /// </summary>
            /// <returns>The newly initialized instance</returns>
            static MpirRandom^ Default()
            {
                auto result = gcnew MpirRandom();
                gmp_randinit_default(result->_value);
                return result;
            }
            
            /// <summary>
            /// Creates a new random generator with the Mersenne Twister algorithm. This algorithm is fast and has good randomness properties.
            /// </summary>
            /// <returns>The newly initialized instance</returns>
            static MpirRandom^ MersenneTwister()
            {
                auto result = gcnew MpirRandom();
                gmp_randinit_mt(result->_value);
                return result;
            }

            /// <summary>
            /// Creates a new random generator with a linear congruential algorithm X = (aX + c) mod 2^m.
            /// <para>The low bits of X in this algorithm are not very random. The least significant bit will have
            /// a period no more than 2, and the second bit no more than 4, etc. For this reason only the
            /// high half of each X is actually used.
            /// </para>When a random number of more than m/2 bits is to be generated, multiple iterations
            /// of the recurrence are used and the results concatenated.
            /// </summary>
            /// <param name="a">The multiplier in the algorithm's formula X = (aX + c) mod 2^m</param>
            /// <param name="c">The addend in the algorithm's formula X = (aX + c) mod 2^m</param>
            /// <param name="m">The power of 2 for the divisor in the algorithm's formula X = (aX + c) mod 2^m</param>
            /// <returns>The newly initialized instance</returns>
            static MpirRandom^ LinearCongruential(HugeInt^ a, mpir_ui c, mp_bitcnt_t m)
            {
                auto result = gcnew MpirRandom();
                gmp_randinit_lc_2exp(result->_value, a->_value, c, m);
                return result;
            }

            /// <summary>
            /// Creates a new random generator with a linear congruential algorithm X = (aX + c) mod 2^m.
            /// <para>a, c and m are selected from a table, chosen so that size bits (or more) of each X will be used, i.e. m/2 >= size.
            /// </para>The maximum size currently supported is 128.
            /// </summary>
            /// <param name="size">The number of high bits that should be used from each iteration of the algorithm.  The current maximum is 128.</param>
            /// <returns>The newly initialized instance</returns>
            static MpirRandom^ LinearCongruential(mp_bitcnt_t size)
            {
                auto result = gcnew MpirRandom();
                if(gmp_randinit_lc_2exp_size(result->_value, size) == 0)
                {
                    result->!MpirRandom();
                    throw gcnew ArgumentException("Invalid size", "size");
                }
                return result;
            }

        #pragma endregion

        #pragma region Seeding

            /// <summary>
            /// Sets an initial seed value into the random number generator.
            /// </summary>
            /// <param name="seed">The seed value</param>
            /// <remarks>
            /// The size of a seed determines how many different sequences of random numbers it's
            /// possible to generate. The "quality" of the seed is the randomness of a given seed compared
            /// to the previous seed used, and this affects the randomness of separate number sequences. The
            /// method for choosing a seed is critical if the generated numbers are to be used for important
            /// applications, such as generating cryptographic keys.
            /// <para>Traditionally the system time has been used to seed, but care needs to be taken with this.
            /// If an application seeds often and the resolution of the system clock is low, then the same
            /// sequence of numbers might be repeated. Also, the system time is quite easy to guess, so if
            /// unpredictability is required then it should definitely not be the only source for the seed value.
            /// </para>On some systems there's a special device '/dev/random' which provides random data better
            /// suited for use as a seed.
            /// </remarks>
            void Seed(mpir_ui seed) { gmp_randseed_ui(_value, seed); }

            /// <summary>
            /// Sets an initial seed value into the random number generator.
            /// </summary>
            /// <param name="seed">The seed value</param>
            /// <remarks>
            /// The size of a seed determines how many different sequences of random numbers it's
            /// possible to generate. The "quality" of the seed is the randomness of a given seed compared
            /// to the previous seed used, and this affects the randomness of separate number sequences. The
            /// method for choosing a seed is critical if the generated numbers are to be used for important
            /// applications, such as generating cryptographic keys.
            /// <para>Traditionally the system time has been used to seed, but care needs to be taken with this.
            /// If an application seeds often and the resolution of the system clock is low, then the same
            /// sequence of numbers might be repeated. Also, the system time is quite easy to guess, so if
            /// unpredictability is required then it should definitely not be the only source for the seed value.
            /// </para>On some systems there's a special device '/dev/random' which provides random data better
            /// suited for use as a seed.
            /// </remarks>
            void Seed(HugeInt^ seed) { gmp_randseed(_value, seed->_value); }

        #pragma endregion

        #pragma region Random Limb

            /// <summary>
            /// Generates a uniformly distributed random number of <paramref name="bitCount"/> bits, i.e. in the range 0 to 2^<paramref name="bitCount"/>-1 inclusive.
            /// </summary>
            /// <param name="bitCount">The number of random bits to generate.  Must be less than or equal to the number of bits in a limb.</param>
            /// <returns>The newly generated number</returns>
            mpir_ui GetLimbBits(mpir_ui bitCount) { return gmp_urandomb_ui(_value, bitCount); }

            /// <summary>
            /// Generates a uniformly distributed random number in the range 0 to <paramref name="max"/>-1 inclusive.
            /// </summary>
            /// <param name="max">The exclusive upper bound for the random number</param>
            /// <returns>The newly generated number</returns>
            mpir_ui GetLimb(mpir_ui max) { return gmp_urandomm_ui(_value, max); }

        #pragma endregion

        #pragma region Random Int

        /// <summary>
        /// Generates a uniformly distributed random integer in the range 0 to 2^<paramref name="bitCount"/> - 1, inclusive.
        /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
        /// </para></summary>
        /// <param name="bitCount">number of bits to generate</param>
        /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, generates the random number</returns>
        IntegerExpression^ GetIntBits(mp_bitcnt_t bitCount) { return gcnew IntegerRandomBitsExpression(this, bitCount); }

        /// <summary>
        /// Generates a random integer with long strings of zeros and ones in the binary representation.
        /// <para>Useful for testing functions and algorithms, since this kind of random numbers have proven
        /// to be more likely to trigger corner-case bugs.
        /// </para>The random number will be in the range 0 to 2^<paramref name="bitCount"/> - 1, inclusive.
        /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
        /// </para></summary>
        /// <param name="bitCount">number of bits to generate</param>
        /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, generates the random number</returns>
        IntegerExpression^ GetIntBitsChunky(mp_bitcnt_t bitCount) { return gcnew IntegerRandomBitsChunkyExpression(this, bitCount); }

        /// <summary>
        /// Generates a uniformly distributed random integer in the range 0 to <paramref name="max"/> - 1, inclusive.
        /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
        /// </para></summary>
        /// <param name="max">exclusive upper bound for the number to generate</param>
        /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, generates the random number</returns>
        IntegerExpression^ GetInt(IntegerExpression^ max) { return gcnew IntegerRandomExpression(this, max); }

        #pragma endregion

        #pragma region Random Float

        /// <summary>
        /// Generates a uniformly distributed random float in the range 0 &lt;= n &lt; 1, using the precision of the destination.
        /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
        /// </para></summary>
        /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, generates the random number</returns>
        FloatExpression^ GetFloat();

        /// <summary>
        /// Generates a uniformly distributed random float in the range 0 &lt;= n &lt; 1 with the specified number of significant bits in the mantissa.
        /// <para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
        /// </para></summary>
        /// <param name="bitCount">number of mantissa bits to generate</param>
        /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, generates the random number</returns>
        FloatExpression^ GetFloatBits(mp_bitcnt_t bitCount);

        /// <summary>
        /// Generates a random float with long strings of zeros and ones in the binary representation, using the precision of the destination.
        /// <para>Useful for testing functions and algorithms, since this kind of random numbers have proven
        /// to be more likely to trigger corner-case bugs.
        /// </para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
        /// </summary>
        /// <param name="maxExponent">The maximum absolute value for the exponent of the generated number (expressed in limbs).  Generated exponent may be positive or negative.</param>
        /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, generates the random number</returns>
        FloatExpression^ GetFloatChunky(mp_exp_t maxExponent);

        /// <summary>
        /// Generates a random float with long strings of zeros and ones in the binary representation, and the specified number of significant limbs in the mantissa.
        /// <para>Useful for testing functions and algorithms, since this kind of random numbers have proven
        /// to be more likely to trigger corner-case bugs.
        /// </para>As with all expressions, the result is not computed until the expression is assigned to the Value property or consumed by a method.
        /// </summary>
        /// <param name="limbCount">number of mantissa limbs to generate.
        /// <para>The sign of this parameter determines the sign of the generated mantissa.</para></param>
        /// <param name="maxExponent">The maximum absolute value for the exponent of the generated number (expressed in limbs).  Generated exponent may be positive or negative.</param>
        /// <returns>An expression object that, when assigned to the Value property or consumed by a primitive-returning method, generates the random number</returns>
        FloatExpression^ GetFloatLimbsChunky(mp_size_t limbCount, mp_exp_t maxExponent);

        #pragma endregion
    };
};
