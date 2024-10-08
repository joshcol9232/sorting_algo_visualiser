
#include "viridis_palette.h"

namespace color_funcs {

sf::Color viridis(const float ratio) {
  const size_t color_index = static_cast<size_t>(ratio * static_cast<float>(VIRIDIS_PALETTE_LENGTH)) * 3;
  return sf::Color(static_cast<char>(VIRIDIS[color_index] * 255.0),
                   static_cast<char>(VIRIDIS[color_index + 1] * 255.0),
                   static_cast<char>(VIRIDIS[color_index + 2] * 255.0));
}

sf::Color bw(const float ratio) {
  const char brightness = static_cast<char>(ratio * 255.0);
  return sf::Color(brightness, brightness, brightness);
}

}  // namespace color_funcs

