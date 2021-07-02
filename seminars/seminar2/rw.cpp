#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <shared_mutex>
#include <vector>

int main() {
  std::shared_mutex sm;


  size_t count = 0;

  auto writer = [&] {
    for (size_t i = 0; i != 1000; ++i) {
      {
        std::unique_lock<std::shared_mutex> lock(sm);
        ++count;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  };

  auto reader = [&] {
    size_t count_count = 0;
    for (size_t i = 0; i != 100000; ++i) {
      size_t x;
      {
        std::shared_lock<std::shared_mutex> lock(sm);
        // sm.lock_shared();
        x = count;
      }
      count_count += x;
    }
    std::cout << count_count << '\n';
  };

  std::vector<std::thread> threads;
  threads.emplace_back(reader);
  threads.emplace_back(reader);
  threads.emplace_back(reader);
  threads.emplace_back(writer);

  for (auto& t : threads) {
    t.join();
  }
}

