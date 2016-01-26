/*++

Copyright (c) 2013 by Antoni Sawicki

Module Name:

    cmdmax.c

Abstract:

    Maximize cmd.exe Window to match the screen size

Author:

    Antoni Sawicki <as@tenoware.com>

License:
   
    Public Domain

--*/


#include <windows.h>
#include <stdio.h>

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

    There are no arguments although further improvement could be made
    to supply user defined window size and position. If you require such
    functionality please email the author.

--*/
{
    HANDLE hConsole;
    COORD buff, winmax;
    SMALL_RECT winpos;


    CmdGetConsoleWindow = (void *) GetProcAddress(GetModuleHandle("kernel32.dll"), "GetConsoleWindow" );

    if(!CmdGetConsoleWindow) 
        CmdGetConsoleWindow = (void *) MyGetConsoleWindow;

    
    hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    winmax=GetLargestConsoleWindowSize(hConsole);

    winpos.Left=0;
    winpos.Top=0;
    winpos.Right=winmax.X-5;
    winpos.Bottom=winmax.Y-5;
    buff.X=winpos.Right+1;
    buff.Y=3000;
     
    SetConsoleScreenBufferSize(hConsole, buff);
    SetConsoleWindowInfo(hConsole, TRUE, &winpos);
    SetWindowPos(CmdGetConsoleWindow(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE);

    return 0;
}

