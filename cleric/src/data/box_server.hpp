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

#ifndef BOX_SERVER_HPP_HAS_BEEN_INCLUDED
#define BOX_SERVER_HPP_HAS_BEEN_INCLUDED

#include "../business/m2m_message.hpp"
#include "box.hpp"
#include <memory>
#include <thread>
#include <unordered_map>

namespace cleric {
namespace data {

class BoxServer {
public:
  BoxServer();
  ~BoxServer();

  ::std::shared_ptr<cleric::data::Box> getBoxById(const ::cleric::BoxId &boxId);
private:
  ::std::unordered_map<::cleric::BoxId, ::std::shared_ptr<cleric::data::Box>>
      boxCache;
  ::std::mutex boxCacheMtx;

  ::std::unique_ptr<::std::thread> delayedWriteThread;
  ::std::atomic_bool shouldStopDelayedWrite;	// TODO verify if atomic is needed; before the stable release
  void delayedWriteOnce(); // goes over the map and persists all dirty boxes, then exits
};
} // namespace data
} // namespace cleric

#endif
