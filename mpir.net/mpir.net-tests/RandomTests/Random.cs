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
        private void TestRandom(MpirRandom r, ulong seed, Action<MpirRandom> assert)
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
                        r.Seed(seed); //r = copy; temporarily disabled copy tests due to MPIR bug
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
                ulong seed = 12345789;
                ulong max = 10000000;
                ulong bits = 62;
                ulong expected1 = 3801341;
                ulong expected2 = 747743;
                ulong expected3 = 3637762780660169521;

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
                ulong seed = 12345789;
                ulong max = 10000000;
                ulong bits = 62;
                ulong expected1 = 3801341;
                ulong expected2 = 747743;
                ulong expected3 = 3637762780660169521;

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
                ulong seed = 12345789;
                ulong max = 10000000;
                ulong bits = 62;
                ulong expected1 = 8017343;
                ulong expected2 = 2122346;
                ulong expected3 = 1653945017297503111;

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
            using (var a = new HugeInt("5209384572093847098342590872309452304529345409827509283745078"))
            using (var r = MpirRandom.LinearCongruential(a, 98570948725939831, 256))
            {
                ulong seed = 12345789;
                ulong max = 10000000;
                ulong bits = 62;
                ulong expected1 = 6524662;
                ulong expected2 = 5428780;
                ulong expected3 = 4189233241027086562;

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
    }
}
