#include "commands.hpp"

#include <tchar.h>
#include <windows.h>

using namespace std;

void clear() {
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

  if (hConsole == INVALID_HANDLE_VALUE){
    return;
  }

  CONSOLE_SCREEN_BUFFER_INFO csbi;

  if (!GetConsoleScreenBufferInfo(hConsole, &csbi)){
    return;
  }

  DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
  DWORD charsWritten;
  FillConsoleOutputCharacter(hConsole, ' ', consoleSize, {0, 0}, &charsWritten);
  FillConsoleOutputAttribute(hConsole, csbi.wAttributes, consoleSize, {0, 0}, &charsWritten);
  SetConsoleCursorPosition(hConsole, {0, 0});
}


void pwd() {
  DWORD bufferSize = GetCurrentDirectoryA(0, nullptr);

  if (bufferSize == 0){
    cerr << "Error getting buffer size for the current directory: "
         << GetLastError() << endl;
    return;
  }

  char* buffer = new char[bufferSize];

  if (GetCurrentDirectoryA(bufferSize, buffer) == 0){
    cerr << "Error getting the current directory: "
         << GetLastError() << endl;
    delete[] buffer;
    return;
  }

  cout << "\nCurrent Directory:\n------------------\n" << buffer << "\n" << endl;
  delete[] buffer;
}

void cd(const string & path){
  DWORD bufferSize = GetCurrentDirectoryA(0, nullptr);

  if (bufferSize == 0){
    cerr << "Error getting buffer size for the current directory: "
         << GetLastError() << endl;
    return;
  }

  char* buffer = new char[bufferSize];

  if (GetCurrentDirectoryA(bufferSize, buffer) == 0){
    cerr << "Error getting the current directory: "
         << GetLastError() << endl;
    delete[] buffer;
    return;
  }

  if (path == ".."){
    if (SetCurrentDirectoryA("..") == 0) {
      cerr << "Error changing to parent directory: "
                << GetLastError() << endl;
    } else {
      cout << "Changed to parent directory." << endl;
    }
    delete[] buffer;
    return;
  }

  if (path == ".") {
    cout << "Current directory remains the same." << endl;
    delete[] buffer;
    return;
  }

  string newPath = string(buffer) + "\\" + path;

  if (SetCurrentDirectoryA(newPath.c_str()) == 0){
    cerr << "Error changing directory to " << newPath << ": "
              << GetLastError() << endl;
  } else {
    cout << "Changed directory to " << newPath << endl;
  }

  delete[] buffer;
}

void ls(){
  WIN32_FIND_DATA ffd;
  HANDLE hFind = INVALID_HANDLE_VALUE;

  DWORD bufferSize = GetCurrentDirectoryA(0, nullptr);
  if (bufferSize == 0){
    cerr << "Error getting buffer size for the current directory: "
         << GetLastError() << endl;
    return;
  }

  char* buffer = new char[bufferSize];

  if (GetCurrentDirectoryA(bufferSize, buffer) == 0){
    cerr << "Error getting the current directory: "
         << GetLastError() << endl;
    delete[] buffer;
    return;
  }

  string searchPath = string(buffer) + "\\*";
  hFind = FindFirstFile(searchPath.c_str(), &ffd);

  if (hFind == INVALID_HANDLE_VALUE) {
    cerr << "Error finding files in the directory: "
              << GetLastError() << endl;
    delete[] buffer;
    return;
  }

  cout << endl << "Directory Listing:" << endl;
  cout << "==========================================" << endl;

  do {
    if (string(ffd.cFileName) != "." && string(ffd.cFileName) != "..") {
      cout << "\nName       : " << ffd.cFileName << endl;

      if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        cout << "Type       : Directory" << endl;
      } else {
        cout << "Type       : File" << endl;
        cout << "Size       : " << ffd.nFileSizeLow << " bytes" << endl;
      }

      SYSTEMTIME systemTime;
      if (FileTimeToSystemTime(&ffd.ftCreationTime, &systemTime)) {
        cout << "Created On : "
             << (systemTime.wDay < 10 ? "0" : "") << systemTime.wDay << "/"
             << (systemTime.wMonth < 10 ? "0" : "") << systemTime.wMonth << "/"
             << systemTime.wYear << " "
             << (systemTime.wHour < 10 ? "0" : "") << systemTime.wHour << ":"
             << (systemTime.wMinute < 10 ? "0" : "") << systemTime.wMinute << ":"
             << (systemTime.wSecond < 10 ? "0" : "") << systemTime.wSecond << endl;
      } else {
        cerr << "Error retrieving file creation time: " << GetLastError() << endl;
      }

      cout << "------------------------------------------" << endl;
    }
  } while (FindNextFile(hFind, &ffd) != 0);

  cout << "==========================================\n" << endl;

  if (GetLastError() != ERROR_NO_MORE_FILES){
    cerr << "Error reading files: " << GetLastError() << endl;
  }

  FindClose(hFind);
  delete[] buffer;
}

void mkdir(const string & path){
  DWORD bufferSize = GetCurrentDirectoryA(0, nullptr);

  if (bufferSize == 0){
    cerr << "Error getting buffer size for the current directory: "
         << GetLastError() << endl;
    return;
  }

  char* buffer = new char[bufferSize];

  if (GetCurrentDirectoryA(bufferSize, buffer) == 0){
    cerr << "Error getting the current directory: "
         << GetLastError() << endl;
    delete[] buffer;
    return;
  }

  string newPath = string(buffer) + "\\" + path;

  if (CreateDirectory(newPath.c_str(), nullptr)){
    cout << "Directory created successfully: " << newPath << endl;
  } else {
    DWORD error = GetLastError();
    if (error == ERROR_ALREADY_EXISTS) {
      cerr << "Error: The directory already exists: " << path << endl;
    } else {
      cerr << "Error creating directory: " << path << " (" << error << ")" << endl;
    }
  }

  delete[] buffer;
}

void rmdir(const string & path){
  DWORD bufferSize = GetCurrentDirectoryA(0, nullptr);

  if (bufferSize == 0){
    cerr << "Error getting buffer size for the current directory: "
         << GetLastError() << endl;
    return;
  }

  char* buffer = new char[bufferSize];

  if (GetCurrentDirectoryA(bufferSize, buffer) == 0){
    cerr << "Error getting the current directory: "
         << GetLastError() << endl;
    delete[] buffer;
    return;
  }

  string newPath = string(buffer) + "\\" + path;

  if (RemoveDirectory(newPath.c_str())){
    cout << "Directory created successfully: " << newPath << endl;
  } else {
    DWORD error = GetLastError();
    if (error == ERROR_DIR_NOT_EMPTY) {
      cerr << "Error: The directory is not empty: " << newPath << endl;
    } else if (error == ERROR_PATH_NOT_FOUND) {
      cerr << "Error: The specified path does not exist: " << newPath << endl;
    } else {
      cerr << "Error removing directory: " << newPath
           << " (Error code: " << error << ")" << endl;
    }
  }

  delete[] buffer;
}

void echo(const string &word) {
  cout << word << endl;
}

void exitHell() {
  cout << "Exiting hell..." << endl;
  exit(0);
}

void execute(const string& pathWithArgs) {
  STARTUPINFOA startupInfo = {0};
  PROCESS_INFORMATION processInfo = {nullptr};
  startupInfo.cb = sizeof(STARTUPINFOA);

  vector<char> commandLine(pathWithArgs.begin(), pathWithArgs.end());
  commandLine.push_back('\0');

  auto start = chrono::high_resolution_clock::now();

  if (!CreateProcessA(
          nullptr,
          commandLine.data(),
          nullptr,
          nullptr,
          false,
          0,
          nullptr,
          nullptr,
          &startupInfo,
          &processInfo
          )) {
    cerr << "Error creating process: " << GetLastError() << endl;
    return;
  }

  WaitForSingleObject(processInfo.hProcess, INFINITE);

  auto end = chrono::high_resolution_clock::now();
  chrono::duration<double> elapsedTime = end - start;

  DWORD exitCode;
  if (GetExitCodeProcess(processInfo.hProcess, &exitCode)) {
    cout << "Process exited with code: " << exitCode << endl;
  } else {
    cerr << "Failed to get process exit code: " << GetLastError() << endl;
  }

  CloseHandle(processInfo.hProcess);
  CloseHandle(processInfo.hThread);

  cout << "Execution time: " << elapsedTime.count() << " seconds" << endl;
}

