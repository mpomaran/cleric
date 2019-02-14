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

#include "m2m_controller.hpp"
#include "../business/m2m_message.hpp"
#include "../data/box.hpp"
#include "../data/box_server.hpp"
#include "../data/box_server_locator.hpp"
#include <boost/type_index.hpp>
#include <easylogging++.h>

using namespace cleric::data;
using namespace cleric;

bool cleric::http::controller::test::isPostInvoked = false;
bool cleric::http::controller::test::isGetInvoked = false;

cleric::http::controller::M2MController::M2MController(
    utility::string_t url,
    std::shared_ptr<granada::http::session::SessionFactory> &session_factory) {

  m_listener_ = std::unique_ptr<http_listener>(new http_listener(url));
  m_listener_->support(methods::GET, std::bind(&M2MController::handleGet, this,
                                               std::placeholders::_1));

  LOG(INFO) << "[M2MController] {m2m_controller_created}";
}

void cleric::http::controller::M2MController::handleGet(http_request request) {
  try {
    cleric::http::controller::test::isGetInvoked = true;

    web::http::http_response response;
    response.set_status_code(status_codes::NotAcceptable);

    VLOG(1) << "[M2MController] {m2m_controller_handle_get}";

    auto paths = uri::split_path(uri::decode(request.relative_uri().path()));
    if (!paths.empty()) {
      auto m2mString = utility::conversions::to_utf8string(paths[0]);

      VLOG(2) << "[M2MController] {m2m_controller_handle_get} {m2mString="
              << m2mString << "}";

      auto boxId = M2MMessage::getBoxId(m2mString);
      auto & boxServer = BoxServerLocator::getBoxServerByBoxId(boxId);

      auto box = boxServer.getBoxById(boxId);
      if (!box) {
        LOG(ERROR) << "[M2MController] {unknown_box} {" << boxId << "}";
      } else {
        response.set_body(box->process(m2mString));
        response.set_status_code(status_codes::OK);
      }
    }
    request.reply(response);
  } catch (std::exception &e) {
    LOG(ERROR) << "[M2MController] {exception} {"
               << boost::typeindex::type_id_runtime(e) << "} {" << e.what()
               << "}";
  } catch (...) {
    LOG(ERROR) << "[M2MController] {unhandled_exception}";
  }
}

void cleric::http::controller::M2MController::handlePost(http_request request) {
  cleric::http::controller::test::isPostInvoked = true;
}
