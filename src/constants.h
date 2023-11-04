#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstddef>
#include <string>
#include <SFML/System.hpp>

namespace constants {

  constexpr size_t WINDOW_HEIGHT = 600;
  constexpr size_t WINDOW_WIDTH = 800;

  constexpr size_t NUM_ELEMENTS = 4;

  const sf::Time ACCESS_COST = sf::milliseconds(500 / NUM_ELEMENTS);
  const sf::Time SWAP_COST_DIV_2 = sf::milliseconds(500 / NUM_ELEMENTS);

  const std::string SOUND_FILE = "./Blop.wav";
  constexpr float PITCH_MULTIPLIER = 4.0;


  constexpr float BAR_STEP = static_cast<float>(constants::WINDOW_WIDTH) /
                                                constants::NUM_ELEMENTS;

}

#endif // CONSTANTS_H
