#include "sortalgo.h"

void SortAlgo::perform_swaps(std::vector<size_t>& data) {
  // Swap those highlighted to swap
  for (size_t i = 1; i < swapping_.size(); i += 2) {
    // 0,1 .. 2,3 ..
    size_t temp = data[swapping_[i]];
    data[swapping_[i]] = data[swapping_[i-1]];
    data[swapping_[i-1]] = temp;
  }
  swapping_.clear();
}

bool SortAlgo::check_sorted(std::vector<size_t>& data) const {
  // Bubble-sort kind of check
  // assume sorted until invalid data found
  for (size_t i = 0; i < data.size()-1; ++i) {
    if (data[i] > data[i + 1]) {
      return false;
    }
  }

  return true;
}

void SortAlgo::reset_vecs() {
  comparing_.clear();
  swapping_.clear();
}
