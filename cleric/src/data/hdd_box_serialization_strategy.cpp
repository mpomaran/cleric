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
#include <cstdio>
#include <cstring>
#include <easylogging++.h>
#include <exception>

using namespace std;

cleric::data::HddBoxSerializationStrategy::HddBoxSerializationStrategy(
    const ::cleric::BoxId &boxId_, const ::std::string &storagePath_)
    : boxId(boxId_), storagePath(storagePath_) {
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
}

::std::vector<uint8_t> cleric::data::HddBoxSerializationStrategy::get() {
  const uint32_t MAX_FILE_SIZE = 10 * 1024 * 1024;
  vector<uint8_t> result;
  FILE *in = fopen(fn.c_str(), "rb");
  if (!in) {
    LOG(WARNING) << "[HddBoxSerializationStrategy::get] "
                    "{cannot open file for reading} "
                 << "{file='" << fn << "'} "
                 << "{message='" << strerror(errno) << "'} { errno=" << errno
                 << "}";

    return result;
  }

  auto size = boost::filesystem::file_size(fn);
  if (size >= MAX_FILE_SIZE) {
    LOG(ERROR) << "[HddBoxSerializationStrategy::get] "
                  "{file too big} {size='"
               << size << "}";
    fclose(in);
    throw out_of_range("File " + fn + " too big to read");
  }

  // TODO benchmark and modify the API before the first release if needed
  auto raw = make_unique<uint8_t[]>(size);
  if (size != fread(raw.get(), 1, size, in)) {
    LOG(ERROR) << "[HddBoxSerializationStrategy::get] "
                  "{error during read} "
               << "{file = '" << fn << "'} "
               << "{message='" << strerror(errno) << "'} { errno=" << errno
               << "}";
    fclose(in);
    return result;
  }
  fclose(in);
  result.assign(raw.get(), raw.get() + size);

  return result;
}

void cleric::data::HddBoxSerializationStrategy::put(
    const ::std::vector<uint8_t> &data) {
  try {
    FILE *out = fopen(tempFn.c_str(), "wb");
    if (!out) {
      LOG(ERROR) << "[HddBoxSerializationStrategy::put] "
                    "{cannot open file for writing} "
                 << "{file='" << tempFn << "'} "
                 << "{message='" << strerror(errno) << "'} { errno=" << errno
                 << "}";
      return;
    }
    if (data.size() != fwrite(data.data(), 1, data.size(), out)) {
      fclose(out);
      LOG(ERROR) << "[HddBoxSerializationStrategy::put] "
                    "{cannot write data} "
                 << "{file='" << tempFn << "'} "
                 << "{message='" << strerror(errno) << "'} { errno=" << errno
                 << "}";
	  return;
    }
	fclose(out);

    if (boost::filesystem::exists(fn)) {
      if (boost::filesystem::exists(bckFn)) {
        boost::filesystem::remove_all(bckFn);
      }
      boost::filesystem::rename(fn, bckFn);
    }
    boost::filesystem::rename(tempFn, fn);
    boost::filesystem::remove_all(bckFn);
  } catch (const exception &e) {
    LOG(WARNING) << "[HddBoxSerializationStrategy::syncOStream] "
                    "{unexpected_exception} {what='"
                 << e.what() << "'}";
  }
}
