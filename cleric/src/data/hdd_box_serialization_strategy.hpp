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

#ifndef HDD_BOX_SERIALIZATION_STRATEGY_HPP_HAS_BEEN_INCLUDED
#define HDD_BOX_SERIALIZATION_STRATEGY_HPP_HAS_BEEN_INCLUDED

#include "../business/m2m_message.hpp"
#include "ibox_serialization_strategy.hpp"
#include <fstream>
#include <istream>
#include <ostream>
#include <string>

namespace cleric {
namespace data {
class HddBoxSerializationStrategy
    : public cleric::data::IBoxSerializationStrategy {
public:
  HddBoxSerializationStrategy(const ::cleric::BoxId &boxId,
                              const ::std::string &storagePath);

  // Inherited via IBoxSerializationStrategy
  virtual ::std::vector<uint8_t> get();
  virtual void put(const ::std::vector<uint8_t> &data);
private:
	::cleric::BoxId boxId;
	::std::string storagePath;

	::std::string fn, tempFn, bckFn;
};
} // namespace data
} // namespace cleric

#endif
