#include<iostream>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<bits/stdc++.h>
#include "echo.h"
using namespace std;

void my_echo(){
    bool flag=false;
    for(int i=5;i<curr_cmnd.size();i++){
        if(curr_cmnd[i]=='"')
            flag!=flag;
        else if(curr_cmnd[i]==' '){
            if(flag==true){
                continue;
            }
            else{
                while(curr_cmnd[i]==' ')
                   i++;
                cout<<" ";
            }    
        }
        cout<<curr_cmnd[i];
    }
    cout<<endl;
}               