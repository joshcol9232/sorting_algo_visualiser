#pragma once

#include <algorithm>

#include "StatArray.h"

template<typename iterator_type>
void bubble_sort(const iterator_type start, const iterator_type end) {
  bool sorted = false;

  while (!sorted) {
    sorted = true;
    for (iterator_type it = start; it != end - 1; ++it) {
      const iterator_type next = it + 1;
      if (*it > *next) {
        std::iter_swap(it, next);
        sorted = false;
      }
    }
  }
}

template<typename iterator_type>
bool is_sorted(const iterator_type start, const iterator_type end) {
  for (iterator_type it = start; it != end - 1; ++it) {
    if (*it > *(it + 1)) {
      return false;
    }
  }
  return true;
}

template<typename iterator_type>
void bogo_sort(const iterator_type start, const iterator_type end) {
  bool sorted = false;

  // Choose two random elements to swap
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> distrib(0, end - start - 1);

  while (!sorted) {
    std::iter_swap(start + distrib(gen), start + distrib(gen));
    sorted = is_sorted(start, end);
  }
}

