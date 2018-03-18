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

#include <boost/endian/buffers.hpp>
#include <boost/endian/conversion.hpp>
#include <cassert>
#include <exception>
#include <memory>
#include <random>
#include <string>
#include <utility>
#include <vector>
#include "portable/base64/base64.hpp"
#include "portable/m2m_portable_utils.hpp"

#include "m2m_message.hpp"

using namespace std;

const size_t MAX_M2M_MESSAGE_SIZE = 128;

namespace cleric {

struct M2MHeader {
  boost::endian::big_uint16_buf_t boxId;
  boost::endian::big_uint32_buf_t nonce;
};

yrand_generator M2MMessage::nonceToGenerator() const {
  return yrand_seed((uint64_t)nonce + (uint64_t)nonce << 32,
                    (uint64_t)nonce + (uint64_t)nonce << 32);
}

void M2MMessage::encryptPayload() {
  auto generator = nonceToGenerator();
  for (auto &b : payload) {
    b ^= yrand_rand(&generator);
  }
}

void M2MMessage::decryptPayload() { encryptPayload(); }

M2MMessage M2MMessage::decode(const std::string &message) {
  if (message.size() == 0 || message.size() > MAX_M2M_MESSAGE_SIZE) {
    throw length_error("M2M message out of bounds");
  }

  auto messageLen = base64_dec_len(message.c_str(), message.size());

  if (messageLen < sizeof(uint8_t) + sizeof(uint64_t)) {
    throw length_error("M2M message too short");
  }



  auto decoded = make_unique<uint8_t[]>(messageLen);
  base64_decode(const_cast<uint8_t *>(decoded.get()), message.c_str(),
                message.size());

  auto header = reinterpret_cast<M2MHeader *>(decoded.get());

  auto encrypted = decoded.get() + sizeof(M2MHeader);
  auto encryptedLen = messageLen - sizeof(M2MHeader);

  std::vector<uint8_t> payload;
  payload.assign(encrypted, encrypted + encryptedLen);

  M2MMessage p = {header->boxId.value(), header->nonce.value(), payload};
  p.decryptPayload();

  return p;
}

std::string M2MMessage::encode(const M2MMessage &msg) {
  M2MMessage buff = msg;
  buff.encryptPayload();

  M2MHeader header;
  header.boxId = buff.getBoxId();
  header.nonce = buff.getNonce();

  auto data = buff.payload;
  data.insert(data.begin(), (uint8_t *)&header,
              ((uint8_t *)&header) + sizeof(M2MHeader));

  auto messageLen = base64_enc_len(data.size());
  auto encoded =
      make_unique<char[]>(messageLen + 1);  // 1 to account for the termination
  auto len = base64_encode(const_cast<char *>(encoded.get()), data.data(),
                           data.size());

  assert(len == messageLen);

  return encoded.get();
}

BoxId M2MMessage::getBoxId() const { return boxId; }

M2MNonce M2MMessage::getNonce() const { return nonce; }

M2MMessage::M2MMessage(BoxId id, const M2MPaylaod &p) : payload(p), boxId(id) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> uniform_dist;

  nonce = uniform_dist(generator);
}

size_t M2MMessage::size() { return payload.size(); }

uint8_t &M2MMessage::operator[](int x) { return payload[x]; }

M2MMessage::M2MMessage(const M2MMessage &&other) {
  boxId = other.boxId;
  nonce = other.nonce;
  payload = std::move(other.payload);
}

M2MMessage::M2MMessage(BoxId boxId, M2MNonce nonce, M2MPaylaod &payload)
    : boxId(boxId), nonce(nonce), payload(payload) {}

}  // namespace cleric
