#ifndef shell_hpp
#define shell_hpp

#include <string>
#include <vector>

void addToHistory(const std::string& command);
void shell();

extern std::vector<std::string> history;

#endif