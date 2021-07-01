#include "philosophers.h"

#include "timeout_guard.h"
#include "barrier.h"
#include <gtest/gtest.h>

#include <random>
#include <thread>


struct Table {
  std::vector<Philosopher> philosophers;
  std::vector<std::unique_ptr<Fork>> forks;
};

static Table SetUpTable(size_t size) {
  Table table;
  if (size < 2) {
    return table;
  }
  for (size_t i = 0; i != size; ++i) {
    table.forks.push_back(std::make_unique<Fork>(i));
  }
  for (size_t i = 0; i != size; ++i) {
    table.philosophers.emplace_back(
        i,
        table.forks[i].get(),
        table.forks[(i + 1) % size].get());
  }
  return table;
}


TEST(Philosophers, Consistency) {
  TimeoutGuard guard(1s);

  Table table = SetUpTable(2);
  std::atomic<bool> second_eating{false};

  table.philosophers[0].Eat();
  ASSERT_FALSE(table.forks[0]->TryGet());
  ASSERT_FALSE(table.forks[1]->TryGet());

  std::thread t([&] {
    table.philosophers[1].Eat();
    second_eating.store(true);
  });

  std::this_thread::sleep_for(250ms);
  ASSERT_FALSE(table.forks[0]->TryGet());
  ASSERT_FALSE(table.forks[1]->TryGet());
  ASSERT_FALSE(second_eating.load());

  table.philosophers[0].Think();
  t.join();

  ASSERT_FALSE(table.forks[0]->TryGet());
  ASSERT_FALSE(table.forks[1]->TryGet());
  ASSERT_TRUE(second_eating.load());

  table.philosophers[1].Think();
  ASSERT_TRUE(table.forks[0]->TryGet());
  ASSERT_TRUE(table.forks[1]->TryGet());
}


static void Dine(Philosopher* p, size_t iters, Barrier& barrier,
                 uint32_t sleep_eat, uint32_t sleep_think) {
  std::mt19937 gen(p->Id() * 42);
  std::uniform_int_distribution<uint32_t> dist_eat(1, sleep_eat);
  std::uniform_int_distribution<uint32_t> dist_think(1, sleep_think);
  barrier.Wait();
  for (size_t i = 0; i != iters; ++i) {
    p->Eat();
    std::this_thread::sleep_for(std::chrono::microseconds(dist_eat(gen)));
    p->Think();
    std::this_thread::sleep_for(std::chrono::microseconds(dist_think(gen)));
  }
}

static void Dining(Table* table, size_t iters,
                   uint32_t sleep_eat = 1000, uint32_t sleep_think = 10000) {
  std::vector<std::thread> threads;
  Barrier barrier(table->philosophers.size());
  for (size_t i = 0; i != table->philosophers.size(); ++i) {
    threads.emplace_back(Dine, &table->philosophers[i], iters, std::ref(barrier),
                         sleep_eat, sleep_think);
  }
  for (auto& t : threads) {
    t.join();
  }
}


TEST(Philosophers, Small) {
  TimeoutGuard guard(1s);

  Table table = SetUpTable(2);
  Dining(&table, 100);
}

TEST(Philosophers, Big) {
  TimeoutGuard guard(1s);

  Table table = SetUpTable(10);
  Dining(&table, 100);
}

TEST(Philosophers, Frequent2) {
  TimeoutGuard guard(1s);

  Table table = SetUpTable(2);
  Dining(&table, 1000, 1, 1);
}

TEST(Philosophers, Frequent4) {
  TimeoutGuard guard(1s);

  Table table = SetUpTable(4);
  Dining(&table, 1000, 1, 1);
}

