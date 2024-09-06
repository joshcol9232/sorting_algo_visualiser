#pragma once

#include <iostream>
#include <ostream>
#include <random>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <mutex>

#include <SFML/System.hpp>

#include "constants.h"

#include <chrono>
#include <thread>

namespace {
  // Functions that cause side effects when doing the following actions
  void access(const int idx) {
#ifdef DEBUG
    std::cout << "ACCESS: (" << idx << ")" << std::endl;
#endif

    if (constants::runtime::disableSleeps() == false)
      sf::sleep(constants::ACCESS_COST);
  }
  void compare(const int idxA, const int idxB) {
#ifdef DEBUG
    std::cout << "COMPARE: (" << idxA << ", " << idxB << ")" << std::endl;
#endif

    if (constants::runtime::disableSleeps() == false)
      sf::sleep(constants::COMPARISON_COST);
  }
}

// Array with additional statistics, and imposed slower access times.
template<typename T>
class StatArray {
 public:
  // -- Iterator --
  class Iterator {
   public:
    using value_type = T;
    using reference = value_type&;
    using pointer = value_type*;
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;

    Iterator(StatArray& parent, pointer ptr) :
      parent_(parent), m_ptr_(ptr)
    {
      static size_t sLastID_ = 0;
      id_ = sLastID_++;
      parent_.insertActive(id_, this);
    }
    ~Iterator() {
      // Remove from parent map
      parent_.eraseActive(id_);
    }
    Iterator(const Iterator& other) :
      Iterator(other.parent_, other.m_ptr_)
    {}

    Iterator& operator=(const Iterator& other) {
      parent_ = other.parent_;
      m_ptr_ = other.m_ptr_;
      return *this;
    }

    // Free accessor for internal use.
    pointer get_ptr() const { return m_ptr_; }

    // === ACCESSORS - all have a sleep ===
    reference operator*() const {
      access(distanceFromStart());
      return *m_ptr_;
    }
    pointer operator->() {
      access(distanceFromStart());
      return m_ptr_;
    }
    reference operator[](difference_type offset) const {
      access(distanceFromStart());
      return m_ptr_[offset];
    }
    // ====================================

    // Increment/decrement
    Iterator& operator++() { m_ptr_++; return *this; }
    Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
    Iterator& operator--() { m_ptr_--; return *this; }
    Iterator operator--(int) { Iterator tmp = *this; --(*this); return tmp; }

    // Arithmetic
    Iterator& operator+=(difference_type offset) { m_ptr_ += offset; return *this; }
    Iterator operator+(difference_type offset) const { return Iterator(parent_, m_ptr_ + offset); }
    friend Iterator operator+(difference_type offset, const Iterator& right) { return Iterator(right.parent_, offset + right.m_ptr_); }

    Iterator& operator-=(difference_type offset) { m_ptr_ -= offset; return *this; }
    Iterator operator-(difference_type offset) const { return Iterator(parent_, m_ptr_ - offset); }
    difference_type operator-(const Iterator& right) const { return m_ptr_ - right.m_ptr_; }

    difference_type distanceFromStart() const {
      // Get distance from beginning of list
      return m_ptr_ - (&parent_.data_[0]);
    }
    
    // === Comparison operators ===
    friend bool operator==(const Iterator& a, const Iterator& b) {
      return a.m_ptr_ == b.m_ptr_;
    }
    friend bool operator!=(const Iterator& a, const Iterator& b) {
      return a.m_ptr_ != b.m_ptr_;
    }
    friend bool operator<(const Iterator& a, const Iterator& b) {
      return a.m_ptr_ < b.m_ptr_;
    }
    friend bool operator<=(const Iterator& a, const Iterator& b) {
      return a.m_ptr_ <= b.m_ptr_;
    }
    friend bool operator>(const Iterator& a, const Iterator& b) {
      return a.m_ptr_ > b.m_ptr_;
    }
    friend bool operator>=(const Iterator& a, const Iterator& b) {
      return a.m_ptr_ >= b.m_ptr_;
    }
    // ============================

   private:
    size_t id_;
    pointer m_ptr_;

    StatArray<T>& parent_;
  };

  struct ConstIterator {
    using iterator_catergory = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = const T;
    using pointer = const T*;
    using reference = const T&;

    ConstIterator(pointer ptr) : m_ptr_(ptr) {}

    const reference operator*() const { return *m_ptr_; }
    const pointer operator->() const { return m_ptr_; }

    ConstIterator& operator++() { m_ptr_++; return *this; }
    ConstIterator operator++(int) { ConstIterator tmp = *this; ++(*this); return tmp; }

    friend bool operator==(const ConstIterator& a, const ConstIterator& b) { return a.m_ptr_ == b.m_ptr_; }
    friend bool operator!=(const ConstIterator& a, const ConstIterator& b) { return a.m_ptr_ != b.m_ptr_; }

   private:
    pointer m_ptr_;
  };
  // -------------

  StatArray() :
    data_{}, active_{}
  {
    static size_t sLastID = 0;
    id_ = sLastID++;
  }
  explicit StatArray(const std::vector<T>& v) : data_(v) {}

  explicit StatArray(const size_t size) {
    data_.reserve(size);
    for (size_t idx = 0; idx < size; ++idx) {
      grow();
    }
  }

  StatArray(const StatArray<T>& other) : StatArray(other.data_) {}

  StatArray<T>& operator=(const StatArray<T>& other) {
    data_ = other.data_;
    return *this;
  }

  T& operator[](size_t idx) {
    return *(begin() + idx);
  }

  const T& operator[](size_t idx) const {
    return *(cbegin() + idx);
  }

  bool isActive(const size_t idx) {
    std::lock_guard<std::mutex> dataLock(activeMapMtx_);
    for (const auto& ele : active_) {
      if (ele.second->distanceFromStart() == idx) {
        return true;
      }
    }
    return false;
  }

  size_t size() const { return data_.size(); }

  void grow() {
    data_.emplace_back(size());
  }
  void shrink() {
    data_.erase(std::remove(data_.begin(), data_.end(), T(size() - 1)), data_.end());
  }

  const T& instantImmutableAccess(size_t idx) const { return data_[idx]; }
  T& instantMutableAccess(size_t idx) { return data_[idx]; }

  void shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(begin(), end(), g);
  }

  Iterator begin() { return Iterator(*this, &data_[0]); }
  Iterator end() { return Iterator(*this, &data_[data_.size()]); }
  ConstIterator cbegin() const { return ConstIterator(&data_[0]); }
  ConstIterator cend() const { return ConstIterator(&data_[data_.size()]); }
  ConstIterator begin() const { return cbegin(); }
  ConstIterator end() const { return cend(); }

 private:
  size_t getID() const { return id_; }

  void insertActive(const size_t subID, Iterator const* sub) {
    std::lock_guard<std::mutex> dataLock(activeMapMtx_);
    active_[subID] = sub;
  }

  void eraseActive(const size_t subID) {
    std::lock_guard<std::mutex> dataLock(activeMapMtx_);
    active_.erase(subID);
  }

  void printActive() {
    std::cout << "StatArray::printActive> Start." << std::endl;

    std::lock_guard<std::mutex> dataLock(activeMapMtx_);
    for (const auto& ele : active_) {
      std::cout << '\t' << ele.first << " : " << ele.second->distanceFromStart() << std::endl;
    }
    std::cout << "StatArray::printActive> End." << std::endl;
  }

  size_t id_;
  std::vector<T> data_;
  // Map of currently active pointers
  std::unordered_map<size_t, Iterator const*> active_;
  std::mutex activeMapMtx_;
};

template<typename T>
std::ostream& operator<<(std::ostream &os, const StatArray<T>& data) {
  os << "{";
  for (const T& element : data) {
    os << element << ", ";
  }
  os << "}";
  return os;
}

