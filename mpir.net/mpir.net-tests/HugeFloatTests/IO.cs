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

namespace MPIR.Tests.HugeFloatTests
{
    [TestClass]
    public class IO
    {
        [TestMethod]
        public void FloatInputOutputStr()
        {
            using (var a = new HugeFloat("10123456789ABCDEF012345.6789ABCDE", 16))
            using (var b = HugeFloat.Allocate(12800))
            using (var ms = new MemoryStream())
            {
                a.Reallocate(12800);
                a.Value = a ^ 100;
                using (var writer = new StreamWriter(ms, Encoding.UTF8, 1024, true))
                    a.Write(writer, 10, 0, false, false);

                ms.Position = 0;

                using (var reader = new StreamReader(ms, Encoding.UTF8, false, 1024, true))
                    b.Read(reader, 10, false);

                Assert.AreEqual(a.ToString(10), b.ToString(10));
                Assert.AreEqual(ms.Length, ms.Position);
                Assert.AreEqual((char)0xFEFF + a.ToString(10), Encoding.UTF8.GetString(ms.ToArray()));
            }
        }

        [TestMethod]
        public void FloatInputOutputStrHex()
        {
            using(var a = new HugeFloat("10123456789ABCDEF012345.6789ABCDE", 16))
            using (var b = HugeFloat.Allocate(12800))
            using (var ms = new MemoryStream())
            {
                a.Reallocate(12800);
                a.Value = a ^ 100;
                using (var writer = new StreamWriter(ms, Encoding.UTF8, 1024, true))
                    a.Write(writer, 16, 0, false, false);

                ms.Position = 0;

                using (var reader = new StreamReader(ms, Encoding.UTF8, false, 1024, true))
                    b.Read(reader, 16, false);

                Assert.AreEqual(a, b);
                Assert.AreEqual(ms.Length, ms.Position);
                Assert.AreEqual((char)0xFEFF + a.ToString(16), Encoding.UTF8.GetString(ms.ToArray()));
            }
        }

        [TestMethod]
        public void FloatInputOutputStrHexLower()
        {
            using(var a = new HugeFloat("10123456789ABCDEF012345.6789ABCDE", 16))
            using(var b = HugeFloat.Allocate(12800))
            using(var ms = new MemoryStream())
            {
                a.Reallocate(12800);
                a.Value = a ^ 100;
                using(var writer = new StreamWriter(ms, Encoding.UTF8, 1024, true))
                    a.Write(writer, 16, 0, true, false);

                ms.Position = 0;

                using(var reader = new StreamReader(ms, Encoding.UTF8, false, 1024, true))
                    b.Read(reader, 16, false);

                Assert.AreEqual(a, b);
                Assert.AreEqual(ms.Length, ms.Position);
                Assert.AreEqual((char)0xFEFF + a.ToString(16, true), Encoding.UTF8.GetString(ms.ToArray()));
            }
        }

        [TestMethod]
        public void FloatInputOutputStrHexExpDecimal()
        {
            using(var a = new HugeFloat("10123456789ABCDEF012345.6789ABCDE", 16))
            using(var b = HugeFloat.Allocate(12800))
            using(var ms = new MemoryStream())
            {
                a.Reallocate(12800);
                a.Value = a ^ 100;
                using(var writer = new StreamWriter(ms, Encoding.UTF8, 1024, true))
                    a.Write(writer, 16, 0, false, true);

                ms.Position = 0;

                using(var reader = new StreamReader(ms, Encoding.UTF8, false, 1024, true))
                    b.Read(reader, 16, true);

                Assert.AreEqual(a, b);
                Assert.AreEqual(ms.Length, ms.Position);
                Assert.AreEqual((char)0xFEFF + a.ToString(16, false, true), Encoding.UTF8.GetString(ms.ToArray()));
            }
        }

        [TestMethod]
        public void FloatInputOutputStr62()
        {
            using(var a = new HugeFloat("10123456789ABCDEF012345.6789ABCDE", 16))
            using(var b = HugeFloat.Allocate(12800))
            using(var ms = new MemoryStream())
            {
                a.Reallocate(12800);
                a.Value = a ^ 100;
                using(var writer = new StreamWriter(ms, Encoding.UTF8, 1024, true))
                    a.Write(writer, 62, 0, false, false);

                ms.Position = 0;

                using(var reader = new StreamReader(ms, Encoding.UTF8, false, 1024, true))
                    b.Read(reader, 62, false);

                Assert.AreEqual(a.ToString(62), b.ToString(62));
                Assert.AreEqual(ms.Length, ms.Position);
                Assert.AreEqual((char)0xFEFF + a.ToString(62), Encoding.UTF8.GetString(ms.ToArray()));
            }
        }

        //more tests coming here
    }
}
