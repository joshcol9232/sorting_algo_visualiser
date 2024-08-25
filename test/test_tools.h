#pragma once

#include <vector>
#include <random>
#include <algorithm>

#include "../src/StatArray.h"

namespace test {

constexpr size_t kNumIters = 200;
constexpr size_t kArraySize = 129;

namespace tools {


std::vector<int> make_int_vec(const size_t size = kArraySize) {
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

std::vector<int>& vec_instance(const bool doShuffle = false) {

  static std::vector<int> the_vector(make_int_vec());

  if (doShuffle) { shuffle(the_vector.begin(), the_vector.end()); }

  return the_vector;
}

StatArray<int>& statarray_instance(const bool doShuffle = false) {

  static StatArray<int> the_st_array(std::move(make_int_vec()));

  if (doShuffle) { the_st_array.shuffle(); }

  return the_st_array;
}

}  // namespace tools
}  // namespace test

