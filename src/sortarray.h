#ifndef SORTARRAY_H
#define SORTARRAY_H

#include <cstddef>
#include <vector>
#include <mutex>
#include <SFML/System.hpp>

#include "constants.h"

/* Idea:
 * Have an array in a thread that sleeps
*/

class SortArray {
 public:
  SortArray(size_t size);
  void shuffle();

  const size_t& operator[](const size_t idx);

  size_t size() const { return data_.size(); }
  bool is_sorted() const;

  void swap(const size_t a, const size_t b);

  // Allow const access to raw vector (rendering etc)
  const size_t& instant_access(const size_t idx) {
    std::lock_guard<std::mutex> data_lock(data_mutex_);
    return data_[idx];
  }

  int get_accessing() const { return accessing_; }
  const int* get_swapping() const { return swapping_; }

  void reset_metadata();

  void reset_swap_change() { swap_change_ = false; }
  bool get_swap_change() { return swap_change_; }

 private:
  std::vector<size_t> data_;
  std::mutex data_mutex_;
//  sf::Time step_timer;
  int accessing_;
  int swapping_[2];
  bool swap_change_;   // For playing audio..
};

#endif // SORTARRAY_H
