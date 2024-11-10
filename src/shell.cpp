#include "shell.hpp"

void shell() {
  while (1) {
    std::string userInput;

    std::cout << "Enter command: ";
    std::getline(std::cin, userInput);

    if (userInput == "clear") {
      clear();
    } else {
      std::cout << "Invalid command!" << std::endl;
    }
  }
}

int main() {
  shell();
  return 1;
}