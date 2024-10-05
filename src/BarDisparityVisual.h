#pragma once

#include "BarVisual.h"

class BarDisparityVisual : public BarVisual {
 public:
  BarDisparityVisual() {}
  void render(sf::RenderWindow& window, const ItemDescriptor descriptor) override;
};

