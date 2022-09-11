#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//Kelsey Porter

//checks if the user needs redirection or not

int is_redirected(char **commands) {
    for (int i = 0; commands[i] != NULL; i++) {
        if (strstr(commands[i], ">") != NULL) {
            return 1;
        }
        if (strstr(commands[i], "<") != NULL) {
            return 2;
        }
    }
    return 0;
}

// this function redirect the input and the output stream

void handle_redirection(char *commands[]) {
    int fileD, state;
    state = is_redirected(commands);
    if (state == 1) {
        int idx = -1;
        for (int i = 0; commands[i] != NULL; i++) {
            if (strcmp(commands[i], ">") == 0) {
                idx = i;
                break;
            }
        }
        if ((fileD = open(commands[idx + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644)) < 0) {
            perror("open error");
            return;
        }
        // redirect the stdout to fileD
        dup2(fileD, 1);
        close(fileD);
        commands[idx] = NULL;
    }
    else if (state == 2) {
        int idx = -1;
        for (int i = 0; commands[i] != NULL; i++) {
            if (strcmp(commands[i], "<") == 0) {
                idx = i;
                break;
            }
        }
        if ((fileD = open(commands[idx + 1], O_RDONLY, 0644)) < 0) {
            perror("open error");
            return;
        }
        // redirect the stdin to fileD
        dup2(fileD, 0);
        close(fileD);
        commands[idx] = NULL;
    }
}