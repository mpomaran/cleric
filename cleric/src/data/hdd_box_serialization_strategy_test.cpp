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
#include "hdd_box_serialization_strategy.hpp"
#include <catch/catch.hpp>
#include <thread>

static std::string createM2MMessage() {
  return "00000000100000000000000003F361CCB02C928BB461DD4EF";
}

static cleric::data::Box createBoxFromScratch() {

  auto retention = std::chrono::hours(24);
  std::unique_ptr<cleric::data::IBoxSerializationStrategy> serializer{
      new cleric::data::HddBoxSerializationStrategy(1, ".")};
  cleric::data::Box result{1, "dummy", retention, 1000, std::move(serializer)};

  return result;
}

TEST_CASE("Box can be serialized and deserialized",
          "[hdd box serialization strategy]") {
  auto m2m = createM2MMessage();
  auto b1 = createBoxFromScratch();

  auto t = std::thread([&]() {
    for (int i = 0; i < 500; i++) {
      b1.process(m2m);
    }
  });
  t.join();
  b1.persist();

  std::unique_ptr<cleric::data::IBoxSerializationStrategy> serializer{
      new cleric::data::HddBoxSerializationStrategy(1, ".")};
  cleric::data::Box b2{std::move(serializer)};

  CHECK(b1 == b2);

  auto t2 = std::thread([&]() {
    for (int i = 0; i < 500; i++) {
      b1.process(m2m);
    }
  });
  t2.join();
  b1.persist();

  std::unique_ptr<cleric::data::IBoxSerializationStrategy> serializer3{
      new cleric::data::HddBoxSerializationStrategy(1, ".")};
  cleric::data::Box b3{std::move(serializer3)};

  CHECK(b1 == b3);
  CHECK(!(b2 == b3));
}
