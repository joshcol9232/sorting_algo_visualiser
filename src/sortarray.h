#ifndef SORTARRAY_H
#define SORTARRAY_H

#include <cstddef>
#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "sortalgo.h"

class SortArray {
 public:
  SortArray(size_t size);

  void step();
  void draw(sf::RenderWindow& window) const;
  void sound(sf::Sound& beep1, sf::Sound& beep2) const;
  void sort(SortAlgo* algo);

  void shuffle();
  void cancel_sort();

 private:
  std::vector<size_t> data_;
  std::unique_ptr<SortAlgo> algo_;
  bool sorted_;
  bool do_sort_;

  // Rendering
  float bar_width_;
};

#endif // SORTARRAY_H
