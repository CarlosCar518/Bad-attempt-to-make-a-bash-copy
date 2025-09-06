#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>
#include "../include/commands.h"

#define KEY_TAB '\t'
#define KEY_ENTER '\r'
#define KEY_BACK_SPACE '\b'
#define EXIT '\x1A'
#define ESC 0x1B
#define global_internal static
#define internal static

typedef void (*cmd_fn)(void);

typedef struct {
    char* name;
    cmd_fn fn;
}commands;

global_internal HANDLE wHndIn;
global_internal HANDLE wHndOut;
global_internal DWORD fdwSaveOldMode;
global_internal int running;

internal void shell_execute(flow_struct * st);
internal void take_command(flow_struct st);
internal void buffer_add_char(flow_struct* st, char c);
internal void Key_event_proc(KEY_EVENT_RECORD ker, flow_struct *line);
internal void error_exit(char* error);
internal void Key_event_arrow(WORD key,flow_struct *line);
internal void key_event(char key, flow_struct *line);
internal void init_console();


/*
* `cat <archivo>` → mostrar contenido de un archivo.
* `grep <palabra> <archivo>` → buscar palabras en un archivo.
* `history` → mostrar los últimos comandos ejecutados.
* `exec <programa>` → ejecutar un programa externo (tipo Bash).
*/

global_internal commands buff_commands[] = { 
    {"lt" , command_print_serie}, 
    {"pm", command_print_message},
    {"echo", command_print_arg}, 
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
    flow_struct line = {NULL, 0, 0};
    shell_execute(&line);    
    return 0;
}

internal void shell_execute(flow_struct* st)
{ 
    DWORD cNumRead, i;
    INPUT_RECORD irInBuf[128];

    init_console();

    running = 1;
    printf("~");
    while (running)
    {
        if (! ReadConsoleInput(wHndIn, irInBuf, 128, &cNumRead))
        {
            error_exit("ReadConsoleInput");
        }

        for (i = 0; i < cNumRead; i++)
        {
            switch (irInBuf[i].EventType)
            {
            case KEY_EVENT:
                Key_event_proc(irInBuf[i].Event.KeyEvent, st);
                break;

            default:
                break;
            }
        }
    }  
}

internal void Key_event_proc(KEY_EVENT_RECORD ker, flow_struct *line)
{
    if(ker.bKeyDown)
    {
        char key_pressed = ker.uChar.AsciiChar;

        if (ker.wVirtualKeyCode == VK_LEFT || ker.wVirtualKeyCode == VK_DOWN ||
            ker.wVirtualKeyCode == VK_UP   || ker.wVirtualKeyCode == VK_RIGHT)
        {
            Key_event_arrow(ker.wVirtualKeyCode, line);
            return;
        }
        
        key_event(key_pressed, line);

    }
}

internal void key_event(char key, flow_struct *line)
{
    switch (key)
    {
        case KEY_ENTER:
        {
            if (line->pos == 0)
            {
                break;
            }
            printf("\n");
            take_command(*line);
            printf("\n~");
            line->pos = 0;
            line->buff[0] = '\0';
        }break;

        case KEY_BACK_SPACE:
        {
            if (line->cursor == 0)
            {
                break;
            }
            printf("\b\x1b[1P");
            (line->pos)--;
            (line->cursor)--;
        }break;

        case KEY_TAB:
        {
            char* possible_aut = (strrchr(line->buff, ' '));
            possible_aut = (possible_aut == NULL) ? line->buff : possible_aut + 1;

            dir_complete(line);
        }break;

        case EXIT:
        {
            running = 0;
        }break;

        default:
        {
            printf("%c",key);               
            buffer_add_char(line, key);
            (line->cursor)++;
        }break;
        
    }
}

internal void Key_event_arrow(WORD key,flow_struct *line)
{
    switch (key)
    {
    case VK_LEFT:
        if (line->cursor <= 0)
            break;
        printf("\x1b[1D");
        (line->cursor)--;
        break;
    case VK_RIGHT:
        if (line->cursor >= line->pos)
            break;
        printf("\x1b[1C");
        (line->cursor)++;
        break;
    default:
        break;
    }
}



internal void buffer_add_char(flow_struct* st, char c)
{
    char* tmp = realloc(st->buff, st->pos + 2);

    if(!tmp)
    {
        free(st->buff);
        st->buff = NULL;
        return;
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

internal void error_exit(char* error)
{
    fprintf(stderr, "%s\n", error);

    SetConsoleMode(wHndIn, fdwSaveOldMode);

    ExitProcess(0);
}

internal void init_console()
{
    DWORD fwMode;

    wHndIn = GetStdHandle(STD_INPUT_HANDLE);
    if (wHndIn == INVALID_HANDLE_VALUE)
    {
        error_exit("GetStdHandle");
    }
    
    wHndOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if (wHndIn == INVALID_HANDLE_VALUE)
    {
        error_exit("GetStdHandle");
    }

    if(!GetConsoleMode(wHndIn, &fdwSaveOldMode))
    {
        error_exit("GetConsoleMode");
    }

    fwMode = (ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS) & ~ENABLE_QUICK_EDIT_MODE;
    if (!SetConsoleMode(wHndIn, fwMode))
    {
        error_exit("SetConsoleMode");
    }

    fwMode = ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    if (!SetConsoleMode(wHndOut,fwMode))
    {
        error_exit("SetConsoleMode");
    }
}