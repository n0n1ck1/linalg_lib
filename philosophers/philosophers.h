#pragma once

#include <mutex>


class Fork {
 public:
  Fork(size_t id) : id_(id) {
  }

  size_t Id() const {
    return id_;
  }

  void Get() {
    mutex_.lock();
  }

  bool TryGet() {
    return mutex_.try_lock();
  }

  void Put() {
    mutex_.unlock();
  }

 private:
  size_t id_;
  std::mutex mutex_;
};

class Philosopher {
 public:
  Philosopher(size_t /*id*/, Fork* /*left_fork*/, Fork* /*right_fork*/) {
    // Your code
  }

  size_t Id() const {
    // Your code
    return -1;
  }

  void Eat() {
    // Your code
  }

  void Think() {
    // Your code
  }

 private:
  // Your code
};

