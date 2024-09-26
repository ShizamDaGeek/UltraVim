// INCLUDES
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>

// DEFINES
#define CTRL_KEY(k) ((k) & 0x1f)

// DATA
#ifdef _WIN32
    DWORD orig_console_mode;
#else
    struct termios orig_termios;
#endif

// Functions
void enableRawMode();
void disableRawMode();
void die(const char *s);
char editorReadKey();
void editorProcessKeypress();

// INIT
int main(int argc, char** argv) 
{
    // Calling enableRawMode
    enableRawMode();

    char c;
    #ifdef _WIN32
        DWORD bytesRead;
        HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);
        while (1) 
        {
            editorProcessKeypress();
            char c = '\0';
            if (!ReadFile(hConsole, &c, 1, &bytesRead, NULL) || bytesRead == 0) {die("ReadFile");}
            if (iscntrl(c)) 
            {
                printf("%d\r\n", c);
            }
            else 
            {
                printf("%d ('%c')\r\n", c, c);
            }
            if (c == CTRL_KEY('q')) break;
        }
    #else
        while (1) 
        {
            char c = '\0';
            if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read");
            if (iscntrl(c)) 
            {
                printf("%d\r\n", c);
            }
            else 
            {
                printf("%d ('%c')\r\n", c, c);
            }
            if (c == CTRL_KEY('q')) break;
        }
    #endif
    
    return 0;
}

// ENABLE RAW MODE FUNCTION
void enableRawMode()
{
    #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);
        
        GetConsoleMode(hConsole, &orig_console_mode);
        SetConsoleMode(hConsole, orig_console_mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT | ENABLE_WINDOW_INPUT));
        
        atexit(disableRawMode);
    #else
        if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
        atexit(disableRawMode);
        
        struct termios raw = orig_termios;
        raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
        raw.c_oflag &= ~(OPOST);
        raw.c_cflag |= (CS8);
        raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 1;
        
        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
    #endif
}

// DISABLE RAW MODE FUNCTION
void disableRawMode()
{
    #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);
        SetConsoleMode(hConsole, orig_console_mode);
    #else
        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
            die("tcsetattr");
    #endif
}

// DIE FUNCTION
void die(const char *s) 
{
    perror(s);
    exit(1);
}

// EDITOR READ KEY FUNCTION
char editorReadKey() 
{
    #ifdef _WIN32
        DWORD bytesRead;
        HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);
        
        char c;
        
        SetConsoleMode(hConsole, ENABLE_WINDOW_INPUT | ENABLE_ECHO_INPUT);

        boolean isRunning = TRUE;
        while (isRunning)
        {
            if (!ReadFile(hConsole, &c, 1, &bytesRead, NULL)) 
            {
                die("ReadFile");
            }
            if (bytesRead == 1) 
            {
                return c;
            }
        }
    #else
        int nread;
        char c;
        while ((nread = read(STDIN_FILENO, &c, 1)) != 1) 
        {
            if (nread == -1 && errno != EAGAIN) die("read");
        }
        return c;
    #endif
}

// EDITOR PROCESS KEYPRESS FUNCTION
void editorProcessKeypress() 
{
    char c = editorReadKey();
    switch (c) 
    {
        case CTRL_KEY('q'):
        exit(0);
        break;
    }
}