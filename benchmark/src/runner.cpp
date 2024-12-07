#include <windows.h>
#include <vector>
#include <string>
#include <iostream>

void launchProcess(const std::string& command, const std::vector<std::string>& args) {
    std::string commandLine = command;
    for (const auto& arg : args) {
        commandLine += " " + arg;
    }

    char commandLineCStr[1024];
    strcpy_s(commandLineCStr, commandLine.c_str());

    STARTUPINFOA si = {0};
    si.cb = sizeof(STARTUPINFOA);
    PROCESS_INFORMATION pi = {0};

    if (!CreateProcessA(
            nullptr,
            commandLineCStr,
            nullptr,
            nullptr,
            FALSE,
            0,
            nullptr,
            nullptr,
            &si,
            &pi
    )) {
        std::cerr << "Failed to start process: " << GetLastError() << std::endl;
        return;
    }

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}

int main(int argc, char* argv[]) {
    if (argc < 9) {
        std::cerr << "Usage: " << argv[0] << " <count> <command> <arg1> <arg2> <arg3> <arg4> <arg5> <arg6>\n";
        return 1;
    }

    int count = std::stoi(argv[1]);
    std::string command = argv[2];
    std::vector<std::string> args = {argv[3], argv[4], argv[5], argv[6], argv[7], argv[8]};

    for (int i = 0; i < count; ++i) {
        launchProcess(command, args);
    }

    return 0;
}
