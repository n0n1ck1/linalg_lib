#include "mpsc_queue.h"

#include "timeout_guard.h"
#include <gtest/gtest.h>

#include <optional>
#include <thread>
#include <vector>


TEST(MPSCQueue, Simple) {
  TimeoutGuard guard(100ms);

  MPSCQueue<int> q;
  q.Push(1);
  ASSERT_EQ(q.Pop(), 1);
  ASSERT_EQ(q.Pop(), std::nullopt);
  q.Push(2);
  q.Push(3);
  q.Push(4);
  ASSERT_EQ(*q.Pop(), 4);
  ASSERT_EQ(*q.Pop(), 3);
  ASSERT_EQ(*q.Pop(), 2);
  ASSERT_EQ(q.Pop(), std::nullopt);
}

TEST(MPSCQueue, Destructor) {
  TimeoutGuard guard(100ms);

  MPSCQueue<int> q;
  q.Push(1);
  q.Push(2);
  q.Push(3);
  q.Push(4);
}

TEST(MPSCQueue, NonBlockingPop) {
  TimeoutGuard guard(500ms);
  MPSCQueue<int> q;
  std::thread t([&] {
      std::this_thread::sleep_for(250ms);
      q.Push(1);
  });

  auto start = std::chrono::system_clock::now();
  ASSERT_EQ(q.Pop(), std::nullopt);
  auto end = std::chrono::system_clock::now();
  ASSERT_LT(end - start, 50ms);

  t.join();
  auto ret = q.Pop();
  ASSERT_NE(ret, std::nullopt);
  ASSERT_EQ(*ret, 1);
}

TEST(MPSCQueue, SPSC) {
  const size_t kIters = 500000;

  TimeoutGuard guard(1s);

  MPSCQueue<int> q;
  std::set<int> dequeued;
  std::atomic<bool> stop{false};

  auto consumer = [&] {
    while (!stop.load()) {
      auto ret = q.Pop();
      if (ret) {
        dequeued.insert(*ret);
      }
    }
    auto ret = q.Pop();
    while (ret) {
      dequeued.insert(*ret);
      ret = q.Pop();
    }
  };

  auto producer = [&] {
    for (size_t i = 0; i != kIters; ++i) {
      q.Push(i);
    }
    stop.store(true);
  };

  std::vector<std::thread> threads;
  threads.emplace_back(consumer);
  threads.emplace_back(producer);

  for (auto& t : threads) {
    t.join();
  }

  ASSERT_EQ(q.Pop(), std::nullopt);
  int curr = 0;
  for (int x : dequeued) {
    ASSERT_EQ(curr, x);
    ++curr;
  }
}

TEST(MPSCQueue, MPSC) {
  const size_t kProducers = 3;
  const size_t kIters = 100000;

  TimeoutGuard guard(1s);

  MPSCQueue<int> q;
  std::set<int> dequeued;
  std::atomic<size_t> producers_stopped{0};

  auto producer = [&] (size_t producer_id) {
    for (size_t i = 0; i < kIters; ++i) {
      q.Push(i + producer_id * kIters);
    }
    ++producers_stopped;
  };

  auto consumer = [&] {
    while (producers_stopped.load() != kProducers) {
      auto ret = q.Pop();
      if (ret) {
        dequeued.insert(*ret);
      }
    }
    auto ret = q.Pop();
    while (ret) {
      dequeued.insert(*ret);
      ret = q.Pop();
    }
  };

  std::vector<std::thread> threads;
  for (size_t i = 0; i != kProducers; ++i) {
    threads.emplace_back(producer, i);
  }
  threads.emplace_back(consumer);

  for (auto& t : threads) {
    t.join();
  }

  ASSERT_EQ(q.Pop(), std::nullopt);
  int curr = 0;
  for (int x : dequeued) {
    ASSERT_EQ(curr, x);
    ++curr;
  }
}

