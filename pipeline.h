#ifndef PIPELINE_H
#define PIPELINE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <string>
using namespace std;

void create_pipes(int (*)[2] , int) ;
void my_pipe(vector<string>);
void exe_pipe(vector<vector<string>> ,int);
#endif