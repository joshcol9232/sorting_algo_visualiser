#ifndef SORTARRAY_H
#define SORTARRAY_H

#include <cstddef>
#include <vector>
#include <mutex>
#include <memory>
#include <assert.h>
#include <SFML/System.hpp>

#include "constants.h"

/* Idea:
 * Have an array in a thread that sleeps
*/

class SortView {
 public:
  SortView(std::shared_ptr<std::vector<size_t>> data, size_t start, size_t end);

  SortView clone() { return SortView(data_, start_, end_); }

  void shuffle();

  const size_t& operator[](const size_t idx);

  size_t size() const { return end_ - start_; }
  size_t start() const { return start_; }
  size_t end() const { return end_; }
  bool is_sorted() const;

  void swap(const size_t a, const size_t b);

  // Allow const access to raw vector (rendering etc)
  const size_t& instant_access(const size_t idx) {
    std::lock_guard<std::mutex> data_lock(local_mutex_);
    return data_->at(idx);
  }

  SortView get_slice(const size_t rel_start, const size_t rel_end) const;

  int get_accessing() const { return accessing_; }
  const int* get_swapping() const { return swapping_; }

  void reset_metadata();

  void reset_swap_change() { swap_change_ = false; }
  bool get_swap_change() { return swap_change_; }

 private:
  std::shared_ptr<std::vector<size_t>> data_;  // Assumes this view only accesses it's own points
  size_t start_, end_;  // Start & end index of this data
  std::mutex local_mutex_;
  sf::Time step_timer;
  int accessing_;
  int swapping_[2];
  bool swap_change_;   // For playing audio..
};

#endif // SORTARRAY_H
