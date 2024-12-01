#include<iostream>
#include<unistd.h> //for chdir
#include<cstdio> //for perror
#include<bits/stdc++.h>
#include "cd.h"
using namespace std;
void my_cd(string arg){
    if(arg== ".."){
        if(strcmp(home_dir_buff,curr_dir)==0){
            cout<<home_dir_buff<<endl;
            return;
        }
        else{
            if(chdir("..")!=0){
                perror("error while changing to parent directory\n");
                return;
            }
            else{
                strcpy(prev_dir,curr_dir);
                getcwd(curr_dir,sizeof(curr_dir));
            }   
        }
    }
    else if(arg=="."){
        return;
    }
    else if(arg == "~"){
        if(chdir(home_dir_buff)!=0){
            perror("Error while executing cd ~ command\n");
            return;
        }
        else{
            strcpy(prev_dir,curr_dir);
            getcwd(curr_dir,sizeof(curr_dir));
        }
    }
    else if(arg == "-"){
        if(chdir(prev_dir)!=0){
            perror("Error while executing cd - command\n");
            return;
        }
        else{
            strcpy(prev_dir,curr_dir);
            getcwd(curr_dir,sizeof(curr_dir));
        }
    }
    else{ //given a path
        
        if(chdir(arg.c_str())!=0){
            perror("Error while changing the directory\n");
            return;
        }
        else{
            strcpy(prev_dir,curr_dir);
            getcwd(curr_dir,sizeof(curr_dir));
        }
    }
    // to change the display dir in case of dir changed by cd .., cd ~, cd -, cd path
    if(strcmp(home_dir_buff,curr_dir)==0)
        strcpy(display_dir,"~");
    else{
        int i=0,j=0;
        while(home_dir_buff[i]==curr_dir[j]){
            i++;j++;
        }
        strcpy(display_dir,"~");
        i=0;
        string temp="~";
        while(curr_dir[j]){
            temp+=curr_dir[j];
            j++;
        }
        strcpy(display_dir,temp.c_str());
    }
}
