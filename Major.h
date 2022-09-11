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

///-----------------------
#include "Pipeline.h"
#include "History.h"
#include "AliasHandler.h"
#include "Path.h"
#include"redirection.h"
#include "MySignals.h"
//------------------

#define MAXW 100
#define BLOCKSIZE 1024
char tmp_line[512];
char custom_prompt[50];
void executeCommand(char *commands[]);
void set_signals_to_default_in_child();
// read a line from the given stream by realloc
char *readDynamicLine(FILE *fp)
{
    char *line = NULL;
    size_t maxlength = 0;
    if (fp == NULL)
    {
        return NULL;
    }
    for (;;)
    {
        char *crlf, *block;

        maxlength += BLOCKSIZE;

        if (NULL == (line = realloc(line, maxlength + 1)))
        {
            break;
        }
        block = line + maxlength - BLOCKSIZE;
        // BLOCKSIZE+1
        if (NULL == fgets(block, BLOCKSIZE + 1, fp))
        {

            if (block == line)
            {

                free(line);
                line = NULL;
            }
            break;
        }

        if (NULL != (crlf = strchr(block, '\n')))
        {
            *crlf = 0x0;
            if (crlf != block)
            {
                if ('\r' == *(--crlf))
                    *crlf = 0x0;
            }
            break;
        }
    }
    return line;
}

void tokenize_user_input(char line[], char *commands[])
{

    int i = 0;
    commands[i] = strtok(line, " \r\n");
    if (commands[i] == NULL)
        return;
    while (commands[i] != NULL)
    {
        i++;
        commands[i] = strtok(NULL, " \r\n");
    }
}

void splitAndExecute(char *line)
{
    char line_code[1024];
    char *commands[MAXW];
    int exit_found = 0;
    char *ptr_exit = strstr(line, "exit");
    memset(commands, 0, sizeof commands); // clear commands
    if (strlen(line))
        add_command_to_history(line);
    has_alias(line);

    // add_to_history(line);
    if (ptr_exit != NULL)
    {
        exit_found = 1;
    }
    int idx = 0, len = strlen(line);
    for (int i = 0; i < len; i++)
    {
        line_code[idx] = line[i];
        if (i == len - 1)
        {

            line_code[idx + 1] = 0;
            if (line_code[idx] == ';')
                line_code[idx] = 0;

            if (strlen(line_code) > 0)
            {

                if (strncmp(line_code, "alias", 5) == 0)
                {
                    run_alias_command(line_code);
                    return;
                }
                tokenize_user_input(line_code, commands);
                // printf("command 1:%s...\n" ,line_code );
                executeCommand(commands);
                memset(commands, 0, sizeof commands); ///clear commands
            }
            continue;
        }

        if (line[i] == ';' || line[i] == '\n')
        {
            line_code[idx] = 0;
            if (idx > 0)
            {
                if (strstr(line_code, "exit") == NULL)
                {
                    puts(line_code);
                    tokenize_user_input(line_code, commands);
                    // check for alias commands

                    executeCommand(commands);
                }
            }

            idx = -1;
        }

        idx++;
    }

    if (exit_found)
    {
        exit(1);
    }
}

int cd_command_handler(char *commands[])
{
    if (strcmp(commands[0], "cd") == 0)
    {
        if (commands[1] == NULL)
        {
            char home[] = "/home";
            commands[1] = (char *)malloc(sizeof(char) * (strlen(home) + 2));
            strcpy(commands[1], home);
            commands[2] = NULL;
        }
        if (chdir(commands[1]) != 0)
        {
            perror("");
        }
        return 1;
    }
    return 0;
}

void child_task(char *commands[])
{
    // set_signals_to_default_in_child();
    // chiled process
    // check if the user asks for redirection
    handle_redirection(commands);
    // check for alias
	if (strcmp(commands[0], "myhistory") == 0) {
		run_history_command(commands, tmp_line);
		exit(0);
	}
	
    char *path = NULL;
    path =find_command_path(commands);
    if (path == NULL)
    {
        fprintf(stderr, "The command %s does not exist or could not be executed.\n"  , commands[0]);
        exit(0);
    }
    if (execvp(path, commands) == -1)
    {
        // exit with 1
        perror(">");
        exit(1);
    }
    exit(0);
}

void executeCommand(char *commands[])
{
    pid_t pid; // to save the process ID
    int status;
	
    if (commands[0] == NULL)
        return;

    if (strcmp(commands[0], "myhistory") == 0)
    {
		if (commands[1] != NULL) {
		    if (strcmp(commands[1], ">") == 0 || strcmp(commands[1], "<") == 0) {
			    goto yeet;
		    }
		    if (strcmp(commands[1], "|") == 0) {
			    goto yeet;
		    }
		}
		else {
			if (run_history_command(commands, tmp_line))
			{
            		// re execute the given command
            		splitAndExecute(tmp_line);
			}
			return;
		}

    }
    // BONUS
    // check for the prompt command
    if (strcmp(commands[0], "prompt") == 0)
    {
        if (commands[1] != NULL)
        {
            strcpy(custom_prompt, commands[1]);
            puts("Prompt updated");
        }
        else
        {
            puts("Prompt usage: prompt <new prompt>");
        }
        return;
    }
    if (strcmp(commands[0], "exit") == 0)
        exit(0);
    if (cd_command_handler(commands))
    {
        return;
    }

    //
    if (strcmp(commands[0], "path") == 0)
    {
        run_path_commands(commands);
        return;
    }
    // check the pipeline
	
	yeet:
    if (pipe_check(commands))
    {
        handle_pipe(commands);
        return;
    }
    pid = fork();

    if (pid == 0)
    {
        child_task(commands);
    }
    else
    {
        tcsetpgrp(0, getpid());

        // Parent process
        wait(NULL);
    }
    return;
}

void ignore_signals_in_parent();

void shell(int argc, const char **argv)
{
    FILE *file;
    char *line;
    // ignore_signals_in_parent();
    path_entries = read_PATH_env();
    strcpy(custom_prompt, ">");
    if (argc == 2)
    {
        file = fopen(argv[1], "r");
        if (file == NULL)
        {
            puts("ERROR:File Not Found");
            exit(1); // end the code with return message 1
        }

        //  main loop of the shell
        while (!feof(file))
        {
            line = readDynamicLine(file);
            splitAndExecute(line);
        }
    }

    if (argc < 1) // checking for file
    {
        fprintf(stderr, "Usage <file name or nothing \n");
        exit(1);
    }

    while (!feof(stdin))
    {
        ignore_signals_in_parent();
        printf("\033[0;32m");

        printf("%s ", custom_prompt);
        printf("\033[0m");
        line = readDynamicLine(stdin);
        if (line == 0)
            exit(0);
        splitAndExecute(line);
    }
}
