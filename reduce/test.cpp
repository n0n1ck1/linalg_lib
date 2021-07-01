#include "reduce.h"

#include "timeout_guard.h"
#include <gtest/gtest.h>

#include <random>


static std::vector<int> gen_vector(size_t size) {
  static std::mt19937 gen;
  std::uniform_int_distribution<int> dist(1);
  std::vector<int> v(size);
  for (int& x : v) {
    x = dist(gen);
  }
  return v;
}

TEST(Reduce, OneThreadSum) {
  const size_t n_threads = 1;
  const size_t size = 1024;
  std::vector<int> v = gen_vector(size);

  auto sum = [] (int a, int b) {
      return a + b;
  };

  int result = std::reduce(v.begin(), v.end(), 0, sum);

  TimeoutGuard guard(50ms);
  int parallel_result = parallel_reduce(v.begin(), v.end(), 0, sum, n_threads);
  ASSERT_EQ(result, parallel_result);
}

TEST(Reduce, OneThreadProd) {
  const size_t n_threads = 1;
  const size_t size = 1024;
  std::vector<int> v = gen_vector(size);

  auto prod = [] (int a, int b) {
      return a * b;
  };

  int result = std::reduce(v.begin(), v.end(), 1, prod);

  TimeoutGuard guard(50ms);
  int parallel_result = parallel_reduce(v.begin(), v.end(), 1, prod, n_threads);
  ASSERT_EQ(result, parallel_result);
}

TEST(Reduce, TwoThreadSum) {
  const size_t n_threads = 2;
  const size_t size = 1ull << 22;
  std::vector<int> v = gen_vector(size);

  auto sum = [] (int a, int b) {
      return a + b;
  };

  int result = std::reduce(v.begin(), v.end(), 0, sum);
  TimeoutGuard guard(250ms);
  int parallel_result = parallel_reduce(v.begin(), v.end(), 0, sum, n_threads);
  ASSERT_EQ(result, parallel_result);
}

TEST(Reduce, TwoThreadsProd) {
  const size_t n_threads = 2;
  const size_t size = 1ull << 22;
  std::vector<int> v = gen_vector(size);

  auto prod = [] (int a, int b) {
      return a * b;
  };

  int result = std::reduce(v.begin(), v.end(), 1, prod);
  TimeoutGuard guard(250ms);
  int parallel_result = parallel_reduce(v.begin(), v.end(), 1, prod, n_threads);
  ASSERT_EQ(result, parallel_result);
}

TEST(Reduce, TenThreadSum) {
  const size_t n_threads = 10;
  const size_t size = 1ull << 22;
  std::vector<int> v = gen_vector(size);

  auto sum = [] (int a, int b) {
      return a + b;
  };

  int result = std::reduce(v.begin(), v.end(), 0, sum);
  TimeoutGuard guard(250ms);
  int parallel_result = parallel_reduce(v.begin(), v.end(), 0, sum, n_threads);
  ASSERT_EQ(result, parallel_result);
}

TEST(Reduce, TenThreadsProd) {
  const size_t n_threads = 10;
  const size_t size = 1ull << 22;
  std::vector<int> v = gen_vector(size);

  auto prod = [] (int a, int b) {
      return a * b;
  };

  int result = std::reduce(v.begin(), v.end(), 1, prod);
  TimeoutGuard guard(250ms);
  int parallel_result = parallel_reduce(v.begin(), v.end(), 1, prod, n_threads);
  ASSERT_EQ(result, parallel_result);
}

TEST(Reduce, TwoThreadsFaster) {
  const size_t size = 1ull << 22;
  std::vector<int> v = gen_vector(size);

  auto sum = [] (int a, int b) {
      return a + b;
  };

  TimeoutGuard guard(500ms);
  auto start1 = std::chrono::high_resolution_clock::now();
  int parallel_result1 = parallel_reduce(v.begin(), v.end(), 0, sum, 1);
  auto end1 = std::chrono::high_resolution_clock::now();
  auto dur1 = end1 - start1;

  auto start2 = std::chrono::high_resolution_clock::now();
  int parallel_result2 = parallel_reduce(v.begin(), v.end(), 0, sum, 2);
  auto end2 = std::chrono::high_resolution_clock::now();
  auto dur2 = end2 - start2;

  ASSERT_EQ(parallel_result1, parallel_result2);
  ASSERT_GE(dur1.count(), 5 * dur2.count() / 4);
}

