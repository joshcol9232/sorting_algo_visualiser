#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstddef>
#include <SFML/System.hpp>

namespace constants {

  const size_t WINDOW_HEIGHT = 600;
  const size_t WINDOW_WIDTH = 800;

  const sf::Time STEP_PERIOD = sf::milliseconds(50);

}

#endif // CONSTANTS_H