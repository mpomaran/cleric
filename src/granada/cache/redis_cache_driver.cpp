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
 * Manages the cache with a redis database.
 */

#include "granada/cache/redis_cache_driver.h"

namespace granada {
namespace cache {

std::string RedisSyncClientWrapper::redis_address_;
unsigned short RedisSyncClientWrapper::redis_port_;
granada::util::mutex::call_once
    RedisSyncClientWrapper::load_properties_call_once_;

RedisSyncClientWrapper::RedisSyncClientWrapper() {
  // load properties only once, and wait all the
  // threads until they are loaded.
  load_properties_call_once_.call([this]() { this->LoadProperties(); });

  // init redis async client
  boost::asio::io_service ioService;
  redis_.reset(new redisclient::RedisSyncClient(ioService));
  ConnectRedisSyncClient(redis_.get(), redis_address_, redis_port_);
}

void RedisSyncClientWrapper::LoadProperties() {
  redis_address_.assign(granada::util::application::GetProperty(
      entity_keys::redis_cache_driver_address));
  if (redis_address_.empty()) {
    redis_address_.assign(default_strings::redis_cache_redis_address);
  }

  std::string redis_port_str = granada::util::application::GetProperty(
      entity_keys::redis_cache_driver_port);
  if (redis_port_str.empty()) {
    redis_port_str = default_strings::redis_cache_redis_port;
    try {
      redis_port_ =
          (unsigned short)std::strtoul(redis_port_str.c_str(), NULL, 0);
    } catch (const std::exception& e) {
    }
  } else {
    try {
      redis_port_ =
          (unsigned short)std::strtoul(redis_port_str.c_str(), NULL, 0);
    } catch (const std::exception& e) {
      redis_port_str = default_strings::redis_cache_redis_port;
      try {
        redis_port_ =
            (unsigned short)std::strtoul(redis_port_str.c_str(), NULL, 0);
      } catch (const std::exception& e) {
      }
    }
  }
}

void RedisSyncClientWrapper::ConnectRedisSyncClient(
    redisclient::RedisSyncClient* redis, const std::string& _address,
    const unsigned short& port) {
  std::string errmsg;
  if (!redis->connect(boost::asio::ip::address::from_string(_address), port,
                      errmsg)) {
    std::cout << "Can't connect to Redis (" << _address << ": " << port << ": "
              << errmsg << std::endl;
  }
}

std::unique_ptr<granada::cache::RedisCacheDriver> RedisIterator::cache_(
    nullptr);

void RedisIterator::initCache() {
  if (cache_ == nullptr) {
    cache_ = std::unique_ptr<granada::cache::RedisCacheDriver>(
        new granada::cache::RedisCacheDriver());
  }
}

RedisIterator::RedisIterator() { initCache(); }

RedisIterator::RedisIterator(const std::string& expression) {
  initCache();
  set(expression);
}

RedisIterator::RedisIterator(RedisIterator::Type type,
                             const std::string& expression) {
  initCache();

  type_ = std::move(type);
  expression_.assign(expression);

  // get first set of keys.
  GetNextVector();
}

void RedisIterator::set(const std::string& expression) {
  set(RedisIterator::Type::KEYS, expression);
}

void RedisIterator::set(RedisIterator::Type type,
                        const std::string& expression) {
  type_ = std::move(type);
  expression_.assign(expression);

  // reset variables.
  index_ = 0;
  cursor_ = "";
  has_next_ = false;

  // get first set of keys.
  GetNextVector();
}

const bool RedisIterator::has_next() { return has_next_; }

const std::string RedisIterator::next() {
  if (keys_.size() > 0 && index_ < keys_.size()) {
    const std::string key(keys_.at(index_).toString());
    if (index_ == keys_.size() - 1) {
      // get new vector
      GetNextVector();
      // reset index
      index_ = 0;
    } else {
      index_++;
    }
    return key;
  }
  return std::string();
}

void RedisIterator::GetNextVector() {
  if (cursor_ == "0") {
    has_next_ = false;
  } else {
    has_next_ = true;
    if (cursor_.empty()) {
      cursor_ = "0";
    }
    if (type_ == 0) {
      const redisclient::RedisValue& result = cache_->Keys(expression_);
      if (result.isOk()) {
        keys_ = result.toArray();
        if (keys_.empty()) {
          has_next_ = false;
        }
      } else {
        has_next_ = false;
      }
    } else if (type_ == 1) {
      // SCAN search.
      const redisclient::RedisValue& result =
          cache_->Scan(cursor_, expression_);
      if (result.isOk()) {
        const std::vector<redisclient::RedisValue>& result_v = result.toArray();
        if (result_v.size() == 2) {
          cursor_ = result_v.at(0).toString();
          keys_ = result_v.at(1).toArray();
          if (keys_.empty()) {
            if (cursor_ == "0") {
              has_next_ = false;
            } else {
              GetNextVector();
            }
          }
        }
      } else {
        has_next_ = false;
      }
    }
  }
}

std::mutex RedisCacheDriver::mtx_;
std::unique_ptr<RedisSyncClientWrapper> RedisCacheDriver::redis_(nullptr);

RedisCacheDriver::RedisCacheDriver() {
  if (redis_ == nullptr) {
    redis_ =
        std::unique_ptr<RedisSyncClientWrapper>(new RedisSyncClientWrapper());
  }
}

const bool RedisCacheDriver::Exists(const std::string& key) {
  const redisclient::RedisValue& result = [&] {
    std::lock_guard<std::mutex> lg(mtx_);
    return redis_->get()->command("EXISTS", {key});
  }();

  if (result.isOk()) {
    if (result.toInt()) {
      return true;
    }
  }
  return false;
}

const bool RedisCacheDriver::Exists(const std::string& hash,
                                    const std::string& key) {
  const redisclient::RedisValue& result = [&] {
    std::lock_guard<std::mutex> lg(mtx_);
    return redis_->get()->command("EXISTS", {hash});
  }();

  if (result.isOk()) {
    if (result.toInt()) {
      return !Read(hash, key).empty();
    }
  }
  return false;
}

const std::string RedisCacheDriver::Read(const std::string& key) {
  const redisclient::RedisValue& result = [&] {
    std::lock_guard<std::mutex> lg(mtx_);
    return redis_->get()->command("GET", {key});
  }();

  if (result.isOk()) {
    return result.toString();
  }
  return std::string();
}

const std::string RedisCacheDriver::Read(const std::string& hash,
                                         const std::string& key) {
  const redisclient::RedisValue& result = [&] {
    std::lock_guard<std::mutex> lg(mtx_);
    return redis_->get()->command("HGET", {hash, key});
  }();

  if (result.isOk()) {
    return result.toString();
  }
  return std::string();
}

void RedisCacheDriver::Write(const std::string& key, const std::string& value) {
  std::lock_guard<std::mutex> lg(mtx_);
  redis_->get()->command("SET", {key, value});
}

void RedisCacheDriver::Write(const std::string& hash, const std::string& key,
                             const std::string& value) {
  std::lock_guard<std::mutex> lg(mtx_);
  redis_->get()->command("HSET", {hash, key, value});
}

void RedisCacheDriver::Destroy(const std::string& key) {
  const std::size_t found(key.find("*"));
  if (found != std::string::npos) {
    std::vector<std::string> keys;
    Match(key, keys);
    for (auto it = keys.begin(); it != keys.end(); ++it) {
      std::lock_guard<std::mutex> lg(mtx_);
      redis_->get()->command("DEL", {*it});
    }
  } else {
    std::lock_guard<std::mutex> lg(mtx_);
    redis_->get()->command("DEL", {key});
  }
}

void RedisCacheDriver::Destroy(const std::string& hash,
                               const std::string& key) {
  std::lock_guard<std::mutex> lg(mtx_);
  redis_->get()->command("HDEL", {hash, key});
}

bool RedisCacheDriver::Rename(const std::string& old_key,
                              const std::string& new_key) {
  const redisclient::RedisValue& result = [&] {
    std::lock_guard<std::mutex> lg(mtx_);
    return redis_->get()->command("RENAMENX", {old_key, new_key});
  }();

  if (result.isOk()) {
    return true;
  }
  return false;
}

redisclient::RedisValue RedisCacheDriver::Scan(const std::string& cursor,
                                               const std::string& expression_) {
  std::lock_guard<std::mutex> lg(mtx_);
  return redis_->get()->command("SCAN", {cursor, "MATCH", expression_});
}

redisclient::RedisValue RedisCacheDriver::Keys(const std::string& expression_) {
  std::lock_guard<std::mutex> lg(mtx_);
  return redis_->get()->command("KEYS", {expression_});
}

}  // namespace cache
}  // namespace granada
