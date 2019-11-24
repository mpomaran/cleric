/**

MIT License

Copyright (c) 2019 mpomaranski at gmail

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "m2m_controller_tester.hpp"
#include "granada/http/session/map_session.h"
#include "m2m_controller.hpp"
#include <catch/catch.hpp>
#include <cpprest/filestream.h>
#include <cpprest/http_client.h>
#include <string>

using namespace utility;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;

using namespace std;
using namespace cleric::http::controller;
using namespace web;
using namespace utility;


TEST_CASE("GET controller is invoked", "[rest_controller]") {
  CHECK(cleric::http::controller::test::isGetInvoked == false);

  cleric::http::controller::test::M2MControllerTester::callGet();

  CHECK(cleric::http::controller::test::isGetInvoked == true);
}

TEST_CASE("POST controller is invoked", "[rest_controller]") {
  CHECK(cleric::http::controller::test::isPostInvoked == false);

  cleric::http::controller::test::M2MControllerTester::callPost();

  CHECK(cleric::http::controller::test::isPostInvoked == true);
}
