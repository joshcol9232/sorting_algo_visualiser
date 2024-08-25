#pragma once

#include <cstddef>
#include <string>
#include <SFML/System.hpp>

namespace constants {

  const size_t WINDOW_HEIGHT = 600;
  const size_t WINDOW_WIDTH  = 800;

  const size_t NUM_ELEMENTS = 200;

  const sf::Time ACCESS_COST = sf::microseconds(500);
  const sf::Time COPY_COST = sf::microseconds(500);
  const sf::Time COMPARISON_COST = sf::microseconds(500);

  const std::string SOUND_FILE = "./Blop.wav";
  const float PITCH_MULTIPLIER = 4.0;

namespace runtime {
  bool disableSleeps(const bool disable = false) {
    static bool doDisable = disable;
    return doDisable;
  }
}  // namespace runtime

}  // namespace constants

