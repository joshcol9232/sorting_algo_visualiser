#include <iostream>
#include <random>
#include <stdexcept>

#include "sort_array.h"


SortArray::SortArray(std::vector<size_t>&& initial, const size_t offset) :
  data_(initial), offset_(offset)
{}

SortArray::SortArray(std::vector<size_t>&& initial) :
  SortArray(std::move(initial), 0)
{}


size_t SortArray::operator[](const size_t idx) {
  assert(idx < size());
  //swapping_[0] = -1;
  //swapping_[1] = -1;
  //accessing_ = idx;
  sf::sleep(constants::ACCESS_COST);
  // ...
  std::lock_guard<std::mutex> data_lock(local_mutex_);
  return data_[idx];
}

void SortArray::concatenate(const SortArray& other) {
  std::lock_guard<std::mutex> data_lock(local_mutex_);
  data_.insert(data_.end(), other.data_.begin(), other.data_.end());
}

void SortArray::push_back(const size_t element) {
  assert(offset_ + size() < constants::NUM_ELEMENTS);
  sf::sleep(constants::ACCESS_COST);
  // ...
  std::lock_guard<std::mutex> data_lock(local_mutex_);
  data_.push_back(element);
}

void SortArray::shuffle() {
  std::random_device rd;
  std::mt19937 g(rd());

  std::lock_guard<std::mutex> data_lock(local_mutex_);
  std::shuffle(data_.begin(), data_.end(), g);
  // Call shuffle in child objects.
  for (auto child : children_) {
    child->shuffle();
  }
}

void SortArray::swap(const size_t a, const size_t b) {
  assert(a != b);
  assert(a >= 0 && a < data_.size());
  assert(b >= 0 && b < data_.size());

  //accessing_ = -1;
  //swapping_[0] = a;
  //swapping_[1] = b;
  //swap_change_ = true;
  sf::sleep(constants::SWAP_COST_DIV_2);
  // ...
  {
    std::lock_guard<std::mutex> data_lock(local_mutex_);
    const size_t temp = data_[a];
    data_[a] = data_[b];
    data_[b] = temp;
  }
  sf::sleep(constants::SWAP_COST_DIV_2);
}

bool SortArray::is_sorted() const {
  // assume sorted until invalid order found
  for (size_t i = 0; i < size()-1; ++i) {
    if (data_[i] > data_[i + 1]) {
      return false;
    }
  }

  return true;
}

void SortArray::increment_offset() {
  assert(size() == 0 || offset_ + 1 < constants::NUM_ELEMENTS);
  offset_++;
}


void SortArray::render(sf::RenderWindow& window, sf::RectangleShape& base_shape) {
  std::lock_guard<std::mutex> data_lock(local_mutex_);
  if (children_.size() > 0) {
    for (auto child : children_) {
      child->render(window, base_shape);
    }
  } else if (size() > 0) {
    const bool sorted = is_sorted();

    for (size_t idx = 0; idx < size(); ++idx) {
      const size_t val = data_[idx];
      const float y_size = (static_cast<float>(val+1) /
                            static_cast<float>(constants::NUM_ELEMENTS)) *
                            static_cast<float>(constants::WINDOW_HEIGHT);

      // Set position and size
      base_shape.setSize(sf::Vector2f(constants::BAR_STEP, y_size));
      base_shape.setPosition(constants::BAR_STEP * (idx + offset_), constants::WINDOW_HEIGHT - y_size);

      // Set colour
      base_shape.setFillColor(sf::Color::White);

      if (sorted) {
        base_shape.setFillColor(sf::Color::Green);
      } /* else {
        const int* swapping = main_view.get_swapping();
        int val_swapped = -1;
        if (idx == swapping[0]) {
          val_swapped = 0;
        } else if (idx == swapping[1]) {
          val_swapped = 1;
        }

        if (val_swapped > 0) {
          base_shape.setFillColor(sf::Color::Red);

          // AUDIO
          if (main_view.get_swap_change()) {  // If swap has changed, play a sound
            beep1.setPitch(get_pitch(val, main_view.size()));
            if (val_swapped == 0) {
              beep2.setPitch(get_pitch(swapping[1], main_view.size()));
            } else {
              beep2.setPitch(get_pitch(swapping[0], main_view.size()));
            }

            beep1.play();
            beep2.play();
            main_view.reset_swap_change();
          }
          // -----
        } else {
          const int acc = main_view.get_accessing();
          if (idx == acc) {
            base_shape.setFillColor(sf::Color::Cyan);
          }
        }
      } */

      window.draw(base_shape);
    }
  }
}

std::shared_ptr<SortArray> SortArray::make_empty_child(const size_t child_offset) {
  assert(child_offset < size());
  assert(offset_ + child_offset < constants::NUM_ELEMENTS);

  auto child = std::make_shared<SortArray>(std::vector<size_t>{}, offset_ + child_offset);
  children_.emplace_back(child);
  return child;
}

void SortArray::clear_all_but_first_child() {
  if (children_.size() > 1) {
    children_.erase(children_.begin() + 1, children_.end());
  }
}

void SortArray::assimilate_children() {
  std::lock_guard<std::mutex> data_lock(local_mutex_);
  for (auto child : children_) {
    child->assimilate_children();
    std::cout << "Assimilating child at offset: " << child->offset_ + offset_ << " -> " << offset_ + child->offset_ + child->size() << std::endl;
    size_t child_idx = 0;
    for (size_t idx = child->offset_; idx < child->offset_ + child->size(); ++idx) {
      data_[idx] = child->data_[child_idx];
      child_idx++;
    }
  }
  children_.clear();
}

