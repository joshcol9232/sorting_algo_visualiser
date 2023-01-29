#ifndef BUBBLE_H
#define BUBBLE_H

#include "sortalgo.h"

// Bubble sort
class Bubble : public SortAlgo {
 public:
  Bubble() : idx_(0) {}

  bool next(std::vector<size_t>& data) override;

 private:
  size_t idx_;
};

#endif // BUBBLE_H
