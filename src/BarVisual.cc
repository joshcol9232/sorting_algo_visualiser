#include "BarVisual.h"
#include "constants.h"


BarVisual::BarVisual() :
  barShape_(sf::Vector2f(1.0f, 1.0f))
{}

void BarVisual::render(sf::RenderWindow& window, const ItemDescriptor descriptor) {
  barShape_.setFillColor(descriptor.colour);

  const float barStep = constants::WINDOW_WIDTH_F /
                        static_cast<float>(descriptor.maxIdx);
  const float ySize = descriptor.ratio * constants::WINDOW_HEIGHT_F;

  // Set position and size
  barShape_.setSize(sf::Vector2f(barStep, ySize));
  barShape_.setPosition(barStep * descriptor.idx, constants::WINDOW_HEIGHT_F - ySize);
  window.draw(barShape_);
}

