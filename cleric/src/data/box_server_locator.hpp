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

#ifndef BOX_SERVER_LOCATOR_HPP_HAS_BEEN_INCLUDED
#define BOX_SERVER_LOCATOR_HPP_HAS_BEEN_INCLUDED

#include "../business/m2m_message.hpp" // for BoxId definition

#include <memory>
#include <vector>

namespace cleric {
namespace data {

class BoxServer;
using BoxServerPtr = cleric::data::BoxServer *;

class BoxServerLocator {
public:
  static cleric::data::BoxServer &
  getBoxServerByBoxId(const ::cleric::BoxId &boxId);

  static ::std::vector< BoxServerPtr > getAllServers();
  static void clearAllServers();
};
} // namespace data
} // namespace cleric

#endif
