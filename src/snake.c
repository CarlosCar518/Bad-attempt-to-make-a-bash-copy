#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/commands.h"

typedef struct 
{
    int x,y;
}Point;

typedef struct 
{
    Point *body;
    int lenght;
    int dx,dy;
}Snake;


static HANDLE wHndIn;
static HANDLE wHndOut;



void _Smain()
{
    Snake sn = {NULL,0, 20, 20};
    DWORD cNumRead, i;
    INPUT_RECORD irInBuf[128];

    printf("\x1b[2J\x1b[?25l");
    wHndIn = GetStdHandle(STD_INPUT_HANDLE);
    wHndOut = GetStdHandle(STD_OUTPUT_HANDLE);
    int running = 1;
    while (running)
    {
        if (! ReadConsoleInput(wHndIn, irInBuf, 128, &cNumRead))
        {
            return;
        }

        for (i = 0; i < cNumRead; i++)
        {
            switch (irInBuf[i].EventType)
            {
            case KEY_EVENT:
                char key = irInBuf[i].Event.KeyEvent.uChar.AsciiChar;

                if (key == 'w')
                {
                    printf("\x1b[2J\x1b[%i;%iH*",sn.dy,sn.dx);
                    sn.dy--;
                }
                else if (key == 'a')
                {
                    printf("\x1b[2J\x1b[%i;%iH*",sn.dy,sn.dx);
                    sn.dx--;                    
                }
                else if( key == 's')
                {
                    printf("\x1b[2J\x1b[%i;%iH*",sn.dy,sn.dx);
                    sn.dy++;
                }
                else if ( key == 'd')
                {
                    printf("\x1b[2J\x1b[%i;%iH*",sn.dy,sn.dx);
                    sn.dx++;
                }
                
                if ( key == 'p')
                    running = 0;

            default:
                break;
            }
        }
    }  

    
}