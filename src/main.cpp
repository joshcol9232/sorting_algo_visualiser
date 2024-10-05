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

#include "constants.h"
#include "Beeper.h"
#include "StatArray.h"
#include "Element.h"
#include "BarDisparityVisual.h"
#include "BarVisual.h"
#include "BarDisparityVisual.h"
#include "Visualisation.h"
#include "sorting_algorithms.h"

#include "viridis_palette.h"

#define DEBUG


namespace {

sf::Color valueToColor(const float valRatio) {
  // Set base colour to viridis
  const size_t colorIndex = static_cast<size_t>(valRatio * static_cast<float>(VIRIDIS_PALETTE_LENGTH)) * 3 - 3;

  return sf::Color(static_cast<char>(VIRIDIS[colorIndex] * 255.0),
                   static_cast<char>(VIRIDIS[colorIndex + 1] * 255.0),
                   static_cast<char>(VIRIDIS[colorIndex + 2] * 255.0));
}

void chooseVisualisation(std::unique_ptr<Visualisation>& current) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
    current = std::make_unique<BarVisual>();
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    current = std::make_unique<BarDisparityVisual>();
  }
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

  Beeper beep;
  std::unique_ptr<Visualisation> visual = std::make_unique<BarDisparityVisual>();  // std::make_unique<BarVisual>();

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    // Process inputs
    if (window.hasFocus()) {
      chooseVisualisation(visual);

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
    }

    // Draw
    window.clear(sf::Color::White);

    for (size_t idx = 0; idx < mainArray.size(); ++idx) {
      Element<size_t>& element = mainArray.instantMutableAccess(idx);
      const float ratio = static_cast<float>(*element + 1) /
                          static_cast<float>(mainArray.size());

      // Set colour
      sf::Color elementColour = sf::Color::White;

      if (sorted) {
        elementColour = sf::Color::Green;
      } else if (mainArray.isActive(idx)) {
        elementColour = sf::Color::Magenta;
      } else if (element.justCopied()) {
        elementColour = sf::Color::Red;
        beep.beep(ratio);

        element.resetCopyFlag();
      } else {
        elementColour = valueToColor(ratio);
      }

      visual->render(window, { ratio, idx, mainArray.size(), elementColour });
    }

    window.display();
  }

  return 0;
}

