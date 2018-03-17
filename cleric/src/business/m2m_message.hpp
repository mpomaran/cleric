/**

MIT License

Copyright (c) 2017 mpomaranski at gmail

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

#ifndef CLERIC_BUSINESS_M2M_MESSAGE_H_
#define CLERIC_BUSINESS_M2M_MESSAGE_H_

#include <cstdint>
#include <string>
#include <vector>
#include "portable/m2m_portable_utils.hpp"

namespace cleric {

using BoxId = uint16_t;
using M2MNonce = uint32_t;
using M2MPaylaod = std::vector<uint8_t>;

class M2MMessage {
 public:
  typedef typename M2MPaylaod::iterator M2MIterator;

  M2MMessage() = delete;
  M2MMessage(const M2MMessage &) = default;
  M2MMessage(const M2MMessage &&other);
  M2MMessage(BoxId boxId, const M2MPaylaod &payload);

  /*
  Decodes the message
  */
  static M2MMessage decode(const std::string &message);
  static std::string encode(const M2MMessage &message);

  /*
  Side channel data, not included in payload
  */
  BoxId getBoxId() const;
  M2MNonce getNonce() const;

  /*
  Size of the payload
  */
  size_t size();

  /*
  Enables access to the payload
  */
  uint8_t &operator[](int x);

  /*
  Methods enabling STL algorithms to work
  */
  M2MIterator begin() { return payload.begin(); };
  M2MIterator end() { return payload.end(); };

 private:
  BoxId boxId;
  M2MNonce nonce;
  M2MPaylaod payload;

  M2MMessage(BoxId boxId, M2MNonce nonce, M2MPaylaod &payload);

  void encryptPayload();
  void decryptPayload();
  yrand_generator nonceToGenerator() const;
};
}  // namespace cleric

#endif
