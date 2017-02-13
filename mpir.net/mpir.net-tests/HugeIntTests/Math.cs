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
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace MPIR.Tests.HugeIntTests
{
    [TestClass]
    public class Math
    {
        #region Exponentiation

        [TestMethod]
        public void IntPowerModHugeInt()
        {
            using (var a = new HugeInt("2835698356928736487698769283645293409781234"))
            using (var b = new HugeInt("3"))
            using (var c = new HugeInt("9786459872639458729387304958673243509870923452745892673402935742456"))
            {
                a.Value = a.PowerMod(b, c);
                Assert.AreEqual("5346078446724436806099093819990997994355321605000165187447171753448", a.ToString());
            }
        }

        [TestMethod]
        public void IntPowerModLimb()
        {
            using (var a = new HugeInt("2835698356928736487698769283645293409781234"))
            using (var c = new HugeInt("9786459872639458729387304958673243509870923452745892673402935742456"))
            {
                a.Value = a.PowerMod(3, c);
                Assert.AreEqual("5346078446724436806099093819990997994355321605000165187447171753448", a.ToString());
            }
        }

        [TestMethod]
        public void IntPowerLimb()
        {
            using (var a = new HugeInt("9785412309485720938412983404349"))
            {
                a.Value = a ^ 4;
                Assert.AreEqual("9168884832199547717402442404668238841010784738902226284286664833331445628675177089723224507720724521226586825967635414667601", a.ToString());
            }
        }

        #endregion

        #region Roots

        [TestMethod]
        public void IntRoot()
        {
            var n = "8984948281360922385394772450147012613851354303";
            using (var a = new HugeInt(n))
            using (var b = new HugeInt())
            using (var c = new HugeInt())
            {
                a.Value = a * a * a;

                b.Value = a.Root(3);
                Assert.AreEqual(n, b.ToString());

                bool exact = false;
                b.Value = a.Root(3).SettingExactTo(x => exact = x);
                Assert.AreEqual(n, b.ToString());
                Assert.IsTrue(exact);
                
                b.Value = (a + 123).Root(3).SettingExactTo(x => exact = x);
                Assert.AreEqual(n, b.ToString());
                Assert.IsFalse(exact);

                b.Value = (a + 123).Root(3).SavingRemainderTo(c);
                Assert.AreEqual(n, b.ToString());
                Assert.AreEqual("123", c.ToString());
            }
        }

        [TestMethod]
        public void IntSquareRoot()
        {
            var n = "8984948281360922385394772450147012613851354303";
            using (var a = new HugeInt(n))
            using (var b = new HugeInt())
            using (var c = new HugeInt())
            {
                a.Value = a * a;

                b.Value = a.SquareRoot();
                Assert.AreEqual(n, b.ToString());

                b.SetTo(0);
                b.Value = (a + 123).SquareRoot().SavingRemainderTo(c);
                Assert.AreEqual(n, b.ToString());
                Assert.AreEqual("123", c.ToString());
            }
        }

        [TestMethod]
        public void IntIsPerfectPower()
        {
            using (var a = new HugeInt("8984948281360922385394772450147012613851354303"))
            using (var b = new HugeInt())
            {
                b.Value = a * a * a;
                Assert.IsTrue(b.IsPerfectPower());
                b.Value = a * a;
                Assert.IsTrue(b.IsPerfectPower());
                b.Value = a * a * a + 1;
                Assert.IsFalse(b.IsPerfectPower());
                b.Value = a * a + 1;
                Assert.IsFalse(b.IsPerfectPower());
            }
        }

        [TestMethod]
        public void IntIsPerfectSquare()
        {
            using (var a = new HugeInt("8984948281360922385394772450147012613851354303"))
            using (var b = new HugeInt())
            {
                b.Value = a * a * a;
                Assert.IsFalse(b.IsPerfectSquare());
                b.Value = a * a;
                Assert.IsTrue(b.IsPerfectSquare());
                b.Value = a * a * a + 1;
                Assert.IsFalse(b.IsPerfectSquare());
                b.Value = a * a + 1;
                Assert.IsFalse(b.IsPerfectSquare());
            }
        }

        #endregion
        //more tests coming here
    }
}
