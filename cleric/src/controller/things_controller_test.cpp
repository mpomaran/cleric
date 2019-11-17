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
#include "cpprest/details/basic_types.h"
#include "things_controller.hpp"
#include <catch/catch.hpp>
#include <cpprest/filestream.h>
#include <cpprest/http_client.h>
#include <string>
#include <picojson.h>

using namespace utility;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;

using namespace std;
using namespace cleric::http::controller;
using namespace web;
using namespace utility;


using namespace utility;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;

using namespace std;
using namespace cleric::http::controller;
using namespace web;
using namespace utility;

namespace cleric {
	namespace http {
		namespace controller {
			namespace test {
				class WebThingControllerTester {
				public:
					static void callPost();
					static string callGet(string url);
				};
			} // namespace test
		} // namespace controller
	} // namespace http
} // namespace cleric


void cleric::http::controller::test::WebThingControllerTester::callPost() {
	throw std::runtime_error((const char *)"Not implemented yet");
}

std::string gulp(std::istream &in)
{
	std::string ret;
	char buffer[1024*64];
	while (in.read(buffer, sizeof(buffer)))
		ret.append(buffer, sizeof(buffer));
	ret.append(buffer, in.gcount());
	return ret;
}

string cleric::http::controller::test::WebThingControllerTester::callGet(string url) {
	std::shared_ptr<granada::http::session::SessionFactory> session_factory =
		std::make_shared<granada::http::session::MapSessionFactory>();
	uri_builder uri(
		utility::conversions::to_string_t(url));
	auto addr = uri.to_uri().to_string();
	ThingsController controller(addr, session_factory);

	http_request msg(methods::GET);
	msg.set_request_uri(uri.to_string());

	controller.handleGet(msg);
	if (msg.get_response().get().status_code() == status_codes::OK) {
		auto wresult = msg.get_response().get().extract_string().get();
		// TODO convert wstring to string if needed
		std::string result(wresult.begin(), wresult.end());
		return result;
	}
	else {
		return string();
	}
}


TEST_CASE("Sensor value sent by sensor is accessible on /things endpoint [thins_controller]") {
	// TODO
}

TEST_CASE("After sending a value sensor is present as property in the thing description [things_controller]") {
	// TODO
}

TEST_CASE("If device does not have any value the descriptor does not have any properties [things_controller]") {
	cleric::http::controller::test::WebThingControllerTester client;

	auto resultJson = client.callGet("https://localhost/things/mixbox-bridge");

	// decode json
	// check against doc: https://iot.mozilla.org/wot/#thing-resource
	picojson::value v;

	std::string err = picojson::parse(v, resultJson);
	REQUIRE(err.empty());

	// check if the type of the value is "object"
	REQUIRE(v.is<picojson::object>());

	// obtain a const reference to the map, and print the contents
	int c = 0;
	const picojson::value::object& obj = v.get<picojson::object>();
	for (picojson::value::object::const_iterator i = obj.begin();
		i != obj.end();
		++i) {
		if (i->first == "id") {
			c++;  REQUIRE(i->second.to_str().size() != 0);
		}
		if (i->first == "title") {
			c++;  REQUIRE(i->second.to_str().size() != 0);
		}
		if (i->first == "description") {
			c++; REQUIRE(i->second.to_str().size() != 0);
		}
		if (i->first == "properties") {
			c++;  
			auto props = i->second;
			REQUIRE(props.is<picojson::object>() == true);
			auto o = props.get<picojson::object>();
			REQUIRE(o.size() == 0);
		}
	}
}
