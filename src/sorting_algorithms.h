#pragma once

#include <algorithm>
#include <thread>

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


// Quick sorting
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

template<typename iterator_type>
void quicksort_multithreaded(const iterator_type b, const iterator_type e) {
  if (std::distance(b, e) > 1) {
    const iterator_type pivot = partition(b, e - 1);

    std::thread thread1(quicksort_multithreaded<iterator_type>, b, pivot);
    std::thread thread2(quicksort_multithreaded<iterator_type>, pivot + 1, e);

    thread1.join();
    thread2.join();
  }
}

// -------


// Merge sorting
namespace {

template<typename iterator_type>
void merge_in_place(iterator_type start, iterator_type mid, const iterator_type end) {
  iterator_type second_start = mid + 1;
  if (*mid < *second_start)
    return;


  while (start <= mid && second_start <= end) {
    if (*start <= *second_start) {  // Then it is in the correct place
      ++start;
    } else {
      // if element 1 is not in the right place, move it until it is.
      iterator_type it = second_start;
      const auto value = *it;

      // Shift all elements between element 1 and element 2 right by 1 to insert this element.
      while (it != start) {
        *it = *(it - 1);
        --it;
      }
      *start = value;

      ++start;
      ++mid;
      ++second_start;
    }
  }
}

}

template<typename iterator_type>
void merge_sort_in_place(const iterator_type l, const iterator_type r) {
  if (l < r) {
    const typename iterator_type::difference_type half_distance = (r - l)/2;
    const iterator_type m = l + half_distance;

    merge_sort_in_place(l, m);
    merge_sort_in_place(m + 1, r);
    merge_in_place(l, m, r);
  }
}

template<typename iterator_type>
void merge_sort_in_place_multithreaded(const iterator_type l, const iterator_type r) {
  if (l < r) {
    const typename iterator_type::difference_type half_distance = (r - l)/2;
    const iterator_type m = l + half_distance;


    std::thread thread1(merge_sort_in_place_multithreaded<iterator_type>, l, m);
    std::thread thread2(merge_sort_in_place_multithreaded<iterator_type>, m + 1, r);

    thread1.join();
    thread2.join();

    merge_in_place(l, m, r);
  }
}


template<typename iterator_type>
void heap_sort(const iterator_type start, const iterator_type end) {
  std::cout << "heap_sort> Making heap..." << std::endl;
  std::make_heap(start, end);
  std::cout << "heap_sort> Heap made. Sorting..." << std::endl;
  std::sort_heap(start, end);
  std::cout << "heap_sort> Sorted!" << std::endl;
}

