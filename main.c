#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "commands.h"

#define STOP 0
#define RUNNING 1
#define SUCCESS 0

#define global_internal static


typedef void (*cmd_fn)(void);

typedef struct {
    char* name;
    cmd_fn fn;
}commands;

typedef struct {
    char* buff;
    int state;
}flow_struct;


void read(flow_struct * st);
void take_com(flow_struct st);
void command_print_serie();
void command_print_message();
int find_command(char* token);
void command_exit();
void command_print_arg();
void command_remove();

global_internal flow_struct line;


int main(void)
{
    char *buffer;
    do
    {
        printf("?");
        read(&line);
        if (line.state == STOP)
        {
            return 0;
        }
        take_com(line);
        if (line.state == STOP)
            break;
        free(line.buff);
        
    } while (1); 
}

void read(flow_struct* st)
{
    st->buff = NULL;
    char c;
    size_t size = 0;

    while ((c = getchar()) != '\n' && c != EOF)
    {
        char *tmp = realloc(st->buff, size + 2);

        if (tmp == NULL)
        {
            free(st->buff);
            st->buff = NULL;
            st->state = STOP;
            return;
        }

        st->buff = tmp;
        (st->buff)[size++] = c;
        (st->buff)[size] = '\0';
    }

    st->state = c == EOF ? STOP : RUNNING; 
}

void take_com(flow_struct st)
{
    char* token = strtok(st.buff, " ");
    int flag;
    do
    {
        flag = find_command(token);
    } while ((token = strtok(NULL, " ")) && flag);
}

int find_command(char* token)
{
    commands command_list[] = { {"lt" , command_print_serie}, {"pm", command_print_message},
{"exit", command_exit} ,{"pr", command_print_arg}, {"rm", command_remove},{NULL, NULL}};

    for (int i = 0; command_list[i].name != NULL ;i++)
    {
        if (strcmp(token, command_list[i].name) == SUCCESS)
        {
            command_list[i].fn();
            return true;
        }
    }

    printf("Command %s is not valid\n", token);
    return false;
}

void command_print_serie()
{
    for(int i = 1; i <= 100; i++)
        printf("%d\n", i);
    
}

void command_print_message()
{
    printf("Hoy comí perro\n");
}

void command_exit()
{
    line.state = STOP;
}

void command_print_arg()
{
    char* arg = strtok(NULL, " ");

    printf("The user said %s\n", arg);
}

void command_remove()
{
    char* arg = strtok(NULL, " ");

    if (!arg)
    {
        printf("Error: no file specified\n");
        return;
    }

    int result = remove(arg);

    if (result == SUCCESS)
    {
        printf("File not found");
    }
}