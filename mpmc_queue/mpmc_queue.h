#pragma once

#include <optional>
#include <cstdint>


template <class T>
class MPMCQueue {
 public:
  explicit MPMCQueue(uint64_t /*max_size*/) {
    // Your code
  }

  bool Push(const T& /*value*/) {
    // Your code
    return false;
  }

  std::optional<T> Pop() {
    // Your code
    return std::nullopt;
  }

 private:
  // Your code
};

