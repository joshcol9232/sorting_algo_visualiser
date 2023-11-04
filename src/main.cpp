#include <iostream>
#include <string>
#include <thread>
#include <memory>
#include <mutex>
#include <functional>
#include <random>
#include <numeric>


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "constants.h"
#include "sorting_algorithms.h"
#include "sort_array.h"

namespace {

float get_pitch(size_t value, size_t data_size) {
  return ((static_cast<float>(value)+1.0) /
           static_cast<float>(data_size)) *
          constants::PITCH_MULTIPLIER + 0.1;
}

}  // namespace

int main() {
  // setup
  sf::RenderWindow window(sf::VideoMode(constants::WINDOW_WIDTH,
                                        constants::WINDOW_HEIGHT),
                          "Sorting Visualiser");

//  window.setFramerateLimit(60);
  std::vector<size_t> base_data(constants::NUM_ELEMENTS, 0);
  std::iota(base_data.begin(), base_data.end(), 0);

  auto base = std::make_shared<SortArray>(std::move(base_data));
  std::mutex base_mutex;

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

  bool sorting = false;
  std::thread sorting_thread;

  sf::RectangleShape base_shape(sf::Vector2f(constants::BAR_STEP, 1.0f));

  auto run_sorting_thread = [&](std::function<void(std::shared_ptr<SortArray>)> F) {
    sorting = true;

    sorting_thread = std::thread([&](){
      F(base);
      // main_view.reset_metadata();
      sorting = false;
    });
    sorting_thread.detach();
  };

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    // Process inputs
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {         // Shuffle
      base->shuffle();
    } else if (!sorting && sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
      run_sorting_thread(bubble_sort);
    } else if (!sorting && sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
      run_sorting_thread(bogo_sort);
    } else if (!sorting && sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
      sorting = true;

      sorting_thread = std::thread([base, &sorting, &base_mutex](){
        quick_sort(base);
        // main_view.reset_metadata();
        sorting = false;
        
        {
          std::cout << "Assimilating..." << std::endl;
          std::lock_guard<std::mutex> base_lock(base_mutex);
          base->clear_children();
          //base->assimilate_children();
        }
      });
      sorting_thread.detach();
    }

    // Draw
    window.clear(sf::Color::Black);
    {
      std::lock_guard<std::mutex> base_lock(base_mutex);
      base->render(window, base_shape);
    }

    window.display();
  }

  return 0;
}

