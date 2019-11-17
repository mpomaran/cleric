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

#include "boxes_controller.hpp"
#include "../data/box.hpp"
#include "../data/box_server.hpp"
#include "../data/box_server_locator.hpp"
#include <boost/type_index.hpp>
#include <easylogging++.h>
#include <sstream>

using namespace cleric::data;
using namespace cleric;
using namespace std;

namespace cleric {
namespace http {
namespace controller {
BoxesController::BoxesController(
    utility::string_t url,
    shared_ptr<granada::http::session::SessionFactory> &session_factory) {

  m_listener_ = unique_ptr<http_listener>(new http_listener(url));
  m_listener_->support(
      methods::GET, bind(&BoxesController::handleGet, this, placeholders::_1));

  LOG(INFO)
      << "[BoxesController::BoxesController] {/api/boxes controller created}";
}

void BoxesController::handleGet(http_request request) {
  try {
    web::http::http_response response;
    response.set_status_code(status_codes::NotAcceptable);

    auto uri = request.absolute_uri().path();
    VLOG(1) << "[BoxesController::handleGet] {request='" << uri << "'}";

    auto paths = uri::split_path(uri::decode(uri));

    if (paths.size() == 2) {
      // get all possible boxes

      // TODO lookup for server based on user, etc - currently I know we have
      // only one server, so I can ask for anythong fix before the first release
      auto &boxServer = BoxServerLocator::getBoxServerByBoxId(0);

      auto allBoxes = boxServer.getAllBoxes();
      auto body = boxListToJSon(allBoxes);
	  reply(request, status_codes::OK, body);
	  return;
    } else if (paths.size() == 3) {
      // get specific box
      auto boxId = stoi(paths[2]);
      auto &boxServer = BoxServerLocator::getBoxServerByBoxId(boxId);
      auto box = boxServer.getBoxById(boxId);
      if (!box) {
        LOG(ERROR) << "[BoxesController::handleGet] {unknown_box} {" << boxId
                   << "}";
        reply(request, status_codes::NotFound);
        return;
      }

      auto body = box->toJson();
      reply(request, status_codes::OK, body);
      return;
    } else {
      reply(request, status_codes::BadRequest);
      return;
    }
  } catch (exception &e) {
    LOG(ERROR) << "[BoxesController::handleGet] {exception} {"
               << boost::typeindex::type_id_runtime(e) << "} {" << e.what()
               << "}";
  } catch (...) {
    LOG(ERROR) << "[BoxesController::handleGet] {unhandled_exception}";
  }
  reply(request, status_codes::InternalError);
}

void BoxesController::handlePost(http_request request) {
  throw illegal_state_exception("Not implemented yet");
}

void BoxesController::reply(web::http::http_request &request,
                            web::http::status_code statusCode) {
  web::http::http_response response;
  response.set_status_code(statusCode);
  request.reply(response);
}

void BoxesController::reply(web::http::http_request &request,
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
BoxesController::boxListToJSon(const vector<::cleric::BoxId> &boxes) {
  stringstream ss;

  ss << "[";
  bool first = true;
  for (const auto &boxId : boxes) {
    // puts comma between serialized boxes
    if (first) {
      first = false;
    } else {
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
