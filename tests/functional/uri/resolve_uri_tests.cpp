#include "stdafx.h"

using namespace web;
using namespace utility;

namespace tests
{
namespace functional
{
namespace uri_tests
{
// testing resolution against examples from Section 5.4 https://tools.ietf.org/html/rfc3986#section-5.4
SUITE(resolve_uri_tests)
{
    // 5.4.1. Normal Examples https://tools.ietf.org/html/rfc3986#section-5.4.1
    TEST(resolve_uri_normal)
    {
        const uri baseUri = __U("http://a/b/c/d;p?q");

        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("g:h")), __U("g:h"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("g")), __U("http://a/b/c/g"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("./g")), __U("http://a/b/c/g"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("g/")), __U("http://a/b/c/g/"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("/g")), __U("http://a/g"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("//g")), __U("http://g"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("?y")), __U("http://a/b/c/d;p?y"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("g?y")), __U("http://a/b/c/g?y"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("#s")), __U("http://a/b/c/d;p?q#s"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("g#s")), __U("http://a/b/c/g#s"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("g?y#s")), __U("http://a/b/c/g?y#s"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U(";x")), __U("http://a/b/c/;x"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("g;x")), __U("http://a/b/c/g;x"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("g;x?y#s")), __U("http://a/b/c/g;x?y#s"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("")), __U("http://a/b/c/d;p?q"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U(".")), __U("http://a/b/c/"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("./")), __U("http://a/b/c/"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("..")), __U("http://a/b/"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("../")), __U("http://a/b/"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("../g")), __U("http://a/b/g"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("../..")), __U("http://a/"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("../../")), __U("http://a/"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("../../g")), __U("http://a/g"));
    }
    // 5.4.2. Abnormal Examples https://tools.ietf.org/html/rfc3986#section-5.4.2
    TEST(resolve_uri_abnormal)
    {
        const uri baseUri = __U("http://a/b/c/d;p?q");

        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("../../../g")), __U("http://a/g"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("../../../../g")), __U("http://a/g"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("/./g")), __U("http://a/g"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("/../g")), __U("http://a/g"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("g.")), __U("http://a/b/c/g."));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U(".g")), __U("http://a/b/c/.g"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("g..")), __U("http://a/b/c/g.."));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("..g")), __U("http://a/b/c/..g"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("./../g")), __U("http://a/b/g"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("./g/.")), __U("http://a/b/c/g/"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("g/./h")), __U("http://a/b/c/g/h"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("g/../h")), __U("http://a/b/c/h"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("g;x=1/./y")), __U("http://a/b/c/g;x=1/y"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("g;x=1/../y")), __U("http://a/b/c/y"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("g?y/./x")), __U("http://a/b/c/g?y/./x"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("g?y/../x")), __U("http://a/b/c/g?y/../x"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("g#s/./x")), __U("http://a/b/c/g#s/./x"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("g#s/../x")), __U("http://a/b/c/g#s/../x"));
        VERIFY_ARE_EQUAL(baseUri.resolve_uri(__U("http:g")), __U("http:g"));
    }

} // SUITE(resolve_uri_tests)

} // namespace uri_tests
} // namespace functional
} // namespace tests
