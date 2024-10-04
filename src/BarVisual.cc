#include "BarVisual.h"
#include "constants.h"


BarVisual::BarVisual() :
  barShape_(sf::Vector2f(1.0f, 1.0f))
{
}

void BarVisual::render(sf::RenderWindow& window, const ItemDescriptor descriptor) {
  barShape_.setFillColor(descriptor.colour);

  const float barStep = static_cast<float>(constants::WINDOW_WIDTH) /
                        static_cast<float>(descriptor.maxIdx);
  const float ySize = descriptor.ratio * static_cast<float>(constants::WINDOW_HEIGHT);

  // Set position and size
  barShape_.setSize(sf::Vector2f(barStep, ySize));
  barShape_.setPosition(barStep * descriptor.idx, constants::WINDOW_HEIGHT - ySize);
  window.draw(barShape_);
}

