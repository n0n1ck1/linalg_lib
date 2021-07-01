#pragma once

#include <optional>


template <typename T>
class MPSCQueue {
 public:
  MPSCQueue() {
  }

  ~MPSCQueue() {
    // Your code
  }

  void Push(const T& /*value*/) {
    // Your code
  }

  std::optional<T> Pop() {
    return std::nullopt;
    // Your code
  }

 private:
  // Your code
};

