#include "shell.hpp"

using namespace std;

void shell() {
  while (1) {
    std::string userInput;
    cout << "Enter command: ";
    getline(cin, userInput);

    if (userInput == "clear") {
      clear();
    } else if (userInput == "pwd") {
      pwd();
    } 
    else if (userInput.find("echo ") != string::npos) {
      echo(userInput);
    } else {
      cout << "Invalid command!" << endl;
    }
  }
}

int main() {
  shell();
  return 1;
}