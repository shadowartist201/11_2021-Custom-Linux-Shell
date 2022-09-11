#include <string.h>
#include <stdio.h>
#include "stdlib.h"
//Bibek Napit

extern void child_task(char *commands[]);

void pipeline(char ***cmd)
{
	int fd[2];
	pid_t pid;
	int fdd = 0;				
	while (*cmd != NULL) {
		pipe(fd);				
    
		if ((pid = fork()) == -1) {
			perror("fork");
			exit(1);
		}
		else if (pid == 0) {

			dup2(fdd, 0);
			if (*(cmd + 1) != NULL) {
				dup2(fd[1], 1);
			}
			close(fd[0]);
            child_task( (*cmd) );
			exit(1);
		}
		else {
			wait(NULL); 		
			close(fd[1]);
			fdd = fd[0];
			cmd++;
		}
	}
}

int pipe_check(char ** commands){
    int cnt = -1;
    for (int i = 0; commands[i] != NULL ; i++)
    {
        if(strstr(commands[i], "|") != NULL)
        {
            return 1;
        }
      
    }
    return 0;
}


int get_number_of_pipes(char ** commands){
    int cnt = 0;
    for (int i = 0; commands[i] != NULL ; i++)
    {
        if(strstr(commands[i], "|") !=  NULL)
        {
            cnt++;
        }
      
    }
    return cnt;
}

void handle_pipe(char ** commands){
    int pipes_count = get_number_of_pipes(commands);
    int len = 0;
    for ( len = 0; commands[len] != NULL; len++);
    
    
    char *** cmd =  (  char ***) malloc(sizeof(char**) * (pipes_count + 1));
    if(cmd == NULL)
    {
        fprintf(stderr , "out of space could not malloc... \nExiting\n");
        exit(0);
    }
    cmd[0] = commands;
    int n = 1;
    for (int i = 1; i < len; i++)
    {
        if(commands[i][0] == '|')
        {
            // this is a cutting command
            cmd[n] = commands+ i+1;
            commands[i] = NULL;
        }
    }
    pipeline(cmd);

}
