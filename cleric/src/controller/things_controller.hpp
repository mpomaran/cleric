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

#pragma once

#include "../data/box.hpp"
#include "cpprest/details/basic_types.h"
#include "granada/http/controller/controller.h"
#include "granada/http/parser.h"
#include "granada/http/session/session.h"
#include <picojson.h>
#include <unordered_map>

namespace cleric {
	namespace http {
		namespace controller {

			namespace test {
				class WebThingControllerTester;
			}

			class ThingsController : public granada::http::controller::Controller {
			public:
				/**
				 * Constructor
				 */
				ThingsController(
					utility::string_t url,
					std::shared_ptr<granada::http::session::SessionFactory> &session_factory);


			private:
				/**
				 * Handles HTTP GET requests.
				 * @param request HTTP request.
				 */
				void handleGet(http_request request);

				/**
				 * Handles HTTP POST requests.
				 * @param request HTTP request.
				 */
				void handlePost(http_request request);

				std::shared_ptr<granada::http::session::SessionFactory> sessionFactory;

				/**
				 * Replies with a status code and an empty content
				 * @param request HTTP request
				 * @param statusCode HTTP status, for example 404 if not found
				 */
				void reply(web::http::http_request &request,
					web::http::status_code statusCode);

				/**
				 * Replies with a status code and an empty content
				 * @param request HTTP request
				 * @param statusCode HTTP status, for example OK
				 * @param body reply in the text format
				 */
				void reply(web::http::http_request &request,
					web::http::status_code statusCode, const std::string &body);

				/**
				 * Serializes list of all boxes available on the server
				 * THIS is for development purposes only, to be removed before the first release
				 */
				::std::string boxListToJSon(const ::std::vector<::cleric::BoxId> &boxes);

				/**
				* Creates JSON payload for description of a thing (https://iot.mozilla.org/wot/#thing-resource)
				*/
				std::string getThingResource() const;

				::std::unique_ptr<::picojson::value::object> getThingPropertiesResource() const;


				/**
				* Creates JSON payload for thing properties (https://iot.mozilla.org/wot/#properties-resource)
				*/
				std::string getPropertiesResource() const;

				std::string getPropertyResource(const BoxId &boxId, const std::string &param) const;

				static std::string getSensorVccPropertyKey(const BoxId &id);
				static std::string getSensorTypePropertyKey(const BoxId &id);
				static std::string getSensorValuePropertyKey(const BoxId &id);
				static std::string getSensorRcvTimestampPropertyKey(const BoxId &id);

				friend class test::WebThingControllerTester;

			};
		} // namespace controller
	} // namespace http
} // namespace cleric
