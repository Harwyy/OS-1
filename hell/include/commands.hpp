#ifndef commands_hpp
#define commands_hpp

#include <iostream>
#include <string>
#include <chrono>
#include <vector>

void clear();

void pwd();

void ls();

void cd(const std::string & path);

void mkdir(const std::string & path);

void rmdir(const std::string & path);

void echo(const std::string& word);

void exitHell();

void execute(const std::string &pathWithArgs);

#endif