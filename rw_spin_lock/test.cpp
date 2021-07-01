#include "rw_spin_lock.h"

#include "timeout_guard.h"
#include "barrier.h"
#include <gtest/gtest.h>

#include <atomic>
#include <thread>
#include <vector>


TEST(RWSpinLock, LockUnlock) {
  TimeoutGuard guard(100ms);
  RWSpinLock rw_spin_lock;
  rw_spin_lock.LockWrite();
  rw_spin_lock.UnlockWrite();
}

TEST(RWSpinLock, WriteWrite) {
  TimeoutGuard guard(1s);
  RWSpinLock rw_spin_lock;
  rw_spin_lock.LockWrite();
  std::atomic<bool> aquired{false};
  std::thread t([&] {
      rw_spin_lock.LockWrite();
      aquired.store(true);
  });

  std::this_thread::sleep_for(250ms);
  ASSERT_FALSE(aquired.load());
  rw_spin_lock.UnlockWrite();
  std::this_thread::sleep_for(250ms);
  ASSERT_TRUE(aquired.load());
  t.join();
}

TEST(RWSpinLock, WriteRead) {
  TimeoutGuard guard(1s);
  RWSpinLock rw_spin_lock;
  rw_spin_lock.LockWrite();
  std::atomic<bool> aquired{false};
  std::thread t([&] {
      rw_spin_lock.LockRead();
      aquired.store(true);
  });

  std::this_thread::sleep_for(250ms);
  ASSERT_FALSE(aquired.load());
  rw_spin_lock.UnlockWrite();
  std::this_thread::sleep_for(250ms);
  ASSERT_TRUE(aquired.load());
  t.join();
}

TEST(RWSpinLock, ReadWrite) {
  TimeoutGuard guard(1s);
  RWSpinLock rw_spin_lock;
  rw_spin_lock.LockRead();
  std::atomic<bool> aquired{false};
  std::thread t([&] {
      rw_spin_lock.LockWrite();
      aquired.store(true);
  });

  std::this_thread::sleep_for(250ms);
  ASSERT_FALSE(aquired.load());
  rw_spin_lock.UnlockRead();
  std::this_thread::sleep_for(250ms);
  ASSERT_TRUE(aquired.load());
  t.join();
}

TEST(RWSpinLock, ReadRead) {
  TimeoutGuard guard(1s);
  RWSpinLock rw_spin_lock;
  rw_spin_lock.LockRead();
  std::atomic<bool> aquired{false};
  std::thread t([&] {
      rw_spin_lock.LockRead();
      aquired.store(true);
  });

  std::this_thread::sleep_for(250ms);
  ASSERT_TRUE(aquired.load());
  t.join();
}

TEST(RWSpinLock, Exclusive) {
  TimeoutGuard guard(2s);
  std::atomic<bool> critical{false};
  std::atomic<bool> error{false};

  const size_t kIters = 200;
  const size_t kThreads = 100;

  Barrier barrier(kThreads);
  RWSpinLock rw_spin_lock;

  auto worker = [&] () {
    barrier.Wait();
    for (size_t i = 0; i != kIters; ++i) {
      rw_spin_lock.LockWrite();
      if (critical.exchange(true, std::memory_order_relaxed)) {
        error.store(true, std::memory_order_relaxed);
      }
      std::this_thread::sleep_for(10ns);
      if (!critical.exchange(false, std::memory_order_relaxed)) {
        error.store(true, std::memory_order_relaxed);
      }
      rw_spin_lock.UnlockWrite();
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

TEST(RWSpinLock, Counter) {
  TimeoutGuard guard(1s);
  size_t counter = 0;
  const size_t kIters = 500000;
  const size_t kThreads = 10;

  Barrier barrier(kThreads);
  RWSpinLock rw_spin_lock;

  auto incrementer = [&] () {
    barrier.Wait();
    for (size_t i = 0; i != kIters; ++i) {
      rw_spin_lock.LockWrite();
      ++counter;
      rw_spin_lock.UnlockWrite();
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

TEST(RWSpinLock, WriterPriority) {
  TimeoutGuard guard(1s);
  const size_t kIters = 100;
  const size_t kWriterIters = 10;
  const size_t kThreads = 10;

  Barrier barrier(kThreads);
  RWSpinLock rw_spin_lock;
  std::atomic<bool> writer_stoped{false};
  std::atomic<bool> error{false};
  auto reader = [&] () {
    barrier.Wait();
    for (size_t i = 0; i != kIters; ++i) {
      rw_spin_lock.LockRead();
      std::this_thread::sleep_for(5ms);
      rw_spin_lock.UnlockRead();
    }
    if (!writer_stoped.load()) {
      error.store(true);
    }
  };

  auto writer = [&] () {
    barrier.Wait();
    for (size_t i = 0; i != kWriterIters; ++i) {
      rw_spin_lock.LockWrite();
      rw_spin_lock.UnlockWrite();
      std::this_thread::sleep_for(10ms);
    }
    writer_stoped.store(true);
  };

  std::vector<std::thread> threads;
  for (size_t i = 1; i != kThreads; ++i) {
    threads.emplace_back(reader);
  }
  threads.emplace_back(writer);

  for (auto& t : threads) {
    t.join();
  }

  ASSERT_FALSE(error.load());
}

