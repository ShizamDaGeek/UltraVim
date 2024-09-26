// If window then windows.h, if not windows then not windows
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

#include <iostream>
#include <stdlib.h>

struct termios orig_termios;

void enableRawMode();
void disableRawMode();

int main(int argc, char** argv) 
{
    // Calling enableRawMode
    enableRawMode();

    char c;
    #ifdef _WIN32
        while (ReadFile(GetStdHandle(STD_INPUT_HANDLE), &c, 1, (LPDWORD)&c, NULL) && c != 'q');
    #else
        while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
    #endif
    
    return 0;
}

// ENABLE RAW MODE
void enableRawMode()
{
    #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);
        DWORD mode;
        GetConsoleMode(hConsole, &mode);
        SetConsoleMode(hConsole, mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));
    #else
        tcgetattr(STDIN_FILENO, &orig_termios);
        atexit(disableRawMode);
        
        struct termios raw = orig_termios;
        raw.c_lflag &= ~(ECHO | ICANON);
        
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    #endif
}

// DISABLE RAW MODE
void disableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}