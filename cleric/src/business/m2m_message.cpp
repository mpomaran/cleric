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

#include "portable/base64/base64.hpp"
#include "portable/m2m_portable_utils.hpp"
#include <boost/endian/buffers.hpp>
#include <boost/endian/conversion.hpp>
#include <cassert>
#include <exception>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "m2m_message.hpp"

using namespace std;

const size_t MAX_M2M_MESSAGE_SIZE = 128;

namespace cleric {

const int M2MMessage::OFFSET = 1;

M2MMessage::M2MMessage(const BoxId &boxId_, const M2MPayload &p_)
    : boxId(boxId_), payload(p_) {}

uint64_t M2MMessage::stringToInt(const ::std::string &str) {
  uint64_t result;
  std::stringstream ss;
  ss << std::hex << str;
  ss >> result;
  return result;
}

static void bufferToSensorData(uint64_t buff, uint32_t &type, uint32_t &value,
                               uint32_t &vcc) {
  type = (buff >> 22) & 511;
  value = buff & 1023;
  vcc = (buff >> 11) & 1023;
}

M2MMessage M2MMessage::decode(const std::string &message, uint64_t secret) {
  if (message.size() != 49) {
    throw length_error("M2M message out of bounds");
  }

  uint64_t boxId = stringToInt(message.substr(0 + OFFSET, 8));
  uint64_t timestamp = stringToInt(message.substr(8 + OFFSET, 8));
  uint64_t random = stringToInt(message.substr(16 + OFFSET, 16));
  uint64_t payloadRaw = stringToInt(message.substr(32 + OFFSET, 16));

  uint64_t decodedPayload = 0;
  auto yrand_generator = yrand_seed(timestamp ^ random, secret);
  for (int i = 0; i < sizeof(payloadRaw); i++) {
    uint8_t b = payloadRaw >> (i * 8) & 0xff;
    uint8_t key = (uint8_t)(yrand_rand(&yrand_generator) & 0xff);
    decodedPayload += ((uint64_t)(b ^ key)) << (i * 8);
  }

  // check if copies match
  uint32_t c1 = decodedPayload & 0xffffffff;
  uint32_t c2 = (decodedPayload >> 32) & 0xffffffff;

  if (c1 != c2) {
    // TODO: potential security flaw, but there is security by obscurity in this
    // solution
    // TODO: review once real encryption is in place
    throw invalid_argument("Wrong format, CRC fail");
  }

  uint32_t type, reading, vcc;
  bufferToSensorData(decodedPayload, type, reading, vcc);

  M2MPayload payload = M2MPayload{type, reading, vcc};

  M2MMessage p = {(BoxId)boxId, payload};

  return p;
}

BoxId M2MMessage::getBoxId() const { return boxId; }

BoxId M2MMessage::getBoxId(const ::std::string &message) {
  if (message.size() < 8) {
    throw invalid_argument("Message too short");
  }

  return (BoxId)stringToInt(message.substr(0+OFFSET, 8));
}

M2MMessage::M2MMessage(const M2MMessage &&other) {
  boxId = other.boxId;
  payload = std::move(other.payload);
}

} // namespace cleric
