#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstddef>
#include <string>
#include <SFML/System.hpp>

namespace constants {

  const size_t WINDOW_HEIGHT = 600;
  const size_t WINDOW_WIDTH = 800;

  const size_t NUM_ELEMENTS = 100;

  const sf::Time ACCESS_COST = sf::microseconds(1);
  const sf::Time SWAP_COST_DIV_2 = sf::microseconds(25);

  const std::string SOUND_FILE = "./Blop.wav";
  const float PITCH_MULTIPLIER = 4.0;

}

#endif // CONSTANTS_H
