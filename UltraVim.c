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
void getConsoleSize();
void displayBuffer(const char *buffer, const char *filename);
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

// DISPLAY BUFFER
void displayBuffer(const char *buffer, const char *filename)
{
    // Clear the console and display the version of UltraVim and the file name
    printf("UltraVim 0.1 - %s\n\n", filename);

    // Display the file contents
    printf("%s\n", buffer);
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
        system("cls");
        displayBuffer(buffer, filename);

        // Show all instruction to the user
        printf("\n'esc' = NORMAL MODE\n");

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
                system("cls");
                printf("\nFile saved successfully.\n");
            }
            else
            {
                printf("\nError saving file.\n");
            }
            break;
        }
        // Arrow keys handling
        else if (ch == 0 || ch == 224)
        {
            // Get the actual key code
            ch = _getch();
            switch (ch)
            {
                case 72: // Up arrow
                    if (cursorPos.Y > 0) cursorPos.Y--;
                    break;
                case 80: // Down arrow
                    cursorPos.Y++;
                    break;
                case 75: // Left arrow
                    if (cursorPos.X > 0) cursorPos.X--;
                    break;
                case 77: // Right arrow
                    cursorPos.X++;
                    break;
            }
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
                buffer[cursor] = '\0';
            }
        }
    }
}
