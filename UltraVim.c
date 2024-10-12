#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <conio.h>

#define MAX_BUFFER 1024

/*
    *----------------------------------------------------*
    |                     Hello World!                   |
    |This is a computer software is made by ShizamDa_Geek|
    .----------------------------------------------------.
*/

// Function declarations
int loadFile(const char *filename, char *buffer);
int saveFile(const char *filename, char *buffer);
void getConsoleSize();
void hideCursor();
void showCursor();
void displayBuffer(char *buffer);
void normalMode(char *buffer);
void insertMode(char *buffer);

int main(int argc, char *argv[]) 
{
    // Buffer for file contents
    char buffer[MAX_BUFFER] = {0};

    // Check if a filename is provided
    if (argc < 2) 
    {
        printf("Usage: uvim <filename>\n");
        return 1;
    }

    // Load the file into the buffer
    if (loadFile(argv[1], buffer) != 0)
    {
        return 1;
    }

    normalMode(buffer);

    return 0;
}

// LOAD FILE
int loadFile(const char *filename, char *buffer) 
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) 
    {
        printf("Error opening file!\n");
        return 1;
    }
    fread(buffer, sizeof(char), MAX_BUFFER, file);
    fclose(file);
    return 0;
}

// SAVE FILE
int saveFile(const char *filename, char *buffer) 
{
    FILE *file = fopen(filename, "w");
    if (file == NULL) 
    {
        printf("Error saving file!\n");
        return 1;
    }
    fwrite(buffer, sizeof(char), strlen(buffer), file);
    fclose(file);
    return 0;
}

// GET CONSOLE SIZE
void getConsoleSize()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;
  
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
  
    printf("columns: %d\n", columns);
    printf("rows: %d\n", rows);
}

// HIDE CURSOR
void hideCursor()
{
    // Hides cursor
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;  
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

// SHOW CURSOR
void showCursor()
{
    // Shows cursor
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

// DISPLAY BUFFER
void displayBuffer(char *buffer)
{
    // Clear the console and display the file contents
    system("cls");
    printf("--- File Contents ---\n%s\n", buffer);
}

// NORMAL MODE
void normalMode(char *buffer)
{
    // Hiding the cursor and display the file contents
    hideCursor();
    displayBuffer(buffer);

    // Loop to keep checking for key presses
    boolean isRunning = TRUE;
    while (isRunning) 
    {
        // If user pressed 'i' the, will be put in insert mode
        printf("--- Press 'i' to enter insert mode ---\n");
        
        char editModeInput = _getch(    );
        scanf(" %c", &editModeInput);
        if (editModeInput == 'i')
        {
            // Takes user to insert mode and shows cursor again
            showCursor();
            insertMode(buffer);
            break;
        }
        else
        {
            printf("Please enter 'i' to go to insert mode\n");
        }
    }
}

// EDIT LOOP
void insertMode(char *buffer)
{
    system("cls");

    displayBuffer(buffer);

}
