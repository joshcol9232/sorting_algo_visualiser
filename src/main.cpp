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

#define DEBUG


namespace {

float getPitch(const float valRatio, size_t dataSize) {
  return valRatio * constants::PITCH_MULTIPLIER + 0.1;
}

sf::Color valueToColor(const float valRatio, const size_t dataSize) {
  // Set base colour to viridis
  const size_t colorIndex = static_cast<size_t>(valRatio * static_cast<float>(VIRIDIS_PALETTE_LENGTH)) * 3 - 3;

  return sf::Color(static_cast<char>(VIRIDIS[colorIndex] * 255.0),
                   static_cast<char>(VIRIDIS[colorIndex + 1] * 255.0),
                   static_cast<char>(VIRIDIS[colorIndex + 2] * 255.0));
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

  std::cout <<"making" << std::endl;

  ArrayType mainArray(constants::NUM_ELEMENTS);
  std::cout <<"LOADED" << std::endl;

  // ------ Load sound ------

  sf::SoundBuffer sndBuff;
  if (!sndBuff.loadFromFile(constants::SOUND_FILE)) {
    throw std::runtime_error("Could not load sound.");
  }
  sf::Sound beep1;
  beep1.setBuffer(sndBuff);

  // ----------------------------

  bool sorted = true;
  bool sorting = false;
  std::thread sortingThread;

  auto runSortingThread = [&](const std::function<void(IteratorType, IteratorType)>& F) {
    // Join previous thread
    if (sortingThread.joinable()) { sortingThread.join(); }

    sorting = true;

    sortingThread = std::thread([&]() {
      F(mainArray.begin(), mainArray.end());
      sorting = false;
      sorted = true;
    });
    //sortingThread.detach();
  };

  auto nonSort = [&](const std::function<void()>& F) {
    if (sortingThread.joinable()) { sortingThread.join(); }

    sorting = true;
    sorted = false;

    sortingThread = std::thread([&]() {
      F();
      sorting = false;
    });
  };

  float ySize;
  sf::RectangleShape baseShape(sf::Vector2f(static_cast<float>(constants::WINDOW_WIDTH) /
                                             mainArray.size(), 1.0f));

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
        nonSort([&]() { mainArray.shuffle(); });
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        std::cout << "REVERSING" << std::endl;
        nonSort([&]() { std::reverse(mainArray.begin(), mainArray.end()); });
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add) ||
                 sf::Keyboard::isKeyPressed(sf::Keyboard::RBracket)) {
        std::cout << "+" << std::endl;
        const bool sortTmp = sorted;
        nonSort([&]() { mainArray.grow(); });
        sorted = sortTmp;
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract) ||
                 sf::Keyboard::isKeyPressed(sf::Keyboard::LBracket)) {
        std::cout << "-" << std::endl;
        const bool sortTmp = sorted;
        nonSort([&]() { mainArray.shrink(); });
        sorted = sortTmp;
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        std::cout << "REVERSING" << std::endl;
        nonSort([&]() { std::reverse(mainArray.begin(), mainArray.end()); });
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
        runSortingThread(std::sort<IteratorType>);
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
        runSortingThread(bubbleSort<IteratorType>);
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
        runSortingThread(bogoSort<IteratorType>);
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
        runSortingThread(quicksort<IteratorType>);
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
        runSortingThread(quicksortMultithreaded<IteratorType>);
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) {
        runSortingThread(mergeSortInPlace<IteratorType>);
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)) {
        runSortingThread(mergeSortInPlaceMultithreaded<IteratorType>);
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9)) {
        runSortingThread(std::min_element<IteratorType>);
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)) {
        runSortingThread(heapSort<IteratorType>);
      }
    }

    // Draw
    window.clear(sf::Color::Black);

    const float barStep = static_cast<float>(constants::WINDOW_WIDTH) /
                           mainArray.size();

    size_t numBeeps = 0;

    for (size_t idx = 0; idx < mainArray.size(); ++idx) {
      Element<size_t>& element = mainArray.instantMutableAccess(idx);
      const float ratio = static_cast<float>(*element + 1) /
                          static_cast<float>(mainArray.size());
      ySize = ratio * static_cast<float>(constants::WINDOW_HEIGHT);

      // Set position and size
      baseShape.setSize(sf::Vector2f(barStep, ySize));
      baseShape.setPosition(barStep * idx, constants::WINDOW_HEIGHT - ySize);

      // Set colour
      sf::Color barColor = sf::Color::White;

      if (sorted) {
        barColor = sf::Color::Green;
      } else if (mainArray.isActive(idx)) {
        barColor = sf::Color::Cyan;
      } else if (element.justCopied()) {
        barColor = sf::Color::Red;

        if (numBeeps < constants::MAX_BEEPS_PER_FRAME) {
          beep1.setPitch(getPitch(ratio, mainArray.size()));
          beep1.play();
          numBeeps++;
        }

        element.resetCopyFlag();
      } else {
        barColor = valueToColor(ratio, mainArray.size());
      }

      baseShape.setFillColor(barColor);

      window.draw(baseShape);
    }

    window.display();
  }

  return 0;
}

