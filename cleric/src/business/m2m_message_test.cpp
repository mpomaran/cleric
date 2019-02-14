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
#include "m2m_message.hpp"
#include "portable/base64/base64.hpp"
#include <algorithm>
#include <catch/catch.hpp>
#include <cstring>
#include <memory>
#include <string>

using namespace cleric;
using namespace std;

SCENARIO("Base64 encoded message can be encoded", "[base64]") {
  GIVEN("A byte array") {
    string str = "abcdef1234567890we##$dd__-/";

    WHEN("message is encoded") {
      auto encoded = make_unique<char[]>(base64_enc_len(str.length()) + 1);
      auto encodedLen =
          base64_encode(const_cast<char *>(encoded.get()),
                        (const uint8_t *)(str.data()), str.length());
      CHECK(encodedLen > str.length());
      CHECK(encodedLen == base64_enc_len(str.length()));
      THEN("message can be decoded properly") {
        auto decoded =
            make_unique<uint8_t[]>(base64_dec_len(encoded.get(), encodedLen));
        auto decodedLen = base64_decode(const_cast<uint8_t *>(decoded.get()),
                                        encoded.get(), encodedLen);
        CHECK(memcmp(str.data(), decoded.get(), decodedLen) == 0);
        CHECK(decodedLen == str.length());
      }
    }
  }
}
/*

0x00aff6c8 "0000000ABE587DE600000000AE30AFFCAC41EAF787E215F4"

0 8 0000 000A box
8 8 BE58 7DE6 ts
16 16 0000 0000 AE30 AFFC rand
32 16 AC41 EAF7 F8E2 1601 pl


box = 10
reading 999
type = 509
secret = 0x474996006560796aL
vcc = 1000

yerrno_t ymessage_construct(uint8_t * buff, uint8_t buff_len, uint32_t box_id,
uint64_t payload, ymessage_cb message_ready_cb)
{
        uint64_t timestamp = (uint32_t)(ytimer_get_uptime_in_ms() / 1000) &
0xffffffff; uint64_t random = yrand_rand(); uint8_t i; uint64_t encoded_payload
= 0;

        yassert(buff_len >= (sizeof(uint64_t) * 3 * 2 + 1));

        buff += uint64_to_bytes(buff, timestamp + ((uint64_t)box_id << 32)) - 1;
        buff += uint64_to_bytes(buff, random) - 1;

        yrand_seed(timestamp ^ random, SECRET_KEY);

        for (i = 0; i < sizeof(payload); i++) {
                uint8_t b = payload >> (i * 8) & 0xff;
                uint8_t key = (uint8_t)(yrand_rand() & 0xff);
                encoded_payload += ((uint64_t)(b ^ key)) << (i * 8);
        }

        buff += uint64_to_bytes(buff, encoded_payload);

        message_ready_cb();

        return Y_OK;
}

*/

TEST_CASE("M2MMessage from box can be decoded", "[M2MMessage]") {
  const auto MESSAGE = "0000000ABE587DE600000000AE30AFFCD31EA91087E215F4";
  const auto BOX_ID = 10;
  const auto READING = 999;
  const auto TYPE = 509;
  const uint64_t SECRET = 0x474996006560796aUL;
  const auto VCC = 1000;

  auto decodedBoxId = M2MMessage::getBoxId(MESSAGE);
  CHECK(decodedBoxId == BOX_ID);

  M2MMessage m2m = M2MMessage::decode(MESSAGE, SECRET);
  CHECK(m2m.getBoxId() == BOX_ID);
  CHECK(m2m.getMeasurement() == READING);
  CHECK(m2m.getSensorPowerSupplyVoltage() == VCC);
  CHECK(m2m.getSensorType() == TYPE);
}
