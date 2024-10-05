#pragma once

#include <cstddef>
#include <string>
#include <SFML/System.hpp>

namespace constants {

  constexpr size_t WINDOW_HEIGHT = 600;
  constexpr size_t WINDOW_WIDTH  = 800;

  constexpr float WINDOW_HEIGHT_F = static_cast<float>(WINDOW_HEIGHT);
  constexpr float WINDOW_WIDTH_F = static_cast<float>(WINDOW_WIDTH);

  constexpr size_t NUM_ELEMENTS = 200;

namespace {
  constexpr size_t __TIME_MULT = 5;
}  // namespace

  const sf::Time ACCESS_COST = sf::microseconds(500 * __TIME_MULT);
  const sf::Time COPY_COST = sf::microseconds(500 * __TIME_MULT);
  const sf::Time COMPARISON_COST = sf::microseconds(500 * __TIME_MULT);

  const std::string SOUND_FILE = "./Blop.wav";
  constexpr float PITCH_MULTIPLIER = 4.0;
  constexpr size_t MAX_BEEPS_PER_FRAME = 1;

namespace runtime {
  bool disableSleeps(const bool disable = false); 
}  // namespace runtime

}  // namespace constants

