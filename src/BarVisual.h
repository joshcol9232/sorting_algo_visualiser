#pragma once

#include "Visualisation.h"

class BarVisual : public Visualisation {
 public:
  BarVisual();
  void render(sf::RenderWindow& window, const ItemDescriptor descriptor) override;
 private:
  sf::RectangleShape barShape_;
};

