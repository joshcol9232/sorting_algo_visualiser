#pragma once

#include <cstddef>
#include <string>
#include <SFML/System.hpp>

namespace constants {

  const size_t WINDOW_HEIGHT = 600;
  const size_t WINDOW_WIDTH  = 800;

  const size_t NUM_ELEMENTS = 500;

  const size_t __TIME_MULT = (size_t)1;
  const sf::Time ACCESS_COST = sf::microseconds(500 * __TIME_MULT);
  const sf::Time COPY_COST = sf::microseconds(500 * __TIME_MULT);
  const sf::Time COMPARISON_COST = sf::microseconds(500 * __TIME_MULT);

  const std::string SOUND_FILE = "./Blop.wav";
  const float PITCH_MULTIPLIER = 4.0;
  const size_t MAX_BEEPS_PER_FRAME = 1;

namespace runtime {
  bool disableSleeps(const bool disable = false); 
}  // namespace runtime

}  // namespace constants

