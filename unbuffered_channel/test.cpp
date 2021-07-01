#include "unbuffered_channel.h"

#include "timeout_guard.h"
#include <gtest/gtest.h>

#include <exception>
#include <optional>
#include <thread>
#include <vector>


TEST(UnbufferedChannel, SendRecv) {
  UnbufferedChannel<int> chan;
  std::thread t([&] {
      chan.Send(42);
  });
  auto ret = chan.Recv();
  ASSERT_NE(ret, std::nullopt);
  ASSERT_EQ(*ret, 42);
  t.join();
}

TEST(UnbufferedChannel, SendClose) {
  TimeoutGuard guard(500ms);

  UnbufferedChannel<int> chan;
  std::thread t([&] {
      std::this_thread::sleep_for(250ms);
      chan.Close();
  });

  auto start = std::chrono::system_clock::now();
  std::string error;
  try {
    chan.Send(42);
  } catch (std::exception& e) {
    error = e.what();
  }
  auto end = std::chrono::system_clock::now();
  ASSERT_STREQ(error.c_str(), "Channel is closed");
  ASSERT_GE(end - start, 100ms);
  t.join();
}

TEST(UnbufferedChannel, RecvClose) {
  TimeoutGuard guard(500ms);

  UnbufferedChannel<int> chan;
  std::thread t([&] {
      std::this_thread::sleep_for(250ms);
      chan.Close();
  });

  auto start = std::chrono::system_clock::now();
  auto ret = chan.Recv();
  auto end = std::chrono::system_clock::now();
  ASSERT_EQ(ret, std::nullopt);
  ASSERT_GE(end - start, 100ms);
  t.join();
}

void CheckSendRecv(const std::vector<std::vector<int>>& send,
                   const std::vector<std::vector<int>>& recv) {
  std::vector<int> all_send;
  for (auto&& v : send) {
    all_send.insert(all_send.end(), v.begin(), v.end());
  }
  std::vector<int> all_recv;
  for (auto&& v : recv) {
    all_recv.insert(all_recv.end(), v.begin(), v.end());
  }

  std::sort(all_send.begin(), all_send.end());
  std::sort(all_recv.begin(), all_recv.end());

  ASSERT_EQ(all_send, all_recv);
}

void RunTest(size_t num_senders, size_t num_recvers) {
  TimeoutGuard guard(5s);

  UnbufferedChannel<int> chan;
  std::atomic<int> send_value{0};
  std::atomic<bool> is_closed{false};
  std::atomic<bool> error{false};

  std::vector<std::vector<int>> send_values(num_senders);
  std::vector<std::vector<int>> recv_values(num_recvers);

  auto sender = [&] (size_t id) {
    for (;;) {
      int value = send_value.fetch_add(1);
      try {
        chan.Send(value);
      } catch (std::exception&) {
        if (!is_closed.load()) {
          error.store(true);
        }
        break;
      }
      send_values[id].push_back(value);
    }
  };

  auto recver = [&] (size_t id) {
    for (;;) {
      auto ret = chan.Recv();
      if (!ret) {
        if (!is_closed.load()) {
          error.store(true);
        }
        break;
      }
      recv_values[id].push_back(*ret);
    }
  };

  std::vector<std::thread> threads;
  for (size_t i = 0; i != num_senders; ++i) {
    threads.emplace_back(sender, i);
  }
  for (size_t i = 0; i != num_recvers; ++i) {
    threads.emplace_back(recver, i);
  }

  std::this_thread::sleep_for(2s);
  is_closed.store(true);
  chan.Close();

  for (auto& t : threads) {
    t.join();
  }

  ASSERT_FALSE(error.load());
  CheckSendRecv(send_values, recv_values);
}

TEST(UnbufferedChannel, Small) {
  RunTest(1, 1);
}

TEST(UnbufferedChannel, Senders) {
  RunTest(3, 1);
}

TEST(UnbufferedChannel, Recvers) {
  RunTest(1, 3);
}

TEST(UnbufferedChannel, Big) {
  RunTest(3, 3);
}
