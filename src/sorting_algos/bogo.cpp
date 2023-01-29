#include "bogo.h"

#include <random>

bool Bogo::next(std::vector<size_t>& data) {
  if (on_compare_step_) {
    comparison_step(data.size());
    on_compare_step_ = false;
  } else {
    // Swap
    swapping_.push_back(comparing_[2]);
    swapping_.push_back(comparing_[1]);

    on_compare_step_ = true;

    return check_sorted(data);
  }

  return false;
}

void Bogo::comparison_step(const size_t data_size) {
  comparing_.clear();
  // Choose two random elements to swap
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, data_size);

  comparing_.push_back(distrib(gen));
  comparing_.push_back(distrib(gen));
}


