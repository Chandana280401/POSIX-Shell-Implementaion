#ifndef PIPE_REDIRECT_H
#define PIPE_REDIRECT_H

#include<iostream>
#include<stdio.h>
#include<vector>
#include<string>

using namespace std;

extern string curr_cmnd;
vector<string> split_into_args_space(string);
vector<string> split_into_args_pipe();
void pipe_redirect();
#endif