#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include "history.h"

using namespace std;

const string filename = "/home/chandana/Desktop/2024201060_A2/history.txt"; // Define the filename as a constant

void save_to_history(string& cmnd) {
    // Read existing history into a vector
    vector<string> write_vec;
    ifstream inputFile(filename);

    if (inputFile.is_open()) {
       // cout << "file opened for reading\n";
        string line;
        while (getline(inputFile, line)) {
            write_vec.push_back(line);
        }
        inputFile.close();
       // cout << "file closed after reading\n";
    } 
    else {
        perror("Error opening file for reading\n");
        return;
    }

    // history size should not exceed 20
    if (write_vec.size() == 20) {
        write_vec.erase(write_vec.begin()); // Remove the oldest entry
    }
    write_vec.push_back(cmnd); // Add the new command

    // Write the updated history back to the file
    ofstream outputFile(filename, ios::trunc);
    if (outputFile.is_open()) {
        for (const auto& command : write_vec) {
            outputFile << command << endl;
        }
        outputFile.close();
       // cout << "Command saved successfully" << endl;
        //cout<<write_vec.size()<<endl;
    }
    else {
        perror("Error opening file for writing\n");
    }
}

void read_history(int n) {
    ifstream file(filename);
    if (!file.is_open()) {
        perror("Error opening file for reading\n");
        return;
    }

    vector<string> read_vec;
    string hist_cmnd;
    while (getline(file, hist_cmnd)) {
        read_vec.push_back(hist_cmnd);
    }
    file.close();

    //if no value of n is given ,I took it as n=0
    if (n <= 0) {
        n = 10; // Default to 10 if n is not positive
    }

    // Print the desired number of lines from the end of the history
    if (n >= read_vec.size()) {
        for (const auto& command : read_vec) {
            cout << command << endl;
        }
    } else {
        for (int i = read_vec.size() - n; i < read_vec.size(); ++i) {
            cout << read_vec[i] << endl;
        }
    }
}