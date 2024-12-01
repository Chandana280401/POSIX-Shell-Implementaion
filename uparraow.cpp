#include <iostream>
#include <vector>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include "uparraow.h"
using namespace std;


// Function to get a single character from user input
char getch() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

// Function to handle special key sequences
void handle_input(std::string &current_input) {
    char ch = getchar();
    vector<string> history;
    ifstream file(hist_file);
    if (!file.is_open()){
        perror("Error opening file for reading\n");
        return;
    }
    
    string hist_cmnd;
    while (getline(file, hist_cmnd)){
        history.push_back(hist_cmnd);
    }
    file.close();
    int history_index=history.size();
    if (ch == 27) { // Escape sequence
        if (getch() == '[') {
            switch (getch()) {
                case 'A': // Up arrow
                    if (history_index > 0) {
                        history_index--;
                        current_input = history[history_index];
                        std::cout << "\r" << std::string(current_input.length(), ' ') << "\r" << current_input;
                    }
                    break;
                case 'B': // Down arrow
                    if (history_index < history.size() - 1) {
                        history_index++;
                        current_input = history[history_index];
                        std::cout << "\r" << std::string(current_input.length(), ' ') << "\r" << current_input;
                    } else if (history_index == history.size()) {
                        history_index--;
                        current_input.clear();
                        std::cout << "\r" << std::string(current_input.length(), ' ') << "\r" << current_input;
                    }
                    break;
            }
        }
    } else {
        if (ch == '\n') { // Enter key
            history.push_back(current_input);
            history_index = history.size();
            std::cout << std::endl;
        } else {
            current_input += ch;
            std::cout << ch;
        }
    }
}

// int main() {
//     std::vector<std::string> history;
//     size_t history_index = 0;

//     std::cout << "Command Line Interface. Press Up Arrow to recall commands, Enter to execute, and Ctrl+C to exit." << std::endl;

//     while (true) {
//         std::string current_input;
//         std::cout << "> ";
//         while (true) {
//             handle_input(current_input, history, history_index);
//             if (current_input.empty() || current_input.back() == '\n') {
//                 break;
//             }
//         }
//     }

//     return 0;
// }
