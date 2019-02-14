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

#include "box.hpp"
#include "box_serialization_strategy.hpp"
#include <catch/catch.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

class TestSerializationStrategy
    : public cleric::data::IBoxSerializationStrategy {
public:
  TestSerializationStrategy(const std::string &message) { is << message; }

  TestSerializationStrategy() {}

  // Inherited via IBoxSerializationStrategy
  virtual ::std::istream &getIStream() override { return is; }

  virtual ::std::ostream &getOStream() override { return os; }

  virtual void syncOStream() override { is << os.rdbuf(); }

private:
  std::stringstream os, is;
};

static std::string createM2MMessage() {
  return "0000000ABE587DE600000000AE30AFFCD31EA91087E215F4";
}

static cleric::data::Box createBox() {

  auto retention = std::chrono::hours(24);
  std::unique_ptr<cleric::data::IBoxSerializationStrategy> serializer{
      new TestSerializationStrategy()};
  cleric::data::Box result{10, retention, 3, std::move(serializer)};

  return result;
}

static cleric::data::Box createBox2() {

	auto retention = std::chrono::hours(1);
	std::unique_ptr<cleric::data::IBoxSerializationStrategy> serializer{
		new TestSerializationStrategy() };
	cleric::data::Box result{ 0, retention, 0, std::move(serializer) };

	return result;
}


TEST_CASE(
    "Box can be serialized and deserialized, giving back the same box, [box]") {
  auto b1 = createBox();
  auto m2m = createM2MMessage();
  b1.process(m2m);

  std::stringstream os, is;
  os << b1;
  is << os.rdbuf();
  b1.setDirty(false);

  auto b2 = createBox2();
  is >> b2;

  bool equals = b1 == b2;
  CHECK(equals); // TBD
}

TEST_CASE("Box is serialized/deserialized according to serialization strategy",
          "[box]") {
  auto b1 = createBox();
  auto m2m = createM2MMessage();

  b1.process(m2m);
  ::std::this_thread::sleep_for(std::chrono::milliseconds(200));

  CHECK(b1.isDirty() == true);
  CHECK(b1.getHowLongIsDirty() >= std::chrono::milliseconds(200));
  b1.persist();
  CHECK(b1.isDirty() == false);
}

TEST_CASE("Retention period works, box[]") {}
