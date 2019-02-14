/**

MIT License

Copyright (c) 2018 mpomaranski at gmail

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

#include "cleric_app.hpp"
#include <stdio.h>
#include <string>
#include "cpprest/details/basic_types.h"
#include "granada/cache/redis_cache_driver.h"
#include "granada/defaults.h"
#include "granada/http/controller/browser_controller.h"
#include "granada/http/controller/oauth2_controller.h"
#include "granada/http/oauth2/redis_oauth2.h"
#include "granada/http/session/map_session.h"
#include "granada/http/session/redis_session.h"

ClericApp::ClericApp() {}

ClericApp::~ClericApp() {
  for (auto const& controller : controllers) {
    controller->close().wait();
  }
  ucout << "Controllers destroyed" << std::endl;
}

void ClericApp::go() {
  std::string port_str = granada::util::application::GetProperty("port");
  if (port_str.empty()) {
    port_str = "80";
  }
  utility::string_t port = utility::conversions::to_string_t(port_str);

  std::string address_str = granada::util::application::GetProperty("address");
  if (address_str.empty()) {
    address_str = "http://localhost:";
  } else {
    address_str += ":";
  }

  utility::string_t address = utility::conversions::to_string_t(address_str);
  address.append(port);

  ////
  // Vector containing all used controllers.
  ////
  // Browser Controller
  // Permits to browse server resources.

  // get property "browser_module" from the server configuration file
  // If this property equals "on" we will use browser controller.
  std::string browser_module =
      granada::util::application::GetProperty("browser_controller");
  if (!browser_module.empty() && browser_module == "on") {
    std::shared_ptr<granada::http::session::SessionFactory>
        map_simple_session_factory(
            new granada::http::session::MapSessionFactory());

    uri_builder uri(address);
    auto addr = uri.to_uri().to_string();
    std::unique_ptr<granada::http::controller::Controller> browser_controller(
        new granada::http::controller::BrowserController(
            addr, map_simple_session_factory));
    browser_controller->open().wait();
    controllers.push_back(std::move(browser_controller));
    ucout << "Browser Controller: Initialized... Listening for requests at: "
          << addr << std::endl;
  }

  // uri_builder uri(address);
  // uri.append_path(U("cart"));
  // auto addr = uri.to_uri().to_string();
  // std::unique_ptr<granada::http::controller::CartController> cart_controller(
  //    new granada::http::controller::CartController(addr));
  // try {
  //  cart_controller->open().wait();
  //  g_controllers.push_back(std::move(cart_controller));
  //  ucout << "Cart Controller: Initialized... Listening for requests at: "
  //        << addr << std::endl;

  //  uri_builder auth_uri(address);
  //  auth_uri.append_path(U("auth"));
  //  addr = auth_uri.to_uri().to_string();
  //  std::unique_ptr<granada::http::controller::AuthController>
  //  auth_controller(
  //      new granada::http::controller::AuthController(addr));
  //  auth_controller->open().wait();
  //  g_controllers.push_back(std::move(auth_controller));
  //  ucout << "Auth Controller: Initialized... Listening for requests at: "
  //        << addr << std::endl;
  //} catch (std::exception& e) {
  //  ucout << e.what() << std::endl;
  //  throw;
  //}

  return;
}