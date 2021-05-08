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
#include <memory>
#include <boost/tokenizer.hpp>

using namespace cleric::data;
using namespace cleric;
using namespace std;
using namespace picojson;

namespace cleric {
	namespace http {
		namespace controller {

			string ThingsController::getSensorVccPropertyKey(const BoxId &id) {
				return std::string("mix_box_") + std::to_string(id) + "_vcc";
			}

			string ThingsController::getSensorTypePropertyKey(const BoxId &id) {
				return std::string("mix_box_") + std::to_string(id) + "_value_type";
			}

			string ThingsController::getSensorValuePropertyKey(const BoxId &id) {
				return std::string("mix_box_") + std::to_string(id) + "_value";
			}

			string ThingsController::getSensorRcvTimestampPropertyKey(const BoxId &id) {
				return std::string("mix_box_") + std::to_string(id) + "_rcv";
			}

			ThingsController::ThingsController(
				utility::string_t url,
				shared_ptr<granada::http::session::SessionFactory> &session_factory) {

				m_listener_ = unique_ptr<http_listener>(new http_listener(url));
				m_listener_->support(
					methods::GET, bind(&ThingsController::handleGet, this, placeholders::_1));

				LOG(INFO)
					<< "[ThingsController::ThingsController] {/things controller created}";
			}

			struct SensorPropertyDescription {
				string title;
				string type;
				string unit;
				bool readOnly;
				string description;
				string shortName;
				/*
				SensorPropertyDescription(string title_,
				string type_,
				string unit_,
				bool readOnly_,
				string description_,
				string shortName_) : title(title_),
					type (type_),
					unit(unit_),
					readOnly(readOnly_),
					description(description_),
					shortName(shortName_) {}*/
			};

			static picojson::value createProperty(
				string linkPrefix,
				string boxName,
				BoxId boxId,
				const SensorPropertyDescription & propDescription
			) {
				value::object result;
				string name = "Box " + boxName + " " + propDescription.title;

				result["title"] = value(name);
				result["type"] = value(propDescription.type);
				result["unit"] = value(propDescription.unit);
				result["read-only"] = value(propDescription.readOnly ? "true" : "false");
				result["description"] = value(propDescription.description);

				value::array link;
				value::object href;
				href["href"] = value(linkPrefix + to_string(boxId) + "_" + propDescription.shortName);
				link.push_back(value(href));
				result["links"] = value(link);

				return value(result);
			}


			map<Box::Reading::ReadableType, SensorPropertyDescription> sensorToNameMapping = {
				{Box::Reading::ReadableType::TEMPERATURE,
					{
						"temperature",
						"number",
						"[degC]",
						true,
						"Current temparature"
						"value"
					}
				},
				{Box::Reading::ReadableType::UNKNOWN,
					{
						"reading",
						"number",
						"[1]",
						true,
						"Current sensor reading"
						"value"
					}
				}
			};

			unique_ptr<::value::object> ThingsController::getThingPropertiesResource() const {

				unique_ptr<value::object> properties = make_unique<value::object>();

				// iterate over all boxes
				auto servers = BoxServerLocator::getAllServers();
				for (auto server : servers) {
					auto boxIds = server->getAllBoxes();

					for (auto boxId : boxIds) {
						auto box = server->getBoxById(boxId);

						auto linkPrefix = string("/things/") + string(THING_NAME) + string("/properties/");

						auto boxName = box->getName();

						// vcc property
						{
							(*properties)[ThingsController::getSensorVccPropertyKey(boxId)] =
								createProperty(
									linkPrefix,
									boxName,
									boxId,
									{
										"voltage",
										"number",
										"[V]",
										true,
										"Current battery voltatge",
										"vcc"
									}
							);
						}

						{
							// value
							auto readingType = box->getSensorReadingType();
							auto propDescIterator = sensorToNameMapping.find(readingType);
							if (propDescIterator != sensorToNameMapping.end()) {
								propDescIterator = sensorToNameMapping.find(Box::Reading::ReadableType::UNKNOWN);
								LOG(ERROR) << "Unknown type: " << readingType;
							}
							const auto & propDesc = propDescIterator->second;
							(*properties)[ThingsController::getSensorValuePropertyKey(boxId)] =
								createProperty(
									linkPrefix,
									boxName,
									boxId,
									propDesc
								);

						}

						// value type property
						{
							(*properties)[ThingsController::getSensorTypePropertyKey(boxId)] =
								createProperty(
									linkPrefix,
									boxName,
									boxId,
									{
										"type",
										"number",
										"[V]",
										true,
										"ID if the sensor type",
										"type"
									}
							);
						}

						{
							// receive date time property
							(*properties)[ThingsController::getSensorRcvTimestampPropertyKey(boxId)] =
								createProperty(
									linkPrefix,
									boxName,
									boxId,
									{
										"last message",
										"string",
										"[t]",
										true,
										"timestamp of the last message",
										"rcv"
									}
							);
						}
					}
				}

				return properties;
			}

			std::string ThingsController::getThingResource() const
			{
				picojson::value::object data;

				// if does not compile check syntax here: https://github.com/kazuho/picojson/pull/53

				data["id"] = picojson::value(std::string("") + "https://" + ::cleric::network::getHostIp() + "/things/" + THING_NAME);
				data["title"] = picojson::value(THING_NAME);
				data["description"] = picojson::value("MixBox bridge");

				auto properties = getThingPropertiesResource();

				data["properties"] = picojson::value(*properties);

				return picojson::value(data).serialize();
			}

			std::string ThingsController::getPropertiesResource() const
			{
				value::object properties = value::object();

				// iterate over all boxes
				auto servers = BoxServerLocator::getAllServers();
				for (auto server : servers) {
					auto boxIds = server->getAllBoxes();

					for (auto boxId : boxIds) {
						auto box = server->getBoxById(boxId);
						if (box->size() > 0) {
							auto data = (*box)[box->size() - 1];
							properties[ThingsController::getSensorVccPropertyKey(boxId)] = value(data.vcc);
							properties[ThingsController::getSensorValuePropertyKey(boxId)] = value(data.value);
							properties[ThingsController::getSensorTypePropertyKey(boxId)] = value((double)data.sensorType);
							properties[ThingsController::getSensorRcvTimestampPropertyKey(boxId)] = value(data.rcvTime);
						}
					}
				}

				return value(properties).serialize();
			}

			std::string ThingsController::getPropertyResource(const BoxId &boxId, const std::string &param) const
			{
				value::object prop = value::object();

				auto &boxServer = BoxServerLocator::getBoxServerByBoxId(boxId);

				auto box = boxServer.getBoxById(boxId);
				if (box->size() > 0) {
					auto data = (*box)[box->size() - 1];
					if (param == "vcc") {
						prop[ThingsController::getSensorVccPropertyKey(boxId)] = value((double)data.vcc);
					}
					else if (param == "value") {
						prop[ThingsController::getSensorValuePropertyKey(boxId)] = value((double)data.value);
					}
					else if (param == "type") {
						prop[ThingsController::getSensorTypePropertyKey(boxId)] = value((double)data.sensorType);
					}
					else if (param == "rcv") {
						prop[ThingsController::getSensorTypePropertyKey(boxId)] = value(data.rcvTime);
					}
					else {
						throw runtime_error("Illegal resource");
					}
				}

				return value(prop).serialize();
			}

			void ThingsController::handleGet(http_request request) {
				// TODO refactor

				try {
					web::http::http_response response;
					response.set_status_code(status_codes::NotFound);

					auto uri = request.absolute_uri().path();
//					LOG(INFO) << "[ThingsController::handleGet] {request='" << uri << "'}";

					auto paths = uri::split_path(uri::decode(uri));

					if (paths.size() < 2) {
						// wrongs request, return with 404
						LOG(TRACE) << "[ThingsController::handleGet] {response='NotFound', details='request too short'}";
						reply(request, status_codes::NotFound);
						return;
					}

					if (paths.size() >= 2) {
						auto name = utility::conversions::to_utf8string(paths[1]);
						if (name != THING_NAME) {
							LOG(TRACE) << "[ThingsController::handleGet] {response='NotFound', details='unknown thing'}";
							reply(request, status_codes::NotFound);
							return;
						}
					}

					if (paths.size() == 2) {
						auto responseJson = getThingResource(); // handle thing descriptor
						LOG(TRACE) << "[ThingsController::handleGet] {response='" << responseJson << "'}";
						reply(request, status_codes::OK, responseJson);
						return;
					}
					else if (paths.size() == 3) {
						auto param = utility::conversions::to_utf8string(paths[2]);

						if (param == PROPERTIES_URL) { // hande properties
							auto responseJson = getPropertiesResource();
							LOG(TRACE) << "[ThingsController::handleGet] {response='" << responseJson << "'}";
							reply(request, status_codes::OK, responseJson);
							return;

						}
						else {
							LOG(TRACE) << "[ThingsController::handleGet] {response='NotImplemented', details='not implemented yet'}";
							reply(request, status_codes::NotImplemented);
							return;
						}

					}
					else if (paths.size() == 4) {	// handle single property
						auto p1 = utility::conversions::to_utf8string(paths[2]);
						if (p1 == PROPERTIES_URL) {
							try {
								auto p2 = utility::conversions::to_utf8string(paths[3]);
								if (p2.find("mix_box_") == 0) {
									p2.erase(0, std::string("mix_box_").size());	// TODO make more efficient
								}

								boost::char_separator<char> sep("_");
								boost::tokenizer<boost::char_separator<char>> tokens(p2, sep);
								auto it = tokens.begin();

								if (it != tokens.end()) {
									auto boxIdStr = *it;
									BoxId boxId = std::stoi(boxIdStr);
									it++;
									if (it != tokens.end()) {
										auto param = *it;

										if (param == "value" || param == "vcc" || param == "type") {

											auto responseJson = getPropertyResource(boxId, param);
//											LOG(TRACE) << "[ThingsController::handleGet] {response='" << responseJson << "'}";
											reply(request, status_codes::OK, responseJson);
											return;
										}
									}
								}
							}
							catch (const std::exception &e) {
								LOG(ERROR) << "[ThingsController::handleGet] {exception='" << e.what() << "'}";
							}

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
				reply(request, status_codes::InternalError);		// TODO reply with not found
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
