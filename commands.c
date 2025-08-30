#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <direct.h>
#include "commands.h"

void command_print_serie()
{
    for(int i = 1; i <= 100; i++)
        printf("%d\n", i);
}

void command_print_message()
{
    printf("Hoy comi perro\n");
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

    if (result != SUCCESS)
    {
        printf("File not found");
    }
}

void command_wd()
{
    char* pwd = _getcwd(NULL, 0);

    if (pwd != NULL)
    {
        printf("%s\n",pwd);
        free(pwd);
        return;
    }

    printf("Error");
}

void command_change_dir()
{
    char* token = strtok(NULL, " ");

    if(_chdir(token))
    {
        printf("Path not found\n"); 
    }
}

void command_make_dir()
{
    char* token = strtok(NULL, " ");

    if (_mkdir(token))
    {
        printf("Error\n");
    }
}

void command_remove_dir()
{
    char* token = strtok(NULL, " ");

    if (rmdir(token))
    {
        printf("Error\n");
    }
}

void command_help()
{
    printf("        Commands list:\n");
    printf("lt -- prints the numbers from 1 to 100 (dont ask me why)\npm -- prints a nice message\nexit -- exits the shell\npr -- prints the next argument\n");

    printf("rm -- deletes the file with the name of the argument\npwd -- prints the current working directory\ncd -- changes the current working directory to the argument\n");
    printf("mkdir -- creates a new directory\nrmdir -- deletes the directory\n");
}