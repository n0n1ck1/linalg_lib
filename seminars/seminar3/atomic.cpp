#include <iostream>
#include <atomic>
#include <thread>



class Node {
};


int main() {
  std::atomic<uint64_t> x{0};
  std::atomic<Node*> tail{nullptr};

  std::cout << x.load() << tail << '\n';

  std::cout << (x += 1) << '\n';
  std::cout << x.fetch_add(1) << '\n';

  std::cout << x.exchange(6) << ' ' << x.load() <<'\n';


  uint64_t expected = x.load();

  // Other thread
  x.store(7);
  x.store(6);


  std::cout << "Expected: " << expected << '\n';
  // Some work
  if (x.compare_exchange_weak(expected, 12)) {
    std::cout << "Success " << expected << ' ' << x.load() << '\n';
  } else {
    std::cout << "Failure " << expected << ' ' << x.load() << '\n';
  }

  for (;;) {
    expected = x.load();
    if (expected & 1) {
      std::this_thread::yield();
      continue;
    }
    uint64_t desired = expected + 2;
    if (x.compare_exchange_weak(expected, desired)) {
      break;
    }
  }
}

