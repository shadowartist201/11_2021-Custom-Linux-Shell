#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#define MAX_PATH_LEN 512
char **path_entries;


char **read_PATH_env()
{
    char **dirs;
    char *pathEnvVar;
    char *token;

    pathEnvVar = (char *)getenv("PATH");

    dirs = malloc(strlen(pathEnvVar) * sizeof(char *));

    
    token = strtok(pathEnvVar, ":");

    int counter = 0;

    while (token != NULL)
    {
        dirs[counter] = token;
        counter++;
        token = strtok(NULL, ":");
    }

    
    dirs[counter] = NULL;

    return dirs;
}

int get_arr_len(char *commands[])
{
    int len = 0;
    for (len = 0; commands[len] != NULL; len++)
		;
    return len;
}

int is_complete_path(char c)
{
    return c == '/';
}

int is_executable(char *comand)
{
    return is_complete_path(comand[1]) && comand[0] == '.';
}


char *find_command_path(char **commands)
{
    char *temp_path_name = (char *)malloc(MAX_PATH_LEN);
    int count = 0;

    
    if (is_complete_path(commands[0][0]))
    {
        strcpy(temp_path_name, commands[0]);
        return temp_path_name;
    }

    if (is_executable(commands[0]))
    {
        strcpy(temp_path_name, commands[0]);
        return temp_path_name;
    }

    for (count = 0; path_entries[count] != NULL; count++)
    {
        
        strcpy(temp_path_name, path_entries[count]);
        strcat(temp_path_name, "/");
        strcat(temp_path_name, commands[0]);
        
        if (access(temp_path_name, R_OK) == 0)
            return temp_path_name;

        
        memset(temp_path_name, 0, MAX_PATH_LEN);
    }

    
    return NULL;
}

void add_path(char *new_path)
{

    int vec_len = 0;
    for (vec_len = 0; path_entries[vec_len] != NULL; vec_len++)
        ;

    path_entries[vec_len] = (char *)malloc(sizeof(char) * MAX_PATH_LEN);
    strcpy(path_entries[vec_len], new_path);
    path_entries[vec_len + 1] = NULL;
}

int remove_path(char *path)
{
    int idx = -1, vec_len = 0;

    for (vec_len = 0; path_entries[vec_len] != NULL; vec_len++)
    {
    }

    for (int i = 0; path_entries[i] != NULL; i++)
    {

        if (strcmp(path, path_entries[i]) == 0)
        {
            idx = i;
            break;
        }
    }
    if (idx <= 0)
    {
        return 0;
    }

    for (int i = idx; i < vec_len - 1; i++)
    {
        strcpy(path_entries[i], path_entries[i + 1]);
    }
    path_entries[vec_len - 1] = NULL;
}

void print_path()
{
    int vec_len = 0;

    for (int i = 0; path_entries[i] != NULL; i++)
    {
        printf("%s:", path_entries[i]);
    }
    printf("\b\b\n");
}

void run_path_commands(char *commands[])
{

    int len = get_arr_len(commands);

    if (len == 1)
    {
        print_path();
    }
    else if (len == 3)
    {
        if (commands[1][0] == '+')
        {

            add_path(commands[2]);
        }
        else if (commands[1][0] == '-')
        {
            remove_path(commands[2]);
        }
        
    }
}
