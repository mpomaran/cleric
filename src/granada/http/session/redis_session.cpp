/**
 * Copyright (c) <2016> granada <afernandez@cookinapps.io>
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
 */

#include "granada/http/session/redis_session.h"

namespace granada {
namespace http {
namespace session {

granada::util::mutex::call_once RedisSession::load_properties_call_once_;
granada::util::mutex::call_once RedisSession::init_statics_once_;

std::unique_ptr<granada::http::session::SessionHandler>
    RedisSession::session_handler_(nullptr);
std::unique_ptr<granada::Functions> RedisSession::close_callbacks_(nullptr);

RedisSession::RedisSession() {
  RedisSession::load_properties_call_once_.call(
      [this]() { this->LoadProperties(); });

  RedisSession::init_statics_once_.call([this]() {
    RedisSession::session_handler_ =
        std::unique_ptr<granada::http::session::SessionHandler>(
            new granada::http::session::RedisSessionHandler());

    RedisSession::close_callbacks_ =
        std::unique_ptr<granada::Functions>(new granada::FunctionsMap());

    roles_ = std::unique_ptr<granada::http::session::SessionRoles>(
        new granada::http::session::RedisSessionRoles(this));
  });
}

RedisSession::RedisSession(const web::http::http_request &request,
                           web::http::http_response &response)
    : RedisSession() {
  Session::LoadSession(request, response);
}

RedisSession::RedisSession(const web::http::http_request &request)
    : RedisSession() {
  Session::LoadSession(request);
}

RedisSession::RedisSession(const std::string &token) : RedisSession() {
  Session::LoadSession(token);
}

granada::util::mutex::call_once RedisSessionHandler::load_properties_call_once_;
granada::util::mutex::call_once RedisSessionHandler::clean_sessions_call_once_;
granada::util::mutex::call_once RedisSessionHandler::initialize_statics_once_;

granada::util::time::timer RedisSessionHandler::clean_sessions_timer_;
std::unique_ptr<granada::cache::CacheHandler> RedisSessionHandler::cache_(
    nullptr);
std::unique_ptr<granada::crypto::NonceGenerator>
    RedisSessionHandler::nonce_generator_(nullptr);
std::unique_ptr<granada::http::session::SessionFactory>
    RedisSessionHandler::factory_(nullptr);

void RedisSessionHandler::initializeStatics() {
  RedisSessionHandler::initialize_statics_once_.call([this]() {
    cache_ = std::unique_ptr<granada::cache::CacheHandler>(
        new granada::cache::RedisCacheDriver());

    nonce_generator_ = std::unique_ptr<granada::crypto::NonceGenerator>(
        new granada::crypto::CPPRESTNonceGenerator());
    RedisSessionHandler::factory_ =
        std::unique_ptr<granada::http::session::SessionFactory>(
            new granada::http::session::RedisSessionFactory());
  });
}

}  // namespace session
}  // namespace http
}  // namespace granada
