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

#pragma once

#include "../business/m2m_message.hpp"
#include "ibox_serialization_strategy.hpp"
#include "iserializable.hpp"
#include "smart_enum.hpp"
#include <chrono>
#include <deque>
#include <exception>
#include <msgpack.hpp>
#include <mutex>
#include <ostream>
#include <string>
#include <boost/variant.hpp>

using string = ::std::string;

namespace cleric {

	class illegal_state_exception : public ::std::runtime_error {
	public:
		illegal_state_exception(const ::std::string &msg) : runtime_error(msg) {}
	};

	namespace data {

		class Box : public ISerializable {
		public:
			struct Reading {
				uint64_t rcvTimeInMsSinceEpoch; // chrono not supported during serialization
				uint64_t sensorType;
				double value;	// value in proper units
				double vcc;		// voltage in volts
			};


			Box(const cleric::BoxId &boxId, const ::std::string &name,
				const ::std::chrono::hours &retention, int maxEntries,
				::std::unique_ptr<IBoxSerializationStrategy> serializationStrategy);

			Box(::std::unique_ptr<IBoxSerializationStrategy> serializationStrategy);

			Box(const Box &box) = default;

			Box(Box &&other);

			~Box();

			// Inherited via ISerializable
			virtual std::vector<uint8_t> toByteArray() const override;
			virtual void fromByteArray(const std::vector<uint8_t> &data) override;

			::std::string process(const ::std::string &message);

			friend bool operator==(const cleric::data::Box &b1,
				const cleric::data::Box &b2);

			::std::chrono::milliseconds getRetention() const { return retention; }
			void setRetention(const ::std::chrono::hours &retention) {
				this->retention = retention;
			}
			int getMaxEntries() const { return (int)maxEntries; }
			void setMaxEntries(int maxEntries) { this->maxEntries = maxEntries; }

			::std::chrono::milliseconds getHowLongIsDirty();
			bool isDirty() { return dirty; }
			void setDirty(bool dirty) { this->dirty = dirty; }

			std::string getName() const { return name; }
			void setName(const std::string &name_) { name = name_; }

			std::string toJson() const;

			// use serialization strategy to store itself
			void persist();

			int size();
			Reading operator[] (int i) const;

		private:
			struct DataPoint {
				uint64_t rcvTimeInMsSinceEpoch; // chrono not supported during serialization
				uint64_t sensorType;
				double value;
				double vcc;
				::std::string originalMessage;
				::std::string secret;			// TODO secutiry before the final release

				bool operator==(const cleric::data::Box::DataPoint &other) const;
				static double fromSensorFormatToVolts(uint64_t sensorVCC);
				static double fromSensorFormatToValue(uint64_t sensorValue,
					uint64_t sensorType, double vcc);

				MSGPACK_DEFINE(rcvTimeInMsSinceEpoch, sensorType, value, vcc,
					originalMessage, secret);
			};


			volatile bool initialized;
			volatile bool dirty; // TODO MPP consider it should be atomic; remove before
								 // the stable release
			::std::chrono::system_clock::time_point lastWrite;
			cleric::BoxId id;
			::std::string name;
			::std::chrono::milliseconds retention;
			size_t maxEntries;
			::std::unique_ptr<cleric::data::IBoxSerializationStrategy>
				serializationStrategy;

			std::string dataPointsToJSon() const;

			mutable ::std::mutex mtx; // mutex for locking data point accesss
			::std::deque<DataPoint>
				dataPoints; // TODO [MPP 22-Jan-2019] benchmark against the vector

			void purgeOldData();

			void assertIsInitialized() const {
				if (!initialized)
					throw ::cleric::illegal_state_exception("instance not initialized");
			}
		};
	} // namespace data
} // namespace cleric
