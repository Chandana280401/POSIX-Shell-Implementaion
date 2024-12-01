#include<iostream>
#include<dirent.h>
#include<string>
#include<cstring>
#include<unistd.h>
#include<sys/stat.h>
#include<bits/stdc++.h>
#include<pwd.h>
#include<grp.h>
#include "ls.h"

using namespace std;

void file_permissions(mode_t f_mode){
    //user permissions
    cout<<((f_mode && S_IRUSR)?'r':'-')
        <<((f_mode && S_IWUSR)?'w':'-')
        <<((f_mode && S_IXUSR)?'x':'-');
    //group permissions
    cout<<((f_mode && S_IRGRP)?'r':'-')
        <<((f_mode && S_IWGRP)?'w':'-')
        <<((f_mode && S_IXGRP)?'x':'-');
    //other permissions
    cout<<((f_mode && S_IROTH)?'r':'-')
        <<((f_mode && S_IWOTH)?'w':'-')
        <<((f_mode && S_IXOTH)?'x':'-');
    cout<<" ";
}
void print_ls(string path, bool flag_a,bool flag_l){
    struct stat path_stat;
    if(stat(path.c_str(), &path_stat)==-1){
        perror("Error:unable to access path in ls command\n");
        return;
    }
    else{
        if(S_ISDIR(path_stat.st_mode)){ //if its a directory
            DIR* dir=opendir(path.c_str());
            if(dir==NULL){
                perror("Error while accessing the directory path\n");
                return;
            }
            struct dirent* entry;
            vector<string>entries;
            while((entry=readdir(dir))!=NULL){
                entries.push_back(entry->d_name);
            }
            closedir(dir);
            sort(entries.begin(),entries.end());
            if(flag_a==false){
                entries.erase(entries.begin(), entries.begin()+2);
            }
            if(flag_l==false){
                cout<<path<<":"<<endl;
                for(int i=0;i<entries.size();i++)
                    cout<<entries[i]<<endl;
            }
            else{ //long format
                    cout<<path<<":"<<endl;
                   for(const auto& e:entries){
                        string full_path=path+"/"+e;
                        struct stat file_stat;
                        if(stat(full_path.c_str(),&file_stat)==-1){
                            perror("Error while fetching the status of file");
                            cout<<full_path<<endl;
                            continue;
                        }
                        if(S_ISDIR(file_stat.st_mode))
                            cout<<"d";
                        else if(S_ISREG(file_stat.st_mode))
                            cout<<"-";
                        //print file permissions
                        file_permissions(file_stat.st_mode);
                        //print no.of hard links
                        cout<<file_stat.st_nlink<<" ";
                        // print the ower and group
                        struct passwd* usr=getpwuid(file_stat.st_uid); //getting user details
                        if(usr==NULL){
                            perror("Error while trying to fetch user name of");
                            cout<<full_path<<endl;
                            continue;
                        }
                        cout<<usr->pw_name<<" ";
                        struct group* grp=getgrgid(file_stat.st_gid);
                        if(grp==NULL){
                            perror("Error while trying to fetch group name of");
                            cout<<full_path<<endl;
                            continue;
                        }
                        cout<<grp->gr_name<<" ";
                        //print the size
                        cout<<file_stat.st_size<<" ";
                        //print modification time
                        struct tm* t_info;
                        char t_buff[80];
                        t_info=localtime(&file_stat.st_mtime);
                        strftime(t_buff,sizeof(t_buff),"%b %d %H:%M ",t_info);
                        cout<<t_buff<<" ";
                        //print file_name
                        cout<<e<<endl;
                   }
            }
        }
        else if(S_ISREG(path_stat.st_mode)){ //if it is a file
            if(flag_l==false) {//only print filename (only flag_a)
                cout<<path<<endl;
            }
            else{// print file details
                    cout<<path<<":"<<endl;
                    cout<<"-"; //indicates it is a reg file
                    //file permissions
                    file_permissions(path_stat.st_mode);
                    //print no.of hard links
                    cout<<path_stat.st_nlink<<" ";
                    //print owner and group names
                    struct passwd* usr=getpwuid(path_stat.st_uid); //getting user details
                    if(usr==NULL){
                        perror("Error while trying to fetch user name of");
                        cout<<path<<endl;
                        return;
                    }
                    cout<<usr->pw_name<<" ";
                    struct group* grp=getgrgid(path_stat.st_gid);
                    if(grp==NULL){
                        perror("Error while trying to fetch group name of");
                        cout<<path<<endl;
                        return;
                    }
                    cout<<grp->gr_name<<" ";
                    //print file size
                    cout<<path_stat.st_size<<" ";
                    //print modification time
                    struct tm* t_info;
                    char t_buff[80];
                    t_info=localtime(&path_stat.st_mtime);
                    strftime(t_buff,sizeof(t_buff),"%b %d %H:%M ",t_info);
                    cout<<t_buff<<" ";
                    //print filename
                    cout<<path<<endl;
            }
        }
    }
}
void my_ls(vector<string> args){
    bool flag_a=false,flag_l=false;
    
    if(args.size()==1){//only ls or ls <file_name/dir_name>
            print_ls(".",flag_a,flag_l);
            return;
    } 
    int flag=0;
    for(int i=1;i<args.size();i++){
        if(args[i]== "-a")
            flag_a=true;
        else if(args[i] == "-l")
            flag_l=true;
        else if(args[i] == "-al" || args[i] == "-la"){
            flag_a=true;
            flag_l=true;
        }
        else{ //consider it as dir or file path
           print_ls(args[i],flag_a,flag_l);
           flag=1;
        //    flag_a=false;
        //    flag_l=false;
        }
    }
    if(flag==0){
        print_ls(".",flag_a,flag_l);
    }
}