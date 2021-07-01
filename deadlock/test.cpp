#include "deadlock.h"

#include "timeout_guard.h"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest.h>

#include <thread>


TEST(Deadlock, OneThread) {
  TimeoutGuard guard(1s);
  Deadlock d;
  d.ThreadOne();
  d.ThreadOne();
  d.ThreadTwo();
  d.ThreadTwo();
  d.ThreadOne();
  d.ThreadOne();
  d.ThreadTwo();
  d.ThreadTwo();
}

TEST(Deadlock, TwoThreads) {
  testing::FLAGS_gtest_death_test_style="threadsafe";
  ASSERT_DEATH({
    TimeoutGuard guard(2s);
    Deadlock d;
    std::thread t1([&] {
        d.ThreadOne();
    });
    std::thread t2([&] {
        d.ThreadTwo();
    });

    t1.join();
    t2.join();
  }, "Timeout!");
}

