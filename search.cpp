#include<dirent.h>
#include<iostream>
#include<string>
#include<cstring>
#include <sys/stat.h>

using namespace std;

bool loop_search(string &dir_name,string &search_item){
    DIR *dir=opendir(dir_name.c_str());
    if(dir==NULL){
        cerr<<"Error while opening directory "<<dir_name<<endl;
        return false;
    }

    struct dirent* entry;
    while((entry=readdir(dir))!=NULL){
        string entry_name=entry->d_name;

        //if search item matches with current item
        if(entry_name == search_item){
             // cout<<dir_name<<endl;
              return true;
        }

        if(entry->d_type == DT_DIR && entry_name!="." && entry_name!=".."){
            string new_dir_name= dir_name + "/"+ entry_name;
            if(loop_search(new_dir_name, search_item))
                return true;
        }
    }
    return false;
}