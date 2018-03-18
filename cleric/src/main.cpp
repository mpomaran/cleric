/**

MIT License

Copyright (c) 2017 mpomaranski at gmail

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

#define CATCH_CONFIG_RUNNER
#include <catch/catch.hpp>
#include <iostream>

#include "cleric_app.hpp"

int runTests(int argc, char* argv[]) {
  return Catch::Session().run(argc, argv);
}

int main(int argc, char* argv[]) {
  int result = -1;

  ClericApp app;

  try {
    auto testResult = runTests(argc, argv);

    if (testResult != 0) {
      std::cout << testResult << " test(s) failed, bailing out" << std::endl;
      result = 1;
    } else {
      app.go();
      result = 0;
    }

  } catch (std::exception& e) {
    std::cerr << "Exception caught" << e.what() << std::endl;
    result = 2;
  }

  std::cout << "Press RETURN to exit" << std::endl;
  auto key = std::cin.get();

  return result;
}
