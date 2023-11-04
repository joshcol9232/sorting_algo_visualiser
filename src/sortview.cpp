#include "sortview.h"

#include <random>
#include <iostream>
#include <stdexcept>

#include "constants.h"

namespace {

float get_pitch(size_t value, size_t data_size) {
  return ((static_cast<float>(value)+1.0) /
           static_cast<float>(data_size)) *
          constants::PITCH_MULTIPLIER + 0.1;
}

}  // namespace

SortView::SortView(std::shared_ptr<std::vector<size_t>> data, size_t start, size_t end) :
  data_(data),
  start_(start), end_(end)
{
//  std::iota(data_.begin(), data_.end(), 0);

}

void SortView::shuffle() {
  std::random_device rd;
  std::mt19937 g(rd());

  std::lock_guard<std::mutex> data_lock(local_mutex_);
  std::shuffle(data_->begin() + start_, data_->begin() + end_, g);
}

void SortView::swap(const size_t a, const size_t b) {
  assert(a != b);
  assert(start_ + b < end_);

  accessing_ = -1;
  swapping_[0] = a;
  swapping_[1] = b;
  swap_change_ = true;
  sf::sleep(constants::SWAP_COST_DIV_2);
  // ...
  {
    std::lock_guard<std::mutex> data_lock(local_mutex_);
    const size_t temp = data_->at(start_ + a);
    data_->at(start_ + a) = data_->at(start_ + b);
    data_->at(start_ + b) = temp;
  }
  sf::sleep(constants::SWAP_COST_DIV_2);
}

const size_t& SortView::operator[](const size_t idx) {
  assert(idx < end_);
  swapping_[0] = -1;
  swapping_[1] = -1;
  accessing_ = idx;
  sf::sleep(constants::ACCESS_COST);
  // ...
  std::lock_guard<std::mutex> data_lock(local_mutex_);
  return data_->at(start_ + idx);
}

void SortView::reset_metadata() {
  swapping_[0] = -1;
  swapping_[1] = -1;
  accessing_ = -1;
}

bool SortView::is_sorted() const {
  // assume sorted until invalid order found
  for (size_t i = 0; i < size()-1; ++i) {
    if (data_->at(start_ + i) > data_->at(start_ + i + 1)) {
      return false;
    }
  }

  return true;
}

SortView SortView::get_slice(const size_t rel_start, const size_t rel_end) const {
  std::cout << "MAKING SLICE WITH: " << start_ + rel_start << ", " << start_ + rel_end << std::endl;
  assert(start_ + rel_end < data_->size());
  assert(start_ + rel_start <= start_ + rel_end);  // Can be of size 0
  assert(start_ + rel_start >= 0);
  return SortView(data_, start_ + rel_start, start_ + rel_end);
}

