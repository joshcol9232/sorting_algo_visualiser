#ifndef BUBBLE_H
#define BUBBLE_H

#include "sortalgo.h"

// Bubble sort
class Bubble : public SortAlgo {
 public:
  Bubble() : idx_(0), on_compare_step_(true) {}

  bool next(std::vector<size_t>& data) override;

 private:
  void comparison_step();
  void swap_step();

  size_t idx_;
  bool on_compare_step_;
};

#endif // BUBBLE_H
