#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include <stack>
#include <vector>

int main() {
  std::stack<int> s;

  bool stop = false;
  std::mutex m;
  std::mutex cout_m;
  std::condition_variable cv;

  auto pusher = [&] {
    for (size_t i = 0; i != 10; ++i) {
      {
        std::unique_lock<std::mutex> lock(m);
        s.push(i);
        cv.notify_one();
      }
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    {
      std::unique_lock<std::mutex> lock(m);
      stop = true;
      cv.notify_all();
    }
  };

  auto poper = [&] {
    for (;;) {
      int x;
      {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [&] { return !s.empty() || stop; });
        if (s.empty() && stop) {
          break;
        }
        x = s.top();
        s.pop();
      }
      std::unique_lock<std::mutex> cout_lock(cout_m);
      std::cout << x << '\n';
    }
  };


  std::vector<std::thread> threads;
  threads.emplace_back(poper);
  threads.emplace_back(pusher);

  for (auto& t : threads) {
    t.join();
  }
}

