#include <iostream>
#include <thread>

void say_hello() {
  std::cout << "Hello from thread!" << std::endl;
}

int main() {
  std::thread t(say_hello);
  t.join();  // Ожидаем завершения потока
  return 0;
}
