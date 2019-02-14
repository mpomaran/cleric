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

#include "../business/m2m_message.hpp"
#include "box_server.hpp"
#include <catch/catch.hpp>

using namespace cleric::data;

TEST_CASE(
    "Given a m2m string, box server can provide correct box, [box server]") {
  const auto M2M_STRING = "0000000ABE587DE600000000AE30AFFCD31EA91087E215F4";

  auto boxId = cleric::M2MMessage::getBoxId(M2M_STRING);

  try {
	BoxServer boxServer;
	auto box = boxServer.getBoxById(boxId);
	CHECK(box != nullptr);
  }
  catch (...) {
	  FAIL("Exception thrown");
  }
}
