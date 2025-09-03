#include "../include/dirent.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

static void list_directory(const char *dirname);


int main(int argc, char *argv[])
{
    int i = 1;
    while (i < argc)
    {
        i++;
    }

    if (argc == 1)
        list_directory(".");

    return EXIT_SUCCESS;
}

static void list_directory(const char *dirname)
{
    DIR *dir = opendir(dirname);
    if(!dir)
    {
        printf("Could not open");
        exit(EXIT_FAILURE);
    }

    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL)
    {
        switch (ent->d_type)
        {
        case DT_REG:
            printf("%s\n", ent->d_name);
            break;
        
        case DT_DIR:
            break;
        
        case DT_LNK:
            printf("%s@\n", ent->d_name);
            break;

        default:
            printf("%s*\n", ent->d_name);
            break;
        }
    }

    closedir(dir); 
}