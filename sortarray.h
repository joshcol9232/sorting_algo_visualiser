#ifndef SORTARRAY_H
#define SORTARRAY_H

#include <cstddef>
#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "sortalgo.h"

class SortArray {
 public:
  SortArray(size_t size);

  void step();
  void draw(sf::RenderWindow& window) const;
  void sort(SortAlgo* algo);

  void shuffle();

 private:
  std::vector<size_t> data_;
  std::unique_ptr<SortAlgo> algo_;
  bool sorted_;
  bool do_sort_;

  float bar_width_;
};

#endif // SORTARRAY_H
