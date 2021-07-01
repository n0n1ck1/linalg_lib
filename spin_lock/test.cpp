#include "spin_lock.h"

#include "timeout_guard.h"
#include "barrier.h"
#include <gtest/gtest.h>

#include <atomic>
#include <thread>
#include <vector>


TEST(SpinLock, LockUnlock) {
  TimeoutGuard guard(100ms);
  SpinLock spin_lock;
  spin_lock.Lock();
  spin_lock.Unlock();
}

TEST(SpinLock, Locked) {
  TimeoutGuard guard(1s);
  SpinLock spin_lock;
  spin_lock.Lock();
  std::atomic<bool> aquired{false};
  std::thread t([&] {
      spin_lock.Lock();
      aquired.store(true);
  });

  std::this_thread::sleep_for(250ms);
  ASSERT_FALSE(aquired.load());
  spin_lock.Unlock();
  std::this_thread::sleep_for(250ms);
  ASSERT_TRUE(aquired.load());
  t.join();
}

TEST(SpinLock, Exclusive) {
  TimeoutGuard guard(2s);
  std::atomic<bool> critical{false};
  std::atomic<bool> error{false};

  const size_t kIters = 200;
  const size_t kThreads = 100;

  Barrier barrier(kThreads);
  SpinLock spin_lock;

  auto worker = [&] () {
    barrier.Wait();
    for (size_t i = 0; i != kIters; ++i) {
      spin_lock.Lock();
      if (critical.exchange(true, std::memory_order_relaxed)) {
        error.store(true, std::memory_order_relaxed);
      }
      std::this_thread::sleep_for(10ns);
      if (!critical.exchange(false, std::memory_order_relaxed)) {
        error.store(true, std::memory_order_relaxed);
      }
      spin_lock.Unlock();
      std::this_thread::sleep_for(30ns);
    }
  };

  std::vector<std::thread> threads;
  for (size_t i = 0; i != kThreads; ++i) {
    threads.emplace_back(worker);
  }

  for (auto& t : threads) {
    t.join();
  }

  ASSERT_FALSE(error.load());
}

TEST(SpinLock, Counter) {
  TimeoutGuard guard(1s);
  size_t counter = 0;
  const size_t kIters = 500000;
  const size_t kThreads = 10;

  Barrier barrier(kThreads);
  SpinLock spin_lock;

  auto incrementer = [&] () {
    barrier.Wait();
    for (size_t i = 0; i != kIters; ++i) {
      spin_lock.Lock();
      ++counter;
      spin_lock.Unlock();
    }
  };

  std::vector<std::thread> threads;
  for (size_t i = 0; i != kThreads; ++i) {
    threads.emplace_back(incrementer);
  }

  for (auto& t : threads) {
    t.join();
  }

  ASSERT_EQ(counter, kIters * kThreads);
}

