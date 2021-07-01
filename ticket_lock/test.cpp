#include "ticket_lock.h"

#include "timeout_guard.h"
#include "barrier.h"
#include <gtest/gtest.h>

#include <atomic>
#include <thread>
#include <vector>


TEST(TicketLock, LockUnlock) {
  TimeoutGuard guard(100ms);
  TicketLock ticket_lock;
  ticket_lock.Lock();
  ticket_lock.Unlock();
}

TEST(TicketLock, Locked) {
  TimeoutGuard guard(1s);
  TicketLock ticket_lock;
  ticket_lock.Lock();
  std::atomic<bool> aquired{false};
  std::thread t([&] {
      ticket_lock.Lock();
      aquired.store(true);
  });

  std::this_thread::sleep_for(250ms);
  ASSERT_FALSE(aquired.load());
  ticket_lock.Unlock();
  std::this_thread::sleep_for(250ms);
  ASSERT_TRUE(aquired.load());
  t.join();
}

TEST(TicketLock, Exclusive) {
  TimeoutGuard guard(2s);
  std::atomic<bool> critical{false};
  std::atomic<bool> error{false};

  const size_t kIters = 100;
  const size_t kThreads = 100;

  Barrier barrier(kThreads);
  TicketLock ticket_lock;

  auto worker = [&] () {
    barrier.Wait();
    for (size_t i = 0; i != kIters; ++i) {
      ticket_lock.Lock();
      if (critical.exchange(true, std::memory_order_relaxed)) {
        error.store(true, std::memory_order_relaxed);
      }
      std::this_thread::sleep_for(10ns);
      if (!critical.exchange(false, std::memory_order_relaxed)) {
        error.store(true, std::memory_order_relaxed);
      }
      ticket_lock.Unlock();
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

TEST(TicketLock, Counter) {
  TimeoutGuard guard(4s);
  size_t counter = 0;
  const size_t kIters = 10000;
  const size_t kThreads = 10;

  Barrier barrier(kThreads);
  TicketLock ticket_lock;

  auto incrementer = [&] () {
    barrier.Wait();
    for (size_t i = 0; i != kIters; ++i) {
      ticket_lock.Lock();
      ++counter;
      ticket_lock.Unlock();
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

