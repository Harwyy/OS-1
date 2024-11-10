#ifndef commands_hpp
#define commands_hpp

#include <tchar.h>
#include <windows.h>

#include <iostream>
#include <string>

using namespace std;

#define BUFSIZE 4096

void clear();

void pwd();

void ls();

void cd();

void execute();

void mkdir();

void rmdir();

void echo(const std::string& word);

void cat();

void exit();

#endif