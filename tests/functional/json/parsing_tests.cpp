/***
 * Copyright (C) Microsoft. All rights reserved.
 * Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
 *
 * =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *
 * Tests for JSON parsing.
 *
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 ****/

#include "stdafx.h"

#include <array>
#include <iomanip>

#if defined(_WIN32) || defined(__APPLE__)
#include <regex>
#elif (defined(ANDROID) || defined(__ANDROID__))
#else
// GCC 4.8 doesn't support regex very well, fall back to Boost. Revist in GCC 4.9.
#include <boost/regex.hpp>
#endif

using namespace web;
using namespace utility;
using namespace utility::conversions;

namespace tests
{
namespace functional
{
namespace json_tests
{
inline bool verify_parsing_error_msg(const std::string& str)
{
#if defined(_WIN32) || defined(__APPLE__)
    auto spattern = "^\\* Line \\d+, Column \\d+ Syntax error: .+";
    static std::regex pattern(spattern);
    return std::regex_match(str, pattern, std::regex_constants::match_flag_type::match_not_null);
#elif (defined(ANDROID) || defined(__ANDROID__))
    return str.find("Syntax error: ") != std::string::npos;
#else
    auto spattern = "^\\* Line \\d+, Column \\d+ Syntax error: .+";
    static boost::regex pattern(spattern);
    return boost::regex_match(str, pattern, boost::regex_constants::match_flag_type::match_not_null);
#endif
}

#if defined(_MSC_VER)
#pragma warning(disable : 4127) // const expression
#endif
#define VERIFY_PARSING_THROW(target)                                                                                   \
    do                                                                                                                 \
    {                                                                                                                  \
        try                                                                                                            \
        {                                                                                                              \
            target;                                                                                                    \
            VERIFY_IS_TRUE(false);                                                                                     \
        }                                                                                                              \
        catch (const json::json_exception& e)                                                                          \
        {                                                                                                              \
            VERIFY_IS_TRUE(verify_parsing_error_msg(e.what()));                                                        \
        }                                                                                                              \
        catch (...)                                                                                                    \
        {                                                                                                              \
            VERIFY_IS_TRUE(false);                                                                                     \
        }                                                                                                              \
    } while (false)

SUITE(parsing_tests)
{
    TEST(stringstream_t)
    {
        utility::stringstream_t ss0;
        ss0 << __U("null");
        json::value v0 = json::value::parse(ss0);

        utility::stringstream_t ss1;
        ss1 << __U("17");
        json::value v1 = json::value::parse(ss1);

        utility::stringstream_t ss2;
        ss2 << __U("3.1415");
        json::value v2 = json::value::parse(ss2);

        utility::stringstream_t ss3;
        ss3 << __U("true");
        json::value v3 = json::value::parse(ss3);

        utility::stringstream_t ss4;
        ss4 << __U("\"Hello!\"");
        json::value v4 = json::value::parse(ss4);

        utility::stringstream_t ss8;
        ss8 << __U("{ \"a\" : 10 }");
        json::value v8 = json::value::parse(ss8);

        utility::stringstream_t ss9;
        ss9 << __U("[1,2,3,true]");
        json::value v9 = json::value::parse(ss9);

        VERIFY_ARE_EQUAL(v1.type(), json::value::Number);
        VERIFY_ARE_EQUAL(v2.type(), json::value::Number);
        VERIFY_ARE_EQUAL(v3.type(), json::value::Boolean);
        VERIFY_ARE_EQUAL(v4.type(), json::value::String);
        VERIFY_ARE_EQUAL(v8.type(), json::value::Object);
        VERIFY_ARE_EQUAL(v9.type(), json::value::Array);
    }

    TEST(whitespace_failure) { VERIFY_PARSING_THROW(json::value::parse(__U("  "))); }

    static const std::array<char, 4> whitespace_chars = {{0x20, 0x09, 0x0A, 0x0D}};

    TEST(whitespace_array)
    {
        // Try all the whitespace characters before/after all the structural characters
        // whitespace characters according to RFC4627: space, horizontal tab, line feed or new line, carriage return
        // structural characters: [{]}:,

        // [,]
        for (auto ch : whitespace_chars)
        {
            utility::string_t input;
            input.append(2, ch);
            input.append(__U("["));
            input.append(2, ch);
            input.append(__U("1"));
            input.append(1, ch);
            input.append(__U(","));
            input.append(4, ch);
            input.append(__U("2"));
            input.append(1, ch);
            input.append(__U("]"));
            input.append(2, ch);
            json::value val = json::value::parse(input);
            VERIFY_IS_TRUE(val.is_array());
            VERIFY_ARE_EQUAL(__U("1"), val[0].serialize());
            VERIFY_ARE_EQUAL(__U("2"), val[1].serialize());
        }
    }

    TEST(whitespace_object)
    {
        // {:}
        for (auto ch : whitespace_chars)
        {
            utility::string_t input;
            input.append(2, ch);
            input.append(__U("{"));
            input.append(2, ch);
            input.append(__U("\"1\""));
            input.append(1, ch);
            input.append(__U(":"));
            input.append(4, ch);
            input.append(__U("2"));
            input.append(1, ch);
            input.append(__U("}"));
            input.append(2, ch);
            json::value val = json::value::parse(input);
            VERIFY_IS_TRUE(val.is_object());
        VERIFY_ARE_EQUAL(__U("2"), val[__U("1"]).serialize());
        }
    }

    TEST(string_t)
    {
        json::value str = json::value::parse(__U("\"\\\"\""));
        VERIFY_ARE_EQUAL(__U("\""), str.as_string());

        str = json::value::parse(__U("\"\""));
        VERIFY_ARE_EQUAL(__U(""), str.as_string());

        str = json::value::parse(__U("\"\\\"ds\""));
        VERIFY_ARE_EQUAL(__U("\"ds"), str.as_string());

        str = json::value::parse(__U("\"\\\"\\\"\""));
        VERIFY_ARE_EQUAL(__U("\"\""), str.as_string());

        // two character escapes
        str = json::value::parse(__U("\"\\\\\""));
        VERIFY_ARE_EQUAL(__U("\\"), str.as_string());

        str = json::value::parse(__U("\"\\/\""));
        VERIFY_ARE_EQUAL(__U("/"), str.as_string());

        str = json::value::parse(__U("\"\\b\""));
        VERIFY_ARE_EQUAL(__U("\b"), str.as_string());

        str = json::value::parse(__U("\"\\f\""));
        VERIFY_ARE_EQUAL(__U("\f"), str.as_string());

        str = json::value::parse(__U("\"\\n\""));
        VERIFY_ARE_EQUAL(__U("\n"), str.as_string());

        str = json::value::parse(__U("\"\\r\""));
        VERIFY_ARE_EQUAL(__U("\r"), str.as_string());

        str = json::value::parse(__U("\"\\t\""));
        VERIFY_ARE_EQUAL(__U("\t"), str.as_string());
    }

    TEST(escaped_unicode_string)
    {
        auto str = json::value::parse(__U("\"\\u0041\""));
        VERIFY_ARE_EQUAL(__U("A"), str.as_string());

        str = json::value::parse(__U("\"\\u004B\""));
        VERIFY_ARE_EQUAL(__U("K"), str.as_string());

        str = json::value::parse(__U("\"\\u20AC\""));
        // Euro sign as a hexidecmial UTF-8
        const auto euro = to_string_t("\xE2\x82\xAC");
        VERIFY_ARE_EQUAL(euro, str.as_string());

        VERIFY_PARSING_THROW(json::value::parse(__U("\"\\u0klB\"")));
    }

    TEST(escaping_control_characters)
    {
        std::vector<int> chars;
        for (int i = 0; i <= 0x1F; ++i)
        {
            chars.push_back(i);
        }
        chars.push_back(0x5C); // backslash '\'
        chars.push_back(0x22); // quotation '"'

        for (int i : chars)
        {
            utility::stringstream_t ss;
            ss << __U("\"\\u") << std::uppercase << std::setfill(__U('0')) << std::setw(4) << std::hex << i << __U("\"");
            const auto& str = ss.str();
            auto expectedStr = str;
            if (i == 0x08)
            {
                expectedStr = __U("\"\\b\"");
            }
            else if (i == 0x09)
            {
                expectedStr = __U("\"\\t\"");
            }
            else if (i == 0x0A)
            {
                expectedStr = __U("\"\\n\"");
            }
            else if (i == 0x0C)
            {
                expectedStr = __U("\"\\f\"");
            }
            else if (i == 0x0D)
            {
                expectedStr = __U("\"\\r\"");
            }
            else if (i == 0x5C)
            {
                expectedStr = __U("\"\\\\\"");
            }
            else if (i == 0x22)
            {
                expectedStr = __U("\"\\\"\"");
            }

            // Try constructing a json string value directly.
            utility::string_t schar;
            schar.push_back(static_cast<utility::string_t::value_type>(i));
            const auto& sv = json::value::string(schar);
            VERIFY_ARE_EQUAL(expectedStr, sv.serialize());

            // Try parsing a string
            const auto& v = json::value::parse(str);
            VERIFY_IS_TRUE(v.is_string());
            VERIFY_ARE_EQUAL(expectedStr, v.serialize());

            // Try parsing a stringstream.
            const auto& ssv = json::value::parse(ss);
            VERIFY_ARE_EQUAL(expectedStr, ssv.serialize());
        }
    }

    TEST(comments_string)
    {
        // Nothing but a comment
        VERIFY_PARSING_THROW(json::value::parse(__U(" /* There's nothing but a comment here */  ")));
        VERIFY_PARSING_THROW(json::value::parse(__U(" // There's nothing but a comment here\n")));

        // Some invalid comments
        VERIFY_PARSING_THROW(json::value::parse(__U(" -22 /*/")));
        VERIFY_PARSING_THROW(json::value::parse(__U(" -22 /* /* nested */ */")));

        // Correctly placed comments
        json::value num1 = json::value::parse(__U("-22 // This is a trailing comment\n"));
        VERIFY_ARE_EQUAL(-22, num1.as_double());
        num1 = json::value::parse(__U(" -22 /* This is a trailing comment with a // nested\n comment */"));
        VERIFY_ARE_EQUAL(-22, num1.as_double());
        json::value num2 = json::value::parse(__U("// This is a leading comment\n -22"));
        VERIFY_ARE_EQUAL(-22, num2.as_double());
        json::value num3 = json::value::parse(__U("-22 /* This is a trailing comment */"));
        VERIFY_ARE_EQUAL(-22, num3.as_double());
        json::value num4 = json::value::parse(__U("/* This is a leading comment */ -22"));
        VERIFY_ARE_EQUAL(-22, num4.as_double());
        json::value num5 = json::value::parse(__U("-22 /***/"));
        VERIFY_ARE_EQUAL(-22, num5.as_double());

        json::value obj1 = json::value::parse(__U("{// A comment in the middle of an empty object\n}"));
        VERIFY_IS_TRUE(obj1.is_object());
        VERIFY_ARE_EQUAL(0u, obj1.size());
        json::value obj2 = json::value::parse(__U("{/* A comment in the middle of an empty object */}"));
        VERIFY_IS_TRUE(obj2.is_object());
        VERIFY_ARE_EQUAL(0u, obj2.size());
        json::value obj3 = json::value::parse(__U("{ \"test\" : // A comment in the middle of a non-empty object\n 2}"));
        VERIFY_IS_TRUE(obj3.is_object());
        VERIFY_ARE_EQUAL(1u, obj3.size());
        json::value obj4 = json::value::parse(__U("{ \"test\" : /* A comment in the middle of a non-empty object */ 2}"));
        VERIFY_IS_TRUE(obj4.is_object());
        VERIFY_ARE_EQUAL(1u, obj4.size());

        json::value arr1 = json::value::parse(__U("[// A comment in the middle of an empty array\n]"));
        VERIFY_IS_TRUE(arr1.is_array());
        VERIFY_ARE_EQUAL(0u, arr1.size());
        json::value arr2 = json::value::parse(__U("[/* A comment in the middle of an empty array */]"));
        VERIFY_IS_TRUE(arr2.is_array());
        VERIFY_ARE_EQUAL(0u, arr2.size());
        json::value arr3 = json::value::parse(__U("[ 1, // A comment in the middle of a non-array\n 2]"));
        VERIFY_IS_TRUE(arr3.is_array());
        VERIFY_ARE_EQUAL(2u, arr3.size());
        json::value arr4 = json::value::parse(__U("[ 1, /* A comment in the middle of a non-empty array */ 2]"));
        VERIFY_IS_TRUE(arr4.is_array());
        VERIFY_ARE_EQUAL(2u, arr4.size());
    }

    TEST(comments_stream)
    {
        // Nothing but a comment
        {
            std::basic_stringstream<utility::char_t> stream;
            stream << __U(" /* There's nothing but a comment here */ ");
            VERIFY_PARSING_THROW(json::value::parse(stream));
        }
        {
            std::basic_stringstream<utility::char_t> stream;
            stream << __U(" // There's nothing but a comment here\n ");
            VERIFY_PARSING_THROW(json::value::parse(stream));
        }

        // Some invalid comments
        {
            std::basic_stringstream<utility::char_t> stream;
            stream << __U(" -22 /*/");
            VERIFY_PARSING_THROW(json::value::parse(stream));
        }
        {
            std::basic_stringstream<utility::char_t> stream;
            stream << __U(" -22 /* /* nested */ */");
            VERIFY_PARSING_THROW(json::value::parse(stream));
        }

        // Correctly placed comments
        {
            std::basic_stringstream<utility::char_t> stream;
            stream << __U("-22 // This is a trailing comment\n");
            json::value num1 = json::value::parse(stream);
            VERIFY_ARE_EQUAL(-22, num1.as_double());
        }
        {
            std::basic_stringstream<utility::char_t> stream;
            stream << __U(" -22 /* This is a trailing comment with a // nested\n comment */");
            json::value num1 = json::value::parse(stream);
            VERIFY_ARE_EQUAL(-22, num1.as_double());
        }
        {
            std::basic_stringstream<utility::char_t> stream;
            stream << __U("// This is a leading comment\n -22");
            json::value num2 = json::value::parse(stream);
            VERIFY_ARE_EQUAL(-22, num2.as_double());
        }
        {
            std::basic_stringstream<utility::char_t> stream;
            stream << __U("-22 /* This is a trailing comment */");
            json::value num3 = json::value::parse(stream);
            VERIFY_ARE_EQUAL(-22, num3.as_double());
        }
        {
            std::basic_stringstream<utility::char_t> stream;
            stream << __U("/* This is a leading comment */ -22");
            json::value num4 = json::value::parse(stream);
            VERIFY_ARE_EQUAL(-22, num4.as_double());
        }
        {
            std::basic_stringstream<utility::char_t> stream;
            stream << __U("-22 /***/");
            json::value num4 = json::value::parse(stream);
            VERIFY_ARE_EQUAL(-22, num4.as_double());
        }

        {
            std::basic_stringstream<utility::char_t> stream;
            stream << __U("{// A comment in the middle of an empty object\n}");
            json::value obj1 = json::value::parse(stream);
            VERIFY_IS_TRUE(obj1.is_object());
            VERIFY_ARE_EQUAL(0u, obj1.size());
        }
        {
            std::basic_stringstream<utility::char_t> stream;
            stream << __U("{/* A comment in the middle of an empty object */}");
            json::value obj2 = json::value::parse(stream);
            VERIFY_IS_TRUE(obj2.is_object());
            VERIFY_ARE_EQUAL(0u, obj2.size());
        }
        {
            std::basic_stringstream<utility::char_t> stream;
            stream << __U("{ \"test1\" : // A comment in the middle of a non-empty object\n 2}");
            json::value obj3 = json::value::parse(stream);
            VERIFY_IS_TRUE(obj3.is_object());
            VERIFY_ARE_EQUAL(1u, obj3.size());
        }
        {
            std::basic_stringstream<utility::char_t> stream;
            stream << __U("{ \"test1\" : /* A comment in the middle of a non-empty object */ 2}");
            json::value obj4 = json::value::parse(stream);
            VERIFY_IS_TRUE(obj4.is_object());
            VERIFY_ARE_EQUAL(1u, obj4.size());
        }

        {
            std::basic_stringstream<utility::char_t> stream;
            stream << __U("[// A comment in the middle of an empty array\n]");
            json::value arr1 = json::value::parse(stream);
            VERIFY_IS_TRUE(arr1.is_array());
            VERIFY_ARE_EQUAL(0u, arr1.size());
        }
        {
            std::basic_stringstream<utility::char_t> stream;
            stream << __U("[/* A comment in the middle of an empty array */]");
            json::value arr2 = json::value::parse(stream);
            VERIFY_IS_TRUE(arr2.is_array());
            VERIFY_ARE_EQUAL(0u, arr2.size());
        }
        {
            std::basic_stringstream<utility::char_t> stream;
            stream << __U("[ 1, // A comment in the middle of a non-array\n 2]");
            json::value arr3 = json::value::parse(stream);
            VERIFY_IS_TRUE(arr3.is_array());
            VERIFY_ARE_EQUAL(2u, arr3.size());
        }
        {
            std::basic_stringstream<utility::char_t> stream;
            stream << __U("[ 1, /* A comment in the middle of a non-empty array */ 2]");
            json::value arr4 = json::value::parse(stream);
            VERIFY_IS_TRUE(arr4.is_array());
            VERIFY_ARE_EQUAL(2u, arr4.size());
        }
    }

    TEST(empty_object_array)
    {
        json::value obj = json::value::parse(__U("{}"));
        VERIFY_IS_TRUE(obj.is_object());
        VERIFY_ARE_EQUAL(0u, obj.size());

        json::value arr = json::value::parse(__U("[]"));
        VERIFY_IS_TRUE(arr.is_array());
        VERIFY_ARE_EQUAL(0u, arr.size());
    }

    TEST(bug_416116)
    {
        json::value data2 = json::value::parse(__U("\"δοκιμή\""));
        auto s = data2.serialize();

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4566)
#endif
        VERIFY_ARE_EQUAL(s, __U("\"δοκιμή\""));
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
    }

    TEST(byte_ptr_parsing_array)
    {
        char s[] = "[ \"test1\",true]";
        std::stringstream ss;
        ss << s;
        json::value v = json::value::parse(ss);
        auto s2 = v.serialize();

        VERIFY_ARE_EQUAL(s2, __U("[\"test1\",true]"));

        std::stringstream os;
        v.serialize(os);
        VERIFY_ARE_EQUAL(s2, to_string_t(os.str()));
    }

    TEST(byte_ptr_parsing_object)
    {
        char s[] = "{\"test1\":true }";
        std::stringstream ss;
        ss << s;
        json::value v = json::value::parse(ss);
        auto s2 = v.serialize();

        VERIFY_ARE_EQUAL(s2, __U("{\"test1\":true}"));

        std::stringstream os;
        v.serialize(os);
        VERIFY_ARE_EQUAL(s2, to_string_t(os.str()));
    }

    TEST(Japanese)
    {
        utility::string_t ws = __U("\"こんにちは\"");
        std::string s = to_utf8string(ws);

        std::stringstream ss;
        ss << s;
        json::value v = json::value::parse(ss);
        auto s2 = v.serialize();

        VERIFY_ARE_EQUAL(s2, ws);

        std::stringstream os;
        v.serialize(os);
        VERIFY_ARE_EQUAL(s2, to_string_t(os.str()));
    }

    TEST(Russian)
    {
        utility::string_t ws = __U("{\"results\":[{\"id\":272655310,\"name\":\"Андрей Ив´анов\"}]}");
        json::value v1 = json::value::parse(ws);
        auto s2 = v1.serialize();

        VERIFY_ARE_EQUAL(s2, ws);

        std::string s = to_utf8string(ws);

        std::stringstream ss;
        ss << s;
        json::value v2 = json::value::parse(ss);
        auto s3 = v2.serialize();

        VERIFY_ARE_EQUAL(s3, ws);
    }

    utility::string_t make_deep_json_string(size_t depth)
    {
        utility::string_t strval;
        for (size_t i = 0; i < depth; ++i)
        {
            strval += __U("{ \"a\" : 10, \"b\" : ");
        }
        strval += __U("20");
        for (size_t i = 0; i < depth; ++i)
        {
            strval += __U("}");
        }
        return strval;
    }

    TEST(deeply_nested)
    {
#if defined(__APPLE__)
        size_t safeDepth = 32;
        size_t overDepth = 33;
#else
        size_t safeDepth = 128;
        size_t overDepth = 129;
#endif

        // This should parse without issues:
        auto strGood = make_deep_json_string(safeDepth);
        json::value::parse(strGood);

        // But this one should throw:
        auto strBad = make_deep_json_string(overDepth);
        VERIFY_PARSING_THROW(json::value::parse(strBad));
    }

    static bool compare_pairs(const std::pair<utility::string_t, json::value>& p1,
                              const std::pair<utility::string_t, json::value>& p2)
    {
        return p1.first < p2.first;
    }

    TEST(unsorted_object_parsing)
    {
        utility::stringstream_t ss;
        ss << __U("{\"z\":2, \"a\":1}");
        json::value v = json::value::parse(ss);
        auto& obj = v.as_object();

        VERIFY_ARE_NOT_EQUAL(obj.find(__U("a")), obj.end());
        VERIFY_ARE_NOT_EQUAL(obj.find(__U("z")), obj.end());
        VERIFY_ARE_EQUAL(obj[__U("a")], 1);
        VERIFY_ARE_EQUAL(obj[__U("z")], 2);
        VERIFY_ARE_EQUAL(obj.size(), 2);

        VERIFY_IS_TRUE(::std::is_sorted(obj.begin(), obj.end(), compare_pairs));
    }

    TEST(keep_order_while_parsing)
    {
        utility::stringstream_t ss;
        ss << __U("{\"k\":3, \"j\":2, \"i\":1}");

        json::keep_object_element_order(true);
        struct restore
        {
            ~restore() { json::keep_object_element_order(false); }
        } _;

        json::value v = json::value::parse(ss);
        auto& obj = v.as_object();

        // Make sure collection stays unsorted:
        auto b = obj.begin();
        VERIFY_ARE_EQUAL(b[0].first, __U("k"));
        VERIFY_ARE_EQUAL(b[1].first, __U("j"));
        VERIFY_ARE_EQUAL(b[2].first, __U("i"));

        // Make sure lookup still works:
        auto val_i = obj[__U("i")];
        VERIFY_ARE_EQUAL(val_i.as_integer(), 1);

        auto val_j = obj[__U("j")];
        VERIFY_ARE_EQUAL(val_j.as_integer(), 2);

        // Make sure 'a' goes to the back of the collection, and
        // can be looked up
        obj[__U("a")] = 4;
        b = obj.begin();
        VERIFY_ARE_EQUAL(b[3].first, __U("a"));
        VERIFY_ARE_EQUAL(obj[__U("a")].as_integer(), 4);
    }

    TEST(non_default_locale, "Ignore:Android", "Locale unsupported on Android")
    {
        std::string originalLocale = setlocale(LC_ALL, nullptr);
#ifdef _WIN32
        std::string changedLocale("fr-FR");
#else
        std::string changedLocale("fr_FR.utf8");
#endif

        // If locale isn't installed on system just silently pass.
        if (setlocale(LC_ALL, changedLocale.c_str()) != nullptr)
        {
            // string serialize
            utility::string_t str(__U("[true,false,-1.55,5,null,{\"abc\":5555}]"));
            json::value v = json::value::parse(str);
            VERIFY_ARE_EQUAL(changedLocale, setlocale(LC_ALL, nullptr));
            VERIFY_ARE_EQUAL(str, v.serialize());
            VERIFY_ARE_EQUAL(changedLocale, setlocale(LC_ALL, nullptr));

            setlocale(LC_ALL, originalLocale.c_str());
            setlocale(LC_NUMERIC, changedLocale.c_str());

            // cpprestsdk stream serialize
            utility::stringstream_t stream;
            stream << v;
            utility::string_t serializedStr;
            stream >> serializedStr;
            VERIFY_ARE_EQUAL(str, serializedStr);

            // std stream serialize
            std::stringstream stdStream;
            v.serialize(stdStream);
            std::string stdStr;
            stdStream >> stdStr;
            VERIFY_ARE_EQUAL(str, utility::conversions::to_string_t(stdStr));

            setlocale(LC_ALL, originalLocale.c_str());
        }
    }

    template<typename T>
    void error_code_helper(T & jsonData)
    {
        std::error_code err;
        auto parsedObject = web::json::value::parse(jsonData, err);
        VERIFY_IS_TRUE(err.value() == 0);
        VERIFY_IS_TRUE(!parsedObject.is_null());
    }

    TEST(parse_overload_success)
    {
        std::error_code err;
        utility::string_t valueStr(__U("\"JSONString\""));
        utility::string_t arrStr(__U("[true,false,-1.55,5,null,{\"abc\":5555}]"));
        utility::string_t objStr(__U("{\"k\":3, \"j\":2, \"i\":1}"));

        error_code_helper(valueStr);
        error_code_helper(arrStr);
        error_code_helper(objStr);

        utility::stringstream_t valueStringStream;
        utility::stringstream_t arrayStringStream;
        utility::stringstream_t objStringStream;

        valueStringStream << valueStr;
        arrayStringStream << arrStr;
        objStringStream << objStr;

        error_code_helper(valueStringStream);
        error_code_helper(arrayStringStream);
        error_code_helper(objStringStream);

#ifdef _WIN32
        std::wstringbuf buf;

        buf.sputn(valueStr.c_str(), valueStr.size());
        std::wistream valStream(&buf);
        error_code_helper(valStream);

        buf.sputn(arrStr.c_str(), arrStr.size());
        std::wistream arrStream(&buf);
        error_code_helper(arrStream);

        buf.sputn(objStr.c_str(), objStr.size());
        std::wistream objStream(&buf);
        error_code_helper(objStream);
#endif
    }

    TEST(parse_overload_failed)
    {
        std::error_code err, streamErr, iStreamErr;
        utility::string_t str(__U("JSONString"));
        utility::string_t arrStr(__U("[true, false"));
        json::value parsedObject = json::value::parse(str, err);

        VERIFY_IS_TRUE(err.value() > 0);
        VERIFY_IS_TRUE(parsedObject.is_null());

        utility::stringstream_t stream;
        stream << str;

        parsedObject = json::value::parse(arrStr, streamErr);
        VERIFY_IS_TRUE(streamErr.value() > 0);
        VERIFY_IS_TRUE(parsedObject.is_null());

#ifdef _WIN32
        std::wstringbuf buf;
        buf.sputn(str.c_str(), str.size());
        std::wistream iStream(&buf);
        parsedObject = json::value::parse(str, iStreamErr);
        VERIFY_IS_TRUE(iStreamErr.value() > 0);
        VERIFY_IS_TRUE(parsedObject.is_null());
#endif
    }

} // SUITE(parsing_tests)

} // namespace json_tests
} // namespace functional
} // namespace tests
