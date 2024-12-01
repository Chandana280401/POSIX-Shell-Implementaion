#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <string.h>
using namespace std;

extern bool background;
extern pid_t main_pid;
extern pid_t foreground_pid;
vector<char*> convertArgs(vector<string>&);
void runForegroundProcess(string&, vector<string>&);
void runBackgroundProcess(string& , vector<string>&);
#endif