#include "bubble.h"

#include <algorithm>

bool Bubble::next(std::vector<size_t>& data) {
  // Compare idx with idx + 1
  if (data[idx_] > data[idx_ + 1]) { // Swap
    swapping_.push_back(idx_);
    swapping_.push_back(idx_ + 1);
  }
  ++idx_;

  if (idx_ >= data.size()) {
    idx_ = 0;
    return check_sorted(data);
  }

  return false;
}
