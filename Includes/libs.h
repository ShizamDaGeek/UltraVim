// C Includes
#ifdef _WIN32
#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#endif

// Define
#define MAX_BUFFER 1024