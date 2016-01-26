/*++

Copyright (c) 2013 by Antoni Sawicki

Module Name:

    cmdmax.c

Abstract:

    Maximize cmd.exe Window to match the screen size
    Allow setting arbitrary window position and size parameters

Version:

    2.0 - added parameters for user specified position / size / buffer size
    1.0 - initial release

Author:

    Antoni Sawicki <as@tenoware.com>

License:
   
    Public Domain

--*/


#include <windows.h>
#include <stdio.h>
#include <stdlib.h>


#pragma comment(lib, "user32.lib")


HWND (__stdcall *CmdGetConsoleWindow)(void);

HWND
MyGetConsoleWindow(
    void
    )
/*++

Routine Description:

    This is a makeshift version of GetConsoleWindow() function substituted
    for Windows < 2000 which do not have the OS supplied version.
    Lack of the Windows routine is detected and substituted at runtime rather than compile time.

--*/
{
    char winname[128];
    HWND cmdwin=NULL;
    int n=0;

    _snprintf(winname, sizeof(winname), "CMDMAX:%d", GetCurrentProcessId());

    SetConsoleTitle(winname);

    do {
        n++;
        Sleep(100);
        cmdwin=FindWindow(NULL, winname);
    } while (!cmdwin && n<600);

    return cmdwin;
}


int
main(
    int argc, 
    char **argv
    )
/*++

Routine Description:

    This routine is the main program for CmdMax.

    The arguments accepted by cmdmax are:

        pos_x               Window Position -> Left in pixels, or 'n' will prevent moving
        pos_y               Window Position -> Top in pixels, or 'n' will prevent moving
        win_w               Window Size -> Width in characters
        win_h               Window Size -> Height in characters
        buf_w               Screen Buffer Size -> Width in characters, if smaller than win_w will be set to win_w
        buf_h               Screen Buffer Size -> Height in characters, if smaller than win_h will be set to win_h

    No arguments will maximize the window.

--*/
{
    HANDLE hConsole;
    COORD conbuf, conmax;
    SMALL_RECT conpos, winpos;
    int winmov=1;

    ZeroMemory(&conbuf, sizeof(COORD));
    ZeroMemory(&conmax, sizeof(COORD));
    ZeroMemory(&conpos, sizeof(SMALL_RECT));
    ZeroMemory(&winpos, sizeof(SMALL_RECT));

    CmdGetConsoleWindow = (void *) GetProcAddress(GetModuleHandle("kernel32.dll"), "GetConsoleWindow" );

    if(!CmdGetConsoleWindow) 
        CmdGetConsoleWindow = (void *) MyGetConsoleWindow;

    hConsole=GetStdHandle(STD_OUTPUT_HANDLE);

    if(argc==1) {
    
        conmax=GetLargestConsoleWindowSize(hConsole);

        winpos.Left=0;
        winpos.Top=0;
        conpos.Left=0;
        conpos.Top=0;
        conpos.Right=conmax.X-5;
        conpos.Bottom=conmax.Y-5;
        conbuf.X=conpos.Right+1;
        conbuf.Y=3001;

    } else if(argc==7) {

        winpos.Left=strtol(argv[1], NULL, 10);
        winpos.Top=strtol(argv[2], NULL, 10);
        conpos.Left=0;
        conpos.Top=0;
        conpos.Right=strtol(argv[3], NULL, 10);
        conpos.Bottom=strtol(argv[4], NULL, 10);
        conbuf.X=strtol(argv[5], NULL, 10)+1;
        conbuf.Y=strtol(argv[6], NULL, 10)+1;

        if(conbuf.X < conpos.Right) conbuf.X=conpos.Right+1;
        if(conbuf.Y < conpos.Bottom) conbuf.Y=conpos.Bottom+1;

        if(argv[1][0]=='n' && argv[2][0]=='n')
            winmov=0;

    } else {

        printf("Usage: cmdmax [<pos_x> <pos_y> <win_w> <win_h> <buf_w> <buf_h>]\n"
               "       pos_x, pos_y are in pixels\n"
               "       win_w, win_h, buf_w, buf_h are in characters\n"
               "       buf_w, buf_h must be greater or equal to win_w, win_h\n"
               "       if both pos_x, pos_y are set to letter 'n' won't move the window\n"
               "       if no parameters specified, a maximum possible size will be used\n\n"
               "Examples:\n"
               "       cmdmax                       (will maximize the window)\n"
               "       cmdmax 100 100  80 24  80 24            (no scrollbars)\n"
               "       cmdmax n n  80 24  80 5000  (add buffer without moving)\n\n");
        ExitProcess(0);
    
    }

    if(getenv("CMDMAX_DEBUG"))
        printf("%d %d  %d %d  %d %d\n", winpos.Left, winpos.Top, conpos.Right, conpos.Bottom, conbuf.X-1, conbuf.Y-1);

    if(winmov)
        SetWindowPos(CmdGetConsoleWindow(), HWND_TOP, winpos.Left, winpos.Top, 0, 0, SWP_NOSIZE);

    SetConsoleScreenBufferSize(hConsole, conbuf);
    SetConsoleWindowInfo(hConsole, TRUE, &conpos);
    SetConsoleScreenBufferSize(hConsole, conbuf);
    SetConsoleWindowInfo(hConsole, TRUE, &conpos);


    return 0;
}

