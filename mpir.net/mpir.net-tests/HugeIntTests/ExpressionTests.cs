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

namespace MPIR.Tests.HugeIntTests
{
    [TestClass]
    public class ExpressionTests
    {
        [TestMethod]
        public void IntTestAllExpressions()
        {
            var baseExpr = typeof(IntegerExpression);
            var allExpressions =
                baseExpr.Assembly.GetTypes()
                .Where(x => baseExpr.IsAssignableFrom(x) && !x.IsAbstract)
                .ToList();

            var one = Platform.Ui(1, 1);

            using (var a = new HugeInt(-9L))
            using (var b = new HugeInt(4L))
            using (var c = new HugeRational(6, 7))
            using (var r = MpirRandom.Default())
            {
                var expr = a + (-a * 2) * 3 * (a.Abs() * -2 + -64 + a * a) + (one * 116U) + a;
                VerifyPartialResult(r, expr, 44);
                expr = expr + a * 5 + (a+b) * (b + 1) * (b + -3) * b + (b * -a) - (b * (one * 25U)) - a + (b << 3) - ((a*b) << 1);
                VerifyPartialResult(r, expr, -52);
                expr = expr - 2 - 3U + (b - (a << 1)) + (b * b - (one * 15U)) * (b - a) * (a - 11) * (b - 3U) - (-340 - a) + ((one * 20U) - b);
                VerifyPartialResult(r, expr, 52);
                expr = expr + (-7 - 2 * a) + (28U - 4 * b) + -(a + b * 2) + (3 * a).Abs();
                VerifyPartialResult(r, expr, 103);
                expr = expr / a + expr / (3 * b) - a / b - b / (a + 10) + a % b - (3 * b) % a + a % (2 * b) - (12 * b) % (-5 * a) + (a * 4 / 8).Rounding(RoundingModes.Floor) + (b * 3 % 7).Rounding(RoundingModes.Ceiling);
                VerifyPartialResult(r, expr, -20);
                expr = expr - (a * 5).DivideExactly(a) + (b * 7 * 5432198).DivideExactly(5432198) + (b >> 1);
                VerifyPartialResult(r, expr, 5);
                expr = expr + (b ^ 3) + a.PowerMod(2, b) + (a + 6).PowerMod(b - 1, b * 5) + (a * a * a).Root(3) + (b * b).SquareRoot();
                VerifyPartialResult(r, expr, 78);
                expr = expr + ((b + 1) & -a) + (b | -a) - (b ^ a) + ~b;
                VerifyPartialResult(r, expr, 100);
                expr = expr + r.GetInt(b + 1) + r.GetIntBits(3) + r.GetIntBitsChunky(3) + (b * 2).NextPrimeCandidate(r) - b.Gcd(a - 1);
                VerifyPartialResult(r, expr, 124);
                expr = expr - a.Lcm(b * 3) - (b + 1).Lcm(2) - (-a).Invert(b + 7) - (1-a).RemoveFactors(b / 2) - HugeInt.Power(2, 3) - HugeInt.Factorial(4);
                VerifyPartialResult(r, expr, 36);
                expr = expr - HugeInt.Primorial(6) + HugeInt.Binomial(4, 2) + HugeInt.Binomial(b, 3) + HugeInt.Fibonacci(6) + HugeInt.Lucas(7);
                VerifyPartialResult(r, expr, 53);
                expr = expr + c.Numerator + c.Denominator;
                VerifyPartialResult(r, expr, 66);

                MarkExpressionsUsed(allExpressions, expr);
            }

            Assert.AreEqual(0, allExpressions.Count, "Expression types not exercised: " + string.Join("",
                allExpressions.Select(x => Environment.NewLine + x.Name).OrderBy(x => x)));
        }

        private void VerifyPartialResult(MpirRandom rnd, IntegerExpression expr, long expected)
        {
            rnd.Seed(123);

            using (var r = new HugeInt())
            {
                r.Value = expr;
                Assert.AreEqual(expected.ToString(), r.ToString());
            }
        }

        private void MarkExpressionsUsed(List<Type> allExpressions, IntegerExpression expr)
        {
            var type = expr.GetType();
            allExpressions.Remove(type);
            
            var children = type.GetFields(BindingFlags.NonPublic | BindingFlags.Instance)
                .Where(x => typeof(IntegerExpression).IsAssignableFrom(x.FieldType))
                .Select(x => (IntegerExpression)x.GetValue(expr))
                .Where(x => x != null)
                .ToList();

            foreach (var childExpr in children)
                MarkExpressionsUsed(allExpressions, childExpr);
        }
    }
}
