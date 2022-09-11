##  Project Organization

The project consists of 8 files:

* main.c - Bibek Napit

    Main file that contains the main function and runs the code
* Major.h

    Main head that handles all the project components and connects them together. Additioanlly, Major.h handles our **exit** and **cd** command. The **exit** command is first searched through using the strstr command, which finds the "exit" needle in the haystack "line", which contains whatever the user inputted. Strstr is utilized several times throughout the code to ensure that it isn't missed, and if its ever found, we set exit_found to 1 and simply call exit. Additionally, if exit is the first element of our array holding the commands, we just exit right then and there, as we need go no further. **CD** is also controlled through Major.h, we pass the string holding our commands through to cd_command_handler, and if our first command is cd, we run through where the command array designates, and go there, error checking throughout the function.

* AliasHandler.h

    File has the functions that handles the alias. It defines  a  shortcut for commands  by  essentially  defining  a  new  command  that  substitutes  a  given string  for  some  command,  perhaps  with  various  flags/options. 


* History.h

    File that handles the myhistory command via a circular linked list.    When adding to list, if list is already 20 or more, the oldest element is overwritten. To handle command arguments, a series of if/else statements are used to determine which variation of arguments are used. If not part of the defined set, the command returns an error. This is the only built-in command to support I/O redirection and pipelining.
* MySignals.h

    Handling, with intializtion of the default signal handler, of both parent and child signals in certain scenarios, either ignoring parent signals or setting default child signals. This is done in order to avoid the closure of the shell through the utilization of ctrl+z/ctrl+c 
* Path.h

    File that contains the functions in order to either read the path of a command, determine whether a path is complete, determine whether a command stands to be executable, determine the general path of a specified command, add a path, remove a path,simply print the path of a command, or run a specified path command. 
path + and path - commands 
* Pipeline.h

    File has the functions which operates pipeline and handles  up  to  three  commands  chained  together  with  the  pipeline  operator.
* redirection.h

    Executed using child process. Checks for which I/O operator is being used and changes stdin or stdout to a file after testing that said file can be opened.

-----------

## Design Overview

The project is designed as follows:
One large function that reads the input, parses it into seperate words, then checks if the command refers to a built-in command. If so, it passes the command to the appriopiate function. If not, create a new process and run the code using execv() system call.

------------

## How to compile?
 
    make

## How to run?

./newshell [batchFile] 
