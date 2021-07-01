#include "is_prime.h"

#include "timeout_guard.h"
#include <gtest/gtest.h>


TEST(IsPrime, One) {
  TimeoutGuard guard(10ms);
  ASSERT_FALSE(IsPrime(1));
}

TEST(IsPrime, Two) {
  TimeoutGuard guard(10ms);
  ASSERT_TRUE(IsPrime(2));
}

TEST(IsPrime, Even) {
  TimeoutGuard guard(10ms);
  for (size_t i = 4; i < 10000000; i += 2) {
    EXPECT_FALSE(IsPrime(i));
  }
}

