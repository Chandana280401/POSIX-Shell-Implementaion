#include <iostream>
#include "pwd.h"
#include <unistd.h>
#include <string.h>
#include<stdio.h>

using namespace std;
void get_mycwd(){
   // cout<<"Inside getcwd"<<endl;
     if(getcwd(curr_dir,sizeof(curr_dir))==NULL){
        perror("Error getting working dir\n");
        return;
    }
    // cout<<"home:"<<home_dir_buff<<" "<<strlen(home_dir_buff)<<endl;
    // cout<<"curr_dir:"<<curr_dir<<" "<<strlen(curr_dir)<<endl;
    if(strcmp(home_dir_buff,curr_dir)==0){
       // strcpy(display_dir,"~");
       cout<<home_dir_buff<<endl;
    }
    else{
        int i=0,j=0;
        while(home_dir_buff[i]==curr_dir[j]){
            i++;j++;
        }
        i=0;
        //strcpy(display_dir,"~");
        while(curr_dir[j]){
           // display_dir[i++]=curr_dir[j];
            cout<<curr_dir[j];
            j++;
        }
        cout<<endl;
    }
    return;
    
}
