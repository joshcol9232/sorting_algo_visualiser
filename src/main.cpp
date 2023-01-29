#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include "constants.h"
#include "sortarray.h"
#include "sortalgo.h"

#include "sorting_algos/bubble.h"
#include "sorting_algos/bogo.h"


int main() {
  // setup
  sf::RenderWindow window(sf::VideoMode(constants::WINDOW_WIDTH,
                                        constants::WINDOW_HEIGHT),
                          "Sorting Vis");

//  window.setFramerateLimit(60);

  sf::Time step_clock;
  sf::Clock deltaClock;

  // ------ Load sound ------

  sf::SoundBuffer snd_buff;
  if (!snd_buff.loadFromFile(constants::SOUND_FILE)) {
    throw std::runtime_error("Could not load sound.");
  }
  sf::Sound beep1;
  sf::Sound beep2;
  beep1.setBuffer(snd_buff);
  beep2.setBuffer(snd_buff);

  // ----------------------------

  SortArray sort_array(constants::NUM_ELEMENTS);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    sf::Time dt_Time = deltaClock.restart();

    // Process inputs
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {         // Shuffle
      sort_array.shuffle();
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {  // Cancel sort
      sort_array.cancel_sort();
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
      sort_array.sort(new Bubble());
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
      sort_array.sort(new Bogo());
    }

    // Update
    step_clock += dt_Time;
    if (step_clock > constants::STEP_PERIOD) {
      sort_array.step();
      sort_array.sound(beep1, beep2);
      step_clock = sf::Time();  // Reset clock
    }

    // Draw
    window.clear(sf::Color::Black);
    sort_array.draw(window);

    window.display();
  }

  return 0;
}

