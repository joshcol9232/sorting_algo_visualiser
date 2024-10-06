#include <iostream>
#include <string_view>
#include <vector>
#include <algorithm>
#include <random>

#include "test_tools.h"
#include "../src/sortingAlgorithms.h"
#include "../src/constants.h"


namespace test {

constexpr bool kShuffle = true;


template<typename Functor>
bool testSortAlgoVector(const Functor& S, const std::string_view name) {
  std::cout << "(test) " << name << "..." << std::endl;

  for (size_t i = 0; i < kNumIters; ++i) {
    std::vector<int>& v = tools::vecInstance(kShuffle);
    std::vector<int> initialV = v;
    std::sort(initialV.begin(), initialV.end());

    S(v.begin(), v.end());
    if (std::is_sorted(v.begin(), v.end()) == false) {
      std::cout << "       Array not sorted!" << std::endl;
      return false;
    }
    if (v != initialV) {
      std::cout << "       Array has changed!" << std::endl;
      return false;
    }
  }

  return true;
}

template<typename Functor>
bool testSortAlgo(const Functor& S, const std::string_view name) {
  std::cout << "(test) " << name << "..." << std::endl;

  for (size_t i = 0; i < kNumIters; ++i) {
    StatArray<int>& v = tools::statarrayInstance(kShuffle);
    StatArray<int> initialV = v;
    std::sort(initialV.begin(), initialV.end());

    S(v.begin(), v.end());
    if (std::is_sorted(v.begin(), v.end()) == false) {
      std::cout << "       Array not sorted!" << std::endl;
      return false;
    }
    if (std::equal(v.begin(), v.end(), initialV.begin()) == false) {
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
  if (test::testSortAlgoVector(bubbleSort<IterTypeV>, "bubble") == false) return 1;
  if (test::testSortAlgoVector(quicksort<IterTypeV>, "quicksort") == false) return 1;
  if (test::testSortAlgoVector(quicksortMultithreaded<IterTypeV>, "quicksortMultithreaded") == false) return 1;
  // TODO: Fix merge sort.
  //if (test::testSortAlgoVector(mergeSortInPlace<IterTypeV>, "mergeSortInPlace") == false) return 1;
  //if (test::testSortAlgoVector(mergeSortInPlaceMultithreaded<IterTypeV>, "mergeSortInPlaceMultithreaded") == false) return 1;
  if (test::testSortAlgoVector(shellSort<IterTypeV>, "shellSort") == false) return 1;


  using IterType = StatArray<int>::Iterator;
  std::cout << "========== Testing with StatArray ==========" << std::endl;
  if (test::testSortAlgo(bubbleSort<IterType>, "bubble") == false) return 1;
  if (test::testSortAlgo(quicksort<IterType>, "quicksort") == false) return 1;
  if (test::testSortAlgo(quicksortMultithreaded<IterType>, "quicksortMultithreaded") == false) return 1;
  //if (test::testSortAlgo(mergeSortInPlace<IterType>, "mergeSortInPlace") == false) return 1;
  //if (test::testSortAlgo(mergeSortInPlaceMultithreaded<IterType>, "mergeSortInPlaceMultithreaded") == false) return 1;
  if (test::testSortAlgo(shellSort<IterType>, "shellSort") == false) return 1;

  return 0;
}
