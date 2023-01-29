#ifndef BOGO_H
#define BOGO_H

#include "../sortalgo.h"

// Bogo sort
class Bogo : public SortAlgo {
 public:
  Bogo() : on_compare_step_(true) {}

  bool next(std::vector<size_t>& data) override;

 private:
  void comparison_step();
  void swap_step();

  bool on_compare_step_;
};

#endif // BOGO_H
