#include <iostream>
#include <string>
#include <thread>
#include <memory>

#include <SFML/Graphics.hpp>

#include "constants.h"
#include "sortarray.h"

namespace {

void bubble_sort(SortArray& arr) {
  bool sorted = false;

  while (!sorted) {
    sorted = true;
    for (size_t i = 0; i < arr.size()-1; ++i) {
      if (arr[i] > arr[i + 1]) {
        arr.swap(i, i+1);
        sorted = false;
      }
    }
  }
}

}  // namespace

int main() {
  // setup
  sf::RenderWindow window(sf::VideoMode(constants::WINDOW_WIDTH,
                                        constants::WINDOW_HEIGHT),
                          "Sorting Vis");

//  window.setFramerateLimit(60);
  SortArray sort_array(constants::NUM_ELEMENTS);

  // ------ Load sound ------

  /*
  sf::SoundBuffer snd_buff;
  if (!snd_buff.loadFromFile(constants::SOUND_FILE)) {
    throw std::runtime_error("Could not load sound.");
  }
  sf::Sound beep1;
  sf::Sound beep2;
  beep1.setBuffer(snd_buff);
  beep2.setBuffer(snd_buff);
  */

  // ----------------------------

  bool sorted = true;
  bool sorting = false;
  std::thread sorting_thread;

  const float bar_step = static_cast<float>(constants::WINDOW_WIDTH) /
                         sort_array.size();

  sf::RectangleShape base_shape(sf::Vector2f(bar_step, 1.0f));
  float y_size;
  size_t idx(0);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    // Process inputs
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {         // Shuffle
      sort_array.shuffle();
      sorted = false;
    } else if (!sorting && sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
      sorting = true;

      sorting_thread = std::thread([&sort_array, &sorting, &sorted](){
        bubble_sort(sort_array);
        sort_array.reset_metadata();
        sorting = false;
        sorted = true;
      });
      sorting_thread.detach();
    }

    // Draw
    window.clear(sf::Color::Black);

    idx = 0;
    for (const auto& val : sort_array.data()) {
      y_size = (static_cast<float>(val+1) /
                static_cast<float>(sort_array.size())) *
               static_cast<float>(constants::WINDOW_HEIGHT);

      // Set position and size
      base_shape.setSize(sf::Vector2f(bar_step, y_size));
      base_shape.setPosition(bar_step * idx, constants::WINDOW_HEIGHT - y_size);

      // Set colour
      base_shape.setFillColor(sf::Color::White);

      if (sorted) {
        base_shape.setFillColor(sf::Color::Green);
      } else {
        /* // DEBUGGING
        std::cout << "SWAPPING: {";
        for (const auto& swp : swapping)
          std::cout << swp << ", ";
        std::cout << "}" << std::endl;

        std::cout << "COMPARING: {";
        for (const auto& cmp : comparing)
          std::cout << cmp << ", ";
        std::cout << "}" << std::endl;
        std::cout << "---------------------" << std::endl;
        */

        const int* swapping = sort_array.get_swapping();
        if (idx == swapping[0] || idx == swapping[1]) {
          base_shape.setFillColor(sf::Color::Red);
        } else {
          const int acc = sort_array.get_accessing();
          if (idx == acc) {
            base_shape.setFillColor(sf::Color::Cyan);
          }
        }
      }

      window.draw(base_shape);
      ++idx;
    }

    window.display();
  }

  return 0;
}

