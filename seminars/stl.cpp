#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>


struct S {
  int64_t x;
  int64_t y;
  int64_t z;
};

int main() {
  size_t kIters = 1000000;

  const size_t kThreads = 4;

  size_t count = 0;

  std::mutex m;

  std::atomic<int> x;
  std::atomic<S> y;
  std::cout << x.is_lock_free() << '\n';
  std::cout << y.is_lock_free() << '\n';


  std::vector<std::thread> threads;
  for (size_t i = 0; i != kThreads; ++i) {
    threads.emplace_back([&] {
      for (size_t j = 0; j != kIters; ++j) {
        // Do some work
        ++count;
        // Do some work
      }
    });
  }

  for (auto& t : threads) {
    t.join();
  }

  std::cout << count << " == " << kIters * kThreads << " ? " << '\n';
}

