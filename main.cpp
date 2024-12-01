#include <iostream>
#include <unistd.h>  // For getlogin, gethostname, getcwd
#include <limits.h>  // For HOST_NAME_MAX
#include <cstring>   // For strerror
#include <sys/types.h>
#include <cstdio> //for peeror
#include <string>
#include <fstream>
#include "pwd.h"
#include "cd.h"
#include "echo.h"
#include "ls.h"
#include "history.h"
#include "background.h"
#include "search.h"
#include "pinfo.h"
#include "redirection.h"
#include "pipeline.h"
#include "pipe_redirect.h"
#include "signals.h"
//#include "uparraow.h"
using namespace std;

char home_dir_buff[PATH_MAX];
char curr_dir[PATH_MAX];
char display_dir[PATH_MAX];
char prev_dir[PATH_MAX];
string curr_cmnd;
string display;
pid_t foreground_pid=-1;
pid_t main_pid;
bool background=false;
//const string hist_file ="/home/chandana/Desktop/2024201060_A2/history.txt";

void handleZ(int num)
{
    if (foreground_pid != main_pid)
    {
        cout << "Terminated\n";
        kill(foreground_pid, SIGCONT);
        foreground_pid = main_pid;
    }
    if(foreground_pid==main_pid)
        cout << foreground_pid << endl;
     cout << foreground_pid << endl;
}

void handleC(int num)
{
    if (foreground_pid != -1)
    {
        cout << "Interrupted\n";
        kill(foreground_pid, SIGINT);
        foreground_pid = 0;
    }
}

vector<string> split_into_args(){
    vector<string> cmnd_args;
    char* inp_cmnd=new char[curr_cmnd.size()+1];
    string temp_cmnd=curr_cmnd;
    strcpy(inp_cmnd,temp_cmnd.c_str()); 
    char* arg= strtok(inp_cmnd," ");
    while(arg){
       // cout<<arg<<endl;
        cmnd_args.push_back(arg);
        arg=strtok(nullptr," ");
    }
    delete[] inp_cmnd;
    return cmnd_args;
}

vector<string> split_cmnd(string cmnd){
        vector<string>my_commands;
       
        char* inp_cmnd= new char[cmnd.size()+1]; //allocate buffer and copy the cmnd into it
        strcpy(inp_cmnd,cmnd.c_str()); 

         //c_str() returns a const char* value. So we cannot assign a const char* value directly to char*. So we use strcpy()
        //char* strtok(char* str, const char* delimiters);
        //for strtok() first we need to pass the input string and then after generating the first token, it inserts
        //'\0 at end of token. form next time we have to pass nullpointer as input to strtok() to get the token

        char* token= strtok(inp_cmnd,";");
        while(token){
            my_commands.push_back(token);
            token=strtok(nullptr, ";");
        }
        delete[] inp_cmnd;
        return my_commands;
}

string trim_cmd(string command){
    size_t idx1=command.find_first_not_of(" \n\r\t"); //trimming the front spaces
    command= command.substr(idx1);
    size_t idx2=command.find_last_not_of(" \n\r\t"); //this gives last index of character that is not equal to whitespcae/tab/newline
    command.erase(idx2+1);
    return command;
}

void executecmnd(string &command){

    //remove unnecessory white spaces 
    curr_cmnd=trim_cmd(command);
    if(!curr_cmnd.empty() && curr_cmnd.back()=='&'){
        background=true;
        curr_cmnd.pop_back();
        curr_cmnd=trim_cmd(curr_cmnd);
    }
    vector<string>cmnd_args;
    cmnd_args=split_into_args();
    if(curr_cmnd.find("|")!=string::npos && (curr_cmnd.find(">")!=string::npos || curr_cmnd.find("<")!=string::npos)){
       // cout<<"In pipe_redirect"<<endl;
        pipe_redirect();
        return;
    }
    else if(curr_cmnd.find("|")!=string::npos){
       // cout<<"In pipeline if"<<endl;
        my_pipe(cmnd_args);
        return;
    }
    else if(curr_cmnd.find("<")!=string::npos || curr_cmnd.find(">")!=string::npos){
        //cout<<"In if of redirect\n";
        if(curr_cmnd.find(">")!=string::npos && curr_cmnd.find("<")!=string::npos)
            io_redirect(cmnd_args);
        else if(curr_cmnd.find(">")!=string::npos)
            out_redirect(cmnd_args);
        else if(curr_cmnd.find("<")!=string::npos)
            inp_redirect(cmnd_args);
        return;
    }
    else if( curr_cmnd == "pwd"){
        if(cmnd_args.size()==1 )
            get_mycwd();
        else{
            perror("Check number of arguments\n");
        }
        return;
    }
    else if(cmnd_args[0]== "cd"){
        if(cmnd_args.size()!=2)
        {
            perror("Invalid number of arguments for cd command\n");
            return;
        }
        my_cd(cmnd_args[1]);
        return;
    }
    else if(cmnd_args[0]== "echo"){
       // cout<<"In echo\n";
        my_echo();
        return;
    }
    else if(cmnd_args[0]== "ls"){
        my_ls(cmnd_args);
        return;
    }
    else if(cmnd_args[0]=="search"){
        if(cmnd_args.size()>2)
        {
            perror("Check the number of arguments for search command\n");
            return;
        }
        string search_curr_dir=".";
        if(loop_search(search_curr_dir,cmnd_args[1]))
            cout<<"True"<<endl;
        else
            cout<<"False"<<endl;
        return;
    }
    else if(cmnd_args[0]=="history"){
        if(cmnd_args.size()>2){
            perror("Check the no.of arguments of history command\n");
            return;
        }
        else if(cmnd_args.size()==1)
        read_history(0);
        else
        read_history(stoi(cmnd_args[1]));
    }
    else if(cmnd_args[0]=="pinfo"){
        if(cmnd_args.size()>2)
        {
            cout<<"Invalid number of arguments\n";
            return;
        }
        if(cmnd_args.size()==1){
            int process_id=getpid();
            printMemoryInfo(process_id);
            getExecutablePath(process_id);
            return;
        }
        my_pinfo(cmnd_args[1]);
        return;
    }
    else{
        if(background){
           // cout<<"background main"<<endl;
            runBackgroundProcess(cmnd_args[0],cmnd_args);
            return;
        }
        else{
            //cout<<"foregrounf main"<<endl;
            runForegroundProcess(cmnd_args[0],cmnd_args);
            return;
        }
    }
}

int main() {
    // Get username
    const char* username = getlogin();
    if (username == nullptr) {
        perror("Error retrieving username\n");
        username = "Unknown";
    }

    // Get hostname
    char hostname[HOST_NAME_MAX];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        perror("Error retrieving hostname\n");
        strcpy(hostname, "Unknown");
    }
    strcpy(display_dir,"~");
    getcwd(home_dir_buff,sizeof(home_dir_buff));
    strcpy(prev_dir,home_dir_buff);
    strcpy(curr_dir,home_dir_buff);

    // setup_signal_handlers(); 
    signal(SIGINT, handleC);
    signal(SIGCONT, handleZ);

    main_pid=getpid();
    foreground_pid=main_pid;
    //cout<<main_pid<<" "<<foreground_pid<<endl;

    while(true){
        display= string(username)+"@"+hostname+ ":"+display_dir+">";
        cout<<display;
        //char ch = getchar();
        
        string cmnd;
        getline(cin, cmnd);
        if(cin.eof()==1){
            exit(0);
        } 
        if(!cmnd.empty()){
            save_to_history(cmnd);
        }
        if(cmnd == "exit")
        {
            exit(0);
        } 
        vector<string> cmnd_args= split_cmnd(cmnd);
        for(auto cmd:cmnd_args){
            executecmnd(cmd);
        }
    }
    return 0;
}

