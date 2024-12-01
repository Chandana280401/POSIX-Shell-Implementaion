// #include <iostream>
// #include <fstream>
// #include <string>
// #include "pinfo.h"
// void printProcessInfo(pid_t pid) {
//     std::cout<<"In Pinfo"<<endl;
//     std::string path = "/proc/" + std::to_string(pid) + "/status";
//     std::ifstream file(path);

//     if (!file.is_open()) {
//         std::cerr << "Error: Unable to open " << path << std::endl;
//         return;
//     }

//     std::string line;
//     while (std::getline(file, line)) {
//         std::cout << line << std::endl;
//     }
// }

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include "pinfo.h"
using namespace std;
// Function to get the executable path of the process
void getExecutablePath(int pid) {
    ostringstream pathStream;
    pathStream << "/proc/" << pid << "/exe";
    string path = pathStream.str();
    
    char buf[1024];
    ssize_t len = readlink(path.c_str(), buf, sizeof(buf) - 1);
    if (len != -1) {
        buf[len] = '\0';
        cout<<"Executable path-->"<<string(buf)<<endl;
    } 
    else {
        cout<<"Unable to retrieve executable path"<<endl;
    }
}

// Function to get the memory allocated to the process
void printMemoryInfo(int pid) {
    ostringstream pathStream;
    pathStream << "/proc/" << pid << "/status";
    ifstream statusFile(pathStream.str());
    
    if (!statusFile.is_open()) {
        cerr << "Error: Unable to open " << pathStream.str() << endl;
        return;
    }

    string line;
    while (getline(statusFile, line)) {
        if (line.find("Pid:")==0){
            cout<<"pid-->"<<line.substr(4)<<endl;
        }
        if(line.find("State:")==0){
            if(background==false) //foreground
            cout<<"Process Status-->"<<line.substr(6)<<"+"<<endl;
            else
            cout<<"Process Status-->"<<line.substr(6)<<endl;
        }
        if(line.find("VmSize:") == 0) {
            cout<<"Memory-->"<<line.substr(7)<<endl;
        }
    }
}
void my_pinfo(string process_id){
    int pid= stoi(process_id);
    printMemoryInfo(pid);
    getExecutablePath(pid);
}
// int main() {
//     pid_t pid;
//     std::cout << "Enter PID: ";
//     std::cin >> pid;
    
//     // Get and print the executable path
//     std::string exePath = getExecutablePath(pid);
//     std::cout << "Executable Path: " << exePath << std::endl;
    
//     // Get and print memory information
//     printMemoryInfo(pid);
    
//     return 0;
// }
