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

#include "granada/http/oauth2/redis_oauth2.h"

namespace granada {

namespace http {

namespace oauth2 {

RedisOAuth2Client::RedisOAuth2Client() {
  RedisOAuth2Client::load_properties_call_once_.call([this]() {
    this->LoadProperties();
    if (cache_ == nullptr) {
      cache_ = std::unique_ptr<granada::cache::CacheHandler>(
          new granada::cache::RedisCacheDriver());

      cryptograph_ = std::unique_ptr<granada::crypto::Cryptograph>(
          new granada::crypto::OpensslAESCryptograph());

      n_generator_ = std::unique_ptr<granada::crypto::NonceGenerator>(
          new granada::crypto::CPPRESTNonceGenerator());
    }
  });
}

RedisOAuth2Client::RedisOAuth2Client(const std::string& id)
    : RedisOAuth2Client() {
  id_ = id;
  Load();
};

RedisOAuth2User::RedisOAuth2User() {
  RedisOAuth2User::load_properties_call_once_.call([this]() {
    this->LoadProperties();
    if (cache_ == nullptr) {
      cache_ = std::unique_ptr<granada::cache::CacheHandler>(
          new granada::cache::RedisCacheDriver());

      cryptograph_ = std::unique_ptr<granada::crypto::Cryptograph>(
          new granada::crypto::OpensslAESCryptograph());

      n_generator_ = std::unique_ptr<granada::crypto::NonceGenerator>(
          new granada::crypto::CPPRESTNonceGenerator());
    }
  });
};

RedisOAuth2User::RedisOAuth2User(const std::string& username)
    : RedisOAuth2User() {
  username_ = username;
  Load();
};

RedisOAuth2Code::RedisOAuth2Code() {
  RedisOAuth2Code::load_properties_call_once_.call([this]() {
    this->LoadProperties();
    if (cache_ == nullptr) {
      cache_ = std::unique_ptr<granada::cache::CacheHandler>(
          new granada::cache::RedisCacheDriver());

      cryptograph_ = std::unique_ptr<granada::crypto::Cryptograph>(
          new granada::crypto::OpensslAESCryptograph());

      n_generator_ = std::unique_ptr<granada::crypto::NonceGenerator>(
          new granada::crypto::CPPRESTNonceGenerator());
    }
  });
};

RedisOAuth2Code::RedisOAuth2Code(const std::string& code) : RedisOAuth2Code() {
  code_ = code;
  Load();
};

RedisOAuth2Authorization::RedisOAuth2Authorization() {
  RedisOAuth2Authorization::load_properties_call_once_.call([this]() {
    this->LoadProperties();
    if (oauth2_factory_ == nullptr) {
      oauth2_factory_ = std::unique_ptr<granada::http::oauth2::OAuth2Factory>(
          new granada::http::oauth2::RedisOAuth2Factory());

      cache_ = std::unique_ptr<granada::cache::CacheHandler>(
          new granada::cache::RedisCacheDriver());
    }
  });
};

RedisOAuth2Authorization::RedisOAuth2Authorization(
    const granada::http::oauth2::OAuth2Parameters& oauth2_parameters,
    granada::http::session::SessionFactory* session_factory)
    : RedisOAuth2Authorization() {
  oauth2_parameters_ = oauth2_parameters;
  session_factory_ = session_factory;
};

granada::util::mutex::call_once RedisOAuth2Client::load_properties_call_once_;
std::unique_ptr<granada::cache::CacheHandler> RedisOAuth2Client::cache_(
    nullptr);
std::unique_ptr<granada::crypto::Cryptograph> RedisOAuth2Client::cryptograph_(
    nullptr);
std::unique_ptr<granada::crypto::NonceGenerator>
    RedisOAuth2Client::n_generator_(nullptr);

granada::util::mutex::call_once RedisOAuth2User::load_properties_call_once_;
std::unique_ptr<granada::cache::CacheHandler> RedisOAuth2User::cache_(nullptr);
std::unique_ptr<granada::crypto::Cryptograph> RedisOAuth2User::cryptograph_(
    nullptr);
std::unique_ptr<granada::crypto::NonceGenerator> RedisOAuth2User::n_generator_(
    nullptr);

granada::util::mutex::call_once RedisOAuth2Code::load_properties_call_once_;
std::unique_ptr<granada::cache::CacheHandler> RedisOAuth2Code::cache_(nullptr);
std::unique_ptr<granada::crypto::Cryptograph> RedisOAuth2Code::cryptograph_(
    nullptr);
std::unique_ptr<granada::crypto::NonceGenerator> RedisOAuth2Code::n_generator_(
    nullptr);

granada::util::mutex::call_once
    RedisOAuth2Authorization::load_properties_call_once_;
std::unique_ptr<granada::http::oauth2::OAuth2Factory>
    RedisOAuth2Authorization::oauth2_factory_(nullptr);
std::unique_ptr<granada::cache::CacheHandler> RedisOAuth2Authorization::cache_(
    nullptr);
}  // namespace oauth2
}  // namespace http
}  // namespace granada
