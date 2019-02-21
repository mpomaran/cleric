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

#ifndef ISERIALIZABLE_HAS_BEEN_DEFINED
#define ISERIALIZABLE_HAS_BEEN_DEFINED

#include <stdint.h>
#include <vector>
#include <sstream>

namespace cleric {
namespace data {

class ISerializable {
public:
  virtual std::vector<uint8_t> toByteArray() const = 0;
  virtual void fromByteArray(const std::vector<uint8_t> &data) = 0;

  static ::std::vector<uint8_t> toBuffer(::std::stringstream &ss) {
	  std::vector<uint8_t> result;
	  const auto str = ss.str();
	  result.assign((uint8_t *)str.c_str(), (uint8_t *)str.c_str() + str.size());
	  return result;
  }
};

} // namespace data
} // namespace cleric

#endif
