#pragma once

#include <condition_variable>
#include <mutex>


class Barrier {
 public:
  explicit Barrier(size_t n_threads)
    : n_threads_(n_threads)
  {
  }

  void Wait() {
    std::unique_lock lock(mutex_);
    ++arrived_;
    if (arrived_ == n_threads_) {
      cv_.notify_all();
    } else {
      cv_.wait(lock, [&] { return arrived_ == n_threads_; });
    }
  }

 private:
  std::mutex mutex_;
  std::condition_variable cv_;
  const size_t n_threads_;
  size_t arrived_ = 0;
};

