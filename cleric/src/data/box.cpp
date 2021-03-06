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
#include <easylogging++.h>
#include <iostream>
#include <msgpack.hpp>
#include <mutex>
#include <sstream>
#include <map>
#include <sstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace ::std;
using namespace ::std::chrono;

namespace cleric {
namespace data {

Box::Box(const cleric::BoxId &boxId_, const ::std::string &name_,
         const ::std::chrono::hours &retention_, int maxEntries_,
         ::std::unique_ptr<IBoxSerializationStrategy> serializationStrategy_)
    : id(boxId_), name(name_), retention(retention_), maxEntries(maxEntries_),
      dirty(false), initialized(true),
      serializationStrategy(::std::move(serializationStrategy_)) {}

Box::Box(::std::unique_ptr<IBoxSerializationStrategy> serializationStrategy_)
    : serializationStrategy(::std::move(serializationStrategy_)) {
  auto buff = serializationStrategy->get();
  fromByteArray(buff);
}

Box::Box(Box &&other)
    : serializationStrategy(::std::move(other.serializationStrategy)) {
  if (other.initialized) {
    lock_guard<mutex> lck{other.mtx};
    id = other.id;
    name = other.name;
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

Box::~Box() {}

std::string Box::process(const ::std::string &message) {
  assertIsInitialized();
  {
	  // TODO fix secrets
    const uint64_t SECRET = 0x474996006560796aUL;
    const ::std::string SECRET_STR = "0x474996006560796a";

    auto msg = M2MMessage::decode(message, SECRET);
    if (msg.getBoxId() != id) {
      throw invalid_argument("Wrong box selected, my friend");
    }

    auto now = std::chrono::system_clock::now();
    uint64_t rcvTimeMsSinceEpoch =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch())
            .count();

    auto convertedVCC =
        DataPoint::fromSensorFormatToVolts(msg.getSensorPowerSupplyVoltage());
    auto convertedValue = DataPoint::fromSensorFormatToValue(
        msg.getMeasurement(), msg.getSensorType(), convertedVCC);

	VLOG(1) << "[Box::process] {message='" << message 
		<< "', rcvTimeMsSinceEpoch='" << rcvTimeMsSinceEpoch
		<< "', rawVcc='" << msg.getSensorPowerSupplyVoltage()
		<< "', rawValue='" << convertedValue 
		<< "', rawType='" << msg.getSensorType() 
		<< "'}";

	// we store unmodified events, since it's easier to fix the server processing then
	// you might thing about it as a some kind of the of event sourcing
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

milliseconds Box::getHowLongIsDirty() {
  if (isDirty() == false) {
    return milliseconds(0);
  }

  const auto result =
      time_point_cast<milliseconds>(system_clock::now()).time_since_epoch() -
      time_point_cast<milliseconds>(lastWrite).time_since_epoch();
  return milliseconds(result);
}

std::string escapeJsonString(const std::string &input) {
  std::ostringstream ss;
  for (auto iter = input.cbegin(); iter != input.cend(); iter++) {
    switch (*iter) {
    case '\\':
      ss << "\\\\";
      break;
    case '"':
      ss << "\\\"";
      break;
    case '/':
      ss << "\\/";
      break;
    case '\b':
      ss << "\\b";
      break;
    case '\f':
      ss << "\\f";
      break;
    case '\n':
      ss << "\\n";
      break;
    case '\r':
      ss << "\\r";
      break;
    case '\t':
      ss << "\\t";
      break;
    default:
      ss << *iter;
      break;
    }
  }
  return ss.str();
}

template <typename T>
std::string jsonPair(const std::string &key, const T &value) {
  stringstream ss, ssVal;
  ssVal << value;
  ss << '"' << escapeJsonString(key) << "\" : \""
     << escapeJsonString(ssVal.str()) << "\"";
  return ss.str();
}

Box::Reading::ReadableType Box::getSensorReadingType() const
{
	if (size() == 0) {
		return Box::Reading::ReadableType::UNKNOWN;
	}
	else {
		auto reading = (*this)[0];
		return DataPoint::fromSensorTypeToReadableType(reading.sensorType);
	}
}

std::string Box::toJson() const {
  stringstream ss;

  lock_guard<mutex> lck{mtx};

  auto measurements = dataPointsToJSon();

  ss << "{" << jsonPair<std::uint64_t>("id", id) << ","
     << jsonPair<std::string>("name", getName()) << ","
     << jsonPair<std::uint64_t>("retentionPeriod", retention.count()) << ","
     << jsonPair<std::uint64_t>("maxDataPointEntries", maxEntries) << ","
     << "\"measurements\" : [" << measurements << " ] "
     << "}";

  return ss.str();
}

std::string Box::dataPointsToJSon() const {
  stringstream ss;
  bool first = true;
  for (const auto &dp : dataPoints) {
    if (!first) {
      ss << ",";
    } else {
      first = false;
    }
    ss << " { " << jsonPair<double>("value", dp.value) << ","
       << jsonPair<uint64_t>("sensorType", dp.sensorType) << ","
       << jsonPair<double>("vcc", dp.vcc) << ","
       << jsonPair<uint64_t>("dt", dp.rcvTimeInMsSinceEpoch) << "}";
  }
  return ss.str();
}

void Box::persist() {
  if (initialized && isDirty()) {
    serializationStrategy->put(toByteArray());
    dirty = false;
  }
}

size_t Box::size() const 
{
	return dataPoints.size();
}

Box::Reading Box::operator[](int i) const
{
	if (i < 0 || i >= dataPoints.size()) {
		throw out_of_range("Reading from illegal index");
	}

	Reading result;
	auto d = dataPoints[i];

	result.rcvTime = DataPoint::fromMsSinceEpochToDateTime(d.rcvTimeInMsSinceEpoch);
	result.sensorType = d.sensorType;
	result.vcc = DataPoint::fromSensorFormatToVolts(d.vcc);
	result.value = DataPoint::fromSensorFormatToValue (d.value, d.sensorType, result.vcc);
	result.readableType = DataPoint::fromSensorTypeToReadableType(d.sensorType);

	return result;
}

void Box::purgeOldData() {
  while (dataPoints.size() > maxEntries) {
    dataPoints.pop_front();
  }

  auto now = duration_cast<milliseconds>(system_clock::now().time_since_epoch())
                 .count();
  auto oldestAllowedTimestampMsSinceEpoch =
      now + duration_cast<milliseconds>(retention).count();

  remove_if(dataPoints.begin(), dataPoints.end(), [&](const DataPoint &d) {
    return d.rcvTimeInMsSinceEpoch <
           (uint64_t)oldestAllowedTimestampMsSinceEpoch;
  });
}

std::vector<uint8_t> Box::toByteArray() const {
  assertIsInitialized();

  // TODO MPP review format not to leak passwords, before beta release
  lock_guard<mutex> lck{mtx};

  int version = 1;
  uint64_t retentionPeriod = retention.count();
  ::std::stringstream os;
  msgpack::pack(os, version);
  msgpack::pack(os, id);
  msgpack::pack(os, name);
  msgpack::pack(os, retentionPeriod); // chrono not supported by msgpack-c 3.11
  msgpack::pack(os, maxEntries);
  msgpack::pack(os, dataPoints);

  return ISerializable::toBuffer(os);
}

void Box::fromByteArray(const std::vector<uint8_t> &buff) {
  size_t offset = 0;

  lock_guard<mutex> lck{mtx};
  auto unpacked = msgpack::unpack((char *)(buff.data()), buff.size(), offset);
  auto decoded = unpacked.get();

  int version;
  decoded.convert(version);
  if (version != 1) {
    throw invalid_argument("Version different than 1 not supported");
  }

  dirty = false;

  // TODO MPP convert to template function (before beta)
  unpacked = msgpack::unpack((char *)buff.data(), buff.size(), offset);
  decoded = unpacked.get();
  decoded.convert(id);

  unpacked = msgpack::unpack((char *)buff.data(), buff.size(), offset);
  decoded = unpacked.get();
  decoded.convert(name);

  unpacked = msgpack::unpack((char *)buff.data(), buff.size(), offset);
  decoded = unpacked.get();
  uint64_t retentionLong;
  decoded.convert(retentionLong);
  retention = milliseconds(retentionLong);

  unpacked = msgpack::unpack((char *)buff.data(), buff.size(), offset);
  decoded = unpacked.get();
  decoded.convert(maxEntries);

  unpacked = msgpack::unpack((char *)buff.data(), buff.size(), offset);
  decoded = unpacked.get();
  decoded.convert(dataPoints);

  initialized = true;
}

bool operator==(const Box &b1, const Box &b2) {
  // Superior solution available in C++17
  //        std::scoped_lock lk(e1.m, e2.m);
  unique_lock<mutex> lck1(b1.mtx, defer_lock);
  unique_lock<mutex> lck2{b2.mtx, defer_lock};
  lock(lck1, lck2); // acuire both at the same time

  return b1.id == b2.id && b1.dirty == b2.dirty && b1.name == b2.name &&
         b1.retention == b2.retention && b1.maxEntries == b2.maxEntries &&
         b1.initialized == b2.initialized && b1.dataPoints == b2.dataPoints;
}

bool Box::DataPoint::operator==(const Box::DataPoint &o) const {
  return rcvTimeInMsSinceEpoch == o.rcvTimeInMsSinceEpoch &&
         sensorType == o.sensorType && value == o.value && vcc == o.vcc &&
         originalMessage == o.originalMessage && secret == o.secret;
}

double Box::DataPoint::fromSensorFormatToVolts(uint64_t sensorVCC) {
  return (sensorVCC * 2 + 4000) / 1000.0;
}

string Box::DataPoint::fromMsSinceEpochToDateTime(uint64_t msSinceEpoch) {
	std::time_t t = msSinceEpoch / 1000;

	//std::tm tm = *std::gmtime(&t); //GMT (UTC)
	std::tm tm = *std::localtime(&t); //Locale time-zone, usually UTC by default.
	std::stringstream ss;
	ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
	return ss.str();
}

Box::Reading::ReadableType Box::DataPoint::fromSensorTypeToReadableType(uint64_t sensorType)
{
	if (sensorType > 150 && sensorType < 250) {
		return Box::Reading::ReadableType::TEMPERATURE;
	}

	return Box::Reading::ReadableType::UNKNOWN;
}

// vcc is vcc in volts
static double fromSensorFormatToTemperature(uint64_t sensorValue, double vcc) {
	// temperature sensor

	// convert reading to millivolts
	double t = (double)sensorValue / 1024.0 * vcc * 1000.0;

	// measure delta between reading and 750 mv
	// using 10mV/C formula caltulate delta in C between this value and 25C
	double deltaC = (750.0 - t) / 10.0;
	t = 25.0 - deltaC;

	//VLOG(1) << "[Box::DataPoint::fromSensorFormatToValue] {sensorType='Temp sensor' value='"
//			<< t << "'}";
	return t;
}

static double fromSensorFormatToDefault(uint64_t sensorValue, double vcc) {
	VLOG(1) << "[Box::DataPoint::fromSensorFormatToValue] {sensorValue='"
		<< sensorValue << "', sensorType='default'"
		<<  ", vcc = '" << vcc << "'}";

	return (double)sensorValue;
}

typedef double(*ConverterFunction)(uint64_t, double);

const static ::std::map<int, ConverterFunction> converters = {
	{Box::Reading::TEMPERATURE, fromSensorFormatToTemperature},
	{Box::Reading::UNKNOWN, fromSensorFormatToDefault}
	};

// vcc is vcc in volts
double Box::DataPoint::fromSensorFormatToValue(uint64_t sensorValue,
                                               uint64_t sensorType,
                                               double vcc) {
	auto type = fromSensorTypeToReadableType(sensorType);
	auto it = converters.find(type);
	if (it == converters.end()) {
		VLOG(1) << "[Box::DataPoint::fromSensorFormatToValue] {sensorValue='"
			<< sensorValue << "', sensorType='"
			<< sensorType << "', vcc = '" << vcc << "'}";
		LOG(WARNING) << "[Box::DataPoint::fromSensorFormatToValue] {Unknown sensor "
			"type} {sensorType='"
			<< sensorType << "'}";
		return (double)sensorValue;
	}
	else {
		auto converter = it->second;
		return converter(sensorValue, vcc);
	}
}

} // namespace data
} // namespace cleric
