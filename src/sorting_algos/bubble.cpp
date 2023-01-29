#include "bubble.h"

#include <algorithm>

bool Bubble::next(std::vector<size_t>& data) {
  if (on_compare_step_) {
    comparison_step();
    on_compare_step_ = false;
  } else {
    // Compare idx with idx + 1
    if (data[idx_] > data[idx_ + 1]) { // Swap
      swapping_.push_back(idx_);
      swapping_.push_back(idx_ + 1);
    }
    ++idx_;

    on_compare_step_ = true;

    if (idx_ >= data.size()) {
      idx_ = 0;
      return check_sorted(data);
    }
  }

  return false;
}

void Bubble::comparison_step() {
  comparing_.clear();
  comparing_.push_back(idx_);
  comparing_.push_back(idx_ + 1);
}



