#include <iostream>
#include <string_view>
#include <vector>
#include <algorithm>
#include <random>

#include "../src/sorting_algorithms.h"

namespace test {

constexpr size_t kNumIters = 200;
constexpr size_t kArraySize = 129;
constexpr bool kShuffle = true;

namespace tools {

template<typename iterator_type>
void shuffle(iterator_type begin, iterator_type end) {
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(begin, end, g);
}


std::vector<int>& vec_instance(const bool doShuffle = false) {

  static std::vector<int> the_vector([&]() {
    std::vector<int> v(kArraySize);
    std::iota(v.begin(), v.end(), 0);
    return v;
  }());

  if (doShuffle) { shuffle(the_vector.begin(), the_vector.end()); }

  return the_vector;
}

StatArray<int>& statarray_instance(const bool doShuffle = false) {

  static StatArray<int> the_st_array([&]() {
    std::vector<int> v(kArraySize);
    std::iota(v.begin(), v.end(), 0);
    return StatArray(std::move(v));
  }());

  if (doShuffle) { the_st_array.shuffle(); }

  return the_st_array;
}

}  // namespace tools


template<typename Functor>
bool test_sort_algo_vector(const Functor& S, const std::string_view name) {
  std::cout << "(test) " << name << "..." << std::endl;

  for (size_t i = 0; i < kNumIters; ++i) {
    std::vector<int>& v = tools::vec_instance(kShuffle);
    std::vector<int> initial_v = v;
    std::sort(initial_v.begin(), initial_v.end());

    S(v.begin(), v.end());
    if (std::is_sorted(v.begin(), v.end()) == false) {
      std::cout << "       Array not sorted!" << std::endl;
      return false;
    }
    if (std::equal(v.begin(), v.end(), initial_v.begin()) == false) {
      std::cout << "       Array has changed!" << std::endl;
      return false;
    }
  }

  return true;
}

template<typename Functor>
bool test_sort_algo(const Functor& S, const std::string_view name) {
  std::cout << "(test) " << name << "..." << std::endl;

  for (size_t i = 0; i < kNumIters; ++i) {
    StatArray<int>& v = tools::statarray_instance(kShuffle);
    StatArray<int> initial_v = v;
    std::sort(initial_v.begin(), initial_v.end());

    S(v.begin(), v.end());
    if (std::is_sorted(v.begin(), v.end()) == false) {
      std::cout << "       Array not sorted!" << std::endl;
      return false;
    }
    if (std::equal(v.begin(), v.end(), initial_v.begin()) == false) {
      std::cout << "       Array has changed!" << std::endl;
      return false;
    }
  }

  return true;
}

}  // namespace test


int main() {
  using IterTypeV = std::vector<int>::iterator;

  constants::runtime::disableSleeps(true);

  std::cout << "========== Testing with vector ==========" << std::endl;
  if (test::test_sort_algo_vector(bubble_sort<IterTypeV>, "bubble") == false) return 1;
  if (test::test_sort_algo_vector(quicksort<IterTypeV>, "quicksort") == false) return 1;
  if (test::test_sort_algo_vector(quicksort_multithreaded<IterTypeV>, "quicksort_multithreaded") == false) return 1;
  if (test::test_sort_algo_vector(merge_sort_in_place<IterTypeV>, "merge_sort_in_place") == false) return 1;
  if (test::test_sort_algo_vector(merge_sort_in_place_multithreaded<IterTypeV>, "merge_sort_in_place_multithreaded") == false) return 1;


  using IterType = StatArray<int>::Iterator;
  std::cout << "========== Testing with StatArray ==========" << std::endl;
  if (test::test_sort_algo(bubble_sort<IterType>, "bubble") == false) return 1;
  if (test::test_sort_algo(quicksort<IterType>, "quicksort") == false) return 1;
  if (test::test_sort_algo(quicksort_multithreaded<IterType>, "quicksort_multithreaded") == false) return 1;
  if (test::test_sort_algo(merge_sort_in_place<IterType>, "merge_sort_in_place") == false) return 1;
  if (test::test_sort_algo(merge_sort_in_place_multithreaded<IterType>, "merge_sort_in_place_multithreaded") == false) return 1;

  return 0;
}
