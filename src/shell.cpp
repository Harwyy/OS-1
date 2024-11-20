#include "shell.hpp"
#include "commands.hpp"

#include <sstream>

using namespace std;

vector<string> history;

void addToHistory(const string& command) {
  const size_t maxHistorySize = 15;
  if (history.size() >= maxHistorySize) {
    history.erase(history.begin());
  }
  history.push_back(command);
}

void shell() {
  while (true) {
    string userInput;
    cout << "\033[32mHell>\033[0m ";
    getline(cin, userInput);

    addToHistory(userInput);

    istringstream iss(userInput);
    string command;
    iss >> command;

    vector<string> args;
    string arg;
    while (iss >> arg) {
      args.push_back(arg);
    }

    if (command == "clear" || command == "cls") {
      clear();
    } else if (command == "pwd") {
      pwd();
    } else if (command == "cd") {
      if (!args.empty()) {
        cd(args[0]);
      } else {
        cout << "cd: Missing argument. Example: cd <path>" << endl;
      }
    } else if (command == "ls") {
      ls();
    } else if (command == "mkdir") {
      if (!args.empty()) {
        mkdir(args[0]);
      } else {
        cout << "mkdir: Missing argument. Example: mkdir <folder-name>" << endl;
      }
    } else if (command == "rmdir") {
      if (!args.empty()) {
        rmdir(args[0]);
      } else {
        cout << "rmdir: Missing argument. Example: rmdir <folder-name>" << endl;
      }
    } else if (command == "echo") {
      if (!args.empty()) {
        string message = userInput.substr(userInput.find(' ') + 1);
        echo(message);
      } else {
        cout << "echo: Missing argument. Example: echo <message>" << endl;
      }
    } else if (command == "exit") {
      exitHell();
    } else if (command == "run") {
      if (!args.empty()) {
        string programAndArgs = userInput.substr(userInput.find(' ') + 1);
        execute(programAndArgs);
      } else {
        cout << "run: Missing argument. Example: run <program> [arguments]" << endl;
      }
    } else if (command == "history") {

      cout << "\nCommand History:" << endl;
      if (history.empty()) {
        cout << "No commands in history." << endl;
      } else {
        for (size_t i = 0; i < history.size(); ++i) {
          cout << i + 1 << ": " << history[i] << endl;
        }
      }
      cout << endl;
    } else {
      cout << "Invalid command!" << endl;
    }
  }
}


int main() {
  shell();
  return 1;
}