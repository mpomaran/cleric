/***
 * ==++==
 *
 * Copyright (c) Microsoft Corporation.  All rights reserved.
 *
 * ==--==
 * =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *
 * header_tests.cpp
 *
 * Tests cases for using HTTP requests/response headers with http_listener.
 *
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 ****/

#include "stdafx.h"

using namespace web::http;
using namespace web::http::experimental::listener;

using namespace tests::functional::http::utilities;

namespace tests
{
namespace functional
{
namespace http
{
namespace listener
{
SUITE(header_tests)
{
    TEST_FIXTURE(uri_address, request_headers)
    {
        http_listener listener(m_uri);
        listener.open().wait();
        test_http_client::scoped_client client(m_uri);
        test_http_client* p_client = client.client();
        const utility::string_t mtd = methods::GET;
        std::map<utility::string_t, utility::string_t> headers;

        // single header value.
        headers[__U("Header1")] = __U("Value1");
        listener.support([&](http_request request) {
            http_asserts::assert_request_equals(request, mtd, __U("/"), headers);
            request.reply(status_codes::OK).wait();
        });
        VERIFY_ARE_EQUAL(0, p_client->request(mtd, __U(""), headers));
        p_client->next_response()
            .then([](test_response* p_response) {
                http_asserts::assert_test_response_equals(p_response, status_codes::OK);
            })
            .wait();

        // empty header value.
        headers.clear();
        headers[__U("Key1")] = __U("");
        listener.support([&](http_request request) {
            http_asserts::assert_request_equals(request, mtd, __U("/"), headers);
            request.reply(status_codes::OK).wait();
        });
        VERIFY_ARE_EQUAL(0, p_client->request(mtd, __U(""), headers));
        p_client->next_response()
            .then([](test_response* p_response) {
                http_asserts::assert_test_response_equals(p_response, status_codes::OK);
            })
            .wait();

        // 10 headers.
        headers.clear();
        headers[__U("MyHeader")] = __U("hehe;blach");
        headers[__U("Yo1")] = __U("You, Too");
        headers[__U("Yo2")] = __U("You2");
        headers[__U("Yo3")] = __U("You3");
        headers[__U("Yo4")] = __U("You4");
        headers[__U("Yo5")] = __U("You5");
        headers[__U("Yo6")] = __U("You6");
        headers[__U("Yo7")] = __U("You7");
        headers[__U("Yo8")] = __U("You8");
        headers[__U("Yo9")] = __U("You9");
        headers[__U("Yo10")] = __U("You10");
        headers[__U("Yo11")] = __U("You11");
        headers[__U("Accept")] = __U("text/plain");
        listener.support([&](http_request request) {
            http_asserts::assert_request_equals(request, mtd, __U("/"), headers);
            request.reply(status_codes::OK).wait();
        });
        VERIFY_ARE_EQUAL(0, p_client->request(mtd, __U(""), headers));
        p_client->next_response()
            .then([](test_response* p_response) {
                http_asserts::assert_test_response_equals(p_response, status_codes::OK);
            })
            .wait();

        // several headers different casings.
        headers.clear();
        headers[__U("CUSTOMHEADER")] = __U("value1");
        headers[__U("customHEADER")] = __U("value2");
        headers[__U("CUSTOMheaDER")] = __U("value3");
        listener.support([&](http_request request) {
            std::map<utility::string_t, utility::string_t> h;
            h[__U("CUSTOMHEADER")] = __U("value1, value3, value2");
            http_asserts::assert_request_equals(request, mtd, __U("/"), h);
            request.reply(status_codes::OK).wait();
        });
        VERIFY_ARE_EQUAL(0, p_client->request(mtd, __U(""), headers));
        p_client->next_response()
            .then([](test_response* p_response) {
                http_asserts::assert_test_response_equals(p_response, status_codes::OK);
            })
            .wait();
        listener.close().wait();
    }

    TEST_FIXTURE(uri_address, response_headers)
    {
        http_listener listener(m_uri);
        listener.open().wait();
        test_http_client::scoped_client client(m_uri);
        test_http_client* p_client = client.client();

        // No http_request/response classes can be around for close to complete.
        {
            // header with empty value
            http_response response(status_codes::OK);
            response.headers()[__U("Key1")] = __U("");
            listener.support([&](http_request request) {
                http_asserts::assert_request_equals(request, methods::POST, __U("/"));
                request.reply(response).wait();
            });
            VERIFY_ARE_EQUAL(0u, p_client->request(methods::POST, __U("")));
            p_client->next_response()
                .then([&](test_response* p_response) {
                    http_asserts::assert_test_response_equals(p_response, status_codes::OK, response.headers());
                })
                .wait();

            // 10 headers
            response = http_response(status_codes::Accepted);
            response.headers()[__U("MyHeader")] = __U("hehe;blach");
            response.headers()[__U("Yo1")] = __U("You, Too");
            response.headers()[__U("Yo2")] = __U("You2");
            response.headers()[__U("Yo3")] = __U("You3");
            response.headers()[__U("Yo4")] = __U("You4");
            response.headers()[__U("Yo5")] = __U("You5");
            response.headers()[__U("Yo6")] = __U("You6");
            response.headers()[__U("Yo7")] = __U("You7");
            response.headers()[__U("Yo8")] = __U("You8");
            response.headers()[__U("Yo9")] = __U("You9");
            response.headers()[__U("Yo10")] = __U("You10");
            response.headers()[__U("Yo11")] = __U("You11");
            response.headers()[__U("Accept")] = __U("text/plain");
            listener.support([&](http_request request) {
                http_asserts::assert_request_equals(request, methods::POST, __U("/"));
                request.reply(response).wait();
            });
            VERIFY_ARE_EQUAL(0u, p_client->request(methods::POST, __U("")));
            p_client->next_response()
                .then([&](test_response* p_response) {
                    http_asserts::assert_test_response_equals(p_response, status_codes::Accepted, response.headers());
                })
                .wait();

            // several headers in different casings
            response = http_response(status_codes::BadGateway);
            response.headers().add(__U("Key1"), __U("value1"));
            response.headers()[__U("KEY1")] += __U("value2");
            listener.support([&](http_request request) {
                http_asserts::assert_request_equals(request, methods::POST, __U("/"));
                request.reply(response).wait();
            });
            VERIFY_ARE_EQUAL(0u, p_client->request(methods::POST, __U("")));
            p_client->next_response()
                .then([&](test_response* p_response) {
                    http_asserts::assert_test_response_equals(p_response, status_codes::BadGateway, response.headers());
                })
                .wait();

            // duplicate headers fields
            response = http_response(status_codes::BadGateway);
            response.headers().add(__U("Key1"), __U("value1"));
            response.headers().add(__U("Key1"), __U("value2"));
            listener.support([&](http_request request) {
                http_asserts::assert_request_equals(request, methods::POST, __U("/"));
                request.reply(response).wait();
            });
            VERIFY_ARE_EQUAL(0u, p_client->request(methods::POST, __U("")));
            p_client->next_response()
                .then([&](test_response* p_response) {
                    http_asserts::assert_test_response_equals(p_response, status_codes::BadGateway, response.headers());
                })
                .wait();
        }

        listener.close().wait();
    }
}

} // namespace listener
} // namespace http
} // namespace functional
} // namespace tests
