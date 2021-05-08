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

#include "things_controller.hpp"
#include "m2m_controller_tester.hpp"
#include "../data/box_server_locator.hpp"
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
using namespace cleric::data;
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
		std::string result(wresult.begin(), wresult.end());
		return result;
	}
	else {
		return string();
	}
}

static picojson::value::object parseJson(const std::string &json)
{
	picojson::value v;

	std::string err = picojson::parse(v, json);
	REQUIRE(err.empty());

	// check if the type of the value is "object"
	REQUIRE(v.is<picojson::object>());

	// obtain a const reference to the map
	const picojson::value::object& obj = v.get<picojson::object>();

	return obj;

}

TEST_CASE("After sending value they are presented in /things endpoint as properties [thins_controller]") {

	// mocked sensor values
	auto BOX_ID = 10;
	auto SENSOR_TYPE = 43;
	auto SENSOR_VALUE = 332;
	auto VCC = 200;

	// send sensor value though m2m interface
	cleric::http::controller::test::M2MControllerTester::callGet(BOX_ID, SENSOR_TYPE, SENSOR_VALUE, VCC);

	// check the endpoind
	cleric::http::controller::test::WebThingControllerTester client;
	auto jsonString = client.callGet("https://localhost/things/mixbox-bridge");

	auto json = parseJson(jsonString);

	REQUIRE(json.find("properties") != json.end());

	auto vcc_key = std::string("mix_box_") + std::to_string(BOX_ID) + "_vcc";
	REQUIRE(json["properties"].get<picojson::object>().count(vcc_key) == 1);

	auto value_key = std::string("mix_box_") + std::to_string(BOX_ID) + "_value";
	REQUIRE(json["properties"].get<picojson::object>().count(value_key) == 1);

	auto type_key = std::string("mix_box_") + std::to_string(BOX_ID) + "_value_type";
	REQUIRE(json["properties"].get<picojson::object>().count(type_key) == 1);

	auto rcv_key = std::string("mix_box_") + std::to_string(BOX_ID) + "_rcv";
	REQUIRE(json["properties"].get<picojson::object>().count(rcv_key) == 1);


	BoxServerLocator::clearAllServers();
}

TEST_CASE("Sensor value sent by sensor is accessible on /things/mixbox-bridge/properties/property endpoint [things_controller]") {
	auto BOX_ID = 10;
	auto SENSOR_TYPE = 43;
	auto SENSOR_VALUE = 332;
	auto VCC = 200;

	BoxServerLocator::clearAllServers();
	cleric::http::controller::test::M2MControllerTester::callGet(BOX_ID, SENSOR_TYPE, SENSOR_VALUE, VCC);

	cleric::http::controller::test::WebThingControllerTester client;
	auto value_key = std::string("mix_box_") + std::to_string(BOX_ID) + "_value";
	auto url = std::string("https://localhost/things/mixbox-bridge/properties/") + value_key;
	auto jsonString = client.callGet(url);

	REQUIRE(!jsonString.empty());
	const auto json = parseJson(jsonString);

	// decode json
	// check against doc: https://iot.mozilla.org/wot/#properties-resource
	auto value = json.find(value_key);

	REQUIRE(value != json.end());
	REQUIRE(value->second.is<double>());

	REQUIRE(value->second.get<double>() == SENSOR_VALUE);

	BoxServerLocator::clearAllServers();
}


TEST_CASE("Sensor value sent by sensor is accessible on /things/mixbox-bridge/properties endpoint [things_controller]") {

	// mocked sensor values
	auto BOX_ID = 10;
	auto SENSOR_TYPE = 43;
	auto SENSOR_VALUE = 332;
	auto VCC = 200;

	// send sensor value though m2m interface
	BoxServerLocator::clearAllServers();
	cleric::http::controller::test::M2MControllerTester::callGet(BOX_ID, SENSOR_TYPE, SENSOR_VALUE, VCC);

	// check the endpoind
	cleric::http::controller::test::WebThingControllerTester client;
	auto jsonString = client.callGet("https://localhost/things/mixbox-bridge/properties");

	// decode json
	// check against doc: https://iot.mozilla.org/wot/#properties-resource
	const auto json = parseJson(jsonString);

	auto vcc_key = std::string("mix_box_") + std::to_string(BOX_ID) + "_vcc";
	auto value_key = std::string("mix_box_") + std::to_string(BOX_ID) + "_value";
	auto type_key = std::string("mix_box_") + std::to_string(BOX_ID) + "_value_type";
	auto time_key = std::string("mix_box_") + std::to_string(BOX_ID) + "_rcv";
	auto vcc = json.find(vcc_key);
	auto value = json.find(value_key);
	auto type = json.find(type_key);
	auto rcv = json.find(time_key);

	REQUIRE(value != json.end());
	REQUIRE(type != json.end());
	REQUIRE(vcc != json.end());
	REQUIRE(rcv != json.end());

	REQUIRE(vcc->second.is<double>());
	REQUIRE(value->second.is<double>());
	REQUIRE(type->second.is<double>());

	REQUIRE(value->second.get<double>() == SENSOR_VALUE);
	REQUIRE(type->second.get<double>() == SENSOR_TYPE);

	BoxServerLocator::clearAllServers();
}

TEST_CASE("If device does not have any value the descriptor does not have any properties [things_controller]") {

	BoxServerLocator::clearAllServers();

	cleric::http::controller::test::WebThingControllerTester client;

	auto json = client.callGet("https://localhost/things/mixbox-bridge");

	// obtain a const reference to the map, and check the contents
	const auto obj = parseJson(json);
	int c = 0;
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
	BoxServerLocator::clearAllServers();
}
