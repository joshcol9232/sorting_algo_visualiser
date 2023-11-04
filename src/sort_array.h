#pragma once

#include <assert.h>
#include <memory>
#include <mutex>
#include <vector>

#include <SFML/Graphics.hpp>

#include "constants.h"

class SortArray {
 public:
  SortArray(std::vector<size_t>&& initial, std::shared_ptr<SortArray> parent, const size_t offset);
  SortArray(std::vector<size_t>&& initial, std::shared_ptr<SortArray> parent);

  // On destruction, write to parent.
  ~SortArray();

  void shuffle();

  size_t operator[](const size_t idx);
  void push_back(size_t element);
  void concatenate(const SortArray& other);
  void increment_offset();

  size_t size() const { return data_.size(); }
  bool is_sorted() const;

  void swap(const size_t a, const size_t b);

  // Allow const access to raw vector (rendering etc)
  const size_t& instant_access(const size_t idx) {
    std::lock_guard<std::mutex> data_lock(local_mutex_);
    return data_.at(idx);
  }

  std::shared_ptr<SortArray> make_empty_child(const size_t child_offset);

  void register_child(std::shared_ptr<SortArray> child) { children_.emplace_back(child); }
  // If array is currently being overwritten (has children)
  bool dirty() const { return children_.size() > 0; }
  void clear_all_but_first_child();
  std::shared_ptr<SortArray> first_child() { return children_[0]; }
  void clear_children() { children_.clear(); }

  void render(sf::RenderWindow& window, sf::RectangleShape& base_shape);

 private:
  bool root_;
  // Global index
  size_t offset_;
  std::mutex local_mutex_;

  // underlying data
  std::vector<size_t> data_;
  // Any arrays spawned under this one.
  std::vector<std::shared_ptr<SortArray>> children_;
};

