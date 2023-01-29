#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstddef>
#include <SFML/System.hpp>

namespace constants {

  const size_t WINDOW_HEIGHT = 600;
  const size_t WINDOW_WIDTH = 800;

  const size_t NUM_ELEMENTS = 50;

  const sf::Time STEP_PERIOD = sf::milliseconds(10);

}

#endif // CONSTANTS_H
