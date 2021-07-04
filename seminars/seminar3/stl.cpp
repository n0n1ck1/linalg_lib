#include <condition_variable>
#include <cstdio>
#include <mutex>
#include <thread>
#include <iostream>
#include <future>
#include <vector>

int f(int x) {
  return 2 * x;
}

// std::latch
class Barrier {
 public:
  Barrier(size_t n_threads) : n_threads_(n_threads) {
  }

  void Wait() {
    std::unique_lock<std::mutex> lock(mutex_);
    arrived_ += 1;
    if (arrived_ == n_threads_) {
      do_wake_ = true;
      arrived_ = 0;
      cv_.notify_all();
      return;
    }
    cv_.wait(lock, [&] { return do_wake_; });
    cv_.notify_all();
  }

 private:
  const size_t n_threads_;
  bool do_wake_ = false;
  size_t arrived_ = 0;
  std::mutex mutex_;
  std::condition_variable cv_;
};


int main() {
  Barrier barrier(4);
  auto work = [&] {
    std::printf("Before\n");
    barrier.Wait();
    std::printf("Inter\n");
    barrier.Wait();
    std::printf("After\n");
  };

  std::vector<std::thread> threads;
  for (size_t i = 0; i != 4; ++i) {
    threads.emplace_back(work);
  }

  for (auto& t : threads) {
    t.join();
  }
}

