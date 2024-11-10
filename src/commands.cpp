#include "commands.hpp"

void clear() {
  COORD coordScreen = {0, 0};
  DWORD dwCharsWritten;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD dwConSize;
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  GetConsoleScreenBufferInfo(hConsole, &csbi);
  dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
  FillConsoleOutputCharacter(hConsole, ' ', dwConSize, coordScreen, &dwCharsWritten);
  GetConsoleScreenBufferInfo(hConsole, &csbi);
  FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &dwCharsWritten);
  SetConsoleCursorPosition(hConsole, coordScreen);
}

void pwd() {
  char buffer[BUFSIZE];
  DWORD dwRet = GetCurrentDirectoryA(BUFSIZE, buffer);
  if (dwRet == 0) {
    cerr << "Error getting the current directory: " << GetLastError() << endl;
  } else {
    cout << "\nPath\n----\n" << buffer << "\n" << endl;
  }
}

void echo(const string& input){
  cout << input << endl;
}
