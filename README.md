main.cpp

->For the terminal display, to get username and systemname, I have use 'getlogin()' and 'gethostname()' and also initially obtaining hime_directory path using 'getcwd()' and storing it  global variable.
->Reading the command from terminal using getline(), and each time a command is read it is being saved into history using save_to_hist() funtion in main().
->The command input is passed to a funtion to split it on the space delimeter to get the individual arguments of the input command.
->Based on the 1st argument which is the command name , the control gets redirected to its specific module to execute the command.
->All the modules are linked to main module using header files.
->All the custom implemented commands have their own modules to execute the command.

executecmnd(string command)--> decides which module to invoke based on the command name.
string trim_cmd(string command)-->trims the extra spaces present in the command.
vector<string> split_into_args()-->splits the input command into arguments on delimeter " " using strtok().
vector<string> split_cmnd(string cmnd)-->If the input given has more than one commands seperated by ";", this command splits them into individual commands using strtok().

strtok()-->used to convert the string to tokens based on the seperation delimeter provided.

-------------------------------------------------------------------------------------------------------------
cd.cpp

->For the argument after the cd, each case is being handled seperately.
->Whenever there is a change in directory, also updating the display of the terminal.

-------------------------------------------------------------------------------------------------------------

pwd.cpp, echo.cpp

->In pwd.cpp , we use getcwd() to get the current working directory of out code.
->Custom module for echo command, which ignores spaces within the quotes and trims away extra spaces.

-------------------------------------------------------------------------------------------------------------

ls.cpp

->Have handles in the format of ls command, except that it doesn't print total value
-> Both the flags -l, -a are handled, and also handled for multiple direcotries and multiple flags

-------------------------------------------------------------------------------------------------------------
search.cpp

->It searches for a file or folder recursively within the current directory.
->It first finds the directory name in which it is present and reads each entry to compare it with file / folder name which need to found.

-------------------------------------------------------------------------------------------------------------
redirection.cpp

-> I'm checking whether the current cmnd contains both "<" and ">", so then io_redirect() gets invoked
-> else if it's only one operator either "<" or ">" , then the respective function gets called.

-------------------------------------------------------------------------------------------------------------

pipeline.cpp

-> all the line input is divided based on '|' and then each command is executed using execvp() and every adjacent commands are interlinked using pipes.
pipefd[0]-->allows to read input
pipefd[1]-->allows to write output

------------------------------------------------------------------------------------------------------------
history.cpp

--> Implemented using a external file and a buffer vector.
-->so whenever a command is entered, it gets saved to history file and when the size crosses 20 cmnds, it erases the begining one and adds new cmnd to last.
-----------------------------------------------------------------------------------------------------------..
background.cpp

-->if the command contains & at the end , it is treated as background process and identified using a flag called background.
-->When the process is background, it prints the pid on screen

------------------------------------------------------------------------------------------------------------
pinfo.cpp

->it gives the process id, status, and execution path of given process id using stat system call.

------------------------------------------------------------------------------------------------------------
signals

->signals are handled in main function itself.
