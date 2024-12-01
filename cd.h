//cd.h
#ifndef CD_H
#define CD_H
#include<limits.h>
#include<iostream>
#include<cstdio>
using namespace std;

extern char home_dir_buff[PATH_MAX]; 
extern char curr_dir[PATH_MAX]; 
extern char display_dir[PATH_MAX];
extern char prev_dir[PATH_MAX];
extern string curr_cmnd;
extern string display;
// vector<string> cd_args;
// char* arg;

void my_cd(string);
#endif // CD_H