#include <iostream>
#include <string>
#include <thread>
#include <memory>
#include <functional>
#include <random>

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

void bogo_sort(SortArray& arr) {
  bool sorted = false;

  // Choose two random elements to swap
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> distrib(0, arr.size()-1);

  while (!sorted) {
    arr.swap(distrib(gen), distrib(gen));
    sorted = arr.is_sorted();
  }
}

}  // namespace

int main() {
  // setup
  sf::RenderWindow window(sf::VideoMode(constants::WINDOW_WIDTH,
                                        constants::WINDOW_HEIGHT),
                          "Sorting Visualiser");

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

  auto run_sorting_thread = [&](std::function<void(SortArray&)> F) {
    sorting = true;

    sorting_thread = std::thread([&](){
      F(sort_array);
      sort_array.reset_metadata();
      sorting = false;
      sorted = true;
    });
    sorting_thread.detach();
  };

  float y_size;

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
      run_sorting_thread(bubble_sort);
    } else if (!sorting && sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
      run_sorting_thread(bogo_sort);
    }

    // Draw
    window.clear(sf::Color::Black);

    for (size_t idx = 0; idx < sort_array.size(); ++idx) {
      y_size = (static_cast<float>(sort_array.instant_access(idx)+1) /
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
    }

    window.display();
  }

  return 0;
}

