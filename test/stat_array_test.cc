#include <iostream>

#include "../src/StatArray.h"

namespace test {

class StatArrayFixture {
 private:
  static StatArrayFixture& getInstance() {
    static StatArrayFixture theStatArrayFixture;
    return theStatArrayFixture;
  }

 public:
  static StatArray<int>& array() { return getInstance().array_; }
  static constexpr size_t size() { return 8; }

 private:
  StatArrayFixture() : array_( std::vector<int>(size()) ) {
    std::iota(array_.begin(), array_.end(), 0);
  }

  StatArray<int> array_;
};

bool startIsBegin() {
  std::cout << "(test) startIsBegin..." << std::endl;
  return StatArrayFixture::array()[0] == *(StatArrayFixture::array().begin());
}
bool endIsEnd() {
  std::cout << "(test) endIsEnd..." << std::endl;
  return StatArrayFixture::array()[StatArrayFixture::size()-1] == *(StatArrayFixture::array().end() - 1);
}

}  // namespace test

int main() {
  if (! test::startIsBegin()) return 1;
  if (! test::endIsEnd()) return 1;

  return 0;
}

