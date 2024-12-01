#include<iostream>
#include<string>
#include<bits/stdc++.h>
#include<vector>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include <sys/wait.h>
#include<fcntl.h>
#include "redirection.h"
#include "pipe_redirect.h"
using namespace std;

void out_redirect(vector<string> cmnd_args){ //>

    pid_t pid = fork();
    if(pid<0){
        cerr<<"Fork Failed"<<endl;
        exit(1);
    }
    int ip_fd,op_fd;
    if(pid==0){
       // cout<<"In child process\n";
        vector<char *> temp_args;
        string redirect;
        for(auto& arg: cmnd_args){
                if(arg!= ">" && arg!= ">>")
                    temp_args.push_back(const_cast<char*>(arg.c_str()));
                else{
                    redirect=arg;
                    break;
                }  
        }
        
        
        char* command=temp_args[0];
        char* inp_file;
        if(temp_args.size()>1){
                //cout<<"debug2"<<endl;
                inp_file=temp_args[1];
                struct stat filestat;
                if(stat(inp_file,&filestat)==0 && S_ISREG(filestat.st_mode)){
                    //it is a valid input file
                    //cout<<"Is reglar file\n";
                    ip_fd=open(inp_file, O_RDONLY);
                    if(ip_fd<0){
                        perror("Error opening input file\n");
                        return;
                    }
                
                }
        }
        temp_args.push_back(NULL);
        //debugging..this loop is not getting executed
        // for(int i=0;i<temp_args.size();i++)
        //     {
        //         cout<<temp_args[i]<<" ";
        //     }
        
        char* out_file=const_cast<char*>(cmnd_args[cmnd_args.size()-1].c_str());
        
        
        if( redirect== ">"){
            op_fd=open(out_file,O_WRONLY | O_CREAT | O_TRUNC, 0644); 
           // cout<<"debug4"<<endl;
        } 
        else if( redirect== ">>"){
            op_fd=open(out_file,O_WRONLY | O_CREAT | O_APPEND, 0644); 
            //cout<<"debug5"<<endl;
        }

        if(op_fd<0){
            perror("Error opening output file\n");
            return;
        }
        else{
           // cout<<"debug 6"<<endl;
           // cout<<command<<endl;
           
           // cout<<endl;
            if (dup2(op_fd, STDOUT_FILENO) >= 0) {
              //  cout<<"before execvpppp\n";
                execvp(command,temp_args.data());
                // perror("dup2 output");
                close(ip_fd);
                close(op_fd);
                // exit(EXIT_FAILURE);
            }
           // cout<<"debug9\n";
        }
    // char *args[]={"echo","Hello",NULL};
        close(ip_fd);
       // close(op_fd);
       // cout<<"debug 7"<<endl;
       // cout<<"before execvp\n";
        
       // cout<<"debug8\n";
        perror("execvp failed\n");
        return;
    }
    else{
       
         waitpid(pid,nullptr,0); 
       // cout<<"In parent process\n";
       // int status;
        close(ip_fd);
        close(op_fd);
        return;
    }
}

void inp_redirect(vector<string>cmnd_args){ // if only < is present
    pid_t pid= fork();
    if(pid<0){
        cerr<<"Fork Failed"<<endl;
        exit(1);
    }
    if(pid==0){
      //  cout<<"In child\n";
        vector<char *> temp_args;
        string redirect;
        char* command=const_cast<char*>(cmnd_args[0].c_str());
        char* inp_file=const_cast<char*>(cmnd_args[cmnd_args.size()-1].c_str());
        temp_args.push_back(command);
        temp_args.push_back(inp_file);
        temp_args.push_back(NULL);

        int ip_fd=open(inp_file, O_RDONLY);
        close(ip_fd);
       // cout<<"bef\n";
        execvp(command,temp_args.data());
        perror("execvp failed\n");
        return;
    }
    else{
        //cout<<"In parent\n";
        int status;
         waitpid(pid,&status,0);
        return;
    }
}

void io_redirect(vector<string> cmnd_args){ // if both < and > are present
    pid_t pid=fork();
    if(pid<0){
        cerr<<"Fork Failed"<<endl;
        exit(1);
    }
    if(pid==0){
        //cout<<"In child\n";
        int i=0,idx1,idx2;
        char* command=const_cast<char*>(cmnd_args[0].c_str());
        char* inp_file,* out_file;
        string redirect;

        for(i=0;i<cmnd_args.size();i++){
            if(cmnd_args[i]=="<"){
                redirect=cmnd_args[i];
                idx1=i;
                break;
            }
        }
        for(i=0;i<cmnd_args.size();i++){
            if(cmnd_args[i]==">" || cmnd_args[i]==">>"){
                redirect=cmnd_args[i];
                idx2=i;
                break;
            }
        }
        int ip_fd,op_fd;
        struct stat filestat;
        inp_file=const_cast<char*>(cmnd_args[idx1+1].c_str());
        if(stat(inp_file,&filestat)==0 && S_ISREG(filestat.st_mode)){
        //it is a valid input file
            ip_fd=open(inp_file, O_RDONLY);
            if(ip_fd<0){
                perror("Error opening input file\n");
                return;
            }
        }
        out_file=const_cast<char*>(cmnd_args[idx2+1].c_str());
        if( redirect== ">")
            op_fd=open(out_file,O_WRONLY | O_CREAT | O_TRUNC, 0644);  
        else if( redirect== ">>")
            op_fd=open(out_file,O_WRONLY | O_CREAT | O_APPEND, 0644);

        if(op_fd<0){
            perror("Error opening output file\n");
            return;
        }
        else{
            if (dup2(op_fd, STDOUT_FILENO) < 0) {
                perror("dup2 output");
                close(ip_fd);
                close(op_fd);
                exit(EXIT_FAILURE);
            }
        }
        vector<char*>temp_args;
        temp_args.push_back(command);
        temp_args.push_back(inp_file);
        temp_args.push_back(NULL);
        close(ip_fd);
        close(op_fd);
        execvp(command,temp_args.data());
        perror("execvp failed\n");
        return;
    }
    else{
        //cout<<"In parent\n";
        int status;
         waitpid(pid,&status,0);
        return;
    }
}