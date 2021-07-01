#pragma once

#include <vector>


template <typename T>
class ThreadSafeVector {
 public:
  ThreadSafeVector() {
  }

  T operator[](size_t index) const {
    // Your code
    return vector_[index];
  }

  size_t Size() const {
    // Your code
    return vector_.size();
  }

  void PushBack(const T& value) {
    // Your code
    vector_.push_back(value);
  }

 private:
  // Your code
  std::vector<T> vector_;
};

