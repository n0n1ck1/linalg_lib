#pragma once

#include <ostream>
#include <string_view>


class HelloWorld {
 public:
  static constexpr std::string_view kHelloPrefix = "Hello, World! From thread ";

  HelloWorld(size_t /*n_threads*/)
  {
    // Your code
  }

  void SayHello(std::ostream& /*os*/) {
    // Your code
  }

 private:
  // Your code
};

