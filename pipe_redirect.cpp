#include <iostream>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>
#include "redirection.h"
#include "pipeline.h"
#include "pipe_redirect.h"

using namespace std;
vector<string> split_into_args_space(string command){
    vector<string> cmnd_args;
    char* inp_cmnd=new char[command.size()+1];
    string temp_cmnd=command;
    strcpy(inp_cmnd,temp_cmnd.c_str()); 
    char* arg= strtok(inp_cmnd," ");
    while(arg){
       // cout<<arg<<endl;
        cmnd_args.push_back(arg);
        arg=strtok(nullptr, " ");
    }
    delete[] inp_cmnd;
    return cmnd_args;
}
vector<string> split_into_args_pipe(){
    vector<string> cmnd_args;
    char* inp_cmnd=new char[curr_cmnd.size()+1];
    string temp_cmnd=curr_cmnd;
    strcpy(inp_cmnd,temp_cmnd.c_str()); 
    char* arg= strtok(inp_cmnd,"|");
    while(arg){
       // cout<<arg<<endl;
        cmnd_args.push_back(arg);
        arg=strtok(nullptr, "|");
    }
    delete[] inp_cmnd;
    return cmnd_args;
}
void pipe_redirect() {
    vector<string> command_splits = split_into_args_pipe();
    vector<vector<string>> pipe_cmnds;

    for (const auto& command_split : command_splits) {
        pipe_cmnds.push_back(split_into_args_space(command_split));
    }

    int pipefd[pipe_cmnds.size() - 1][2];  // Array to hold pipe file descriptors
    for (int i = 0; i < pipe_cmnds.size() - 1; ++i) {
        if (pipe(pipefd[i]) == -1) {
            perror("pipe failed");
            exit(EXIT_FAILURE);
        }
    }

    int ip_fd=STDIN_FILENO;
    for (int i = 0; i < pipe_cmnds.size(); ++i) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            if(ip_fd != STDIN_FILENO){
                dup2(ip_fd,STDIN_FILENO); 
                close(ip_fd);
            }
            // if (i > 0) {
            //     dup2(pipefd[i - 1][0], STDIN_FILENO);
            // }
            if (i < pipe_cmnds.size() - 1) {
                dup2(pipefd[i][1], STDOUT_FILENO);
            }
            // for (int j = 0; j < pipe_cmnds.size() - 1; ++j) {
            //     close(pipefd[j][0]);
            //     close(pipefd[j][1]);
            // }
            close(ip_fd);
            //close(op_fd);

            // Handle redirection
            string command_str = command_splits[i];
            if (command_str.find("<") != string::npos || command_str.find(">") != string::npos) {
                if (command_str.find("<") != string::npos && command_str.find(">") != string::npos) {
                    io_redirect(pipe_cmnds[i]);
                } else if (command_str.find(">") != string::npos) {
                    out_redirect(pipe_cmnds[i]);
                    waitpid(pid,nullptr,0);
                } else if (command_str.find("<") != string::npos) {
                    inp_redirect(pipe_cmnds[i]);
                }
                //exit(EXIT_SUCCESS);
            }
            else{
                vector<char*> args;
                for (const auto& arg : pipe_cmnds[i]) {
                    args.push_back(const_cast<char*>(arg.c_str()));
                }
                args.push_back(nullptr);

                execvp(args[0], args.data());
                perror("execvp failed");
                exit(1);
            }
        }
        else{
            waitpid(pid, nullptr, 0);
            if (pipefd[i][0] != STDIN_FILENO) {
                close(pipefd[i][0]); 
            }
            if (i < pipe_cmnds.size() - 1) {
                close(pipefd[i][1]); 
                ip_fd=pipefd[i][0];
            }
        }
    }

    for (int i = 0; i < pipe_cmnds.size() - 1; ++i) {
        close(pipefd[i][0]);
        close(pipefd[i][1]);
    }

    // for (int i = 0; i < pipe_cmnds.size(); ++i) {
    //     waitpid(pid,nullptr,0);
    // }
}