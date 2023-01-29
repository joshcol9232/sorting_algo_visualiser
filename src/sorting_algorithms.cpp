#include "sorting_algorithms.h"

#include <random>

void bubble_sort(SortView& arr) {
  bool sorted = false;

  while (!sorted) {
    sorted = true;
    for (size_t i = 0; i < arr.size()-1; ++i) {
      if (arr[i] > arr[i + 1]) {
        arr.swap(i, i+1);
        sorted = false;
      }
    }
  }
}

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
