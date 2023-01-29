#ifndef SORTARRAY_H
#define SORTARRAY_H

#include <cstddef>
#include <vector>
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
  const std::vector<size_t>& data() { return data_; }

  int get_accessing() const { return accessing_; }
  const int* get_swapping() const { return swapping_; }

  void reset_metadata();

 private:
  std::vector<size_t> data_;
//  sf::Time step_timer;
  int accessing_;
  int swapping_[2];
};

#endif // SORTARRAY_H
