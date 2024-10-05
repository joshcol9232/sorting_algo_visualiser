#include "BarDisparityVisual.h"
#include "constants.h"

void BarDisparityVisual::render(sf::RenderWindow& window, const ItemDescriptor descriptor) {
  constexpr float minYSizeRatio = 0.01;

  barShape_.setFillColor(descriptor.colour);

  const float barStep = constants::WINDOW_WIDTH_F /
                        static_cast<float>(descriptor.maxIdx);
  //const float ySize = descriptor.ratio * static_cast<float>(constants::WINDOW_HEIGHT);
  
  const float disparityRatio = std::abs(descriptor.ratio - (static_cast<float>(descriptor.idx) /
                               static_cast<float>(descriptor.maxIdx)));
  const float ySize = constants::WINDOW_HEIGHT_F * (disparityRatio * (1.0 - minYSizeRatio) + minYSizeRatio);

  // Set position and size
  barShape_.setSize(sf::Vector2f(barStep, ySize));
  barShape_.setPosition(barStep * descriptor.idx,
                        (constants::WINDOW_HEIGHT_F - ySize)/2.0);
  window.draw(barShape_);
}

