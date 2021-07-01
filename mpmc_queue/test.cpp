#include "mpmc_queue.h"

#include "timeout_guard.h"
#include "barrier.h"
#include <gtest/gtest.h>

#include <optional>
#include <thread>
#include <vector>


TEST(MPMCQueue, Correctness) {
  TimeoutGuard guard(100ms);

  MPMCQueue<int> q(2);
  ASSERT_EQ(q.Pop(), std::nullopt);
  ASSERT_EQ(q.Pop(), std::nullopt);
  ASSERT_TRUE(q.Push(1));
  ASSERT_TRUE(q.Push(2));
  ASSERT_FALSE(q.Push(3));
  ASSERT_EQ(*q.Pop(), 1);
  ASSERT_EQ(*q.Pop(), 2);
  ASSERT_EQ(q.Pop(), std::nullopt);
  ASSERT_EQ(q.Pop(), std::nullopt);
}

TEST(MPMCQueue, OneGen) {
  TimeoutGuard guard(2s);

  const size_t kSize = 1024 * 1024;
  const size_t kThreads = 4;
  MPMCQueue<int> q(kSize * kThreads);
  std::atomic<bool> error{false};

  auto writer = [&] {
    for (size_t i = 0; i < kSize; ++i) {
      if (!q.Push(42)) {
        error.store(true);
      }
    }
  };

  auto reader = [&] {
    for (size_t i = 0; i != kSize; ++i) {
      auto ret = q.Pop();
      if (!ret || *ret != 42) {
        error.store(true);
      }
    }
  };

  std::vector<std::thread> writers;
  for (size_t i = 0; i != kThreads; i++) {
    writers.emplace_back(writer);
  }

  for (auto& t : writers) {
    t.join();
  }

  ASSERT_FALSE(error.load());

  std::vector<std::thread> readers;
  for (size_t i = 0; i != kThreads; i++) {
    readers.emplace_back(reader);
  }

  for (auto& t : readers) {
    t.join();
  }

  ASSERT_FALSE(error.load());
}

TEST(MPMCQueue, NoLock) {
  TimeoutGuard guard(1s);

  const size_t kSize = 1024 * 1024;
  const size_t kThreads = 4;
  MPMCQueue<int> q(64);
  Barrier barrier(2 * kThreads);

  auto writer = [&] {
    barrier.Wait();
    for (size_t i = 0; i < kSize; ++i) {
      q.Push(42);
    }
  };

  auto reader = [&] {
    barrier.Wait();
    for (size_t i = 0; i != kSize; ++i) {
      q.Pop();
    }
  };

  std::vector<std::thread> threads;
  for (size_t i = 0; i != kThreads; i++) {
    threads.emplace_back(writer);
  }
  for (size_t i = 0; i != kThreads; i++) {
    threads.emplace_back(reader);
  }

  for (auto& t : threads) {
    t.join();
  }

  while (q.Pop()) {
  }

  ASSERT_EQ(q.Pop(), std::nullopt);
  ASSERT_TRUE(q.Push(17));
  ASSERT_EQ(*q.Pop(), 17);
  ASSERT_EQ(q.Pop(), std::nullopt);
}

