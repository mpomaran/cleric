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

#include "portable/m2m_portable_utils.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace cleric {

using BoxId = uint16_t;

struct M2MPayload {
  uint32_t type;
  uint32_t reading;
  uint32_t vcc;
};

class M2MMessage {
public:
  M2MMessage() = delete;
  M2MMessage(const M2MMessage &) = default;
  M2MMessage(const M2MMessage &&other);

  static M2MMessage decode(const std::string &message, uint64_t secret);

  /*
  Side channel data, not included in payload
  */
  BoxId getBoxId() const;
  static BoxId getBoxId(const ::std::string &message);

  uint64_t getSensorPowerSupplyVoltage() const { return payload.vcc; }
  uint64_t getMeasurement() const { return payload.reading; }
  uint64_t getSensorType() const { return payload.type; }

private:
  BoxId boxId;
  M2MPayload payload;

  M2MMessage(const BoxId &boxId, const M2MPayload &p);

  static uint64_t stringToInt(const ::std::string &str);
};
} // namespace cleric

#endif
