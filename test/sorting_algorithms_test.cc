#include <iostream>
#include <string_view>
#include <vector>
#include <algorithm>
#include <random>

#include "test_tools.h"
#include "../src/sorting_algorithms.h"
#include "../src/constants.h"


namespace test {

constexpr bool kShuffle = true;


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
    if (v != initial_v) {
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
