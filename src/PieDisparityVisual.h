#pragma once

#include "Visualisation.h"

class PieDisparityVisual : public Visualisation {
 public:
  PieDisparityVisual();
  virtual void render(sf::RenderWindow& window, const ItemDescriptor descriptor) override;
};

