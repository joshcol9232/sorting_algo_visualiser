#pragma once

#include <SFML/Graphics.hpp>

struct ItemDescriptor {
  float ratio;
  size_t idx;
  size_t maxIdx;
  sf::Color colour;

  float getDisparity() const {
    return std::abs(ratio - (static_cast<float>(idx) / static_cast<float>(maxIdx)));
  }
};

class Visualisation {
 public:
  virtual void render(sf::RenderWindow& window, const ItemDescriptor descriptor) = 0;
};


