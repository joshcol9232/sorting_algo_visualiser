#include "sortarray.h"

#include <numeric>
#include <random>
#include <iostream>
#include <stdexcept>

#include "constants.h"
#include "sorting_algos/bubble.h"

namespace {

float get_pitch(size_t value, size_t data_size) {
  return ((static_cast<float>(value)+1.0) /
           static_cast<float>(data_size)) *
          constants::PITCH_MULTIPLIER + 0.1;
}

}  // namespace

SortArray::SortArray(size_t size) :
  algo_(new Bubble()),
  data_(size),
  bar_width_(static_cast<float>(constants::WINDOW_WIDTH) /
             static_cast<float>(size)),
  sorted_(true),
  do_sort_(false)
{
  std::iota(data_.begin(), data_.end(), 0);
}

void SortArray::step() {
  if (!sorted_ && do_sort_) {
    algo_->perform_swaps(data_);
    sorted_ = algo_->next(data_);

    if (sorted_) {  // Finished sorting
      do_sort_ = false;
      algo_->reset_vecs();
    }
  }
}

void SortArray::sound(sf::Sound& beep1, sf::Sound& beep2) const {
  const std::vector<size_t>& swapping = algo_->get_swapping();

  for (size_t i = 0; i+1 < swapping.size(); i += 2) {
    beep1.setPitch(get_pitch(data_[swapping[i]], data_.size()));
    beep1.play();
    beep2.setPitch(get_pitch(data_[swapping[i+1]], data_.size()));
    beep2.play();
  }
}

void SortArray::draw(sf::RenderWindow& window) const {
  sf::RectangleShape base_shape(sf::Vector2f(bar_width_, 1.0f));

  const float bar_step = static_cast<float>(constants::WINDOW_WIDTH) /
                         data_.size();

  const std::vector<size_t>& swapping = algo_->get_swapping();
  const std::vector<size_t>& comparing = algo_->get_comparing();

  float y_size;
  size_t idx(0);

  for (const auto& val : data_) {
    y_size = (static_cast<float>(val+1) /
              static_cast<float>(data_.size())) *
             static_cast<float>(constants::WINDOW_HEIGHT);

    // Set position and size
    base_shape.setSize(sf::Vector2f(bar_width_, y_size));
    base_shape.setPosition(bar_step * idx, constants::WINDOW_HEIGHT - y_size);

    // Set colour
    base_shape.setFillColor(sf::Color::White);

    if (sorted_) {
      base_shape.setFillColor(sf::Color::Green);
    } else {
      /*  // DEBUGGING
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

      if (std::find(swapping.begin(), swapping.end(), idx) != swapping.end()) {
        base_shape.setFillColor(sf::Color::Red);
      } else if (std::find(comparing.begin(), comparing.end(), idx) != comparing.end()) {
        base_shape.setFillColor(sf::Color::Cyan);
      }
    }

    window.draw(base_shape);

    ++idx;
  }
}

void SortArray::shuffle() {
  std::random_device rd;
  std::mt19937 g(rd());

  std::shuffle(data_.begin(), data_.end(), g);
  sorted_ = false;
}

void SortArray::cancel_sort() {
  do_sort_ = false;
  algo_->reset_vecs();
}

void SortArray::sort(SortAlgo* algo) {
  algo_.reset(algo);
  if (!sorted_) {
    do_sort_ = true;
  }
}
