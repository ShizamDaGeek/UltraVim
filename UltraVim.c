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
void normalMode(char *buffer, const char *filename);
void insertMode(char *buffer, const char *filename);

int main(int argc, char *argv[]) 
{
    // Buffer for file contents
    char buffer[MAX_BUFFER] = {0};

    // Check if a filename is provided
    if (argc < 2) 
    {
        printf("Usage: UVim <filename>\n");
        return 1;
    }

    // Load the file into the buffer
    if (loadFile(argv[1], buffer) != 0)
    {
        return 1;
    }

    normalMode(buffer, argv[1]);

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
void normalMode(char *buffer, const char *filename)
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
        // If user pressed 'q' the program will quit
        printf("--- Press 'q' to enter insert mode ---\n");
        
        char editModeInput;
        scanf(" %c", &editModeInput);
        if (editModeInput == 'i')
        {
            // Takes user to insert mode and shows cursor again
            showCursor();
            insertMode(buffer, filename);
            break;
        }
        if (editModeInput == 'q')
        {
            system("cls");
            break;
        }
    }
}

// EDIT LOOP
void insertMode(char *buffer, const char *filename)
{
    // Start editing from the end of the buffer
    int cursor = strlen(buffer);
    int ch;

    boolean isEditing = TRUE;
    while (isEditing)
    {
        // clear screen, display file contents and show cursor
        system("cls");
        displayBuffer(buffer);
        showCursor();

        // Show instructions to go back to insert mode
        printf("\n--- INSERT MODE --- Press ESC to return to normal mode ---\n");

        // Capture user input
        char ch = _getch();

        // If 'ESC' key to return to normal mode
        if (ch == 27)
        {
             // Save the file contents before exiting insert mode
            if (saveFile(filename, buffer) == 0)
            {
                printf("\nFile saved successfully.\n");
            }
            else
            {
                printf("\nError saving file.\n");
            }

            // Hide cursor and exit to insert mode
            hideCursor();
            break;
        }
        // 'Backspace' key
        else if (ch == '\b')
        {
            if (cursor > 0)
            {
                buffer[cursor - 1] = '\0';  // Remove last character
                cursor--;
            }
        }
        // 'Enter' key
        else if (ch == '\r')
        {
            if (cursor < MAX_BUFFER - 1)
            {
                // Adds newline
                buffer[cursor] = '\n';
                cursor++;
            }
        }
        else
        {
            // Add the input character to the buffer
            if (cursor < MAX_BUFFER - 1)
            {
                buffer[cursor] = ch;
                cursor++;
                buffer[cursor] = '\0';  // Ensure null-termination of the string
            }
        }
    }
    
    // Return to normal mode after done editing
    normalMode(buffer, filename);
}
