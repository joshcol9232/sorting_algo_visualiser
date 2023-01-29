#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include "constants.h"
#include "sortarray.h"
#include "sortalgo.h"
#include "sorting_algos/bubble.h"

int main() {
  // setup
  sf::RenderWindow window(sf::VideoMode(constants::WINDOW_WIDTH,
                                        constants::WINDOW_HEIGHT),
                          "Sorting Vis");

  window.setFramerateLimit(60);

  sf::Time step_clock;

  sf::Clock deltaClock;

  // ------ Set up sprites ------

//  sf::RectangleShape bar_shape(sf::Vector2f(constants::BAR_WIDTH,
//                                            constants::BAR_HEIGHT));
//  paddle_shape.setFillColor(sf::Color::White);
//  paddle_shape.setOrigin(constants::PADDLE_WIDTH/2.0,
//                         constants::PADDLE_HEIGHT/2.0);

  // ----------------------------

  SortArray sort_array(10);
//  sort_array.set_algo();

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    sf::Time dt_Time = deltaClock.restart();

//    const double dt = static_cast<double>(dt_Time.asSeconds());

    // Process inputs
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
      sort_array.shuffle();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
      sort_array.sort(new Bubble());
    }

    // Update
    step_clock += dt_Time;
    if (step_clock > constants::STEP_PERIOD) {
      sort_array.step();
      step_clock = sf::Time();  // Reset clock
    }

    // Draw
    window.clear(sf::Color::Black);
    sort_array.draw(window);

    window.display();
  }

  return 0;
}

