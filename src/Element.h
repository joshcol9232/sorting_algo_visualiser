#pragma once

#include <atomic>
#include "constants.h"


namespace {
  void copy() {
#ifdef DEBUG
    std::cout << "ELEMENT COPY" << std::endl;
#endif
    sf::sleep(constants::COPY_COST);
  }

  void compare() {
#ifdef DEBUG
    std::cout << "ELEMENT COMPARE" << std::endl;
#endif
    sf::sleep(constants::COMPARISON_COST);
  }
}

// Idea here is to wrap operators with sleeps

template<typename T>
class Element {
 public:
  Element() : inner_{} {}
  Element(T in) : inner_(in) {}
  Element(T&& in) : inner_(in) {}

  // Copy
  Element(const Element& other) :
    Element(other.inner_)
  {
    copy();
    just_copied_ = true;
  }
  Element& operator=(const Element& other) {
    copy();
    inner_ = other.inner_;
    just_copied_ = true;
    return *this;
  }

  // Used for rendering when an element is copied
  bool just_copied() const { return just_copied_.load(); }
  void reset_copy_flag() { just_copied_.store(false); }

  // Reading (no cost)
  const T& operator*() const { return inner_; }

  // Comparisons
  friend bool operator==(const Element& a, const Element& b) {
    compare();
    return a.inner_ == b.inner_;
  }
  friend bool operator!=(const Element& a, const Element& b) {
    compare();
    return a.inner_ != b.inner_;
  }
  friend bool operator<(const Element& a, const Element& b) {
    compare();
    return a.inner_ < b.inner_;
  }
  friend bool operator<=(const Element& a, const Element& b) {
    compare();
    return a.inner_ <= b.inner_;
  }
  friend bool operator>(const Element& a, const Element& b) {
    compare();
    return a.inner_ > b.inner_;
  }
  friend bool operator>=(const Element& a, const Element& b) {
    compare();
    return a.inner_ >= b.inner_;
  }

 private:
  T inner_;
  std::atomic_bool just_copied_;
};



