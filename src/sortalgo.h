#ifndef SORTALGO_H
#define SORTALGO_H

#include <cstddef>
#include <vector>

// Interface for sorting algorithms

class SortAlgo {
 public:
  SortAlgo() {}

  virtual bool next(std::vector<size_t>& data) = 0;  // Stepping defined in algorithm
  virtual void perform_swaps(std::vector<size_t>& data);

  const std::vector<size_t>& get_comparing() const { return comparing_; }
  const std::vector<size_t>& get_swapping() const { return swapping_; }
  void reset_vecs();

 protected:
  bool check_sorted(std::vector<size_t>& data) const;

  std::vector<size_t> comparing_;    // Indicies in comparison (Yellow)
  std::vector<size_t> swapping_;     // Indicies being swapped (Red)
};

#endif // SORTALGO_H
