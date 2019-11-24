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
#include "../business/m2m_message.hpp"

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

#define VALID_M2M_MESSAGE_URI "http://localhost/0000000ABE587DE600000000AE30AFFCD31EA91087E215F4"


void cleric::http::controller::test::M2MControllerTester::callPost(const std::string &messageUri)
{
	std::shared_ptr<granada::http::session::SessionFactory> session_factory =
		std::make_shared<granada::http::session::MapSessionFactory>();
	uri_builder uri(
		utility::conversions::to_string_t(messageUri));
	auto addr = uri.to_uri().to_string();
	M2MController controller(addr, session_factory);

	http_request msg(methods::POST);
	msg.set_request_uri(uri_builder(utility::conversions::to_string_t(messageUri))
		.to_string());

	controller.handlePost(msg);
}


void cleric::http::controller::test::M2MControllerTester::callPost() {
	callPost(VALID_M2M_MESSAGE_URI);
}

void cleric::http::controller::test::M2MControllerTester::callGet() {
	std::shared_ptr<granada::http::session::SessionFactory> session_factory =
		std::make_shared<granada::http::session::MapSessionFactory>();
	uri_builder uri(
		utility::conversions::to_string_t(VALID_M2M_MESSAGE_URI));
	auto addr = uri.to_uri().to_string();
	M2MController controller(addr, session_factory);

	http_request msg(methods::GET);
	msg.set_request_uri(uri_builder(__U(VALID_M2M_MESSAGE_URI))
		.to_string());

	controller.handleGet(msg);
}

void cleric::http::controller::test::M2MControllerTester::callGet(const std::string &messageUri) {
	std::shared_ptr<granada::http::session::SessionFactory> session_factory =
		std::make_shared<granada::http::session::MapSessionFactory>();
	uri_builder uri(
		utility::conversions::to_string_t(messageUri));
	auto addr = uri.to_uri().to_string();
	M2MController controller(addr, session_factory);

	http_request msg(methods::GET);
	msg.set_request_uri(uri_builder(utility::conversions::to_string_t(messageUri))
		.to_string());

	controller.handleGet(msg);
}

void cleric::http::controller::test::M2MControllerTester::callGet(uint32_t boxId, uint32_t sensorType, uint32_t sensorValue, uint32_t vcc)
{
	std::string messageUri = "http://localhost/";

	auto payload = M2MMessage::encode(boxId, vcc, sensorValue, sensorType, M2M_SECRET);

	callGet(messageUri + payload);
}

void cleric::http::controller::test::M2MControllerTester::callPost(uint32_t boxId, uint32_t sensorType, uint32_t sensorValue, uint32_t vcc)
{
	std::string messageUri = "http://localhost/";

	auto payload = M2MMessage::encode(boxId, vcc, sensorValue, sensorType, M2M_SECRET);

	callPost(messageUri + payload);
}
