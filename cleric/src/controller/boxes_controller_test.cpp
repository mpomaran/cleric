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

#include "granada/http/session/map_session.h"
#include "boxes_controller.hpp"
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

SCENARIO("correct GET request for a existing box", "boxes_controller") {
	GIVEN("Correct GET request and box exists") {
		WHEN("request is processed") {
			THEN("the given box is returned") {
//				FAIL("not implemented yet");

				// TODO implement
			}
		}
	}
}

SCENARIO("correct GET request for a not existing box", "boxes_controller") {
	GIVEN("Correct GET request and box does not exists") {
		WHEN("request is processed") {
			THEN("resource does not exist error is returned") {
				//FAIL("not implemented yet");

				// TODO implement
			}
		}
	}
}

SCENARIO("incorrect GET request", "boxes_controller") {
	GIVEN("Incorrect GET request") {
		WHEN("request is processed") {
			THEN("resource does not exist error is returned") {
		//		FAIL("not implemented yet");
				// TODO implement
			}
		}
	}
}
