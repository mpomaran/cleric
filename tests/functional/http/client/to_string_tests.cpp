/***
 * Copyright (C) Microsoft. All rights reserved.
 * Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
 *
 * =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *
 * Tests cases for to_string APIs on HTTP request and responses.
 *
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 ****/

#include "stdafx.h"

using namespace web;
using namespace utility;
using namespace web::http;
using namespace web::http::client;

using namespace tests::functional::http::utilities;

namespace tests
{
namespace functional
{
namespace http
{
namespace client
{
SUITE(to_string_tests)
{
    TEST_FIXTURE(uri_address, request_to_string_without_body)
    {
        test_http_server::scoped_server scoped(m_uri);
        http_client client(m_uri);
        const method mtd = methods::GET;
        const utility::string_t path = __U("/pathbaby/");
        const utility::string_t content_type = __U("text/plain; charset= utf-8");

        // to_string
        http_request msg(mtd);
        msg.set_request_uri(path);
        msg.headers()[__U("Content-Type")] = content_type;

        std::map<utility::string_t, utility::string_t> expected_headers;
        expected_headers[__U("Content-Type")] = content_type;
        http_asserts::assert_request_string_equals(msg.to_string(), mtd, path, __U("HTTP/1.1"), expected_headers, __U(""));
    }

    TEST_FIXTURE(uri_address, request_to_string_with_body)
    {
        test_http_server::scoped_server scoped(m_uri);
        http_client client(m_uri);
        const method mtd = methods::POST;
        const utility::string_t path = __U("/path baby/");
        const utility::string_t content_type = __U("text/plain;charset=utf-8");
        const utility::string_t body = __U("YES THIS IS THE MSG BODY!!!!!");

        // to_string
        http_request msg(mtd);
        msg.set_request_uri(uri::encode_uri(path, uri::components::path));
        msg.headers()[__U("Content-Type")] = content_type;
        msg.set_body(body);

        std::map<utility::string_t, utility::string_t> expected_headers;
        expected_headers[__U("Content-Type")] = content_type;
        expected_headers[__U("Content-Length")] = __U("29");
        http_asserts::assert_request_string_equals(
            msg.to_string(), mtd, __U("/path%20baby/"), __U("HTTP/1.1"), expected_headers, body);
    }

    TEST_FIXTURE(uri_address, response_to_string_without_body)
    {
        test_http_server::scoped_server scoped(m_uri);
        http_client client(m_uri);
        const web::http::status_code code = status_codes::OK;
        const utility::string_t reason = __U("OK YEAH!");
        const utility::string_t content_type = __U("not; charset= utf-8");

        // to_string
        scoped.server()->next_request().then([&](test_request* request) {
            std::map<utility::string_t, utility::string_t> headers;
            headers[__U("Content-Type")] = content_type;
            request->reply(code, reason, headers);
        });
        http_response rsp = client.request(methods::GET).get();

        std::map<utility::string_t, utility::string_t> expected_headers;
        expected_headers[__U("Content-Length")] = __U("0");
        expected_headers[__U("Content-Type")] = content_type;
        http_asserts::assert_response_string_equals(
            rsp.to_string(), __U("HTTP/1.1"), code, __U("OK"), expected_headers, __U(""));

#ifdef _WIN32
        // Don't verify the values of each of these headers, but make sure they exist.
        if (!rsp.headers().has(__U("Date")) || !rsp.headers().has(__U("Cache-Control")) || !rsp.headers().has(__U("Server")))
        {
            CHECK(false);
        }
#endif
    }

    TEST_FIXTURE(uri_address, response_to_string_with_body)
    {
        test_http_server::scoped_server scoped(m_uri);
        http_client client(m_uri);
        const ::http::status_code code = status_codes::OK;
        const utility::string_t reason = __U("OK YEAH!");
        const std::string data = "HERE IS THE RESPONSE body!";
        const utility::string_t content_type = __U("text/yeah;charset=utf-8");

        // to_string
        scoped.server()->next_request().then([&](test_request* request) {
            std::map<utility::string_t, utility::string_t> headers;
            headers[__U("Content-Type")] = content_type;
            request->reply(code, reason, headers, data);
        });

        http_response rsp = client.request(methods::GET).get();
        rsp.content_ready().wait();

        std::map<utility::string_t, utility::string_t> expected_headers;
        expected_headers[__U("Content-Length")] = __U("26");
        expected_headers[__U("Content-Type")] = content_type;
        http_asserts::assert_response_string_equals(
            rsp.to_string(), __U("HTTP/1.1"), code, __U("OK"), expected_headers, ::utility::conversions::to_string_t(data));
    }

} // SUITE(to_string_tests)

} // namespace client
} // namespace http
} // namespace functional
} // namespace tests
