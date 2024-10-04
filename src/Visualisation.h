#pragma once

#include <SFML/Graphics.hpp>

struct ItemDescriptor {
  float ratio;
  size_t idx;
  size_t maxIdx;
  sf::Color colour;
};

class Visualisation {
 public:
  virtual void render(sf::RenderWindow& window, const ItemDescriptor descriptor) = 0;
};


