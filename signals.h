#ifndef SIGNALS_H
#define SIGNALS_H
#include<iostream>
#include<stdio.h>

using namespace std;

extern pid_t foreground_pid;
extern bool background;
void setup_signal_handlers();
void handle_sigstp(int);
void handle_sigint(int);
void handle_eof(int);

#endif
