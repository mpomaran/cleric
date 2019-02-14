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

#include "box_server.hpp"
#include "box.hpp"
#include "hdd_box_serialization_strategy.hpp"
#include "mem_box_serialization_strategy.hpp"
#include <easylogging++.h>
#include <granada/util/application.h>
#include <memory>
#include <props.hpp>
#include <sstream>

using namespace std;
using namespace cleric::data;

cleric::data::BoxServer::BoxServer() : shouldStopDelayedWrite(false) {
  delayedWriteThread = make_unique<thread>([&]() {
    LOG(INFO) << "[BoxServer::delayedWriteThread] {thread_started}";

    const auto MIN_WRITE_DELAY_MS = 100;

    while (true) {

      for (auto i = 0; i < MIN_WRITE_DELAY_MS; i++) {
        if (shouldStopDelayedWrite) {
          LOG(INFO) << "[BoxServer::delayedWriteThread] {thread_stopped}";
          return;
        }
        this_thread::sleep_for(chrono::milliseconds(1));
      }

      delayedWriteOnce();
    }
  });
}

cleric::data::BoxServer::~BoxServer() {
  try {
    shouldStopDelayedWrite = true;
    if (delayedWriteThread->joinable()) {
      delayedWriteThread->join();
    }
  } catch (...) {
    // catch all
  }
  LOG(INFO) << "[BoxServer::BoxServer] {desytoying}";
}

shared_ptr<Box>
cleric::data::BoxServer::getBoxById(const cleric::BoxId &boxId) {
  lock_guard<mutex> lock(boxCacheMtx);
  auto cached = boxCache.find(boxId);
  if (cached == boxCache.end()) {
    // no box found, create one
    auto storageStrategy = granada::util::application::GetProperty(
        cleric::props::STORAGE_STRATEGY_PROP_NAME);
    auto retentionPeriod =
        std::chrono::hours(24 * 365); // TODO expose in configurtion
    auto maxEntries = 1000;           // TODO expose in configurtion

    LOG(INFO) << "[BoxServer@" << (void *)this << "@" << this_thread::get_id() << "::getBoxById] {createBox} {boxId='" << boxId << "'"
              << " storageStrategy='" << storageStrategy << "'"
              << " retentionPeriod='" << retentionPeriod.count() << "'"
              << " maxEntries='" << maxEntries << "'}";

    if (storageStrategy == cleric::props::STORAGE_STRATEGY_PROP_MEMORY) {
      std::unique_ptr<cleric::data::IBoxSerializationStrategy> serializer{
          new MemBoxSerializationStrategy()};
      auto box = make_shared<Box>(boxId, retentionPeriod, maxEntries,
                                  std::move(serializer));
      boxCache[boxId] = box;
      LOG(INFO) << "[BoxServer::getBoxById] {createBox} {new_box}";
      return box;
    } else if (storageStrategy == cleric::props::STORAGE_STRATEGY_PROP_HDD) {
      auto storagePath = granada::util::application::GetProperty(
          cleric::props::STORAGE_ROOT_PROP_NAME);

	  std::unique_ptr<cleric::data::IBoxSerializationStrategy> serializer{
          new HddBoxSerializationStrategy(boxId, storagePath)};

      if (!serializer->getIStream().fail()) {
        try {
          auto box = make_shared<Box>(std::move(serializer));
          boxCache[boxId] = box;
          LOG(INFO) << "[BoxServer::getBoxById] {createBox} {deserialized}";
          return box;
        } catch (const std::exception &e) {
          LOG(ERROR) << "[BoxServer::getBoxById] {createBox} "
                        "{unexpected_exception} {what='"
                     << e.what() << "'}";
        }
      }

	  if (!serializer) {
		  // we're here because of the exception below, recreate serializer
		  std::unique_ptr<cleric::data::IBoxSerializationStrategy> newSerializer{
		  new HddBoxSerializationStrategy(boxId, storagePath) };
		  serializer = move(newSerializer);
	  }

      LOG(INFO) << "[BoxServer::getBoxById] {createBox} {new_box}";
      auto box = make_shared<Box>(boxId, retentionPeriod, maxEntries,
                                  std::move(serializer));
      boxCache[boxId] = box;
      return box;
    } else {
      throw illegal_state_exception("State strategy not known");
    }
  } else {
    return cached->second;
  }
}

// delayed write of the box tada
void cleric::data::BoxServer::delayedWriteOnce() {
  try {
    vector<shared_ptr<cleric::data::Box>>
        toPersist; // TODO - benchmark when list is faster than vector; fix
                   // before the stable release

    {
      unique_lock<mutex>(boxCacheMtx);
      for (auto &b : boxCache) {
        if (b.second->isDirty()) {
          toPersist.push_back(b.second);
        }
      }
    }

    for (auto &b : toPersist) {
      b->persist();
      b->setDirty(false);
    }
    if (toPersist.size() > 0) {
      VLOG(1) << "[BoxServer::delayedWriteOnce] {persistBoxes} {toPersistSize='"
              << toPersist.size() << "'}";
    }
  } catch (const std::exception &e) {
    LOG(ERROR) << "[BoxServer::delayedWriteOnce] {unexpected_exception} {what='"
               << e.what() << "'}";
  }
}
