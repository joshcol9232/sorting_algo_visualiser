#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include "constants.h"

int main() {
  // setup
  sf::RenderWindow window(sf::VideoMode(constants::WINDOW_WIDTH,
                                        constants::WINDOW_HEIGHT),
                          "Sorting Vis");

//  window.setFramerateLimit(60);
  sf::Clock deltaClock;

  // ------ Set up sprites ------

//  sf::RectangleShape bar_shape(sf::Vector2f(constants::BAR_WIDTH,
//                                            constants::BAR_HEIGHT));
//  paddle_shape.setFillColor(sf::Color::White);
//  paddle_shape.setOrigin(constants::PADDLE_WIDTH/2.0,
//                         constants::PADDLE_HEIGHT/2.0);

  // ----------------------------

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    sf::Time dt_Time = deltaClock.restart();
    const double dt = static_cast<double>(dt_Time.asSeconds());

    // Process inputs
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
//      right_paddle.move(dt, false);
//    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
//      right_paddle.move(dt, true);
//    }

    // Update

    // Draw
    window.clear(sf::Color::Black);

    window.display();
  }

  return 0;
}

