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
#include <picojson.h>
#include <sstream>
#include <string>
#include <thread>

class SharedMemBoxSerializationStrategy
    : public cleric::data::IBoxSerializationStrategy {
public:
  SharedMemBoxSerializationStrategy(const ::std::string &message,
                                    std::vector<uint8_t> &data_)
      : data(data_) {
    data.assign(message.c_str(), message.c_str() + message.length());
  }
  SharedMemBoxSerializationStrategy(std::vector<uint8_t> &data_)
      : data(data_) {}

  // Inherited via IBoxSerializationStrategy
  virtual ::std::vector<uint8_t> get() override { return data; }
  virtual void put(const ::std::vector<uint8_t> &data) override {
    this->data = data;
  }

private:
  std::vector<uint8_t> &data;
};

static std::string createM2MMessage() {
  return "0000000ABE587DE600000000AE30AFFCD31EA91087E215F4";
}

std::vector<uint8_t> sharedData;

static cleric::data::Box createBoxFromScratch() {

  auto retention = std::chrono::hours(24);
  std::unique_ptr<cleric::data::IBoxSerializationStrategy> serializer{
      new SharedMemBoxSerializationStrategy(sharedData)};
  cleric::data::Box result{10, "dummy", retention, 3, std::move(serializer)};

  return result;
}

static cleric::data::Box createBoxFromSharedData() {

  auto retention = std::chrono::hours(1);
  std::unique_ptr<cleric::data::IBoxSerializationStrategy> serializer{
      new SharedMemBoxSerializationStrategy(sharedData)};
  cleric::data::Box result{std::move(serializer)};

  return result;
}

TEST_CASE(
    "Box can be serialized and deserialized, giving back the same box, [box]") {
  auto b1 = createBoxFromScratch();
  auto m2m = createM2MMessage();
  b1.process(m2m);
  b1.persist();

  auto b2 = createBoxFromSharedData();

  CHECK(b1 == b2); // TBD
}

TEST_CASE("Box is serialized/deserialized according to serialization strategy",
          "[box]") {
  auto b1 = createBoxFromScratch();
  auto m2m = createM2MMessage();

  b1.process(m2m);
  ::std::this_thread::sleep_for(std::chrono::milliseconds(200));

  CHECK(b1.isDirty() == true);
  CHECK(b1.getHowLongIsDirty() >= std::chrono::milliseconds(200));
  b1.persist();
  CHECK(b1.isDirty() == false);
}

TEST_CASE("Retention period works") {
  // TODO write test
}

TEST_CASE("Conversion for temperature sensor") {
  // TODO write test
}

TEST_CASE("JSon Serialisation of the box without measurements") {
  auto box = createBoxFromScratch();
  auto json = box.toJson();
  std::cout << json << std::endl;

  ::std::stringstream ss;
  ss << json;

  picojson::value v;

  // parse the input
  ss >> v;
  std::string err = picojson::get_last_error();
  if (!err.empty()) {
    FAIL(err);
  }

  // check if the type of the value is "object"
  if (!v.is<picojson::object>()) {
    FAIL("JSON is not an object");
  }

  // obtain a const reference to the map, and print the contents
  const picojson::value::object &obj = v.get<picojson::object>();
  for (picojson::value::object::const_iterator i = obj.begin(); i != obj.end();
       ++i) {
    auto first = i->first;
    auto second = i->second;
    std::cout << first << ": " << second << std::endl;
  }
}

TEST_CASE("JSon Serialisation of the box with measurements") {
  auto box = createBoxFromScratch();
  auto m2m = createM2MMessage();
  box.process(m2m);
  box.process(m2m);

  auto json = box.toJson();
  std::cout << json << std::endl;

  ::std::stringstream ss;
  ss << json;

  picojson::value v;

  // parse the input
  ss >> v;
  std::string err = picojson::get_last_error();
  if (!err.empty()) {
    FAIL(err);
  }

  // check if the type of the value is "object"
  if (!v.is<picojson::object>()) {
    FAIL("JSON is not an object");
  }

  // obtain a const reference to the map, and print the contents
  const picojson::value::object &obj = v.get<picojson::object>();
  for (picojson::value::object::const_iterator i = obj.begin(); i != obj.end();
       ++i) {
    auto first = i->first;
    auto second = i->second;
    std::cout << first << ": " << second << std::endl;
  }
}
