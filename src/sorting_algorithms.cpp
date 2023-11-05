#include "sorting_algorithms.h"

#include <random>

void bogo_sort(SortView& arr) {
  bool sorted = false;

  // Choose two random elements to swap
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> distrib(0, arr.size()-1);

  while (!sorted) {
    arr.swap(distrib(gen), distrib(gen));
    sorted = arr.is_sorted();
  }
}
