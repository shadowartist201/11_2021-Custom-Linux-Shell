#include <string.h>
#include <stdio.h>
#include "stdlib.h"
 
 //Bibek Napit

// A linked list node
struct AliasNode
{
    char command[500];
    char alias[500];
    struct AliasNode* next;
};

struct AliasNode* AliasNode;

void add_new_alias(struct AliasNode** head_ref, char* command, char* alias)
{
    struct AliasNode* new_node = (struct AliasNode*)malloc(sizeof(struct AliasNode));
    strcpy(new_node->command, command);
    strcpy(new_node->alias, alias);

    new_node->next = (*head_ref);
    (*head_ref) = new_node;
}

/* Given a reference (pointer to pointer) to the head of a list
   and a key, deletes the first occurrence of key in linked list */
void remove_alias(struct AliasNode** head_ref, char* alias)
{
    // Store head node
    struct AliasNode* temp = *head_ref, * prev = NULL;

    // If head node itself holds the key to be deleted
    if (temp != NULL && strcmp(temp->alias, alias) == 0)
    {
        *head_ref = temp->next; // Changed head
        free(temp);             // free old head
        return;
    }

    // Search for the key to be deleted, keep track of the
    // previous node as we need to change 'prev->next'
    while (temp != NULL && strcmp(temp->alias, alias) != 0)
    {
        prev = temp;
        temp = temp->next;
    }

    // If key was not present in linked list
    if (temp == NULL)
        return;

    // Unlink the node from linked list
    prev->next = temp->next;

    free(temp); // Free memory
}

void print_alias(struct AliasNode* node)
{
    while (node != NULL)
    {
        printf("alias %s=\'%s\'\n", node->alias, node->command);
        node = node->next;
    }
}
void has_alias(char* line)
{
    struct AliasNode* node = AliasNode;
    while (node != NULL)
    {

        if (strcmp(node->alias, line) == 0)
        {
            strcpy(line, node->command);
            return;
        }
        node = node->next;
    }
}

int is_valid_alias_add(char* line, char* alias, char* command)
{
    char holder[500];
    memset(holder, 0, sizeof(holder));
    int idx = 0, len = strlen(line);
    int eq_ix = -1;
    for (int i = 5; i < len; i++)
    {
        if (line[i] == '=')
        {
            eq_ix = i;
            break;
        }
        if (line[i] != ' ')
        {
            holder[idx] = line[i];
            idx++;
        }
    }
    if (eq_ix == -1)
        return 0;

    strcpy(alias, holder);
    // clear the str again
    memset(holder, 0, sizeof(holder));
    idx = 0;
    int flag = -1;
    for (int i = eq_ix + 2; i < len; i++)
    {
        if (line[i] == '\'')
        {
            flag = 0;
            break;
        }
        holder[idx] = line[i];
        idx++;
    }
    strcpy(command, holder);
    if (flag != -1)
        return 1;
    return 0;
}

void run_alias_command(char* line)
{
    char  alias[500], command[500];
    if (strcmp("alias", line) == 0)
    {
        print_alias(AliasNode);
    }
    else if (strcmp("alias -c", line) == 0)
    {

        // delete the alias list
        AliasNode = NULL;
        printf("Alias List is cleared\n");
    }
    else if (strncmp(line, "alias -r", 8) == 0)
    {
        char tmpline[300];
        strcpy(tmpline, line + 9);
        struct AliasNode* node = AliasNode;
        while (node != NULL)
        {

            if (strcmp(node->alias, tmpline) == 0)
            {
                remove_alias(&AliasNode, tmpline);
                printf("alias of \'%s\' is deleted\n", tmpline);
                return;
            }
            node = node->next;
        }
        printf("Alias is not found\n");
    }
    else if (is_valid_alias_add(line, alias, command))
    {
        add_new_alias(&AliasNode, command, alias);
    }
    else
    {
        fprintf(stderr, "Error using the alias command\n");
    }
}
