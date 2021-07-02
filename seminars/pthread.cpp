#include <iostream>
#include <string>
#include <stdlib.h>
#include <pthread.h>


void* print_message(void* arg) {
  std::string* message = static_cast<std::string*>(arg);
  std::cout << *message << '\n';
  int* x = new int(42);
  return static_cast<void*>(x);
}

int main() {
  pthread_t thread1, thread2;
  std::string message1 = "Thread 1";
  std::string message2 = "Thread 2";

  int iret1 = pthread_create(&thread1, NULL, print_message,
                            static_cast<void*>(&message1));
  int iret2 = pthread_create(&thread2, NULL, print_message, &message2);

  std::cout << "Thread 1 returns: " << iret1 << '\n';
  std::cout << "Thread 2 returns: " << iret2 << '\n';

  void* res;
  pthread_join(thread1, &res);
  std::cout << "Thread1 joined with result "
            << *static_cast<int*>(res) << '\n';




  pthread_join(thread2, NULL);

  pthread_detach(thread2);
}

