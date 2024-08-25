#include <iostream>
#include <vector>

#include "../src/StatArray.h"

#include "test_tools.h"

namespace test {


bool startIsBegin() {
  std::cout << "(test) startIsBegin..." << std::endl;
  return tools::statarray_instance()[0] == *(tools::statarray_instance().begin());
}
bool endIsEnd() {
  std::cout << "(test) endIsEnd..." << std::endl;
  return tools::statarray_instance()[tools::statarray_instance().size()-1] == *(tools::statarray_instance().end() - 1);
}

bool grow() {
  std::cout << "(test) grow..." << std::endl;
  StatArray<int> arr(tools::make_int_vec(10));
  arr.grow();
  return arr.size() == 11;
}

bool shrink() {
  std::cout << "(test) shrink..." << std::endl;
  StatArray<int> arr(tools::make_int_vec(10));
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

