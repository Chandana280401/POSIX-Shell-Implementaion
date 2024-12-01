// pwd.h
#ifndef PWD_H
#define PWD_H
#include<limits.h>

extern char home_dir_buff[PATH_MAX]; 
extern char curr_dir[PATH_MAX]; 
extern char display_dir[PATH_MAX];
void get_mycwd();


#endif // PWD_H