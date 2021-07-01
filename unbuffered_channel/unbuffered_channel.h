#pragma once

#include <optional>


template <typename T>
class UnbufferedChannel{
 public:
  UnbufferedChannel() {
  }

  void Send(const T& /*value*/) {
    // Your code
  }

  std::optional<T> Recv() {
    // Your code
    return std::nullopt;
  }

  void Close() {
    // Your code
  }

 private:
  // Your code
};

