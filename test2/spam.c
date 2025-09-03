#include <stdio.h>
#include <Windows.h>

#define EXIT '\x1A'

HANDLE wHndIn;
DWORD fdwSaveOldMode;

VOID KeyEventProc(KEY_EVENT_RECORD ker);
VOID ErrorExit(LPCSTR lpszMessage);

static size_t pos;
static CHAR buffer[1000];
static int running;


int main(VOID)
{
    DWORD cNumRead, fwMode, i;
    INPUT_RECORD irInBuf[128];

    wHndIn = GetStdHandle(STD_INPUT_HANDLE);
    if ( wHndIn == INVALID_HANDLE_VALUE)
       ErrorExit("GetStdHandle");

    if (!GetConsoleMode(wHndIn, &fdwSaveOldMode))
        ErrorExit("GetConsoleMode");

    fwMode = (ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS) & ~ENABLE_QUICK_EDIT_MODE;
    if (! SetConsoleMode(wHndIn, fwMode))
        ErrorExit("SetConsoleMode");
    running = 1;
    printf("~");
    while (running)
    {
        if (! ReadConsoleInput(wHndIn, irInBuf, 128, &cNumRead))
            ErrorExit("ReadConsoleInput");
        
        for (i = 0; i < cNumRead; i++)
        {
            switch (irInBuf[i].EventType)
            {
            case KEY_EVENT:
                KeyEventProc(irInBuf[i].Event.KeyEvent);
                break;

            case MOUSE_EVENT: // mouse input
                    break;

            case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing
                break;

            case FOCUS_EVENT:  // disregard focus events

            case MENU_EVENT:   // disregard menu events
                break;
            
            default:
                ErrorExit("Unknown event type");
                break;
            }
        }
    }
    SetConsoleMode(wHndIn, fdwSaveOldMode);
}

VOID KeyEventProc(KEY_EVENT_RECORD ker)
{
    if(ker.bKeyDown)
    {
        char key_pressed = ker.uChar.AsciiChar;

        switch (key_pressed)
        {
            case '\r':
            {
                printf("\n~");
                pos = 0;
            }break;

            case '\b':
            {
                if (pos == 0)
                    break;
                printf("\b \b");
                pos--;
            }break;

            case EXIT:
            {
                running = 0;
            }break;

            default:
            {
                printf("%c",ker.uChar.AsciiChar);
                buffer[pos++] = ker.uChar.AsciiChar;
                buffer[pos] = '\0';
            }break;
        }
    }
}

VOID ErrorExit(LPCSTR lpszMessage)
{
    fprintf(stderr, "%s\n", lpszMessage);

    SetConsoleMode(wHndIn, fdwSaveOldMode);

    ExitProcess(0);

}