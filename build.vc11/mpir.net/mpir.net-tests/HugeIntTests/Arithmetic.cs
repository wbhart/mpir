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
    public class Arithmetic
    {
        [TestMethod]
        public void AddHugeInt()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            using (var b = new HugeInt("222987435987982730594288574029879874539"))
            using (var c = new HugeInt())
            {
                c.Value = a + b;
                Assert.AreEqual("445497268491433028939318409770173720259", c.ToString());
            }
        }

        [TestMethod]
        public void AddLimb()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            {
                var b = 4288574029879874539UL;
                a.Value = a + b;
                Assert.AreEqual("222509832503450298349318409770173720259", a.ToString());
            }
        }

        [TestMethod]
        public void AddToLimb()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            {
                var b = 4288574029879874539UL;
                a.Value = b + a;
                Assert.AreEqual("222509832503450298349318409770173720259", a.ToString());
            }
        }

        [TestMethod]
        public void AddSignedLimb()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            {
                var b = -4288574029879874539L;
                a.Value = a + b;
                Assert.AreEqual("222509832503450298340741261710413971181", a.ToString());
            }
        }

        [TestMethod]
        public void AddToSignedLimb()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            {
                var b = -4288574029879874539L;
                a.Value = b + a;
                Assert.AreEqual("222509832503450298340741261710413971181", a.ToString());
            }
        }

        [TestMethod]
        public void AddToMaxSignedLimb()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            {
                var b = long.MinValue;
                a.Value = b + a;
                Assert.AreEqual("222509832503450298335806463703439069912", a.ToString());
            }
        }
       
        [TestMethod]
        public void SubtractHugeInt()
        {
            using (var a = new HugeInt("445497268491433028939318409770173720259"))
            using (var b = new HugeInt("222987435987982730594288574029879874539"))
            {
                a.Value = a - b;
                Assert.AreEqual("222509832503450298345029835740293845720", a.ToString());
            }
        }

        [TestMethod]
        public void SubtractLimb()
        {
            using (var a = new HugeInt("222509832503450298349318409770173720259"))
            {
                var b = 4288574029879874539UL;
                a.Value = a - b;
                Assert.AreEqual("222509832503450298345029835740293845720", a.ToString());
            }
        }

        [TestMethod]
        public void SubtractFromLimb()
        {
            using (var a = new HugeInt("222509832503450298349318409770173720259"))
            {
                var b = 4288574029879874539UL;
                a.Value = b - a;
                Assert.AreEqual("-222509832503450298345029835740293845720", a.ToString());
            }
        }

        [TestMethod]
        public void SubtractPositiveSignedLimb()
        {
            using (var a = new HugeInt("222509832503450298349318409770173720259"))
            {
                var b = 4288574029879874539L;
                a.Value = a - b;
                Assert.AreEqual("222509832503450298345029835740293845720", a.ToString());
            }
        }

        [TestMethod]
        public void SubtractFromPositiveSignedLimb()
        {
            using (var a = new HugeInt("222509832503450298349318409770173720259"))
            {
                var b = 4288574029879874539L;
                a.Value = b - a;
                Assert.AreEqual("-222509832503450298345029835740293845720", a.ToString());
            }
        }

        [TestMethod]
        public void SubtractSignedLimb()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            {
                var b = -4288574029879874539L;
                a.Value = a - b;
                Assert.AreEqual("222509832503450298349318409770173720259", a.ToString());
            }
        }

        [TestMethod]
        public void SubtractFromSignedLimb()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            {
                var b = -4288574029879874539L;
                a.Value = b - a;
                Assert.AreEqual("-222509832503450298349318409770173720259", a.ToString());
            }
        }

        [TestMethod]
        public void MultiplyByHugeInt()
        {
            using (var a = new HugeInt("90234098723098475098479385345098345"))
            using (var b = new HugeInt("7859487359873459872354987610987897"))
            {
                a.Value = a * b;
                Assert.AreEqual("709193758343766370701419953880162061353595657143399816050772069730465", a.ToString());
            }
        }

        [TestMethod]
        public void MultiplyByLimb()
        {
            using (var a = new HugeInt("90234098723098475098479385345098345"))
            {
                ulong b = 17390538260286101342;
                a.Value = a * b;
                Assert.AreEqual("1569219546226477273686601978789044606491747469626478990", a.ToString());
            }
        }

        [TestMethod]
        public void MultiplyLimbBy()
        {
            using (var a = new HugeInt("90234098723098475098479385345098345"))
            {
                ulong b = 17390538260286101342;
                a.Value = b * a;
                Assert.AreEqual("1569219546226477273686601978789044606491747469626478990", a.ToString());
            }
        }

        [TestMethod]
        public void MultiplyBySignedLimb()
        {
            using (var a = new HugeInt("90234098723098475098479385345098345"))
            {
                long b = -7390538260286101342;
                a.Value = a * b;
                Assert.AreEqual("-666878558995492522701808125338061156491747469626478990", a.ToString());
            }
        }

        [TestMethod]
        public void MultiplySignedLimbBy()
        {
            using (var a = new HugeInt("90234098723098475098479385345098345"))
            {
                long b = -7390538260286101342;
                a.Value = b * a;
                Assert.AreEqual("-666878558995492522701808125338061156491747469626478990", a.ToString());
            }
        }

        [TestMethod]
        public void AddProductHugeInt()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("23094582093845093574093845093485039450934"))
            using (var b = new HugeInt("-394580293847502987609283945873594873409587"))
            {
                var expr = a + c*b;
                a.Value = expr;
                Assert.AreEqual("-9112666988874677841199955832262586145147830205230375090322356322089362221491205901", a.ToString());
            }
        }

        [TestMethod]
        public void AddProductLimb()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            {
                ulong b = 498734523097853458;
                var expr = a + c*b;
                a.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void AddProductLimbTo()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            {
                ulong b = 498734523097853458;
                var expr = a + b*c;
                a.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void AddProductLimbTo2()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            {
                ulong b = 498734523097853458;
                var expr = b*c + a;
                a.Value = expr;
                //TODO how can we test a single addmul was called?
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void AddProductLimbTo3()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            using (var d = new HugeInt())
            {
                ulong b = 498734523097853458;
                var expr = b*c + a;
                d.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", d.ToString());
            }
        }

        [TestMethod]
        public void AddProductSignedLimb()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("23094582093845093574093845093485039450934"))
            {
                long b = -498734523097853458;
                var expr = a + c*b;
                a.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void AddProductSignedLimbTo()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            {
                long b = 498734523097853458;
                var expr = a + b*c;
                a.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void AddProductSignedLimbTo2()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            {
                long b = 498734523097853458;
                var expr = b*c + a;
                a.Value = expr;
                //TODO how can we test a single addmul was called?
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void AddProductSignedLimbTo3()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("23094582093845093574093845093485039450934"))
            using (var d = new HugeInt())
            {
                long b = -498734523097853458;
                var expr = b*c + a;
                d.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", d.ToString());
            }
        }

        [TestMethod]
        public void SubtractProductHugeInt()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("23094582093845093574093845093485039450934"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                var expr = a - c*b;
                a.Value = expr;
                Assert.AreEqual("-9112666988874677841199955832262586145147830205230375090322356322089362221491205901", a.ToString());
            }
        }

        [TestMethod]
        public void SubtractProductLimb()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("23094582093845093574093845093485039450934"))
            {
                ulong b = 498734523097853458;
                var expr = a - c*b;
                a.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void SubtractProductSignedLimb()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            {
                long b = -498734523097853458;
                var expr = a - b*c;
                a.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void SubtractProductSignedLimb2()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            {
                long b = -498734523097853458;
                a.Value = a - c*b;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void SubtractProductSignedLimb3()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("23094582093845093574093845093485039450934"))
            using (var d = new HugeInt())
            {
                long b = 498734523097853458;
                d.Value = a - c*b;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", d.ToString());
            }
        }

        [TestMethod]
        public void ShiftLeft()
        {
            using (var a = new HugeInt("-12345700987ABCDEF2345CBDEFA245230948", 16))
            {
                ulong b = 40;
                a.Value = a << b;
                Assert.AreEqual("-12345700987ABCDEF2345CBDEFA2452309480000000000", a.ToString(-16));
            }
        }

        [TestMethod]
        public void Negate()
        {
            using (var a = new HugeInt("24092854092874502983745029345723098457209"))
            {
                a.Value = -a;
                Assert.AreEqual("-24092854092874502983745029345723098457209", a.ToString());
                a.Value = -a;
                Assert.AreEqual("24092854092874502983745029345723098457209", a.ToString());
            }
        }

        [TestMethod]
        public void MakeAbsolute()
        {
            using (var a = new HugeInt("-24092854092874502983745029345723098457209"))
            {
                a.Value = a.Abs();
                Assert.AreEqual("24092854092874502983745029345723098457209", a.ToString());
                a.Value = a.Abs();
                Assert.AreEqual("24092854092874502983745029345723098457209", a.ToString());
            }
        }

        [TestMethod]
        public void AddExpressionHugeInt()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            using (var b = new HugeInt("222987435987982730594288574029879874539"))
            using (var c = new HugeInt())
            {
                c.Value = 1 + (a + b);
                Assert.AreEqual("445497268491433028939318409770173720260", c.ToString());
            }
        }
        //more tests coming here
    }
}
