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

#include "hdd_box_serialization_strategy.hpp"
#include <boost/filesystem.hpp>
#include <easylogging++.h>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

cleric::data::HddBoxSerializationStrategy::HddBoxSerializationStrategy(
    const ::cleric::BoxId &boxId, const ::std::string &storagePath) {
  stringstream ss;
  ss << storagePath << "/box_" << boxId << ".dat";
  fn = ss.str();
  tempFn = fn + ".tmp";
  bckFn = fn + ".bck";

  LOG(INFO) << "[HddBoxSerializationStrategy::HddBoxSerializationStrategy] "
               "{set_paths} {filename="
            << fn << "}";

  if (boost::filesystem::exists(tempFn)) {
    boost::filesystem::remove_all(tempFn);
  }

  try {
    if (!boost::filesystem::exists(fn) && boost::filesystem::exists(bckFn)) {
      boost::filesystem::rename(bckFn, fn);
    }
  } catch (const exception &e) {
    LOG(ERROR) << "[HddBoxSerializationStrategy::HddBoxSerializationStrategy] "
                  "{unexpected_exception} {what='"
               << e.what() << "'}";
  }
  is.setstate(ios::failbit);
  os.setstate(ios::failbit);
}

::std::istream &cleric::data::HddBoxSerializationStrategy::getIStream() {
  // TODO rework this wierd reading before beta - get streams is not good
  // mechanism
  if (is.is_open()) {
    is.close();
  }

  try {
    is.open(fn, ios::binary);
  } catch (const exception &e) {
    LOG(WARNING) << "[HddBoxSerializationStrategy::getIStream] "
                    "{unexpected_exception} {what='"
                 << e.what() << "'}";
    is.setstate(ios::failbit);
  }

  return is;
}

::std::ostream &cleric::data::HddBoxSerializationStrategy::getOStream() {
  if (os.is_open()) {
    return os;
  }

  try {
    os.open(tempFn, ios::binary);
  } catch (const exception &e) {
    LOG(WARNING) << "[HddBoxSerializationStrategy::getOStream] "
                    "{unexpected_exception} {what='"
                 << e.what() << "'}";
    os.setstate(ios::failbit);
  }
  return os;
}

void cleric::data::HddBoxSerializationStrategy::syncOStream() {
  if (is.is_open()) {
    is.close();
  }

  if (os.good()) {
    try {
      os.close();

      if (boost::filesystem::exists(fn)) {
        if (boost::filesystem::exists(bckFn)) {
          boost::filesystem::remove_all(bckFn);
        }
        boost::filesystem::rename(fn, bckFn);
      }
      boost::filesystem::rename(tempFn, fn);
      boost::filesystem::remove_all(bckFn);
      os.setstate(ios::failbit);
    } catch (const exception &e) {
      LOG(WARNING) << "[HddBoxSerializationStrategy::syncOStream] "
                      "{unexpected_exception} {what='"
                   << e.what() << "'}";
      os.setstate(ios::failbit);
    }
  }
}
