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

#include "viridis_palette.h"

//#define DEBUG


namespace {

float get_pitch(size_t value, size_t data_size) {
  return ((static_cast<float>(value) + 1.0) /
           static_cast<float>(data_size)) *
           constants::PITCH_MULTIPLIER + 0.1;
}

sf::Color value_to_color(const size_t value, const size_t data_size) {
  // Set base colour to viridis
  const float ratio = static_cast<float>(value) / static_cast<float>(data_size);
  const size_t color_index = static_cast<size_t>(ratio * static_cast<float>(VIRIDIS_PALETTE_LENGTH)) * 3;
  return sf::Color(static_cast<char>(VIRIDIS[color_index] * 255.0),
                   static_cast<char>(VIRIDIS[color_index + 1] * 255.0),
                   static_cast<char>(VIRIDIS[color_index + 2] * 255.0));
}

}  // namespace

int main() {
  using ArrayType = StatArray<Element<size_t>>;
  using IteratorType = ArrayType::Iterator;

  constants::runtime::disableSleeps(true);

  sf::RenderWindow window(sf::VideoMode(constants::WINDOW_WIDTH,
                                        constants::WINDOW_HEIGHT),
                          "Sorting Visualiser");

  window.setFramerateLimit(60);
  auto nums = std::vector<size_t>(constants::NUM_ELEMENTS, 0);
  std::iota(nums.begin(), nums.end(), 0);
  std::vector<Element<size_t>> data(constants::NUM_ELEMENTS);
  for (size_t idx = 0; idx < constants::NUM_ELEMENTS; ++idx) { data[idx] = Element(nums[idx]); }

  ArrayType main_array(std::move(data));

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
  std::thread sorting_thread;

  auto run_sorting_thread = [&](const std::function<void(IteratorType, IteratorType)>& F) {
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

  auto non_sort = [&](const std::function<void()>& F) {
    if (sorting_thread.joinable()) { sorting_thread.join(); }

    sorting = true;
    sorted = false;

    sorting_thread = std::thread([&]() {
      F();
      sorting = false;
    });
  };

  float y_size;
  sf::RectangleShape base_shape(sf::Vector2f(static_cast<float>(constants::WINDOW_WIDTH) /
                                             main_array.size(), 1.0f));

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    // Process inputs
    if (!sorting) {
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        std::cout << "SHUFFLING" << std::endl;
        non_sort([&]() { main_array.shuffle(); });
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        std::cout << "REVERSING" << std::endl;
        non_sort([&]() { std::reverse(main_array.begin(), main_array.end()); });
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) {
        const bool sort_tmp = sorted;
        non_sort([&]() { main_array.grow(); });
        sorted = sort_tmp;
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) {
        const bool sort_tmp = sorted;
        non_sort([&]() { main_array.shrink(); });
        sorted = sort_tmp;
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        std::cout << "REVERSING" << std::endl;
        non_sort([&]() { std::reverse(main_array.begin(), main_array.end()); });
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
        run_sorting_thread(std::sort<IteratorType>);
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
        run_sorting_thread(bubble_sort<IteratorType>);
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
        run_sorting_thread(bogo_sort<IteratorType>);
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
        run_sorting_thread(quicksort<IteratorType>);
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
        run_sorting_thread(quicksort_multithreaded<IteratorType>);
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) {
        run_sorting_thread(merge_sort_in_place<IteratorType>);
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)) {
        run_sorting_thread(merge_sort_in_place_multithreaded<IteratorType>);
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)) {
        run_sorting_thread(heap_sort<IteratorType>);
      }
    }

    // Draw
    window.clear(sf::Color::Black);

    const float bar_step = static_cast<float>(constants::WINDOW_WIDTH) /
                           main_array.size();

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
      sf::Color barColor = sf::Color::White;

      if (sorted) {
        barColor = sf::Color::Green;
      } else if (main_array.is_active(idx)) {
        barColor = sf::Color::Cyan;
      } else if (element.just_copied()) {
        barColor = sf::Color::Red;
        beep1.setPitch(get_pitch(val, main_array.size()));
        beep1.play();

        element.reset_copy_flag();
      } else {
        barColor = value_to_color(val, main_array.size());
      }

      base_shape.setFillColor(barColor);

      window.draw(base_shape);
    }

    window.display();
  }

  return 0;
}

