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
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace MPIR.Tests.HugeFloatTests
{
    [TestClass]
    public class ExpressionTests
    {
        [ClassInitialize]
        public static void Setup(TestContext context)
        {
            HugeFloat.DefaultPrecision = 64;
        }

        [TestMethod]
        public void FloatTestAllExpressions()
        {
            var baseExpr = typeof(FloatExpression);
            var allExpressions =
                baseExpr.Assembly.GetTypes()
                .Where(x => baseExpr.IsAssignableFrom(x) && !x.IsAbstract)
                .ToList();

            var one = Platform.Ui(1UL, 1U);

            using (var a = new HugeFloat(-9))
            using (var b = new HugeFloat(4))
            using (var c = new HugeInt(3))
            using(var r = MpirRandom.Default())
            {
                var expr = a + (-a * 2) * 3 * (a.Abs() * -2 + -64 + a * a) + (one * 116U) + a;
                VerifyPartialResult(r, expr, 44);
                expr = expr + a * 5 + (a+b) * (b + 1) * (b + -3) * b + (b * -a) - (b * (one * 25U)) - a + (b << 3) - ((a*b) << 1);
                VerifyPartialResult(r, expr, -52);
                expr = expr - 2 - 3U + (b - (a << 1)) + (b * b - 15U) * (b - a) * (a - 11) * (b - (one * 3U)) - (-340 - a) + ((one * 20U) - b);
                VerifyPartialResult(r, expr, 52);
                expr = expr + (-7 - 2 * a) + (28U - 4 * b) + -(a + b * 2) + (3 * a).Abs();
                VerifyPartialResult(r, expr, 103);
                expr = 36 * (expr / a + expr / (3 * b) - a / b) - b / (a + 10) + 6;
                VerifyPartialResult(r, expr, -20);
                expr = expr + (b >> 1) + ((b / -7) + (a / (one * 7U))) * 7 + (7 / a) - ((one * 2U) / (b + 5));
                VerifyPartialResult(r, expr, -32);
                expr = expr - (b + 13 + 64) / a / -3;
                VerifyPartialResult(r, expr, -35);
                expr = expr + b.SquareRoot() + HugeFloat.SquareRoot(25) + ((b - 2) ^ 3) - (-b).RelativeDifferenceFrom(a + 1);
                VerifyPartialResult(r, expr, -19);
                expr = expr - (a / 4).Floor() + (b / 3).Ceiling() - (a / b).Truncate();
                VerifyPartialResult(r, expr, -12);
                expr = expr + (r.GetFloatBits(64) * 10).Ceiling();
                VerifyPartialResult(r, expr, -10);
                //float random generation seems to give different results in Win32 and Win64.  Thus, we're having to adjust the results for Win32.
                expr = expr + (r.GetFloatLimbsChunky(128 / MpirSettings.BITS_PER_LIMB, 256 / MpirSettings.BITS_PER_LIMB) << 233 >> Platform.Ui(0, 480)).Ceiling();
                VerifyPartialResult(r, expr, -6);
                expr = expr + (r.GetFloat() * 10).Floor() - Platform.Ui(0, 3);
                VerifyPartialResult(r, expr, -2);
                expr = expr + (r.GetFloatChunky(3) << 101 >> Platform.Ui(177, 23)).Truncate();
                VerifyPartialResult(r, expr, 13);

                MarkExpressionsUsed(allExpressions, expr);
            }

            Assert.AreEqual(0, allExpressions.Count, "Expression types not exercised: " + string.Join("",
                allExpressions.Select(x => Environment.NewLine + x.Name).OrderBy(x => x)));
        }

        private void VerifyPartialResult(MpirRandom rnd, FloatExpression expr, long expected)
        {
            rnd.Seed(123);

            using (var r = new HugeFloat())
            {
                using (var exp = new HugeFloat(expected))
                using (var epsilon = new HugeFloat("0.001"))
                {
                    r.Value = expr;
                    Assert.IsTrue(r - epsilon < exp && r + epsilon > exp, "Expected {0}, Actual {1}", exp, r);
                }
            }
        }

        private void MarkExpressionsUsed(List<Type> allExpressions, FloatExpression expr)
        {
            var type = expr.GetType();
            allExpressions.Remove(type);
            
            var children = type.GetFields(BindingFlags.NonPublic | BindingFlags.Instance)
                .Where(x => typeof(FloatExpression).IsAssignableFrom(x.FieldType))
                .Select(x => (FloatExpression)x.GetValue(expr))
                .Where(x => x != null)
                .ToList();

            foreach (var childExpr in children)
                MarkExpressionsUsed(allExpressions, childExpr);
        }
    }
}
