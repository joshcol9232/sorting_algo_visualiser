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


namespace {

template<typename iterator_type>
iterator_type partition(const iterator_type left, const iterator_type right) {
  iterator_type i = left - 1;
  iterator_type it = left;

  while(it < right) {
    if(*it <= *right) {
      ++i;
      std::iter_swap(i, it);
    }
    ++it;
  }

  std::iter_swap(i + 1, right); 
  return ++i;
}

}

template<typename iterator_type>
void quicksort(const iterator_type b, const iterator_type e) {
  if (std::distance(b, e) > 1) {
    const iterator_type pivot = partition(b, e - 1);
    quicksort(b, pivot);
    quicksort(pivot + 1, e);
  }
}

