#include "PieDisparityVisual.h"
#include "constants.h"

#include <math.h>

PieDisparityVisual::PieDisparityVisual()
{}

void PieDisparityVisual::render(sf::RenderWindow& window, const ItemDescriptor descriptor) {
  constexpr float absoluteMaxRadius = std::min(constants::WINDOW_HEIGHT_F/2.0,
                                               constants::WINDOW_WIDTH_F/2.0);
  constexpr float maxRadiusModifier = 0.9;

  constexpr float maxRadius = absoluteMaxRadius * maxRadiusModifier;
  constexpr float minRadiusRatio = 0.2;

  const float radiusRatio = (1.0 - descriptor.getDisparity()) * (1.0 - minRadiusRatio) + minRadiusRatio;
  const float radius = maxRadius * radiusRatio;

  const float pieStep = 2.0 * M_PI / static_cast<float>(descriptor.maxIdx);
  const float angle0 = pieStep * static_cast<float>(descriptor.idx);
  const float angle1 = pieStep * static_cast<float>(descriptor.idx + 1);

  sf::ConvexShape slice;
  slice.setPointCount(3);

  const sf::Vector2f centre(constants::WINDOW_WIDTH_F/2.0, constants::WINDOW_HEIGHT_F/2.0);

  slice.setPoint(0, centre);
  slice.setPoint(1, centre + sf::Vector2f(std::sin(angle0), std::cos(angle0)) * radius);
  slice.setPoint(2, centre + sf::Vector2f(std::sin(angle1), std::cos(angle1)) * radius);

  slice.setFillColor(descriptor.colour);

  window.draw(slice);
}

