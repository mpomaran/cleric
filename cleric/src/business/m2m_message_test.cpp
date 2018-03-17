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
#include <algorithm>
#include <catch/catch.hpp>
#include <memory>
#include <string>
#include "portable\base64\base64.hpp"

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
SCENARIO("M2MMessage can be created", "[M2MMessage]") {
  GIVEN("An plain payload, which is then encoded") {
    M2MPaylaod payload = {1, 2, 3};
    BoxId boxId = {1};
    auto m2m = M2MMessage::M2MMessage(boxId, payload);
    auto encodedMsg = M2MMessage::encode(m2m);

    WHEN("box is found and key is valid") {
      THEN("M2M message can be decrypted") {
        try {
          M2MMessage msg = M2MMessage::decode(encodedMsg);
          CHECK(msg.getBoxId() == boxId);
          CHECK(std::equal(payload.begin(), payload.end(), msg.begin()));
        } catch (...) {
          CHECK(false);
        }
      }
    }
  }
}
