#include "thread_safe_queue.h"

#include "timeout_guard.h"
#include <gtest/gtest.h>

#include <thread>
#include <vector>


TEST(ThreadSafeQueue, Simple) {
  TimeoutGuard guard(100ms);
  ThreadSafeQueue<int> q;
  q.Push(1);
  ASSERT_EQ(q.Pop(), 1);
  q.Push(2);
  q.Push(3);
  q.Push(4);
  ASSERT_EQ(q.Pop(), 2);
  ASSERT_EQ(q.Pop(), 3);
  ASSERT_EQ(q.Pop(), 4);
  ASSERT_EQ(q.TryPop(), std::nullopt);
}

TEST(ThreadSafeQueue, BlockingPop) {
  TimeoutGuard guard(500ms);
  ThreadSafeQueue<int> q;
  std::thread t([&] {
      std::this_thread::sleep_for(250ms);
      q.Push(1);
  });

  auto start = std::chrono::system_clock::now();
  ASSERT_EQ(q.Pop(), 1);
  auto end = std::chrono::system_clock::now();
  ASSERT_GE(end - start, 100ms);
  t.join();
}

TEST(ThreadSafeQueue, NonBlockingTryPop) {
  TimeoutGuard guard(500ms);
  ThreadSafeQueue<int> q;
  std::thread t([&] {
      std::this_thread::sleep_for(250ms);
      q.Push(1);
  });

  auto start = std::chrono::system_clock::now();
  ASSERT_EQ(q.TryPop(), std::nullopt);
  auto end = std::chrono::system_clock::now();
  ASSERT_LT(end - start, 50ms);

  t.join();
  auto ret = q.TryPop();
  ASSERT_NE(ret, std::nullopt);
  ASSERT_EQ(*ret, 1);
}

TEST(ThreadSafeQueue, ProducerConsumers) {
  const size_t kConsumers = 5;
  const size_t kIters = 100000;
  std::atomic<bool> error{false};

  TimeoutGuard guard(1s);

  ThreadSafeQueue<int> q;

  auto producer = [&] {
    for (size_t i = 0; i < kConsumers * kIters; ++i) {
      q.Push(i);
    }
  };

  auto consumer = [&] {
    size_t iter = 0;
    int prev_x = q.Pop();
    ++iter;
    while (iter < kIters) {
      int curr_x = q.Pop();
      if (curr_x < prev_x) {
        error.store(true);
      }
      prev_x = curr_x;
      ++iter;
    }
  };

  std::vector<std::thread> threads;
  std::vector<std::thread> consumers;
  for (size_t i = 0; i != kConsumers; ++i) {
    threads.emplace_back(consumer);
  }
  threads.emplace_back(producer);

  for (auto& t : threads) {
    t.join();
  }

  ASSERT_FALSE(error.load());
  ASSERT_EQ(q.TryPop(), std::nullopt);
}

TEST(ThreadSafeQueue, ProducersConsumers) {
  const size_t kProducers = 10;
  const size_t kConsumers = 10;
  const size_t kIters = 50000;

  TimeoutGuard guard(1s);

  ThreadSafeQueue<int> q;

  auto producer = [&] {
    for (size_t i = 0; i < kIters; ++i) {
      q.Push(i);
    }
  };

  auto consumer = [&] {
    size_t iter = 0;
    while (iter < kIters) {
      q.Pop();
      ++iter;
    }
  };

  std::vector<std::thread> threads;
  std::vector<std::thread> consumers;
  for (size_t i = 0; i != kConsumers; ++i) {
    threads.emplace_back(consumer);
  }
  for (size_t i = 0; i != kProducers; ++i) {
    threads.emplace_back(producer);
  }

  for (auto& t : threads) {
    t.join();
  }

  ASSERT_EQ(q.TryPop(), std::nullopt);
}

