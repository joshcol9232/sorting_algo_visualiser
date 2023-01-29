#include "sortarray.h"

#include <numeric>
#include <random>
#include <iostream>
#include <stdexcept>

#include "constants.h"

/*
namespace {

float get_pitch(size_t value, size_t data_size) {
  return ((static_cast<float>(value)+1.0) /
           static_cast<float>(data_size)) *
          constants::PITCH_MULTIPLIER + 0.1;
}

}  // namespace
*/

SortArray::SortArray(size_t size) :
  data_(size),
  swapping_{-1, -1},
  accessing_(-1)
{
  std::iota(data_.begin(), data_.end(), 0);
}


void SortArray::shuffle() {
  std::random_device rd;
  std::mt19937 g(rd());

  std::shuffle(data_.begin(), data_.end(), g);
}

void SortArray::swap(const size_t a, const size_t b) {
  accessing_ = -1;
  swapping_[0] = a;
  swapping_[1] = b;
  sf::sleep(constants::SWAP_COST_DIV_2);
  // ...
  const size_t temp = data_[a];
  data_[a] = data_[b];
  data_[b] = temp;
  sf::sleep(constants::SWAP_COST_DIV_2);
}

const size_t& SortArray::operator[](const size_t idx) {
  swapping_[0] = -1;
  swapping_[1] = -1;
  accessing_ = idx;
  sf::sleep(constants::ACCESS_COST);
  // ...
  return data_[idx];
}

void SortArray::reset_metadata() {
  swapping_[0] = -1;
  swapping_[1] = -1;
  accessing_ = -1;
}

bool SortArray::is_sorted() const {
  // assume sorted until invalid order found
  for (size_t i = 0; i < data_.size()-1; ++i) {
    if (data_[i] > data_[i + 1]) {
      return false;
    }
  }

  return true;
}
