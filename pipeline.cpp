#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include "pipeline.h"
#include "pipe_redirect.h"

using namespace std;
void create_pipes(int pipefd[][2],int n){
    for(int i=0;i<n;i++){
        if(pipe(pipefd[i])==-1)
        {
            perror("pipe");
            exit(1);
        }
    }

}
void exe_pipe(vector<vector<string>> pipe_cmds,int ip_fd){

    //creating pipes
    int pipefd[pipe_cmds.size()][2];
    create_pipes(pipefd,pipe_cmds.size());

    //looping over all command statements
    for(int i=0;i<pipe_cmds.size();i++){

        //packing args for each cmnd statement
        char* args[pipe_cmds[i].size()+1];
        for(int j=0;j<pipe_cmds[i].size();j++)
            args[j]=const_cast<char*>(pipe_cmds[i][j].c_str()); 
        args[pipe_cmds[i].size()]=NULL;
  
        //fork the current process
        pid_t pid = fork();
        if(pid==-1) {
            perror("fork failed");
            exit(1);
        }
        if(pid==0) { //child process
            if(ip_fd != STDIN_FILENO){
                dup2(ip_fd,STDIN_FILENO); 
                close(ip_fd);
            }
            if (i< pipe_cmds.size()-1) {
                dup2(pipefd[i][1],STDOUT_FILENO); 
                close(pipefd[i][1]);
            }

            execvp(args[0],args);
            perror("execvp failed");
            exit(1);
        }
        else{ // Parent process
            waitpid(pid, nullptr, 0);
            if (ip_fd != STDIN_FILENO) {
                close(ip_fd); 
            }
            if (i < pipe_cmds.size() - 1) {
                close(pipefd[i][1]); 
                ip_fd=pipefd[i][0];
            }
        }
    }
}
void my_pipe(vector<string>cmnd_args){
    vector<vector<string>> pipe_cmds;
    vector<string> cmds;
    for(int i=0;i<cmnd_args.size();i++){
        if(cmnd_args[i]!= "|")
            cmds.push_back(cmnd_args[i]);
        else{
            pipe_cmds.push_back(cmds);
            cmds.clear();
        }
    }
    pipe_cmds.push_back(cmds);

    exe_pipe(pipe_cmds,STDIN_FILENO);
    return;
}	
