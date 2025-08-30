#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"

#define global_internal static

typedef void (*cmd_fn)(void);

typedef struct {
    char* name;
    cmd_fn fn;
}commands;

flow_struct line;

void read(flow_struct * st);
void take_com(flow_struct st);
int find_command(char* token);

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
    
    return 0;
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
{"ex", command_exit} ,{"pr", command_print_arg}, {"rm", command_remove},{"pwd",command_wd},{"cd", command_change_dir},
{"mkdir", command_make_dir},{"rmdir", command_remove_dir},{"help", command_help},{"dir",command_listFiles},{"time", command_print_date},
{"jk", command_joke},{NULL, NULL}};

    for (int i = 0; command_list[i].name != NULL ;i++)
    {
        if (strcmp(token, command_list[i].name) == SUCCESS)
        {
            command_list[i].fn();
            return 1;
        }
    }

    printf("Command %s is not valid\n", token);
    return 0;
}