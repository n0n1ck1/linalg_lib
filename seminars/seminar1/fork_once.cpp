#include <unistd.h>
#include <iostream>

int main() {
  pid_t pid = getpid();
  pid_t parent_pid = getppid();

  std::cout << "pid: " << pid << "; parent pid: " << parent_pid << '\n' << '\n';

  pid_t fork_pid = fork();
  pid = getpid();
  parent_pid = getppid();
  std::cout << "pid: " << pid << "; parent pid: " << parent_pid << "; fork pid: " << fork_pid << '\n';

  if (fork_pid != 0) {
    // wait(NULL);
  }
}

