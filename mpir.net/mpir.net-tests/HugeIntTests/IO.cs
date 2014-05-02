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

namespace MPIR.Tests.HugeIntTests
{
    [TestClass]
    public class IO
    {
        [TestMethod]
        public void InputOutputRaw()
        {
            using (var a = new HugeInt("0x10123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"))
            using (var b = new HugeInt())
            using (var ms = new MemoryStream())
            {
                a.Value = a ^ 100;
                a.Write(ms);
                ms.Position = 0;
                b.Read(ms);
                Assert.AreEqual(a, b);
                Assert.AreEqual(ms.Length, ms.Position);
            }
        }

        [TestMethod]
        public void InputOutputStr()
        {
            using (var a = new HugeInt("0x10123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"))
            using (var b = new HugeInt())
            using (var ms = new MemoryStream())
            {
                a.Value = a ^ 100;
                using (var writer = new StreamWriter(ms, Encoding.UTF8, 1024, true))
                    a.Write(writer);

                ms.Position = 0;

                using (var reader = new StreamReader(ms, Encoding.UTF8, false, 1024, true))
                    b.Read(reader);

                Assert.AreEqual(a, b);
                Assert.AreEqual(ms.Length, ms.Position);
            }
        }

        [TestMethod]
        public void InputOutputStrHex()
        {
            using (var a = new HugeInt("0x10123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"))
            using (var b = new HugeInt())
            using (var ms = new MemoryStream())
            {
                a.Value = a ^ 100;
                using (var writer = new StreamWriter(ms, Encoding.UTF8, 1024, true))
                {
                    writer.Write("0x");
                    a.Write(writer, 16);
                }

                ms.Position = 0;

                using (var reader = new StreamReader(ms, Encoding.UTF8, false, 1024, true))
                    b.Read(reader);

                Assert.AreEqual(a, b);
                Assert.AreEqual(ms.Length, ms.Position);
                Assert.AreEqual((char)0xFEFF + "0x" + a.ToString(16), Encoding.UTF8.GetString(ms.ToArray()));
            }
        }

        [TestMethod]
        public void InputOutputStrHexLower()
        {
            using (var a = new HugeInt("0x10123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"))
            using (var b = new HugeInt())
            using (var ms = new MemoryStream())
            {
                a.Value = a ^ 100;
                using (var writer = new StreamWriter(ms, Encoding.UTF8, 1024, true))
                {
                    writer.Write("0x");
                    a.Write(writer, 16, true);
                }

                ms.Position = 0;

                using (var reader = new StreamReader(ms, Encoding.UTF8, false, 1024, true))
                    b.Read(reader);

                Assert.AreEqual(a, b);
                Assert.AreEqual(ms.Length, ms.Position);
                Assert.AreEqual((char)0xFEFF + "0x" + a.ToString(16, true), Encoding.UTF8.GetString(ms.ToArray()));
            }
        }

        [TestMethod]
        public void InputOutputStrOctal()
        {
            using (var a = new HugeInt("0x10123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"))
            using (var b = new HugeInt())
            using (var ms = new MemoryStream())
            {
                a.Value = a ^ 1;
                using (var writer = new StreamWriter(ms, Encoding.UTF8, 1024, true))
                {
                    writer.Write('0');
                    a.Write(writer, 8);
                }

                ms.Position = 0;

                using (var reader = new StreamReader(ms, Encoding.UTF8, false, 1024, true))
                    b.Read(reader);

                Assert.AreEqual(a, b);
                Assert.AreEqual(ms.Length, ms.Position);
                Assert.AreEqual((char)0xFEFF + "0" + a.ToString(8), Encoding.UTF8.GetString(ms.ToArray()));
            }
        }

        [TestMethod]
        public void InputOutputStrBinary()
        {
            using (var a = new HugeInt("0x10123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"))
            using (var b = new HugeInt())
            using (var ms = new MemoryStream())
            {
                a.Value = a ^ 100;
                using (var writer = new StreamWriter(ms, Encoding.UTF8, 1024, true))
                {
                    writer.Write("0b");
                    a.Write(writer, 2);
                }

                ms.Position = 0;

                using (var reader = new StreamReader(ms, Encoding.UTF8, false, 1024, true))
                    b.Read(reader);

                Assert.AreEqual(a, b);
                Assert.AreEqual(ms.Length, ms.Position);
                Assert.AreEqual((char)0xFEFF + "0b" + a.ToString(2), Encoding.UTF8.GetString(ms.ToArray()));
            }
        }

        [TestMethod]
        public void InputOutputStr62()
        {
            using (var a = new HugeInt("0x10123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"))
            using (var b = new HugeInt())
            using (var ms = new MemoryStream())
            {
                a.Value = a ^ 100;
                using (var writer = new StreamWriter(ms, Encoding.UTF8, 1024, true))
                    a.Write(writer, 62);

                ms.Position = 0;

                using (var reader = new StreamReader(ms, Encoding.UTF8, false, 1024, true))
                    b.Read(reader, 62);

                Assert.AreEqual(a, b);
                Assert.AreEqual(ms.Length, ms.Position);
                Assert.AreEqual((char)0xFEFF + a.ToString(62), Encoding.UTF8.GetString(ms.ToArray()));
            }
        }

        [TestMethod]
        public void ImportExport()
        {
            using (var a = new HugeInt("0x10123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"))
            using (var b = new HugeInt())
            {
                var bytes = new byte[1000];

                foreach (var order in Enum.GetValues(typeof(LimbOrder)).Cast<LimbOrder>())
                foreach (var endianness in Enum.GetValues(typeof(Endianness)).Cast<Endianness>())
                foreach (var nails in new[] { 0, 5, 10, 16 })
                foreach (var size in new[] { 8, 11, 16 })
                {
                    var words = a.Export(bytes, size, order, endianness, nails);
                    var expected = (ulong)System.Math.Ceiling(193m / (size * 8 - nails));
                    Assert.AreEqual(expected, words);

                    b.SetTo(0);
                    b.Import(bytes, words, size, order, endianness, nails);
                    Assert.AreEqual(a, b);
                }
            }
        }

        [TestMethod]
        public void ImportExportAllocating()
        {
            using (var a = new HugeInt("0x10123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"))
            using (var b = new HugeInt())
            {
                foreach (var order in Enum.GetValues(typeof(LimbOrder)).Cast<LimbOrder>())
                    foreach (var endianness in Enum.GetValues(typeof(Endianness)).Cast<Endianness>())
                        foreach (var nails in new[] { 0, 5, 10, 16 })
                            foreach (var size in new[] { 8, 11, 16 })
                            {
                                var bytes = a.Export<byte>(size, order, endianness, nails);
                                var expected = (int)System.Math.Ceiling(193m / (size * 8 - nails));
                                Assert.AreEqual(expected, bytes.Length / size);

                                b.SetTo(0);
                                b.Import(bytes, (ulong)(bytes.Length / size), size, order, endianness, nails);
                                Assert.AreEqual(a, b);
                            }
            }
        }

        [TestMethod]
        public void ImportExportAllocatingShort()
        {
            using (var a = new HugeInt("0x10123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"))
            using (var b = new HugeInt())
            {
                foreach (var order in Enum.GetValues(typeof(LimbOrder)).Cast<LimbOrder>())
                    foreach (var endianness in Enum.GetValues(typeof(Endianness)).Cast<Endianness>())
                        foreach (var nails in new[] { 0, 5, 10, 16 })
                            foreach (var size in new[] { 8, 11, 16 })
                            {
                                var bytes = a.Export<short>(size, order, endianness, nails);
                                var expected = (int)System.Math.Ceiling(193m / (size * 8 - nails));
                                Assert.AreEqual(expected, bytes.Length * 2 / size);

                                b.SetTo(0);
                                b.Import(bytes, (ulong)(bytes.Length * 2 / size), size, order, endianness, nails);
                                Assert.AreEqual(a, b);
                            }
            }
        }

        [TestMethod]
        public void ImportExportAllocatingZero()
        {
            using (var a = new HugeInt())
            using (var b = new HugeInt())
            {
                var order = LimbOrder.LeastSignificantFirst;
                var endianness = Endianness.Native;
                var nails = 5;
                var size = 4;

                var bytes = a.Export<byte>(size, order, endianness, nails);
                Assert.AreEqual(0, bytes.Length);

                b.SetTo(1);
                b.Import(bytes, 0, size, order, endianness, nails);
                Assert.AreEqual(a, b);
            }
        }

        [TestMethod]
        public void Size()
        {
            using (var a = new HugeInt("-0x10123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"))
            {
                Assert.AreEqual(4UL, a.Size());
                Assert.AreEqual(4UL, (-a).Size());
            }
        }

        [TestMethod]
        public void GetLimb()
        {
            using (var a = new HugeInt("-0x10123456789ABCDEFA123456789ABCDEF0123456789ABCDEF"))
            {
                Assert.AreEqual(0x0123456789ABCDEFUL, a.GetLimb(0));
                Assert.AreEqual(0xA123456789ABCDEFUL, a.GetLimb(1));
            }
        }
        //more tests coming here
    }
}