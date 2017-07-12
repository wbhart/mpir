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
        public void IntInputOutputRaw()
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
                Assert.IsTrue(b > 0);
                Assert.AreEqual(ms.Length, ms.Position);
            }
        }

        [TestMethod]
        public void IntInputOutputRawNegative()
        {
            using (var a = new HugeInt("0x10123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"))
            using (var b = new HugeInt())
            using (var ms = new MemoryStream())
            {
                a.Value = -(a ^ 100);
                a.Write(ms);
                ms.Position = 0;
                b.Read(ms);
                Assert.AreEqual(a, b);
                Assert.IsTrue(b < 0);
                Assert.AreEqual(ms.Length, ms.Position);
            }
        }

        [TestMethod]
        public void IntInputOutputStr()
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
        public void IntInputOutputStrHex()
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
        public void IntInputOutputStrHexLower()
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
        public void IntInputOutputStrOctal()
        {
            using (var a = new HugeInt("0x10123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"))
            using (var b = new HugeInt())
            using (var ms = new MemoryStream())
            {
                a.Value = a ^ 100;
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
        public void IntInputOutputStrBinary()
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
        public void IntInputOutputStr62()
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
        public void IntImportExport()
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
        public void IntImportExportAllocating()
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
                                b.Import(bytes, (uint)(bytes.Length / size), size, order, endianness, nails);
                                Assert.AreEqual(a, b);
                            }
            }
        }

        [TestMethod]
        public void IntImportExportAllocatingShort()
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
                                b.Import(bytes, (uint)(bytes.Length * 2 / size), size, order, endianness, nails);
                                Assert.AreEqual(a, b);
                            }
            }
        }

        [TestMethod]
        public void IntImportExportAllocatingZero()
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
        public void IntSize()
        {
            using (var a = new HugeInt("-0x10123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"))
            {
                var limbs = 192U / MpirSettings.BITS_PER_LIMB + 1;
                Assert.AreEqual(limbs, a.Size());
                Assert.AreEqual(limbs, (-a).Size());
            }
        }

        [TestMethod]
        public void IntAllocatedSize()
        {
            using (var a = new HugeInt("-0x10123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"))
            {
                var allocated = a.AllocatedSize;
                Assert.IsTrue(allocated >= (int)a.Size());
                a.Value = -a;
                Assert.AreEqual(allocated, a.AllocatedSize);
                Assert.AreEqual(192U / MpirSettings.BITS_PER_LIMB + 1, a.Size());

                a.Value >>= 64;
                Assert.AreEqual(128U / MpirSettings.BITS_PER_LIMB + 1, a.Size());
                Assert.AreEqual(allocated, a.AllocatedSize);
            }
        }

        [TestMethod]
        public void IntGetLimb()
        {
            using (var a = new HugeInt("-0x10123456789ABCDEFA123456789ABCDEF0123456789ABCDEF"))
            {
                Assert.AreEqual(Platform.Ui(0x0123456789ABCDEFUL, 0x89ABCDEFU), a.GetLimb(0));
                Assert.AreEqual(Platform.Ui(0xA123456789ABCDEFUL, 0x01234567U), a.GetLimb(1));
            }
        }

        [TestMethod]
        public void IntReadLimbs()
        {
            var dest = Enumerable.Repeat(Platform.Ui(0, 0), 8).ToArray();

            using (var a = new HugeInt("-0x55533123456789ABCDEF02468ACEFDB9753171122334455667788"))
            {
                a.ReadLimbs(dest, 1, 2, 3);
                a.ReadLimbs(dest, 2, 2, 6);
                Assert.AreEqual(Platform.Ui(0x2468ACEFDB975317UL, 0x11223344U), dest[3]);
                Assert.AreEqual(Platform.Ui(0x123456789ABCDEF0UL, 0xDB975317U), dest[4]);
                Assert.AreEqual(Platform.Ui(0x123456789ABCDEF0UL, 0xDB975317U), dest[6]);
                Assert.AreEqual(Platform.Ui(0x0000000000055533UL, 0x2468ACEFU), dest[7]);
            }
        }

        [TestMethod]
        public void IntModifyLimbs()
        {
            var src = new[]
                {
                    Platform.Ui(0, 0),
                    Platform.Ui(0, 0),
                    Platform.Ui(0, 0),
                    Platform.Ui(0x2468ACEFDB975317UL, 0x11223344U),
                    Platform.Ui(0x3456789ABCDEF044UL, 0x09872458U),
                    Platform.Ui(0, 0),
                    Platform.Ui(0x123456789ABCDEF0UL, 0xDB975317U),
                    Platform.Ui(0x0000000000055533UL, 0x2468ACEFU),
                };

            using (var a = new HugeInt(Platform.Select("0x1122334455667788", "0x55667788")))
            using (var expected1 = new HugeInt(Platform.Select(      "0x3456789ABCDEF0442468ACEFDB9753171122334455667788",          "0x098724581122334455667788")))
            using (var expected2 = new HugeInt(Platform.Select("-0x55533123456789ABCDEF02468ACEFDB9753171122334455667788", "-0x2468ACEFDB9753171122334455667788")))
            {
                a.ModifyLimbs(src, 1, 2, 3, false);
                Assert.AreEqual(expected1, a);

                a.ModifyLimbs(src, 2, 2, 6, true);
                Assert.AreEqual(expected2, a);
            }
        }

        [TestMethod]
        public void IntModifyLimbsWithGap()
        {
            var src = new[]
                {
                    Platform.Ui(0, 0),
                    Platform.Ui(0, 0),
                    Platform.Ui(0, 0),
                    Platform.Ui(0x2468ACEFDB975317UL, 0x11223344U),
                    Platform.Ui(0x3456789ABCDEF044UL, 0x09872458U),
                    Platform.Ui(0, 0),
                    Platform.Ui(0x123456789ABCDEF0UL, 0xDB975317U),
                    Platform.Ui(0x0000000000055533UL, 0x2468ACEFU),
                };

            using (var a = new HugeInt(Platform.Select("0x1122334455667788", "0x55667788")))
            using (var expected = new HugeInt(Platform.Select("0x3456789ABCDEF0442468ACEFDB97531700000000000000001122334455667788", "0x09872458112233440000000055667788")))
            {
                a.ModifyLimbs(src, 2, 2, 3, false);
                Assert.AreEqual(expected, a);
            }
        }

        [TestMethod]
        public void IntWriteLimbs()
        {
            var src = new[]
                {
                    Platform.Ui(0, 0),
                    Platform.Ui(0, 0),
                    Platform.Ui(0, 0),
                    Platform.Ui(0x2468ACEFDB975317UL, 0x11223344U),
                    Platform.Ui(0x3456789ABCDEF044UL, 0x09872458U),
                    Platform.Ui(0, 0),
                    Platform.Ui(0x123456789ABCDEF0UL, 0xDB975317U),
                    Platform.Ui(0x0000000000055533UL, 0x2468ACEFU),
                };

            using (var a = new HugeInt(Platform.Select("0x1122334455667788", "0x55667788")))
            using (var expected = new HugeInt(Platform.Select("-0x123456789ABCDEF000000000000000003456789ABCDEF0442468ACEFDB975317", "-0xDB975317000000000987245811223344")))
            {
                a.WriteLimbs(src, 3, 4, false);
                Assert.IsTrue(-expected == a);

                a.WriteLimbs(src, 3, 4, true);
                Assert.AreEqual(expected, a);
            }
        }
        //more tests coming here
    }
}
