#pragma once

#include <vector>
#include <random>
#include <algorithm>

#include "../src/StatArray.h"

namespace test {

constexpr size_t kNumIters = 200;
constexpr size_t kArraySize = 129;

namespace tools {


std::vector<int> makeIntVec(const size_t size = kArraySize) {
  std::vector<int> v(size);
  std::iota(v.begin(), v.end(), 0);
  return v;
}


template<typename iterator_type>
void shuffle(iterator_type begin, iterator_type end) {
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(begin, end, g);
}

std::vector<int>& vecInstance(const bool doShuffle = false) {

  static std::vector<int> theVector(makeIntVec());

  if (doShuffle) { shuffle(theVector.begin(), theVector.end()); }

  return theVector;
}

StatArray<int>& statarrayInstance(const bool doShuffle = false) {

  static StatArray<int> theStArray(std::move(makeIntVec()));

  if (doShuffle) { theStArray.shuffle(); }

  return theStArray;
}

}  // namespace tools
}  // namespace test

