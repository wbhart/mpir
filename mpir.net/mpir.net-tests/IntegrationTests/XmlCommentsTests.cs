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
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Serialization;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace MPIR.Tests.HugeIntTests
{
    [TestClass]
    public class XmlCommentsTests
    {
        private static readonly string[] ValidMemberTypePrefixes = { "M:", "P:", "F:", "C:" };

        [TestMethod]
        public void TestComments()
        {
            var assembly = typeof(HugeInt).Assembly;
            var xmlPath = Path.GetFileNameWithoutExtension(assembly.Location) + ".xml";
            var xml = XmlCommentsDoc.Deserialize(xmlPath);

            var publicTypes = assembly.GetTypes().Where(x => x.IsPublic).ToArray();
            var definedClassComments = xml.Members.Where(x => x.Name.StartsWith("T:") && ("" + x.Summary).Trim().Length > 0);
            var missingPublicTypes = publicTypes.Select(x => x.FullName).Except(definedClassComments.Select(x => x.MemberName)).ToArray();
            Assert.AreEqual(0, missingPublicTypes.Length, "XML Comments are not defined on class(es): " + string.Join(Environment.NewLine, missingPublicTypes));

            var publicMembers = publicTypes.SelectMany(x => x.GetMembers())
                .Where(XmlCommentsMember.IsIncluded)
                .ToArray();
            var definedMemberComments = xml.Members.Where(x => ValidMemberTypePrefixes.Contains(x.Name.Substring(0, 2)) && ("" + x.Summary).Trim().Length > 0).ToArray();
            var invalidComments = publicMembers.Join(definedMemberComments, XmlCommentsMember.Signature, x => x.MemberName, (Member, Comment) => new { Member, Comment })
                .SelectMany(x => x.Comment.Validate(x.Member)).ToArray();
            Assert.AreEqual(0, invalidComments.Length, string.Join(Environment.NewLine, invalidComments));

            var missingComments = publicMembers.Select(XmlCommentsMember.Signature).Except(definedMemberComments.Select(x => x.MemberName)).ToArray();
            Assert.AreEqual(0, missingComments.Length, "XML Comments are not defined on public member(s): " + string.Join(Environment.NewLine, missingComments));
        }
    }

    [Serializable]
    [XmlRoot("doc")]
    public class XmlCommentsDoc
    {
        [XmlElement("assembly")]
        public string Assembly { get; set; }
        [XmlArray("members"), XmlArrayItem("member")]
        public List<XmlCommentsMember> Members { get; set; }

        public static XmlCommentsDoc Deserialize(string path)
        {
            var contents = File.ReadAllText(path);
            contents = Regex.Replace(contents, "<paramref[^>]*>", "paramref");
            contents = Regex.Replace(contents, "</?para>", "");
            contents = contents.Replace("!System.Runtime.CompilerServices.IsLong", "");

            var serializer = new XmlSerializer(typeof(XmlCommentsDoc));
            using (var reader = new StringReader(contents))
                return (XmlCommentsDoc)serializer.Deserialize(reader);
        }
    }

    [Serializable]
    public class XmlCommentsMember
    {
        [XmlAttribute("name")]
        public string Name { get; set; }
        [XmlElement("summary")]
        public string Summary { get; set; }
        [XmlElement("typeparam")]
        public List<XmlCommentsParam> TypeParams { get; set; }
        [XmlElement("param")]
        public List<XmlCommentsParam> Params { get; set; }
        [XmlElement("returns")]
        public string Returns { get; set; }

        [XmlIgnore]
        public string MemberName { get { return Name.Substring(2); } }

        public IEnumerable<string> Validate(MemberInfo member)
        {
            if (("" + Summary).Trim().Length == 0)
                yield return "Missing Summary on " + MemberName;

            var method = member as MethodBase;
            if (method != null)
            {
                var missingParams = method.GetParameters().Select(x => x.Name).Except(Params.Where(x => ("" + x.Text).Trim().Length > 0).Select(x => x.Name)).ToArray();
                if (missingParams.Any())
                    yield return string.Format("Missing param {0} on {1}", string.Join(", ", missingParams), MemberName);

                var invalidParams = Params.Select(x => x.Name).Except(method.GetParameters().Select(x => x.Name)).ToArray();
                if (invalidParams.Any())
                    yield return string.Format("Invalid param {0} on {1}", string.Join(", ", invalidParams), MemberName);

                if (method.ContainsGenericParameters)
                {
                    var missingGenericParms = method.GetGenericArguments().Select(x => x.Name).Except(TypeParams.Where(x => ("" + x.Text).Trim().Length > 0).Select(x => x.Name)).ToArray();
                    if (missingGenericParms.Any())
                        yield return string.Format("Missing generic param {0} on {1}", string.Join(", ", missingGenericParms), MemberName);

                    var invalidGenericParams = TypeParams.Select(x => x.Name).Except(method.GetGenericArguments().Select(x => x.Name)).ToArray();
                    if (invalidGenericParams.Any())
                        yield return string.Format("Invalid generic param {0} on {1}", string.Join(", ", invalidGenericParams), MemberName);
                }
            }

            var method2 = member as MethodInfo;
            if (method2 != null)
            {
                if (method2.ReturnType != typeof(void) && ("" + Returns).Trim().Length == 0)
                    yield return "Missing Returns on " + MemberName;
            }
        }

        public static bool IsIncluded(MemberInfo member)
        {
            var method = member as MethodInfo;
            if (method != null)
            {
                if (method.Name.StartsWith("get_") || method.Name.StartsWith("set_"))
                    return false;

                if (method.ReflectedType != method.DeclaringType)
                    return false;
            }

            if (member.Name == "value__" && member.DeclaringType.IsEnum)
                return false;

            return true;
        }

        public static string Signature(MemberInfo member)
        {
            var sig = new StringBuilder();
            sig.Append(member.DeclaringType.FullName).Append('.');

            var method = member as MethodInfo;
            var ctor = member as ConstructorInfo;
            var methodBase = member as MethodBase;

            if (ctor != null)
                sig.Append("#ctor");
            else
                sig.Append(member.Name);

            if(methodBase != null)
            {
                if (methodBase.IsGenericMethodDefinition)
                {
                    sig.Append("``").Append(methodBase.GetGenericArguments().Length);
                }

                var parameters = methodBase.GetParameters();
                if(parameters.Length > 0)
                {
                    sig.Append('(')
                       .Append(string.Join(",", parameters.Select(FormatParameterType)))
                       .Append(')');
                }
            }

            return sig.ToString();
        }

        public static string FormatParameterType(ParameterInfo p)
        {
            if (p.ParameterType.IsGenericType)
                return p.ParameterType.Namespace + "." + p.ParameterType.Name + "{" + string.Join(",", p.ParameterType.GetGenericArguments().Select(x => x.FullName)) + "}";

            if (p.ParameterType.FullName == null)
                return "``" + p.ParameterType.Name.Replace((p.Member as MethodInfo).GetGenericArguments()[p.Position].Name, p.Position.ToString());

            if (p.IsOut)
                return p.ParameterType.FullName.Replace('&', '@');

            return p.ParameterType.FullName;
        }

        public override string ToString()
        {
            return MemberName;
        }
    }

    [Serializable]
    public class XmlCommentsParam
    {
        [XmlAttribute("name")]
        public string Name { get; set; }
        [XmlText]
        public string Text { get; set; }
    }
}
