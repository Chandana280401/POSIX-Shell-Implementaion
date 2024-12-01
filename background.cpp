#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <string>
#include "background.h"
using namespace std;

vector<char*> convertArgs(vector<string>& cmnd_args) {
    vector<char*> args;
    for (auto& arg : cmnd_args) {
        args.push_back(const_cast<char*>(arg.c_str()));
    }
    args.push_back(nullptr); // Null-terminate the array
    return args;
}
void runForegroundProcess(string& cmnd_name,vector<string>& cmnd_args) {
    background=false;
    
    pid_t pid = fork(); // Create a new process
    if (pid < 0) {
        cerr << "Fork failed" << endl;
        exit(1);
    }
    if (pid == 0) {
        // Child process
        // foreground_pid=pid;
       

        vector<char*> args = convertArgs(cmnd_args);
        execvp(cmnd_name.c_str(), args.data());
        // If execvp fails
        cerr << "execvp failed" << endl;
        exit(1);
    } 
    else {
        
        foreground_pid=getpid();
       // cout<<main_pid<<" "<<foreground_pid<<endl;
        //int status;
        waitpid(pid,nullptr,0);
        foreground_pid=main_pid;
    }
}
void runBackgroundProcess(string& cmnd_name,vector<string>& cmnd_args) {
   /// cout << "In background" << endl;
   background=true;
    pid_t pid = fork(); // Create a new process
    if (pid < 0) {
        cerr << "Fork failed" << endl;
        exit(1);
    }
    if (pid == 0) {
        //child Process
        vector<char*> args = convertArgs(cmnd_args);
        execvp(cmnd_name.c_str(), args.data());
        // If execvp fails
        cerr << "execvp failed" << endl;
        exit(1);
    } 
    else {
        // Parent process
        // The parent process does not wait for the child process
        sleep(1);
        cout<<pid<<endl;
       // background=false;
    }
}