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

#ifndef REST_CONTROLLER_HPP_HAS_BEEN_INCLUDED
#define REST_CONTROLLER_HPP_HAS_BEEN_INCLUDED

#include "cpprest/details/basic_types.h"
#include "granada/http/controller/controller.h"
#include "granada/http/parser.h"
#include "granada/http/session/session.h"
#include <unordered_map>

namespace cleric {
namespace http {
namespace controller {

namespace test {
class RestControllerTester;
extern bool isGetInvoked;
extern bool isPostInvoked;

} // namespace test

class M2MController : public granada::http::controller::Controller {
public:
  /**
   * Constructor
   */
  M2MController(
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

  friend class test::RestControllerTester;
};
} // namespace controller
} // namespace http
} // namespace cleric

#endif
