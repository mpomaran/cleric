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
#include "../data/box.hpp"
#include "../data/box_server.hpp"
#include "../data/box_server_locator.hpp"
#include "../network/host_info.hpp"
#include <props.hpp>
#include <boost/type_index.hpp>
#include <easylogging++.h>
#include <sstream>
#include <picojson.h>

using namespace cleric::data;
using namespace cleric;
using namespace std;

namespace cleric {
	namespace http {
		namespace controller {
			ThingsController::ThingsController(
				utility::string_t url,
				shared_ptr<granada::http::session::SessionFactory> &session_factory) {

				m_listener_ = unique_ptr<http_listener>(new http_listener(url));
				m_listener_->support(
					methods::GET, bind(&ThingsController::handleGet, this, placeholders::_1));

				LOG(INFO)
					<< "[ThingsController::ThingsController] {/things controller created}";
			}

			std::string ThingsController::getThingResource() const
			{
				picojson::value::object data;

				// if does not compile check syntax here: https://github.com/kazuho/picojson/pull/53

				data["id"] = picojson::value(std::string("") + "https://" + ::cleric::network::getHostIp() + "/things" + THING_NAME);
				data["title"] = picojson::value(THING_NAME);
				data["description"] = picojson::value("MixBox bridge");

				picojson::value::object properties;
				data["properties"] = picojson::value(properties);

				return picojson::value(data).serialize();
			}


			void ThingsController::handleGet(http_request request) {
				try {
					web::http::http_response response;
					response.set_status_code(status_codes::NotAcceptable);

					auto uri = request.absolute_uri().path();
					LOG(INFO) << "[ThingsController::handleGet] {request='" << uri << "'}";

					auto paths = uri::split_path(uri::decode(uri));

					if (paths.size() < 2) {
						// wrong request, return with 404
						LOG(TRACE) << "[ThingsController::handleGet] {response='NotFound', details='request too short'}";
						reply(request, status_codes::NotFound);
						return;
					}

					if (paths.size() == 2) {
						auto name = utility::conversions::to_utf8string(paths[1]);
						if (name == THING_NAME) {
							auto responseJson = getThingResource();
							LOG(TRACE) << "[ThingsController::handleGet] {response='" << responseJson << "'}";
							reply(request, status_codes::OK, responseJson);
							return;
						}
						else {
							LOG(TRACE) << "[ThingsController::handleGet] {response='NotFound', details='unknown thing'}";
							reply(request, status_codes::NotFound);
							return;
						}
					}
					LOG(TRACE) << "[ThingsController::handleGet] {response='NotImplemented'}";
					reply(request, status_codes::NotImplemented);
					return;
				}
				catch (exception &e) {
					LOG(ERROR) << "[ThingsController::handleGet] {exception} {"
						<< boost::typeindex::type_id_runtime(e) << "} {" << e.what()
						<< "}";
				}
				catch (...) {
					LOG(ERROR) << "[ThingsController::handleGet] {unhandled_exception}";
				}
				reply(request, status_codes::InternalError);
			}

			void ThingsController::handlePost(http_request request) {
				throw illegal_state_exception("Not implemented yet");
			}

			void ThingsController::reply(web::http::http_request &request,
				web::http::status_code statusCode) {
				web::http::http_response response;
				response.set_status_code(statusCode);
				request.reply(response);
			}

			void ThingsController::reply(web::http::http_request &request,
				web::http::status_code statusCode,
				const string &body) {
				web::http::http_response response;
				response.set_status_code(statusCode);

				response.headers().add(
					header_names::content_type, utility::conversions::to_string_t("application/json"));

				response.set_body(body);
				request.reply(response);
			}

			string
				ThingsController::boxListToJSon(const vector<::cleric::BoxId> &boxes) {
				stringstream ss;

				ss << "[";
				bool first = true;
				for (const auto &boxId : boxes) {
					// puts comma between serialized boxes
					if (first) {
						first = false;
					}
					else {
						ss << ", ";
					}

					// find and serialize box
					auto &boxServer = BoxServerLocator::getBoxServerByBoxId(boxId);
					auto box = boxServer.getBoxById(boxId);
					ss << box->toJson();
				}
				ss << "]";

				return ss.str();
			}

		} // namespace controller
	} // namespace http
} // namespace cleric
