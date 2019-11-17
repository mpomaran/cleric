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


pi@whale:~/repos/arcticpanther/Cleric/build/Binaries $ sudo ./cleric-server --prop-file=/home/pi/repos/arcticpanther/Cleric/build/cleric.properties --storage-strategy=hdd --storage-root=/home/pi/storage --root_path=/home/pi/repos/arcticpanther/Cleric/cleric/www/cleric-front/dist/cleric-front -v
2019-02-22 22:16:42,853 INFO [default] [main] {prop-file} {/home/pi/repos/arcticpanther/Cleric/build/cleric.properties}
2019-02-22 22:16:42,854 INFO [default] [main] {root_path} {/home/pi/repos/arcticpanther/Cleric/cleric/www/cleric-front/dist/cleric-front}
2019-02-22 22:16:42,854 INFO [default] [main] {storage-root} {/home/pi/storage}
2019-02-22 22:16:42,854 INFO [default] [main] {storage-strategy} {hdd}
2019-02-22 22:16:42,862 INFO [default] [ClericApp] {browser_controller_initialized} {http://0.0.0.0:8081/}
2019-02-22 22:16:42,863 INFO [default] [M2MController] {/m2m conroller created}
2019-02-22 22:16:42,864 INFO [default] [ClericApp] {m2m_controller initialized} {http://0.0.0.0:8081/m2m/1/}
2019-02-22 22:16:42,864 INFO [default] [BoxesController::BoxesController] {/api/boxes controller created}
2019-02-22 22:16:42,865 INFO [default] [ClericApp] {boxes_controller initialized} {http://0.0.0.0:8081/api/boxes}
Press RETURN to exit
2019-02-22 22:17:45,172 VERBOSE-1 [default] [M2MController] {m2m_controller_handle_get}
2019-02-22 22:17:45,173 VERBOSE-2 [default] [M2MController] {m2m_controller_handle_get} {m2mString=00000000100000000000000003F361CCB02C928BB461DD4EF}
2019-02-22 22:17:45,173 INFO [default] [BoxServer::delayedWriteThread] {thread_started}
2019-02-22 22:17:45,174 INFO [default] [BoxServer@0x66689c@1952724048::getBoxById] {createBox} {boxId='0' name='0' storageStrategy='hdd' retentionPeriod='8760' maxEntries='1000'}
2019-02-22 22:17:45,174 INFO [default] [HddBoxSerializationStrategy::HddBoxSerializationStrategy] {set_paths} {filename=/home/pi/storage/box_0.dat}
2019-02-22 22:17:45,174 WARNING [default] [HddBoxSerializationStrategy::get] {cannot open file for reading} {file='/home/pi/storage/box_0.dat'} {message='No such file or directory'} { errno=2}
2019-02-22 22:17:45,174 INFO [default] [BoxServer::getBoxById] {createBox} {new_box}
2019-02-22 22:17:45,175 ERROR [default] [M2MController] {exception} {std::length_error} {M2M message out of bounds}
2019-02-22 22:21:20,220 VERBOSE-1 [default] [M2MController] {m2m_controller_handle_get}
2019-02-22 22:21:20,220 VERBOSE-2 [default] [M2MController] {m2m_controller_handle_get} {m2mString=000000001000000D9000000009707DD8C565774E872CBB09C}
2019-02-22 22:21:20,220 ERROR [default] [M2MController] {exception} {std::length_error} {M2M message out of bounds}

2019-02-22 22:43:41,072 INFO [default] [ClericApp] {controllers_destroyed}
2019-02-22 22:43:41,073 INFO [default] [BoxServer::delayedWriteThread] {thread_stopped}
2019-02-22 22:43:41,074 INFO [default] [BoxServer::BoxServer] {desytoying}

0cccccccccccccccc00112233445566778899aabbccddeeff
00000000100000000000000003F361CCB02C928BB461DD4EF
*/

TEST_CASE("M2MMessage from box can be decoded", "[M2MMessage]") {
  const auto MESSAGE = "00000000100000000000000003F361CCB02C928BB461DD4EF";
  const auto BOX_ID = 1;
  const auto READING = 0x3ff;
  const auto TYPE = 0x1ff;
  const uint64_t SECRET = 0x474996006560796aUL;
  const auto VCC = 0x3ff;

  auto decodedBoxId = M2MMessage::getBoxId(MESSAGE);
  CHECK(decodedBoxId == BOX_ID);

  M2MMessage m2m = M2MMessage::decode(MESSAGE, SECRET);
  CHECK(m2m.getBoxId() == BOX_ID);
  CHECK(m2m.getMeasurement() == READING);
  CHECK(m2m.getSensorPowerSupplyVoltage() == VCC);
  CHECK(m2m.getSensorType() == TYPE);
}
