#include "thread_safe_vector.h"

#include "timeout_guard.h"
#include <gtest/gtest.h>

#include <atomic>
#include <thread>


TEST(ThreadSafeVector, Simple) {
  TimeoutGuard guard(100ms);
  const size_t kSize = 1024;
  ThreadSafeVector<int> v;
  for (int i = 0; i != kSize; ++i) {
    v.PushBack(i);
    ASSERT_EQ(v.Size(), i + 1);
  }

  for (int i = 0; i != kSize; ++i) {
    ASSERT_EQ(v[i], i);
  }
}

TEST(ThreadSafeVector, OneProducer) {
  TimeoutGuard guard(1s);
  const size_t kSize = 1ull << 17;
  const size_t kProducers = 1;
  const size_t kReaders = 3;
  ThreadSafeVector<int> v;
  std::atomic<bool> stop{false};
  std::atomic<bool> error{false};

  auto reader = [&] {
    while (!stop.load(std::memory_order_relaxed)) {
      size_t size = v.Size();
      for (size_t i = 0; i != size && !stop.load(std::memory_order_relaxed); ++i) {
        int x = v[i];
        if (x != static_cast<int>(i)) {
          error.store(true);
        }
      }
    }
  };

  auto producer = [&] {
    for (size_t i = 0; i != kSize; ++i) {
      v.PushBack(i);
    }
  };

  std::vector<std::thread> threads;
  for (size_t i = 0; i != kProducers; ++i) {
    threads.emplace_back(producer);
  }
  for (size_t i = 0; i != kReaders; ++i) {
    threads.emplace_back(reader);
  }

  for (size_t i = 0; i != kProducers; ++i) {
    threads[i].join();
  }
  stop.store(true, std::memory_order_relaxed);
  for (size_t i = 0; i != kReaders; ++i) {
    threads[kProducers + i].join();
  }

  ASSERT_EQ(v.Size(), kProducers * kSize);
  ASSERT_FALSE(error.load());
}


TEST(ThreadSafeVector, Big) {
  TimeoutGuard guard(4s);
  const size_t kSize = 1ull << 20;
  const size_t kReaders = 4;
  const size_t kProducers = 2;
  ThreadSafeVector<int> v;
  std::atomic<bool> stop{false};

  auto reader = [&] {
    while (!stop.load(std::memory_order_relaxed)) {
      size_t size = v.Size();
      for (size_t i = 0; i != size && !stop.load(std::memory_order_relaxed); ++i) {
        int x = v[i];
        (void) x;
      }
    }
  };

  auto producer = [&] {
    for (size_t i = 0; i != kSize; ++i) {
      v.PushBack(i);
    }
  };

  std::vector<std::thread> threads;
  for (size_t i = 0; i != kProducers; ++i) {
    threads.emplace_back(producer);
  }
  for (size_t i = 0; i != kReaders; ++i) {
    threads.emplace_back(reader);
  }

  for (size_t i = 0; i != kProducers; ++i) {
    threads[i].join();
  }
  stop.store(true, std::memory_order_relaxed);
  for (size_t i = 0; i != kReaders; ++i) {
    threads[kProducers + i].join();
  }

  ASSERT_EQ(v.Size(), kProducers * kSize);
}


class SlowItem {
 public:
  SlowItem() {
  }

  SlowItem(const SlowItem&) {
    std::this_thread::sleep_for(20ms);
  }

  SlowItem(SlowItem&&) {
    std::this_thread::sleep_for(20ms);
  }

  SlowItem& operator=(SlowItem) {
    return *this;
  }
};

TEST(ThreadSafeVector, ReadWrite) {
  TimeoutGuard guard(5s);
  const size_t kSize = 9;
  const size_t kReaders = 4;
  const size_t kProducers = 1;
  ThreadSafeVector<SlowItem> v;
  std::atomic<bool> stop{false};
  std::atomic<size_t> reads{0};

  auto reader = [&] {
    size_t local_reads = 0;
    while (!stop.load(std::memory_order_relaxed)) {
      size_t size = v.Size();
      for (size_t i = 0; i != 10; ++i) {
        for (size_t j = 0; j != size; ++j) {
          SlowItem x = v[j];
          ++local_reads;
          (void) x;
        }
      }
    }
    reads += local_reads;
  };

  auto producer = [&] {
    for (size_t i = 0; i != kSize; ++i) {
      v.PushBack(SlowItem());
      std::this_thread::sleep_for(100ms);
    }
  };

  std::vector<std::thread> threads;
  for (size_t i = 0; i != kProducers; ++i) {
    threads.emplace_back(producer);
  }
  for (size_t i = 0; i != kReaders; ++i) {
    threads.emplace_back(reader);
  }


  for (size_t i = 0; i != kProducers; ++i) {
    threads[i].join();
  }
  stop.store(true, std::memory_order_relaxed);
  for (size_t i = 0; i != kReaders; ++i) {
    threads[kProducers + i].join();
  }

  ASSERT_EQ(v.Size(), kProducers * kSize);
}

