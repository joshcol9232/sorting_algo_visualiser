#include <iostream>
#include <string>
#include <thread>
#include <memory>
#include <functional>
#include <random>
#include <numeric>
#include <algorithm>
#include <thread>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "constants.h"
#include "StatArray.h"
#include "Element.h"
#include "sorting_algorithms.h"

namespace {

float get_pitch(size_t value, size_t data_size) {
  return ((static_cast<float>(value)+1.0) /
           static_cast<float>(data_size)) *
           constants::PITCH_MULTIPLIER + 0.1;
}

}  // namespace

int main() {
  /*
  StatArray<int> aArray(std::vector<int>{1, 2, 4, 3});
  using val_type = StatArray<int>::Iterator::value_type;

  aArray.print_active();

  std::cout << aArray << std::endl;

  std::thread thread_obj(&std::sort<StatArray<int>::Iterator>, aArray.begin(), aArray.end());
  
  sf::sleep(constants::ACCESS_COST);
  aArray.print_active();

  thread_obj.join();
  */
  // ----------

  sf::RenderWindow window(sf::VideoMode(constants::WINDOW_WIDTH,
                                        constants::WINDOW_HEIGHT),
                          "Sorting Visualiser");

  window.setFramerateLimit(60);
  auto nums = std::vector<size_t>(constants::NUM_ELEMENTS, 0);
  std::iota(nums.begin(), nums.end(), 0);
  std::vector<Element<size_t>> data(constants::NUM_ELEMENTS);
  for (size_t idx = 0; idx < constants::NUM_ELEMENTS; ++idx) { data[idx] = Element(nums[idx]); }

  StatArray<Element<size_t>> main_array(std::move(data));

  // ------ Load sound ------

  sf::SoundBuffer snd_buff;
  if (!snd_buff.loadFromFile(constants::SOUND_FILE)) {
    throw std::runtime_error("Could not load sound.");
  }
  sf::Sound beep1;
  beep1.setBuffer(snd_buff);

  // ----------------------------

  bool sorted = true;
  bool sorting = false;
  bool shuffling = false;
  std::thread sorting_thread;

  const float bar_step = static_cast<float>(constants::WINDOW_WIDTH) /
                         main_array.size();

  sf::RectangleShape base_shape(sf::Vector2f(bar_step, 1.0f));

  float y_size;

  auto run_sorting_thread = [&](std::function<void(StatArray<Element<size_t>>::Iterator, StatArray<Element<size_t>>::Iterator)> F) {
    // Join previous thread
    if (sorting_thread.joinable()) { sorting_thread.join(); }

    sorting = true;

    sorting_thread = std::thread([&]() {
      F(main_array.begin(), main_array.end());
      sorting = false;
      sorted = true;
    });
    //sorting_thread.detach();
  };

  auto shuffle = [&]() {
    if (sorting_thread.joinable()) { sorting_thread.join(); }

    shuffling = true;
    sorted = false;

    sorting_thread = std::thread([&]() {
      main_array.shuffle();
      shuffling = false;
    });
  };

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    using IteratorType = StatArray<Element<size_t>>::Iterator;
    // Process inputs
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !shuffling) {
      std::cout << "SHUFFLING :)" << std::endl;
      shuffle();
    } else if (!sorting && sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
      run_sorting_thread(std::sort<IteratorType>);
    } else if (!sorting && sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
      run_sorting_thread(bubble_sort<IteratorType>);
    } else if (!sorting && sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
      run_sorting_thread(bogo_sort<IteratorType>);
    } else if (!sorting && sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
      run_sorting_thread(quicksort<IteratorType>);
    } else if (!sorting && sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
      run_sorting_thread(merge_sort_in_place<IteratorType>);
    }

    // Draw
    window.clear(sf::Color::Black);

    for (size_t idx = 0; idx < main_array.size(); ++idx) {
      Element<size_t>& element = main_array.instant_mutable_access(idx);
      const size_t val = *element;
      y_size = (static_cast<float>(val+1) /
                static_cast<float>(main_array.size())) *
                static_cast<float>(constants::WINDOW_HEIGHT);

      // Set position and size
      base_shape.setSize(sf::Vector2f(bar_step, y_size));
      base_shape.setPosition(bar_step * idx, constants::WINDOW_HEIGHT - y_size);

      // Set colour
      base_shape.setFillColor(sf::Color::White);

      if (sorted) {
        base_shape.setFillColor(sf::Color::Green);
      } else {
        if (main_array.is_active(idx)) {
          base_shape.setFillColor(sf::Color::Cyan);
        }

        if (element.just_copied()) {
          base_shape.setFillColor(sf::Color::Red);
          beep1.setPitch(get_pitch(val, main_array.size()));
          beep1.play();

          element.reset_copy_flag();
        }
      }

      window.draw(base_shape);
    }

    window.display();
  }

  return 0;
}

