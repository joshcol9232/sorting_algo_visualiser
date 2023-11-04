#include "sorting_algorithms.h"

#include <random>
#include <iostream>

void bubble_sort(std::shared_ptr<SortArray> arr) {
  bool sorted = false;

  while (!sorted) {
    sorted = true;
    for (size_t i = 0; i < arr->size()-1; ++i) {
      if ((*arr)[i] > (*arr)[i + 1]) {
        arr->swap(i, i+1);
        sorted = false;
      }
    }
  }
}

void bogo_sort(std::shared_ptr<SortArray> arr) {
  bool sorted = false;

  // Choose two random elements to swap
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> distrib(0, arr->size()-1);

  size_t a, b;
  while (!sorted) {
    a = distrib(gen);
    b = distrib(gen);
    while (a == b)
      b = distrib(gen);

    arr->swap(a, b);
    sorted = arr->is_sorted();
  }
}

/*
std::shared_ptr<SortArray> quick_sort(std::shared_ptr<SortArray> arr) {
  if (arr->size() < 2) {
    std::cout << "Returning..." << std::endl;
    return arr;
  }

  std::cout << "ARRAY SIZE=" << arr->size();
  const size_t mid = arr->size() / 2;
  std::cout << "mid=" << mid << std::endl;
  const size_t pivot = (*arr)[mid];
  std::cout << "pivot=" << pivot << std::endl;

  auto low = arr->make_empty_child(0);
  for (size_t idx = 0; idx < arr->size(); ++idx) {
    const size_t value = (*arr)[idx];
    if (value <= pivot) {
      low->push_back(value);
    }
  }
  std::cout << "Low size: " << low->size() << std::endl;

  auto high = arr->make_empty_child(low->size()-1);
  for (size_t idx = 0; idx < arr->size(); ++idx) {
    const size_t value = (*arr)[idx];
    if (value > pivot) {
      high->push_back(value);
    }
  }
  std::cout << "High size: " << high->size() << std::endl;

  low = quick_sort(low);
  high = quick_sort(high);
  low->push_back(pivot);
  low->concatenate(*high);
  return low;
}
*/

std::shared_ptr<SortArray> quick_sort(std::shared_ptr<SortArray> arr) {
  if (arr->size() < 2) return arr;

  const size_t pivot = (*arr)[arr->size() / 2];

  auto low = arr->make_empty_child(0);
  auto mid = arr->make_empty_child(0);
  auto high = arr->make_empty_child(0);

  for (size_t idx = 0; idx < arr->size(); ++idx) {
    const size_t val = (*arr)[idx];
    if (val < pivot) {
      low->push_back(val);
      mid->increment_offset();
      high->increment_offset();
    } else if (val > pivot) {
      high->push_back(val);
    } else {
      mid->push_back(val);
      high->increment_offset();
    }
  }

  low = quick_sort(low);
  high = quick_sort(high);
  low->concatenate(*mid);
  low->concatenate(*high);

  return low;
}

