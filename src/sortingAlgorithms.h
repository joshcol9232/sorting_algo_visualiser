#pragma once

#include <algorithm>
#include <cmath>
#include <thread>

#include "StatArray.h"

template<typename iterator_type>
void bubbleSort(const iterator_type start, const iterator_type end) {
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
void bogoSort(const iterator_type start, const iterator_type end) {
  bool sorted = false;

  // Choose two random elements to swap
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> distrib(0, end - start - 1);

  while (!sorted) {
    std::iter_swap(start + distrib(gen), start + distrib(gen));
    sorted = std::is_sorted(start, end);
  }
}


// Quick sorting
namespace {

template<typename iterator_type>
iterator_type partition(const iterator_type left, const iterator_type right) {
  iterator_type i = left - 1;

  for (iterator_type it = left; it < right; ++it) {
    if (*it <= *right) {
      ++i;
      std::iter_swap(i, it);
    }
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

template<typename iterator_type>
void quicksortMultithreaded(const iterator_type b, const iterator_type e) {
  if (std::distance(b, e) > 1) {
    const iterator_type pivot = partition(b, e - 1);

    std::thread thread1(quicksortMultithreaded<iterator_type>, b, pivot);
    std::thread thread2(quicksortMultithreaded<iterator_type>, pivot + 1, e);

    thread1.join();
    thread2.join();
  }
}

// -------


// Merge sorting
namespace {

template<typename iterator_type>
void mergeInPlace(iterator_type start, iterator_type mid, const iterator_type end) {
  iterator_type secondStart = mid + 1;
  if (*mid <= *secondStart)
    return;

  while (start <= mid && secondStart <= end) {
    if (*start <= *secondStart) {  // Then it is in the correct place
      ++start;
    } else {
      // if element 1 is not in the right place, move it until it is.
      // Shift all elements between element 1 and element 2 left by 1 to insert this element.
      const auto value = *secondStart;
      iterator_type it = secondStart;

      while (it > start) {
        *it = *(it - 1);
        --it;
      }
      *start = value;

      ++start;
      ++mid;
      ++secondStart;
    }
  }
}

}

template<typename iterator_type>
void mergeSortInPlace(const iterator_type l, const iterator_type r) {
  if (l < r) {
    const typename iterator_type::difference_type halfDistance = (r - l)/2;
    const iterator_type m = l + halfDistance;

    mergeSortInPlace(l, m);
    mergeSortInPlace(m + 1, r);
    mergeInPlace(l, m, r);
  }
}

template<typename iterator_type>
void mergeSortInPlaceMultithreaded(const iterator_type l, const iterator_type r) {
  if (l < r) {
    const typename iterator_type::difference_type halfDistance = (r - l)/2;
    const iterator_type m = l + halfDistance;


    std::thread thread1(mergeSortInPlaceMultithreaded<iterator_type>, l, m);
    std::thread thread2(mergeSortInPlaceMultithreaded<iterator_type>, m + 1, r);

    thread1.join();
    thread2.join();

    mergeInPlace(l, m, r);
  }
}


template<typename iterator_type>
void heapSort(const iterator_type start, const iterator_type end) {
  std::cout << "heap_sort> Making heap..." << std::endl;
  std::make_heap(start, end);
  std::cout << "heap_sort> Heap made. Sorting..." << std::endl;
  std::sort_heap(start, end);
  std::cout << "heap_sort> Sorted!" << std::endl;
}


namespace {

std::vector<size_t> shellSortGaps(const size_t maxIdx) {
  // From https://en.wikipedia.org/wiki/Shellsort#Gap_sequences
  // A036562

  auto gapFunc = [](const size_t k) { return pow(4, k) + 3 * pow(2, k - 1) + 1; };

  std::vector<size_t> gaps { 1 };

  size_t next = 0;

  // Pretty weird loop!
  // Reason: Needs to check the next `gapFunc(k)` is below maxIdx, BEFORE append.
  for (size_t k = 1; (next = gapFunc(k)) < maxIdx; ++k) {
    gaps.emplace_back(next);
  }

  std::reverse(gaps.begin(), gaps.end());

  return gaps;
}

}  // namespace

template<typename iterator_type>
void shellSort(const iterator_type start, const iterator_type end) {
  const size_t size = end - start;

  for (const size_t gap : shellSortGaps(size)) {
    for (auto it = start + gap; it != end; ++it) {
      const auto tempVal = *it;

      auto itJ = it;
      for (itJ = it; (itJ - start) >= gap && (*(itJ - gap) > tempVal); itJ -= gap) {
        *itJ = *(itJ - gap);
      }
      
      *itJ = tempVal;
    }
  }
}


