#pragma once

#include <algorithm>

#include "StatArray.h"

template<typename iterator_type>
void bubble_sort(const iterator_type start, const iterator_type end) {
  bool sorted = false;

  while (!sorted) {
    sorted = true;
    for (iterator_type it = start; it != end-1; ++it) {
      const iterator_type next = it + 1;
      if (*it > *next) {
        std::iter_swap(it, next);
        sorted = false;
      }
    }
  }
}

