#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>
#include <conio.h>
#include "commands.h"

#define KEY_TAB '\t'
#define KEY_ENTER '\r'
#define KEY_BACK_SPACE '\b'

#define global_internal static
#define internal static


typedef void (*cmd_fn)(void);

typedef struct {
    char* name;
    cmd_fn fn;
}commands;

internal void shell_execute(flow_struct * st);
internal void take_command(flow_struct st);
internal void buffer_add_char(flow_struct* st, char c);

global_internal commands buff_commands[] = { 
    {"lt" , command_print_serie}, 
    {"pm", command_print_message},
    {"pr", command_print_arg}, 
    {"rm", command_remove},
    {"pwd",command_print_currentWorkingDir},
    {"cd", command_change_dir},
    {"mkdir", command_make_dir},
    {"rmdir", command_remove_dir},
    {"help", command_help},
    {"dir",command_listFiles},
    {"time", command_print_date},
    {"jk", command_joke},
    {"clear", command_clear},
    {NULL, NULL}};

int main(void)
{
    flow_struct line = {NULL, 0};
    shell_execute(&line);    
    return 0;
}

internal void shell_execute(flow_struct* st)
{
    char* possible_aut;
    char c;
    printf("?");

    while (1)
    {
        if (_kbhit())
        {
            c = _getch();
            if (st->pos == 0 && (c == KEY_BACK_SPACE || c == KEY_ENTER))
                continue;
            
            switch (c)
            {
            case  KEY_TAB:
            
                possible_aut = (strrchr(st->buff, ' '));
                possible_aut = (possible_aut == NULL) ? st->buff : possible_aut + 1;
                macro_autocomplete(possible_aut, st);
                break;

            case KEY_ENTER:
            
                st->pos = 0;
                printf("\n");
                if (strcmp(st->buff,"ex") == 0)
                    return;

                take_command(*st);
                printf("?");
                break;

            case KEY_BACK_SPACE:
            
                if (st->pos <= 0)
                    break;
                
                st->pos--;
                (st->buff)[st->pos] = '\0';
                printf("\b \b");
                break;
                
            default:
            
                buffer_add_char(st, c);
                printf("%c",c);
                break;  
            }
        }     
    }
}

internal void buffer_add_char(flow_struct* st, char c)
{
    char* tmp = realloc(st->buff, st->pos + 2);

    if(!tmp)
    {
        free(st->buff);
        free(tmp);
        st->buff = NULL;
    }
    st->buff = tmp;
    (st->buff)[st->pos++] = c;
    (st->buff)[st->pos] = '\0';
}

internal void take_command(flow_struct st)
{
    int i;
    char* token = strtok(st.buff, " ");
    do
    {
        for (i = 0;buff_commands[i].name != NULL && (strcmp(token, buff_commands[i].name)) != 0; i++)
            ;
        if (buff_commands[i].name == NULL)
            printf("Command %s is not valid\n", token);
        else
            buff_commands[i].fn();     
    } while ((token = strtok(NULL, " ")));
}