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
#include <algorithm>
#include <cassert>
#include <iostream>
#include <msgpack.hpp>
#include <mutex>

using namespace ::std;
using namespace ::std::chrono;

cleric::data::Box::Box(
    const cleric::BoxId &boxId_, const hours &retention_, int maxEntries_,
    ::std::unique_ptr<IBoxSerializationStrategy> serializationStrategy_)
    : id(boxId_), retention(retention_), maxEntries(maxEntries_), dirty(false),
      initialized(true),
      serializationStrategy(::std::move(serializationStrategy_)) {}

cleric::data::Box::Box(
    ::std::unique_ptr<IBoxSerializationStrategy> serializationStrategy_)
    : serializationStrategy(::std::move(serializationStrategy_)) {
  serializationStrategy->getIStream() >> *this;
}

cleric::data::Box::Box(Box &&other)
    : serializationStrategy(::std::move(other.serializationStrategy)) {
  if (other.initialized) {
    lock_guard<mutex> lck{other.mtx};
    id = other.id;
    dirty = other.dirty;
    retention = other.retention;
    maxEntries = other.maxEntries;
    initialized = other.initialized;
    dataPoints = ::std::move(other.dataPoints);
    lastWrite = other.lastWrite;
    other.dirty = false;
    other.id = 0;
    other.initialized = false;
  } else {
    initialized = false;
  }
}

cleric::data::Box::~Box() {}

std::string cleric::data::Box::process(const ::std::string &message) {
  assertIsInitialized();
  {
    const uint64_t SECRET = 0x474996006560796aUL;
    const ::std::string SECRET_STR = "0x474996006560796a";

    auto msg = M2MMessage::decode(message, SECRET);
    if (msg.getBoxId() != id) {
      throw invalid_argument("Wrong box selected, my friend");
    }

    auto now = std::chrono::system_clock::now();
    long long rcvTimeMsSinceEpoch =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch())
            .count();

    DataPoint data{rcvTimeMsSinceEpoch,
                   msg.getSensorType(),
                   msg.getMeasurement(),
                   msg.getSensorPowerSupplyVoltage(),
                   message,
                   SECRET_STR};

    lock_guard<mutex> lck{mtx};
    setDirty(true);
    dataPoints.push_back(data);

    purgeOldData();
  }
  return ::std::string();
}

milliseconds cleric::data::Box::getHowLongIsDirty() {
  if (isDirty() == false) {
    return milliseconds(0);
  }

  const auto result =
      time_point_cast<milliseconds>(system_clock::now()).time_since_epoch() -
      time_point_cast<milliseconds>(lastWrite).time_since_epoch();
  return milliseconds(result);
}

void cleric::data::Box::persist() {
  if (initialized && isDirty()) {
    serializationStrategy->getOStream() << *this;
    serializationStrategy->syncOStream();
    dirty = false;
  }
}

void cleric::data::Box::purgeOldData() {
  while (dataPoints.size() > maxEntries) {
    dataPoints.pop_front();
  }

  auto now = duration_cast<milliseconds>(system_clock::now().time_since_epoch())
                 .count();
  auto oldestAllowedTimestampMsSinceEpoch =
      now + duration_cast<milliseconds>(retention).count();

  remove_if(dataPoints.begin(), dataPoints.end(), [&](const DataPoint &d) {
    return d.rcvTimeInMsSinceEpoch < oldestAllowedTimestampMsSinceEpoch;
  });
}

::std::ostream &cleric::data::operator<<(::std::ostream &os,
                                         const cleric::data::Box &b) {
  b.assertIsInitialized();

  // TODO MPP review format not to leak passwords, before beta release

  lock_guard<mutex> lck{b.mtx};

  int version = 1;
  uint64_t retention = b.retention.count();
  msgpack::pack(os, version);
  msgpack::pack(os, b.id);
  msgpack::pack(os, retention); // chrono not supported by msgpack-c 3.11
  msgpack::pack(os, b.maxEntries);
  msgpack::pack(os, b.dataPoints);

  return os;
}

::std::istream &cleric::data::operator>>(::std::istream &is,
                                         cleric::data::Box &b) {

  is.seekg(0, std::ios::end);
  auto size = is.tellg();
  is.seekg(0, std::ios::beg);

  std::vector<char> buff((const unsigned int)size);
  if (is.read(buff.data(), size)) {
    size_t offset = 0;

    lock_guard<mutex> lck{b.mtx};
    auto unpacked = msgpack::unpack(buff.data(), buff.size(), offset);
    auto decoded = unpacked.get();

    int version;
    decoded.convert(version);
    if (version != 1) {
      throw invalid_argument("Version different than 1 not supported");
    }

    b.dirty = false;

    unpacked = msgpack::unpack(buff.data(), buff.size(), offset);
    decoded = unpacked.get();
    decoded.convert(b.id);

    unpacked = msgpack::unpack(buff.data(), buff.size(), offset);
    decoded = unpacked.get();
    uint64_t retentionLong;
    decoded.convert(retentionLong);
    b.retention = milliseconds(retentionLong);

    unpacked = msgpack::unpack(buff.data(), buff.size(), offset);
    decoded = unpacked.get();
    decoded.convert(b.maxEntries);

    unpacked = msgpack::unpack(buff.data(), buff.size(), offset);
    decoded = unpacked.get();
    decoded.convert(b.dataPoints);

    b.initialized = true;
  }
  else {
    throw illegal_state_exception(
        "Cannot deserialize, broken data or read error");
  }

  return is;
}

bool cleric::data::operator==(const cleric::data::Box &b1,
                              const cleric::data::Box &b2) {
  // Superior solution available in C++17
  //        std::scoped_lock lk(e1.m, e2.m);
  unique_lock<mutex> lck1(b1.mtx, defer_lock);
  unique_lock<mutex> lck2{b2.mtx, defer_lock};
  lock(lck1, lck2); // acuire both at the same time

  return b1.id == b2.id && b1.dirty == b2.dirty &&
         b1.retention == b2.retention && b1.maxEntries == b2.maxEntries &&
         b1.initialized == b2.initialized && b1.dataPoints == b2.dataPoints;
}

bool cleric::data::Box::DataPoint::
operator==(const cleric::data::Box::DataPoint &o) const {
  return rcvTimeInMsSinceEpoch == o.rcvTimeInMsSinceEpoch &&
         sensorType == o.sensorType && value == o.value && vcc == o.vcc &&
         originalMessage == o.originalMessage && secret == o.secret;
}
