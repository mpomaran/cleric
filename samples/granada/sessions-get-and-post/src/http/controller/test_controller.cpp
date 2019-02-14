/**
  * Copyright (c) <2016> HTML Puzzle Team <htmlpuzzleteam@gmail.com>
  *
  * This source code is licensed under the MIT license.
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to deal
  * in the Software without restriction, including without limitation the rights
  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  * copies of the Software, and to permit persons to whom the Software is
  * furnished to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in
  * all copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  *
  * Controller for managing cart.
  *
  */

#include "test_controller.h"

namespace granada{
  namespace http{
    namespace controller{
      TestController::TestController(utility::string_t url,std::shared_ptr<granada::http::session::SessionFactory>& session_factory)
      {
        m_listener_ = std::unique_ptr<http_listener>(new http_listener(url));
        session_factory_ = session_factory;
        m_listener_->support(methods::GET, std::bind(&TestController::handle_get, this, std::placeholders::_1));
        m_listener_->support(methods::POST, std::bind(&TestController::handle_post, this, std::placeholders::_1));
      }

      void TestController::handle_get(web::http::http_request request)
      {

        web::http::http_response response;

        {
          auto paths = uri::split_path(uri::decode(request.relative_uri().path()));
          if (!paths.empty()){
            std::string name = utility::conversions::to_utf8string(paths[0]);

            std::unique_ptr<granada::http::session::Session> session = session_factory_->Session_unique_ptr(request,response);

            if(name == "set"){
              session->Write("test","testvalue!");
              std::string value = session->Read("test");
              response.set_body("{\"success\":true,\"value\":\"" + value + "\",\"token\":\"" + session->GetToken() + "\"}");

            }else if (name == "get"){
              std::string value = session->Read("test");
              response.set_body("{\"success\":true,\"value\":\"" + value + "\",\"token\":\"" + session->GetToken() + "\"}");
            }else if (name == "auth"){
              if (session->GetToken().empty()){
                session->Open();
              }
              response.set_body("{\"success\":true,\"token\":\"" + session->GetToken() + "\"}");
            }
          }
        }

        response.set_status_code(status_codes::OK);
        response.headers().add(U("Content-Type"), U("text/json; charset=utf-8"));

        request.reply(response);
      }

      void TestController::handle_post(web::http::http_request request)
      {
        web::http::http_response response;

        {
          auto paths = uri::split_path(uri::decode(request.relative_uri().path()));
          if (!paths.empty()){
            std::string name = utility::conversions::to_utf8string(paths[0]);

            std::unique_ptr<granada::http::session::Session> session = session_factory_->Session_unique_ptr(request,response);
            if(name == "set"){
              session->Write("test","testvalue!");
              std::string value = session->Read("test");
              response.set_body("{\"success\":true,\"value\":\"" + value + "\",\"token\":\"" + session->GetToken() + "\"}");

            }else if (name == "get"){
              std::string value = session->Read("test");
              response.set_body("{\"success\":true,\"value\":\"" + value + "\",\"token\":\"" + session->GetToken() + "\"}");
            }else if (name == "auth"){
              if (session->GetToken().empty()){
                session->Open();
              }
              response.set_body("{\"success\":true,\"token\":\"" + session->GetToken() + "\"}");
            }
          }
        }

        response.set_status_code(status_codes::OK);
        response.headers().add(U("Content-Type"), U("text/json; charset=utf-8"));

        request.reply(response);
      }
    }
  }
}
