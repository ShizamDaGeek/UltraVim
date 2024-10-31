#include "includes/libs.h"

/*
    *----------------------------------------------------*
    |                     Hello World!                   |
    |This is a computer software is made by ShizamDa_Geek|
    .----------------------------------------------------.
*/

// Function declarations
int loadFile(const char *filename, char *buffer);
int saveFile(const char *filename, char *buffer);
void clearConsole();
void displayBuffer(const char *buffer, const char *filename);
void printAtBottom(HANDLE console, const char *message);
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

    insertMode(buffer, argv[1]);

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

// CLEAR CONSOLE
void clearConsole()
{
    #ifdef _WIN32
        system("cls");
    #else
        clearScreen();
    #endif
}

// DISPLAY BUFFER
void displayBuffer(const char *buffer, const char *filename)
{
    // Clear the console and display the version of UltraVim and the file name
    printf("UltraVim 0.1 - %s\n\n", filename);

    // Display the file contents
    printf("%s\n", buffer);
}

// PRINT AT BOTTOM OF TERMINAL
void printAtBottom(HANDLE console, const char *message)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(console, &csbi);
    
    // Calculate the position at the bottom
    COORD bottomPos;
    bottomPos.X = 0;
    bottomPos.Y = csbi.srWindow.Bottom - 1; // Last row of the window

    // Move the cursor to the bottom position
    SetConsoleCursorPosition(console, bottomPos);

    // Print the message
    printf("%s", message);
}

// EDIT LOOP
void insertMode(char *buffer, const char *filename)
{
    // Init cursor
    int cursor = strlen(buffer);
    COORD cursorPos = {0, 0};
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    boolean isEditing = TRUE;
    while (isEditing)
    {
        // clear screen, display file contents and show cursor
        clearConsole();
        displayBuffer(buffer, filename);

        // Print initial instruction at the bottom
        printAtBottom(console, "esc = SAVE&QUIT | arrow-keys = MOVE CURSOR");

        // Calculate the new cursor position
        int row = 0, col = 0;
        for (int i = 0; i < cursor; ++i)
        {
            if (buffer[i] == '\n')
            {
                row++;
                col = 0; // Reset column after a newline
            }
            else
            {
                col++;
            }
        }
        // Add an offset to the row for the instructions printed below the buffer
        row += 2;
        cursorPos.X = col;
        cursorPos.Y = row;

        // Move the cursor to the current cursorPos
        SetConsoleCursorPosition(console, cursorPos);

        // Capture user input
        char ch = _getch();

        // If 'ESC' key to return to normal mode
        if (ch == 27)
        {
            // Save the file contents before exiting insert mode
            if (saveFile(filename, buffer) == 0)
            {
                clearConsole();
            }
            else
            {
                clearConsole();
                printf("\nERROR WHEN SAVING THE FILE!\n");
            }
            break;
        }
        // 'Backspace' key
        else if (ch == '\b')
        {
            if (cursor > 0)
            {
                // Shift buffer content left to remove the character
                memmove(buffer + cursor - 1, buffer + cursor, strlen(buffer) - cursor + 1);
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
                buffer[cursor] = '\0';
            }
        }
        else
        {
            // Add the input character to the buffer
            if (cursor < MAX_BUFFER - 1)
            {
                // Shift buffer content right to make space for the new character
                memmove(buffer + cursor + 1, buffer + cursor, strlen(buffer) - cursor + 1);
                buffer[cursor] = ch;
                cursor++;
            }
        }
    }
}