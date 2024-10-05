#pragma once

#include "Visualisation.h"

class BarVisual : public Visualisation {
 public:
  BarVisual();
  virtual void render(sf::RenderWindow& window, const ItemDescriptor descriptor) override;
 protected:
  sf::RectangleShape barShape_;
};

