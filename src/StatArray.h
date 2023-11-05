#pragma once

#include <ostream>
#include <vector>
#include <unordered_map>
#include <iterator>

#include <SFML/System.hpp>

#include "constants.h"

#include <chrono>
#include <thread>

namespace {
  // Functions that cause side effects when doing the following actions
  void access() {
    std::cout << "ACCESS" << std::endl;
    sf::sleep(constants::ACCESS_COST);
  }
  void compare() {
    std::cout << "COMPARE" << std::endl;
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
      parent_.active_[id_] = this;
    }
    ~Iterator() {
      // Remove from parent map
      parent_.active_.erase(id_);
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
      access();
      return *m_ptr_;
    }
    pointer operator->() {
      access();
      return m_ptr_;
    }
    reference operator[](difference_type offset) const {
      access();
      return m_ptr_[offset];
    }
    // ====================================

    // Increment/decrement
    Iterator& operator++() { m_ptr_++; return *this; }
    Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
    Iterator& operator--() { m_ptr_--; return *this; }
    Iterator operator--(int) { Iterator tmp = *this; --(*this); return tmp; }

    // Arithmetic
    Iterator& operator+=(difference_type offset) { m_ptr_ += offset; return this; }
    Iterator operator+(difference_type offset) const { return Iterator(parent_, m_ptr_ + offset); }
    friend Iterator operator+(difference_type offset, const Iterator& right) { return Iterator(right.parent_, offset + right.m_ptr_); }

    Iterator& operator-=(difference_type offset) { m_ptr_ -= offset; return this; }
    Iterator operator-(difference_type offset) const { return Iterator(parent_, m_ptr_ - offset); }
    difference_type operator-(const Iterator& right) const { return m_ptr_ - right.m_ptr_; }

    difference_type distance_from_start() const {
      // Get distance from beginning of list
      return m_ptr_ - (&parent_.data_[0]);
    }
    
    // === Comparison operators ===
    //  All have sleeps
    friend bool operator==(const Iterator& a, const Iterator& b) {
      compare();
      return a.m_ptr_ == b.m_ptr_;
    }
    friend bool operator!=(const Iterator& a, const Iterator& b) {
      compare();
      return a.m_ptr_ != b.m_ptr_;
    }
    friend bool operator<(const Iterator& a, const Iterator& b) {
      compare();
      return a.m_ptr_ < b.m_ptr_;
    }
    friend bool operator<=(const Iterator& a, const Iterator& b) {
      compare();
      return a.m_ptr_ <= b.m_ptr_;
    }
    friend bool operator>(const Iterator& a, const Iterator& b) {
      compare();
      return a.m_ptr_ > b.m_ptr_;
    }
    friend bool operator>=(const Iterator& a, const Iterator& b) {
      compare();
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
  StatArray(std::vector<T>&& v) : data_(v) {}

  T& operator[](size_t idx) {
    return *(begin() + idx);
  }

  const T& operator[](size_t idx) const {
    return *(cbegin() + idx);
  }

  size_t get_id() const { return id_; }

  void print_active() const {
    std::cout << "StatArray::print_active> Start." << std::endl;

    sf::sleep(constants::ACCESS_COST);
    sf::sleep(constants::ACCESS_COST);

    for (const auto& ele : active_) {
      std::cout << '\t' << ele.first << " : " << ele.second->distance_from_start() << std::endl;
    }
    std::cout << "StatArray::print_active> End." << std::endl;
  }


  Iterator begin() { return Iterator(*this, &data_[0]); }
  Iterator end() { return Iterator(*this, &data_[data_.size()]); }
  ConstIterator cbegin() const { return ConstIterator(&data_[0]); }
  ConstIterator cend() const { return ConstIterator(&data_[data_.size()]); }
  ConstIterator begin() const { return cbegin(); }
  ConstIterator end() const { return cend(); }

 private:
  size_t id_;
  std::vector<T> data_;
  // Map of currently active pointers
  std::unordered_map<size_t, Iterator const*> active_;
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

