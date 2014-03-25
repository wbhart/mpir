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
        #region PowerMod

        [TestMethod]
        public void PowerModHugeInt()
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
        public void PowerModLimb()
        {
            using (var a = new HugeInt("2835698356928736487698769283645293409781234"))
            using (var c = new HugeInt("9786459872639458729387304958673243509870923452745892673402935742456"))
            {
                a.Value = a.PowerMod(3, c);
                Assert.AreEqual("5346078446724436806099093819990997994355321605000165187447171753448", a.ToString());
            }
        }

        #endregion
    }
}
