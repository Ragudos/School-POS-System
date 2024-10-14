#ifndef KEYBOARD

#define KEYBOARD

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_S 83
#define KEY_s 115
#define KEY_A 65
#define KEY_a 97
#define KEY_B 66
#define KEY_C 67
#define KEY_c 99
#define KEY_D 68
#define KEY_Q 81
#define KEY_q 113
#define KEY_PLUS 43
#define KEY_HYPHEN_MINUS 45
#define KEY_ESC 27
#define KEY_BACKSPACE 8
#define KEY_OPENING_BRACKET 91

#ifdef LINUX_PLATFORM

#include <iostream>
#include <termios.h>
#include <unistd.h>

using namespace std;

int getPressedKeyCode() {
    struct termios oldt, newt;

    int ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}

#elif defined(MAC_PLATFORM)

#include <iostream>

using namespace std;

int getPressedKeyCode() { throw logic_error("unimplemented"); }

#elif defined(WINDOWS_PLATFORM)

#include <conio.h>
#include <windows.h>

#include <iostream>

int getPressedKeyCode() {
    if (_kbhit()) {
        return _getch();
    }

    return -1;
}

#else

#error "Unsupported Platform!"

#endif

#endif
