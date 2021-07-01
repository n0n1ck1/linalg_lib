#include "hello_world.h"

#include "timeout_guard.h"
#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <set>


static void check_output(const std::string& str, size_t n_threads) {
  size_t prev_end = 0;
  std::set<std::string> tids;
  for (size_t i = 0; i != n_threads; ++i) {
    size_t curr_start = prev_end;
    auto curr_end = str.find('\n', prev_end);
    // new line should exists
    ASSERT_NE(curr_end, std::string::npos);

    // check prefix
    ASSERT_EQ(str.substr(curr_start, HelloWorld::kHelloPrefix.size()),
              HelloWorld::kHelloPrefix);

    // check thread id
    size_t id_start = curr_start + HelloWorld::kHelloPrefix.size();
    size_t id_end = curr_end - 1;
    std::string thread_id = str.substr(id_start, id_end - id_start);
    ASSERT_EQ(tids.count(thread_id), 0);
    tids.insert(thread_id);

    prev_end = curr_end + 1;
  }
  ASSERT_EQ(prev_end, str.size());
}

TEST(HelloWorld, OneThread) {
  TimeoutGuard guard(100ms);
  const size_t kThreads = 1;
  HelloWorld w(kThreads);
  std::stringstream ss;
  w.SayHello(ss);
  check_output(ss.str(), kThreads);
}

TEST(HelloWorld, TwoThreads) {
  TimeoutGuard guard(100ms);
  const size_t kThreads = 2;
  HelloWorld w(kThreads);
  std::stringstream ss;
  w.SayHello(ss);
  check_output(ss.str(), kThreads);
}

TEST(HelloWorld, ManyThreads) {
  TimeoutGuard guard(100ms);
  const size_t kThreads = 100;
  HelloWorld w(kThreads);
  std::stringstream ss;
  w.SayHello(ss);
  check_output(ss.str(), kThreads);
}

