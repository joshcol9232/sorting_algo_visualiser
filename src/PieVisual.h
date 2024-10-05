#pragma once

#include "Visualisation.h"

class PieVisual : public Visualisation {
 public:
  PieVisual();
  virtual void render(sf::RenderWindow& window, const ItemDescriptor descriptor) override;
};

