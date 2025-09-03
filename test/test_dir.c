#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <locale.h>
#include <time.h>
#include <sys/stat.h>



static int _main(int argc, char* argv[])
{

}


static void list_directory(const char* dirname)
{
    char path[PATH_MAX];

    DIR *dir = opendir(dirname);

    
}