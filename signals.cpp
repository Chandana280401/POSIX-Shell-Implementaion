#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <cstring>
#include <cstdlib>
#include "signals.h"

using namespace std;

//pid_t foreground_pid = -1;  // Global variable to keep track of the foreground process ID

// Signal handler for SIGTSTP (CTRL-Z)
void handle_sigstp(int signal) {
     cout<<"in ctrl+z"<<endl;
    if (foreground_pid > 0) {
        cout << "\nPushing foreground job " << foreground_pid << " to background" << endl;
        background=true;
        cout<<foreground_pid<<endl;
        kill(foreground_pid, SIGTSTP);  // Send SIGTSTP to stop the foreground process
        foreground_pid = -1;  // Reset the foreground PID
    }
}

// Signal handler for SIGINT (CTRL-C)
void handle_sigint(int signal) {
    cout<<"in ctrl+c"<<endl;
    if (foreground_pid > 0) {
        cout << "\nInterrupting foreground job " << foreground_pid << endl;
        kill(foreground_pid, SIGINT);  // Send SIGINT to interrupt the foreground process
        foreground_pid = -1;  // Reset the foreground PID
    }
}

// Signal handler for EOF (CTRL-D)
void handle_eof(int signal) {
     cout<<"in ctrl+d"<<endl;
    // Terminate the shell (simulate logout)
    kill(foreground_pid,SIGTSTP);
    cout << "\nLogging out of the shell" << endl;
    exit(0);
}

void setup_signal_handlers() {
    struct sigaction sa;
    cout<<"in setup handler"<<endl;
    // Setup SIGTSTP (CTRL-Z) handler
    memset(&sa, 0, sizeof(sa));
    cout<<"in memset z"<<endl;
    sa.sa_handler = handle_sigstp;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGTSTP, &sa, NULL) == -1) {
        perror("sigaction SIGTSTP");
        return;
    }

    // Setup SIGINT (CTRL-C) handler
    memset(&sa, 0, sizeof(sa));
    cout<<"in memset c"<<endl;
    sa.sa_handler = handle_sigint;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction SIGINT");
        return;
    }   
    // Setup SIGTERM (for logout simulation, similar to EOF)
    memset(&sa, 0, sizeof(sa));
    cout<<"in memset d"<<endl;
    sa.sa_handler = handle_eof;
    if (sigaction(SIGTERM, &sa, NULL) == -1) {  // Use SIGTERM to simulate logout behavior
        perror("sigaction SIGTERM");
        exit(1);
    }
}

// int main() {
//     setup_signal_handlers();

//     cout << "Shell running. Press CTRL-Z, CTRL-C or send SIGTERM to test signal handling." << endl;

//     while (true) {
//         // Main shell loop. Simulate work by sleeping.
//         sleep(1);
//     }

//     return 0;
// }
