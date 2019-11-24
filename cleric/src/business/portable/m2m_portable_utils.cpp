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

#include "m2m_portable_utils.hpp"

static int64_t seed = 0;

uint64_t yrand_rand(yrand_generator *generator) {
  generator->seed = generator->seed * 1664525L + 1013904223L;

  return generator->seed & 0xFFFFFFFFL;
}

yrand_generator yrand_seed(uint64_t a, uint64_t b) {
  yrand_generator result;
  result.seed = (int64_t)(a + b);

  return result;
}

uint8_t nibble_to_char(uint8_t nibble)
{
	if (nibble < 10)
		return '0' + nibble;
	else
		return 'A' + nibble - 10;
}

/*
output always 17 = 16 bytes + 1 (zero) byte
*/
uint8_t uint64_to_bytes(char *buff, uint64_t number)
{
	uint8_t i;
	for (i = 0; i < sizeof(uint64_t); i++)
	{
		uint8_t b = 0xff & (number >> ((sizeof(uint64_t) - i - 1) * 8));

		buff[i * 2 + 1] = nibble_to_char(b & 0xf);
		buff[i * 2] = nibble_to_char((b >> 4) & 0xf);
	}
	buff[sizeof(uint64_t) * 2] = 0;

	return sizeof(uint64_t) * 2 + 1;
}

