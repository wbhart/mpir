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
    public class MpirSettingsTests
    {
        [TestMethod]
        public void BitsPerLimb()
        {
            unsafe
            {
                Assert.AreEqual(8 * sizeof(IntPtr), MpirSettings.BITS_PER_LIMB);
                Assert.AreEqual(8 * sizeof(IntPtr), MpirSettings.USABLE_BITS_PER_LIMB);
                Assert.AreEqual(0, MpirSettings.NAIL_BITS_PER_LIMB);
            }
        }

        [TestMethod]
        public void Version()
        {
            Assert.AreEqual("6.0.0", MpirSettings.GMP_VERSION.ToString());
            Assert.AreEqual("3.0.0", MpirSettings.MPIR_VERSION.ToString());
        }

        [TestMethod]
        public void ToStringDigits()
        {
            const string strA = "9520398475029834502983475028934705982734095827304958723409758230498573034928750938475987498473958743";
            using (var a = new HugeInt(strA))
            {
                Assert.AreEqual(256, MpirSettings.ToStringDigits);
                Assert.AreEqual(strA, a.ToString());

                MpirSettings.ToStringDigits = 32;
                Assert.AreEqual("..." + strA.Substring(strA.Length - 32), a.ToString());

                MpirSettings.ToStringDigits = 256;
                Assert.AreEqual(strA, a.ToString());
            }
        }

        [TestMethod]
        public void RoundingMode()
        {
            const string strA = "9520398475029834502983475028934705982734095827304958723409758230498573034928750938475987498473958743";
            var down = strA.Substring(0, strA.Length - 1);
            var up = strA.Substring(0, strA.Length - 2) + (char) (strA[strA.Length - 2] + 1);

            using (var a = new HugeInt(strA))
            using (var b = new HugeInt())
            {
                Assert.AreEqual(RoundingModes.Truncate, MpirSettings.RoundingMode);
                b.Value = a / 10;
                Assert.AreEqual(down, b.ToString());
                b.Value = -a / 10;
                Assert.AreEqual("-" + down, b.ToString());

                MpirSettings.RoundingMode = RoundingModes.Default;
                b.Value = a / 10;
                Assert.AreEqual(down, b.ToString());
                b.Value = -a / 10;
                Assert.AreEqual("-" + down, b.ToString());

                MpirSettings.RoundingMode = RoundingModes.Floor;
                b.Value = a / 10;
                Assert.AreEqual(down, b.ToString());
                b.Value = -a / 10;
                Assert.AreEqual("-" + up, b.ToString());

                MpirSettings.RoundingMode = RoundingModes.Ceiling;
                b.Value = a / 10;
                Assert.AreEqual(up, b.ToString());
                b.Value = -a / 10;
                Assert.AreEqual("-" + down, b.ToString());

                MpirSettings.RoundingMode = RoundingModes.Truncate;
            }
        }
    }
}
