#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <memory>
#include <stdexcept>
#include <thread>


using namespace std::literals::chrono_literals;

class TimeoutGuard {
 public:
  template <typename Rep, typename Period>
  explicit TimeoutGuard(const std::chrono::duration<Rep, Period>& dur)
#if defined(NO_TIMEOUT)
  {
    (void) dur;
  }
#else
    : should_fail_(new std::atomic<bool>(true))
    , thread_([] (std::chrono::duration<Rep, Period> duration,
                  std::shared_ptr<std::atomic<bool>> should_fail) {
          std::this_thread::sleep_for(duration);
          if (should_fail->load()) {
            throw std::runtime_error("Timeout!");
          }
        }, dur, should_fail_)
  {
    thread_.detach();
  }
#endif

  ~TimeoutGuard() {
#if defined(NO_TIMEOUT)
#else
    should_fail_->store(false);
#endif
  }

  static auto s(size_t sec_count) {
    return TimeoutGuard(std::chrono::seconds(sec_count));
  }

  static auto ms(size_t msec_count) {
    return TimeoutGuard(std::chrono::milliseconds(msec_count));
  }

 private:
  std::shared_ptr<std::atomic<bool>> should_fail_;
  std::thread thread_;
};


