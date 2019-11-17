/***
 * Copyright (C) Microsoft. All rights reserved.
 * Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
 *
 * =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *
 * http_client_fuzz_tests.cpp
 *
 * Tests cases for fuzzing http_client (headers).
 *
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 ****/

#include "stdafx.h"

using namespace web::http;
using namespace web::http::client;
using namespace concurrency;
using namespace concurrency::streams;
using namespace utility;
using namespace tests::functional::http::utilities;

namespace tests
{
namespace functional
{
namespace http
{
namespace client
{
SUITE(http_client_fuzz_tests)
{
    class fuzz_uri_address
    {
    public:
        // Ensure that your traffic goes to port 8877 on the machine where NetFuzz is running
        // Netfuzz sets an HTTP proxy at that location which your client must talk to
        fuzz_uri_address() : m_uri(__U("http://localhost:8877/")) {}
        web::http::uri m_uri;
    };

    TEST_FIXTURE(fuzz_uri_address, fuzz_header_basic, "Ignore", "Manual")
    {
        http_client client(m_uri);
        method requestMethod = methods::GET;
        http_request msg(requestMethod);

        try
        {
            auto response = client.request(msg).get();
            printf("Response code:%d\n", response.status_code());
            auto response2 = response.content_ready().get();
            printf("Response2 code:%d\n", response2.status_code());
        }
        catch (http_exception& e)
        {
            printf("Exception:%s\n", e.what());
        }
    }

    TEST_FIXTURE(fuzz_uri_address, fuzz_request_headers, "Ignore", "Manual")
    {
        http_client client(m_uri);
        http_request msg(methods::POST);

#ifndef __cplusplus_winrt
        // The WinRT-based HTTP stack does not support headers that have no
        // value, which means that there is no point in making this particular
        // header test, it is an unsupported feature on WinRT.
        msg.headers().add(__U("HEHE"), __U(""));
#endif

        msg.headers().add(__U("MyHeader"), __U("hehe;blach"));
        msg.headers().add(__U("Yo1"), __U("You, Too"));
        msg.headers().add(__U("Yo2"), __U("You2"));
        msg.headers().add(__U("Yo3"), __U("You3"));
        msg.headers().add(__U("Yo4"), __U("You4"));
        msg.headers().add(__U("Yo5"), __U("You5"));
        msg.headers().add(__U("Yo6"), __U("You6"));
        msg.headers().add(__U("Yo7"), __U("You7"));
        msg.headers().add(__U("Yo8"), __U("You8"));
        msg.headers().add(__U("Yo9"), __U("You9"));
        msg.headers().add(__U("Yo10"), __U("You10"));
        msg.headers().add(__U("Yo11"), __U("You11"));
        msg.headers().add(__U("Accept"), __U("text/plain"));
        VERIFY_ARE_EQUAL(__U("You5"), msg.headers()[__U("Yo5")]);
        try
        {
            auto response = client.request(msg).get();
            printf("Response code:%d\n", response.status_code());
        }
        catch (http_exception& e)
        {
            printf("Exception:%s\n", e.what());
        }
    }
} // SUITE(http_client_fuzz_tests)

} // namespace client
} // namespace http
} // namespace functional
} // namespace tests
