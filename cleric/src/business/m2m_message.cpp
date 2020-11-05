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
	const uint64_t M2M_SECRET = 0x474996006560796aUL;


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

	static uint64_t sensorDataToBuffer(uint32_t type, uint32_t value,
		uint32_t vcc) {

		uint64_t buff;

		buff = (uint64_t)(type & 511) << 22;
		buff |= (uint64_t)(vcc & 1023) << 11;
		buff |= (uint64_t)(value & 1023);
		buff |= buff << 32;

		return buff;
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
			// TODO: security by obscurity
			throw invalid_argument("Wrong format, CRC fail");
		}

		uint32_t type, reading, vcc;
		bufferToSensorData(decodedPayload, type, reading, vcc);

		M2MPayload payload = M2MPayload{ type, reading, vcc };

		M2MMessage p = { (BoxId)boxId, payload };

		return p;
	}

	BoxId M2MMessage::getBoxId() const { return boxId; }

	BoxId M2MMessage::getBoxId(const ::std::string &message) {
		if (message.size() < 8) {
			throw invalid_argument("Message too short");
		}

		return (BoxId)stringToInt(message.substr(0 + OFFSET, 8));
	}

	M2MMessage::M2MMessage(const M2MMessage &&other) {
		boxId = other.boxId;
		payload = std::move(other.payload);
	}

	std::string M2MMessage::encode(const BoxId &boxId, uint64_t vcc, uint64_t measurement, uint64_t sensorType, uint64_t secret)
	{
		char buffer[sizeof(uint64_t) * 3 * 2 + 1 + 1];
		auto buff = &buffer[0];
		uint64_t timestamp = (uint32_t)(rand()) & 0xffffffff;
		uint64_t random = rand();
		uint8_t i;
		uint64_t encoded_payload = 0;

		buff += uint64_to_bytes(buff, timestamp + ((uint64_t)boxId << 32)) - 1;
		buff += uint64_to_bytes(buff, random) - 1;

		auto generator = yrand_seed(timestamp ^ random, secret);

		auto payload = sensorDataToBuffer(sensorType, measurement, vcc);;

		for (i = 0; i < sizeof(payload); i++) {
			uint8_t b = payload >> (i * 8) & 0xff;
			uint8_t key = (uint8_t)(yrand_rand(&generator) & 0xff);
			encoded_payload += ((uint64_t)(b ^ key)) << (i * 8);
		}

		buff += uint64_to_bytes(buff, encoded_payload);
		buffer[sizeof(buffer) - 1] = 0;

		return string("0") + buffer;
}


} // namespace cleric
