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

#ifndef MEM_BOX_SERIALIZATION_STRATEGY_HPP_HAS_BEEN_INCLUDED
#define MEM_BOX_SERIALIZATION_STRATEGY_HPP_HAS_BEEN_INCLUDED

#include "box_serialization_strategy.hpp"
#include <easylogging++.h>
#include <istream>
#include <ostream>
#include <string>

namespace cleric {
namespace data {
class MemBoxSerializationStrategy
    : public cleric::data::IBoxSerializationStrategy {
public:
  MemBoxSerializationStrategy(const ::std::string &message) {
    data.assign(message.c_str(), message.c_str() + message.length());
  }
  MemBoxSerializationStrategy() {}

  // Inherited via IBoxSerializationStrategy
  virtual ::std::vector<uint8_t> get() override { return data; }
  virtual void put(const ::std::vector<uint8_t> &data) override {
    this->data = data;
  }

private:
  std::vector<uint8_t> data;
};
} // namespace data
} // namespace cleric

#endif

