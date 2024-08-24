#include <iostream>
#include <exception>

#include "../src/StatArray.h"

namespace test {

StatArray<int> makeArray(const int size = 8) {
  std::vector<int> v(size);
  std::iota(v.begin(), v.end(), 0);

  return StatArray<int>(std::move(v));
}

static StatArray<int>& array() {
  static StatArray<int> theArray([]() { return makeArray(); }());
  return theArray;
}


bool startIsBegin() {
  std::cout << "(test) startIsBegin..." << std::endl;
  return array()[0] == *(array().begin());
}
bool endIsEnd() {
  std::cout << "(test) endIsEnd..." << std::endl;
  return array()[array().size()-1] == *(array().end() - 1);
}

bool grow() {
  std::cout << "(test) grow..." << std::endl;
  auto arr = makeArray(10);
  arr.grow();
  return arr.size() == 11;
}

bool shrink() {
  std::cout << "(test) shrink..." << std::endl;
  auto arr = makeArray(10);
  arr.shrink();
  return arr.size() == 9;
}

}  // namespace test


int main() {
  const auto testCases = {
    test::startIsBegin,
    test::endIsEnd,
    test::grow,
    test::shrink,
  };

  for (auto& testFunc : testCases) {
    if (! testFunc()) return 1;
  }

  return 0;
}

