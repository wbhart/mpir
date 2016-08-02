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

using System;
using System.IO;
using System.Text;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace MPIR.Tests.RandomTests
{
    [TestClass]
    public class RandomTests
    {
#if WIN64
        private void TestRandom(MpirRandom r, ulong seed, Action<MpirRandom> assert)
#else
        private void TestRandom(MpirRandom r, uint seed, Action<MpirRandom> assert)
#endif
        {
            MpirRandom copy = null;

            for (var i = 0; i < 6; i++)
            {
                switch (i)
                {
                    case 0:
                    case 1:
                    case 5:
                        r.Seed(seed);
                        break;

                    case 2:
                        using (var s = new HugeInt(seed))
                            r.Seed(s);
                        break;

                    case 3:
                        using (var s = new HugeInt(seed))
                            r.Seed(s);
                        copy = r.Copy();
                        break;

                    case 4:
                        r.Seed(seed); //todo r = copy; temporarily disabled copy tests due to MPIR bug
                        break;
                }

                assert(r);
            }

            copy.Dispose();
        }

        [TestMethod]
        public void RandomSeedingMersenneTwister()
        {
            using (var r = MpirRandom.MersenneTwister())
            {
                uint seed = 12345789;
                uint max = 10000000;
                uint bits = 62;
                uint expected1 = 3801341;
                uint expected2 = 747743;
                var expected3 = Platform.Ui(3637762780660169521, 2925722417);

                TestRandom(r, seed, x =>
                {
                    Assert.AreEqual(expected1, x.GetLimb(max));
                    Assert.AreEqual(expected2, x.GetLimb(max));
                    Assert.AreEqual(expected3, x.GetLimbBits(bits));
                });
            }
        }

        [TestMethod]
        public void RandomSeedingDefault()
        {
            using (var r = MpirRandom.Default())
            {
                uint seed = 12345789;
                uint max = 10000000;
                uint bits = 62;
                uint expected1 = 3801341;
                uint expected2 = 747743;
                var expected3 = Platform.Ui(3637762780660169521, 2925722417);

                TestRandom(r, seed, x =>
                {
                    Assert.AreEqual(expected1, x.GetLimb(max));
                    Assert.AreEqual(expected2, x.GetLimb(max));
                    Assert.AreEqual(expected3, x.GetLimbBits(bits));
                });
            }
        }

        [TestMethod]
        public void RandomSeedingLCSize()
        {
            using (var r = MpirRandom.LinearCongruential(128))
            {
                uint seed = 12345789;
                uint max = 10000000;
                uint bits = 62;
                uint expected1 = 8017343;
                uint expected2 = 2122346;
                var expected3 = Platform.Ui(1653945017297503111, 4060840839);

                TestRandom(r, seed, x =>
                {
                    Assert.AreEqual(expected1, x.GetLimb(max));
                    Assert.AreEqual(expected2, x.GetLimb(max));
                    Assert.AreEqual(expected3, x.GetLimbBits(bits));
                });
            }
        }

        [TestMethod]
        public void RandomSeedingLC()
        {
            uint seed = 12345789;
            uint max = 10000000;
            uint bits = 62;
            var expected1 = Platform.Ui(6524662, 5635868);
            var expected2 = Platform.Ui(5428780, 5488683);
            var expected3 = Platform.Ui(4189233241027086562, 278391078);
            var c = Platform.Ui(98570948725939831, 985709487);

            using (var a = new HugeInt("5209384572093847098342590872309452304529345409827509283745078"))
            using (var r = MpirRandom.LinearCongruential(a, c, 256))
            {

                TestRandom(r, seed, x =>
                {
                    Assert.AreEqual(expected1, x.GetLimb(max));
                    Assert.AreEqual(expected2, x.GetLimb(max));
                    Assert.AreEqual(expected3, x.GetLimbBits(bits));
                });
            }
        }

        [TestMethod]
        public void RandomHugeIntBits()
        {
            using (var r = MpirRandom.Default())
            using (var a = new HugeInt())
            {
                r.Seed(12345789);
                a.Value = r.GetIntBits(256);
                Assert.AreEqual("9E056474F27BEDF9AE62FB31A30B68DFA0B96F29D0C8767A88F8937D6F3A00FD", a.ToString(16));
            }
        }

        [TestMethod]
        public void RandomHugeInt1()
        {
            using (var r = MpirRandom.Default())
            using (var a = new HugeInt())
            {
                r.Seed(12345789);
                a.Value = r.GetInt((a + 2345234534) ^ 10);
                Assert.AreEqual("1157052590454954734126533665801738434624905444540864190630086248362027895885687865247828410621", a.ToString());
            }
        }

        [TestMethod]
        public void RandomHugeIntBitsChunky()
        {
            using (var r = MpirRandom.Default())
            using (var a = new HugeInt())
            {
                r.Seed(12345789);
                a.Value = r.GetIntBitsChunky(256);
                Assert.AreEqual("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000000000000000000000000000007F", a.ToString(16));
            }
        }

        [TestMethod]
        public void RandomHugeFloat()
        {
            using(var r = MpirRandom.Default())
            using(var a = HugeFloat.Allocate(256))
            {
                r.Seed(12345789);
                a.Value = r.GetFloat();
                Assert.AreEqual("0.9E056474F27BEDF9AE62FB31A30B68DFA0B96F29D0C8767A88F8937D6F3A00FD@0", a.ToString(16));
            }
        }

        [TestMethod]
        public void RandomHugeFloatBits()
        {
            using(var r = MpirRandom.Default())
            using(var a = HugeFloat.Allocate(256))
            {
                r.Seed(12345789);
                a.Value = r.GetFloatBits(128);
                Assert.AreEqual("0.A0B96F29D0C8767A88F8937D6F3A00FD@0", a.ToString(16));
            }
        }

        [TestMethod]
        public void RandomHugeFloatChunky()
        {
            using(var r = MpirRandom.Default())
            using(var a = HugeFloat.Allocate(256))
            {
                r.Seed(12345789);
                a.Value = r.GetFloatChunky(100);
                Assert.AreEqual(Platform.Select("0.7FFFFFFF0180000000000000000007FFFFFFFFFFFFFFFFFFF@-2EF", "0.7FFFFFFFFFFFF00000000001FFE000000000000000000000007FFFFFF@29"), a.ToString(16));
            }
        }

        [TestMethod]
        public void RandomHugeFloatLimbsChunky()
        {
            using(var r = MpirRandom.Default())
            using(var a = HugeFloat.Allocate(256))
            {
                r.Seed(12345789);
                a.Value = r.GetFloatLimbsChunky(128 / MpirSettings.BITS_PER_LIMB, 100);
                Assert.AreEqual(Platform.Select("0.7FFFFFF8000007FFF@2C1", "0.7FFFFFFFFFC000000003FFFFF@2A1"), a.ToString(16));
            }
        }
    }
}
